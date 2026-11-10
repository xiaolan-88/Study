#include "sem.h"
#include <sys/sem.h>

#define SEM_SET_PATHNAME "."
#define SEM_SET_PROJ_ID 104

union semun{
    unsigned short *array;// SETALL

};
/*
    创建信号量的集合
    @param nsems : 信号量的个数
    @param values : 信号量的值的数组
*/
int sem_create(int nsems, unsigned short *values)
{
    key_t key;
    int semid;
    int ret;
    union semun s;
    key = ftok(SEM_SET_PATHNAME, SEM_SET_PROJ_ID);
    if(key == -1){
        perror("[ERROR] ftok():");
        return -1;
    }

    semid = semget(key, nsems, IPC_CREAT|0644);
    if(semid == -1){
        perror("[ERROR] semget():");
        return -1;
    }

    s.array = values;

    ret = semctl(semid, 0, SETALL ,s);
    if(ret == -1){
        perror("[ERROR] semctl():");
        return -1;
    }

    return semid;

}

// int sem_setall(int semid, unsigned short *values)
// {
//     union semun s;

//     s.array = values;
//     return semctl(semid, 0, SETALL, s);
// }

/*
    占用资源
    @param semid : 信号量集合的id
    @param semnum : 信号量的编号
*/
int sem_p(int semid,int semnum)
{
    struct sembuf sops;
    sops.sem_num = semnum; //信号量编号
    sops.sem_op = -1;// -1 : 占用资源
    sops.sem_flg = SEM_UNDO;//进程终止，会自动释放信号量

    /*
        semop(int semid, struct sembuf *sops, size_t nsops)
        sops : 结构体指针
    */
    return semop(semid,&sops,1);
}

/*
    释放资源
    @param semid : 信号量集合的id
    @param semnum : 信号量的编号
*/
int sem_v(int semid,int semnum)
{
    struct sembuf sops;
    sops.sem_num = semnum;
    sops.sem_op = 1; // +1 释放资源
    sops.sem_flg = SEM_UNDO;//进程终止，会自动释放信号量

    return semop(semid,&sops,1);

}


/*
    删除信号量集合
    @param semid : 信号量集合的id
*/
int sem_del(int semid)
{
    return semctl(semid,0,IPC_RMID,NULL);
}
