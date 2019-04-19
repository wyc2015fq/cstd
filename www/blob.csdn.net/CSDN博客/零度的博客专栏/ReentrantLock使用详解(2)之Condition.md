# ReentrantLock使用详解(2)之Condition - 零度的博客专栏 - CSDN博客
2017年04月12日 10:15:52[零度anngle](https://me.csdn.net/zmx729618)阅读数：366
(1) ReentrantLock的条件对象
通常，线程进入临界区，却发现在某一条件满足之后才能执行，条件对象就是用来管理那些已经获得了锁，但是却不能做有用工作的线程。
一个锁对象可以有一个或多个相关的条件对象，我们可用lock.newCondition()方法获得一个条件对象。
**[java]**[view plain](http://blog.csdn.net/u011784767/article/details/51656697#)[copy](http://blog.csdn.net/u011784767/article/details/51656697#)[print](http://blog.csdn.net/u011784767/article/details/51656697#)[?](http://blog.csdn.net/u011784767/article/details/51656697#)
- ReentrantLock myLock = new ReentrantLock();  
- // 获得锁myLock的一个条件对象
- Condition condition = myLock.newCondition();  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
ReentrantLock myLock = new ReentrantLock();
// 获得锁myLock的一个条件对象
Condition condition = myLock.newCondition();
```
下面我们直接通过代码来学习条件对象的使用。
以下代码实现的是简单的生产者和消费者案例：
**[java]**[view plain](http://blog.csdn.net/u011784767/article/details/51656697#)[copy](http://blog.csdn.net/u011784767/article/details/51656697#)[print](http://blog.csdn.net/u011784767/article/details/51656697#)[?](http://blog.csdn.net/u011784767/article/details/51656697#)
- publicclass ReentrantLockTest3 {  
- 
- private ReentrantLock myLock = new ReentrantLock();  
- 
- private Condition condition = myLock.newCondition();  
- 
- private List<Integer> listBuffer = new ArrayList<Integer>();  
- 
- privatevolatileboolean runFlag = true;  
- 
- /**
-      * 生产者 生产数据
-      */
- publicvoid produce() {  
- int i = 0;  
- while(runFlag) {  
-             myLock.lock();  
- try {  
- // 生产者检查容器中是否有数据，如果容器中有数据则生产者等待
- // 如果容器中没有数据则生产数据放入容器中并通知消费者
- if (listBuffer.size() > 0) {   
- try {  
- // 调用await()方法，生产者线程阻塞并释放锁，之后进入该条件的等待集中
- // 直到消费者调用signalAll()方法之后，生产者线程解除阻塞并重新竞争锁
- // 生产者线程获得锁之后，重新开始从被阻塞的地方继续执行程序
-                         condition.await();  
-                     } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-                         e.printStackTrace();  
-                     }  
-                 } else {  
-                     System.out.println(Thread.currentThread().getName() + " add Integer");  
-                     listBuffer.add(i++);  
- // 生产者线程调用signalAll()方法，通知消费者线程容器中有数据
-                     condition.signalAll();  
-                 }  
-             } finally {  
-                 myLock.unlock();  
-             }             
-         }  
-     }  
- 
- /**
-      * 消费者 读取数据
-      */
- publicvoid consume() {  
- while(runFlag) {  
-             myLock.lock();  
- try {  
- // 消费者检查容器中是否有数据，如果没有数据消费者等待
- // 如果容器中有数据则读取数据，读完之后通知生产者
- if (listBuffer.size() == 0) {  
- try {  
- // 同生产者线程一样，消费者线程调用await()方法阻塞并进入该条件等待集中
-                         condition.await();  
-                     } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-                         e.printStackTrace();  
-                     }  
-                 } else {  
-                     System.out.println(Thread.currentThread().getName() + " get Integer");  
- long beginTime = 0;  
-                     System.out.println(listBuffer.remove(0));  
-                     beginTime = System.currentTimeMillis();  
- while(System.currentTimeMillis() - beginTime < 100) {}  
- // 消费者线程调用signalAll()方法，通知生产者生产数据
-                     condition.signalAll();  
-                 }  
-             } finally {  
-                 myLock.unlock();  
-             }         
-         }         
-     }  
- 
- 
- 
- publicboolean isRunFlag() {  
- return runFlag;  
-     }  
- 
- publicvoid setRunFlag(boolean runFlag) {  
- this.runFlag = runFlag;  
-     }  
- 
- publicstaticvoid main(String[] args) {  
- // TODO Auto-generated method stub
- final ReentrantLockTest3 test = new ReentrantLockTest3();  
- 
-         Thread produce = new Thread(new Runnable() {  
- publicvoid run() {  
-                 test.produce();  
-             }  
-         },"A");  
- 
-         Thread consume = new Thread(new Runnable() {  
- publicvoid run() {  
-                 test.consume();  
-             }  
-         },"B");  
- 
-         produce.start();  
-         consume.start();  
- 
- try {  
-             Thread.sleep(5000);  
-         } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
-         test.setRunFlag(false);  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class ReentrantLockTest3 {
	private ReentrantLock myLock = new ReentrantLock();
	
	private Condition condition = myLock.newCondition();
	
	private List<Integer> listBuffer = new ArrayList<Integer>();
	
	private volatile boolean runFlag = true;
	
	/**
	 * 生产者 生产数据
	 */
	public void produce() {
		int i = 0;
		while(runFlag) {
			myLock.lock();
			try {
				// 生产者检查容器中是否有数据，如果容器中有数据则生产者等待
				// 如果容器中没有数据则生产数据放入容器中并通知消费者
				if (listBuffer.size() > 0) {	
					try {
						// 调用await()方法，生产者线程阻塞并释放锁，之后进入该条件的等待集中
						// 直到消费者调用signalAll()方法之后，生产者线程解除阻塞并重新竞争锁
						// 生产者线程获得锁之后，重新开始从被阻塞的地方继续执行程序
						condition.await();
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				} else {
					System.out.println(Thread.currentThread().getName() + " add Integer");
					listBuffer.add(i++);
					// 生产者线程调用signalAll()方法，通知消费者线程容器中有数据
					condition.signalAll();
				}
			} finally {
				myLock.unlock();
			}			
		}
	}
	
	/**
	 * 消费者 读取数据
	 */
	public void consume() {
		while(runFlag) {
			myLock.lock();
			try {
				// 消费者检查容器中是否有数据，如果没有数据消费者等待
				// 如果容器中有数据则读取数据，读完之后通知生产者
				if (listBuffer.size() == 0) {
					try {
						// 同生产者线程一样，消费者线程调用await()方法阻塞并进入该条件等待集中
						condition.await();
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				} else {
					System.out.println(Thread.currentThread().getName() + " get Integer");
					long beginTime = 0;
					System.out.println(listBuffer.remove(0));
					beginTime = System.currentTimeMillis();
					while(System.currentTimeMillis() - beginTime < 100) {}
					// 消费者线程调用signalAll()方法，通知生产者生产数据
					condition.signalAll();
				}
			} finally {
				myLock.unlock();
			}		
		}		
	}
	
	
	
	public boolean isRunFlag() {
		return runFlag;
	}
	public void setRunFlag(boolean runFlag) {
		this.runFlag = runFlag;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final ReentrantLockTest3 test = new ReentrantLockTest3();
		
		Thread produce = new Thread(new Runnable() {
			public void run() {
				test.produce();
			}
		},"A");
		
		Thread consume = new Thread(new Runnable() {
			public void run() {
				test.consume();
			}
		},"B");
		
		produce.start();
		consume.start();
		
		try {
			Thread.sleep(5000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		test.setRunFlag(false);
	}
}
```
condition.await()：调用该方法的线程会被阻塞，同时释放持有的锁对象，之后线程进入该条件的等待集中。注意等待获得锁的线程和调用await()进入等待状态的线程不同。
等待获得锁的线程：如果锁可用的话，当线程获得cpu时间片后线程立即解除阻塞状态
调用await()进入等待状态的线程：当锁可用时，该线程不能立即解除阻塞状态，相反它一直处于阻塞状态，直到另一个线程调用同一条件上的signalAll()方法，它才解除阻塞状态。
condition.signalAll()：该方法重新激活因为conditon这一条件而等待的所有线程。当这些线程从等待集中移出时，他们再次成为可运行的，调度器将再次激活它们，同时它们将试图重新进入该对象。一旦锁成为可用的，它们中的某个将从await（）调用返回，获得锁并从被阻塞的地方继续执行。
注意：signalAll()方法不会立即激活一个等待的线程。它仅仅解除等待的线程阻塞，以便这些线程可以在当前线程退出同步方法之后，通过竞争实现对对象的访问。
**避免死锁的问题：**
当一个线程调用await()方法时，它没有办法重新激活自己进入可运行状态，它寄希望于其他线程调用signalAll()方法或signal()方法。如果没有其他线程来重新激活等待的线程，调用await()方法的线程就永远不会再运行了，这样就会导致“死锁”。
目前死锁问题 没有什么有效的机制来避免，只能通过我们谨慎的调用await()和signal()\singalAll() 方法。
