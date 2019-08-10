/*************************************************************************
	> File Name: log.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月06日 星期二 16时47分51秒
 ************************************************************************/
int write_log(char *pathname, const char *format, ...) {
    FILE *fp = fopen(pathname, "a");
    flock(fp->_fileno, LOCK_EX);
    va_list arg;
    int ret;
    va_start(arg, format);
    time_t log_t = time(NULL);
    struct tm *tm_log = localtime(&log_t);
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d :", tm_log->tm_year + 1900, tm_log->tm_mon + 1, tm_log->tm_mday, tm_log->tm_hour, tm_log->tm_min, tm_log->tm_sec);
    ret = vfprintf(fp, format, arg);
    fprintf(fp, "\n");
    va_end(arg);
    fflush(fp);
    fclose(fp);
    return ret;
}
//线程虚拟处理器，认为自己独占了一个cpu,
//多线程提高并发，抽象模块
//所有进程用同一个始终，略微大于计时时间
//状态机，事件驱动
//多线程避免io阻塞，上下文切换，
//多线程天然共享内存，多进程可以提高响应速度，即使一个核心，把一个进程份成四个核心可以被更快的调用到，
//缺点：有临界区，资源竞争，导致数据出错，需要加锁，容易造成死锁，引起的原因：循环等待，资源分配不当，再加互斥琐时以同样的顺序去加，
//io复用epoll解决io并发，异步io, 提高并发度
//进程是怎么产生的，由一个进程复制而来，用exec替换，
//线程模型，协同程序，协程是c , c++ 的子程序，控制执行流程。
//本质上是一个子程序，协程每一个连接对应一个线程，函数编程的一个抽象，
//并发时间间隔，并行同一时刻多人
//竞争两个或多个可以对文件访问，写，临界区是代码段
//线程加互斥锁：同时只有一个人持有，
//线程安全：线程封闭：，线程同步：加锁。
//线程创建，连接，线程分离，
//进程kill发信号，
//父进程等待连接结束，主线程全部，
//pthread_ioin.cp,
//同步，


int write_log(char *pathname, const char *format, ...) {
    FILE *fp = fopen(pathname, "a");
    flock(fp->_fileno, LOCK_EX);
    va_list arg;
    int ret;
    va_start(arg, format);
    time_t log_t = time(NULL);
    struct tm *tm_log = localtime(&log_t);
    fprintf()
}
