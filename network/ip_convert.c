#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

void ip_convert_first(const char *ip,const char *port)
{
    in_addr_t net_addr;
    //ip字符串 ---》 网络字节序
    //inet_addr输入255.255.255.255也会返回-1，使用inet_aton()
    net_addr = inet_addr(ip);
    if(net_addr == -1){
        perror("[ERROR] inet_addr()");
        exit(EXIT_FAILURE);
    }
    printf("net_addr(addr) = %#x\n",net_addr);

}

void ip_convert_second(const char *ip,const char *port)
{
    //  ip字符串 ---》 网络字节序
    struct in_addr net_addr;//此结构体用来存放网络字节序
    int ret;
    ret = inet_aton(ip, &net_addr);
    if(ret == 0){ 
        perror("[ERROR] inet_aton()");
        exit(EXIT_FAILURE);
    }
    printf("net_addr(aton) = %#x\n",net_addr.s_addr);

    // port ---》 network port
    uint16_t net_port;
    uint16_t host_port;

    net_port = htons(atoi(port));
    printf("net_port(htons) = %#x\n",net_port);

    // networt port ---》 host port
    host_port = ntohs(net_port);
    printf("host_port(ntohs) = %d\n",host_port);

    // 网络字节序 ---》 IP字符串
    struct in_addr addr;
    addr.s_addr = net_addr.s_addr;
    printf("host_ip(ntoa) = %s\n",inet_ntoa(addr));
    
}

int main(int argc,char *argv[])
{
    if(argc != 3){
        fprintf(stderr,"Usage : %s  < ip >  < port >!\n",argv[0]);
        return -1;
    }
    ip_convert_first(argv[1],argv[2]);
    ip_convert_second(argv[1],argv[2]);
    return 0; 
}