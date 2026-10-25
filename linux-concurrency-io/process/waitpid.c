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
            pid_t rcpid = wait(&status);等价于 pid_t rcpid = waitpid(-1,&status,0);
        
            waitpid(pid,wstatus,options):
                pid > 0：只等待PID等于这个值的子进程（精准等待某一个子进程，最常用）
                pid = -1：等待任意一个子进程，行为和 wait 一模一样
                pid = 0：等待同进程组内任意子进程
                pid < -1：等待指定进程组里任意子进程

                wstatus :终止信息

                options:
                    0：阻塞，直到子进程结束（和wait一样卡住父进程）
                    WNOHANG：非阻塞！如果子进程还没退出，waitpid立刻返回0，不阻塞父进程；
                            子进程已结束，返回子进程pid；调用失败返回-1。

        */
        //pid_t rcpid = waitpid(-1,&status,0);//等待任意子进程，默认阻塞
        //pid_t rcpid = waitpid(-1,&status,WNOHANG);//非阻塞

        pid_t rcpid = 0;
        //如果设置非阻塞，没有子进程退出，就会返回0，有子进程退出，返回pid
        while((rcpid = waitpid(-1,&status,WNOHANG)) == 0)
        {

        }
        printf("rcpid = %d ,exit code : %d, status : %d\n",rcpid,WEXITSTATUS(status),status);
    }

    return 0;

}