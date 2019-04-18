# Java 多线程详解（五）------线程的声明周期 - weixin_33985507的博客 - CSDN博客
2017年05月25日 09:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
这篇博客介绍线程的生命周期。
　　线程是一个动态执行的过程，它也有从创建到死亡的过程。
在 Thread 类中，有一个枚举内部类：
![](https://images2015.cnblogs.com/blog/1120165/201705/1120165-20170523213933544-1652703376.png)
上面的信息以图片表示如下：
　　第一张图：
![](https://images2015.cnblogs.com/blog/1120165/201705/1120165-20170523213522279-925058645.png)
　　第二张图：把等待、计时等待、阻塞看成阻塞一个状态了
![](https://images2015.cnblogs.com/blog/1120165/201705/1120165-20170523213730622-775091783.png)
**1、新建状态（new）:使用 new 创建一个线程，仅仅只是在堆中分配了内存空间**
　　　　　　　　　　 新建状态下，线程还没有调用 start()方法启动，只是存在一个线程对象而已
　　　　　　　　　　Thread t = new Thread();//这就是t线程的新建状态
**2、可运行状态（runnable）：新建状态调用 start() 方法，进入可运行状态。而这个又分成两种状态，ready 和 running，分别表示就绪状态和运行状态**
　　　　就绪状态：线程对象调用了 start() 方法，等待 JVM 的调度，（此时该线程并没有运行）
　　　　运行状态：线程对象获得 JVM 调度，如果存在多个 CPU，那么运行多个线程并行运行
　　注意：线程对象只能调用一次 start() 方法，否则报错：illegaThreadStateExecptiong
**3、阻塞状态（blocked）:正在运行的线程因为某种原因放弃 CPU，暂时停止运行，就会进入阻塞状态。此时 JVM 不会给线程分配 CPU，知道线程重新进入就绪状态，才有机会转到 运行状态。**
　　注意：阻塞状态只能先进入就绪状态，不能直接进入运行状态
　　阻塞状态分为两种情况：
　　　　①、当线程 A 处于可运行状态中，试图获取同步锁时，却被 B 线程获取，此时 JVM 把当前 A 线程放入锁池中，A线程进入阻塞状态
　　　　②、当线程处于运行状态时，发出了 IO 请求，此时进入阻塞状态
**4、等待状态（waiting）:等待状态只能被其他线程唤醒，此时使用的是无参数的 wait() 方法**
　　①、当线程处于运行状态时，调用了 wait() 方法，此时 JVM 把该线程放入等待池中
**5、计时等待（timed waiting）:调用了带参数的 wait（long time）或 sleep(long time) 方法**
　　①、当线程处于运行状态时，调用了带参数 wait 方法，此时 JVM 把该线程放入等待池中
　　②、当前线程调用了 sleep(long time) 方法
**6、终止状态（terminated）:通常称为死亡状态，表示线程终止**
　　①、正常终止，执行完 run() 方法，正常结束
　　②、强制终止，如调用 stop() 方法或 destory() 方法
　　③、异常终止，执行过程中发生异常
**下面详细介绍线程的几种方法：**
**1、sleep(long millis)线程休眠：让执行的线程暂停一段时间，进入计时等待状态。**
**　　　　static void sleep(long millis):调用此方法后，当前线程放弃 CPU 资源，在指定的时间内，sleep 所在的线程不会获得可运行的机会，此状态下的线程不会释放同步锁（注意和 wait() 的区别，wait 会放弃 CPU 资源，同时也会放弃 同步锁）**
**　　　　该方法更多的是用来模拟网络延迟，让多线程并发访问同一资源时的错误效果更加明显。**
**2、join()联合线程：表示这个线程等待另一个线程完成后（死亡）才执行，join 方法被调用之后，线程对象处于阻塞状态。写在哪个线程中，哪个线程阻塞**
**这种也称为联合线程，就是说把当前线程和当前线程所在的线程联合成一个线程**
```java
package com.ys.thread;
class Join extends Thread{
	@Override
	public void run() {
		for(int i = 0 ; i < 10 ;i++){
			System.out.println("播放音乐"+i);
		}
	}
}
public class ThreadTest {
	public static void main(String[] args) {
		//创建 join 线程对象
		Join joinThread = new Join();
		for(int i = 0 ; i < 10 ; i++){
			System.out.println("玩游戏"+i);
			if(i==3){
				joinThread.start();
			}
			if(i==5){
				try {
					joinThread.join();//强制运行 join 线程，知道 join 运行完毕了，main 才有机会运行
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
```
　　结果：
```java
玩游戏0
玩游戏1
玩游戏2
玩游戏3
玩游戏4
玩游戏5
播放音乐0
播放音乐1
播放音乐2
播放音乐3
播放音乐4
播放音乐5
播放音乐6
播放音乐7
播放音乐8
播放音乐9
玩游戏6
玩游戏7
玩游戏8
玩游戏9
```
**后台线程（守护线程）：在后台运行的线程，其目的是为其他线程提供服务，也称为“守护线程”。**
**①、JVM 的垃圾回收线程就是守护线程。**
**②、main 方法是前台线程，不是后台线程**
**![](https://images2015.cnblogs.com/blog/1120165/201705/1120165-20170523223345763-681049369.png)**
```java
public static void main(String[] args) {
		String mainThreadName = Thread.currentThread().getName();
		System.out.println(mainThreadName);  //main
		
		System.out.println(Thread.currentThread().isDaemon());//false
		
	}
```
**特点：**
**①、若所有的前台线程都死亡，则后台线程自动死亡；**
**②、前台线程没有结束，后台线程是不会结束的；**
**③、前台线程创建的线程是前台线程，后台线程创建的线程是后台线程。**
**　　Thread.setDaemon(Boolean on)必须在 start() 的方法前调用。否则会报错。**
**线程的优先级：**
**　　每个线程都有一个优先级，这有助于 系统确定线程的调动顺序。**
**　　Java 线程的优先级是一个整数，取值范围是：1 （Thread.MIN_PRIORITY ） - 10 （Thread.MAX_PRIORITY ）**
**　　默认情况下，每一个线程都会分配一个优先级 NORM_PRIORITY（5）。**
**　　具有较高优先级的线程对程序更重要，并且应该在低优先级的线程之前分配处理器资源。但是，线程优先级不能保证线程执行的顺序，而且非常依赖于平台。**
**线程礼让：**
**yield()方法：表示当前线程对象提示调度器自己愿意让出 CPU 资源，但是调度器可以自由的忽略该提示。**
**　　　　　　　调用该方法后，线程对象进入就绪状态，所以完全有可能：某个线程调用了 yield() 方法，但是线程调度器又把它调度出来重新执行。**
**从 Java7 提供的文档上可以清楚的看出，开发中会很少使用该方法，该方法主要运用于调试或测试，它可能有助于多线程竞争条件下的错误重现现象。**
**sleep() 和 yield() 方法的区别：**
**　　①、都能使当前处于运行状态的线程放弃 CPU资源，把运行的机会给其他线程**
**　　②、sleep 方法会给其他线程运行的机会，但是不考虑其他线程优先级的问题；yield 方法会优先给更高优先级的线程运行机会**
**　　③、调用 sleep 方法后，线程进入计时等待状态，调用 yield 方法后，线程进入就绪状态。**
