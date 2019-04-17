# 生产者和消费者问题【java等待通知机制实现】 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月09日 18:02:17[QuJack](https://me.csdn.net/A784586)阅读数：430








**           生产者和消费者问题【等待通知机制实现】**

1.生产者和消费者问题是经典的问题，这里利用等待通知机制实现：wait(),notify()

重复下多线程问题的一般框架：



多线程的程序一般框架：

1.**一个服务类A**，用于生成对象a，a中包含多个的同步方法或者同步块；a.method1(); a.mothod2();

2.**多个线程类**：线程类ThreadA，线程类ThreadB；含有一个A类型的私有变量,构造函数中对其赋值；

Run中调用a中的同步方法；

3.**主程序**：将类A的对象a通过构造函数给ThreadA/ThreadB的A类型的成员，启动线程。




首先实现**一个生产者和一个消费者**：**操作值**的方式实现(操作栈也可以实现)：



第一个java类是：**ValueObject 工具类**，里面包含一个公开的静态变量，供消费者和生产者共享；



```java
public class ValueObject {
	public static String value="";
}
```


第二个是一个**生产者类**：包括同步块生产过程。





```java
package com.mytest.pc;

//生产者
public class Production {
	private String lock;
	public Production(String lock){
		super();
		this.lock=lock;
	}
	public void setValue(){
		synchronized (lock) {
			if(!ValueObject.value.equals("")){//不为空值；则等待后生产；
				try {
					lock.wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			//空，则可以生产；
			String valueString=System.currentTimeMillis()+"_"
			+System.nanoTime();
			System.out.println("set的值为："+valueString);
			ValueObject.value=valueString;
			lock.notify();
		}
		
	}
}
```


下面是**消费者**的java类：包括同步消费块；





```java
package com.mytest.pc;

//生产者
public class Consumer {
	private String lock;
	public Consumer(String lock){
		super();
		this.lock=lock;
	}
	public void getValue(){
		synchronized (lock) {
			if(ValueObject.value.equals("")){//为空值；则等待后消费；
				try {
					lock.wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			//不空，则可以消费；
			System.out.println("get值为："+ValueObject.value);
			ValueObject.value="";
			lock.notify();
		}
		
	}
}
```
下面是**生产者的线程类**Java类：生产方法

```java
package com.mytest.pc;

public class ThreadP extends Thread {
	private Production p;
	
	public ThreadP(Production p){
		super();
		this.p=p;
	}
	
	@Override
	public void run() {
		while(true){
			p.setValue();
		}
	}

}
```

下面是**消费者的线程类**Java类：消费方法；





```java
package com.mytest.pc;

public class ThreadC extends Thread {
	private Consumer c;
	
	public ThreadC(Consumer c){
		super();
		this.c=c;
	}
	
	@Override
	public void run() {
		while(true){
			c.getValue();
		}
	}

}
```
下面是**主程序测试类**：




```java
package com.mytest.pc;

public class P1_C1_test {
	public static void main(String[] args) {
		String lock="";
		Production production=new Production(lock);
		Consumer consumer=new Consumer(lock);
		
		ThreadP p=new ThreadP(production);
		ThreadC c=new ThreadC(consumer);
		p.start();
		c.start();
		//ThreadP p2=new ThreadP(production);
		//ThreadC c2=new ThreadC(consumer);
		//p2.start();
		//c2.start();

	}

}
```

程序测试结果如下：程序运行正确，不停的生产和消费，而且生产和消费的值一致。


set的值为：1491730702480_2881014140368

get值为：1491730702480_2881014140368

set的值为：1491730702480_2881014157414

get值为：1491730702480_2881014157414

set的值为：1491730702480_2881014173910

get值为：1491730702480_2881014173910





改写为多个生产者+1个消费者；n-1;

           1个生产者+多个消费者；1-n;

           多个生产者+多个消费者。n-n;

测试办法：

1.将测试主函数中添加如下代码;

ThreadP p2=new ThreadP(production);
ThreadC c2=new ThreadC(consumer);
p2.start();
c2.start();


              2.把类生产者Production和消费者类Consumer中的**if改为while.**

             3. 运行效果: 程序会结束，但中途停止打印输出。

程序都会出现**假死状态**：

**假死**，就是线程进入了waiting状态。

**原因**：生产者唤醒的可能是生产者，消费者唤醒的可能是消费者。不能保证notify()唤醒的是异类。

**解决办法**：将notify()改为notifyAll();




多线程中经常遇到的两个问题：

1.通过过早问题：通过发生了，wait不必再执行，而已在通知后设置个flag，根据flag判断有无必要再执行wait;

2.等待wait的条件发生了变化：if判断改为while();




备注：可以通过操作栈实现生产者和消费者问题。













