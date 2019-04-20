# Java多线程实现的三种方式 - Spark高级玩法 - CSDN博客
2018年03月26日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：450
Java多线程实现方式主要有三种：继承Thread类、实现Runnable接口、使用ExecutorService、Callable、Future实现有返回结果的多线程。其中前两种方式线程执行完后都没有返回值，只有最后一种是带返回值的。
1、继承Thread类实现多线程
继承Thread类的方法尽管被我列为一种多线程实现方式，但Thread本质上也是实现了Runnable接口的一个实例，它代表一个线程的实例，并且，启动线程的唯一方法就是通过Thread类的start()实例方法。start()方法是一个native方法，它将启动一个新线程，并执行run()方法。这种方式实现多线程很简单，通过自己的类直接extend Thread，并复写run()方法，就可以启动新线程并执行自己定义的run()方法。例如：
```
public class MyThread extends Thread { 
　　public void run() { 
　　 System.out.println("MyThread.run()"); 
　　} 
}
```
在合适的地方启动线程如下：
```
MyThread myThread1 = new MyThread(); 
MyThread myThread2 = new MyThread(); 
myThread1.start(); 
myThread2.start();
```
2、实现Runnable接口方式实现多线程
如果自己的类已经extends另一个类，就无法直接extends Thread，此时，必须实现一个Runnable接口，如下：
```
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
事实上，当传入一个Runnable target参数给Thread后，Thread的run()方法就会调用target.run()，参考JDK源代码：
```
public void run() { 
　　if (target != null) { 
　　 target.run(); 
　　} 
}
```
3、使用ExecutorService、Callable、Future实现有返回结果的多线程
ExecutorService、Callable、Future这个对象实际上都是属于Executor框架中的功能类。想要详细了解Executor框架的可以访问http://www.javaeye.com/topic/366591 ，这里面对该框架做了很详细的解释。返回结果的线程是在JDK1.5中引入的新特征，确实很实用，有了这种特征我就不需要再为了得到返回值而大费周折了，而且即便实现了也可能漏洞百出。
可返回值的任务必须实现Callable接口，类似的，无返回值的任务必须Runnable接口。执行Callable任务后，可以获取一个Future的对象，在该对象上调用get就可以获取到Callable任务返回的Object了，再结合线程池接口ExecutorService就可以实现传说中有返回结果的多线程了。下面提供了一个完整的有返回结果的多线程测试例子，在JDK1.5下验证过没问题可以直接使用。代码如下：
```
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
**代码说明：**
上述代码中Executors类，提供了一系列工厂方法用于创先线程池，返回的线程池都实现了ExecutorService接口。
public static ExecutorService newFixedThreadPool(int nThreads) 创建固定数目线程的线程池。
public static ExecutorService newCachedThreadPool() 创建一个可缓存的线程池，调用execute 将重用以前构造的线程（如果线程可用）。如果现有线程没有可用的，则创建一个新线程并添加到池中。终止并从缓存中移除那些已有 60 秒钟未被使用的线程。
public static ExecutorService newSingleThreadExecutor() 创建一个单线程化的Executor。
public static ScheduledExecutorService newScheduledThreadPool(int corePoolSize) 创建一个支持定时及周期性的任务执行的线程池，多数情况下可用来替代Timer类。
ExecutoreService提供了submit()方法，传递一个Callable，或Runnable，返回Future。如果Executor后台线程池还没有完成Callable的计算，这调用返回Future对象的get()方法，会阻塞直到计算完成。
- 
**原文：blog.csdn.net/aboy123/article/details/38307539**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
