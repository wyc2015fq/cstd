# Java Thread Status(转) - weixin_33985507的博客 - CSDN博客
2014年09月21日 02:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**public static enum Thread.State  extends Enum<Thread.State>**线程状态。
线程可以处于下列状态之一： 
**1.NEW** 至今尚未启动的线程的状态。 
**2.RUNNABLE** 可运行线程的线程状态。
        处于可运行状态的某一线程正在 Java 虚拟机中运行，但它可能正在等待操作系统中的其他资源，比如处理器。 
**3.BLOCKED** 受阻塞并且正在等待监视器锁的某一线程的线程状态。
        处于受阻塞状态的某一线程正在等待监视器锁，以便进入一个同步的块/方法，或者在调用 Object.wait 之后再次进入同步的块/方法。
**4.WAITING** 某一等待线程的线程状态。某一线程因为调用下列方法之一而处于等待状态：
- 不带超时值的 Object.wait
- 不带超时值的 Thread.join
LockSupport.park
处于等待状态的线程正等待另一个线程，以执行特定操作。
 例如，已经在某一对象上调用了 Object.wait() 的线程正等待另一个线程，以便在该对象上调用 Object.notify() 或 Object.notifyAll()。
    已经调用了 Thread.join() 的线程正在等待指定线程终止。
**5.TIMED_WAITING**具有指定等待时间的某一等待线程的线程状态。某一线程因为调用以下带有指定正等待时间的方法之一而处于定时等待状态：
- Thread.sleep
- 带有超时值的 Object.wait
- 带有超时值的 Thread.join
- LockSupport.parkNanos
- LockSupport.parkUntil
**6.TERMINATED**
已终止线程的线程状态。线程已经结束执行。
**注意：在给定时间点上，一个线程只能处于一种状态。这些状态是虚拟机状态，它们并没有反映所有操作系统线程状态。**
为了展现线程在运行时的状态及其转换，我画了下面这个图
![](https://images0.cnblogs.com/blog/280044/201409/210219518789897.jpg)
[http://www.blogjava.net/cpegtop/articles/377980.html](http://www.blogjava.net/cpegtop/articles/377980.html)
