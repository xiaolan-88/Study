#ifndef __SHM_H__
#define __SHM_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
enum shm_create_status{
    SHM_HAS_EXIST = 0,
    SHM_CREAT_NEW,
    SHM_CREAT_ERR,
};

extern enum shm_create_status shm_creat(size_t size,int *pshmid);
extern void *shm_at(int shmid);
extern int shm_dt (const void *shmaddr);
extern int shm_del(int shmid);

#endif
