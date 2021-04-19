#include <unistd.h>
#include <string.h>

/**
 * exitコマンド
**/
void iaeExit(char command[])
{
    if (strcmp(command, "exit") == 0) {
        _exit(2);
    }
}