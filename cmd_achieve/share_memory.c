/*************************************************************************
	> File Name: 共享内存.cpp
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月28日 星期日 18时35分58秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
 
struct shareMemory{
 
    int is_write;
    char data[20];
};
 
int main(int argc, const char *argv[])
{
    key_t  key_n ;
    int    shm_id;
    pid_t  pid;
    int running = 1;   //决定是否继续运行的变量
    void *shm = NULL;  //共享内存的首地址
    struct shareMemory *shared;
    char buf[1024+1];
 
    if((key_n = ftok("/",'s')<0))  //生成共享内存的key值
    {
        perror("Fail to ftok");
        exit(EXIT_SUCCESS);
    }
    printf("key num:%d\n",key_n);
 
    if((shm_id = shmget(key_n,1024,0666|IPC_CREAT))==-1)  //创建共享内存
    {
        perror("Fail to shmget");
        exit(EXIT_SUCCESS);
    }
    printf("share memory id:%d\n",shm_id);
 
    if((pid = fork())==-1)
    {
        perror("Fail to fork");
        exit(EXIT_SUCCESS);
    }
 
    if(pid==0)                                         //子进程，用来读
    {
        printf("I is son process!\n");
        shm = shmat(shm_id,0,0);                     //将共享内存连接到当前进程的地址空间
        if(shm==(void *)-1)
        {
            perror("Fail to shmat");
            exit(EXIT_SUCCESS);
        }
        printf(" son Memory attached at %x\n",(int)shm);
 
        shared = (struct shareMemory *)shm;        //设置共享内存
        shared->is_write = 0;
 
        while(running)  //读取共享内存中的数据
        {
            if(shared->is_write!=0)
            {
                printf("YOu worte:%s\n",shared->data);
                sleep(rand()%3);
                shared->is_write = 0;
 
                if(strncmp(shared->data,"end",3)==0)  //输入end退出循环
                {
                    running = 0;
                }
                else      //有其他进程在写数据，不能读
                    sleep(1);
            }
        }
        if(shmdt(shm)==-1)   //把共享内存从当前进程分离
        {
            perror("Fail to shmdt");
            exit(EXIT_SUCCESS);
        }
 
        if(shmctl(shm_id,IPC_RMID,0)==-1)      //删除这块共享内存
        {
            perror("Fail to shmctl(IPC_RMID)");
            exit(EXIT_SUCCESS);
        }
        exit(EXIT_SUCCESS);
    }
 
    if(pid!=0)                                        //父进程，用来写
    {
 
        printf("I is father process:%d\n",pid);
        shm = shmat(shm_id,0,0);                     //将共享内存连接到当前进程的地址空间
        if(shm==(void *)-1)
        {
            perror("Fail to shmat");
            exit(EXIT_SUCCESS);
        }
        printf("father Memory attached at %x\n",(int)shm);
 
        shared = (struct shareMemory *)shm;        //设置共享内存
        shared->is_write = 0;
 
        while(running)  //向共享内存中写数据
        {
            while(shared->is_write==1)
            {
                printf("wait...\n");
                sleep(1);
            }
            //向共享内存中写入数据
            printf("input some data:\n");
            fgets(buf,1024,stdin);
            strncpy(shared->data,buf,1024);
            shared->is_write = 1;        //写完数据后，置1使可读
 
            if(strncmp(shared->data,"end",3)==0)
            {
                running = 0;
            }
        } 
 
        if(shmdt(shm)==-1)   //把共享内存从这块进程分离
        {
            perror("Fail to shmdt");
            exit(EXIT_SUCCESS);
        }
        sleep(2);
        exit(EXIT_SUCCESS);
    }
 
    return 0;
}
