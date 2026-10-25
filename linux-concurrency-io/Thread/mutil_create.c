#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//线程执行函数，直接由操作系统进行调度执行  并发执行
void *do_thread_a(void *arg)
{
    printf("do thread A ...\n");
    pthread_exit(NULL);//线程退出
}
void *do_thread_b(void *arg)
{
    printf("do thread B ...\n");
    pthread_exit(NULL);//线程退出
}

int main(void)
{
    pthread_t tida,tidb;
    int err;

    err = pthread_create(&tida, NULL, do_thread_a, NULL);
    if(err != 0){
        fprintf(stderr, "[ERROR] pthread_creat(): %s\n",strerror(err));
        exit(EXIT_FAILURE);
    }

    err = pthread_create(&tidb, NULL, do_thread_b, NULL);
    if(err != 0){
        fprintf(stderr, "[ERROR] pthread_creat(): %s\n",strerror(err));
        exit(EXIT_FAILURE);
    }

    pthread_join(tida,NULL);//等待子线程退出，这里会阻塞主线程.  == wait()
    pthread_join(tidb,NULL);//等待子线程退出，这里会阻塞主线程.  == wait()

    // pthread_detach(tid);//线程分离，由操作系统自动释放子线程的资源，不会阻塞主线程
    // //后面必须跟程序，否则分离后主线程没有执行代码会直接退出，导致子线程提前退出


    return 0;
}

//编译时要链接 -pthread。  gcc thread_create.c -lpthread
//运行结果：只打印tid，子线程没有执行。
//原因：子线程还没有来得及执行，主线程已经执行结束，导致所有其他子线程都要必须结束