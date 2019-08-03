/*************************************************************************
	> File Name: master.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年08月02日 星期五 11时32分02秒
 ************************************************************************/

#include <stdio.h>
#include "master.h"


int main() {
    
    char *config = "./conf_master.log";
    
    char startIp[20] = {0};
    char endIp[20] ={0};
    int Ins, heartport, dataport, listenport;
    long timeout;
    char reval[20] = {0};

    get_conf_value(config, "INS", reval);
    Ins = atoi(reval);
    memset(reval, 0, sizeof(reval));
//printf("111\n");
    get_conf_value(config, "HeartPort", reval);
    heartport = atoi(reval);
    memset(reval, 0, sizeof(reval));
//printf("222\n");

    get_conf_value(config, "DataPort", reval);
    dataport = atoi(reval);
    memset(reval, 0, sizeof(reval));
//printf("333\n");

    get_conf_value(config, "ListenPort", reval);
    listenport = atoi(reval);
//printf("%d  ++++\n ", listenport);
    memset(reval, 0, sizeof(reval));

    get_conf_value(config, "StartIp", reval);
    strcpy(startIp, reval);
//printf("%s   ---\n",startIp);
    memset(reval, 0, sizeof(reval));
    
    get_conf_value(config, "EndIp", reval); 
    strcpy(endIp, reval);
    memset(reval, 0, sizeof(reval));
//printf("%s ....\n", endIp);
    
    get_conf_value(config, "TimeOut", reval);
    timeout = atol(reval);
//printf("%d  ++++\n ", listenport);
    memset(reval, 0, sizeof(reval));

    printf("%d--%d--%d---%d\n%s\n%s\n", Ins, heartport, dataport, listenport, startIp, endIp);

    LinkList *linklist =(LinkList *)malloc(sizeof(LinkList) * Ins);//表头数组
    int *sum = (int *)malloc(sizeof(int) * Ins);
    memset(sum, 0, sizeof(sum));

    struct sockaddr_in initaddr;

    initaddr.sin_family = AF_INET;
    initaddr.sin_port = htons(0);
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

//printf("666\n" );
//初始化链表头结点
    for(int i = 0; i < Ins; i++) {
        Node *p = (Node *)malloc(sizeof(Node));
        p->addr = initaddr;
        p->fd = -1;
        p->next = NULL;
        linklist[i] = p;
    }
    
//printf("7777\n" );
//循环ip
    unsigned int sip, eip;
    sip = ntohl(inet_addr(startIp));
    eip = ntohl(inet_addr(endIp));
    
//printf("888\n" );
//printf("%d %d\n", sip, eip );
//printf("9999\n");
    for (unsigned int i = sip; i <= eip; i++) {
        if(i % 256 == 0 || i % 256 == 255) continue;           
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

    for (int i = 0; i < Ins; i++) {
        printf("<%d>\n", i);
        output((linklist[i]));
    }

    //心跳线程
    pthread_t pth_heart,pth_data[Ins];
    struct Heart heart;
    heart.sum = sum;
    heart.linklist = linklist;
    heart.ins = Ins;
    heart.timeout = timeout;
    pthread_create(&pth_heart, NULL, do_heart, (void *)&heart);
    struct Data darg[Ins];
    
    for (int i = 0; i < Ins; i++) {
        darg[i].head = linklist[i];
        darg[i].ind = i;
        darg[i].port = dataport;
        pthread_create(&pth_data[i], NULL, do_data, (void *)&darg[i]);
    }
     pthread_join(pth_heart, NULL);
   while(1){
       sleep(1);
   }
    return 0;
}
