# Timer的schedule和scheduleAtFixedRate方法的区别解析 - z69183787的专栏 - CSDN博客
2012年11月19日 09:50:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1078
在java中，Timer类主要用于**定时性、周期性任务**的触发，这个类中有两个方法比较难理解，那就是schedule和scheduleAtFixedRate方法，在这里就用实例分析一下
（1）schedule方法：“fixed-delay”；如果第一次执行时间被delay了，随后的执行时间**按 照****上一次****实际执行完成的时间点**进行计算
（2）scheduleAtFixedRate方法：“fixed-rate”；如果第一次执行时间被delay了，随后的执行时间**按照****上一次开始的****时间点**进行计算，并且为了”catch
 up”会多次执行任务,TimerTask中的执行体**需要考虑同步**
**[c-sharp]**[view
 plain](http://blog.csdn.net/gtuu0123/article/details/6040159#)[copy](http://blog.csdn.net/gtuu0123/article/details/6040159#)
- SimpleDateFormat dateFormatter = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");  
- Date startDate = dateFormatter.parse("2010/11/26 00:20:00");  
- Timer timer = new Timer();  
- timer.scheduleAtFixedRate(new TimerTask(){  
- publicvoid run()  
-    {  
-        System.out.println("execute task!" + this.scheduledExecutionTime());  
-    }  
- },startDate,3*60*1000);  
以上的代码，表示在2010-11-26 00:20:00秒开始执行，每3分钟执行一次
假设在2010/11/26 00:27:00执行
以上会打印出3次
execute task!   00:20
execute task!   00:23    catch up
execute task!   00:26    catch up
下一次执行时间是00:29，相对于00:26
当换成schedule方法时，在2010/11/26 00:27:00执行
会打印出1次
execute task!   00:20   无catch up
下一次执行时间为00:30，相对于00:27
以上考虑的都是在你设定的timer开始时间后，程序才被执行
当执行任务的时间大于周期间隔时，会发生什么呢？
（1）schedule方法：下一次执行时间相对于**上一次****实际执行完成的时间点**，因此执行时间会不断延后
（2）scheduleAtFixedRate方法：下一次执行时间相对于**上一次开始的****时间点**，因此执行时间不会延后，存在并发性
以下例程序来测试上述结论，TimerTask需要执行6秒钟，但是间隔周期为5秒钟
**[java]**[view
 plain](http://blog.csdn.net/gtuu0123/article/details/6040159#)[copy](http://blog.csdn.net/gtuu0123/article/details/6040159#)
- package test;  
- import java.text.ParseException;  
- import java.text.SimpleDateFormat;  
- import java.util.Date;  
- import java.util.Timer;  
- import java.util.TimerTask;  
- publicclass Test {  
- 
- publicstaticvoid main(String[] args) throws ParseException {  
-         SimpleDateFormat dateFormatter = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");  
-         Date startDate = dateFormatter.parse("2010/11/28 01:06:00");  
-         Timer timer = new Timer();  
-         timer.schedule(new TimerTask(){  
- publicvoid run() {  
- try {  
-                    Thread.sleep(6000);  
-                } catch (InterruptedException e) {  
-                    e.printStackTrace();  
-                }  
-                System.out.println("execute task!"+ this.scheduledExecutionTime());  
-            }  
-         },startDate, 5 * 1000);  
-     }  
- 
- }  
schedule方法的执行结果如下：
execute task!1290877560001
execute task!1290877566001
execute task!1290877572001
execute task!1290877578001
execute task!1290877584001
execute task!1290877590001
execute task!1290877596001
execute task!1290877602001
execute task!1290877608001
execute task!1290877614001
execute task!1290877620001
execute task!1290877626001
execute task!1290877632001
execute task!1290877638001
可以看出，间隔时间都为6秒，因此，**下一次的执行时间点=上一次程序执行完成的时间点+间隔时间**
当换成scheduleAtFixedRate方法的执行结果如下：
execute task!1290877860000
execute task!1290877865000
execute task!1290877870000
execute task!1290877875000
execute task!1290877880000
execute task!1290877885000
execute task!1290877890000
execute task!1290877895000
execute task!1290877900000
execute task!1290877905000
execute task!1290877910000
execute task!1290877915000
execute task!1290877920000
execute task!1290877925000
execute task!1290877930000
可以看出，间隔时间都为5秒，因此，**下一次的执行时间点=上一次程序开始执行的时间点+间隔时间**；并且因为前一个任务要执行6秒，而当前任务已经开始执行了，因此两个任务间存在重叠，**需要考虑线程同步**
