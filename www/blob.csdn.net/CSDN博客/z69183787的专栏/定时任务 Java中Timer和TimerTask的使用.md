# 定时任务:Java中Timer和TimerTask的使用 - z69183787的专栏 - CSDN博客
2012年11月19日 09:50:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：846
java.util.Timer定时器，实际上是个线程，定时调度所拥有的TimerTasks。 
一个TimerTask实际上就是一个拥有run方法的类，需要定时执行的代码放到run方法体内，TimerTask一般是以匿名类的方式创建。 
在java中，Timer类主要用于**定时性、周期性任务**的触发，主要有2个常用方法，那就是schedule和scheduleAtFixedRate方法，
schedule和scheduleAtFixedRate的区别可参考 [http://blog.csdn.net/z69183787/article/details/8198141](http://blog.csdn.net/z69183787/article/details/8198141)
一个完整的Timer： 
Java代码  ![收藏代码](http://batitan.iteye.com/images/icon_star.png)
- java.util.Timer timer = new java.util.Timer(true);   
- // true 说明这个timer以daemon方式运行（优先级低， 
- // 程序结束timer也自动结束），注意，javax.swing 
- // 包中也有一个Timer类，如果import中用到swing包， 
- // 要注意名字的冲突。 
- 
- TimerTask task = new TimerTask() {   
- publicvoid run() {   
- ... //每次需要执行的代码放到这里面。 
- }   
- };   
- 
- //以下是几种调度task的方法： 
- 
- timer.schedule(task, time);   
- // time为Date类型：在指定时间执行一次。 
- 
- timer.schedule(task, firstTime, period);   
- // firstTime为Date类型,period为long 
- // 从firstTime时刻开始，每隔period毫秒执行一次。 
- 
- timer.schedule(task, delay)   
- // delay 为long类型：从现在起过delay毫秒执行一次 
- 
- timer.schedule(task, delay, period)   
- // delay为long,period为long：从现在起过delay毫秒以后，每隔period 
- // 毫秒执行一次。
下面是一个完整的例子,由两个类组成,一个定制任务,一个调用java.util.Timer 
定制任务: 
Java代码  ![收藏代码](http://batitan.iteye.com/images/icon_star.png)
- import java.util.Timer;  
- 
- publicclass TimerTaskTest extends java.util.TimerTask{  
- 
- @Override
- publicvoid run() {  
- // TODO Auto-generated method stub
-    System.out.println("start");  
- }  
- }  
2.调用java.util.Timer 
Java代码  ![收藏代码](http://batitan.iteye.com/images/icon_star.png)
- import java.util.Timer;  
- 
- publicclass Test {  
- publicstaticvoid main(String[] args){  
-    Timer timer = new Timer();  
-    timer.schedule(new TimerTaskTest(), 1000, 2000);  
- }  
- }  
根据上面的介绍,便可以在1秒后,每隔2秒执行一次程序 
