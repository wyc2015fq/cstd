# synchronized和ReentrantLock区别浅析 - 零度的博客专栏 - CSDN博客
2016年06月06日 15:16:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：2999
#### 一、什么是sychronized
       sychronized是java中最基本同步互斥的手段,可以修饰代码块,方法,类.
      在修饰代码块的时候需要一个reference对象作为锁的对象.
      在修饰方法的时候默认是当前对象作为锁的对象.
      在修饰类时候默认是当前类的Class对象作为锁的对象.
      synchronized会在进入同步块的前后分别形成monitorenter和monitorexit字节码指令.在执行monitorenter指令时会尝试获取对象的锁,如果此没对象没有被锁,或者此对象已经被当前线程锁住,那么锁的计数器加一,每当monitorexit被锁的对象的计数器减一.直到为0就释放该对象的锁.由此synchronized是可重入的,不会出现自己把自己锁死.
#### 二、什么ReentrantLock
      以对象的方式来操作对象锁.相对于sychronized需要在finally中去释放锁。
#### 三、synchronized和ReentrantLock的区别
     1. 等待可中断
     在持有锁的线程长时间不释放锁的时候,等待的线程可以选择放弃等待.   tryLock(long timeout, TimeUnit unit)。
     ReentrantLock 拥有Synchronized相同的并发性和内存语义，此外还多了 锁投票，定时锁等候和中断锁等候。
 线程A和B都要获取对象O的锁定，假设A获取了对象O锁，B将等待A释放对O的锁定。
  如果使用 synchronized ，如果A不释放，B将一直等下去，不能被中断。
  如果使用ReentrantLock，如果A不释放，可以使B在等待了足够长的时间以后，中断等待，而干别的事情。
  ReentrantLock获取锁定与三种方式： 
        a)  lock(), 如果获取了锁立即返回，如果别的线程持有锁，当前线程则一直处于休眠状态，直到获取锁
    b) tryLock(), 如果获取了锁立即返回true，如果别的线程正持有锁，立即返回false；
    c) tryLock (long timeout, [TimeUnit](http://houlinyan.iteye.com/java/util/concurrent/TimeUnit.html)  unit)，   如果获取了锁定立即返回true，如果别的线程正持有锁，会等待参数给定的时间，在等待的过程中，如果获取了锁定，就返回true，如果等待超时，返回false；
    d) lockInterruptibly: 如果获取了锁定立即返回，如果没有获取锁定，当前线程处于休眠状态，直到或者锁定，或者当前线程被别的线程中断
    2.公平锁与非公平锁
        按照申请锁的顺序来一次获得锁称为公平锁.synchronized的是非公平锁,ReentrantLock可以通过构造函数实现公平锁.    new RenentrantLock(boolean fair)
        3.绑定多个Condition
        通过多次newCondition可以获得多个Condition对象,可以简单的实现比较复杂的线程同步的功能.通过await(),signal()
        此外，synchronized是在JVM层面上实现的，不但可以通过一些监控工具监控synchronized的锁定，而且在代码执行时出现异常，JVM会自动释放锁定，但是使用Lock则不行，lock是通过代码实现的，要保证锁定一定会被释放，就必须将 unLock()放到finally{} 中。在资源竞争不是很激烈的情况下，Synchronized的性能要优于ReetrantLock，但是在资源竞争很激烈的情况下，Synchronized的性能会下降几十倍，但是ReetrantLock的性能能维持常态。
#### 四、实例演示
ReentrantLock 的lock机制有2种，忽略中断锁和响应中断锁，这给我们带来了很大的灵活性。比如：如果A、B两个线程去竞争锁，A线程得到了锁，B线程等待，但是A线程这个时候实在有太多事情要处理，就是一直不返回，B线程可能就会等不及了，想中断自己，不再等待这个锁了，转而处理其他事情。这个时候ReentrantLock就提供了两种机制：
       一、B线程中断自己（或者别的线程中断它），但ReentrantLock 不去响应，让B线程继续等待，你再怎么中断，我全当耳边风（synchronized原语就是如此）；
       二、B线程中断自己（或者别的线程中断它），ReentrantLock 处理了这个中断，并且不再等待这个锁的到来，完全放弃。
      请看例子：
```
package zmx.multithread.test.reentrantlock;
import java.util.concurrent.locks.ReentrantLock;
/**
 * 
 * @author zhangwenchao
 *
 */
public class ReentrantLockTest {
    //是用ReentrantLock，还是用synchronized
	
    public static boolean useSynchronized = false;
    
    public static void main(String[] args) {
        IBuffer buff = null;
        if(useSynchronized){
            buff = new Buffer();
        }else{
            buff = new BufferInterruptibly();    
        }
        final Writer writer = new Writer(buff);
        final Reader reader = new Reader(buff);
        writer.start();
        reader.start();
        new Thread(new Runnable() {
            public void run() {
                long start = System.currentTimeMillis();
                for (;;) {
                    // 等5秒钟去中断读
                    if (System.currentTimeMillis() - start > 5000) {
                        System.out.println("不等了，尝试中断");
                        reader.interrupt();
                        break;
                    }
                }
            }
        }).start();
    }
}
interface IBuffer{
    public void write();
    public void read() throws InterruptedException;
}
class Buffer implements IBuffer{
    private Object lock;
    public Buffer() {
        lock = this;
    }
    public void write() {
        synchronized (lock) {
            long startTime = System.currentTimeMillis();
            System.out.println("开始往这个buff写入数据…");
            for (;;)// 模拟要处理很长时间
            {
                if (System.currentTimeMillis() - startTime > Integer.MAX_VALUE)
                    break;
            }
            System.out.println("终于写完了");
        }
    }
    public void read() {
        synchronized (lock) {
            System.out.println("从这个buff读数据");
        }
    }
}
class BufferInterruptibly implements IBuffer{
    private ReentrantLock lock = new ReentrantLock();
    public void write() {
        lock.lock();
        try {
            long startTime = System.currentTimeMillis();
            System.out.println("开始往这个buff写入数据…");
            for (;;)// 模拟要处理很长时间
            {
                if (System.currentTimeMillis() - startTime > Integer.MAX_VALUE)
                    break;
            }
            System.out.println("终于写完了");
        } finally {
            lock.unlock();
        }
    }
    public void read() throws InterruptedException{
        lock.lockInterruptibly();// 注意这里，可以响应中断
       // lock.lock();// 注意这里，不可以响应中断
        try {
            System.out.println("从这个buff读数据");
        } finally {
            lock.unlock();
        }
    }
}
class Writer extends Thread {
    private IBuffer buff;
    public Writer(IBuffer buff) {
        this.buff = buff;
    }
    @Override
    public void run() {
        buff.write();
    }
}
class Reader extends Thread {
    private IBuffer buff;
    public Reader(IBuffer buff) {
        this.buff = buff;
    }
    @Override
    public void run() {
        try {
            buff.read();
        } catch (InterruptedException e) {
            System.out.println("我不读了");   
        }
        System.out.println("读结束");
    }
}
```
      1) 如果使用lock.lockInterruptibly();指定可以响应中断，则输出如下：
```
开始往这个buff写入数据…
不等了，尝试中断
我不读了
读结束
```
      则：获取到中断异常，执行中断异常处理程序。
     2) 如果使用lock.lock();指定不可以响应中断，则输出如下：
```
开始往这个buff写入数据…
不等了，尝试中断
```
     则：不能获取到中断异常，线程等待。
    示例二：
```
package zmx.multithread.test.reentrantlock;
import java.util.concurrent.TimeUnit;  
import java.util.concurrent.locks.Lock;  
import java.util.concurrent.locks.ReentrantLock;  
  
public class T2{  
    public static void main(String[] args){    
        Thread i1 = new Thread(new RunIt3());  
        Thread i2 = new Thread(new RunIt3());  
        i1.start();  
        i2.start();  
        i2.interrupt();  //中断
    }  
  }  
 
class RunIt3 implements Runnable{  
  
    private static Lock lock = new ReentrantLock();  
    public void run(){  
        try{  
            //---------a--------------------------  
            //lock.lock();            
            lock.lockInterruptibly(); 
            //lock.tryLock();
            //lock.tryLock(5,TimeUnit.SECONDS); 
            System.out.println(Thread.currentThread().getName() + " running");  
            TimeUnit.SECONDS.sleep(10);             
            System.out.println(Thread.currentThread().getName() + " finished"); 
            lock.unlock();
            
        }catch (InterruptedException e){  
            System.out.println(Thread.currentThread().getName() + " interrupted");  
  
        }  
  
    }  
}
```
    如果a处是lock.lock(); 输出: 
Thread-0 running 
(这里休眠了10s) 
Thread-0 finished 
Thread-1 running 
Thread-1 interrupted 
============================ 
    如果a处是lock.lockInterruptibly();输出： 
Thread-0 running 
Thread-1 interrupted 
(这里休眠了10s) 
Thread-0 finished 

