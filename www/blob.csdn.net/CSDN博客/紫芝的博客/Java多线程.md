# Java多线程 - 紫芝的博客 - CSDN博客





2018年12月31日 20:41:30[紫芝](https://me.csdn.net/qq_40507857)阅读数：1395








## 继承Thread类创建多线程

*单线程，main()方法的打印语句不会执行*

```java
public class Main {
    public static void main(String[] args) {
        //单线程程序
        MyThread myThread = new MyThread();//创建实例对象
        myThread.run();
        while (true) {//不会执行
            System.out.println("Main方法正在运行");
        }
    }
}

class MyThread {
    public void run() {
        while (true) {//一直输出
            System.out.println("MyThread类中的run()方法在运行");
        }
    }
}
```

*继承Thread类、重写run()方法，实现多线程*

```java
public class Main {
    public static void main(String[] args) {
        //多线程程序
        MyThread myThread = new MyThread();//创建实例对象
        myThread.start();//开启多线程
        while (true) {//会执行
            System.out.println("Main方法正在运行");
        }
    }
}

class MyThread extends Thread {
    public void run() {
        while (true) {//一直输出
            System.out.println("MyThread类中的run()方法在运行");
        }
    }
}
```

## 实现Runnable接口创建多线程

Java只支持单继承，一个类一旦继承了某个类就无法继承Thread类

**Thread类提供另外一个构造方法：Thread(Runnable target)**

```java
public class Main {
    public static void main(String[] args) {
        //多线程程序
        MyThread myThread = new MyThread();//创建实例对象
        Thread thread = new Thread(myThread);//创建线程对象
        thread.start();//开启多线程，执行run()方法
        while (true) {//会执行
            System.out.println("Main方法正在运行");
        }
    }
}

class MyThread implements Runnable {
    public void run() {//线程代码段，当调用start()方法时，线程从此处开始执行
        while (true) {//一直输出
            System.out.println("MyThread类中的run()方法在运行");
        }
    }
}
```

## 两种实现方式的比较

*每张票被打印了四次，每个进程独立的处理各自的资源*

```java
public class Main {
    public static void main(String[] args) {
        new TicketWindows().start();//创建第一个线程对象并开启
        new TicketWindows().start();//创建第二个线程对象并开启
        new TicketWindows().start();//创建第三个线程对象并开启
        new TicketWindows().start();//创建第四个线程对象并开启
    }
}

class TicketWindows extends Thread {
    private int ticket = 100;

    public void run() {

        while (true) {
            if (ticket > 0) {
                Thread th = Thread.currentThread();//获取当前线程
                String th_name = th.getName();//获取当前线程的名字
                System.out.println(th_name + "正在发售第" + ticket-- + "张票");
            }
        }
    }

}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181231203010375.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

*为了保证资源共享，在程序中只能创建一个售票对象，然后开启多个线程去运行同一个售票对象方法*

```java
public class Main {
    public static void main(String[] args) {
        TicketWindows task = new TicketWindows();//创建线程任务类对象
        new Thread(task, "windows 1").start();//创建第一个线程对象并开启
        new Thread(task, "windows 2").start();//创建第二个线程对象并开启
        new Thread(task, "windows 3").start();//创建第三个线程对象并开启
        new Thread(task, "windows 4").start();//创建第四个线程对象并开启
    }
}

class TicketWindows implements Runnable {
    private int ticket = 100;

    public void run() {

        while (true) {
            if (ticket > 0) {
                Thread th = Thread.currentThread();//获取当前线程
                String th_name = th.getName();//获取当前线程的名字
                System.out.println(th_name + "正在发售第" + ticket-- + "张票");
            }
        }
    }

}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181231204051947.png)

## 线程的优先级

```java
public class Main {
    public static void main(String[] args) {
        //创建两个线程
        Thread minPriority = new Thread(new Task(), "优先级较低的线程");
        Thread maxPriority = new Thread(new Task(), "优先级较高的线程");
        minPriority.setPriority(Thread.MIN_PRIORITY);//设置线程的优先级为1
        maxPriority.setPriority(Thread.MAX_PRIORITY);//设置线程的优先级为10
        minPriority.start();
        maxPriority.start();
    }
}

class Task implements Runnable {


    public void run() {
        for (int i = 0; i < 10; i++) {
            System.out.println(Thread.currentThread().getName() + "正在输出" + i);
        }

    }
}
```

## Java创建三个进程，分别显示各自的时间

```java
import java.util.Date;

public class Main {
    public static void main(String[] args) {
        Time task = new Time();
        new Thread(task).start();
        new Thread(task).start();
        new Thread(task).start();
    }
}

class Time implements Runnable {
    public void run() {
        Thread th = Thread.currentThread();
        Date date = new Date();
        System.out.println(th.getName() + ":  " + date.toString());
    }
}
```

**继承Thrad类实现多线程**

```java
import java.text.SimpleDateFormat;
import java.util.Date;

public class Main {
    public static void main(String[] args) {
        new ThreeTimeThread("first").start();
        new ThreeTimeThread("second").start();
        new ThreeTimeThread("thrid").start();
    }
}

class ThreeTimeThread extends Thread {
    public ThreeTimeThread(String name) {
        super(name);
    }

    public void run() {
        while (true) {
            SimpleDateFormat formatter = new SimpleDateFormat("yyyy.MM.dd G 'at' hh:mm:ss z");
            Date currentTime = new Date();
            try {
                sleep(1000);
            } catch (Exception e) {
            }
            String dateString = formatter.format(currentTime);
            System.out.println(getName() + ":" + currentTime.toString());
        }
    }
}
```



