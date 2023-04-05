#include "header.h"

int main() {
    pthread_t philosopher_threads[NUM_PHILOSOPHERS];
    int i;

    // Initialize the chopstick mutexes and philosopher states
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&chopstick_mutex[i], NULL);
        philosopher_states[i] = 0;
    }

    // Create the philosopher threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosopher_threads[i], NULL, philosopher, (void *)&i);
    }

    // Wait for the philosopher threads to finish
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosopher_threads[i], NULL);
    }

    // Clean up the chopstick mutexes
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&chopstick_mutex[i]);
    }

    return 0;
}

void *philosopher(void *arg) {
    int philosopher_num = *(int *)arg;
    while (1) {
        // Philosopher is thinking
        printf("Philosopher %d is thinking\n", philosopher_num);
        // Wait for a random amount of time
        sleep(rand() % MAX_WAIT_TIME);

        // Philosopher is hungry and wants to eat
        printf("Philosopher %d is hungry and wants to eat\n", philosopher_num);
        // Request the chopsticks
        release_chopstick(philosopher_num);
        
        // Philosopher has the chopsticks and is eating
        printf("Philosopher %d has the chopsticks and is eating\n", philosopher_num);
        // Wait for a random amount of time
        sleep(rand() % MAX_WAIT_TIME);
        
        // Philosopher is done eating and releases the chopsticks
        printf("Philosopher %d is done eating and releases the chopsticks\n", philosopher_num);
        // Release the chopsticks
        release_chopstick(philosopher_num);
    }
}
