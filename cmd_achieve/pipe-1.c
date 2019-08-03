/*************************************************************************
	> File Name: pipe-1.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月01日 星期四 11时21分59秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int fd[2];
    pid_t pid;
    //fd[0]读端
    //fd[1]写端
    char str[1024];
    char buff[1024];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }
    pid = fork();
    //父写子读
    if (pid > 0) {
        //in parent,父进程里关闭父读;
        close(fd[0]);
        scanf("%s", str);
        write(fd[1], str, strlen(str));
        wait(NULL);
    }
    else if (pid == 0) {
        //子进程里，关闭子写
        close(fd[1]);
        int len = read(fd[0], str, strlen(str));
        //sprintf(str, "child %s", buff);
        //printf("%s", str);
        write(STDOUT_FILENO, str, len);
    }
    else {
        perror("fork");
        exit(1);
    }
    return 0;
}
