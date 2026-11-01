#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void recv_data(int sockfd)
{
    int n = 0;
    char buf[1024] = {0};
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);
    while(1)
    {
        memset(buf,0,sizeof(buf));
        n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&len);

        if(n < 0){
            perror("Fail to recvfrom()");
            exit(EXIT_FAILURE);
        }

        printf("==========================\n");
        printf("Recv from IP = %s\n",inet_ntoa(client_addr.sin_addr));
        printf("Recv from Port = %d\n",ntohs(client_addr.sin_port));
        printf("Recv %d bytes : %s\n",n,buf);

        if(strncmp(buf,"quit",4) == 0){
            break;
        }
       
    }
	return;
}


int main(int argc,char *argv[])
{
    int sockfd;
    struct sockaddr_in my_addr;
    int len = sizeof(my_addr);
    if(argc != 3)
    {
        fprintf(stderr,"Usage : %s < ip > < port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    //通过socker创建文件描述符
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
        perror("Fail to socket()");
        exit(EXIT_FAILURE);
    }

    //填充服务器自己的 ip + 端口
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[2]));
    my_addr.sin_addr.s_addr = inet_addr(argv[1]);

    //把ip + port 与 socker绑定
    if(bind(sockfd,(struct sockaddr *)&my_addr,len) < 0)
    {
        perror("Fail to bind");
        exit(EXIT_FAILURE);
    }
    printf("wait recv data!\n");

    //接收数据
    recv_data(sockfd);

    close(sockfd);

    return 0;
}
