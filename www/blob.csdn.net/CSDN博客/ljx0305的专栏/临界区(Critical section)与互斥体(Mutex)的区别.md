# 临界区(Critical section)与互斥体(Mutex)的区别 - ljx0305的专栏 - CSDN博客
2008年06月01日 20:40:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：2375标签：[windows																[linux																[平台](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
[临界区(Critical section)与互斥体(Mutex)的区别](http://blog.csdn.net/fisher_jiang/archive/2008/05/24/2475788.aspx)
1、临界区只能用于对象在同一进程里线程间的互斥访问；互斥体可以用于对象进程间或线程间的互斥访问。
2、临界区是非内核对象，只在用户态进行锁操作，速度快；互斥体是内核对象，在核心态进行锁操作，速度慢。
3、临界区和互斥体在Windows平台都下可用；Linux下只有互斥体可用。
