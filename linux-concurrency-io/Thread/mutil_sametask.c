#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

//线程执行函数，直接由操作系统进行调度执行  并发执行
void *task(void *arg)
{
    int i;
    for(i = 0;i < 5;i ++){{
        printf("i = %d\n",i);
        sleep(1);
    }}
    pthread_exit(NULL);//线程退出
}


int main(void)
{
    pthread_t tid[2] = {0};
    int err,i;
    for(i = 0;i < 2;i++){
        err = pthread_create(&tid[i], NULL, task, NULL);
        if(err != 0){
            fprintf(stderr, "[ERROR] pthread_creat(): %s\n",strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    pthread_join(tid[0],NULL);//等待子线程退出，这里会阻塞主线程.  == wait()
    pthread_join(tid[1],NULL);//等待子线程退出，这里会阻塞主线程.  == wait()

    // pthread_detach(tid);//线程分离，由操作系统自动释放子线程的资源，不会阻塞主线程
    // //后面必须跟程序，否则分离后主线程没有执行代码会直接退出，导致子线程提前退出


    return 0;
}

//编译时要链接 -pthread。  gcc thread_create.c -lpthread
//运行结果：只打印tid，子线程没有执行。
//原因：子线程还没有来得及执行，主线程已经执行结束，导致所有其他子线程都要必须结束