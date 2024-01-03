#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

sem_t s; 

void *child(void *arg) {
    printf("child\n");
    sem_post(&s); //increment semaphore
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    sem_init(&s, 0, 0); //init semaphore to 0
    Pthread_create(&p, NULL, child, NULL);
    sem_wait(&s); //will wait if child has not posted (since semaphore is 0), if child has posted, will decrement semaphore and continue
    printf("parent: end\n");
    return 0;
}

