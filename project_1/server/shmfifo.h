#ifndef __SHM_FIFO_H_
#define __SHM_FIFO_H_

#include "shm.h"
#include "sem.h"


typedef struct shm_head{
    int rpos; //出队列的位置
    int wpos; //入队列的位置

    int blocks; //数据块的数量
    int blksz; //数据块的大小

    int semid; //信号量集合的id
}shm_head_t;

typedef struct shm_fifo{
    shm_head_t *p_head;
    char *p_payload;
}shm_fifo_t;

//创建环形队列，初始化
extern shm_fifo_t *shmfifo_init(int block,int blksz);
// //清空队列；应在没有客户端读写时调用
// extern int shmfifo_reset(shm_fifo_t *fifo);
//销毁环形队列
extern void shmfifo_destroy(shm_fifo_t *fifo);
//写入环形队列
extern void shmfifo_put(shm_fifo_t *fifo,const void *buf);
//读取环形队列
extern void shmfifo_get(shm_fifo_t *fifo,void *buf);

#endif
