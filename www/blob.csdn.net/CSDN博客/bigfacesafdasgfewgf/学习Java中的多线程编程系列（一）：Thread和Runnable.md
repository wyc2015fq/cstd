# 学习Java中的多线程编程系列（一）：Thread和Runnable - bigfacesafdasgfewgf - CSDN博客





2015年03月12日 17:29:13[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：901标签：[多线程																[thread																[runnable																[线程赛跑																[派生重写](https://so.csdn.net/so/search/s.do?q=派生重写&t=blog)
个人分类：[Java](https://blog.csdn.net/puqutogether/article/category/2595127)










    Java编程时，为了提高程序运行的效率和处理速度，可以在一个程序中同时运行多个线程。




    关于多线程有下面几个特性：

1）Java程序可同时并行运行多个相对独立的线程。例如创建一个线程来接收数据，另一个线程发送数据。

2）线程是相互独立，任何线程都不能访问除自身之外的其他线程的局部变量。

3）如果两个线程同时访问同一个方法，那每个线程将各自得到此方法的一个拷贝。




    Java中线程的实现通常有两种方式：派生Thread类，并覆盖run方法；通过Runnable接口。




    我们平时编程都是单线程，一个程序就是一个线程上运行。如下程序：



```java
package testing;
/***
 * 
 * @author Hadoop
 * 
 * Implementation in only one thread
 *
 */
public class OnlyThread {
	/*
	 * 单线程版本
	 */

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		run(); 
	}
	
	public static void run() {
		/*
		 * 输出直角三角形状 '*'
		 */
		for(int count = 1, row = 1; row < 10; row++, count++) {
			for(int i = 0; i < count; i++) {
				System.out.print('*'); 
			}
			System.out.println();
		}
	}

}
```



    学习了多线程编程之后，我们就可以通过派生Thread的方式或者Runnable接口的方式来操作。




**方式一：派生Thread类**




1）创建一个新的线程类，继承Thread类并覆盖Thread类的run()方法；

2）创建一个线程类的对象，创建方法与一般对象的创建相同，使用关键字new完成；

3）启动新线程对象，调用start()方法；

4）线程自己调用run()方法。




下面两个程序就是用派生Thread类的方式来实现多线程：



```java
package testing;

public class ThreadDemo1 extends Thread{  // 使用派生Thread类的方法实现多线程，
	//其中Thread类是一个通用的线程类：java.lang.Thread 可以不import

	/**
	 * 
	 * @param args
	 * 
	 * MAIN CLASS  单线程版本
	 */
	
	ThreadDemo1() {  // 无餐构造方法
		
	}
	ThreadDemo1(String szName) {  //带参构造方法
		super(szName);  // 调用父类的构造方法
	}
	
	public void run() {  // 覆盖Thread类中的run方法，注意不是重载
		for(int count = 1, row = 1; row < 10; row++, count++) {
			for(int i = 0; i < count; i++) {
				System.out.print('*'); 
			}
			System.out.println();
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		ThreadDemo1 td = new ThreadDemo1();   // 对象
		td.start();
	}

}
```




运行结果：

*

**

***

****

*****

******

*******

********

*********



```java
package testing;

public class ThreadDemo2 extends Thread{
	
	ThreadDemo2() {  // 无餐构造方法
		
	}
	ThreadDemo2(String szName) {  //带参构造方法
		super(szName);  // 调用父类的构造方法
	}
	
	public void run() {  // 覆盖Thread类中的run方法，注意不是重载
		for(int count = 1, row = 1; row < 10; row++, count++) {
			for(int i = 0; i < count; i++) {
				System.out.print('*'); 
			}
			System.out.println();
		}
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		ThreadDemo2 td1 = new ThreadDemo2(); 
		ThreadDemo2 td2 = new ThreadDemo2(); 
		ThreadDemo2 td3 = new ThreadDemo2();
		td1.start();
		td2.start();
		td3.start();
	}

}
```


运行结果：

*

**

***

****

*****

******

*******

********

*********

*

**

***

****

*****

**

**

***

****

*****

******

*******

********

*********

*****

*******

********

*********





第二段程序的结果表明：线程并没有按照程序中调用的顺序来执行，而是产生了多个线程赛跑现象。




**方式二：Runnable接口**




1）创建一个实现Runnable接口的类，并且在这个类中重写run方法；


2）使用关键字new新建一个ThreadType的实例；

3）通过Runnable的实例创建一个线程对象，在创建线程对象时，调用的构造函数是new Thread(ThreadType)，它用ThreadType中实现的run()方法作为新线程对象的run()方法；

4）通过调用ThreadType对象的start()方法启动线程运行。




程序如下：



```java
package testing;

public class ThreadDemo3 implements Runnable{  // 用Runnable接口的方式实现多线程

	public void run() {  // 重载run函数，这里是重载
		for(int count = 1, row = 1; row < 10; row++, count++) {
			for(int i = 0; i < count; i++) {
				System.out.print('*'); 
			}
			System.out.println();
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		Runnable rb1 = new ThreadDemo3();   // 对象
		Runnable rb2 = new ThreadDemo3(); 
		Runnable rb3 = new ThreadDemo3(); 
		
		Thread td1 = new Thread(rb1);   // 还要回到Thread类的调用中，只不过上面不再需要派生Thread类了
		Thread td2 = new Thread(rb2); 
		Thread td3 = new Thread(rb3); 
		
		td1.start();
		td2.start();
		td3.start();
	}

}
```



需要注意的是，方式二也会出现线程赛跑的现象。




参考：

《Java从入门到精通》](https://so.csdn.net/so/search/s.do?q=线程赛跑&t=blog)](https://so.csdn.net/so/search/s.do?q=runnable&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)




