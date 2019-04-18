# J.U.C--locks--AQS分析 - weixin_33985507的博客 - CSDN博客
2017年08月17日 11:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
> 
看一下AbstractQueuedSynchronizer(下面简称AQS)的子类就行知道，J.U.C中宣传的封装良好的同步工具类Semaphore、CountDownLatch、ReentrantLock、ReentrantReadWriteLock、FutureTask等尽管各自都有不同特征，可是其内部的实现都与AQS分不开。
所以分析AQS的实现原理对其余显示锁或则同步工具类的理解非常重要。
# 1.主要属性和内部类
这一篇blog主要分析AQS的实现中的重要属性和内部类。尤其是对于ReentrantLock和ReentrantReadWriteLock。其lock()方法和unlock()方法的实现终于都是由AQS同步器实现的。由此可见分析AQS类的重要性可见一斑。
在AQS中，我们先看属性远比看方法来的更加easy理解这个类的作用。首先看AQS类的主要属性：
```java
//等待队列的头指针
private transient volatile Node head;
//等待队列的尾指针
private transient volatile Node tail;
//同步器的状态位，注意这里state是声明了volatile。保证了可视性
private volatile int state;
```
凝视事实上已经告诉我们了。Node类型的 head 和 tail 是一个FIFO的wait queue。一个int类型的状态位state。到这里也能猜到AQS对外呈现（或者说声明）的主要行为就是由一个状态位和一个有序队列来配合完毕。
## state属性
对于state状态的管理，在AQS中仅仅通过三个方法来实现：
```java
java.util.concurrent.locks.AbstractQueuedSynchronizer.getState();
java.util.concurrent.locks.AbstractQueuedSynchronizer.setState(int);
java.util.concurrent.locks.AbstractQueuedSynchronizer.compareAndSetState(int, int);
```
前面两个函数事实上就是get和set方法。
第三个函数事实上是通过Unsafe类实现CAS设置状态值，CAS+volatile 保证了state变量的线程安全。
## Node结点
前面还提到了同步器的实现还依赖于一个FIFO的队列。队列中的元素Node就是保存着线程引用和线程状态的容器，每一个线程对同步器的訪问。都可以看做是队列中的一个节点。
Node类的源代码不多，我直接所有粘贴出来：
```java
static final class Node {
    static final Node SHARED = new Node();
    static final Node EXCLUSIVE = null;
    static final int CANCELLED =  1;
    static final int SIGNAL    = -1;
    static final int CONDITION = -2;
    static final int PROPAGATE = -3;
    volatile int waitStatus;
    volatile Node prev;
    volatile Node next;
    volatile Thread thread;
    Node nextWaiter;
    final boolean isShared() {
        return nextWaiter == SHARED;
    }
    final Node predecessor() throws NullPointerException {
        Node p = prev;
        if (p == null)
            throw new NullPointerException();
        else
            return p;
    }
    /** 构造器 */
    Node() {    // Used to establish initial head or SHARED marker
    }
    /** 构造器 */
    Node(Thread thread, Node mode) {     // Used by addWaiter
        this.nextWaiter = mode;
        this.thread = thread;
    }
    /** 构造器 */
    Node(Thread thread, int waitStatus) { // Used by Condition
        this.waitStatus = waitStatus;
        this.thread = thread;
    }
}
```
Node类主要有5个属性：
```java
volatile int waitStatus;//
volatile Node prev;//
volatile Node next;//
volatile Thread thread;//
Node nextWaiter;//
```
以上五个成员变量主要负责保存该节点的线程引用，同步等待队列（下面简称sync队列）的前驱和后继节点。同一时候也包括了同步状态。 
对这5个变量的解释例如以下：
|属性名称|描写叙述|
|----|----|
|int waitStatus|表示节点的状态。当中包括的状态有：1.CANCELLED，值为1，表示当前的线程被取消。2.SIGNAL，值为-1。表示当前节点的后继节点包括的线程须要执行，也就是unpark；3.CONDITION，值为-2。表示当前节点在等待condition，也就是在condition队列中；4.PROPAGATE，值为-3。表示当前场景下兴许的acquireShared可以得以执行；5.值为0，表示当前节点在sync队列中。等待着获取锁。|
|Node prev|前驱节点。比方当前节点被取消，那就须要前驱节点和后继节点来完毕连接。|
|Node next|后继节点。|
|Node nextWaiter|存储condition队列中的后继节点。|
|Thread thread|入队列时的当前线程。|
节点成为sync队列和condition队列构建的基础，在同步器中就包括了sync队列。
同步器拥有三个成员变量：sync队列的头结点head、sync队列的尾节点tail和状态state。对于锁的获取。请求形成节点，将其挂载在尾部。而锁资源的转移（释放再获取）是从头部開始向后进行。对于同步器维护的状态state，多个线程对其的获取将会产生一个链式的结构。
![这里写图片描写叙述](https://img-blog.csdn.net/20170126231502764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1MzI2MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 2.重要函数的源代码解析
### 获取锁相关函数
```java
acquire(int arg);//以独占模式获取对象，忽略中断。
acquireInterruptibly(int arg);//以独占模式获取对象。假设被中断则中止。
acquireShared(int arg);//以共享模式获取对象，忽略中断。
acquireSharedInterruptibly(int arg);//以共享模式获取对象，假设被中断则中止。
tryAcquire(int arg);//试图在独占模式下获取对象状态。
tryAcquireNanos(int arg, long nanosTimeout);//试图以独占模式获取对象，假设被中断则中止。假设到了给定超时时间，则会失败。
tryAcquireShared(int arg);//试图在共享模式下获取对象状态。
tryAcquireSharedNanos(int arg, long nanosTimeout);//试图以共享模式获取对象，假设被中断则中止。假设到了给定超时时间。则会失败。
```
```java
```
```java
```
### 释放锁相关函数
```java
release(int arg);//以独占模式释放对象。
releaseShared(int arg);//以共享模式释放对象
tryRelease(int arg);//试图设置状态来反映独占模式下的一个释放。
tryReleaseShared(int arg);//试图设置状态来反映共享模式下的一个释放。
```
## 1）acquire(int arg)函数
首先看看Javadoc的定义：
> 
以独占模式获取对象，忽略中断。
通过至少调用一次 tryAcquire(int) 来实现此方法，并在成功时返回。否则在成功之前，一直调用 tryAcquire(int) 将线程加入队列，线程可能反复被堵塞或不被堵塞。可以使用此方法来实现 Lock.lock() 方法。
可知该函数是以独占模式获取对象而且忽略中断，完毕synchronized语义。
在AQS类中的源代码例如以下：
```java
public final void acquire(int arg) {
    if (!tryAcquire(arg) &&
        acquireQueued(addWaiter(Node.EXCLUSIVE), arg))
        selfInterrupt();
}
```
该函数主要完毕的逻辑例如以下： 
1）首先调用tryAcquire(arg)函数尝试获取； 
尝试更改状态state的值，而且保证原子性。
在tryAcquire方法中使用了同步器提供的对state操作的方法。利用compareAndSet保证仅仅有一个线程可以对状态进行成功改动，而没有成功改动的线程将进入sync队列排队。
值得注意的是这个函数在AQS中并没有实现，而是在其继承子类中实现（比方在ReentrantLock类中的内部类中NonfairSync和FairSync中均实现了这种方法）。
当获取成功时，就会返回true,这时源代码中的if语句就会直接执行if(0),也就是不满足执行条件。
2）假设获取不到，将当前线程构造成节点Node并加入sync队列。 
进入队列的每一个线程都是一个节点Node，从而形成了一个双向队列，相似CLH队列，这样做的目的是线程间的通信会被限制在较小规模（也就是两个节点左右）。
3）再次尝试获取。假设没有获取到那么将当前线程从线程调度器上摘下。进入等待状态。
使用LockSupport将当前线程unpark，关于LockSupport兴许会具体介绍。
看看addWaiter()方法的逻辑：
> 
凝视解释的是：通过给定的模式和当前线程创建同步队列结点。
源代码例如以下：
```java
private Node addWaiter(Node mode) {
    Node node = new Node(Thread.currentThread(), mode);
    // 高速尝试在尾部加入
    Node pred = tail;
    if (pred != null) {
        node.prev = pred;
        if (compareAndSetTail(pred, node)) {
            pred.next = node;
            return node;
        }
    }
    //
    enq(node);
    return node;
}
//Inserts node into queue, initializing if necessary. See picture above.
private Node enq(final Node node) {
    //死循环直至return
    for (;;) {
        Node t = tail;
        //必须初始化的步骤
        if (t == null) { 
            if (compareAndSetHead(new Node()))
                tail = head;
        } else {
            node.prev = t;
            if (compareAndSetTail(t, node)) {
                t.next = node;
                return t;
            }
        }
    }
}
```
上面的逻辑解释： 
1）使用当前线程构造Node； 
对于一个新创建的节点须要做的是：将新节点前驱节点指向尾节点（current.prev = tail）。尾节点指向它（tail = current），原有的尾节点的后继节点指向它（t.next = current）而这些操作要求是原子的。上面的操作是利用尾节点的设置来保证的，也就是compareAndSetTail来完毕的。
2）先行尝试在队尾加入； 
假设尾节点已经有了。然后做例如以下操作： 
(1)分配引用pred指向尾节点。 
(2)调用compareAndSetTail(pred, node)将新节点更新为尾节点； 
(3)直接return，返回新插入的结点。
3）假设队尾加入失败或者是第一个入队的节点。 
假设是第1个节点，也就是sync队列没有初始化，那么会进入到enq这种方法，进入的线程可能有多个，或者说在addWaiter中没有成功入队的线程都将进入enq这种方法。
enq(node)函数的逻辑是确保进入的Node都会有机会顺序的加入到sync队列中，而加入的过程例如以下： 
(1)假设尾节点为空，那么原子化的分配一个头节点。并将尾节点指向头节点，这一步是初始化； 
(2)然后是反复在addWaiter中做的工作，可是在一个for (;;)的循环中，直到当前节点入队为止。
至此。addWaiter()方法的逻辑分析完毕。接下来就是分析(final Node node, int arg) 方法的逻辑。
进入sync队列之后。接下来就是要进行锁的获取，或者说是訪问控制了。仅仅有一个线程可以在同一时刻继续的执行。而其它的进入等待状态。而每一个线程都是一个独立的个体，它们自省的观察。当条件满足的时候（自己的前驱是头结点而且原子性的获取了状态）。那么这个线程可以继续执行。
```java
final boolean acquireQueued(final Node node, int arg) {
    boolean failed = true;
    try {
        boolean interrupted = false;
        for (;;) {
            //获取前驱结点
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
上述逻辑主要包括： 
1. 获取当前节点的前驱节点； 
须要获取当前节点的前驱节点。而头结点所相应的含义是当前站有锁且正在执行。 
2. 当前驱节点是头结点而且可以获取状态，代表该当前节点占有锁； 
假设满足上述条件，那么代表可以占有锁。根据节点对锁占有的含义，设置头结点为当前节点。 
3. 否则进入等待状态。 
假设没有轮到当前节点执行。那么将当前线程从线程调度器上摘下。也就是进入等待状态。
这里针对acquire做一下总结： 
1. 状态的维护； 
须要在锁定时。须要维护一个状态(int类型)。而对状态的操作是原子和非堵塞的，通过同步器提供的对状态訪问的方法对状态进行操纵，而且利用compareAndSet来确保原子性的改动。 
2. 状态的获取； 
一旦成功的改动了状态，当前线程或者说节点，就被设置为头节点。 
3. sync队列的维护。
在获取资源未果的过程中条件不符合的情况下(不该自己，前驱节点不是头节点或者没有获取到资源)进入睡眠状态，停止线程调度器对当前节点线程的调度。
这时引入的一个释放的问题，也就是说使睡眠中的Node或者说线程获得通知的关键，就是前驱节点的通知，而这一个过程就是释放。释放会通知它的后继节点从睡眠中返回准备执行。 
下面的流程图基本描写叙述了一次acquire所须要经历的过程： 
![这里写图片描写叙述](https://img-blog.csdn.net/20170128204445598?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDg1MzI2MQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如上图所看到的，当中的判定退出队列的条件，判定条件是否满足和休眠当前线程就是完毕了自旋spin的过程。
## 2）release(int arg)
首先看看Javadoc的定义：
> 
以独占模式释放对象。假设 tryRelease(int) 返回 true。则通过消除一个或多个线程的堵塞来实现此方法。可以使用此方法来实现 Lock.unlock() 方法 
  源代码例如以下：
```java
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
在unlock方法的实现中。使用了同步器的release方法。
相对于在之前的acquire方法中可以得出调用acquire，保证可以获取到锁（成功获取状态），而release则表示将状态设置回去，也就是将资源释放，或者说将锁释放。
上述逻辑主要包括： 
1）尝试释放状态。 
tryRelease()函数可以保证原子化的将状态设置回去。当然须要使用compareAndSet来保证。假设释放状态成功过之后。将会进入后继节点的唤醒过程。 
2. 唤醒当前节点的后继节点所包括的线程。 
通过LockSupport的unpark方法将休眠中的线程唤醒，让其继续acquire状态。
```java
private void unparkSuccessor(Node node) {
    // 将状态设置为同步状态
    int ws = node.waitStatus;
    if (ws < 0)         
        compareAndSetWaitStatus(node, ws, 0);   
    // 获取当前节点的后继节点，假设满足状态，那么进行唤醒操作  
    // 假设没有满足状态，从尾部開始找寻符合要求的节点并将其唤醒     
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
上述逻辑主要包括，该方法取出了当前节点的next引用，然后对其线程(Node)进行了唤醒。这时就仅仅有一个或合理个数的线程被唤醒。被唤醒的线程继续进行对资源的获取与争夺。
回想整个资源的获取和释放过程： 
1)在获取时，维护了一个sync队列。每一个节点都是一个线程在进行自旋，而根据就是自己是否是首节点的后继而且可以获取资源； 
2)在释放时。仅仅须要将资源还回去。然后通知一下后继节点并将其唤醒。
这里须要注意，队列的维护（首节点的更换）是依靠消费者（获取时）来完毕的，也就是说在满足了自旋退出的条件时的一刻。这个节点就会被设置成为首节点。
至此AQS基本的两个函数分析完毕。这两个函数也是lock()函数和unlock()函数的核心。
