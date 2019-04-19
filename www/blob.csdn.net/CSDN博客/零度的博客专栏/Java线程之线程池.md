# Java线程之线程池 - 零度的博客专栏 - CSDN博客
2016年06月07日 15:14:54[零度anngle](https://me.csdn.net/zmx729618)阅读数：234
       自JDK5之后，Java推出了一个并发包，[java.util.concurrent](http://write.blog.csdn.net/postedit)，在Java开发中，我们接触到了好多池的技术，String类的对象池、Integer的共享池、连接数据库的连接池等等，池的最终目的都是节约资源，以更小的开销做更多的事情，从而提高性能。
        我们的web项目都是部署在服务器上，浏览器端的每一个request就是一个线程，那么服务器需要并发的处理多个请求，就需要线程池技术，下面来看一下Java并发包下如何创建线程池。
        1.  创建一个可重用固定线程集合的线程池，以共享的无界队列方式来运行这些线程。
**[java]**view
 plaincopyprint?
- ExecutorService threadPool = Executors.newFixedThreadPool(3);// 创建可以容纳3个线程的线程池
        2. 创建一个可根据需要创建新线程的线程池，但是在以前构造的线程可用时将重用它们。
**[java]**view
 plaincopyprint?
- ExecutorService threadPool = Executors.newCachedThreadPool();// 线程池的大小会根据执行的任务数动态分配
        3. 创建一个使用单个 worker 线程的 Executor，以无界队列方式来运行该线程。
**[java]**view
 plaincopyprint?
- ExecutorService threadPool = Executors.newSingleThreadExecutor();// 创建单个线程的线程池，如果当前线程在执行任务时突然中断，则会创建一个新的线程替代它继续执行任务
        4. 创建一个可安排在给定延迟后运行命令或者定期地执行的线程池。
**[java]**view
 plaincopyprint?
- ScheduledExecutorService threadPool = Executors.newScheduledThreadPool(3);// 效果类似于Timer定时器
        每种线程池都有不同的使用场景，下面看一下这四种线程池使用起来有什么不同。
        1. FixedThreadPool
**[java]**view
 plaincopyprint?
- import java.util.concurrent.ExecutorService;  
- import java.util.concurrent.Executors;  
- publicclass ThreadPoolTest {  
- publicstaticvoid main(String[] args) {  
-         ExecutorService threadPool = Executors.newFixedThreadPool(3);  
- for(int i = 1; i < 5; i++) {  
- finalint taskID = i;  
-             threadPool.execute(new Runnable() {  
- publicvoid run() {  
- for(int i = 1; i < 5; i++) {  
- try {  
-                             Thread.sleep(20);// 为了测试出效果，让每次任务执行都需要一定时间
-                         } catch (InterruptedException e) {  
-                             e.printStackTrace();  
-                         }  
-                         System.out.println("第" + taskID + "次任务的第" + i + "次执行");  
-                     }  
-                 }  
-             });  
-         }  
-         threadPool.shutdown();// 任务执行完毕，关闭线程池
-     }  
- }  
        输出结果：
**[java]**view
 plaincopyprint?
- 第1次任务的第1次执行  
- 第2次任务的第1次执行  
- 第3次任务的第1次执行  
- 第2次任务的第2次执行  
- 第3次任务的第2次执行  
- 第1次任务的第2次执行  
- 第3次任务的第3次执行  
- 第1次任务的第3次执行  
- 第2次任务的第3次执行  
- 第3次任务的第4次执行  
- 第2次任务的第4次执行  
- 第1次任务的第4次执行  
- 第4次任务的第1次执行  
- 第4次任务的第2次执行  
- 第4次任务的第3次执行  
- 第4次任务的第4次执行  
        上段代码中，创建了一个固定大小的线程池，容量为3，然后循环执行了4个任务，由输出结果可以看到，前3个任务首先执行完，然后空闲下来的线程去执行第4个任务，在FixedThreadPool中，有一个固定大小的池，如果当前需要执行的任务超过了池大小，那么多于的任务等待状态，直到有空闲下来的线程执行任务，而当执行的任务小于池大小，空闲的线程也不会去销毁。
        2. CachedThreadPool
        上段代码其它地方不变，将newFixedThreadPool方法换成newCachedThreadPool方法。
        输出结果：
**[java]**view
 plaincopyprint?
- 第3次任务的第1次执行  
- 第4次任务的第1次执行  
- 第1次任务的第1次执行  
- 第2次任务的第1次执行  
- 第4次任务的第2次执行  
- 第3次任务的第2次执行  
- 第2次任务的第2次执行  
- 第1次任务的第2次执行  
- 第2次任务的第3次执行  
- 第3次任务的第3次执行  
- 第1次任务的第3次执行  
- 第4次任务的第3次执行  
- 第2次任务的第4次执行  
- 第4次任务的第4次执行  
- 第3次任务的第4次执行  
- 第1次任务的第4次执行  
        可见，4个任务是交替执行的，CachedThreadPool会创建一个缓存区，将初始化的线程缓存起来，如果线程有可用的，就使用之前创建好的线程，如果没有可用的，就新创建线程，终止并且从缓存中移除已有60秒未被使用的线程。
        3. SingleThreadExecutor        
       上段代码其它地方不变，将newFixedThreadPool方法换成newSingleThreadExecutor方法。       
输出结果：
**[java]**view
 plaincopyprint?
- 第1次任务的第1次执行  
- 第1次任务的第2次执行  
- 第1次任务的第3次执行  
- 第1次任务的第4次执行  
- 第2次任务的第1次执行  
- 第2次任务的第2次执行  
- 第2次任务的第3次执行  
- 第2次任务的第4次执行  
- 第3次任务的第1次执行  
- 第3次任务的第2次执行  
- 第3次任务的第3次执行  
- 第3次任务的第4次执行  
- 第4次任务的第1次执行  
- 第4次任务的第2次执行  
- 第4次任务的第3次执行  
- 第4次任务的第4次执行  
        4个任务是顺序执行的，SingleThreadExecutor得到的是一个单个的线程，这个线程会保证你的任务执行完成，如果当前线程意外终止，会创建一个新线程继续执行任务，这和我们直接创建线程不同，也和newFixedThreadPool(1)不同。
    4.ScheduledThreadPool
**[java]**view
 plaincopyprint?
- import java.util.concurrent.ScheduledExecutorService;  
- import java.util.concurrent.TimeUnit;  
- publicclass ThreadPoolTest {  
- publicstaticvoid main(String[] args) {  
-         ScheduledExecutorService schedulePool = Executors.newScheduledThreadPool(1);  
- // 5秒后执行任务
-         schedulePool.schedule(new Runnable() {  
- publicvoid run() {  
-                 System.out.println("爆炸");  
-             }  
-         }, 5, TimeUnit.SECONDS);  
- // 5秒后执行任务，以后每2秒执行一次
-         schedulePool.scheduleAtFixedRate(new Runnable() {  
- @Override
- publicvoid run() {  
-                 System.out.println("爆炸");  
-             }  
-         }, 5, 2, TimeUnit.SECONDS);  
-     }  
- }  
ScheduledThreadPool可以定时的或延时的执行任务。
