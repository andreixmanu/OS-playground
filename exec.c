//
// Created by andreixmanu on 25/03/24.
//

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