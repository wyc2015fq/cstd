# Java语言cron4j轻量调度框架 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年09月10日 11:48:58[boonya](https://me.csdn.net/boonya)阅读数：11558








```java

```
前言：cron4j是一个轻量级的java任务调度工具，可以采用Unix Crontab语法来设置任务调度时间。1、官方学习网址：[http://www.sauronsoftware.it/projects/cron4j/manual.php](http://www.sauronsoftware.it/projects/cron4j/manual.php)2、快速开发示例开启线程调度：

```java
import it.sauronsoftware.cron4j.Scheduler;

public class Quickstart {

	public static void main(String[] args) {
		// Creates a Scheduler instance.
		Scheduler s = new Scheduler();
		// Schedule a once-a-minute task.
		s.schedule("* * * * *", new Runnable() {
			public void run() {
				System.out.println("Another minute ticked away...");
			}
		});
		// Starts the scheduler.
		s.start();
		// Will run for ten minutes.
		try {
			Thread.sleep(1000L * 60L * 10L);
		} catch (InterruptedException e) {
			;
		}
		// Stops the scheduler.
		s.stop();
	}

}
```
系统进程调度：
```java
public void doTask(){
  ProcessTask task = new ProcessTask("C:\\Windows\\System32\\notepad.exe");
  Scheduler scheduler = new Scheduler();
  scheduler.schedule("* * * * *", task);
  scheduler.start();
}
```
这么简单的就是实现了任务调度了,确实够轻量,perfect!



