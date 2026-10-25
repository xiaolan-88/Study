#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define SHM_PATHNAME "."
#define SHM_PROJ_ID 102
#define SHM_SZ 256

//使用 ipcs -m 查看shm

int main(void)
{
    key_t key;
    int shmid;
    int ret;
    void *addr = NULL; //保存映射之后的起始地址
    char buffer[16] = {0};

    // ftok：根据文件路径+项目ID生成唯一key
    key = ftok(SHM_PATHNAME,SHM_PROJ_ID);
    if(key == -1){
        perror("[ERROR] ftok():");
        exit(EXIT_FAILURE);
    }

    // shmget：创建/打开共享内存
    // IPC_CREAT : 不存在对象返回0  已存在对象返回已存在对象的id
    // IPC_CREAT | IPC_EXCL : 不存在对象返回0  已存在对象返回-1
    shmid = shmget(key,SHM_SZ,IPC_CREAT|0644);
    if(shmid == -1){
        perror("[ERROR] msgget():");
        exit(EXIT_FAILURE);
    }

    printf("shmid : %d\n",shmid);

    //将共享内存映射到进程的地址空间中
    /*
        void *shmat(int shmid, const void *shmaddr, int shmflg);
            const void *shmaddr : 指定挂载到进程的哪个虚拟地址
                                NULL : 让内核自动选一个合适地址
                                非NULL : 指定挂载地址

            shmflg :    0   读写权限，正常读写共享内存
                        SHM_RDONLY  只读挂载，不能写
        成功返回 0  
        失败返回 (void *)-1
            
    */
    addr = shmat(shmid,NULL,0);
    if(addr == (void *)-1){
        perror("[ERROR] shmat():");
        exit(EXIT_FAILURE);
    }
    //从addr中复制10字节数据到buffer中
    memcpy(buffer,addr,10);//将共享内存中的数据拷贝到数组中
    printf("buffer : %s\n",buffer);

#if 0
    /* 
    删除共享内存
    shmctl(int msqid, int cmd, struct msqid_ds *buf)
        cmd : IPC_RMID 删除共享内存属性,第三个参数为NULL
              IPC_STAT 获取共享内存属性  
              IPC_SET  设置共享内存属性

        buf : 共享内存属性结构体对象指针
    返回值: 成功返回0
           失败返回-1，并设置errno
    */
    ret = shmctl(shmid, IPC_RMID, NULL);
    if(ret == -1){
         perror("[ERROR] shmctl():");
        exit(EXIT_FAILURE);
    }
#endif

    

    return 0;
}