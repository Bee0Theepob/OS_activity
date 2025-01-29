#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    int pid;
    pid = fork();
    if (pid > 0) {
        sleep(20);
        printf("Parent process exit\n");
    }
    else if (pid == 0) {
        exit(0);
    }   
}