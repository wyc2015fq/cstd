# Java线程学习经典例子-读写者演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年02月14日 00:24:07[gloomyfish](https://me.csdn.net/jia20003)阅读数：2638








Java线程学习经典例子-读写者演示

Java线程学习最经典的例子-读写者，主要用到Thread相关知识如下：

-         线程的start与run

-         线程的休眠(sleep)

-         数据对象加锁(synchronized)

-         数据对象的等待与释放(wait and notify)

程序实现：

-ObjectData数据类对象，通过synchronized关键字实现加锁，在线程读写者中使用。

-ConsumerThread消费者线程，读取数据对象中count值之后，通知生产者线程

-ProductThread生产者线程，对数据对象中count值操作，每次加1，然后通知消费者线程

类结构图如下：

![](https://img-blog.csdn.net/20160214001250548)


**代码实现**

**消费者-读线程**



```java
package com.gloomyfish.jse.thirdteen;

public class ConsumerThread extends Thread {
	private ObjectData data;
	public ConsumerThread(ObjectData data) {
		this.data = data;
	}

	@Override
	public void run() {
		while(true) {
			try {
				synchronized (data) {
					data.wait();
					data.read();
					data.notify();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
```
**写线程-生产者线程**




```java
package com.gloomyfish.jse.thirdteen;

public class ProductThread extends Thread {
	private ObjectData data;

	public ProductThread(ObjectData data) {
		this.data = data;
	}

	@Override
	public void run() {
		while (true) {
			try {
				synchronized (data) {
					data.write();
					Thread.sleep(3000);
					data.notify();
					data.wait();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
```
**数据对象类**




```java
package com.gloomyfish.jse.thirdteen;

public class ObjectData {

	private int count;
	
	public ObjectData() {
		count = 0;
	}
	
	public void read() {
		System.out.println("read count : " + count);
		System.out.println();
	}
	
	public void write() {
		count++;
		System.out.println("write count : " + count);
	}
}
```
**测试代码：**




```java
public static void main(String[] args) {
		ObjectData data = new ObjectData();
		ConsumerThread ct = new ConsumerThread(data);
		ProductThread pt = new ProductThread(data);

		ct.start();
		pt.start();
}
```
**总结：**


示例程序代码完成了线程之间如何通过wait与notify实现数据的读写

同步控制。演示了Java的同步关键字synchronized的用法与线程的用法。
















