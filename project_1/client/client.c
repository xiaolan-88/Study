#include "shmfifo.h"
#include "client.h"
#include "msg.h"

#define BLOCK_CNT 10

static shm_fifo_t *g_shmfifo = NULL;
static int g_msgid = 0;
void client_init()
{
    g_shmfifo = shmfifo_init(BLOCK_CNT,sizeof(packet_t));
    g_msgid = msg_create();
}

void *do_client_subscribe(void *arg)
{
    char buffer[64] = {0};
    for(;;){
        memset(buffer,0,sizeof(buffer));
        msg_recv(g_msgid,getpid(),buffer,64);
        printf("<message> : %s\n",buffer);
    }
}

void subscribe(char *topic)
{
    packet_t packet;
    pthread_t tid;
    int err;
    err = pthread_create(&tid,NULL,do_client_subscribe,NULL);
    if(err != 0){
        fprintf(stderr,"[ERROR] pthread_create():%s",strerror(err));
        return;
    }
    pthread_detach(tid);

    strcpy(packet.topic,topic);
    packet.pid = getpid();
    packet.mode = SUBSCRIBE;
    shmfifo_put(g_shmfifo,&packet);
}

void publish(char *topic,char *content)
{
    packet_t packet;
    strcpy(packet.topic,topic);
    strcpy(packet.content,content);
    packet.pid = getpid();
    packet.mode = PUBLISH;
    shmfifo_put(g_shmfifo,&packet);
}
