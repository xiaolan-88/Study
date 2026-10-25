#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PATHNAME "."
#define PROJ_ID 88

//使用 ipcs -q 查看msgid

#define MSG_TYPE 101
#define MSG_SZ 64

struct msgbuf{
    long mtype; //消息类型
    char mtext[MSG_SZ]; //消息内容
};

int main(void)
{
    key_t key; //IPC键值，用于定位消息队列
    int msgid; //消息队列标识符
    int ret;
    struct msgbuf msg;

    // ftok：根据文件路径+项目ID生成唯一key
    key = ftok(PATHNAME,PROJ_ID);
    if(key == -1){
        perror("[ERROR] ftok():");
        exit(EXIT_FAILURE);
    }

    // msgget：创建/打开消息队列
    // IPC_CREAT : 不存在对象返回0  已存在对象返回已存在对象的id
    // IPC_CREAT | IPC_EXCL : 不存在对象返回0  已存在对象返回-1
    msgid = msgget(key, IPC_CREAT | 0644);
    if(msgid == -1){
         perror("[ERROR] msgget():");
        exit(EXIT_FAILURE);
    }
    printf("msgid : %d\n",msgid);

    msg.mtype = MSG_TYPE;
    strcpy(msg.mtext, "Hello,msg queue");
    ret = msgsnd(msgid, (const void *)&msg,strlen(msg.mtext),0);
    if(ret == -1){
        perror("[ERROR] msgsnd():");
        exit(EXIT_FAILURE);
    }

#if 0 
    //删除消息队列
    /* 
    msgctl(int msqid, int cmd, struct msqid_ds *buf)
        cmd : IPC_RMID 删除消息队列属性,第三个参数为NULL
              IPC_STAT 获取消息队列属性  
              IPC_SET  设置消息队列属性

        buf : 消息队列属性结构体对象指针
    返回值: 成功返回0
           失败返回-1，并设置errno
    */
    ret = msgctl(msgid, IPC_RMID, NULL);
#endif
    

    return 0;
}