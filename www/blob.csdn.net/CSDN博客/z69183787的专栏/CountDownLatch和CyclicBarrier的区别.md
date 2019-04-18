# CountDownLatch和CyclicBarrier的区别 - z69183787的专栏 - CSDN博客
2015年10月23日 12:01:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：574
在网上看到很多人对于CountDownLatch和CyclicBarrier的区别简单理解为CountDownLatch是一次性的，而CyclicBarrier在调用reset之后还可以继续使用。那如果只是这么简单的话，我觉得CyclicBarrier简单命名为**ResetableCountDownLatch**好了，显然不是的。
我的理解是，要从他们的设计目的去看这两个类。javadoc里面的描述是这样的。
> 
CountDownLatch: A synchronization aid that allows one or more threads to wait until a set of operations being performed in other threads completes.
CyclicBarrier : A synchronization aid that allows a set of threads to all wait for each other to reach a common barrier point.
可能是我的英语不够好吧， 我感觉从这个javadoc里面要准确理解他们的差异还是不容易的。
我的理解是
CountDownLatch : **一个线程**(或者多个)， 等待另外**N个线程**完成**某个事情**之后才能执行。   CyclicBarrier        : **N个线程**相互等待，任何一个线程完成之前，所有的线程都必须等待。
这样应该就清楚一点了，对于CountDownLatch来说，重点是那个**“一个线程”**, 是它在等待， 而另外那N的线程在把**“某个事情”**做完之后可以继续等待，可以终止。而对于CyclicBarrier来说，重点是那**N个线程**，他们之间任何一个没有完成，所有的线程都必须等待。
CountDownLatch 是计数器, 线程完成一个就记一个, 就像 报数一样, 只不过是递减的.
而CyclicBarrier更像一个水闸, 线程执行就想水流, 在水闸处都会堵住, 等到水满(线程到齐)了, 才开始泄流.
