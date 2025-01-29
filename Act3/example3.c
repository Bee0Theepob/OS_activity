#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    int pid;
    int childpid , parentpid;
    int childppid , parentppid;
    pid = fork();
    if (pid == 0){
        childpid = getpid();
        childppid = getppid();
        printf("This is child process %d\n", childpid);
        printf("child parent pid = %d\n", childppid);
    }
    else {
        parentpid = getpid();
        parentppid = getppid();
        printf("This is parent process %d\n", parentpid);
        printf("parent's parent id = %d\n", parentppid);
    }
}