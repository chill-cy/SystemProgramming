/*************************************************************************
	> File Name: shm1.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月30日 星期二 09时54分07秒
 ************************************************************************/

#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

struct sm_msg{
    char buff[1024];
    pthread_mutex_t sm_mutex;
    pthread_cond_t sm_ready;
};

int main() {
    int shmid, pid;
    char *share_memory = NULL;

    pthread_mutexattr_t m_attr;
    pthread_condattr_t c_attr;

    pthread_mutexattr_init(&m_attr);
    pthread_condattr_init(&c_attr);

    pthread_mutexattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&c_attr, PTHREAD_PROCESS_SHARED);


   // if ((shmid = shmget(IPC_PRIVATE, sizeof(struct sm_msg), 0666|IPC_CREAT)) == -1) {
   //   perror("shmget");
   //     exit(1);
   // }

   // if ((share_memory = shmat(shmid, 0, 0)) == NULL) {
   //     perror("shmat");
   //   exit(1);
   // }
    
    char *p = mmap(NULL, sizeof(struct sm_msg), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    
    //munmap(p, sizeof(struct sm_msg));

    struct sm_msg *msg = (struct sm_msg *)p;

    pthread_mutex_init(&(msg->sm_mutex), &m_attr);
    pthread_cond_init(&msg->sm_ready, &c_attr);

    //char *buff = share_memory;
    pid = fork();
    if(pid < 0) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0) {
        printf("In child: ");
        while (1) {
            scanf("%s", msg->buff);
            pthread_mutex_lock(&msg->sm_mutex);
            pthread_cond_signal(&msg->sm_ready);
            pthread_mutex_unlock(&msg->sm_mutex);
        }
    }
    if (pid > 0) {
        while (1) {
            pthread_mutex_lock(&msg->sm_mutex);
            pthread_cond_wait(&msg->sm_ready, &msg->sm_mutex);
            printf("In father -> %s \n", msg->buff);
            memset(msg->buff, 0, sizeof(msg->buff));
            printf("In child: ");
            fflush(stdout);
            pthread_mutex_unlock(&msg->sm_mutex);
        }
        wait(&pid);
    }
    return 0;
}
