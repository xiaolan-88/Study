#include "thread_pool.h"


/*
    创建并初始化线程池，同时启动指定数量的工作线程。
    @param cnt 工作线程数量
    @param queuesize 任务队列容量
*/
tpool_t *thread_pool_init(int cnt,int queuesize)
{
   
    int ret; // 保存 pthread 接口的返回值；成功时为 0。

    // 为线程池结构体分配内存。
    tpool_t *pool = (tpool_t *)malloc(sizeof(tpool_t));
    if(!pool)
        goto err;

    // 为工作线程 ID 数组分配空间，并将数组内容清零。
    pool->tp_work_thread_ids = (pthread_t *)malloc(sizeof(pthread_t) * cnt);
    if(!pool->tp_work_thread_ids)
        goto err;

    memset(pool->tp_work_thread_ids,0,sizeof(pthread_t) * cnt);
    pool->tp_number_of_threads = cnt; // 记录线程池中的工作线程数量。

    // 初始化互斥锁，用于保护线程池及任务队列的共享状态。
    ret = pthread_mutex_init(&pool->tp_mutex_pool,NULL);
    if(ret != 0)
        goto err;

    // 初始化条件变量：分别用于通知任务队列非空和非满。
    ret = pthread_cond_init(&pool->tp_cond_empty,NULL);
    if(ret != 0)
        goto err;
    ret = pthread_cond_init(&pool->tp_cond_full,NULL);
    if(ret != 0)
        goto err;

    // 按指定容量为任务队列分配空间。
    pool->tp_qcapacity = queuesize;
    pool->tp_task = (tpool_task_t *)malloc(sizeof(tpool_task_t) * pool->tp_qcapacity);
    if(!pool->tp_task)
        goto err;

    // 初始化循环队列的队首、队尾和当前任务数量。
    pool->tp_qfront = 0;
    pool->tp_qrear = 0;
    pool->tp_qsize = 0;
    pool->tp_shutdown = false; // 初始状态为运行中，尚未请求关闭线程池。

    // 创建工作线程；每个线程都从 tp_worker 开始执行。
    for(int i = 0;i < pool->tp_number_of_threads;i++){
        ret = pthread_create(&pool->tp_work_thread_ids[i],NULL,tp_worker,pool);
        if(ret != 0){
            // 创建失败时输出 pthread_create 提供的错误信息。
            fprintf(stderr,"[ERROR] pthread_create(): %s",strerror(ret));
            goto err;
        }
        // 将线程设为分离状态，使其结束时由系统自动回收线程资源。
        pthread_detach(pool->tp_work_thread_ids[i]);
    }

    // 初始化成功，返回线程池指针。
    return pool;

err:
    // 初始化失败时释放已经分配的任务队列、线程 ID 数组和线程池结构体。
    if(pool && pool->tp_task)
        free(pool->tp_task);
    if(pool && pool->tp_work_thread_ids)
        free(pool->tp_work_thread_ids);
    if(pool)
        free(pool);
}

// 工作线程入口
void *tp_worker(void *arg)
{
    //获取线程池数据结构对象
    tpool_t *pool = (tpool_t *)arg;
    tpool_task_t task;
    while(1){
        pthread_mutex_lock(&pool->tp_mutex_pool);

        //判断任务是否为空，如果为空，则让工作线程进入阻塞状态
        //队列里出现任务，条件不满足，开始执行队列里的任务
        while(pool->tp_qsize == 0 && !pool->tp_shutdown){
            pthread_cond_wait(&pool->tp_cond_empty,&pool->tp_mutex_pool);//条件变量睡眠，互斥锁释放，等待唤醒重新拿到锁
        }
        
        //如果队列被销毁（true），解锁
        if(pool->tp_shutdown){
            pthread_mutex_unlock(&pool->tp_mutex_pool);
            pthread_exit(NULL);
        }

        //取出任务并执行
        task.function = pool->tp_task[pool->tp_qfront].function;
        task.arg = pool->tp_task[pool->tp_qfront].arg;
        pool->tp_qfront = (pool->tp_qfront + 1) % pool->tp_qcapacity;
        pool->tp_qsize --;

        pthread_cond_signal(&pool->tp_cond_full);//唤醒生产者
        pthread_mutex_unlock(&pool->tp_mutex_pool);//解锁

        //执行任务
        printf("[DEBUG] Thread %ld start working...\n",pthread_self());
        task.function(task.arg);
        free(task.arg);
        printf("[DEBUG] Thread %ld end working...\n",pthread_self());
    }

}


/*
    向线程池的任务队列添加任务
    @param task 任务函数
    @param arg 传递给任务函数的参数。
*/
void thread_pool_add_task(tpool_t *pool,void (*task)(void *arg),void *arg)
{
    //获取线程池互斥锁
    pthread_mutex_lock(&pool->tp_mutex_pool);

    //判断任务队列是否已满，如果已满，则阻塞用户线程
    //队列里出现空位，不满足条件，继续生产任务
    while(pool->tp_qsize == pool->tp_qcapacity && !pool->tp_shutdown)
    {
        pthread_cond_wait(&pool->tp_cond_full,&pool->tp_mutex_pool);
    }

    //判断线程是否被销毁
    if(pool->tp_shutdown)
    {
        pthread_mutex_unlock(&pool->tp_mutex_pool);
        return;
    }

    //将任务添加到队列
    pool->tp_task[pool->tp_qrear].function = task;
    pool->tp_task[pool->tp_qrear].arg = arg;
    pool->tp_qrear = (pool->tp_qrear + 1) % pool->tp_qcapacity;
    pool->tp_qsize ++;

    //唤醒工作线程执行任务
    pthread_cond_signal(&pool->tp_cond_empty);//唤醒消费者
    //解锁 
    pthread_mutex_unlock(&pool->tp_mutex_pool);

    return;
}   

//销毁线程池
int thread_pool_destroy(tpool_t *pool)
{
    //如果pool不存在
    if(!pool)
        return -1;
    pool->tp_shutdown = true;

    //将阻塞在empty的线程唤醒，检查shutdown标志，正常退出线程
    for(int i = 0;i < pool->tp_number_of_threads;i++){
        pthread_cond_signal(&pool->tp_cond_empty);
    }
    if(pool->tp_task)
        free(pool->tp_task);
    if(pool->tp_work_thread_ids)
        free(pool->tp_work_thread_ids);
    pthread_mutex_destroy(&pool->tp_mutex_pool);
    pthread_cond_destroy(&pool->tp_cond_empty);
    pthread_cond_destroy(&pool->tp_cond_full);

    free(pool);
    pool = NULL;

    return 0;
}