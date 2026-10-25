#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//线程间通信

// number --> arg
void *do_thread(void *arg)
{
    //强转int *型，赋值给number
    static int number;
    number = *(int *)arg;
    printf("number = %d\n",number);

    number++;
    //pthread_exit(void *retval)返回的数据传入pthread_join(pthread_t th, void **retval)
    //指针 --》 指针的指针
    pthread_exit((void *)&number);//线程退出并将返回值给join
}

int main(void)
{
    pthread_t tid;
    int err;
    int number = 100;
    void *pret = NULL;
    //第四个参数为地址，将number转换为地址后再传
    err = pthread_create(&tid, NULL, do_thread,(void *)&number);
    if(err != 0){
        fprintf(stderr, "[ERROR] pthread_creat(): %s\n",strerror(err));
        exit(EXIT_FAILURE);
    }

    // 定义一个指针pret，将子线程返回回来的值给指向指针pret的指针
    pthread_join(tid,(void **)&pret);//等待子线程退出，这里会阻塞主线程.  == wait()

    printf("result == %d\n",*(int *)pret);

    // pthread_detach(tid);//线程分离，由操作系统自动释放子线程的资源，不会阻塞主线程
    // //后面必须跟程序，否则分离后主线程没有执行代码会直接退出，导致子线程提前退出


    return 0;
}

//编译时要链接 -pthread。  gcc thread_create.c -lpthread
//运行结果：只打印tid，子线程没有执行。
//原因：子线程还没有来得及执行，主线程已经执行结束，导致所有其他子线程都要必须结束