#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static int global = 0;//临界资源 所有线程都可以访问

static pthread_mutex_t mutex;//定义互斥锁

void *do_thread(void *arg)
{
    int loops = *(int *)arg;
    int i,tmp =0;
    for(i = 0;i < loops;i++){
        pthread_mutex_lock(&mutex);//竞争锁，一旦竞争成功，则获取临界资源的访问权限
        tmp = global;
        tmp ++;
        global = tmp;
        pthread_mutex_unlock(&mutex);
    }
}

//argv[0] : "./a.out"    argc[1] : "100"
int main(int argc,char *argv[])
{
    pthread_mutex_init(&mutex,NULL);//动态初始化锁
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

    pthread_mutex_destroy(&mutex);//互斥锁销毁
}