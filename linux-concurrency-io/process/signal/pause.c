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
        sleep(3);
        //SIGUSR1 :默认为结束进程 
        kill(getppid(),SIGUSR1);//子进程给父进程发送 SIGUSR1 信号
        fprintf(stdout,"\t child %d exit\n",getpid());
        exit(EXIT_SUCCESS);
    }else if(cpid > 0){
    
        fprintf(stdout,"Main Process start\n");
        pause(); //阻塞进程，收到信号后唤醒
        fprintf(stdout,"Main Process end\n");

        wait(NULL);
    }

}