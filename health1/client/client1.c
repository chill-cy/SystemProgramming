/*************************************************************************
	> File Name: client1.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月03日 星期六 16时36分27秒
 ************************************************************************/

#include <stdio.h>
#include "util.h"
#include <pthread.h>
#define MAXCLIENT 10000
#define BUFFSIZE 2048

typedef struct Node {
    struct sockaddr_in addr;
    int fd;
    struct Node *next;
} Node, *LinkList;

struct Heart {
    int *sum;
    LinkList *linklist;
    int ins;
    long timeout;
};

struct Data {
    LinkList head;
    int ind;
    int port;
};

struct Work {
    LinkList link;
    int index;
    int ctlport;
    int dataport;
};

int insert(LinkList head, Node *node);
void output(LinkList head);
int find_min(int *sum, int ins);
void *do_heart(void *arg);
void *do_data(void *arg);
int check_connect(struct sockaddr_in addr, long timeout);
void listen_epoll(int listenfd, LinkList *linklist, int *sum, int ins, int heartPort);;

int find_min(int *sum, int ins) {
    int min = 999999, ind;
    for (int i = 0; i < ins; i++) {
        if (sum[i] < min) {
            ind = i;
            min = sum[i];
        }
    }
    return ind;
}

int insert(LinkList head, Node *node) {
    Node *p = head;
    while (p->next) {
        p = p->next;
    }
    p->next = node;
    return 1;
}

void output(LinkList head) {
    Node *p = head;
    int cnt = 0; 
    while (p->next) {
        printf("[%d] : %s\n", ++cnt, inet_ntoa(p->next->addr.sin_addr));
        p = p->next;
    }
}

void *do_heart(void *arg) {
    struct Heart *harg = (struct Heart *)arg;
    while (1) {
        for (int i = 0; i < harg->ins; i++) {
            Node *p = harg->linklist[i];
            while (p->next) {
                if (check_connect(p->next->addr, harg->timeout) < 0) {
                    printf("%s is \033[31mnot online\033[0m on %d\n", inet_ntoa(p->next->addr.sin_addr), ntohs(p->next->addr.sin_addr));
                    Node *q = p->next;
                    p->next = q->next;
                    free(q);
                    harg->sum[i] -= 1;
                } else {
                    printf("%s is \033[32monline\033[0m on %d\n", inet_ntoa(p->next->addr.sin_addr), ntohs(p->next->addr.sin_port));
                    p = p->next;
                }
            }
        }
        sleep(5);
        printf("\n");
    }
}

int check_connect(struct sockaddr_in addr)

int main() {
    
    return 0;
}
