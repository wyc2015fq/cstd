# java.util.concurrent / Executor 相关接口、类 关系整理（一）未完待续... - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月24日 22:24:37[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：62标签：[Executor 线程池](https://so.csdn.net/so/search/s.do?q=Executor 线程池&t=blog)
个人分类：[Executor 线程池](https://blog.csdn.net/weixin_40247263/article/category/7535558)



![](http://on-img.com/chart_image/5ab64dc2e4b0a248b0e13e99.png)

ExecutorService接口方法

1、execut(Runnable)  无返回值

2、submit(Runnable) 无返回值，但会返回一个Future对象可以用来检查是否执行完毕

3、invokeAny（一系列的Callable接口或其子接口的实例对象） 随机返回其中一个Callable对象的结果，只能表明其中一个已执行结束。

4、invokeAll（一系列的Callable接口或其子接口的实例对象）  返回一个Future 可以获得所有的callable执行结果。

5、shutDown() 结束空闲线程，shutDown之前提交的任务可以继续执行

6、shutDownNow() 立即结束大部分线程，这里不是所有，能力不够

ScheduledExecutorService接口方法

1、schedule(Runnable task,long delay,TimeUnit timeunit) 

参数1：执行谁。  参数2：过多久执行。   参数3：时间单位。

2、schedule(Callable task，long delay,TimeUnit timeunit)

跟 上一个的区别： 一个Runnable 不带返回值，一个Runnable 带返回值

3、scheduleAtFixedRate(Runnable,long initialDelay,long period,TimeUnit timeunit)

参数1：执行谁。  参数2：过多久执行。   参数3：过多久执行一次。  参数4：时间单位。

period：前一个执行的开始和下一个执行开始的间隔时间

注意：如果一个线程没有在指定的时间间隔内执行完，那么不会执行下一个线程。执行完了，再执行下一个线程，计划任务在同一时间，不会有多个线程同时执行。

4、scheduleWithAtFixedRate（Runnable,long initialDelay,long period,TimeUnit timeunit）

跟上一个的区别： period 的定义不同   ：前一个执行的结束和下一个执行结束的间隔时间

shutDown和shutDownNow 同上。


ThreadPoolExecutor线程池执行者

就是用 线程池内部的线程 来执行任务（Callable或 Runnable）

构造方法参数解释：

1、corePoolSize ： 池中线程数

2、maximumPoolSize：池中允许最大线程数

3、keepAliveTime ：当前线程数>corePoolSize 。空闲线程等待任务的最长时间

4、unit ： 时间单位

5、workQueue ：执行前任务队列。

6、threadFactory ：可以实现 线程工场接口，对线程属性进行配置

7、RejectedExecutionHandler： 线程池中线程全部被使用，会执行的方法

