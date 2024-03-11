# OS Cheatsheet

# C

## Fork()

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
#include <sys/wait.h>
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


