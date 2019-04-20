# 细谈CAS与ABA - Spark高级玩法 - CSDN博客
2018年04月09日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：154
题目：如何实现乐观锁（CAS），如何避免ABA问题？
![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/fgnkxfGnnkQHrbiaR05ibWjdTib5rVzbZRE7f8VcgckIV4tiaVMvquMYiaQkJj2VfdGuO34ZTGqIqqjzMkDfEsbx9TA/640?wxfrom=5&wx_lazy=1)
本文阅读大概需要15分钟。
这个题主要考查原子操作、悲观锁、乐观锁及ABA问题。
一
原子操作 
原子在化学中，有基本粒子，不可再分之意（现在早已不是不可再分），计算机科学因此引入了原子操作这一名词，用来指代某一或一系列计算机操作具有原子性。原子操作是不可分割的，在执行完毕之前不会被任何其它任务或事件中断。例如在计算机底层的汇编指令，每条指令都是原子操作，每条指令只有执行完成与未执行两个状态，不存在执行一半的情况。在软件开发体系中原子操作也比比皆是，如数据库操作中的事务就是原子操作，事物的执行只有两个状态，执行成功或执行失败，不会存在执行了一半成功一半失败的情况，事务中某一操作的执行失败，都会触发回滚操作，将当前状态恢复到事物执行前状态。如果用有限状态机来描述原子操作，那就是原子操作只会在原态与执行成功两个状态之间来回转换，不存在其他状态（也不会产生其他状态）。原子操作有限状态机如下图所示。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/R5ic1icyNBNd478iaib6TUr1rwVLQyicULy3jHVV1fLSHXnUuc6nGjez6DRnOR1zYGu09VKr9E3ppicNdIGBeGr1oVDQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
二
独占锁（悲观锁）与乐观锁 
在多线程编程的时候，为了保证多个线程对一个对象可以安全进行访问时,我们需要加同步锁synchronized,保证对象的在使用时的正确性，synchronized就是一种独占锁，它会导致所有需要此锁的线程挂起，等待锁的释放。
加锁会导致一下问题：
- 
加多线程竞争下，加锁和释放锁会导致较多的上下文切换，引起性能问题。
- 
多线程可以导致死锁的问题。
- 
多线程持有的锁会导致其他需要此锁的线程挂起。
乐观锁（CAS）的思想却是不加锁，那不加锁如何确保某一变量的操作没有被其他线程修改过？
这里就需要CAS操作（CompareAndSwap）来实现。
CAS有三个操作参数：内存地址，期望值，要修改的新值，当期望值和内存当中的值进行比较不相等的时候，表示内存中的值已经被别线程改动过，这时候失败返回，只有相等时，才会将内存中的值改为新的值，并返回成功。
在JVM中的CAS操作就是基于处理器的CMPXCHG汇编指令实现的，因此，JVM中的CAS的原子性是处理器保障的。
这里我们可以看一下JAVA的原子类AtomicLong.getAndIncrement()的实现，来理解一下CAS这一乐观锁（JDK 1.8）。
public final long getAndIncrement() {
    return unsafe.getAndAddLong(this, valueOffset, 1L);
}
接着看一下 Unsafe.getAndAddLong()的实现：
public final long getAndAddLong(Object var1, long var2, long var4) {
    long var6;
    do {
        var6 = this.getLongVolatile(var1, var2);
    } while(!this.compareAndSwapLong(var1, var2, var6, var6 + var4));
    return var6;
}
这里我们可以看到AtomicLong.getAndIncrement()的实现就是通过CAS循环操作的实现，只有期望值与真实值相同情况下，CAS操作才会成功执行，退出循环，如果失败则继续自旋，直到成功。
三
ABA问题
ABA问题是指在CAS操作时，其他线程将变量值A改为了B，但是又被改回了A，等到本线程使用期望值A与当前变量进行比较时，发现变量A没有变，于是CAS就将A值进行了交换操作，但是实际上该值已经被其他线程改变过，这与乐观锁的设计思想不符合。ABA问题的解决思路是，每次变量更新的时候把变量的版本号加1，那么A-B-A就会变成A1-B2-A3，只要变量被某一线程修改过，改变量对应的版本号就会发生递增变化，从而解决了ABA问题。在JDK的java.util.concurrent.atomic包中提供了AtomicStampedReference来解决ABA问题，该类的compareAndSet是该类的核心方法，实现如下：
public boolean compareAndSet(V   expectedReference,
                             V   newReference,
                             int expectedStamp,
                             int newStamp) {
    Pair<V> current = pair;
    return
        expectedReference == current.reference &&
        expectedStamp == current.stamp &&
        ((newReference == current.reference &&
          newStamp == current.stamp) ||
         casPair(current, Pair.of(newReference, newStamp)));
}
我们可以发现，该类检查了当前引用与当前标志是否与预期相同，如果全部相等，才会以原子方式将该引用和该标志的值设为新的更新值，这样CAS操作中的比较就不依赖于变量的值了。
**记住****，这里全部都是干货！！！**
