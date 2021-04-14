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
            printf("Iae: fork(%d) %s\n", errno, strerror(errno));
            return -1;
        }

        if (pid == 0) {
            childProcess();
        } else {
            wait_pid = wait(&status);
            if (wait_pid == -1) {
                printf("Iae: wait(%d) %s\n", errno, strerror(errno));
                return -1;
            }
        }
    }
    return 0;
}

static void childProcess()
{
    char buffer[256] = "";
    char filepath[512] = "/bin/";
    char *command, *comand_argv = "";

    printf("> ");
    if (scanf("%255[^\n]%*[^\n]", buffer) == EOF) {
        exit(EXIT_FAILURE);
    }
    command = strtok(buffer, " ");
    comand_argv = strtok(NULL, " ");
    strcat(filepath, command);
    char *argv[] = {command, comand_argv, NULL};

    if(execve(filepath, argv, NULL) == -1) {
        printf("Iae: command not found: %s\n", command);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}