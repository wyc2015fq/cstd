# ACE Condition类 - 深之JohnChen的专栏 - CSDN博客

2012年02月15日 11:37:37[byxdaz](https://me.csdn.net/byxdaz)阅读数：2133标签：[thread																[signal																[os																[null																[工作																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=os&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)
个人分类：[ACE网络编程](https://blog.csdn.net/byxdaz/article/category/628882)


进来工作中碰到关于ACE的代码，有些疑惑，今天上网看到一篇关于ACE Condition类属 的文章，解决了一些自己的疑惑，鉴于好东西多传播的思路，我就再贴一下。原文在 
[http://www.cnblogs.com/TianFang/archive/2006/12/04/581854.html](http://www.cnblogs.com/TianFang/archive/2006/12/04/581854.html)
ACE Condition类属

ACE Condition类属（条件变量）提供风格与互斥体、读者/作者锁和计数信号量不同的锁定机制。当持有锁的线程在临界区执行代码时，这三种机制让协作线程进行等待。相反，条件变量通常被一个线程用于使自己等待，直到一个涉及共享数据的条件表达式到达特定的状态。当另外的协作线程指示共享数据的状态已发生变化，调度器就唤醒一个在该条件变量上挂起的线程。于是新唤醒的线程重新对它的条件表达式进行求值，如果共享数据已到达合适状态，就恢复处理。

ACE线程库提供一个叫作Condition的类来可移植地在C++包装类中实现条件变量语义。定义方式如下：
ACE_Thread_Mutex mutex;
ACE_Condition<ACE_Thread_Mutex> cond(mutex);

该对象有两个常用方法。

signal()
向使用该条件变量的其它线程发送满足条件信号。 
wait()
查询是否满足条件，如果满足，则继续往下执行；如果不满足条件，主线程就等待在此条件变量上。条件变量随即自动释放互斥体，并使主线程进入睡眠。 
条件变量总是与互斥体一起使用。这是一种可如下描述的一般模式：

while( expression NOT TRUE ) wait on condition variable;

条件变量不是用于互斥，往往用于线程间的协作，下面例子演示了通过条件变量实现线程协作。

#include "ace/Thread.h"
#include "ace/Synch.h"

#include <iostream>
using namespace std;

ACE_Thread_Mutex mutex;
ACE_Condition<ACE_Thread_Mutex> cond(mutex);

void* worker(void *arg)
{
    ACE_OS::sleep(2);        //保证eater线程的cond.wait()在worker线程的cond.signal()先执行
    mutex.acquire();
    ACE_OS::sleep(1);
    cout<<endl<<"produce"<<endl;
    cond.signal();
    mutex.release();
    return NULL;
}

void* eater(void *arg)
{
    mutex.acquire();
    cond.wait();
    cout<<endl<<"eat"<<endl;
    mutex.release();
    return NULL;
}

int main(int argc, char *argv[])
{
    ACE_Thread::spawn((ACE_THR_FUNC)worker);
    ACE_OS::sleep(1);
    ACE_Thread::spawn((ACE_THR_FUNC)eater);

    while(true)
        ACE_OS::sleep(10);

    return 0;
}

这个例子中，首先创建了一个生产者线程worker和一个消费者线程eater，消费者线程执行比生产者快，两个线程不加限制并发执行会导致先消费，后生产的情况（只是加互斥锁也不能很好的解决，以为无法保证生产者一定先获得互斥体）。所以这里通过条件变量的通知方式保证线程的顺序执行：

消费者线程获取互斥体，等待条件满足（生产者生产了食品）。同时释放互斥体，进入休眠状态。
生产者获取互斥体（虽然是消费者先获取的互斥体，但消费者调用的wait函数会释放消费者的互斥体），生产商品后，通过条件变量发送信号（调用signal函数）通知消费者生产完成，结束生产过程，释放互斥体。
消费者收到信号后，重新获取互斥体，完成消费过程。

使用条件变量的注意事项：

条件变量必须和互斥体一起使用，也就是说使用前必须加锁（调用互斥体acquire函数），使用完后需释放互斥体。
条件变量中的wait()和signal()成对使用的话，必须保证wait()函数在signal()之前执行，这样才能保证wait()能收到条件满足通知，不至于一直等待下去，形成死锁（worker线程中的第一句话就是起的这个作用）。


