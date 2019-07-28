/*************************************************************************
	> File Name: wait.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月27日 星期六 10时26分20秒
 ************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    pid_t pid, pid_c;
    pid = fork();
    if (pid > 0) {
        //int fd = open(argv[1], O_RDWR, 0744);
        while (1) {
          //  printf("I am parent %d\n", getpid());
            pid_c = wait(NULL);//wait是一个阻塞函数，等待回收子进程资源，如果没有子进程，wait返回－１
            if (pid_c != 0) {
                printf("error\n");
                exit(1);
            }
            execlp(argv[1], "./a.out", NULL);
            printf("wait for child %d\n", pid_c);
            sleep(1);
        }
    }
    //子进程
    else if(pid == 0) {
        execlp(argv[1], "gcc", argv[1], NULL);
        //printf("I am child %d\n", getpid());
        sleep(10);
    }
    else {
        perror("fork");
        exit(1);
    }
    return 0;
}
