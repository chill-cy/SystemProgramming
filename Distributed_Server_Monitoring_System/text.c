/*************************************************************************
	> File Name: text.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年08月06日 星期二 16时52分53秒
 ************************************************************************/

#include <stdio.h>

int main() {
    FILE *fp = fopen(pathname, "a");
    flock(fp->_fileno, LOCK_EX);
    va_list arg;
    int ret;
    va_start(arg, format);
    time_t log_t = time(NULL);
    struct tm *tm_log = localtime(&log_t);
    fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d :", tm_log-tm_year + 1900, tm_log->tm_mon + 1, tm_log->tm_mday, tm_log->tm_hour, tm_log->tm_min, tm_log->tm_sec);
    
    return 0;
}
