/*************************************************************************
	> File Name: inot.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月24日 星期三 15时18分11秒
 ************************************************************************/
#include<stdio.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <arpa/inet.h>


char *my_inet_ntoa(struct in_addr in) {
    char *i = (char *)&(in.s_addr);
    printf("%u.%u.%u.%u\n", i[0], i[1], i[2], i[3]);

}

int main() {
    struct in_addr in;
    in.s_addr = inet_addr("192.168.1.33");
    char *ans = my_inet_ntoa(in);

    return 0;
}

