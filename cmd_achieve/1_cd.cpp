/*************************************************************************
	> File Name: 001.c
	> Author:zhengyang 
	> Mail:1021606521@qq.com 
	> Created Time: 2019年04月25日 星期四 11时49分21秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include<unistd.h>
#include<pwd.h>
#include<math.h>
#include<string.h>
#include<signal.h>
#define PATH_SIZE 100

char buf1[80];
char buf[80];//存放路径
struct passwd *pwd;
char hostname[60] = {'\0'};
int home_length;

void  cd_command() {

    char str1[10];
    char path[PATH_SIZE];
    int res;
    memset(path, '\0', PATH_SIZE); 
    //初始化path数组

    while(~ scanf("%s", str1)) {
        if (!strcmp(str1,"exit")) break;
        scanf(" %s", path);
        if (!strcmp(str1, "cd")) {
             res =  chdir(path);
            if(res != 0)
               printf("%s is not a path,please check again\n", path);
         }
        else  printf("%s is not a command,please check\n", str1);

    getcwd(buf, sizeof(buf)); //把当前工作的绝对路径 复制到buf的数组里 
    if(strlen(buf) < home_length) 
    printf("\033[34m%s@%s:%s\n\033[0m",pwd->pw_name,hostname,buf);
    else printf("\033[33m%s@%s:~%s\n\033[0m", pwd->pw_name, hostname, buf + home_length);
         
   }
}

void get_password() {
    
   struct passwd *qwe;
    qwe = getpwuid(getuid());
    printf("%s\n", qwe->pw_passwd);                                       //获取密码
}


void get_username_hostname_path() {  

    gethostname(hostname, sizeof(hostname));                             //获取主机名
    pwd = getpwuid(getuid());                                            //先用getuid()获取id号  通过 getwuid()函数通过uid 查看用户的passwd数据  获取用户名
    getcwd(buf, sizeof(buf)); //把当前工作的绝对路径 复制到buf的数组里 
    if(strlen(buf) < home_length) 
    printf("\033[31m%s@%s:%s\n\033[0m", pwd->pw_name, hostname, buf);
    else printf("\033[31m%s@%s:~%s\n\033[0m", pwd->pw_name, hostname, buf + home_length);
}



int main(){
    home_length = strlen(getcwd(buf1, sizeof(buf1)));
    signal(SIGINT, SIG_IGN);
    get_username_hostname_path();
    cd_command();
    return 0;
}
