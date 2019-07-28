/*************************************************************************
	> File Name: wait1.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月28日 星期日 15时48分27秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    char op[20] = {0};
    char filename[20] = {0};
    strcpy(filename, argv[1]);
    pid_t pid = fork();
    if (pid > 0) {
        wait(&pid);
        pid_t pid1 = fork();
        if (pid1 == 0) {
            strcpy(op, "gcc");
            char *exestr1[] = {op,"-o", "ccc", filename, NULL};
            int k = execv("/usr/bin/gcc", exestr1);
            if (k < 0) printf("failed1\n");
            exit(0);
        }
        wait(&pid1);
        pid_t pid2 = fork();
        if (pid2 == 0) {
            int k = execlp("./ccc", "./ccc", NULL);
            if (k < 0) printf("failed2\n");
            exit(0);
        } else {
            wait(&pid2);
        }
    } else {
            strcpy(filename, argv[1]);
            strcpy(op, "vim");
            char *exestr[] = {op, filename, NULL};
            int k = execv("/usr/bin/vim", exestr);
            if (k < 0) printf("failed0\n");
        exit(0);
    }

    return 0;
}
