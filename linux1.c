/*************************************************************************
	> File Name: linux1.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年04月23日 星期二 21时26分54秒
 ************************************************************************/

#include <stdio.h>
#include <pwd.h> //getpwuid的头文件
#include <unistd.h> // getuid,gethostname,getcwd的头文件

int main(){
    struct passwd *pwd; // passwd结构体描述了/etc/passwd的文件记录行 包括用户名 密码 用户 用户组ID 真实姓名 用户主目录 和默认Shell 
    pwd = getpwuid(getuid()); 
    // 根据传入的用户ID返回指向passwd的结构体 该结构体初始化了里面的所有成员
    // getuid()用来取得执行目前进程的用户识别码。
    char hostname[20];
    char dir[200];
    gethostname(hostname, sizeof(hostname));
    getcwd(dir, sizeof(dir));
    printf("\033[1;35m%s@%s\033[0m:\033[1;36m%s\033[0m\n", pwd->pw_name, hostname, dir);
    return 0;
}
