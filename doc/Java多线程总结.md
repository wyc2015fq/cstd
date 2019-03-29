## Java多线程总结

## (一)高并发和多线程的关系



​     “高并发和多线程”总是被一起提起，给人感觉两者好像相等，实则 **高并发 ≠ 多线程**

　　多线程是完成任务的一种方法，高并发是系统运行的一种状态，通过多线程有助于系统承受高并发状态的实现。

　  高并发是一种系统运行过程中遇到的一种“短时间内遇到大量操作请求”的情况，主要发生在web系统集中大量访问或者socket端口集中性收到大量请求（例如：12306的抢票情况；天猫双十一活动）。该情况的发生会导致系统在这段时间内执行大量操作，例如对资源的请求，数据库的操作等。如果高并发处理不好，不仅仅降低了用户的体验度（请求响应时间过长），同时可能导致系统宕机，严重的甚至导致OOM异常，系统停止工作等。如果要想系统能够适应高并发状态，则需要从各个方面进行系统优化，包括，硬件、网络、系统架构、开发语言的选取、数据结构的运用、算法优化、数据库优化……而**多线程只是其中解决方法之一**。

　　　实现高并发需要考虑：
                系统的架构设计，如何在架构层面减少不必要的处理（网络请求，数据库操作等）
                网络拓扑优化减少网络请求时间、如何设计拓扑结构，分布式如何实现？
                系统代码级别的代码优化，使用什么设计模式来进行工作？哪些类需要使用单例，哪些需要尽量减少new操作？
                提高代码层面的运行效率、如何选取合适的数据结构进行数据存取？如何设计合适的算法？
                任务执行方式级别的同异步操作，在哪里使用同步，哪里使用异步？
                JVM调优，是以server模式还是以clien模式运行，如何设置Heap、Stack、Eden的大小，如何选择GC策略,控制Full GC的频率？
                数据库优化减少查询修改时间。数据库的选取？数据库引擎的选取？数据库表结构的设计？数据库索引、触发器等设计？是否使用读写分离？还是需要考虑使用数据仓库？
                缓存数据库的使用，如何选择缓存数据库？是Redis还是Memcache? 如何设计缓存机制？
                数据通信问题，如何选择通信方式？是使用TCP还是UDP，是使用长连接还是短连接？NIO还是BIO？netty、mina还是原生socket？
                操作系统选取，是使用winserver还是Linux？或者Unix？
                硬件配置？是8G内存还是32G，网卡10G还是1G?
                ……
                ……

以上的这些问题在高并发中都是必须要深入考虑的，就像木桶原理一样，只要其中的某一方面没有考虑到，都会造成系统瓶颈，影响整个系统的运行。而高并发问题不仅仅涉及面之广，同时又要求有足够的深度！！！

　　　而**多线程在这里只是在同/异步角度上解决高并发问题的其中的一个方法手段，是在同一时刻利用计算机闲置资源的一种方式。**

多线程在解决高并发问题中所起到的作用就是使计算机的资源在每一时刻都能达到最大的利用率，不至于浪费计算机资源使其闲置。







## (二)使用多线程的准备知识



**一、为什么要使用多线程？**
        【使计算机所有资源在执行任务的时候能够全部利用上，以提升计算机资源利用率的方式来提升系统执行效率】
　　CPU的单核运行速度由于硬件技术问题已经遇到瓶颈，而概念性的“光脑”貌似离我们还很遥远，现在的计算机性能提升方向是向多核发展。多核同时工作，协同完成任务。大家熟知的神威·太湖之光超级计算机总共使用了40960颗处理器，总计拥有10649600颗核心、1.31PB内存。即使是目前市场上的主流家用电脑也一般达到四核心八线程的配置标准。那么对于这些多核的CPU，在开发软件的时候就不得不考虑如何充分利用上每一个核的性能，以至于使系统运行的效率更高。

**二、CPU是执行的是线程任务还是进程任务？**
        【CPU执行的是线程中定义的任务】
        进程是对内存资源的抽象，线程是对执行任务的抽象。CPU执行的是线程任务，和进程没有任何关系。所以，CPU中的核在任意时间点只能执行某一个线程的任务，具体执行哪个线程就要看操作系统的任务调度策略。在单核多线程任务中，操作系统会把CPU资源按照时间片划分，根据线程的优先级选择线程进行执行任务。

**三、****程序中线程的数量控制在核数的1~2倍对吗？**
        【不对！】

​         左图为工作时win10 四核八线程 线程数： 2094。右图为闲置centos7  单核 线程数 ：127 。线程数量远远超过CPU核心数量的上百倍。

  ![img](https://images2017.cnblogs.com/blog/837242/201708/837242-20170811142332492-987620200.png)![img](https://images2017.cnblogs.com/blog/837242/201708/837242-20170811142520210-754270767.png)
        所以在开发程序过程中，如果不是线程开启机制错误，就不会产生成上万级别的线程导致线程切换浪费资源，而几百个线程切换对CPU来说还不至于产生过多的资源消耗。
             线程开启机制错误例子：socket每接收一个请求便创建一个线程执行任务。这种情况下，很容易的便能开启上千上万个线程（在工作中遇到过）。
**四、线程频繁切换会耗费资源？**
        【消耗资源肯定会，但是消耗的资源一般情况下没必要重视，当需要去重视的时候你就不会在看本文了】
        线程消耗必定会耗费计算机资源，影响系统的执行效率。但是这种级别的资源浪费还没必要引起关注去考虑优化，随便优化一个SQL查询就远远比优化线程切换性能提升的多。
        
    测试：
        **计算任务**：计算1亿次 f(i)=(i * 10000.56) / 200 (i从1循环到10000)  每组数据测试4次。
        **电脑配置**： 四核八核心CPU 8G内存 win10系统（测试过程中运行的有其它程序，但整个过程中没有新的应用程序开启或者关闭。测试前后计算机进程数102左右，线程数210左右）
        数据如下：单位s
                *1线程执行10000 \* 10000次相同任务运算时间：0.043584183000000006*
                *1线程执行10000 \* 10000次相同任务运算时间：0.03411422*
                *1线程执行10000 \* 10000次相同任务运算时间：0.033755275*
                *1线程执行10000 \* 10000次相同任务运算时间：0.037857329*
                
                *10线程执行10000 \* 1000次相同任务运算时间：0.020971566*
                *10线程执行10000 \* 1000次相同任务运算时间：0.020400791*
                *10线程执行10000 \* 1000次相同任务运算时间：0.043275417000000004*
                *10线程执行10000 \* 1000次相同任务运算时间：0.020918537*
                
                *50线程执行10000 \* 200次相同任务运算时间：0.041526313*
                *50线程执行10000 \* 200次相同任务运算时间：0.028923265*
                *50线程执行10000 \* 200次相同任务运算时间：0.020633701000000003*
                *50线程执行10000 \* 200次相同任务运算时间：0.01057820000000002*

​                
​                *100线程执行10000 \* 100次相同任务运算时间：0.034126764000000004*
​                *100线程执行10000 \* 100次相同任务运算时间：0.036614853*
​                *100线程执行10000 \* 100次相同任务运算时间：0.060594536000000004*
​                *100线程执行10000 \* 100次相同任务运算时间：0.046860865*
​                
​                *500线程执行10000 \* 20次相同任务运算时间：0.054617363*
​                *500线程执行10000 \* 20次相同任务运算时间：0.090330437*
​                *500线程执行10000 \* 20次相同任务运算时间：0.060385270000000005*
​                *500线程执行10000 \* 20次相同任务运算时间：0.06421362700000001*
​                
​                *1000线程执行10000 \* 10次相同任务运算时间：0.101977442*
​                *1000线程执行10000 \* 10次相同任务运算时间：0.09040313700000001*
​                *1000线程执行10000 \* 10次相同任务运算时间：0.094736125*
​                *1000线程执行10000 \* 10次相同任务运算时间：0.09127384000000001*
​                
​                *2000线程执行10000 \* 5次相同任务运算时间：0.16429143000000002*
​                *2000线程执行10000 \* 5次相同任务运算时间：0.18807325200000002*
​                *2000线程执行10000 \* 5次相同任务运算时间：0.16652977000000002*
​                *2000线程执行10000 \* 5次相同任务运算时间：0.18108197*
​                
​                
​                *5000线程执行10000 \* 2次相同任务运算时间：0.44204793600000003*
​                *5000线程执行10000 \* 2次相同任务运算时间：0.484968116*
​                *5000线程执行10000 \* 2次相同任务运算时间：0.405066727*
​                *5000线程执行10000 \* 2次相同任务运算时间：0.42432967400000005*
​                
​                
​                *10000线程执行10000 \* 1次相同任务运算时间：1.453686674*
​                *10000线程执行10000 \* 1次相同任务运算时间：1.307516071*
​                *10000线程执行10000 \* 1次相同任务运算时间：1.5121169970000001*
​                *10000线程执行10000 \* 1次相同任务运算时间：1.332141795*
​                

由数据中可以看出，在程序开启1-50个线程时，执行时间在下降，10-1000个线程之内，执行时间都在同一个量级，超过一千线程之后，时间提升一个量级。因此，粗略来看 1000个以下线程切换并不会浪费大量系统资源。在程序中如果线程开启机制正确，同时使用到线程池，那么系统优化瓶颈就不会在线程切换的问题上。没必要在这个角度考虑系统优化的问题。
测试代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
 //scala2.12.12 jdk1.8
  def showNCPUTime(): Unit = {
   //测试过程中调整threadNum，inner 这两个参数,同时保证两者之积不变 inner * theradNum =10000
　　val threadNum = 2000
    val inner = 5
    //固定不变
    val top = 10000    
    val state = new CountDownLatch(threadNum)
    var threads = new Array[Thread](threadNum)
    for (time <- (0 until threadNum)) {
      val thread = new Thread(new Runnable {
        override def run(): Unit = {
          for (i <- (1 to top)) {
            for (j <- (1 to inner)) {
              //i 每次都是从1 到 1000000
              val temp = (i * 105300.56) / 200
            }
          }
          state.countDown()
        }
      })
      threads(time) = thread
    }
    var startTime = System.nanoTime
    for (time <- (0 until threadNum)) {
      threads(time).start()
    }
    state.await()
    val endTime = System.nanoTime
    println(threadNum + "线程执行" + top + " * " + inner + "次相同任务运算时间：" + (endTime - startTime) * 0.000000001)
  }
```



 
**五、使用多线程的难点在哪里？**
         多线程的难点在于共享数据读写顺序的问题，保证多个线程对同一数据的操作不会产生混乱。
        程序中流动的都是数据，程序影响的也都是数据。在多个线程同时访问共享数据的时候，由于线程读取/写入的时机不对而导致数据出错，进而影响业务。

 

下面的问题涉及到操作系统底层知识，列出来只是为了引起读者的思考，不再回答，以免误人误己。
六、对于多核CPU，线程任务是如何分配到每个核上面的？
七、单核CPU任务调度策略。在多个线程中，操作系统是如何选取其中某个线程分配给CPU执行的？选取规则是什么？
八、线程切换耗费资源，那么都耗费在什么地方？挂起一个线程的时候操作系统都进行了哪些操作？
九、操作系统开启一个新线程需要执行哪些步骤？计算机能够开启的最大线程数是多少？JVM能够开启的最大线程数是多少？他们和内存的大小有什么关系？







## (三)java中线程的简单使用



​       java中，启动线程通常是通过Thread或其子类通过调用start()方法启动。
 常见使用线程有两种：实现Runnable接口和继承Thread。而继承Thread亦或使用TimerTask其底层依旧是实现了Runnabel接口。考虑到java的单继承的限制，所以在开发过程中大部分情况在使用线程的时候是通过实现Runnabel接口或者Runnbel匿名类来实现的。
 例如：



```java
package com.zpj.thread.blogTest;
/**
 * Created by PerkinsZhu on 2017/8/11 16:42.
 */
public class ThreadTest {

    public static void main(String[] args) {
        ThreadTest threadTest = new ThreadTest();
        //所有的这些启动方式的执行体都是在run方法中完成的
        threadTest.testLambda();
        threadTest.testRunnableWithAnonymousRunnable();
        threadTest.testRunnableWithAnonymousThread();
        threadTest.testRunnable();
        threadTest.testMyThread();
    }

    public void testLambda() {//lambda表达式开启线程 jdk1.8中的特性
        new Thread(() -> System.out.println("i am lambda Thread....")).start();
    }

    public void testRunnableWithAnonymousThread() {//匿名Thread类开启线程
        new Thread() {
            @Override
            public void run() {
                System.out.println("i am ThreadWithAnoymous");
            }
        }.start();
    }

    public void testRunnableWithAnonymousRunnable() {//匿名Runnable类开启线程
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("i am RunableWithAnoymous");
            }
        });
        thread.start();
    }

    public void testRunnable() {//实现Runnable接口
        MyRunnable runable = new MyRunnable();
        Thread thread = new Thread(runable);
        thread.start();
    }

    public void testMyThread() {//继承自Thread
        MyThread thread = new MyThread();
        thread.setName("MyThread");
        thread.start();
    }
}

class MyRunnable implements Runnable {//实现Runnable接口

    @Override
    public void run() {
        System.out.println("i am MyRunnable");
    }
}

class MyThread extends Thread {//继承Thread

    @Override
    public void run() {
        System.out.println(" i am MyThread!!");
    }
}
```



 注意，直接调用run()方法的方式执行线程体并未开启新线程，只是在main方法中调用了一个普通方法而已。而使用start()方法则会开启一个新的线程执行。两者的区别主要表现在前者是阻塞式的，而后者为非阻塞式。

​      例如：



```java
    public void testStartAndRun(){
        MyThread thread = new MyThread();
        thread.setName("MyThread");
        thread.start();//start启动两者异步非阻塞运行
        while (true){
            System.out.println("---- "+Thread.currentThread().getName());
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
class MyThread extends Thread {//继承Thread

    @Override
    public void run() {
        while(true){
            System.out.println("=== "+Thread.currentThread().getName());
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
```



 运行结果如下：



```
---- main
==== MyThread
---- main
==== MyThread
==== MyThread
---- main
==== MyThread
---- main
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 而把thread.start() 修改为thread.run();之后，则如下

```
==== main
==== main
==== main
==== main
==== main
==== main
```

 这里之所以线程名称为main是因为是在main线程中调用的run方法，所以打印出来的是===main。而thread.run();语句下面的循环则永远不会执行，程序将会一直在run方法中循环下去。

那么调用start方法，程序都做了些什么呢？看一下底层实现。



```java
    public synchronized void start() {
        //验证线程的状态
        if (threadStatus != 0) {//这里的验证涉及到线程不能重复启动的问题，线程多次调用start则会抛出该异常
            throw new IllegalThreadStateException();
        }
        //把该线程加入到线程组中
        group.add(this);
        boolean started = false;//标识线程是否启动成功
        try {
            start0();//调用native方法启动线程 该方法是阻塞的，程序等待其完成之后执行下面语句如果执行失败则直接抛出异常进入finally。
            started = true;//修改线程启动状态
        } finally {
            try {
                if (!started) {//启动失败，则移出线程组
                    group.threadStartFailed(this);
                }
            } catch (Throwable ignore) {
            }
        }
    }

private native void start0();//native方法启动线程
```



 通过Runnable启动具有一定的局限，执行线程没有返回值，无法捕获异常。在有些特殊情况下需要线程返回结果的时候就不太合适。这时可以选择Callable接口来完成。Callable涉及到Future模型，这到后面再说。

 



## (四)如何停止线程



　　在Thread类中提供了可以停止线程的方法（包括杀死和挂起）：

　　　　@Deprecated
　　　　**public final void stop(){}**
　　　　@Deprecated
　　　　**public final void suspend(){}**　　　　
　　 stop 和 suspend 添加的有Deprecated注释，也即是该方法已经废弃使用。那么为什么会不建议使用这两种方法呢？还有没有其他停止线程的方法？

1、stop()会立即杀死线程，无法保证原子操作能够顺利完成，存在数据错误的风险。无论线程执行到哪里、是否加锁，stop会立即停止线程运行，直接退出。

　　　如下代码：



```java
 int account01 = 10;
    int account02= 0;
    Object lock = new Object();
    
    public void testStop() {
        class StopRunnable implements Runnable {
            @Override
            public void run() {
                //要求 account01 + account02 =10  恒成立
                while (true) {
                    synchronized (lock) {//加锁保证操作的原子性
                        account01--;
                        sleep(200);//睡眠模拟执行过程
                        account02++;
                    }
                }
            }
        }
        Thread thread = new Thread(new StopRunnable());
        thread.start();
        sleep(1300);
        thread.stop();
        System.out.println("account01: " + account01 + "\naccount02: " + account02);
    }
    private void sleep(int time){
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
```



 运行结果如下：

```
account01: 3
account02: 6
```

 很明显没有保证两者的和为10。说明在线程循环过程中，最后一个加锁循环体没有完整执行结束，数据发生错误。除此之外，在执行stop方法之后，线程会立即释放锁，这同样也会导致原子操作失败数据异常。

官方注释：

```
Forces the thread to stop executing.
It is permitted to stop a thread that has not yet been started.
If the thread is eventually started, it immediately terminates.
```

 2、suspend()并未杀死线程，只是把线程挂起，停止线程的运行。但挂起之后并不会释放锁，这样，如果有其它多个线程在等待该锁，则程序将会发生死锁。

　　如下代码：



```java
    int account01 = 10;
    int account02= 0;
    Object lock = new Object();

    public void testStop() {
        class StopRunnable implements Runnable {
            @Override
            public void run() {
                //要求 account01 + account02 =10  恒成立
                for(int i =0;i<5;i++){
                    synchronized (lock) {//加锁保证操作的原子性
                        account01--;
                        System.out.println("....."+Thread.currentThread().getName());//为了看到线程停止添加输出线程名称操作
                        sleep(200);//睡眠200ms
                        account02++;
                    }
                }
            }
        }
        Thread thread01 = new Thread(new StopRunnable());
        thread01.setName("thread01");
        Thread thread02 = new Thread(new StopRunnable());
        thread02.setName("thread02");

        thread01.start();
        thread02.start();

        sleep(500);
        thread01.suspend();
        while (true){
            sleep(1000);
            System.out.println("account01: " + account01 + " account02: " + account02+" thread01 isAlive:"+thread01.isAlive()+" thread02 isAlive:"+thread02.isAlive());
        }
    }
```



运行结果如下：　　



```java
.....thread01
.....thread01
.....thread01
account01: 7 account02: 2 thread01 isAlive:true thread02 isAlive:true
account01: 7 account02: 2 thread01 isAlive:true thread02 isAlive:true
account01: 7 account02: 2 thread01 isAlive:true thread02 isAlive:true
account01: 7 account02: 2 thread01 isAlive:true thread02 isAlive:true
account01: 7 account02: 2 thread01 isAlive:true thread02 isAlive:true
account01: 7 account02: 2 thread01 isAlive:true thread02 isAlive:true
```



由结果可以看出，thread01一直在运行，而thread02一次也没有执行到run方法。然后在执行thread01.suspend()之后，两个线程都停止了运行run方法。但同时两个线程都没有死亡。

在代码中只对thread01执行了suspend，那么如果thread02获取到锁则应该继续由thread02执行run方法，但并没有，说明锁lock一直由thread01持有，在挂起之后并未释放。

其实在使用suspend()方法的时候是需要配合resume()同时使用的。



```java
　　　　　....
　　　　　....
        sleep(500);
        thread01.suspend();
        int  time = 0;//添加time计数，在循环三次之后释放
        while (true){
            time ++;
            if(time ==3){
                thread01.resume();//释放线程
            }
            sleep(1000);
            System.out.println("account01: " + account01 + " account02: " + account02+" thread01 isAlive:"+thread01.isAlive()+" thread02 isAlive:"+thread02.isAlive());
        }
```



 执行结果如下：　　



```java
.....thread01
.....thread01
.....thread01
account01: 7 account02: 2 thread01 isAlive:true thread02 isAlive:true
account01: 7 account02: 2 thread01 isAlive:true thread02 isAlive:true
.....thread01  //释放之后继续运行
.....thread01
.....thread02  //thread01释放锁，thread02获取锁继续运行
.....thread02
.....thread02
account01: 2 account02: 7 thread01 isAlive:false thread02 isAlive:true  //thread01 死亡，thread02活着
.....thread02
.....thread02
account01: 0 account02: 10 thread01 isAlive:false thread02 isAlive:false
account01: 0 account02: 10 thread01 isAlive:false thread02 isAlive:false
account01: 0 account02: 10 thread01 isAlive:false thread02 isAlive:false
account01: 0 account02: 10 thread01 isAlive:false thread02 isAlive:false
```



 可以看出，thread01.resume()之后thread01继续运行，然后运行结束释放锁之后，thread02接着运行起来，这时可以看到thread01已经死亡，而thread02依旧活着。直至两个线程全部结束。如果正常使用suspend()和resume()并不会出现太大问题，只是在涉及到锁的时候久需要格外小心了。r如果没有使用到锁，那么其中一个线程的挂起并不会影响到其他线程的执行。

对于**public void interrupt(){}**方法，该方法只是对阻塞状态的线程（seleep、wait、join和IO/NIO操作）进行中断操作，在调用interrupt方法的时候，如果线程处于阻塞状态则会抛出**InterruptedException/ClosedByInterruptException。**在程序中只需对异常进行捕获，并不影响后续的操作。对于未处于阻塞状态的线程，调用interrupt方法没有任何影响。所以interrupt严格意义上说并不属于停止线程的方法。

那么，到底该如何安全的停止线程呢？

　　遵循的规则：**让线程自己停止自己**。

 　   两种方法**：1、线程任务执行完成，顺利结束退出。2、设置终止标志位，在循环的时候进行终止标志位检测，如果设置为终止状态则return结束线程。**

　例如：1、线程执行完成自动退出：

```java
            public void run() {
                for(int i = 0;i<10;i++){//循环十次之后run方法结束自动结束线程
                    System.out.println(Thread.currentThread().getName());
                }
            }
```

 2、设置终止标志位。



```java
    boolean isStop = false;//终止标志位 当需要结束线程时，更改为true
    public void testInterrupt(){
        Thread th = new Thread(new Runnable() {
            @Override
            public void run() {
                while(true){
                    if(isStop){//当需要结束线程的时候终止线程
                        //doSomething  进行一些收尾工作
                        return;
                    }
                    System.out.println(Thread.currentThread().getName());
                }
            }
        });
```



  设置终止标志位的时候线程不会立即终止，只有当循环到标志位判断的时候才会执行退出操作，这样就可以在循环体中合适的地方执行退出逻辑，可以保证原子操作的顺利完成以及锁的释放。

 对于ExecutorService的**void shutdown();**方法，该方法只是停止线程池接受新的任务同时等待已提交线程结束，并不会停止线程。所以该方法不属于停止线程的方法。

 





## (五)java的线程锁



　　在多线程中，每个线程的执行顺序，是无法预测不可控制的，那么在对数据进行读写的时候便存在由于读写顺序多乱而造成数据混乱错误的可能性。那么如何控制，每个线程对于数据的读写顺序呢？这里就涉及到线程锁。

**什么是线程锁？使用锁的目的是什么？**先看一个例子。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
   private void testSimple(){
        SimpleRunner runner = new SimpleRunner();
        pool.execute(runner);
        pool.execute(runner);
    }
    int account01 =10;
    int account02 = 0;
    class SimpleRunner implements Runnable{
        @Override
        public void run() {
            while(true){//保证两个账户的总额度不变
                account01 --;
                sleep(1000);
                account02 ++;
                Console.println("account01:"+account01+"  account02:"+account02);
            }
        }
    }
    private void sleep(int time){
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 调用testSimple()方法开启两个线程执行账户金额转移，运行结果如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
account01:9  account02:2
account01:9  account02:2
account01:8  account02:4
account01:8  account02:4
account01:6  account02:6
account01:6  account02:6
account01:5  account02:7
account01:4  account02:8
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 很明显两个账户的金额总和无法保证为10，甚至变多了。之所以发生这种状况一方面是因为++ 和--操作不是原子操作，其次两个变量的修改也没有保证同步进行。由于线程的不确定性则将导致数据严重混乱。下面换一种方式看看如何：

我们修改while循环体，不使用++或者--操作，同时对操作进行加锁：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
 while(true){//保证两个账户的总额度不变
                synchronized ("lock"){//通过synchronized锁把两个变量的修改进行同步
                    account01 = account01 -1;
                    account02 = account02 +1;
                    Console.println("account01:"+account01+"  account02:"+account02);
                    sleep(1000);
                }
            }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 执行结果如下：

```
account01:9  account02:1
account01:8  account02:2
account01:7  account02:3
account01:6  account02:4
account01:5  account02:5
```

 现在数据就能够完全正常了。这里涉及到synchronized 锁，其目的就是**保证在任意时刻，只允许一个线程进行对临界区资源（被锁着的代码块）的操作**。

习惯上喜欢称这种机制为加锁，为了容易理解，可以把这种机制理解为一把钥匙和被锁着的代码块，只有拿到钥匙的线程才能执行被锁住的代码块。而钥匙就是synchronized(“lock”)中的字符串对象"lock"，而被锁着的代码块则是{}中的代码。

![img](https://images2017.cnblogs.com/blog/837242/201708/837242-20170818194248787-567830459.png)

某个线程如果想要执行代码块中的内容，则必须要拥有钥匙"lock"对象。但“lock”有个特性，同一时刻只允许一个线程拥有（暂时不考虑共享锁）。这样就可以保证所有的线程依次执行被锁着的代码块，避免数据混乱。在这里有一个前提条件，也就是钥匙是对于所有线程可见的，应该设置为全局变量且只有一个实例，否则每一个线程都有一个自己的钥匙，那么就起不到锁的作用了。例如：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
            while(true){
                String lock = new String("lock");//每个线程进入run方法的时候都new一个自己的钥匙
                synchronized (lock){
                    account01 = account01 -1;
                    account02 = account02 +1;
                    Console.println("account01:"+account01+"  account02:"+account02);
                    sleep(1000);
                }
            }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 执行结果如下：

```
account01:8  account02:2
account01:8  account02:2
account01:6  account02:3
account01:6  account02:3
account01:5  account02:5
account01:4  account02:5
```

 这样便又发生了混乱，每个线程都有自己的钥匙，他们随时都可以操作临界区资源，和没有加锁无任何区别。所以在多线程操作中，锁的使用至关重要！！！

 **在java中有哪些锁？该如何进行分类呢？**

**1、共享锁/排它锁** 

　　　　共享锁和排他锁是从同一时刻是否允许多个线程持有该锁的角度来划分。
              共享锁允许同一时刻多个线程进入持有锁，访问临界区资源。而排他锁就是通常意义上的锁，同一时刻只允许一个线程访问临界资源。对于共享锁，主要是指对数据库读操作中的读锁，在读写资源的时候如果没有线程持有写锁和请求写锁，则此时允许多个线程持有读锁。 
              在这里理解共享锁的时候，不是任意时刻都允许多线程持有共享锁的，而是在某些特殊情况下才允许多线程持有共享锁，在某些情况下不允许多个线程持有共享锁，否则，如果没有前提条件任意时刻都允许线程任意持有共享锁，则共享锁的存在无意义的。例如读写锁中的读锁，只有当没有写锁和写锁请求的时候，就可以允许多个线程同时持有读锁。这里的前提条件就是“没有写锁和写锁请求”，而不是任意时刻都允许多线程持有共享读锁。
  **2、悲观锁/乐观锁**  
            主要用于数据库数据的操作中，而对于线程锁中较为少见。
            悲观锁和乐观锁是一种加锁思想。对于乐观锁，在进行数据读取的时候不会加锁，而在进行写入操作的时候会判断一下数据是否被其它线程修改过，如果修改则更新数据，如果没有则继续进行数据写入操作。乐观锁不是系统中自带的锁，而是一种数据读取写入思想。应用场景例如：在向数据库中插入数据的时候，先从数据库中读取记录修改版本标识字段，如果该字段没有发生变化（没有其他线程对数据进行写操作）则执行写入操作，如果发生变化则重新计算数据。
             对于悲观锁，无论是进行读操作还是进行写操作都会进行加锁操作。对于悲观锁，如果并发量较大则比较耗费资源，当然保证了数据的安全性。

 **3、可重入锁/不可重入**
                这两个概念是从同一个线程在已经持有锁的前提下能否再次持有锁的角度来区分的。
                对于可重入锁，如果该线程已经获取到锁且未释放的情况下允许再次获取该锁访问临界区资源。此种情况主要是用在递归调用的情况下和不同的临界区使用相同的锁的情况下。
                对于不可重入锁，则不允许同一线程在持有锁的情况下再次获取该锁并访问临界区资源。对于不可重入锁，使用的时候需要小心以免造成死锁。

 **4、公平锁/非公平锁**
                这两个概念主要使用线程获取锁的顺序角度来区分的。
                对于公平锁，所有等待的线程按照按照请求锁的先后循序分别依次获取锁。
                对于非公平锁，等待线程的线程获取锁的顺序和请求的先后不是对应关系。有可能是随机的获取锁，也有可能按照其他策略获取锁，总之不是按照FIFO的顺序获取锁。
                在使用ReentrantLock的时候可以通过构造方法主动选择是实现公平锁还是非公平锁。

**5、自旋锁/非自旋锁**
                这两种概念是从线程等待的处理机制来区分的。
                自旋锁在进行锁请求等待的时候不进行wait挂起，不释放CPU资源，执行while空循环。直至获取锁访问临界区资源。适用于等待锁时间较短的情景，如果等待时间较长，则会耗费大量的CPU资源。而如果等待时间较短则可以节约大量的线程切换资源。
                非自旋锁在进行锁等待的时候会释放CPU资源，可以通多sleep wait 或者CPU中断切换上下文，切换该线程。在线程等待时间较长的情况下可以选择此种实现机制。
        除此之外还有一种介于两者之间的锁机制——自适应自旋锁。当线程进行等待的时候先进性自旋等待，在自旋一定时间(次数)之后如果依旧没有持有锁则挂起等待。在jvm中synchronized锁已经使用该机制进行处理锁等待的情况。
在工作中可以根据不同的情况选取合适的锁进行使用。无论使用哪种锁，其目的都是保证程序能够按照要求顺利执行，避免数据混乱情况的发生。

**常用锁的使用方法**
        **1、synchronized锁：**

　　　　对于synchronized锁首先需要明白加锁的底层原理。每一个对象实例在对象头中都会有monitor record列表记录持有该锁的线程，底层通多对该列表的查询来判断是否已经有线程在访问临界区资源。[JVM内部细节之一：synchronized关键字及实现细节（轻量级锁Lightweight Locking）](http://www.cnblogs.com/javaminer/p/3889023.html)

　　　　在使用synchronized的时候必须弄清楚谁是“钥匙”，属于全局变量还是线程内局部变量，每个加锁的临界区是使用的哪个“钥匙”对象。必须理清楚加锁线程和“钥匙”对象的关系！！！！

　　　　synchronized只可以对方法和方法中的代码块进行加锁，而网上所说的“类锁”并不是对类进行加锁，而是synchronized(XXXX.class)。synchronized是不支持对类、构造方法和静态代码块进行加锁的。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
     public synchronized void showInfo01(){//这里synchronized锁的是this对象，也即synchronized(this)
     }
    public void showInfo02(){
        synchronized (this){//这里的this可以替换为任意Object对象。注意是Object对象，基本变量不行。java中字符串是String实例，所以字符串是可以的。
            //doSomething
        }
    }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

​         **2、reentranLock**

　　　　synchronized加锁机制使基于JVM层面的加锁，而ReentrantLock是基于jdk层面的加锁机制。ReentrantLock根据名称可以看出是可重入锁，其提供的构造方法可以指定公平锁或非公平锁。ReentrantLock使用起来比synchronized更加灵活、方便和高效。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
    /**
     * Creates an instance of {@code ReentrantLock} with the
     * given fairness policy.
     *
     * @param fair {@code true} if this lock should use a fair ordering policy
     */
    public ReentrantLock(boolean fair) {//通过true或false来指定公平锁或非公平锁
        sync = fair ? new FairSync() : new NonfairSync();
    }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 下面看一下使用方法：这里使用的是默认非公平锁进行测试。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
    private void testReentrantLock() {
        MyRunnerForReentrantLock run = new MyRunnerForReentrantLock();
        for (int i = 0; i < 10; i++) {//开启10个线程进行测试
　　　　　　　sleep(10);//睡眠10ms保证线程开启的顺序能够按照1-10依次开启
            pool.execute(run);
        }
    }
    LockTest lockTest = new LockTest();
    class MyRunnerForReentrantLock implements Runnable {
        @Override
        public void run() {
            lockTest.reEnterLock(new AtomicInteger(3));//在run方法中调用reEnterLock()方法测试重入测试
        }
    }
    class LockTest {
        ReentrantLock reentrantLock = new ReentrantLock();//使用默认的非公平锁ReentrantLock
        private void reEnterLock(AtomicInteger time) {
            reentrantLock.lock();//加锁
            Console.println(Thread.currentThread().getName() + "--" + time);
            try {
                if (time.get() == 0) {
                    return;
                } else {
                    time.getAndDecrement();
                    reEnterLock(time);//这里使用递归来测试重入
                }
            } finally {
                reentrantLock.unlock();//释放锁。注意这里在finally中释放锁避免加锁代码抛出异常导致锁无法释放造成阻塞
            }
        }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 执行结果如下，注意线程输出的顺序.

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
pool-1-thread-1--3
pool-1-thread-1--2
pool-1-thread-1--1
pool-1-thread-1--0

pool-1-thread-2--3
pool-1-thread-2--2
pool-1-thread-2--1
pool-1-thread-2--0

pool-1-thread-4--3
pool-1-thread-4--2
pool-1-thread-4--1
pool-1-thread-4--0

pool-1-thread-5--3
pool-1-thread-5--2
pool-1-thread-5--1
pool-1-thread-5--0

pool-1-thread-8--3
......
......
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 可以看出同一个线程中time变量从3、2、1、0依次循环，说明线程进入了循环体，那么线程确实是允许重入，同一个线程可以多次获取该锁。

但是注意以下线程输出的顺序却不是由1到10.而是 pool-1-thread-1、pool-1-thread-2、pool-1-thread-4、pool-1-thread-5、pool-1-thread-8.这就是因为ReentrantLock使用的非公平锁造成的，使用非公平锁的线程在获取“钥匙”的顺序上和线程开始等待的顺序是没有关系的。我们修改一下使用公平锁测试一下：修改以下代码：

```
        ReentrantLock reentrantLock = new ReentrantLock(true);//使用公平锁ReentrantLock
```

 执行结果如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
pool-1-thread-1--3
pool-1-thread-1--2
pool-1-thread-1--1
pool-1-thread-1--0
pool-1-thread-2--3
pool-1-thread-2--2
pool-1-thread-2--1
pool-1-thread-2--0
pool-1-thread-3--3
pool-1-thread-3--2
pool-1-thread-3--1
pool-1-thread-3--0
pool-1-thread-4--3
pool-1-thread-4--2
pool-1-thread-4--1
pool-1-thread-4--0
pool-1-thread-5--3
pool-1-thread-5--2
....
....
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 可以看出线程的执行顺序按照1、2、3、4的顺序进行输出。

除了上面的lock()方法外ReentrantLock还提供了两个重载的方法tryLock。ReentrantLock在进行等待持锁的时候不同于synchronized之处就在于ReentrantLock可以中断线程的等待，不再等待锁。其主要方法就是tryLock()的使用。

　　tryLock被重载了两个方法，方法签名为：

```
public boolean tryLock() {}
public boolean tryLock(long timeout, TimeUnit unit)throws InterruptedException {}
后者指定了等待超时时间

官方文档中注明了tryLock等待锁的机制：
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public boolean tryLock()
Acquires the lock only if it is not held by another thread at the time of invocation.
Acquires the lock if it is not held by another thread and returns immediately with the value true, setting the lock hold count to one. Even when this lock has been set to use a fair ordering policy, a call to tryLock() will immediately acquire the lock if it is available, whether or not other threads are currently waiting for the lock. This "barging" behavior can be useful in certain circumstances, even though it breaks fairness. If you want to honor the fairness setting for this lock, then use tryLock(0, TimeUnit.SECONDS) which is almost equivalent (it also detects interruption).
If the current thread already holds this lock then the hold count is incremented by one and the method returns true.
If the lock is held by another thread then this method will return immediately with the value false.
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public boolean tryLock(long timeout,  @NotNull TimeUnit unit) throws InterruptedException
Acquires the lock if it is not held by another thread within the given waiting time and the current thread has not been interrupted.
Acquires the lock if it is not held by another thread and returns immediately with the value true, setting the lock hold count to one. If this lock has been set to use a fair ordering policy then an available lock will not be acquired if any other threads are waiting for the lock. This is in contrast to the tryLock() method. If you want a timed tryLock that does permit barging on a fair lock then combine the timed and un-timed forms together:
       if (lock.tryLock() ||
          lock.tryLock(timeout, unit)) {
        ...
      }
If the current thread already holds this lock then the hold count is incremented by one and the method returns true.
If the lock is held by another thread then the current thread becomes disabled for thread scheduling purposes and lies dormant until one of three things happens:
The lock is acquired by the current thread; or
Some other thread interrupts the current thread; or
The specified waiting time elapses
If the lock is acquired then the value true is returned and the lock hold count is set to one.
If the current thread:
has its interrupted status set on entry to this method; or
is interrupted while acquiring the lock,
then InterruptedException is thrown and the current thread's interrupted status is cleared.
If the specified waiting time elapses then the value false is returned. If the time is less than or equal to zero, the method will not wait at all.
In this implementation, as this method is an explicit interruption point, preference is given to responding to the interrupt over normal or reentrant acquisition of the lock, and over reporting the elapse of the waiting time.
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 这里有中文的翻译：[Java中Lock，tryLock，lockInterruptibly有什么区别？(](https://www.zhihu.com/question/36771163)[郭无心的回答](https://www.zhihu.com/people/guo-wu-xin))
       **3、读写锁的使用**

　　　　对于读写锁的请求“钥匙”策略如下：

　　　　　　　　当写锁操作临界区资源时，其它新过来的线程一律等待，无论是读锁还是写锁。

　　　　　　　　当读锁操作临界区资源时，如果有读锁请求资源可以立即获取，不用等待；如果有写锁过来请求资源则需要等待读锁释放之后才可获取；如果有写锁在等待，然后又过来的有读锁，则读锁将会等待，写锁将会优先获取临界区资源操作权限，这样可以避免写线程的长期等待。

使用方法如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
    private void testReentrantRWLock() {
        MyRunnerForReentrantRWLock run = new MyRunnerForReentrantRWLock();
        for (int i = 0; i < 10; i++) {//开启10个线程测试

　　　　　　　sleep(10);//睡眠10ms保证线程开启的顺序能够按照1-10依次开启
            pool.execute(run);
        }
    }
    AtomicInteger num = new AtomicInteger(1);//用来切换读写锁测试方法
    ReentrantReadWriteLock rwlock = new ReentrantReadWriteLock(true);//公平读写锁
    private class MyRunnerForReentrantRWLock implements Runnable {
        @Override
        public void run() {
            if(num.getAndIncrement() ==3){
                lockTest.write();//调用写锁测试
            }else{
                lockTest.read();//调用读锁测试
            }
        }
    }
        public void read() {//使用读锁
            rwlock.readLock().lock();
            try {
                Console.println(Thread.currentThread().getName()+"------read");
　　　　　　　　　 sleep(2000);
            } finally {
                rwlock.readLock().unlock();
            }
        }
        public void write() {//使用写锁
            rwlock.writeLock().lock();
            try {
                sleep(2000);//模拟写操作
                Console.println(Thread.currentThread().getName()+"------write");
            }finally {
                rwlock.writeLock().unlock();
            }
        }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 执行结果如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
pool-1-thread-1------read
pool-1-thread-2------read
//在这里有明显的停顿，大约2s之后下面的直接输出，没有停顿
pool-1-thread-3------write
pool-1-thread-4------read
pool-1-thread-5------read
pool-1-thread-7------read
pool-1-thread-10------read
pool-1-thread-6------read
pool-1-thread-8------read
pool-1-thread-9------read
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 由运行结果执行顺序和时间可以看出，在进行write的时候其它读线程进行了等待操作，然后write释放之后，其它读操作同时操作临界区资源，未发生阻塞等待。
        **4、自旋锁**

　　　　自旋锁是在线程等待的时候通过自选while(){}空循环避免了线程挂起切换，减少了线程切换执行的时间。因此在选择使用自旋锁的时候尽量保证加锁代码的执行时间小于等待时间，这样就可以避免自旋锁大量占用CPU空转，同时又免去了非自旋锁线程切换的花销。如果加锁代码块较多，此时自旋锁就哟啊占用太多的CPU进行空转，此时如果发生大量线程请求锁则会大量浪费资源。用户可以根据具体情况来自定义自旋锁的实现，可以实现公平自旋锁和非公平自旋锁。

这里有介绍自定义自旋锁的实现方式：[Java锁的种类以及辨析（二）：自旋锁的其他种类](http://ifeve.com/java_lock_see2/)
    文章中介绍的很清楚了，`TicketLock `和`CLHLock 逻辑比较简单，`这里不再详述，只对MCSLock的实现做一下解读。其中原文中MCSLock的实现unlock()方法中在释放资源解锁下一个等待线程的机制有些问题，已经做出了修改，请注意辨别。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
package com.zpj.thread.blogTest.lock;

/**
 * Created by PerkinsZhu on 2017/8/16 18:01.
 */

import java.util.concurrent.atomic.AtomicReferenceFieldUpdater;

public class MCSLock {//这是通过链表实现对线程的控制的。每过来一个新的线程则把它添加到链表上阻塞进行while循环，当前一个线程结束之后，修改下一个线程的开关，开启下个线程持有锁。
    public static class MCSNode {
        volatile MCSNode next;
        volatile boolean isLocked = true;
    }
    private static final ThreadLocal<MCSNode> NODE = new ThreadLocal<MCSNode>();//这里保存的是当前线程的node，要理解ThreadLocal 的工作机制
    @SuppressWarnings("unused")
    private volatile MCSNode queue;
    private static final AtomicReferenceFieldUpdater<MCSLock, MCSNode> UPDATER = AtomicReferenceFieldUpdater.newUpdater(MCSLock.class, MCSNode.class, "queue");

    public void lock() {
        MCSNode currentNode = new MCSNode();//过来一个新线程创建一个node，同时防止在当前线程的NODE中进行保存。
        NODE.set(currentNode);//注意，这里的NODE存储的数据各个线程中是不共享的
        MCSNode preNode = UPDATER.getAndSet(this, currentNode);//获取前一个node节点，并更新当前节点
        if (preNode != null) {//前一个节点存在说明有线程正在操作临界区资源。则当前线程循环等待
            preNode.next = currentNode;//把当前节点加入到链表中，等待获取资源
            while (currentNode.isLocked) {}//循环等待，直至前一个线程释放资源，修改当前node的isLocked标志位
        }
    }

    public void unlock() {
        MCSNode currentNode = NODE.get();//取出当前线程的node节点
        if (currentNode.next == null) {//如果没有新的线程等待持锁
            if (UPDATER.compareAndSet(this, currentNode, null)) {//把当前node释放，如果成功则结束，如果失败进入else
            } else { //设置失败说明突然有线程在请求临界区资源进行等待。此时有新的线程更新了UPDATER数据。
　　　　　　　　//***********************注意下面的逻辑，已经进行修改  【start】*********************************
                while (currentNode.next == null) {}//等待新加入的线程把节点加入链表
                // 此时currentNode.next ！= null 这里理应使用锁资源，而不应该直接结束，不然等待的线程无法获取“钥匙”访问临界区资源。所以添加以下两行代码释放锁资源
                currentNode.next.isLocked = false;//释放新添加线程的等待
                currentNode.next = null;
　　　　　　　　　//********************************** end  ******************************
            }
        } else {
            currentNode.next.isLocked = false;//释放下一个等待锁的线程
            currentNode.next = null;
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　**5、信号量实现锁效果**

　　在jdk中，除了以上提供的Lock之外，还有信号量Semaphore也可以实现加锁特性。Semaphore是控制访问临界区资源的线程数量，Semaphore设置一个允许同时操作临界区资源的阈值，如果请求的线程在阈值之内则允许所有线程同时访问临界区资源，如果超出设置的该阈值则挂起等待，直至有线程退出释放之后，才允许新的资源获得操作临界区资源的权利。如果需要把它当做锁使用，则只需要设置该阈值为1，即任意时刻只允许一个线程对临界区资源进行操作即可。虽然不是锁，但却实现了锁的功能——线程互斥串行。

使用示例：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```java
Semaphore semaphore = new Semaphore(1);//同时只允许一个线程可以访问临界区资源
    private void testSemaphore(){
        for(int i = 0; i<5;i++){//开启5个线程竞争资源
            pool.execute(new SemapRunner());
        }
    }
    class SemapRunner implements Runnable{
        @Override
        public void run() {
            try {
                Console.println(Thread.currentThread().getName()+"  请求资源");
                semaphore.acquire();//请求资源
                Console.println(Thread.currentThread().getName()+"  获取到资源");
                sleep(2000);
                Console.println(Thread.currentThread().getName()+"  释放资源");
                semaphore.release();//释放资源
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 运行结果如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
pool-1-thread-2  请求资源
pool-1-thread-4  请求资源
pool-1-thread-2  获取到资源
pool-1-thread-5  请求资源
pool-1-thread-1  请求资源
pool-1-thread-3  请求资源
pool-1-thread-2  释放资源
pool-1-thread-4  获取到资源
pool-1-thread-4  释放资源
pool-1-thread-5  获取到资源
pool-1-thread-5  释放资源
pool-1-thread-1  获取到资源
pool-1-thread-1  释放资源
pool-1-thread-3  获取到资源
pool-1-thread-3  释放资源
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 由结果可以看出，只有当一个线程释放资源之后，才允许一个等待的资源获取到资源，这样便实现了类似加锁的操作。

 

　　在进行线程操作的过程中需要根据实际情况选取不同的锁机制来对线程进行控制，以保证数据、执行逻辑的正确！！！无论是使用synchronized锁还是使用jdk提供的锁亦或自定义锁，都要清晰明确使用锁的最终目的是什么，各种锁的特性是什么，使用场景分别是什么？这样才能够在线程中熟练运用各种锁。

 

 

## [(六)线程间的通信和协作](https://www.cnblogs.com/PerkinsZhu/p/7439330.html)

Posted on

 

2017-08-30 14:32

 

PerKins.Zhu

 

阅读(

5405

) 评论(

3

)

 

编辑

 

收藏

　　系统要实现某个全局功能必定要需要各个子模块之间的协调和配合，就像一个团队要完成某项任务的时候需要团队各个成员之间密切配合一样。而对于系统中的各个子线程来说，如果要完成一个系统功能，同样需要各个线程的配合，这样就少不了线程之间的通信与协作。常见的线程之间通信方式有如下几种：

　　**1、wait和notify/notifyAll**

　　 **2、await和signal/signalAll**

 　　**3、sleep/yield/join**

　　 **4、CyclicBarrier 栅栏**

　　 **5、CountDownLatch 闭锁**

　　 **6、Semaphore 信号量**

 

**一、wait和notify/notifyAll**

在使用之前先明确 ：

　　　wait和notify是Object的方法，任何一个对象都具有该方法。在使用的时候，首先需要设置一个全局锁对象，通过对该锁的释放和持有来控制该线程的运行和等待。因此在调用wait和notify的时候，该线程必须要已经持有该锁，然后才可调用，否则将会抛出IllegalMonitorStateException异常。
           确定要让哪个线程等待？让哪个线程等待就在哪个线程中调用锁对象的wait方法。调用wait等待的是**当前线程**，而**不是被调用线程,**并不是theread.wait（）就可以让thread等待，而是让当前线程（调用wait方法的线程，不是调用者）进行等待。尽量不要把线程对象当做全局锁使用，以免混淆等待线程。
看一下使用方法：（代码中省略了main方法，对sleep()和println()方法进行了封装）

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
package thread.blogs.cooperation;

import scala.Console;

import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by PerkinsZhu on 2017/8/21 10:25.
 */
public class TestWaitAndNotify {
    public static void main(String[] args) {
        TestWaitAndNotify test = new TestWaitAndNotify();
        test.testWait();
    }

    Object obj = new Object();//创建一个全局变量，用来协调各个线程
    ThreadLocal<AtomicInteger> num = new ThreadLocal<AtomicInteger>();//设置一个线程wait和notify的触发条件
    class MyRunner implements Runnable {
        @Override
        public void run() {
            num.set(new AtomicInteger(0));
            while (true) {
                Console.println(Thread.currentThread().getName());
                if (num.get().getAndIncrement() == 1) {
                    synchronized (obj) {//如果要想调用wait方法，则必须持有该对象。否则将会抛出IllegalMonitorStateException
                        try {
                            Console.println(Thread.currentThread().getName() + "挂起等待");
                            obj.wait();//同一个线程可以wait多次，多个线程也可以使用同一个obj调用wait
                            Console.println(Thread.currentThread().getName() + "唤醒！！！");
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
                sleep(1000);
            }
        }
    }

    private void testWait() {
        MyRunner runner = new MyRunner();
        new Thread(runner).start();
        new Thread(runner).start();

        AtomicInteger num03 = new AtomicInteger(0);
        Thread th03 = new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    synchronized (obj) {//调用notify/notifyAll和wait一样，同样需要持有该对象
                        if (num03.getAndIncrement() == 5) {
                            obj.notify();//唤醒最先一个挂在obj上面的线程.每次只唤醒一个。这里是按照等待的先后顺序进行唤醒
                        }
                    }
                    sleep(1000);
                }
            }
        });
        th03.start();
    }

    private void sleep(int time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

   运行结果如下:

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
Thread-1
Thread-0
Thread-1
Thread-0
Thread-1挂起等待
Thread-0挂起等待
Thread-1唤醒！！！
Thread-1
Thread-1
Thread-1
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 从执行结果只中可以看出，在执行两次输出之后，两个线程被分别挂起等待。过一会之后线程1被成功唤醒。这里之所以唤醒的是Thread-1是因为Thread-1是第一个先挂起的，所以在notify()方法在唤起wait线程的时候也是公平的，依照wait的挂起顺序来执行唤醒。

在使用wait的时候，同一个obj可以被多个线程调用obj.wait()，也可以被同一个线程执行多次obj.wait();

例如，修改try catch代码代码块

```
                            Console.println(Thread.currentThread().getName() + "挂起等待");
                            obj.wait();//执行多次wait操作
                            obj.wait();
                            obj.wait();
                            Console.println(Thread.currentThread().getName() + "唤醒！！！");
```

然后只启动一个线程

```
        new Thread(runner,"thread--01").start();
//        new Thread(runner,"thread--02").start();
```

 执行结果如下：

```
thread--01
thread--01
thread--01挂起等待
```

 线程一直停滞在此处，无法继续执行，这是因为线程调用了三此wait，而如果要想成功唤醒线程，则同样需要调用三次notify或者调用一次notifyAll()。这里就不再列出代码。

wait方法有两个重载方法：

```
public final native void wait(long timeout) throws InterruptedException;
public final void wait(long timeout, int nanos) throws InterruptedException ；

```

两个方法都是wait指定时间之后，如果依旧没有被其它线程唤醒或者被中断则会自动停止wait。其中第二个方法指定了时间的单位。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
public final void wait(long timeout,
                       int nanos)
               throws InterruptedException
Causes the current thread to wait until another thread invokes the notify() method or the notifyAll() method for this object, or some other thread interrupts the current thread, or a certain amount of real time has elapsed.
This method is similar to the wait method of one argument, but it allows finer control over the amount of time to wait for a notification before giving up. The amount of real time, measured in nanoseconds, is given by:
       1000000*timeout+nanos
In all other respects, this method does the same thing as the method wait(long) of one argument. In particular, wait(0, 0) means the same thing as wait(0).
The current thread must own this object's monitor. The thread releases ownership of this monitor and waits until either of the following two conditions has occurred:
Another thread notifies threads waiting on this object's monitor to wake up either through a call to the notify method or the notifyAll method.
The timeout period, specified by timeout milliseconds plus nanos nanoseconds arguments, has elapsed.
The thread then waits until it can re-obtain ownership of the monitor and resumes execution.
As in the one argument version, interrupts and spurious wakeups are possible, and this method should always be used in a loop:
           synchronized (obj) {
               while (<condition does not hold>)
                   obj.wait(timeout, nanos);
               ... // Perform action appropriate to condition
           }
       
This method should only be called by a thread that is the owner of this object's monitor. See the notify method for a description of the ways in which a thread can become the owner of a monitor.
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

　　注意这里的synchronized的目的不是加锁控制线程的串行，而是为了持有锁来调用wait和notify对象。
　　 在理解这线程调用obj.wait()的时候可以理解为"挂在obj对象上的线程"，而对于线程调用obj.notify()可以理解为"唤起最后一个挂在obj上面的那个线程"，而对于线程调用obj.notifyAll()，则可以理解为"唤起所有挂在obj对象上的线程"。obj对象在这里起的作用就是一个信息载体中介。各个线程通过这个中介进行通行协作，控制线程之间的暂停和执行。

**二、await和signal/signalAll**

　　await和signal是Condition的两个方法，其作用和wait和notify一样，目的都是让线程挂起等待，不同的是，这两种方法是属于Condition的两个方法，而Condition对象是由ReentrantLock调用newCondition()方法得到的。Condition对象就相当于前面所说的中介，在线程中调用contiton.await()和condition.signal()可以分别使线程等待和唤醒。

如需要了解tryLock的使用可以看这里：[多线程(五) java的线程锁](http://www.cnblogs.com/PerkinsZhu/p/7392006.html)

 使用示例：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
package thread.blogs.cooperation;

import scala.Console;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created by PerkinsZhu on 2017/8/21 11:59.
 */
public class TestCondition {
    public static void main(String[] args) {
        TestCondition test = new TestCondition();
        test.testWait();
    }

    ReentrantLock lock = new ReentrantLock();
    ThreadLocal<AtomicInteger> num = new ThreadLocal<AtomicInteger>();
    Condition condition = lock.newCondition();

    private void testWait() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                num.set(new AtomicInteger(1));
                while (true) {
                    if (num.get().getAndIncrement() == 5) {
                        Console.println("signal---！！！");
                        try {
                            lock.lock();
                            condition.signal();
                        } finally {
                            lock.unlock();
                        }
                    }
                    Console.println("thread ---- 01");
                    sleep(1000);
                }
            }
        }).start();

        new Thread(new Runnable() {
            @Override
            public void run() {
                num.set(new AtomicInteger(1));
                while (true) {
                    if (num.get().getAndIncrement() == 2) {
                        try {
                           //lock.tryLock();
                            //lock.tryLock(5000, TimeUnit.MILLISECONDS);
                            lock.lock();//这里同样要加锁，否则会抛出IllegalMonitorStateException异常。注意的是这里不要使用synchronized进行加锁，而是使用lock
                            condition.await();//注意这里不要调用wait!!!
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }finally {
                            lock.unlock();
                        }
                    }
                    Console.println("thread ---- 02");
                    sleep(1000);
                }
            }
        }).start();
    }
    private void sleep(int time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  在使用Condition的时候和Synchronized没有太大的区别，只是调用的方法变为await和signal。需要注意的是这里加锁不再使用synchronized()进行加锁，而是使用lock和unlock进行加锁。

 执行结果如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
thread ---- 01
thread ---- 02
thread ---- 01
thread ---- 01
thread ---- 01
signal---！！！
thread ---- 01
thread ---- 02
thread ---- 02
thread ---- 01
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  **三、sleep/yield/join**

对于sleep()方法应该很熟悉了，让当前线程睡眠一段时间。期间**不会释放任何持有的锁**。

```
public static native void sleep(long millis) throws InterruptedException;
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1     /**
 2      * Causes the currently executing thread to sleep (temporarily cease
 3      * execution) for the specified number of milliseconds, subject to
 4      * the precision and accuracy of system timers and schedulers. The thread
 5      * does not lose ownership of any monitors.
 6      *
 7      * @param  millis
 8      *         the length of time to sleep in milliseconds
 9      *
10      * @throws  IllegalArgumentException
11      *          if the value of {@code millis} is negative
12      *
13      * @throws  InterruptedException
14      *          if any thread has interrupted the current thread. The
15      *          <i>interrupted status</i> of the current thread is
16      *          cleared when this exception is thrown.
17      */
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  对于yield()方法可能使用的情况少一下。其作用主要是让当前线程从运行状态转变为就绪状态，由线程调度重新选择就绪状态的线程分配CPU资源。至于最终会选取哪个线程分配CPU资源就由调度策略来决定了，有可能还是该线程，有可能换为其它线程。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1   /**
 2      * A hint to the scheduler that the current thread is willing to yield
 3      * its current use of a processor. The scheduler is free to ignore this
 4      * hint.
 5      *
 6      * <p> Yield is a heuristic attempt to improve relative progression
 7      * between threads that would otherwise over-utilise a CPU. Its use
 8      * should be combined with detailed profiling and benchmarking to
 9      * ensure that it actually has the desired effect.
10      *
11      * <p> It is rarely appropriate to use this method. It may be useful
12      * for debugging or testing purposes, where it may help to reproduce
13      * bugs due to race conditions. It may also be useful when designing
14      * concurrency control constructs such as the ones in the
15      * {@link java.util.concurrent.locks} package.
16      */
17     public static native void yield();
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  对于join方法，作用是暂停当前线程，等待被调用线程指向结束之后再继续执行。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1  /**
 2      * Waits for this thread to die.
 3      *
 4      * <p> An invocation of this method behaves in exactly the same
 5      * way as the invocation
 6      *
 7      * <blockquote>
 8      * {@linkplain #join(long) join}{@code (0)}
 9      * </blockquote>
10      *
11      * @throws  InterruptedException
12      *          if any thread has interrupted the current thread. The
13      *          <i>interrupted status</i> of the current thread is
14      *          cleared when this exception is thrown.
15      */
16     public final void join() throws InterruptedException；
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  使用join的时候需要注意：

　　1、调用join的时候，当前线程不会释放掉锁，如果调用线程也需要该锁则就会导致死锁！

　　2、join方法不会启动调用线程，所以，在调用join之前，该调用线程必须已经start启动，否则不会达到想要的效果。

join的底层实际是就是使用了一个自旋等待机制，判断调用线程是否死亡，如果没有则一直让当前线程wait。可以看一下底层实现源码：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 public final synchronized void join(long millis) throws InterruptedException {
 2         long base = System.currentTimeMillis();
 3         long now = 0;
 4         if (millis < 0) {
 5             throw new IllegalArgumentException("timeout value is negative");
 6         }
 7         if (millis == 0) {
 8             while (isAlive()) {//如果调用者依旧没有结束,让当前线程进行等待
 9                 wait(0);//注意这里的wait是等待的当前线程，而不是调用者线程
10             }
11         } else {
12             while (isAlive()) {
13                 long delay = millis - now;
14                 if (delay <= 0) {
15                     break;
16                 }
17                 wait(delay);//指定等待的时间
18                 now = System.currentTimeMillis() - base;
19             }
20         }
21     }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  **四、CyclicBarrier栅栏**

　　CyclicBarrier字面理解为线程屏障，当指定数量的线程执行到指定位置的时候，才能触发后续动作的进行。其最终目的是让所有线程同时开始后续的工作。

例如：三个员工来公司开会，由于三人住的地方与公司距离不同，所以到会议室的时间也不同。而会议开始必须等待三者都到达会议室之后才能进行。

代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package thread.blogs.cooperation;
 2 
 3 import scala.Console;
 4 
 5 import java.util.concurrent.CyclicBarrier;
 6 
 7 /**
 8  * Created by PerkinsZhu on 2017/8/30 10:32.
 9  */
10 public class TestCyclicBarrier {
11     public static void main(String[] args) {
12         testCyclicBarrier();
13     }
14 
15     private static void testCyclicBarrier() {
16         /**
17          * 注意这里等待的是三个线程。这就相当于一个线程计数器，当指定个数的线程执行 barrier.await();方法之后，才会执行后续的代码，否则每个线程都会一直进行等待。
18          * 如果把3修改为4，则将永远等待下去，不会开始会议。
19          * 如果把3修改为2，则小张到达之后就会提前开始会议，不会继续等待小王。
20          */
21         CyclicBarrier barrier = new CyclicBarrier(3);
22 
23         Thread 小李 = new Thread(new MyRunner(barrier, "小李", 2000));
24         小李.start();
25         Thread 小张 = new Thread(new MyRunner(barrier, "小张", 4000));
26         小张.start();
27         Thread 小王 = new Thread(new MyRunner(barrier, "小王", 5000));
28         小王.start();
29     }
30 
31     static class MyRunner implements Runnable {
32         CyclicBarrier barrier;
33         String name;
34         int time;
35 
36         public MyRunner(CyclicBarrier barrier, String name, int time) {
37             this.barrier = barrier;
38             this.name = name;
39             this.time = time;
40         }
41 
42         @Override
43         public void run() {
44             Console.println(name + " 开始出发去公司。");
45             sleep(time);
46             Console.println(name + " 终于到会议室！！！");
47             try {
48                 barrier.await();
49             } catch (Exception e) {
50                 e.printStackTrace();
51             }
52             startMeeting(name);
53         }
54     }
55 
56     private static void startMeeting(String name) {
57         Console.println(name + "说：人齐了。会议开始！！");
58     }
59 
60     private static void sleep(int time) {
61         try {
62             Thread.sleep(time);
63         } catch (InterruptedException e) {
64             e.printStackTrace();
65         }
66     }
67 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  运行结果：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 小李 开始出发去公司。
2 小王 开始出发去公司。
3 小张 开始出发去公司。
4 小李 终于到会议室！！！
5 小张 终于到会议室！！！
6 小王 终于到会议室！！！
7 小王说：人齐了。会议开始！！
8 小李说：人齐了。会议开始！！
9 小张说：人齐了。会议开始！！
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  在使用CyclicBarrier的时候，提供了一个重载的构造器。

```
public CyclicBarrier(int parties, Runnable barrierAction) {}
barrierAction会在一组线程中的最后一个线程到达之后（但在释放所有线程之前）触发。
例如修改上面的代码21行为：
        CyclicBarrier barrier = new CyclicBarrier(3, new Runnable() {
            @Override
            public void run() {
                Console.println("======");
            }
        });
```

   运行结果：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
小张 开始出发去公司。
小李 开始出发去公司。
小王 开始出发去公司。
小李 终于到会议室！！！
小张 终于到会议室！！！
小王 终于到会议室！！！
======
小王说：人齐了。会议开始！！
小李说：人齐了。会议开始！！
小张说：人齐了。会议开始！！
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 **五、CountDownLatch闭锁**

与CycliBarrier不同的是CountDownLatch是某一个线程等待其他线程执行到某一位置之后，该线程（调用countDownLatch.await();等待的线程）才会继续后续工作。而CycliBarrier是各个线程执行到某位置之后，然后**所有线程一齐开始**后续的工作。相同的是两者都属于线程计数器。

使用示例如下： boss等待所有员工来开会，当所有人员都到齐之后，boss宣布开始会议！！！

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package thread.blogs.cooperation;
 2 
 3 import scala.Console;
 4 
 5 import java.util.concurrent.CountDownLatch;
 6 
 7 /**
 8  * Created by PerkinsZhu on 2017/8/30 10:32.
 9  */
10 public class TestCyclicBarrier {
11     public static void main(String[] args) {
12         testCyclicBarrier();
13     }
14 
15     private static void testCyclicBarrier() {
16 
17         CountDownLatch countDownLatch = new CountDownLatch(3);//注意这里的参数指定了等待的线程数量
18 
19         new Thread(new MyRunner(countDownLatch, "小李", 2000)).start();
20         new Thread(new MyRunner(countDownLatch, "小张", 4000)).start();
21         new Thread(new MyRunner(countDownLatch, "小王", 5000)).start();
22 
23         try {
24             Console.println("等待员工到来开会。。。。。。。");
25             countDownLatch.await();//注意这里是await。主线程将会一直等待在这里，当所有线程都执行 countDownLatch.countDown();之后当前线程才会继续执行
26             startMeeting("Boss");
27         } catch (InterruptedException e) {
28             e.printStackTrace();
29         }
30     }
31 
32     static class MyRunner implements Runnable {
33         CountDownLatch countDownLatch;
34         String name;
35         int time;
36 
37         public MyRunner(CountDownLatch countDownLatch, String name, int time) {
38             this.countDownLatch = countDownLatch;
39             this.name = name;
40             this.time = time;
41         }
42 
43         @Override
44         public void run() {
45             Console.println(name + " 开始出发去公司。");
46             sleep(time);
47             Console.println(name + " 终于到会议室！！！");
48             countDownLatch.countDown();
　　　　　　　　 Console.println(name + " 准备好了！！");
49         }
50     }
51 
52     private static void startMeeting(String name) {
53         Console.println(name + "说：人齐了。会议开始！！");
54     }
55 
56     private static void sleep(int time) {
57         try {
58             Thread.sleep(time);
59         } catch (InterruptedException e) {
60             e.printStackTrace();
61         }
62     }
63 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  执行结果如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
等待员工到来开会。。。。。。。
小王 开始出发去公司。
小张 开始出发去公司。
小李 开始出发去公司。
小李 终于到会议室！！！
小李 准备好了！！
小张 终于到会议室！！！
小张 准备好了！！
小王 终于到会议室！！！
小王 准备好了！！
Boss说：人齐了。会议开始！！
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  注意区分是某一个线程等待其他线程还是所有线程在达到某一条件之后一起执行！！！

 **6、Semaphore 信号量**

　　Semaphore在线程协作方面主要用于**控制同时访问临界区资源的线程个数**。信号量是属于操作系统层面的概念，jdk提供了操作接口。

 使用示例如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package thread.blogs.cooperation;
 2 
 3 import scala.Console;
 4 
 5 import java.util.concurrent.ExecutorService;
 6 import java.util.concurrent.Executors;
 7 import java.util.concurrent.Semaphore;
 8 
 9 /**
10  * Created by PerkinsZhu on 2017/8/30 11:43.
11  */
12 public class TestSemaphore {
13     public static void main(String[] args) {
14         testSemaphore();
15     }
16 
17     private static void testSemaphore() {
18         Semaphore semaphore = new Semaphore(2, true);//指定同时访问临界区资源的线程数量。第二个参数指定以公平方式访问临界区资源
19         ExecutorService excutorService = Executors.newFixedThreadPool(10);
20         for (int i = 0; i < 6; i++) {//启动10个线程请求资源
21             excutorService.execute(new MyRunner(semaphore));
22             sleep(0);//逐个启动线程
23         }
24         excutorService.shutdown();
25     }
26 
27     static class MyRunner implements Runnable {
28         Semaphore semaphore;
29 
30         public MyRunner(Semaphore semaphore) {
31             this.semaphore = semaphore;
32         }
33 
34         @Override
35         public void run() {
36             String name = Thread.currentThread().getName();
37             try {
38                 Console.println(name + "  ------请求资源！！");
39                 //semaphore.acquire(2);//设置请求资源的数量。必须有足够数量的资源才可进去临界区。不过释放的时候也要一起释放，请求几个就要调用几次release()
40                 semaphore.acquire();//请求获取资源，如果有空闲资源则会立即获取，进入临界区，否则将会等待，一直等待到获取到临界区资源
41                 Console.println(name + "  ======获取资源！！");
42                 sleep(1000);
43                 //semaphore.release();
44                 semaphore.release();//释放资源
45                 Console.println(name + "  ******释放资源！！");
46 
47             } catch (InterruptedException e) {
48                 e.printStackTrace();
49             }
50         }
51     }
52 
53     private static void sleep(int time) {
54         try {
55             Thread.sleep(time);
56         } catch (InterruptedException e) {
57             e.printStackTrace();
58         }
59     }
60 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  执行结果如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
pool-1-thread-1  ------请求资源！！
pool-1-thread-2  ------请求资源！！
pool-1-thread-6  ------请求资源！！
pool-1-thread-5  ------请求资源！！
pool-1-thread-3  ------请求资源！！
pool-1-thread-4  ------请求资源！！
pool-1-thread-2  ======获取资源！！
pool-1-thread-1  ======获取资源！！
pool-1-thread-1  ******释放资源！！
pool-1-thread-6  ======获取资源！！
pool-1-thread-5  ======获取资源！！
pool-1-thread-2  ******释放资源！！
pool-1-thread-6  ******释放资源！！
pool-1-thread-4  ======获取资源！！
pool-1-thread-3  ======获取资源！！
pool-1-thread-5  ******释放资源！！
pool-1-thread-4  ******释放资源！！
pool-1-thread-3  ******释放资源！！
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 根据结果可以看出只有当有线程释放资源之后，才会有新的线程获取到资源。即控制了同一时间访问临界区资源的线程数量。当Semaphore(1)设置为1的时候,此时可以当做锁来使用。[多线程(五) java的线程锁](http://www.cnblogs.com/PerkinsZhu/p/7392006.html)

 

 线程之间的通信和协作方式大概就以上六种，要熟悉每种工具的使用场景和方法特性，通过灵活的组合各个工具来灵活控制各个线程的工作。在决定使用哪种工具之前必须要**明确自己的目的是什么**，要实现什么样的机制，这样才能确定选择哪种工具协调各个线程。

 





## [(七)JDK原生线程池](https://www.cnblogs.com/PerkinsZhu/p/7466439.html)

Posted on

 

2017-09-04 19:48

 

PerKins.Zhu

 

阅读(

517

) 评论(

0

)

 

编辑

 

收藏

如同数据库连接一样，线程的创建、切换和销毁同样会耗费大量的系统资源。为了复用创建好的线程，减少频繁创建线程的次数，提高线程利用率可以引用线程池技术。使用线程池的优势有如下几点：
        1、保持一定数量的线程，减少了线程频繁创建和销毁资源消耗。
        2、使用线程的时候直接由线程池中取出线程，省去了创建线程的时间，侧面提高了系统的响应时间。
        3、需要使用线程的时候直接从线程池中取出，避免了人为的不合理创建线程，减少了潜在的风险。
Doug Lea在实现JUC中提供了原生的线程池，并提供了各种线程管理策略来适应于不同的使用场景。使用的时候可通过Executors获取各种线程池实例。

![img](https://images2017.cnblogs.com/blog/837242/201709/837242-20170902085018062-563596047.png)

这里提供了六对12个方法来创建ExecutorService，其中每种类型的ExecutorService可以适用于不同的应用场景，对线程的管理策略也各不相同。下面就看一下各个方法的注释：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
/**
     * Creates a thread pool that creates new threads as needed, but will reuse previously constructed threads when they are available.
     * 创建一个线程池，需要的时候会创建新的线程，如果有可用的线程则会复用以前已经创建好的线程。
     * These pools will typically improve the performance of programs that execute many short-lived asynchronous tasks.
     * 这些线程池通常情况下可以提升哪些短期异步任务的性能
     * Calls to {@code execute} will reuse previously constructed threads if available.
     * 如果以创建的线程状态可用的话，调用execute可以复用他们
     * If no existing thread is available, a new thread will be created and added to the pool.
     * 如果不存在可用状态的线程，那么将会创建一个新线程同时会把该线程添加到线程池中
     * Threads that have not been used for sixty seconds are terminated and removed from the cache.
     *那些超过60s没用的线程将会被销毁同时从缓存中移除
     * Thus, a pool that remains idle for long enough will not consume any resources.
     *因此长时间空闲的线程池不会消耗任何资源
     * Note that pools with similar properties but different details (for example, timeout parameters) may be created using {@link ThreadPoolExecutor} constructors.
     *可以使用ThreadPoolExecutor创建性质相似但实现细节不同的线程池
     * @return the newly created thread pool
     */

    public static ExecutorService newCachedThreadPool();

    //可以使用自定义的ThreadFactory 类创建线程，其它和无参方法一致
    public static ExecutorService newCachedThreadPool(ThreadFactory threadFactory);
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
/**
     * Creates a thread pool that reuses a fixed number of threads operating off a shared unbounded queue.
     *创建一个可重用、固定数量线程的线程池
     * At any point, at most {@code nThreads} threads will be active processing tasks.
     *任何时间最多只有 nThreads 个线程被激活来执行任务
     * If additional tasks are submitted when all threads are active, they will wait in the queue until a thread is available.
     * 当无可用空闲线程的时候，如果有新任务被提交，这些新任务将会一直等待直至有可用线程来执行。
     * If any thread terminates due to a failure during execution  prior to shutdown, a new one will take its place if needed to execute subsequent tasks.
     *如果任何线程正常关闭之前在执行过程中因失败而提前终止，那么如果有未被执行的后续任务，则会创建新的线程来继续执行。
     * The threads in the pool will exist  until it is explicitly {@link ExecutorService#shutdown shutdown}.
     * 线程池中的所有线程在明确掉用shutdown之后将会退出
     *
     * @param nThreads the number of threads in the pool
     * @return the newly created thread pool
     * @throws IllegalArgumentException if {@code nThreads <= 0}
     */
    public static ExecutorService newFixedThreadPool(int nThreads);
    //可自定义ThreadFactory
    public static ExecutorService newFixedThreadPool(int nThreads, ThreadFactory threadFactory);
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
/**
     * Creates a thread pool that can schedule commands to run after a given delay, or to execute periodically.
     *  创建一个线程池，该线程在延迟指定时间之后可以周期性的执行线程体
     * @param corePoolSize the number of threads to keep in the pool,
     * even if they are idle
     * @return a newly created scheduled thread pool 注意返回值类型是ScheduledExecutorService，不要使用ExecutorService来接收，否则找不到schedule执行方法
     * @throws IllegalArgumentException if {@code corePoolSize < 0}
     */

    public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize);
    //可自定义ThreadFactory
    public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize, ThreadFactory threadFactory);
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
/**
     * Creates an Executor that uses a single worker thread operating off an unbounded queue.
     *创建一个Executor，使用一个线程来工作，该线程存储在LinkedBlockingQueue中
     * (Note however that if this single thread terminates due to a failure during execution prior to shutdown, a new one will take its place if needed to execute subsequent tasks.)
     *注，如果任何线程正常关闭之前在执行过程中因失败而提前终止，那么如果有未被执行的后续任务，则会创建新的线程来继续执行。
     * Tasks are guaranteed to execute sequentially, and no more than one task will be active at any given time.
     *  任务是按顺序执行的，任何时间都只有一个线程来执行任务
     * Unlike the otherwise equivalent {@code newFixedThreadPool(1)} the returned executor is guaranteed not to be reconfigurable to use additional threads.
     *
     * @return the newly created single-threaded Executor
     */

    public static ExecutorService newSingleThreadExecutor();
    //可自定义ThreadFactory
    public static ExecutorService newSingleThreadExecutor(ThreadFactory threadFactory);
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
/**
     * Creates a single-threaded executor that can schedule commands
     * to run after a given delay, or to execute periodically.
     * (Note however that if this single
     * thread terminates due to a failure during execution prior to
     * shutdown, a new one will take its place if needed to execute
     * subsequent tasks.)  Tasks are guaranteed to execute
     * sequentially, and no more than one task will be active at any
     * given time. Unlike the otherwise equivalent
     * {@code newScheduledThreadPool(1)} the returned executor is
     * guaranteed not to be reconfigurable to use additional threads.
     * @return the newly created scheduled executor
     */

    public static ScheduledExecutorService newSingleThreadScheduledExecutor();

    public static ScheduledExecutorService newSingleThreadScheduledExecutor(ThreadFactory threadFactory)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 使用示例：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
package thread.blogs.threadpool;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by PerkinsZhu on 2017/8/31 13:59.
 */
public class PoolTest {

    public static void main(String[] args) {
//        testCachedThreadPool();
//        testSingleThreadExecutor();
//        testFixedThreadPool();
        testScheduledThreadPool();
    }
    private static ThreadFactory myFactory = new MyThreadFactory();

    private static void testSingleThreadExecutor() {
        //一个一个的依次执行
        doHandle(Executors.newSingleThreadExecutor(myFactory));
    }

    private static void testFixedThreadPool() {
        //两个两个的一起执行
        doHandle(Executors.newFixedThreadPool(2,myFactory));
    }

    private static void testCachedThreadPool() {
        //10个一起一次性执行完
        doHandle(Executors.newCachedThreadPool(myFactory));
    }

    private static void testScheduledThreadPool() {
        //定时周期执行
        Executors.newScheduledThreadPool(1,myFactory).scheduleAtFixedRate(runnable, 500, 2000, TimeUnit.MILLISECONDS);
    }

    private static Runnable runnable = () -> {
        sleep(1000);
        System.out.println(Thread.currentThread().getName() + "    work!!!");
    };


    private static void doHandle(ExecutorService executorService) {
        for (int i = 0; i < 10; i++) {
            executorService.execute(runnable);
        }
        executorService.shutdown();
    }

    private static void sleep(int time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

class MyThreadFactory implements ThreadFactory {//自定义ThreadFactory
    private static final AtomicInteger poolNumber = new AtomicInteger(1);
    private final ThreadGroup group;
    private final AtomicInteger threadNumber = new AtomicInteger(1);
    private final String namePrefix;

    MyThreadFactory() {
        SecurityManager s = System.getSecurityManager();
        group = (s != null) ? s.getThreadGroup() : Thread.currentThread().getThreadGroup();
        namePrefix = "☆☆☆--" + poolNumber.getAndIncrement() + "-****-";
    }

    public Thread newThread(Runnable r) {
        Thread t = new Thread(group, r, namePrefix + threadNumber.getAndIncrement(), 0);
        if (t.isDaemon()) {
            t.setDaemon(false);
        }
        if (t.getPriority() != Thread.NORM_PRIORITY) {
            t.setPriority(Thread.NORM_PRIORITY);
        }
        return t;
    }

}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 在使用的时候必须明白各种线程池是否适用于自己的情景，选取合适的线程池进行使用。

 



## [(八)常用的线程模型](https://www.cnblogs.com/PerkinsZhu/p/7570775.html)

Posted on

 

2017-09-22 10:16

 

PerKins.Zhu

 

阅读(

5643

) 评论(

2

)

 

编辑

 

收藏

　　在处理业务的时候，有时候需要根据情况使用不同的线程处理模型来处理业务逻辑，这里演示一下常见的线程模型使用技巧。

**1、Future模型**

　　前面的章节中提到过Future模型，该模型通常在使用的时候需要结合Callable接口配合使用。Future：未来的、将来的，再结合Callable大概可以明白其功能。

　　Future是把结果放在将来获取，当前主线程并不急于获取处理结果。允许子线程先进行处理一段时间，处理结束之后就把结果保存下来，当主线程需要使用的时候再向子线程索取。

　　Callable是类似于Runnable的接口，其中call方法类似于run方法，所不同的是**run方法不能抛出受检异常没有返回值，而call方法则可以抛出受检异常并可设置返回值**。两者的方法体都是线程执行体。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
    /**
     * When an object implementing interface <code>Runnable</code> is used
     * to create a thread, starting the thread causes the object's
     * <code>run</code> method to be called in that separately executing
     * thread.
     * <p>
     * The general contract of the method <code>run</code> is that it may
     * take any action whatsoever.
     *
     * @see     java.lang.Thread#run()
     */
    public abstract void run();

    /**
     * Computes a result, or throws an exception if unable to do so.
     *
     * @return computed result
     * @throws Exception if unable to compute a result
     */
    V call() throws Exception;
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 　　注意这里，无法抛出受检异常不等于无法捕获线程中throws的异常。run方法执行体中抛出异常是可以被捕获的，前提是使用Future来处理，后面会有说明。

​     　 如果有一种场景需要一个线程处理一段业务，处理结束之后主线程将会使用处理结果进行后续处理。这样，按照普通逻辑，就需要使用到一个全局变量来保存子线程处理之后的结果。子线程处理结束之后，把结果保存在全局变量中供主线程进行调用。一旦涉及到全局能量便存在着多线程读写全局变量错误的风险。而使用Future模式便可以省去全局变量的使用，直接从线程中获取子线程处理结果。下面看一下使用示例;

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
package thread.blogs.threadmodel;

/**
 * Created by PerkinsZhu on 2017/9/1 15:34.
 */
public class AbstractModel {
    protected static void sleep(int time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    protected static void println(Object info) {
        System.out.println(info);
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
package thread.blogs.threadmodel;

import java.util.concurrent.*;

/**
 * Created by PerkinsZhu on 2017/9/1 15:32.
 */
public class FutureModel extends AbstractModel {
    public static void main(String[] args) {
        testFuture();
    }

    /**
     * 区别： CallAble  可以有返回值  可以抛出受检异常
     * Runnable  没有返回值   无法抛出受检异常但可捕获线程中发生的异常。
     * 者都可通过对future.get()进行try cathch捕获异常
     */
    private static void testFuture() {
        MyCallable myCallable = new MyCallable();
        MyRunnable myRunnable = new MyRunnable();
        ExecutorService executorService = Executors.newFixedThreadPool(5);
        Future<?> future = executorService.submit(myCallable);
        sleep(2000);
        try {
            //String data = future.get(2000, TimeUnit.MILLISECONDS);//可以指定超时时间
            Object data = future.get();//当执行Runnable的时候，这里返回的为nul。此时如果有run方法体中有异常异常抛出，可以在此捕获到,虽然Run方法没有显示的抛出受检异常。
            println(data + "---" + data.getClass().toString());
        } catch (InterruptedException e) {
            println(e.getMessage());
        } catch (ExecutionException e) {
            println(e.getMessage());
        } catch (Exception e) {
            println(e.getMessage());
        }
        executorService.shutdown();
    }

    static class MyCallable implements Callable<String> {
        @Override
        public String call() throws Exception {
            sleep(500);
            println("I am Callable...");
            //int num = 10/0;
            //throw  new RuntimeException("异常");
            return "hello";
        }
    }

    static class MyRunnable implements Runnable {
        @Override
        public void run() {//不支持返回值，无法对线程捕获异常。
            sleep(500);
            println("I am Runnable...");
            // int num = 10/0;
            //throw  new RuntimeException("异常");
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 可以取消注释 分别测试 myCallable  和myRunnable 对异常捕获和结果获取进行测试。

**2、fork&join 模型**
        该模型是jdk中提供的线程模型。该模型包含递归思想和回溯思想，递归用来拆分任务，回溯用合并结果。 可以用来处理一些可以进行拆分的大任务。其主要是**把一个大任务逐级拆分为多个子任务，然后分别在子线程中执行，当每个子线程执行结束之后逐级回溯，返回结果进行汇总合并，最终得出想要的结果。**这里模拟一个摘苹果的场景：有100棵苹果树，每棵苹果树有10个苹果，现在要把他们摘下来。为了节约时间，规定每个线程最多只能摘10棵苹树以便于节约时间。各个线程摘完之后汇总计算总苹果树。代码实现如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
package thread.blogs.threadmodel;

import scala.Console;

import java.util.concurrent.*;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created by PerkinsZhu on 2017/9/5 13:05.
 */
public class ForkJoin {
    public static void main(String[] args) {
        testAcation();
    }

    private static void testAcation() {
        ForkJoinPool pool = new ForkJoinPool();
        ForkJoinTask<Integer> future = pool.submit(new ResultTask(100));//共100棵苹果树
        try {
            Console.println(future.get());
            pool.awaitTermination(1000, TimeUnit.MILLISECONDS);
        } catch (Exception e) {
            e.printStackTrace();
        }
        pool.shutdown();
    }
}

class ResultTask extends RecursiveTask<Integer> { //也可继承自RecursiveAction抽象类，区别在于compute方法没有返回值，如果只需要执行动作则可以使用该接口
    private int treeNum;

    public ResultTask(int num) {
        this.treeNum = num;
    }

    @Override
    protected Integer compute() {
        if (treeNum < 10) {//每个线程最多只能摘10棵苹果树
            return getAppleNum(treeNum);
        } else {

            //对任务进行拆分，注意这里不仅仅可以一分为二进行拆分，也可以拆为多个子任务
            int temp = treeNum / 2;
            ResultTask left = new ResultTask(temp);
            ResultTask right = new ResultTask(treeNum - temp);
            left.fork();
            right.fork();
            //对子任务处理的结果进行合并
            int result = left.join() + right.join();

            Console.println("========" + Thread.currentThread().getName() + "=========" + result);
            return result;
        }
    }

    public Integer getAppleNum(int treeNum) {
        return treeNum * 10;//每棵树上10个苹果
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 这里需要看一下执行结果，主要是为了明白在拆分子任务的时候并不是无限制开启线程，而是使用了线程池ForkJoinPool复用线程。注意下面输出的线程名称！

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
========ForkJoinPool-1-worker-3=========120
========ForkJoinPool-1-worker-7=========120
========ForkJoinPool-1-worker-0=========120
========ForkJoinPool-1-worker-5=========120
========ForkJoinPool-1-worker-1=========130
========ForkJoinPool-1-worker-11=========130
========ForkJoinPool-1-worker-4=========250
========ForkJoinPool-1-worker-7=========130
========ForkJoinPool-1-worker-7=========250
========ForkJoinPool-1-worker-3=========130
========ForkJoinPool-1-worker-5=========250
========ForkJoinPool-1-worker-6=========250
========ForkJoinPool-1-worker-2=========500
========ForkJoinPool-1-worker-3=========500
========ForkJoinPool-1-worker-1=========1000
1000
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  **3、actor消息模型**

　　actor模型属于一种基于消息传递机制并行任务处理思想，它以消息的形式来进行线程间数据传输，避免了全局变量的使用，进而避免了数据同步错误的隐患。actor在接受到消息之后可以自己进行处理，也可以继续传递（分发）给其它actor进行处理。在使用actor模型的时候需要使用第三方Akka提供的框架[点击查看](http://akka.io/docs/)。这里使用scala进行演示，如果需要看java使用方法则可以查阅官方文档：[actor for java 使用。](http://doc.akka.io/docs/akka/current/java/actors.html#creating-actors)

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
package thread.blogs.threadmodel

import akka.actor.{Actor, ActorSystem, Props}

/**
  * Created by PerkinsZhu on 2017/9/21 18:58. 
  */
object ActorTest {
  def main(args: Array[String]): Unit = {
    val actorSystem = ActorSystem("MyActor")
    val actor = actorSystem.actorOf(Props[MyActor], "MyActor")
    actor ! "很高兴认识你！"//发送消息给actor
  }
}

class MyActor extends Actor {
  override def receive: Receive = {//接收消息，根据消息类型进行case匹配，可以在此actor进行处理，也可以继续传递给其它actor进行处理（参考master-worker）。
    case str: String => println(str)
  }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 **4、生产者消费者模型**

　　生产者消费者模型都比较熟悉，其核心是使用一个缓存来保存任务。开启一个/多个线程来生产任务，然后再开启一个/多个来从缓存中取出任务进行处理。这样的好处是任务的生成和处理分隔开，生产者不需要处理任务，只负责向生成任务然后保存到缓存。而消费者只需要从缓存中取出任务进行处理。使用的时候可以根据任务的生成情况和处理情况开启不同的线程来处理。比如，生成的任务速度较快，那么就可以灵活的多开启几个消费者线程进行处理，这样就可以避免任务的处理响应缓慢的问题。使用示例如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
package thread.blogs.threadmodel;

import java.util.LinkedList;
import java.util.Queue;
import java.util.UUID;

/**
 * Created by PerkinsZhu on 2017/9/22 8:58.
 */
public class PCModel {
    public static void main(String[] args) {
        testPCModel();
    }
    private static Queue<String> queue = new LinkedList<String>();//任务缓存，这里保存简单的字符串模拟任务
    private static void testPCModel() {
        new Thread(() -> {//生产者线程
            while (true) {
                String uuid = UUID.randomUUID().toString();
                queue.add(uuid);
                sleep(100);
            }
        }).start();
        for (int i = 0; i < 10; i++) {//开启10消费者处理任务,保证生产者产生的任务能够被及时处理
            new Thread(() -> {
                while (true) {
                    doWork(queue);
                }
            }).start();
        }
    }

    private static void doWork(Queue<String> queue) {
        sleep(1000);
        synchronized (queue) {
            if (queue.size() > 0) {
                sleep(10);
                System.out.println(queue.poll() + "----" + queue.size());
            }
        }
    }
    private static void sleep(int time) {
        try {
            Thread.sleep(time);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 **5、master-worker模型**

　　master-worker模型类似于任务分发策略，开启一个master线程接收任务，然后在master中根据任务的具体情况进行分发给其它worker子线程，然后由子线程处理任务。如需返回结果，则worker处理结束之后把处理结果返回给master。下面的代码示例是使用akka actor for scala演示。使用的时候也可以使用java Thread来实现该模型。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
package thread.blogs.threadmodel

import akka.actor.{Actor, ActorSystem, Props}

/**
  * Created by PerkinsZhu on 2017/9/21 18:58. 
  */
object ActorTest {
  val actorSystem = ActorSystem("Master")


  def main(args: Array[String]): Unit = {
    val actor = actorSystem.actorOf(Props[Master], "Master")
    var taskNum = 0;
    while (true) {
      taskNum = taskNum + 1;
      actor ! Task("做作业!  --" + taskNum) //发送消息给actor
      Thread.sleep(100)
    }
  }
}

class Master extends Actor {
  val actorSystem = ActorSystem("worker")
  var num = 0;

  override def receive: Receive = {
    case task: Task => {
      num = num + 1;
      //接收到任务之后分发给其它worker线程。可以使用worker池 复用actor
      actorSystem.actorOf(Props[Worker], "worker" + num) ! task
    }
    case any: Any => println(any)
  }
}

class Worker extends Actor {

  def doWork(task: Task): Unit = println(task.name)

  override def receive: Receive = {
    case task: Task => doWork(task) //worker处理接受到的任务
    case any: Any => println(any)
  }
}

case class Task(name: String)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

  这里如果需要worker返回处理结果，则只需要在worker中调用sender 发送处理结果即可。

   