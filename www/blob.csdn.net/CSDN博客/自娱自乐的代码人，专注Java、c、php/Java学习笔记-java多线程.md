# Java学习笔记 - java多线程 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年04月01日 13:22:39[initphp](https://me.csdn.net/initphp)阅读数：1516
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









**线程简介：**

线程，有时被称为轻量级进程(Lightweight Process，LWP），是程序执行流的最小单元。一个标准的线程由线程ID，当前指令指针(PC），寄存器集合和堆栈组成。另外，线程是进程中的一个实体，是被系统独立调度和分派的基本单位，线程自己不拥有系统资源，只拥有一点在运行中必不可少的资源，但它可与同属一个进程的其它线程共享进程所拥有的全部资源。一个线程可以创建和撤消另一个线程，同一进程中的多个线程之间可以并发执行。由于线程之间的相互制约，致使线程在运行中呈现出间断性。线程也有就绪、阻塞和运行三种基本状态。每一个程序都至少有一个线程，若程序只有一个线程，那就是程序本身。




**JAVA中的多线程：**



在java中要想实现多线程，有两种手段，一种是继续Thread类，另外一种是实现Runable接口。

1. 直接继承Thread



```java
package com.xxx.controller.api;


public class Test extends Thread {
    
    
    public void run() {
        System.out.println("线程ID：" + Thread.currentThread());
    }
 
    public static void main(String[] args) {
        Test t1 = new Test();
        Test t2 = new Test();
        t1.start();
        t2.start();
    }
}
```


结果：





```
线程ID：Thread[Thread-0,5,main]
线程ID：Thread[Thread-1,5,main]
```


2. 实现
```java
Runnable
```
接口



```java
package com.xxx.controller.api;


public class Test implements Runnable {
    
    
    public void run() {
        System.out.println("线程ID：" + Thread.currentThread());
    }
 
    public static void main(String[] args) {
        Test test = new Test();
        Thread t1 = new Thread(test);
        Thread t2 = new Thread(test);
        t1.start();
        t2.start();
    }
}
```


结果：





```
线程ID：Thread[Thread-0,5,main]
线程ID：Thread[Thread-1,5,main]
```


一般情况下，我们都会使用“实现
```java
Runnable
```
接口”方法。


```java
Runnable
```
比直接继承Thread的优势：
- 适合多个相同的程序代码的线程去处理同一个资源
- 可以避免java中的单继承的限制
- 增加程序的健壮性，代码可以被多个线程共享，代码和数据独立。

**线程常用：**



```java
package com.xxx.controller.api;


public class Test implements Runnable {

    public void run() {
        System.out.println(Thread.currentThread().getName()); //打印线程名称
        System.out.println(Thread.currentThread() + "   休眠开始"); //打印出线程ID
        try {
        Thread.sleep(2000); //线程休眠2秒
        } catch (Exception e) {}
        System.out.println(Thread.currentThread() + "   休眠结束");
    }
 
    public static void main(String[] args) {
        Test test = new Test();
        Thread t1 = new Thread(test, "线程1"); //可以给线程命名
        Thread t2 = new Thread(test, "线程2");
        t2.setPriority(9); //设置线程优先级，值越大，越先执行
        t1.setPriority(2);
        System.out.println("t1 线程启动前状态" + t1.isAlive()); //线程状态
        t1.setDaemon(true); //设置true后，就算java进程关闭，线程也会在后台运行
        t1.start();
        System.out.println("t1 线程启动后状态" + t1.isAlive());
        t1.interrupt(); //中断休眠线程
        t2.start();
    }
}
```


返回结果：





```
t1 线程启动前状态false
t1 线程启动后状态true
线程2
Thread[线程2,9,main]   休眠开始
线程1
Thread[线程1,2,main]   休眠开始
Thread[线程1,2,main]   休眠结束
Thread[线程2,9,main]   休眠结束
```










****






