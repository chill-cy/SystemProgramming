/*************************************************************************
	> File Name: group.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月27日 星期六 11时14分03秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unstd
int main() {
    pid_t pid;
    int n = 3;
    while (n--) {
        pid = fork();
        if (pid == 0) {
            break;
        }
    }
    if (pid == 0) {
        while (1) {
            printf("I am child %d\n", getpid());
            sleep(3);
        }
    }
    else if(pid > 0) {
        printf("n = %d\n", )
        pid_t pid_c;
        while (1) {
            printf("I am parent \n");
            pid_c = waitpid(0, NULL, WNOHANG);
            if (pid_c == -1) {
                n++;
                if (n == 3) 
                    break;
            } 
            if (pid_c > 0)
            else {
                printf("wait for child %d\n", pid_c);
            }
        }
    }
    return 0;
}
