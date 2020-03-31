#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<stdlib.h>

#define MAX_WORDS 2

int ReadLine(char* input);
void tokenize(char* line, char** words, int* nwords);
int CreateNewProcess(char** tokens, char* errorBuffer);
int FindExecute(char** tokens);

void HandleStatus(int status, char* errorBuffer);

int main()
{
    int exit = 1, numInput = 0;
    char input[4096], * words[MAX_WORDS];

    while (ReadLine(input))
    {
        tokenize(input, words, &numInput);

        FindExecute(words);
    }

    printf("\nGOOD BYE HUMAN!\n");
    return 0;
}

// Returns 0 on exit condition
int ReadLine(char* input)
{
    printf("\nPlease enter input: ");
    if (fgets(input, 4096, stdin) > 0)
    {
        return strcmp(input, "exit\n");
    }
    else
    {
        return 0; // fgets returned end of file;
    }
}

// Tokenize
// loops while

//Words read is less than max words
//&&
//strtok does not return a null pointer
void tokenize(char* line, char** words, int* nwords)
{
    *nwords = 1;

    for (words[0] = strtok(line, " \t\n"); (*nwords < MAX_WORDS) && (words[*nwords] = strtok(NULL, " \t\n")); *nwords = *nwords + 1)
        ; /* empty body */
    return;
}

int CreateNewProcess(char** tokens, char* errorBuffer)
{
    pid_t pid = fork();
    int status;
    if (pid == -1)
    {
        errorBuffer = "Could not fork a new process";
    }
    else if (pid == 0) //executes for child
    {
        printf("%s\n", tokens[0]);
        printf("%s\n", tokens[1]);
        execlp(tokens[0], tokens[0], tokens[1], NULL);
        perror("What?? ");
        exit(0);
    }
    else if (pid > 0) // executes for parent
    {
        wait(&status);
    }
    else // Should never execute but maybe it will, I don't know everything.
    {
        errorBuffer = "A strange error occured and I have no idea what happened";
    }
    return status;
}

int FindExecute(char** tokens)
{
    char error[60] = "\0";

    int status = CreateNewProcess(tokens, error);

    HandleStatus(status, error);

    return 0;
}

void HandleStatus(int status, char* errorBuffer)
{
    printf("%s", errorBuffer);

    if (WIFEXITED(status))
    {
        printf("Process Exited Normally.");
    }
    else
    {
        printf("Process exited Abnormally");
    }
}

