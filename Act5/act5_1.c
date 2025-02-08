#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void *say_hello(void *data)
{
    char *str;
    str = (char *)data;
    while (1)
    {
        printf("%s\n", str);
        sleep(1);
    }
}
void main(int argc, char *argv[])
{
    pthread_t t1, t2;
    if(argc!=3){
        printf("usage : labthread1 arg1 arg2\n");
        return ;
    }

    char *av[10];
    for(int i=1;i<argc;i++){
        av[i-1]=argv[i];
    }

    pthread_create(&t1, NULL, say_hello, av[0]);
    pthread_create(&t2, NULL, say_hello, av[1]);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}