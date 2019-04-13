
# Java实用方法整理（二十六）——线程安全 - manduner的博客 - CSDN博客


2019年01月23日 21:38:26[Manduner_TJU](https://me.csdn.net/manduner)阅读数：39


# 1，线程安全问题产生原因
多线程环境
共享数据
多条语句操作共享数据
# 2，线程安全问题解决方案
## （1）方案一：同步代码块
1）特征描述
格式：synchronized(对象){需要同步的代码}
同步可以解决安全问题的根本原因就在对象上，该对象如同锁的功能。该对象为任意对象（除同步方法，它的对象为this）
同步方案的优点：可解决多线程的安全问题
同步方案的缺点：当线程相当多时，每个线程都会去判断同步上的锁，这样会耗费资源，降低程序的运行效率
2）简单案例：模拟多个窗口销售火车票的问题
创建线程类，利用同步代码块的方案解决线程安全问题
```python
package manduner_03;
/**
* @author Manduner_TJU
* @version 创建时间：2019年1月23日下午9:14:00
*/
public class SellTickets implements Runnable {
	
	//定义共享数据
	int tickets = 100;
	//定义共享对象（锁）
	Object obj = new Object();
	
	@Override
	public void run() {
		while(true) {
			//把多个线程操作共享数据的代码放到同步代码块中
			synchronized(obj) {
				if(tickets > 0) {
					try {
						Thread.sleep(100); //模拟网络延迟问题
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					System.out.println(Thread.currentThread().getName()+"正在销售第" 
					                   + tickets-- +"张票");
				}
			}
		}
	}
}
```
启动多线程
```python
package manduner_03;
/**
* @author Manduner_TJU
* @version 创建时间：2019年1月23日下午9:22:54
*/
public class SellTicketsDemo {
	public static void main(String[] args) {
		//创建资源对象
		SellTickets st = new SellTickets();
		
		//创建售票窗口（多线程）
		Thread t1 = new Thread(st, "窗口1");
		Thread t2 = new Thread(st, "窗口2");
		Thread t3 = new Thread(st, "窗口3");
		
		//开始售票(启动多线程)
		t1.start();
		t2.start();
		t3.start();
	}
}
```
## （2）Lock锁（待更新）


