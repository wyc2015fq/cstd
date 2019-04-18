# Posix多线程编程学习笔记（四）—互斥量（1） - ljx0305的专栏 - CSDN博客
2008年08月01日 11:13:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：564
**一、什么是互斥锁<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />**
另一种在多线程程序中同步访问手段是使用互斥量。程序员给某个对象加上一把“锁”，每次只允许一个线程去访问它。如果想对代码关键部分的访问进行控制，你必须在进入这段代码之前锁定一把互斥量，在完成操作之后再打开它。
互斥量函数有
pthread_mutex_init 初始化一个互斥量
pthread_mutex_lock 给一个互斥量加锁
pthread_mutex_trylock 加锁，如果失败不阻塞
pthread_mutex_unlock 解锁
可以通过使用pthread的互斥接口保护数据，确保同一时间只有一个线程访问数据。互斥量从本质上说是一把锁，在访问共享资源前对互斥量进行加锁，在访问完成后释放互斥量上的锁。对互斥量进行加锁以后，任何其他试图再次对互斥量加锁的线程将会被阻塞直到当前线程释放该互斥锁。如果释放互斥锁时有多个线程阻塞，所以在该互斥锁上的阻塞线程都会变成可进行状态，第一个变成运行状态的线程可以对互斥量加锁，其他线程在次被阻塞，等待下次运行状态。
互斥量用pthread_mutex_t数据类型来表示，在使用互斥量以前，必须首先对它进行初始化，可以把它置为常量PTHREAD_MUTEX_INITIALIZER(只对静态分配的互斥量)，也可以通过调用pthread_mutex_init函数进行初始化，如果动态地分配互斥量，那么释放内存前需要调用pthread_mutex_destroy.
**二、初始化/****回收互斥锁**
1．
|名称:：|pthread_mutexattr_init|
|----|----|
|功能：|初始化互斥锁。|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_mutex_init(pthread_mutex_t * mutex,const pthread_mutex_t *attr);|
|参数：|mutex互斥量attr互斥锁属性|
|返回值：|若成功则返回0，否则返回错误编号。|

mutex是我们要锁住的互斥量，attr是互斥锁的属性，可用相应的函数修改，我们在下章介绍，要用默认的属性初始化互斥量，只需把attr设置为NULL。
2．
|名称:：|pthread_mutex_destroy|
|----|----|
|功能：|释放对互斥变量分配的资源|
|头文件：|#include <pthread.h>|
|函数原形：|intpthread_mutex_destroy(pthread_mutex_t *mutex);|
|参数：||
|返回值：|若成功则返回0，否则返回错误编号。|


**三、对互斥量加减锁**
3．
|名称:：|pthread_mutex_lock/ pthread_mutex_trylock/ pthread_mutex_unlock|
|----|----|
|功能：|对互斥量加/减锁|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_mutex_lock(pthread_mutex_t *mutex);int pthread_mutex_trylock(pthread_mutex_t *mutex);int pthread_mutex_unlock(pthread_mutex_t *mutex);|
|参数：||
|返回值：|若成功则返回0，否则返回错误编号。|

对互斥量进行加锁，需要调用pthread_mutex_lock,如果互斥量已经上锁，调用线程阻塞直至互斥量解锁。对互斥量解锁，需要调用pthread_mutex_unlock.
如果线程不希望被阻塞，他可以使用pthread_mutex_trylock尝试对互斥量进行加锁。如果调用pthread_mutex_trylock时互斥量处于未锁住状态，那么pthread_mutex_trylock将锁住互斥量，否则就会失败，不能锁住互斥量，而返回EBUSY。
下面试例子可以证明对互斥量加锁的必要性：
我们先来看不加锁的程序。
#inlcude <stdio.h>
#include <pthread.h>
#inlcude <stdio.h>
#include <unistd.h>
viid *thread_function(void *arg);
int run_now=1;/*用run_now代表共享资源*/
int main()
{
int print_count1=0; /*用于控制循环*/
prhread_t a_thread; 
if(pthread_create(&a_thread,NULL,thread_function,NULL)!=0)/*创建一个进程*/
{
perror(“Thread createion failed”);
exit(1);
}
while(print_count1++<5)
{
if(run_now==1)/主线程：如果run_now为1就把它修改为2*/
{
printf(“main thread is run/n”);
run_now=2; 
}
else
{
printf(“main thread is sleep/n”);
sleep(1);
}
}
pthread_join(a_thread,NULL); /*等待子线程结束*/
exit(0);
}
void *thread_function(void *arg)
{
int print_count2=0;
while(print_count2++<5)
{
if(run_now==2)/子线程：如果run_now为1就把它修改为1*/
{
printf(“function thread is run/n”);
run_now=1;
}
else
{
printf(“function thread is sleep/n”);
sleep(1);
}
}
pthread_exit(NULL);
}
运行上面程序的运行结果为:
function thread is sleep
main thread is run
main thread is sleep
main thread is sleep
function thread is run 
function thread is sleep
main thread is run
main thread is sleep
function thread is run
function thread is sleep
我们可以看到main线程和function线程是交替运行的。它们都可以对run_now进行操作。
下面是加锁的程序。
#inlcude <stdio.h>
#include <pthread.h>
#inlcude <stdio.h>
viid *thread_function(void *arg);
int run_now=1;/*用run_now代表共享资源*/ 
pthread_mutex_t work_mutex;/*定义互斥量*/
int main()
{
int res;
int print_count1=0;
prhread_t a_thread;
if(pthread_mutex_init(&work_mutex,NULL)!=0)/*初始化互斥量*/
{
perror(“Mutex init faied”);
exit(1);
}
if(pthread_create(&a_thread,NULL,thread_function,NULL)!=0)/*创建新线程*/
{
perror(“Thread createion failed”);
exit(1);
}
if(pthread_mutex_lock(&work_mutex)!=0)/*对互斥量加锁*/
{
preeor(“Lock failed”);
exit(1);
}
else
printf(“main lock/n”);
while(print_count1++<5) 
{
if(run_now==1)/主线程：如果run_now为1就把它修改为2*/
{
printf(“main thread is run/n”);
run_now=2;
}
else
{
printf(“main thread is sleep/n”);
sleep(1);
}
}
if(pthread_mutex_unlock(&work_mutex)!=0) /*对互斥量解锁*/
{
preeor(“unlock failed”);
exit(1);
}
else
printf(“main unlock/n”);
pthread_mutex_destroy(&work_mutex); /*收回互斥量资源*/
pthread_join(a_thread,NULL); /*等待子线程结束*/
exit(0);
}
void *thread_function(void *arg)
{
int print_count2=0;
sleep(1);
if(pthread_mutex_lock(&work_mutex)!=0)
{
perror(“Lock failed”);
exit(1);
}
else
printf(“function lock/n”);
while(print_count2++<5)
{
if(run_now==2)/分进程：如果run_now为1就把它修改为1*/
{
printf(“function thread is run/n”);
run_now=1;
}
else
{
printf(“function thread is sleep/n”);
sleep(1);
}
}
if(pthread_mutex_unlock(&work_mutex)!=0)/*对互斥量解锁*/
{
perror(“unlock failed”);
exit(1);
}
else
printf(“function unlock/n”);
pthread_exit(NULL);
}
下面是运行结果：
main lock
main thread is run
main thread is sleep
main thread is sleep
main thread is sleep
main thread is sleep
main unlock
function lock
function thread is run
function thread is sleep
function thread is sleep
function thread is sleep
function thread is sleep
function unlock
我们从运行结果可以看到，当主进程把互斥量锁住后，子进程就不能对共享资源进行操作了。
引用：[http://blog.chinaunix.net/u/22935/showart_340404.html](http://blog.chinaunix.net/u/22935/showart_340404.html)
