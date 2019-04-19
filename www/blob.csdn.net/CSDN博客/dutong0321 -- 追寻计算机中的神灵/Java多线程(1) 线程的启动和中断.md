# Java多线程(1) 线程的启动和中断 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2017年11月29日 22:10:57[dutong0321](https://me.csdn.net/dutong0321)阅读数：100
### 概述
去年写过一篇关C/C++的多线程介绍，现在回看，简直是一塌糊涂。以后有机会，会重新改写的。最近开始看Java多线程，所以打算边输入边输出的方式来写这一套读书笔记。其实，多线程在实际项目中已经用过很多了，私以为网络编程和多线程编程是一个程序员熟悉一门语言最基本的功底了。安卓程序员还好，在4.0后要求网络编程等耗费时间的操作，必须开启新的线程，不得在绘制界面的主线程中操作，避免出现无响应。对于开发WEB的同学可能就觉得无所谓了，其实开发WEB当中，必须是多线程，不过是Java或者是框架帮助完成了，其实很多高级的操作也是需要开发多线程的。对于开发网络桌面程序，其实多线程的应用也是有很多的，这里就不一一叙述了。多线程的用途其实有很多，例如：需要中断等待资源的程序开辟一条进程，可以保证主线程继续运行，最常见的就是网络编程。以下的例子将不会设计图形界面和网络编程，参照书籍是《Java核心技术 卷1》中的第14章，但是我的将会比其更加详细，理论少些，代码多些，易懂不易懂就不一定了^-^。
### 创建多线程
```java
package com.utopia.createThread;
// 实现Runnable接口类
public class CreateRunnable implements Runnable {
    private String message;
    public CreateRunnable(String message) {
        this.message = message;
    }
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        // 创建Runnable的接口类对象
        Runnable createRunnable1 = new CreateRunnable("Thread1 ");
        Runnable createRunnable2 = new CreateRunnable("Thread2 ");
        // 由Runnable创建一个Thread对象
        Thread thread1 = new Thread(createRunnable1);
        Thread thread2 = new Thread(createRunnable2);
        // 开启两个线程
        thread1.start();
        thread2.start();
    }
    // 重写run方法(将线程中的需要运行的程序写到该方法中)
    @Override
    public void run() {
        // TODO Auto-generated method stub
        for (int i = 0; i < 1000; i++) {
            System.out.println(message + " count: " + i);
        }
    }
}
```
部分运行截图：
```
...
Thread1  count: 6
Thread2  count: 10
Thread1  count: 7
Thread2  count: 11
Thread2  count: 12
Thread1  count: 8
Thread2  count: 13
...
```
如果发现截图和我不一样，那么可能是电脑配置高，运行速度快，建议把循环次数增加，只要多试几次，你就会发现thread1和thread2是杂乱随机输出的。
#### 采用Thread的子类方法启动线程
在实际工程中，一般都是采用Runnable的方式来实现多线程的，不采用Thread的子类的原因是： 
1.Java是单继承的，如果继承自Thread那么就无法继承其他类 
2.代码可以被多个线程共享 
这里简单做个例子：
```java
package com.utopia.createThread;
public class CreateThread extends Thread {
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        Thread thread = new CreateThread();
        thread.start();
    }
    @Override
    public void run() {
        // TODO Auto-generated method stub
        super.run();
        System.out.println("I'm child thread!");
    }
}
```
简单的截图：
`I'm child thread!`
### 线程的中断
#### stop()方法
stop()方法目前已经被弃用，原因是因为其太暴力了。其中止线程所有未结束的方法，立即释放锁住的所有对象的锁(用于保护对象，将会在后面章节详解)，会破坏对象。
#### interrupt()方法
其实stop()方法就像方法名一样是结束，而interrupt()方法才是中断，更准确的说是请求中断。请求中断必须要有相对应的方法来相应中断，然后处理好善后方法后结束自己。stop是从外部(主线程)直接结束子线程的，而interrupt是从外部(主线程)请求子线程结束，然后由子线程相应内部(结束)。更加直白的说，就是stop是意外死亡，而interrupt如预知天命般，知道自己离去时间，安排好身后事，然后安然离去。 
sleep() 如其名，使线程休眠，参数为int意为毫秒 
interrupted() 子线程内使用，检测是否发来interrupt()信号
```java
package com.utopia.createThread;
public class InterruptRunnable implements Runnable {
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        try {
            Runnable interruptRunnable = new InterruptRunnable();
            Thread thread = new Thread(interruptRunnable);
            // 线程启动
            thread.start();
            // 主线程休眠10毫秒
            Thread.sleep(10);
            // 向子线程发出中断请求
            thread.interrupt();
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
    @Override
    public void run() {
        // TODO Auto-generated method stub
        for (int i = 0; i < 10000; i++) {
            // 检测是否发出中断信号
            if (Thread.currentThread().isInterrupted()) {
                System.out.println("Child thread have get interrupted!");
                // 检测到后跳出循环
                break;
            }
            System.out.println("Thread count " + i);
        }
        // 检测到中断信号，跳出循环，执行输出代码后程序结束，如果希望检测到中断信号即可结束时，应将if循环体中的代码块换为return即可
        System.out.println("Thread will be really finish!");
    }
}
```
部分运行截图：
```
...
Thread count 219
Thread count 220
Child thread have get interrupted!
Thread will be really finish!
```
#### interrupted的注解
interrupted只可以相应一次，也就是说线程在被请求中断后，第一次使用interrupted就会返回true，第二次则会返回false。
```java
package com.utopia.createThread;
public class InterruptThread {
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        // 对本线程发出中断请求
        Thread.currentThread().interrupt();
        // 第一次相应中断
        System.out.println("First interrupted:" + Thread.interrupted());
        // 第二次相应中断
        System.out.println("Second interrupted:" + Thread.interrupted());
    }
}
```
截图
```
First interrupted:true
Second interrupted:false
```
#### 当sleep遇到了interrupt(待决中断)
如果子线程在被interrupt()后，其内部尚未采用interrupted()来响应，却调用了sleep()函数，那么就会抛出InterruptedException异常，这就是待决中断（其实，在使用sleep()时，Eclipse会提示捕捉异常就是捕捉InterruptedException）。
```java
package com.utopia.createThread;
public class MayRunnable implements Runnable {
    @Override
    public void run() {
        // TODO Auto-generated method stub
        for (int i = 0; i < 10; i++) {
            System.out.println("Child Thread sleep count:" + i);
            try {
                // 子线程在被interrupt后，尚未使用isInterrupted()，却使用了sleep()
                Thread.sleep(1);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                System.out.println("It's InterrupttedException!");
                return;
            }
        }
    }
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        Thread thread = new Thread(new MayRunnable());
        thread.start();
        try {
            Thread.sleep(2);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        thread.interrupt();
    }
}
```
运行截图
```
Child Thread sleep count:0
Child Thread sleep count:1
It's InterrupttedException!
```
至此，Java多线程的启动和中断便暂时告一段落了。 
不日我便将会更新第2篇。
