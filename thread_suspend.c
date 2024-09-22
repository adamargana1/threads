#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

volatile int counter = 0;
volatile int suspended = 0;
pthread_mutex_t lock;

void* increment_counter(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        if (!suspended) {
            counter++;
            printf("Counter: %d\n", counter);
        }
        pthread_mutex_unlock(&lock);
        sleep(1); // Sleep for 1 second
    }
    return NULL;
}

void* toggle_suspend(void* arg) {
    char input;
    while (1) {
        printf("Enter 's' to suspend, 'r' to resume: ");
        scanf(" %c", &input); 
        pthread_mutex_lock(&lock);
        if (input == 's') {
            suspended = 1;
            printf("Counter suspended.\n");
        } else if (input == 'r') {
            suspended = 0;
            printf("Counter resumed.\n");
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t counter_thread, suspend_thread;

    // Initialize mutex
    pthread_mutex_init(&lock, NULL);

    // Create threads
    pthread_create(&counter_thread, NULL, increment_counter, NULL);
    pthread_create(&suspend_thread, NULL, toggle_suspend, NULL);

    // Wait for threads to finish 
    pthread_join(counter_thread, NULL);
    pthread_join(suspend_thread, NULL);

    // Cleanup mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
