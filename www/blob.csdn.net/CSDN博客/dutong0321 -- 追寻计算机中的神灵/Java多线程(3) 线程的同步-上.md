# Java多线程(3) 线程的同步-上 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2017年12月13日 00:09:26[dutong0321](https://me.csdn.net/dutong0321)阅读数：55标签：[Java																[多线程																[线程同步																[线程锁																[竞争条件](https://so.csdn.net/so/search/s.do?q=竞争条件&t=blog)](https://so.csdn.net/so/search/s.do?q=线程锁&t=blog)](https://so.csdn.net/so/search/s.do?q=线程同步&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)
个人分类：[编程世界](https://blog.csdn.net/dutong0321/article/category/6127295)
#### 概述
最近比较忙，线程的同步其实在12月上旬就完成的差不多了，但是一直因为项目的关系，所以没有办法及时的发出来，在新的一年里会努力的扩充自己的技术栈。在多线程的实际应用中，多个线程经常需要共享对同一数据的存取，这样的情况就称为竞争条件。假设i的值为0，A线程执行i=i+1，B线程执行i=i+2，那么最后的值有可能不是3，而是1或者2。因为A读取i后尚未完成计算写入，B线程马上读取i，此时i的值还是0，当A线程执行后i的值变为1，但是B线程已经不会再重新读取i了，在B线程完成计算写入i的值就是2了，当然，如果A线程后完成计算，值也有可能就是1了。所以，多线程中同步是一个很重要的问题(我更喜欢叫控制线程)。
#### 没有同步存取的线程
先看一个没有进行同步存取的例子，在这个例子当中共有2个子线程，1个负责累加，对value从加1至加1000，1个负责累减，对value从减1000至减1，那么接下来就开始贴代码。
```java
package com.utopia.controls;
public class NoControl {
    private static int value;
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        // 对value赋初值
        value = 0;
        System.out.println(value);
        // 累加线程
        Thread threadAdd = new Thread(new Runnable() {
            @Override
            public void run() {
                // TODO Auto-generated method stub
                for (int i = 1; i <= 1000; i++) {
                    value += i;
                }
            }
        });
        // 累减线程
        Thread threadSub = new Thread(new Runnable() {
            @Override
            public void run() {
                // TODO Auto-generated method stub
                for (int i = 1000; i > 0; i--) {
                    value -= i;
                }
            }
        });
        // 同时运行俩个线程
        threadAdd.start();
        threadSub.start();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        System.out.println(value);
    }
}
```
从理论上来讲，最后输出的结果应该是0，但是在实际运行中，虽然有时候结果也是0，但是大部分结果并不是0，如果把10000调至100000，那么结果出现不是0的概率将会更大。 
其中一个结果截图： 
![这里写图片描述](https://img-blog.csdn.net/20171212230025992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果使用多线程，那么出现这种情况我们必然是无法容忍的，所以就有了锁对象。
#### 锁对象
其实，锁对象的概念是很好理解的。在上面我们分析过，出现存取不同步的原因是，Java语言中一条代码：i+=1;在翻译成机器码运行时会变成：读取i;完成加法;写到i的内存中;一共三条指令（在实际中，可能要比三条还多），A线程读取i进行计算加法时，B线程可能将减法的计算结果保存到i中，但是A线程再次保存就会将B线程的值进行覆盖，举个例子： 
value = 0; 
A: value += 10; 
B: value -= 10; 
1.A 读取value是0 
2.B 读取value是0 
3.B 计算得数是 -10 A计算得数是10 
4.B 保存value是-10 
5.A保存value是10 
虽然理论上value的值是0，但最后结果value就是10 
如果，我们可以加个锁，在A线程对value执行代码时，不允许B线程去执行，那么就不会出现这种情况了。对锁的使用一般分为3步： 
1.对锁进行实例化 
2.对线程进行加锁 
3.对线程进行解锁 
代码如图所示：
```java
package com.utopia.controls;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
public class Control {
    private static int value;
    // 对锁进行实例化
    private static Lock valueLock = new ReentrantLock();
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        // 对value赋初值
        value = 0;
        System.out.println(value);
        // 累加线程
        Thread threadAdd = new Thread(new Runnable() {
            @Override
            public void run() {
                // TODO Auto-generated method stub
                for (int i = 1; i <= 10000; i++) {
                    // 对累加线程上锁
                    valueLock.lock();
                    value += i;
                    // 对累加线程解锁
                    valueLock.unlock();
                }
            }
        });
        // 累减线程
        Thread threadSub = new Thread(new Runnable() {
            @Override
            public void run() {
                // TODO Auto-generated method stub
                for (int i = 10000; i > 0; i--) {
                    // 对累减线程上锁
                    valueLock.lock();
                    value -= i;
                    // 对累减线程去锁
                    valueLock.unlock();
                }
            }
        });
        // 同时运行俩个线程
        threadAdd.start();
        threadSub.start();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        System.out.println(value);
    }
}
```
在进行线程同步后，可以发现结果永远都是0，如图： 
![这里写图片描述](https://img-blog.csdn.net/20171212235343343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
锁是可重入的，线程可以重复的获得已经持有的锁，锁保持一个持有计数来跟踪lock方法的嵌套调用。例如：线程A在调用lock后持有锁，线程B在线程A调用unlock释放之前如果调用lock会发生阻塞，但是如果线程A再次调用lock方法则不会发生阻塞，但是线程A必须要执行2次unlock才可以释放锁。
