#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    printf("pid : %d ppid : %d\n",getpid(),getppid());
    return 0;

}