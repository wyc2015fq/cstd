# lockInterruptibly 和lock的区别 - z69183787的专栏 - CSDN博客
2015年09月13日 21:01:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1531
lockInterruptibly 与 lock比较区别在于 
lockInterruptibly 优先考虑响应中断，而不是响应锁定的普通获取或重入获取 
Java代码  ![收藏代码](http://lzmhehe.iteye.com/images/icon_star.png)
- package test;  
- 
- import java.util.concurrent.TimeUnit;  
- import java.util.concurrent.locks.Lock;  
- import java.util.concurrent.locks.ReentrantLock;  
- 
- publicclass TTTT {  
- publicstaticvoid main(String[] args){  
- 
-         Thread i1 = new Thread(new RunIt3());  
-         Thread i2 = new Thread(new RunIt3());  
-         i1.start();  
-         i2.start();  
-         i2.interrupt();  
-     }  
- 
- }  
- 
- class RunIt3 implements Runnable{  
- 
- privatestatic Lock lock = new ReentrantLock();  
- publicvoid run(){  
- try{  
- //---------------------------------a
-             lock.lock();  
- //lock.lockInterruptibly(); 
- 
- 
-             System.out.println(Thread.currentThread().getName() + " running");  
-             TimeUnit.SECONDS.sleep(20);  
-             lock.unlock();  
-             System.out.println(Thread.currentThread().getName() + " finished");  
-         }  
- catch (InterruptedException e){  
-             System.out.println(Thread.currentThread().getName() + " interrupted");  
- 
-         }  
- 
-     }  
- }  
如果a处 是lock.lock(); 
输出 
Thread-0 running 
(这里休眠了20s) 
Thread-0 finished 
Thread-1 running 
Thread-1 interrupted 
============================ 
如果a处是lock.lockInterruptibly() 
Thread-0 running 
Thread-1 interrupted 
(这里休眠了20s) 
Thread-0 finished 
========================= 
如果a处是lock.tryLock(); 
Thread-1 running 
Thread-0 running 
(这里休眠了20s) 
Thread-1 finished 
Exception in thread "Thread-0" java.lang.IllegalMonitorStateException 
at java.util.concurrent.locks.ReentrantLock$Sync.tryRelease(Unknown Source) 
at java.util.concurrent.locks.AbstractQueuedSynchronizer.release(Unknown Source) 
at java.util.concurrent.locks.ReentrantLock.unlock(Unknown Source) 
at test.RunIt3.run(TTTT.java:32) 
at java.lang.Thread.run(Unknown Source) 
========================= 
如果a处是lock.tryLock(10,TimeUnit.SECONDS); 
Thread-0 running 
(这里休眠了10s) 
Thread-1 running 
(这里休眠了10s) 
Thread-0 finished 
(这里休眠了10s) 
Exception in thread "Thread-1" java.lang.IllegalMonitorStateException 
at java.util.concurrent.locks.ReentrantLock$Sync.tryRelease(Unknown Source) 
at java.util.concurrent.locks.AbstractQueuedSynchronizer.release(Unknown Source) 
at java.util.concurrent.locks.ReentrantLock.unlock(Unknown Source) 
at test.RunIt3.run(TTTT.java:32) 
at java.lang.Thread.run(Unknown Source) 
转自：[http://lzmhehe.iteye.com/blog/402053](http://lzmhehe.iteye.com/blog/402053)
