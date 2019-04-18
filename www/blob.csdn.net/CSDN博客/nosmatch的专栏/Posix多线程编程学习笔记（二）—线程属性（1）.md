# Posix多线程编程学习笔记（二）—线程属性（1） - nosmatch的专栏 - CSDN博客
2012年01月09日 13:21:47[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：481标签：[多线程																[编程																[pthreads																[thread																[struct																[join](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=pthreads&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[多线程](https://blog.csdn.net/HDUTigerkin/article/category/1061405)
**一．线程属性**
线程具有属性，用pthread_attr_t表示，在对该结构进行处理之前必须进行初始化，在使用后需要对其去除初始化。我们用pthread_attr_init函数对其初始化，用pthread_attr_destroy对其去除初始化。
1．
|名称:：|pthread_attr_init/pthread_attr_destroy|
|----|----|
|功能：|对线程属性初始化/去除初始化|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_attr_init(pthread_attr_t *attr);int pthread_attr_destroy(pthread_attr_t *attr);|
|参数：|Attr   线程属性变量|
|返回值：|若成功返回0，若失败返回-1。|

调用pthread_attr_init之后，pthread_t结构所包含的内容就是操作系统实现支持的线程所有属性的默认值。
如果要去除对pthread_attr_t结构的初始化，可以调用pthread_attr_destroy函数。如果pthread_attr_init实现时为属性对象分配了动态内存空间，pthread_attr_destroy还会用无效的值初始化属性对象，因此如果经pthread_attr_destroy去除初始化之后的pthread_attr_t结构被pthread_create函数调用，将会导致其返回错误。
线程属性结构如下：
typedef struct
{
       int                               detachstate;   线程的分离状态
       int                               schedpolicy; 
线程调度策略
       struct sched_param              schedparam; 
线程的调度参数
       int                               inheritsched; 
线程的继承性
       int                                scope;      
线程的作用域
       size_t                           guardsize;  
线程栈末尾的警戒缓冲区大小
       int                                stackaddr_set;
       void *                          stackaddr;  
线程栈的位置
       size_t                           stacksize;   
线程栈的大小
}pthread_attr_t;
每个个属性都对应一些函数对其查看或修改。下面我们分别介绍。
**二、线程的分离状态**
线程的分离状态决定一个线程以什么样的方式来终止自己。在默认情况下线程是非分离状态的，这种情况下，原有的线程等待创建的线程结束。只有当pthread_join（）函数返回时，创建的线程才算终止，才能释放自己占用的系统资源。
而分离线程不是这样子的，它没有被其他的线程所等待，自己运行结束了，线程也就终止了，马上释放系统资源。程序员应该根据自己的需要，选择适当的分离状态。所以如果我们在创建线程时就知道不需要了解线程的终止状态，则可以pthread_attr_t结构中的detachstate线程属性，让线程以分离状态启动。
2．
|名称:：|pthread_attr_getdetachstate/pthread_attr_setdetachstate|
|----|----|
|功能：|获取/修改线程的分离状态属性|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_attr_getdetachstate(const pthread_attr_t * attr,int *detachstate);int pthread_attr_setdetachstate(pthread_attr_t *attr,int detachstate);|
|参数：|Attr   线程属性变量Detachstate 线程的分离状态属性|
|返回值：|若成功返回0，若失败返回-1。|

可以使用pthread_attr_setdetachstate函数把线程属性detachstate设置为下面的两个合法值之一：设置为PTHREAD_CREATE_DETACHED,以分离状态启动线程；或者设置为PTHREAD_CREATE_JOINABLE,正常启动线程。可以使用pthread_attr_getdetachstate函数获取当前的datachstate线程属性。
以分离状态创建线程
#iinclude <pthread.h>
void *child_thread(void *arg)
{
printf(“child thread run!\n”);
}
int main(int argc,char *argv[ ])
{
      pthread_t tid;
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
      pthread_create(&tid,&attr,fn,arg);
      pthread_attr_destroy(&attr);
      sleep(1);
}
**三、线程的继承性**
函数pthread_attr_setinheritsched和pthread_attr_getinheritsched分别用来设置和得到线程的继承性，这两个函数的定义如下：
3.
|名称:：|pthread_attr_getinheritschedpthread_attr_setinheritsched|
|----|----|
|功能：|获得/设置线程的继承性|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_attr_getinheritsched(const pthread_attr_t *attr,int *inheritsched);int pthread_attr_setinheritsched(pthread_attr_t *attr,int inheritsched);|
|参数：|attr            线程属性变量inheritsched    线程的继承性|
|返回值：|若成功返回0，若失败返回-1。|

这两个函数具有两个参数，第1个是指向属性对象的指针，第2个是继承性或指向继承性的指针。继承性决定调度的参数是从创建的进程中继承还是使用在schedpolicy和schedparam属性中显式设置的调度信息。Pthreads不为inheritsched指定默认值，因此如果你关心线程的调度策略和参数，必须先设置该属性。
继承性的可能值是PTHREAD_INHERIT_SCHED（表示新现成将继承创建线程的调度策略和参数）和PTHREAD_EXPLICIT_SCHED（表示使用在schedpolicy和schedparam属性中显式设置的调度策略和参数）。
如果你需要显式的设置一个线程的调度策略或参数，那么你必须在设置之前将inheritsched属性设置为PTHREAD_EXPLICIT_SCHED.
下面我来讲进程的调度策略和调度参数。我会结合下面的函数给出本函数的程序例子。
