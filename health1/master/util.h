/*************************************************************************
	> File Name: util.h
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月02日 星期五 10时37分00秒
 ************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>
#include <pwd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/file.h>


#ifndef _UTIL_H
#define _UTIL_H
//从文件中提取数据，获取文件中的值，获取哪个文件，关键字，传出参数value存值
int get_conf_value(char *file, char *key, char *value);
//创建绑定监听的套接字，端口号，ip
int socket_create(int port);
int socket_connect(int port, char *ip);
#endif
