# 并发工具箱 concurrent包的原理分析以及使用



# 1、java.util.concurrent 包下的类分类图

##  

![J.U.C](https://img-blog.csdn.net/20170301212847989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2J3ang=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- locks部分：显式锁(互斥锁和速写锁)相关；
- atomic部分：原子变量类相关，是构建非阻塞算法的基础；
- executor部分：线程池相关；
- collections部分：并发容器相关；
- tools部分：同步工具相关，如信号量、闭锁、栅栏等功能；

## 1、1 collections部分:

### 1、1、1  BlockingQueue

　　BlockingQueue为接口，如果要是他，需要使用实现他的子类；

　　BlockingQueue的子类包括'

　　　　ArrayBlockingQueue;

　　　　DelayQueue;

　　　　LinkedBlockingQueue;

　　　　SynchronousQueue;

　　　　PriorirtyBlockingQueue;

　　　　TransferQueue;

　　

### BlockingQueue 用法

BlockingQueue 通常用于一个线程生产对象，而另外一个线程消费这些对象的场景。下图是对这个原理的阐述：

[![5401760-27d2be9af3d08a19](http://incdn1.b0.upaiyun.com/2017/09/70941e8446e6c4e021e98b3b1ed65968.png)](http://www.importnew.com/26461.html/5401760-27d2be9af3d08a19)

**一个线程往里边放，另外一个线程从里边取的一个 BlockingQueue。**

一个线程将会持续生产新对象并将其插入到队列之中，直到队列达到它所能容纳的临界点。也就是说，它是有限的。如果该阻塞队列到达了其临界点，负责生产的线程将会在往里边插入新对象时发生阻塞。它会一直处于阻塞之中，直到负责消费的线程从队列中拿走一个对象。负责消费的线程将会一直从该阻塞队列中拿出对象。如果消费线程尝试去从一个空的队列中提取对象的话，这个消费线程将会处于阻塞之中，直到一个生产线程把一个对象丢进队列。

### BlockingQueue 的方法

BlockingQueue 具有 4 组不同的方法用于插入、移除以及对队列中的元素进行检查。如果请求的操作不能得到立即执行的话，每个方法的表现也不同。这些方法如下：

[![QQ201709021702232x](http://incdn1.b0.upaiyun.com/2017/09/da0e4b92bffc70c760fce065764bf117.png)](http://www.importnew.com/26461.html/qq201709021702232x)

四组不同的行为方式解释：

**抛异常**：如果试图的操作无法立即执行，抛一个异常。
**特定值**：如果试图的操作无法立即执行，返回一个特定的值(常常是 true / false)。
**阻塞**：如果试图的操作无法立即执行，该方法调用将会发生阻塞，直到能够执行。
**超时**：如果试图的操作无法立即执行，该方法调用将会发生阻塞，直到能够执行，但等待时间不会超过给定值。返回一个特定值以告知该操作是否成功(典型的是 true / false)。

无法向一个 BlockingQueue 中插入 null。如果你试图插入 null，BlockingQueue 将会抛出一个 NullPointerException。

可以访问到 BlockingQueue  中的所有元素，而不仅仅是开始和结束的元素。比如说，你将一个对象放入队列之中以等待处理，但你的应用想要将其取消掉。那么你可以调用诸如  remove(o)  方法来将队列之中的特定对象进行移除。但是这么干效率并不高(译者注：基于队列的数据结构，获取除开始或结束位置的其他对象的效率不会太高)，因此你尽量不要用这一类的方法，除非你确实不得不那么做。

### Java 中使用 BlockingQueue 的例子

这里是一个 Java 中使用 BlockingQueue 的示例。本示例使用的是 BlockingQueue 接口的 ArrayBlockingQueue 实现。

首先，BlockingQueueExample 类分别在两个独立的线程中启动了一个 Producer 和 一个 Consumer。

Producer 向一个共享的 BlockingQueue 中注入字符串，而 Consumer 则会从中把它们拿出来。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class BlockingQueueExample { 
 
    public static void main(String[] args) throws Exception { 
 
        BlockingQueue queue = new ArrayBlockingQueue(1024); 
 
        Producer producer = new Producer(queue); 
        Consumer consumer = new Consumer(queue); 
 
        new Thread(producer).start(); 
        new Thread(consumer).start(); 
 
        Thread.sleep(4000); 
    } 
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

以下是 Producer 类。注意它在每次 put() 调用时是如何休眠一秒钟的。这将导致 Consumer 在等待队列中对象的时候发生阻塞。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class Producer implements Runnable{ 
 
    protected BlockingQueue queue = null; 
 
    public Producer(BlockingQueue queue) { 
        this.queue = queue; 
    } 
 
    public void run() { 
        try { 
            queue.put("1"); //无需考虑安全问题 直接使用
            Thread.sleep(1000); 
            queue.put("2"); 
            Thread.sleep(1000); 
            queue.put("3"); 
        } catch (InterruptedException e) { 
            e.printStackTrace(); 
        } 
    } 
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

 

以下是 Consumer 类。它只是把对象从队列中抽取出来，然后将它们打印到 System.out。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class Consumer implements Runnable{ 
 
    protected BlockingQueue queue = null; 
 
    public Consumer(BlockingQueue queue) { 
        this.queue = queue; 
    } 
 
    public void run() { 
        try { 
            System.out.println(queue.take()); 
            System.out.println(queue.take()); 
            System.out.println(queue.take()); 
        } catch (InterruptedException e) { 
            e.printStackTrace(); 
        } 
    } 
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

## 数组阻塞队列 ArrayBlockingQueue

　　ArrayBlockingQueue 类实现了 BlockingQueue 接口。

　　ArrayBlockingQueue 是一个有界的阻塞队列，其内部实现是将对象放到一个数组里。有界也就意味着，它不能够存储无限多数量的元素。它有一个同一时间能够存储元素数量的上限。

　　你可以在对其初始化的时候设定这个上限，但之后就无法对这个上限进行修改了(译者注：因为它是基于数组实现的，也就具有数组的特性：一旦初始化，大小就无法修改)。

　　‘ArrayBlockingQueue 内部以 FIFO(先进先出)的顺序对元素进行存储。队列中的头元素在所有元素之中是放入时间最久的那个，而尾元素则是最短的那个。

　　ArrayBlockingQueue的使用代码案例见上例代码。

## 4. 延迟队列 DelayQueue

DelayQueue 实现了 BlockingQueue 接口。DelayQueue 对元素进行持有直到一个特定的延迟到期。注入其中的元素必须实现 java.util.concurrent.Delayed 接口，该接口定义：

```
public interface Delayed extends Comparable<Delayed< { 
 
 public long getDelay(TimeUnit timeUnit);  //注意此处的TimeUnit的使用
 
}
```

 

DelayQueue 将会在每个元素的 getDelay() 方法返回的值的时间段之后才释放掉该元素。如果返回的是 0 或者负值，延迟将被认为过期，该元素将会在 DelayQueue 的下一次 take 被调用的时候被释放掉。

也就是说通过getDelay返回元素到期时间，只有元素在队列中存在的时间超过该时间后，才可以在延迟队列取出该对象；

传递给 getDelay 方法的 getDelay 实例是一个枚举类型，它表明了将要延迟的时间段。

TimeUnit 枚举将会取以下值：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
DAYS 
HOURS 
MINUTES 
SECONDS 
MILLISECONDS 
MICROSECONDS 
NANOSECONDS
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

正如你所看到的，Delayed 接口也继承了 java.lang.Comparable 接口，这也就意味着 Delayed 对象之间可以进行对比。这个在对 DelayQueue 队列中的元素进行添加时排序，因此它们可以根据过期时间进行有序释放。以下是使用 DelayQueue 的例子：

 

DelayQueue的两个应用案例：【注意该案例中用到了 concrrent 包下的 tools 下的  countDownLatch 如果对该部分不理解 先看 1.2 部门的 Tools 】

 

## 1、2  Tools部分:

## 　　CountDownLatch用法

　　CountDownLatch类位于java.util.concurrent包下，利用它可以实现类似计数器的功能。比如有一个任务A，它要等待其他4个任务执行完毕之后才能执行，此时就可以利用CountDownLatch来实现这种功能了。

　　通俗一点的说 就是 使用CountDownLatch 时  几个线程使用同一个CountDownLatch对象 有一个线程调用wait方法  其他线程使用countDown()方法，当最后计数器减到0的时候，wait调用处 继续往下执行；

　　CountDownLatch类只提供了一个构造器：

 

```
public CountDownLatch(int count) {  };  //参数count为计数值
```

 

然后下面这3个方法是CountDownLatch类中最重要的方法：

```
public void await() throws InterruptedException { };   //调用await()方法的线程会被挂起，它会等待直到count值为0才继续执行
public boolean await(long timeout, TimeUnit unit) throws InterruptedException { };  //和await()类似，只不过等待一定的时间后count值还没变为0的话就会继续执行
public void countDown() { };  //将count值减1
```

下面看使用范例：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class Test {
     public static void main(String[] args) {   
         final CountDownLatch latch = new CountDownLatch(2);
 
         new Thread(){
             public void run() {
                 try {
                     System.out.println("子线程"+Thread.currentThread().getName()+"正在执行");
                    Thread.sleep(3000);
                    System.out.println("子线程"+Thread.currentThread().getName()+"执行完毕");
                    latch.countDown();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
             };
         }.start();
 
         new Thread(){
             public void run() {
                 try {
                     System.out.println("子线程"+Thread.currentThread().getName()+"正在执行");
                     Thread.sleep(3000);
                     System.out.println("子线程"+Thread.currentThread().getName()+"执行完毕");
                     latch.countDown();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
             };
         }.start();
 
         try {
             System.out.println("等待2个子线程执行完毕...");
            latch.await();
            System.out.println("2个子线程已经执行完毕");
            System.out.println("继续执行主线程");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
     }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

执行结果；

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
线程Thread-0正在执行
线程Thread-1正在执行
等待2个子线程执行完毕...
线程Thread-0执行完毕
线程Thread-1执行完毕
2个子线程已经执行完毕
继续执行主线程
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## CyclicBarrier用法

字面意思回环栅栏，通过它可以实现让一组线程等待至某个状态之后再全部同时执行。叫做回环是因为当所有等待线程都被释放以后，CyclicBarrier可以被重用。我们暂且把这个状态就叫做barrier，当调用await()方法之后，线程就处于barrier了。

通俗的讲 就是 使用 CyclicBrarrier 几个线程在执行到需要进行栅栏的地方时 调用 CyclicBrarrier 对象的 wait 方法 然后当所有的对象都执行到栅栏的地方后 统一进行放行 往后执行；

CyclicBarrier类位于java.util.concurrent包下，CyclicBarrier提供2个构造器：

```
public CyclicBarrier(int parties, Runnable barrierAction) {
}
 
public CyclicBarrier(int parties) {
}
```

参数parties指让多少个线程或者任务等待至barrier状态；

参数barrierAction为当这些线程都达到barrier状态时会执行的内容；其实就是 当所有线程到达栅栏后，如果需要执行在放行前需要执行一个其他操作，可以实现runnable的run方法，此时会调用一个线程执行run方法；

然后CyclicBarrier中最重要的方法就是await方法，它有2个重载版本：

```
public int await() throws InterruptedException, BrokenBarrierException { };
public int await(long timeout, TimeUnit unit)throws InterruptedException,BrokenBarrierException,TimeoutException { };
```

第一个版本比较常用，用来挂起当前线程，直至所有线程都到达barrier状态再同时执行后续任务；

第二个版本是让这些线程等待至一定的时间，如果还有线程没有到达barrier状态就直接让到达barrier的线程执行后续任务。

下面举几个例子就明白了：

假若有若干个线程都要进行写数据操作，并且只有所有线程都完成写数据操作之后，这些线程才能继续做后面的事情，此时就可以利用CyclicBarrier了：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class Test {
    public static void main(String[] args) {
        int N = 4;
        CyclicBarrier barrier  = new CyclicBarrier(N);
        for(int i=0;i<N;i++)
            new Writer(barrier).start();
    }
    static class Writer extends Thread{
        private CyclicBarrier cyclicBarrier;
        public Writer(CyclicBarrier cyclicBarrier) {
            this.cyclicBarrier = cyclicBarrier;
        }
 
        @Override
        public void run() {
            System.out.println("线程"+Thread.currentThread().getName()+"正在写入数据...");
            try {
                Thread.sleep(5000);      //以睡眠来模拟写入数据操作
                System.out.println("线程"+Thread.currentThread().getName()+"写入数据完毕，等待其他线程写入完毕");
                cyclicBarrier.await(); //栅栏的地方 所有线程停留在此处等待其他线程执行到此
            } catch (InterruptedException e) {
                e.printStackTrace();
            }catch(BrokenBarrierException e){
                e.printStackTrace();
            }
            System.out.println("所有线程写入完毕，继续处理其他任务...");
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

测试结果：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
线程Thread-0正在写入数据...
线程Thread-3正在写入数据...
线程Thread-2正在写入数据...
线程Thread-1正在写入数据...
线程Thread-2写入数据完毕，等待其他线程写入完毕
线程Thread-0写入数据完毕，等待其他线程写入完毕
线程Thread-3写入数据完毕，等待其他线程写入完毕
线程Thread-1写入数据完毕，等待其他线程写入完毕
所有线程写入完毕，继续处理其他任务...
所有线程写入完毕，继续处理其他任务...
所有线程写入完毕，继续处理其他任务...
所有线程写入完毕，继续处理其他任务...
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　从上面输出结果可以看出，每个写入线程执行完写数据操作之后，就在等待其他线程写入操作完毕。

　　当所有线程线程写入操作完毕之后，所有线程就继续进行后续的操作了。

　　如果说想在所有线程写入操作完之后，进行额外的其他操作可以为CyclicBarrier提供Runnable参数：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class Test {
    public static void main(String[] args) {
        int N = 4;
        CyclicBarrier barrier  = new CyclicBarrier(N,new Runnable() {
            @Override
            public void run() {
                System.out.println("当前线程"+Thread.currentThread().getName());   
            }
        });
 
        for(int i=0;i<N;i++)
            new Writer(barrier).start();
    }
    static class Writer extends Thread{
        private CyclicBarrier cyclicBarrier;
        public Writer(CyclicBarrier cyclicBarrier) {
            this.cyclicBarrier = cyclicBarrier;
        }
 
        @Override
        public void run() {
            System.out.println("线程"+Thread.currentThread().getName()+"正在写入数据...");
            try {
                Thread.sleep(5000);      //以睡眠来模拟写入数据操作
                System.out.println("线程"+Thread.currentThread().getName()+"写入数据完毕，等待其他线程写入完毕");
                cyclicBarrier.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }catch(BrokenBarrierException e){
                e.printStackTrace();
            }
            System.out.println("所有线程写入完毕，继续处理其他任务...");
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

测试结果：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
线程Thread-0正在写入数据...
线程Thread-1正在写入数据...
线程Thread-2正在写入数据...
线程Thread-3正在写入数据...
线程Thread-0写入数据完毕，等待其他线程写入完毕
线程Thread-1写入数据完毕，等待其他线程写入完毕
线程Thread-2写入数据完毕，等待其他线程写入完毕
线程Thread-3写入数据完毕，等待其他线程写入完毕
当前线程Thread-3
所有线程写入完毕，继续处理其他任务...
所有线程写入完毕，继续处理其他任务...
所有线程写入完毕，继续处理其他任务...
所有线程写入完毕，继续处理其他任务...
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

当栅栏处的wait方法使用带有设定时间参数的时候，当等待Barrier过程中 超过时间后  就抛出异常并继续执行后面的任务。

案例代码：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class Test {
    public static void main(String[] args) {
        int N = 4;
        CyclicBarrier barrier  = new CyclicBarrier(N);
 
        for(int i=0;i<N;i++) {
            if(i<N-1)
                new Writer(barrier).start();
            else {                                //最后一个线程等待5000毫秒后再执行，使得其他线程会发生超时 抛出异常
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                new Writer(barrier).start();
            }
        }
    }
    static class Writer extends Thread{
        private CyclicBarrier cyclicBarrier;
        public Writer(CyclicBarrier cyclicBarrier) {
            this.cyclicBarrier = cyclicBarrier;
        }
 
        @Override
        public void run() {
            System.out.println("线程"+Thread.currentThread().getName()+"正在写入数据...");
            try {
                Thread.sleep(5000);      //以睡眠来模拟写入数据操作
                System.out.println("线程"+Thread.currentThread().getName()+"写入数据完毕，等待其他线程写入完毕");
                try {
                    cyclicBarrier.await(2000, TimeUnit.MILLISECONDS);
                } catch (TimeoutException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }catch(BrokenBarrierException e){
                e.printStackTrace();
            }
            System.out.println(Thread.currentThread().getName()+"所有线程写入完毕，继续处理其他任务...");
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
线程Thread-0正在写入数据...
线程Thread-2正在写入数据...
线程Thread-1正在写入数据...
线程Thread-2写入数据完毕，等待其他线程写入完毕
线程Thread-0写入数据完毕，等待其他线程写入完毕
线程Thread-1写入数据完毕，等待其他线程写入完毕
线程Thread-3正在写入数据...
java.util.concurrent.TimeoutException
Thread-1所有线程写入完毕，继续处理其他任务...
Thread-0所有线程写入完毕，继续处理其他任务...
    at java.util.concurrent.CyclicBarrier.dowait(Unknown Source)
    at java.util.concurrent.CyclicBarrier.await(Unknown Source)
    at com.cxh.test1.Test$Writer.run(Test.java:58)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(Unknown Source)
    at java.util.concurrent.CyclicBarrier.await(Unknown Source)
    at com.cxh.test1.Test$Writer.run(Test.java:58)
java.util.concurrent.BrokenBarrierException
    at java.util.concurrent.CyclicBarrier.dowait(Unknown Source)
    at java.util.concurrent.CyclicBarrier.await(Unknown Source)
    at com.cxh.test1.Test$Writer.run(Test.java:58)
Thread-2所有线程写入完毕，继续处理其他任务...
java.util.concurrent.BrokenBarrierException
线程Thread-3写入数据完毕，等待其他线程写入完毕
    at java.util.concurrent.CyclicBarrier.dowait(Unknown Source)
    at java.util.concurrent.CyclicBarrier.await(Unknown Source)
    at com.cxh.test1.Test$Writer.run(Test.java:58)
Thread-3所有线程写入完毕，继续处理其他任务...
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

另外CyclicBarrier是可以重用的，看下面这个例子：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class Test {
    public static void main(String[] args) {
        int N = 4;
        CyclicBarrier barrier  = new CyclicBarrier(N);
 
        for(int i=0;i<N;i++) {
            new Writer(barrier).start();
        }
 
        try {
            Thread.sleep(25000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
 
        System.out.println("CyclicBarrier重用");
 
        for(int i=0;i<N;i++) {
            new Writer(barrier).start();
        }
    }
    static class Writer extends Thread{
        private CyclicBarrier cyclicBarrier;
        public Writer(CyclicBarrier cyclicBarrier) {
            this.cyclicBarrier = cyclicBarrier;
        }
 
        @Override
        public void run() {
            System.out.println("线程"+Thread.currentThread().getName()+"正在写入数据...");
            try {
                Thread.sleep(5000);      //以睡眠来模拟写入数据操作
                System.out.println("线程"+Thread.currentThread().getName()+"写入数据完毕，等待其他线程写入完毕");
 
                cyclicBarrier.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }catch(BrokenBarrierException e){
                e.printStackTrace();
            }
            System.out.println(Thread.currentThread().getName()+"所有线程写入完毕，继续处理其他任务...");
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

测试结果：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
线程Thread-0正在写入数据...
线程Thread-1正在写入数据...
线程Thread-3正在写入数据...
线程Thread-2正在写入数据...
线程Thread-1写入数据完毕，等待其他线程写入完毕
线程Thread-3写入数据完毕，等待其他线程写入完毕
线程Thread-2写入数据完毕，等待其他线程写入完毕
线程Thread-0写入数据完毕，等待其他线程写入完毕
Thread-0所有线程写入完毕，继续处理其他任务...
Thread-3所有线程写入完毕，继续处理其他任务...
Thread-1所有线程写入完毕，继续处理其他任务...
Thread-2所有线程写入完毕，继续处理其他任务...
CyclicBarrier重用
线程Thread-4正在写入数据...
线程Thread-5正在写入数据...
线程Thread-6正在写入数据...
线程Thread-7正在写入数据...
线程Thread-7写入数据完毕，等待其他线程写入完毕
线程Thread-5写入数据完毕，等待其他线程写入完毕
线程Thread-6写入数据完毕，等待其他线程写入完毕
线程Thread-4写入数据完毕，等待其他线程写入完毕
Thread-4所有线程写入完毕，继续处理其他任务...
Thread-5所有线程写入完毕，继续处理其他任务...
Thread-6所有线程写入完毕，继续处理其他任务...
Thread-7所有线程写入完毕，继续处理其他任务...
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## Semaphore用法

　　Semaphore翻译成字面意思为 信号量，Semaphore可以控同时访问的线程个数，通过 acquire() 获取一个许可，如果没有就等待，而 release() 释放一个许可。

　　Semaphore类位于java.util.concurrent包下，它提供了2个构造器：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public Semaphore(int permits) {          //参数permits表示许可数目，即同时可以允许多少线程进行访问
    sync = new NonfairSync(permits);
}
public Semaphore(int permits, boolean fair) {    //这个多了一个参数fair表示是否是公平的，即等待时间越久的越先获取许可
    sync = (fair)? new FairSync(permits) : new NonfairSync(permits);
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

下面说一下Semaphore类中比较重要的几个方法，首先是acquire()、release()方法：

```
public void acquire() throws InterruptedException {  }     //获取一个许可
public void acquire(int permits) throws InterruptedException { }    //获取permits个许可
public void release() { }          //释放一个许可
public void release(int permits) { }    //释放permits个许可
```

acquire()用来获取一个许可，若无许可能够获得，则会一直等待，直到获得许可。

release()用来释放许可。注意，在释放许可之前，必须先获获得许可。

这4个方法都会被阻塞，如果想立即得到执行结果，可以使用下面几个方法

```
public boolean tryAcquire() { };    //尝试获取一个许可，若获取成功，则立即返回true，若获取失败，则立即返回false
public boolean tryAcquire(long timeout, TimeUnit unit) throws InterruptedException { };  //尝试获取一个许可，若在指定的时间内获取成功，则立即返回true，否则则立即返回false
public boolean tryAcquire(int permits) { }; //尝试获取permits个许可，若获取成功，则立即返回true，若获取失败，则立即返回false
public boolean tryAcquire(int permits, long timeout, TimeUnit unit) throws InterruptedException { }; //尝试获取permits个许可，若在指定的时间内获取成功，则立即返回true，否则则立即返回false
```

下面通过一个例子来看一下Semaphore的具体使用：

假若一个工厂有5台机器，但是有8个工人，一台机器同时只能被一个工人使用，只有使用完了，其他工人才能继续使用。那么我们就可以通过Semaphore来实现：【acquire 是阻塞式等待获得执行许可】

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class Test {
    public static void main(String[] args) {
        int N = 8;            //工人数
        Semaphore semaphore = new Semaphore(5); //机器数目
        for(int i=0;i<N;i++)
            new Worker(i,semaphore).start();
    }
 
    static class Worker extends Thread{
        private int num;
        private Semaphore semaphore;
        public Worker(int num,Semaphore semaphore){
            this.num = num;
            this.semaphore = semaphore;
        }
 
        @Override
        public void run() {
            try {
                semaphore.acquire(); //如果新线程进入后不能够获得许可  则会阻塞 直到获得许可然后再往下执行
　　　　　　　　　 //获得到使用资源的能力 并发执行的线程数 取决于 semaphore 的个数配置  此配置数也就是 可用资源的数目

                System.out.println("工人"+this.num+"占用一个机器在生产...");
                Thread.sleep(2000);
                System.out.println("工人"+this.num+"释放出机器");
                semaphore.release();    //释放许可       
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

测试结果：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
工人0占用一个机器在生产...
工人1占用一个机器在生产...
工人2占用一个机器在生产...
工人4占用一个机器在生产...
工人5占用一个机器在生产...
工人0释放出机器
工人2释放出机器
工人3占用一个机器在生产...
工人7占用一个机器在生产...
工人4释放出机器
工人5释放出机器
工人1释放出机器
工人6占用一个机器在生产...
工人3释放出机器
工人7释放出机器
工人6释放出机器
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

## exchanger 用法

　　Exchanger是在两个任务之间交换对象的栅栏，当这些任务进入栅栏时，它们各自拥有一个对象。当他们离开时，它们都拥有之前由对象持有的对象。

　　它典型的应用场景是：一个任务在创建对象，这些对象的生产代价很高昂，而另一个任务在消费这些对象。通过这种方式，可以有更多的对象在被创建的同时被消费。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public class ExchangerTest {
    public static void main(String[] args) {
        ExecutorService executor = Executors.newCachedThreadPool();
        final Exchanger exchanger = new Exchanger();
        executor.execute(new Runnable() {
            String data1 = "Ling";

            @Override
            public void run() {
                doExchangeWork(data1, exchanger);
            }
        });

        executor.execute(new Runnable() {
            String data2 = "huhx";

            @Override
            public void run() {
                doExchangeWork(data2, exchanger);
            }
        });
        executor.shutdown(); //把线程池停掉 释放资源
    }

    private static void doExchangeWork(String data1, Exchanger exchanger) {
        try {
            System.out.println(Thread.currentThread().getName() + "正在把数据 " + data1 + " 交换出去");
            Thread.sleep((long) (Math.random() * 1000));

            String data2 = (String) exchanger.exchange(data1);
            System.out.println(Thread.currentThread().getName() + "交换数据 到  " + data2);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

 

 

### Tools 总结：

下面对上面说的三个辅助类进行一个总结：

1）CountDownLatch和CyclicBarrier都能够实现线程之间的等待，只不过它们侧重点不同：

CountDownLatch一般用于某个线程A等待若干个其他线程执行完任务之后，它才执行；

而CyclicBarrier一般用于一组线程互相等待至某个状态，然后这一组线程再同时执行；

另外，CountDownLatch是不能够重用的，而CyclicBarrier是可以重用的。

2）Semaphore其实和锁有点类似，它一般用于控制对某组资源的访问权限。

 