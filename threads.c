#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

void * example_code(void * a) {
    pthread_t tid;
    int ptid;

    tid  = pthread_self();      // library tid
    ptid = syscall(SYS_gettid);

    printf("I am thread %lu (%i) from process %i\n",tid,ptid,getpid());
    sleep(1);
    pthread_exit(NULL);
}

int main() {
    pthread_t tid[2];
    int i,err;

    // creates a thread given the pthread_t variable as first parameter
    // thread attributes are default (second parameter is NULL)
    // example_code is the function the thread has to execute
    // no function parameters (fourth parameter is NULL)
    for (i=0;i<2;i++) {
        if ((err=pthread_create(&tid[i],NULL,example_code,NULL))) {
            printf("errore create [%i]\n",err);
            exit(EXIT_FAILURE); }
    }
    // waits for threads. Doesn't read return value (second parameter is NULL)
    for (i=0;i<2;i++) {
        if ((err=pthread_join(tid[i],NULL))) {
            printf("Join error [%i]\n",err);
            exit(EXIT_FAILURE); }
    }
    printf("Threads terminated execution successfully\n");
}