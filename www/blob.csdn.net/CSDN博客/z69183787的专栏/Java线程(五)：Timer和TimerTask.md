# Java线程(五)：Timer和TimerTask - z69183787的专栏 - CSDN博客
2014年06月05日 16:55:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：648
个人分类：[多线程-专栏](https://blog.csdn.net/z69183787/article/category/6816123)

  Timer和TimerTask可以做为实现线程的第三种方式，前两中方式分别是继承自Thread类和实现Runnable接口。
        Timer是一种线程设施，用于安排以后在后台线程中执行的任务。可安排任务执行一次，或者定期重复执行，可以看成一个定时器，可以调度TimerTask。TimerTask是一个抽象类，实现了Runnable接口，所以具备了多线程的能力。
        一个Timer可以调度任意多个TimerTask，它会将TimerTask存储在一个队列中，顺序调度，如果想两个TimerTask并发执行，则需要创建两个Timer。下面来看一个简单的例子：        
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7436458#)[copy](http://blog.csdn.net/ghsau/article/details/7436458#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/92424/fork)
- import java.util.Timer;  
- import java.util.TimerTask;  
- publicclass TimerTest {  
- staticclass MyTimerTask1 extends TimerTask {  
- publicvoid run() {  
-             System.out.println("爆炸！！！");  
-         }  
-     }     
- publicstaticvoid main(String[] args) {  
-         Timer timer = new Timer();  
-         timer.schedule(new MyTimerTask1(), 2000);// 两秒后启动任务
-     }  
- }  
        schedule是Timer调度任务的方法，Timer重构了四个schedule方法，具体可以查看JDK API。
        看一个稍复杂的例子，假设有这样一种需求，实现一个连环炸弹，2秒后爆炸一次，3秒后爆炸一次，如此循环下去，这就需要创建两个任务，互相调度，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7436458#)[copy](http://blog.csdn.net/ghsau/article/details/7436458#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/92424/fork)
- import java.util.Date;  
- import java.util.Timer;  
- import java.util.TimerTask;  
- publicclass TimerTest {  
- staticclass MyTimerTask1 extends TimerTask {  
- publicvoid run() {  
-             System.out.println("爆炸！！！");  
- new Timer().schedule(new MyTimerTask2(), 2000);  
-         }  
-     }  
- staticclass MyTimerTask2 extends TimerTask {  
- publicvoid run() {  
-             System.out.println("爆炸！！！");  
- new Timer().schedule(new MyTimerTask1(), 3000);  
-         }  
-     }  
- publicstaticvoid main(String[] args) {  
-         Timer timer = new Timer();  
-         timer.schedule(new MyTimerTask2(), 2000);  
- while(true) {  
-             System.out.println(new Date().getSeconds());  
- try {  
-                 Thread.sleep(1000);  
-             } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- }  
        自JDK5之后，可以用ScheduledThreadPoolExecutor来替代Timer。
        本文来自：[高爽|Coder](http://blog.csdn.net/ghsau)，原文地址：[http://blog.csdn.net/ghsau/article/details/7436458](http://blog.csdn.net/ghsau/article/details/7436458)，转载请注明
