#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    int pid ;
    pid = fork();
    if (pid == 0)
        printf("This is child process\n");
    else if (pid > 0)
        printf("This is parent process\n");
    else
        printf("Cannot fork\n");
}