#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t cpid;
    int ret;
    int pipefd[2];
    //fd[0]:读端    fd[1]:写端
    ret = pipe(pipefd);//创建管道后，内核将文件描述符存储到数组中

    if(ret == -1){
        perror("[ERROR] pipe():");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if(cpid == -1){
        perror("[ERROR] fork():");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){ //从管道中读取数据

        ssize_t rbytes;
        char buffer[64] = {0};
        close(pipefd[1]);//关闭子进程写端文件描述符

        rbytes = read(pipefd[0],buffer,sizeof(buffer));//当管道里没有数据会阻塞
        if(rbytes == -1){
            perror("[ERROR] read():");
            close(pipefd[0]);
            exit(EXIT_FAILURE);
        } 
        printf("buffer : %s\n",buffer);

        close(pipefd[0]);
    }else if(cpid > 0){
        ssize_t wbytes;
        char buffer[] = "hello,pipe";
        close(pipefd[0]);

        wbytes = write(pipefd[1],buffer,strlen(buffer));
        if(wbytes == -1){
            perror("[ERROR] write():");
            wait(NULL);
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]);
        wait(NULL);
    }

    return 0;
}
