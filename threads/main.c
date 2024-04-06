//
// Created by andreixmanu on 06/04/24.
//
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

void* routine(){
    printf("Thread routine started\n");
    sleep(3);
    printf("Thread routine ended\n");
}


int main(int argc, char* argv[]){

    pthread_t thread1, thread2;
    if(pthread_create(&thread1, NULL, &routine, NULL)) return 1;
    if(pthread_create(&thread2, NULL, &routine, NULL)) return 2;

    if(pthread_join(thread1, NULL)) return 3;
    if(pthread_join(thread2, NULL)) return 4;

    return 0;
}