# ScheduledExecutorService执行周期任务 - z69183787的专栏 - CSDN博客
2012年11月19日 10:30:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1386
个人分类：[Java-知识																[定时任务调度](https://blog.csdn.net/z69183787/article/category/2175967)](https://blog.csdn.net/z69183787/article/category/2175279)
鉴于 Timer 的上述缺陷，Java 5 推出了基于线程池设计的 ScheduledExecutor。其设计思想是，每一个被调度的任务都会由线程池中一个线程去执行，因此任务是并发执行的，相互之间不会受到干扰。需要注意的是，只有当任务的执行时间到来时，ScheduedExecutor 才会真正启动一个线程，其余时间
 ScheduledExecutor 都是在轮询任务的状态
项目中用到了cheduledExecutorService中的一个方法，就顺便把其中几个关重要的方法学习下：
**1.schedule**
     schedule(Runnable command, long delay, TimeUnit unit)，schedule方法被用来延迟指定时间后执行某个指定任务。
a.代码如下：
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- publicclass Job implements Runnable {  
-     SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");  
- 
- publicvoid run() {  
- try {  
-             Thread.sleep(5000);  
-         } catch (InterruptedException ex) {  
-             ex.printStackTrace();  
-         }  
-         System.out.println("do something  at:" + sdf.format(new Date()));  
-     }  
- }  
- 
- publicclass ScheduledExecutorServiceTest {  
- 
- publicstaticvoid main(String[] args) {  
-         ScheduledExecutorService schedule = Executors.newScheduledThreadPool(5);  
- final SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");  
-         System.out.println(" begin to do something at:" + sdf.format(new Date()));  
-         schedule.schedule(new Job(),1, TimeUnit.SECONDS);  
-     }  
- }  
b.输出如下：
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- begin to do something at:2012-08-0309:31:36
- do something  at:2012-08-0309:31:42
**2.scheduleWithFixedDelay**
         scheduleWithFixedDelay(Runnable command, long initialDelay, long delay,TimeUnit unit)
         创建并执行一个在给定初始延迟后首次启用的定期操作，随后，在每一次执行终止和下一次执行开始之间都存在给定的延迟，如果任务的执行时间超过了廷迟时间（delay），下一个任务则会在
（当前任务执行所需时间+delay）后执行。
    a.代码如下：
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- publicclass ScheduledExecutorServiceTest {  
- publicstaticvoid main(String[] args) {  
-             ScheduledExecutorService schedule = Executors.newScheduledThreadPool(5);  
- final SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");  
-             System.out.println(" begin to do something at:" + sdf.format(new Date()));  
-             schedule.scheduleWithFixedDelay(new Job(), 1, 2, TimeUnit.SECONDS);  
-         }  
-     }  
    b.输出如下：
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- begin to do something at:2012-08-0309:36:53
- do something at:2012-08-0309:36:59
- do something at:2012-08-0309:37:06
- do something at:2012-08-0309:37:13
**3.scheduleAtFixedRate**
         scheduleAtFixedRate(Runnable command, long initialDelay, long period, TimeUnitunit)
         创建并执行一个在给定初始延迟后首次启用的定期操作，后续操作具有给定的周期；也就是将在 initialDelay 后开始执行，然后在initialDelay+period 后执行，接着在 initialDelay + 2 * period 后执行，依此类推。
如果任务的执行时间小于period，将会按上述规律执行。否则，则会按 任务的实际执行时间进行周期执行。
    a.代码如下：
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- publicclass ScheduledExecutorServiceTest {  
- 
- publicstaticvoid main(String[] args) {  
-         ScheduledExecutorService schedule = Executors.newScheduledThreadPool(2);  
- final SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");  
-         System.out.println(" begin to do something at:" + sdf.format(new Date()));  
-         schedule.scheduleAtFixedRate(new Job(), 1,2, TimeUnit.SECONDS);  
-     }  
    b.结果输出：
Java代码  ![收藏代码](http://kim-miao.iteye.com/images/icon_star.png)
- begin to do something at:2012-08-0408:53:30
- do something at:2012-08-0408:53:36
- do something at:2012-08-0408:53:41
- do something at:2012-08-0408:53:46
- do something at:2012-08-0408:53:51
