//
// Created by andreixmanu on 06/04/24.
//
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N_THREADS 5

int x = 0;
pthread_mutex_t mutex;

void *routine(){
    for(int i = 0; i < 1000000; i++){
        pthread_mutex_lock(&mutex);
        x++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


int main(int argc, char* argv[]){

    pthread_t threads[N_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < N_THREADS; i++){
        if(pthread_create(&threads[i], NULL, &routine, NULL)) return 1;
        printf("Thread %d created successfully\n", i);
    }

    for(int i = 0; i < N_THREADS; i++){
        if(pthread_join(threads[i], NULL)) return 2;
        printf("Thread %d finished successfully\n", i);
    }

    pthread_mutex_destroy(&mutex);
    printf("x: %d", x);

    return 0;
}