# 阻塞队列BlockingQueue使用 - 零度的博客专栏 - CSDN博客
2016年10月28日 15:40:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：627
            BlockingQueue的原理及方法：BlockingQueue最终会有四种状况，抛出异常、返回特殊值、阻塞、超时，下表总结了这些方法：
> ||*抛出异常*|*特殊值*|*阻塞*|*超时*|
|----|----|----|----|----|
|**插入**|[`add(e)`](http://blog.csdn.net/ghsau/article/details/8108292)|[`offer(e)`](http://blog.csdn.net/ghsau/article/details/8108292)|[`put(e)`](http://blog.csdn.net/ghsau/article/details/8108292)|[`offer(e, time, unit)`](http://blog.csdn.net/ghsau/article/details/8108292)|
|**移除**|[`remove()`](http://blog.csdn.net/ghsau/article/details/8108292)|[`poll()`](http://blog.csdn.net/ghsau/article/details/8108292)|[`take()`](http://blog.csdn.net/ghsau/article/details/8108292)|[`poll(time, unit)`](http://blog.csdn.net/ghsau/article/details/8108292)|
|**检查**|[`element()`](http://blog.csdn.net/ghsau/article/details/8108292)|[`peek()`](http://blog.csdn.net/ghsau/article/details/8108292)|*不可用*|*不可用*|
BlockingQueue是个接口，有如下实现类：
       1. ArrayBlockQueue：一个由数组支持的有界阻塞队列。此队列按 FIFO（先进先出）原则对元素进行排序。创建其对象必须明确大小，像数组一样。
       2. LinkedBlockQueue：一个可改变大小的阻塞队列。此队列按 FIFO（先进先出）原则对元素进行排序。创建其对象如果没有明确大小，默认值是Integer.MAX_VALUE。链接队列的吞吐量通常要高于基于数组的队列，但是在大多数并发应用程序中，其可预知的性能要低。 
       3. PriorityBlockingQueue：类似于LinkedBlockingQueue，但其所含对象的排序不是FIFO，而是依据对象的自然排序顺序或者是构造函数所带的Comparator决定的顺序。
       4. SynchronousQueue：同步队列。同步队列没有任何容量，每个插入必须等待另一个线程移除，反之亦然。
       下面使用ArrayBlockQueue来实现[生产者消/费者模式](http://blog.csdn.net/ghsau/article/details/7433673)，代码如下：
```
/** 定义一个盘子类，可以放鸡蛋和取鸡蛋 */
public class BigPlate {
	/** 装鸡蛋的盘子，大小为5 */
	private BlockingQueue<Object> eggs = new ArrayBlockingQueue<Object>(5);
	
	/** 放鸡蛋 */
	public void putEgg(Object egg) {
		try {
			eggs.put(egg);// 向盘子末尾放一个鸡蛋，如果盘子满了，当前线程阻塞
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		// 下面输出有时不准确，因为与put操作不是一个原子操作
		System.out.println("放入鸡蛋");
	}
	
	/** 取鸡蛋 */
	public Object getEgg() {
		Object egg = null;
		try {
			egg = eggs.take();// 从盘子开始取一个鸡蛋，如果盘子空了，当前线程阻塞
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		// 下面输出有时不准确，因为与take操作不是一个原子操作
		System.out.println("拿到鸡蛋");
		return egg;
	}
	
	/** 放鸡蛋线程 */
	static class AddThread extends Thread {
		private BigPlate plate;
		private Object egg = new Object();
		public AddThread(BigPlate plate) {
			this.plate = plate;
		}
		public void run() {
			plate.putEgg(egg);
		}
	}
	/** 取鸡蛋线程 */
	static class GetThread extends Thread {
		private BigPlate plate;
		public GetThread(BigPlate plate) {
			this.plate = plate;
		}
		public void run() {
			plate.getEgg();
		}
	}
	
	public static void main(String[] args) {
		BigPlate plate = new BigPlate();
		// 先启动10个放鸡蛋线程
		for(int i = 0; i < 10; i++) {
			new Thread(new AddThread(plate)).start();
		}
		// 再启动10个取鸡蛋线程
		for(int i = 0; i < 10; i++) {
			new Thread(new GetThread(plate)).start();
		}
	}
}
```
执行结果：
```
放入鸡蛋
放入鸡蛋
放入鸡蛋
拿到鸡蛋
拿到鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
放入鸡蛋
拿到鸡蛋
拿到鸡蛋
```

