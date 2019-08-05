/*************************************************************************
	> File Name: procee_mmap.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月05日 星期一 11时35分59秒
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define MAPLEN 0x1000
void sys_err(char *str, int exitno) {
    perror(str);
    exit(exitno);
}
int main(int argc, char *argv[]) {
    char *mm;
    int fd;
    if (argc < 2) {
        printf("./a.out filename\n");
        exit(1);
    }
    fd = open(argv[1], O_RDWR | O_CREAT, 0777);
    if (fd < 0) {
        sys_err("open", 1);
    }
    //映射的长度，映射到内存空间的权限，shared映射，映射的文件fd, 从０地址开始映射
    mm = mmap(NULL, MAPLEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (mm == MAP_FAILED) {
        sys_err("mmap", 2);
    }
    close (fd);
    while (1) {
        
        printf("%d\n", mm);
        sleep(1);
    }
    munmap(mm, MAPLEN);
    return 0;
}
