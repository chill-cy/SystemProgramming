/*************************************************************************
	> File Name: client.h
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月24日 星期三 19时28分46秒
 ************************************************************************/
#include "common.h"
#define BUFFSIZE 1024

int socket_connect(int port, char *host) {//建立连接传入参数为服务端端口，ip地址
  int sockfd;
  struct sockaddr_in dest_addr;
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {//创建自己的套接字
    perror("socket() error");
    return -1;
  }

  memset(&dest_addr, 0, sizeof(dest_addr));
 //将对面的信息存入结构体，传给服务端
  dest_addr.sin_family = AF_INET;   //地址族
  dest_addr.sin_port = htons(port);  //端口转为网络字节序（htons）host to net short将本机字节序转化成网络字节序，大小端系统不同
  dest_addr.sin_addr.s_addr = inet_addr(host);//ipv4 32位无符号整型(网络字节序) inet_addr将ip转换成整形值

  //DBG("Connetion TO %s:%d\n",host,port);
  //fflush(stdout);
//建立连接，中间的参数取dest_addr地址，结构体大小
  if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
    //perror("connect() error");
    //DBG("connect() error : %s!\n", strerror(errno));
    return -1;
  }
  return sockfd;  //已经建立好连接的套接字

}
