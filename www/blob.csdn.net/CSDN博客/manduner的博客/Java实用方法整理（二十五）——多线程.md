
# Java实用方法整理（二十五）——多线程 - manduner的博客 - CSDN博客


2019年01月07日 21:07:11[Manduner_TJU](https://me.csdn.net/manduner)阅读数：44


# 1，线程与进程
## （1）概念
进程：简单理解，就是上正在执行的应用程序，例如：QQ，微信，英雄联盟。。。
线程：进程的执行单元，也可以理解成执行路径
单线程：一个应用程序只有一条执行路径
多线程：一个应用程序有多条执行路径
## （2）意义
多进程的意义：提高CPU的使用率
多线程的意义：提高应用程序的使用率
# 2，线程的生命周期
## （1）线程的生命周期分为以下5个阶段：
新建：创建线程对象
就绪：有执行资格，没有执行权
运行：有执行资格，有执行权
阻塞：由于一些操作让线程处于该状态。在阻塞状态下，线程没有执行资格，没有执行权；而另一些操作可以再把它激活，激活后的线程处于就绪状态
死亡：线程对象变成垃圾，等待被回收
## （2）下面给出线程生命周期的图解
![](https://img-blog.csdnimg.cn/20190122210840648.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy,size_16,color_FFFFFF,t_70)
# 3，线程的调度和优先级问题
## （1）线程的调度
分时调度
抢占式调度（java采用的是该调度方式）
## （2）优先级
默认的优先级是5
优先级范围是【1-10】
优先级越高，线程越被优先执行
# 4，常用的线程操作方法
线程休眠：public static void sleep(long millis)
线程加入：public final void join()
线程礼让：public static void yield()
线程中断：（1）public final void stop();  （2）public void interrupt()
# 5，多线程的实现方案
## （1）继承Thread类
创建MyThread类，并继承Thread类；
```python
package manduner_01;
/**
* @author Manduner_TJU
* @version 创建时间：2019年1月4日下午11:04:48
*/
public class MyThread extends Thread{
	//1,重写run()方法，将需要多线程执行的代码（方法），放到run方法体内
	@Override
	public void run() {
		for (int x=0; x<=100;x++) {
			//2,getName(),获取线程名称，名称默认的是“Thread-n”的形式
			System.out.println(getName() + ":" + x);
		}
	}
}
```
开启多线程
```python
package manduner_01;
/**
* @author Manduner_TJU
* @version 创建时间：2019年1月7日下午7:56:27
*/
public class MyThreadDemo {
	public static void main(String[] args) {
		MyThread my1 = new MyThread();
		MyThread my2 = new MyThread();
		//1,setName(), 设置线程名称
		my1.setName("线程-01");
		my2.setName("线程-02");
		
		my1.start();
		my2.start();
		
		//获取main方法所在的线程对象名称
		System.out.println("主线程对象名称：" + Thread.currentThread().getName());
	}
}
```
## （2）实现Runnable接口
该方法能够实现代码和数据分离，所以常用此方法实现多线程。简单实现以下案例：模拟多个窗口售火车票
创建线程类
```python
package manduner_02;
/**
* @author Manduner_TJU
* @version 创建时间：2019年1月22日下午9:32:37
*/
public class SellTicket implements Runnable{
	//定义100张票
	private int tickets = 100;
	@Override
	public void run() {
		while(true) {
			if(tickets>0) {
				System.out.println(Thread.currentThread().getName()+"正在出售第"+(tickets--)+"张票");
			}
		}
	}
}
```
启动线程
```python
package manduner_02;
/**
* @author Manduner_TJU
* @version 创建时间：2019年1月22日下午9:44:01
*/
public class SellTicketDemo {
	public static void main(String[] args) {
		// 创建资源对象
		SellTicket st = new SellTicket();
		
		//创建三个线程对象
		Thread t1 = new Thread(st, "窗口1");
		Thread t2 = new Thread(st, "窗口2");
		Thread t3 = new Thread(st, "窗口3");
		
		//启动线程
		t1.start();
		t2.start();
		t3.start();
				
	}
}
```


