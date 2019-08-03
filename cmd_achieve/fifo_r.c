/*************************************************************************
	> File Name: fifo.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月02日 星期五 09时37分15秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

void sys_err(char *str, int exitno) {
    perror(str);
    exit(exitno);
}

int main(int argc, char *argv[]) {
    int fd, len;
    char buf[1024] = "hello xwp";
    if (argc < 2) {
        printf("./a.out fifoname\n");
        exit(1);
    }

    //fd = open(argv[i], O_RDONLY);
    fd = open(argv[1], O_WRONLY);
    if (fd < 0) {
        sys_err("open", 1);
    }
    
    len = read(fd, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, len);
    close(fd);
    
    return 0;
}
