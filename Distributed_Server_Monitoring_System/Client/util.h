/*************************************************************************
	> File Name: common.h
	> Author: victoria 
	> Mail: 1105847344@qq.com
	> Created Time: 2019年07月18日 星期四 20时19分39秒
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
int get_conf_value(char *file, char *key, char *value);
char *my_ntoa(struct in_addr in);
int socket_create(int port);
int socket_connect(int port, char *host);

#endif
