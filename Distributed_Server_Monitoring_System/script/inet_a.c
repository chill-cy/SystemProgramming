/*************************************************************************
	> File Name: inet_a.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月18日 星期四 21时04分45秒
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>

char *my_inet_ntoa(struct in_addr in) {
    static char ip[20] = {0};
    char *p;
    p = (char *)&in.s_addr;
    sprintf(ip, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return ip;
}
int main(int argc, char **argv) {
    struct in_addr in;
    char *ip = argv[1];
    //将点分十进制ip转化为网络字节序
    in = inet_addr("192.168.1.32");
    printf("%d --> ", in.s_addr);
    printf("%s\n", my_inet_ntoa(in));
    return 0;
}
