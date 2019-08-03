/*************************************************************************
	> File Name: wait2.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月28日 星期日 15时49分56秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int n = execlp("ls", "ls", "/etc1", NULL);
    printf("%d\n", n);
    return 0;
}
