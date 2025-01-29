#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int tokenize(char *string, char *delimiters, char ***arrayOfTokens){
    char *token;
    int numtokens;
    int i;
    /* skip the beginning delimiters */
    string += strspn(string, delimiters);
    if ((token = malloc(strlen(string) + 1)) == NULL)
        return -1;
    /* count tokens */
    strcpy(token, string);
    numtokens = 0;
    if (strtok(token, delimiters) != NULL)
        for (numtokens = 1; strtok(NULL, delimiters) != NULL;numtokens++);
    /* create array of pointers to tokens */
            if ((*arrayOfTokens = malloc((numtokens+1)*sizeof(char *))) ==NULL) {
            free(token);
            return -1;
            }
    /* fill pointers to tokens into the array */
    if (numtokens == 0)
    free(token);
    else {
    strcpy(token, string);
    (*arrayOfTokens)[0] = strtok(token, delimiters);
    for (i = 1; i < numtokens; i++)
    (*arrayOfTokens)[i] = strtok(NULL, delimiters);
    (*arrayOfTokens)[numtokens] = NULL;
    }
    return numtokens;
}

int main(){
    int run = 1;
    while(run) {
    printf("mysh >");
    char command[100];
    int n = 100;
    fgets(command, n, stdin);
    char delimiters[] = " \n";
    char **tokens; // Array of token strings
    int len = tokenize(command, delimiters, &tokens);

    pid_t pid = fork() ;
    if (pid < 0) {
        printf("Error : cannot fork\n");
        exit(1);
    }
    else if (pid == 0) {      
        if(strcmp(tokens[0],"exit") == 0){
            printf("i dont know !!!????\n");
        }
        execvp(tokens[0],tokens);
    }
    else {
        wait(0);
        // return(0);
    }

    /*
    After reading user input, do these steps
    1. use tokenize() function to get command
    2. fork a child process
    3. child use execvp() to run command
    4. parent call wait() until user enter "exit"
    */
    }
}
