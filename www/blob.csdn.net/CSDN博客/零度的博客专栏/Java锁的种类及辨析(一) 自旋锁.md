# Java锁的种类及辨析(一):自旋锁 - 零度的博客专栏 - CSDN博客
2017年11月14日 10:16:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：183
锁作为并发共享数据，保证一致性的工具，在JAVA平台有多种实现(如 synchronized 和 ReentrantLock等等 ) 。这些已经写好提供的锁为我们开发提供了便利，但是锁的具体性质以及类型却很少被提及。本系列文章将分析JAVA下常见的锁名称以及特性，为大家答疑解惑。
### 1、自旋锁
自旋锁是采用让当前线程不停地的在循环体内执行实现的，当循环的条件被其他线程改变时 才能进入临界区。如下
|`01`|`public``class````SpinLock {```|
|`02`||
|`03`|`  ``private````AtomicReference<Thread> sign =````new``AtomicReference<>();`|
|`04`||
|`05`|`  ``public``void``lock(){`|
|`06`|`    ````Thread current = Thread.currentThread();```|
|`07`|`    ``while````(!sign .compareAndSet(````null````, current)){```|
|`08`|`    ``}`|
|`09`|`  ``}`|
|`10`||
|`11`|`  ``public``void````unlock (){```|
|`12`|`    ````Thread current = Thread.currentThread();```|
|`13`|`    ````sign .compareAndSet(current,````null``);`|
|`14`|`  ``}`|
|`15`|`}`|
使用了CAS原子操作，lock函数将owner设置为当前线程，并且预测原来的值为空。unlock函数将owner设置为null，并且预测值为当前线程。
当有第二个线程调用lock操作时由于owner值不为空，导致循环一直被执行，直至第一个线程调用unlock函数将owner设置为null，第二个线程才能进入临界区。
由于自旋锁只是将当前线程不停地执行循环体，不进行线程状态的改变，所以响应速度更快。但当线程数不停增加时，性能下降明显，因为每个线程都需要执行，占用CPU时间。如果线程竞争不激烈，并且保持锁的时间段。适合使用自旋锁。
注：该例子为非公平锁，获得锁的先后顺序，不会按照进入lock的先后顺序进行。
测试如下：
```java
package org.zwc.javalocktest;
import java.util.concurrent.atomic.AtomicReference;
/**
 * Created by zhangwenchao on 2017/11/14.
 */
public class SpinLock {
    private static Integer a = 1;
    private AtomicReference<Thread> sign = new AtomicReference<Thread>();
    public void lock() {
        Thread current = Thread.currentThread();
        while (!sign.compareAndSet(null, current)) {
        }
    }
    public void unlock() {
        Thread current = Thread.currentThread();
        sign.compareAndSet(current, null);
    }
    public static void main(String[] args) {
        final SpinLock spinLock = new SpinLock();
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true){
                    spinLock.lock();
                    a++;
                    System.out.println("Thread a puls a+1=:"+a);
                    spinLock.unlock();
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        },"a").start();
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    spinLock.lock();
                    a++;
                    System.out.println("Thread b puls a+1=:" + a);
                    spinLock.unlock();
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        },"b").start();
    }
}
```
运行结果：
Thread a puls a+1=:2
Thread b puls a+1=:3
Thread b puls a+1=:4
Thread a puls a+1=:5
Thread a puls a+1=:6
Thread b puls a+1=:7
Thread a puls a+1=:8
Thread b puls a+1=:9
Thread a puls a+1=:10
