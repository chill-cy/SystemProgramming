/*************************************************************************
	> File Name: master.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月02日 星期五 11时31分40秒
 ************************************************************************/

#include <stdio.h>
#include "master.h"

int main() {
    //从文件中将配置信息取出
    char *config = "./conf_master.log";
    long int timeout;
    char startIp[20] = {0}, endIp[20] = {0};
    //并发度,3个端口，心跳端口, 数据请求端口，等待加入端口
    int Ins, heartport, dataport, listenport;
    //每次需要清空
    char reval[20] = {0};
    get_conf_value(config, "INS", reval);
    Ins = atoi(reval);
    memset(reval, 0, sizeof(reval));

    get_conf_value(config, "HeartPort", reval);
    heartport = atoi(reval);
    memset(reval, 0, sizeof(reval));
    
    get_conf_value(config, "DataPort", reval);
    dataport = atoi(reval);
    memset(reval, 0, sizeof(reval));
    
    get_conf_value(config, "ListenPort", reval);
    listenport = atoi(reval);
    memset(reval, 0, sizeof(reval));
    
    get_conf_value(config, "StartIp", reval);
    strcpy(startIp, reval);
    memset(reval, 0, sizeof(reval));

    get_conf_value(config, "EndIp", reval);
    strcpy(endIp, reval);
    memset(reval, 0, sizeof(reval));
    
    get_conf_value(config, "TimeOut", reval);
    timeout = atol(reval);
    memset(reval, 0, sizeof(reval));
    
    LinkList *linklist = (LinkList *)malloc(sizeof(LinkList) * Ins);
    //每个链表维护的人数
    int *sum = (int *)malloc(sizeof(int) * Ins);
    //初始化人加入链表中
    memset(sum, 0, sizeof(int) * Ins);
    //初始化的虚拟头结点
    struct sockaddr_in initaddr;
    initaddr.sin_family = AF_INET;
    initaddr.sin_port = htons(0);
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    //给每条链表初始化虚拟头结点
    for (int i = 0; i < Ins; i++) {
        Node *p = (Node *)malloc(sizeof(Node));
        p->addr = initaddr;
        p->fd = -1;
        p->next = NULL;
        //将结点p插入第i个链表
        linklist[i] = p;
    }
    
    unsigned int sip, eip;//比有符号的大两倍
    sip = ntohl(inet_addr(startIp));
    eip = ntohl(inet_addr(endIp));
    //.0 .255是不被用户自己使用的
    for (unsigned int i = sip; i <= eip; i++) {
        if (i % 256 == 0 || i % 256 == 255) continue;
        initaddr.sin_addr.s_addr = htonl(i);
        initaddr.sin_port = htons(heartport);
        Node *p = (Node *)malloc(sizeof(Node));
        p->addr = initaddr;
        p->fd = -1;
        p->next = NULL;
        int sub = find_min(sum, Ins);
        insert(linklist[sub], p);
        sum[sub] += 1;
    }
    //将每条链的信息打印出来
    for (int i = 0; i < Ins; i++) {
        printf("<%d>\n", i);
        output(linklist[i]);
    }
    
    
    pthread_t pth_heart, pth_data[Ins];
    struct Heart heart;
    heart.ins = Ins;
    heart.linklist = linklist;
    heart.sum = sum;
    heart.timeout = timeout;
    //主动心跳，将heart作为参数传给do_heart函数
    pthread_create(&pth_heart, NULL, do_heart, (void *)&heart);
   
    //请求数据
    struct Data darg[Ins];
    for (int i = 0; i < Ins; i++) {
        darg[i].head = linklist[i];//第i条链表的头结点地址
        darg[i].ind = i;//第i条链表
        darg[i].port = dataport;//数据请求端口
        pthread_create(&pth_data[i], NULL, do_data, (void *)&darg[i]);
    }
    int listenfd = socket_create(listenport);
    listen_epoll(listenfd, linklist, sum, Ins, heartport);
    
    //等待do_heart执行接束
    pthread_join(pth_heart, NULL);

    return 0;
}
