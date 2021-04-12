#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

static void childProcess();

int main()
{
    while(1) {
        int status;
        pid_t pid, wait_pid;
        pid = fork();

        if (pid < 0) {
            printf("Error: fork(%d) %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            childProcess();
        } else {
            wait_pid = wait(&status);
            if (wait_pid == -1) {
                printf("Error: wait(%d) %s\n", errno, strerror(errno));
                exit(EXIT_FAILURE);
            }
            printf("\n");
        }
    }
    return 0;
}

static void childProcess()
{
    char command[256];
    char filepath[256] = "/bin/";
    char *first_command, *command_argv;

    printf("> ");
    scanf("%[^\n]%*c", command);
    first_command = strtok(command, " ");
    command_argv = strtok(NULL, " ");
    strcat(filepath, command);
    char *argv[] = {command, command_argv, NULL};

    if(execve(filepath, argv, NULL) == -1) {
        printf("Errors: don't %s command", first_command);
    }
    exit(EXIT_SUCCESS);
}