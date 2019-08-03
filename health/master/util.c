/*************************************************************************
	> File Name: util.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月02日 星期五 10时43分50秒
 ************************************************************************/

#include <stdio.h>
#include "util.h"

int get_conf_value (char *file, char *key, char *value) {
    //打开文件,初始化空
    
    FILE *fp = NULL;
    //防错判断，参数错误若关键字空，没分配空间则出错
    if (key == NULL || value == NULL) {
        printf("failed to get value\n");
    }
    fp = fopen(file, "r");
    //打开文件失败
    if (fp == NULL) {
        printf("open file failed\n");
        return -1;
    }
    //读文件
    ssize_t read;//返回读取的大小，
    size_t n, len;//读取长度n, 存入line,
    char *line = NULL, *substr = NULL;
    //每次读一行，大于０成功
    while ((read = getline(&line, &n, fp)) >= 0) {
        //根据关键字找到对应参数，定义指向单词的指针
        substr = strstr(line, key);
        if (substr == NULL) continue;
        len = strlen(key);
        if (line[len] != '=') continue;
        //取关键字对应的值存如value
        strncpy(value, line + len + 1, (int)read - len - 2);
        break;
    }
    //排除没找到值的情况
    if (value == NULL) {
        printf("not found key\n");
        return -1;
    }
    free(line);
    fclose(fp);
    return 0;
}

int socket_create(int port) {
    int socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    //端口复用，解绑后可以再次立即使用
    int flag = 1, len = sizeof(flag);
    //将flag赋给宏１
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &flag, len) < 0) {
        close(socketfd);
        perror("setsockopt");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    //本地字节序转网络字节序的短整型
    addr.sin_port = htons(port);
    //绑定本地ip
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("Error bind on socketfd");
        return -1;
    }
    listen(socketfd, 2000);
    return socketfd;
}

int socket_connect(int port, char *ip) {
    int socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("failed to socket\n");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    if (connect(socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("failed to connect\n");
        return -1;
    }
    return socketfd;
}
