# Java线程之线程协作（生产者/消费者问题） - 零度的博客专栏 - CSDN博客
2016年06月07日 12:46:51[零度anngle](https://me.csdn.net/zmx729618)阅读数：297
       上一篇讲述了线程的互斥(同步)，但是在很多情况下，仅仅同步是不够的，还需要线程与线程协作(通信)，生产者/消费者问题是一个经典的线程同步以及通信的案例。该问题描述了两个共享固定大小缓冲区的线程，即所谓的“生产者”和“消费者”在实际运行时会发生的问题。生产者的主要作用是生成一定量的数据放到缓冲区中，然后重复此过程。与此同时，消费者也在缓冲区消耗这些数据。该问题的关键就是要保证生产者不会在缓冲区满时加入数据，消费者也不会在缓冲区中空时消耗数据。要解决该问题，就必须让生产者在缓冲区满时休眠（要么干脆就放弃数据），等到下次消费者消耗缓冲区中的数据的时候，生产者才能被唤醒，开始往缓冲区添加数据。同样，也可以让消费者在缓冲区空时进入休眠，等到生产者往缓冲区添加数据之后，再唤醒消费者，通常采用线程间通信的方法解决该问题。如果解决方法不够完善，则容易出现死锁的情况。出现死锁时，两个线程都会陷入休眠，等待对方唤醒自己。该问题也能被推广到多个生产者和消费者的情形。本文讲述了JDK5之前传统线程的通信方式，更高级的通信方式可参见[Java线程(九)：Condition-线程通信更高效的方式](http://blog.csdn.net/ghsau/article/details/7481142)和[Java线程(篇外篇)：阻塞队列BlockingQueue](http://blog.csdn.net/ghsau/article/details/8108292)。
        假设有这样一种情况，有一个盘子，盘子里只能放一个鸡蛋，A线程专门往盘子里放鸡蛋，如果盘子里有鸡蛋，则一直等到盘子里没鸡蛋，B线程专门从盘子里取鸡蛋，如果盘子里没鸡蛋，则一直等到盘子里有鸡蛋。这里盘子是一个互斥区，每次放鸡蛋是互斥的，每次取鸡蛋也是互斥的，A线程放鸡蛋，如果这时B线程要取鸡蛋，由于A没有释放锁，B线程处于等待状态，进入阻塞队列，放鸡蛋之后，要通知B线程取鸡蛋，B线程进入就绪队列，反过来，B线程取鸡蛋，如果A线程要放鸡蛋，由于B线程没有释放锁，A线程处于等待状态，进入阻塞队列，取鸡蛋之后，要通知A线程放鸡蛋，A线程进入就绪队列。我们希望当盘子里有鸡蛋时，A线程阻塞，B线程就绪，盘子里没鸡蛋时，A线程就绪，B线程阻塞，代码如下：
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7433673#)[copy](http://blog.csdn.net/ghsau/article/details/7433673#)[print](http://blog.csdn.net/ghsau/article/details/7433673#)[?](http://blog.csdn.net/ghsau/article/details/7433673#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/125318/fork)
- import java.util.ArrayList;  
- import java.util.List;  
- /** 定义一个盘子类，可以放鸡蛋和取鸡蛋 */
- publicclass Plate {  
- /** 装鸡蛋的盘子 */
-     List<Object> eggs = new ArrayList<Object>();  
- /** 取鸡蛋 */
- publicsynchronized Object getEgg() {  
- while (eggs.size() == 0) {  
- try {  
-                 wait();  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-         Object egg = eggs.get(0);  
-         eggs.clear();// 清空盘子
-         notify();// 唤醒阻塞队列的某线程到就绪队列
-         System.out.println("拿到鸡蛋");  
- return egg;  
-     }  
- /** 放鸡蛋 */
- publicsynchronizedvoid putEgg(Object egg) {  
- while (eggs.size() > 0) {  
- try {  
-                 wait();  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-         eggs.add(egg);// 往盘子里放鸡蛋
-         notify();// 唤醒阻塞队列的某线程到就绪队列
-         System.out.println("放入鸡蛋");  
-     }  
- staticclass AddThread implements Runnable  {  
- private Plate plate;  
- private Object egg = new Object();  
- public AddThread(Plate plate) {  
- this.plate = plate;  
-         }  
- publicvoid run() {  
-             plate.putEgg(egg);  
-         }  
-     }  
- staticclass GetThread implements Runnable  {  
- private Plate plate;  
- public GetThread(Plate plate) {  
- this.plate = plate;  
-         }  
- publicvoid run() {  
-             plate.getEgg();  
-         }  
-     }  
- publicstaticvoid main(String args[]) {  
-         Plate plate = new Plate();  
- for(int i = 0; i < 10; i++) {  
- new Thread(new AddThread(plate)).start();  
- new Thread(new GetThread(plate)).start();  
-         }  
-     }  
- }  
```java
<span style="font-family:Arial;font-size:12px;">import java.util.ArrayList;
import java.util.List;
/** 定义一个盘子类，可以放鸡蛋和取鸡蛋 */
public class Plate {
	/** 装鸡蛋的盘子 */
	List<Object> eggs = new ArrayList<Object>();
	/** 取鸡蛋 */
	public synchronized Object getEgg() {
		while (eggs.size() == 0) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		Object egg = eggs.get(0);
		eggs.clear();// 清空盘子
		notify();// 唤醒阻塞队列的某线程到就绪队列
		System.out.println("拿到鸡蛋");
		return egg;
	}
	/** 放鸡蛋 */
	public synchronized void putEgg(Object egg) {
		while (eggs.size() > 0) {
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		eggs.add(egg);// 往盘子里放鸡蛋
		notify();// 唤醒阻塞队列的某线程到就绪队列
		System.out.println("放入鸡蛋");
	}
	static class AddThread implements Runnable  {
		private Plate plate;
		private Object egg = new Object();
		public AddThread(Plate plate) {
			this.plate = plate;
		}
		public void run() {
			plate.putEgg(egg);
		}
	}
	static class GetThread implements Runnable  {
		private Plate plate;
		public GetThread(Plate plate) {
			this.plate = plate;
		}
		public void run() {
			plate.getEgg();
		}
	}
	public static void main(String args[]) {
		Plate plate = new Plate();
		for(int i = 0; i < 10; i++) {
			new Thread(new AddThread(plate)).start();
			new Thread(new GetThread(plate)).start();
		}
	}
}
</span>
```
        输出结果： 
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7433673#)[copy](http://blog.csdn.net/ghsau/article/details/7433673#)[print](http://blog.csdn.net/ghsau/article/details/7433673#)[?](http://blog.csdn.net/ghsau/article/details/7433673#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/125318/fork)
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
```java
<span style="font-family:Arial;font-size:12px;">放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋</span>
```
        程序开始，A线程判断盘子是否为空，放入一个鸡蛋，并且唤醒在阻塞队列的一个线程，阻塞队列为空；假设CPU又调度了一个A线程，盘子非空，执行等待，这个A线程进入阻塞队列；然后一个B线程执行，盘子非空，取走鸡蛋，并唤醒阻塞队列的A线程，A线程进入就绪队列，此时就绪队列就一个A线程，马上执行，放入鸡蛋；如果再来A线程重复第一步，在来B线程重复第二步，整个过程就是生产者(A线程)生产鸡蛋，消费者(B线程)消费鸡蛋。
        前段时间看了张孝祥老师线程的视频，讲述了一个其学员的面试题，也是线程通信的，在此也分享一下。
        题目：子线程循环10次，主线程循环100次，如此循环100次，好像是空中网的笔试题。
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7433673#)[copy](http://blog.csdn.net/ghsau/article/details/7433673#)[print](http://blog.csdn.net/ghsau/article/details/7433673#)[?](http://blog.csdn.net/ghsau/article/details/7433673#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/125318/fork)
- publicclass ThreadTest2 {  
- publicstaticvoid main(String[] args) {  
- final Business business = new Business();  
- new Thread(new Runnable() {  
- @Override
- publicvoid run() {  
-                 threadExecute(business, "sub");  
-             }  
-         }).start();  
-         threadExecute(business, "main");  
-     }     
- publicstaticvoid threadExecute(Business business, String threadType) {  
- for(int i = 0; i < 100; i++) {  
- try {  
- if("main".equals(threadType)) {  
-                     business.main(i);  
-                 } else {  
-                     business.sub(i);  
-                 }  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- }  
- class Business {  
- privateboolean bool = true;  
- publicsynchronizedvoid main(int loop) throws InterruptedException {  
- while(bool) {  
- this.wait();  
-         }  
- for(int i = 0; i < 100; i++) {  
-             System.out.println("main thread seq of " + i + ", loop of " + loop);  
-         }  
-         bool = true;  
- this.notify();  
-     }     
- publicsynchronizedvoid sub(int loop) throws InterruptedException {  
- while(!bool) {  
- this.wait();  
-         }  
- for(int i = 0; i < 10; i++) {  
-             System.out.println("sub thread seq of " + i + ", loop of " + loop);  
-         }  
-         bool = false;  
- this.notify();  
-     }  
- }  
```java
<span style="font-family:Arial;font-size:12px;">public class ThreadTest2 {
	public static void main(String[] args) {
		final Business business = new Business();
		new Thread(new Runnable() {
			@Override
			public void run() {
				threadExecute(business, "sub");
			}
		}).start();
		threadExecute(business, "main");
	}	
	public static void threadExecute(Business business, String threadType) {
		for(int i = 0; i < 100; i++) {
			try {
				if("main".equals(threadType)) {
					business.main(i);
				} else {
					business.sub(i);
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}
class Business {
	private boolean bool = true;
	public synchronized void main(int loop) throws InterruptedException {
		while(bool) {
			this.wait();
		}
		for(int i = 0; i < 100; i++) {
			System.out.println("main thread seq of " + i + ", loop of " + loop);
		}
		bool = true;
		this.notify();
	}	
	public synchronized void sub(int loop) throws InterruptedException {
		while(!bool) {
			this.wait();
		}
		for(int i = 0; i < 10; i++) {
			System.out.println("sub thread seq of " + i + ", loop of " + loop);
		}
		bool = false;
		this.notify();
	}
}</span>
```
       大家注意到没有，在调用wait方法时，都是用while判断条件的，而不是if，在wait方法说明中，也推荐使用while，因为在某些特定的情况下，线程有可能被假唤醒，使用while会循环检测更稳妥。wait和notify方法必须工作于synchronized内部，且这两个方法只能由锁对象来调用。
