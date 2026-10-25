#include <stdio.h>
#include <stdlib.h>
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

    

    return 0;
}