//
// Created by andreixmanu on 12/03/24.
//
// Esercitazione 1 - Sistemi Operativi

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[])
{
    // apro le due pipe
    int fd0, fd1;
    fd0 = open("pipeIn", O_WRONLY);
    if(fd0 == -1) perror("fd0 error\n");
    fd1 = open("pipeOut", O_RDONLY);
    if(fd1 == -1) perror("fd1 error\n");

    char c = 'a';
    char first_n[10];
    int i1 =0;
    char second_n[10];
    int first_n_found = 1;
    int i2 = 0;

    while(c != '\n'){
        read(fd1, &c, 1);
        printf("%c\n", c);

        // una volta che trovo il primo numero, lo metto in first_n
        // e flaggo che ho trovato il primo numero, così poi
        // trovo il secondo
        while(c >= '0' && c <= '9'){
            first_n[i1] = c;
            i1++;
            first_n_found =0;
        }

        // una volta trovato il secondo numero, lo metto in second_n
        while(c >= '0' && c <= '9' && first_n_found == 0){
            second_n[i2] = c;
            i2++;
        }
    }

    int first = atoi(first_n);
    int second = atoi(second_n);

    int res = first - second;

    // convert res to a string
    char res_str2[10];
    sprintf(res_str2, "%d", res);

    // write res to pipe
    char res_str[10];
    int i = 0;
    while (res) {
        res_str[i++] = (res % 10) + '0';
        res /= 10;
    }
    res_str[i] = '\0';
    i--;
    while (i >= 0) {
        write(fd1, &res_str[i], 1);
        i--;
    }


    close(fd0);
    close(fd1);
   return EXIT_SUCCESS;
}