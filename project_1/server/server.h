#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define TOPIC_SZ 64
#define CONTENT_SZ 64

//数据包工作模式
enum work_mode{
    PUBLISH = 0,//发布消息
    SUBSCRIBE,//订阅消息
};

//数据包类型结构体
typedef struct packet{
    char topic[TOPIC_SZ];//主题
    pid_t pid;//pid
    enum work_mode mode;//工作模式
    char content[CONTENT_SZ];//存储数据包内容
}packet_t;
//初始化服务器
extern void server_init();
//执行事件循环
extern void runloop();
//处理客户端订阅的消息
extern void do_subscribe(char *topic,pid_t pid);
//处理客户端发布的消息
extern void do_publish(char *topic,char *content);
#endif