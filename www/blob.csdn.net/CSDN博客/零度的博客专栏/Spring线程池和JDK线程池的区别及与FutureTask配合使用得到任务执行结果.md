# Spring线程池和JDK线程池的区别及与FutureTask配合使用得到任务执行结果 - 零度的博客专栏 - CSDN博客
2017年04月07日 15:54:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：3331
[API  文档](http://docs.spring.io/spring/docs/current/javadoc-api/org/springframework/scheduling/concurrent/ThreadPoolTaskExecutor.html)中很清楚，SpringFrameWork 的 ThreadPoolTaskExecutor 是辅助 JDK 的 ThreadPoolExecutor  的工具类，它将属性通过 JavaBeans 的命名规则提供出来，方便进行配置。
### 1.JDK之ThreadPoolExecutor的使用
 Spring中的ThreadPoolTaskExecutor是借助于JDK并发包中的java.util.concurrent.ThreadPoolExecutor来实现的.下面先学习下ThreadPoolExecutor中的相关信息.ThreadPoolExecutor构造函数如下: 
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- public ThreadPoolExecutor(int corePoolSize,  
- int maximumPoolSize,  
- long keepAliveTime,  
-                           TimeUnit unit,  
-                           BlockingQueue<Runnable> workQueue,  
-                           ThreadFactory threadFactory,  
-                           RejectedExecutionHandler handler) {  
![](http://static.blog.csdn.net/images/save_snippets.png)
下面分别说下各项代表的具体意义: 
int corePoolSize:线程池维护线程的最小数量. 
int maximumPoolSize:线程池维护线程的最大数量. 
long keepAliveTime:空闲线程的存活时间. 
TimeUnit unit: 时间单位,现有纳秒,微秒,毫秒,秒枚举值. 
BlockingQueue<Runnable> workQueue:持有等待执行的任务队列. 
RejectedExecutionHandler handler: 用来拒绝一个任务的执行，有两种情况会发生这种情况。 
一是：在execute方法中若addIfUnderMaximumPoolSize(command)为false，即线程池已经饱和； 
二是：在execute方法中, 发现runState!=RUNNING || poolSize == 0,即已经shutdown,就调用ensureQueuedTaskHandled(Runnable command)，在该方法中有可能调用reject。 
Reject策略预定义有四种： 
(1)ThreadPoolExecutor.AbortPolicy策略，是默认的策略,处理程序遭到拒绝将抛出运行时 RejectedExecutionException。 
(2)ThreadPoolExecutor.CallerRunsPolicy策略 ,调用者的线程会执行该任务,如果执行器已关闭,则丢弃. 
(3)ThreadPoolExecutor.DiscardPolicy策略，不能执行的任务将被丢弃. 
(4)ThreadPoolExecutor.DiscardOldestPolicy策略，如果执行程序尚未关闭，则位于工作队列头部的任务将被删除，然后重试执行程序（如果再次失败，则重复此过程）. 
### 2. Spring中ThreadPoolTaskExecutor的使用
 最常用方式就是做为BEAN注入到容器中,如下代码: 
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- <bean id="threadPoolTaskExecutor"
- class="org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor">  
-     <property name="corePoolSize" value="10" />  
-     <property name="maxPoolSize" value="15" />  
-     <property name="queueCapacity" value="1000" />  
- </bean>  
![](http://static.blog.csdn.net/images/save_snippets.png)
ThreadPoolExecutor执行器的处理流程: 
(1)当线程池大小小于corePoolSize就新建线程，并处理请求. 
(2)当线程池大小等于corePoolSize，把请求放入workQueue中，池子里的空闲线程就去从workQueue中取任务并处理. 
(3)当workQueue放不下新入的任务时，新建线程加入线程池，并处理请求，如果池子大小撑到了maximumPoolSize就用RejectedExecutionHandler来做拒绝处理. 
(4)另外，当线程池的线程数大于corePoolSize的时候，多余的线程会等待keepAliveTime长的时间，如果无请求可处理就自行销毁. 
了解清楚了ThreadPoolExecutor的执行流程,开头提到的org.springframework.core.task.TaskRejectedException异常也就好理解了,ThreadPoolTaskExecutor类中使用的 就是ThreadPoolExecutor.AbortPolicy()策略,直接抛出异常。
### 3.spring线程池与FutureTask配合使用获取任务执行状态
用ThreadPoolExecutor的时候，又想知道被执行的任务的执行情况，这时就可以用FutureTask。
 ThreadPoolTask：
```
package zmx.spring.threadpool.test;
import java.io.Serializable; 
import java.util.concurrent.Callable; 
   
public class ThreadPoolTask implements Callable<String>, Serializable { 
     private static final long serialVersionUID = 0; 
     
     // 保存任务所需要的数据 
     private Object threadPoolTaskData; 
     private static int consumeTaskSleepTime = 2000; 
     public ThreadPoolTask(Object tasks) { 
         this.threadPoolTaskData = tasks; 
     }
     
	 @Override
     public String call() throws Exception { 
        // 处理一个任务，这里的处理方式太简单了，仅仅是一个打印语句 
        System.out.println("开始执行任务：" + threadPoolTaskData); 
        String result = ""; 
        //便于观察，等待一段时间 
        try {  
            //Thread.sleep((long)(Math.random()*consumeTaskSleepTime));
            for (long i = 0; i < 10000000; i++) { 
            } 
            result = "OK"; 
        } catch (Exception e) { 
            e.printStackTrace(); 
            result = "ERROR"; 
        }  
        return result; 
    }
 
}
```
模拟客户端提交的线程：
```
package zmx.spring.threadpool.test;
import java.util.concurrent.ExecutionException; 
import java.util.concurrent.FutureTask; 
import java.util.concurrent.TimeUnit; 
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor; 
public class StartTaskThread implements Runnable { 
     private ThreadPoolTaskExecutor threadPoolTaskExecutor; 
     private int i; 
     public StartTaskThread(ThreadPoolTaskExecutor threadPoolTaskExecutor, int i) { 
         this.threadPoolTaskExecutor = threadPoolTaskExecutor; 
         this.i = i; 
     } 
     @Override 
     public void run() { 
         String task = "task@ " + i; 
         System.out.println("创建任务并提交到线程池中：" + task); 
         FutureTask<String> futureTask = new FutureTask<String>(new ThreadPoolTask(task)); 
         threadPoolTaskExecutor.execute(futureTask); 
         // 在这里可以做别的任何事情 
         String result = null; 
         try { 
             // 取得结果，同时设置超时执行时间为1秒。同样可以用future.get()，不设置执行超时时间取得结果 
             // result = futureTask.get(1000, TimeUnit.MILLISECONDS);
             while(true){
                 if(futureTask.isDone()){
                	 result = futureTask.get();
                	 break;
                 } 
             }
         } catch (InterruptedException e) { 
             futureTask.cancel(true); 
         } catch (ExecutionException e) { 
             futureTask.cancel(true); 
         } catch (Exception e) { 
             futureTask.cancel(true); 
             //超时后，进行相应处理 
         } finally { 
             System.out.println("task@" + i + ":result=" + result); 
         } 
     } 
}
```
[spring](http://lib.csdn.net/base/javaee)配置文件：
```
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:jee="http://www.springframework.org/schema/jee" 
       xmlns:tx="http://www.springframework.org/schema/tx"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd   
       http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd   
       http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd   
       http://www.springframework.org/schema/jee http://www.springframework.org/schema/jee/spring-jee-3.0.xsd   
       http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd">
     
 
    
   <bean id="threadPoolTaskExecutor" class="org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor"> 
         <!-- 核心线程数，默认为1 --> 
         <property name="corePoolSize" value="10" /> 
         <!-- 最大线程数，默认为Integer.MAX_VALUE --> 
         <property name="maxPoolSize" value="50" /> 
         <!-- 队列最大长度，一般需要设置值: 大于等于notifyScheduledMainExecutor.maxNum；默认为Integer.MAX_VALUE -->
         <property name="queueCapacity" value="50" /> 
 
         <!-- 线程池维护线程所允许的空闲时间，默认为60s --> 
         <property name="keepAliveSeconds" value="300" /> 
         <!-- 线程池对拒绝任务（无线程可用）的处理策略，目前只支持AbortPolicy、CallerRunsPolicy；默认为后者 --> 
         <property name="rejectedExecutionHandler"> 
             <!-- AbortPolicy:直接抛出java.util.concurrent.RejectedExecutionException异常 --> 
             <!-- CallerRunsPolicy:主线程直接执行该任务，执行完之后尝试添加下一个任务到线程池中，可以有效降低向线程池内添加任务的速度 --> 
             <!-- DiscardOldestPolicy:抛弃旧的任务、暂不支持；会导致被丢弃的任务无法再次被执行 --> 
             <!-- DiscardPolicy:抛弃当前任务、暂不支持；会导致被丢弃的任务无法再次被执行 --> 
             <bean class="java.util.concurrent.ThreadPoolExecutor$CallerRunsPolicy" /> 
         </property> 
     </bean> 
    
    
</beans>
```
[测试](http://lib.csdn.net/base/softwaretest)类：
```
package zmx.spring.threadpool.test;
import org.junit.Test; 
import org.junit.runner.RunWith; 
import org.springframework.beans.factory.annotation.Autowired; 
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor; 
import org.springframework.test.context.ContextConfiguration;  
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner; 
@RunWith(SpringJUnit4ClassRunner.class) 
// 指定的运行runner，并且把你所指定的Runner作为参数传递给它 
@ContextConfiguration(locations = "classpath:zmx/spring/threadpool/test/applicationContext.xml") 
public class TestThreadPool{
	
     private static int produceTaskSleepTime = 10; 
     private static int produceTaskMaxNumber = 100; 
     @Autowired 
     private ThreadPoolTaskExecutor threadPoolTaskExecutor; 
     
     public ThreadPoolTaskExecutor getThreadPoolTaskExecutor() { 
         return threadPoolTaskExecutor; 
     } 
     public void setThreadPoolTaskExecutor(ThreadPoolTaskExecutor threadPoolTaskExecutor) { 
         this.threadPoolTaskExecutor = threadPoolTaskExecutor; 
     } 
     @Test 
     public void testThreadPoolExecutor() { 
         for (int i = 1; i <= produceTaskMaxNumber; i++) { 
             try { 
                 Thread.sleep(produceTaskSleepTime); 
             } catch (InterruptedException e1) { 
                 e1.printStackTrace(); 
             } 
             new Thread(new StartTaskThread(threadPoolTaskExecutor, i)).start(); 
             
             
         } 
         
         threadPoolTaskExecutor.shutdown();
     } 
 }
```
运行结果：
```
创建任务并提交到线程池中：task@ 1
开始执行任务：task@ 1
创建任务并提交到线程池中：task@ 2
task@1:result=OK
开始执行任务：task@ 2
task@2:result=OK
创建任务并提交到线程池中：task@ 3
开始执行任务：task@ 3
task@3:result=OK
创建任务并提交到线程池中：task@ 4
开始执行任务：task@ 4
task@4:result=OK
创建任务并提交到线程池中：task@ 5
开始执行任务：task@ 5
task@5:result=OK
创建任务并提交到线程池中：task@ 6
开始执行任务：task@ 6
task@6:result=OK
创建任务并提交到线程池中：task@ 7
开始执行任务：task@ 7
```

