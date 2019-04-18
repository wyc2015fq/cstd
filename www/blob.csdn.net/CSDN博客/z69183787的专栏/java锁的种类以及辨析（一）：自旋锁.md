# java锁的种类以及辨析（一）：自旋锁 - z69183787的专栏 - CSDN博客
2017年09月29日 15:36:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：222
个人分类：[锁-概述](https://blog.csdn.net/z69183787/article/category/7202705)

锁作为并发共享数据，保证一致性的工具，在JAVA平台有多种实现(如 synchronized 和 ReentrantLock等等 ) 。这些已经写好提供的锁为我们开发提供了便利，但是锁的具体性质以及类型却很少被提及。本系列文章将分析JAVA下常见的锁名称以及特性，为大家答疑解惑。
### 1、自旋锁
自旋锁是采用让当前线程不停地的在循环体内执行实现的，当循环的条件被其他线程改变时 才能进入临界区。如下
|`01`|`public``class````SpinLock {```|
|`02`||
|`03`|`  ``private````AtomicReference<Thread> sign =````new``AtomicReference<>();`|
|`04`||
|`05`|`  ``public``void``lock(){`|
|`06`|`    ````Thread current = Thread.currentThread();```|
|`07`|`    ``while````(!sign .compareAndSet(````null````, current)){```|
|`08`|`    ``}`|
|`09`|`  ``}`|
|`10`||
|`11`|`  ``public``void````unlock (){```|
|`12`|`    ````Thread current = Thread.currentThread();```|
|`13`|`    ````sign .compareAndSet(current,````null``);`|
|`14`|`  ``}`|
|`15`|`}`|
使用了CAS原子操作，lock函数将owner设置为当前线程，并且预测原来的值为空。unlock函数将owner设置为null，并且预测值为当前线程。
当有第二个线程调用lock操作时由于owner值不为空，导致循环一直被执行，直至第一个线程调用unlock函数将owner设置为null，第二个线程才能进入临界区。
由于自旋锁只是将当前线程不停地执行循环体，不进行线程状态的改变，所以响应速度更快。但当线程数不停增加时，性能下降明显，因为每个线程都需要执行，占用CPU时间。如果线程竞争不激烈，并且保持锁的时间段。适合使用自旋锁。
注：该例子为非公平锁，获得锁的先后顺序，不会按照进入lock的先后顺序进行。
