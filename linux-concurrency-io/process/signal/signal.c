#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

//信号处理函数，通过signal函数来关联对应的信号
//@param sig：当前接收到的信号（与这个处理函数相关联）
void do_sig_usr(int sig)
{
    //将信号编号
    printf("Receive < %s >\n",strsignal(sig));
}

int main(void)
{
    pid_t cpid;
    __sighandler_t ret;
    //将信号与信号处理函数关联并注册到内核
    ret = signal(SIGUSR1,do_sig_usr);
    if(ret == SIG_ERR){
        perror("[ERROR] signal():");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if(cpid == -1){
        perror("[ERROR] fork():");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){
        fprintf(stdout,"\t child %d start...\n",getpid());

        pause();//等待信号唤醒，这里需要信号到达之前进入睡眠状态
        //父进程发送信号过来，先执行信号处理函数，再接着执行此进程

        fprintf(stdout,"\t child %d end...\n",getpid());

        exit(EXIT_SUCCESS);
    }else if(cpid > 0){
        int ret;

        sleep(1);

        kill(cpid,SIGUSR1);//给子进程发送 SIGUSR1 信号

        wait(NULL);
    }

}