# Java多线程系列--“基础篇”09之 interrupt()和线程终止方式 - z69183787的专栏 - CSDN博客
2014年05月05日 20:15:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：560
### **概要**
本章，会对线程的interrupt()中断和终止方式进行介绍。涉及到的内容包括：
[1. interrupt()说明](http://www.cnblogs.com/skywang12345/p/3479949.html#p1)
[2. 终止线程的方式](http://www.cnblogs.com/skywang12345/p/3479949.html#p2)[2.1 终止处于“阻塞状态”的线程](http://www.cnblogs.com/skywang12345/p/3479949.html#a21)[](http://www.cnblogs.com/skywang12345/p/3479949.html#p2)[2.2 终止处于“运行状态”的线程](http://www.cnblogs.com/skywang12345/p/3479949.html#a22)
[3. 终止线程的示例](http://www.cnblogs.com/skywang12345/p/3479949.html#p3)
[4. interrupted() 和 isInterrupted()的区别](http://www.cnblogs.com/skywang12345/p/3479949.html#p4)
转载请注明出处：**[http://www.cnblogs.com/skywang12345/p/3479949.html](http://www.cnblogs.com/skywang12345/p/3479949.html)**
### 1. interrupt()说明
在介绍终止线程的方式之前，有必要先对interrupt()进行了解。
关于interrupt()，java的djk文档描述如下：[http://docs.oracle.com/javase/7/docs/api/](http://docs.oracle.com/javase/7/docs/api/)
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
Interrupts this thread.
Unless the current thread is interrupting itself, which is always permitted, the checkAccess method of this thread is invoked, which may cause a SecurityException to be thrown.
If this thread is blocked in an invocation of the wait(), wait(long), or wait(long, int) methods of the Object class, or of the join(), join(long), join(long, int), sleep(long), or sleep(long, int), methods of this class, then its interrupt status will be cleared and it will receive an InterruptedException.
If this thread is blocked in an I/O operation upon an interruptible channel then the channel will be closed, the thread's interrupt status will be set, and the thread will receive a ClosedByInterruptException.
If this thread is blocked in a Selector then the thread's interrupt status will be set and it will return immediately from the selection operation, possibly with a non-zero value, just as if the selector's wakeup method were invoked.
If none of the previous conditions hold then this thread's interrupt status will be set.
Interrupting a thread that is not alive need not have any effect.
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
大致意思是：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
interrupt()的作用是中断本线程。
本线程中断自己是被允许的；其它线程调用本线程的interrupt()方法时，会通过checkAccess()检查权限。这有可能抛出SecurityException异常。
如果本线程是处于阻塞状态：调用线程的wait(), wait(long)或wait(long, int)会让它进入等待(阻塞)状态，或者调用线程的join(), join(long), join(long, int), sleep(long), sleep(long, int)也会让它进入阻塞状态。若线程在阻塞状态时，调用了它的interrupt()方法，那么它的“中断状态”会被清除并且会收到一个InterruptedException异常。例如，线程通过wait()进入阻塞状态，此时通过interrupt()中断该线程；调用interrupt()会立即将线程的中断标记设为“true”，但是由于线程处于阻塞状态，所以该“中断标记”会立即被清除为“false”，同时，会产生一个InterruptedException的异常。
如果线程被阻塞在一个Selector选择器中，那么通过interrupt()中断它时；线程的中断标记会被设置为true，并且它会立即从选择操作中返回。
如果不属于前面所说的情况，那么通过interrupt()中断线程时，它的中断标记会被设置为“true”。
中断一个“已终止的线程”不会产生任何操作。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
### 2. 终止线程的方式
Thread中的stop()和suspend()方法，由于固有的不安全性，已经建议不再使用！
下面，我先分别讨论线程在“阻塞状态”和“运行状态”的终止方式，然后再总结出一个通用的方式。
# 2.1 终止处于“阻塞状态”的线程
通常，我们通过“中断”方式终止处于“阻塞状态”的线程。
当线程由于被调用了sleep(), wait(), join()等方法而进入阻塞状态；若此时调用线程的interrupt()将线程的中断标记设为true。由于处于阻塞状态，中断标记会被清除，同时产生一个InterruptedException异常。将InterruptedException放在适当的为止就能终止线程，形式如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@Override
public void run() {
    try {
        while (true) {
            // 执行任务...
        }
    } catch (InterruptedException ie) {  
        // 由于产生InterruptedException异常，退出while(true)循环，线程终止！
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**说明**：在while(true)中不断的执行任务，当线程处于阻塞状态时，调用线程的interrupt()产生InterruptedException中断。中断的捕获在while(true)之外，这样就退出了while(true)循环！
注意：对InterruptedException的捕获务一般放在while(true)循环体的外面，这样，在产生异常时就退出了while(true)循环。否则，InterruptedException在while(true)循环体之内，就需要额外的添加退出处理。形式如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@Override
public void run() {
    while (true) {
        try {
            // 执行任务...
        } catch (InterruptedException ie) {  
            // InterruptedException在while(true)循环体内。
            // 当线程产生了InterruptedException异常时，while(true)仍能继续运行！需要手动退出
            break;
        }
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
说明：上面的InterruptedException异常的捕获在whle(true)之内。当产生InterruptedException异常时，被catch处理之外，仍然在while(true)循环体内；要退出while(true)循环体，需要额外的执行退出while(true)的操作。
# 2.2 终止处于“运行状态”的线程
通常，我们通过“标记”方式终止处于“运行状态”的线程。其中，包括“**中断标记**”和“**额外添加标记**”。
**(01) 通过“中断标记”终止线程。**
形式如下：
```
@Override
public void run() {
    while (!isInterrupted()) {
        // 执行任务...
    }
}
```
**说明**：isInterrupted()是判断线程的中断标记是不是为true。当线程处于运行状态，并且我们需要终止它时；可以调用线程的interrupt()方法，使用线程的中断标记为true，即isInterrupted()会返回true。此时，就会退出while循环。
注意：interrupt()并不会终止处于“运行状态”的线程！它会将线程的中断标记设为true。
**(02) 通过“额外添加标记”。**
形式如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
private volatile boolean flag= true;
protected void stopTask() {
    flag = false;
}
@Override
public void run() {
    while (flag) {
        // 执行任务...
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**说明**：线程中有一个flag标记，它的默认值是true；并且我们提供stopTask()来设置flag标记。当我们需要终止该线程时，调用该线程的stopTask()方法就可以让线程退出while循环。
注意：将flag定义为volatile类型，是为了保证flag的可见性。即其它线程通过stopTask()修改了flag之后，本线程能看到修改后的flag的值。
综合线程处于“阻塞状态”和“运行状态”的终止方式，比较通用的终止线程的形式如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@Override
public void run() {
    try {
        // 1. isInterrupted()保证，只要中断标记为true就终止线程。
        while (!isInterrupted()) {
            // 执行任务...
        }
    } catch (InterruptedException ie) {  
        // 2. InterruptedException异常保证，当InterruptedException异常产生时，线程被终止。
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
### **3. 终止线程的示例**
interrupt()常常被用来终止“阻塞状态”线程。参考下面示例：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 // Demo1.java的源码
 2 class MyThread extends Thread {
 3     
 4     public MyThread(String name) {
 5         super(name);
 6     }
 7 
 8     @Override
 9     public void run() {
10         try {  
11             int i=0;
12             while (!isInterrupted()) {
13                 Thread.sleep(100); // 休眠100ms
14                 i++;
15                 System.out.println(Thread.currentThread().getName()+" ("+this.getState()+") loop " + i);  
16             }
17         } catch (InterruptedException e) {  
18             System.out.println(Thread.currentThread().getName() +" ("+this.getState()+") catch InterruptedException.");  
19         }
20     }
21 }
22 
23 public class Demo1 {
24 
25     public static void main(String[] args) {  
26         try {  
27             Thread t1 = new MyThread("t1");  // 新建“线程t1”
28             System.out.println(t1.getName() +" ("+t1.getState()+") is new.");  
29 
30             t1.start();                      // 启动“线程t1”
31             System.out.println(t1.getName() +" ("+t1.getState()+") is started.");  
32 
33             // 主线程休眠300ms，然后主线程给t1发“中断”指令。
34             Thread.sleep(300);
35             t1.interrupt();
36             System.out.println(t1.getName() +" ("+t1.getState()+") is interrupted.");
37 
38             // 主线程休眠300ms，然后查看t1的状态。
39             Thread.sleep(300);
40             System.out.println(t1.getName() +" ("+t1.getState()+") is interrupted now.");
41         } catch (InterruptedException e) {  
42             e.printStackTrace();
43         }
44     } 
45 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**运行结果**：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
t1 (NEW) is new.
t1 (RUNNABLE) is started.
t1 (RUNNABLE) loop 1
t1 (RUNNABLE) loop 2
t1 (TIMED_WAITING) is interrupted.
t1 (RUNNABLE) catch InterruptedException.
t1 (TERMINATED) is interrupted now.
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**结果说明**：
(01) 主线程main中通过new MyThread("t1")创建线程t1，之后通过t1.start()启动线程t1。
(02) t1启动之后，会不断的检查它的中断标记，如果中断标记为“false”；则休眠100ms。
(03) t1休眠之后，会切换到主线程main；主线程再次运行时，会执行t1.interrupt()中断线程t1。t1收到中断指令之后，会将t1的中断标记设置“false”，而且会抛出InterruptedException异常。在t1的run()方法中，是在循环体while之外捕获的异常；因此循环被终止。
我们对上面的结果进行小小的修改，将run()方法中捕获InterruptedException异常的代码块移到while循环体内。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 // Demo2.java的源码
 2 class MyThread extends Thread {
 3     
 4     public MyThread(String name) {
 5         super(name);
 6     }
 7 
 8     @Override
 9     public void run() {
10         int i=0;
11         while (!isInterrupted()) {
12             try {
13                 Thread.sleep(100); // 休眠100ms
14             } catch (InterruptedException ie) {  
15                 System.out.println(Thread.currentThread().getName() +" ("+this.getState()+") catch InterruptedException.");  
16             }
17             i++;
18             System.out.println(Thread.currentThread().getName()+" ("+this.getState()+") loop " + i);  
19         }
20     }
21 }
22 
23 public class Demo2 {
24 
25     public static void main(String[] args) {  
26         try {  
27             Thread t1 = new MyThread("t1");  // 新建“线程t1”
28             System.out.println(t1.getName() +" ("+t1.getState()+") is new.");  
29 
30             t1.start();                      // 启动“线程t1”
31             System.out.println(t1.getName() +" ("+t1.getState()+") is started.");  
32 
33             // 主线程休眠300ms，然后主线程给t1发“中断”指令。
34             Thread.sleep(300);
35             t1.interrupt();
36             System.out.println(t1.getName() +" ("+t1.getState()+") is interrupted.");
37 
38             // 主线程休眠300ms，然后查看t1的状态。
39             Thread.sleep(300);
40             System.out.println(t1.getName() +" ("+t1.getState()+") is interrupted now.");
41         } catch (InterruptedException e) {  
42             e.printStackTrace();
43         }
44     } 
45 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**运行结果**：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
t1 (NEW) is new.
t1 (RUNNABLE) is started.
t1 (RUNNABLE) loop 1
t1 (RUNNABLE) loop 2
t1 (TIMED_WAITING) is interrupted.
t1 (RUNNABLE) catch InterruptedException.
t1 (RUNNABLE) loop 3
t1 (RUNNABLE) loop 4
t1 (RUNNABLE) loop 5
t1 (TIMED_WAITING) is interrupted now.
t1 (RUNNABLE) loop 6
t1 (RUNNABLE) loop 7
t1 (RUNNABLE) loop 8
t1 (RUNNABLE) loop 9
...
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**结果说明**：
程序进入了死循环！
为什么会这样呢？这是因为，t1在“等待(阻塞)状态”时，被interrupt()中断；此时，会清除中断标记[即isInterrupted()会返回false]，而且会抛出InterruptedException异常[该异常在while循环体内被捕获]。因此，t1理所当然的会进入死循环了。
解决该问题，需要我们在捕获异常时，额外的进行退出while循环的处理。例如，在MyThread的catch(InterruptedException)中添加break 或 return就能解决该问题。
下面是通过“额外添加标记”的方式终止“状态状态”的线程的示例：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 // Demo3.java的源码
 2 class MyThread extends Thread {
 3 
 4     private volatile boolean flag= true;
 5     public void stopTask() {
 6         flag = false;
 7     }
 8     
 9     public MyThread(String name) {
10         super(name);
11     }
12 
13     @Override
14     public void run() {
15         synchronized(this) {
16             try {
17                 int i=0;
18                 while (flag) {
19                     Thread.sleep(100); // 休眠100ms
20                     i++;
21                     System.out.println(Thread.currentThread().getName()+" ("+this.getState()+") loop " + i);  
22                 }
23             } catch (InterruptedException ie) {  
24                 System.out.println(Thread.currentThread().getName() +" ("+this.getState()+") catch InterruptedException.");  
25             }
26         }  
27     }
28 }
29 
30 public class Demo3 {
31 
32     public static void main(String[] args) {  
33         try {  
34             MyThread t1 = new MyThread("t1");  // 新建“线程t1”
35             System.out.println(t1.getName() +" ("+t1.getState()+") is new.");  
36 
37             t1.start();                      // 启动“线程t1”
38             System.out.println(t1.getName() +" ("+t1.getState()+") is started.");  
39 
40             // 主线程休眠300ms，然后主线程给t1发“中断”指令。
41             Thread.sleep(300);
42             t1.stopTask();
43             System.out.println(t1.getName() +" ("+t1.getState()+") is interrupted.");
44 
45             // 主线程休眠300ms，然后查看t1的状态。
46             Thread.sleep(300);
47             System.out.println(t1.getName() +" ("+t1.getState()+") is interrupted now.");
48         } catch (InterruptedException e) {  
49             e.printStackTrace();
50         }
51     } 
52 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**运行结果**：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
t1 (NEW) is new.
t1 (RUNNABLE) is started.
t1 (RUNNABLE) loop 1
t1 (RUNNABLE) loop 2
t1 (TIMED_WAITING) is interrupted.
t1 (RUNNABLE) loop 3
t1 (TERMINATED) is interrupted now.
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
### **4. interrupted() 和 isInterrupted()的区别**
最后谈谈 interrupted() 和 isInterrupted()。
interrupted() 和 isInterrupted()都能够用于检测对象的“中断标记”。
区别是，interrupted()除了返回中断标记之外，它还会清除中断标记(即将中断标记设为false)；而isInterrupted()仅仅返回中断标记。
**更多内容**
**1. [Java多线程目录(共xx篇)](http://www.cnblogs.com/skywang12345/p/java_threads_category.html)**
**2. [Java多线程系列--“基础篇”01之 基本概念](http://www.cnblogs.com/skywang12345/p/3479024.html)**
**3. [Java多线程系列--“基础篇”02之 常用的实现多线程的两种方式](http://www.cnblogs.com/skywang12345/p/3479063.html)**
**4. [Java多线程系列--“基础篇”03之 Thread中start()和run()的区别](http://www.cnblogs.com/skywang12345/p/3479083.html)**
**5. [Java多线程系列--“基础篇”04之 synchronized关键字](http://www.cnblogs.com/skywang12345/p/3479202.html)**
**6. [Java多线程系列--“基础篇”05之 线程等待与唤醒](http://www.cnblogs.com/skywang12345/p/3479224.html)**
**7. [Java多线程系列--“基础篇”06之 线程让步](http://www.cnblogs.com/skywang12345/p/3479243.html)**
**8. [Java多线程系列--“基础篇”07之 线程休眠](http://www.cnblogs.com/skywang12345/p/3479256.html)**
**9. [Java多线程系列--“基础篇”08之 join()](http://www.cnblogs.com/skywang12345/p/3479275.html)**
