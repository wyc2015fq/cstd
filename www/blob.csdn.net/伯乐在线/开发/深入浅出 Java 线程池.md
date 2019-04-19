# 深入浅出 Java 线程池 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
### 前言
Java1.5后引入的Executor框架的最大优点是把任务的提交和执行解耦，只需把Task描述清楚，然后提交即可。至于这个Task是怎么被执行的，被谁执行的，什么时候执行的，就全部交给线程池管理。
### 小案例
### 废话不多说，先来一个小小案例。

Java
```
import java.util.concurrent.*;
/**
 * Created by j_zhan on 2016/7/15.
 */
public class ExecutorTest {
    static ExecutorService executor = Executors.newFixedThreadPool(10);
    public static void main(String[] args) throws Exception {
        Future<String> future = executor.submit(new Task());
        String result = future.get();
        System.out.println(result);
    }
    static class Task implements Callable<String> {
        @Override
        public String call() throws Exception {
            TimeUnit.SECONDS.sleep(10);
            return "A";
        }
    }
}
```
代码很简单：初始化一个线程池，提交一个任务，主线程的future.get()会阻塞线程直到任务执行完成。
### Executor框架成员
线程池实现框架中包含了一堆实现类，它们之间的关系如下，只有了解了各个类之间的关系，才能方便我们更好的理解线程池的实现。
![paste_image](http://jbcdn2.b0.upaiyun.com/2016/08/b02b59e6133fbc20b0b8ec4c7df8c018.png)
Paste_Image.png
从图中可以看到Executor、ExecutorService、ScheduledExecutorService定义线程池接口，ThreadPoolExecutor和ScheduledThreadPoolExecutor是线程池的实现，前者是一个普通的线程池，后者一个定期调度的线程池，Executors是辅助工具，用以帮助我们快速定义线程池。
### 参数
在初始化线程池时，不同的应用场景中，对参数的选择是很重要的，先来看看线程池的各个参数的含义：
1.workQueue：用来保存等待执行的任务的阻塞队列。
2.corePoolSize：线程池的基本大小。当提交一个任务到线程池时，线程池会创建一个线程来执行任务，即使有其它空闲的线程，直到线程数达到corePoolSize时就不再创建，这时会把提交的新任务放到阻塞队列。如果调用了线程池的prestartAllCoreThreads()方法，线程池会提前创建并启动所有基本线程。
3.maximumPoolSize：线程池允许创建的最大线程数。如果阻塞队列满了，并且已经创建的线程数小于最大线程数，则线程池会再创建新的线程执行任务。
4.threadFactory：创建线程的工厂。可以通过自定义线程工厂给每个线程设置有意义的名称。如guava提供的ThreadFactoryBuilder。

Java
```
new ThreadFactoryBuilder().setNameFormat("XX-task-%d").build();
```
5.rejectedExecutionHandler：饱和策略。当阻塞队列满了且没有空闲的工作线程，说明线程池处于饱和状态，那么必须采取一种策略处理提交的新任务。这个策略在默认情况下是AbortPolicy，表示无法处理新任务时抛出异常。不过，线程池提供了4种策略：
1、AbortPolicy：直接抛出异常。
2、CallerRunsPolicy：只用调用者所在的线程来运行任务。
3、DiscardOldestPolicy：丢弃阻塞队列中最近的一个任务，并执行当前任务。
4、DiscardPolicy：直接丢弃。
当然，也可以根据应用场景来实现RejectedExecutionHandler接口自定义饱和策略，如记录日志或持久化存储不能处理的任务。
6.keepAliveTime：线程活动保持时间。指工作线程空闲后，继续保持存活的时间。默认情况下，这个参数只有在线程数大于corePoolSize时才起作用。所以，如果任务很多，且每个任务的执行时间比较短，可以调大keepAliveTime，提高线程的利用率。
在初始化线程池时，对阻塞队列的选择也很重要，jdk中提供了以下几个阻塞队列：
- ArrayBlockingQueue：基于数组结构的有界阻塞队列，按FIFO原则对元素进行排序。
- LinkedBlockingQuene：基于链表结构的阻塞队列，按FIFO排序元素，吞吐量通常要高于ArrayBlockingQuene。
- SynchronousQuene：一个不存储元素的阻塞队列，每个插入操作必须等到另一个线程调用移除操作，否则插入操作一直处于阻塞状态，吞吐量通常要高于LinkedBlockingQuene。
- priorityBlockingQuene：具有优先级的无界阻塞队列。
### Exectors
Exectors是java线程池的工厂类，通过它可以快速初始化一个符合业务需求的线程池，主要提供了以下几种便捷的方式：
1.newFixedThreadPool：创建一个指定工作线程数的线程池，其中参数corePoolSize和maximumPoolSize相等，阻塞队列基于LinkedBlockingQuene。
它是一个典型且优秀的线程池，它具有线程池提高程序效率和节省创建线程时所耗的开销的优点。但是在线程池空闲时，即线程池中没有可运行任务时，它不会释放工作线程，还会占用一定的系统资源。

Java
```
public static ExecutorService newFixedThreadPool(int nThreads) {
return new ThreadPoolExecutor(nThreads, nThreads,
                              0L, TimeUnit.MILLISECONDS,
                              new LinkedBlockingQueue<Runnable>());
}
```
2.newCachedThreadPool：创建一个可缓存工作线程的线程池（工作线程默认存活时间1分钟）。该线程池有以下特点：
1、工作线程数量几乎没有上线，因为maximumPoolSize为Integer.MAX_VALUE（2147483647）。
2、如果长时间没有提交任务，且工作线程空闲了指定的时间，则该工作线程将自动终止。如果重新提交了任务，则线程池重新创建一个工作线程。
它在没有任务执行时，会释放工作线程，从而释放工作线程所占用的资源。但是，但当提交新任务时，又要创建新的工作线程，有一定的系统开销。另外一定要注意控制任务的数量，否则由于大量线程同时运行，很有会造成系统瘫痪。

Java
```
public static ExecutorService newCachedThreadPool() {
 return new ThreadPoolExecutor(0, Integer.MAX_VALUE,
                               60L, TimeUnit.SECONDS,
                               new SynchronousQueue<Runnable>());
}
```
3.newSingleThreadExecutor：创建一个只有单一工作线程的线程池。如果这个工作线程异常结束，会有另一个取代它。唯一的工作线程可以保证任务的顺序执行。
4.newScheduledThreadPool：创建一个可以在指定时间内周期性的执行任务的线程池。在实际业务中常用的场景是周期性的同步数据。

Java
```
public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize) {
 return new ScheduledThreadPoolExecutor(corePoolSize);
}
```
### Future和Callable
如果提交的任务需要返回结果，必须实现Callable接口。在上述小案例中，我们向线程池提交了一个实现Callable接口的任务，并通过Future的get方法，获取到返回值。第一次看这个代码，估计会有疑惑，等理解原理之后，就可以大彻大悟了。
在实际业务场景中，Callable和Future是成对出现的，Callable负责产生结果，Future负责获取结果。
Callable接口类似于Runnable，不过Runnable没有返回值。
Callable任务被执行，除了可以返回执行结果之外，如果任务发生异常，这个异常也可以被Future获取，即Future可以拿到异步执行任务各种结果。
通过深入分析FutureTask（Future的实现类）来看看案例中的逻辑是如何实现的：
Callable任务执行完成后返回一个FutureTask对象，FutureTask实现了Runnable和 Future接口。

Java
```
public class FutureTask<V> implements RunnableFuture<V> {
    private volatile int state;
    private static final int NEW          = 0;
    private static final int COMPLETING   = 1;
    private static final int NORMAL       = 2;
    private static final int EXCEPTIONAL  = 3;
    private static final int CANCELLED    = 4;
    private static final int INTERRUPTING = 5;
    private static final int INTERRUPTED  = 6;
    private Callable<V> callable;
    ...
}
```
1.通过submit提交任务后，任务被封装成一个FutureTask对象。
2.因为FutureTask实现了Runable接口，可以通过线程池的execute执行，这个过程后续会说明。
3.主线程执行FutureTask.get方法，导致主线程阻塞，看看是如何实现的：

Java
```
public V get() throws InterruptedException, ExecutionException {
 int s = state;
 if (s <= COMPLETING)
     s = awaitDone(false, 0L);
 return report(s);
}
// 实现主线程阻塞
private int awaitDone(boolean timed, long nanos)
 throws InterruptedException {
 final long deadline = timed ? System.nanoTime() + nanos : 0L;
 WaitNode q = null;
 boolean queued = false;
 for (;;) {
     if (Thread.interrupted()) {
         removeWaiter(q);
         throw new InterruptedException();
     }
     int s = state;
     if (s > COMPLETING) {
         if (q != null)
             q.thread = null;
         return s;
     }
     else if (s == COMPLETING) // cannot time out yet
         Thread.yield();
     else if (q == null)
         q = new WaitNode();
     else if (!queued)
         queued = UNSAFE.compareAndSwapObject(this, waitersOffset,
                                              q.next = waiters, q);
     else if (timed) {
         nanos = deadline - System.nanoTime();
         if (nanos <= 0L) {
             removeWaiter(q);
             return state;
         }
         LockSupport.parkNanos(this, nanos);
     }
     else
         LockSupport.park(this);
 }
}
```
如果FutureTask的状态state小于等于COMPLETING，awaitDone方法最终会通过 LockSupport.park阻塞主线程。
4.线程池中的工作线程执行FutureTask的run方法，代码如下：

Java
```
public void run() {
 if (state != NEW || !UNSAFE.compareAndSwapObject(this, runnerOffset, null, Thread.currentThread()))
     return;
 try {
     Callable<V> c = callable;
     if (c != null && state == NEW) {
         V result;
         boolean ran;
         try {
             result = c.call();
             ran = true;
         } catch (Throwable ex) {
             result = null;
             ran = false;
             setException(ex);
         }
         if (ran)
             set(result);
     }
 } finally {
     // runner must be non-null until state is settled to
     // prevent concurrent calls to run()
     runner = null;
     // state must be re-read after nulling runner to prevent
     // leaked interrupts
     int s = state;
     if (s >= INTERRUPTING)
         handlePossibleCancellationInterrupt(s);
 }
}
```
5.FutureTask的run方法中，如果任务执行成功，执行set(v)，设置正常返回值，否则执行setException(e)设置异常，其中唤醒主线程的奥秘就在这里中，一起来看下是如何实现的。

Java
```
protected void set(V v) {
 if (UNSAFE.compareAndSwapInt(this, stateOffset, NEW, COMPLETING)) {
     outcome = v;
     UNSAFE.putOrderedInt(this, stateOffset, NORMAL); // final state
     finishCompletion();
 }
}
//设置异常
protected void setException(Throwable t) {
 if (UNSAFE.compareAndSwapInt(this, stateOffset, NEW, COMPLETING)) {
     outcome = t;
     UNSAFE.putOrderedInt(this, stateOffset, EXCEPTIONAL); // final state
     finishCompletion();
 }
}
//唤醒主线程的核心实现
private void finishCompletion() {
 // assert state > COMPLETING;
 for (WaitNode q; (q = waiters) != null;) {
     if (UNSAFE.compareAndSwapObject(this, waitersOffset, q, null)) {
         for (;;) {
             Thread t = q.thread;
             if (t != null) {
                 q.thread = null;
                 LockSupport.unpark(t);
             }
             WaitNode next = q.next;
             if (next == null)
                 break;
             q.next = null; // unlink to help gc
             q = next;
         }
         break;
     }
 }
 done();
 callable = null;        // to reduce footprint
}
```
1、 把正常返回值或执行期间捕获的异常赋值给outcome。
2、设置FutureTask的状态为NORMAL或EXCEPTIONAL。
3、通过LockSupport.unpark(t)唤醒主线程。
### 任务提交
向线程池提交任务有两种：
1.execute()：用于提交不需要返回值的任务，这个方式无法判断任务是否执行成功。
executor.execute(runnableTask);
2.submit()：用于提交需要返回值的任务。线程池会返回一个Future对象，通过这个对象可以判断任务是否执行成功。

Java
```
Future future = executor.submit(callableTask);
```
大家注意到没有，我对两个方法的参数用了不同的变量，是因为方便大家的理解，execute提交的任务需要实现Runnable接口，而submit提交的任务需要实现Callable接口。
### 实现原理
接下去一起进入线程池内部的实现细节。

Java
```
public class ThreadPoolExecutor extends AbstractExecutorService {
    // 线程池的控制状态（用来表示线程池的运行状态（整形的高3位）和运行的worker数量（低29位））
    private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));
    // 29位的偏移量
    private static final int COUNT_BITS = Integer.SIZE - 3;
    // 最大容量（2^29 - 1）
    private static final int CAPACITY   = (1 << COUNT_BITS) - 1;
    // runState is stored in the high-order bits
    // 线程运行状态，总共有5个状态，需要3位来表示（所以偏移量的29 = 32 - 3）
    private static final int RUNNING    = -1 << COUNT_BITS;
    private static final int SHUTDOWN   =  0 << COUNT_BITS;
    private static final int STOP       =  1 << COUNT_BITS;
    private static final int TIDYING    =  2 << COUNT_BITS;
    private static final int TERMINATED =  3 << COUNT_BITS;
    // 阻塞队列
    private final BlockingQueue<Runnable> workQueue;
    // 可重入锁
    private final ReentrantLock mainLock = new ReentrantLock();
    // 存放工作线程集合
    private final HashSet<Worker> workers = new HashSet<Worker>();
    // 终止条件
    private final Condition termination = mainLock.newCondition();
    // 最大线程池容量
    private int largestPoolSize;
    // 已完成任务数量
    private long completedTaskCount;
    // 线程工厂
    private volatile ThreadFactory threadFactory;
    // 拒绝执行处理器
    private volatile RejectedExecutionHandler handler;
    // 线程等待运行时间
    private volatile long keepAliveTime;
    // 是否运行核心线程超时
    private volatile boolean allowCoreThreadTimeOut;
    // 核心池的大小
    private volatile int corePoolSize;
    // 最大线程池大小
    private volatile int maximumPoolSize;
    // 默认拒绝执行处理器
    private static final RejectedExecutionHandler defaultHandler =
        new AbortPolicy();
    //
    private static final RuntimePermission shutdownPerm =
        new RuntimePermission("modifyThread");
    ...
}
```
ctl这个变量，第一眼看上去，完全不知所云，其实它是用来描述线程池的状态：整数的高3位表示线程池的运行状态、低29位用来描述工作线程（后面会讲到）数量，最多表示2^29 – 1，当然实际应用中不可能创建这么多线程。不要问我为什么要这么设计，我猜，实在猜不出来，大概位运算效率高吧。其中，线程池的运行状态有如下几种：
1.RUNNING ： 接受新任务并且处理已经进入阻塞队列的任务。
2.SHUTDOWN ： 不接受新任务，但是处理已经进入阻塞队列的任务。
3.STOP : 不接受新任务，不处理已经进入阻塞队列的任务并且中断正在运行的任务。
4.TIDYING : 所有的任务都已经终止，workerCount为0， 线程转化为TIDYING状态并且调用terminated钩子函数 。
5.TERMINATED: terminated钩子函数已经运行完成。
当向线程池提交一个任务时，线程池是如何处理这个任务？先看示意图和代码实现：
![Paste_Image1](http://jbcdn2.b0.upaiyun.com/2016/08/9c4b388bc7ccb6424e3ae5ca0d05df8d.png)
Paste_Image.png

Java
```
public void execute(Runnable command) {
        if (command == null)  throw new NullPointerException();    
        int c = ctl.get();// 获取线程池状态
        if (workerCountOf(c) < corePoolSize) { 
            if (addWorker(command, true)) // 添加工作线程
                return;
            c = ctl.get();
        }
        if (isRunning(c) && workQueue.offer(command)) { 
            int recheck = ctl.get();
            if (!isRunning(recheck) && remove(command)) 
                reject(command);
            else if (workerCountOf(recheck) == 0) 
                addWorker(null, false);
        }
        else if (!addWorker(command, false)) 
            reject(command);
    }
```
1.如果当前运行的线程数少于corePoolSize，则创建新的工作线程处理任务，否则进入步骤2。
2.如果线程池处于运行状态，则把任务放入BlockingQueue中，如果可用工作线程为0时，则创建新的工作线程，处理BlockingQueue的任务。
3.如果无法将任务加入到BlockingQueue，则创建新的线程处理任务，前提是目前运行的线程数小于maximumPoolSize，否则进入步骤4 任务被拒绝。
由于工作线程时存放在HashSet中的，所以在访问工作线程的时候，需要进行加锁操作。
以前我有一个疑问：为什么线程池中的一个线程在生命周期内可以连续执行多个任务？按理说线程执行完之后，理应被回收销毁。
线程池创建线程时，会将线程封装成工作线程Worker：

Java
```
private final class Worker extends AbstractQueuedSynchronizer implements Runnable {
        final Thread thread;
        Runnable firstTask;
        volatile long completedTasks;
        Worker(Runnable firstTask) {
            setState(-1); // inhibit interrupts until runWorker
            this.firstTask = firstTask;
            this.thread = getThreadFactory().newThread(this);
        }
        /** Delegates main run loop to outer runWorker  */
        public void run() {
            runWorker(this);
        }
        protected boolean tryAcquire(int unused) {
            if (compareAndSetState(0, 1)) {
                setExclusiveOwnerThread(Thread.currentThread());
                return true;
            }
            return false;
        }
        protected boolean tryRelease(int unused) {
            setExclusiveOwnerThread(null);
            setState(0);
            return true;
        }
        public void lock()        { acquire(1); }
        public boolean tryLock()  { return tryAcquire(1); }
        public void unlock()      { release(1); }
       ...
    }
```
从上面可以看出，工作线程Worker继承了同步器AQS，对同步器AQS不了解的可以看看深入浅出java同步器，同时还实现Runable接口，为什么要这么设计？因为在线程池创建工作线程worker成功后，直接调用work.start()方法启动该线程（即在worker实例中初始化的线程），并在runWorker方法中传递了自身实例，接下去让我们看看线程池的核心方法runWorker：

Java
```
final void runWorker(Worker w) {
        Thread wt = Thread.currentThread();
        Runnable task = w.firstTask;
        w.firstTask = null;
        w.unlock(); // allow interrupts
        boolean completedAbruptly = true;
        try {
            while (task != null || (task = getTask()) != null) {
                w.lock();
                try {
                    task.run();
                } finally {
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
    private Runnable getTask() {
        boolean timedOut = false; // Did the last poll() time out?
        for (;;) {
            int c = ctl.get();
            int rs = runStateOf(c);
            // Check if queue empty only if necessary.
            if (rs >= SHUTDOWN && (rs >= STOP || workQueue.isEmpty())) {
                decrementWorkerCount();
                return null;
            }
            int wc = workerCountOf(c);
            // Are workers subject to culling?
            boolean timed = allowCoreThreadTimeOut || wc > corePoolSize;
            if ((wc > maximumPoolSize || (timed && timedOut))
                && (wc > 1 || workQueue.isEmpty())) {
                if (compareAndDecrementWorkerCount(c))
                    return null;
                continue;
            }
            try {
                Runnable r = timed ?
                    workQueue.poll(keepAliveTime, TimeUnit.NANOSECONDS) :
                    workQueue.take();
                if (r != null)
                    return r;
                timedOut = true;
            } catch (InterruptedException retry) {
                timedOut = false;
            }
        }
    }
```
从上述实现可以看出，
1.工作线程开始执行任务之前，先释放锁（设置state为0），表示允许中断。
2.如果当前任务为空，则通过getTask从阻塞队列中获取任务。如果线程池状态或配置参数改变，导致getTask返回null，或者其它内部错误抛出异常，会触发processWorkerExit方法。
3.在工作线程执行任务之前，需要获取锁。其实我没明白这里为什么要获取锁，注释上是说为了防止在任务执行期间，其它线程中断，了解的同学可以留言告知，不慎感激。
4.每个任务执行时都会触发beforeExecute操作，用户可以自定义任务执行前的操作，如果beforeExecute抛出异常，就不会执行任务了。
5.假设beforeExecute和task都执行完成，会触发afterExecute操作，这个方法也可以自定义用户操作。
工作线程在执行完当前任务后，通过while循环，从阻塞队列中获取任务。
1.workQueue.take()方法，如果阻塞队列中没有任务，当前工作线程在阻塞队列的条件变量上等待挂起。
2.workQueue.poll(keepAliveTime, TimeUnit.NANOSECONDS)方法，如果在keepAliveTime时间内，阻塞队列还是没有任务，返回null。
### 总结
合理的利用线程池，可以给我们带来以下好处：
1.降低资源消耗。
2.提高响应速度。
3.提高线程的可管理性。
线程是稀缺资源，如果无限制的创建，不仅会消耗系统资源，还会降低系统的稳定性，使用线程池可以进行统一的分配、调优和监控。
ThreadPoolExecutor是线程池框架的一个核心类，通过对源码的分析，可以知道其对资源进行了复用，并非无限制的创建线程，可以有效的减少线程创建和切换的开销。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
