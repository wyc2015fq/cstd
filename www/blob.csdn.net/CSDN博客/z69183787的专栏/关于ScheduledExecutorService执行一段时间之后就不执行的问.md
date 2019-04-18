# 关于ScheduledExecutorService执行一段时间之后就不执行的问 - z69183787的专栏 - CSDN博客
2015年09月24日 09:50:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5712
最近写了个程序，程序中使用到了ScheduledExecutorService的scheduleAtFixedRate方法，用于定时执行任务，但是发现程序运行一段时间之后定时任务不执行了，查看日志和perfcounter都没有看到任何异常，比较郁闷。最后看了一下JDK的源码，在源码的Java
 doc中的发现了如下一句话：
If any execution of the task encounters an exception, subsequent executions are suppressed.Otherwise, the task will only terminate via cancellation or termination of the executor.
简单总结就是：如果定时任务执行过程中遇到发生异常，则后面的任务将不再执行。
我们可以做个实验验证一下：
先看下面一段代码：
/** *  [Copyright] *  @author  QiFuguang  *  Aug 25, 2014 9:48:41 PM	 */package com.winwill.xx;import java.util.concurrent.Executors;import java.util.concurrent.ScheduledExecutorService;import java.util.concurrent.TimeUnit;publicclassTest{privatefinalstaticScheduledExecutorService scheduler =Executors.newSingleThreadScheduledExecutor();publicstaticvoid main(String[] args){        scheduler.scheduleAtFixedRate(newRunnable(){@Overridepublicvoid run(){int[] s =newint[1];System.out.println("OK");System.out.println(s[1]);// 数组越界}},0,2,TimeUnit.SECONDS);}}
这段代码的执行效果如下:
![关于ScheduledExecutorService执行一段时间之后不在执行的问题 - 黯然神伤 - WinWill2014](http://img0.ph.126.net/g1VP_LQODQyX3m4poHrWVQ==/200973133472084220.png)
 可以看到定时任务只执行了一次，因为第一次执行就遇到了数组越界异常，后面的任务被取消了。
再看如下代码：
/** *  [Copyright] *  @author  QiFuguang  *  Aug 25, 2014 9:48:41 PM	 */package com.winwill.xx;import java.util.concurrent.Executors;import java.util.concurrent.ScheduledExecutorService;import java.util.concurrent.TimeUnit;publicclassTest{privatefinalstaticScheduledExecutorService scheduler =Executors.newSingleThreadScheduledExecutor();publicstaticvoid main(String[] args){        scheduler.scheduleAtFixedRate(newRunnable(){@Overridepublicvoid run(){try{int[] s =newint[1];System.out.println("OK");System.out.println(s[1]);// 数组越界}catch(Throwable t){System.out.println("Error");}}},0,2,TimeUnit.SECONDS);}}
运行效果如下：
![关于ScheduledExecutorService执行一段时间之后不在执行的问题 - 黯然神伤 - WinWill2014](http://img0.ph.126.net/cXO35if4kjszSQlTn-NYjg==/6598211059728073983.png)
加了try/catch之后可以看到定时任务没有被取消。
