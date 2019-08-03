/*************************************************************************
	> File Name: child.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月01日 星期四 10时18分30秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int n = 10;
    pid = fork();
    if (pid > 0) {
        printf("I am parent\n");
        exit(0);
    }
    else if (pid == 0) {
        while (n--) {
            printf("I am %d, my parent is %d \n", getpid(), getppid());
            sleep(1);
        }
    }
    else {
        perror("fork");
        exit(-1);
    }
    return 0;
}
