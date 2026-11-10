#include "msg.h"

#define PATHNAME "../msg"
#define PRO_ID 102

#define MSG_SZ 64
struct msgbuf{
    long mtype;
    char mtext[MSG_SZ];
};

int msg_create()
{
    key_t key;
    int msgid;
    key = ftok(PATHNAME,PRO_ID);
    msgid = msgget(key,0);
    if(msgid == -1){
        msgid = msgget(key,IPC_CREAT|0644);
        if(msgid == -1){
            perror("[ERROR] msgget()");
            return -1;
        }
    }
    return msgid;
}

int msg_send(int msgid,long mtype,char *buf)
{
    int ret;
    struct msgbuf msg;
    msg.mtype = mtype;
    strcpy(msg.mtext,buf);
    ret = msgsnd(msgid,(const void *)&msg,strlen(msg.mtext)+ 1,0);
    if(ret == -1){
        perror("[ERROR] msgsnd():");
        return -1;
    }
    return ret;
}

int msg_recv(int msgid,long mtype,char *buf,int len)
{
    struct msgbuf msg;
    int ret;

    ret = msgrcv(msgid,(void *)&msg,len,mtype,0);
    if(ret == -1){
        perror("[ERROR] msgrcv():");
        return -1;
    }
    strcpy(buf,msg.mtext);
    return ret;
}

int msg_del(int msgid)
{
    return msgctl(msgid,IPC_RMID,NULL);
}