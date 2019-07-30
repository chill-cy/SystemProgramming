/*************************************************************************
	> File Name: shm2.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月30日 星期二 11时03分53秒
 ************************************************************************/
#include <iostream>
#include <algorithm>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

using namespace std;
struct Share{
    int index;
    char str[1024];
};


int main () {
    pid_t pid;
    struct Share *shareaddr = NULL;
    int shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT | 0666);
    if (shmid < 0) cout << "failed 1" << endl;
    shareaddr = (struct Share*)shmat(shmid, NULL, 0);
    if (shareaddr == (void*)-1) cout << "failed 2" << endl;
    shareaddr->index = 0;
    pid = fork();
    if (pid > 0) {
        while (1) {
            if (shareaddr->index) {
                shareaddr->index = 0;
                printf("子进程：%s\n", shareaddr->str);
            } else {
                usleep(20000);
            }
        }
        shmdt((void*)shareaddr);
    } else if (pid == 0) {
        while(1){
            char buff[50]={0};
            scanf("%s", buff);
            shareaddr->index = 1;
            strcpy(shareaddr->str, buff);
        }
        exit(0);
    }
    wait(&pid);
    return 0;
}

