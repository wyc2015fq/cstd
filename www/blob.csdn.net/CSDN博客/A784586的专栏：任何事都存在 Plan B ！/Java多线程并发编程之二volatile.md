# Java多线程并发编程之二volatile - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月03日 17:17:30[QuJack](https://me.csdn.net/A784586)阅读数：467












    Java多线程并发编程之二volatile





volatile：【线程安全包括：原子性+可见性】【“外练互斥，内修可见”】

1.多线程可以解决同步死循环问题；按顺序执行的两个函数，其中前个有死循环，后面一个修改循环条件变量；

2.在服务器上执行时候，共享变量需要加上volatile，使得变量变化可见，服务器执行的时候，为了提高效率，线程一直在私有堆栈中取值；【变量在公共堆栈和线程私有堆栈】；

3.volatile和synchronized比较：

    a.volatile是线程同步的轻量级实现；性能自然要好；

    b.volatile只能修饰变量，synchronized可以修饰方法，块；

    c.volatile不会阻塞；

    d.volatile保证可见性，不保证原子性，synchronized能保证原子性，间接保证了可见性；

4.volatile public static int i; i++;i++不是原子操作，拆分为了三步，volatile不能保证原子性；volatile只能保证线程从主存加载数据到线程工作内存的值是最新的。，保证的是读数据时候的可见性，无法保证原子性，多线程访问同一个实例变量，任需要加synchronized。

5.原子类的方法是原子的，但是方法之间的调用不是原子的，所以多次调用原子类的多个方法时候，原子类不能保证安全，需要加同步修饰；

6.synchronized块有volatile同步的功能。一个线程执行同步方法的死循环，另外个线程执行另外个方法，修改死循环条件，若需要前者跳出死循环：a.循环条件的变量前需要volatile；b.死循环里面用synchronized("haha"){.........}



共有四个文件：     
java实现代码说明a和b两种方法的一致效果；

MyServer.java包含一个死循环方法，一个改变循环条件的方法；一个循环标志flag变量；


```java
package com.mytest.volatileandsynchronized;

public class MyService {
	 private boolean flag=true;//方法一：前面加volatile
	//运行死循环方法
	public void runMethod(){
		while(flag){
			//方法二：synchronized("aaa"){}  间接保证可见性
		}
		System.out.println("RunMethod end");
	}
	//改变循环条件
	public void stopMethod(){
		flag=false;
	}
	
}
```






ThreadA.java调用死循环方法：


```java
package com.mytest.volatileandsynchronized;

public class ThreadA extends Thread {
	private MyService myService;
	public ThreadA(MyService  myService){
		super();
		this.myService=myService;
	}
	@Override
	public void run() {
		myService.runMethod();
	}
}
```


ThreadB.java调用改变循环条件的方法


```java
package com.mytest.volatileandsynchronized;

public class ThreadB extends Thread {
	private MyService myService;
	public ThreadB(MyService  myService){
		super();
		this.myService=myService;
	}
	@Override
	public void run() {
		myService.stopMethod();
	}
}
```

MyRun.java 多线程测试主程序



```java
package com.mytest.volatileandsynchronized;

public class MyRun {
	public static void main(String[] args) throws Exception {
		MyService service=new MyService();
		ThreadA a=new ThreadA(service);
		ThreadB b=new ThreadB(service);
		a.start();
		Thread.sleep(1000);
		b.start();
		System.out.println("已经发出停止指令了");

	}

}
```
没有方法1和方法2，则无法跳出死循环：运行效果

![](https://img-blog.csdn.net/20170403171101631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



修改MyService.java 使用方法1或者方法2，效果一致 运行如下图：

![](https://img-blog.csdn.net/20170403171315430?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


-------------------------------------------------

附：

Java 多线程编程基础



进程：每个进程都有独立的代码和数据空间（进程上下文），进程间的切换会有较大的开销，一个进程包含1--n个线程。

线程：同一类线程共享代码和数据空间，每个线程有独立的运行栈和程序计数器(PC)，线程切换开销小。

线程和进程一样分为五个阶段：创建、就绪、运行、阻塞、终止。

多进程是指操作系统能同时运行多个任务（程序）。

多线程是指在同一程序中有多个顺序流在执行。


在java中要想实现多线程，有两种手段，一种是继续Thread类，另外一种是实现Runable接口

**一种是继续Thread类：**



```java
package test;

public class ThreadOne extends Thread{

    private String name;
    public ThreadOne(String name){
        this.name=name;
    }

    public void run(){
        for(int i=0;i<5;i++){
            System.out.println(name+" 运行 "+i);
        }

        try {
            sleep((int)Math.random()*10);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
    public static void main(String[] args) {
        ThreadOne thread01=new ThreadOne("A");
        ThreadOne thread02=new ThreadOne("B");
        thread01.start();
        thread02.start();    
    }

}
```








说明：

程序启动运行main时候，java虚拟机启动一个进程，主线程main在main()调用时候被创建。随着调用MitiSay的两个对象的start方法，另外两个线程也启动了，这样，整个应用就在多线程下运行。



注意：start()方法的调用后并不是立即执行多线程代码，而是使得该线程变为可运行态（Runnable），什么时候运行是由操作系统决定的。

从程序运行的结果可以发现，多线程程序是乱序执行。因此，只有乱序执行的代码才有必要设计为多线程。

Thread.sleep()方法调用目的是不让当前线程独自霸占该进程所获取的CPU资源，以留出一定时间给其他线程执行的机会。

实际上所有的多线程代码执行顺序都是不确定的，每次执行的结果都是随机的。





**实现Runable接口：**


```java
package test;

public class ThreadTwo implements Runnable {

     private String name;
        public ThreadTwo(String name){
            this.name=name;
        }

    @Override
    public void run() {
         for(int i=0;i<5;i++){
                System.out.println(name+" 运行 "+i);
            }

            try {
                Thread.sleep((int)Math.random()*10);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }

    }
    public static void main(String[] args) {
        ThreadTwo thread03=new ThreadTwo("C");
        ThreadTwo thread04=new ThreadTwo("D");
        new Thread(thread03).start();
        new Thread(thread04).start();
    }

}
```






--------------------------------------------------------------------


synchronized 和ReentrantLock 稍作比较


**对 synchronized 的改进**


同步是不错，但它并不完美。它有一些功能性的限制 —— 它无法中断一个正在等候获得锁的线程，也无法通过投票得到锁，如果不想等下去，也就没法得到锁。同步还要求锁的释放只能在与获得锁所在的堆栈帧相同的堆栈帧中进行，多数情况下，这没问题（而且与异常处理交互得很好），但是，确实存在一些非块结构的锁定更合适的情况。


**ReentrantLock 类**

`java.util.concurrent.lock`中的`Lock`框架是锁定的一个抽象，它允许把锁定的实现作为
 Java 类，而不是作为语言的特性来实现。这就为`Lock`的多种实现留下了空间，各种实现可能有不同的调度算法、性能特性或者锁定语义。


`ReentrantLock` 类实现了`Lock` ，它拥有与 `synchronized` 相同的并发性和内存语义，但是添加了类似锁投票、定时锁等候和可中断锁等候的一些特性。此外，它还提供了在激烈争用情况下更佳的性能。（换句话说，当许多线程都想访问共享资源时，JVM
 可以花更少的时候来调度线程，把更多时间用在执行线程上。）

reentrant锁意味着什么呢？简单来说，它有一个与锁相关的获取计数器，如果拥有锁的某个线程再次得到锁，那么获取计数器就加1，然后锁需要被释放两次才能获得真正释放。这模仿了`synchronized`的语义；如果线程进入由线程已经拥有的监控器保护的
 synchronized 块，就允许线程继续进行，当线程退出第二个（或者后续）`synchronized`块的时候，不释放锁，只有线程退出它进入的监控器保护的第一个`synchronized`块时，才释放锁。

`Lock`和
 synchronized 区别 —— lock 必须在 finally 块中释放。否则，如果受保护的代码将抛出异常，锁就有可能永远得不到释放！这一点区别看起来可能没什么，但是实际上，它极为重要。忘记在 finally 块中释放锁，可能会在程序中留下一个定时炸弹，当有一天炸弹爆炸时，您要花费很大力气才有找到源头在哪。而使用同步，JVM 将确保锁会获得自动释放。










