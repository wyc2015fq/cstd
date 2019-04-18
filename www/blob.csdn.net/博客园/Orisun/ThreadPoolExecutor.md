# ThreadPoolExecutor - Orisun - 博客园







# [ThreadPoolExecutor](https://www.cnblogs.com/zhangchaoyang/articles/4340803.html)





### BlockingQueue

先来说3种BlockingQueue
- SynchronousQueue。队列长度始终为0，因为任何一个put操作必须等待一个take操作，否则put就一直阻塞。所以一旦put成功之后，元素马上就被take出去了，队列长度始终为0。
- ArrayBlockingQueue。基于定长数组的队列。生产者和消费者使用的锁不是隔离的，即take和put共用同一个锁。
- LinkedBloackingQueue。不指定容量时其容量为Integer.MAX，有耗尽内存的风险。指定capacity后，如果size达到capacity，则继续进行put操作会阻塞，这一点跟普通的java collection是不一样的，普通的collection即使指定了容量，当size达到capacity后也会自动扩容。生产者和消费者使用的锁是隔离的。

### ThreadPoolExecutor

    public ThreadPoolExecutor(int corePoolSize,
                              int maximumPoolSize,
                              long keepAliveTime,
                              TimeUnit unit,
                              BlockingQueue<Runnable> workQueue,
                              ThreadFactory threadFactory,
                              RejectedExecutionHandler handler)


当向ThreadPoolExecutor提交任务时存在以下几种情况：
- 线程池中的线程数没有达到corePoolSize，则直接创建新的线程执行任务，并把该线程放到线程池中。
- 线程池中的线程数达到了corePoolSize，同时workQueue队列还没满，则把任务放到workQueue中去，等待被执行。线程池中如果有某个线程空闲下来，它会去workQueue中take()一个任务来执行，如果workQueue已空，则take()操作会阻塞。
- 如果线程池中的线程数达到corePoolSize了，且workQueue也满了，则创建新的线程执行任务，并把该线程放到线程池中。此时线程池中的线程数已经开始超过corePoolSize了，线程池中超过corePoolSize的那部分线程，当它空闲下来且空闲时间达到keepAliveTime（时间单位由unit指定）后线程自行销毁。线程空闲下来后为什么不直接销毁，而是要先keep一段时间？因为频繁地创建销毁线程是很消耗CPU资源的，好不容易从CPU那儿借来一个线程，就索性多借一会儿，说不定刚空闲一小会儿就又有新任务需要执行呢。
- 线程池数目达到了maximumPoolSize，则采用handler指定的策略丢弃任务。

注意：如果workQueue使用的是没有指定容量的LinkedBloackingQueue，则workQueue永远不会满，线程池的线程数永远不会超过corePoolSize，此时指定maximumPoolSize、keepAliveTime、handler都是没有意义的。

public interface ThreadFactory {
    Thread newThread(Runnable r);
}


自己实现的ThreadFactory，在newThread()方法中可以自定义Thread的一些属性，比如线程优化先、名称、是否为后台线程、线程所属的组、线程对异常的处理方式等等。看一下Executors类中的DefaultThreadFactory是怎么写的：

static class DefaultThreadFactory implements ThreadFactory {
        static final AtomicInteger poolNumber = new AtomicInteger(1);
        final ThreadGroup group;
        final AtomicInteger threadNumber = new AtomicInteger(1);
        final String namePrefix;

        DefaultThreadFactory() {
            SecurityManager s = System.getSecurityManager();
            group = (s != null)? s.getThreadGroup() :
                                 Thread.currentThread().getThreadGroup();
            namePrefix = "pool-" +
                          poolNumber.getAndIncrement() +
                         "-thread-";
        }

        public Thread newThread(Runnable r) {
            Thread t = new Thread(group, r,
                                  namePrefix + threadNumber.getAndIncrement(),
                                  0);
            if (t.isDaemon())
                t.setDaemon(false);
            if (t.getPriority() != Thread.NORM_PRIORITY)
                t.setPriority(Thread.NORM_PRIORITY);
            return t;
        }
    }


RejectedExecutionHandler有这么几种：
- 
ThreadPoolExecutor.AbortPolicy()：抛出java.util .concurrent.RejectedExecutionException异常。这是ThreadPoolExecutor默认采用的拒绝策略。

- ThreadPoolExecutor.CallerRunsPolicy()：重试添加当前的任务，他会自动重复调用execute()方法
- ThreadPoolExecutor.DiscardOldestPolicy()：抛弃旧的任务
- ThreadPoolExecutor.DiscardPolicy()：抛弃当前的任务

### 用ThreadPoolExecutor构建ExecutorService

java中几种常见的ExecutorService都是通过ThreadPoolExecutor来实现的。

public static ExecutorService newFixedThreadPool(int nThreads) {  
        return new ThreadPoolExecutor(nThreads, nThreads,  
                                      0L, TimeUnit.MILLISECONDS,  
                                      new LinkedBlockingQueue<Runnable>());  
}  


使用的是无界队列LinkedBlockingQueue，所以线程池中的线程数始终不会超过nThreads。

public static ExecutorService newSingleThreadExecutor() {  
        return new FinalizableDelegatedExecutorService  
            (new ThreadPoolExecutor(1, 1,  
                                    0L, TimeUnit.MILLISECONDS,  
                                    new LinkedBlockingQueue<Runnable>()));  
}  


SingleThreadExecutor是FixedThreadPool的特例，把nThreads改为1即可。

public static ExecutorService newCachedThreadPool() {  
        return new ThreadPoolExecutor(0, Integer.MAX_VALUE,  
                                      60L, TimeUnit.SECONDS,  
                                      new SynchronousQueue<Runnable>());  
} 


使用了SynchronousQueue，所以一旦提交一个任务马上就会创建一个线程去执行它，这种线程的keepAlive时间较长，是60秒。又因为corePoolSize为0，所以当确实没有什么任务需要执行时，线程池中的线程数就是0。这一点跟其他ThreadPoolExecutor是不一样的，其他ThreadPoolExecutor当线程池中的数目达到corePoolSize后就再也降不到corePoolSize以下了，因为就算无任务可做这部分线程会因为在workQueue上调用了take()操作而阻塞，但这部分线程不会被销毁。












