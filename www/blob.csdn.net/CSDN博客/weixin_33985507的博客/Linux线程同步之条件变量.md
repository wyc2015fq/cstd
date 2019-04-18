# Linux线程同步之条件变量 - weixin_33985507的博客 - CSDN博客
2019年01月07日 23:55:44[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

转自:[http://www.cnblogs.com/feisky/archive/2010/03/08/1680950.html](http://www.cnblogs.com/feisky/archive/2010/03/08/1680950.html)
与互斥锁不同，**条件变量是用来等待而不是用来上锁的。条件变量用来自动阻塞一个线程，直到某特殊情况发生为止**。**通常条件变量和互斥锁同时使用。**
条件变量使我们可以睡眠等待某种条件出现。条件变量是利用线程间共享的全局变量进行同步的一种机制，主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；另一个线程使"条件成立"（给出条件成立信号）。
条件的检测是在互斥锁的保护下进行的。如果一个条件为假，一个线程自动阻塞，并释放等待状态改变的互斥锁。如果另一个线程改变了条件，它发信号给关联的条件变量，唤醒一个或多个等待它的线程，重新获得互斥锁，重新评价条件。如果两进程共享可读写的内存，条件变量可以被用来实现这两进程间的线程同步。
使用条件变量之前要先进行初始化。可以在单个语句中生成和初始化一个条件变量如：pthread_cond_t my_condition=PTHREAD_COND_INITIALIZER;（用于进程间线程的通信）。可以利用函数pthread_cond_init动态初始化。
条件变量分为两部分: 条件和变量. 条件本身是由互斥量保护的. 线程在改变条件状态前先要锁住互斥量. 它利用线程间共享的全局变量进行同步的一种机制。
相关的函数如下：
```
1 int pthread_cond_init(pthread_cond_t *cond,pthread_condattr_t *cond_attr);     
2  int pthread_cond_wait(pthread_cond_t *cond,pthread_mutex_t *mutex);
3 int pthread_cond_timewait(pthread_cond_t *cond,pthread_mutex *mutex,const timespec *abstime);
4 int pthread_cond_destroy(pthread_cond_t *cond);  
5 int pthread_cond_signal(pthread_cond_t *cond);
6 int pthread_cond_broadcast(pthread_cond_t *cond);  //解除所有线程的阻塞
```
#### 简要说明：     
      (1)初始化.init()或者pthread_cond_t cond=PTHREAD_COND_INITIALIER;属性置为NULL
      (2)等待条件成立.pthread_wait,pthread_timewait.wait()释放锁,并阻塞等待条件变量为真
      timewait()设置等待时间,仍未signal,返回ETIMEOUT(加锁保证只有一个线程wait)
      (3)激活条件变量:pthread_cond_signal,pthread_cond_broadcast(激活所有等待线程)
      (4)清除条件变量:destroy;无线程等待,否则返回EBUSY
#### **详细说明**
1. 初始化:
    条件变量采用的数据类型是pthread_cond_t, 在使用之前必须要进行初始化, 这包括两种方式:
- 静态: 可以把常量PTHREAD_COND_INITIALIZER给静态分配的条件变量.
- 动态: pthread_cond_init函数, 是释放动态条件变量的内存空间之前, 要用pthread_cond_destroy对其进行清理.
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <pthread.h>
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int pthread_cond_init(pthread_cond_t *restrict cond, pthread_condattr_t *restrict attr);
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int pthread_cond_destroy(pthread_cond_t *cond);
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)成功则返回0, 出错则返回错误编号.
    当pthread_cond_init的attr参数为NULL时, 会创建一个默认属性的条件变量; 非默认情况以后讨论.
2. 等待条件:
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <pthread.h>
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restric mutex);
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict timeout);
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)成功则返回0, 出错则返回错误编号.
    这两个函数分别是阻塞等待和超时等待.
    等待条件函数等待条件变为真, 传递给pthread_cond_wait的互斥量对条件进行保护, 调用者把锁住的互斥量传递给函数. 函数把调用线程放到等待条件的线程列表上, 然后对互斥量解锁, 这两个操作是原子的. 这样便关闭了条件检查和线程进入休眠状态等待条件改变这两个操作之间的时间通道, 这样线程就不会错过条件的任何变化.
    当pthread_cond_wait返回时, 互斥量再次被锁住.
3. 通知条件:
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)#include <pthread.h>
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int pthread_cond_signal(pthread_cond_t *cond);
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int pthread_cond_broadcast(pthread_cond_t *cond);
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![None.gif](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)成功则返回0, 出错则返回错误编号.
    这两个函数用于通知线程条件已经满足. 调用这两个函数, 也称向线程或条件发送信号. 必须注意, 一定要在改变条件状态以后再给线程发送信号.
#### 示例程序
#include <stdio.h>
#include <pthread.h>
 pthread_mutex_t mutex;
pthread_cond_t cond;
void *thread1(void *arg) 
{
	
pthread_cleanup_push(pthread_mutex_unlock, &mutex);
	
	    //提供函数回调保护while (1) {
		
printf("thread1 is running\n");
		
pthread_mutex_lock(&mutex);
		
pthread_cond_wait(&cond, &mutex);
		
printf("thread1 applied the condition\n");
		
pthread_mutex_unlock(&mutex);
		
sleep(4);
	
}
	
pthread_cleanup_pop(0);
}
void *thread2(void *arg) 
{
	
while (1) {
		
printf("thread2 is running\n");
		
pthread_mutex_lock(&mutex);
		
pthread_cond_wait(&cond, &mutex);
		
printf("thread2 applied the condition\n");
		
pthread_mutex_unlock(&mutex);
		
sleep(1);
	
}
}
int main() 
{
	
pthread_t thid1, thid2;
	
printf("condition variable study!\n");
	
pthread_mutex_init(&mutex, NULL);
	
pthread_cond_init(&cond, NULL);
	
pthread_create(&thid1, NULL, (void *) thread1, NULL);
	
pthread_create(&thid2, NULL, (void *) thread2, NULL);
	
	do {
		
pthread_cond_signal(&cond);
	
} while (1);
	
sleep(20);
	
pthread_exit(0);
	
return 0;
}
#### **条件变量与互斥锁、信号量的区别**
       1.互斥锁必须总是由给它上锁的线程解锁，信号量的挂出即不必由执行过它的等待操作的同一进程执行。一个线程可以等待某个给定信号灯，而另一个线程可以挂出该信号灯。
       2.互斥锁要么锁住，要么被解开（二值状态，类型二值信号量）。
       3.由于信号量有一个与之关联的状态（它的计数值），信号量挂出操作总是被记住。然而当向一个条件变量发送信号时，如果没有线程等待在该条件变量上，那么该信号将丢失。
       4.互斥锁是为了上锁而设计的，条件变量是为了等待而设计的，信号灯即可用于上锁，也可用于等待，因而可能导致更多的开销和更高的复杂性。
参考：[http://blog.csdn.net/dai_weitao/archive/2007/08/22/1754964.aspx](http://blog.csdn.net/dai_weitao/archive/2007/08/22/1754964.aspx)
作者：[Feisky](http://www.cnblogs.com/feisky/)
出处：[http://www.cnblogs.com/feisky/](http://www.cnblogs.com/feisky/)
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文连接，否则保留追究法律责任的权利。
