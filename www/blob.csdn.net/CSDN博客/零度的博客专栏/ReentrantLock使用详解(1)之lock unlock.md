# ReentrantLock使用详解(1)之lock/unlock - 零度的博客专栏 - CSDN博客
2017年04月12日 10:08:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：729
在大多数实际的多线程应用中，两个或两个以上的线程需要共享对同一数据的存取。如果两个线程存取相同的对象，并且每一个线程都调用了一个修改该对象状态的方法，那么线程彼此踩了对方的脚，根据各线程访问数据的次序，可能会产生讹误的对象。这样的一种现象称之为**竞争条件。**
当然多个线程共享一个变量在实际的应用中有时难以避免，但是我们可以通过java提供的一些技术来避免线程彼此踩脚的行为发生。
java提供了锁机制来对多个线程共享一个变量进行同步，在[Java SE](http://lib.csdn.net/base/12) 5.0引入了显式锁ReentrantLock类，以及从1.0版本开始的对象内部锁 可以通过synchronized关键字声明某个线程持有这个对象内部锁，我们这小结先学习ReentrantLock锁的使用，下一节学习synchronized关键字的使用。
**(1) ReentrantLock锁的使用结构**
ReentrantLock 是java.unti.concurrent包下的一个类，它的一般使用结构如下所示：
**[java]**[view plain](http://blog.csdn.net/u011784767/article/details/51439327#)[copy](http://blog.csdn.net/u011784767/article/details/51439327#)[print](http://blog.csdn.net/u011784767/article/details/51439327#)[?](http://blog.csdn.net/u011784767/article/details/51439327#)
- publicvoid lockMethod() {  
-     ReentrantLock myLock = new ReentrantLock();  
-     myLock.lock();  
- try{  
- // 受保护的代码段
- //critical section
-     } finally {  
- // 可以保证发生异常 锁可以得到释放 避免死锁的发生
-         myLock.unlock();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public void lockMethod() {
	ReentrantLock myLock = new ReentrantLock();
	myLock.lock();
	try{
		// 受保护的代码段
		//critical section
	} finally {
		// 可以保证发生异常 锁可以得到释放 避免死锁的发生
		myLock.unlock();
	}
}
```
把解锁操作括在finally字句之内是至关重要的，如果受保护的代码抛出异常，锁可以得到释放，这样可以避免死锁的发生
我们执行下面代码：
**[java]**[view plain](http://blog.csdn.net/u011784767/article/details/51439327#)[copy](http://blog.csdn.net/u011784767/article/details/51439327#)[print](http://blog.csdn.net/u011784767/article/details/51439327#)[?](http://blog.csdn.net/u011784767/article/details/51439327#)
- publicclass ReentrantLockTest1 {  
- 
- privateint num = 10;  
- private ReentrantLock myLock = new ReentrantLock();  
- publicvoid writeNumMethod() {    
- //myLock.lock();
- try{  
- // 受保护的代码段
- int index =10;  
- while(index > 0) {  
-                 System.out.println(Thread.currentThread().getName() + " : "+ num);  
-                 num-=10;  
- long beginTime = System.currentTimeMillis();  
- while(System.currentTimeMillis() - beginTime < 10){}  
-                 num+=10;  
-                 System.out.println(Thread.currentThread().getName() + " : "+ num);  
-                 index--;  
-             }  
- 
-     } finally {  
- // 可以保证发生异常 锁可以得到释放 避免死锁的发生
- //myLock.unlock();
-         }  
-     }  
- 
- publicvoid readNumMethod() {     
- //myLock.lock();
- try{  
- int index = 10;  
- // 受保护的代码段
- while(index > 0) {  
-                 System.out.println(Thread.currentThread().getName() + " : "+ num);  
-                 index--;  
-             }  
- 
-     } finally {  
- // 可以保证发生异常 锁可以得到释放 避免死锁的发生
- //myLock.unlock();
-         }  
-     }  
- 
- 
- publicstaticvoid main(String [] args) {  
- final ReentrantLockTest1 myLockTest = new ReentrantLockTest1();  
- 
-         Thread t1 = new Thread(new Runnable() {  
- publicvoid run() {  
-                 myLockTest.writeNumMethod();  
-             }  
-         },"A");  
- 
-         Thread t2 = new Thread(new Runnable() {  
- publicvoid run() {  
-                 myLockTest.readNumMethod();  
-             }  
-         },"B");  
- 
-         t1.start();  
-         t2.start();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class ReentrantLockTest1 {
	
	private int num = 10;
    private ReentrantLock myLock = new ReentrantLock();
	public void writeNumMethod() {	
		//myLock.lock();
		try{
			// 受保护的代码段
			int index =10;
			while(index > 0) {
				System.out.println(Thread.currentThread().getName() + " : "+ num);
				num-=10;
				long beginTime = System.currentTimeMillis();
				while(System.currentTimeMillis() - beginTime < 10){}
				num+=10;
				System.out.println(Thread.currentThread().getName() + " : "+ num);
				index--;
			}
	} finally {
			// 可以保证发生异常 锁可以得到释放 避免死锁的发生
			//myLock.unlock();
		}
	}
	
	public void readNumMethod() {	
		//myLock.lock();
		try{
			int index = 10;
			// 受保护的代码段
			while(index > 0) {
				System.out.println(Thread.currentThread().getName() + " : "+ num);
				index--;
			}
	} finally {
			// 可以保证发生异常 锁可以得到释放 避免死锁的发生
			//myLock.unlock();
		}
	}
	
	
	public static void main(String [] args) {
		final ReentrantLockTest1 myLockTest = new ReentrantLockTest1();
		
		Thread t1 = new Thread(new Runnable() {
			public void run() {
				myLockTest.writeNumMethod();
			}
		},"A");
		
		Thread t2 = new Thread(new Runnable() {
			public void run() {
				myLockTest.readNumMethod();
			}
		},"B");
		
		t1.start();
		t2.start();
	}
}
```
上面示例中 writeNumMethod方法 对num变量进行先减10 然后再加10，readNumMethod方法读取num的值，线程A执行writeNumMethod方法，线程B执行readNumMethos方法，我们期望两个线程的 输出的结果都是10，但是由于我们没有对这个共享变量num 进行同步，假如此时线程A执行到num-=10这句代码时，线程的cpu时间片到时了，这时操作系统就会去调度线程B这样线程B就会输出0值。我们看一输出结果。
输出结果：
**[java]**[view plain](http://blog.csdn.net/u011784767/article/details/51439327#)[copy](http://blog.csdn.net/u011784767/article/details/51439327#)[print](http://blog.csdn.net/u011784767/article/details/51439327#)[?](http://blog.csdn.net/u011784767/article/details/51439327#)
- A : 10
- B : 10
- B : 0
- B : 0
- B : 0
- B : 0
- B : 0
- B : 0
- B : 0
- B : 0
- B : 0
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
A : 10
B : 10
B : 0
B : 0
B : 0
B : 0
B : 0
B : 0
B : 0
B : 0
B : 0
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
```
果然共享变量出现了讹误的现象。
那么如果我们取消代码中显式锁前的注释，再次运行代码。
**[java]**[view plain](http://blog.csdn.net/u011784767/article/details/51439327#)[copy](http://blog.csdn.net/u011784767/article/details/51439327#)[print](http://blog.csdn.net/u011784767/article/details/51439327#)[?](http://blog.csdn.net/u011784767/article/details/51439327#)
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- A : 10
- B : 10
- B : 10
- B : 10
- B : 10
- B : 10
- B : 10
- B : 10
- B : 10
- B : 10
- B : 10
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
A : 10
B : 10
B : 10
B : 10
B : 10
B : 10
B : 10
B : 10
B : 10
B : 10
B : 10
```
输出结果是显式锁很好的保护了 writeNumMethod 的操作，假设线程A在执行结束前被剥夺了运行权，这是线程B调用readNumMethod方法去读取num的值，但是由于线程B不能获得锁，线程B必须等待线程A释放锁才能执行readNumMethod方法。这样就保证了writeNumMethod方法的原子性，也就保证了共享变量num不会出现讹误的现象。
**(2)ReentrantLock是可重入锁**
ReentrantLock持有一个所计数器，当已持有所的线程再次获得该锁时计数器值加1，每调用一次lock.unlock()时所计数器值减一，直到所计数器值为0，此时线程释放锁。
我们测试以下代码：
**[java]**[view plain](http://blog.csdn.net/u011784767/article/details/51439327#)[copy](http://blog.csdn.net/u011784767/article/details/51439327#)[print](http://blog.csdn.net/u011784767/article/details/51439327#)[?](http://blog.csdn.net/u011784767/article/details/51439327#)
- publicclass ReentrantLockTest2 {  
- 
- private ReentrantLock lock = new ReentrantLock();  
- 
- publicvoid testReentrantLock() {  
- // 线程获得锁
-         lock.lock();  
- try {  
-             System.out.println(Thread.currentThread().getName() + " get lock");  
- long beginTime = System.currentTimeMillis();  
- while(System.currentTimeMillis() - beginTime < 100){}  
-             lock.lock();  
- try {  
-                 System.out.println(Thread.currentThread().getName() + " get lock again");  
- long beginTime2 = System.currentTimeMillis();  
- while(System.currentTimeMillis() - beginTime2 < 100){}  
-             }finally {  
- // 线程释放锁
-                 lock.unlock();  
-                 System.out.println(Thread.currentThread().getName() + " release lock");  
-             }  
-         } finally {  
- // 线程释放锁
-             lock.unlock();  
-             System.out.println(Thread.currentThread().getName() + " release lock again");  
-         }  
-     }  
- publicstaticvoid main(String[] args) {  
- // TODO Auto-generated method stub
- final ReentrantLockTest2 test2 = new ReentrantLockTest2();  
-         Thread thread = new Thread(new Runnable(){  
- publicvoid run() {  
-                 test2.testReentrantLock();  
-             }  
-         },"A");  
-         thread.start();  
-     }  
- 
- }  
- 
- 输出结果：  
- A get lock  
- A get lock again  
- A release lock  
- A release lock again  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class ReentrantLockTest2 {
	
	private ReentrantLock lock = new ReentrantLock();
	
	public void testReentrantLock() {
		// 线程获得锁
		lock.lock();
		try {
			System.out.println(Thread.currentThread().getName() + " get lock");
			long beginTime = System.currentTimeMillis();
			while(System.currentTimeMillis() - beginTime < 100){}
			lock.lock();
			try {
				System.out.println(Thread.currentThread().getName() + " get lock again");
				long beginTime2 = System.currentTimeMillis();
				while(System.currentTimeMillis() - beginTime2 < 100){}
			}finally {
				// 线程释放锁
				lock.unlock();
				System.out.println(Thread.currentThread().getName() + " release lock");
			}
		} finally {
			// 线程释放锁
			lock.unlock();
			System.out.println(Thread.currentThread().getName() + " release lock again");
		}
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final ReentrantLockTest2 test2 = new ReentrantLockTest2();
		Thread thread = new Thread(new Runnable(){
			public void run() {
				test2.testReentrantLock();
			}
		},"A");
		thread.start();
	}
}
输出结果：
A get lock
A get lock again
A release lock
A release lock again
```
