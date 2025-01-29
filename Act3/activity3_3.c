#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int i;
    int n;
    pid_t childpid;

    n = 4;
    for (i = 0; i < n; ++i) {
        // parent will recieve childpid, child will recieve 0
        childpid = fork();
        wait(0);
        if (childpid > 0){ // if is parent, and finished wait get out and print
            break;
        }
    }
    printf("This is process %ld with parent %ld\n", (long)getpid(), (long)getppid());
    wait(0);
}
