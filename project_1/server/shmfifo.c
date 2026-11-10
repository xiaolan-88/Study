#include "shmfifo.h"

static int g_shmid;

#define SEM_EMPTY_ID 0 // 0
#define SEM_FULL_ID 1  // blocks
#define SEM_MUTEX_ID 2 // 1


shm_fifo_t *shmfifo_init(int blocks,int blksz)
{
    shm_fifo_t *p_shm_fifo = malloc(sizeof(shm_fifo_t));
    int shm_sz = 0;
    enum shm_create_status s_status;
    unsigned short values[] = {0,blocks,1};

    if(!p_shm_fifo)
        return NULL;
    //计算共享内存大小
    shm_sz = blocks * blksz + sizeof(shm_head_t);

    s_status = shm_creat(shm_sz,&g_shmid);
    if(s_status != SHM_CREAT_ERR){
        p_shm_fifo->p_head = (shm_head_t *)shm_at(g_shmid);
        if(s_status == SHM_CREAT_NEW){
            p_shm_fifo->p_head->rpos = 0;
            p_shm_fifo->p_head->wpos = 0;
            p_shm_fifo->p_head->blocks = blocks;
            p_shm_fifo->p_head->blksz = blksz;
            p_shm_fifo->p_head->semid = sem_create(3,values);
        }
        p_shm_fifo->p_payload = (char *)(p_shm_fifo->p_head + 1);
        return p_shm_fifo;
    }
    return NULL;
    
}

// int shmfifo_reset(shm_fifo_t *fifo)
// {
//     unsigned short values[] = {0, fifo->p_head->blocks, 1};

//     fifo->p_head->rpos = 0;
//     fifo->p_head->wpos = 0;
//     return sem_setall(fifo->p_head->semid, values);
// }

void shmfifo_destroy(shm_fifo_t *fifo)
{
    sem_del(fifo->p_head->semid);
    shm_dt(fifo->p_head);
    shm_del(g_shmid);
    free(fifo);
}

void shmfifo_put(shm_fifo_t *fifo,const void *buf)
{
    int pos = 0;

    sem_p(fifo->p_head->semid,SEM_FULL_ID);
    sem_p(fifo->p_head->semid,SEM_MUTEX_ID);

    //计算偏移量
    pos = fifo->p_head->wpos * fifo->p_head->blksz;
    memcpy(fifo->p_payload + pos,buf,fifo->p_head->blksz);//拷贝数据到环形队列
    fifo->p_head->wpos = (fifo->p_head->wpos + 1) % (fifo->p_head->blocks);

    sem_v(fifo->p_head->semid,SEM_MUTEX_ID);
    sem_v(fifo->p_head->semid,SEM_EMPTY_ID);

}
void shmfifo_get(shm_fifo_t *fifo,void *buf)
{
    int pos = 0;

    sem_p(fifo->p_head->semid,SEM_EMPTY_ID);
    sem_p(fifo->p_head->semid,SEM_MUTEX_ID);

    //计算偏移量
    pos = fifo->p_head->rpos * fifo->p_head->blksz;

    //从环形队列中拷贝数据
    memcpy(buf,fifo->p_payload + pos,fifo->p_head->blksz);
    fifo->p_head->rpos = (fifo->p_head->rpos + 1) % (fifo->p_head->blocks);

    sem_v(fifo->p_head->semid,SEM_MUTEX_ID);
    sem_v(fifo->p_head->semid,SEM_FULL_ID);
    
}