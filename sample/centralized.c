// This file would typically contain the main program logic, which 
// sets up the initial state of the problem (e.g., creating the central 
// coordinator and philosopher processes), runs the simulation, and 
// prints any relevant output. It might also contain any necessary helper 
// functions or utility functions used throughout the implementation.

#include "philosopher.h"


int main(int argc, char* argv[]) {
    int num_philosophers = 5; // default number of philosophers
    int num_meals = 5; // default number of meals per philosopher

    // Parse command line arguments
    if (argc > 1) {
        num_philosophers = atoi(argv[1]);
    }
    if (argc > 2) {
        num_meals = atoi(argv[2]);
    }

    // Allocate memory for the philosophers and chopsticks
    Philosopher* philosophers = malloc(num_philosophers * sizeof(Philosopher));
    Chopstick* chopsticks = malloc(num_philosophers * sizeof(Chopstick));

    // Initialize the philosophers and chopsticks
    for (int i = 0; i < num_philosophers; i++) {
        philosophers[i].id = i;
        philosophers[i].num_meals = num_meals;
        philosophers[i].num_eaten = 0;
        philosophers[i].left_chopstick = &chopsticks[i];
        philosophers[i].right_chopstick = &chopsticks[(i+1) % num_philosophers];
        pthread_mutex_init(&philosophers[i].mutex, NULL);
    }
    for (int i = 0; i < num_philosophers; i++) {
        chopsticks[i].id = i;
        pthread_mutex_init(&chopsticks[i].mutex, NULL);
    }

    // Create the coordinator thread
    pthread_t coordinator_thread;
    pthread_create(&coordinator_thread, NULL, coordinator_thread_func, philosophers);

    // Create the philosopher threads
    pthread_t philosopher_threads[num_philosophers];
    for (int i = 0; i < num_philosophers; i++) {
        pthread_create(&philosopher_threads[i], NULL, philosopher_thread_func, &philosophers[i]);
    }

    // Wait for all philosopher threads to complete
    for (int i = 0; i < num_philosophers; i++) {
        pthread_join(philosopher_threads[i], NULL);
    }

    // Cleanup resources
    pthread_join(coordinator_thread, NULL);
    for (int i = 0; i < num_philosophers; i++) {
        pthread_mutex_destroy(&philosophers[i].mutex);
        pthread_mutex_destroy(&chopsticks[i].mutex);
    }
    free(philosophers);
    free(chopsticks);

    return 0;
}
