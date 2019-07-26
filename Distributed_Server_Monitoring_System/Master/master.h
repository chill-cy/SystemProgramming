/*************************************************************************
	> File Name: master.h
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月24日 星期三 16时06分18秒
 ************************************************************************/


#include "common.h"
#define CTLPORT 8732
#define BUFFSIZE 1024

typedef struct Node{
    struct sockaddr_in client_addr;
    int node_sock;
    struct Node *next;
}Node, *LinkedList;

struct HEART{
    LinkedList *head;
    int ins;
    int *sum;
    long timeout;
};


struct WORK{
    LinkedList head;
    int index;
};

int insert(LinkedList head, Node *node);
int find_min(int *sum, int ins);
void print_linkedlist(LinkedList head);
void *heartbeat_all(void *arg);
void *work(void *arg);
int check_connect(struct sockaddr_in, long timeout);
int do_event(struct sockaddr_in);
int socket_connect1(struct sockaddr_in);


int insert(LinkedList head, Node *node) {
    Node *p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
    return 0;
}

int find_min(int *sum, int ins) {
    int sub = 0;
    for (int i = 0; i < ins; i++){
        if (*(sum + i) < *(sum + sub)) {
            sub = i;
        }
    }
    return sub;
}

void print_linkedlist(LinkedList head){
    Node *p = head;
    int cnt = 0;
    while( p->next != NULL ) {
        printf("<%d> %s\n", cnt++, inet_ntoa(p->client_addr.sin_addr));
        p = p->next;
    }
}

//非阻塞连接每个结点
int check_connect(struct sockaddr_in addr, long timeout){
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 0;
    }
    int error = -1, len, ret = 0;
    struct timeval tv;
    unsigned long ul = 1;
    tv.tv_sec = 0;
    tv.tv_usec = timeout;
    len = sizeof(int);
    ioctl(sockfd, FIONBIO, &ul);

    fd_set set;
    FD_ZERO(&set);
    FD_SET(sockfd, &set);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        if (select(sockfd + 1, NULL, &set, NULL, &tv) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            if (error == 0) {
                ret = 1;
            } else
            ret = 0;
        }
    }
    close(sockfd);
    return ret;
}


//心跳遍历所有结点
void  *heartbeat_all(void *arg) {
    struct HEART *heart = (struct HEART *)arg;

    while (1) {
        //遍历所有链表
        for (int i = 0; i < heart->ins; i++) {
            Node *p = heart->head[i];
            //遍历每个链表结点
            while (p->next != NULL) {
                if (!check_connect(p->next->client_addr, heart->timeout)) {
                    printf("%s:%d \033[31mdeleting\033[0m ... \n", inet_ntoa(p->next->client_addr.sin_addr), ntohs(p->next->client_addr.sin_port));
                    Node *q;
                    q = p->next;
                    p->next = p->next->next;
                    free(q);
                    heart->sum[i]--;
                } else {
                    printf("%s:%d \033[32monline\033[0m ... \n", inet_ntoa(p->next->client_addr.sin_addr), ntohs(p->next->client_addr.sin_port));
                    p = p->next;
                }
            }
        }
        sleep(5);
    }
    return NULL;
}


//加入日志信息
void *work(void *arg) {
    struct WORK *inarg = (struct WORK *)arg;
    char log[50] = {0};
    sprintf(log, "./%d.log", inarg->index);
    while (1) {
        FILE *file = fopen(log, "w");
        Node *p = inarg->head;

        while(p->next != NULL) {
            fprintf(file, "%s:%d\n", inet_ntoa(p->next->client_addr.sin_addr), ntohs(p->next->client_addr.sin_port));
            do_event(p->next->client_addr);
            p = p->next;
        }
        fclose(file);
        sleep(5);
    }

    return NULL;
}

int socket_connect1(struct sockaddr_in addr) {
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return -1;
    }

    return sockfd;
}




int do_event(struct sockaddr_in addr) {
    int sockfd;
    char buff[BUFFSIZE] = {0};
    addr.sin_port = htons(CTLPORT);
    if ((sockfd = socket_connect1(addr)) < 0) {
        perror("socket_connect1");
        return -1;
    }
    send(sockfd, "Hello", strlen("hello"), 0);

    if (recv(sockfd, buff, BUFFSIZE, 0) <= 0) {
        close(sockfd);
        return 0;
    }
    close(sockfd);
    printf("%s\n", buff);
    return 0;
}

