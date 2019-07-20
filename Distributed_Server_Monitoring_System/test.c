/*************************************************************************
	> File Name: test.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月18日 星期四 20时43分02秒
 ************************************************************************/

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./test key!\n");
        exit(1);
    }
    char *file = argv[1];
    char *key = argv[2];
    char value[100] = {0};

    if (get_conf_value(file, key, value) < 0) {
        printf("x!\n");
        exit(1);
    } else {
        printf("value", )
    }
    return 0;
}
