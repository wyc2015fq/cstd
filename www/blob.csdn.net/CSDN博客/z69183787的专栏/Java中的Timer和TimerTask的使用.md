# Java中的Timer和TimerTask的使用 - z69183787的专栏 - CSDN博客
2012年11月19日 09:43:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：566
Timer是一个定时器类，通过该类可以为指定的定时任务进行配置。TimerTask类是一个定时任务类，该类实现了Runnable接口，而且是一个抽象类，如下所示：
　　public abstract class TimerTask implements Runnable
　　可以通过继承该类，来实现自己的定时任务。
　　Timer定时器实例有多种构造方法：
　　Timer()
　　创建一个新计时器。
　　Timer(boolean isDaemon)
　　创建一个新计时器，可以指定其相关的线程作为守护程序运行。
　　Timer(String name)
　　创建一个新计时器，其相关的线程具有指定的名称。
　　Timer(String name, boolean isDaemon)
　　创建一个新计时器，其相关的线程具有指定的名称，并且可以指定作为守护程序运行。
　　下面是一个使用Timer和TimerTask的例子：
　　package org.shirdrn;
　　import java.util.Date;
　　import java.util.Timer;
　　import java.util.TimerTask;
　　public class MyTimerTask extends TimerTask {
　　private int index = 0;
　　private Timer timer = new Timer();
　　@Override
　　public void run() {
　　System.out.println(index + ". current time : " + new Date().getTime());
　　index++;
　　}
　　public void start() {
　　timer.schedule(this, 5000);  // 设置该定时任务MyTimerTask在5s后执行
　　System.out.println("scheduled time : " + this.scheduledExecutionTime());   // 打印出定时任务执行时间
　　}
　　public long getScheduleTime() {
　　return this.scheduledExecutionTime();
　　}
　　public static void main(String[] args) {
　　MyTimerTask mtt = new MyTimerTask();
　　mtt.start();
　　}
　　}
　　可以得到如下输出：
　　scheduled time : 1233552478062
　　0. current time : 1233552478062
　　这里，只是设置了一个定时任务，只是执行一次。可以调用Timer的scheduleAtFixedRate方法来设置定时任务执行时间，并以指定的速率来重复执行定时任务，修改start方法，如下：
　　public void start() {
　　timer.scheduleAtFixedRate(this, new Date(), 2000);  // 设置定时任务执行时间，及其速率（2s执行一次）
　　System.out.println("scheduled time : " + this.scheduledExecutionTime());
　　}
　　可以得到类似如下的输出：
　　scheduled time : 1233552825031
　　0. current time : 1233552825031
　　1. current time : 1233552827031
　　2. current time : 1233552829031
　　3. current time : 1233552831046
　　4. current time : 1233552833031
　　5. current time : 1233552835031
　　6. current time : 1233552837031
　　7. current time : 1233552839031
　　8. current time : 1233552841031
　　可见，使用scheduleAtFixedRate方法设置，可以使run方法体中的代码重复执行，也就是重复执行定时任务，相当于在线程的run方法体中使用一个无穷循环。
　　另外，Timer类中定义了一个定时任务队列，用来管理基于该定时器Timer的所有定时任务。可以定义多个定时任务，同时指定一个定时器，用同一个定时器来安排定时任务，例如：
　　package org.shirdrn;
　　import java.util.Date;
　　import java.util.Timer;
　　import java.util.TimerTask;
　　public class YourTimerTask extends TimerTask {
　　private String name;  // 定时任务的名称
　　public YourTimerTask(String name) {
　　this.name = name;
　　}
　　@Override
　　public void run() {
　　System.out.println(name + "-> current time : " + new Date().getTime());
　　}
　　public void setTimer(Timer timer) {
　　timer.scheduleAtFixedRate(this, new Date(), 2000);
　　System.out.println("scheduled time : " + this.scheduledExecutionTime());
　　}
　　public String getName() {
　　return this.name;
　　}
　　}
该类YourTimerTask 是一个定时任务类，可以实例化多个定时任务的实例。
　　创建一个Timer定时器实例，并为多个定时任务设置同一个定时器，如下所示：
　　package org.shirdrn;
　　import java.util.Date;
　　import java.util.Timer;
　　public class TimerTaskTest {
　　public static void main(String[] args) {
　　Timer timer = new Timer();
　　YourTimerTask ytta = new YourTimerTask("a");
　　YourTimerTask yttb = new YourTimerTask("b");
　　YourTimerTask yttc = new YourTimerTask("c");
　　ytta.setTimer(timer);
　　try {
　　Thread.sleep(2000);
　　} catch (InterruptedException e) {
　　e.printStackTrace();
　　}
　　yttb.setTimer(timer);
　　try {
　　Thread.sleep(2000);
　　} catch (InterruptedException e) {
　　e.printStackTrace();
　　}
　　yttc.setTimer(timer);
　　}
　　}
　　运行结果如下所示：
　　scheduled time : 1233554559343
　　a-> current time : 1233554559343
　　a-> current time : 1233554561343
　　scheduled time : 1233554559343
　　b-> current time : 1233554561343
　　scheduled time : 1233554561359
　　b-> current time : 1233554563359
　　a-> current time : 1233554563359
　　c-> current time : 1233554563359
　　b-> current time : 1233554565343
　　a-> current time : 1233554565343
　　c-> current time : 1233554565375
　　a-> current time : 1233554567343
　　b-> current time : 1233554567343
　　c-> current time : 1233554567375
　　b-> current time : 1233554569343
　　a-> current time : 1233554569343
　　c-> current time : 1233554569375
　　a-> current time : 1233554571343
　　b-> current time : 1233554571343
　　c-> current time : 1233554571375
　　可以通过调用Timer类的cancel方法终止该定时器，调用purge方法重该定时器队列中移除所有已经取消的定时任务。
　　更多的可以参考帮助文档：
　　void cancel()
　　终止此计时器，丢弃所有当前已安排的任务。
　　int purge()
　　从此计时器的任务队列中移除所有已取消的任务。
　　void schedule(TimerTask task, Date time)
　　安排在指定的时间执行指定的任务。
　　void schedule(TimerTask task, Date firstTime, long period)
　　安排指定的任务在指定的时间开始进行重复的固定延迟执行。
　　void schedule(TimerTask task, long delay)
　　安排在指定延迟后执行指定的任务。
　　void schedule(TimerTask task, long delay, long period)
　　安排指定的任务从指定的延迟后开始进行重复的固定延迟执行。
　　void scheduleAtFixedRate(TimerTask task, Date firstTime, long period)
　　安排指定的任务在指定的时间开始进行重复的固定速率执行。
　　void scheduleAtFixedRate(TimerTask task, long delay, long period)
　　安排指定的任务在指定的延迟后开始进行重复的固定速率执行。
