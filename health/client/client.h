/*************************************************************************
	> File Name: client.h
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月02日 星期五 16时46分11秒
 ************************************************************************/

#include "util.h"
#include <sys/shm.h>
#include <sys/ipc.h>
#define BUFFSIZE 2048
#include <termios.h>
struct Share {
    int shareCnt;//自检次数，超过五次表示client掉线，
    pthread_mutex_t smutex;
    pthread_cond_t sready;//执行wait的触发条件，是由pthread_cont_sigal发出的，
};

//存所有脚本名
char basename[6][20] = {"cpu_info.sh", "disk_info.sh", "mem_info.sh", "user_info.sh", "SysInfo.sh", "enermy_pro.sh"};

char destname[6][20] = {"cpu.log", "disk.log", "mem.log", "user.log", "sys.log", "enermy.log"};

//存暂时的文件信息,存五次写入文件
char tmp_info[BUFFSIZE] = {0};
//内存占用率
double dyAver = 0;

void recv_heart(int port, struct Share *share);
void recv_data(int dataport, int ctlport, struct Share *share);
void to_load(char *ip, int loadPort, struct Share *share);
void do_check(int inx, struct Share *share, int cnt);
int do_bash(int inx, struct Share *msg, int cnt);//执行脚本,执行失败返回－１
//目标文件名，第几个线程
int do_bash(int inx, struct Share *msg, int cnt) {
    //执行脚本的命令
    char opstr[100] = {0};
    FILE *pfile = NULL;
    //拼接字符串，写入opstr
        //mem脚本需要传入参数dyAver
    if (inx == 2) {
        sprintf(opstr, "bash ./%s %lf", basename[inx], dyAver);
    } else {
        sprintf(opstr, "bash ./%s", basename[inx]);
    }
    //popen开个进程，替换成opstr操作
    pfile = popen(opstr, "r");
    //没有文件流入，读取文件失败
    if (!pfile) {
        perror("popen");
        return -1;
    }
    char buff[BUFFSIZE] = {0};
    //取mem脚本信息存入tmp_info
    if (inx == 2) {
        //只将倒数第一行有用信息存入tmp_info
        if (fgets(buff, BUFFSIZE, pfile) != NULL) {
            strcat(tmp_info, buff);
        }
        //取出脚本中最后一行，更新动态平均值
        if (fgets(buff, BUFFSIZE, pfile) != NULL) {
            dyAver = atof(buff);
        }
    } else {
        //从pfile中取BUFFSIZE存入buff;如果遇到换行fgets获取一行
        while (fgets(buff, BUFFSIZE, pfile) != NULL) {
            //将一次取出的数据，存入tmp_info;
            strcat(tmp_info, buff);
            //memset(buff, 0, sizeof(buff));
        }
    }
    pclose(pfile);
    
    //向目标文件中写入
    if (cnt == 5) {
        char dest[100] = {0};
        sprintf(dest, "./%s", destname[inx]);
        FILE *fw = fopen(dest, "a+");
        //写失败
        if (!fw) {
            perror("fopen");
            return -1;
        }
        //防止写文件时有人读
        flock(fw->_fileno, LOCK_EX);
        fprintf(fw, "%s", tmp_info);
        //关闭文件描述符自动解锁
        fclose(fw);
        memset(tmp_info, 0, sizeof(tmp_info));
    }
    return 0;
}
void do_check(int inx, struct Share *share, int cnt) {
    do_bash(inx, share, cnt);

    if (inx == 0) {
        pthread_mutex_lock(&share->smutex);
        if (share->shareCnt == 5) {
            pthread_mutex_unlock(&share->smutex);
            return ;
        }  
        share->shareCnt += 1;
        //自检五次断线重连
        if (share->shareCnt >= 5) {
            pthread_cond_signal(&share->sready);
        }
        pthread_mutex_unlock(&share->smutex);
    }
    
}

void to_load(char *ip, int loadPort, struct Share* share) {
    printf("\nclient heart start\n");
    int stime = 

}

void recv_data(int dataport, int ctlport, struct Share *share) {
    int listenfd = socket_create(ctlport);
    if (listenfd < 0) {
        perror("socket_create");
        return ;
    }
    while (1) {
        int newfd;
        //控制端口的文件描述符
        newfd = accept(listenfd, NULL, NULL);
        if (newfd < 0) {
            perror("accept");
            continue;
        }
        //２接收master要请求的脚本
        for (int i = 0; i < 6; i++) {
            //存对方发回的码
            int fno = -1;
            int ret = recv(newfd, &fno, sizeof(int), 0);
            if (ret <= 0) {
                perror("recv");
                break;
            }
            char path[50] = {0};
            sprintf(path, "./%s", destname[fno - 100]);
            int ack = 0;
            if (access(path, F_OK) < 0) {
                send(newfd, &ack, sizeof(int), 0);
                continue;
            }
            //创建数据端口
            int sendfd = socket_create(dataport);
            if (sendfd < 0) {
                ack = 0;
                send(newfd, &ack, sizeof(int), 0);
                continue;
            }
            ack = 1;
            //文件存在准备发送文件
            send(newfd, &ack, sizeof(int), 0);
            
            int nsendfd = accept(sendfd, NULL, NULL);
            if (nsendfd < 0) {
                perror("nsendfd");
                close(sendfd);
                continue;
            }

            FILE *fp = fopen(path, "r");
            flock(fp->_fileno, LOCK_EX);
            //读一行发一行
            char buff[BUFFSIZE] = {0};
            while (fgets(buff, BUFFSIZE, fp) != NULL) {
                send(nsendfd, buff, strlen(buff), 0);
                memset(buff, 0, sizeof(buff));
            }
            fclose(fp);
            close(nsendfd);
            close(sendfd);
            //将本地文件删除
            remove(path);
        }
        close(newfd);
        //每接收一次数据cnt清零
        pthread_mutex_lock(&share->smutex);
        share->shareCnt = 0;
        pthread_mutex_unlock(&share->smutex);
    }
    close(listenfd);
}

/*void recv_data(int port, struct Share *share) {
    int listenfd;
    //无法绑定端口，根本错误
    if ((listenfd = socket_create(port)) < 0) {
        perror("Error socket_create on listenfd");
        return ;
    }
    while (1) {
        //可能对方网络问题
        int newfd = accept(listenfd, NULL, NULL);
        if (newfd < 0) {
            perror("accept");
            continue;
        }
        char buff[BUFFSIZE] = {0};

        int k = recv(newfd, buff, sizeof(buff), 0);
        //数据传输失败
        if (k < 0) {
            perror("recv");
            close(newfd);
            continue;
        }
        printf("\n%s\n", buff);
        strcpy(buff, "bye");
        send(newfd, buff, strlen(buff), 0);
        close(newfd);
        pthread_mutex_lock(&share->smutex);
        share->shareCnt = 0;
        pthread_mutex_unlock(&share->smutex);
    }
    close(listenfd);
}
*/
void recv_heart(int port, struct Share *share) {
    int sockfd;
    //获取已经是监听状态的套接字
    sockfd = socket_create(port);
    if (sockfd < 0) {
        perror("Error bind on heartPort");
        return ;
    }
    while (1) {
        int newfd = accept(sockfd, NULL, NULL);
        //一次心跳
        printf(" ❤ ");
        pthread_mutex_lock(&share->smutex);
        share->shareCnt = 0;
        pthread_mutex_unlock(&share->smutex);
        fflush(stdout);
        close(newfd);
    }
    close(sockfd);
}
