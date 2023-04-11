// This file would typically contain any function prototypes, data 
// structures, or constants that are shared across multiple source 
// files in the implementation. For example, it might define a Philosopher 
// struct that contains information about each philosopher, or a Chopstick
// struct that represents a single chopstick.

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>

#define PORT 8080 // coordinator's port number
#define MAX_BUFFER_SIZE 256
#define MAX_PHILOSOPHERS 5
#define NUM_CHOPSTICKS 5

// Struct to represent a chopstick
typedef struct {
    int id;                      // The ID of the chopstick
    pthread_mutex_t mutex;       // The mutex used to protect the chopstick
} Chopstick;

// Struct to represent a philosopher
typedef struct Philosopher {
    int num_meals; 
    int num_eaten; 
    pthread_mutex_t mutex; 
    int sockfd;
    int min_think_time; 
    int max_think_time; 
    int min_eat_time; 
    int max_eat_time; 
    int id;                      // The ID of the philosopher
    Chopstick *left_chopstick;
    Chopstick *right_chopstick;
    int left_chopstick_id;       // The ID of the left chopstick
    int right_chopstick_id;      // The ID of the right chopstick
    pthread_t thread;            // The thread representing the philosopher
} Philosopher;

// // Shared memory structure for storing the state of each chopstick
// struct ChopstickState {
//     bool is_taken;
//     pthread_mutex_t mutex;
// };

// // Shared memory structure for storing the state of the dining philosophers problem
// struct DiningState {
//     int num_chopsticks;
//     int num_philosophers;
//     struct Philosopher Philosophers[MAX_PHILOSOPHERS];
//     struct ChopstickState Chopsticks[NUM_CHOPSTICKS];
// };

// Function prototypes
void error(const char *msg);
void *coordinator_thread_func(void *arg);
void *philosopher_thread_func(void *arg);
void take_chopsticks(int left_chopstick_id, int right_chopstick_id);
int request_chopsticks(int left_chopstick_id, int right_chopstick_id, int sockfd);
void release_chopsticks(int left_chopstick_id, int right_chopstick_id, int sockfd);

#endif
