/*************************************************************************
	> File Name: until.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年08月02日 星期五 10时43分49秒
 ************************************************************************/

#include <stdio.h>
#include "until.h"



int get_conf_value (char *file, char *key, char *value) {
    
    FILE *fp = NULL;
    
    if(key == NULL || value == NULL) {
        printf("key or value errr\n");
        return -1;
    }

    fp = fopen(file, "r");

    if (fp == NULL) {
        printf("open file fail\n");
        return -1;
    }
        
    ssize_t read;
    size_t n, len;
    char *line = NULL, *substr = NULL;
    
    while ((read  = getline(&line, &n, fp)) >= 0) {
        substr = strstr(line, key);
        if (substr == NULL) continue;
        len = strlen(key);
        if (line[len] != '=') continue;
        strncpy(value, line + len + 1, (int)read - len - 2);
        break;
    }
    if (value == NULL) {
        printf("not found\n");
        return -1;
    }
    free(line);// line 是会被函数真实开辟空间的 
    fclose(fp);
    return 0;

}



int socket_create(int port) {
    int socketfd;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket errror\n");
        return -1;
    }

    int flag = 1, len = sizeof(int);

    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1) {
        close(socketfd);
        perror("setsockopt() errr\n");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定累本机
    
    if (bind(socketfd, (struct sockaddr *) &addr, sizeof(addr)) <0) {
        close(socketfd);
        perror("bind");
        return -1;
    }

    if (listen(socketfd, 1000) <0) {
        printf("listen fail\n");
        return -1;
    }
    return socketfd;


}


int socket_connect(int port, char *host) {
    int socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) <0) {
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);

    if(connect(socketfd, (struct sockaddr *)&addr, sizeof(addr)) <0) {
        perror ("connect :");
        return -1;
    }
    return socketfd;
}
