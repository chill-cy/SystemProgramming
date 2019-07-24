/*************************************************************************
	> File Name: client-1.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月24日 星期三 19时28分33秒
 ************************************************************************/
#include "client.h"
#define PORT 8732
int main() {
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

    return 0;
}
