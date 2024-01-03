#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_INCREMENTS 20000000

// Shared counter
int counter = 0;
pthread_mutex_t lock;

// Thread function to increment the counter
void* incrementCounter(void* arg) {
    for (int i = 0; i < NUM_INCREMENTS; ++i) {
        pthread_mutex_lock(&lock);
        ++counter;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    const int numberOfThreadsList[] = {1, 2, 4, 8, 16}; // Adjust based on your system capabilities
    int numTests = sizeof(numberOfThreadsList) / sizeof(numberOfThreadsList[0]);

    pthread_mutex_init(&lock, NULL);

    for (int test = 0; test < numTests; ++test) {
        int numThreads = numberOfThreadsList[test];
        pthread_t threads[numThreads];
        struct timeval start, end;

        gettimeofday(&start, NULL);

        // Create threads
        for (int i = 0; i < numThreads; ++i) {
            if (pthread_create(&threads[i], NULL, incrementCounter, NULL)) {
                fprintf(stderr, "Error creating thread\n");
                return 1;
            }
        }

        // Wait for threads to finish
        for (int i = 0; i < numThreads; ++i) {
            if (pthread_join(threads[i], NULL)) {
                fprintf(stderr, "Error joining thread\n");
                return 1;
            }
        }

        gettimeofday(&end, NULL);

        long seconds = end.tv_sec - start.tv_sec;
        long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

        printf("Number of threads: %d, Time taken: %ld microseconds, Counter: %d\n", numThreads, micros, counter);

        // Reset counter for the next test
        counter = 0;
    }

    pthread_mutex_destroy(&lock);

    return 0;
}
