# Java可重入锁原理 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [biakia](http://www.jobbole.com/members/biakia) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 一、 概述
本文首先介绍Lock接口、ReentrantLock的类层次结构以及锁功能模板类AbstractQueuedSynchronizer的简单原理，然后通过分析ReentrantLock的lock方法和unlock方法，来解释ReentrantLock的内部原理，最后做一个总结。本文不涉及ReentrantLock中的条件变量。
### 1.1、Lock接口
Lock接口，是对控制并发的工具的抽象。它比使用synchronized关键词更灵活，并且能够支持条件变量。它是一种控制并发的工具，一般来说，它控制对某种共享资源的独占。也就是说，同一时间内只有一个线程可以获取这个锁并占用资源。其他线程想要获取锁，必须等待这个线程释放锁。在Java实现中的ReentrantLock就是这样的锁。另外一种锁，它可以允许多个线程读取资源，但是只能允许一个线程写入资源，ReadWriteLock就是这样一种特殊的锁，简称读写锁。下面是对Lock接口的几个方法的总体描述：
|方法名称|描述|
|----|----|
|lock|获取锁，如果锁无法获取，那么当前的线程就变为不可被调度，直到锁被获取到|
|lockInterruptibly|获取锁，除非当前线程被中断。如果获取到了锁，那么立即返回，如果获取不到，那么当前线程变得不可被调度，一直休眠直到下面两件事情发生：1、当前线程获取到了锁2、其他的线程中断了当前的线程|
|tryLock|如果调用的时候能够获取锁，那么就获取锁并且返回true，如果当前的锁无法获取到，那么这个方法会立刻返回false|
|tryLcok(long time,TimeUnit unit)|在指定时间内尝试获取锁如果可以获取锁，那么获取锁并且返回true，如果当前的锁无法获取，那么当前的线程变得不可被调度，直到下面三件事之一发生：1、当前线程获取到了锁2、当前线程被其他线程中断3、指定的等待时间到了|
|unlock|释放当前线程占用的锁|
|newCondition|返回一个与当前的锁关联的条件变量。在使用这个条件变量之前，当前线程必须占用锁。调用Condition的await方法，会在等待之前原子地释放锁，并在等待被唤醒后原子的获取锁|
接下来，我们将围绕lock和unlock这两个方法，来介绍整个ReentrantLock是怎么工作的。在介绍ReentrantLock之前，我们首先来看一下ReentrantLock的类层次结构以及和它密切相关的AbstractQueuedSynchronizer
### 1.2、ReentrantLock类层次结构
![](http://jbcdn2.b0.upaiyun.com/2016/12/26430e48fdc6db8ddcf8a5804f3ee208.png)
ReentrantLock实现了Lock接口，内部有三个内部类，Sync、NonfairSync、FairSync，Sync是一个抽象类型，它继承AbstractQueuedSynchronizer，这个AbstractQueuedSynchronizer是一个模板类，它实现了许多和锁相关的功能，并提供了钩子方法供用户实现，比如tryAcquire，tryRelease等。Sync实现了AbstractQueuedSynchronizer的tryRelease方法。NonfairSync和FairSync两个类继承自Sync，实现了lock方法，然后分别公平抢占和非公平抢占针对tryAcquire有不同的实现。
### 1.3、AbstractQueuedSynchronizer
首先，AbstractQueuedSynchronizer继承自AbstractOwnableSynchronizer，AbstractOwnableSynchronizer的实现很简单，它表示独占的同步器，内部使用变量exclusiveOwnerThread表示独占的线程。
其次，AbstractQueuedSynchronizer内部使用CLH锁队列来将并发执行变成串行执行。整个队列是一个双向链表。每个CLH锁队列的节点，会保存前一个节点和后一个节点的引用，当前节点对应的线程，以及一个状态。这个状态用来表明该线程是否应该block。当节点的前一个节点被释放的时候，当前节点就被唤醒，成为头部。新加入的节点会放在队列尾部。
## 二、 非公平锁的lock方法
### 2.1、lock方法流程图
![](http://jbcdn2.b0.upaiyun.com/2016/12/697ca5184743b788559af0c804a490c0.png)
### 2.2、lock方法详细描述
1、在初始化ReentrantLock的时候，如果我们不传参数是否公平，那么默认使用非公平锁，也就是NonfairSync。
2、当我们调用ReentrantLock的lock方法的时候，实际上是调用了NonfairSync的lock方法，这个方法先用CAS操作，去尝试抢占该锁。如果成功，就把当前线程设置在这个锁上，表示抢占成功。如果失败，则调用acquire模板方法，等待抢占。代码如下：

Java
```
static final class NonfairSync extends Sync {
        final void lock() {
            if (compareAndSetState(0, 1))
                setExclusiveOwnerThread(Thread.currentThread());
            else
                acquire(1);
        }
        protected final boolean tryAcquire(int acquires) {
            return nonfairTryAcquire(acquires);
        }
}
```
3、调用acquire(1)实际上使用的是AbstractQueuedSynchronizer的acquire方法，它是一套锁抢占的模板，总体原理是先去尝试获取锁，如果没有获取成功，就在CLH队列中增加一个当前线程的节点，表示等待抢占。然后进入CLH队列的抢占模式，进入的时候也会去执行一次获取锁的操作，如果还是获取不到，就调用LockSupport.park将当前线程挂起。那么当前线程什么时候会被唤醒呢？当持有锁的那个线程调用unlock的时候，会将CLH队列的头节点的下一个节点上的线程唤醒，调用的是LockSupport.unpark方法。acquire代码比较简单，具体如下：

Java
```
public final void acquire(int arg) {
        if (!tryAcquire(arg) &&
acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
            selfInterrupt();
}
```
3.1、acquire方法内部先使用tryAcquire这个钩子方法去尝试再次获取锁，这个方法在NonfairSync这个类中其实就是使用了nonfairTryAcquire，具体实现原理是先比较当前锁的状态是否是0，如果是0，则尝试去原子抢占这个锁（设置状态为1，然后把当前线程设置成独占线程），如果当前锁的状态不是0，就去比较当前线程和占用锁的线程是不是一个线程，如果是，会去增加状态变量的值，从这里看出可重入锁之所以可重入，就是同一个线程可以反复使用它占用的锁。如果以上两种情况都不通过，则返回失败false。代码如下：

Java
```
final boolean nonfairTryAcquire(int acquires) {
            final Thread current = Thread.currentThread();
            int c = getState();
            if (c == 0) {
                if (compareAndSetState(0, acquires)) {
                    setExclusiveOwnerThread(current);
                    return true;
                }
            }
            else if (current == getExclusiveOwnerThread()) {
                int nextc = c + acquires;
                if (nextc < 0) // overflow
                    throw new Error("Maximum lock count exceeded");
                setState(nextc);
                return true;
            }
            return false;
        }
```
3.2、tryAcquire一旦返回false，就会则进入acquireQueued流程，也就是基于CLH队列的抢占模式：
3.2.1、首先，在CLH锁队列尾部增加一个等待节点，这个节点保存了当前线程，通过调用addWaiter实现，这里需要考虑初始化的情况，在第一个等待节点进入的时候，需要初始化一个头节点然后把当前节点加入到尾部，后续则直接在尾部加入节点就行了。
代码如下：

Java
```
private Node addWaiter(Node mode) {
		// 初始化一个节点，这个节点保存当前线程
        Node node = new Node(Thread.currentThread(), mode);
        // 当CLH队列不为空的视乎，直接在队列尾部插入一个节点
        Node pred = tail;
        if (pred != null) {
            node.prev = pred;
            if (compareAndSetTail(pred, node)) {
                pred.next = node;
                return node;
            }
        }
		// 当CLH队列为空的时候，调用enq方法初始化队列
        enq(node);
        return node;
}
private Node enq(final Node node) {
        for (;;) {
            Node t = tail;
            if (t == null) { // 初始化节点，头尾都指向一个空节点
                if (compareAndSetHead(new Node()))
                    tail = head;
            } else {// 考虑并发初始化
                node.prev = t;
                if (compareAndSetTail(t, node)) {
                    t.next = node;
                    return t;
                }
            }
        }
}
```
3.2.2、将节点增加到CLH队列后，进入acquireQueued方法。
首先，外层是一个无限for循环，如果当前节点是头节点的下个节点，并且通过tryAcquire获取到了锁，说明头节点已经释放了锁，当前线程是被头节点那个线程唤醒的，这时候就可以将当前节点设置成头节点，并且将failed标记设置成false，然后返回。至于上一个节点，它的next变量被设置为null，在下次GC的时候会清理掉。
如果本次循环没有获取到锁，就进入线程挂起阶段，也就是shouldParkAfterFailedAcquire这个方法。
代码如下：

Java
```
final boolean acquireQueued(final Node node, int arg) {
        boolean failed = true;
        try {
            boolean interrupted = false;
            for (;;) {
                final Node p = node.predecessor();
                if (p == head && tryAcquire(arg)) {
                    setHead(node);
                    p.next = null; // help GC
                    failed = false;
                    return interrupted;
                }
                if (shouldParkAfterFailedAcquire(p, node) &&
                    parkAndCheckInterrupt())
                    interrupted = true;
            }
        } finally {
            if (failed)
                cancelAcquire(node);
        }
}
```
3.2.3、如果尝试获取锁失败，就会进入shouldParkAfterFailedAcquire方法，会判断当前线程是否挂起，如果前一个节点已经是SIGNAL状态，则当前线程需要挂起。如果前一个节点是取消状态，则需要将取消节点从队列移除。如果前一个节点状态是其他状态，则尝试设置成SIGNAL状态，并返回不需要挂起，从而进行第二次抢占。完成上面的事后进入挂起阶段。
代码如下：

Java
```
private static boolean shouldParkAfterFailedAcquire(Node pred, Node node) {
        int ws = pred.waitStatus;
        if (ws == Node.SIGNAL)
            //
            return true;
        if (ws > 0) {
            //
            do {
                node.prev = pred = pred.prev;
            } while (pred.waitStatus > 0);
            pred.next = node;
        } else {
            //
            compareAndSetWaitStatus(pred, ws, Node.SIGNAL);
        }
        return false;
    }
```
3.2.4、当进入挂起阶段，会进入parkAndCheckInterrupt方法，则会调用LockSupport.park(this)将当前线程挂起。代码：

Java
```
private final boolean parkAndCheckInterrupt() {
        LockSupport.park(this);
        return Thread.interrupted();
}
```
## 三、 非公平锁的unlock方法
### 3.1、unlock方法的活动图
![](http://jbcdn2.b0.upaiyun.com/2016/12/a5e128d3a07f60da644c3cb84bc2636a.png)
### 3.2、unlock方法详细描述
1、调用unlock方法，其实是直接调用AbstractQueuedSynchronizer的release操作。
2、进入release方法，内部先尝试tryRelease操作,主要是去除锁的独占线程，然后将状态减一，这里减一主要是考虑到可重入锁可能自身会多次占用锁，只有当状态变成0，才表示完全释放了锁。
3、一旦tryRelease成功，则将CHL队列的头节点的状态设置为0，然后唤醒下一个非取消的节点线程。
4、一旦下一个节点的线程被唤醒，被唤醒的线程就会进入acquireQueued代码流程中，去获取锁。
具体代码如下：
unlock代码：

Java
```
public void unlock() {
        sync.release(1);
}
```
release方法代码：

Java
```
public final boolean release(int arg) {
        if (tryRelease(arg)) {
            Node h = head;
            if (h != null && h.waitStatus != 0)
                unparkSuccessor(h);
            return true;
        }
        return false;
}
```
Sync中通用的tryRelease方法代码：

Java
```
protected final boolean tryRelease(int releases) {
     int c = getState() - releases;
    if (Thread.currentThread() != getExclusiveOwnerThread())
                throw new IllegalMonitorStateException();
    boolean free = false;
    if (c == 0) {
          free = true;
          setExclusiveOwnerThread(null);
    }
    setState(c);
    return free;
 }
```
unparkSuccessor代码：

Java
```
private void unparkSuccessor(Node node) {
        int ws = node.waitStatus;
        if (ws < 0)
            compareAndSetWaitStatus(node, ws, 0);
        Node s = node.next;
        if (s == null || s.waitStatus > 0) {
            s = null;
            for (Node t = tail; t != null && t != node; t = t.prev)
                if (t.waitStatus <= 0)
                    s = t;
        }
        if (s != null) 
            LockSupport.unpark(s.thread);
}
```
## 四、 公平锁和非公平锁的区别
公平锁和非公平锁，在CHL队列抢占模式上都是一致的，也就是在进入acquireQueued这个方法之后都一样，它们的区别在初次抢占上有区别，也就是tryAcquire上的区别，下面是两者内部调用关系的简图：

Java
```
NonfairSync
lock —> compareAndSetState
                | —> setExclusiveOwnerThread
      —> accquire
		     | —> tryAcquire
                           |—>nonfairTryAcquire
                |—> acquireQueued
FairSync
lock —> acquire
               | —> tryAcquire
                           |—>!hasQueuePredecessors
                           |—>compareAndSetState
                           |—>setExclusiveOwnerThread
               |—> acquireQueued
```
真正的区别就是公平锁多了hasQueuePredecessors这个方法，这个方法用于判断CHL队列中是否有节点，对于公平锁，如果CHL队列有节点，则新进入竞争的线程一定要在CHL上排队，而非公平锁则是无视CHL队列中的节点，直接进行竞争抢占，这就有可能导致CHL队列上的节点永远获取不到锁，这就是非公平锁之所以不公平的原因。
## 五、 总结
线程使用ReentrantLock获取锁分为两个阶段，第一个阶段是初次竞争，第二个阶段是基于CHL队列的竞争。在初次竞争的时候是否考虑队列节点直接区分出了公平锁和非公平锁。在基于CHL队列的锁竞争中，依靠CAS操作保证原子操作，依靠LockSupport来做线程的挂起和唤醒，使用队列来保证并发执行变成了串行执行，从而消除了并发所带来的问题。总体来说，ReentrantLock是一个比较轻量级的锁，而且使用面向对象的思想去实现了锁的功能，比原来的synchronized关键字更加好理解。
