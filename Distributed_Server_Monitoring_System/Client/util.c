/*************************************************************************
	> File Name: common.c
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月18日 星期四 20时24分58秒
 ************************************************************************/

#include <stdio.h>
#include "util.h"

int get_conf_value(char *file, char *key, char *value) {
    FILE *fp = NULL;
    char *line = NULL, *substr = NULL;
    size_t n,len = 0;
    ssize_t read;
    if (key == NULL || value == NULL) {
        printf("error\n");
        return -1;
    }
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Open file failed !\n");
        return -1;
    }
    while((read = getline(&line, &n, fp)) >= 0) {
        substr = strstr(line, key);
        if (substr == NULL) continue;
        len = strlen(key);
        if (line[len] != '=') continue;
        strncpy(value, line + len+1, (int)read -len - 2);
        break;
    }
    if (value == NULL) {
        printf("not found!\n");
        return -1;
    }
    free(line);
    fclose(fp);
    return 0;
}

char *my_ntoa(struct in_addr in) {
    static char ip[20] = {0} ;
    /*char *p;
    p = (char*)&in.s_addr;
    sprintf(ip, "%d.%d.%d.%d", p[0], p[1],p[2],p[3]);*/
    int a = in.s_addr >> 24;
    int b = (in.s_addr >> 16) &0xff;
    int c = (in.s_addr >> 8) &0xff;
    int d = (in.s_addr) &0xff;
    sprintf(ip, "%d.%d.%d.%d",d, c, b, a);
    return ip;
}

int socket_create(int port) {
    int socketfd;
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    } 
    int yes = 1 ,len = sizeof(int);
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
        close(socketfd);
        perror("setsockopt() error\n");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    listen(socketfd, 200);
    return socketfd;
}

int socket_connect(int port, char *host) {
    int socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);
    if (connect(socketfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    return socketfd;
}

int socket_connect_ioc(int port, char *host, int sec, double usec) {
    int sockfd;
    struct timeval tm;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    unsigned long ul = 1;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);
    ioctl (sockfd, FIONBIO, &ul);
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        if (errno == EINPROGRESS){
            int error;
            int len = sizeof(int);
            fd_set wfds;
            FD_ZERO(&wfds);
            FD_SET(sockfd, &wfds);
            tm.tv_sec = sec;
            tm.tv_usec = usec;
            if (select(sockfd + 1, NULL, &wfds, NULL, &tm) > 0) {
                getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *) &len);
                if (error < 0) {
                    close(sockfd);
                    return -1;
                }
            } else {
                close(sockfd);
                return -1;
            }
        } else {
            close(sockfd);
            return -1;
        } 
    }
    ul = 0;
    ioctl(sockfd, FIONBIO, &ul);
    return sockfd; 
}




