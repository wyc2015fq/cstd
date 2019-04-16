# Java深入 - Java多线程 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年07月14日 19:31:27[initphp](https://me.csdn.net/initphp)阅读数：2056
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









### 线程概念

线程：线程是比进程更小的执行单位，它是在一个进程中独立的控制流，即程序内部的控制流。特点：线程不能独立运行，必须依赖于进程，在进程中运行。




### Java实现线程

Java实现线程一般由Thread类和实现Runnable接口的类组成。可以看一个Java实现线程的例子。

这里我们现实了Runnable接口的run方法，并且使用Thread类来实现了一个线程。Thread类会调用Runnable的run方法，并且运行在线程中。




```java
package com.test.www;


public class mainTest {

    public static void main(String[] args) {
        //实现Runbale的run方法
        class Test implements Runnable {

            @Override
            public void run() {
                System.out.println("Hello World");
                try {
                    Thread.sleep(2000); //sleep 2秒
                } catch (Exception e) {
                }
                System.out.println("Hello World2");
            }

        }
        Test test = new Test(); 
        Thread thread = new Thread(test); //线程类
        thread.start(); //线程启动执行
    }

}
```


输出：



Hello World

Hello World2




### Java线程的状态



**NEW**状态是指线程刚创建, 尚未启动

**RUNNABLE** 状态是线程正在正常运行中, 当然可能会有某种耗时计算/IO等待的操作/CPU时间片切换等, 这个状态下发生的等待一般是其他系统资源, 而不是锁, Sleep等

**BLOCKED ** 这个状态下, 是在多个线程有同步操作的场景, 比如正在等待另一个线程的synchronized 块的执行释放, 或者可重入的 synchronized块里别人调用wait() 方法, 也就是这里是线程在等待进入临界区


**WAITING ** 这个状态下是指线程拥有了某个锁之后, 调用了他的wait方法, 等待其他线程/锁拥有者调用 notify / notifyAll 一遍该线程可以继续下一步操作, 这里要区分 BLOCKED 和 WATING 的区别, 一个是在临界点外面等待进入, 一个是在理解点里面wait等待别人notify, 线程调用了join方法 join了另外的线程的时候, 也会进入WAITING状态, 等待被他join的线程执行结束

**TIMED_WAITING ** 这个状态就是有限的(时间限制)的WAITING, 一般出现在调用wait(long), join(long)等情况下, 另外一个线程sleep后, 也会进入TIMED_WAITING状态

**TERMINATED** 这个状态下表示 该线程的run方法已经执行完毕了, 基本上就等于死亡了(当时如果线程被持久持有, 可能不会被回收)





### 同步锁ReentrantLock

为什么在多线程环境下需要有锁，并且多线程下进行数据共享会造成不安全。主要是因为多个线程一起读取或者写入一个数据域的时候，会造成读写的竞争。Java是读取一个域的值的时候，是通过拷贝的方法。当多个线程同时进行读取或者写入的时候，有可能用户读取到的数据不是最新的，这就造成了数据的不准确。

可以先看一个造成线程数据竞争的例子：



```java
package com.test.www;


public class mainTest {

    public static void main(String[] args) {

        //卖票的例子
        class Test implements Runnable {

            private int ticket = 10; //10张火车票

            @Override
            public void run() {
                int i = 10;
                while (i > 0) {
                    i--;
                    try {
                        Thread.sleep(1000); //sleep 2秒
                    } catch (Exception e) {
                    }
                    sale(); //卖一张票
                }
            }

            private void sale() {
                if (ticket > 0) {
                    System.out.println(Thread.currentThread().getName() + "卖出一张票，剩余：" + ticket--);
                }
            }

        }
        Test test = new Test();
        new Thread(test, "A窗口").start();
        new Thread(test, "B窗口").start();
        new Thread(test, "C窗口").start();

    }

}
```


结果：



A窗口卖出一张票，剩余：10

B窗口卖出一张票，剩余：9

C窗口卖出一张票，剩余：10

B窗口卖出一张票，剩余：8

A窗口卖出一张票，剩余：7

C窗口卖出一张票，剩余：6

A窗口卖出一张票，剩余：5

B窗口卖出一张票，剩余：4

C窗口卖出一张票，剩余：3

C窗口卖出一张票，剩余：2

A窗口卖出一张票，剩余：1




你会发现上面的例子中，一共10张票，最终因为线程的抢夺，造成了卖出了11张票，造成了数据的不一致，这种现象其实是很可怕的。

Java提供了ReentrantLock同步锁，帮助我们解决数据的不安全问题。

我们可以优化上面的例子：



```java
package com.test.www;


import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class mainTest {

    public static void main(String[] args) {

        //卖票的例子
        class Test implements Runnable {

            private Lock myLock = new ReentrantLock(); //定义一个ReentrantLock

            private int  ticket = 10;                 //10张火车票

            @Override
            public void run() {
                int i = 10;
                while (i > 0) {
                    i--;
                    try {
                        Thread.sleep(1000); //sleep 2秒
                    } catch (Exception e) {
                    }
                    sale(); //卖一张票
                }
            }

            private void sale() {
                myLock.lock(); //同步锁，锁住
                if (ticket > 0) {
                    System.out.println(Thread.currentThread().getName() + "卖出一张票，剩余：" + ticket--);
                }
                myLock.unlock(); //解锁
            }

        }
        Test test = new Test();
        new Thread(test, "A窗口").start();
        new Thread(test, "B窗口").start();
        new Thread(test, "C窗口").start();

    }

}
```


结果：



A窗口卖出一张票，剩余：10

B窗口卖出一张票，剩余：9

C窗口卖出一张票，剩余：8

A窗口卖出一张票，剩余：7

B窗口卖出一张票，剩余：6

C窗口卖出一张票，剩余：5

A窗口卖出一张票，剩余：4

B窗口卖出一张票，剩余：3

C窗口卖出一张票，剩余：2

A窗口卖出一张票，剩余：1


上面的例子就很好的控制了线程间对资源的抢夺。

ReentrantLock还可以具体细分为读锁和写锁。具体的使用在这边不展开了。



```java
ReentrantReadWriteLock rwl = new ReentrantReadWriteLock();
        Lock readLock = rwl.readLock();
        Lock writeLock = rwl.writeLock();
```




### 同步关键字synchronized

使用ReentrantLock的时候，每次都要定义一个锁，然后在代码块中每次手动去解锁，这个过程其实是很繁琐的。关键字：**synchronized**很好的解决了这些问题，让一切变得更加简单。

可以看一个上面具体例子修改后的结果：



```java
package com.test.www;



public class mainTest {

    public static void main(String[] args) {

        //卖票的例子
        class Test implements Runnable {

            private int ticket = 10; //10张火车票

            @Override
            public void run() {
                int i = 10;
                while (i > 0) {
                    i--;
                    try {
                        Thread.sleep(1000); //sleep 2秒
                    } catch (Exception e) {
                    }
                    sale(); //卖一张票
                }
            }

            //使用了synchronized的效果和使用ReentrantLock的效果是一样的
            //而且使用synchronized会更加简单
            private synchronized void sale() {
                if (ticket > 0) {
                    System.out.println(Thread.currentThread().getName() + "卖出一张票，剩余：" + ticket--);
                }
            }

        }
        Test test = new Test();
        new Thread(test, "A窗口").start();
        new Thread(test, "B窗口").start();
        new Thread(test, "C窗口").start();

    }

}
```


结果：



A窗口卖出一张票，剩余：10

B窗口卖出一张票，剩余：9

C窗口卖出一张票，剩余：8

B窗口卖出一张票，剩余：7

C窗口卖出一张票，剩余：6

A窗口卖出一张票，剩余：5

B窗口卖出一张票，剩余：4

C窗口卖出一张票，剩余：3

A窗口卖出一张票，剩余：2

B窗口卖出一张票，剩余：1


结果是一样的。**synchronized一般可以用在方法上面，当然也可以用在代码块中。**



```java
public static void main(String[] args) {

        //卖票的例子  
        class Test implements Runnable {

            private int ticket = 10; //10张火车票  

            @Override
            public void run() {
                int i = 10;
                while (i > 0) {
                    i--;
                    try {
                        Thread.sleep(1000); //sleep 2秒  
                    } catch (Exception e) {
                    }
                    sale(); //卖一张票  
                }
            }

            private void sale() {
                synchronized (this) {
                    if (ticket > 0) {

                        System.out.println(Thread.currentThread().getName() + "卖出一张票，剩余："
                                + ticket--);

                    }
                }
            }

        }
        Test test = new Test();
        new Thread(test, "A窗口").start();
        new Thread(test, "B窗口").start();
        new Thread(test, "C窗口").start();

    }
```




### 关键字Volatile

如果只是对对象的域进行同步访问，我们可以使用**volatile**关键字。

域的访问安全有两种：

1. 标示为final的域。

2.域是volatile

3.对域的访问由公有的锁进行保护，例如**synchronized**关键词保护的。




```java
package com.test.www;


public class mainTest {

    public static void main(String[] args) {

        //卖票的例子
        class Test implements Runnable {

            //volatile 关键词，保证了该域的读写安全
            private volatile int ticket = 10; //10张火车票

            @Override
            public void run() {
                int i = 10;
                while (i > 0) {
                    i--;
                    try {
                        Thread.sleep(1000); //sleep 2秒
                    } catch (Exception e) {
                    }
                    sale(); //卖一张票
                }
            }

            private void sale() {
                if (ticket > 0) {
                    System.out.println(Thread.currentThread().getName() + "卖出一张票，剩余：" + ticket--);
                }
            }

        }
        Test test = new Test();
        new Thread(test, "A窗口").start();
        new Thread(test, "B窗口").start();
        new Thread(test, "C窗口").start();

    }

}
```


结果：



A窗口卖出一张票，剩余：10

B窗口卖出一张票，剩余：9

C窗口卖出一张票，剩余：8

A窗口卖出一张票，剩余：7

B窗口卖出一张票，剩余：6

C窗口卖出一张票，剩余：5

A窗口卖出一张票，剩余：4

B窗口卖出一张票，剩余：3

C窗口卖出一张票，剩余：2

C窗口卖出一张票，剩余：1




### 关于死锁等

多个线程同时被阻塞，它们中的一个或者全部都在等待某个资源被释放。由于线程被无限期地阻塞，因此程序不可能正常终止。导致死锁的根源在于不适当地运用synchronized关键词来管理线程对特定对象的访问。synchronized关键词的作用是，确保在某个时刻只有一个线程被允许执行特定的代码块，因此，被允许执行的线程首先必须拥有对变量或对象的排他性的访问权。当线程访问对象时，线程会给对象加锁，而这个锁导致其它也想访问同一对象的线程被阻塞，直至第一个线程释放它加在对象上的锁。

**死锁，主要还是需要自己的程序代码中去防范。**

多线程中，我们一般不使用stop和suspend方法。stop方法是终止所有未结束的方法，包括run方法。当线程被终止，立即释放被他锁住的所有对象的锁，这会导致对象处于不一致的状态。

c语言调试死锁一般使用pstack命令；Java调试死锁一般使用[jstack](http://blog.csdn.net/initphp/article/details/31040349#t5)





### Callable

Callable和Runnable不一样，Runnable运行了之后不会等待结果的返回，而Callable执行一个线程的时候，主线程会等待结果的返回。



```java
package com.test.www;


import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;

public class mainTest {

    public static void main(String[] args) throws InterruptedException, ExecutionException {

        //实现一个runable，返回字符串的结果
        class Test implements Callable<String> {

            //volatile 关键词，保证了该域的读写安全
            private volatile int ticket = 10; //10张火车票

            private void sale() {
                if (ticket > 0) {
                    System.out.println(Thread.currentThread().getName() + "卖出一张票，剩余：" + ticket--);
                }
            }

            @Override
            public String call() throws Exception {
                int i = 10;
                while (i > 0) {
                    i--;
                    try {
                        Thread.sleep(1000); //sleep 2秒
                    } catch (Exception e) {
                    }
                    sale(); //卖一张票
                }
                return Thread.currentThread().getName() + " 下班了";
            }

        }
        Test test = new Test();
        FutureTask<String> futrue = new FutureTask<String>(test); //使用futrue来接收异步的Callable
        new Thread(futrue, "A窗口").start(); //这边是阻塞等待
        System.out.println(futrue.get() + " "); //接收到Callable的返回值

    }

}
```


结果：



A窗口卖出一张票，剩余：10

A窗口卖出一张票，剩余：9

A窗口卖出一张票，剩余：8

A窗口卖出一张票，剩余：7

A窗口卖出一张票，剩余：6

A窗口卖出一张票，剩余：5

A窗口卖出一张票，剩余：4

A窗口卖出一张票，剩余：3

A窗口卖出一张票，剩余：2

A窗口卖出一张票，剩余：1

A窗口 下班了 




### 线程池

线程池的作用就是限制程序中线程的数量，并将空闲的线程有效的最大化利用起来。通过线程池，用户不需要关系线程创建和销毁的细节，只需关心实现业务的需求。

1.减少了创建和销毁线程的次数，每个工作线程都可以被重复利用，可执行多个任务。

2.可以根据系统的承受能力，调整线程池中工作线线程的数目，防止因为消耗过多的内存。

Java里面线程池的顶级接口是Executor，比较重要的几个类：


|ExecutorService|真正的线程池接口。|
|----|----|
|ScheduledExecutorService|能和Timer/TimerTask类似，解决那些需要任务重复执行的问题。|
|ThreadPoolExecutor|ExecutorService的默认实现。|
|ScheduledThreadPoolExecutor|继承ThreadPoolExecutor的ScheduledExecutorService接口实现，周期性任务调度的类实现。|


Java提供了四中创建线程池的方式：

1. **newSingleThreadExecutor**

创建一个单线程的线程池。这个线程池只有一个线程在工作。

2. **newFixedThreadPool**

创建固定大小的线程池。每次提交一个任务就创建一个线程，直到线程达到线程池的最大大小。

3. **newCachedThreadPool**



创建一个可缓存的线程池。如果线程池的大小超过了处理任务所需要的线程，

那么就会回收部分空闲（60秒不执行任务）的线程，当任务数增加时，此线程池又可以智能的添加新线程来处理任务。

4. **newScheduledThreadPool**

创建一个大小无限的线程池。此线程池支持定时以及周期性执行任务的需求。




创建一个可伸缩大小的线程池




```java
package com.test.www;


import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class mainTest {

    public static void main(String[] args) throws InterruptedException, ExecutionException {

        //runnable
        class Test implements Runnable {

            @Override
            public void run() {
                System.out.println("当前线程名称：" + Thread.currentThread().getName());
            }

        }
        Test test = new Test();
        Thread thread = new Thread(test);
        ExecutorService pool = Executors.newCachedThreadPool(); //创建一个可伸缩的线程池
        pool.submit(thread); //执行一个线程
        pool.submit(thread);
        pool.submit(thread);
        pool.submit(thread);
        pool.submit(thread);
        pool.submit(thread);
        pool.submit(thread);
        pool.submit(thread);
        pool.submit(thread);
        pool.submit(thread);
        pool.shutdown(); //关闭线程池

    }
}
```


结果：



当前线程名称：pool-1-thread-1

当前线程名称：pool-1-thread-3

当前线程名称：pool-1-thread-2

当前线程名称：pool-1-thread-1

当前线程名称：pool-1-thread-3

当前线程名称：pool-1-thread-6

当前线程名称：pool-1-thread-2

当前线程名称：pool-1-thread-4

当前线程名称：pool-1-thread-7

当前线程名称：pool-1-thread-5public class Test2 {



创建一个固定大小的线程池



```java
public class Test2 {

    public static void main(String[] args) throws InterruptedException, ExecutionException {

        //runnable  
        class Test implements Runnable {

            @Override
            public void run() {
                System.out.println("当前线程名称：" + Thread.currentThread().getName());
                try {
                    Thread.sleep(2000);
                } catch (Exception e) {
                }
            }

        }
        Test test = new Test();
        Thread thread = new Thread(test);
        ExecutorService pool = Executors.newFixedThreadPool(3);
        for (int i = 0; i < 100; i++) {
            pool.submit(thread); //执行一个线程
        }
        pool.shutdown(); //关闭线程池  

    }
}
```


结果：

当前线程名称：pool-1-thread-1

当前线程名称：pool-1-thread-2

当前线程名称：pool-1-thread-3

当前线程名称：pool-1-thread-1

当前线程名称：pool-1-thread-2

当前线程名称：pool-1-thread-3















