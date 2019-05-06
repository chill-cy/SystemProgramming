/*************************************************************************
	> File Name: 实现ll.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年05月06日 星期一 13时57分10秒
 ************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#define PATH "./" //做一个宏定义，在预编译阶段，让PATH代替"./"   //这里可以定一个输入参数，就可以指定目录文件了
//​
int main(void)
{
    DIR *p_dir = NULL; //定义一个DIR*型指针p_dir 并初始化为空
    p_dir = opendir(PATH); //指针p_dir指向函数opendir(PATH)
    if (NULL == p_dir) //判断是否指向成功
    {
        perror("opendir");
        exit(0);
    }
    char *p_name;//定义一个char*指针p_name，用来存储目录下文件名
    struct dirent *d_dir = NULL;//定义一个readdir函数结构体指针d_dir，并初始化为空
    struct stat st; //定义一个stat函数的对象st
    memset(&st,0,sizeof(st));//清空对象st内存中的垃圾数据
    while (d_dir = readdir(p_dir))
    //while遍历PATH目录下所有文件，并且d_dir指针绑定readdir(p_dir)函数。注意：每赋值一次，指针跳转一次，这是文件指针
    {
        if ('.' == *(d_dir->d_name))
        //判断是否为隐藏文件，如果是，则跳过
            continue;
        p_name = d_dir->d_name;
        //给p_name赋值遍历文件的文件名，这里右值是结构体对象访问结构体内成员d_name。参照man 3 readdir
    stat(p_name,&st);
        //使用函数stat绑定文件名和对象st
    int n_size,n_link;//定义n_size，n_link，用来存储之后访问到的结果
    n_link = st.st_nlink;
        //使用stat结构体对象st访问结构体内成员变量st_nlink，并赋值给n_link，stat结构体内st_link用来表示硬链接数
    n_size = st.st_size;
        //使用stat结构体对象st访问结构体内成员变量st_size，并赋值给n_size，stat结构体内st_size用来表示文件大小​
#if 1
    char n_dir;//定义一个char变量，用来存放文件类型标识
    if (S_ISREG(st.st_mode))//判断是否为文件类型，S_ISREG详情可以参照man 2 stat
        n_dir = '-';
    else if (S_ISDIR(st.st_mode))//判断是否为目录类型
        n_dir = 'd';
    else if (S_ISCHR(st.st_mode))//判断是否为chr类型
        n_dir = 'c';
    else if (S_ISBLK(st.st_mode))//判断是否为块设备类型
        n_dir = 'b';
    else if (S_ISFIFO(st.st_mode))//判断是否为管道类型
        n_dir = 'p';
    else if (S_ISLNK(st.st_mode))//判断是否为软链接类型
        n_dir = 'l';
    else if (S_ISSOCK(st.st_mode))//判断是否为套接字类型
        n_dir = 's';
    else
        n_dir = '?';//如果都不是，用?表示无法识别
#endif
#if 1
    char n_power[10] = {};//定义一个固定长度的char型数组n_power用来存放九个权限位
    if (S_IRUSR & st.st_mode)//判断用户位是否可读,S_IRUSR详情参照man 2 stat，st_mode位stat结构体中的成员变量
        n_power[0] = 'r';
    else
        n_power[0] = '-';
    if (S_IWUSR & st.st_mode)//判断用户位是否可写
        n_power[1] = 'w';
    else
        n_power[1] = '-';
    if (S_IXUSR & st.st_mode)//判断用户位是否可执行
        n_power[2] = 'x';
    else
        n_power[2] = '-';
    if (S_IRGRP & st.st_mode)//判断用户组位是否可读
        n_power[3] = 'r';
    else
        n_power[3] = '-';
    if (S_IWGRP & st.st_mode)//判断用户组位是否可写
        n_power[4] = 'w';
    else
        n_power[4] = '-';
    if (S_IXGRP & st.st_mode)//判断用户组位是否可执行
        n_power[5] = 'x';
    else
        n_power[5] = '-';
    if (S_IROTH & st.st_mode)//判断其他用户是否可读
        n_power[6] = 'r';
    else
        n_power[6] = '-';
    if (S_IWOTH & st.st_mode)//判断其他用户是否可写
        n_power[7] = 'w';
    else
        n_power[7] = '-';
    if (S_IXOTH & st.st_mode)//判断其他用户是否可执行
        n_power[8] = 'x';
    else
        n_power[8] = '-';
#endif
#if 1
    struct passwd *u_uid = NULL;//定义getpwuid结构体指针u_uid，并初始化为空
    u_uid = getpwuid(st.st_uid);//u_uid指针绑定函数getpwuid(st.st_uid)，st_st_uid是stat结构体对象st的成员变量
    if (NULL == u_uid)//判断u_uid指针是否指向成功
    {
        perror("getpwuid");
        exit(0);
    }
    char *g_uid = u_uid->pw_name;//定义char*型指针g_uid用来保存得到的用户名
#endif

#if 1
    struct group *gr_gid = NULL;//定义getgrgid结构体指针gr_gid,并初始化为空
    gr_gid = getgrgid(st.st_gid);//gr_gid指针绑定函数getgrgid(st.st_gid)，st.st_gid是stat结构体对象st的成员变量
    if (NULL == gr_gid)//判断gr_gid指针是否指向成功
    {
        perror("getgrgid");
        exit(0);
    }
    char *g_gid = gr_gid->gr_name;//定义char*型指针g_gid用来保存得到的组名
#endif

#if 1
    struct tm *local_time = NULL;//定义localtime结构体指针local_time，并初始化为空
    local_time = localtime(&st.st_ctime);//local_time指针绑定函数localtime(&st.st_ctime)，st.st_ctime是stat结构体对象st的成员变量

    if (NULL == local_time) //判断local_time指针是否绑定成功
    {
        perror("localtime");
        exit(0);
    }
    int n_num = local_time->tm_mon;
    //定义int型变量n_num用来保存local_time指针访问到的月份，注意：这里得到的月份是数字表示，且localtime函数得到的月份是0~11,不符合正常思维的1~12
    char *n_mon;//定义char*型变来嗯n_mon，用来转换数字月份到英文简写月份
    int n_day = local_time->tm_mday;//定义int型变来嗯n_day用来存放local_time指针访问到的日期
    int n_hour = local_time->tm_hour;//定义int型变来嗯n_hour用来存放local_time指针访问到的小时
    int n_min = local_time->tm_min;//定义int型变来嗯n_min用来存放local_time指针访问到的分钟
    switch (n_num)//利用switch-case语句用来转换数字月份到英文简写月份
    {
        case 0:
            n_mon = "Jan";
            break;
        case 1:
            n_mon = "Feb";
            break;
        case 2:
            n_mon = "Mar";
            break;
        case 3:
            n_mon = "Apr";
            break;
        case 4:
            n_mon = "May";
            break;
        case 5:
            n_mon = "Jun";
            break;
        case 6:
            n_mon = "Jul";
            break;
        case 7:
            n_mon = "Aug";
            break;
        case 8:
            n_mon = "Sep";
            break;
        case 9:
            n_mon = "Otc";
            break;
        case 10:
            n_mon = "Nov";
            break;
        case 11:
            n_mon = "Dec";
            break;
        default:
            break;
    }
#endif
    printf("%c%s %d %s %s %d \t %s %d %d:%d\t%s\n",n_dir,n_power,n_link,g_uid,g_gid,n_size,n_mon,n_day,n_hour,n_min,p_name);//输出需要的变量访问结果
    }
    return 0;
}
