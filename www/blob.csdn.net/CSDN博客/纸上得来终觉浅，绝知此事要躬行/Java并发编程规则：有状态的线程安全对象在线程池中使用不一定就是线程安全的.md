# Java并发编程规则：有状态的线程安全对象在线程池中使用不一定就是线程安全的 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月22日 17:33:36[boonya](https://me.csdn.net/boonya)阅读数：842
所属专栏：[Java并发编程实践](https://blog.csdn.net/column/details/13974.html)









如题：Java并发编程规则>有状态的线程安全对象在线程池中使用不一定就是线程安全的。在线程安全里面我们可以用的几个关键字是：volatile、Synchronized，另外状态变量我们可以使用原子变量。如果我们在编写了一个状态安全的线程类后，却不慎地使用非线程安全的线程池，那么也会导致线程不安全问题产生。

## 安全基本概述

**volatile的使用要求满足下面的两个条件：**


1）对变量或者引用的写操作不依赖于变量或者引用的当前值(如果只有特定的单个线程修改共享变量，那么修改操作也是可以依赖于当前值)；


2）该变量或者引用没有包含在其它的不变式条件中；


volatile最常见的错误使用场景是使用volatile来实现并发 i++; 错误的原因是，该操作依赖于 i 变量的当前值，他是在 i 变量的当前值的基础上加一，所以说他依赖于 i 的当前值。多个线程执行 i++; 会丢失更新。比如两个线程同时读到 i 的当前值8，都进行加一，然后写回，最终 i 的结果是 9，而不是我们期待的10，丢失了更新。那么原子变量的引入就是针对volatile的这个缺陷的！！！原子变量的修改操作允许它依赖于当前值，所以说”原子变量“是比volatile的语义稍微强化一点！他不仅具有volatile的可见性，同时对原子变量。

**从Java 1.5开始引入了原子变量和原子引用：**




java.util.concurrent.atomic.AtomicBoolean

java.util.concurrent.atomic.AtomicInteger

java.util.concurrent.atomic.AtomicLong

java.util.concurrent.atomic.AtomicReference

**以及他们对应的数组：**

java.util.concurrent.atomic.AtomicIntegerArray

java.util.concurrent.atomic.AtomicLongArray

java.util.concurrent.atomic.AtomicReferenceArray

注：synchronized关键字用的比较多，这里就不说明了。


## 编写一个状态安全的类



```java
package com.boonya.thread;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
/**
 * 状态安全线程
 * 
 * @package com.boonya.thread.StateSafeThread
 * @date   2017年3月22日  下午4:28:57
 * @author pengjunlin
 * @comment   
 * @update
 */
public class StateSafeThread extends Thread {

	private static AtomicInteger atomicCount = new AtomicInteger(0);// 多线程：线程安全的

	private volatile static AtomicInteger volatileAtomicCount = new AtomicInteger(0);// 多线程：线程安全的
	
	
	@Override
	public void run() {
		final Object lock = new Object();  
		for (int i = 0; i < 50000; i++) {  
            new Thread(new Runnable() {  
                  
                @Override  
                public void run() {  
                   synchronized (lock) {
                	   // Nothing....
				   }
                   atomicCount.incrementAndGet();  
                   volatileAtomicCount.incrementAndGet();  
                }  
            }).start();  
        }  
		// 休息5秒, 保证线程中的计算完成  
        try {  
            TimeUnit.SECONDS.sleep(5);  //关键
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
        System.out.println("Thread Name=====:"+this.getName());  
        System.out.println("线程并发执行对计数器累计5000次，看并发结果！");  
        System.out.println("atomicCount=" + atomicCount.get());  
        System.out.println("volatileAtomicCount=" + volatileAtomicCount.get());
	}

}
```


## 编写一个线程池测试类






```java
package com.boonya.thread;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import org.junit.Test;
/**
 * 状态安全线程测试类
 * 
 * @package com.boonya.thread.StateSafeThreadTest
 * @date   2017年3月22日  下午4:41:37
 * @author pengjunlin
 * @comment   
 * @update
 */
public class StateSafeThreadTest {
	
	/**
	 * 线程安全的
	 * 
	 * @MethodName: testByOneThread 
	 * @Description: 
	 * @throws
	 */
	@Test
	public void testByOneThread(){
		
		for (int i = 0; i < 5; i++) {
			Thread thread=new StateSafeThread();
			thread.start();
		}
		
		try {
			Thread.sleep(30000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * 线程安全的
	 * 
	 * @MethodName: testByThreadPool 
	 * @Description: 
	 * @throws
	 */
	@Test
	public void testByThreadPool(){
		Thread thread=null;
		ExecutorService executorService=Executors.newFixedThreadPool(5);// 线程个数大于或等于线程个数时线程安全
		
		for (int i = 0; i < 5; i++) {
			thread=new StateSafeThread();
			executorService.execute(thread);
		}
		/*Future future=executorService.submit(thread);
		
		try {
			System.out.println(future.get());
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (ExecutionException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}*/
		
		try {
			executorService.awaitTermination(30000, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}finally{
			executorService.shutdown();
		}
		
	}
	
	/**
	 * 非线程安全的
	 * 
	 * @MethodName: testByThreadPool2 
	 * @Description: 
	 * @throws
	 */
	@Test
	public void testByThreadPool2(){
		Thread thread=null;
		ExecutorService executorService=Executors.newFixedThreadPool(3);// 小于线程个数导致线程不安全
		
		
		for (int i = 0; i < 5; i++) {
			thread=new StateSafeThread();
			executorService.execute(thread);
		}
		/*Future future=executorService.submit(thread);
		
		try {
			System.out.println(future.get());
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (ExecutionException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}*/

		try {
			executorService.awaitTermination(30000, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}finally{
			executorService.shutdown();
		}
		
	}
	
	/**
	 * 线程安全的
	 * 
	 * @MethodName: testByThreadPool3
	 * @Description: 
	 * @throws
	 */
	@Test
	public void testByThreadPool3(){
		Thread thread=null;
		ExecutorService executorService=Executors.newCachedThreadPool();
		
		
		for (int i = 0; i < 5; i++) {
			thread=new StateSafeThread();
			executorService.execute(thread);
		}
		/*Future future=executorService.submit(thread);
		
		try {
			System.out.println(future.get());
		} catch (InterruptedException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (ExecutionException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}*/

		try {
			executorService.awaitTermination(30000, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}finally{
			executorService.shutdown();
		}
		
	}

}
```


注：可以测试运行查看结果，推荐使用缓存线程池。









