/*************************************************************************
	> File Name: pathconf.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月01日 星期四 13时16分48秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);
    printf("pipe buf %ld\n", fpathconf(fd[0], _PC_PIPE_BUF));
    printf("%ld\n", fpathconf(STDOUT_FILENO, _PC_NAME_MAX));
    printf("%ld\n", pathconf("hello", _PC_NAME_MAX));

    return 0;
}
