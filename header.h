#ifndef HEADER_H
#define HEADER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_PHILOSOPHERS 5
#define MAX_WAIT_TIME 5

extern pthread_mutex_t chopstick_mutex[NUM_PHILOSOPHERS];
extern int philosopher_states[NUM_PHILOSOPHERS];

void *philosopher(void *id);
void pickup_chopstick(int id);
void release_chopstick(int id);
void think(int id);
void eat(int id);

#endif

