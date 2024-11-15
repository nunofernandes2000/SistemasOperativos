/*
 * counter.c - synchronized counter with even and odd threads
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h> // for sleep

int Counter = 0; // variable to be incremented
pthread_mutex_t Mutex; // mutex

void* thr_inc_even(void* ptr)
{
    while (Counter < 20) {
        pthread_mutex_lock(&Mutex); // lock
        if (Counter < 20 && Counter % 2 == 0) {
            Counter++;
            printf("[E]counter = %d\n", Counter);
        }
        pthread_mutex_unlock(&Mutex); // unlock
    }
    return NULL;
}

void* thr_inc_odd(void* ptr)
{
    while (Counter < 20) {
        pthread_mutex_lock(&Mutex);
        if (Counter < 20 && Counter % 2 != 0) {
            Counter++;
            printf("[O] counter = %d\n", Counter);
        }
        pthread_mutex_unlock(&Mutex);
    }
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    pthread_mutex_init(&Mutex, NULL); // initialize the mutex -- é necessario sempre antes dos pthread_create

    if (pthread_create(&tid1, NULL, thr_inc_even, NULL) != 0) {
        printf("Error creating even thread.\n");
        return -1;
    }

    if (pthread_create(&tid2, NULL, thr_inc_odd, NULL) != 0) {
        printf("Error creating odd thread.\n");
        return -1;
    }

    if (pthread_join(tid1, NULL) != 0) {
        printf("Error joining even thread.\n");
        return -1;
    }

    if (pthread_join(tid2, NULL) != 0) {
        printf("Error joining odd thread.\n");
        return -1;
    }

    printf("Finished: Counter=%d\n", Counter);
    return 0;
}