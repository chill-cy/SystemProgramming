/*************************************************************************
	> File Name: wait2.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月01日 星期四 09时55分20秒
 ************************************************************************/

#include <stdio.h>

int main() {
    pid_t pid;
    int n = 3;
    //创建３个子进程
    while (n--) {
        pid = fork();
        if (pid == 0) {
            break;
        }
    }
    
    if (pid == 0) {
        printf("I am child %d \n", getpid());
        sleep(3);
    }

    else if (pid > 0) {
        printf("%n = %d\n", n);
        pid_t pid_c;
        while (1) {
            printf("I am parent \n");
            pid_c = waitpid(0, NULL, WNOHANG);
            //已存在的子进程个数
            if (pid_c > 0) {
                n++;
            }
            //返回值为－１，还有子进程未结束
            if (pid_c == -1) {
                continue;
            }    
            else {
                printf("wait for child %d \n", pid_c);
            }
            //回收了３个子进程结束
            if (n == 3) {
                break;
            }
        }
    }
    return 0;
}
