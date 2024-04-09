# OS Cheatsheet

# C

## fork()

*Necessary Libraries*

```c
#include <unistd.h>
```

*Description*
> Creates a child process from the parent process.
> The child process has pid 0, and has ppid of the parent process.
> Both child and parent process execute the whole program. 

*Usage*

```cpp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    
    pid_t pid = fork();

    if(pid == 0)
    { // child process
        printf("[FIGLIO]\t%d\n", pid);
    } else if(pid < 0)
    { // fork error
        perror("Errore nella fork\n");
        exit(EXIT_FAILURE);
    } else
    { // parent process
       printf("[GENITORE]\t%d\n", pid);
    }

    // this is going to be executed by child and parent
    printf("[1]\t%d\n", pid);
    printf("[2]\t%d\n", pid);
    
    return 0;
}
```
___

## wait()

*Necessary Libraries*

```cpp
#include <wait.h>
```

*Description*

> Makes the parent process wait for the child process to finish its execution.
> It then resumes the parent process' execution.

*Usage*

```cpp
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    pid_t pid = fork();
    int n;

    if(pid == 0)
    { //child process
        n = 1;
    } else
    { //parent process
        n = 6;
    }
    if(pid != 0) wait(NULL);
    
    for (int i = n; i < n + 5; i++) {
        printf("%d ", i);
        fflush(stdout);
    }
    printf("\n");
    
    return 0;
}
```
---

## pipe()

*Necessary Libraries*

```cpp
#include <unistd.h>
```

*Description*

> Creates a pipe with a buffer memory, used to connect and pass
> data from 2 processes. Each pipe has 2 ends, one for reading from the stream,
> the other for writing on the stream. The pipe is an int array of 2 elements, one for 
> reading the content[0], the other one for writing[1]. pipe() returns 0 if the creation has been successful,
> -1 otherwise.

*Usage*

```cpp
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int fd[2]; // file descriptor
    // fd[0] -> read
    // fd[1] -> write
    if(pipe(fd) == -1) perror("Pipe error\n");

    pid_t pid = fork();
    if(pid == 0)
    { // child process
        close(fd[0]); // close useless end of pipe
        int x;
        printf("Write the number: ");
        scanf(" %d", &x);
        write(fd[1], &x, sizeof(int)); // write data on the pipe
        close(fd[1]); // close the pipe after use
    } else
    {
        close(fd[1]); // close useless end of pipe
        int y;
        read(fd[0], &y, sizeof(int)); // read data from the pipe
        close(fd[0]); // close the pipe after use 
        printf("Value read: %d", y);
    }
    return 0;
}
```
<br>

*Complementary functions*


```cpp
write(int* pipe, void* buffer, size_t n);
```
Writes the *buffer* of *n* bytes in the pipe. 

<br>

```cpp
read(int* pipe, void* buffer, size_t n);
```
Reads the *buffer* of *n* bytes in the pipe. 

<br>

```cpp
close(int* pipe);
```
Closes the pipe on the selected end (1 for writing-end, 0 for reading-end).

*It's advised to close the ends that the process does not need. Every different 
process has its own pipe entrances. Closing one end in one process does not close
all the entrances to the pipe for the other processes.*

*It's advised to first create the pipe, and then fork the process, this way each
process will have the same pipe cloned from the previous code.*

--- 

## mkfifo()

*Necessary Libraries*

```cpp
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
```

*Description*
> Creates a pipe file in the local path. The pipe can be opened by 
> any other process or program, and can be read or written. Use the previous
> pipe() commands to interact with the pipe.
> 
> If the pipe is opened in write-only or read-only mode, the program stops executing,
> waiting for another process to read or write the pipe.
> 
> If the pipe in opened in read and write mode, the program continues its execution.

*Usage*

```cpp
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#define FIFO_NAME "myFifo"

int main(int argc, char* argv[])
{
    if (mkfifo(FIFO_NAME, 0777) == -1) // creates a pipe file in the filesystem
    { // the pipe has permissions '0777' in octal value
        if (errno == EEXIST) 
            printf("FIFO '%s' already exists.\n", FIFO_NAME);
        else 
        {
            perror("Fifo creation error\n");
            return 1;
        }
    } else
        printf("FIFO '%s' created successfully.\n", FIFO_NAME);
    
    int fd = open(FIFO_NAME, O_WRONLY); 
    
    /* program waits here for a process that reads the pipe
    * use parameter "O_RDWR" to both read and write
    * use parameter "O_RDONLY" to only read the pipe file
    * use command "cat" on the pipe to read it
    */
    
    int x = 97;
    if(write(fd, &x, sizeof(x)) == -1) perror("Write error\n");
    close(fd);

    return 0;
}
```
---

## Exercise

> In directory 'pipe_exercise' there is a little example of real usage of the pipes.
> 
> - 'write_pipe' writes the array on 'my_pipe'
> 
> - 'read_pipe' reads the array from 'my_pipe'
> 
> To execute the exercise, first execute write_pipe, then read_pipe.

---

## execlp()

*Necessary libraries*

```cpp
#include <unistd.h>
```

*Description*
> Executes a terminal command. If the function ends correctly, the program replaces its execution
> with the terminal command. There are many 'exec' variations, this option takes a (l)ist of arguments,
> and uses the (p)ath variables to run the commands. The function parameters must end with a NULL.
> It returns -1 if the execution of the command was not successful, otherwise the command is run and the 
> process is replaced. 

*Usage*

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char* argv[])
{
    int pid = fork();
    if(pid == -1) exit(1);

    if(pid == 0)
    { // child process
         /* the function substitutes the whole program with this command
         * any code in the same scope after the function will not be executed
         * the function returns -1 if the command could not be found, and
         * keeps running the code underneath
         */
        int error = execlp("ping", "ping", "-c", "3", "google.con", NULL);
        if(error == -1) printf("Could not execute command\n");
        return 1;
    } else
    { // parent process
        int wstatus;
        wait(&wstatus);
        if(WIFEXITED(wstatus))
        {
            int status_code = WEXITSTATUS(wstatus);
            if(status_code == 0) printf("Code executed successfully\n");
            else printf("Code failed: error %d\n", status_code);
        }
    }
    return 0;
}
```
---

# Threads

*Description* 
> A thread is the simplest execution process that can perform a single function.
> Every thread created shares the same memory, while multiple processes have their 
> own memory. Because of that, more than one thread can access the same variable 
> at the same time, causing a so called 'race condition' that must be managed by
> the operating system.

## pthread_create(), pthread_join()

*Necessary libraries*
```cpp
#include <pthread.h>
```

*Description*
>   1) Creates a thread that executes the given function with parameters.
>   The function returns 0 if the creation has been successful. 
>   2) Waits for the thread to finish its execution, and can collect its 
>   return value.

*Usage*
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>  

void* example_code(void* a) {
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
```
___

## Mutex

*Necessary libraries*

```cpp
#include <pthread.h>
```

*Description*
> Mutex is used to partially solve the 'race condition' problem regarding threads.
> You need to declare a mutex global variable, so u can 'lock' and 'unlock' the 
> critical parts of your functions that could result in a race condition. The lock
> function lets the first thread execute the following code, and makes the others
> wait for the first thread to unlock the code. Then the second-arrived thread 
> can execute the code, and so on. 

*Usage*

```cpp
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
        // lock the execution to only the going thread
        pthread_mutex_lock(&mutex);
        x++;
        // let the next thread continue the execution
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


int main(int argc, char* argv[]){
    
    // array of n threads
    pthread_t threads[N_THREADS];
    // initialize the mutex variable
    pthread_mutex_init(&mutex, NULL);
    
    // create n threads
    for(int i = 0; i < N_THREADS; i++){
        if(pthread_create(&threads[i], NULL, &routine, NULL)) return 1;
        printf("Thread %d created successfully\n", i);
    }
    
    // what for n threads returns
    for(int i = 0; i < N_THREADS; i++){
        if(pthread_join(threads[i], NULL)) return 2;
        printf("Thread %d finished successfully\n", i);
    }
    
    // destroy the mutex
    pthread_mutex_destroy(&mutex);
    printf("x: %d", x);

    return 0;
}
```
___

## semaphores

*Necessary libraries*
```cpp
#include <semaphore.h>
```

*Description*

> Semaphores are similar to mutexes, but they allow multiple n threads
> to execute the same critical code at the same time. When the 
> semaphore counter reaches 0, no more threads are allowed to execute
> the code and have to wait.

*Usage*

```cpp
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define THREAD_NUM 4

// global semaphore variable
sem_t semaphore;

void* routine(void* args) {
    sem_wait(&semaphore);
    sleep(1);
    printf("Hello from thread %d\n", *(int*)args);
    sem_post(&semaphore);
    free(args);
}

int main(int argc, char *argv[]) {

    pthread_t th[THREAD_NUM];
    // initiate the semaphore for 0 other processes, 
    // starting from the third parameter's value
    sem_init(&semaphore, 0, 1);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semaphore);
    return 0;
}
```