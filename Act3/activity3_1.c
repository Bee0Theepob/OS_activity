#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    int notChildFlag, notGrandchildFlag;
    int childpid , parentpid;
    int childppid , parentppid;
    notChildFlag = fork();
    if (!notChildFlag) {
        childpid = getpid();
        childppid = getppid();
        printf("I am the child process");
        printf("My PID is %d", childpid);
        printf(" and ");
        printf("my parent's PID is %d\n", childppid);
        notGrandchildFlag = fork();
        if (!notGrandchildFlag) {
            childpid = getpid();
            childppid = getppid();
            printf("I am grandchild process");
            printf("My PID is %d", childpid);
            printf(" and ");
            printf("my parent's PID is %d\n", childppid);
        }
    }
    else {
        parentpid = getpid();
        printf("I am parent process");
        printf("My PID is %d\n", parentpid);
    }
}