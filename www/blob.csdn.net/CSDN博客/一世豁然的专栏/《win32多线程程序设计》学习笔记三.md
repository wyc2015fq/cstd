# 《win32多线程程序设计》学习笔记三 - 一世豁然的专栏 - CSDN博客





2017年03月11日 12:27:17[一世豁然](https://me.csdn.net/Explorer_day)阅读数：521








一、Busy Waiting

1、绝对不要再win32中使用Busy Loop




二、性能呢监视器（略）




三、等待一个线程结束

1、Win32提供函数WaitForSingleObject()，他能够在某个线程结束时被调用。

![](https://img-blog.csdn.net/20170311114417926?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170311114422500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2、获得一个线程对线的handle后，WaitForSingleObject()要求操作系统让即将等待的线程睡觉，直到以下任何情况的发生：

 1）、正在执行的线程结束

 2）、dwMilliseconds时间结束，该值系从函数调用后开始计算




3、由于操作系统持续追踪正在执行的线程，即使正在执行的线程失事或被强迫结束，WaitForSingleObject()任然能够正常运行。




4、设定time-out为0，使你能够检查handle的状态并立即返回，没有片刻停留。如果handle已经备妥，那么这个函数会成功并传回WAIT_OBJECT_0。否则，这个函数立即返回并传回WAIT_TIMEOUT。







三、被激发的对象

1、当线程正在执行时，线程对象出于未激发状态。当线程结束时，线程对象就被激发了。因此，任何一个线程如果等待的是一个线程对象，将在等待对象结束时被调用，因为当前线程对象自动变为激发状态。




2、核心对象激发状态的含义

![](https://img-blog.csdn.net/20170311120109009?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170311120112931?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








四、等待多个对象

1、Win32函数中的WaitForMultipleObjects()允许你在同一时间等待一个以上的对象

![](https://img-blog.csdn.net/20170311121008719?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170311121015079?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2、注意，handle数组中的元素个数上限，绝对不能够超过MAXIMUX_WAIT_OBJECTS。




五、在一个GUI程序中等待

1、MsgWaitForMultipleObjects()函数会在"对象被激发”或“消息到达队列“时被唤醒而返回

![](https://img-blog.csdn.net/20170311122249618?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170311122256241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2、MsgWaitForMultipleObjects()的正确使用方式是改写主循环，使得激发状态之handle得以像消息一样被对待。



