#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define LOGIN_SUCCESS 1
#define LOGIN_FAILURE 0

void recv_data(int new_sockfd)
{
    int n = 0;
    char buf[1024] = {0};
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);
    while(1)
    {
        memset(buf,0,sizeof(buf));
        n = recvfrom(new_sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&len);

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
    close(new_sockfd);
    //结束当前子进程
	exit(EXIT_SUCCESS);
}

int init_socket(const char *ip,const char *port)
{
    int sockfd;
    struct sockaddr_in my_addr;
    int len = sizeof(my_addr);

    //1.通过socker创建文件描述符
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
        perror("Fail to socket()");
        exit(EXIT_FAILURE);
    }

    //2.填充服务器自己的 ip + 端口
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(port));
    my_addr.sin_addr.s_addr = inet_addr(ip);

    //3.把ip + port 与 socker绑定
    if(bind(sockfd,(struct sockaddr *)&my_addr,len) < 0)
    {
        perror("Fail to bind");
        exit(EXIT_FAILURE);
    }
    printf("wait recv data!\n");

    return sockfd;
}

int user_login(const char *ip,const char *port)
{
    int sockfd;
    int n = 0;
    char buf[20] = {0};
    struct sockaddr_in client_addr;
    int len = sizeof(client_addr);
    unsigned char login_flag;
    int new_sockfd;

    //1.创建套接字
    sockfd = init_socket(ip,port);

    //2.接收登陆信息,父进程负责验证密钥信息，子进程负责与用户进行数据交互
    while(1)
    {
        memset(buf,0,sizeof(buf));
        n = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,&len);

        if(n < 0){
            perror("Fail to recvfrom()");
            exit(EXIT_FAILURE);
        }

        printf("key = %s\n",buf);

        login_flag = (strncmp(buf,"root",4) == 0) ? LOGIN_SUCCESS : LOGIN_FAILURE;

        if(login_flag == LOGIN_SUCCESS)
        {
            //创建子进程，准备数据的交互
            if(fork() == 0)
            {
                close(sockfd);

                //绑定0号端口，系统会随机分配一个可用的端口
                new_sockfd = init_socket(ip,"0");

                //给客户端的回复-----登录成功
                sendto(new_sockfd,&login_flag,sizeof(login_flag),0,(struct sockaddr *)&client_addr,len);
                break;
            }
        }else{
            //给客户端回复=====登录失败
            sendto(sockfd,&login_flag,sizeof(login_flag),0,(struct sockaddr *)&client_addr,len);
        }
    }
    return new_sockfd;

}

void signal_handler(int signum)
{
    waitpid(-1,NULL,WNOHANG);
    printf("recv signum = %d zombie\n",signum);
    return;
}

int main(int argc,char *argv[])
{
    int sockfd;
    if(argc != 3)
    {
        fprintf(stderr,"Usage : %s < ip > < port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    //0.主进程回收僵尸态子进程
    if(signal(SIGCHLD,signal_handler) == SIG_ERR)
    {
        perror("Fail to signal");
        return -1;
    }

    //1.接收用户的密钥，准备登陆
    sockfd = user_login(argv[1],argv[2]);
   
    //2.接收数据
    recv_data(sockfd);

    //3.关闭文件描述符
    close(sockfd);

    return 0;
}
