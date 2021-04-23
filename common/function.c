#include <string.h>
#include <unistd.h>

#include "constants.h"

/**
 * 現在のパスを取得 
**/
void getCurrentPath(char pathname[])
{
    // 変数初期化
    memset(pathname, '\0', PATHNAME_SIZE); 
    // カレントディレクトリ取得
    getcwd(pathname, PATHNAME_SIZE);
}