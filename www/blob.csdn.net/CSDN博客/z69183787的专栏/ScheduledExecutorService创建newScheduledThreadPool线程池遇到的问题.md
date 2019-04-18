# ScheduledExecutorService创建newScheduledThreadPool线程池遇到的问题 - z69183787的专栏 - CSDN博客
2014年12月29日 21:33:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10544
最近线程池老是遇到执行两次的情况，故作了分析，情况如下
```java
package test;
import java.util.Date;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;
public class Test {
	static class EchoServer1 implements Runnable {
		@Override
		public void run() {
			// TODO Auto-generated method stub
			try {
				Thread.sleep(10000);
				i++;
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if(i==3){
				throw new java.lang.RuntimeException("Dasds");
			}
			System.out.println(i+"-"+10+"-"+Thread.currentThread().getName() +"-"+new Date());
		}
		
	}
	static int i =0;
	static class EchoServer2 implements Runnable {
		@Override
		public void run() {
			// TODO Auto-generated method stub
			
			System.out.println(20+"-"+Thread.currentThread().getName() +"-"+new Date());
		}
		
	}
	
	public static void main(String[] args){
		ScheduledExecutorService executor = Executors.newScheduledThreadPool(2);  
		executor.scheduleAtFixedRate(  
	            new EchoServer2(),  
	            0,  
	            10,  
	            TimeUnit.SECONDS);  
		ScheduledFuture<?> future = executor.scheduleAtFixedRate(  
		            new EchoServer1(),  
		            0,  
		            5,  
		            TimeUnit.SECONDS);  
		try {
			     try {
					future.get();
					
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			 } catch (ExecutionException e) {
			     Throwable cause = e.getCause();
			     cause.printStackTrace();
			     future.cancel(false);
			 }
	}
}
```
1、可用scheduledFuture 捕获 task异常，并停止某个task，不影响其余task的运行。
2、若task运行时间大于间隔时间，则下一次task将在上一次task后马上运行，不会产生同时2个相同task的运行情况发生。
3、若线程池大小小于task大小，则若在情况2下，有另一条task占据了现有线程池（即阻塞了原task的一次运行），
则会造成 原task在另一条task结束后，执行两次。
线程池为1：
20-pool-1-thread-1-Mon Dec 29 21:34:08 CST 2014
1-10-pool-1-thread-1-Mon Dec 29 21:34:18 CST 2014
2-10-pool-1-thread-1-Mon Dec 29 21:34:28 CST 2014
20-pool-1-thread-1-Mon Dec 29 21:34:28 CST 2014
20-pool-1-thread-1-Mon Dec 29 21:34:38 CST 2014
20-pool-1-thread-1-Mon Dec 29 21:34:38 CST 2014
java.lang.RuntimeException: Dasds
at test.Test$EchoServer1.run(Test.java:23)
at java.util.concurrent.Executors$RunnableAdapter.call(Unknown Source)
at java.util.concurrent.FutureTask$Sync.innerRunAndReset(Unknown Source)
at java.util.concurrent.FutureTask.runAndReset(Unknown Source)
at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.access$101(Unknown Source)
at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.runPeriodic(Unknown Source)
at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.run(Unknown Source)
at java.util.concurrent.ThreadPoolExecutor$Worker.runTask(Unknown Source)
at java.util.concurrent.ThreadPoolExecutor$Worker.run(Unknown Source)
at java.lang.Thread.run(Unknown Source)
20-pool-1-thread-1-Mon Dec 29 21:34:48 CST 2014
线程池为2：
20-pool-1-thread-1-Mon Dec 29 21:33:13 CST 2014
20-pool-1-thread-1-Mon Dec 29 21:33:23 CST 2014
1-10-pool-1-thread-2-Mon Dec 29 21:33:23 CST 2014
20-pool-1-thread-1-Mon Dec 29 21:33:33 CST 2014
2-10-pool-1-thread-2-Mon Dec 29 21:33:33 CST 2014
20-pool-1-thread-1-Mon Dec 29 21:33:43 CST 2014
java.lang.RuntimeException: Dasds
at test.Test$EchoServer1.run(Test.java:23)
at java.util.concurrent.Executors$RunnableAdapter.call(Unknown Source)
at java.util.concurrent.FutureTask$Sync.innerRunAndReset(Unknown Source)
at java.util.concurrent.FutureTask.runAndReset(Unknown Source)
at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.access$101(Unknown Source)
at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.runPeriodic(Unknown Source)
at java.util.concurrent.ScheduledThreadPoolExecutor$ScheduledFutureTask.run(Unknown Source)
at java.util.concurrent.ThreadPoolExecutor$Worker.runTask(Unknown Source)
at java.util.concurrent.ThreadPoolExecutor$Worker.run(Unknown Source)
at java.lang.Thread.run(Unknown Source)
20-pool-1-thread-2-Mon Dec 29 21:33:53 CST 2014
