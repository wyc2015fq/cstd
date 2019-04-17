# Android 面试题整理总结（三）Java 并发 - 我的学习笔记 - CSDN博客





2018年12月01日 22:21:50[zouzhiheng](https://me.csdn.net/u011330638)阅读数：162








# 零、目录

一、并发基础

1、Java 有哪几种创建新线程的方法及区别？销毁一个线程的方法呢？

2、线程的生命周期？

3、Thread 的 join 有什么作用？

4、sleep 和 wait 的区别？

5、多线程的使用场景？为什么要使用多线程？多线程需要注意的问题？（上下文开销、死锁等）

6、多线程是否一定会高效？关于并发理解多少？

7、Java 多线程之间如何通信？除了 notify 还有什么别的方式可以唤醒线程？
二、线程池

1、线程池的几个参数的理解？

2、线程池核心线程数一般定义多少，为什么？

3、线程池中核心线程数跟最大线程数如何定义？

4、四种线程池的使用场景？

5、线程池的实现原理？

6、自定义线程池应该如何设计？
三、并发集合

1、说几个并发的集合？

2、Concurrenthashmap 的插入操作是直接操作数组中的链表吗？

3、ConcurrentHashMap 的原理，分的段数是多少？

4、ConcurrentHashMap 是如何实现分段锁的？
四、线程安全

1、什么是线程安全？如何保证线程安全？

2、同步和非同步、阻塞和非阻塞的概念？

3、lock 语句和 synchronize 对比？ReentrantLock 和 synchronized 的区别？

4、volatile 关键字的作用？

5、synchronized 同步代码块还有同步方法本质上锁住的是谁？为什么？

6、synchronized、volatile 关键字有什么区别？还有哪些具有类似功能的关键字？

7、乐观锁、悲观锁概念及使用场景？

8、静态同步锁与普通同步锁的区别？

9、Java 中的锁，各种同步方式之间的区别？

10、不用锁如何保证 int 自增安全？

11、JVM 锁的优化，偏向锁、轻量级锁概念及原理？

12、volatile 原理？

13、sycronized 原理？

13、ReentrantLock，AQS 的底层实现？

15、对 Java 内存模型的理解？

16、对 Java 对象头、markword 的理解？

16、ThreadLocal 的理解？
# 一、并发基础

## 1、Java 有哪几种创建新线程的方法及区别？销毁一个线程的方法呢？

创建线程的方法：Thread、Runnable、FutureTask(Callable)、Executors，其实最终都需要通过 Thread 来完成，比如 FutureTask，比如不配合 Thread 使用的话，仍然无法创建新线程

销毁线程：
- 
Thread 无法销毁，只能中断

- 
FutureTask 可以调用 cancel 方法取消，内部实际上调用的是 interrupt

- 
ExecutorService 可以调用方法 shutdown 或 shutdownNow 结束线程，内部实际上调用的仍然是 interrupt


总结：只能调用 interrupt 方法结束线程，无法销毁线程。

## 2、线程的生命周期？

《Java 编程思想》：
- 
新建。创建时只会短暂处于这种状态，已分配了必需的系统资源，并执行了初始化。

- 
就绪。这种状态下，只要线程调度器把时间片分配给线程，线程就可以运行。

- 
阻塞。线程可以运行，但被某个条件阻止了，调度器将忽略该线程，不会分配 CPU 时间。

- 
死亡。任务已结束，不再是可运行的，但还可以被中断。


《深入理解 Java 虚拟机》：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201222525955.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=,size_16,color_FFFFFF,t_70)
## 3、Thread 的 join 有什么作用？

阻塞直到线程运行结束或等待时间到期：

```
public class Thread implements Runnable {

    private final Object lock = new Object();

    public final void join(long millis) throws InterruptedException {
        synchronized(lock) {
        long base = System.currentTimeMillis();
        long now = 0;

        if (millis < 0) {
            throw new IllegalArgumentException("timeout value is negative");
        }

        if (millis == 0) { // 时间不限
             // 等待直接线程执行完毕
            while (isAlive()) {
                lock.wait(0);
            }
        } else {
             // 等待直接线程执行完毕或等待时间到期
            while (isAlive()) {
                long delay = millis - now;
                if (delay <= 0) {
                    break;
                }
                lock.wait(delay);
                now = System.currentTimeMillis() - base;
            }
        }
    }
    
}
```

## 4、sleep 和 wait 的区别？

sleep 会阻塞直到等待时间结束，或抛出 interrupt 异常，并一直持有对象锁

wait 会阻塞直到被 notify 唤醒，或等待时间结束，或抛出 interrupt 异常，只能在 synchronized 内部调用，会释放对象锁，唤醒后重新获取该锁，获取失败则再次进入阻塞状态

## 5、多线程的使用场景？

使用场景：
- 
IO，包括网络请求、文件读写等会阻塞主线程的

- 
耗费时间较长的任务，比如音视频编解码

- 
后台任务，比如定期更新某些数据的任务、持续监控采集信息的任务

- 
……


## 6、多线程是否一定会高效？线程的开销？

不是。

线程带来的开销有：
- 
上下文切换。如果可运行的线程大于 CPU 的数量，那么操作系统会把某个正在运行的线程调度出来，从而使其它线程能够使用 CPU，这将导致一次上下文切换，在这个过程中将保存当前运行线程的执行上下文，并将新调度进来的线程的执行上下文设置为当前上下文。

- 
资源消耗。活跃的线程会消耗系统资源，尤其是内存；空闲的线程会占用许多内存，大量线程在竞争 CPU 资源时还将产生其他的性能开销。

- 
稳定性。可创建的线程数量是有限的，如果超过会抛出 OOM 异常。

- 
系统调用。 系统调用的代价较高，且需要在用户态和内核态中来回切换，会消耗一定的内核资源，包括线程之间的协调（锁、信号、同步）、线程的创建于与销毁、线程的调度等。


## 7、Java 多线程之间如何通信？除了 notify 还有什么别的方式可以唤醒线程？

通信方式：
- 
wait、notify

- 
生产者消费者队列。BlockingQueue

- 
管道。PipedReader、PipedWriter

- 
yield。建议调用器把 CPU 时间分配给其它线程，可以忽略。


wait 被唤醒的方式：
- 
notify

- 
中断线程

- 
设置等待时间


# 二、线程池

## 1、线程池的几个参数的理解？

ThreadPoolExecutor 的构造方法如下：

```
public class ThreadPoolExecutor extends AbstractExecutorService {

    public ThreadPoolExecutor(int corePoolSize,
                              int maximumPoolSize,
                              long keepAliveTime,
                              TimeUnit unit,
                              BlockingQueue<Runnable> workQueue,
                              ThreadFactory threadFactory,
                              RejectedExecutionHandler handler) {
        ...
    }
    
}
```
- 
corePoolSize:: 核心线程数, 除非设置了 allowCoreThreadTimeOut, 否则会一直存在于线程池中

- 
maximumPoolSize: 线程池允许存在的最大线程数

- 
keepAliveTime: 非核心线程能够存活的最长时间，如果设置了 allowCoreThreadTimeOut，那么也将对核心线程产生作用

- 
unit: keepAliveTime 的时间单位

- 
workQueue: 存储线程的工作队列。实现类有 ArrayBlockingQueue、LinkedBlockingQueue、SynchronousQueue、PriorityBlockingQueue 等。

- 
threadFactory: 用于创建线程的工厂类。默认的线程工厂（ThreadFactory）将创建一个新的非守护（后台）线程，不包含特殊的配置信息。

- 
hanlder: 饱和策略。工作队列被填满之后，如果继续添加任务，饱和策略就开始发挥作用。Java 提供了 AbortPolicy、CallerRunsPolicy、DiscardPolicy、DiscardOldestPolicy 四种。


## 2、线程池大小要怎么设置？

线程池大小应该根据 CPU 个数（Runtime.availableProcessors）来动态计算，避免过大或过小，过大将导致大量线程在相对很少的 CPU 和 内存资源上发生竞争；过少将导致许多空闲的处理器无法执行工作。

要正确设置线程池的大小，必须分析计算环境、资源预算和任务的特性，然后估算出任务的等待时间与计算时间的比值，再进行调节。
- 
对于计算密集型的任务，当核心线程数设置为 Runtime.availableProcessors + 1 时，能实现最优的利用率。因为当计算密集型的线程偶尔由于页缺失故障或者其他原因暂停时，这个额外的线程也能确保 CPU 的始终周期不会被浪费。

- 
对于包含 IO 操作或者其它阻塞操作的任务，由于线程并不会一直执行，因此线程池的规模应该更大。


《Java 并发编程》给出了一个计算公式：

Count = CPU 个数 * 目标 CPU 利用率( 0 ~ 1) * (1 + 等待时间 / 计算时间)

## 3、线程池中核心线程数跟最大线程数如何定义？为什么？

// TODO 如果有更好的解答，欢迎指出

核心线程数、最大线程数、keepAliveTime 共同管理者线程的创建和销毁。

核心线程的大小是线程池试图维护的池大小，即使没有任务执行，线程池的大小也等于核心池的大小，并且直到工作队列被充满前，池都不会创建更多的线程。个人理解，可以按照第 9 题所说的来设置。

最大线程数的设置该和工作队列一起考虑，如果是 SynchronousQueue，最大线程数通常设置为 Integer.MAX_VALUE，适合处理大量的耗时较少的任务；如果是无界队列，最大线程数则应该和核心线程数一致，因为非核心线程永远也不会被创建；如果是有界队列，则要慢慢调整。具体可见第 13 题。

## 11、四种线程池的使用场景？
- 
FixedThreadPool。corePoolSize 不能过大，且核心线程不会被回收，因此适用于需要把同时运行的任务数量控制在一定范围内的场景，任务数量不多时能够立即处理。

- 
CachedThreadPool。没有核心线程，非核心线程数量没有限制，工作队列为不存储元素的 SynchronousQueue，因此适合执行大量的需要立即处理且耗时较少的任务，但对于高负载的服务器，它就不适用了，因为有更多的任务时，它会不断地创建新线程，很容易导致机器负载过重。

- 
ScheduledThreadPool。主要用于执行定时任务和具有固定周期的重复任务。

- 
SingleThreadExecutor。适合任务需要排队进行且不用考虑同步的场景。


## 4、线程池的实现原理？

ThreadPoolExecutor 将根据 corePoolSize 和 maximumPoolSize 设置的边界自动调整池大小，当在方法 execute 中提交新任务时，如果运行的线程少于corePoolSize，即使其他工作线程处于空闲状态，也会创建一个新线程来处理该请求。如果有多个核心线程正在运行，但运行的线程少于 maximumPoolSize，则只有在队列已满时才会创建新线程。

## 5、自定义线程池应该如何设计？

核心线程数跟最大线程数参考第 9、10 题。

KeepAliveTime 提供了一种不主动使用池时减少资源消耗的方法，一般设置为 60s 即可，如果不希望有非核心线程，如 FixThreadPool，那么可以设为 0s。

工作队列有三种策略：
- 
直接交接。比如 SynchronousQueue，它将任务交给线程而不保存它们，如果没有可以空闲线程，那么就会创建一个新线程来执行任务，通常需要无限制的 maximumPoolSizes 以避免拒绝新提交的任务。但需要注意任务执行时间过长过多的话，可能存在线程无限制增长的可能。

- 
无界队列。使用无限制队列（例如，没有预定义容量的 LinkedBlockingQueue）将导致新任务在所有 corePoolSize 线程忙时在队列中等待。因此，只会创建 corePoolSize 线程，并且 maximumPoolSize 的值因此没有任何影响。当每个任务完全独立于其他任务时，这是适用的，因此任务不会影响彼此的执行。

- 
有界队列。有限的队列（例如 ArrayBlockingQueue）与有限 maximumPoolSizes 一起使用时有助于防止资源耗尽，但可能更难以调整和控制。使用大型队列和小型池最小化 CPU 使用率，系统资源和上下文切换开销，但可能导致人为的低吞吐量。使用小队列通常需要更大的池大小，这会使 CPU 更加繁忙，可能会遇到不可接受的调度开销，这也会降低吞吐量。


ThreadFactory 默认创建非守护线程，不包含特殊的配置信息，可以看需求自定义，比如可以为创建的线程附加 UncaughtExceptionHandler。

RejectedExecutionHandler 有 AbortPolicy（抛出异常）、CallerRunsPolicy（在用户线程执行）、DiscardPolicy（丢弃）、DiscardOldestPolicy（丢弃排队时间最长的任务） 四种策略。

此外可以扩展 beforeExecute、afterExecute 等方法。

# 三、并发集合

## 1、说几个并发的集合？

ConcurrentHashMap 使用分段锁机制，可以在并发访问环境下将实现更高的吞吐量，而在单线程环境中只损失非常小的性能。部分源码如下：

```
public class ConcurrentHashMap<K,V> extends AbstractMap<K,V>
    implements ConcurrentMap<K,V>, Serializable {
    
    
    private static final int MAXIMUM_CAPACITY = 1 << 30;
    private static final int DEFAULT_CAPACITY = 16;

    private static final float LOAD_FACTOR = 0.75f;

    static final int NCPU = Runtime.getRuntime().availableProcessors();

    // 懒加载，在第一次插入元素的时候初始化
    // 元素数量只会是 2 的次幂
    transient volatile Node<K,V>[] table;

    /**
     * 在 resize 的时候使用
     */
    private transient volatile Node<K,V>[] nextTable;

    // views
    private transient KeySetView<K,V> keySet;
    private transient ValuesView<K,V> values;
    private transient EntrySetView<K,V> entrySet;
    
    static class Node<K,V> implements Map.Entry<K,V> {
        final int hash;
        final K key;
        volatile V val;
        volatile Node<K,V> next;
        ...
        
        // 遍历链表
        Node<K,V> find(int h, Object k) {
            Node<K,V> e = this;
            if (k != null) {
                do {
                    K ek;
                    if (e.hash == h &&
                        ((ek = e.key) == k || (ek != null && k.equals(ek))))
                        return e;
                } while ((e = e.next) != null);
            }
            return null;
        }
        
    }
    
    ...
    
    // key 和 value 都不能为 null
    public V put(K key, V value) {
        return putVal(key, value, false);
    }

    /** Implementation for put and putIfAbsent */
    final V putVal(K key, V value, boolean onlyIfAbsent) {
        if (key == null || value == null) throw new NullPointerException();
        int hash = spread(key.hashCode()); // 求哈希值，得出存储该对象的数组下标
        int binCount = 0;
        for (Node<K,V>[] tab = table;;) {
            Node<K,V> f; int n, i, fh;
            if (tab == null || (n = tab.length) == 0)
                tab = initTable(); // 懒加载
            else if ((f = tabAt(tab, i = (n - 1) & hash)) == null) { // 如果这个位置上没有元素，(n - 1) & hash 相当于 n % table.size
                // CAS 操作，把 new 的 Node 放入位置 i
                if (casTabAt(tab, i, null,
                             new Node<K,V>(hash, key, value, null)))
                    break;                   // no lock when adding to empty bin
            }
            else if ((fh = f.hash) == MOVED)
                tab = helpTransfer(tab, f);
            else { // 如果这个位置上存在元素链表
                V oldVal = null;
                synchronized (f) { // f 是这个位置上的第一个元素，使用该对象作为同步锁，并发时不会影响其它位置上的元素的修改
                    if (tabAt(tab, i) == f) {
                        if (fh >= 0) {
                            binCount = 1;
                            for (Node<K,V> e = f;; ++binCount) {
                                K ek;
                                if (e.hash == hash &&
                                    ((ek = e.key) == key ||
                                     (ek != null && key.equals(ek)))) { // 找到了一个相同的元素
                                    oldVal = e.val;
                                    if (!onlyIfAbsent) // 如果设置了只在没有相同元素的时候更新值，那么忽略这个新的值
                                        e.val = value;
                                    break;
                                }
                                Node<K,V> pred = e; // 遍历
                                if ((e = e.next) == null) { // 把新的元素添加到尾节点
                                    pred.next = new Node<K,V>(hash, key,
                                                              value, null);
                                    break;
                                }
                            }
                        }
                        else if (f instanceof TreeBin) { // 可能和 HashMap 一样，是为了维护插入顺序或者自然循序而设置的，忽略
                            Node<K,V> p;
                            binCount = 2;
                            if ((p = ((TreeBin<K,V>)f).putTreeVal(hash, key,
                                                           value)) != null) {
                                oldVal = p.val;
                                if (!onlyIfAbsent)
                                    p.val = value;
                            }
                        }
                        else if (f instanceof ReservationNode)
                            throw new IllegalStateException("Recursive update");
                    }
                }
                if (binCount != 0) { // 正常插入元素
                    if (binCount >= TREEIFY_THRESHOLD) // 同一链表下的结点过多
                        treeifyBin(tab, i); // 调整节点位置
                    if (oldVal != null)
                        return oldVal;
                    break;
                }
            }
        }
        addCount(1L, binCount);
        return null;
    }
    
    // 无同步
    public V get(Object key) {
        Node<K,V>[] tab; Node<K,V> e, p; int n, eh; K ek;
        int h = spread(key.hashCode());  // 求哈希值，得出存储该对象的数组下标
        if ((tab = table) != null && (n = tab.length) > 0 &&
            (e = tabAt(tab, (n - 1) & h)) != null) { // 如果 table 不为 null，且数组在该下标的位置上存在元素
            if ((eh = e.hash) == h) {
                if ((ek = e.key) == key || (ek != null && key.equals(ek))) // 如果找到了相等的对象，则返回
                    return e.val;
            }
            else if (eh < 0)
                return (p = e.find(h, key)) != null ? p.val : null; // 使用内部类 Node 的 find 方法遍历链表
            while ((e = e.next) != null) { // 遍历链表
                if (e.hash == h &&
                    ((ek = e.key) == key || (ek != null && key.equals(ek))))
                    return e.val;
            }
        }
        return null;
    }
    
}
```

可以看到，ConcurrentHashMap 的大体逻辑和 HashMap 和一样的，所采用的同步方法是：
- 
get 方法不同步，但链表数组 table 使用了 volatile 修饰，保证了可见性

- 
put 方法使用了 CAS 和分段锁的方式同步


a) 如果 table 对应位置上为空，则使用 CAS 操作，把对象放进去

b) 如果 table 对应位置上有元素，则把这个位置上存储的链表的第一个元素作为同步对象，把新增的元素放入链表尾部，或更新旧的值

CopyOnWriteArrayList，在执行读取操作时不需要同步，只有进行写入操作时才会复制一个副本，不影响原来的数据，因此只需要保证底层数组的可见性即可，适用于迭代（读取）操作远多于修改操作的场合。部分源码如下：

```
public class CopyOnWriteArrayList<E>
    implements List<E>, RandomAccess, Cloneable, java.io.Serializable {
    
    final transient Object lock = new Object();

    private transient volatile Object[] elements;
    
    // 读操作不会进行同步
    final Object[] getArray() {
        return elements;
    }
    
    public int size() {
        return getArray().length;
    }

    public boolean isEmpty() {
        return size() == 0;
    }
    
    // 写操作会复制一个副本
    public boolean add(E e) {
        synchronized (lock) {
            Object[] elements = getArray();
            int len = elements.length;
            Object[] newElements = Arrays.copyOf(elements, len + 1);
            newElements[len] = e;
            setArray(newElements);
            return true;
        }
    }
    
    final void setArray(Object[] a) {
        elements = a;
    }
    
}
```

BlockingQueue 适用于生产者-消费者模式，实现类有 LinkedBlockingQueue、ArrayBlockingQueue、PriorityBlockingQueue、SynchronousQueue。以 LinkedBlockingQueue 为例，添加操作和删除操作分别使用不同的锁进行同步，对于 size 等读取操作则使用 AtomicInteger 来获取：

```
public class LinkedBlockingQueue<E> extends AbstractQueue<E>
        implements BlockingQueue<E>, java.io.Serializable {
    
    static class Node<E> {
        E item;
        Node<E> next;
        Node(E x) { item = x; }
    }

    private final int capacity;
    /** Current number of elements */
    private final AtomicInteger count = new AtomicInteger();

    transient Node<E> head;
    private transient Node<E> last;

    private final ReentrantLock takeLock = new ReentrantLock();
    private final ReentrantLock putLock = new ReentrantLock();
    private final Condition notEmpty = takeLock.newCondition();
    private final Condition notFull = putLock.newCondition();
    
    public LinkedBlockingQueue() {
        this(Integer.MAX_VALUE);
    }

    public LinkedBlockingQueue(int capacity) {
        if (capacity <= 0) throw new IllegalArgumentException();
        this.capacity = capacity;
        last = head = new Node<E>(null);
    }

    public void put(E e) throws InterruptedException {
        if (e == null) throw new NullPointerException();

        int c = -1;
        Node<E> node = new Node<E>(e);
        final ReentrantLock putLock = this.putLock;
        final AtomicInteger count = this.count;
        putLock.lockInterruptibly();
        try {
            // 通过 AtomicInteger 判断队列是否满了
            while (count.get() == capacity) {
                notFull.await(); // 等待直到队列有空位置
            }
            enqueue(node);
            c = count.getAndIncrement();
            if (c + 1 < capacity)
                notFull.signal();
        } finally {
            putLock.unlock();
        }
        if (c == 0)
            signalNotEmpty();
    }

    // 放到尾节点的位置上
    private void enqueue(Node<E> node) {
        last = last.next = node;
    }
    
    public E poll() {
        final AtomicInteger count = this.count;
        if (count.get() == 0)
            return null;
        E x = null;
        int c = -1;
        final ReentrantLock takeLock = this.takeLock;
        takeLock.lock();
        try {
            // 通过 AtomicInteger 判断是否存在元素
            if (count.get() > 0) {
                x = dequeue();
                c = count.getAndDecrement();
                if (c > 1)
                    notEmpty.signal();
            }
        } finally {
            takeLock.unlock();
        }
        if (c == capacity)
            signalNotFull(); // 提醒现在队列有空位置
        return x;
    }

    // 获取头结点之后的第一项元素
    private E dequeue() {
        Node<E> h = head;
        Node<E> first = h.next;
        h.next = h; // help GC
        head = first;
        E x = first.item;
        first.item = null;
        return x;
    }

    public int size() {
        return count.get();
    }
    
}
```

## 2、ConcurrentHashMap 的插入操作是直接操作数组中的链表吗？

是的。如果链表为空，就通过 CAS 操作把对象放到数组里面，作为链表的第一个元素；如果链表不为空，则把链表的第一个元素作为同步对象，把新增的元素放入链表尾部，或更新旧的值。

## 3、ConcurrentHashMap 的原理，分的段数是多少？

table 的 size 是多少，段数就有多少。

## 4、ConcurrentHashMap 是如何实现分段锁的？

把链表的第一个元素作为同步对象。

# 四、线程安全

## 1、什么是线程安全？如何保证线程安全？

线程安全是针对共享的、可变的对象而言的，不可变的、线程私有的对象是线程安全的

Java 的线程安全分 5 个等级：不可变、绝对线程安全、相对线程安全、线程兼容、线程对立

保证线程安全的方法有：
- 
互斥，就是各种锁，属于悲观锁

- 
CAS 操作，属于乐观锁

- 
可重入代码，即可以随时中断，回来继续执行后不会出现任何错误的代码，如果一个方法，它的返回结果是可预测的，只要输入了相同的数据，就能返回相同的结果，那么它就满足可重入性的要求。

- 
线程本地存储，比如 ThreadLocal。

- 
不可变对象。


## 2、同步和非同步、阻塞和非阻塞的概念？

同步：多条线程同时操作可共享的同一个资源时，在当前正在执行的线程完成对数据的修改前，其它线程会被阻塞，直到数据成功被修改后，才轮到第二条线程执行

非同步：线程不安全

阻塞：sleep、wait、竞争锁、长时间的 IO 等

## 3、lock 语句和 synchronize 对比？ReentrantLock 和 synchronized 的区别？

Lock 是一个接口，提供了 lock、lockInterruptibly、tryLock、unlock、newCondition 等方法，synchronized 和 Lock 区别如下：
- 
lock 和 unlcok 较为灵活，但也更难以控制，一般需要配合 try-finnaly 使用

- 
synchronized 在竞争锁而处于阻塞状态时，无法被中断，而 Lock 可以通过 lockInterruptibly 中断阻塞状态

- 
Lock 可以使用 tryLock 方法在无法获取锁的时候放弃操作，而 synchronized 如果获取不到锁，只能阻塞等待

- 
一个 Lock 可以对应多个 Condition（newCondition），而 synchronized 只能使用对象锁的 wait 和 notify 方法


ReentrantLock 拥有和 Lock 接口相同的几个方法，但除了以上区别之外，ReentranLock 还能实现公平锁，即等待的线程可以按照申请锁的时间顺序来依次获得锁

## 4、volatile 关键字的作用？

可见性：保证修改数据后，其它线程能够立刻得知这个修改

禁止指令重排序：Java 虚拟机的即时编译器可能会在不影响代码执行结果的情况下，对代码的顺序重新排列，volatile 能够禁止这个优化

## 5、synchronized 同步代码块还有同步方法本质上锁住的是谁？为什么？

在 Java 虚拟机中，每个类和对象都是和同一个 moniter 关联的，因此 synchronized 本质上锁住的是对象自己，但如果是静态方法，那么使用的是类锁，类锁实际上也是对象锁，Java 虚拟机会在加载一个 class 文件的时候创建一个 Class 实例，synchronized 修饰的静态方法，锁住的就是这个 Class 实例。

## 6、synchronized、volatile 关键字有什么区别？还有哪些具有类似功能的关键字？

synchronized 除了保证可见性、禁止指令重排序之外，还能保证大范围的原子性，即线程安全。

类似的关键字：final。

## 7、乐观锁、悲观锁概念及使用场景？

悲观锁：即互斥锁，保证同一时间只有一条线程修改某一资源。适用于写操作比较频繁的场景。

乐观锁：基于冲突检测，对应 CAS 操作，先比较数据是否被修改了，如果没有，就更新数据，存在 ABA 问题。适用于读操作比较频繁的场景。

## 8、静态同步锁与普通同步锁的区别？

静态同步锁锁住的是类实例

普通同步锁锁住的是对象自己

## 9、Java 中的锁，各种同步方式之间的区别？

锁：
- 
synchronized：使用对象的内置锁

- 
ReentrantLock：可实现公平锁，有 lockInteruptibly、tryLock、newCondition 等方法

- 
Semaphore：可以控制同时执行的线程数量

- 
CountDownLatch：计数为 0 时释放锁，计数器无法重置

- 
CyclicBarrier：同 CountDownLatch，但计数器可以重用

- 
ReadWriteLock：允许多个读操作同时进行，但每次只允许一个写操作，保证在获取 readLock 时，能看到上一次释放 writeLock 时的所有更新。


同步方式：
- 
互斥，就是各种锁，属于悲观锁

- 
CAS 操作，属于乐观锁

- 
可重入代码，即可以随时中断，回来继续执行后不会出现任何错误的代码，如果一个方法，它的返回结果是可预测的，只要输入了相同的数据，就能返回相同的结果，那么它就满足可重入性的要求。

- 
线程本地存储，比如 ThreadLocal。

- 
不可变对象。


## 10、不用锁如何保证 int 自增安全？

CAS 操作，可通过 AtomicInteger 等类实现

## 11、JVM 锁的优化，偏向锁、轻量级锁概念及原理？

虚拟机对锁的优化：
- 
自选锁与自适应自旋。不同步，而是循环等待一段时间，如果其它线程释放了锁，就继续执行，否则进行同步。

- 
锁清除。代码上使用了同步，但实际上不需要同步的代码，可以把同步锁清除。

- 
锁粗化。对于频繁的互斥同步操作，可以把锁的作用范围粗化到操作序列的外部，以减少加锁解锁的操作。

- 
轻量级锁。当需要锁定某个对象时，可以不使用重量级的同步锁（synchronized），而使用对象头的 Mark Word 执行 CAS 操作标记该线程拥有了这个对象的锁，如果有两条以上的线程竞争同一个锁，再膨胀为重量级锁（synchronized）。

- 
偏向锁。偏向锁的在无竞争的情况下把整个同步都消除掉。意思是这个锁会偏向于第一个获得它的线程，如果在接下来的执行过程中，该锁没有被其它的线程获取，则持有偏向锁的线程将永远不需要再进行同步。同样需要借助对象头的 Mark Word 和 CAS 操作。


## 12、volatile 原理？

可见性的原理：
- 
修改 volatile 变量时会强制将修改后的值刷新的主内存中

- 
修改 volatile 变量后会导致其他线程工作内存中对应的变量值失效


禁止指令重排序的原理：  对 volatile 变量的赋值操作后面会有一个内存屏障，读操作不会被重排序到内存屏障之前

## 13、sycronized 原理？

对应的字节码指令是 moniterenter、moniterexit

## 14、ReentrantLock、AQS 的底层实现？

AQS 核心是通过一个共享变量来同步状态，变量的状态由子类去维护，而AQS框架做的是：
- 
线程阻塞队列的维护

- 
线程阻塞和唤醒


在 AQS 中维护着一个 FIFO 的同步队列，当线程获取同步状态失败后，则会加入到这个同步队列的队尾并一直保持着自旋。在同步队列中的线程在自旋时会判断其前驱节点是否为首节点，如果为首节点则不断尝试获取同步状态，获取成功则退出同步队列，执行任务。当线程执行结束后，会释放同步状态，释放后会唤醒其后继节点。

共享变量的修改都是通过 Unsafe 类提供的 CAS 操作完成的。 AbstractQueuedSynchronizer 类的主要方法是 acquire 和 release，典型的模板方法， 下面这4个方法由子类去实现：

```
protected boolean tryAcquire(int arg)  
protected boolean tryRelease(int arg)  
protected int tryAcquireShared(int arg)  
protected boolean tryReleaseShared(int arg)
```

acquire 方法用来获取锁，返回 true 说明线程获取成功继续执行，一旦返回 false 则线程加入到等待队列中，等待被唤醒，release 方法用来释放锁。 一般来说实现的时候这两个方法被封装为 lock 和 unlock 方法。

ReentrantLock 基于 AbstractQueuedSynchronizer，内部类 Sync 继承 AbstractQueuedSynchronizer，Sync 又有两个子类 NonfairSync、FairSync，两个内部类的区别在于 lock 方法不同。每 lock 一次，计数器加 1，并记录占有锁的线程为当前线程；每 unlock 一次，计数器减 1，为 0 时，设置占有锁的线程为 null，表明其它线程可以获取该锁了。

```
public class ReentrantLock implements Lock, java.io.Serializable {
    /** Synchronizer providing all implementation mechanics */
    private final Sync sync;
    
    public ReentrantLock() {
        sync = new NonfairSync();
    }

    public ReentrantLock(boolean fair) {
        sync = fair ? new FairSync() : new NonfairSync();
    }
    
    public void lock() {
        sync.lock();
    }
    
    public boolean tryLock() {
        return sync.nonfairTryAcquire(1);
    }
    
    public void unlock() {
        sync.release(1); // 最终调用了 tryRelease
    }
    
    abstract static class Sync extends AbstractQueuedSynchronizer {

        abstract void lock();

        final boolean nonfairTryAcquire(int acquires) {
            final Thread current = Thread.currentThread();
            int c = getState();
            if (c == 0) {
                if (compareAndSetState(0, acquires)) {  // 更新状态
                    setExclusiveOwnerThread(current); // Reentrant 是独占锁，设置当前占有锁的对象为 current 线程
                    return true;
                }
            }
            else if (current == getExclusiveOwnerThread()) { // 如果当前线程已经占有锁了，则更新状态(计数器加 1)
                int nextc = c + acquires;
                if (nextc < 0) // overflow
                    throw new Error("Maximum lock count exceeded");
                setState(nextc);
                return true;
            }
            return false;
        }

        protected final boolean tryRelease(int releases) {
            int c = getState() - releases;
            if (Thread.currentThread() != getExclusiveOwnerThread()) // 当前线程没有占有锁，无法释放，抛出异常
                throw new IllegalMonitorStateException();
            boolean free = false;
            if (c == 0) {
                free = true;
                setExclusiveOwnerThread(null); // 设置占有锁的对象为空，其它线程可以获取锁了
            }
            setState(c); // 更新状态(计数器减 1)
            return free;
        }

        protected final boolean isHeldExclusively() {
            // While we must in general read state before owner,
            // we don't need to do so to check if current thread is owner
            return getExclusiveOwnerThread() == Thread.currentThread(); // 当前线程是否占有了锁
        }

        final boolean isLocked() {
            return getState() != 0;
        }
        
    }
    
    static final class NonfairSync extends Sync {

        final void lock() {
            if (compareAndSetState(0, 1))
                setExclusiveOwnerThread(Thread.currentThread());
            else
                acquire(1); // 最终调用的是 tryAcquire
        }

        protected final boolean tryAcquire(int acquires) {
            return nonfairTryAcquire(acquires);
        }
    }
    
}
```

## 15、对 Java 内存模型的理解？

让计算机并发执行并没有想象中那么简单，一个重要的复杂性是绝大多数的运算任务都不能只靠处理器计算就能完成，处理器至少要与内存交互，由于内存与处理器的运算速度相差巨大，因此现在计算机系统不得不加入一层接近处理器运算速度的高速缓存来作为内存与处理器之间的缓冲：将运算需要用到的数据复制到缓存中，让运算能快速进行，当运算结束后再从缓存同步回内存之中。

但高速缓存也带来了一个新的问题：缓存一致性，因为每个处理器都有自己的高速缓存，而它们又共享同一主内存。为了解决这个问题，每个处理器访问缓存时都需要遵守例如 MSI 之类的协议。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201222612571.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=,size_16,color_FFFFFF,t_70)

Java 虚拟机规范试图定义一种 Java 内存模型（Java Memory Model，JMM）来屏蔽掉各种硬件和操作系统的内存访问差异，以实现让 Java 程序在各种平台下都能达到一致的内存访问效果。

Java 内存模型的主要目标是定义程序中各个变量的访问规则，即在虚拟机中将变量存储到内存和从内存中取出变量这样的底层细节。变量指实例字段、静态字段和构成数组对象的元素，不包括局部变量和方法参数。

Java 内存模型规定了所有的变量都存储在主内存中（可类比为物理硬件中的主内存），每条线程还有自己的工作内存（可类比为高速缓存），线程的工作内存中保存了被该线程使用到的变量的主内存副本拷贝，线程对变量的所有操作都必须在工作内存中进行，而不能直接读写主内存中的变量，不同线程之间也无法直接访问对方工作内存中的变量，线程间变量值的传递均需要通过主内存来完成。线程、主内存、工作内存三者的交互关系如图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201222621933.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=,size_16,color_FFFFFF,t_70)

简而言之：

缓存一致性：
- 
处理器。绝大多数的运算任务都不能只靠处理器计算就能完成

- 
高速缓存。由于内存计算速度和处理器相差巨大，因此加速了高速缓存作为缓冲

- 
缓存一致性。每个处理器都有自己的高速缓存，它们共享同一主内存，每个处理器访问缓存时必须遵守一定协议


Java 内存模型：
- 
Java 虚拟机规范试图定义一种 Java 内存模型来屏蔽掉各种硬件和操作系统的内存访问差异，以实现让 Java 程序在各种平台下都能达到一致的内存访问效果

- 
Java 内存模型规定了所有的变量都存储在主内存中（可类比为物理硬件中的主内存），每条线程还有自己的工作内存（可类比为高速缓存）

- 
线程的工作内存中保存了被该线程使用到的变量的主内存副本拷贝，线程对变量的所有操作都必须在工作内存中进行，而不能直接读写主内存中的变量

- 
不同线程之间也无法直接访问对方工作内存中的变量，线程间变量值的传递均需要通过主内存来完成


## 16、对 Java 对象头、Mark Word 的理解？

在 HotSpot 虚拟机中，对象在内存中存储的布局可以分为 3 个区域：对象头、实例数据、对齐填充（Padding）。

HotSpot 虚拟机的对象头分为两部分信息，第一部分用于存储对象自身的运行时数据，如哈希码、GC 分代年龄等，这部分数据的长度在 32bit 和 64bit 的虚拟机中分别为 32bit、64bit，官方称它为 “Mark Word”。对象头的另一部分是类型指针，即对象指向它的类元数据的指针，虚拟机通过这个指针来确定这个对象是哪个类的实例。如果对象是一个数组，那在对象头中还必须有一块用于记录数组长度的数据。

在实现轻量级锁、偏向锁时，需要借助对象头的 Mark Word 标记当前线程拥有了锁。

简而言之：
- 
对象在内存中存储的信息包括：对象头、实例数据、对齐填充

- 
对象头包含三部分：Mark Word、类型指针、数组长度。Mark Word 指哈希码、GC 分代年龄等；类型指针是指向方法去对象类型数据的指针；如果是数组，那么还有一个长度信息。

- 
Mark Word 和 CAS 是实现轻量级锁、偏向锁的基础。


## 17、ThreadLocal 的理解？

ThreadLocal 提供了线程本地存储的功能，即线程和对象之间存在一一对应的关系，每条线程都有自己对于同一对象的副本，每条线程可以修改自己的数据而不会对其它线程造成影响。

部分源码如下：

```
public class ThreadLocal<T> {

    private final int threadLocalHashCode = nextHashCode();

    // 静态变量，整个类使用
    private static AtomicInteger nextHashCode = new AtomicInteger();

    private static final int HASH_INCREMENT = 0x61c88647;

    // 静态方法
    private static int nextHashCode() {
        return nextHashCode.getAndAdd(HASH_INCREMENT);
    }

    public ThreadLocal() {
    }
    
    public T get() {
        Thread t = Thread.currentThread();
        ThreadLocalMap map = getMap(t);
        if (map != null) {
            ThreadLocalMap.Entry e = map.getEntry(this); // 找到了这条线程对应的元素
            if (e != null) {
                @SuppressWarnings("unchecked")
                T result = (T)e.value; // value 是执行 set 或 setInitialValue 方法时保存的
                return result;
            }
        }
        return setInitialValue(); // 否则返回初始值，并新建 ThreadLocalMap
    }
    
    ThreadLocalMap getMap(Thread t) {
        return t.threadLocals; // threadLocals 是 thread 的一个成员变量，默认为 null
    }
    
    // 返回初始值，并新建 ThreadLocalMap
    // 内容和 set 方法很像，不调用 set 方法是考虑到 set 方法被子类复写的情况
    private T setInitialValue() {
        T value = initialValue();
        Thread t = Thread.currentThread();
        ThreadLocalMap map = getMap(t);
        if (map != null)
            map.set(this, value); // 设置新的值
        else
            createMap(t, value); // 创建 ThreadLocalMap，并赋予初始值
        return value;
    }
    
    // 这个方法用于提供给子类复写
    protected T initialValue() {
        return null;
    }
    
    void createMap(Thread t, T firstValue) {
        // 一条线程对应一个 ThreadLocalMap，但可能使用了多个 ThreadLocal
        // 每个 ThreadLocal 存储不同的值
        t.threadLocals = new ThreadLocalMap(this, firstValue);
    }
    
    public void set(T value) {
        Thread t = Thread.currentThread();
        ThreadLocalMap map = getMap(t);
        if (map != null)
            map.set(this, value); // 设置新的值
        else
            createMap(t, value); // 创建 ThreadLocalMap 并赋予初始值
    }
    
    // ThreadLocalMap 是自定义的 HashMap
    // 一条线程可能有一个或多个 ThreadLocal，因此需要使用类似 HashMap 的结构保存数据
    static class ThreadLocalMap {
    
        // key 为 null 时表明该 Entry 是陈旧的，可以删除
        static class Entry extends WeakReference<ThreadLocal<?>> {
            /** The value associated with this ThreadLocal. */
            Object value;

            Entry(ThreadLocal<?> k, Object v) {
                super(k);
                value = v;
            }
        }
        
        private static final int INITIAL_CAPACITY = 16;
        private Entry[] table;
        private int size = 0;
        private int threshold; // Default to 0
    
        ThreadLocalMap(ThreadLocal<?> firstKey, Object firstValue) {
            table = new Entry[INITIAL_CAPACITY];
            int i = firstKey.threadLocalHashCode & (INITIAL_CAPACITY - 1);
            table[i] = new Entry(firstKey, firstValue);
            size = 1;
            setThreshold(INITIAL_CAPACITY);
        }
        
        private void set(ThreadLocal<?> key, Object value) {
            Entry[] tab = table;
            int len = tab.length;
            int i = key.threadLocalHashCode & (len-1);

            // 遍历
            for (Entry e = tab[i];
                 e != null;
                 e = tab[i = nextIndex(i, len)]) {
                ThreadLocal<?> k = e.get();

                // found，更新元素的值
                if (k == key) {
                    e.value = value;
                    return;
                }

                // key 为 null，属于陈旧条目
                if (k == null) {
                    replaceStaleEntry(key, value, i);
                    return;
                }
            }

            // 没找到对应的元素，new 一个
            tab[i] = new Entry(key, value);
            int sz = ++size;
            // 删除陈旧条目，resize
            if (!cleanSomeSlots(i, sz) && sz >= threshold)
                rehash();
        }
        
        private Entry getEntry(ThreadLocal<?> key) {
            int i = key.threadLocalHashCode & (table.length - 1);
            Entry e = table[i];
            if (e != null && e.get() == key) // 找到了这条线程对应的元素
                return e;
            else
                return getEntryAfterMiss(key, i, e);
        }
        
    }

}
```

可以看到，Thread 内部有一个 ThreadLocalMap 对象，这是一个类似于 HashMap 的结构，键的类型为 ThreadLocal，即一条线程可以存储多个 ThreadLocal 变量（一个 ThreadLocal 对应一个变量），它们由 ThreadLocalMap 进行管理。get 或 set 方法都会创建 ThreadLocalMap，并设置初始值，get 方法的初始值由 initialValue 提供。








