# Posix多线程编程学习笔记（二）—线程属性（2） - nosmatch的专栏 - CSDN博客
2012年01月09日 15:08:38[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：490标签：[多线程																[编程																[thread																[struct																[null																[join](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[多线程](https://blog.csdn.net/HDUTigerkin/article/category/1061405)
**四、线程的调度策略**
函数pthread_attr_setschedpolicy和pthread_attr_getschedpolicy分别用来设置和得到线程的调度策略。
4.
|名称:：|pthread_attr_getschedpolicypthread_attr_setschedpolicy|
|----|----|
|功能：|获得/设置线程的调度策略|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_attr_getschedpolicy(const pthread_attr_t *attr,int *policy);int pthread_attr_setschedpolicy(pthread_attr_t *attr,int policy);|
|参数：|attr          线程属性变量policy        调度策略|
|返回值：|若成功返回0，若失败返回-1。|


这两个函数具有两个参数，第1个参数是指向属性对象的指针，第2个参数是调度策略或指向调度策略的指针。调度策略可能的值是先进先出（SCHED_FIFO）、轮转法（SCHED_RR）,或其它（SCHED_OTHER）。
       SCHED_FIFO策略允许一个线程运行直到有更高优先级的线程准备好，或者直到它自愿阻塞自己。在SCHED_FIFO调度策略下，当有一个线程准备好时，除非有平等或更高优先级的线程已经在运行，否则它会很快开始执行。
    SCHED_RR(轮循)策略是基本相同的，不同之处在于：如果有一个SCHED_RR
策略的线程执行了超过一个固定的时期(时间片间隔)没有阻塞，而另外的SCHED_RR或SCHBD_FIPO策略的相同优先级的线程准备好时，运行的线程将被抢占以便准备好的线程可以执行。
当有SCHED_FIFO或SCHED_RR策赂的线程在一个条件变量上等持或等持加锁同一个互斥量时，它们将以优先级顺序被唤醒。即，如果一个低优先级的SCHED_FIFO线程和一个高优先织的SCHED_FIFO线程都在等待锁相同的互斥且，则当互斥量被解锁时，高优先级线程将总是被首先解除阻塞。
**五、线程的调度参数**
函数pthread_attr_getschedparam 和pthread_attr_setschedparam分别用来设置和得到线程的调度参数。
5.
|名称:：|pthread_attr_getschedparampthread_attr_setschedparam|
|----|----|
|功能：|获得/设置线程的调度参数|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_attr_getschedparam(const pthread_attr_t *attr,struct sched_param *param);int pthread_attr_setschedparam(pthread_attr_t *attr,const struct sched_param *param);|
|参数：|attr          线程属性变量param          sched_param结构|
|返回值：|若成功返回0，若失败返回-1。|

这两个函数具有两个参数，第1个参数是指向属性对象的指针，第2个参数是sched_param结构或指向该结构的指针。结构sched_param在文件/usr/include
 /bits/sched.h中定义如下：
struct sched_param
{
       int sched_priority;
};
结构sched_param的子成员sched_priority控制一个优先权值，大的优先权值对应高的优先权。系统支持的最大和最小优先权值可以用sched_get_priority_max函数和sched_get_priority_min函数分别得到。
注意：如果不是编写实时程序，不建议修改线程的优先级。因为，调度策略是一件非常复杂的事情，如果不正确使用会导致程序错误，从而导致死锁等问题。如：在多线程应用程序中为线程设置不同的优先级别，有可能因为共享资源而导致优先级倒置。
6.
|名称:：|sched_get_priority_maxsched_get_priority_min|
|----|----|
|功能：|获得系统支持的线程优先权的最大和最小值|
|头文件：|#include <pthread.h>|
|函数原形：|int sched_get_priority_max(int policy);int sched_get_priority_min(int policy);|
|参数：|policy           系统支持的线程优先权的最大和最小值|
|返回值：|若成功返回0，若失败返回-1。|

下面是上面几个函数的程序例子：
#include <pthread.h>
#include <sched.h>
void *child_thread(void *arg)
{
int policy;
int max_priority,min_priority;
struct sched_param param;
pthread_attr_t attr;
pthread_attr_init(&attr); /*初始化线程属性变量*/
pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED); /*设置线程继承性*/
pthread_attr_getinheritsched(&attr,&policy); /*获得线程的继承性*/
if(policy==PTHREAD_EXPLICIT_SCHED)
    printf(“Inheritsched:PTHREAD_EXPLICIT_SCHED\n”);
if(policy==PTHREAD_INHERIT_SCHED)
    printf(“Inheritsched:PTHREAD_INHERIT_SCHED\n”);
pthread_attr_setschedpolicy(&attr,SCHED_RR);/*设置线程调度策略*/
pthread_attr_getschedpolicy(&attr,&policy);/*取得线程的调度策略*/
if(policy==SCHED_FIFO)
    printf(“Schedpolicy:SCHED_FIFO\n”);
if(policy==SCHED_RR)
    printf(“Schedpolicy:SCHED_RR\n”);
if(policy==SCHED_OTHER)
    printf(“Schedpolicy:SCHED_OTHER\n”);
sched_get_priority_max(max_priority);/*获得系统支持的线程优先权的最大值*/
sched_get_priority_min(min_priority);/*
获得系统支持的线程优先权的最小值*/
printf(“Max priority:%u\n”,max_priority);
printf(“Min priority:%u\n”,min_priority);
param.sched_priority=max_priority;
pthread_attr_setschedparam(&attr,¶m);/*设置线程的调度参数*/
printf(“sched_priority:%u\n”,param.sched_priority);/*获得线程的调度参数*/
pthread_attr_destroy(&attr);
}
int main(int argc,char *argv[ ])
{
pthread_t child_thread_id;
pthread_create(&child_thread_id,NULL,child_thread,NULL);
pthread_join(child_thread_id,NULL);
}
