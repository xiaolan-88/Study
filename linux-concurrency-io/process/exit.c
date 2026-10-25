#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//给父进程返回的是子进程的pid，返回给子进程的是0
int main(void)
{
    pid_t cpid;

    cpid = fork();
    if(cpid == -1){
        perror("[ERROR]: fork()");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){ //子进程A
        //printf属于行缓冲，遇到换行符自动刷新缓冲区，把缓冲区的数据输出到屏幕上
        printf("Child A Process running < %d >",getpid());
        sleep(2);
        //exit(EXIT_SUCCESS);//终止进程，刷新缓冲区，输出
        _exit(EXIT_SUCCESS);//不刷新缓冲区
        printf("Child A Process exit < %d >\n",getpid());
    }else if(cpid > 0){

    }

    return 0;

}