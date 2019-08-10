/*************************************************************************
	> File Name: client.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月02日 星期五 16时45分17秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include "client.h"

int main() {
    int heartPort, dataPort, loadPort, ctlPort;    
    char tmp[20] = {0};
    char *config = "./client.conf";
    char ip[20] = {0};

    get_conf_value(config, "IP", tmp);
    strcpy(ip, tmp);
    memset(tmp, 0, sizeof(tmp));

    get_conf_value(config, "HeartPort", tmp);
    heartPort = atoi(tmp);
    memset(tmp, 0, sizeof(tmp));

    get_conf_value(config, "DataPort", tmp);
    dataPort = atoi(tmp);
    memset(tmp, 0, sizeof(tmp));

    get_conf_value(config, "LoadPort", tmp);
    loadPort = atoi(tmp);
    memset(tmp, 0, sizeof(tmp));

    get_conf_value(config, "CtlPort", tmp);
    ctlPort = atoi(tmp);
    memset(tmp, 0, sizeof(tmp));

    //配置锁参数，进程可共享这个锁,互斥锁
    pthread_mutexattr_t mattr;//一条性质
    //条件变量的一个性质
    pthread_condattr_t cattr;
    //初始化条件变量
    pthread_mutexattr_init(&mattr);
    //初始化锁
    pthread_condattr_init(&cattr);
    //设置锁和共享内存的性质,宏，进程之间可以用同一个锁
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);

    struct Share *share = NULL;
    //设置共享内存,申请一块子孙共用的内存空间
    int shmid = shmget(IPC_PRIVATE, sizeof(struct Share), IPC_CREAT | 0666);
    //申请失败
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    //根据共享内存的地址，定位内存id, 路径
    share = (struct Share *)shmat(shmid, NULL, 0);
    if (share == (void *)-1) {
        perror("shmat");
        exit(1);
    }
    //给锁初始化, 将给定的属性赋给share->smutex
    pthread_mutex_init(&share->smutex, &mattr);
    pthread_cond_init(&(share->sready), &cattr);
    
    pid_t pid;
    pid = fork();
    if (pid > 0) {
        //返回服务端发来的数据请求
        recv_data(dataPort, ctlPort, share);
        wait(&pid);
        //保证其正常退出，让其上一层为其收尸
    } else {
        pid_t pid_1 = fork();
        //监听心跳
        if (pid_1 > 0) {
            recv_heart(heartPort, share);
            wait(&pid_1);
            exit(0);
        } else if (pid_1 == 0){
            pid_t pid_2;
            int inx;
            for (int i = 0; i < 6; i++) {
                inx = i;
                pid_2 = fork();
                if (pid_2 == 0) break;
            }
            if (pid_2 > 0) {
                while(1) {
                    //条件触发，to_load只执行一次上线,
                    //先加锁,只保护条件变量wait//需要等待的条件变量和要加回的锁
                    pthread_mutex_lock(&share->smutex);
                    pthread_cond_wait(&share->sready, &share->smutex);
                    pthread_mutex_unlock(&share->smutex);
                    //主动心跳时需要判断share>cnt是否小于５，如果小于服务端已经将其加入列表：
                    to_load(ip, loadPort, share);
                    //连接成功
                    pthread_mutex_unlock(&share->smutex);
                    share->shareCnt = 0;
                    pthread_mutex_unlock(&share->smutex);         
                }
                //父为子收尸
                for (int i = 0; i < 6; i++) {
                    wait(&pid_2);
                }
                exit(0);
            } else if (pid_2 == 0) {
                //自检五次写入文件一次
                //看脚本执行了几次
                int cnt = 0;
                while (1) {
                    cnt += 1;
                    //第几个进程
                    do_check(inx, share, cnt); 
                    if (cnt == 5) {
                        cnt = 0;
                    }
                    sleep(2);
                }
                exit(0);
            }
        }
    }
    return 0;
}
