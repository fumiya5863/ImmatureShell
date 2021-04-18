#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

static void childProcess();
static void getCommandPath(char command_path[], char command[]);
static void createCommandPath(char command_path[], char env_path[], char command[]);
static int fileOpenFlag(char command_path[]);

// 内部コマンド
static void iaeExit(char command[]);

/**
 * 親プロセス処理
**/
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

        // 子プロセス
        if (pid == 0) {
            childProcess();
        }

        // 子プロセスが終了するまで待機
        wait(&status);
        // printf("%d", status);
        if (!WIFEXITED(status)) {
            printf("Iae: wait(%d) %s\n", errno, strerror(errno));
            return -1;
        }
        printf("%d", WIFEXITED(status));
    }
    return 0;
}

/**
 * 子プロセス処理
**/
static void childProcess()
{
    char buffer[256], command_path[256] = "";
    char *command, *comand_argv;

    printf("> ");
    if (scanf("%255[^\n]%*[^\n]", buffer) != 1) {
        exit(EXIT_SUCCESS);
    }
    
    command = strtok(buffer, " ");
    comand_argv = strtok(NULL, " ");
    char *argv[] = {command, comand_argv, NULL};

    // コマンドのパスが存在した場合パスを取得
    getCommandPath(command_path, command);

    execve(command_path, argv, NULL);
    exit(EXIT_SUCCESS);
}

/**
 *  コマンドパス取得
**/
static void getCommandPath(char command_path[], char command[])
{
    char *str_env, *env_path;
    char slash[2] = "/";

    strcpy(command_path, command);
    if (fileOpenFlag(command_path)) {
        return;
    }

    str_env = getenv("PATH");

    env_path = strtok(str_env, ":");
    createCommandPath(command_path, env_path, command);
    if (fileOpenFlag(command_path)) {
        return;
    }

    while ((env_path = strtok(NULL, ":")) != NULL) {
        createCommandPath(command_path, env_path, command);
        if (fileOpenFlag(command_path)) {
            return;
        }
    }

    printf("Iae: command not found: %s\n", command);
    exit(EXIT_FAILURE);
}

/**
 * コマンドパス作成
**/
static void createCommandPath(char command_path[], char env_path[], char command[])
{
    strcpy(command_path, "");
    strcat(command_path, env_path);
    strcat(command_path, "/");
    strcat(command_path, command);
}

/**
 *  コマンドファイルの存在確認 
**/
static int fileOpenFlag(char command_path[])
{
    FILE *fp = fopen(command_path, "r");
    if (fp != NULL) {
        fclose(fp);
        return 1;
    }

    fclose(fp);   
    return 0;
}

// 内部コマンド

/**
 * 親プロセス終了
**/
static void iaeExit(char command[])
{
    if (strcmp(command, "exit") == 0) {
        exit(2);
    }
}