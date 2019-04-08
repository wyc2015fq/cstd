# Java线程相关笔记







# 一、基础知识

### 1、什么是进程？

正在运行的程序；是系统进行资源分配和调度的独立单位；每一个进程都有自己的内存空间和系统资源。

### 2、多进程的意义

多进程可以同时执行多件事，单进程只能一次执行一件事了。提高cpu的使用率

### 3、什么是线程？

在一个进程内可以执行多个任务，每一个任务可以看作为一个线程，是程序执行的单元，是程序使用cpu的基本单位。

### 4、多线程的意义

多线程的存在，不是提高程序的运行速度，其实是为了提高应用程序的使用率。

### 5、Java 程序运行原理

Java 命令启动 Java 虚拟机，启动 jvm ，等于启动一个应用程序，就是启动一个进程，该进程会自动启动一个“主线程”，主线程调用各个类的 main 方法。main 方法运行在主线程当中。

jvm启动是多线程的，最少是2个线程，一个是垃圾回收线程（先启动），另一个是主线程。

### 6、Java 实现多线程程序

线程由进程创建，进程由系统创建。但是 Java 不能直接调用系统功能，所以没办法直接实现多线程程序，但是 Java 可以调用 c/c++ 写好的程序实现多线程程序。因为 c/c++ 可以创建进程，然后 Java 调用一下类内的方法即可实现多线程。

# 二、开启一个工作线程

### 1、继承自 Thread 类实现开启多线程

```
public class Main {

    public static void main(String[] args) {
        //同时开启两个线程
        MyThread t1 = new MyThread();
        MyThread t2 = new MyThread();
        t1.start();
        t2.start();
    }

    static class MyThread extends Thread {
        @Override
        public void run() {
            for (int i = 0; i < 10000; i++) {
                System.out.println(i);
            }
        }
    }
}
```

`getName()` 获取线程的名称
`getId()` 获取线程Id

### 2、关于线程名称的修改以及，获取

两种方式，构造方法与 setName 方法

```
public class Main {

    public static void main(String[] args) {

        //同时开启两个线程
        MyThread t1 = new MyThread();
        //第二种线程命名方法
        t1.setName("线程1:");
        //第一种线程命名方法
        MyThread t2 = new MyThread("线程2");
        //启动线程
        t1.start();
        t2.start();
        
        //获取当前线程名称
        System.out.println(Thread.currentThread().getName());

    }

    static class MyThread extends Thread {
        MyThread() {

        }

        MyThread(String name) {
            super(name);
        }

        @Override
        public void run() {
            for (int i = 0; i < 10000; i++) {
                System.out.println(i + ":" + getName() + ":" + getId());
            }
        }
    }
}
```

### 3、设置和获取线程的优先级

由于 Java 对 cpu 的使用属于抢占模式，所以各个线程在使用 cpu 的权利是优先级高则获取 cpu 的几率较高。线程有限的设置值为：1~~10 的整数，默认为 5。

```
public class Main {

    public static void main(String[] args) {

        //同时开启两个线程
        MyThread t1 = new MyThread();
        MyThread t2 = new MyThread();
        //设置线程优先级
        t1.setPriority(1);
        t2.setPriority(10);
        //启动线程
        t1.start();
        t2.start();

        //获取线程优先级
        System.out.println("线程1的优先级：" + t1.getPriority());
        System.out.println("线程2的优先级：" + t2.getPriority());
    }

    static class MyThread extends Thread {
        @Override
        public void run() {
            for (int i = 0; i < 10000; i++) {
                System.out.println(i + ":" + getName() + ":" + getId());
            }
        }
    }
}
```

### 4、线程控制

##### 1）、线程休眠

让线程休息是一段时间再次执行。

```
public class Main {

    public static void main(String[] args) {

        //同时开启两个线程
        MyThread t1 = new MyThread();
        t1.setName("测试线程 1");
        //启动线程
        t1.start();
    }

    static class MyThread extends Thread {
        @Override
        public void run() {
            for (int i = 0; i < 100; i++) {

                //线程休眠 1 秒，再执行以下代码
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                System.out.println(i + ":" + getName());
            }
        }
    }
}
```

##### 2）、加入线程

调用线程的 join（）方法，待该线程执行完毕之后，再执行其他线程。

```
public class Main {

    public static void main(String[] args) {

        //同时开启两个线程
        MyThread t1 = new MyThread();
        MyThread t2 = new MyThread();
        MyThread t3 = new MyThread();
        t1.setName("测试线程 1 : ");
        t2.setName("测试线程 2 ： ");
        t3.setName("测试线程 3 ： ");
       
        t1.start();
        
        //无参 线程 1 执行完毕，其他线程才可执行 ；
        // 一个参数 线程1执行在规定毫秒之后 其他线程执行； 
        // 两个参数 线程1执行在规定毫秒+纳秒之后 其他线程执行。
        try {
            t1.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        t2.start();
        t3.start();
    }

    static class MyThread extends Thread {
        @Override
        public void run() {
            for (int i = 0; i < 100; i++) {
                System.out.println(getName() + i);
            }
        }
    }
}
```

##### 3）、礼让线程

可以使线程的执行更加和谐，但是不能完全和谐（总结，没啥用）。

```
public class Main {

    public static void main(String[] args) {

        //同时开启两个线程
        MyThread t1 = new MyThread();
        MyThread t2 = new MyThread();

        t1.setName("测试线程 1 : ");
        t2.setName("测试线程 2 ： ");

        t1.start();
        t2.start();

    }

    static class MyThread extends Thread {
        @Override
        public void run() {
            for (int i = 0; i < 100; i++) {

                //礼让方法
                Thread.yield();

                System.out.println(getName() + i);
            }
        }
    }
}
```

##### 4）、后台线程---->守护线程

将目标线程配置为守护线程即，在该线程启动前调用 setDaemon(boolean no); 方法，即可将该线程设置为守护线程。当进程内全部为守护线程时，该程序停止运行。

```
public class Main {

    public static void main(String[] args) {

        //同时开启两个线程
        MyThread t1 = new MyThread();
        MyThread t2 = new MyThread();

        t1.setName("测试线程 1 : ");
        t2.setName("测试线程 2 ： ");
        
        //配置守护线程
        t1.setDaemon(true);
        t2.setDaemon(true);

        t1.start();
        t2.start();

        for (int i = 0; i < 10; i++) {
            System.out.println(Thread.currentThread().getName() + " : " + i);
        }

    }

    static class MyThread extends Thread {
        @Override
        public void run() {
            for (int i = 0; i < 100; i++) {
                System.out.println(getName() + i);
            }
        }
    }
}
```

#### 5）、终止线程

###### a、stop() 方法

`public final void stop` 让线程停止，该方法已经过时,不建议使用。

`public void interrupt` 中断线程，将线程状态进行终止，并抛出一个 InterruptedException。

```
public class Main {

    public static void main(String[] args) {

        //同时开启两个线程
        MyThread t1 = new MyThread();
        t1.start();

        try {
            Thread.sleep(3000);
            // t1.stop();
            t1.interrupt();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("程序运行完毕");
    }

    static class MyThread extends Thread {
        @Override
        public void run() {
            for (int i = 0; i < 100; i++) {

                System.out.println("循环数据： " + i);

                if (i == 20) {
                    try {
                        System.out.println("我累了，我要睡 10s");
                        Thread.sleep(10000);
                    } catch (InterruptedException e) {
                        //当使用 interrupt() 方法终止线程时候，在这里进行后期处理，因为肯定会抛出该异常
                        System.out.println("抛出异常");
                    }
                }
            }
        }
    }
}
```

##### 5)、线程的生命周期

###### a、正常的生命周期

新建：创建线程对象；
就绪：有执行资格，没有执行权。调用start（）方法之后；
运行：有执行资格，有执行权
阻塞状态：线程调用 wait（）方法，或者是调用 sleep（）方法时，线程进入休眠状态。再次唤醒 调用 notify（）或休眠到期线程会进入就绪状态，之后是运行状态。
死亡：对象编程垃圾等待回收。



![img](https://upload-images.jianshu.io/upload_images/6433394-c6cff0ebaff1bbeb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/966/format/webp)

线程生命周期

### 5、通过实现 Runnable 接口实现多线程

```
public class Main {

    public static void main(String[] args) {
        
        MyRunnable runnable = new MyRunnable();
        Thread thread1 = new Thread(runnable,"测试线程 1");
        Thread thread2 = new Thread(runnable,"测试线程 2");

        thread1.start();
        thread2.start();
    }

    static class MyRunnable implements Runnable {

        @Override
        public void run() {
            for (int i = 0; i < 100; i++) {
                System.out.println(Thread.currentThread().getName() + " : " + i);
            }
        }
    }
}
```

为什么会有两种线程的实现方式？
a、实现 Runnable 接口创建线程避免了 Java 单继承的局限性；
b、Runnable 的方式适合多个相同程序代码处理同一个资源的情况，将线程同程序代码、数据有效分离，体现了面向对象的设计思想。

# 三、线程安全问题

当多个线程操作统一数据的时候，会产生数据紊乱的现象，就涉及到了线程安全问题。

### 1、同步代码块

通过 synchronized 关键字对代码块进行加锁。

synchronized（对象）{
需要同步代码
}

同步可以解决线程安全问题根本原因在于这个对对象上，该对象类似于锁，多个线程必须使用这一把锁，才可保证安全。

代码实现：

```
public class Main {

    public static void main(String[] args) {

        MyRunnable runnable = new MyRunnable();
        Thread thread1 = new Thread(runnable, "丈夫");
        Thread thread2 = new Thread(runnable, "妻子");

        thread1.start();
        thread2.start();
    }

    static class MyRunnable implements Runnable {

        private int money = 100;
        private final Object object = new Object();

        @Override
        public void run() {
            synchronized (object) {

                while (money > 0) {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    System.out.println(Thread.currentThread().getName() + " : 消费剩余" + (money--) + "元");
                }
            }
        }
    }
}
```

同步操作的特点：
前提：多个线程

解决问题的注意点：
多个线程使用同一对象。

同步的好处
同步解决了多线程的安全问题

同步弊端
当线程过多时候，每个线程都会判断同步上的锁，很耗费资源，降低性能。

同步代码块的锁对象是谁？
任意对象

同步方法的格式以及锁对象问题
格式：将同步关键字 synchronized 加载权限修饰符之后即可
锁对象：this

静态方法及锁对象
当前类的 class 对象

# 四、Lock 接口使用

### 1、Lock 简单使用

JDK 5 一种机制，可以方便的理解锁机制。

```
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Main {

    public static void main(String[] args) {

        MyRunnable runnable = new MyRunnable();

        Thread thread1 = new Thread(runnable, "丈夫");
        Thread thread2 = new Thread(runnable, "妻子");
        Thread thread3 = new Thread(runnable, "儿子");
        Thread thread4 = new Thread(runnable, "闺女");

        thread1.start();
        thread2.start();
        thread3.start();
        thread4.start();
    }

    static class MyRunnable implements Runnable {

        private int money = 100;
        private Lock lock = new ReentrantLock();

        @Override
        public void run() {
            //加锁
            lock.lock();
            //加入 try 块目的：不管 try 块内发生了什么，都可以释放掉锁，防止产生不必要的麻烦。
            try {
                while (money > 0) {
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    System.out.println(Thread.currentThread().getName() + " : 消费剩余" + (money--) + "元");
                }
            } finally {
                //释放锁
                lock.unlock();
            }
        }
    }
}
```

### 2、死锁问题

###### 1）、产生原因：

两个或两个以上的线程在争夺资源的过程中，发生的一种相互等待的现象

##### 2）、解决方法：

进行线程间通信（不同种类的线程，针对同一个资源的操作）。

通过 Object 类的 wait（）和 notify（）方法对线程进行等待和唤醒。感觉完全没有 Android 的 handle 好用。

# 五、定时器

### 1、基础知识

定时器可以在指定的时间内做某件事情或者重复的操作某件事情

依赖于 Timer（定时） 和 TimerTask （任务）

### 2、基础使用

程序运行 3s 之后打印日志，之后结束程序，否则定时器会一直存在于内存之中。

```
import java.util.Timer;
import java.util.TimerTask;

public class Main {

    public static void main(String[] args) {

        Timer timer = new Timer();
        //3 秒之后操作 该任务
        timer.schedule(new MyTask(timer), 3000);
    }

    //定时完成的任务，即时间到了之后操作的任务
    static class MyTask extends TimerTask {

        Timer timer;
        MyTask(Timer timer) {
            this.timer = timer;
        }

        @Override
        public void run() {
            System.out.println("测试输出");

            //操作完任务之后，关闭定时器
            timer.cancel();
        }
    }
}
```

持续打印日志

```
import java.util.Timer;
import java.util.TimerTask;

public class Main {

    public static void main(String[] args) {

        Timer timer = new Timer();
        // 持续打印日志
        timer.schedule(new MyTask(), 2000, 2000);
    }

    //定时完成的任务，即时间到了之后操作的任务
    static class MyTask extends TimerTask {

        @Override
        public void run() {
            System.out.println("测试输出");
        }
    }
}
```

小礼物走一走，来简书关注我