#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "built_in_command.h"

static int iaeExitParent();
static void iaeExitChild();
static void iaeCdParent(int pipe_fd[]); 
static void iaeCdChild(char command[], char command_argv[], int pipe_fd[]);

/**
 * ビルトインコマンド実行(親プロセス)
**/
int runBuiltInCommand(int builtInCommandNum, int pipe_fd[])
{
    if (builtInCommandNum == exitNum) {
        return iaeExitParent();
    } else if (builtInCommandNum == cdNum) {
        iaeCdParent(pipe_fd);
    }
    
    return 0;
}

/**
 * ビルトインコマンド判定(子プロセス)
**/
void builtInCommandJudge(char command[], char command_argv[], int pipe_fd[])
{
    if (strcmp(command, "exit") == 0) {
        iaeExitChild();
    } else if(strcmp(command, "cd") == 0) {
        iaeCdChild(command, command_argv, pipe_fd);
    }
}

/**
 * exitコマンド(親プロセス呼び出し)
**/
static int iaeExitParent()
{
    return 1;
}

/**
 * exitコマンド(子プロセス呼び出し)
**/
static void iaeExitChild()
{
    _exit(exitNum);
}

/**
 * cdコマンド(親プロセス呼び出し)
**/
static void iaeCdParent(int pipe_fd[])
{
    char buf[512] = "";

    close(pipe_fd[1]);
    read(pipe_fd[0], buf, sizeof buf);
    close(pipe_fd[0]);

    // 現在のディレクトリを変更
    if (chdir(buf) != 0) {
        perror("cd");
    }
}

/**
 * cdコマンド(子プロセス呼び出し)
**/
static void iaeCdChild(char command[], char command_argv[], int pipe_fd[])
{
    close(pipe_fd[0]);
    write(pipe_fd[1], command_argv, strlen(command_argv));
    close(pipe_fd[1]);
    
    _exit(cdNum);
}