# Java 多线程与线程池 Thread弊端与Executor存在问题 及解决方法

 																				2019年02月24日 22:08:53 					[小鹅鹅](https://me.csdn.net/asd136912) 						阅读数：14 						 																															

 									

​                   					 					版权声明：本文为博主原创文章，转载请附http://blog.csdn.net/asd136912。					https://blog.csdn.net/asd136912/article/details/87908629				



### 文章目录

- - [Java 多线程与线程池 Thread弊端与Executor存在问题 及解决方法](https://blog.csdn.net/asd136912/article/details/87908629#Java__ThreadExecutor__1)

  - - [1. 使用Thread弊端](https://blog.csdn.net/asd136912/article/details/87908629#1_Thread_3)

    - [2. 线程池背景及优势](https://blog.csdn.net/asd136912/article/details/87908629#2__21)

    - [3. Executor解析](https://blog.csdn.net/asd136912/article/details/87908629#3_Executor_44)

    - - [3.1 线程池原理](https://blog.csdn.net/asd136912/article/details/87908629#31__46)
      - [3.2 配置线程池](https://blog.csdn.net/asd136912/article/details/87908629#32__146)
      - [3.3 优雅关闭线程池](https://blog.csdn.net/asd136912/article/details/87908629#33__157)

    - [4. Executor存在问题](https://blog.csdn.net/asd136912/article/details/87908629#4_Executor_189)

    - [解决方法](https://blog.csdn.net/asd136912/article/details/87908629#_257)

    - [5. Ref](https://blog.csdn.net/asd136912/article/details/87908629#5_Ref_293)



## Java 多线程与线程池 Thread弊端与Executor存在问题 及解决方法

### 1. 使用Thread弊端

```java
new Thread(new Runnable() {

    @Override
    public void run() {
        // TODO Auto-generated method stub
        }
    }
).start();
12345678
```

Thread的弊端如下：

1. 每次new Thread新建对象性能差。
2. 线程缺乏统一管理，可能无限制新建线程，相互之间竞争，及可能占用过多系统资源导致死机或oom。
3. 缺乏更多功能，如定时执行、定期执行、线程中断。

### 2. 线程池背景及优势

我们使用线程的时候就去创建一个线程，这样实现起来非常简便，但是就会有一个问题：

> 如果并发的线程数量很多，并且每个线程都是执行一个时间很短的任务就结束了，这样频繁创建线程就会大大降低系统的效率，因为频繁创建线程和销毁线程需要时间。

那么有没有一种办法使得线程可以复用，就是执行完一个任务，并不被销毁，而是可以继续执行其他的任务？

在Java中可以通过线程池来达到这样的效果。

什么时候使用线程池？

- 单个任务处理时间比较短
- 需要处理的任务数量很大

使用线程池的好处:

- 降低资源消耗。通过重复利用已创建的线程降低线程创建和销毁造成的消耗。
- 提高响应速度。当任务到达时，任务可以不需要的等到线程创建就能立即执行。
- 提高线程的可管理性。线程是稀缺资源，如果无限制的创建，不仅会消耗系统资源，还会降低系统的稳定性，使用线程池可以进行统一的分配，调优和监控。

### 3. Executor解析

#### 3.1 线程池原理

Java通过Executors提供四种线程池，分别为：

- newCachedThreadPool创建一个可缓存线程池，如果线程池长度超过处理需要，可灵活回收空闲线程，若无可回收，则新建线程。
- newFixedThreadPool 创建一个定长线程池，可控制线程最大并发数，超出的线程会在队列中等待。
- newScheduledThreadPool 创建一个定长线程池，支持定时及周期性任务执行。
- newSingleThreadExecutor 创建一个单线程化的线程池，它只会用唯一的工作线程来执行任务，保证所有任务按照指定顺序(FIFO, LIFO, 优先级)执行。
- 

其实看这几种方式创建的源码就会发现：

```
    public static ExecutorService newCachedThreadPool() {
        return new ThreadPoolExecutor(0, Integer.MAX_VALUE,
                                      60L, TimeUnit.SECONDS,
                                      new SynchronousQueue<Runnable>());
    }
12345
```

实际上还是利用 `ThreadPoolExecutor` 类实现的。

所以我们重点来看下 `ThreadPoolExecutor` 是怎么玩的。

首先是创建线程的 api：

```
ThreadPoolExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, TimeUnit unit, BlockingQueue<Runnable> workQueue, RejectedExecutionHandler handler)
1
```

这几个核心参数的作用：

- `corePoolSize` 为线程池的基本大小。
- `maximumPoolSize` 为线程池最大线程大小。
- `keepAliveTime` 和 `unit` 则是线程空闲后的存活时间。
- `workQueue` 用于存放任务的阻塞队列。
- `handler` 当队列和最大线程池都满了之后的饱和策略。

了解了这几个参数再来看看实际的运用。

通常我们都是使用:

```
threadPool.execute(new Job());
1
```

这样的方式来提交一个任务到线程池中，所以核心的逻辑就是 `execute()` 函数了。

在具体分析之前先了解下线程池中所定义的状态，这些状态都和线程的执行密切相关：

ThreadPoolExecutor继承自AbstractExecutorService，也是实现了ExecutorService接口。

```
private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));
private static final int COUNT_BITS = Integer.SIZE - 3;
private static final int CAPACITY   = (1 << COUNT_BITS) - 1;

// runState is stored in the high-order bits
private static final int RUNNING    = -1 << COUNT_BITS;
private static final int SHUTDOWN   =  0 << COUNT_BITS;
private static final int STOP       =  1 << COUNT_BITS;
private static final int TIDYING    =  2 << COUNT_BITS;
private static final int TERMINATED =  3 << COUNT_BITS;
12345678910
```

`ctl`是对线程池的运行状态和线程池中有效线程的数量进行控制的一个字段， 它**包含两部分的信息: 线程池的运行状态 (runState) 和线程池内有效线程的数量 (workerCount)**，这里可以看到，使用了Integer类型来保存，高3位保存runState，低29位保存workerCount。COUNT_BITS 就是29，CAPACITY就是1左移29位减1（29个1），这个常量表示workerCount的上限值，大约是5亿。

下面再介绍下线程池的运行状态. 线程池一共有五种状态, 分别是:

1. **RUNNING** ：能接受新提交的任务，并且也能处理阻塞队列中的任务；

2. **SHUTDOWN**：关闭状态，不再接受新提交的任务，但却可以继续处理阻塞队列中已保存的任务。在线程池处于 RUNNING 状态时，调用 shutdown()方法会使线程池进入到该状态。（finalize() 方法在执行过程中也会调用shutdown()方法进入该状态）；

3. **STOP**：不能接受新任务，也不处理队列中的任务，会中断正在处理任务的线程。在线程池处于 RUNNING 或 SHUTDOWN 状态时，调用 shutdownNow() 方法会使线程池进入到该状态；

4. **TIDYING**：如果所有的任务都已终止了，workerCount (有效线程数) 为0，线程池进入该状态后会调用 terminated() 方法进入TERMINATED 状态。

5. **TERMINATED**：在terminated() 方法执行完后进入该状态，默认terminated()方法中什么也没有做。

   进入TERMINATED的条件如下：

   - 线程池不是RUNNING状态；
   - 线程池状态不是TIDYING状态或TERMINATED状态；
   - 如果线程池状态是SHUTDOWN并且workerQueue为空；
   - workerCount为0；
   - 设置TIDYING状态成功。
      ![img](https://img-blog.csdnimg.cn/20190224220821676.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

然后看看 `execute()` 方法是如何处理的：

![img](https://img-blog.csdnimg.cn/20190224220730789.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

1. 获取当前线程池的状态。
2. 当前线程数量小于 coreSize 时创建一个新的线程运行。
3. 如果当前线程处于运行状态，并且写入阻塞队列成功。
4. 双重检查，再次获取线程状态；如果线程状态变了（非运行状态）就需要从阻塞队列移除任务，并尝试判断线程是否全部执行完毕。同时执行拒绝策略。
5. 如果当前线程池为空就新创建一个线程并执行。
6. 如果在第三步的判断为非运行状态，尝试新建线程，如果失败则执行拒绝策略。

《聊聊并发》的一张图来描述这个流程：

![img](https://img-blog.csdnimg.cn/20190224220754419.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)

#### 3.2 配置线程池

流程聊完了再来看看上文提到了几个核心参数应该如何配置呢？

有一点是肯定的，线程池肯定是不是越大越好。

通常我们是需要根据这批任务执行的性质来确定的。

- IO 密集型任务：由于线程并不是一直在运行，所以可以尽可能的多配置线程，比如 CPU 个数 * 2
- CPU 密集型任务（大量复杂的运算）应当分配较少的线程，比如 CPU 个数相当的大小。

当然这些都是经验值，最好的方式还是根据实际情况测试得出最佳配置。

#### 3.3 优雅关闭线程池

有运行任务自然也有关闭任务，从上文提到的 5 个状态就能看出如何来关闭线程池。

其实无非就是两个方法 `shutdown()/shutdownNow()`。

但他们有着重要的区别：

- `shutdown()` 执行后停止接受新任务，会把队列的任务执行完毕。
- `shutdownNow()` 也是停止接受新任务，但会中断所有的任务，将线程池状态变为 stop。

> 两个方法都会中断线程，用户可自行判断是否需要响应中断。

`shutdownNow()` 要更简单粗暴，可以根据实际场景选择不同的方法。

我通常是按照以下方式关闭线程池的：

```java
        long start = System.currentTimeMillis();
        for (int i = 0; i <= 5; i++) {
            pool.execute(new Job());
        }

        pool.shutdown();

        while (!pool.awaitTermination(1, TimeUnit.SECONDS)) {
            LOGGER.info("线程还在执行。。。");
        }
        long end = System.currentTimeMillis();
        LOGGER.info("一共处理了【{}】", (end - start));
123456789101112
```

`pool.awaitTermination(1, TimeUnit.SECONDS)` 会每隔一秒钟检查一次是否执行完毕（状态为 `TERMINATED`），当从 while 循环退出时就表明线程池已经完全终止了。

### 4. Executor存在问题

在阿里巴巴Java开发手册中明确指出，**不允许**使用Executors创建线程池。

![img](https://img-blog.csdnimg.cn/2019022422081223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FzZDEzNjkxMg==,size_16,color_FFFFFF,t_70)
 在阿里巴巴Java开发手册中提到，使用Executors创建线程池可能会导致OOM(OutOfMemory ,内存溢出)，但是并没有说明为什么，那么接下来我们就来看一下到底为什么不允许使用Executors？

```java
public class ExecutorsDemo {
    private static ExecutorService executor = Executors.newFixedThreadPool(15);
    public static void main(String[] args) {
        for (int i = 0; i < Integer.MAX_VALUE; i++) {
            executor.execute(new SubThread());
        }
    }
}

class SubThread implements Runnable {
    @Override
    public void run() {
        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            //do nothing
        }
    }
}
12345678910111213141516171819
```

通过指定JVM参数：`-Xmx8m -Xms8m` 运行以上代码，会抛出OOM:

```
Exception in thread "main" java.lang.OutOfMemoryError: GC overhead limit exceeded
    at java.util.concurrent.LinkedBlockingQueue.offer(LinkedBlockingQueue.java:416)
    at java.util.concurrent.ThreadPoolExecutor.execute(ThreadPoolExecutor.java:1371)
    at com.hollis.ExecutorsDemo.main(ExecutorsDemo.java:16)
1234
```

以上代码指出，`ExecutorsDemo.java`的第16行，就是代码中的`executor.execute(new SubThread());`

通过上面的例子，我们知道了`Executors`创建的线程池存在OOM的风险，那么到底是什么原因导致的呢？我们需要深入`Executors`的源码来分析一下。

其实，在上面的报错信息中，我们是可以看出蛛丝马迹的，在以上的代码中其实已经说了，真正的导致OOM的其实是`LinkedBlockingQueue.offer`方法。

```text
Exception in thread "main" java.lang.OutOfMemoryError: GC overhead limit exceeded
    at java.util.concurrent.LinkedBlockingQueue.offer(LinkedBlockingQueue.java:416)
    at java.util.concurrent.ThreadPoolExecutor.execute(ThreadPoolExecutor.java:1371)
    at com.hollis.ExecutorsDemo.main(ExecutorsDemo.java:16)
1234
```

如果翻看代码的话，也可以发现，其实底层确实是通过`LinkedBlockingQueue`实现的：

```java
public static ExecutorService newFixedThreadPool(int nThreads) {
        return new ThreadPoolExecutor(nThreads, nThreads,
                                      0L, TimeUnit.MILLISECONDS,
                                      new LinkedBlockingQueue<Runnable>());
1234
```

如果对Java中的阻塞队列有所了解的话，看到这里或许就能够明白原因了。

Java中的`BlockingQueue`主要有两种实现，分别是`ArrayBlockingQueue` 和 `LinkedBlockingQueue`。

`ArrayBlockingQueue`是一个用数组实现的有界阻塞队列，必须设置容量。

`LinkedBlockingQueue`是一个用链表实现的有界阻塞队列，容量可以选择进行设置，不设置的话，将是一个无边界的阻塞队列，最大长度为`Integer.MAX_VALUE`。

这里的问题就出在：**不设置的话，将是一个无边界的阻塞队列，最大长度为Integer.MAX_VALUE。**也就是说，如果我们不设置`LinkedBlockingQueue`的容量的话，其默认容量将会是`Integer.MAX_VALUE`。

而`newFixedThreadPool`中创建`LinkedBlockingQueue`时，并未指定容量。此时，`LinkedBlockingQueue`就是一个无边界队列，对于一个无边界队列来说，是可以不断的向队列中加入任务的，这种情况下就有可能因为任务过多而导致内存溢出问题。

上面提到的问题主要体现在`newFixedThreadPool`和`newSingleThreadExecutor`两个工厂方法上，并不是说`newCachedThreadPool`和`newScheduledThreadPool`这两个方法就安全了，这两种方式创建的最大线程数可能是`Integer.MAX_VALUE`，而创建这么多线程，必然就有可能导致OOM。

### 解决方法

避免使用Executors创建线程池，主要是避免使用其中的默认实现，那么我们可以自己直接调用`ThreadPoolExecutor`的构造函数来自己创建线程池。在创建的同时，给`BlockQueue`指定容量就可以了。

```java
private static ExecutorService executor = new ThreadPoolExecutor(10, 10,
        60L, TimeUnit.SECONDS,
        new ArrayBlockingQueue(10));
123
```

这种情况下，一旦提交的线程数超过当前可用线程数时，就会抛出`java.util.concurrent.RejectedExecutionException`，这是因为当前线程池使用的队列是有边界队列，队列已经满了便无法继续处理新的请求。但是异常（Exception）总比发生错误（Error）要好。

除了自己定义`ThreadPoolExecutor`外。还有其他方法。这个时候第一时间就应该想到开源类库，如apache和guava等。

推荐使用guava提供的ThreadFactoryBuilder来创建线程池。

```java
public class ExecutorsDemo {

    private static ThreadFactory namedThreadFactory = new ThreadFactoryBuilder()
        .setNameFormat("demo-pool-%d").build();

    private static ExecutorService pool = new ThreadPoolExecutor(5, 200,
        0L, TimeUnit.MILLISECONDS,
        new LinkedBlockingQueue<Runnable>(1024), namedThreadFactory, new ThreadPoolExecutor.AbortPolicy());

    public static void main(String[] args) {

        for (int i = 0; i < Integer.MAX_VALUE; i++) {
            pool.execute(new SubThread());
        }
    }
}
12345678910111213141516
```

通过上述方式创建线程时，不仅可以避免OOM的问题，还可以自定义线程名称，更加方便的出错的时候溯源。

### 5. Ref

1. <https://juejin.im/entry/58fada5d570c350058d3aaad>
2. <https://blog.csdn.net/u011974987/article/details/51027795>
3. <https://www.hollischuang.com/archives/2888>
4. <https://segmentfault.com/a/1190000015808897>