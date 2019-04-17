# 基于标准C实现的信号量管理结构体Semaphore - DumpDoctorWang的博客 - CSDN博客





2018年04月02日 10:20:51[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：174








## 一、缘起

就是闲的慌，写写技术博客，熟悉下C语言。顺便提示下，本篇文章实现的Semaphore只能在Linux使用，对于Windows平台，大家也根据同样的思路来实现一个。这个结构体的作用是在程序中使用很多信号量时，通过类似于C++的类的方式来调用来避免混淆各个信号量。

## 二、实现

[1、信号量机制简介](https://www.cnblogs.com/han-bing/p/6166391.html)

2、实现

放上代码，里面有详细的注释。

```
//Semaphore.h
#include <semaphore.h>

typedef struct Semaphore {
    char name[200];//信号量结构体名字,不超过200个字符,应是只读变量
    sem_t _sem;    //标准信号量，应是私有变量
    struct Semaphore *this;//指向自己的指针，只读变量，不能修改

    //接下来的这几行，每行定义了一个函数指针，指向一个函数，不了解的可以去了解一下。
    void (*init)(struct Semaphore *sem,char name[], unsigned value);
    void (*wait)(struct Semaphore *sem);
    void (*wait_time)(struct Semaphore *sem,int ms);
    void (*signal)(struct Semaphore *sem);
    void (*destroy)(struct Semaphore *sem);
}Semaphore;

//初始化信号量的第一种方式
void init_semaphore(Semaphore *sem);
//初始化信号量的第二种方式
Semaphore *get_semaphore();
```



```
//Semaphore.c
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>
#include "Semaphore.h"

/**
 * 初始化信号量
 * @param name
 */
void init(Semaphore *sem,char name[], unsigned value) {
    strcpy(sem->name,name);
    sem_init(&(sem->_sem), 0, value);
}

/**
 * 相当于信号量机制里面的P操作.
 * 有资源时,函数会立即返回,否则会阻塞调用此函数的线程.
 */
void wait(Semaphore *sem) {
    sem_wait(&(sem->_sem));
}

/**
 * 相当于信号量机制里面的P操作.
 * 有资源时,函数会立即返回,否则会阻塞调用此函数的线程.
 * 但如果等待时间超过ms指定的值，会唤醒所有阻塞线程.
 * @param ms 等待时间(ms)
 */
void wait_time(Semaphore *sem,int ms) {
    struct timespec t;
    t.tv_sec = ms / 1000;
    t.tv_nsec = (ms % 1000) * 1000;
    sem_timedwait(&(sem->_sem), &t);
}

/**
 * 如果有阻塞的线程,则随机唤醒一个线程，相当于
 * 信号量机制里面的V操作.否则立即返回.
 */
void signal(Semaphore *sem) {
    sem_post(&(sem->_sem));
}

/**
 * 销毁信号量
 */
void destroy(Semaphore *sem) {
    sem_destroy(&(sem->_sem));
    free(sem);
    sem = NULL;
}

/**
 * 初始化信号量结构体
 * @param sem 指向信号量的指针
 */
void init_semaphore(Semaphore *sem) {
    sem->this = sem;
    sem->init = init;
    sem->wait = wait;
    sem->wait_time = wait_time;
    sem->signal = signal;
    sem->destroy = destroy;
}

/**
 * 初始化信号量
 * @return 返回指向一个信号量的指针
 */
Semaphore *get_semaphore() {
    Semaphore *sem;
    sem = (Semaphore *)malloc(sizeof(Semaphore));
    if(sem == NULL){
        printf("申请内存失败，正在退出...");
        exit(-1);
    }

    sem->this = sem;
    sem->init = init;
    sem->wait = wait;
    sem->wait_time = wait_time;
    sem->signal = signal;
    sem->destroy = destroy;

    return sem;
}
```

使用：一个简单的生产者---消费者模型

```
//main.c
#include "Semaphore.h"
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

struct Goods{
    int id;//产品id
};//产品

Semaphore *g_sem;//信号量

struct Goods g_goods;//商品货架

pthread_mutex_t g_mutex;//互斥锁

/**
 * 生产这回调函数，生产商品
 * @param arg 未使用
 * @return 未使用
 */
void *producer(void *arg){

    int i=0;
    while (++i<5*5) {
        //休眠一段随机时间,代表生产过程
        sleep((unsigned)rand()%3+1);
        //生产产品
        pthread_mutex_lock(&g_mutex);
        g_goods.id = rand()%1000;
        printf("生产产品:%d\n",g_goods.id);
        pthread_mutex_unlock(&g_mutex);
        //唤醒一个阻塞的消费者线程
        g_sem->signal(g_sem->this);
    }
}

/**
 * 消费者回调函数
 * @param arg 消费者编号
 * @return 未使用
 */
void *costumer(void *arg){
    int i=0;
    int id = *((int *)arg);
    while (++i<5) {
        //有资源会立即返回,没有资源则会等待
        g_sem->wait(g_sem->this);
        //消费
        pthread_mutex_lock(&g_mutex);
        printf("消费者%d消费产品:%d\n",id,g_goods.id);
        pthread_mutex_unlock(&g_mutex);
    }
}

int main() {
    //获取信号量
    g_sem = get_semaphore();
    //初始化信号量
    g_sem->init(g_sem->this,"sem", 0);
    //打印信号量名字
    printf("sem name:%s\n",g_sem->name);
    //设置随机数种子
    srand((unsigned)time(NULL));
    //初始化互斥锁
    pthread_mutex_init(&g_mutex,NULL);

    pthread_t producer_t;   //生产者线程
    pthread_t costumer_t[5];//5个消费者线程

    //创建生产者线程
    pthread_create(&producer_t,NULL,producer,NULL);

    //创建消费者线程
    for(int i=0;i<5;i++){
        pthread_t p;
        costumer_t[i] = p;
        pthread_create(&(costumer_t[i]),NULL,costumer,(void *)&i);
    }

    //等待生产者线程退出
    pthread_join(producer_t,NULL);

    //等待消费者线程退出
    for(int i=0;i<5;i++){
        pthread_join(costumer_t[i],NULL);
    }

    //销毁互斥锁
    pthread_mutex_destroy(&g_mutex);
    //销毁信号量
    g_sem->destroy(g_sem->this);
    return 0;
}
```

编译命令：

```
gcc main.c Semaphore.c -o main -l pthread
```

最后安利一波我自己写的多线程编程模型,非常简单好用!

[https://github.com/Mannix1994/ThreadPool](https://github.com/Mannix1994/ThreadPool)





