# Posix多线程编程学习笔记（一）—线程基础（1） - nosmatch的专栏 - CSDN博客
2012年01月09日 09:15:23[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：442标签：[多线程																[编程																[thread																[join																[null																[returning](https://so.csdn.net/so/search/s.do?q=returning&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[多线程](https://blog.csdn.net/HDUTigerkin/article/category/1061405)
**一．什么是线程**
在一个程序里的多个执行路线就叫做线程。更准确的定义是：线程是“一个进程内部的一个控制序列”。
典型的unix进程可以看成只有一个控制线程：一个进程在同一时刻只做一件事情。有了多个控制线程以后，在程序设计时可以把进程设计成在同一时刻能够做不止一件事，每个线程处理各只独立的任务。
**二．线程的优点**
（1）通过为每种事件类型的处理分配单独的线程，能够简化处理异步时间的代码。
（2）多个线程可以自动共享相同的存储地址空间和文件描述符。
（3）有些问题可以通过将其分解从而改善整个程序的吞吐量。
（4）交互的程序可以通过使用多线程实现相应时间的改善，多线程可以把程序中处理用户输入输出的部分与其它部分分开。
**三．线程的缺点**
线程也有不足之处。编写多线程程序需要更全面更深入的思考。在一个多线程程序里，因时间分配上的细微偏差或者因共享了不该共享的变量而造成不良影响的可能性是很大的。调试一个多线程程序也比调试一个单线程程序困难得多。
**四．线程的结构**
线程包含了表示进程内执行环境必需的信息，其中包括进程中标识线程的线程ID，一组寄存器值、栈、调度优先级和策略、信号屏蔽子，errno变量以及线程私有数据。进程的所有信息对该进程的所有线程都是共享的，包括可执行的程序文本，程序的全局内存和堆内存、栈以及文件描述符。
**五．线程标识**
就像每个进程有一个进程ID一样，每个线程也有一个线程ID，进程ID在整个系统中是唯一的，但线程不同，线程ID只在它所属的进程环境中有效。线程ID用pthread_t数据类型来表示，实现的时候可以用一个结构来代表pthread_t数据类型，所以可以移植的操作系统不能把它作为整数处理。因此必须使用函数来对来对两个线程ID进行比较。
1．
|名称:：|pthread_equal|
|----|----|
|功能：|比较两个线程ID|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_equal(pthread_t tid1,pthread_t tid2);|
|参数：|tid1 进程1idtid2 进程2id|
|返回值：|若相等返回非0值，否则返回0|

2．
|名称:：|pthread_self|
|----|----|
|功能：|获取自身线程的id|
|头文件：|#include <pthread.h>|
|函数原形：|pthread_t pthread_self(void);|
|参数：|无|
|返回值：|调用线程的线程id|

**六．线程的创建**
3．
|名称:：|pthread_create|
|----|----|
|功能：|创建线程|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_create(pthread_t *restrict tidp,const pthread _attr_t *restrict attr,void *(*start_rtn)(void*),void *restrict arg);|
|参数：||
|返回值：|若成功返回则返回0，否则返回错误编号|
 当pthread_creat成功返回时， tidp指向的内存单元被设置为新创建线程的线程ID。attr参数用于定制各种不同的线程属性。可以把它设置为NULL，创建默认的线程属性。
新创建的线程从start_rtn函数的地址开始运行，该函数只有一个无类型指针参数arg,如果需要向start_rtn函数传递的参数不止一个，那么需要把这些参数放到一个结构中，然后把这个结构的地址作为arg参数传入。
#include <pthread.h>
void printids(const char *s)
{
printf(“%s pid:%u  tid:%u \n“, getpid(),pthread_self());
}
void *thr_fn(void *arg)
{
printf (“new thread: “);
}
int main()
{
int err;
pthread_t tid;
err=pthread_create(&tid,NULL,thr_fn,NULL);
if(err=0)
    printf(“can’t create thread:%s\n”,strerror(err));
printids(“main thread: “);
sleep(1);
exit(0);
}
关于进程的编译我们都要加上参数 –lpthread否则提示找不到函数的错误。
具体编译方法是 cc –lpthread –o gettid gettid.c
运行结果为
main thread: pid 14954 tid 134529024
new thread: pid 14954 tid 134530048
**七．.线程的终止**
线程是依进程而存在的，当进程终止时，线程也就终止了。当然也有在不终止整个进程的情况下停止它的控制流。
（1）线程只是从启动例程中返回，返回值是线程的退出码。
（2）县城可以被同一进程中的其他线程取消。
（3）线程调用pthread_exit.
4．
|名称:：|pthread_exit|
|----|----|
|功能：|终止一个线程|
|头文件：|#include <pthread.h>|
|函数原形：|void pthread_exit(void *rval_ptr);|
|参数：||
|返回值：|无|

rval_prt是一个无类型指针，与传给启动例程的单个参数类似。进程中的其他线程可以调用pthread_join函数访问到这个指针。
5．
|名称:：|pthread_join|
|----|----|
|功能：|获得进程的终止状态|
|头文件：|#include <pthread.h>|
|函数原形：|int pthread_join(pthread_t thread,void **rval_ptr);|
|参数：||
|返回值：|若成功返回0，否则返回错误编号。|

        当一个线程通过调用pthread_exit退出或者简单地从启动历程中返回时，进程中的其他线程可以通过调用pthread_join函数获得进程的退出状态。调用pthread_join进程将一直阻塞，直到指定的线程调用pthread_exit,从启动例程中或者被取消。
如果线程只是从它的启动历程返回，rval_ptr将包含返回码。
#include <pthread.h>
#include <string.h>
void *thr_fn1(void *arg)
{
printf(“thread 1 returning\n”);
return((void *)1);
}
void *thr_fn2(void *arg)
{
printf(“thread 2 exiting\n”);
return((void *)2);
}
int main()
{
pthread_t tid1,tid2;
void *tret;
pthread_create(&tid1,NULL,thr_fn1,NULL);
pthread_create(&tid2,NULL,thr_fn2,NULL);
pthread_join(tid1,&tret);
printf(“thread 1 exit code %d\n”,(int)tret);
pthread_join(tid2,&tret);
printf(“thread 2 exit code %d\n”,(int)tret);
exit(0);
}
运行结果是：
thread 1 returning
thread 2 exiting
thread 1 exit code 1
thread 2 exit code 2
 转自－－湖光倒影：http://blog.chinaunix.net/u/22935/showart_310711.html
