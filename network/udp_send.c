#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void send_data(int sockfd,struct sockaddr_in *addr,int len)
{
    int n = 0;
    char buf[1024] = {0};
    while(1)
    {
        putchar('>');
	fflush(stdout);
        memset(buf,0,sizeof(buf));
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf) - 1] = '\0';

        n = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)addr,len);

        if(n < 0){
            perror("Fail to sendto()");
            exit(EXIT_FAILURE);
        }

        if(strncmp(buf,"quit",4) == 0){
            break;
        }
       
    }
	return;
}


int main(int argc,char *argv[])
{
    int sockfd;
    struct sockaddr_in peer_addr;
    int len = sizeof(peer_addr);
    if(argc != 3)
    {
        fprintf(stderr,"Usage : %s < ip > < port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
        perror("Fail to socket()");
        exit(EXIT_FAILURE);
    }

    memset(&peer_addr,0,sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(atoi(argv[2]));
    peer_addr.sin_addr.s_addr = inet_addr(argv[1]);

    send_data(sockfd,&peer_addr,len);

    close(sockfd);

    return 0;
}
