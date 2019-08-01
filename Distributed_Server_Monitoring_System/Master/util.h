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
#ifndef _UNTIL_H
#define _UNTIL_H
int get_conf_value(char *file, char *key, char *value);
char *my_ntoa(struct in_addr in);
int socket_create(int port);
int socket_connect(int port, char *host);
int socket_connect_ioc(int port, char *host, int sec, double usec);
#endif
