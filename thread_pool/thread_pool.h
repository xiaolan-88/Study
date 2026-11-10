#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct tpool_task{
    void (* function)(void *arg);
    void *arg;
}tpool_task_t;

typedef struct tpool{
    tpool_task_t *tp_task;

    int tp_qcapacity;//容量
    int tp_qsize;//长度

    int tp_qfront;//队尾
    int tp_qrear;//队头
    
    pthread_t *tp_work_thread_ids;//指向存储线程id空间的指针
    int tp_number_of_threads;//总的线程数量

    pthread_mutex_t tp_mutex_pool;//线程互斥锁

    pthread_cond_t tp_cond_empty;//队列是否为空
    pthread_cond_t tp_cond_full;//队列是否为满

    bool tp_shutdown;// 销毁标志
}tpool_t;

extern tpool_t *thread_pool_init(int cnt,int queuesize);
extern void thread_pool_add_task(tpool_t *pool,void (*task)(void *arg),void *arg);
extern int thread_pool_destroy(tpool_t *pool);
extern void *tp_worker(void *arg);

#endif