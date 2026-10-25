#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int ret;

    ret = execl("/bin/ls","ls","-l",NULL);
    if(ret == -1){
        perror("[ERROR] execl() :");
        exit(EXIT_FAILURE);
    }

    printf("Process running\n");//由于execl进行了进程的替换，导致原进程的代码不执行
    return 0;
}