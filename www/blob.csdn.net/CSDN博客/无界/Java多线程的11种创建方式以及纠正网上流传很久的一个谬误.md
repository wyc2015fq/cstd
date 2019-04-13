
# Java多线程的11种创建方式以及纠正网上流传很久的一个谬误 - 无界 - CSDN博客

置顶2019年04月06日 21:22:58[21aspnet](https://me.csdn.net/21aspnet)阅读数：387


创建线程比较传统的方式是继承Thread类和实现Runnable，也可以用内部类，Lambda表达式，线程池，FutureTask等。
经常面试会问到继承Thread类和实现Runnable的区别，然后网上会流传如下这样的说法，这是错误的。
## 流传很久的错误说法：
这个说法是举一个火车票售票的例子，大致意思是说实现Runnable接口可以实现多继承，这一点说的是正确的，但是错误的是下面的，那些例子会说实现Runnable接口的多线程可以实现共享，然后会举这个火车票的例子。其实之所以造成他们的说法看起来对是刻意在继承Thread类的时候故意新建几个线程，而成员变量又不是静态的自然是不能共享的。
下面就是那个流传很广的关于继承Thread类和实现Runnable的两种方式的“区别”的来源：
参考类似此文《[创建线程的两种方式比较Thread VS Runnable](https://www.cnblogs.com/lylife/p/4756981.html)》
![](https://img-blog.csdnimg.cn/20190406175116555.png)
## 说明：上面截图中的是片面的，然后很多培训机构和很多博主还引用这个，所以有必要澄清下这个问题。
## 正确的说法是：继承Thread类和实现Runnable的最本质的区别是继承接口可以实现多继承。继承Thread类的一样可以实现共享。
先看Thread类和Runnable接口的关系
![](https://img-blog.csdnimg.cn/20190406175645868.png)
![](https://img-blog.csdnimg.cn/20190406175907744.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
可以看到 Runnable接口就一个run方法，然后Thread类实现了这个run方法，同时自己又实现了很多其他方法。
## 1.继承Thread类和重写run()方法
```python
public class MyThread extends Thread{
    private String name;
    private int i = 0;
    public MyThread(String name){
        this.name = name;
    }
    @Override
    public void run() {
        i++;
        System.out.println(name+" i计数为："+i+"    "+Thread.currentThread().getName());
    }
}
```
```python
public class MyThreadTest {
    public static void main(String[] args) {
        MyThread mt1 = new MyThread("线程");
        Thread t1 = new Thread(mt1);
        Thread t2 = new Thread(mt1);
        Thread t3 = new Thread(mt1);
        t1.start();
        t2.start();
        t3.start();
    }
}
```
![](https://img-blog.csdnimg.cn/20190406194521391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
说明：这样就实现了所谓继承的方式不能多个线程处理同一个资源的情况。
你可以自定义线程的name，这样就不会是Thread-0,1,2,3这样的系统生成的name了。
![](https://img-blog.csdnimg.cn/20190406195133271.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
再修改代码：
```python
public class MyThreadTest {
    public static void main(String[] args) {
        MyThread t1 = new MyThread("线程1");
        MyThread t2 = new MyThread("线程2");
        MyThread t3 = new MyThread("线程3");
        t1.start();
        t2.start();
        t3.start();
    }
}
```
![](https://img-blog.csdnimg.cn/20190406195155236.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这次每个线程之间就不会共享数据，也就是那些谬误中的用法。

## 2.实现Runnable接口
```python
public class MyThreadRunnable implements Runnable{
    private String name;
    private int i = 0;
    public MyThreadRunnable(String name){
        this.name = name;
    }
    @Override
    public void run() {
        i++;
        System.out.println(name+" i计数为："+i+"    "+Thread.currentThread().getName());
    }
}
```
```python
public class MyThreadTest {
    public static void main(String[] args) {
        //MyThread t1 = new MyThread("线程1");
        //MyThread t2 = new MyThread("线程2");
        //MyThread t3 = new MyThread("线程3");
        MyThreadRunnable mt=new MyThreadRunnable("线程1");
        Thread t1 = new Thread(mt);
        Thread t2 = new Thread(mt);
        Thread t3 = new Thread(mt);
        t1.start();
        t2.start();
        t3.start();
    }
}
```
![](https://img-blog.csdnimg.cn/20190406200751144.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
实现Runnable接口可以共享变量。
再把代码改一改：
```python
public class MyThreadTest {
    public static void main(String[] args) {
        //MyThread t1 = new MyThread("线程1");
        //MyThread t2 = new MyThread("线程2");
        //MyThread t3 = new MyThread("线程3");
        MyThreadRunnable mt1=new MyThreadRunnable("线程1");
        MyThreadRunnable mt2=new MyThreadRunnable("线程2");
        MyThreadRunnable mt3=new MyThreadRunnable("线程3");
        Thread t1 = new Thread(mt1);
        Thread t2 = new Thread(mt2);
        Thread t3 = new Thread(mt3);
        t1.start();
        t2.start();
        t3.start();
    }
}
```
![](https://img-blog.csdnimg.cn/20190406201314788.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这次就不会共享变量了。
所以这样就证明了共享变量和继承Thread类和实现Runnable并无关系！

## 3.实现线程创建的两种简洁方式（匿名内部类+Lambda表达式）
这种方式适合于创建启动线程次数较少的环境，一般书写更加简便。
```python
//方式1：相当于继承了Thread类，作为子类重写run()实现
        new Thread() {
            public void run() {
                System.out.println("匿名内部类创建线程方式1..." + Thread.currentThread().getName());
            }
        }.start();
        //方式2:实现Runnable,Runnable作为匿名内部类
        new Thread(new Runnable() {
            public void run() {
                System.out.println("匿名内部类创建线程方式2..." + Thread.currentThread().getName());
            }
        }).start();
        //方式3:Lambda表达式创建线程
        new Thread(() -> {
            System.out.println("Lambda表达式创建线程方式..." + Thread.currentThread().getName());
        }).start();
```
![](https://img-blog.csdnimg.cn/20190406202345234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
如果需要自定义线程名字可以修改如下：
```python
//方式3:Lambda表达式创建线程
        new Thread(() -> {
            System.out.println("Lambda表达式创建线程方式..." + Thread.currentThread().getName());
        },"线程1").start();
```
![](https://img-blog.csdnimg.cn/20190406202512510.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

## 4.实现线程的线程池方式
```python
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
public class ThreadPool {
    public static void main(String[] args) {
        ExecutorService fixedThreadPool = Executors.newFixedThreadPool(4);
        for (int i = 0; i < 10; i++) {
            final int index = i;
            fixedThreadPool.execute(new Runnable() {
                public void run() {
                    try {
                        System.out.println(index+ " "+Thread.currentThread().getName()+" "+DateUtil.getNowTimeString());
                        Thread.sleep(2000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            });
        }
        fixedThreadPool.shutdown();
    }
}
```
![](https://img-blog.csdnimg.cn/20190406205051568.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这个帖子写的很好，还有其他几种模式： 《[java常用的几种线程池比较](https://www.cnblogs.com/aaron911/p/6213808.html)》
## newCachedThreadPool
创建一个可缓存线程池，如果线程池长度超过处理需要，可灵活回收空闲线程，若无可回收，则新建线程。
## newFixedThreadPool
创建一个指定工作线程数量的线程池。每当提交一个任务就创建一个工作线程，如果工作线程数量达到线程池初始的最大数，则将提交的任务存入到池队列中。
FixedThreadPool是一个典型且优秀的线程池，它具有线程池提高程序效率和节省创建线程时所耗的开销的优点。但是，在线程池空闲时，即线程池中没有可运行任务时，它不会释放工作线程，还会占用一定的系统资源。
## newSingleThreadExecutor
创建一个单线程化的Executor，即只创建唯一的工作者线程来执行任务，它只会用唯一的工作线程来执行任务，保证所有任务按照指定顺序(FIFO, LIFO, 优先级)执行。如果这个线程异常结束，会有另一个取代它，保证顺序执行。单工作线程最大的特点是可保证顺序地执行各个任务，并且在任意给定的时间不会有多个线程是活动的。
## newScheduleThreadPool
创建一个定长的线程池，而且支持定时的以及周期性的任务执行，支持定时及周期性任务执行。
![](https://img-blog.csdnimg.cn/20190411181954211.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
## 5.实现异步的Future，FutureTask，CompletableFuture方式
本质还是线程，因为Java目前语言层面没有协程，需要三方类库或者修改JVM才可以，
参考本人另一帖：《[异步编程原理以及Java实现](https://linuxstyle.blog.csdn.net/article/details/88172890)》，具体源码分析都有本质还是调用class Executors，只不过是这种方式可以回调而已。原因是继承和实现了Runnable接口这2种方式创建线程都有一个缺陷就是：在执行完任务之后无法获取执行结果。
自从Java 1.5开始，就提供了**Callable和Future**，通过它们可以在任务执行完毕之后**得到任务执行结果**。
虽然 Future 以及相关使用方法提供了异步执行任务的能力，但是对于结果的获取却是很不方便，只能通过阻塞或者轮询的方式得到任务的结果。阻塞的方式显然和我们的异步编程的初衷相违背，轮询的方式又会耗费无谓的 CPU 资源，而且也不能及时地得到计算结果。
在Java8中，**CompletableFuture**提供了非常强大的Future的扩展功能，可以帮助我们简化异步编程的复杂性，并且提供了函数式编程的能力，可以通过**回调的方式处理计算结果**，也提供了转换和组合 CompletableFuture 的方法。
《[CompletableFuture基本用法](https://www.cnblogs.com/cjsblog/p/9267163.html)》
## 6.定时器方式
这种方式不是为了实现线程，但是他确实是起了一个线程
```python
import java.util.Timer;
import java.util.TimerTask;
public class TimerTest {
    public static void main(String[] args) {
        Timer timer = new Timer();
        timer.schedule(new
                               TimerTask() {
                                   @Override
                                   public void run() {
                                       System.out.println("定时任务延迟0(即立刻执行),每隔1000ms执行一次"+ " "+Thread.currentThread().getName()+" "+ DateUtil.getNowTimeString());
                                   }
                               }, 0, 1000);
    }
}
```
![](https://img-blog.csdnimg.cn/20190406210739696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
```python
public abstract class TimerTask implements Runnable
```
![](https://img-blog.csdnimg.cn/20190406211847556.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
从类图可以看出TimerTask实现了Runnable接口的run()方法。
详细分析可以参考《[Java 定时器 Timer 源码分析和使用建议](https://www.jianshu.com/p/e3967ed84c2b)》：
Timer 可以按计划执行重复的任务或者定时执行指定任务，这是因为 Timer 内部利用了一个后台线程 TimerThread 有计划地执行指定任务。
Timer：是一个实用工具类，该类用来调度一个线程(schedule a thread)，使它可以在将来某一时刻执行。 Java 的 Timer 类可以调度一个任务运行一次或定期循环运行。 Timer tasks should complete quickly. 即定时器中的操作要尽可能花费短的时间。
TimerTask：一个抽象类，它实现了 Runnable 接口。我们需要扩展该类以便创建自己的 TimerTask ，这个 TimerTask 可以被 Timer 调度。
一个 Timer 对象对应的是单个后台线程，其内部维护了一个 TaskQueue，用于顺序执行计时器任务 TimeTask 。

## 7.Spring异步任务支持
@EnableAsync和@Async开始异步任务支持
Spring通过任务执行器（TaskExecutor）来实现多线程和并发编程。使用ThreadPoolTaskExecutor可实现一个基于线程池的TaskExecutor.在开发中实现异步任务，我们可以在配置类中添加@EnableAsync开始对异步任务的支持，并在相应的方法中使用@Async注解来声明一个异步任务。
详细参考《[@EnableAsync和@Async开始异步任务支持](https://blog.csdn.net/u014231523/article/details/76272273/)》

## 8.可以用并行流创建线程
```python
import java.util.Arrays;
import java.util.stream.Collectors;
public class StreamParallel {
    public static void main(String args[]) {
        for (int i = 0; i < 1000000; i++) {
            Arrays.asList(1, 2, 3, 4, 5, 6, 7, 9, 8, 0, 1)
                    .stream()
                    .parallel()
                    .collect(Collectors.groupingBy(x -> x % 10))
                    .forEach((x, y) -> System.out.println(x + ":" + y));
        }
    }
}
```
![](https://img-blog.csdnimg.cn/20190411183002533.PNG)
![](https://img-blog.csdnimg.cn/20190411183012314.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)可以看到默认的parallel计算启动了三个线程进行并行。
参考《[Java8 Stream 并行计算实现的原理](https://blog.csdn.net/u013898617/article/details/79146389)》

