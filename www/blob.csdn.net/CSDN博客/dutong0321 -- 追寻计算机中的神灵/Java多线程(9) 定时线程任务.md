# Java多线程(9) 定时线程任务 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年02月25日 17:04:51[dutong0321](https://me.csdn.net/dutong0321)阅读数：47标签：[java																[多线程																[线程池																[预定执行																[ScheduledExecutorService](https://so.csdn.net/so/search/s.do?q=ScheduledExecutorService&t=blog)](https://so.csdn.net/so/search/s.do?q=预定执行&t=blog)](https://so.csdn.net/so/search/s.do?q=线程池&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[编程世界](https://blog.csdn.net/dutong0321/article/category/6127295)
#### 概述
在上一章中写到执行器的后2个方法是预定执行或重复执行而设计的方法，这是一种允许使用线程池机制的java.util.Timer的泛化。 
部分API详解
```
java.util.concurrent.Executors
ScheduledExecutorService newScheduledThreadPool(int threads)
// 返回一个线程池，他使用给定的线程数来调度任务
ScheduledExecutorService newSingleThreadScheduledExecutor()
// 返回一个执行器，他在一个单独线程中调度任务
java.util.concurrent.ScheduledExecutorService
ScheduledFuture<V> schedule(Callable<V> task,long time,TimeUnit unit)
ScheduledFuture<?> schedule(Runnable task,long time,TimeUnit unit)
// 预定在指定的事件后执行任务
ScheduledFuture<?> scheduleAtFixedRate(Runnable task,long initialDelay, long period, TimeUnit unit)
// 预定在初始的延迟结束后，周期性地运行给定的任务，周期长度是period
ScheduledFuture<?> scheduleWithFixedDelay(Runnable task, long initialDelay, long delay,TimeUnit unit)
// 预定在初始的延迟结束后周期性的给定任务，在一次调用完成和下一次调用开始之间有长度为delay的延迟
```
我写了一个每隔100毫秒就输出一次当前时间的范例供参考：
```
package com.utopia.scheduledThreadPool;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;
public class ScheduledThreadTest {
    @SuppressWarnings("unused")
    public static void main(String args[]) {
        ScheduledExecutorService pool = Executors.newSingleThreadScheduledExecutor();
        ScheduledFuture<?> scheduledFuture = pool.scheduleAtFixedRate(new TimeClocker(), 0, 100, TimeUnit.MILLISECONDS);
    }
}
```
```java
package com.utopia.scheduledThreadPool;
import java.text.SimpleDateFormat;
public class TimeClocker implements Runnable {
    @Override
    public void run() {
        // TODO Auto-generated method stub
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
        System.out.println("Time:" + sdf.format(System.currentTimeMillis()) + "." + System.currentTimeMillis() % 1000);
    }
}
```
部分运行截图： 
![运行截图](https://img-blog.csdn.net/20180225170214709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这是程序运行的一小部分截图，有一些小小的误差，很正常。
