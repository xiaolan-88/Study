#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void do_sig_alarm(int sig)
{
    printf("Receive < %s >\n",strsignal(sig));
}

int main(void)
{
    unsigned int ret;
    __sighandler_t sigret;

    sigret = signal(SIGALRM,do_sig_alarm);
    if(sigret == SIG_ERR){
        perror("[ERROR] signal():");
        exit(EXIT_FAILURE);
    }

    printf("Main process start.\n");

    ret = alarm(3);//设置定时时间为 3 秒，由内核来定时

    pause();

    printf("Main process end.\n");//SIGALRM 信号默认处理方式为结束进程
    //如果是自定义信号处理函数就会执行完do_sig_alarm函数后接着执行未执行代码

    return 0;
}