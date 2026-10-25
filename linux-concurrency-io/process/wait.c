#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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
        printf("Child A Process running < %d >\n",getpid());
        sleep(2);
        exit(88);//退出码任意
    }else if(cpid > 0){
        int status = 0;
        /*
        wait()作用:
            阻塞父进程,将status地址传给wait函数，
            wait函数等待子进程终止，内核将子进程的终止信息打包，
            写入地址对应的status变量里,自动释放死掉的子进程的内核资源
            rcpid:成功返回退出子进程的pid，失败返回-1
        */
        pid_t rcpid = wait(&status);
        if(rcpid == -1){
            perror("[ERROR] wait():");
            exit(EXIT_FAILURE);
        }
        //WEXITSTATUS() :将status里的关于子进程的退出码提取出来
        printf("rcpid = %d ,exit code : %d, status : %d\n",rcpid,WEXITSTATUS(status),status);

    }

    return 0;

}