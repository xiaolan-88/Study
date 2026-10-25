#include "sem_stdout/sem.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>


#define SEM_PATHNAME "."
#define PROJ_ID 103

union semun{
    int val;
};

int main(void)
{
    key_t key;
    int semid;
    int ret;
    union semun s;
    
    key = ftok(SEM_PATHNAME, PROJ_ID);
    if(key == -1){
        perror("[ERROR] ftok():");
        exit(EXIT_FAILURE);
    }

    //创建信号量集合
    /*
        semget(key_t key, int nsems, int semflg)
        nsems : 信号量的数量
        semflg : 信号量集合的标志
            IPC_CREAT
            IPC_CREAT|IPC_EXCL
    */
    
    semid = semget(key, 1, IPC_CREAT|0644);
    if(semid == -1){
        perror("[ERROR] semget( ):");
        exit(EXIT_FAILURE);
    }

    //信号量的值设为1
    s.val = 1;
    //信号量初始化
    /*
        semctl(int semid, int semnum, int cmd, ...)
            semid : 信号量集合的id
            semnum : 信号量的编号
            cmd : 命令控制字
                SETVAL : 设置信号量的值
                GETVAL : 获取信号量的值
            ... : 可变参数列表
    */
    ret = semctl(semid,0,SETVAL,s);
    if(ret == -1){
        perror("[ERROR] semctl( ):");
        exit(EXIT_FAILURE);
    }

    semctl(semid,0,IPC_RMID,NULL);

}