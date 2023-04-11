// This file would typically contain the implementation of the 
// philosopher processes, which each represent an individual philosopher 
// in the Dining Philosopher problem. Each philosopher process would 
// likely be responsible for requesting and releasing chopsticks as 
// needed to eat their meals, and would communicate with the central 
// coordinator to coordinate their actions.

#include <netinet/in.h>
#include <arpa/inet.h>
#include "philosopher.h"

#define SERVER_IP "172.17.254.52"

int connect_to_coordinator(int port) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    // Create a sockaddr_in struct for the server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }

    return sockfd;
}

int request_chopsticks(int left_chopstick_id, int right_chopstick_id, int sockfd) {
    char buffer[MAX_BUFFER_SIZE];
    snprintf(buffer, MAX_BUFFER_SIZE, "REQUEST %d %d\n", left_chopstick_id, right_chopstick_id);
    if (write(sockfd, buffer, strlen(buffer)) < 0) {
        error("ERROR writing to socket");
    }

    memset(buffer, 0, MAX_BUFFER_SIZE);
    if (read(sockfd, buffer, MAX_BUFFER_SIZE - 1) < 0) {
        error("ERROR reading from socket");
    }

    if (strcmp(buffer, "GRANTED\n") == 0) {
        return 0;
    } else {
        return -1;
    }
}

void release_chopsticks(int left_chopstick_id, int right_chopstick_id, int sockfd) {
    char buffer[MAX_BUFFER_SIZE];
    snprintf(buffer, MAX_BUFFER_SIZE, "RELEASE %d %d\n", left_chopstick_id, right_chopstick_id);
    if (write(sockfd, buffer, strlen(buffer)) < 0) {
        error("ERROR writing to socket");
    }
}


// void *philosopher_thread_func(void *arg) {
//     Philosopher *philosopher = (Philosopher *) arg;
//     printf("Philosopher %d has started.\n", philosopher->id);

//     while (1) {
//         // Think for a random amount of time
//         int think_time = rand() % (philosopher->max_think_time - philosopher->min_think_time + 1) + philosopher->min_think_time;
//         printf("Philosopher %d is thinking for %d seconds.\n", philosopher->id, think_time);
//         sleep(think_time);

//         // Request chopsticks
//         printf("Philosopher %d is hungry and wants to eat.\n", philosopher->id);
//         int result = request_chopsticks(philosopher->id, philosopher->left_chopstick_id, philosopher->right_chopstick_id);
//         if (result == -1) {
//             printf("Philosopher %d couldn't get chopsticks and is continuing to think.\n", philosopher->id);
//             continue;
//         }

//         // Eat for a random amount of time
//         int eat_time = rand() % (philosopher->max_eat_time - philosopher->min_eat_time + 1) + philosopher->min_eat_time;
//         printf("Philosopher %d is eating for %d seconds.\n", philosopher->id, eat_time);
//         sleep(eat_time);

//         // Release chopsticks
//         release_chopsticks(philosopher->id, philosopher->left_chopstick_id, philosopher->right_chopstick_id);

//         printf("Philosopher %d has released chopsticks.\n", philosopher->id);
//     }
// }


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <id> <min think time> <max think time>\n", argv[0]);
        exit(1);
    }

    // Parse command-line arguments
    int id = atoi(argv[1]);
    int min_think_time = atoi(argv[2]);
    int max_think_time = atoi(argv[3]);

    // Initialize philosopher
    Philosopher philosopher;
    philosopher.id = id;
    philosopher.min_think_time = min_think_time;
    philosopher.max_think_time = max_think_time;

    // Connect to coordinator
    int sockfd = connect_to_coordinator(PORT);
    philosopher.sockfd = sockfd;

    // Start philosopher thread
    pthread_t thread;
    pthread_create(&thread, NULL, philosopher_thread_func, &philosopher);

    // Wait for philosopher thread to finish
    pthread_join(thread, NULL);

    return 0;
}
