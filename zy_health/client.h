/*************************************************************************
	> File Name: client.h
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年08月02日 星期五 16时46分11秒
 ************************************************************************/
#include "until.h"
#define BUFFSIZE 1024

void recv_heart(int port);
void recv_data(int port);

void recv_heart(int port) {
   int sockfd;
    sockfd = socket_create(port);
    if (sockfd < 0) {
        perror("heartprot");
        return ;
    }
    while (1) {
        int newfd = accept(sockfd, NULL, NULL);
        printf(" ❤ ");
        fflush(stdout);
        close(newfd);
    }
    close(sockfd);
}



void recv_data(int port) {
    int listenfd;
    if ((listenfd = socket_create(port)) < 0) {
        perror ("Error socket_create on listen");
        return;
    }
    while (1) {
        int newfd = accept (listenfd, NULL, NULL);
        if (newfd < 0) {
            perror("accept");
            continue;
        }
        char buff[BUFFSIZE] = {0};
        int k = recv(newfd, buff, sizeof(buff), 0);
        if (k <= 0) {
            perror("recv");
            close(newfd);
            continue;
        }
        printf("%s\n", buff);
        strcpy(buff,"client : hello");
        send(newfd, buff, strlen(buff), 0);
        close(newfd);
    }
    close(listenfd);
}

