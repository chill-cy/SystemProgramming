/*************************************************************************
	> File Name: master.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月20日 星期六 20时18分01秒
 ************************************************************************/

#include <stdio.h>

int main(int argc, char **argv) {//argc参数总个数，argv参数值；
    int socket_fd, port;//建立连接的返回值，端口号；
    char *ip_addr;//ip地址
    if (argc == 1) {
        ip_addr = "192.168.1.74";
        port = 8888;
    } else if (argc == 3) {//参数有三个，argv[1]是地址，argv[2]是端口号；
        ip_addr = argv[1];
        port = atoi(argv[2]);//字符串转换成数字；
    } else {
        printf("Use : ./master ip_addr port\n");
    }
    socket_fd = socket_connect(port, ip_addr);
    if (socket_fd < 0) {
        printf("socket_fd error\n");                                 
    }
    while(1) {
        int retcode, pid;
        if ((retcode = recv_response(socket_fd)) <= 0) {
            break;
        }
        printf("retcode: %d\n", retcode);
        pid = fork();
        if (pid == 0) {//pid = 0是子进程，!=0是父进程
            int socket_data = socket_connect(port + 1, ip_addr);//短连接
            if (socket_data < 0) {
                printf("socket_data error\n");
                exit(1);
            }
         //   printf("send_response\n");
            send_response(socket_fd, 200);//发送确认码，可以开始发送信息了；
            printf("send_response\n");
            master_get(socket_data, retcode, ip_addr);//接收文件信息；
            close(socket_data);
            exit(0);
        } else {
            wait(NULL);
            send_response(socket_fd, 201);
            //本次文件接收结束，可以进行下次连接（子进程连接，发下一次文件）
            printf("%d", );
        }
    }
    return 0;
}
