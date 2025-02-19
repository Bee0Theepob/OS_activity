#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void *say_hello(void *data)
{
    char *str;
    str = (char *)data;
    printf("%s\n", str);
}
void main()
{
    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, NULL, say_hello, "First thread from parent process");
    pthread_create(&t2, NULL, say_hello, "Second thread from parent process");

    pid_t pid = fork();
    if (pid != 0)
    {
        return;
    }
    else
    {
        pthread_create(&t3, NULL, say_hello, "First thread from child process");
        pthread_create(&t4, NULL, say_hello, "Second thread from child process");
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
}