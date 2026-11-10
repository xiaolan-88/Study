#ifndef __SEM_H__
#define __SEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

extern int sem_create(int nsems,unsigned short values[]);//创建信号量集合
// extern int sem_setall(int semid,unsigned short values[]);//设置已有信号量集合的值
extern int sem_p(int semid,int semnum);//占用资源
extern int sem_v(int semid,int semnum);//释放资源
extern int sem_del(int semid);//删除信号量的集合
#endif