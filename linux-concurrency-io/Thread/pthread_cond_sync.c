#include <stdio.h>      // printf、fprintf
#include <stdlib.h>     // atoi、exit、EXIT_FAILURE
#include <string.h>     // strerror
#include <pthread.h>    // 线程、互斥锁、条件变量
#include <unistd.h>     // sleep

// 模拟仓库：记录当前产品数量，是生产者和消费者共享的数据
static int home = 0;

// 互斥锁：保护 home，确保同一时刻只有一个线程读写它
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 条件变量：仓库为空时，消费者可以在这里等待“有产品”的通知
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


// 生产者线程的入口函数。
// arg 是创建线程时传入的参数，这里是一个表示生产数量的字符串。
void *thread_handler(void *arg)
{
    int cnt, i, tmp;            // cnt：本线程要生产的数量；tmp 当前未使用
    cnt = atoi((char *)arg);    // 把字符串转换成整数，例如 "3" -> 3

    // 本线程循环生产 cnt 个产品
    for (i = 0; i < cnt; i++) {
        pthread_mutex_lock(&mutex); // 加锁：开始访问共享变量 home

        home++;                     // 生产一个产品，仓库数量加一
        printf("线程[%ld] 生产了一个产品，产品数量为：%d\n",
               pthread_self(), home);

        pthread_mutex_unlock(&mutex); // 解锁：允许其他线程访问 home

        // 通知可能正在等待的消费者：仓库可能有产品了。
        // 条件变量通知本身不负责保护 home，home 仍由 mutex 保护。
        pthread_cond_signal(&cond);
    }

    pthread_exit(NULL); // 结束当前生产者线程
}


// 示例：./a.out 1 2 3 4
// argc 为 5；主线程会创建 4 个生产者，分别生产 1、2、3、4 个产品。
int main(int argc, char *argv[])
{
    int produce = 0; // 所有生产者计划生产的总数量
    int consume = 0; // 主线程已经消费的总数量
    int flag;        // 是否已经达到计划消费总量
    pthread_t tid;   // 保存线程 ID（每次创建都会覆盖之前的值）
    int i, err;

    // 从 argv[1] 开始，因为 argv[0] 是程序名
    for (i = 1; i < argc; i++) {
        // argv[i] 是命令行参数字符串，例如 "3"
        produce += atoi(argv[i]);

        // 创建一个生产者线程，并把本次生产数量的字符串传给它
        err = pthread_create(&tid, NULL, thread_handler, (void *)argv[i]);
        if (err != 0) {
            // pthread_create 返回错误码；strerror 将错误码转成文字
            fprintf(stderr, "[ERROR] pthread_create(): %s\n", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    // 主线程充当消费者
    while (1) {
        pthread_mutex_lock(&mutex); // 加锁后检查并修改仓库数量

        // 仓库为空时，消费者不能消费，等待生产者的通知。
        // pthread_cond_wait 会原子地：
        // 1. 释放 mutex 并进入等待；
        // 2. 被唤醒后重新竞争 mutex；
        // 3. 重新拿到 mutex 后才返回。
        //
        // 用 while 而不是 if，是因为被唤醒不代表仓库一定还有产品：
        // 可能是虚假唤醒，也可能是其他消费者先消费了产品。
        while (home == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        // 仓库有产品时，主线程持续消费，直到仓库再次为空
        while (home > 0) {
            consume++; // 消费总数加一
            printf("消费一个产品，产品数量：%d\n", --home);

            // 已消费数量达到计划生产总量时，标记可以结束
            flag = consume >= produce ? 1 : 0;

            // 暂停一秒，方便观察生产、消费过程。
            // 注意：此时仍持有 mutex，生产者也会被阻塞。
            sleep(1);
        }

        pthread_mutex_unlock(&mutex); // 完成本轮消费后释放锁

        // 消费数量达到计划总量，主线程退出循环
        if (flag)
            break;
    }

    pthread_mutex_destroy(&mutex); // 销毁互斥锁
    return 0;                       // 结束主线程，进程退出
}