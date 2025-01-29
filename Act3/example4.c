#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    int pid;
    pid = fork();
    if (pid > 0) {
        printf("Parent process exit\n");
        exit(0);
    }
    else if (pid == 0) {
        printf("Child is sleeping \n");
        sleep(30);
        printf("Child process exit\n");
    }   
}