/*************************************************************************
	> File Name: master.h
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月02日 星期五 11时33分12秒
 ************************************************************************/

#include "util.h"
#include <pthread.h>
#define MAXCLIENT 10000
#define BUFFSIZE 2048

typedef struct Node {
    //ip, port集合，心跳的
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
    int ind;//当前几号线程
    int port;
};

int insert(LinkList head, Node *node);
void output(LinkList head);
int find_min(int *sum, int ins);
void *do_heart(void *arg);
void *do_data(void *arg);
int check_connect(struct sockaddr_in addr, long timeout);
void listen_epoll(int listenfd, LinkList *linklist, int *sum, int ins, int heartPort);
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
    Node *p = head;//虚拟头结点不存值
    //当前是第几个人
    int cnt = 0;
    while (p->next) {
        //将网络字节序转化成本地字符串
        printf("[%d] : %s\n", ++cnt, inet_ntoa(p->next->addr.sin_addr));
        p = p->next;
    }

}

void *do_heart(void *arg) {
    struct Heart *harg = (struct Heart *)arg;
    while (1) {
        for (int i = 0; i < harg->ins; i++) {
            Node *p = harg->linklist[i];//指向第i条链表起始位置
            while (p->next) {
                if (check_connect(p->next->addr, harg->timeout) < 0) {
                    printf("%s is \033[31mnot online\033[0m on %d\n", inet_ntoa(p->next->addr.sin_addr), ntohs(p->next->addr.sin_port));
                    Node *q = p->next;
                    p->next = q->next;
                    free(q);
                    harg->sum[i] -= 1;
                } else {//在线
                    printf("%s is \033[32monline\033[0m on %d\n", inet_ntoa(p->next->addr.sin_addr), ntohs(p->next->addr.sin_port));
                    p = p->next;
                }
            }
        }
        sleep(5);
        printf("\n");
    }
}

int check_connect(struct sockaddr_in addr, long timeout) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    //设置非阻塞
    unsigned long ul = 1;
    //0是阻塞
    ioctl(sockfd, FIONBIO, &ul);
    struct timeval tm;
    tm.tv_sec = 0;
    tm.tv_usec = timeout;
    fd_set wset;
    FD_ZERO(&wset);
    //将套接子加入到wset集合，用select监控此集合
    FD_SET(sockfd, &wset);
    int error = -1;
    int ret = -1;
    int len = sizeof(int);
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        if (select(sockfd + 1, NULL, &wset, NULL, &tm) > 0) {
            //确定是否真正连接,获取套接字状态，SO_ERROR查看具体出错状态，error返回负值出错，len获取error的长度
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            //连接成功
            if (error == 0) {
                ret = 0;
            } else {
                ret = -1;
            }
        } else {//超时或者select失败
            ret = -1;
        }
    }
    //短连接一定要将连接关闭
    close(sockfd);
    return ret;
}

void *do_data(void *arg) {
    struct Data *darg = (struct Data *)arg;
    char log[50] = {0};
    //表示第几号线程
    sprintf(log, "./pth %d.log", darg->ind);
    //参数：监听的数目大小，创建好后会返回fd,使用完epoll后最好需要关闭？＃＃＃
    int epollfd = epoll_create(10000);
    //
    struct epoll_event ev, events[MAXCLIENT];

    while (1) {
        //a+打开可读写文件，文件不存在创建该文件，文件存在，写入的数据会被追加到文件尾后，即文件原先的内容会被保留
        FILE *fp = fopen(log, "a+");
        //遍历链表,将连接加到epoll池中
        Node *p = darg->head;//记录表头
        while (p->next) {//遍历表中每个结点
            int askfd = socket(AF_INET, SOCK_STREAM, 0);
            if (askfd < 0) {
                printf("Error socket on askfd");
                p = p->next;
                continue;
            }
            //设置非阻塞，
            unsigned long ul = 1;
            ioctl(askfd, FIONBIO, &ul);
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(darg->port);
            addr.sin_addr = p->next->addr.sin_addr;
            connect(askfd, (struct sockaddr *)&addr, sizeof(addr));
            fprintf(fp, "%s : %d login\n", inet_ntoa(addr.sin_addr), darg->port);
            //数据可写？？？
            ev.events = EPOLLOUT;
            ev.data.fd = askfd;
            //epoll_ctl注册,修改，删除，当前使用注册宏对askfd注册加入到epollfd中
            epoll_ctl(epollfd, EPOLL_CTL_ADD, askfd, &ev);
            //将创建好的已连接的套接字加入到链表结点p
            p->fd = askfd;
            p = p->next;
        }
        //将有变化的加入events事件池中,返回值：发生的事件数
        int reval = epoll_wait(epollfd, events, MAXCLIENT, 3 * 1000);
        if (reval < 0) {
            perror("epoll_wait");
            fclose(fp);
            return NULL;
        } else if (reval == 0) {//没有人连接成功，
            sleep(5);
            continue;
        }

        for (int i = 0; i < reval; i++) {
            //是否可写
            int clientfd = events[i].data.fd;
            if (events[i].events & EPOLLOUT) {
                unsigned long ul = 0;
                //将套接字变为阻塞，
                ioctl(clientfd, FIONBIO, &ul);
                char buff[BUFFSIZE] = {0};
                strcpy(buff, "hello");
                send(clientfd, buff, strlen(buff), 0);
                memset(buff, 0, sizeof(buff));
                int k = recv(clientfd, buff, sizeof(buff), 0);
                if (k > 0) {
                    printf("%s\n", buff);
                }
            }
            struct epoll_event em;
            em.data.fd = clientfd;
            //重新注册,MOD或删除DEL都可以,下次关注clientfd时需要重新设置clientfd的事件类型
            epoll_ctl(epollfd, EPOLL_CTL_DEL, clientfd, &em);
            close(clientfd);
        }
        fclose(fp);
        sleep(5);
    }

}

void listen_epoll(int listenfd, LinkList *linklist, int *sum, int ins, int heartPort) {
    unsigned long ul = 1;
    //设为非阻塞
    ioctl(listenfd, FIONBIO, &ul);
    int epollfd = epoll_create(MAXCLIENT);
    if (epollfd < 0) {
        perror("epoll_create");
        //若用进程写不可直接退出
        exit(1);
    }
    struct epoll_event ev, events[MAXCLIENT];
    ev.data.fd = listenfd;
    // EPOLLIN连接到达，有数据来临，EPOLLOUT有数据要写
    ev.events = EPOLLIN;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
        perror("epoll_ctl");
        exit(1);
    }
    while (1) {
        //－１一直阻塞,等待人连
        int reval = epoll_wait(epollfd, events, MAXCLIENT, -1);
        //防错
        if (reval < 0) {
            perror("epoll_wait");
            exit(1);
        } else if (reval == 0) {
            continue;
        }
        for (int i = 0; i < reval; i++) {
            //触发事件有人真正连接
            if (events[i].data.fd = listenfd && events[i].events & EPOLLIN) {
                struct sockaddr_in addr;
                socklen_t len = sizeof(addr);
                int newfd = accept(listenfd, (struct sockaddr *)&addr, &len);
                if (newfd < 0) {
                    perror("accept");
                    exit(1);
                }
                //加入
                int sub = find_min(sum, ins);
                Node *p = (Node *)malloc(sizeof(Node));
                addr.sin_port = heartPort;
                p->fd = newfd;
                p->addr = addr;
                insert(linklist[sub], p);
                sum[sub] += 1;
                printf("someone come in\n");
                close(newfd);
            }
        }
    }
    close(listenfd);
}
