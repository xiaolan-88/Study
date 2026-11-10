#include "shm.h"

#define PATHNAME "../shm"
#define PRO_ID 101

//共享内存创建
extern enum shm_create_status shm_creat(size_t size,int *pshmid)
{
    key_t key;
    int shmid;

    key = ftok(PATHNAME,PRO_ID);
    //获取/创建共享内存
    shmid = shmget(key,size,0);
    if(shmid == -1){
        shmid = shmget(key,size,IPC_CREAT|0644);
        if(shmid == -1){
            perror("[ERROR] shmget():");
            return SHM_CREAT_ERR;
        }

        *pshmid = shmid;
        return SHM_CREAT_NEW;
    }else{
        *pshmid = shmid;
        return SHM_HAS_EXIST;
    }
}
//共享内存映射
void *shm_at(int shmid)
{
    void *addr = NULL;
    addr = shmat(shmid,NULL,0);//目的是把原本的失败返回值-1转为NULL
    if(addr == (void*)-1){
        perror("[ERROR] shmat():");
        return NULL;
    }
    return addr;
}

//解除映射
int shm_dt (const void *shmaddr)
{
    return shmdt(shmaddr);
}

//删除共享内存
int shm_del(int shmid)
{
    return shmctl(shmid,IPC_RMID,NULL);
}
