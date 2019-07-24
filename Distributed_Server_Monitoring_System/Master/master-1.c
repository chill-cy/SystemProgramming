/*************************************************************************
	> File Name: master-1.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月24日 星期三 14时19分40秒
 ************************************************************************/

#include "master.h"

typedef struct Node {
    struct sockaddr_in client_addr;
    int node_sock;
    struct Node *next;
} Node, *LinkedList;



int main() {
    char *config = "./pihealthd.conf";
    int Port, Ins, CHPort;
    char FromIp[20] = {0};
    char ToIp[20] = {0};
    char tmp[20] = {0};

    get_conf_value(config, "INS", tmp);
    ins = atoi(tmp);
    get_conf_value(config, "From", tmp);
    strcpy(FromIp, tmp);
    get_conf_value(config, "To", tmp);
    strcpy(ToIp, tmp);
    get_conf_value(config, "clientHeart", tmp);
    CHPort = atoi(tmp);
    get_conf_value(config, "MasterPort", tmp);
    Port = atoi(tmp);
    //创建Ins个链表的头指针,每个线程有一个链表，希望知道每个链表的成员个数，需要一个sum数组与ins对应
    LinkedList *linkedlist = (LinkedList *)malloc(Ins * sizeof(LinkedList));
    int *sum = (int *)malloc(Ins * sizeof(int));
    memset(sum, 0, Ins * sizeof(int));
    
    struct sockaddr_in initaddr;
    initaddr.sin_family = AF_INET;
    initaddr.sin_port = htons(CHPort);
    initaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int port = inet_addr(ToIp);
    if (inet_addr << 8 == 0) continue;
    f
        
    return 0;
}
