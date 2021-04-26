enum {
    exitNum = 2,
    cdNum
};

int runBuiltInCommand(int builtInCommandNum, int pipe_fd[]);
void builtInCommandJudge(char command[], char command_argv[], int pipe_fd[]);