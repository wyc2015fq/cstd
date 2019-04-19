# ThreadPoolExecutor使用和思考(一)-线程池大小设置与BlockingQueue的三种实现区别 - 零度的博客专栏 - CSDN博客
2017年03月28日 10:30:41[零度anngle](https://me.csdn.net/zmx729618)阅读数：553
工作中多处接触到了ThreadPoolExecutor。趁着现在还算空，学习总结一下。
前记：
- jdk官方文档（javadoc）是学习的最好，最权威的参考。
- 文章分上中下。上篇中主要介绍ThreadPoolExecutor接受任务相关的两方面入参的意义和区别，池大小参数corePoolSize和maximumPoolSize，BlockingQueue选型（**SynchronousQueue，****`LinkedBlockingQueue，`****`ArrayBlockingQueue`**）；中篇中主要聊聊与keepAliveTime这个参数相关的话题；下片中介绍一下一些比较少用的该类的API，及他的近亲：ScheduledThreadPoolExecutor。
- 如果理解错误，请直接指出。
查看JDK帮助文档，可以发现该类比较简单，继承自AbstractExecutorService，而AbstractExecutorService实现了ExecutorService接口。
ThreadPoolExecutor的完整构造方法的签名是：
```
**ThreadPoolExecutor**(int corePoolSize, int maximumPoolSize,
 long keepAliveTime,TimeUnit unit,BlockingQueue<Runnable> workQueue,ThreadFactory threadFactory,RejectedExecutionHandler handler)
```
先记着，后面慢慢解释。
===============================神奇分割线==================================
其实对于ThreadPoolExecutor的构造函数网上有N多的解释的，大多讲得都很好，不过我想先换个方式，从Executors这个类入手。因为他的几个构造工厂构造方法名字取得令人很容易了解有什么特点。但是其实Executors类的底层实现便是ThreadPoolExecutor！
**ThreadPoolExecutor是Executors类的底层实现。**
在JDK帮助文档中，有如此一段话：
“强烈建议程序员使用较为方便的 `Executors` 工厂方法 `Executors.newCachedThreadPool()`（无界线程池，可以进行自动线程回收）、`Executors.newFixedThreadPool(int)`（固定大小线程池）和`Executors.newSingleThreadExecutor()`（单个后台线程），它们均为大多数使用场景预定义了设置。”
## 可以推断出ThreadPoolExecutor与Executors类必然关系密切。
===============================神奇分割线==================================
OK，那就来看看源码吧，从newFixedThreadPool开始。
**ExecutorService newFixedThreadPool(int nThreads):固定大小线程池。**
可以看到，corePoolSize和maximumPoolSize的大小是一样的（实际上，后面会介绍，如果使用无界queue的话maximumPoolSize参数是没有意义的），keepAliveTime和unit的设值表名什么？-就是该实现不想keep
 alive！最后的BlockingQueue选择了LinkedBlockingQueue，该queue有一个特点，他是无界的。
Java代码  ![收藏代码](http://dongxuan.iteye.com/images/icon_star.png)
- publicstatic ExecutorService newFixedThreadPool(int nThreads) {  
- returnnew ThreadPoolExecutor(nThreads, nThreads,  
-                                       0L, TimeUnit.MILLISECONDS,  
- new LinkedBlockingQueue<Runnable>());  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
**ExecutorService newSingleThreadExecutor()：单线程。**
可以看到，与fixedThreadPool很像，只不过fixedThreadPool中的入参直接退化为1
Java代码  ![收藏代码](http://dongxuan.iteye.com/images/icon_star.png)
- publicstatic ExecutorService newSingleThreadExecutor() {  
- returnnew FinalizableDelegatedExecutorService  
-             (new ThreadPoolExecutor(1, 1,  
-                                     0L, TimeUnit.MILLISECONDS,  
- new LinkedBlockingQueue<Runnable>()));  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
**ExecutorService newCachedThreadPool()：无界线程池，可以进行自动线程回收。**
这个实现就有意思了。首先是无界的线程池，所以我们可以发现maximumPoolSize为big big。其次BlockingQueue的选择上使用SynchronousQueue。可能对于该BlockingQueue有些陌生，简单说：该QUEUE中，每个插入操作必须等待另一个
线程的对应移除操作。比如，我先添加一个元素，接下来如果继续想尝试添加则会阻塞，直到另一个线程取走一个元素，反之亦然。（想到什么？就是缓冲区为1的生产者消费者模式^_^）
注意到介绍中的自动回收线程的特性吗，为什么呢？先不说，但注意到该实现中corePoolSize和maximumPoolSize的大小不同。
Java代码  ![收藏代码](http://dongxuan.iteye.com/images/icon_star.png)
- publicstatic ExecutorService newCachedThreadPool() {  
- returnnew ThreadPoolExecutor(0, Integer.MAX_VALUE,  
-                                       60L, TimeUnit.SECONDS,  
- new SynchronousQueue<Runnable>());  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
===============================神奇分割线==================================
到此如果有很多疑问，那是必然了（除非你也很了解了）
先从[BlockingQueue](http://dongxuan.iteye.com/admin/blogs/901659/)<[Runnable](http://dongxuan.iteye.com/admin/blogs/901659/)> workQueue这个入参开始说起。在JDK中，其实已经说得很清楚了，一共有三种类型的queue。以下为引用：（我会稍微修改一下，并用红色突出显示）
所有 `BlockingQueue` 都可用于传输和保持提交的任务。可以使用此队列与池大小进行交互：
- 如果运行的线程少于 corePoolSize，则 Executor 始终首选添加新的线程，而不进行排队。（什么意思？如果当前运行的线程小于corePoolSize，则任务根本不会存放，添加到queue中，而是直接抄家伙（thread）开始运行）
- 如果运行的线程等于或多于 corePoolSize，则 Executor 始终首选将请求加入队列，**而不添加新的线程**。
- 如果无法将请求加入队列，则创建新的线程，除非创建此线程超出 maximumPoolSize，在这种情况下，任务将被拒绝。
**先不着急举例子，因为首先需要知道queue上的三种类型。**
排队有三种通用策略：
- *直接提交。*工作队列的默认选项是 `SynchronousQueue`，它将任务直接提交给线程而不保持它们。在此，如果不存在可用于立即运行任务的线程，则试图把任务加入队列将失败，因此会构造一个新的线程。此策略可以避免在处理可能具有内部依赖性的请求集时出现锁。直接提交通常要求无界 
 maximumPoolSizes 以避免拒绝新提交的任务。当命令以超过队列所能处理的平均数连续到达时，此策略允许无界线程具有增长的可能性。
- *无界队列。*使用无界队列（例如，不具有预定义容量的 `LinkedBlockingQueue`）将导致在所有 
 corePoolSize 线程都忙时新任务在队列中等待。这样，创建的线程就不会超过 corePoolSize。（因此，maximumPoolSize 的值也就无效了。）当每个任务完全独立于其他任务，即任务执行互不影响时，适合于使用无界队列；例如，在 Web 页服务器中。这种排队可用于处理瞬态突发请求，当命令以超过队列所能处理的平均数连续到达时，此策略允许无界线程具有增长的可能性。
- *有界队列。*当使用有限的 maximumPoolSizes 时，有界队列（如 `ArrayBlockingQueue`）有助于防止资源耗尽，但是可能较难调整和控制。队列大小和最大池大小可能需要相互折衷：使用大型队列和小型池可以最大限度地降低 
 CPU 使用率、操作系统资源和上下文切换开销，但是可能导致人工降低吞吐量。如果任务频繁阻塞（例如，如果它们是 I/O 边界），则系统可能为超过您许可的更多线程安排时间。使用小型队列通常要求较大的池大小，CPU 使用率较高，但是可能遇到不可接受的调度开销，这样也会降低吞吐量。  
===============================神奇分割线==================================
到这里，该了解的理论已经够多了，可以调节的就是corePoolSize和maximumPoolSizes 这对参数还有就是BlockingQueue的选择。
**例子一：使用直接提交策略，也即SynchronousQueue。**
首先SynchronousQueue是无界的，也就是说他存数任务的能力是没有限制的，但是由于该Queue本身的特性，**在某次添加元素后必须等待其他线程取走后才能继续添加**。在这里不是核心线程便是新创建的线程，但是我们试想一样下，下面的场景。
我们使用一下参数构造ThreadPoolExecutor：
Java代码  ![收藏代码](http://dongxuan.iteye.com/images/icon_star.png)
- new ThreadPoolExecutor(  
- 2, 3, 30, TimeUnit.SECONDS,   
- new SynchronousQueue<Runnable>(),   
- new RecorderThreadFactory("CookieRecorderPool"),   
- new ThreadPoolExecutor.CallerRunsPolicy());  
![](http://static.blog.csdn.net/images/save_snippets.png)
 当核心线程已经有2个正在运行.
- 此时继续来了一个任务（A），根据前面介绍的“如果运行的线程等于或多于 corePoolSize，则 Executor 始终首选将请求加入队列，**而不添加新的线程**。”,所以A被添加到queue中。
- 又来了一个任务（B），且核心2个线程还没有忙完，OK，接下来首先尝试1中描述，但是由于使用的SynchronousQueue，所以一定无法加入进去。
- 此时便满足了上面提到的“如果无法将请求加入队列，则创建新的线程，除非创建此线程超出maximumPoolSize，在这种情况下，任务将被拒绝。”，所以必然会新建一个线程来运行这个任务。
- 暂时还可以，但是如果这三个任务都还没完成，连续来了两个任务，第一个添加入queue中，后一个呢？queue中无法插入，而线程数达到了maximumPoolSize，所以只好执行异常策略了。
所以在使用SynchronousQueue通常要求maximumPoolSize是无界的，这样就可以避免上述情况发生（如果希望限制就直接使用有界队列）。对于使用SynchronousQueue的作用jdk中写的很清楚：此策略可以避免在处理可能具有内部依赖性的请求集时出现锁。
什么意思？如果你的任务A1，A2有内部关联，A1需要先运行，那么先提交A1，再提交A2，当使用SynchronousQueue我们可以保证，A1必定先被执行，在A1么有被执行前，A2不可能添加入queue中
**例子二：使用无界队列策略，即`LinkedBlockingQueue`**
这个就拿**newFixedThreadPool**来说，根据前文提到的规则：
 写道
如果运行的线程少于 corePoolSize，则 Executor 始终首选添加新的线程，而不进行排队。
 那么当任务继续增加，会发生什么呢？
 写道
如果运行的线程等于或多于 corePoolSize，则 Executor 始终首选将请求加入队列，而不添加新的线程。
 OK，此时任务变加入队列之中了，那什么时候才会添加新线程呢？
 写道
如果无法将请求加入队列，则创建新的线程，除非创建此线程超出 maximumPoolSize，在这种情况下，任务将被拒绝。
这里就很有意思了，可能会出现无法加入队列吗？不像SynchronousQueue那样有其自身的特点，对于无界队列来说，总是可以加入的（资源耗尽，当然另当别论）。换句说，永远也不会触发产生新的线程！corePoolSize大小的线程数会一直运行，忙完当前的，就从队列中拿任务开始运行。所以要防止任务疯长，比如任务运行的实行比较长，而添加任务的速度远远超过处理任务的时间，而且还不断增加，如果任务内存大一些，不一会儿就爆了，呵呵。
可以仔细想想哈。
**例子三：有界队列，使用`ArrayBlockingQueue。`**
这个是最为复杂的使用，所以JDK不推荐使用也有些道理。与上面的相比，最大的特点便是可以防止资源耗尽的情况发生。
举例来说，请看如下构造方法：
Java代码  ![收藏代码](http://dongxuan.iteye.com/images/icon_star.png)
- new ThreadPoolExecutor(  
- 2, 4, 30, TimeUnit.SECONDS,   
- new ArrayBlockingQueue<Runnable>(2),   
- new RecorderThreadFactory("CookieRecorderPool"),   
- new ThreadPoolExecutor.CallerRunsPolicy());  
![](http://static.blog.csdn.net/images/save_snippets.png)
假设，所有的任务都永远无法执行完。
对于首先来的A,B来说直接运行，接下来，如果来了C,D，他们会被放到queu中，如果接下来再来E,F，则增加线程运行E，F。但是如果再来任务，队列无法再接受了，线程数也到达最大的限制了，所以就会使用拒绝策略来处理。
总结：
- ThreadPoolExecutor的使用还是很有技巧的。
- 使用无界queue可能会耗尽系统资源。
- 使用有界queue可能不能很好的满足性能，需要调节线程数和queue大小
- 线程数自然也有开销，所以需要根据不同应用进行调节。
通常来说对于静态任务可以归为：
- 数量大，但是执行时间很短
- 数量小，但是执行时间较长
- 数量又大执行时间又长
- 除了以上特点外，任务间还有些内在关系
