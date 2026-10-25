#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t cpid;

    write(1,"hello",5);//stdout,数据不进入缓冲区
    fputs("hello",stdout);//数据进入缓冲区，在创建进程之前

    cpid = fork();//子进程从fork之后开始执行   

/*
输出结果
    hellohellopid : 93413 父进程
    Hello world.
    hellopid : 93414      子进程，同时拷贝了父进程缓冲区的内容
    Hello world.
*/
    if(cpid == -1){
        perror("[ERROR] fork():");
    }

    //子进程创建后与父进程并发执行
    //父子进程的执行顺序由操作系统调度算法决定
    //子进程会拷贝父进程的地址空间的数据，以及缓冲区内容
    printf("pid : %d\n",getpid());//获取当前进程pid
    printf("Hello world.\n");

    return 0;
}