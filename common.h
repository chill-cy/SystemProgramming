/*************************************************************************
	> File Name: common.h
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年07月18日 星期四 20时19分58秒
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

typedef struct Node {
    struct sockaddr_in client_addr;
    int node_sock;
    struct Node *next;
} Node, *LinkedList;

int insert(LinkedList head, Node *p);

int insert(LinkedList head, Node *node) {
    Node *p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
    return 1;
}

int get_conf_value(char *file, char *key, char *value) {
    FILE *fd = NULL;
    char *line = NULL, substr;
    size_t n = 0, length = 0;
    ssize_t read;

    if (key == NULL || value == NULL) {
        printf("error\n");
        return -1;
    }
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Open config file error!\n");
        return -1;
    }

    while((read = getline(&line, &n, fp)) != -1) {
        substr = strstr(line, key);
        if (substr == NULL) {
            continue;
        } else {
            len = strlen(key);
            if (line[len] == '=') {
                strncpy(value, line + len + 1, (int)read - len - 1);
                break;
            } else {
                continue;
            }
        }
    }
    if (substr == NULL) {
        printf("%s Not found in %s!\n", key, file);
        fclose(fp);
        free(line);
        return -1;
    }
    fclose(fp);
    free(line);
    return 0;
}
#endif
