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
    }else if(cpid == 0){
        printf("Child process task.\n");//只在子进程中执行
    }else if(cpid > 0){
        printf("Parent process task.\n");//只在父进程中执行
    }

    printf("Parent and child process task.\n");//父子进程做相同的任务

    return 0;

}