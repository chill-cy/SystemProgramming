/*************************************************************************
	> File Name: exec.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月28日 星期日 15时39分00秒
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int pid = fork();
    if (pid != 0){
        if(execlp("gcc", "gcc", argv[1], "-o", "haizei", NULL) < 0){
            perror("exec");
        }
        /*int pid = wait(NULL);
        printf("%d\n", pid);*/
        sleep(2);
    } else {
        execlp("./haizei","./haizei", NULL);
        sleep(2);
    }
    return 0;
}
