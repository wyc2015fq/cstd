# Java多线程 - yixianfeng41的专栏 - CSDN博客
2017年08月28日 23:24:42[第2梦](https://me.csdn.net/yixianfeng41)阅读数：382
### 一、java多线程实现方式
Java多线程实现方式主要有四种：继承Thread类、实现Runnable接口、实现Callable接口通过FutureTask包装器来创建Thread线程、使用ExecutorService、Callable、Future实现有返回结果的多线程。
#### 1、继承Thread类创建线程
Thread类本质上是实现了Runnable接口的一个实例，代表一个线程的实例。启动线程的唯一方法就是通过Thread类的start()实例方法。start()方法是一个native方法，它将启动一个新线程，并执行run()方法。这种方式实现多线程很简单，通过自己的类直接extend Thread，并复写run()方法，就可以启动新线程并执行自己定义的run()方法。例如：
```java
public class MyThread extends Thread {  
　　public void run() {  
　　 System.out.println("MyThread.run()");  
　　}  
}  
MyThread myThread1 = new MyThread();  
MyThread myThread2 = new MyThread();  
myThread1.start();  
myThread2.start();
```
#### 2、实现Runnable接口创建线程
如果自己的类已经extends另一个类，就无法直接extends Thread，此时，可以实现一个Runnable接口，如下：
```java
public class MyThread extends OtherClass implements Runnable {  
　　public void run() {  
　　 System.out.println("MyThread.run()");  
　　}  
}
```
为了启动MyThread，需要首先实例化一个Thread，并传入自己的MyThread实例：
```
MyThread myThread = new MyThread();  
Thread thread = new Thread(myThread);  
thread.start();
```
#### 3、实现Callable接口通过FutureTask包装器来创建Thread线程
Callable接口（也只有一个方法）定义如下：
```
public interface Callable<V>{ 
  V call（） throws Exception;   
}
```
```php
public class SomeCallable<V> extends OtherClass implements Callable<V> {
    @Override
    public V call() throws Exception {
        // TODO Auto-generated method stub
        return null;
    }
}
```
```xml
Callable<V> oneCallable = new SomeCallable<V>();   
//由Callable<Integer>创建一个FutureTask<Integer>对象：   
FutureTask<V> oneTask = new FutureTask<V>(oneCallable);   
//注释：FutureTask<Integer>是一个包装器，它通过接受Callable<Integer>来创建，它同时实现了Future和Runnable接口。 
  //由FutureTask<Integer>创建一个Thread对象：   
Thread oneThread = new Thread(oneTask);   
oneThread.start();   
//至此，一个线程就创建完成了。
```
#### 4、使用ExecutorService、Callable、Future实现有返回结果的线程
ExecutorService、Callable、Future三个接口实际上都是属于Executor框架。返回结果的线程是在JDK1.5中引入的新特征，有了这种特征就不需要再为了得到返回值而大费周折了。而且自己实现了也可能漏洞百出。
可返回值的任务必须实现Callable接口。类似的，无返回值的任务必须实现Runnable接口。
执行Callable任务后，可以获取一个Future的对象，在该对象上调用get就可以获取到Callable任务返回的Object了。
注意：get方法是阻塞的，即：线程无返回结果，get方法会一直等待。
再结合线程池接口ExecutorService就可以实现传说中有返回结果的多线程了。
下面提供了一个完整的有返回结果的多线程测试例子，在JDK1.5下验证过没问题可以直接使用。代码如下：
```java
import java.util.concurrent.*;  
import java.util.Date;  
import java.util.List;  
import java.util.ArrayList;  
/** 
* 有返回值的线程 
*/  
@SuppressWarnings("unchecked")  
public class Test {  
public static void main(String[] args) throws ExecutionException,  
    InterruptedException {  
   System.out.println("----程序开始运行----");  
   Date date1 = new Date();  
   int taskSize = 5;  
   // 创建一个线程池  
   ExecutorService pool = Executors.newFixedThreadPool(taskSize);  
   // 创建多个有返回值的任务  
   List<Future> list = new ArrayList<Future>();  
   for (int i = 0; i < taskSize; i++) {  
    Callable c = new MyCallable(i + " ");  
    // 执行任务并获取Future对象  
    Future f = pool.submit(c);  
    // System.out.println(">>>" + f.get().toString());  
    list.add(f);  
   }  
   // 关闭线程池  
   pool.shutdown();  
   // 获取所有并发任务的运行结果  
   for (Future f : list) {  
    // 从Future对象上获取任务的返回值，并输出到控制台  
    System.out.println(">>>" + f.get().toString());  
   }  
   Date date2 = new Date();  
   System.out.println("----程序结束运行----，程序运行时间【"  
     + (date2.getTime() - date1.getTime()) + "毫秒】");  
}  
}  
class MyCallable implements Callable<Object> {  
private String taskNum;  
MyCallable(String taskNum) {  
   this.taskNum = taskNum;  
}  
public Object call() throws Exception {  
   System.out.println(">>>" + taskNum + "任务启动");  
   Date dateTmp1 = new Date();  
   Thread.sleep(1000);  
   Date dateTmp2 = new Date();  
   long time = dateTmp2.getTime() - dateTmp1.getTime();  
   System.out.println(">>>" + taskNum + "任务终止");  
   return taskNum + "任务返回运行结果,当前任务时间【" + time + "毫秒】";  
}  
}
```
代码说明： 
上述代码中Executors类，提供了一系列工厂方法用于创建线程池，返回的线程池都实现了ExecutorService接口。 
public static ExecutorService newFixedThreadPool(int nThreads)  
创建固定数目线程的线程池。 
public static ExecutorService newCachedThreadPool()  
创建一个可缓存的线程池，调用execute 将重用以前构造的线程（如果线程可用）。如果现有线程没有可用的，则创建一个新线程并添加到池中。终止并从缓存中移除那些已有 60 秒钟未被使用的线程。 
public static ExecutorService newSingleThreadExecutor()  
创建一个单线程化的Executor。 
public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize)  
创建一个支持定时及周期性的任务执行的线程池，多数情况下可用来替代Timer类。
ExecutoreService提供了submit()方法，传递一个Callable，或Runnable，返回Future。如果Executor后台线程池还没有完成Callable的计算，这调用返回Future对象的get()方法，会阻塞直到计算完成。
内容来源：[http://www.cnblogs.com/felixzh/p/6036074.html](http://www.cnblogs.com/felixzh/p/6036074.html)
### 二、线程同步
#### 1、synchronized关键字修饰的方法
1.1、 将synchronized加在需要互斥的方法上
```
public synchronized void method(String name) {  
    // TODO 线程方法  
    //........
}
```
这种方式就相当于用this锁住整个方法内的代码块，如果用synchronized加在静态方法上，就相当于用××××.class锁住整个方法内的代码块。使用synchronized在某些情况下会造成死锁，死锁问题以后会说明。使用synchronized修饰的方法或者代码块可以看成是一个原子操作。 
1.2、 使用synchronized同步代码块 
即有synchronized关键字修饰的语句块。被该关键字修饰的语句块会自动被加上内置锁，从而实现同步
```
public class Bank {  
    private int count =0;//账户余额  
    //存钱  
    public   void addMoney(int money){  
        synchronized (this) {  
            count +=money;  
        }  
        System.out.println(System.currentTimeMillis()+"存进："+money);  
    }  
    //取钱  
    public   void subMoney(int money){  
        synchronized (this) {  
            if(count-money < 0){  
                System.out.println("余额不足");  
                return;  
            }  
            count -=money;  
        }  
        System.out.println(+System.currentTimeMillis()+"取出："+money);  
    }  
    //查询  
    public void lookMoney(){  
        System.out.println("账户余额："+count);  
    }  
}
```
#### 2、Volatile实现线程同步
a.volatile关键字为域变量的访问提供了一种免锁机制 
b.使用volatile修饰域相当于告诉虚拟机该域可能会被其他线程更新 
c.因此每次使用该域就要重新计算，而不是使用寄存器中的值 
d.volatile不会提供任何原子操作，它也不能用来修饰final类型的变量
```
public class Bank {  
    private volatile int count = 0;// 账户余额  
    // 存钱  
    public void addMoney(int money) {  
        count += money;  
        System.out.println(System.currentTimeMillis() + "存进：" + money);  
    }  
    // 取钱  
    public void subMoney(int money) {  
        if (count - money < 0) {  
            System.out.println("余额不足");  
            return;  
        }  
        count -= money;  
        System.out.println(+System.currentTimeMillis() + "取出：" + money);  
    }  
    // 查询  
    public void lookMoney() {  
        System.out.println("账户余额：" + count);  
    }  
}
```
#### 3、使用重入锁实现线程同步
在JavaSE5.0中新增了一个java.util.concurrent包来支持同步。ReentrantLock类是可重入、互斥、实现了Lock接口的锁， 它与使用synchronized方法和快具有相同的基本行为和语义，并且扩展了其能力。 
ReenreantLock类的常用方法有： 
ReentrantLock() : 创建一个ReentrantLock实例 
lock() : 获得锁 
unlock() : 释放锁 
注：ReentrantLock()还有一个可以创建公平锁的构造方法，但由于能大幅度降低程序运行效率，不推荐使用
```
public class Bank {  
    private  int count = 0;// 账户余额  
    //需要声明这个锁  
    private Lock lock = new ReentrantLock();  
    // 存钱  
    public void addMoney(int money) {  
        lock.lock();//上锁  
        try{  
        count += money;  
        System.out.println(System.currentTimeMillis() + "存进：" + money);  
        }finally{  
            lock.unlock();//解锁  
        }  
    }  
    // 取钱  
    public void subMoney(int money) {  
        lock.lock();  
        try{  
        if (count - money < 0) {  
            System.out.println("余额不足");  
            return;  
        }  
        count -= money;  
        System.out.println(+System.currentTimeMillis() + "取出：" + money);  
        }finally{  
            lock.unlock();  
        }  
    }  
    // 查询  
    public void lookMoney() {  
        System.out.println("账户余额：" + count);  
    }  
}
```
内容来源：[http://developer.51cto.com/art/201509/490965.htm](http://developer.51cto.com/art/201509/490965.htm)
