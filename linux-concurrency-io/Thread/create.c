#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//线程执行函数，直接由操作系统进行调度执行  并发执行
void *do_thread(void *arg)
{
    printf("Thread start...\n");
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

    return 0;
}

//编译时要链接 -pthread。  gcc thread_create.c -lpthread
//运行结果：只打印tid，子线程没有执行。
//原因：子线程还没有来得及执行，主线程已经执行结束，导致所有其他子线程都要必须结束