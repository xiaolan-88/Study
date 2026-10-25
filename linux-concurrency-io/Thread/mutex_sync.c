#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

static int home = 0;//仓库的值（临界资源）

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//静态初始化锁

//生产者线程执行函数
void *thread_handler(void *arg)
{
    int cnt,i,tmp;
    cnt = atoi((char *)arg);
    for(i = 0;i < cnt;i++){
        pthread_mutex_lock(&mutex);//锁定
        home++;
        printf("线程[%ld] 生产了一个产品，产品数量为：%d\n",pthread_self(),home);
        pthread_mutex_unlock(&mutex);//解锁
    }
    pthread_exit(NULL);

}

// ./a.out 1 2 3 4. -->  argc=5. 分别生产 1 ，2，3，4个产品
int main(int argc,char *argv[])
{
    int produce = 0;//记录生产总数
    int consume = 0;//记录消费总数
    int flag;
    pthread_t tid;
    int i,err;

    for(i = 1;i < argc;i++){
        //argv[i]为指针数组，atoi(const char *nptr)
        produce += atoi(argv[i]);
        err = pthread_create(&tid,NULL,thread_handler,(void *)argv[i]);
        if(err != 0){
            fprintf(stderr,"[ERROR] pthread_create(): %s\n",strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    //主线程为消费者
    while(1){
        pthread_mutex_lock(&mutex);//锁定

        while(home > 0){
            consume ++;
            printf("消费一个产品，产品数量：%d\n",--home);
            flag = consume >= produce ? 1 : 0;//判断消费是否超过生产
            sleep(1);
        }

        pthread_mutex_unlock(&mutex);//解锁
        //如果超过生产量或等于生产量则退出
        if(flag)
        break;
    }

    pthread_mutex_destroy(&mutex);
    return 0;

}
