/*************************************************************************
	> File Name: 5_cp.c
	> Author:jiangxiaoyu 
	> Mail:2291372880@qq.com 
	> Created Time: 2019年05月10日 星期五 21时47分10秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

int Exist(char* path);//判断文件、目录是否存在
int IsFolder(char* path);//判断是否为目录
int FileCopy(char* src, char* des);//复制文件
int FileCopyToFolder(char* file, char* folder);//复制文件到目录
int CreateFolder(char* folder);//创建文件夹
int FolderCopy(char* src, char* des);//直接复制目录
int FolderCopyToFolder(char* src, char* des);//复制目录到目录

int main(int argc, char* argv[]) {
    int i;
    if (argc < 3) {//参数不够
        printf("Not enough input arguments!\n");
        return -1;
    } else if (argc == 3) {//只有两个参数
        if (!Exist(argv[1])) {//源文件、目录不存在
            printf("'%s'does not exist!\n", argv[1]);
            return -2;
        } else if (IsFolder(argv[1]) && !IsFolder(argv[2])) {//直接复制源目录，制作目录副本
            return FolderCopy(argv[1], argv[2]); 
        } else if (IsFolder(argv[1]) && IsFolder(argv[2])) {//将源目录复制到目标目录下
            return FolderCopyToFolder(argv[1], argv[2]);
        } else if (!IsFolder(argv[2])) {//直接复制源文件，制作文件副本
            return FileCopy(argv[1], argv[2]);
        } else if (IsFolder(argv[2])) {//将源文件复制到目标目录下
            return FileCopyToFolder(argv[1], argv[2]);
        }
    } else {//多个源文件，目录参数
        if (!IsFolder(argv[argc - 1])) {//目标目录不存在
            printf("'%s' is not a folder!\n", argv[argc - 1]);
            return -3;
        }
        //遍历各个源文件，目录不存在
        for (i = 1; i < argc - 1; i++) {
            if (!Exist(argv[i])) {//源文件，目录不存在
                printf("'%s' does not exist!\n", argv[i]);
            } else if (IsFolder(argv[i])) {//复制目录
                FolderCopyToFolder(argv[i], argv[argc - 1]);
            } else {//复制文件
                FileCopyToFolder(argv[i], argv[argc - 1]);
            }
        }
    }
    return 0;
}

//判断文件、目录是否存在
//调用函数：fopen, fclose
//参数：路径名
//返回值：0不存在；1=存在
int Exist(char* path) {
    FILE *fp;
    if ((fp = fopen(path, "r")) == NULL) {
        return 0;
    } else {
        fclose(fp);
        return 1;
    }
}

//函数名称：判断是否为目录
//调用函数：opendir, closedir
//参数：路径名
//返回值：0=不存在；1=存在
int IsFolder(char* path) {
    DIR *dp;
    if ((dp = opendir(path)) == NULL) 
        return 0;
    else {
        closedir(dp);
        return 1;
    }
}

//函数名称：复制文件
//调用函数：fopen, fclose, fgetc, fputc, feof
//参数；源文件路径名，目标文件路径名
//返回值：-1=源文件打开失败；-2=目标文件打开失败；0=复制成功
int FileCopy(char* src, char* des) {
    FILE *input, *output;
    char ch;
    if ((input = fopen(src, "r")) == NULL) {
        printf("'%s' does not exist!\n", src);
        return -1;
    }
    if ((output = fopen(des,"w")) == NULL) {
        fclose(input);
        printf("Wrong output file name or Permission denied!\n");
        return -2;
    }
    ch = fgetc(input);
    while (!feof(input)) {//判断最后一位的状态
        fputc(ch, output);
        ch = fgetc(input);
    }
    fclose(input);
    fclose(output);
    return 0;
}

//函数名称：复制文件到目录
//调用函数：strlen, strncpy, strcpy, FileCopy
//参数：源文件路径名，目标目录路径名
//返回值：-1=源文件打开失败；-2=目标文件打开失败；0=复制成功
int FileCopyToFolder(char* file, char* folder) {
    char fileName[500];//文件名称
    char fileDest[500];//目标文件路径
    int i;
    int Length = strlen(file);//获得源文件路径名的长度
    //从后往前循环遍历源文件路径名
    for (i = Length - 1; i >= 0; i--) {
        if (file[i] == '/') {//提取斜杠后的目录名
            strncpy(fileName, file + i + 1, Length - i);
            break;
        }
    }
    if (i <= -1) strcpy(fileName, file);//没有斜杠，路径名就是文件名
    //构造目标文件路径名
    strcpy(fileDest, folder);
    strcat(fileDest, "/");
    strcat(fileDest, fileName);
    return FileCopy(file, fileDest);//调用文件复制函数
}

//函数名称：创建目录
//调用函数：strcpy, strlen, strcat, access, mkdirm, IsFolder
//参数：要创建的目录路径名
//返回值：-1=创建失败；-2=同名文件已存在；0=创建成功
int CreateFolder(char* folder) {
    char folderPath[500];//目录路径名
    int i;
    int len;
    strcpy(folderPath, folder);//拷贝目录路径名
    len = strlen(folderPath);

    //确保路径名最后字符为斜杠
    if (folderPath[len - 1] != '/') {
        strcat(folderPath, "/");
        len++;
    }

    //循环遍历路径名，逐层往下创建目录
    for (i = 1; i < len; i++) {
        if (folderPath[i] == '/') {
            folderPath[i] = '\0';//将斜杠替换为结束符
            //尝试创建目录
            if (access(folderPath, F_OK) < 0) {//目录不存在
                if (mkdir(folderPath, 0755) < 0) {//创建失败
                    printf("mkdir '%s' error!\nWrong folder name or Permission denied!\n",folderPath);
                    return -1;
                }
            } else if (!IsFolder(folderPath)) {//已存在同名文件
                printf("cannot Create directory '%s', File exists!\n", folder);
                return -2;
            }
            folderPath[i] = '/';//替换回斜杠
        }
    }
    return 0;
}

//函数名称：直接复制目录
//直接把源目录下的文件和目录复制到目标目录下
//调用函数：opendir,CreateFolder, readdir, strcpy, strcat, strcmp, FileCopy;
//参数：源目录路径名，目标目录路径名
//返回值：-1=源目录打开失败；-2=目标目录创建失败；0=复制成功
int FolderCopy(char* src, char* des) {
    DIR *dp;//目录指针
    struct dirent *entry;//读取目录返回的结构体
    char srcInside[500];//源目录，文件路径名
    char desInside[500];//目标目录，文件路径名
    if((dp = opendir(src)) == NULL) {
        printf("%s is not a folder!\n", src);
        return -1;
    }
    //目标目录创建失败，目标目录已经存在
    if(CreateFolder(des) != 0) return -2;
    //循环读取目录下的所有条目信息
    while((entry = readdir(dp)) != NULL) {
        //创建对应的目的源，目标路径名
        strcpy(srcInside, src);
        strcpy(desInside, des);
        strcat(srcInside, "/");
        strcat(desInside, "/");
        strcat(srcInside, entry->d_name);
        strcat(desInside, entry->d_name);
        //如果条目是目录
        if(IsFolder(srcInside)) {
            //忽略.和..目录
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
            FolderCopy(srcInside, desInside);
        } else {//如果是文件
            FileCopy(srcInside, desInside);//调用文件复制函数
        }
    }
    return 0;
}

//函数名称：复制目录到目录
//将源目录直接复制到目标目录下
//调用函数：strlen, strncpy, strcpy, strcat, FolderCopy;
//参数：源目录路径名，目标目录路径名
//返回值：-1=源目录打开失败；-2=目标目录创建失败；0=复制成功
int FolderCopyToFolder(char* src, char* des) {
    char folderName[500];//源目录名称
    char folderDesPath[500];//目标目录路径名
    int i;
    int Length = strlen(src);
    //从后往前循环遍历源文件路径名
    for (i = Length - 2; i >= 0; i--) {//从倒数第二个字符开始遍历
        if (src[i] == '/') {//如果遇到斜杠
            //提取斜杠后的目录名
            strncpy(folderName, src + i + 1, Length - i);
            break;
        }
    }
    if (i <= -1) strcpy(folderName, src);//如果没有斜杠，路径名就是目录名
    //复制目标目录路径名
    strcpy(folderDesPath, des);
    //确认目标目录路径名以斜杠结尾
    if (folderDesPath[strlen(folderDesPath) - 1] != '/') strcat(folderDesPath, "/");
    //创建新的目标目录路径名
    strcat(folderDesPath, folderName);
    //调用直接复制目录函数
    return FolderCopy(src, folderDesPath);
} 
