# ReentrantLock使用详解(3)之测试锁与超时 - 零度的博客专栏 - CSDN博客
2017年04月12日 10:23:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：2665
我在上面的这篇博客中，曾经写到过线程在获取锁的过程中是无法中断的。
但是ReentrantLock提供了tryLock()、tryLock(long timeout, TimeUnit unit)、lock.lockInterruptibly()
tryLock() 方法试图申请一个锁，在成功获得锁后返回true，否则，立即返回false,而且线程可以立即离开去做其他的事情。
tryLock(long timeout, TimeUnit unit) 是一个具有超时参数的尝试申请锁的方法，阻塞时间不会超过给定的值；如果成功则返回true
lockInterruptibly() 获得锁，但是会不确定地发生阻塞。如果线程被中断，抛出一个InterruptedException异常。
如果当前获得锁的线程在做大量耗时的工作，使用lock.lock()方法申请锁的线程会一直阻塞，这样就降低了多线程的效率。而使用tryLock()方法申请锁，如果锁不可用则线程不会阻塞，转而可以去做其他工作。代码实例如下：
**[java]**[view plain](http://blog.csdn.net/u011784767/article/details/51659701#)[copy](http://blog.csdn.net/u011784767/article/details/51659701#)[print](http://blog.csdn.net/u011784767/article/details/51659701#)[?](http://blog.csdn.net/u011784767/article/details/51659701#)
- publicclass ReentrantLockTest4 {  
- 
- private ReentrantLock lock = new ReentrantLock();  
- 
- publicvoid tryLockTest() throws InterruptedException {  
- 
- long beginTime = System.currentTimeMillis();  
- while(System.currentTimeMillis() - beginTime <100) {}  
- // 当前线程尝试获得锁，如果获得锁返回true，否则返回false
- if(lock.tryLock()) {  
- try{  
-                 System.out.println(Thread.currentThread().getName() + " tryLock get lock");  
-             } finally {  
-                 lock.unlock();  
-                 System.out.println(Thread.currentThread().getName() + " tryLock release lock");  
-             }  
-         } else {  
-             System.out.println(Thread.currentThread().getName() + " tryLock can not get lock");  
-         }  
- 
-     }  
- 
- 
- publicvoid lockTest() {  
- try{  
- // 当前线程在锁可用时直接获得锁，锁不可用时阻塞当前线程
-             lock.lock();  
-             System.out.println(Thread.currentThread().getName() + " lock get lock");  
- long beginTime = System.currentTimeMillis();  
- while(System.currentTimeMillis() - beginTime <1000) {}  
-         } finally {  
-             lock.unlock();  
-             System.out.println(Thread.currentThread().getName() + " lock release lock");  
-         }  
- 
-     }  
- 
- publicstaticvoid main(String[] args) {  
- // TODO Auto-generated method stub
- final ReentrantLockTest4 test = new ReentrantLockTest4();  
-         Thread tryLock = new Thread(new Runnable() {  
- publicvoid run() {  
- try {  
-                     test.tryLockTest();  
-                 } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-                     e.printStackTrace();  
-                 }  
-             }  
-         },"tryLock_thread");  
- 
-         Thread lock = new Thread(new Runnable() {  
- publicvoid run() {  
-                 test.lockTest();  
-             }  
-         },"lock_thread");  
- 
-         tryLock.start();  
-         lock.start();  
-     }  
- 
- }  
- 
- 输出结果：  
- lock_thread lock get lock  
- tryLock_thread tryLock can not get lock  
- lock_thread lock release lock  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class ReentrantLockTest4 {
	private ReentrantLock lock = new ReentrantLock();
	
	public void tryLockTest() throws InterruptedException {
		
		long beginTime = System.currentTimeMillis();
		while(System.currentTimeMillis() - beginTime <100) {}
		// 当前线程尝试获得锁，如果获得锁返回true，否则返回false
		if(lock.tryLock()) {
			try{
				System.out.println(Thread.currentThread().getName() + " tryLock get lock");
			} finally {
				lock.unlock();
				System.out.println(Thread.currentThread().getName() + " tryLock release lock");
			}
		} else {
			System.out.println(Thread.currentThread().getName() + " tryLock can not get lock");
		}
		
	}
	
	
	public void lockTest() {
		try{
			// 当前线程在锁可用时直接获得锁，锁不可用时阻塞当前线程
			lock.lock();
			System.out.println(Thread.currentThread().getName() + " lock get lock");
			long beginTime = System.currentTimeMillis();
			while(System.currentTimeMillis() - beginTime <1000) {}
		} finally {
			lock.unlock();
			System.out.println(Thread.currentThread().getName() + " lock release lock");
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final ReentrantLockTest4 test = new ReentrantLockTest4();
		Thread tryLock = new Thread(new Runnable() {
			public void run() {
				try {
					test.tryLockTest();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		},"tryLock_thread");
		
		Thread lock = new Thread(new Runnable() {
			public void run() {
				test.lockTest();
			}
		},"lock_thread");
		
		tryLock.start();
		lock.start();
	}
}
输出结果：
lock_thread lock get lock
tryLock_thread tryLock can not get lock
lock_thread lock release lock
```
lock方法不能被中断。如果一个线程在等待获得一个锁时被中断，中断线程在获得锁之前会一直处于 阻塞状态。如果出现死锁，那么lock方法就无法被终止。但是tryLock和lockInterruptibly方法在申请锁的过程中是可以被中断的。代码如下
**[java]**[view plain](http://blog.csdn.net/u011784767/article/details/51659701#)[copy](http://blog.csdn.net/u011784767/article/details/51659701#)[print](http://blog.csdn.net/u011784767/article/details/51659701#)[?](http://blog.csdn.net/u011784767/article/details/51659701#)
- publicclass ReentrantLockTest5 {  
- 
- private ReentrantLock lock = new ReentrantLock();  
- 
- publicvoid tryLockInterruptTest() {  
- long beginTime = System.currentTimeMillis();  
- while(System.currentTimeMillis() - beginTime <100) {}  
- try {  
- if (lock.tryLock(5000, TimeUnit.MILLISECONDS)) {  
- try{  
-                     System.out.println(Thread.currentThread().getName() + " tryLock get lock");  
-                 }finally {  
-                     lock.unlock();  
-                 }  
-             }  
-         } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-             System.out.println(Thread.currentThread().getName() + " was interrupted");  
-         }  
-     }  
- 
- 
- publicvoid lockTest() {  
- try{  
- // 当前线程在锁可用时直接获得锁，锁不可用时阻塞当前线程
-             lock.lock();  
-             System.out.println(Thread.currentThread().getName() + " lock get lock");  
- long beginTime = System.currentTimeMillis();  
- while(System.currentTimeMillis() - beginTime <1000) {}  
-         } finally {  
-             lock.unlock();  
-             System.out.println(Thread.currentThread().getName() + " lock release lock");  
-         }  
- 
-     }  
- 
- publicstaticvoid main(String[] args) {  
- // TODO Auto-generated method stub
- final ReentrantLockTest5 test =  new ReentrantLockTest5();  
-         Thread thread_tryLock = new Thread(new Runnable(){  
- @Override
- publicvoid run() {  
- // TODO Auto-generated method stub
-                 test.tryLockInterruptTest();  
-             }  
-         },"tryLockInterruptTest");  
-         Thread thread_lock = new Thread(new Runnable(){  
- @Override
- publicvoid run() {  
- // TODO Auto-generated method stub
-                 test.lockTest();  
-             }  
-         },"lockTest");  
-         thread_tryLock.start();  
-         thread_lock.start();  
- try {  
-             Thread.sleep(200);  
-         } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-             System.out.println("main thread was interrupted");  
-         }  
-         thread_tryLock.interrupt();  
-     }  
- 
- }  
- 
- 
- 输出结果：  
- lockTest lock get lock  
- tryLockInterruptTest was interrupted  
- lockTest lock release lock  
