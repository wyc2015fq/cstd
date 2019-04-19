# java多线程之Condition实现更高效线程通信 - 零度的博客专栏 - CSDN博客
2016年04月14日 10:41:44[零度anngle](https://me.csdn.net/zmx729618)阅读数：740
         Condition 将 Object 监视器方法（wait、notify 和 notifyAll）分解成截然不同的对象，以便通过将这些对象与任意 Lock 实现组合使用，为每个对象提供多个等待 set （wait-set）。其中，Lock 替代了 synchronized 方法和语句的使用，Condition 替代了 Object 监视器方法的使用。
下面提供一个测试代码：
```
package zmx.thread.test;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
public class ThreadTest{
	public static void main(String[] args) {
		
		final Business business = new Business();
				
		/**
		 * 该代码必须子线程先执行，否则会出现死锁现象
		 */
		
		//子线程
		new Thread(new Runnable() {
			@Override
			public void run() {
				threadExecute(business, "sub");
			}
		}).start();
		
		
		//主线程
		threadExecute(business, "main");
		
		
		
	}
	
	/**
	 * 执行线程
	 * @param business
	 * @param threadType
	 */
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
	private Lock lock = new ReentrantLock();  //重入锁
	private Condition condition = lock.newCondition(); //使用锁生成一个Condition实例
	
	/**
	 * 主线程打印序列
	 * @param loop
	 * @throws InterruptedException
	 */
	public /*synchronized*/ void main(int loop) throws InterruptedException {
		lock.lock();
		try {
			while(bool) {  //此处本人测试也可以使用if，但是高手都用while，
				//线程等待
				condition.await();//this.wait();  
			}
			for(int i = 0; i < 100; i++) {
				System.out.println("main thread seq of " + i + ", loop of " + loop);
			}
			Thread.sleep(1000);
			bool = true;
			condition.signal();//this.notify();
		} finally {
			lock.unlock();
		}
	}
	
	
	/**
	 * 子线程打印序列
	 * @param loop
	 * @throws InterruptedException
	 */
	public /*synchronized*/ void sub(int loop) throws InterruptedException {
		lock.lock();
		try {
			while(!bool) {//此处本人测试也可以使用if，但是高手都用while，
				condition.await();//this.wait();
			}
			for(int i = 0; i < 10; i++) {
				System.out.println("sub thread seq of " + i + ", loop of " + loop);
			}
			Thread.sleep(1000);
			bool = false;
			condition.signal();//this.notify();
		} finally {
			lock.unlock();
		}
	}
}
```
在Condition中，用await()替换wait()，用signal()替换notify()，用signalAll()替换notifyAll()，传统线程的通信方式，Condition都可以实现，这里注意，Condition是被绑定到Lock上的，要创建一个Lock的Condition必须用newCondition()方法。
        这样看来，Condition和传统的线程通信没什么区别，Condition的强大之处在于它可以为多个线程间建立不同的Condition，下面引入另一段代码，加以说明。
```
<span style="font-size:14px;">package zmx.thread.test;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
/**
 * 一个处于多线程工作环境下的对象缓存区
 * 对象池
 * @author zhangwenchao
 *
 */
public class BoundedBuffer {
	final Lock lock = new ReentrantLock();// 锁对象
	final Condition notFull = lock.newCondition(); // 写线程条件
	final Condition notEmpty = lock.newCondition();// 读线程条件
	final Object[] items = new Object[100];// 对象缓存队列
	int putptr; /* 写索引 */
	int takeptr;/* 读索引 */
	int count;  /* 队列中存在的数据个数 */;
	/**
	 * 往缓冲池中加入对象--写入写索引对应的位置
	 * @param x
	 * @throws InterruptedException
	 */
	public void put(Object x) throws InterruptedException {
		lock.lock();
		try {
			while (count == items.length){// 如果队列满了
				notFull.await();// 阻塞写线程
			}	
			items[putptr] = x;// 赋值
			System.out.println("写入数据："+x+",写索引:"+ putptr);  //注意打印一定要放到锁内
			if (++putptr == items.length){
				putptr = 0;// 如果写索引写到队列的最后一个位置了，那么置为0
			}
				
			++count;// 个数++
			notEmpty.signal();// 唤醒读线程
		} finally {
			lock.unlock();
		}
	}
    
	/**
	 * 从缓冲池读出数据对象--从读索引对应的位置读出一个数据对象
	 * @return
	 * @throws InterruptedException
	 */
	public Object take() throws InterruptedException {
		lock.lock();
		try {
			while (count == 0){// 如果队列为空				
				notEmpty.await();// 阻塞读线程
			}	
			Object x = items[takeptr];// 取值
			
			System.out.println("读出数据："+x+",读索引:"+ takeptr);   //打印一定要放到锁内
			
			if (++takeptr == items.length){
				takeptr = 0;// 如果读索引读到队列的最后一个位置了，那么置为0
			}
				
			--count;// 个数--
			notFull.signal();// 唤醒写线程
			return x;
		} finally {
			lock.unlock();
		}
	}
	
	
	
	public static void main(String[] args) throws Exception {
		
	    final BoundedBuffer boundedBuffer = new BoundedBuffer();
	    
		for( int i=0; i<200;i++ ){//分别开启100个读/写线程
			final int data = i;
			new Thread(new Runnable() {
				@Override
				public void run() {
					try {	
						boundedBuffer.put(data);						
						Thread.sleep((int)(Math.random()*1000));
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					
				}
			}).start();
			
			Thread.sleep(200);
			
			new Thread(new Runnable() {
				@Override
				public void run() {
					try {
						Integer obj = (Integer) boundedBuffer.take();
						Thread.sleep((int)(Math.random()*1000));
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					
				}
			}).start();			
			
			
			
						
		}
		
	}
}
</span>
```
这是一个处于多线程工作环境下的缓存区，缓存区提供了两个方法，put和take，put是存数据，take是取数据，内部有个缓存队列，具体变量和方法说明见代码，这个缓存区类实现的功能：有多个线程往里面存数据和从里面取数据，其缓存队列(先进先出后进后出)能缓存的最大数值是100，多个线程间是互斥的，当缓存队列中存储的值达到100时，将写线程阻塞，并唤醒读线程，当缓存队列中存储的值为0时，将读线程阻塞，并唤醒写线程，下面分析一下代码的执行过程：
        1. 一个写线程执行，调用put方法；
        2. 判断count是否为100，显然没有100；
        3. 继续执行，存入值；
        4. 判断当前写入的索引位置++后，是否和100相等，相等将写入索引值变为0，并将count+1；
        5. 仅唤醒**读线程阻塞队列**中的一个；
        6. 一个读线程执行，调用take方法；
        7. ……
        8. 仅唤醒**写线程阻塞队列**中的一个。
        这就是多个Condition的强大之处，假设缓存队列中已经存满，那么阻塞的肯定是写线程，唤醒的肯定是读线程，相反，阻塞的肯定是读线程，唤醒的肯定是写线程，那么假设只有一个Condition会有什么效果呢，缓存队列中已经存满，这个Lock不知道唤醒的是读线程还是写线程了，如果唤醒的是读线程，皆大欢喜，如果唤醒的是写线程，那么线程刚被唤醒，又被阻塞了，这时又去唤醒，这样就浪费了很多时间。

