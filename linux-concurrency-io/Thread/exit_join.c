#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//线程执行函数，直接由操作系统进行调度执行  并发执行
void *do_thread(void *arg)
{
    printf("do thread ...\n");
    pthread_exit(NULL);//线程退出
}

int main(void)
{
    pthread_t tid;
    int err;

    err = pthread_create(&tid, NULL, do_thread, NULL);
    if(err != 0){
        fprintf(stderr, "[ERROR] pthread_creat(): %s\n",strerror(err));
    }

    printf("tid = %ld\n",tid);

    //pthread_join(tid,NULL);//等待子线程退出，这里会阻塞主线程.  == wait()

    pthread_detach(tid);//线程分离，由操作系统自动释放子线程的资源，不会阻塞主线程
    //后面必须跟程序，否则分离后主线程没有执行代码会直接退出，导致子线程提前退出

    while(1){

    }

    return 0;
}

//编译时要链接 -pthread。  gcc thread_create.c -lpthread
//运行结果：只打印tid，子线程没有执行。
//原因：子线程还没有来得及执行，主线程已经执行结束，导致所有其他子线程都要必须结束