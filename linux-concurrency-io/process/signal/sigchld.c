#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int flag = 0;

//SIGCHLD 默认的处理方式是忽略
void do_sig_child(int sig)
{
    printf("Receive < %s >\n",strsignal(sig));
    wait(NULL);//子进程结束后不会阻塞，立即释放子进程资源
    flag = 1;
}

int main(void)
{
    pid_t cpid;

    __sighandler_t sigret;
    sigret = signal(SIGCHLD,do_sig_child);
    if(sigret == SIG_ERR){
        perror("[ERROR] signal():");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if(cpid == -1){
        perror("[ERROR] fork():");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){
        fprintf(stdout,"\t child %d start...\n",getpid());

        sleep(2);

        exit(EXIT_SUCCESS); 
    }else if(cpid > 0){
        fprintf(stdout,"\t Parent %d start...\n",getpid());
        while(flag == 0){}
        fprintf(stdout,"\t Parent %d end...\n",getpid());
    }
    return 0;
}