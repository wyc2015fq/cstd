# Java线程之由生产者/消费者问题看线程同步与通信 - 零度的博客专栏 - CSDN博客
2016年06月08日 10:35:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：388标签：[线程																[通信](https://so.csdn.net/so/search/s.do?q=通信&t=blog)](https://so.csdn.net/so/search/s.do?q=线程&t=blog)
个人分类：[Java并发编程](https://blog.csdn.net/zmx729618/article/category/6181602)
      生产者消费者问题是研究多线程程序时绕不开的问题，它的描述是有一块生产者和消费者共享的有界缓冲区，生产者往缓冲区放入产品，消费者从缓冲区取走产品，这个过程可以无休止的执行，不能因缓冲区满生产者放不进产品而终止，也不能因缓冲区空消费者无产品可取而终止。
      解决生产者消费者问题的方法有两种，一种是采用某种机制保持生产者和消费者之间的同步，一种是在生产者和消费者之间建立一个管道。前一种有较高的效率并且可控制性较好，比较常用，后一种由于管道缓冲区不易控制及被传输数据对象不易封装等原因，比较少用。
       同步问题的核心在于，CPU是按时间片轮询的方式执行程序，我们无法知道某一个线程是否被执行、是否被抢占、是否结束等，因此生产者完全可能当缓冲区已满的时候还在放入产品，消费者也完全可能当缓冲区为空时还在取出产品。同步问题的解决方法一般是采用信号或者加锁机制，即生产者线程当缓冲区已满时放弃自己的执行权，进入等待状态，并通知消费者线程执行。消费者线程当缓冲区已空时放弃自己的执行权，进入等待状态，并通知生产者线程执行。这样一来就保持了线程的同步，并避免了线程间互相等待而进入死锁状态。
        JAVA语言提供了独立于平台的线程机制，保持了”write once, run anywhere”的特色。同时也提供了对同步机制的良好支持。
在JAVA中，一共有四种方法支持同步，其中三个是同步方法，一个是管道方法。
        1.wait()/notify()方法；
        2.await()/signal()方法；
        3.BlockingQueue阻塞队列；
        4.PipedInputStream/PipedOutputStream管道。
        下面我们看各个方法的实现：
**        1、wait()/notify()**
        wait()和notify()是根类Object的两个方法，也就意味着所有的JAVA类都会具有这个两个方法，为什么会被这样设计呢？我们可以认为所有的对象默认都具有一个锁，虽然我们看不到，也没有办法直接操作，但它是存在的。
         wait()方法表示：当缓冲区已满或空时，生产者或消费者线程停止自己的执行，放弃锁，使自己处于等待状态，让另一个线程开始执行；
         notify()方法表示：当生产者或消费者对缓冲区放入或取出一个产品时，向另一个线程发出可执行通知，同时放弃锁，使自己处于等待状态。
        下面是一个例子代码：
```
import java.util.LinkedList;
public class Sycn1{
    private LinkedList<Object> myList =new LinkedList<Object>();
    private int MAX = 10;
    
    public Sycn1(){
    }
    
    public void start(){
            new Producer().start();
            new Consumer().start();
    }
    
    public static void main(String[] args) throws Exception{
        Sycn1 s1 = new Sycn1();
        s1.start();
    }
    
    class Producer extends Thread{        
        public void run(){
            while(true){
                synchronized(myList){
                    try{
                        while(myList.size() == MAX){
                            System.out.println("warning: it's full!");
                            myList.wait();
                        }
                        Object o = new Object();
                        if(myList.add(o)){
                            System.out.println("Producer: " + o);
                            myList.notify();
                        }
                    }catch(InterruptedException ie){
                        System.out.println("producer is interrupted!");
                    }
                }
            }
        }
    }
    
    class Consumer extends Thread{
        public void run(){
            while(true){
                synchronized(myList){
                    try{
                        while(myList.size() == 0){
                            System.out.println("warning: it's empty!");
                            myList.wait();
                        }
                        Object o = myList.removeLast();
                        System.out.println("Consumer: " + o);
                        myList.notify();
                    }catch(InterruptedException ie){
                        System.out.println("consumer is interrupted!");
                    }
                }
            }
        }
    }
    
}
```
**       2、await()/signal()**
在JDK5.0以后，JAVA提供了新的更加健壮的线程处理机制，包括了同步、锁定、线程池等等，它们可以实现更小粒度上的控制。await()和signal()就是其中用来做同步的两种方法，它们的功能基本上和wait()/notify()相同，完全可以取代它们，但是它们和新引入的锁定机制Lock直接挂钩，具有更大的灵活性。
       下面是一个例子代码：
```
import java.util.LinkedList;
import java.util.concurrent.locks.*;
public class Sycn2{
    private LinkedList<Object> myList = new LinkedList<Object>();
    private int MAX = 10;
    private final Lock lock = new ReentrantLock();
    private final Condition full = lock.newCondition();
    private final Condition empty = lock.newCondition();
    
    public Sycn2(){
    }
    
    public void start(){
            new Producer().start();
            new Consumer().start();
    }
    
    public static void main(String[] args) throws Exception{
        Sycn2 s2 = new Sycn2();
        s2.start();
    }
    
    class Producer extends Thread{        
        public void run(){
            while(true){
                lock.lock();
                try{
                    while(myList.size() == MAX){
                        System.out.println("warning: it's full!");
                        full.await();
                    }
                    Object o = new Object();
                    if(myList.add(o)){
                        System.out.println("Producer: " + o);
                        empty.signal();
                    }
                }catch(InterruptedException ie){
                    System.out.println("producer is interrupted!");
                }finally{
                    lock.unlock();
                }
            }
        }
    }
    
    class Consumer extends Thread{
        public void run(){
            while(true){
                lock.lock();
                try{
                    while(myList.size() == 0){
                        System.out.println("warning: it's empty!");
                        empty.await();
                    }
                    Object o = myList.removeLast();
                    System.out.println("Consumer: " + o);
                    full.signal();
                }catch(InterruptedException ie){
                    System.out.println("consumer is interrupted!");
                }finally{
                    lock.unlock();
                }
            }
        }
    }
    
}
```
**        3、BlockingQueue阻塞队列**
BlockingQueue也是JDK5.0的一部分，它是一个已经在内部实现了同步的队列，实现方式采用的是我们的第2种await()/signal()方法。它可以在生成对象时指定容量大小。
       它用于阻塞操作的是put()和take()方法：
        put()方法类似于我们上面的生产者线程，容量最大时，自动阻塞。
        take()方法类似于我们上面的消费者线程，容量为0时，自动阻塞。
        下面是一个例子代码：
```
import java.util.concurrent.*;
public class Sycn3{
    private LinkedBlockingQueue<Object> queue = new LinkedBlockingQueue<Object>(10);
    private int MAX = 10;
    
    public Sycn3(){
    }
    
    public void start(){
            new Producer().start();
            new Consumer().start();
    }
    
    public static void main(String[] args) throws Exception{
        Sycn3 s3 = new Sycn3();
        s3.start();
    }
    
    class Producer extends Thread{        
        public void run(){
            while(true){
                //synchronized(this){
                try{
                    if(queue.size() == MAX)
                        System.out.println("warning: it's full!");
                    Object o = new Object();
                    queue.put(o);
                    System.out.println("Producer: " + o);
                    }catch(InterruptedException e){
                        System.out.println("producer is interrupted!");
                    }
                //}
            }
        }
    }
    
    class Consumer extends Thread{
        public void run(){
            while(true){
                //synchronized(this){
                try{
                    if(queue.size() == 0)
                        System.out.println("warning: it's empty!");
                    Object o = queue.take();
                    System.out.println("Consumer: " + o);
                    }catch(InterruptedException e){
                        System.out.println("producer is interrupted!");
                    }
                //}
            }
        }
    }
    
}
```
       你发现这个例子中的问题了吗？
       如果没有，我建议你运行一下这段代码，仔细观察它的输出，是不是有下面这个样子的？为什么会这样呢
       …
       warning: it's full!
       Producer: [java.lang.object@4526e2a](mailto:java.lang.object@4526e2a)
       …
       你可能会说这是因为put()和System.out.println()之间没有同步造成的，我也这样认为，我也这样认为，但是你把run()中的synchronized前面的注释去掉，重新编译运行，有改观吗？没有。为什么？
       这是因为，当缓冲区已满，生产者在put()操作时，put()内部调用了await()方法，放弃了线程的执行，然后消费者线程执行，调用take()方法，take()内部调用了signal()方法，通知生产者线程可以执行，致使在消费者的println()还没运行的情况下生产者的println()先被执行，所以有了上面的输出。run()中的synchronized其实并没有起什么作用。
        对于BlockingQueue大家可以放心使用，这可不是它的问题，只是在它和别的对象之间的同步有问题。
**       4、PipedInputStream/PipedOutputStream管道**
        这个类位于java.io包中，是解决同步问题的最简单的办法，一个线程将数据写入管道，另一个线程从管道读取数据，这样便构成了一种生产者/消费者的缓冲区编程模式。
       下面是一个例子代码，在这个代码我没有使用Object对象，而是简单的读写字节值，这是因为PipedInputStream/PipedOutputStream不允许传输对象，这是JAVA本身的一个bug,具体的大家可以看sun的解释：[http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4131126](http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4131126)
```
import java.io.*;
public class Sycn4{
    private PipedOutputStream pos;
    private PipedInputStream pis;
    //private ObjectOutputStream oos;
    //private ObjectInputStream ois;
    
    public Sycn4(){
        try{
            pos = new PipedOutputStream();
            pis = new PipedInputStream(pos);
            //oos = new ObjectOutputStream(pos);
            //ois = new ObjectInputStream(pis);
        }catch(IOException e){
            System.out.println(e);
        }
    }
    
    public void start(){
        new Producer().start();
        new Consumer().start();
    }
    
    public static void main(String[] args) throws Exception{
        Sycn4 s4 = new Sycn4();
        s4.start();
    }
    
    class Producer extends Thread{
        public void run() {
            try{
                while(true){
                    int b = (int) (Math.random() * 255);
                    System.out.println("Producer: a byte, the value is " + b);
                    pos.write(b);
                    pos.flush();
                    //Object o = new MyObject();
                    //oos.writeObject(o);
                    //oos.flush();
                    //System.out.println("Producer: " + o);
                }
            }catch(Exception e){
                //System.out.println(e);
                e.printStackTrace();
            }finally{
                try{
                    pos.close();
                    pis.close();
                    //oos.close();
                    //ois.close();
                }catch(IOException e){
                    System.out.println(e);
                }
            }
        }
    }
    
    class Consumer extends Thread{
        public void run(){
            try{
                while(true){
                    int b = pis.read();
                    System.out.println("Consumer: a byte, the value is " + String.valueOf(b));
                    //Object o = ois.readObject();
                    //if(o != null)
                        //System.out.println("Consumer: " + o);
                }
            }catch(Exception e){
                //System.out.println(e);
                e.printStackTrace();
            }finally{
                try{
                    pos.close();
                    pis.close();
                    //oos.close();
                    //ois.close();
                }catch(IOException e){
                    System.out.println(e);
                }
            }
        }
    }
    
    //class MyObject implements Serializable {
    //}
}
```
