/*************************************************************************
	> File Name: client.h
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月02日 星期五 16时46分11秒
 ************************************************************************/

#include "util.h"
#define BUFFSIZE 2048

void recv_heart(int port);
void recv_data(int port);

void recv_data(int port) {
    int listenfd;
    //无法绑定端口，根本错误
    if ((listenfd = socket_create(port)) < 0) {
        perror("Error socket_create on listenfd");
        return ;
    }
    while (1) {
        //可能对方网络问题
        int newfd = accept(listenfd, NULL, NULL);
        if (newfd < 0) {
            perror("accept");
            continue;
        }
        char buff[BUFFSIZE] = {0};

        int k = recv(newfd, buff, sizeof(buff), 0);
        //数据传输失败
        if (k < 0) {
            perror("recv");
            close(newfd);
            continue;
        }
        printf("\n%s\n", buff);
        strcpy(buff, "bye");
        send(newfd, buff, strlen(buff), 0);
        close(newfd);
    }
    close(listenfd);
}

void recv_heart(int port) {
    int sockfd;
    //获取已经是监听状态的套接字
    sockfd = socket_create(port);
    if (sockfd < 0) {
        perror("Error bind on heartPort");
        return ;
    }
    while (1) {
        int newfd = accept(sockfd, NULL, NULL);
        //一次心跳
        printf(" ❤ ");
        fflush(stdout);
        close(newfd);
    }
    close(sockfd);
}
