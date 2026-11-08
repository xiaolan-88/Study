#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "shmfifo.h"

typedef struct person{
    int age;
    char name[32];
}person_t;

int main(void)
{
    pid_t cpid;
    int i;
    shm_fifo_t *fifo = shmfifo_init(3,sizeof(person_t));
    person_t person;

    cpid = fork();
    if(cpid == -1){
        perror("[ERROR] fork():");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){
        for(i = 0;i < 10;i++){
            strcpy(person.name,"lisi");
            person.age = 20;
            shmfifo_put(fifo,&person);
            sleep(1);
        }
        exit(EXIT_SUCCESS);
    }else if(cpid > 0){
        cpid = fork();
        if(cpid == -1){
            perror("[ERROR] fork():");
            exit(EXIT_FAILURE);
        }else if(cpid == 0){
            for(i = 0;i < 10;i++){
                strcpy(person.name,"zhangsan");
                person.age = 30;
                shmfifo_put(fifo,&person);
                sleep(2);
            }
            exit(EXIT_SUCCESS);
        }else if(cpid > 0){
            wait(NULL);
            wait(NULL);
        }
    }

    return 0;
}
