#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

typedef struct {
    int id;
    int has_token;
    int eat_count;
    int sock;
    struct sockaddr_in prev_philosopher;
    struct sockaddr_in next_philosopher;
} Philosopher;

void think(Philosopher *philosopher) {
    int think_time = rand() % 5 + 1;
    printf("Philosopher %d: Thinking for %d seconds\n", philosopher->id, think_time);
    sleep(think_time);
}

void eat(Philosopher *philosopher) {
    int eat_time = rand() % 5 + 1;
    printf("Philosopher %d: Eating for %d seconds\n", philosopher->id, eat_time);
    sleep(eat_time);
}

void *philosopher_behavior(void *arg) {
    Philosopher *philosopher = (Philosopher *)arg;
    char token_msg[8];

    while (philosopher->eat_count < (rand() % 8 + 3)) {
        think(philosopher);

        if (philosopher->has_token) {
            printf("Philosopher %d: Picked up forks\n", philosopher->id);
            eat(philosopher);
            printf("Philosopher %d: Put down forks\n", philosopher->id);

            philosopher->eat_count++;

            // Pass the token to the next philosopher
            strcpy(token_msg, "TOKEN");
            sendto(philosopher->sock, token_msg, sizeof(token_msg), 0, (struct sockaddr *)&(philosopher->next_philosopher), sizeof(philosopher->next_philosopher));
            printf("Philosopher %d: Token sent to Philosopher %d\n", philosopher->id, (philosopher->id + 1) % 5);
            philosopher->has_token = 0;
        } else {
            // Receive the token
            recvfrom(philosopher->sock, token_msg, sizeof(token_msg), 0, NULL, NULL);
            if (strcmp(token_msg, "TOKEN") == 0) {
                philosopher->has_token = 1;
            }
        }
    }



    printf("Philosopher %d has finished eating.\n", philosopher->id);
    return NULL;
}

void shuffle(pthread_t *array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        pthread_t temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int main() {
    srand(time(NULL));
    pthread_t philosophers[5];
    Philosopher philosopher_data[5];
    int port_base = 5000;
    int i;

    for (i = 0; i < 5; i++) {
        philosopher_data[i].id = i;
        philosopher_data[i].has_token = 0;
        philosopher_data[i].eat_count = 0;

        philosopher_data[i].sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (philosopher_data[i].sock < 0) {
            perror("Error creating socket");
            exit(1);
        }

        memset(&philosopher_data[i].prev_philosopher, 0, sizeof(philosopher_data[i].prev_philosopher));
        philosopher_data[i].prev_philosopher.sin_family = AF_INET;
        philosopher_data[i].prev_philosopher.sin_addr.s_addr = htonl(INADDR_ANY);
        philosopher_data[i].prev_philosopher.sin_port = htons(port_base + i);

                memset(&philosopher_data[i].next_philosopher, 0, sizeof(philosopher_data[i].next_philosopher));
        philosopher_data[i].next_philosopher.sin_family = AF_INET;
        philosopher_data[i].next_philosopher.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        philosopher_data[i].next_philosopher.sin_port = htons(port_base + ((i + 1) % 5));

        if (bind(philosopher_data[i].sock, (struct sockaddr *)&(philosopher_data[i].prev_philosopher), sizeof(philosopher_data[i].prev_philosopher)) < 0) {
            perror("Error binding socket");
            exit(1);
        }

        printf("Philosopher %d server connected.\n", i);
    }

    // Start philosopher threads
    shuffle(philosophers, 5);
    for (i = 0; i < 5; i++) {
        pthread_create(&philosophers[i], NULL, philosopher_behavior, &philosopher_data[i]);
    }

    // Send initial token to philosopher 0
    char initial_token_msg[8];
    strcpy(initial_token_msg, "TOKEN");
    sendto(philosopher_data[0].sock, initial_token_msg, sizeof(initial_token_msg), 0, (struct sockaddr *)&(philosopher_data[0].next_philosopher), sizeof(philosopher_data[0].next_philosopher));

    for (i = 0; i < 5; i++) {
        pthread_join(philosophers[i], NULL);
    }

    printf("All philosophers have finished eating.\n");

    return 0;
}

