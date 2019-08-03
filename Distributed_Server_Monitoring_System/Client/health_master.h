/*************************************************************************
	> File Name: master.h
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月24日 星期三 14时17分38秒
 ************************************************************************/
/*************************************************************************
	> File Name: master.h
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月24日 星期三 14时17分38秒
 ************************************************************************/
#include "util.h"
#include <stdio.h>
#define MAXCLIENT 10000
#define BUFFSIZE 1024

typedef struct Node {
    struct sockaddr_in addr;
    int fd;
    struct Node *next;
} Node, *LinkList;

typedef struct Heart {
    LinkList *link;
    int ins;
    int *sum;
    long timeout;
} Heart;

struct Work {
    LinkList link;
    int index;
    int ctlport;
    int dataport;
};

struct Script {
    char logname[50];
    char message[BUFFSIZE];
};

int find_min(int *sum, int ins);
int insert(LinkList head, Node *q);
void output(LinkList head);
void *do_heart(void *arg);
void *do_work(void *arg);
int check_connect(struct sockaddr_in addr, long timeout);
int listen_epoll(int listenfd, LinkList *link, int *sum, int ins, int heartport);
int do_event(struct sockaddr_in addr, int port);
int socket_con(struct sockaddr_in addr);

int find_min(int *sum, int ins) {
    int min = 99999, ind;    
    for (int i = 0; i < ins; i++) {
        if (sum[i] < min) {
            min = sum[i];
            ind = i;
        }
    }
    return ind;
}

int insert(LinkList head, Node *q) {
    Node *p = head;
    while (p->next) {
        p = p->next;
    }
    p->next = q;
    return 1;
}

void output(LinkList head) {
    Node *p = head;
    int cnt = 0;
    while (p->next) {
        printf("[%d]:%s\n", ++cnt, inet_ntoa(p->next->addr.sin_addr));
        p = p->next;
    }
}

int check_connect(struct sockaddr_in addr, long timeout) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return 0;
    }
    int error = -1, ret = 0;
    int len = sizeof(int);
    struct timeval tm;
    tm.tv_sec = 0;
    tm.tv_usec = timeout;
    unsigned long ul = 1;
    ioctl(sockfd, FIONBIO, &ul);
    
    fd_set set;
    FD_ZERO(&set);
    FD_SET(sockfd, &set);
    
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        if (select(sockfd + 1, NULL, &set, NULL, &tm) > 0) {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&len);
            if (error == 0) {
                ret = 1;
            } else {
                ret = 0;
            }
        }
    }
    close(sockfd);
    return ret;
}


void *do_heart(void *arg) {
    Heart *heartarg = (Heart*)arg;
    while (1) {
        for (int i = 0; i < heartarg->ins; i++) {
            Node *p = heartarg->link[i];
            while (p->next) {
                if (!check_connect(p->next->addr, heartarg->timeout)) {
                    printf("%s : %d \033[31mhas deleted\033[0m\n", inet_ntoa(p->next->addr.sin_addr), ntohs(p->next->addr.sin_port));
                    Node *q = p->next;
                    p->next = q->next;
                    free(q);
                    heartarg->sum[i] -= 1;
                } else {
                    printf("%s \033[32mis online\033[0m at port [%d]\n", inet_ntoa(p->next->addr.sin_addr), ntohs(p->next->addr.sin_port));
                    p = p->next;
                }
            }
        }
        sleep(5);
        printf("\n");
    }
}

void *do_work(void *arg) {
    struct Work *inarg = (struct Work*)arg;
    char log[50] = {0};
    sprintf(log, "./%d.log", inarg->index);
    while (1) {
        sleep(15);
        Node *p = inarg->link;
        while (p->next) {
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                perror("socket");
                return NULL;
            }
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(inarg->ctlport);
            addr.sin_addr = p->next->addr.sin_addr;
            if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
                close(sockfd);
                continue;
            }
            for (int i = 1001; i <= 1006; i++) {
                send(sockfd, &i, sizeof(int), 0);
                int ack = 0;
                int k = recv(sockfd, &ack, sizeof(int), 0);
                if (k <= 0) {
                    printf("this recv");
                    continue;
                }
                if (!ack) continue;
                int recvfd = socket_create(inarg->dataport);
                if (recvfd < 0) {
                    perror("socket_create");
                    break;
                }
                send(sockfd, &ack, sizeof(int), 0);
                struct sockaddr_in naddr;
                socklen_t len = sizeof(naddr);
                int newfd = accept(recvfd, (struct sockaddr*)&naddr, &len);
                if (newfd < 0) {
                    perror("accept");
                    close(recvfd);
                    continue;
                }
                struct Script sct;
                char filename[100] = {0};
                char fdir[50] = {0};
                strcpy(fdir, inet_ntoa(naddr.sin_addr));
                if (access(fdir, F_OK) < 0) {
                    int statu = mkdir(fdir, 0755);
                    if (statu < 0) {
                        printf("创建文件夹失败\n");
                        break;
                    }
                }
                while(1) {
                    memset(&sct, 0, sizeof(sct));
                    int k = recv(newfd, &sct, sizeof(struct Script), 0);
                    if (k <= 0) break;
                    //写入文件
                    sprintf(filename, "./%s/%s", fdir, sct.logname);
                    FILE *fw = fopen(filename, "a+");
                    flock(fw->_fileno, LOCK_EX);
                    fprintf(fw, "%s", sct.message);
                    fclose(fw);
                }
                close(newfd);
                close(recvfd);
            }
        close(sockfd);
        p = p->next;
        }
    } 
    
}


int listen_epoll(int listenfd, LinkList *link, int *sum, int ins, int heartport) {
    unsigned long ul = 1;
    int nfds;
    ioctl(listenfd, FIONBIO, &ul);
    int epollfd = epoll_create(MAXCLIENT);
    if (epollfd < 0) {
        perror("epoll_create");
        exit(1);
    }
    struct epoll_event events[MAXCLIENT], ev;
    ev.data.fd = listenfd;
    ev.events = EPOLLIN;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
        perror("epoll_ctl");
        exit(0);
    }
    while (1) {
        nfds = epoll_wait(epollfd, events, MAXCLIENT, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        } else if (nfds == 0) {
            continue;
        }
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == listenfd && events[i].events & EPOLLIN) {
                struct sockaddr_in addr;
                socklen_t len = sizeof(addr);
                int newfd = accept(listenfd, (struct sockaddr*)&addr, &len);
                if (newfd < 0) {
                    perror("accept");
                    exit(1);
                }
                int sub = find_min(sum, ins);
                Node *p = (Node *) malloc (sizeof(Node));
                addr.sin_port = htons(heartport);
                p->addr = addr;
                p->fd = newfd;
                p->next = NULL;
                insert(link[sub], p);
                sum[sub] += 1;
                printf("加入成功\n");
            }
        }
    }
}

/*void *do_work(void *arg) {
    struct Work *inarg = (struct Work*)arg;
    char log[50] = {0};
    sprintf(log, "./%d.log", inarg->index);
    while (1) {
        FILE *fp = fopen(log, "w");
        Node *p = inarg->link;

        while (p->next) {
            fprintf(fp, "%s:%d\n", inet_ntoa(p->next->addr.sin_addr), ntohs(p->next->addr.sin_port));
            int port = inarg->ctlport;
            do_event(p->next->addr, port);
            p = p->next;
        }
        fclose(fp);
        sleep(5);
    }
}


int do_event(struct sockaddr_in addr, int port) {
    int sockfd ;
    addr.sin_port = htons(port);
    if ((sockfd = socket_con(addr)) < 0) {
        perror("socket_connect");
        return -1;
    }
    char buff[BUFFSIZE] = {0};
    send(sockfd, "Hello", strlen("Hello"), 0);
    int k = recv(sockfd, buff, BUFFSIZE, 0);
    if (k <= 0) {
        close(sockfd);
        return 0;
    }
    close(sockfd);
    printf("%s\n", buff);
    return 1;
}

int socket_con(struct sockaddr_in addr) {
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return -1;
    }
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return -1;
    }
    return sockfd;
}
*/


/*
void *do_heart1(void *arg) {
    Heart *heartarg = (Heart *)arg;
    while (1) {
        for (int i = 0; i < heartarg->ins; i++) {
            Node *p = heartarg->link[i];
            fd_set set;
            FD_ZERO(&set);

            struct timeval tm;
            tm.tv_sec = 0;
            tm.tv_usec = heartarg->timeout;
            unsigned long ul = 1;
            int maxfd = -1;
            while (p->next) {
                int sockfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd < 0) {
                    perror("socket");
                    continue;
                }
                ioctl(sockfd, FIONBIO, &ul);
                connect(sockfd, (struct sockaddr*)&p->next->addr, sizeof(struct sockaddr));
                FD_SET(sockfd, &set);
                p->next->fd = sockfd;
                p = p->next;
            }
            int reval = select(maxfd + 1, NULL, &set, NULL, &tm);
            if (reval > 0) {
                p = heartarg->link[i];
                int error = -1, len = sizeof(int);
                while (p->next) {
                    int tmpfd = p->next->fd;
                    getsockopt(tmpfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&len);
                    if (error == 0) {
                        printf("%s \033[32mis online\033[0m at port [%d]\n", inet_ntoa(p->next->addr.sin_addr), ntohs(p->next->addr.sin_port));
                        p = p->next;
                    } else {
                        printf("%s : %d \033[31mhas deleted\033[0m\n", inet_ntoa(p->next->addr.sin_addr), ntohs(p->next->addr.sin_port));
                        Node *q = p->next;
                        p->next = q->next;
                        free(q);
                        heartarg->sum[i] -= 1;
                    }
                    close(tmpfd);
                }
            }
        }
        sleep(5);
        printf("\n");
    }
}
*/



