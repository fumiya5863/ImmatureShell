#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "common/constants.h"
#include "common/function.h"
#include "built_in_command.h"

static void sigintSignalIgnore();
static void shellProcess();
static void childProcess(int pipe_fd[]);
static void getCommandPath(char command_path[], char command[]);
static int absoluteCommandPathJudge(char command_path[], char command[]);
static int relativeCommandPathJudge(char command_path[], char command[]);
static void createCommandPath(char command_path[], char env_path[], char command[]);
static int fileOpenFlag(char command_path[]);

/**
 * 親プロセス処理
**/
int main(void)
{    
    // シェル
    shellProcess();
    
    exit(EXIT_SUCCESS);
}

/**
 * SIGINTシグナルを無視
**/
static void sigintSignalIgnore()
{
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = SIG_IGN;
    act.sa_flags = SA_RESETHAND;

    if (sigaction(SIGINT, &act, NULL) < 0) {
        exit(EXIT_FAILURE);
    }
}

/**
 * シェル
**/
static void shellProcess()
{
    // SIGINTシグナルを無視
    sigintSignalIgnore();

    while(1) {
        int status, pipe_fd[2];
        pid_t pid, wait_pid;

        if (pipe(pipe_fd) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        if ((pid = fork()) < 0) {
            perror("Iae");
            exit(EXIT_FAILURE);
        }

        // 子プロセス
        if (pid == 0) {
            childProcess(pipe_fd);
        }

        // 子プロセスが終了するまで待機
        wait(&status);
        if (!WIFEXITED(status)) {
            perror("Iae");
            exit(EXIT_FAILURE);
        }

        // ビルトインコマンドがexitの場合は親プロセス終了
        int exitCommandFlag = runBuiltInCommand(WEXITSTATUS(status), pipe_fd);
        if (exitCommandFlag == 1) {
            break;
        }
    }
}

/**
 * 子プロセス処理
**/
static void childProcess(int pipe_fd[])
{
    char buffer[PATHNAME_SIZE], current_path[PATHNAME_SIZE];
    char command_path[PATHNAME_SIZE] = "";
    char *command, *command_argv;

    getCurrentPath(current_path);
    fprintf(stdout, "%s> ", current_path);

    if (scanf("%255[^\n]%*[^\n]", buffer) != 1) {
        _exit(EXIT_FAILURE);
    }
    
    // ユーザーが標準入力した値を分割
    command = strtok(buffer, " ");
    command_argv = strtok(NULL, " ");
    char *execve_argv[] = {command, command_argv, NULL};

    builtInCommandJudge(command, command_argv, pipe_fd);

    getCommandPath(command_path, command);

    execve(command_path, execve_argv, NULL);

    _exit(EXIT_SUCCESS);
}

/**
 *  コマンドパス取得
**/
static void getCommandPath(char command_path[], char command[])
{
    // 絶対パスで指定した場合
    if (absoluteCommandPathJudge(command_path, command)) {
        return;
    }

    // 相対パスで指定した場合
    if (relativeCommandPathJudge(command_path, command)) {
        return;
    }

    fprintf(stdout, "Iae: command not found: %s\n", command);
    _exit(EXIT_FAILURE);
}

/**
 * コマンドが絶対パスかどうか判定
**/
static int absoluteCommandPathJudge(char command_path[], char command[])
{
    strcpy(command_path, command);
    return fileOpenFlag(command_path);
}

/**
 * コマンドが相対パスかどうか判定
**/
static int relativeCommandPathJudge(char command_path[], char command[])
{
    char slash[2] = "/";
    char *str_env, *env_path;

    str_env = getenv("PATH");
    env_path = strtok(str_env, ":");
    createCommandPath(command_path, env_path, command);
    if (fileOpenFlag(command_path)) {
        return 1;
    }

    while ((env_path = strtok(NULL, ":")) != NULL) {
        createCommandPath(command_path, env_path, command);
        if (fileOpenFlag(command_path)) {
            return 1;
        }
    }

    return 0;
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
 *  ファイルの存在確認
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