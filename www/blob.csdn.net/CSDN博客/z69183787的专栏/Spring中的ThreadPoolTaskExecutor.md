# Spring中的ThreadPoolTaskExecutor - z69183787的专栏 - CSDN博客
2016年05月11日 11:56:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1730
在观察线上系统的运行情况下,发现在错误日志中有这类错误信息,org.springframework.core.task.TaskRejectedException,于是便对ThreadPoolTaskExecutor类做下梳理. 
1.ThreadPoolExecutor
Spring中的ThreadPoolTaskExecutor是借助于JDK并发包中的java.util.concurrent.ThreadPoolExecutor来实现的.下面先学习下ThreadPoolExecutor中的相关信息.ThreadPoolExecutor构造函数如下: 
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- public ThreadPoolExecutor(int corePoolSize,  
- int maximumPoolSize,  
- long keepAliveTime,  
-                           TimeUnit unit,  
-                           BlockingQueue<Runnable> workQueue,  
-                           ThreadFactory threadFactory,  
-                           RejectedExecutionHandler handler) {  
下面分别说下各项代表的具体意义: 
int corePoolSize:线程池维护线程的最小数量. 
int maximumPoolSize:线程池维护线程的最大数量. 
long keepAliveTime:空闲线程的存活时间. 
TimeUnit unit: 时间单位,现有纳秒,微秒,毫秒,秒枚举值. 
BlockingQueue<Runnable> workQueue:持有等待执行的任务队列. 
RejectedExecutionHandler handler: 
用来拒绝一个任务的执行，有两种情况会发生这种情况。 
一是在execute方法中若addIfUnderMaximumPoolSize(command)为false，即线程池已经饱和； 
二是在execute方法中, 发现runState!=RUNNING || poolSize == 0,即已经shutdown,就调用ensureQueuedTaskHandled(Runnable command)，在该方法中有可能调用reject。 
Reject策略预定义有四种： 
(1)ThreadPoolExecutor.AbortPolicy策略，是默认的策略,处理程序遭到拒绝将抛出运行时 RejectedExecutionException。 
(2)ThreadPoolExecutor.CallerRunsPolicy策略 ,调用者的线程会执行该任务,如果执行器已关闭,则丢弃. 
(3)ThreadPoolExecutor.DiscardPolicy策略，不能执行的任务将被丢弃. 
(4)ThreadPoolExecutor.DiscardOldestPolicy策略，如果执行程序尚未关闭，则位于工作队列头部的任务将被删除，然后重试执行程序（如果再次失败，则重复此过程）. 
2. Spring中ThreadPoolTaskExecutor的使用
最常用方式就是做为BEAN注入到容器中,如下代码: 
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- <bean id="threadPoolTaskExecutor"
- class="org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor">  
-     <property name="corePoolSize" value="10" />  
-     <property name="maxPoolSize" value="15" />  
-     <property name="queueCapacity" value="1000" />  
- </bean>  
ThreadPoolExecutor执行器的处理流程: 
(1)当线程池大小小于corePoolSize就新建线程，并处理请求. 
(2)当线程池大小等于corePoolSize，把请求放入workQueue中，池子里的空闲线程就去从workQueue中取任务并处理. 
(3)当workQueue放不下新入的任务时，新建线程加入线程池，并处理请求，如果池子大小撑到了maximumPoolSize就用RejectedExecutionHandler来做拒绝处理. 
(4)另外，当线程池的线程数大于corePoolSize的时候，多余的线程会等待keepAliveTime长的时间，如果无请求可处理就自行销毁. 
了解清楚了ThreadPoolExecutor的执行流程,开头提到的org.springframework.core.task.TaskRejectedException异常也就好理解和解决了.ThreadPoolTaskExecutor类中使用的 
就是ThreadPoolExecutor.AbortPolicy()策略,直接抛出异常. 
