# Posix多线程编程学习笔记（五）—条件变量（1） - ljx0305的专栏 - CSDN博客
2008年08月01日 11:39:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：663
**一、什么是条件变量<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />**
与互斥锁不同，条件变量是用来等待而不是用来上锁的。条件变量用来自动阻塞一个线程，直到某特殊情况发生为止。通常条件变量和互斥锁同时使用。
条件变量使我们可以睡眠等待某种条件出现。条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"（给出条件成立信号）。
条件的检测是在互斥锁的保护下进行的。如果一个条件为假，一个线程自动阻塞，并释放等待状态改变的互斥锁。如果另一个线程改变了条件，它发信号给关联的条件变量，唤醒一个或多个等待它的线程，重新获得互斥锁，重新评价条件。如果两进程共享可读写的内存，条件变量可以被用来实现这两进程间的线程同步。
使用条件变量之前要先进行初始化。可以在单个语句中生成和初始化一个条件变量如：
pthread_cond_t my_condition=PTHREAD_COND_INITIALIZER;（用于进程间线程的通信）。
也可以利用函数pthread_cond_init动态初始化。
**二、条件变量函数**
1．
|名称：|pthread_cond_init|
|----|----|
|目标：|条件变量初始化|
|头文件：|#include < pthread.h>|
|函数原形：|int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);|
|参数:|cptr条件变量attr条件变量属性|
|返回值：|成功返回0，出错返回错误编号。|

pthread_cond_init函数可以用来初始化一个条件变量。他使用变量attr所指定的属性来初始化一个条件变量，如果参数attr为空，那么它将使用缺省的属性来设置所指定的条件变量。
2.
|名称：|pthread_cond_destroy|
|----|----|
|目标：|条件变量摧毁|
|头文件：|#include < pthread.h>|
|函数原形：|int pthread_cond_destroy(pthread_cond_t *cond);|
|参数:|cptr条件变量|
|返回值：|成功返回0，出错返回错误编号。|
pthread_cond_destroy函数可以用来摧毁所指定的条件变量，同时将会释放所给它分配的资源。调用该函数的进程也并不要求等待在参数所指定的条件变量上。
3.
|名称：|pthread_cond_wait/pthread_cond_timedwait|
|----|----|
|目标：|条件变量等待|
|头文件：|#include < pthread.h>|
|函数原形：|int pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex); int pthread_cond_timedwait(pthread_cond_t *cond,pthread_mutex_t mytex,const struct timespec *abstime);|
|参数:|cond 条件变量mutex 互斥锁|
|返回值：|成功返回0，出错返回错误编号。|

第一个参数*cond是指向一个条件变量的指针。第二个参数*mutex则是对相关的互斥锁的指针。函数pthread_cond_timedwait函数类型与函数pthread_cond_wait,区别在于，如果达到或是超过所引用的参数*abstime,它将结束并返回错误ETIME.pthread_cond_timedwait函数的参数*abstime指向一个timespec结构。该结构如下：
typedef struct timespec{
time_t tv_sec;
long tv_nsex;
}timespec_t;
3.
|名称：|pthread_cond_signal/pthread_cond_broadcast|
|----|----|
|目标：|条件变量通知|
|头文件：|#include < pthread.h>|
|函数原形：|int pthread_cond_signal(pthread_cond_t *cond); int pthread_cond_broadcast(pthread_cond_t *cond);|
|参数:|cond 条件变量|
|返回值：|成功返回0，出错返回错误编号。|

参数*cond是对类型为pthread_cond_t 的一个条件变量的指针。当调用pthread_cond_signal时一个在相同条件变量上阻塞的线程将被解锁。如果同时有多个线程阻塞，则由调度策略确定接收通知的线程。如果调用pthread_cond_broadcast,则将通知阻塞在这个条件变量上的所有线程。一旦被唤醒，线程仍然会要求互斥锁。如果当前没有线程等待通知，则上面两种调用实际上成为一个空操作。如果参数*cond指向非法地址，则返回值EINVAL。
下面是一个简单的例子，我们可以从程序的运行来了解条件变量的作用。
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/
void *thread1(void *);
void *thread2(void *);
int i=1;
int main(void)
{
pthread_t t_a;
pthread_t t_b;
pthread_create(&t_a,NULL,thread2,(void *)NULL);/*创建进程t_a*/
pthread_create(&t_b,NULL,thread1,(void *)NULL); /*创建进程t_b*/
pthread_join(t_b, NULL);/*等待进程t_b结束*/
pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&cond);
exit(0);
}
void *thread1(void *junk)
{
for(i=1;i<=9;i++) 
{
pthread_mutex_lock(&mutex);/*锁住互斥量*/
if(i%3==0)
pthread_cond_signal(&cond);/*条件改变，发送信号，通知t_b进程*/
else        
printf("thead1:%d/n",i);
pthread_mutex_unlock(&mutex);/*解锁互斥量*/
sleep(1);
}
}
void *thread2(void *junk)
{
while(i<9)
{
pthread_mutex_lock(&mutex);
if(i%3!=0)
pthread_cond_wait(&cond,&mutex);/*等待*/
printf("thread2:%d/n",i);
pthread_mutex_unlock(&mutex);
sleep(1);
}
}
程序创建了2个新线程使他们同步运行，实现进程t_b打印20以内3的倍数，t_a打印其他的数，程序开始线程t_b不满足条件等待，线程t_a运行使a循环加1并打印。直到i为3的倍数时，线程t_a发送信号通知进程t_b，这时t_b满足条件，打印i值。
下面是运行结果：
#cc –lpthread –o cond cond.c
#./cond
thread1:1
thread1:2
thread2:3
thread1:4
thread1:5
thread2:6
thread1:7
thread1:8
thread2:9
引用：[http://blog.chinaunix.net/u/22935/showart_340462.html](http://blog.chinaunix.net/u/22935/showart_340462.html)
