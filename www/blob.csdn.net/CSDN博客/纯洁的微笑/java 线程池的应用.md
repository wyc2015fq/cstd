# java 线程池的应用 - 纯洁的微笑 - CSDN博客
2018年09月14日 08:08:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：100
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqK9dyrBK5usicvWu7BXQheqtP9C2O8EIn239aGSKGL4YFYeAbN08o0xd8Bk8RJ96RN3UKGbTdDmAQ/640?wx_fmt=jpeg)
作者
疼蛋之丸
来源
https://www.jianshu.com/p/22c225a5ee59
**一、前言**
在总结了线程池的一些原理及实现细节之后，而本文就成了（二）。因为在写完第一篇关于java线程池的文章之后，越发觉得还有太多内容需要补充，每次都是修修补补，总觉得还缺点什么。在第一篇中，我着重描述了java线程池的原理以及它的实现，主要的点在于它是如何工作的。而本文的内容将更为上层，重点在于如何应用java线程池，算是对第一篇文章的一点补充，这样对于java线程池的学习和总结稍微完整一些。
使用过java线程池的人应该知道，我们都习惯使用Executors这个工厂类来获取我们需要的线程池，而这个工厂不仅仅可以产生一种线程池，而是可以产生若干种不同应用场景的线程池，你应当在合适的场景中使用合适的线程池，以保证最好的效率。下文将主要剖析这个类的一些细节，为了保证本文的相对独立性，可能会提及一些在第一篇文章中提过的内容，这样阅读起来相对流畅一些，体验更佳。本文依然不会基于java线程池做更多应用方面的描述，而是从线程池类型这个角度出发，试图探索不同种类的线程池的特点和使用场景，从某种意义上来说，这样描述的意义较于实际的例子来说更为有用。
“授人以鱼不如授人以渔” ！！！！
**二、Executors工厂类详解**
介于本文的重点在于Executors这个工厂类，下面首先列出了Executors这个类提供的一些方法。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqK9dyrBK5usicvWu7BXQheqibXsAuff6iarQAtGFpw8ypgR13709PficNkeUMNUsrUpWBAoIa0ypQ5UA/640?wx_fmt=jpeg)
Executors方法
本文需要对以上12个类做一些区分，从其特点出发，然后分析其应用场景。
1.public static ExecutorService newFixedThreadPool(int nThreads)
使用这个方法会产生这样一个线程池：线程池最多会保持nThreads个线程处于活动状态，如果当前所有任务都处于活动状态，那么新提交的任务会被添加到任务阻塞队列中去。总结一下就是：使用固定大小的线程池，并发数是固定的。
```
* Creates a thread pool that reuses a fixed number of threads
     * operating off a shared unbounded queue.  At any point, at most
     * {@code nThreads} threads will be active processing tasks.
     * If additional tasks are submitted when all threads are active,
     * they will wait in the queue until a thread is available.
     * If any thread terminates due to a failure during execution
     * prior to shutdown, a new one will take its place if needed to
     * execute subsequent tasks.  The threads in the pool will exist
     * until it is explicitly {@link ExecutorService#shutdown shutdown}.
```
2.public static ExecutorService newFixedThreadPool(int nThreads, ThreadFactory threadFactory)
相比于newFixedThreadPool(int nThreads)， 你可以使用这个方法来传递你自己的线程工厂，线程工厂是用来干嘛的？就是用来生成线程的，你可以使用线程工厂做一些个性化的线程特性定制。
3.public static ExecutorService newWorkStealingPool(int parallelism)
在了解或者使用这个方法之前，你该对java的Fork/Join并行框架有一些了解，从名字上我们就知道这个方法生产出来的线程池具有某种“小偷”的行为，在Fork/Join里面，线程的工作模式为“盗窃算法”，也就是在自己的任务队列消费完了之后不是进入等到状态，而是会主动去偷窃别的线程的任务来做，其实是没有一种奖励机制来鼓励这些线程去帮助别的线程去消费任务的，所以可以认为这些线程都是好人，都为了快速完成任务协调作战。这种工作方式的重点在于，每个线程都将有一个任务队列，线程之间通过“偷窃”的方式互相帮助来完成任务的消费。
可以看下这个方法的实现：
```
return new ForkJoinPool(parallelism, ForkJoinPool.defaultForkJoinWorkerThreadFactory, null, true);
```
可以发现，这个方法不是使用我们在第一篇文章中分析了ThreadPoolExecutor来生成线程池。而是使用了ForkJoinPool，也就是Fork/Join里面的线程池，关于ForkJoinPool更为深入的分析不再本文的涉及范围内，你只要知道Fork/Join框架的一般运行原理就可以了，下面的描述可以帮助你决策你是否需要该方法提供的线程池来工作：
```
* Creates a thread pool that maintains enough threads to support
 * the given parallelism level, and may use multiple queues to
 * reduce contention. The parallelism level corresponds to the
 * maximum number of threads actively engaged in, or available to
 * engage in, task processing. The actual number of threads may
 * grow and shrink dynamically. A work-stealing pool makes no
 * guarantees about the order in which submitted tasks are
 * executed.
```
4.public static ExecutorService newWorkStealingPool()
参考newWorkStealingPool(int parallelism)。
5.public static ExecutorService newSingleThreadExecutor()
下面是对该方法的描述：
```
* Creates an Executor that uses a single worker thread operating
 * off an unbounded queue. (Note however that if this single
 * thread terminates due to a failure during execution prior to
 * shutdown, a new one will take its place if needed to execute
 * subsequent tasks.)  Tasks are guaranteed to execute
 * sequentially, and no more than one task will be active at any
 * given time. Unlike the otherwise equivalent
 * {@code newFixedThreadPool(1)} the returned executor is
 * guaranteed not to be reconfigurable to use additional threads.
```
可以从方法的名字上知道，该方法产生的线程池仅仅有一个Worker，任何时刻都将只有一个Worker在工作，添加的任务有很大概率被放在阻塞任务队列中等待执行。这些任务会被顺序执行，这个方法的返回值其实是对ThreadPoolExecutor的一层包装，下面的代码展示了最终执行任务的类：
```java
static class DelegatedExecutorService extends AbstractExecutorService {
    private final ExecutorService e;
    DelegatedExecutorService(ExecutorService executor) { e = executor; }
    public void execute(Runnable command) { e.execute(command); }
    public void shutdown() { e.shutdown(); }
    public List<Runnable> shutdownNow() { return e.shutdownNow(); }
    public boolean isShutdown() { return e.isShutdown(); }
    public boolean isTerminated() { return e.isTerminated(); }
    public boolean awaitTermination(long timeout, TimeUnit unit)
        throws InterruptedException {
        return e.awaitTermination(timeout, unit);
    }
    public Future<?> submit(Runnable task) {
        return e.submit(task);
    }
    public <T> Future<T> submit(Callable<T> task) {
        return e.submit(task);
    }
    public <T> Future<T> submit(Runnable task, T result) {
        return e.submit(task, result);
    }
    public <T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks)
        throws InterruptedException {
        return e.invokeAll(tasks);
    }
    public <T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks,
                                         long timeout, TimeUnit unit)
        throws InterruptedException {
        return e.invokeAll(tasks, timeout, unit);
    }
    public <T> T invokeAny(Collection<? extends Callable<T>> tasks)
        throws InterruptedException, ExecutionException {
        return e.invokeAny(tasks);
    }
    public <T> T invokeAny(Collection<? extends Callable<T>> tasks,
                           long timeout, TimeUnit unit)
        throws InterruptedException, ExecutionException, TimeoutException {
        return e.invokeAny(tasks, timeout, unit);
    }
}
```
从上面的代码可以看出，这个类其实就是使用了构造时传递的参数e来完成，更像是代理。而e是什么？看下面的代码：
```
ThreadPoolExecutor(1, 1, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<Runnable>())
```
其实就是一个只有一个线程的ThreadPoolExecutor。
6.public static ExecutorService newSingleThreadExecutor(ThreadFactory threadFactory)
参考newSingleThreadExecutor()，多了一个线程工厂参数。
7.public static ExecutorService newCachedThreadPool()
首先看它的方法体内容：
```
return new ThreadPoolExecutor(0, Integer.MAX_VALUE, 60L, TimeUnit.SECONDS, new SynchronousQueue<Runnable>());
```
可以看到，核心线程数量为0，而上限为Integer.MAX_VALUE，而且keepAliveTime为60秒，那么这个线程池的工作模式为：只要有任务呗提交，而且当前没有空闲的线程可用，那么就会创建一个新的Worker来工作，一个线程工作完了之后会缓存（idle）60秒，如果60秒之内有新的任务提交，则会被唤醒进入工作模式，否则60秒后就会被回收。可以参考下面的描述：
```
* Creates a thread pool that creates new threads as needed, but
 * will reuse previously constructed threads when they are
 * available.  These pools will typically improve the performance
 * of programs that execute many short-lived asynchronous tasks.
 * Calls to {@code execute} will reuse previously constructed
 * threads if available. If no existing thread is available, a new
 * thread will be created and added to the pool. Threads that have
 * not been used for sixty seconds are terminated and removed from
 * the cache. Thus, a pool that remains idle for long enough will
 * not consume any resources. Note that pools with similar
 * properties but different details (for example, timeout parameters)
 * may be created using {@link ThreadPoolExecutor} constructors.
```
从描述上，我们可以想到，其实这种类型的线程池比较适合于短期高流量的场景，也就是我们所说的“秒杀”场景，在那样的场景下，需要的线程数量较多，那么使用该类型的线程池可以满足，而且该线程池还有自动收缩的功能，在不需要那么多线程的时候，会自动回收线程，释放资源。
8.public static ExecutorService newCachedThreadPool(ThreadFactory threadFactory)
参考newCachedThreadPool()。
9.public static ScheduledExecutorService newSingleThreadScheduledExecutor()
只有一个线程的调度线程池，类似于newSingleThreadExecutor，但是该方法生产的线程池具备调度功能，下面是对该方法的描述：
```
* Creates a single-threaded executor that can schedule commands
 * to run after a given delay, or to execute periodically.
 * (Note however that if this single
 * thread terminates due to a failure during execution prior to
 * shutdown, a new one will take its place if needed to execute
 * subsequent tasks.)  Tasks are guaranteed to execute
 * sequentially, and no more than one task will be active at any
 * given time. Unlike the otherwise equivalent
 * {@code newScheduledThreadPool(1)} the returned executor is
 * guaranteed not to be reconfigurable to use additional threads.
```
10.public static ScheduledExecutorService newSingleThreadScheduledExecutor(ThreadFactory threadFactory)
参考newSingleThreadExecutor和newSingleThreadScheduledExecutor。
11.public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize)
参考newFixedThreadPool，但是返回类型不一样。
12.public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize, ThreadFactory threadFactory)
参考newFixedThreadPool。
通过上面的分析，我们应该对java线程池的理解更为深入，再次说明，本文仅仅是对第一篇java线程池文章内容的补充，你应该首先阅读第一篇文章，之后再来阅读本文，那样内容上更完整，但是单独阅读本文一样具备独立性，但是收获肯定没有同时阅读两篇文章那样多。
当然，还有许多需要补充的内容，比如Fork/Join框架的线程池的实现原理以及其细节，以及线程池使用的阻塞队列的特点以及实现细节，这些内容要更为底层，需要的知识与要求的能力要多更高，会在以后的文章中陆续来探索。
END
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnrnzUNX2KRVxAAzQcdevxOMnrGvPJ9mMeUR4XiatKR6F3auSmDICibxseIKoNLfo3fhiam8icfbSl4FTw/640?wx_fmt=jpeg)
