# Java 线程池 ThreadPoolExecutor 源码分析 - Soul Joy Hub - CSDN博客

2017年03月09日 17:05:00[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：354标签：[线程池																[java																[源码																[线程](https://so.csdn.net/so/search/s.do?q=线程&t=blog)](https://so.csdn.net/so/search/s.do?q=源码&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=线程池&t=blog)
个人分类：[Java																[并发/网络编程](https://blog.csdn.net/u011239443/article/category/6268713)](https://blog.csdn.net/u011239443/article/category/6296471)



转载请注明本文出自 clevergump 的博客：[](http://blog.csdn.net/clevergump/article/details/50688008)[](http://blog.csdn.net/clevergump/article/details/50688008)[http://blog.csdn.net/clevergump/article/details/50688008](http://blog.csdn.net/clevergump/article/details/50688008), 谢谢!

线程池能够对线程进行有效的管理, 复用和数量上限的限制, 如果你需要创建多个线程来执行多个异步任务, 那么使用线程池显然要比频繁地 new Thread().start() 这种方式要好.

Java 中的线程池是用 **ThreadPoolExecutor** 类来表示的. 我们今天就结合该类的源码来分析一下这个类内部对于线程的创建, 管理以及后台任务的调度等方面的执行原理. 我这里分析的是 Oracle JDK 1.8 的源码.

## 1.  ctl

ThreadPoolExecutor 类中有个非常重要的字段 ctl,  ctl 其实可以理解为单词 control 的简写, 翻译过来就是 “控制”,  具体来说就是对线程池的运行状态和池子中有效线程的数量进行控制的一个字段. 我们看下该字段在源码中的定义:

```java
/**
 * The main pool control state, ctl, is an atomic integer packing
 * two conceptual fields
 *   workerCount, indicating the effective number of threads
 *   runState,    indicating whether running, shutting down etc
 *
 * In order to pack them into one int, we limit workerCount to
 * (2^29)-1 (about 500 million) threads rather than (2^31)-1 (2
 * billion) otherwise representable. If this is ever an issue in
 * the future, the variable can be changed to be an AtomicLong,
 * and the shift/mask constants below adjusted. But until the need
 * arises, this code is a bit faster and simpler using an int.
 *
 * The workerCount is the number of workers that have been
 * permitted to start and not permitted to stop.  The value may be
 * transiently different from the actual number of live threads,
 * for example when a ThreadFactory fails to create a thread when
 * asked, and when exiting threads are still performing
 * bookkeeping before terminating. The user-visible pool size is
 * reported as the current size of the workers set.
 *
 * The runState provides the main lifecycle control, taking on values:
 *
 *   RUNNING:  Accept new tasks and process queued tasks
 *   SHUTDOWN: Don't accept new tasks, but process queued tasks
 *   STOP:     Don't accept new tasks, don't process queued tasks,
 *             and interrupt in-progress tasks
 *   TIDYING:  All tasks have terminated, workerCount is zero,
 *             the thread transitioning to state TIDYING
 *             will run the terminated() hook method
 *   TERMINATED: terminated() has completed
 *
 * The numerical order among these values matters, to allow
 * ordered comparisons. The runState monotonically increases over
 * time, but need not hit each state. The transitions are:
 *
 * RUNNING -> SHUTDOWN
 *    On invocation of shutdown(), perhaps implicitly in finalize()
 * (RUNNING or SHUTDOWN) -> STOP
 *    On invocation of shutdownNow()
 * SHUTDOWN -> TIDYING
 *    When both queue and pool are empty
 * STOP -> TIDYING
 *    When pool is empty
 * TIDYING -> TERMINATED
 *    When the terminated() hook method has completed
 *
 * Threads waiting in awaitTermination() will return when the
 * state reaches TERMINATED.
 *
 * Detecting the transition from SHUTDOWN to TIDYING is less
 * straightforward than you'd like because the queue may become
 * empty after non-empty and vice versa during SHUTDOWN state, but
 * we can only terminate if, after seeing that it is empty, we see
 * that workerCount is 0 (which sometimes entails a recheck -- see
 * below).
 */
private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));
```

ctl 是一个 AtomicInteger 对象,  也就是一个特殊的 int 型变量, 特殊之处在于所有需要修改其数值的操作都是原子化的.  如果你不熟悉原子化 (atomic) 这个概念, 那么你可以将它简单理解为 synchronized,  即: 所有修改其数值的操作都需要在加了同步锁的情况下来进行.

一个 ctl 变量可以包含两部分信息: 线程池的运行状态 (runState) 和线程池内有效线程的数量 (workerCount).  由于 int 型的变量是由32位二进制的数构成, 所以用 ctl 的高3位来表示线程池的运行状态, 用低29位来表示线程池内有效线程的数量. 由于这两部分信息在该类中很多地方都会使用到, 所以我们也经常会涉及到要获取其中一个信息的操作, 通常来说, 代表这两个信息的变量的名称直接用他们各自英文单词首字母的组合来表示, 所以, 表示线程池运行状态的变量通常命名为 rs, 表示线程池中有效线程数量的变量通常命名为 wc, 另外, ctl 也通常会简写作 c, 你一定要对这里提到的几个变量名稍微留个印象哦.  如果你在该类源码的某个地方遇到了见名却不知意的变量名时, 你在抱怨这糟糕的命名的时候, 要试着去核实一下, 那些变量是不是正是这里提到的几个信息哦. 

由于 ctl 变量是由线程池的运行状态 (runState) 和线程池内有效线程的数量 (workerCount)这两个信息组合而成, 所以, 如果知道了这两部分信息各自的数值, 就可以调用下面的 ctlOf() 方法来计算出 ctl 的数值:

```java
// rs: 表示线程池的运行状态 (rs 是 runState中各单词首字母的简写组合)
// wc: 表示线程池内有效线程的数量 (wc 是 workerCount中各单词首字母的简写组合)
private static int ctlOf(int rs, int wc) { return rs | wc; }
```

反过来, 如果知道了 ctl 的值, 那么也可以通过如下的 runStateOf() 和 workerCountOf() 两个方法来分别获取线程池的运行状态和线程池内有效线程的数量.

```java
private static int runStateOf(int c)     { return c & ~CAPACITY; }
private static int workerCountOf(int c)  { return c & CAPACITY; }
```

其中,  CAPACITY 等于 (2^29)-1, 也就是高3位是0, 低29位是1的一个int型的数, 

```java
private static final int COUNT_BITS = Integer.SIZE - 3;     // 29
private static final int CAPACITY = (1 << COUNT_BITS) - 1;  // COUNT_BITS == 29
```

所以上边两个方法的计算过程也就不难理解了吧 (ps: 如果此时你还是不理解这两个方法的计算过程, 请先学习二进制位运算的相关知识, 然后再来看这两个方法, 你会发现他们很容易理解的).  另外, CAPACITY 这个常量从名字上可以知道, 该常量表示某个容量值, 那么表示的是什么容量值呢? 其实, 我们前面介绍过, ctl 只用他的低29位来表示线程池内的有效线程数, 也就是说, 线程池内有效线程的数量上限就是29个二进制1所表示的数值 (约为5亿), 而线程池就是用 CAPACITY 这个常量来表示这个上限数值的. 

下面再介绍下线程池的运行状态. 线程池一共有五种状态, 分别是:

> 
**① RUNNING (运行状态):**  能接受新提交的任务, 并且也能处理阻塞队列中的任务. 
**② SHUTDOWN (关闭状态):** 不再接受新提交的任务, 但却可以继续处理阻塞队列中已保存的任务. 在线程池处于 RUNNING 状态时, 调用 shutdown()方法会使线程池进入到该状态. 当然, finalize() 方法在执行过程中或许也会隐式地进入该状态. 
**③ STOP :**   不能接受新提交的任务, 也不能处理阻塞队列中已保存的任务, 并且会中断正在处理中的任务. 在线程池处于 RUNNING 或 SHUTDOWN 状态时, 调用 shutdownNow() 方法会使线程池进入到该状态.  
**④ TIDYING (清理状态):**  所有的任务都已终止了, workerCount (有效线程数) 为0, 线程池进入该状态后会调用 terminated() 方法以让该线程池进入TERMINATED 状态. 当线程池处于 SHUTDOWN 状态时, 如果此后线程池内没有线程了并且阻塞队列内也没有待执行的任务了 (即: 二者都为空), 线程池就会进入到该状态.  当线程池处于 STOP 状态时, 如果此后线程池内没有线程了, 线程池就会进入到该状态. 
**⑤ TERMINATED :** terminated() 方法执行完后就进入该状态.

中文翻译可能不太准确, 也不能充分表达源码所表示的所有含义, 还可能造成歧义, 例如: STOP 和 TERMINATED 似乎翻译过来的意思没太大区别啊. 所以我们在描述线程池的运行状态时, 建议直接使用上面的5个英文单词来表示. 这五种状态的具体数值如下:

```java
// runState is stored in the high-order bits
private static final int RUNNING    = -1 << COUNT_BITS;
private static final int SHUTDOWN   =  0 << COUNT_BITS;
private static final int STOP       =  1 << COUNT_BITS;
private static final int TIDYING    =  2 << COUNT_BITS;
private static final int TERMINATED =  3 << COUNT_BITS;
```

前边提到过 COUNT_BITS == 29.  其实我们只需要知道,  上边这5个常量是按照从小到大的顺序列出的即可. 如果你在源码中看到  

rs < SHUTDOWN (假如用 rs 代表线程池的运行状态), 那么你要知道, 这表示线程池处于 RUNNING 状态.

## 2. 几个重要的参数

ThreadPoolExecutor 类的构造方法中提供了几个非常重要的参数, 这几个参数也对应着该类中的几个同名的字段. 理解这几个重要参数/字段的含义, 将有助于我们分析线程池对线程调度的原理. 下面我们就来看看该类的构造方法吧, 如下图所示:

![这里写图片描述](https://img-blog.csdn.net/20160228113035248)

前三个方法最终都会去调用第四个方法, 也就是参数数量最多的那个方法, 所以我们来看看这第四个方法的源码, 如下:

```java
/**
 * Creates a new {@code ThreadPoolExecutor} with the given initial
 * parameters.
 *
 * @param corePoolSize the number of threads to keep in the pool, even
 *        if they are idle, unless {@code allowCoreThreadTimeOut} is set
 * @param maximumPoolSize the maximum number of threads to allow in the
 *        pool
 * @param keepAliveTime when the number of threads is greater than
 *        the core, this is the maximum time that excess idle threads
 *        will wait for new tasks before terminating.
 * @param unit the time unit for the {@code keepAliveTime} argument
 * @param workQueue the queue to use for holding tasks before they are
 *        executed.  This queue will hold only the {@code Runnable}
 *        tasks submitted by the {@code execute} method.
 * @param threadFactory the factory to use when the executor
 *        creates a new thread
 * @param handler the handler to use when execution is blocked
 *        because the thread bounds and queue capacities are reached
 * @throws IllegalArgumentException if one of the following holds:<br>
 *         {@code corePoolSize < 0}<br>
 *         {@code keepAliveTime < 0}<br>
 *         {@code maximumPoolSize <= 0}<br>
 *         {@code maximumPoolSize < corePoolSize}
 * @throws NullPointerException if {@code workQueue}
 *         or {@code threadFactory} or {@code handler} is null
 */
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler) {
    if (corePoolSize < 0 ||
        maximumPoolSize <= 0 ||
        maximumPoolSize < corePoolSize ||
        keepAliveTime < 0)
        throw new IllegalArgumentException();
    if (workQueue == null || threadFactory == null || handler == null)
        throw new NullPointerException();
    this.corePoolSize = corePoolSize;
    this.maximumPoolSize = maximumPoolSize;
    this.workQueue = workQueue;
    this.keepAliveTime = unit.toNanos(keepAliveTime);
    this.threadFactory = threadFactory;
    this.handler = handler;
}
```

从上述源码可知, 传递的参数必须符合如下要求:

> 
(1) corePoolSize >= 0,  maximumPoolSize > 0,  maximumPoolSize >= corePoolSize, keepAliveTime >= 0 

  (2) workQueue, threadFactory, handler 还不能为null. 

如果传递的所有参数都符合上述要求, 那么就会执行后边的6个赋值语句, 将6个参数分别赋值给该类内部的6个成员字段. 接下来我们就分别分析一下这6个参数各自的含义. 而要想正确理解这些参数以及对应字段的含义, 需要同时结合该构造方法的注释以及对应字段的注释, 对应字段的getter(), setter()方法的注释才能基本无误地理解透彻, 有时甚至还需要结合源码才能真正理解. 下面我们来逐一分析这几个参数(其实也就是分析6个成员字段的含义).
- **corePoolSize**

将该类最前边关于 Core and maximum pool sizes 和 On-demand construction 的注释, 构造方法对该参数 corePoolSize 的注释, 以及该类对同名字段 corePoolSize 的注释汇总如下:

```java
/**
 * <dt>Core and maximum pool sizes</dt>
 *
 * <dd>A {@code ThreadPoolExecutor} will automatically adjust the
 * pool size (see {@link #getPoolSize})
 * according to the bounds set by
 * corePoolSize (see {@link #getCorePoolSize}) and
 * maximumPoolSize (see {@link #getMaximumPoolSize}).
 *
 * When a new task is submitted in method {@link #execute(Runnable)},
 * and fewer than corePoolSize threads are running, a new thread is
 * created to handle the request, even if other worker threads are
 * idle.  If there are more than corePoolSize but less than
 * maximumPoolSize threads running, a new thread will be created only
 * if the queue is full.  By setting corePoolSize and maximumPoolSize
 * the same, you create a fixed-size thread pool. By setting
 * maximumPoolSize to an essentially unbounded value such as {@code
 * Integer.MAX_VALUE}, you allow the pool to accommodate an arbitrary
 * number of concurrent tasks. Most typically, core and maximum pool
 * sizes are set only upon construction, but they may also be changed
 * dynamically using {@link #setCorePoolSize} and {@link
 * #setMaximumPoolSize}. </dd>
 *
 * <dt>On-demand construction</dt>
 *
 * <dd>By default, even core threads are initially created and
 * started only when new tasks arrive, but this can be overridden
 * dynamically using method {@link #prestartCoreThread} or {@link
 * #prestartAllCoreThreads}.  You probably want to prestart threads if
 * you construct the pool with a non-empty queue. </dd>
 */

/**
 * Core pool size is the minimum number of workers to keep alive
 * (and not allow to time out etc) unless allowCoreThreadTimeOut
 * is set, in which case the minimum is zero.
 */
private volatile int corePoolSize;

/**
 * Creates a new {@code ThreadPoolExecutor} with the given initial
 * parameters.
 *
 * @param corePoolSize the number of threads to keep in the pool, even
 *        if they are idle, unless {@code allowCoreThreadTimeOut} is set
 * ...
 */
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler) {
    // ...
    this.corePoolSize = corePoolSize;
    // ...
}
```

结合上述注释可知,  corePoolSize 字段表示的是线程池中一直存活着的线程的最小数量, 这些一直存活着的线程又被称为核心线程. 默认情况下, 核心线程的这个最小数量都是正数, 除非调用了allowCoreThreadTimeOut()方法并传递参数为true, 设置允许核心线程因超时而停止(terminated), 在那种情况下, 一旦所有的核心线程都先后因超时而停止了, 将使得线程池中的核心线程数量最终变为0, 也就是一直存活着的线程数为0, 这将是那种情况下, 线程池中核心线程数量的最小值. 默认情况下, 核心线程是按需创建并启动的, 也就是说, 只有当线程池接收到我们提交给他的任务后, 他才会去创建并启动一定数量的核心线程来执行这些任务. 如果他没有接收到相关任务, 他就不会主动去创建核心线程. 这种默认的核心线程的创建启动机制, 有助于降低系统资源的消耗. 变主动为被动, 类似于常见的观察者模式. 当然这只是系统默认的方式, 如果有特殊需求的话, 我们也可以通过调用 prestartCoreThread() 或 prestartAllCoreThreads() 方法来改变这一机制, 使得在新任务还未提交到线程池的时候, 线程池就已经创建并启动了一个或所有核心线程, 并让这些核心线程在池子里等待着新任务的到来.
- **maximumPoolSize**

将该类最前边对 Core and maximum pool sizes 的注释, 构造方法对该参数 maximumPoolSize 的注释, 以及该类中的同名字段 maximumPoolSize 的注释进行汇总如下:

```java
/**
 * <dt>Core and maximum pool sizes</dt>
 *
 * <dd>A {@code ThreadPoolExecutor} will automatically adjust the
 * pool size (see {@link #getPoolSize})
 * according to the bounds set by
 * corePoolSize (see {@link #getCorePoolSize}) and
 * maximumPoolSize (see {@link #getMaximumPoolSize}).
 *
 * When a new task is submitted in method {@link #execute(Runnable)},
 * and fewer than corePoolSize threads are running, a new thread is
 * created to handle the request, even if other worker threads are
 * idle.  If there are more than corePoolSize but less than
 * maximumPoolSize threads running, a new thread will be created only
 * if the queue is full.  By setting corePoolSize and maximumPoolSize
 * the same, you create a fixed-size thread pool. By setting
 * maximumPoolSize to an essentially unbounded value such as {@code
 * Integer.MAX_VALUE}, you allow the pool to accommodate an arbitrary
 * number of concurrent tasks. Most typically, core and maximum pool
 * sizes are set only upon construction, but they may also be changed
 * dynamically using {@link #setCorePoolSize} and {@link
 * #setMaximumPoolSize}. </dd>
 */

/**
 * Maximum pool size. Note that the actual maximum is internally
 * bounded by CAPACITY.
 */
private volatile int maximumPoolSize;

/**
 * Creates a new {@code ThreadPoolExecutor} with the given initial
 * parameters.
 *
 * @param maximumPoolSize the maximum number of threads to allow in the
 *        pool
 */
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler) {
    // ...
    this.maximumPoolSize = maximumPoolSize;
    // ...
}
```

maximumPoolSize 表示线程池内能够容纳线程数量的最大值. 当然, 线程数量的最大值还不能超过常量 CAPACITY 的数值大小, 根据如下的源码, 我们很容易计算出 CAPACITY 的数值等于 (1 << 29 - 1), 也就是 1先左移29位然后再减1以后的数值, 在介绍 ctl 的时候曾经提到过, 这个数值约等于5亿.

```java
private static final int COUNT_BITS = Integer.SIZE - 3;
private static final int CAPACITY   = (1 << COUNT_BITS) - 1;
```

所以, 我们设定的参数 maximumPoolSize 和 CAPACITY 二者中较小的那个数值才是线程池中线程数量的最大值.  这里多说一句, **如果我们提供的阻塞队列 (也就是参数 workQueue) 是一个无界的队列, 那么这里提供的 maximumPoolSize 的数值将毫无意义**, 这一点我们会在后面解释. 当我们通过方法 execute(Runnable) 提交一个任务到线程池时, 如果处于运行状态(RUNNING)的线程数量少于核心线程数(corePoolSize), 那么即使有一些非核心线程处于空闲等待状态, 系统也会倾向于创建一个新的线程来处理这个任务. 如果此时处于运行状态(RUNNING)的线程数量大于核心线程数(corePoolSize), 但又小于最大线程数(maximumPoolSize), 那么系统将会去判断线程池内部的阻塞队列 workQueue 中是否还有空位子. 如果发现有空位子, 系统就会将该任务先存入该阻塞队列; 如果发现队列中已没有空位子(即: 队列已满), 系统就会新创建一个线程来执行该任务. 

如果将线程池的核心线程数 corePoolSize 和 最大线程数 maximumPoolSize 设置为相同的数值(也就是说, 线程池中的所有线程都是核心线程), 那么该线程池就是一个容量固定的线程池. 如果将最大线程数 maximumPoolSize 设置为一个非常大的数值(例如: Integer.MAX_VALUE), 那么就相当于允许线程池自己在不同时段去动态调整参与并发的任务总数. 通常情况下, 核心线程数 corePoolSize 和 最大线程数 maximumPoolSize 仅在创建线程池的时候才去进行设定, 但是, 如果在线程池创建完成以后, 你又想去修改这两个字段的值, 你就可以调用 setCorePoolSize() 和 setMaximumPoolSize() 方法来分别重新设定核心线程数 corePoolSize 和 最大线程数 maximumPoolSize 的数值.
- **keepAliveTime**

将该类最前面关于 Keep-alive times 的注释, 构造方法对参数keepAliveTime 的注释, 以及字段 keepAliveTime 的注释汇总如下:

```java
/**
 * <dt>Keep-alive times</dt>
 *
 * <dd>If the pool currently has more than corePoolSize threads,
 * excess threads will be terminated if they have been idle for more
 * than the keepAliveTime (see {@link #getKeepAliveTime(TimeUnit)}).
 * This provides a means of reducing resource consumption when the
 * pool is not being actively used. If the pool becomes more active
 * later, new threads will be constructed. This parameter can also be
 * changed dynamically using method {@link #setKeepAliveTime(long,
 * TimeUnit)}.  Using a value of {@code Long.MAX_VALUE} {@link
 * TimeUnit#NANOSECONDS} effectively disables idle threads from ever
 * terminating prior to shut down. By default, the keep-alive policy
 * applies only when there are more than corePoolSize threads. But
 * method {@link #allowCoreThreadTimeOut(boolean)} can be used to
 * apply this time-out policy to core threads as well, so long as the
 * keepAliveTime value is non-zero. </dd>
 */

/**
 * Timeout in nanoseconds for idle threads waiting for work.
 * Threads use this timeout when there are more than corePoolSize
 * present or if allowCoreThreadTimeOut. Otherwise they wait
 * forever for new work.
 */
private volatile long keepAliveTime;

/**
 * Creates a new {@code ThreadPoolExecutor} with the given initial
 * parameters.
 *
 * @param keepAliveTime when the number of threads is greater than
 *        the core, this is the maximum time that excess idle threads
 *        will wait for new tasks before terminating.
 */
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler) {
    // ...
    this.keepAliveTime = unit.toNanos(keepAliveTime);
    // ...
}
```

从注释可知, keepAliveTime 表示空闲线程处于等待状态的超时时间(也即, 等待时间的上限值, 超过该时间后该线程会停止工作). 当总线程数大于 corePoolSize (核心线程数) 并且 allowCoreThreadTimeOut 为 false 时, 这些多出来的非核心线程一旦进入到空闲等待状态, 就开始计算各自的等待时间, 并用这里设定的 keepAliveTime 的数值作为他们的超时时间, 一旦某个非核心线程的等待时间达到了超时时间, 该线程就会停止工作(terminated), 而核心线程在这种情况下却不会受超时机制的制约, 核心线程即使等待的时间超出了这里设定的 keepAliveTime, 也依然可以继续处于空闲等待状态而不会停止工作. 但是, 如果 allowCoreThreadTimeOut 被设置为 true 并且设置的 keepAliveTime > 0, 那么不论是非核心线程还是核心线程, 都将受超时机制的制约.  所以, **如果要执行的任务相对较多，并且每个任务执行的时间比较短，那么可以为该参数设置一个相对较大的数值，以提高线程的利用率。如果执行的任务相对较少, 线程池使用率相对较低, 那么可以先将该参数设置为一个较小的数值, 通过超时停止的机制来降低系统线程资源的开销, 后续如果发现线程池的使用率逐渐增高以后, 线程池会根据当前提交的任务数自动创建新的线程, 当然, 我们也可以自己手动调用 setKeepAliveTime(long, TimeUnit)方法来重新设定 keepAliveTime  字段的值.** 如果将 keepAliveTime 和 unit 这两个参数分别设置为 Long.MAX_VALUE 和 TimeUnit.NANOSECONDS(纳秒), 这可以让空闲线程基本上一直处于存活状态. 因为这两个数值的组合表示设置超时时间约为292年, 这是个非常非常长的时间了, 所以我们可以认为这种设置基本等效于让空闲线程一直处于存活状态. 另外值得注意的是, 构造方法中的参数 keepAliveTime 的数值和字段 keepAliveTime 的数值很可能不同. 因为参数 keepAliveTime 对应的时间单位可以是任意的, 这取决于另一个参数 unit 赋的是什么值, 可选的值有纳秒(NANOSECONDS), 微秒(MICROSECONDS), 毫秒(MILLISECONDS), 秒(SECONDS), 分钟(MINUTES), 小时(HOURS), 天(DAYS). 而与其同名的字段 keepAliveTime 对应的时间单位则被强制要求是纳秒. 所以, 构造方法会将参数 keepAliveTime 和 unit 二者组合起来的时间值换算成以纳秒为单位的数值, 并将换算后得到的数值作为字段 keepAliveTime 的值. 注意这里的参数 keepAliveTime 是 long 型而不是 int 型的, 因为如果 keepAliveTime  为 int 型, 并且为 unit 赋的值是 NANOSECONDS(纳秒), 那么即使取 int 型的最大值 Integer.MAX_VALUE 作为 keepAliveTime  的数值, 设置的超时时间就是 0x7FFFFFFF 纳秒, 换算成秒就是2秒, 即: 这种情况下能够设置的最大超时时间是2秒, 时间太短, 所以将参数 keepAliveTime 定义为 long 型就保证了能够设置的超时时间不至于太短.
- **workQueue**

从源码可知, 这里的 workQueue 是一个 BlockingQueue(阻塞队列) 的实例, 传入的泛型类型是 Runnable. 也就是说, workQueue 是一个内部元素为 Runnable(各种任务, 通常是异步的任务) 的阻塞队列. 阻塞队列是一种类似于 “生产者 - 消费者”模型的队列. 当队列已满时如果继续向队列中插入元素, 该插入操作将被阻塞一直处于等待状态, 直到队列中有元素被移除产生空位子后, 才有可能执行这次插入操作; 当队列为空时如果继续执行元素的删除或获取操作, 该操作同样会被阻塞而进入等待状态, 直到队列中又有了该元素后, 才有可能执行该操作.

下面将该类最前面的注释中关于 Queuing 的那部分注释,  字段 workQueue 的注释, 以及构造方法对参数 workQueue 的注释三者汇总如下:

```java
/**
 * <dt>Queuing</dt>
 *
 * Any {@link BlockingQueue} may be used to transfer and hold
 * submitted tasks.  The use of this queue interacts with pool sizing:
 *
 * If fewer than corePoolSize threads are running, the Executor
 * always prefers adding a new thread
 * rather than queuing.
 *
 * If corePoolSize or more threads are running, the Executor
 * always prefers queuing a request rather than adding a new
 * thread.
 *
 * If a request cannot be queued, a new thread is created unless
 * this would exceed maximumPoolSize, in which case, the task will be
 * rejected.
 *
 *
 * There are three general strategies for queuing:
 *
 * <em> Direct handoffs.</em> A good default choice for a work
 * queue is a {@link SynchronousQueue} that hands off tasks to threads
 * without otherwise holding them. Here, an attempt to queue a task
 * will fail if no threads are immediately available to run it, so a
 * new thread will be constructed. This policy avoids lockups when
 * handling sets of requests that might have internal dependencies.
 * Direct handoffs generally require unbounded maximumPoolSizes to
 * avoid rejection of new submitted tasks. This in turn admits the
 * possibility of unbounded thread growth when commands continue to
 * arrive on average faster than they can be processed.
 *
 * <em> Unbounded queues.</em> Using an unbounded queue (for
 * example a {@link LinkedBlockingQueue} without a predefined
 * capacity) will cause new tasks to wait in the queue when all
 * corePoolSize threads are busy. Thus, no more than corePoolSize
 * threads will ever be created. (And the value of the maximumPoolSize
 * therefore doesn't have any effect.)  This may be appropriate when
 * each task is completely independent of others, so tasks cannot
 * affect each others execution; for example, in a web page server.
 * While this style of queuing can be useful in smoothing out
 * transient bursts of requests, it admits the possibility of
 * unbounded work queue growth when commands continue to arrive on
 * average faster than they can be processed.
 *
 * <em>Bounded queues.</em> A bounded queue (for example, an
 * {@link ArrayBlockingQueue}) helps prevent resource exhaustion when
 * used with finite maximumPoolSizes, but can be more difficult to
 * tune and control.  Queue sizes and maximum pool sizes may be traded
 * off for each other: Using large queues and small pools minimizes
 * CPU usage, OS resources, and context-switching overhead, but can
 * lead to artificially low throughput.  If tasks frequently block (for
 * example if they are I/O bound), a system may be able to schedule
 * time for more threads than you otherwise allow. Use of small queues
 * generally requires larger pool sizes, which keeps CPUs busier but
 * may encounter unacceptable scheduling overhead, which also
 * decreases throughput.
 */

/**
 * The queue used for holding tasks and handing off to worker
 * threads.  We do not require that workQueue.poll() returning
 * null necessarily means that workQueue.isEmpty(), so rely
 * solely on isEmpty to see if the queue is empty (which we must
 * do for example when deciding whether to transition from
 * SHUTDOWN to TIDYING).  This accommodates special-purpose
 * queues such as DelayQueues for which poll() is allowed to
 * return null even if it may later return non-null when delays
 * expire.
 */
private final BlockingQueue<Runnable> workQueue;

/**
 * Creates a new {@code ThreadPoolExecutor} with the given initial
 * parameters.
 *
 * @param workQueue the queue to use for holding tasks before they are
 *        executed.  This queue will hold only the {@code Runnable}
 *        tasks submitted by the {@code execute} method.
 */
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler) {
    // ...
    this.workQueue = workQueue;
    // ...
}
```

workQueue 是一个用于保存等待执行的任务的阻塞队列. 当提交一个新的任务到线程池以后, 线程池会根据当前池子中正在运行着的线程的数量, 指定出对该任务相应的处理方式, 主要有以下几种处理方式: 

(1) 如果线程池中正在运行的线程数少于核心线程数, 那么线程池总是倾向于创建一个新线程来执行该任务, 而不是将该任务提交到该队列 workQueue 中进行等待.  

(2) 如果线程池中正在运行的线程数不少于核心线程数, 那么线程池总是倾向于将该任务先提交到队列 workQueue 中先让其等待, 而不是创建一个新线程来执行该任务. 

(3) 如果线程池中正在运行的线程数不少于核心线程数, 并且线程池中的阻塞队列也满了使得该任务入队失败, 那么线程池会去判断当前池子中运行的线程数是否已经等于了该线程池允许运行的最大线程数 maximumPoolSize. 如果发现已经等于了, 说明池子已满, 无法再继续创建新的线程了, 那么就会拒绝执行该任务. 如果发现运行的线程数小于池子允许的最大线程数, 那么就会创建一个线程(这里创建的线程是非核心线程)来执行该任务.

一般来说, 队列对新提交的任务有三种常见的处理策略: 
**(1) 直接切换**. 常用的队列是 SynchronousQueue (同步队列). 这种队列内部不会存储元素. 每一次插入操作都会先进入阻塞状态, 一直等到另一个线程执行了删除操作, 然后该插入操作才会执行. 同样地, 每一次删除操作也都会先进入阻塞状态, 一直等到另一个线程执行了插入操作, 然后该删除操作才会执行. 当提交一个任务到包含这种 SynchronousQueue 队列的线程池以后, 线程池会去检测是否有可用的空闲线程来执行该任务, 如果没有就直接新建一个线程来执行该任务而不是将该任务先暂存在队列中. “直接切换”的意思就是, 处理方式由”将任务暂时存入队列”直接切换为”新建一个线程来处理该任务”. 这种策略适合用来处理多个有相互依赖关系的任务, 因为该策略可以避免这些任务因一个没有及时处理而导致依赖于该任务的其他任务也不能及时处理而造成的锁定效果. 因为这种策略的目的是要让几乎每一个新提交的任务都能得到立即处理, 所以这种策略通常要求最大线程数 maximumPoolSizes 是无界的(即: Integer.MAX_VALUE). 静态工厂方法 Executors.newCachedThreadPool() 使用了这个队列。 
**(2) 使用无界队列** (也就是不预设队列的容量, 队列将使用 Integer.MAX_VALUE 作为其默认容量, 例如: 基于链表的阻塞队列 LinkedBlockingQueue). 使用无界队列将使得线程池中能够创建的最大线程数就等于核心线程数 corePoolSize, 这样线程池的 maximumPoolSize 的数值起不到任何作用. 如果向这种线程池中提交一个新任务时发现所有核心线程都处于运行状态, 那么该任务将被放入无界队列中等待处理. 当要处理的多个任务之间没有任何相互依赖关系时, 就适合使用这种队列策略来处理这些任务. 静态工厂方法 Executors.newFixedThreadPool() 使用了这个队列。 
**(3) 使用有界队列** (例如: 基于数组的阻塞队列 ArrayBlockingQueue). 当要求线程池的最大线程数 maximumPoolSizes 要限定在某个值以内时, 线程池使用有界队列能够降低资源的消耗, 但这也使得线程池对线程的调控变得更加困难. 因为队列容量和线程池容量都是有限的值, 要想使线程处理任务的吞吐量能够在一个相对合理的范围内, 同时又能使线程调配的难度相对较低, 并且又尽可能节省系统资源的消耗, 那么就需要合理地调配这两个数值. 通常来说, 设置较大的队列容量和较小的线程池容量, 能够降低系统资源的消耗(包括CPU的使用率, 操作系统资源的消耗, 上下文环境切换的开销等), 但却会降低线程处理任务的吞吐量. 如果发现提交的任务经常频繁地发生阻塞的情况, 那么你就可以考虑增大线程池的容量, 可以通过调用 setMaximumPoolSize() 方法来重新设定线程池的容量. 而设置较小的队列容量时, 通常需要将线程池的容量设置大一点,  这种情况下, CPU的使用率会相对较高, 当然如果线程池的容量设置过大的话, 可能会有非常非常多的线程来同时处理提交来的多个任务, 并发数过大时, 线程之间的调度将会是个非常严峻的问题, 这反而有可能降低任务处理的吞吐量, 出现过犹不及的局面.
- **threadFactory**

将该类前边关于该参数的那部分注释 “Creating new threads”,  构造方法对该参数的注释, 以及该类对字段  threadFactory 的注释汇总如下:

```java
/** <dt>Creating new threads</dt>
 *
 * <dd>New threads are created using a {@link ThreadFactory}.  If not
 * otherwise specified, a {@link Executors#defaultThreadFactory} is
 * used, that creates threads to all be in the same {@link
 * ThreadGroup} and with the same {@code NORM_PRIORITY} priority and
 * non-daemon status. By supplying a different ThreadFactory, you can
 * alter the thread's name, thread group, priority, daemon status,
 * etc. If a {@code ThreadFactory} fails to create a thread when asked
 * by returning null from {@code newThread}, the executor will
 * continue, but might not be able to execute any tasks.</dd>
 */

/**
 * Factory for new threads. All threads are created using this
 * factory (via method addWorker).  All callers must be prepared
 * for addWorker to fail, which may reflect a system or user's
 * policy limiting the number of threads.  Even though it is not
 * treated as an error, failure to create threads may result in
 * new tasks being rejected or existing ones remaining stuck in
 * the queue.
 *
 * We go further and preserve pool invariants even in the face of
 * errors such as OutOfMemoryError, that might be thrown while
 * trying to create threads.  Such errors are rather common due to
 * the need to allocate a native stack in Thread.start, and users
 * will want to perform clean pool shutdown to clean up.  There
 * will likely be enough memory available for the cleanup code to
 * complete without encountering yet another OutOfMemoryError.
 */
private volatile ThreadFactory threadFactory;

/**
 * Creates a new {@code ThreadPoolExecutor} with the given initial
 * parameters.
 *
 * @param threadFactory the factory to use when the executor
 *        creates a new thread
 */
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler) {
    // ...
    this.threadFactory = threadFactory;
    // ...
}
```

threadFactory 线程工厂, 用于创建线程. 如果我们在创建线程池的时候未指定该 threadFactory 参数,  线程池则会使用 Executors.defaultThreadFactory() 方法创建默认的线程工厂. 如果我们想要为线程工厂创建的线程设置一些特殊的属性, 例如: 设置见名知意的名字, 设置特定的优先级等等, 那么我们就需要自己去实现 ThreadFactory 接口, 并在实现其抽象方法 newThread()的时候, 使用Thread类包含 threadName (线程名字)的那个构造方法就可以指定线程的名字(通常可以指定见名知意的名字), 还可以用 setPriority() 方法为线程设置特定的优先级等. 然后在创建线程池的时候, 将我们自己实现的 ThreadFactory 接口的实现类对象作为 threadFactory 参数的值传递给线程池的构造方法即可.
- **handler**

将该类前边关于该参数的那部分注释 “Rejected tasks”,  构造方法对该参数的注释, 以及该类对字段 handler 的注释汇总如下:

```java
/** 
 * <dt>Rejected tasks</dt>
 *
 * <dd>New tasks submitted in method {@link #execute(Runnable)} will be
 * <em>rejected</em> when the Executor has been shut down, and also when
 * the Executor uses finite bounds for both maximum threads and work queue
 * capacity, and is saturated.  In either case, the {@code execute} method
 * invokes the {@link
 * RejectedExecutionHandler#rejectedExecution(Runnable, ThreadPoolExecutor)}
 * method of its {@link RejectedExecutionHandler}.  Four predefined handler
 * policies are provided:
 *
 * <ol>
 *
 * <li> In the default {@link ThreadPoolExecutor.AbortPolicy}, the
 * handler throws a runtime {@link RejectedExecutionException} upon
 * rejection. </li>
 *
 * <li> In {@link ThreadPoolExecutor.CallerRunsPolicy}, the thread
 * that invokes {@code execute} itself runs the task. This provides a
 * simple feedback control mechanism that will slow down the rate that
 * new tasks are submitted. </li>
 *
 * <li> In {@link ThreadPoolExecutor.DiscardPolicy}, a task that
 * cannot be executed is simply dropped.  </li>
 *
 * <li>In {@link ThreadPoolExecutor.DiscardOldestPolicy}, if the
 * executor is not shut down, the task at the head of the work queue
 * is dropped, and then execution is retried (which can fail again,
 * causing this to be repeated.) </li>
 *
 * </ol>
 *
 * It is possible to define and use other kinds of {@link
 * RejectedExecutionHandler} classes. Doing so requires some care
 * especially when policies are designed to work only under particular
 * capacity or queuing policies. </dd>
 */

/**
 * Handler called when saturated or shutdown in execute.
 */
private volatile RejectedExecutionHandler handler;

/**
 * Creates a new {@code ThreadPoolExecutor} with the given initial
 * parameters.
 *
 * @param handler the handler to use when execution is blocked
 *        because the thread bounds and queue capacities are reached
 */
public ThreadPoolExecutor(int corePoolSize,
                          int maximumPoolSize,
                          long keepAliveTime,
                          TimeUnit unit,
                          BlockingQueue<Runnable> workQueue,
                          ThreadFactory threadFactory,
                          RejectedExecutionHandler handler) {
    // ...
    this.handler = handler;
    // ...
}
```

根据注释可知,  以下两个条件满足其中任意一个的时候, 如果继续向该线程池中提交新的任务, 那么线程池将会调用他内部的 RejectedExecutionHandler 对象(也就是 handler)的 rejectedExecution()方法, 表示拒绝执行这些新提交的任务: 

> 
① 当线程池处于 SHUTDOWN (关闭) 状态时 (不论线程池和阻塞队列是否都已满) 

  ② 当线程池中的所有线程都处于运行状态并且线程池中的阻塞队列已满时

可能只是用文字描述不太容易理解,  并且印象也不深刻, 下面我们写个简单的 demo来给大家展示一下这两种情况到底是什么意思:

```java
package com.example.thread_pool_executor_test;

import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * 线程池触发 RejectedExecutionHandler拒绝新提交任务的场景模拟 demo
 *
 * @author zhangzhiyi
 * @version 1.0
 * @createTime 2016/2/25 13:53
 * @projectName ThreadPoolExecutorTest
 */
public class ThreadPoolExecutorRejectNewTaskDemo {

    // 线程池的最大容量
    private static final int MAX_POOL_SIZE = 3;
    // 阻塞队列的容量
    private static final int QUEUE_CAPACITY = 2;
    // 非核心线程处于空闲状态的最长时间
    private static final int KEEP_ALIVE_TIME_VALUE = 1;
    // 线程池对象
    private static final ThreadPoolExecutor THREAD_POOL_EXECUTOR = new ThreadPoolExecutor(
            MAX_POOL_SIZE, MAX_POOL_SIZE,
            KEEP_ALIVE_TIME_VALUE, TimeUnit.SECONDS,
            new LinkedBlockingQueue<Runnable>(QUEUE_CAPACITY),
            new MyThreadFactory());

    public static void main(String[] args) {
        // 模拟线程池及其内部的队列都已满后, 继续向其提交新任务将会被拒绝的场景
//        threadPoolFullToRejectNewTask();

        // 模拟线程池被关闭(shutdown)后, 继续向其提交新任务将会被拒绝的场景
        shutdownThreadPoolToRejectNewTask();
    }

    /**
     * 模拟线程池被关闭(shutdown)后, 继续向其提交新任务将会被拒绝的场景
     */
    private static void shutdownThreadPoolToRejectNewTask() {
        MyRunnable r = new MyRunnable();

        int cycleCount = Math.max(MAX_POOL_SIZE - 1, 0);

        // 先提交(MAX_POOL_SIZE - 1)个任务. 显然, 线程池此时还未满
        for (int i = 0; i < cycleCount; i++) {
            System.out.println("提交任务" + i);
            THREAD_POOL_EXECUTOR.execute(r);
        }
        // 在线程池未满的情况下关闭线程池.
        THREAD_POOL_EXECUTOR.shutdown();

        // 在线程池已处于关闭(SHUTDOWN)的状态下
        if (THREAD_POOL_EXECUTOR.isShutdown()) {
            try {
                System.out.println("提交任务" + cycleCount);
                Thread.sleep(10);
                // 在线程池未满但却已经关闭了的情况下, 继续向该线程池中提交任务.
                THREAD_POOL_EXECUTOR.execute(r);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * 模拟线程池及其内部的队列都已满后, 继续向其提交新任务将会被拒绝的场景
     */
    private static void threadPoolFullToRejectNewTask() {
        MyRunnable r = new MyRunnable();
        // 循环提交任务的总次数. 该总次数等于"线程池的最大线程容量和阻塞队列的容量之和", 在执行完
        // 该循环后, 线程池和阻塞队列都已满.
        int cycleCount = MAX_POOL_SIZE + QUEUE_CAPACITY;

        for (int i = 0; i < cycleCount; i++) {
            System.out.println("提交任务" + i);
            THREAD_POOL_EXECUTOR.execute(r);
        }
        // 当前已提交的任务数
        int tasksCount = cycleCount;

        // 在线程池和阻塞队列都已满的情况下, 继续提交任务.
        try {
            System.out.println("提交任务" + (tasksCount));
            Thread.sleep(10);
            THREAD_POOL_EXECUTOR.execute(r);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * 自定义的线程工厂类, 用于为线程池创建线程对象.
     */
    private static class MyThreadFactory implements ThreadFactory {
        static int threadNumber = 0;

        @Override
        public Thread newThread(Runnable r) {
            String threadName = "thread-" + (threadNumber++);
            System.out.println("创建线程 " + threadName);
            return new Thread(r, threadName);
        }
    }

    /**
     * 表示向线程池提交的任务的 Runnable实现类
     */
    private static class MyRunnable implements Runnable {
        @Override
        public void run() {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
```

在上边的demo中, 我们定义了一个最大线程数为3, 阻塞队列容量为2的线程池, 并且我们使用的是不带 RejectedExecutionHandler 参数的那个构造方法. 我们将上述两个条件结合到我们这个demo中来说就是: 

① 当线程池的 isShutdown() 方法返回 true 的时候 (即使此时线程池中处于运行状态的线程数少于3个, 或者阻塞队列中的元素个数少于2个, 也会拒绝新提交的任务) 

② 当线程池中处于运行状态的线程数为3个, 并且阻塞队列中包含的元素个数等于2个的时候, 继续提交的任务就会被拒绝.

我们定义了两个方法 shutdownThreadPoolToRejectNewTask() 和 threadPoolFullToRejectNewTask(), 来模拟在分别满足上边两个条件的情况下, 向线程池中继续提交任务的情景.

在方法 shutdownThreadPoolToRejectNewTask() 中, 我们先提交了 (MAX_POOL_SIZE - 1) 个任务, 也就是2个任务, 此时线程池还未满, 阻塞队列还是空的, 我们调用线程池的 shutdown() 方法 (也可调用 shutdownNow()方法) 将线程池关闭,  并且为了确保线程池已经关闭, 我们又调用了线程池的 isShutdown()方法来判断线程池是否已经关闭了, 在该方法返回 true的时候, 也就是说, 在确认线程池已经处于关闭(SHUTDOWN) 状态时, 我们向该线程池中又提交了一个任务 (见代码第61行), 也就是第3个任务, 我们来看看此时会发生什么情况呢? 下面是执行过程所打印的 log信息: 
![这里写图片描述](https://img-blog.csdn.net/20160226150139193)

从log信息可以看出, 当我们提交第3个任务 (即: 任务2) 的时候, 线程池直接抛出了异常, 并且导致异常发生的代码就是61行的 THREAD_POOL_EXECUTOR.execute(r); 这句代码. 所以, 线程池拒绝接受新任务的处理方式就是直接抛异常 (其实, 在看完后边的介绍后, 你就会知道, 抛异常只是其中的一种处理方式, 也是线程池默认的处理方式, 线程池还为我们提供了其他几种处理方式, 当然我们自己也可以提供自定义的处理方式). 

我们再来看看另一个方法 threadPoolFullToRejectNewTask() 所模拟的场景. 在该方法中, 我们一共提交了 (MAX_POOL_SIZE + QUEUE_CAPACITY + 1) 个任务, 也就是6个任务,  而线程池的最大线程数和核心线程数都为3, 所以在提交前3个任务 (即: 任务0, 任务1, 任务2) 后, 线程池会分别创建3个核心线程 (此时线程池已满)来处理这3个任务, 接下来我们再提交的任务就会被暂时存入阻塞队列中, 而我们为阻塞队列设定的容量为2, 所以我们接下来提交的第4个和第5个任务 (即: 任务3和任务4), 都将存入阻塞队列中 (此时阻塞队列也满了), 这时已经满足了前边提到的条件②, 而我们一共要提交6个任务, 所以当我们继续提交第6个任务 (也就是任务5, 见代码第88行) 时, 同样也会被拒绝, 而拒绝的方式也是抛出异常, 见如下的 log 信息:  
![这里写图片描述](https://img-blog.csdn.net/20160226150154020)

从 log 也可以看出, 正是第88行提交的第6个任务, 导致了异常的抛出. 当然, 抛异常也同样是这种情况下线程池默认的处理方式, 我们也可以改为使用线程池提供的其他处理方式, 或者我们自己提供自定义的处理方式.

在上面的demo例子中, 我们对拒绝新任务的处理方式有了一个较为直观的认识, 我们知道了默认情况下线程池是使用抛异常的方式来拒绝新提交的任务的,  这种抛异常的方式在线程池中被称为 AbortPolicy. 当然, 除了这种 AbortPolicy 方式外, 线程池还为我们提供了 CallerRunsPolicy, DiscardPolicy和 DiscardOldestPolicy 的方式, 下面我们就来分别简要介绍下这几种方式:

**①  AbortPolicy**: 

> 
前面已经介绍过, 这是一种直接抛异常的处理方式, 抛出 RejectedExecutionException 异常.  如果在 ThreadPoolExecutor 的构造方法中未指定 RejectedExecutionHandler 参数, 那么线程池将使用他内部预定义的 defaultHandler 这个字段作为该参数的值, 而这个 defaultHandler 就是采用的 AbortPolicy 抛异常的方式 . 这也就解释了为什么在前边的demo例子中, 线程池在满足前边提到的两个条件中的任意一个时, 都会采取抛异常的方式来拒绝新提交的任务. 另外, 在Android开发中, 我们常用的 AsyncTask 类中也有个已定义好的线程池对象 THREAD_POOL_EXECUTOR, 而这个线程池同样采用的是 AbortPolicy 抛异常的方式来拒绝新任务,  抛异常会导致我们的 Android APP 直接 crash 掉,  这是一种非常糟糕的用户体验. 所以我们在Android开发中要使用 AsyncTask 结合线程池来并发处理异步任务, 如果并发执行的任务数较多的话, 建议不要直接使用 AsyncTask 内部自带的那个线程池, 而应该自定义一个线程池对象, 并为 RejectedExecutionHandler 参数赋予一个能够提供更友好处理方式的实现类对象.

**② CallerRunsPolicy**:

> 
将新提交的任务放在 ThreadPoolExecutor.execute()方法所在的那个线程中执行.

**③ DiscardPolicy**:

> 
直接不执行新提交的任务.

**④ DiscardOldestPolicy**: 

为避免片面理解, 我们有必要看下他的源码: 

```java
/**
 * A handler for rejected tasks that discards the oldest unhandled
 * request and then retries {@code execute}, unless the executor
 * is shut down, in which case the task is discarded.
 */
public static class DiscardOldestPolicy implements RejectedExecutionHandler {
    /**
     * Creates a {@code DiscardOldestPolicy} for the given executor.
     */
    public DiscardOldestPolicy() { }

    /**
     * Obtains and ignores the next task that the executor
     * would otherwise execute, if one is immediately available,
     * and then retries execution of task r, unless the executor
     * is shut down, in which case task r is instead discarded.
     *
     * @param r the runnable task requested to be executed
     * @param e the executor attempting to execute this task
     */
    public void rejectedExecution(Runnable r, ThreadPoolExecutor e) {
        if (!e.isShutdown()) {
            e.getQueue().poll();
            e.execute(r);
        }
    }
}
```

> 
结合注释和源码可知, 这种处理方式分为两种情况: 

  ① 当线程池已经关闭 (SHUTDOWN) 时,  就不执行这个任务了, 这也是 DiscardPolicy 的处理方式. 

  ② 当线程池未关闭时, 会将阻塞队列中处于队首 (head) 的那个任务从队列中移除, 然后再将这个新提交的任务加入到该阻塞队列的队尾 (tail) 等待执行.

以上就是对这四种处理方式的简要介绍.  但是, 你可千万不要认为就只能在这四种方式中挑选一种, 因为这四种方式只是线程池预定义的四种常用方式, 而 RejectedExecutionHandler 这个参数其实是个接口, 线程池所提供的这四种方式其实都是该接口的实现类, 所以, 只要我们自定义一个类来实现该接口, 并在重写该接口的 rejectedExecution() 方法时提供我们自己的处理逻辑,  那么我们就可以将我们自定义的这个类的对象作为参数传递给线程池的构造方法. 当线程池满足拒绝新任务的条件时, 如果我们继续向其提交新任务, 那么线程池就会采用我们自己提供的那套逻辑来处理这些新提交的任务了.

## 3. 线程池进行任务调度的原理

我们只能向线程池提交任务, 而被提交的任务最终能否执行以及能否立即执行, 则都由线程池自己来控制, 至于怎么控制就涉及到线程池对任务调度的原理了. 我们要向线程池提交一个任务, 可以通过调用 execute() 或 submit()方法来实现, 而二者的区别是, execute()方法只能进行任务的提交而不能获取该任务执行的结果, 但 submit()方法则既能进行任务的提交, 又能获取该任务执行的结果. 所以, 如果你需要获取一个任务执行的结果或者需要对一个任务执行的结果进行某种操作, 那么就需要使用 submit()方法来提交任务. 其实 submit()方法就是对 execute()方法的一种封装, 它内部也是调用 execute()方法来实现任务提交的, 只是因为 submit()方法的返回值是一个 Future 对象, 通过返回的 Future对象就能获取该任务最终执行的结果. 由于我们这里介绍的主题是线程池对任务调度的原理, 而任务调度, 用较为通俗的话来说, 就是一个任务被提交到线程池后能否被执行, 如果能被执行, 那么是立即执行, 还是在未来的某个时刻去执行, 用哪个线程执行; 如果不能被执行, 那么又将怎么处理这个任务等等. 而这些既与前面介绍过的线程池中那[几个重要的参数](#important_params)的设置有关, 还与任务被提交的时刻有关(准确来说, 就是与该任务被提交时, 线程池内已有任务的情况有关).  所以, 我们有必要从线程的提交开始分析, 由于 submit()方法内部也是调用 execute()方法, 所以我们就直接分析 execute()方法, 其源码如下: 

```java
/**
 * Executes the given task sometime in the future.  The task
 * may execute in a new thread or in an existing pooled thread.
 *
 * If the task cannot be submitted for execution, either because this
 * executor has been shutdown or because its capacity has been reached,
 * the task is handled by the current {@code RejectedExecutionHandler}.
 *
 * @param command the task to execute
 * @throws RejectedExecutionException at discretion of
 *         {@code RejectedExecutionHandler}, if the task
 *         cannot be accepted for execution
 * @throws NullPointerException if {@code command} is null
 */
public void execute(Runnable command) {
    if (command == null)
        throw new NullPointerException();
    /*
     * Proceed in 3 steps:
     *
     * 1. If fewer than corePoolSize threads are running, try to
     * start a new thread with the given command as its first
     * task.  The call to addWorker atomically checks runState and
     * workerCount, and so prevents false alarms that would add
     * threads when it shouldn't, by returning false.
     *
     * 2. If a task can be successfully queued, then we still need
     * to double-check whether we should have added a thread
     * (because existing ones died since last checking) or that
     * the pool shut down since entry into this method. So we
     * recheck state and if necessary roll back the enqueuing if
     * stopped, or start a new thread if there are none.
     *
     * 3. If we cannot queue task, then we try to add a new
     * thread.  If it fails, we know we are shut down or saturated
     * and so reject the task.
     */

    // 获取ctl的值.
    int c = ctl.get();

    /********************************* 情况1 ************************************/

    // 根据ctl的值, 获取线程池中的有效线程数 workerCount, 如果 workerCount
    // 小于核心线程数 corePoolSize
    if (workerCountOf(c) < corePoolSize) {

        // 调用addWorker()方法, 将核心线程数corePoolSize设置为线程池中线程
        // 数的上限值, 将此次提交的任务command作为参数传递进去, 然后再次获取
        // 线程池中的有效线程数 workerCount, 如果 workerCount依然小于核心
        // 线程数 corePoolSize, 就创建并启动一个线程, 然后返回 true结束整个
        // execute()方法. 如果此时的线程池已经关闭, 或者此时再次获取到的有
        // 效线程数 workerCount已经 >= 核心线程数 corePoolSize, 就再继续执
        // 行后边的内容. 
        if (addWorker(command, true))
            return;

        // 再次获取 ctl的值
        c = ctl.get();
    }

    /***** 分析1 ****/
    // 如果情况1的判断条件不满足, 则直接进入情况2. 如果情况1的判断条件满足, 
    // 但情况1中的 addWorker()方法返回 false, 也同样会进入情况2.  
    // 总之, 进入情况2时, 线程池要么已经不处于RUNNING(运行)状态, 要么仍处于RUNNING
    // (运行)状态但线程池内的有效线程数 workerCount >= 核心线程数 corePoolSize

    /********************************* 情况2 ************************************/

    /***** 分析2 ****/
    // 经过上一段分析可知, 进入这个情况时, 线程池要么已经不处于RUNNING(运行)
    // 状态, 要么仍处于RUNNING(运行)状态但线程池内的有效线程数 workerCount
    // 已经 >= 核心线程数 corePoolSize

    // 如果线程池未处于RUNNING(运行)状态, 或者虽然处于RUNNING(运行)状态但线程池
    // 内的阻塞队列 workQueue已满, 则跳过此情况直接进入情况3.
    // 如果线程池处于RUNNING(运行)状态并且线程池内的阻塞队列 workQueue未满, 
    // 则将提交的任务 command 添加到阻塞队列 workQueue中.
    if (isRunning(c) && workQueue.offer(command)) {
        // 再次获取 ctl的值.
        int recheck = ctl.get();

        // 再次判断线程池此时的运行状态. 如果发现线程池未处于 RUNNING(运行)
        // 状态, 由于先前已将任务 command加入到阻塞队列 workQueue中了, 所以需
        // 要将该任务从 workQueue中移除. 一般来说, 该移除操作都能顺利进行. 
        // 所以一旦移除成功, 就再调用 handler的 rejectedExecution()方法, 根据
        // 该 handler定义的拒绝策略, 对该任务进行处理. 当然, 默认的拒绝策略是
        // AbortPolicy, 也就是直接抛出 RejectedExecutionException 异常, 同时也
        // 结束了整个 execute()方法的执行.
        if (! isRunning(recheck) && remove(command))
            reject(command);

        // 再次计算线程池内的有效线程数 workerCount, 一旦发现该数量变为0, 
        // 就将线程池内的线程数上限值设置为最大线程数 maximumPoolSize, 然后
        // 只是创建一个线程而不去启动它, 并结束整个 execute()方法的执行.
        else if (workerCountOf(recheck) == 0)
            addWorker(null, false);

        // 如果线程池处于 RUNNING(运行)状态并且线程池内的有效线程数大于0, 那么就直接结束该 
        // execute()方法, 被添加到阻塞队列中的该任务将会在未来的某个时刻被执行.
    }

    /********************************* 情况3 ************************************/

    /***** 分析3 ****/
    // 如果该方法能够执行到这里, 那么结合分析1和分析2可知, 线程池此时必定是
    // 下面两种情况中的一种:
    // ① 已经不处于RUNNING(运行)状态
    // ② 处于RUNNING(运行)状态, 并且线程池内的有效线程数 workerCount已经
    //   >= 核心线程数 corePoolSize, 并且线程池内的阻塞队列 workQueue已满

    // 再次执行addWorker() 方法, 将线程池内的线程数上限值设置为最大线程数 
    // maximumPoolSize, 并将提交的任务 command作为被执行的对象, 尝试创建并
    // 启动一个线程来执行该任务. 如果此时线程池的状态为如下两种中的一种, 
    // 就会触发 handler的 rejectedExecution()方法来拒绝该任务的执行:
    // ① 未处于RUNNING(运行)状态.
    // ② 处于RUNNING(运行)状态, 但线程池内的有效线程数已达到本次设定的最大
    // 线程数 (另外根据分析3可知, 此时线程池内的阻塞队列 workQueue已满).
    //
    // 如果线程池处于 RUNNING(运行)状态, 但有效线程数还未达到本次设定的最大
    // 线程数, 那么就会尝试创建并启动一个线程来执行任务 command. 如果线程的
    // 创建和启动都很顺利, 那么就直接结束掉该 execute()方法; 如果线程的创建或
    // 启动失败, 则同样会触发 handler的 rejectedExecution()方法来拒绝该
    // 任务的执行并结束掉该 execute()方法.
    else if (!addWorker(command, false))
        reject(command);
}
```

代码中的注释非常详细,  这里再简要概括一下. execute()方法主要分为以下四种情况: 
**情况1:** 如果线程池内的有效线程数少于核心线程数 corePoolSize, 那么就创建并启动一个线程来执行新提交的任务. 
**情况2:** 如果线程池内的有效线程数达到了核心线程数 corePoolSize, 并且线程池内的阻塞队列未满, 那么就将新提交的任务加入到该阻塞队列中. 
**情况3:** 如果线程池内的有效线程数达到了核心线程数 corePoolSize 但却小于最大线程数 maximumPoolSize, 并且线程池内的阻塞队列已满, 那么就创建并启动一个线程来执行新提交的任务. 
**情况4:** 如果线程池内的有效线程数达到了最大线程数 maximumPoolSize, 并且线程池内的阻塞队列已满, 那么就让 RejectedExecutionHandler 根据它的拒绝策略来处理该任务, 默认的处理方式是直接抛异常.

上述四种情况可以使用下面的流程图来描述 (这里就直接借用[方腾飞](http://www.infoq.com/cn/author/%E6%96%B9%E8%85%BE%E9%A3%9E)老师在 [聊聊并发（三）——JAVA线程池的分析和使用](http://www.infoq.com/cn/articles/java-threadPool) 这篇文章里的流程图. 若侵删): 
![这里写图片描述](https://img-blog.csdn.net/20160228204222307)

上述 execute()源码在三种情况下分别调用了 addWorker()方法, 并且三次传递的参数都不同 (见第 56, 99, 128行):

```java
addWorker(command, true);
addWorker(null, false);
addWorker(command, false)
```

所以, 要想彻底看懂 execute()方法的逻辑, 就必须要先大致了解 addWorker()方法的逻辑以及该方法分别在上述三组赋值情况下各自到底做了什么事情. 下面我们还是来分析一下 addWorker()方法的源码吧: 

```java
/**
 * Checks if a new worker can be added with respect to current
 * pool state and the given bound (either core or maximum). If so,
 * the worker count is adjusted accordingly, and, if possible, a
 * new worker is created and started, running firstTask as its
 * first task. This method returns false if the pool is stopped or
 * eligible to shut down. It also returns false if the thread
 * factory fails to create a thread when asked.  If the thread
 * creation fails, either due to the thread factory returning
 * null, or due to an exception (typically OutOfMemoryError in
 * Thread.start()), we roll back cleanly.
 *
 * @param firstTask the task the new thread should run first (or
 * null if none). Workers are created with an initial first task
 * (in method execute()) to bypass queuing when there are fewer
 * than corePoolSize threads (in which case we always start one),
 * or when the queue is full (in which case we must bypass queue).
 * Initially idle threads are usually created via
 * prestartCoreThread or to replace other dying workers.
 *
 * @param core if true use corePoolSize as bound, else
 * maximumPoolSize. (A boolean indicator is used here rather than a
 * value to ensure reads of fresh values after checking other pool
 * state).
 * @return true if successful
 */
private boolean addWorker(Runnable firstTask, boolean core) {

    // retry 是个无限循环. 当线程池处于 RUNNING (运行)状态时, 只有在线程池中
    // 的有效线程数被成功加一以后, 才会退出该循环而去执行后边的代码. 也就是说,
    // 当线程池在 RUNNING (运行)状态下退出该 retry 循环时, 线程池中的有效线程数
    // 一定少于此次设定的最大线程数(可能是 corePoolSize 或 maximumPoolSize).
    retry:
    for (;;) {
        int c = ctl.get();
        int rs = runStateOf(c);

        // 线程池满足如下条件中的任意一种时, 就会直接结束该方法, 并且返回 false
        // 表示没有创建新线程, 新提交的任务也没有被执行.
        // ① 处于 STOP, TYDING 或 TERMINATD 状态
        // ② 处于 SHUTDOWN 状态, 并且参数 firstTask != null
        // ③ 处于 SHUTDOWN 状态, 并且阻塞队列 workQueue为空

        // Check if queue empty only if necessary.
        if (rs >= SHUTDOWN &&
            ! (rs == SHUTDOWN &&
               firstTask == null &&
               ! workQueue.isEmpty()))
            return false;

        for (;;) {
            int wc = workerCountOf(c);

            // 如果线程池内的有效线程数大于或等于了理论上的最大容量 CAPACITY 或者实际
            // 设定的最大容量, 就返回 false直接结束该方法. 这样同样没有创建新线程, 
            // 新提交的任务也同样未被执行.
            // (core ? corePoolSize : maximumPoolSize) 表示如果 core为 true,
            // 那么实际设定的最大容量为 corePoolSize, 反之则为 maximumPoolSize.
            if (wc >= CAPACITY ||
                wc >= (core ? corePoolSize : maximumPoolSize))
                return false;

            // 有效线程数加一
            if (compareAndIncrementWorkerCount(c))
                break retry;
            c = ctl.get();  // Re-read ctl
            if (runStateOf(c) != rs)
                continue retry;
            // else CAS failed due to workerCount change; retry inner loop
        }
    }

    boolean workerStarted = false;
    boolean workerAdded = false;
    Worker w = null;
    try {
        // 根据参数 firstTask来创建 Worker对象 w.
        w = new Worker(firstTask);
        // 用 w创建线程对象 t.
        final Thread t = w.thread;
        if (t != null) {
            final ReentrantLock mainLock = this.mainLock;
            mainLock.lock();
            try {
                // Recheck while holding lock.
                // Back out on ThreadFactory failure or if
                // shut down before lock acquired.
                int rs = runStateOf(ctl.get());

                if (rs < SHUTDOWN ||
                    (rs == SHUTDOWN && firstTask == null)) {
                    if (t.isAlive()) // precheck that t is startable
                        throw new IllegalThreadStateException();
                    workers.add(w);
                    int s = workers.size();
                    if (s > largestPoolSize)
                        largestPoolSize = s;
                    workerAdded = true;
                }
            } finally {
                mainLock.unlock();
            }
            if (workerAdded) {

                // 启动线程 t. 由于 t指向 w.thread所引用的对象, 所以相当于启动的是 w.thread所引用的线程对象.
                // 而 w是 Runnable 的实现类, w.thread 是以 w作为 Runnable参数所创建的一个线程对象, 所以启动
                // w.thread所引用的线程对象, 也就是要执行 w 的 run()方法.            
                t.start();
                workerStarted = true;
            }
        }
    } finally {
        if (! workerStarted)
            addWorkerFailed(w);
    }
    return workerStarted;
}
```

这里我们只分析线程池处于 RUNNING(运行)状态时的情况, 并且只分析上述方法中核心的重要的代码, 至于其他情况以及其他行的代码, 就不分析了.  

先看第78和80行,  根据参数 firstTask来创建 Worker对象 w, 并用 w再创建线程对象 t, t 指向 w.thread所指向的线程对象. 既然这里提到了 Worker 类,  那么我们就来简要介绍一下这个类.  Worker是 ThreadPoolExecutor类的一个内部类, 同时也是 Runnable接口的实现类, 其源码如下: 

```java
/**
 * Class Worker mainly maintains interrupt control state for
 * threads running tasks, along with other minor bookkeeping.
 * This class opportunistically extends AbstractQueuedSynchronizer
 * to simplify acquiring and releasing a lock surrounding each
 * task execution.  This protects against interrupts that are
 * intended to wake up a worker thread waiting for a task from
 * instead interrupting a task being run.  We implement a simple
 * non-reentrant mutual exclusion lock rather than use
 * ReentrantLock because we do not want worker tasks to be able to
 * reacquire the lock when they invoke pool control methods like
 * setCorePoolSize.  Additionally, to suppress interrupts until
 * the thread actually starts running tasks, we initialize lock
 * state to a negative value, and clear it upon start (in
 * runWorker).
 */
private final class Worker
    extends AbstractQueuedSynchronizer
    implements Runnable
{
    // ...

    /** Thread this worker is running in.  Null if factory fails. */
    final Thread thread;
    /** Initial task to run.  Possibly null. */
    Runnable firstTask;

    /**
     * Creates with given first task and thread from ThreadFactory.
     * @param firstTask the first task (null if none)
     */
    Worker(Runnable firstTask) {
        setState(-1); // inhibit interrupts until runWorker
        this.firstTask = firstTask;
        this.thread = getThreadFactory().newThread(this);
    }

    /** Delegates main run loop to outer runWorker. */
    public void run() {
        runWorker(this);
    }

    // ...
}
```

这里我只贴出了 Worker类中和本次分析有关的代码, 其他代码就省略掉了. Worker类内部还包含一个线程对象 thread 和一个 Runnable对象 firstTask. 而这个线程对象的创建过程见35行, 将Worker对象自身作为这个线程对象 thread的 Runnable参数传递给 thread的构造方法. 那么我们就可以知道, 如果要运行该线程, 也就是执行 thread.start(), 那么实际上就是要执行 thread所在的 Worker类中的 run()方法, 而从第40行又可以知道, Worker类中的 run()方法又是调用 runWorker(this); 这个方法的, 并将 thread所在的 Worker对象作为这个 runWorker()方法的参数. 简单来说就是:

> 
** 启动一个 Worker对象中包含的线程 thread, 就相当于要执行 runWorker()方法, 并将该 Worker对象作为该方法的参数.**

我们对 Worker类的分析就到此为止吧, 我们再回到 addWorker()方法继续来分析. 我们看 addWorker() 方法的第108行, 启动了线程 t, 而从第80行我们可以知道, 线程 t 就是 w.thread 所指向的对象, 所以第108行就相当于启动了 w.thread 这个线程, 也就是启动了 Worker对象 w 中的线程对象 thread. 而我们在分析 Worker类时, 曾得出过这样的结论: **启动一个 Worker对象中包含的线程 thread, 就相当于要执行 runWorker()方法, 并将该 Worker对象作为该方法的参数.** (结论见[这里](#conclusion_Worker)) 所以, 这里要启动线程 w.thread, 也就相当于要执行 runWorker(w)方法. 我们再来看一下 runWorker()方法的源码吧: 

```java
/**
 * Main worker run loop.  Repeatedly gets tasks from queue and
 * executes them, while coping with a number of issues:
 *
 * 1. We may start out with an initial task, in which case we
 * don't need to get the first one. Otherwise, as long as pool is
 * running, we get tasks from getTask. If it returns null then the
 * worker exits due to changed pool state or configuration
 * parameters.  Other exits result from exception throws in
 * external code, in which case completedAbruptly holds, which
 * usually leads processWorkerExit to replace this thread.
 *
 * 2. Before running any task, the lock is acquired to prevent
 * other pool interrupts while the task is executing, and then we
 * ensure that unless pool is stopping, this thread does not have
 * its interrupt set.
 *
 * 3. Each task run is preceded by a call to beforeExecute, which
 * might throw an exception, in which case we cause thread to die
 * (breaking loop with completedAbruptly true) without processing
 * the task.
 *
 * 4. Assuming beforeExecute completes normally, we run the task,
 * gathering any of its thrown exceptions to send to afterExecute.
 * We separately handle RuntimeException, Error (both of which the
 * specs guarantee that we trap) and arbitrary Throwables.
 * Because we cannot rethrow Throwables within Runnable.run, we
 * wrap them within Errors on the way out (to the thread's
 * UncaughtExceptionHandler).  Any thrown exception also
 * conservatively causes thread to die.
 *
 * 5. After task.run completes, we call afterExecute, which may
 * also throw an exception, which will also cause thread to
 * die. According to JLS Sec 14.20, this exception is the one that
 * will be in effect even if task.run throws.
 *
 * The net effect of the exception mechanics is that afterExecute
 * and the thread's UncaughtExceptionHandler have as accurate
 * information as we can provide about any problems encountered by
 * user code.
 *
 * @param w the worker
 */
final void runWorker(Worker w) {
    Thread wt = Thread.currentThread();
    Runnable task = w.firstTask;
    w.firstTask = null;
    w.unlock(); // allow interrupts
    boolean completedAbruptly = true;
    try {
        // 由前边可知, task 就是 w.firstTask
        // 如果 task为 null, 那么就不进入该 while循环, 也就不运行该 task. 如果    
        // task不为 null, 那么就执行 getTask()方法. 而getTask()方法是个无限
        // 循环, 会从阻塞队列 workQueue中不断取出任务来执行. 当阻塞队列 workQueue
        // 中所有的任务都被取完之后, 就结束下面的while循环.
        while (task != null || (task = getTask()) != null) {
            w.lock();
            // If pool is stopping, ensure thread is interrupted;
            // if not, ensure thread is not interrupted.  This
            // requires a recheck in second case to deal with
            // shutdownNow race while clearing interrupt
            if ((runStateAtLeast(ctl.get(), STOP) ||
                 (Thread.interrupted() &&
                  runStateAtLeast(ctl.get(), STOP))) &&
                !wt.isInterrupted())
                wt.interrupt();
            try {
                beforeExecute(wt, task);
                Throwable thrown = null;
                try {
                    // 执行从阻塞队列 workQueue中取出的任务.
                    task.run();
                } catch (RuntimeException x) {
                    thrown = x; throw x;
                } catch (Error x) {
                    thrown = x; throw x;
                } catch (Throwable x) {
                    thrown = x; throw new Error(x);
                } finally {
                    afterExecute(task, thrown);
                }
            } finally {
                // 将 task 置为 null, 这样使得 while循环是否继续执行的判断, 就只能依赖于判断
                // 第二个条件, 也就是 (task = getTask()) != null 这个条件, 是否满足.
                task = null;
                w.completedTasks++;
                w.unlock();
            }
        }
        completedAbruptly = false;
    } finally {
        processWorkerExit(w, completedAbruptly);
    }
}
```

第46行, 将 w.firstTask 赋值给局部变量 task. 

第56行,  这一行分为下面两部分内容来介绍:

(1)  

如果 task 为 null (也就是 w.firstTask 为 null),  那么就不进入 while循环, 也就不运行 task.  

还记得在分析 [execute()方法的源码](#source_execute) 时, 我们提到过, 该方法会在三种情况下分别调用 addWorker()方法吗? 只是这三次调用时, 分别传入的参数都不同, 而该方法第98, 99行代码以及我们对该代码的注释如下:

```java
// 再次计算线程池内的有效线程数 workerCount, 一旦发现该数量变为0, 
// 就将线程池内的线程数上限值设置为最大线程数 maximumPoolSize, 然后
// 只是创建一个线程而不去启动它, 并结束整个 execute()方法的执行.
else if (workerCountOf(recheck) == 0)
    addWorker(null, false);
```

我们在注释中写到”一旦发现该数量变为0, 就只是创建一个线程而不去启动它”,  而在分析过 [Worker类](#source_Worker) 以及 [addWorker()](#source_addWorker) 和 [runWorker()](#source_runWorker) 方法的源码后, 这句注释就很容易理解了.  下面我们就来分析一下 addWorker(null, false); 这句代码的含义吧.  这句代码为参数 firstTask 赋值为 null, 为参数 core 赋值为 false. 回到 [addWorker()方法的源码](#source_addWorker), 第78行有如下代码:

```java
w = new Worker(firstTask);
```

我们传递的 firstTask 为 null, 再看 [Worker类的源码](#source_Worker),  第32~36行是其构造方法, 在构造方法中, 将值为 null 的参数 firstTask 传给该Worker类内部的 Runnable字段 firstTask (也就是设置 w.firstTask = null), 而在 [addWorker()方法](#source_addWorker) 的第108行又去运行线程t, 也就是运行线程 w.thread, 即: 启动 Worker 对象 w 中的线程 thread. 而我们又在前边总结过一个结论: **启动一个 Worker对象中包含的线程 thread, 就相当于要执行 runWorker()方法, 并将该 Worker对象作为该方法的参数.** (见[这里](#conclusion_Worker)) 所以, 启动线程 w.thread, 就相当于是执行 runWorker(w)方法, 而我们还需要记住, 此时的 w.firstTask 是 null 的, 所以再回到 [runWorker()方法的源码](#source_runWorker), 见第46行, 我们将值为 null 的 w.firstTask 传递给局部变量 task, 这样 task 也为 null. 这样第56行 while循环的 task != null 判断条件就为 false, 从而直接跳过该 while 循环, 也就跳过了该 while 循环中第72行 task.run(); 这句启动线程的代码, 所以说, addWorker(null, false); 这句代码的含义是”只是创建一个线程而不去启动它”.

(2)  

我们继续回到 runWorker() 方法的第56行. 我们在 (1) 中分析了 w.firstTask 为 null 导致直接 while()循环直接不执行的情况. 而如果 w.firstTask 不为 null (也就是我们提交的任务不为 null). 那么在执行到 runWorker() 方法的第56行时, 由于满足了第一个条件 task != null, 所以会进入到 while 循环中,  在72行会去执行我们提交的任务, 然后在第85行, 将 task 置为 null, 使得下次循环开始时, 也就是再次来到该方法的第56行时, 由于第一个条件 task != null 为 false, 使得我们必须去判断第二个条件 (task = getTask()) != null 是否成立. 如果 getTask() 方法的返回值不为 null, 那么该方法会将其返回值赋值给 task, 然后再次进入 while 循环, 然后还是在第72行去执行这个 task, 在第85行再次将 task 置为 null, 又回到第56行,  第一个条件 task != null 依然为 false, 所以再次需要判断第二个条件 (task = getTask()) != null 是否成立, 又一次执行 getTask()方法……这样一直循环下去, 直到 getTask() 方法返回 null, 才会结束 while循环, 然后结束掉 runWorker() 方法. 那么, getTask() 方法到底是用来做什么的呢? 其实, 根据该方法的名字, 想必你应该已经猜到了吧, “获取任务”,  这个猜测对不对呢? 我们还是来看看它的源码吧, 如下所示:

```java
/**
 * Performs blocking or timed wait for a task, depending on
 * current configuration settings, or returns null if this worker
 * must exit because of any of:
 * 1. There are more than maximumPoolSize workers (due to
 *    a call to setMaximumPoolSize).
 * 2. The pool is stopped.
 * 3. The pool is shutdown and the queue is empty.
 * 4. This worker timed out waiting for a task, and timed-out
 *    workers are subject to termination (that is,
 *    {@code allowCoreThreadTimeOut || workerCount > corePoolSize})
 *    both before and after the timed wait, and if the queue is
 *    non-empty, this worker is not the last thread in the pool.
 *
 * @return task, or null if the worker must exit, in which case
 *         workerCount is decremented
 */
private Runnable getTask() {
    boolean timedOut = false; // Did the last poll() time out?

    // 无限循环.
    for (;;) {
        int c = ctl.get();
        int rs = runStateOf(c);

        // 如果线程池已停止, 或者线程池被关闭并且线程池内的阻塞队列为空, 则结束该方法并返回 null.
        // Check if queue empty only if necessary.
        if (rs >= SHUTDOWN && (rs >= STOP || workQueue.isEmpty())) {
            decrementWorkerCount();
            return null;
        }

        int wc = workerCountOf(c);

        // 如果 allowCoreThreadTimeOut 这个字段设置为 true(也就是允许核心线程受超时机制的控制), 则
        // 直接设置 timed 为 true. 反之, 则再看当前线程池中的有效线程数是否已经超过了核心线程数, 也
        // 就是是否存在非核心线程. 如果存在非核心线程, 那么也会设置 timed 为true. 
        // 如果 wc <= corePoolSize (线程池中的有效线程数少于核心线程数, 即: 线程池内运行着的都是核心线程), 
        // 并且 allowCoreThreadTimeOut 为 false(即: 核心线程即使空闲, 也不会受超时机制的限制), 
        // 那么就设置 timed 为 false.
        // Are workers subject to culling?
        boolean timed = allowCoreThreadTimeOut || wc > corePoolSize;

        // 当线程池处于 RUNNING (运行)状态但阻塞队列内已经没有任务(为空)时, 将导致有线程接下来会一直
        // 处于空闲状态. 如果空闲的是核心线程并且设置核心线程不受超时机制的影响(默认情况下就是这个设置), 
        // 那么这些核心线程将一直在线程池中处于空闲状态, 等待着新任务的到来, 只要线程池处于 RUNNING 
        // (运行)状态, 那么, 这些空闲的核心线程将一直在池子中而不会被销毁. 如果空闲的是非核心线程, 或者
        // 虽然是核心线程但是设置了核心线程受超时机制的限制, 那么当空闲达到超时时间时, 这就满足了这里的
        // if条件而去执行 if内部的代码, 通过返回 null 结束掉该 getTask()方法, 也最终结束掉 runWorker()方法.
        if ((wc > maximumPoolSize || (timed && timedOut))
            && (wc > 1 || workQueue.isEmpty())) {
            if (compareAndDecrementWorkerCount(c))
                return null;
            continue;
        }

        try {
            // 从阻塞队列中取出队首的那个任务, 设置给 r. 如果空闲线程等待超时或者该队列已经为空, 则 r为 null.
            Runnable r = timed ?
                workQueue.poll(keepAliveTime, TimeUnit.NANOSECONDS) :
                workQueue.take();

            // 如果阻塞队列不为空并且未发生超时的情况, 那么取出的任务就不为 null, 就直接返回该任务对象. 
            if (r != null)
                return r;
            timedOut = true;
        } catch (InterruptedException retry) {
            timedOut = false;
        }
    }
}
```

getTask()方法内有个无限循环, 要想结束一个无限循环, 要么用 break 语句, 要么用 return 语句. 而观察上述源码可以发现, 也只有第30, 54, 66行满足结束循环的条件, 三个return语句, 我们接下来会分别进行分析. 

先看第30行的 return null; 语句, 这是当线程池处于关闭或者停止时, 如果满足其他条件, 就会结束该循环, 并返回 null, 表示线程池已关闭且阻塞队列已空, 或者线程池已停止, 这两种情况下都没有或无法获取要处理的任务了. 再看第60~62行,  尝试从阻塞队列中取出位于队首的那个任务, 如果发生了线程等待超时, 或者执行取出动作之前, 阻塞队列已经空了, 那么任务 r 就为 null; 如果队列中还有任务并且未发生超时, 那么 r 就不为 null. 第65~66行, 如果 r 不为 null, 就表示此次取出的确实是一个实实在在的任务对象, 于是就将该任务对象返回并结束该无限循环和该方法. 当然, 我们终究是会遇到执行取出动作的时候阻塞队列已经为空的情况,  那么取出的 r 就为 null, 这样就不满足第65行的条件, 也就不会执行第66行的 return 语句, 这样又会来到第22行, 再次执行这个无限循环. 如果检测到超时条件满足了, 由于此时阻塞队列为空, 那么这就满足了第51~52行的条件, 就会执行第54行的 return null; 语句, 表示已经没有可以取出的任务了, 所以返回 null, 并结束该无限循环也结束该方法.

总之,  如果有任务可以取出的话, getTask()方法就会返回一个具体的任务对象;  如果线程池被关闭或停止, 或者阻塞队列在相当长的一段时间 (超过超时时间) 内一直没有任务可供取出, 那么就会返回 null, 表示没有任务了或者无法再获取任务了,  那么 [runWorker() 方法](#source_runWorker) 第56行的判断条件 (task = getTask()) != null 就为 false, 导致程序退出 while 循环, 最终结束该方法.

以上就是线程池对任务调度的大致原理.  至此, 这篇文章也就结束了. 感谢各位有耐心的读者能够读完这篇相对较长也较为枯燥的文章, 也希望大家能够多提宝贵的建议或意见, 互相交流学习!

**参考资料:**

1. Oracle JDK 1.8 源码 

2. [知乎—Android中的Thread与AsyncTask的区别? —肥肥鱼的回答](https://www.zhihu.com/question/30804052)

3. [聊聊并发（三）——JAVA线程池的分析和使用](http://www.infoq.com/cn/articles/java-threadPool)

4. 《Android开发艺术探索》第11章 Android的线程和线程池


