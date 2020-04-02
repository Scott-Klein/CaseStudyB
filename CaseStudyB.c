#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_WORDS 2

// Read a line from the user and writes it to the input array
int ReadLine(char *input);

// Separates a line of input into up to two different tokens
void tokenize(char *line, char **words, int *nwords);

// Runs the system call and catches specific problems into the error buffer
int CreateNewProcess(char **tokens, int numOfTokens);

// Runs createNewProcess on the tokens and makes sure that errors get handled
void FindExecute(char **tokens, int numOfTokens);

// Takes all status codes and the errorBuffer to collate it all on screen for the user easily.
void HandleStatus(int status);

// Built in change directory for the running process.
void ChangeDirectory(char *destination);

int main()
{
    printf("\nScotts Interpreter. Version 1\n");
    int exit = 1, numInput;
    char input[4096], *words[MAX_WORDS];

    while (ReadLine(input))
    {
        numInput = 0;
        tokenize(input, words, &numInput);
        FindExecute(words, numInput);
    }

    printf("\nGoodbye!\n");
    return 0;
}

// Returns 0 on exit condition
// Otherwise writes user input to the buffer char array.
int ReadLine(char *input)
{
    //printf("\nPlease enter input: ");
    printf("%s$ ", getcwd(NULL, 100));
    if (fgets(input, 4096, stdin) > 0)
    {
        return strcmp(input, "exit\n"); // returns 0 if exit found
    }
    else
    {
        return 0; // fgets returned end of file;
    }
}

void FindExecute(char **tokens, int numOfTokens)
{
    if (!strcmp(tokens[0], "cd"))
    {
        ChangeDirectory(tokens[1]);
    }
    else
    {
        HandleStatus(CreateNewProcess(tokens, numOfTokens));
    }
}

void HandleStatus(int status)
{
    if (WIFEXITED(status))
    {
        printf("Process Exited with status %d\n", WEXITSTATUS(status));
    }
    else
    {
        printf("Process exited Abnormally"); // notify user that an issue occurred.
    }
}

/*      Tokenize
loops while
    Words read is less than max words
    &&
    strtok does not return a null pointer
End Loop
*/
void tokenize(char *line, char **words, int *nwords)
{
    *nwords = 1;

    for (words[0] = strtok(line, " \t\n"); (*nwords < MAX_WORDS) && (words[*nwords] = strtok(NULL, " \t\n")); *nwords = *nwords + 1)
        ;
    return;
}

int CreateNewProcess(char **tokens, int numOfTokens)
{
    pid_t pid = fork();
    int status = -1;
    if (pid == -1) // process couldn't create.
    {
        printf("Could not fork a new process");
    }
    else if (pid == 0) //executes for child
    {
        if (numOfTokens == 1)
        {
            execlp(tokens[0], tokens[0], NULL);
        }
        else if (numOfTokens == 2)
        {
            execlp(tokens[0], tokens[0], tokens[1], NULL);
        }
        perror("Execution problem: ");
        exit(0);
    }
    else if (pid > 0) // executes for parent
    {
        wait(&status);
    }
    return status;
}

void ChangeDirectory(char *dest)
{
    if (chdir(dest)) // if there's an error let's hear it.
    {
        perror("Directory change error: ");
    }
}
