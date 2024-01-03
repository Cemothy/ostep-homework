#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

typedef struct __barrier_t {
    sem_t s1, lock;
    int threads;
    int num_threads;
    // add semaphores and other information here
} barrier_t;


// the single barrier we are using for this program
barrier_t b;

void barrier_init(barrier_t *b, int num_threads) {
    sem_init(&b->s1, 0, 0);
    sem_init(&b->lock, 0, 1);
    b->num_threads = num_threads;
    b->threads = 0;
}

void barrier(barrier_t *b) {
    sem_wait(&b->lock); // lock for value threads
    b->threads++;
    if (b->threads >= b->num_threads) {
        sem_post(&b->s1);   // the last thread arriving will post, and all threads will continue
    }
    sem_post(&b->lock); // unlock for value threads
    sem_wait(&b->s1); // all threads wait for post here until the last one has arrived
    sem_post(&b->s1); // will signal the next thread to continue
    // barrier code goes here
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}


// run with a single argument indicating the number of 
// threads you wish to create (1 or more)
int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);
    
    int i;
    for (i = 0; i < num_threads; i++) {
	t[i].thread_id = i;
	Pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++) 
	Pthread_join(p[i], NULL);

    printf("parent: end\n");
    return 0;
}

