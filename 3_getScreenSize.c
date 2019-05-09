/*************************************************************************
	> File Name: ls-al.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年05月07日 星期二 21时43分42秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ioctl.h>

void getsize() {
    struct winsize size;  
    if (isatty(STDOUT_FILENO) == 0)  
        exit(1);  
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &size)<0) 
    {
        perror("ioctl TIOCGWINSZ error");
        exit(1);
    } 

    printf("%d rows, %d columns\n", size.ws_row, size.ws_col);  
    return ; 
}
int main(void) 
{
    getsize();
    return 0; 
}
