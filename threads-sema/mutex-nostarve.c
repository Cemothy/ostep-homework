#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct {
    sem_t mutex;
    sem_t queue;
    sem_t lock;
    int waiting;
    int ticket;
    int turn;
} ns_mutex_t;


void ns_mutex_init(ns_mutex_t *m) {
    sem_init(&m->mutex, 0, 1);
    sem_init(&m->lock, 0, 1);
    sem_init(&m->queue, 0, 0);
    m->ticket = 0;  
    m->turn = 0;
}

void ns_mutex_acquire(ns_mutex_t *m) {
    sem_wait(&m->lock);
    int ticket = m->ticket;
    m->ticket++;
    sem_post(&m->lock);
    while(ticket != m->turn) {
        sem_wait(&m->queue);
        sem_post(&m->queue);
    }
    sem_wait(&m->mutex);

}

void ns_mutex_release(ns_mutex_t *m) {
    sem_wait(&m->lock);
    m->turn++;
    sem_post(&m->lock);
    sem_post(&m->queue);
    sem_post(&m->mutex);
    
}

ns_mutex_t m;

void *worker(void *arg) {
    int number = *((int *) arg);
    ns_mutex_acquire(&m);
    printf("child %d: begin\n", number);
    printf("child %d: end\n", number);
    ns_mutex_release(&m);
    return NULL;
}

int main(int argc, char *argv[]) {
    ns_mutex_init(&m);
    assert(argc == 2);
    int num_readers = atoi(argv[1]);

    pthread_t pr [num_readers];

    printf("parent: begin\n");
    int i;
    for (i = 0; i < num_readers; i++)
	Pthread_create(&pr[i], NULL, worker, &i);
    for (i = 0; i < num_readers; i++)
	Pthread_join(pr[i], NULL);
    printf("parent: end\n");
    return 0;
}

