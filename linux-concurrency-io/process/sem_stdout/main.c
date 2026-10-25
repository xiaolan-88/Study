#include "sem.h"
#include <unistd.h>

//信号量互斥，每次只有一个进程占用信号
int main(void)
{
    pid_t cpid;
    int semid;

    unsigned short values[]= {1};//信号量的值

    semid = sem_create(1, values);//信号量集合中只创建一个信号量
    if(semid == -1){
        return -1;
    }

    cpid = fork();
    if(cpid == -1){
        perror("[ERROR] fork():");
    }else if(cpid == 0){
        for(;;){
            sem_p(semid,0);//占用信号量资源，占用成功继续执行，否则阻塞
            printf("----------------------\n");
            printf("C start.\n");
            sleep(1);
            printf("C end.\n");
            printf("----------------------\n");
            sem_v(semid,0);//释放资源
        }
    }else if(cpid > 0){
       for(;;){
            sem_p(semid,0);
            printf("----------------------\n");
            printf("P start.\n");
            sleep(1);
            printf("P end.\n");
            printf("----------------------\n");
            sem_v(semid,0);
        }
    }
    return 0;
}