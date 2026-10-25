#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static int global = 0;//临界资源 所有线程都可以访问

void *do_thread(void *arg)
{
    int loops = *(int *)arg;
    int i,tmp =0;
    for(i = 0;i < loops;i++){
        tmp = global;
        tmp ++;
        global = tmp;
    }
}

/*
    在没有用互斥锁的情况下，两个线程同时运行，当线程A运行到一半时，切换到线程B执行，
    线程B执行完毕后，线程A接着没执行完的接着执行，而不是重新去执行。
    两个线程同时去抢一个任务时，就会造成运行结果混乱
*/

//argv[0] : "./a.out"    argc[1] : "100"
int main(int argc,char *argv[])
{
        
    pthread_t tid[2];
    int loops;
    int err;

    if(argc !=2){
        fprintf(stderr,"Usage : < %s > < count loops >\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    loops = atoi(argv[1]); //字符转整数

    for(int i = 0;i < 2;i++){
        err = pthread_create(&tid[i], NULL, do_thread,(void *)&loops);
        if(err != 0){
            fprintf(stderr,"[ERROR] pthread_create(): %s",strerror(err));
            exit(EXIT_FAILURE);
        }
    }
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    printf("global = %d\n",global);
}