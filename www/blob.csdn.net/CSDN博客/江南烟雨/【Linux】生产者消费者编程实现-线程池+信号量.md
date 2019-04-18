# 【Linux】生产者消费者编程实现-线程池+信号量 - 江南烟雨 - CSDN博客
2013年09月22日 09:38:53[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：9142
生产者消费者编程实现，采用了线程池以及信号量技术。
线程的概念就不多说，首先说一下多线程的好处：多线程技术主要解决处理器单元内多个线程执行的问题，它可以显著减少处理器单元的闲置时间，增加处理器单元的吞吐能力。
那么为什么又需要线程池呢？
我们知道应用程序创建一个对象，然后销毁对象是很耗费资源的。创建线程，销毁线程，也是如此。因此，我们就预先生成一些线程，等到我们使用的时候在进行调度，于是，一些"池化资源"技术就这样的产生了。
一般一个简单线程池至少包含下列组成部分。
1)线程池管理器（ThreadPoolManager）:用于创建并管理线程池
2)工作线程（WorkThread）: 线程池中线程
3)任务接口（Task）:每个任务必须实现的接口，以供工作线程调度任务的执行。
4)任务队列:用于存放没有处理的任务。提供一种缓冲机制。
图示：
![](https://img-blog.csdn.net/20130528141425575)
图1 线程池图解
生产者消费者模型C语言代码实现：
thread_pool_pv.h：
```cpp
//线程池编程实现
#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>//信号量sem_t
#include <pthread.h>
//任务接口,线程调用的函数
typedef void* (*FUNC)(void *arg);
//任务数据结构
typedef struct thread_pool_job_s{
        FUNC function;//线程调用的函数
        void *arg;//函数参数
        struct thread_pool_job_s *pre;//指向上一个节点
        struct thread_pool_job_s *next;//指向下一个节点
}thread_pool_job;
//工作队列
typedef struct thread_pool_job_queue_s{
        thread_pool_job *head;//队列头指针
        thread_pool_job *tail;//队列尾指针
        int num;//任务数目
        sem_t *quene_sem;//信号量
}thread_pool_job_queue;
//线程池(存放消费者进程)
typedef struct thread_pool_s{
        pthread_t *threads;//线程
        int threads_num;//线程数目
        thread_pool_job_queue *job_queue;//指向工作队列的指针
}thread_pool;
//typedef struct thread_data_s{
//      pthread_mutex_t *mutex_t;//互斥量
//      thread_pool *tp_p;//指向线程池的指针
//}thread_data;
//初始化线程池
thread_pool* tp_init(int thread_num);
//初始化工作队列
int tp_job_quene_init(thread_pool *tp);
//向工作队列中添加一个元素
void tp_job_quene_add(thread_pool *tp,thread_pool_job *new_job);
//向线程池中添加一个工作项
int tp_add_work(thread_pool *tp,void *(*func_p)(void *),void *arg);
//取得工作队列的最后个节点
thread_pool_job* tp_get_lastjob(thread_pool *tp);
//删除工作队列的最后个节点
int tp_delete__lastjob(thread_pool *tp);
//销毁线程池
void tp_destroy(thread_pool *tp);
//消费者线程函数
void* tp_thread_func(thread_pool *tp);
//生产者线程执行函数
void* thread_func_producer(thread_pool *tp);
#endif
```
thread_pool_pv.c：
```cpp
//线程池编程实现
#include "thread_pool.h"
//互斥量,用于对工作队列的访问
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//标记线程池是否处于可用状态
static int tp_alive = 1;
//初始化线程池
thread_pool* tp_init(int thread_num){
        thread_pool *tp;
        int i;
        if(thread_num < 1)
                thread_num = 1;
        tp = (thread_pool *)malloc(sizeof(thread_pool));
        //判断内存分配是否成功
        if(NULL == tp){
                printf("ERROR:allocate memory for thread_pool failed\n");
                return NULL;
        }
        tp->threads_num = thread_num;
        //分配线程所占内存空间
        tp->threads = (pthread_t*)malloc(thread_num * sizeof(pthread_t));
        //判断内存分配是否成功
        if(NULL == tp->threads){
                printf("ERROR:allocate memory for threads in thread pool failed\n");
                return NULL;
        }
        
        if(tp_job_quene_init(tp))
                return NULL;
        
        tp->job_queue->quene_sem = (sem_t *)malloc(sizeof(sem_t));
        sem_init(tp->job_queue->quene_sem,0,0);//信号量初始化
        //初始化线程
        for(i = 0;i < thread_num;++i){
                pthread_create(&(tp->threads[i]),NULL,(void *)tp_thread_func,(void *)tp);
        }
        return tp;
}
//初始化工作队列
int tp_job_quene_init(thread_pool *tp){
        tp->job_queue = (thread_pool_job_queue *)malloc(sizeof(thread_pool_job_queue));
        if(NULL == tp->job_queue){
                return -1;
        }
        tp->job_queue->head = NULL;
        tp->job_queue->tail = NULL;
        tp->job_queue->num = 0;
        return 0;
}
//线程函数
void* tp_thread_func(thread_pool *tp){
        FUNC function;
        void *arg_buf;
        thread_pool_job *job_p;
        while(tp_alive){
                //线程阻塞,等待信号量
                if(sem_wait(tp->job_queue->quene_sem)){
                        printf("thread waiting for semaphore....\n");
                        exit(1);
                }
                if(tp_alive){
                        pthread_mutex_lock(&mutex);
                        job_p = tp_get_lastjob(tp);
                        if(NULL == job_p){
                                pthread_mutex_unlock(&mutex);
                                continue;
                        }
                        function = job_p->function;
                        arg_buf = job_p->arg;
                        if(tp_delete__lastjob(tp))
                                return;
                        pthread_mutex_unlock(&mutex);
                        //运行指定的线程函数
                        printf("consumer...get a job from job quene and run it!\n");
                        function(arg_buf);
                        free(job_p);
                }
                else
                        return;
        }
        return;
}
//向工作队列中添加一个元素
void tp_job_quene_add(thread_pool *tp,thread_pool_job *new_job){
        new_job->pre = NULL;
        new_job->next = NULL;
        thread_pool_job *old_head_job = tp->job_queue->head;
        if(NULL == old_head_job){
                tp->job_queue->head = new_job;
                tp->job_queue->tail = new_job;
        }
        else{
                old_head_job->pre = new_job;
                new_job->next = old_head_job;
                tp->job_queue->head = new_job;
        }
        ++(tp->job_queue->num);
        sem_post(tp->job_queue->quene_sem);
}
//取得工作队列的最后一个节点
thread_pool_job* tp_get_lastjob(thread_pool *tp){
        return tp->job_queue->tail;
}
//删除工作队列的最后个节点
int tp_delete__lastjob(thread_pool *tp){
        if(NULL == tp)
                return -1;
        thread_pool_job *last_job = tp->job_queue->tail;
        if(0 == tp->job_queue->num){
                return -1;
        }
        else if(1 == tp->job_queue->num){
                tp->job_queue->head = NULL;
                tp->job_queue->tail = NULL;
        }
        else{
                last_job->pre->next = NULL;
                tp->job_queue->tail = last_job->pre;
        }
        //修改相关变量
        --(tp->job_queue->num);
        return 0;
}
//向线程池中添加一个工作项
int tp_add_work(thread_pool *tp,void *(*func_p)(void *),void *arg){
        thread_pool_job *new_job = (thread_pool_job *)malloc(sizeof(thread_pool_job));
        if(NULL == new_job){
                printf("ERROR:allocate memory for new job failed!\n");
                exit(1);
        }
        new_job->function = func_p;
        new_job->arg = arg;
        pthread_mutex_lock(&mutex);
        tp_job_quene_add(tp,new_job);
        pthread_mutex_unlock(&mutex);
}
//销毁线程池
void tp_destroy(thread_pool *tp){
        int i;
        tp_alive = 0;
        
        //等待线程运行结束
        //sleep(10);
        for(i = 0;i < tp->threads_num;++i){
                pthread_join(tp->threads[i],NULL);
        }
        free(tp->threads);
        if(sem_destroy(tp->job_queue->quene_sem)){
                printf("ERROR:destroy semaphore failed!\n");
        }
        free(tp->job_queue->quene_sem);
        //删除ｊｏｂ队列
        thread_pool_job *current_job = tp->job_queue->tail;
        while(tp->job_queue->num){
                tp->job_queue->tail = current_job->pre;
                free(current_job);
                current_job = tp->job_queue->tail;
                --(tp->job_queue->num);
        }
        tp->job_queue->head = NULL;
        tp->job_queue->tail = NULL;
}
//自定义线程执行函数
void* thread_func1(){
        printf("Task1 running...by Thread  :%u\n",(unsigned int)pthread_self());
}
//自定义线程执行函数
void* thread_func2(){
        printf("Task2 running...by Thread  :%u\n",(unsigned int)pthread_self());
}
//生产者线程执行函数
void* thread_func_producer(thread_pool *tp){
        while(1){
                printf("producer...add a job(job1) to job quene!\n");
                tp_add_work(tp,(void*)thread_func1,NULL);
                sleep(1);
                printf("producer...add a job(job2) to job quene!\n");
                tp_add_work(tp,(void*)thread_func2,NULL);
        }
}
int main(){
        thread_pool *tp = tp_init(5);
        int i;
        int arg = 7;
        pthread_t producer_thread_id;//生产者线程ID
        pthread_create(&producer_thread_id,NULL,(void *)thread_func_producer,(void *)tp);
        pthread_join(producer_thread_id,NULL);
        tp_destroy(tp);
        return 0;
}
```
运行结果：
![](https://img-blog.csdn.net/20130528141746772)
