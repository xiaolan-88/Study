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
    ssize_t wbytes;
    char buffer[] = "hello,pipe.";

    //打开有名管道文件
    fd = open(PATHNAME,O_WRONLY);
    if(fd == -1){
        perror("[ERROR] open():");
        exit(EXIT_FAILURE);
    }

    //写数据到管道
    wbytes = write(fd,buffer,strlen(buffer));
    if(wbytes == -1){
        perror("[ERROR] write():");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    return 0;
}