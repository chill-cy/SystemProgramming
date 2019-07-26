/*************************************************************************
	> File Name: client-1.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月24日 星期三 19时28分33秒
 ************************************************************************/
#include "client.h"
#define PORT 8732

struct Message {
    char from[20];
    char message[1024];
};

int main() {
    
    pid_t pid = fork();
    if (pid > 0) {
        int sockfd, listen;
        if ((listen = socket_create(PORT)) < 0) {
            perror("socket_create");
            exit(1);
        }

        while (1) {
            if ((sockfd = accept(listen, NULL, NULL)) < 0) {
                perror("accept");
                continue;
            }
            char buff[BUFFSIZE] = {0};
            char buff1[BUFFSIZE] = {0};
            if (recv(sockfd, buff, BUFFSIZE, 0) >0){
                printf("%s\n", buff);
                sprintf(buff1, "client: %s ", buff);
            send(sockfd, buff1, strlen(buff1), 0);
            close(sockfd);
            } else {
                close(sockfd);
            }
        }
    }

    if (pid == 0) {
        pid_t pid = fork();
            int socket_fd;
            if (pid > 0) {
                struct passwd *pwd;
                char ip_addr[20] = "192.168.2.142";
                int port = 8755;
                char username[20] = {0};
                socket_fd = socket_connect(port, ip_addr);
                if (socket_fd > 0) {
                    printf("连接成功!\n");
                } else {
                    printf("连接失败!\n");
                }
                char buff[1024];
                scanf("%s", buff);
                send(socket_fd, buff, strlen(buff), 0); 
            } 
            if (pid == 0) {
                struct Message mes;
                int k = recv(socket_fd, &mes, sizeof(mes), 0);
                if (k != -1) {
                    printf("已加入!\n");
                }
            }
        close(socket_fd);
    }
    return 0;
}
