#include "sem.h"
#include <unistd.h>

#define SEM_CONTROL_P 0 //控制父进程的信号量的编号
#define SEM_CONTROL_C 1 //控制子进程的信号量的编号

//信号量互斥，每次只有一个进程占用信号
int main(void)
{
    pid_t cpid;
    int semid;

    unsigned short values[2]= {1,0};//信号量的值
    //释放+1，占用-1
    //只有信号量的值 > 0时才会执行

    semid = sem_create(2, values);//信号量集合中只创建两个信号量，信号量的值分别是 1 0，对应 P C
    if(semid == -1){
        return -1;
    }

    cpid = fork();
    if(cpid == -1){
        perror("[ERROR] fork():");
    }else if(cpid == 0){
        for(;;){
            sem_p(semid,SEM_CONTROL_C);// C = 1 ==> 0
            printf("B");
            fflush(stdout);
            sem_v(semid,SEM_CONTROL_P);// P = 0 ==> 1 跳到父进程
        }                                                          
    }else if(cpid > 0){
       for(;;){
            sem_p(semid,SEM_CONTROL_P); // P = 1 ==> 0
            printf("A");
            fflush(stdout);
            sem_v(semid,SEM_CONTROL_C);// C = 0 ==> 1 跳到子进程
            sem_p(semid,SEM_CONTROL_P);// P = 1 ==> 0 从子进程跳到这里接着执行
            printf("A");
            fflush(stdout);
            sem_v(semid,SEM_CONTROL_P);// P = 0 ==> 1
            sleep(1);
            putchar('\n');
        }
    }
    return 0;
}