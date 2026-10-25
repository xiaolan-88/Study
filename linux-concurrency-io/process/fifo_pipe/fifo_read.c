#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PATHNAME "./fifo_test"

int main(void)
{
    int ret,fd;
    ssize_t rbytes;
    char buffer[64] = {0};

    //创建有名管道
    ret = mkfifo(PATHNAME, 0644);
    if(ret == -1){
        perror("[ERROR] mkfifo():");
        exit(EXIT_FAILURE);
    }

    //打开有名管道文件
    fd = open(PATHNAME,O_RDONLY);
    if(fd == -1){
        perror("[ERROR] open():");
        exit(EXIT_FAILURE);
    }

    //读取管道内数据
    rbytes = read(fd,buffer,sizeof(buffer));//管道内没有数据时阻塞
    if(rbytes == -1){
        perror("[ERROR] read():");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("buffer : %s\n",buffer);

    close(fd);

    return 0;
}