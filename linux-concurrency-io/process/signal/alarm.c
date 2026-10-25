#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    unsigned int ret;

    ret = alarm(5);//设置定时时间为5秒，由内核来定时
    //返回值为 前一次设置定时器或者定时器时间已经用完

    sleep(2);

    ret = alarm(4);

    printf("ret = %d\n",ret); //ret = 3
    //从上次定时器定时开始，中间睡眠 2 秒，到这里定时器只剩 3 秒    

    return 0;
}