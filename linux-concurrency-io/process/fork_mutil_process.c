#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//给父进程返回的是子进程的pid，返回给子进程的是0
int main(void)
{
    pid_t cpid;

    cpid = fork();
    if(cpid == -1){
        perror("[ERROR]: fork()");
        exit(EXIT_FAILURE);
    }else if(cpid == 0){ //子进程A
        printf("Child A Process < %d >\n",getpid());
        sleep(2);
        exit(EXIT_SUCCESS);
    }else if(cpid > 0){
        cpid = fork();
        if(cpid == -1){
            perror("[ERROR]: fork()");
            exit(EXIT_FAILURE);
        }else if(cpid == 0){ //子进程B
            printf("Child B Process < %d >\n",getpid());
            sleep(3);
            exit(EXIT_SUCCESS);
        }else if(cpid > 0){

        }
    }

    return 0;

}