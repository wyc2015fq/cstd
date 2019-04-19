# 2011年Android IPC进程间通讯机制学习笔记之一 - 2019跟着小虎玩着去软考 - CSDN博客
2011年04月12日 13:45:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：1732标签：[android																[通讯																[service																[manager																[system																[migration](https://so.csdn.net/so/search/s.do?q=migration&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=manager&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android																[手机开发](https://blog.csdn.net/littletigerat/article/category/649495)](https://blog.csdn.net/littletigerat/article/category/613910)
# 2011年Android IPC进程间通讯机制学习笔记之一
一.Linux系统进程间通信有哪些方式？
1.socket；
2.name pipe命名管道；
3.message queue消息队列；
4.singal信号量；
5.share memory共享内存；
二.Java系统的通信方式是什么？
1.socket;
2.name pipe;
三.Android系统通信方式是什么？
Binder 通信；
四.Binder通信的优势是什么？
高效率
五.Binder通信的特点是什么？
是同步，而不是异步；
六.Binder通信是如何实现的？
1.Binder通信是通过linux的binder driver来实现的，
2.Binder通信操作类似线程迁移(thread migration)，两个进程间IPC看起来就象是一个进程进入另一个进程执行代码然后带着执行的结果返回；
3.Binder的用户空间为每一个进程维护着一个可用的线程池，线程池用于处理到来的IPC以及执行进程本地消息，Binder通信是同步而不是异步。
七. Android中的 Binder通信实现要点：
1. Android中的Binder通信是基于Service与Client的工作模型的；
2. 所有需要IBinder通信的进程都必须创建一个IBinder接口；
3. 系统中有一个进程管理所有的system service：
4. Android不允许用户添加非授权的System service；
5. 现在源码开放了，我们可以修改一些代码来实现添加底层system Service的目的；
6. 对用户程序来说，我们也要创建server,或者Service用于进程间通信；
7. ActivityManagerService管理JAVA应用层所有的service创建与连接(connect),disconnect；
8. 所有的Activity也是通过这个service来启动，加载的；
9. ActivityManagerService也是加载在Systems Servcie中的；
八.Android的 Service工作流程
1.Android虚拟机启动之前系统会先启动service Manager进程；2.service Manager打开binder驱动，并通知binder kernel驱动程序这个进程将作为System Service Manager；
3.然后该进程将进入一个循环，等待处理来自其他进程的数据。4.用户创建一个System service后，通过defaultServiceManager得到一个远程ServiceManager的接口，通过这个接口我们可以调用addService函数将System service添加到Service Manager进程中；
5.然后client可以通过getService获取到需要连接的目的Service的IBinder对象，这个IBinder是Service的BBinder在binder kernel的一个参考，
6.所以service IBinder 在binder kernel中不会存在相同的两个IBinder对象，每一个Client进程同样需要打开Binder驱动程序。对用户程序而言，我们获得这个对象就可以通过binder kernel访问service对象中的方法。
7.Client与Service在不同的进程中，通过这种方式实现了类似线程间的迁移的通信方式，对用户程序而言当调用Service返回的IBinder接口后，访问Service中的方法就如同调用自己的函数。
