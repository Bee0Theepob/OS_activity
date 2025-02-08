#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
void *say_hello(void *data)
{
    char *str;
    str = (char *)data;
    printf("This is thread %s\n", str);
}
void main()
{   
    pthread_t t[10];
    char *str[] = {"1","2","3","4","5","6","7","8","9","10"};
    for(int i=0;i<10;i++){
        pthread_create(&t[i], NULL, *say_hello,str[i]);
    }
    for(int i=0;i<10;i++){
        pthread_join(t[i], NULL);
    }
}