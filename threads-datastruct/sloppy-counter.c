#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUMCPUS 16 // Max number of CPUs
int globalTID = 0;
__thread long thread_id;

typedef struct __counter_t {
    int global;
    pthread_mutex_t glock;
    int local[NUMCPUS];
    pthread_mutex_t llock[NUMCPUS];
    int threshold;
} counter_t;

void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    for (int i = 0; i < NUMCPUS; i++) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

void update(counter_t *c, int threadID, int amt) {
    int cpu = threadID % NUMCPUS;
    pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold) {
        pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    pthread_mutex_unlock(&c->llock[cpu]);
}

int get(counter_t *c) {
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val;
}

void *thread_function(void *arg) {
    thread_id = __sync_fetch_and_add(&globalTID, 1);
    counter_t *c = (counter_t *)arg;
    //printf("Thread %ld started\n", thread_id%NUMCPUS);
    for (int i = 0; i < 10000000; i++) { // Beispielhafte Update-Operationen
        update(c, thread_id, 1);
    }
    return NULL;
}

void run_threads(int n) {
    counter_t counter;
    init(&counter, 10000000); // Beispielhafter Threshold

    pthread_t threads[n];
    struct timeval start, end;

    gettimeofday(&start, NULL);
    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, thread_function, &counter);
    }
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
    printf("Number of threads: %d, Time taken: %ld microseconds, ", n, micros);
    printf("Counter: %d\n", get(&counter));
}

int main() {
    int thread_counts[] = {1, 2, 4, 8, 16};
    for (int i = 0; i < 5; i++) {
        run_threads(thread_counts[i]);
    }
    return 0;
}
