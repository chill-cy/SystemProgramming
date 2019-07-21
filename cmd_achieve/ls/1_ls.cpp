/*************************************************************************
	> File Name: 1_ls.cpp
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年05月11日 星期六 14时44分04秒
 ************************************************************************/

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <string.h>
#include <grp.h>
#include <stdlib.h>
#include <sys/loctl.h>

#define LEN 512
int ls_opt = 0;
void do_ls (char *dirname);
char work_dir[LEN]; 
int main(int argc, char *argv[]) {
    int opt;
    getcwd(work_dir, )
    while ((opt = getopt(argc, argv, "al")) != -1) {
        switch (opt) {
            case 'a':
            ls_opt++;
            break;
            case 'l':
            ls_opt += 2;
            break;
            default:
            exit(1);
        }
    }
    argc -= (optind - 1);
    argv += (optind - 1);
    printf("argv[%d] = %s\n", argc -1, *(argv + 1));
    if (argc == 1) {
        do_ls(".");
    } else {
        while(--argc) {
            do_ls(*(++argv));
        }
    }
    return 0;
} 

int cmp_name(const void* _a, const void* _b) {
    char *a = (char*)_a;
    char *b = (char*)_b;
    return strcmp(a, b);
}
void do_ls(char *dirname) {
    printf("%s:\n", dirname);
    chdir(work_dir);
    DIR *dir_ptr = NULL;
    struct dirent *direntp;
    char file_name[1024][LEN] = {0};
    if ((dir_ptr = opendir(dirname)) == NULL) {
        perror("opendir");
    } else {
        while ((direntp = readdir(dir_ptr)) != NULL) {
            strcpy(file_name[cnt++], direntp->d_name);
            printf("%s\n", direntp->d_name);
        }
        qsort(file_name, cnt, LEN, cmp_name);
        /*for (int i = 0; i < cnt; i++) {
            printf("file_name[%d] = %s\n", i, file_name)
        }
        */
        if (is_opt >= 2) {
            for (int i = 0; i < cnt; i++) {
                do_stat(file_name[i], &info);
            }

        }
    }
}

void do_stat(char *filename, struct stat *info) {
    struct stat info;
    if (stat(filename, &info))
}

void show_file_info(char *filename, struct stat *info) {
    char modestr[11] = "-------------";
    mode_to_letters(info->st_mode , modester);
}

void mode 
void mode_to_letters(int);
void uid_to_name(uid_t);
void gid_to_name(git)
