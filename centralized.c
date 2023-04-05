#include "header.h"

pthread_mutex_t chopstick_mutex[NUM_PHILOSOPHERS];
int philosopher_states[NUM_PHILOSOPHERS];

void think(int philosopher_id) {
    printf("Philosopher %d is thinking.\n", philosopher_id);
    sleep(1);
}

void eat(int philosopher_id) {
    printf("Philosopher %d is eating.\n", philosopher_id);
    sleep(1);
}

void release_chopstick(int philosopher_id) {
    int left = philosopher_id;
    int right = (philosopher_id + 1) % NUM_PHILOSOPHERS;
    pthread_mutex_unlock(&chopstick_mutex[left]);
    printf("Philosopher %d released chopstick %d\n", philosopher_id, left);
    pthread_mutex_unlock(&chopstick_mutex[right]);
    printf("Philosopher %d released chopstick %d\n", philosopher_id, right);
}

void pickup_chopstick(int philosopher_id) {
    int left = philosopher_id;
    int right = (philosopher_id + 1) % NUM_PHILOSOPHERS;
    pthread_mutex_lock(&chopstick_mutex[left]);
    printf("Philosopher %d picked up chopstick %d\n", philosopher_id, left);
    pthread_mutex_lock(&chopstick_mutex[right]);
    printf("Philosopher %d picked up chopstick %d\n", philosopher_id, right);
}


