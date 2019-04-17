# java多线程学习笔记 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:03:03[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：61








# 概念对比

## 进程 & 线程

> 
*进程*：进程（Process）是计算机中的程序关于某数据集合上的一次运行活动，是系统进行资源分配和调度的基本单位，是操作系统结构的基础。进程是线程的容器，也是程序的实体。*（进程是资源分配的最小单位）*

*线程*：线程，有时被称为轻量级进程(Lightweight Process，LWP），线程是进程中的一个实体，是被系统独立调度和分派的基本单位，线程自己不拥有系统资源，只拥有一点儿在运行中必不可少的资源，但它可与同属一个进程的其它线程共享进程所拥有的全部资源。每一个程序都至少有一个线程，若程序只有一个线程，那就是程序本身。*（线程是cpu调度的最小单位）*
## 并行 & 并发

> 
*并行*：并行在操作系统中是指，一组程序按独立异步的速度执行，无论从微观还是宏观，程序都是一起执行的。 

*并发*：并发是指:在同一个时间段内，两个或多个程序执行，有时间上的重叠(宏观上是同时,微观上仍是顺序执行)。
# 实现方法

## 继承Thread类：

a) 继承Thread类 

b) 重写run方法 

c) 调用start方法 

d）调用start方法并不是直接执行多线程，而是使线程成为Runnable状态
```
public class ThreadTest{
    public static void main(String [] args){
        new myThread1("一线程").start();
        new myThread1("二线程").start();
    }
}
class myThread1 extends Thread{
    private String name;
    public myThread1(String name){
        this.name=name;
    }

    @Override
    public void run() {
        for(int i=0;i<10;i++){
            System.out.println(name+"运行"+i);
        }
    }
}
```

## 2.实现Runnable接口：

a) 类实现Runnable接口 

b) 以该类实例作为Thread构造参数，构造Thread类实例 

c) 调用Thrad的start方法 

d）多线程代码最终都是通过调用Thread的start方法来运行
```
public class ThreadTest{
    public static void main(String [] args){
        new Thread(new myThread2("三线程")).start();
        new Thread(new myThread2("四线程")).start();
    }
}
class myThread2 implements Runnable{
    private String name;
    public myThread2(String name){
        this.name=name;
    }

    @Override
    public void run() {
        for(int i=0;i<10;i++){
            System.out.println(name+"运行"+i);
        }
        try{
            Thread.sleep((int)Math.random()*10);
        }catch (InterruptedException e){
            e.printStackTrace();
        }
    }
    //因为需要实例化多个不同的Thread对象，各自享有资源，而实现Runnable接口可以将同一个实现接口的实例传给Thread构造方法，所以不同的Thread类实例其实享有同样资源**
}
```

## 3.实现Callable接口：

a) 类实现Callable接口 

b) 以该类实例作为FutureTask构造参数，构造FutureTask类实例(对比实现Runnable接口) 

c) 以该FutureTask类实例作为Thread构造参数，构造Thread类实例 

d) 调用Thrad的start方法  
```
public class ThreadTest{
    public static void main(String [] args){
        new Thread(new FutureTask<Integer>(new myThread3("五线程"))).start();
        new Thread(new FutureTask<Integer>(new myThread3("六线程"))).start();
    }
}
class myThread3 implements Callable<Integer>{
    private String name;
    public myThread3(String name){
        this.name=name;
    }

    @Override
    public Integer call() throws Exception { //有返回值,直接抛出异常
        int sum=0;
        for(int i=0;i<10;i++){
            sum+=i;
            System.out.println(name+"运行"+i);
        }
        return sum;
    }
}
```

## 区别对比

### implements Callable vs implements Runnable

a) 实现Callable有返回值，而实现Runnable没有返回值 

b) 实现Callable支持抛出异常，而实现Runnable不能抛出

### extends Thread vs implements Runnable

a) 继承Thread有单继承局限，实现Runnable可以实现多个接口 

b) 实现Runnable接口可以较方便实现资源共享，而继承Thread较为麻烦。

### 注意：

当FutureTask处于未启动或者是已启动状态时，此时还未得到线程执行结果，调用FutureTask.get方法会导致*线程阻塞*

–>导致get方法以下未启动的线程无法正常执行  

当FutureTask处于已完成状态时，此时已经得到线程执行结果，调用FutureTask.get方法会立即返回线程执行结果

## 线程的几种状态：

> 
新建状态(New)：新建了一个线程对象 

  就绪状态(Runnable)：调用了**start**方法，线程处于就绪状态，等待获得CPU时间 

  运行状态(Running)：就绪状态的线程获得CPU时间，开始真正执行**run**方法 

  阻塞状态(Blocked)：线程暂时停止运行，让出CPU，使得其他就绪状态的线程获得CPU时间 

  —->等待阻塞：运行的线程执行**wait**（wait会释放持有的锁）方法 

  —->同步阻塞：线程获取同步锁，而同步锁被其他线程占用 

  —->其他阻塞：线程执行**sleep**（sleep不会释放持有的锁），**join**方法，或者发出了I/O请求 

  死亡状态(Dead)：线程执行完毕或者异常退出run方法











