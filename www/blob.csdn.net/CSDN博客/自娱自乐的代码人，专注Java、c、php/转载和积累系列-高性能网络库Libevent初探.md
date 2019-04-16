# 转载和积累系列 - 高性能网络库Libevent初探 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年04月15日 11:51:38[initphp](https://me.csdn.net/initphp)阅读数：1084








**高性能网络库Libevent初探******作者：zza （360电商技术组）


libevent是一个基于事件触发的轻量级高性能开源网络库，很多著名的软件如Chromium、memcached、Netchat等都是基于libevent开发的。



之所以受到如此多的关注和青睐，主要是libevent具有的一些显著亮点：

1) 事件驱动(event-driven)，超高的性能；

2) 轻量级，专注于网络，短小精干；

3) 跨平台，可以在linux、bsd、mac和windows环境中根据不同环境自动选择恰当的io多路复用技术；

4) 支持定时器、信号、io三种方式驱动；

libevent的最新release是2.0.21，本文基于此版本。



Libevent由以下几大部分组成：

1) 事件管理包括IO（socket）、定时器、信号三种事件；

2) evbuffer，对缓存管理封装；

3) evdns，异步DNS查询库；

4) evhttp，基于libevent的一个轻量级http实现，实现了服务器和客户端，线程不安全，因此性能上有所局限。



使用libevent典型的应用流程如下：

event_init() => event_set() => event_base_set()=> event_add() =>event_dispatch()

1) 首先初始化一个event_base对象：

struct event_base * base = event_init();

这个地方实际上是初始化了存放io事件和信号事件的链表、定时事件的小堆(mini_heap)、和激发事件的链表，在此基础上我们就可以注册一些我们想要的事件了。

2) 实例化事件event，并将回调函数和该时间绑定：

event_set(&ev, -1, 0, timer_cb, NULL);

event_set的含义如下：

void event_set(struct event *ev, int fd, short event, void (*cb)(int, short, void *), void *arg)

ev：执行要初始化的event对象；

fd：该event绑定的fd，对于信号事件，它就是关注的信号；

event：和fd绑定的事件类型，可以是EV_READ, EV_WRITE, EV_SIGNAL的任意组合；

cb：回调函数的函数指针，当fd上的事件event发生时，调用该函数执行处理，它有三个参数，调用时由event_base负责传入，按顺序，实际上就是event_set时的fd, event和arg；

arg：传递给cb函数指针的参数；

由于定时事件不需要fd，并且定时事件是根据添加时（event_add）的超时值设定的，因此这里event也不需要设置。

3) 将event和event_base关联：

event_base_set(base, &ev);

4) 制定event的超时时间，超时的时候自动调用

event_add(&ev, timeout);

5) 程序进入死循环，等待时间就绪或者超时执行回调函数

event_base_dispatch(base);

基础的demo大家可以直接到网络上去自己查看，此处不再赘述。



图1描述了libevent对各种事件的管理情况。之前笔者也提到过，event_init()函数初始化了这三个链表作为时间队列，并用小堆来处理定时时间。每当有时间就绪或者超时都会被添加到active队列中一一执行，priority保证了优先级高的时间被优先处理。可以用event_priority_set()设置事件的优先级，函数原型是int event_priority_set(struct event *ev, int pri)。需要特别说明的是当时间在就绪状态的时候不能设置优先级，函数返回-1表示设置失败。

![](https://img-blog.csdn.net/20150415114943737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







此外libevent是一个Reactor的典型实现。Reactor模式是编写高性能网络技术的模式之一。和普通的io方式不同的是，reactor需要io的程序不是主动调用io接口，而是将相应的动作封装成回调函数并将之与相应的io事件关联，当io事件发生时回调函数将被自动执行。


reactor有以下一些优点：

1) 异步，需要io的线程可以在注册io事件之后异步的处理其他一些事务，而不必阻塞等待io事件的发生；

2) 可扩展性强，该模式把很多可以复用的步骤抽象出来，避免了重复代码，码农同志只要关心事件的注册和回调函数的逻辑即可。



其类图如下，虽然c语言不是面向对象的语言，但通过函数指针的方式libevent完美演绎了reactor的每个环节。感兴趣的童鞋可以从git上获取最新的libevent源码一探究竟，虽然注释很啰嗦，但是其很多细节还是可圈可点的。

![](https://img-blog.csdn.net/20150415115100969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



本文简单介绍了libevent的特点、功能、典型的应用方式、对各个事件的管理方式和对reactor的完美演绎，感兴趣的同学可以自己研究一下其框架、工作原理乃至源码，也可以联系作者共同研讨之。






