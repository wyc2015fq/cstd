# Servlet监听器与Timer定时器配合实现JAVA WEB应用简单自动作业 - 零度的博客专栏 - CSDN博客
2016年05月13日 16:37:25[零度anngle](https://me.csdn.net/zmx729618)阅读数：5015
       在web应用中,有时候客户需要一些定时程序，不需要客户自己去操作，而是由应用程序自行触发执行某些操作。这个时候监听与定时器的配合使用就基本可以实现这个需求了。
** 1.创建一个监听的SERVELET,这个类继承javax.servlet.http.HttpServlet并且实现javax.servlet.ServletContextListener。**
```
package com.bndz.outMail;
import java.util.Timer;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import javax.servlet.http.HttpServlet;
/**
  * 
  * 文件发送的监听 需要再WEB-XML中配置
 */
public class MailSendListener extends HttpServlet implements
   ServletContextListener {
 /**
   * 监听开始销毁
  */
  public void contextDestroyed(ServletContextEvent sce) {
 }
 /**
   * 监听开始执行
  */
  public void contextInitialized(ServletContextEvent sce) {
  // 当监听开始执行时,设置一个TIME
   Timer timer = new Timer();
   System.out.println("-------MailSend   Timer开始进行执行--------------");
   MailSendTask msendTask = new MailSendTask();
   timer.schedule(msendTask, 60 * 1000, 10 * 1000);
   System.out.println("-------MailSend   Timer已经在执行--------------");
  }
 }
```
**2.配置WEB.XML文件，用于进行配置。**
```
<listener>
     <listener-class>com.bndz.outMail.MailSendListener</listener-class>
   </listener>
```
**3.创建一个MailSendTask，继承java.util.TimerTask，在RUN方法里面进行程序的执行。**
```
package com.bndz.outMail;
import java.util.Date;
import java.util.TimerTask;
/**
  * JAVA 一个定时任务
 * 
  * @author Guox2011-05-10
  * 
  */
 public class MailSendTask extends TimerTask {
 public MailSendTask() {
 }
 /**
   * RUN方法放入一个SCHEDULE的时候 进行执行 一个定时程序执行的进口
  */
  public void run() {
  System.out.println(new Date()+ "---MailSendTask begin to RUN ------------");
   MailSendManager mainSendManager = new MailSendManager();
   mainSendManager.waitingMailRemind();
  }
}
```
**4.在监听SERVELET的的contextInitialized方法中，来写一个TIMER；让其SCHEDULE方法执行上面所描写的MailSendTask任务。**
```
// 当监听开始执行时,设置一个TIME
  Timer timer = new Timer();
   System.out.println("-------MailSend   Timer开始进行执行--------------");
   MailSendTask msendTask = new MailSendTask();
  timer.schedule(msendTask, 60 * 1000, 10 * 1000);
   System.out.println("-------MailSend   Timer已经在执行--------------");
```
