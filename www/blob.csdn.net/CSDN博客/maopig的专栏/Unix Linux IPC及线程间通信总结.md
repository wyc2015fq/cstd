# Unix/Linux IPC及线程间通信总结 - maopig的专栏 - CSDN博客
2011年11月08日 22:45:46[maopig](https://me.csdn.net/maopig)阅读数：1323标签：[linux																[windows																[system																[存储																[平台](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/maopig/article/category/850267)
一、互斥与同步
1.互斥：是指某一资源同时只允许一个访问者对其进行访问，具有唯一性和排它性。但互斥无法限制访问者对资源的访问顺序，即访问是无序的。
2.同步：是指在互斥的基础上（大多数情况），通过其它机制实现访问者对资源的有序访问。在大多数情况下，同步已经实现了互斥，特别是所有写入资源的情况必定是互斥的。少数情况是指可以允许多个访问者同时访问资源.
3.同步是一种更为复杂的互斥，而互斥是一种特殊的同步.
二、通信与同步
进程间同步本身也是一种进程间通信(因为涉及信息的交换)，当然也是一种原始的进程间通信，但同时又是更高级的进程间通信机制的基石。
对线程亦然.
三、临界区(Critical section)与互斥体(Mutex)的区别
1、临界区只能用于对象在同一进程里线程间的互斥访问；互斥体可以用于对象进程间或线程间的互斥访问。
2、临界区是非内核对象，只在用户态进行锁操作，速度快；互斥体是内核对象，在核心态进行锁操作，速度慢。
3、临界区和互斥体在Windows平台都下可用；Linux下只有互斥体可用。
四、linux IPC
1.经典IPC：
(1)管道、命名管道       //最基本最常用
(2)消息队列、信号量、共享内存
//分为Posix IPC和System V IPC,共享存储是运行在同一台机器上的进程间通信最快的方式
2高级IPC： 流管道、命名流管道
(以上是限于同一台主机的各个进程间的IPC)
3.支持不同主机上各个进程的IPC：套接口、流
五、线程间通信机制:
1.互斥锁
2.条件变量
3.读写锁
4.线程信号量
5.线程信号 
