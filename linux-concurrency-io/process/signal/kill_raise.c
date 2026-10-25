#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(void)
{
    pid_t cpid;
    cpid = fork();
    if(cpid == -1){
        perror("[ERROR] fork():");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){
        fprintf(stdout,"\t child %d running...\n",getpid());
        raise(SIGSTOP); //  子进程发送 SIGSTOP 信号,子进程暂停执行，等待父进程发送信号
        fprintf(stdout,"\t child %d exit\n",getpid());
        exit(EXIT_SUCCESS);
    }else if(cpid > 0){
        int ret;

        sleep(2);//给子进程留出时间执行到停止信号

        ret = kill(cpid,SIGKILL); //此时子进程为暂停状态， 父进程给子进程发送 SIGKILL 信号
        if(ret == 0){
            fprintf(stdout,"Father %d Killed child %d\n",getpid(),cpid);
        }
        wait(NULL);
    }

}