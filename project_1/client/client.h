#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#define TOPIC_SZ 64
#define CONTENT_SZ 64

enum work_mode{
    PUBLISH = 0,//发布消息
    SUBSCRIBE,//订阅消息
};

typedef struct packet{
    char topic[TOPIC_SZ];
    pid_t pid;
    enum work_mode mode;
    char content[CONTENT_SZ];
}packet_t;

extern void client_init();
extern void subscribe(char *topic);//订阅消息
extern void publish(char *topic,char *content);//发布消息
#endif