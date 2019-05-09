/*************************************************************************
	> File Name: 5_ls.cpp
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年05月09日 星期四 15时12分14秒
 ************************************************************************/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

//获得文件权限
void getmod(int mode, char *mod) {
    if (S_ISREG(mode)) {
        mod[0] = '-';
    } else if (S_ISDIR(mode)) {
        mod[0] = 'd';
    } else if (S_ISLNK(mode)) {
        mod[0] = 'l';
    } else if (S_ISCHR(mode)) {
        mod[0] = 'c';
    } else if (S_ISBLK(mode)) {
        mod[0] = 'b';
    } else if (S_ISFIFO(mode)) {
        mod[0] = 'p';
    } else {
        mod[0] = 's';
    }
	
    if (mode & S_IRUSR) {
        mod[1] = 'r';
    } else {
        mod[1] = '-';
    }
    if (mode & S_IWUSR) {
        mod[2] = 'w';
    } else {
        mod[2] = '-';
    }
    if (mode & S_IXUSR) {
        mod[3] = 'x';
    } else {
        mod[3] = '-';
    }

　　if (mode & S_IRGRP) {
        mod[4] = 'r';
    } else {
        mod[4] = '-';
    }
    if (mode & S_IWGRP) {
        mod[5] = 'w';
    } else {
        mod[5] = '-';
    }
    if (mode & S_IXGRP) {
        mod[6] = 'x';
    } else {
        mod[6] = '-';
    }

    if (mode & S_IROTH) {
        mod[7] = 'r';
    } else {
        mod[7] = '-';
    }
    if (mode & S_IWOTH) {
        mod[8] = 'w';
    } else {
        mod[8] = '-';
    }
    if (mode & S_IXOTH) {
        mod[9] = 'x';
    } else {
        mod[9] = '-';
    }
}

//判断输入的路径是不是目录
int is_dir(char *is_dirname) {
    struct stat *st = (struct stat *)malloc(sizeof(struct stat));
    stat(is_dirname, st);
    if (S_ISDIR(st->st_mode)) {
        return 1;
    } else {
        return 0;
    }
    free(st);
}

//输出文件的详细信息
void showinfo(char *name) {
    struct stat *file = (struct stat *)malloc(sizeof(struct stat));
    if (stat(name, file) < 0) {
        perror("stat");
        return;
    }
    struct passwd *usr;
    usr = getpwuid(file->st_uid);
    struct group *grp;
    grp = getgrgid(file->st_gid);
    struct tm *t = localtime(&file->st_mtime);
    char mod[11];
    getmod(file->st_mode, mod);
    printf("%s", mod);
    printf("%2d", file->st_nlink);
    printf("%5s", usr->pw_name);
    printf("%5s", grp->gr_name);
    printf("%6lld", file->st_size);
    printf("%2d %2d %02d:%02d ", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
    printf("%s\n", name);
    free(file);
}

//输出目录下的所有文件信息
void showdirinfo(char *dirname) {
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    struct dirent *entry;
    char pwd[100];
    getcwd(pwd, 100);
    chdir(dirname);
    //printf("%s:\n", dirname);
    int sum = 0;
    string filename[3005];
    //filename记录文件名
    while ((entry = readdir(dir)) != NULL) { 
        filename[sum] = entry->d_name;
        sum++;
    }
    sort(filename, filename + sum);
    //cout << filename << endl;
    char s[3005];
    //按序输出
    for (int i = 0; i < sum; i++) {
        char *p = const_cast<char *>(filename[i].c_str());
        showinfo(p);
    }
    chdir(pwd);
}

//显示文件名称
void showname(char *name) {
    struct stat *file = (struct stat *)malloc(sizeof(struct stat));
    if (stat(name, file) < 0) {
        perror("stat");
        return;
    }
    printf("%s\n", name);
}

//获得屏幕大小
int getsize() {
    struct winsize size;
    if (isatty(STDOUT_FILENO) == 0) {
        exit(1);
    }
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
        perror("ioctl TIOCGWINSZ error");
        exit(1);
    }
    return size.ws_row;
}

//得到所有文件中最大长度的文件
int maxname(string filename[3005], int sum) {
    int max = filename[0].length();
    for (int i = 1; i < sum; i++) {
        if (filename[i].length() > max) {
            max = filename[i].length();
        }
    }
    return max;
}
//输出文件名
void print_name(string filename[3005], int sum) {
    int max_namelen;//所有文件的最大长度
    max_namelen = maxname(filename, sum);
    int col = getsize() / max_namelen; //col初始列数
    int row = sum / col;
    if (row * col < sum) row++;
    int k = 0;
    int max_col[1000] = {0}; //存储每列最大长度
    for (int j = 0; j < sum; j += row) {
        for (int i = j; i < j + row; i++) {
            if (filename[i].length() > max_col[k]) {
                max_col[k] = filename[i].length();
            }
        }
        k++;
    }
    for (int i = 0; i < row; i++) {
        k = 0;
        for (int j = i; j < sum; j += row) {
            cout << filename[j];
            int spaceNum = max_col[k] - filename[j].length() + 2;
            while (spaceNum--) {
                cout << " ";
            }
            k++;
        }
        cout << endl;
    }
}

//得到目录下的文件名
void getdirname(char *dirname, string filename[3005], int *sum) {
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        return ;
    }
    struct dirent *entry;
    char pwd[100];
    getcwd(pwd, 100);
    chdir(dirname);
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {//屏蔽隐藏文件
        if (!strncmp(entry->d_name, ".", 1)) {
            continue;
        }
        filename[count] = entry->d_name;
        count++;
    }
    chdir(pwd);
    *sum = count;
    sort(filename, filename + count);
}

//显示目录下的文件名
void showdirname (char *dirname) {
    string filename[3005];
    int sum = 0;
    getdirname(dirname, filename, &sum);
    print_name(filename, sum);
}

//加选项
void is_option(int argc, char *argv[]) {
    char point[] = ".";
    if (strcmp(argv[1] + 1, "al") == 0) {
        if (argc == 2) {
            showdirinfo(point);
        } else {
            for (int i = 2; i < argc; i++) {
                if (is_dir(argv[i])) {
                    showdirinfo(argv[i]);
                } else {
                    showinfo(argv[i]);
                }       
                if (i < argc - 1) {
                    printf("\n");
                }
            }
        }
    } else {
        printf(".\n", argv[0]);
    }
}

int main(int argc, char *argv[]) {
    char point[] = ".";
    if (argc == 1) {
        showdirname(point);
    } else if (argc >= 2){
        if (strncmp(argv, '-', 1) == 0) {
            is_option(argc, argv);         
        } else {
            for (int i = 1; i < argc; i++) {
                if (is_dir(argv[i])) {
                    showdirname(argv[i]);
                } else {
                    showdirname(argv[i]);
                }
                if (i < argc - 1) {
                    printf("\n");
                }
            }
        }
    }
    return 0;
}
