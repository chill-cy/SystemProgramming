/*************************************************************************
	> File Name: client.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月20日 星期六 17时48分54秒
 ************************************************************************/

#include <stdio.h>
#include "common.h"
#define INS 6
#define MAX_SIZE 1000
#define PORT 8201
pthread_mutex_t mutex[10];

struct mypara {
    char path[50];
    char dirname[20];
    char error[20];
    char errpath[50];
    char num;
    char sleep_mes[10];
};

/*
void init(int n) {
    struct mypara *para;
    para->dirname *para;
    para->path = get_conf_value("./dirname.conf", n);
    para->num = n;
}
*/

int socket_listen(int);
int socket_create(int, char *);
int *func(void *);
int *func1(void *);

int main(int argc, char *argv[]) {
    for (int i = 0; i < 6; i++) {
        pthread_mutex_init(&mutex[i], NULL);
    }
    char buffer[MAX_SIZE];
    char *host = argv[1];
    pthread_t t[INS + 1];
    struct mypara para[INS + 1];

    for (int i = 0; i < INS; ++i) {
        para[i].num = i;
        if (pthread_create(&t[i], NULL, func, (void*)&para[i]) == -1) {
            printf("error\n");
            return -1;
        }
        printf("Current pthread 1 id = %ld \n", t[i]);
    }
    
    if (pthread_create(&t[INS], NULL, func1, (void *)&para[INS] ) == -1) {
        printf("error\n");
        return -1;
    }
    printf("Current pthread 2 id = %ld\n", t[INS]);
    int sock_client;
    while (1) {
        sock_client = socket_create(port, host);
        if (sock_client > 0) {
            printf("[success] heart shot \n");
        } else {
            printf("[false]heart shot\n");
        }
        close(sock_client);
        sleep(15);
    }
    return 0;
}

char * get_conf_value(const char *pathname, const char *key_name) {
    char *line;
    size_t len = 0;
    ssize_t read;
    char *value = (char *)calloc(sizeof(char), 100);
    FILE *fp = NULL;
    fp = fopen(pathname, "r");
    if (fp == NULL) {
        perror("fopen:");
        return NULL;
    }
    while ((read = getline(&line, &len, fp)) > 0) {
        char *ptr = strstr(line, key_name);
        if (ptr == NULL) continue;
        ptr += strlen(key_name);
        if (*ptr != '=') continue;
        strncpy(value, (ptr + 1), strlen(ptr + 2));
        int tempvalue = strlen(value);
        value[tempvalue] = '\0';
    }
    fclose(fp);
    return value;
}

int socket_listen(int port) {
    int socket_server;
    struct sockaddr_in s_addr;
    socklen_t len;
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket create");
        return -1;
    }
    
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(port);
    s_addr.sin_addr.s_addr = htons(INADDR_ANY);
    if ((bind(socket_server, (struct sockaddr*)&s_addr, sizeof(struct sockaddr))) < 0) {
        perror("bind");
        return -1;
    }
    if (listen(socket_server, 20) < 0) {
        perror("listen");
        return -1;
    }
    return socket_server;
}

int socket_create(int port, char *host) {
    int sock_client;
    struct sockaddr_in dest_addr;
    if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        return -1;
    }
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr.s_addr = inet_addr(host);

    if (connect(sock_client, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("Connect");
        return -1;
    }
    return sock_client;
}

void *func(void *argv) {
    FILE *fp;
    struct mypara *para;
    para = (struct mypara *)argv;
    char buffer[MAX_SIZE];
    char num[50];
    int warn_send;
    sprintf(num, "%d", para->num);
    char *path = get_conf_value("./path.conf", num);
    char *dirname = get_conf_value("./dirname.conf", num);
    char *sleep_mes = get_conf_value("./sleep_mes", num);
    int sleep_num = atoi(sleep_mes);
    while(1) {
        int retread = 0;
        fp = popen(path, "r");
        while ((retread = fread(buffer, 1, MAX_SIZE, fp)) > 0) {
            buffer[retread] = '\0';
            char str[100] = "./log/";
            mkdir(str, 0777);
            strcat(str, dirname);
            pthread_mutex_lock(&mutex[para->num]);
            fp = fopen(str, "a+");
            fprintf(fp, "%s\n", buffer);
            pthread_mutex_lock(&mutex[para->num]);
            if (para->num == 3 || para->num == 0) {
                if (strstr(buffer, "warning") != NULL) {
                    warn_send = socket_create(7771, "192.168.1.156");
                    if (send(warn_send, &buffer, strlen(buffer), 0) <= 0) {
                        perror("send_erron");
                    }
                    close(warn_send);
                }
            } else if (para->num == 4) {
                if (strlen(buffer) != 0) {
                    warn_send = socket_create(7771, "192.168.1.156");
                    if (send(warn_send, &buffer, strlen(buffer), 0) <= 0) {
                        perror("send_erron");
                    }
                    close(warn_send);
                }
            }
            fclose(fp);
        }
        fflush(stdout);
        memset(buffer, 0, sizeof(buffer));
        fclose(fp);
        sleep(sleep_num);
    }
    return NULL;
}

void *func1(void *argv) {
    FILE *fp;
    struct mypara *para;
    para = (struct mypara *)argv;
    char buffer[MAX_SIZE];
    int recv_socket = socket_listen(PORT);
    while(1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int confd;
        continue;
    } else {
        char *temp = inet_ntoa(client_addr.sin_addr);
        printf("receive connect from server:%s\n", temp);

        int retnum;
        int i;
        for (retnum = 100, i = 0; retnum <= 105, i < 6; retnum++, i++) {
            char num[50];
            sprintf(num, "%d", i);
            char *dirname = get_conf_value("./dirname.conf", num);
            char str[200] = "./log/";
            strcat(str, dirname);
            sleep(3);
            if (send(confd, &retnum, 4, 0) <= 0) {
                perror("send");
                break;
            }
            if (recv(confd, &buffer, 4, 0) <= 0) {
                perror("recv");
                break;
            }
            sleep(5);
            int newfd = socket_create(1029, "192.168.1.156");
            if (newfd == -1) continue;
            if (retnum == 100) {
                fp = fopen("./log/cpu.log", "r");
            } else if (retnum == 101) {
                fp = fopen("./log/disk.log", "r");
            } else if (retnum == 102) {
                fp = fopen("./log/mem.log", "r");
            } else if {
                fp = fopen("./log/cup.log", "r");
            } else if {
                fp = fopen("./log/malice.log", "r");
            } else if {
                fp = fopen("./log/user.log", "r");
            }
            int retread = 0;
            pthread_mutex_lock(&mutex[i]);
            while((retread = fread(buffer, 1, MAX_SIZE, fp)) > 0) {
                printf("[%d]th had send from %s!!!\n", i, dirname);
                send(newfd, buffer, retread, 0);
                memset(buffer, 0, sizeof(buffer));
            }
            fclose(fp);
            pthread_mutex_unlock(&mutex[i]);
            printf("send end!!\n");
            fflush(stdout);
            close(newfd);
            char str1[50];
            sprintf(str1, "> %s", str);
            system(str1);
            printf("\n");
            printf("[%d] remove success\n", i);
        }
        
    }
}
