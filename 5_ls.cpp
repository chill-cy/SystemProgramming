/*************************************************************************
	> File Name: 5_ls.cpp
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年05月09日 星期四 15时12分14秒
 ************************************************************************/

#include<iostream>
using namespace std;

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

int maxname(string filename[3005], int sum) {
    int max = filename[0].length();
    for (int i = 1; i < sum; i++) {
        if (filename[i].length() > max) {
            max = filename[i].length();
        }
    }
    return max;
}

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

void showdirname (char *dirname) {
    string filename[3005];
    int sum = 0;
    getdirname(dirname, filename, &sum);
    print_name(filename, sum);
}

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
