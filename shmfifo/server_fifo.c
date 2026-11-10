#include <stdio.h>
#include <string.h>
#include "shmfifo.h"

typedef struct person{
    int age;
    char name[32];
}person_t;

int main(void)
{
    shm_fifo_t *fifo = shmfifo_init(3,sizeof(person_t));
    person_t person;

    for(;;){
        shmfifo_get(fifo,&person);
        printf("name = %s , age = %d\n",person.name,person.age);
    }

    return 0;
}

