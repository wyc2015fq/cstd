# 《win32多线程程序设计》学习笔记二 - 一世豁然的专栏 - CSDN博客





2017年03月08日 12:31:18[一世豁然](https://me.csdn.net/Explorer_day)阅读数：407








一、产生一个线程

1、产生一个线程（并因而成就一个多线程程序），是以CreateThread()函数作为开始。此函数原型如下：

![](https://img-blog.csdn.net/20170308111602124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170308112851000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





二、使用多线程的结果

1、多线程程序无法预期



2、执行顺序无法保证




3、线程对于小的改变有高度的敏感




4、线程并不总是立即启动







三、核心对象

1、CreateThread()传回来的handle被称为一个核心对象。核心对象其实和所谓的GDI对象差不多。




2、核心对象以HANDLE为使用时的参考依据。与GDI的HBRUSH、HPEN、HPALETTE以及其他handle不同的是，只有一种handle可以代表核心对象。




3、win32核心对象的清单：

 1）、进程（processes）

 2）、线程（threads）

 3）、文件（files）

 4）、事件（evnets）

 5）、信号量（semaphores）

 7）、互斥量（mutexs）

 8）、管道（pipes，分为named和anonymous两种）




4、GDI对象和核心对象有一个主要的不同。GDI对象有单一拥有者，不是进程就是线程。核心对象可以有一个以上拥有者，甚至可以跨进程。




5、释放核心对象

![](https://img-blog.csdn.net/20170308120731854?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





6、线程对象与线程的不同

 1）、线程的handle是指向“线程核心对象”，而不是指向线程本身。“线程核心对象”引用到的那个线程也会令线程核心对象开启。

 2）、“引用计数机制”保证有个地方可以写下其返回值。这样的机制也能够保证旧的线程能够读取那个返回值——只要没有调用CloseHandle()。







四、结束代码

![](https://img-blog.csdn.net/20170308121839952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








五、结束一个线程

![](https://img-blog.csdn.net/20170308121948796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


1、结束主线程

 1）、程序启动后就执行的那个线程称为主线程。

 2）、主线程有两个特点：

    I、必须负责GUI程序中的主消息循环。

   II、这一线程的结束会使程序中的所有线程都强迫结束，程序也因此二结束，其他线程没有机会做清理工作。







六、错误处理（GetLastError）




七、经验

1、各线程的数据要分离开来，避免使用全局变量。

2、不要在线程之间共享GDI对象。

3、确定你知道你的线程状态，不要径自结束程序而不等待它们的结束。

4、让主线程处理用户界面（UI）



