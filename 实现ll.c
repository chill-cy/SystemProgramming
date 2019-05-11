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
/*
opendir函数
首先，应该打开目录，这里就使用到了opendir函数，属于c库函数。

打开目录之后，就要读取目录文件中所有文件，即使用readdir函数。readdir函数其实是制定了一个文件指针，指针每跳一次，就读取一个文件名。

这里要注意，每个目录文件中有隐藏文件，ll命令默认是不读取隐藏文件的，所以应该屏蔽之。

文件名的获取是通过readdir函数获取的。

使用opendir函数打开一个目录

[root@localhost c]# man opendir
使用到的头文件:

#include <sys/types.h>
#include <dirent.h>
函数体:

DIR *opendir(const char *name);
函数类型是一个DIR * 类型的指针,传参是一个输入形参数,char*类型
所以定义:

DIR *p_dir = NULL;
        p_dir = opendir(PATH);
        if (NULL == p_dir)  //如果打开失败,则直接退出程序
        {
                perror("opendir");
                exit(0);
        }
readdir函数
readdir函数解析:

[root@localhost day16]# man 3 readdir
用到的头文件:

#include <dirent.h>
函数体:

struct dirent *readdir(DIR *dirp);
//是一个结构体指针,传入参数为DIR*形指针,即目录指针
readdir函数中的结构体:

struct dirent {
               ino_t          d_ino;       /* inode number */
               off_t          d_off;       /* not an offset; see NOTES */
               unsigned short d_reclen;    /* length of this record */
               unsigned char  d_type;      /* type of file; not supported
                                              by all file system types */
               char           d_name[256]; /* filename */
           };
//注意:d_name为filename,即我们需要的文件名
则可以定义:

struct dirent *d_dir = NULL;
//d_dir = readdir(p_dir)就可以将d_dir函数结构体指针与函数绑定,使用while循环,每循环一次,指针跳动一次,即达到遍历目录下所有文件的效果.这里的指针是文件指针。
循环条件：

while (d_dir = readdir(p_dir)){
  ...
}
下来就可以进入到循环体内，进行一系列操作。

过滤掉两个隐藏目录文件
注意：目录内有两个隐藏文件，.和..，如果不需要该文件，应当在循环体内做判断：

f ('.' == *(d_dir->d_name))
                        continue;
//判断是否为隐藏文件，如果是，则跳过
文件名定义
定义一个char*类型的指针，用来存储目录文件下的文件名

char *p_name;
并且将dirent结构体指针指向结构体成员d_name，赋值给p_name。

p_name = d_dir->d_name;
到此，就获取到了目录文件下的文件。

stat函数的结构体
获取到文件名以后，开始获取文件的各个属性，这里用到了stat函数的结构体struct stat。

stat结构体
[root@localhost c]# man 2 stat
用到的头文件：

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
函数体：

int stat(const char *pathname, struct stat *buf);
//返回值为整形，传入参数为输入性参数的文件指针(char*类型)，stat结构体指针类型buf
结构体：

struct stat {
               dev_t     st_dev;         /* ID of device containing file */ //文件的设备id
               ino_t     st_ino;         /* inode number */ //文件的inode号
               mode_t    st_mode;        /* file type and mode */   //文件的属性，关于文件类型，文件权限，都依靠该成员
               nlink_t   st_nlink;       /* number of hard links */ //文件硬连接数
               uid_t     st_uid;         /* user ID of owner */     //文件所属用户id
               gid_t     st_gid;         /* group ID of owner */    //文件所属用户组id
               dev_t     st_rdev;        /* device ID (if special file) */  //特殊文件设备id
               off_t     st_size;        /* total size, in bytes */     //文件大小
               blksize_t st_blksize;     /* blocksize for filesystem I/O */ //文件文件系统io的块大小
               blkcnt_t  st_blocks;      /* number of 512B blocks allocated */
​
               /* Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES. */
​
               struct timespec st_atim;  /* time of last access */  //访问时间
               struct timespec st_mtim;  /* time of last modification */    //修改内容时间
               struct timespec st_ctim;  /* time of last status change */   //写该属性时间
           #define st_atime st_atim.tv_sec      /* Backward compatibility */
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
           };
该结构体里面有各个文件属性，其中有成员st_mode，可以通过该成员计算文件类型和权限值。

定义结构体
struct stat st; //定义一个stat函数的对象st
memset(&st,0,sizeof(st));//清空对象st内存中的垃圾数据
stat(p_name,&st);//使用函数stat绑定文件名和对象st
计算文件大小
struct stat结构体中有st_size成员，用于计算文件大小，返回之值为off_t为整形。则定义：

int n_size;
n_size = st.st_size;
//使用stat结构体对象st访问结构体内成员变量st_size，并赋值给n_size，stat结构体内st_size用来表示文件大小
计算文件硬连接数
struct stat结构体中有st_nlink成员，用于计算文件的硬连接数，返回值为nlink_t为整形。则定义：

int n_link;
n_link=st.st->st_nlink;
//使用stat结构体对象st访问结构体内成员变量st_nlink>，并赋值给n_link，stat结构体内st_link用来表示硬链接数
文件类型
linux在一切皆文件的思想下：文件被分为不同类型：目录、普通文件、套接字、字符设备、块设备、管道、链接等。

struct stat结构体中有st_mode，通过一定计算可以从此获取文件类型。有两种风格的判断方式：

system V

在man 2 stat中可以看到该方式的使用方法：

The following mask values are defined for the file type of the  st_mode
       field:
​
           S_IFMT     0170000   bit mask for the file type bit field
​
           S_IFSOCK   0140000   socket
           S_IFLNK    0120000   symbolic link
           S_IFREG    0100000   regular file
           S_IFBLK    0060000   block device
           S_IFDIR    0040000   directory
           S_IFCHR    0020000   character device
           S_IFIFO    0010000   FIFO
即，使用st_mode与不同的类型进行位与计算，如果返回值为1，则说明类型正确，为0则说明错误。

具体使用：

char n_dir; //定义一个char类型变量，用来存放文件类型。
if(S_IFREG & st.st_mode)
                n_dir = '-';
如果判断成功，则将"-"字符赋值给n_dir。

POSIX

第二种风格是posix风格的方式：使用man 2 stat可以查看：

Because  tests  of  the  above  form  are common, additional macros are
       defined by POSIX to allow the test of the file type in  st_mode  to  be
       written more concisely:
​
           S_ISREG(m)  is it a regular file?
​
           S_ISDIR(m)  directory?
​
           S_ISCHR(m)  character device?
​
           S_ISBLK(m)  block device?
​
           S_ISFIFO(m) FIFO (named pipe)?
​
           S_ISLNK(m)  symbolic link?  (Not in POSIX.1-1996.)
​
           S_ISSOCK(m) socket?  (Not in POSIX.1-1996.)
即：将st_mode作为参数，传给不同类型的mode，进行判断，如果返回1，则表示类型匹配，返回0，则表示类型不匹配。具体：

char n_dir;
if (S_ISDIR(st.st_mode))//判断是否为目录类型
                n_dir = 'd';
如果判断成功，则将"d"字符赋值给n_dir。

这里使用了POSIX风格进行计算文件类型

具体代码：

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
文件权限
通过ll命令，我们知道文件权限除了文件类型还有9个字符，则可以定一个10位的char型数组来存放。

定义方法：

 char n_power[10] = {};//定义一个固定长度的char型数组n_power用来存放九个权限位
具体获取每位的权限，还是要使用stat结构体中st_mode成员。具体方法，man 2 stat：

The  following  mask values are defined for the file mode component of the st_mode
       field:
​
           S_ISUID     04000   set-user-ID bit
           S_ISGID     02000   set-group-ID bit (see below)
           S_ISVTX     01000   sticky bit (see below)
​
           S_IRWXU     00700   owner has read, write, and execute permission
           S_IRUSR     00400   owner has read permission
           S_IWUSR     00200   owner has write permission
           S_IXUSR     00100   owner has execute permission
           S_IXUSR     00100   owner has execute permission
​
           S_IRWXG     00070   group has read, write, and execute permission
           S_IRGRP     00040   group has read permission
           S_IWGRP     00020   group has write permission
           S_IXGRP     00010   group has execute permission
​
           S_IRWXO     00007   others (not in group) have read,  write,  and
                               execute permission
           S_IROTH     00004   others have read permission
           S_IWOTH     00002   others have write permission
           S_IXOTH     00001   others have execute permission
可以看到里面有：S_IRUSR、S_IWUSR、S_IXUSR、S_IRGRPS_IWGRP、S_IXGRP、S_IROTH、S_IWOTH、S_IXOTH九个值，分别代表权限上的九位。计算方法，将这九个值分别和st_mode进行位与计算，返回值为1，则表示该权限存在，返回值为0，则表示该权限不存在。

具体：

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
文件所属用户
以为stat中的结构体只能获取到所属用户的id号，而不能获取所属用户用户名，所以这里还要使用getpwuid函数。具体：

[root@localhost day16]# man getpwuid
使用到的头文件：

#include <sys/types.h>
#include <pwd.h>
函数体：

struct passwd *getpwuid(uid_t uid);
//返回值类型是一个结构体指针类型，传入参数为uid_t类型的uid值
结构体：

The passwd structure is defined in <pwd.h> as follows:
​
           struct passwd {
               char   *pw_name;       /* username */
               char   *pw_passwd;     /* user password */
               uid_t   pw_uid;        /* user ID */
               gid_t   pw_gid;        /* group ID */
               char   *pw_gecos;      /* user information */
               char   *pw_dir;        /* home directory */
               char   *pw_shell;      /* shell program */
           };
//注意里面pw_name为我们需要的用名
则，先定义一个passwd结构体：

struct passwd *u_uid = NULL;//定义getpwuid结构体指针u_uid，并初始化为空
然后将stat结构体中的st.st_uid作为参数传给getpwuid。并将结果赋值给u_uid。

u_uid = getpwuid(st.st_uid);//u_uid指针绑定函数getpwuid(st.st_uid)，st_st_uid是stat>结构体对象st的成员变量
这里最好判断一下是否赋值成功

if (NULL == u_uid)//判断u_uid指针是否指向成功
        {
                perror("getpwuid");
                exit(0);
        }
然后定一个char*类型的指针，保存得到的用户名：

char *g_uid = u_uid->pw_name;//定义char*型指针g_uid用来保存得到的用户名
文件所属用户组
以为stat中的结构体只能获取到所属用户组的id号，而不能获取所属用户用户组的组名，所以这里还要使用gegrgid函数。具体：

[root@localhost day16]# man getgrgid
使用到的头文件：

 #include <sys/types.h>
 #include <grp.h>
函数体：

struct group *getgrgid(gid_t gid);
//返回值类型为结构体指针类型，传入参数为gid_t类型的gid(所数组的组id)
结构体：

struct group {
               char   *gr_name;       /* group name */
               char   *gr_passwd;     /* group password */
               gid_t   gr_gid;        /* group ID */
               char  **gr_mem;        /* group members */
           };
//注意：gr_name为我们需要计算的文件所属用户组组名
具体：

先定义一个group结构体指针：

struct group *gr_gid = NULL;//定义getgrgid结构体指针gr_gid,并初始化为空
然后，将stat函数结构体中获取的st_gid作为参数，传入getgrgid函数，将结果赋值给gr_gid(group结构体指针类型变量)。

gr_gid = getgrgid(st.st_gid);//gr_gid指针绑定函数getgrgid(st.st_gid)，st.st_gid是stat结构体对象st的成员变量
最好做一下判断

if (NULL == gr_gid)//判断gr_gid指针是否指向成功
        {
                perror("getgrgid");
                exit(0);
        }
定义一个char*类型变量用来保存获取到的用户组名称：

char *g_gid = gr_gid->gr_name;//定义char*型指针g_gid用来保存得到的组名
获取时间
获取时间使用了localtime函数，查看一下localtime相关参数：

[root@localhost day16]# man localtime
使用到的头文件：

 #include <time.h>
函数体：

struct tm *localtime(const time_t *timep);
//函数返回值为结构体指针，传入参数为一个输入形指针参数
从stat结构体的成员变量mtime中获取到localtime的参数，并定义一个struct tm *的指针保存函数返回值。

tm结构体：

struct tm {
               int tm_sec;         /* seconds */
               int tm_min;         /* minutes */
               int tm_hour;        /* hours */
               int tm_mday;        /* day of the month */
               int tm_mon;         /* month */
               int tm_year;        /* year */
               int tm_wday;        /* day of the week */
               int tm_yday;        /* day in the year */
               int tm_isdst;       /* daylight saving time */
           };
//从ll命令可以看出来我们需要获取，tm_mon、tm_day、tm_hour、tm_min
则，具体：

struct tm *local_time = NULL;//定义localtime结构体指针local_time，并初始化为空
        local_time = localtime(&st.st_mtime);//local_time指针绑定函数localtime(&st.st_mtime)，st.st_mtime是stat结构体对象st的成员变量
​
        if (NULL == local_time) //判断local_time指针是否绑定成功
        {
                perror("localtime");
                exit(0);
        }
        int n_num = local_time->tm_mon;//定义int型变量n_num用来保存local_time指针访问到的月>份，注意：这里得到的月份是数字表示，且localtime函数得到的月份是0~11,不符合正常思维的1~12
        char *n_mon;//定义char*型变来嗯n_mon，用来转换数字月份到英文简写月份
        int n_day = local_time->tm_mday;//定义int型变来嗯n_day用来存放local_time指针访问到的
日期
        int n_hour = local_time->tm_hour;//定义int型变来嗯n_hour用来存放local_time指针访问到
的小时
        int n_min = local_time->tm_min;//定义int型变来嗯n_min用来存放local_time指针访问到的>分钟
这里，tm_mon的返回值为0-11，ll命令显示的是各个月份的英文字母缩写，所以需要进行转换：

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
输出函数
搞定以后，进行排序输出：

printf("%c%s %d %s %s %d \t %s %d %d:%d\t%s\n",n_dir,n_power,n_link,g_uid,g_gid,n_size,n_mon,n_day,n_hour,n_min,p_name);//输出需要的变量访问结果
测试结果：

[root@localhost day16]# gcc ls_res.c -o my_ls
[root@localhost day16]# ./my_ls 
drwxrwxrwx 3 root root 4096      May 5 11:38    fsx_test_1
drwxrwxrwx 3 root root 4096      May 5 11:38    after
drwxrwxrwx 3 root root 4096      May 5 11:38    fsx_test_2
-rwxr-xr-x 1 root root 13072     Aug 16 21:0    a.out
-rwxrwxrwx 1 root root 6126      Aug 16 21:2    ls_res.c
drwxrwxrwx 2 root root 4096      May 5 11:38    mor
-rwxr-xr-x 1 root root 13072     Aug 16 21:4    my_ls
drwxrwxrwx 3 root root 4096      May 5 11:38    teacher
-rw-r--r-- 1 root root 23586     Aug 16 21:2    stat.txt
-rw-r--r-- 1 root root 0     Aug 16 20:50   fsx
对比ll命令：

[root@localhost day16]# ll
total 84
drwxrwxrwx. 3 root root  4096 May  5 11:38 after
-rwxr-xr-x  1 root root 13072 Aug 16 21:00 a.out
-rw-r--r--  1 root root     0 Aug 16 20:50 fsx
drwxrwxrwx. 3 root root  4096 May  5 11:38 fsx_test_1
drwxrwxrwx. 3 root root  4096 May  5 11:38 fsx_test_2
-rwxrwxrwx. 1 root root  6126 Aug 16 21:02 ls_res.c
drwxrwxrwx. 2 root root  4096 May  5 11:38 mor
-rwxr-xr-x  1 root root 13072 Aug 16 21:04 my_ls
-rw-r--r--  1 root root 23586 Aug 16 21:02 stat.txt
drwxrwxrwx. 3 root root  4096 May  5 11:38 teacher
----------------
*/
