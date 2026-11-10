#ifndef __MSG_H__
#define __MSG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

extern int msg_create();//创建消息队列
extern int msg_send(int msgid,long mtype,char *buf);//发送消息
extern int msg_recv(int msgid,long mtype,char *buf,int len);//接收消息
extern int msg_del(int msgid);//销毁消息队列
#endif