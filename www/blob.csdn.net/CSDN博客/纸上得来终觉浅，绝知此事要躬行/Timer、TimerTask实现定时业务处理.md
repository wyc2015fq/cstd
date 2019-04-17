# Timer、TimerTask实现定时业务处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年03月13日 20:58:05[boonya](https://me.csdn.net/boonya)阅读数：2430








**1、编写任务 **

```java
package com.boonya.timer; 
import java.text.SimpleDateFormat; 
import java.util.Date; 
import java.util.TimerTask; 
/** 
* myTask.java 
* 功能:编写定时执行的任务 
* @author boonya 
* @version 1.0 2012-10-11 
*/ 
public class myTask extends TimerTask{ 

  @Override 
  public void run() { 
    // TODO Auto-generated method stub 
    Date mydate=new Date(); 
    String runTime=new SimpleDateFormat("yyy-MM-dd hh:mm:ss").format(mydate); 
    System.out.println(runTime+"定时任务正在执行........."); 
 } 
}
```
**2、设置调用简单任务 **

```java
package com.boonya.timer; 
import java.util.Timer; 
/** 
* RunMyTask.java 
* 功能:调用定时执行的任务 
* @author boonya 
* @version 1.0 2012-10-11 
*/ 
public class RunMyTask { 

 public static void execute(){ 
   Timer timer = new Timer(); 
   //一天执行一次 
   /*Date start=new Date(); 
     start.setDate(start.getDate()+1); 
     start.setHours(2); 
     start.setMinutes(0); 
     start.setSeconds(0); 
     long l = start.getTime()-System.currentTimeMillis(); 
     timer.schedule(new myTask(), l, 24*1000*60*60);*/ 
   //1分钟执行一次的任务 
   timer.schedule(new myTask(), 60000, 60*1000);//task,delay,peroid 
 } 
}
```
**3、编写复杂任务调度策略的范围接口 **

```java
public interface SchedulerService { 
  void schedule(TimerTask task, long delay); 
  void schedule(TimerTask task, long delay, long period); 
  void schedule(TimerTask task, Date firstTime, long period); 
  void scheduleAtFixedRate(TimerTask task, long delay, long period); 
  void scheduleAtFixedRate(TimerTask task, Date firstTime, long period); 
}
```
**4、实现任务策略接口 **


```java
public class SchedulerServiceImpl implements SchedulerService { 
  //延迟多长时间调用一次就陷入线程等待 
  @Override 
  public void schedule(TimerTask task, long delay) { 
    new Timer().schedule(task, delay); 
  } 
  //延迟delay微秒调用一次，以后每隔period微秒执行一次 
  @Override 
  public void schedule(TimerTask task, long delay, long period) { 
    new Timer().schedule(task, delay, period); 
  } 
  //从firstTime开始执行，每隔period微秒执行一次 
  @Override 
  public void schedule(TimerTask task, Date firstTime, long period) { 
    new Timer().scheduleAtFixedRate(task, firstTime, period); 
  } 
  //延迟delay微秒调用一次，以后每隔period微秒执行一次：方法重在执行频率 
  @Override 
  public void scheduleAtFixedRate(TimerTask task, long delay, long period) { 
    new Timer().scheduleAtFixedRate(task, delay, period); 
 } 
 //从firstTime开始执行，每隔period微秒执行一次：方法重在执行频率 
 @Override 
 public void scheduleAtFixedRate(TimerTask task, Date firstTime, long period) { 
   new Timer().scheduleAtFixedRate(task, firstTime, period); 
 } 
}
```
注：建议只使用schedule方法。 
**5、调度业务调用 **

```java
public static void main(String[] args) { 
   SchedulerService service=new SchedulerServiceImpl(); 
   // execute one time then the thread turn to wait status 
      service.schedule(new TemplateTask(), 5*1000); 
   //start from now every 5 seconds execute my task 
   // service.schedule(new TemplateTask(), new Date(), 5*1000); 
   //delay 5 seconds then as 5 seconds time-step to execute my task 
   // service.schedule(new TemplateTask(), 5*1000, 5*1000); 
   // service.scheduleAtFixedRate(new TemplateTask(), new Date(), 5*1000); 
   // service.scheduleAtFixedRate(new TemplateTask(), 5*1000, 5*1000); 
}
```
**6、web配置项目启动执行任务 **

---可以设置一个Servlet在项目启动时启动定时任务，web.xml配置如下： 

```
<servlet> 
<description>This is the description of my J2EE component</description> 
<display-name>This is the display name of my J2EE component</display-name> 
<servlet-name>ReadUserListHandler</servlet-name> 
<servlet-class>com.boonya.test.servlet.ReadUserListHandler</servlet-class> 
<load-on-startup>5</load-on-startup> 
</servlet> 

<servlet-mapping> 
<servlet-name>ReadUserListHandler</servlet-name> 
<url-pattern>/servlet/ReadUserListHandler</url-pattern> 
</servlet-mapping>
```

注： <load-on-startup>说明， 

在servlet的配置当中，<load-on-startup>5</load-on-startup>的含义是： 

标记容器是否在启动的时候就加载这个servlet。 

当值为0或者大于0时，表示容器在应用启动时就加载这个servlet； 

当是一个负数时或者没有指定时，则指示容器在该servlet被选择时才加载。 

正数的值越小，启动该servlet的优先级越高。 
**7、注意事项 **

每一个Timer仅对应唯一一个线程。 

Timer不保证任务执行的十分精确。 

Timer存在线程安全隐患。  




