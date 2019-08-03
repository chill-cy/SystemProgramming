/*************************************************************************
	> File Name: until.h
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年08月02日 星期五 10时37分02秒
 ************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/file.h>




#ifndef _UNTIL_H
#define _UNTIL_H
int get_conf_value(char *file, char *key, char *value);

int socket_create(int port);
int socket_connect(int port, char *ip);





#endif
