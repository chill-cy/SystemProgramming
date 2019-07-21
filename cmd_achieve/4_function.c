#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
        /* 在这里，为了简单，直接用了数组存放当前工作目录 */
        /* 可以使用 [color=#000000] 中的 char *path_alloc(unsigned int *sizep); 函数为存放路径名的缓冲区动态分配内存 [/color]*/
        char save_cwd[1024];
        char buf[1024];
        /* 保存当前工作目录 */
        if (getcwd(save_cwd, sizeof(save_cwd)) == NULL)
                perror("getcwd error");
        printf("before chdir, cwd = %s/n", save_cwd);
        /* change cwd */
        if (chdir("/")  == 0)
                perror("chdir error");
        /* 下面调用 getcwd 函数，只是为了显示 当前工作目录 */
        if (getcwd(buf, sizeof(buf)) == NULL)
                perror("getcwd error");
        printf("after chdir, cwd = %s/n", buf);
        /* 返回到先前的工作目录，需要使用更换目录前保存的路径名 */
        if (chdir(save_cwd) == 0)
                perror("chdir error");
        //[color=#000000]/* 下面调用 getcwd 函数，只是为了显示 当前工作目录 */[/color]
        if (getcwd(buf, sizeof(buf)) == NULL)
                perror("getcwd error");
        printf("previous chdir, cwd = %s/n", buf);
        exit(0);
}
