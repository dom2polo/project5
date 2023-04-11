#include "philosophers.h"

// states that a philosopher can be in
enum
{
    THINKING,
    HUNGRY,
    EATING
} state[NUMPHIL];

// mutex and condition variables
pthread_mutex_t mutex;
pthread_cond_t cond_vars[NUMPHIL];

// declaring arrays to store chopsticks and philosopher IDs
int chopsticks[NUMPHIL];
int philosopher_ids[NUMPHIL];

// declare a buffer for passing tokens between philosophers and variables
// tracks the head and tail of the buffer
int token_buffer[NUMPHIL];
int token_head = 0;
int token_tail = 0;

// declare an array of sockets for communication between philosophers
int sockets[NUMPHIL];

// function to initialize mutex, condition variables, and philosopher IDs
void init()
{
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < NUMPHIL; i++)
    {
        pthread_cond_init(&cond_vars[i], NULL);
        philosopher_ids[i] = i;
    }
    for (int i = 0; i < NUMPHIL; i++)
    {
        token_buffer[i] = i;
    }
}

void *philosopher(void *arg)
{
    int id = *(int *)arg;
    int left_chopstick = id;
    int right_chopstick = (id + 1) % NUMPHIL;
    while (1)
    {
        printf("Philosopher %d is thinking.\n", id);
        sleep(rand() % 5); // thinking
        pthread_mutex_lock(&mutex);
        state[id] = HUNGRY;
        printf("Philosopher %d is hungry and wants to eat.\n", id);
        while (chopsticks[left_chopstick] || chopsticks[right_chopstick] || token_buffer[token_tail] != id)
        {
            pthread_cond_wait(&cond_vars[id], &mutex);
        }
        chopsticks[left_chopstick] = 1;
        chopsticks[right_chopstick] = 1;
        state[id] = EATING;
        printf("Philosopher %d is eating.\n", id);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 5); // eating
        pthread_mutex_lock(&mutex);
        chopsticks[left_chopstick] = 0;
        chopsticks[right_chopstick] = 0;
        state[id] = THINKING;
        printf("Philosopher %d has finished eating and is thinking.\n", id);
        token_tail = (token_tail + 1) % (NUMPHIL + 1);
        pthread_cond_signal(&cond_vars[token_tail]);
        pthread_mutex_unlock(&mutex);
    }
    close(sockets[id]);
    return NULL;
}

void *token_ring(void *arg)
{
    int id = *(int *)arg;
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERIP, &server_addr.sin_addr);

    // create socket
    sockets[id] = socket(AF_INET, SOCK_STREAM, 0);
    if (sockets[id] < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // connect to server
    if (connect(sockets[id], (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    int next_philosopher_id = (id + 1) % NUMPHIL; // get ID of the next philosopher
    while (1)
    {
        // receive token
        int token_received;
        recv(sockets[id], &token_received, sizeof(token_received), 0);

        // pass token
        if (token_received == philosopher_ids[id])
        {
            printf("Philosopher %d received token.\n", id);
            fflush(stdout);
            token_buffer[token_head] = philosopher_ids[id];
            token_head = (token_head + 1) % NUMPHIL;
            pthread_cond_signal(&cond_vars[philosopher_ids[id]]);
        }

        // send token
        if (state[next_philosopher_id] != EATING)
        {
            printf("Philosopher %d is sending token to philosopher %d.\n", id, next_philosopher_id);
            fflush(stdout);
            send(sockets[next_philosopher_id], &philosopher_ids[id], sizeof(philosopher_ids[id]), 0);

            // wait for philosopher to finish eating before sending token again
            pthread_mutex_lock(&mutex);
            while (state[next_philosopher_id] == EATING)
            {
                pthread_cond_wait(&cond_vars[next_philosopher_id], &mutex);
            }
            pthread_mutex_unlock(&mutex);
        }
        next_philosopher_id = (next_philosopher_id + 1) % NUMPHIL; // update ID of the next philosopher
    }
    close(sockets[id]);
    return NULL;
}

// main function
int main()
{
    init();
    pthread_t philosophers[NUMPHIL];
    pthread_t token_ring_thread;
    for (int i = 0; i < NUMPHIL; i++)
    {
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }
    pthread_create(&token_ring_thread, NULL, token_ring, &philosopher_ids[0]);
    for (int i = 0; i < NUMPHIL; i++)
    {
        pthread_join(philosophers[i], NULL);
    }
    return 0;
}
