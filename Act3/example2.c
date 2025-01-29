#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    int pid;
    int childpid , parentpid;
    pid = fork();
    if (pid == 0){
        childpid = getpid();
        printf("This is child process %d\n", childpid);
    }
    else {
        parentpid = getpid();
        printf("This is parent process %d\n", parentpid);
    }
}