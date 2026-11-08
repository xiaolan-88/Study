#include <stdio.h>
#include <string.h>

#include "shm.h"

#define SHM_SZ 256

int main(void)
{
    enum shm_create_status shm_status;
    int shmid;
    void *addr = NULL;
    char buffer[16] = {0};

    //1.创建共享内存/获取共享内存，得到ID
    shm_status = shm_creat(SHM_SZ,&shmid);
    if(shm_status == SHM_CREAT_NEW){
        printf("Shared memory create new.\n");
    }else if(shm_status == SHM_HAS_EXIST){
        printf("Shared memory has exist\n");
    }

    //2.获取共享内存映射后的虚拟地址
    addr = shm_at(shmid);

    //3.获得共享内存里的10个字节数据
    memcpy(buffer,addr,10);
    printf("buffer = %s\n",buffer);

    //4.解除映射
    shm_dt(addr);

    //5.删除共享内存
    shm_del(shmid);

    return 0;
}
