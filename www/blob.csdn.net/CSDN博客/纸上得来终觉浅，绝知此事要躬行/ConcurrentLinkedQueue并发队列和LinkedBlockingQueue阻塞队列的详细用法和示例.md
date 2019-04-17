# ConcurrentLinkedQueue并发队列和LinkedBlockingQueue阻塞队列的详细用法和示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年09月13日 09:04:03[boonya](https://me.csdn.net/boonya)阅读数：1668








参考文章：[http://blog.csdn.net/ac903919/article/details/6967728](http://blog.csdn.net/ac903919/article/details/6967728)，[http://www.cnblogs.com/linjiqin/archive/2013/05/30/3108188.html](http://www.cnblogs.com/linjiqin/archive/2013/05/30/3108188.html)


### 1.LinkedBlockingQueue<E>：java.util.concurrent

API中的解释：



**[java]**[view plain](http://blog.csdn.net/ac903919/article/details/6967728#)[copy](http://blog.csdn.net/ac903919/article/details/6967728#)

- publicclass LinkedBlockingQueue<E> extends AbstractQueue<E> implements BlockingQueue<E>, Serializable  


一个基于已链接节点的、范围任意的 [blocking queue](http://blog.csdn.net/java/util/concurrent/BlockingQueue.html)。此队列按 FIFO（先进先出）排序元素。队列的*头部* 是在队列中时间最长的元素。队列的*尾部* 是在队列中时间最短的元素。新元素插入到队列的尾部，并且队列检索操作会获得位于队列头部的元素。链接队列的吞吐量通常要高于基于数组的队列，但是在大多数并发应用程序中，其可预知的性能要低。可选的容量范围构造方法参数作为防止队列过度扩展的一种方法。如果未指定容量，则它等于[`Integer.MAX_VALUE`](http://blog.csdn.net/java/lang/Integer.html#MAX_VALUE)。除非插入节点会使队列超出容量，否则每次插入后会动态地创建链接节点。

适用阻塞队列的好处：多线程操作共同的队列时不需要额外的同步，另外就是队列会自动平衡负载，即那边（生产与消费两边）处理快了就会被阻塞掉，从而减少两边的处理速度差距。



### 2.ConcurrentLinkedQueue<E>:java.util.concurrent

API中的解释：



**[java]**[view plain](http://blog.csdn.net/ac903919/article/details/6967728#)[copy](http://blog.csdn.net/ac903919/article/details/6967728#)

- publicclass ConcurrentLinkedQueue<E>extends AbstractQueue<E>implements Queue<E>, Serializable  


一个基于链接节点的、无界的、线程安全的[队列](http://blog.csdn.net/java/util/Queue.html)。此队列按照 FIFO（先进先出）原则对元素进行排序。队列的*头部* 是队列中时间最长的元素。队列的*尾部* 是队列中时间最短的元素。新的元素插入到队列的尾部，队列检索操作从队列头部获得元素。当许多线程共享访问一个公共
 collection 时，ConcurrentLinkedQueue 是一个恰当的选择。此队列不允许 null 元素。



### 3.使用Executor和ExecutorService线程池处理并发任务

并发编程中的一种编程方式是把任务拆分为一些的小任务，即Runnable,然后再提交给一个Executor执行，Executor.execute(Runnable).Executor在执行时适用内部的线程池完成操作。Executor在执行时使用内部的线程池完成操作。



#### 3.1创建固定数目或可缓存线程池

```java
Executor executor = Executors.newFixedThreadPool(10);  //创建固定数目线程的线程池

Executor executor = Executors.newCachedThreadPool();  //创建一个可缓存的线程池，调用execute将重用以前构造的线程（如线程可用）。如果现有线程没有可用的。则创建一个新的线程并添加到池中。
```



#### 3.2ExecutorService与生命周期

ExecutorService扩展了Executor并添加了一些生命周期管理的方法。一个Executor的生命周期有三种状态，运行，关闭，终止。Executor创建时出于运行状态。当调用ExecutorService.shutdown()后，出于关闭状态，isShutdown方法返回true。这时，不应该再想Executor中添加任务，所有已添加的任务执行完毕后，Executor出于终止状态，IsTerminated（）返回true.如果Executor处于关闭状态，往Executor提交任务会抛出unchecked
 exception RejectedExecutionException.

API中的解释：



**[java]**[view plain](http://blog.csdn.net/ac903919/article/details/6967728#)[copy](http://blog.csdn.net/ac903919/article/details/6967728#)

- publicinterface ExecutorService extends Executor  


[`Executor`](http://blog.csdn.net/java/util/concurrent/Executor.html) 提供了管理终止的方法，以及可为跟踪一个或多个异步任务执行状况而生成[`Future`](http://blog.csdn.net/java/util/concurrent/Future.html)
 的方法。 可以关闭 ExecutorService，这将导致其停止接受新任务。关闭后，执行程序将最后终止，这时没有任务在执行，也没有任务在等待执行，并且无法提交新任务。通过创建并返回一个可用于取消执行和/或等待完成的[`Future`](http://blog.csdn.net/java/util/concurrent/Future.html)，方法submit
 扩展了基本方法 [`Executor.execute(java.lang.Runnable)`](http://blog.csdn.net/java/util/concurrent/Executor.html#execute%28java.lang.Runnable%29)。方法 invokeAny 和 
invokeAll 是批量执行的最常用形式，它们执行任务集合，然后等待至少一个，或全部任务完成（可使用 [`ExecutorCompletionService`](http://blog.csdn.net/java/util/concurrent/ExecutorCompletionService.html) 类来编写这些方法的自定义变体）。[`Executors`](http://blog.csdn.net/java/util/concurrent/Executors.html) 类提供了用于此包中所提供的执行程序服务的工厂方法。



void **shutdown**():启动一次顺序关闭，执行以前提交的任务，但不接受新任务。如果已经关闭，则调用没有其他作用。


```
**[submit](http://blog.csdn.net/java/util/concurrent/ExecutorService.html#submit%28java.lang.Runnable%29)**([Runnable](http://blog.csdn.net/java/lang/Runnable.html)
 task)
```
 提交一个 Runnable 任务用于执行，并返回一个表示该任务的 Future。

### 4.ConcurrentLinkedQueue并发队列和LinkedBlockingQueue阻塞队列用法



在Java多线程应用中，队列的使用率很高，多数生产消费模型的首选数据结构就是队列(先进先出)。Java提供的线程安全的Queue可以分为阻塞队列和非阻塞队列，其中阻塞队列的典型例子是BlockingQueue，非阻塞队列的典型例子是ConcurrentLinkedQueue，在实际应用中要根据实际需要选用阻塞队列或者非阻塞队列。

注：什么叫线程安全？这个首先要明确。线程安全就是说多线程访问同一代码，不会产生不确定的结果。

**并行和并发区别**

1、并行是指两者同时执行一件事，比如赛跑，两个人都在不停的往前跑；

2、并发是指资源有限的情况下，两者交替轮流使用资源，比如一段路(单核CPU资源)同时只能过一个人，A走一段后，让给B，B用完继续给A ，交替使用，目的是提高效率

**LinkedBlockingQueue**
**由于LinkedBlockingQueue实现是线程安全的，实现了先进先出等特性，是作为生产者消费者的首选**，LinkedBlockingQueue 可以指定容量，也可以不指定，不指定的话，默认最大是Integer.MAX_VALUE，其中主要用到put和take方法，put方法在队列满的时候会阻塞直到有队列成员被消费，take方法在队列空的时候会阻塞，直到有队列成员被放进来。

```java
package cn.thread;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * 多线程模拟实现生产者／消费者模型
 *  
 * @author 林计钦
 * @version 1.0 2013-7-25 下午05:23:11
 */
public class BlockingQueueTest2 {
    /**
     * 
     * 定义装苹果的篮子
     * 
     */
    public class Basket {
        // 篮子，能够容纳3个苹果
        BlockingQueue<String> basket = new LinkedBlockingQueue<String>(3);

        // 生产苹果，放入篮子
        public void produce() throws InterruptedException {
            // put方法放入一个苹果，若basket满了，等到basket有位置
            basket.put("An apple");
        }

        // 消费苹果，从篮子中取走
        public String consume() throws InterruptedException {
            // take方法取出一个苹果，若basket为空，等到basket有苹果为止(获取并移除此队列的头部)
            return basket.take();
        }
    }

    // 定义苹果生产者
    class Producer implements Runnable {
        private String instance;
        private Basket basket;

        public Producer(String instance, Basket basket) {
            this.instance = instance;
            this.basket = basket;
        }

        public void run() {
            try {
                while (true) {
                    // 生产苹果
                    System.out.println("生产者准备生产苹果：" + instance);
                    basket.produce();
                    System.out.println("!生产者生产苹果完毕：" + instance);
                    // 休眠300ms
                    Thread.sleep(300);
                }
            } catch (InterruptedException ex) {
                System.out.println("Producer Interrupted");
            }
        }
    }

    // 定义苹果消费者
    class Consumer implements Runnable {
        private String instance;
        private Basket basket;

        public Consumer(String instance, Basket basket) {
            this.instance = instance;
            this.basket = basket;
        }

        public void run() {
            try {
                while (true) {
                    // 消费苹果
                    System.out.println("消费者准备消费苹果：" + instance);
                    System.out.println(basket.consume());
                    System.out.println("!消费者消费苹果完毕：" + instance);
                    // 休眠1000ms
                    Thread.sleep(1000);
                }
            } catch (InterruptedException ex) {
                System.out.println("Consumer Interrupted");
            }
        }
    }

    public static void main(String[] args) {
        BlockingQueueTest2 test = new BlockingQueueTest2();

        // 建立一个装苹果的篮子
        Basket basket = test.new Basket();

        ExecutorService service = Executors.newCachedThreadPool();
        Producer producer = test.new Producer("生产者001", basket);
        Producer producer2 = test.new Producer("生产者002", basket);
        Consumer consumer = test.new Consumer("消费者001", basket);
        service.submit(producer);
        service.submit(producer2);
        service.submit(consumer);
        // 程序运行5s后，所有任务停止
//        try {
//            Thread.sleep(1000 * 5);
//        } catch (InterruptedException e) {
//            e.printStackTrace();
//        }
//        service.shutdownNow();
    }

}
```

**ConcurrentLinkedQueue**

ConcurrentLinkedQueue是Queue的一个安全实现．Queue中元素按FIFO原则进行排序．采用CAS操作，来保证元素的一致性。

LinkedBlockingQueue是一个线程安全的阻塞队列，它实现了BlockingQueue接口，BlockingQueue接口继承自java.util.Queue接口，并在这个接口的基础上增加了take和put方法，这两个方法正是队列操作的阻塞版本。



```java
package cn.thread;

import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ConcurrentLinkedQueueTest {
    private static ConcurrentLinkedQueue<Integer> queue = new ConcurrentLinkedQueue<Integer>();
    private static int count = 2; // 线程个数
    //CountDownLatch，一个同步辅助类，在完成一组正在其他线程中执行的操作之前，它允许一个或多个线程一直等待。
    private static CountDownLatch latch = new CountDownLatch(count);

    public static void main(String[] args) throws InterruptedException {
        long timeStart = System.currentTimeMillis();
        ExecutorService es = Executors.newFixedThreadPool(4);
        ConcurrentLinkedQueueTest.offer();
        for (int i = 0; i < count; i++) {
            es.submit(new Poll());
        }
        latch.await(); //使得主线程(main)阻塞直到latch.countDown()为零才继续执行
        System.out.println("cost time " + (System.currentTimeMillis() - timeStart) + "ms");
        es.shutdown();
    }
    
    /**
     * 生产
     */
    public static void offer() {
        for (int i = 0; i < 100000; i++) {
            queue.offer(i);
        }
    }


    /**
     * 消费
     *  
     * @author 林计钦
     * @version 1.0 2013-7-25 下午05:32:56
     */
    static class Poll implements Runnable {
        public void run() {
            // while (queue.size()>0) {
            while (!queue.isEmpty()) {
                System.out.println(queue.poll());
            }
            latch.countDown();
        }
    }
}
```


**运行结果：costtime 2360ms**

**改用while (queue.size()>0)后运行结果：cost time 46422ms**

**结果居然相差那么大，看了下ConcurrentLinkedQueue的API原来.size()是要遍历一遍集合的，难怪那么慢，所以尽量要避免用size而改用isEmpty().**

总结：在单位缺乏性能测试下，对自己的编程要求更加要严格，特别是在生产环境下更是要小心谨慎。





