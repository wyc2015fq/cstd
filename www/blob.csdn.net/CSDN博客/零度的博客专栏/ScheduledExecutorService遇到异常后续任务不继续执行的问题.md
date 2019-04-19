# ScheduledExecutorService遇到异常后续任务不继续执行的问题 - 零度的博客专栏 - CSDN博客
2016年05月17日 15:17:27[零度anngle](https://me.csdn.net/zmx729618)阅读数：10697
最近写了个程序，程序中使用到了ScheduledExecutorService的scheduleAtFixedRate方法，用于定时执行任务，但是发现程序运行一段时间之后定时任务不执行了，查看日志和perfcounter都没有看到任何异常，比较郁闷。最后看了一下JDK的源码，在源码的Java
 doc中的发现了如下一句话：If any execution of the task encounters an exception, subsequent executions are suppressed.Otherwise, the task will only terminate via cancellation or
 termination of the executor.
简单总结就是：如果定时任务执行过程中遇到发生异常，则后面的任务将不再执行。
       我们可以做个实验验证一下：
```
package zmx.jdkthreadpool.test;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
public class Test {
	
    private final static ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();
    public static void main(String[] args) {
        scheduler.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                int[] s = new int[1];
                System.out.println("OK");
                System.out.println(s[1]);  // 数组越界
            }
        }, 0, 2, TimeUnit.SECONDS);
    }
}
```
执行结果：
`OK`
        可以看到定时任务只执行了一次，因为第一次执行就遇到了数组越界异常，后面的任务被取消了。
        对上面代码进行修改：
```
package zmx.jdkthreadpool.test;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
public class Test {
	
    private final static ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();
    public static void main(String[] args) {
        scheduler.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                try {
                    int[] s = new int[1];
                    System.out.println("OK");
                    System.out.println(s[1]); // 数组越界
                } catch (Throwable t) {
                    System.out.println("Error");
                }
            }
        }, 0, 2, TimeUnit.SECONDS);
    }
}
```
执行效果：
```
OK
Error
OK
Error
OK
Error
OK
Error
```
加了try/catch之后可以看到定时任务没有被取消。
