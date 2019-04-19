# SynchronousQueue的使用 - 零度的博客专栏 - CSDN博客
2016年10月31日 10:30:40[零度anngle](https://me.csdn.net/zmx729618)阅读数：4575
SynchronousQueue是这样一种阻塞队列，其中每个 put 必须等待一个 take，反之亦然。同步队列没有任何内部容量，甚至连一个队列的容量都没有。 
        不能在同步队列上进行 peek，因为仅在试图要取得元素时，该元素才存在； 
        除非另一个线程试图移除某个元素，否则也不能（使用任何方法）添加元素;也不能迭代队列，因为其中没有元素可用于迭代。队列的头是尝试添加到队列中的首个已排队线程元素； 如果没有已排队线程，则不添加元素并且头为 null。 
        对于其他 Collection 方法（例如 contains），SynchronousQueue 作为一个空集合。此队列不允许 null 元素。
        它非常适合于传递性设计，在这种设计中，在一个线程中运行的对象要将某些信息、事件或任务传递给在另一个线程中运行的对象，它就必须与该对象同步。 
        对于正在等待的生产者和使用者线程而言，此类支持可选的公平排序策略。默认情况下不保证这种排序。但是，使用公平设置为 true 所构造的队列可保证线程以 FIFO 的顺序进行访问。 公平通常会降低吞吐量，但是可以减小可变性并避免得不到服务。 
        注意1：它一种阻塞队列，其中每个 put 必须等待一个 take，反之亦然。同步队列没有任何内部容量，甚至连一个队列的容量都没有。 
        注意2：它是线程安全的，是阻塞的。 
        注意3:不允许使用 null 元素。 
        注意4：公平排序策略是指调用put的线程之间，或take的线程之间。公平排序策略可以查考ArrayBlockingQueue中的公平策略。 
        注意5:SynchronousQueue的以下方法： 
        * iterator() 永远返回空，因为里面没东西。 
        * peek() 永远返回null。 
        * put() 往queue放进去一个element以后就一直wait直到有其他thread进来把这个element取走。 
        * offer() 往queue里放一个element后立即返回，如果碰巧这个element被另一个thread取走了，offer方法返回true，认为offer成功；否则返回false。 
        * offer(2000, TimeUnit.SECONDS) 往queue里放一个element但是等待指定的时间后才返回，返回的逻辑和offer()方法一样。 
        * take() 取出并且remove掉queue里的element（认为是在queue里的。。。），取不到东西他会一直等。 
        * poll() 取出并且remove掉queue里的element（认为是在queue里的。。。），只有到碰巧另外一个线程正在往queue里offer数据或者put数据的时候，该方法才会取到东西。否则立即返回null。 
        * poll(2000, TimeUnit.SECONDS) 等待指定的时间然后取出并且remove掉queue里的element,其实就是再等其他的thread来往里塞。 
        * isEmpty()永远是true。 
        * remainingCapacity() 永远是0。 
        * remove()和removeAll() 永远是false。 
这是一个很有意思的阻塞队列，其中每个插入操作必须等待另一个线程的移除操作，同样任何一个移除操作都等待另一个线程的插入操作。因此此队列内部其 实没有任何一个元素，或者说容量是0，严格说并不是一种容器。由于队列没有容量，因此不能调用peek操作，因为只有移除元素时才有元素。
        一个没有容量的并发队列有什么用了？或者说存在的意义是什么？SynchronousQueue 的实现非常复杂，SynchronousQueue 内部没有容量，但是由于一个插入操作总是对应一个移除操作，反过来同样需要满足。那么一个元素就不会再SynchronousQueue 里面长时间停留，一旦有了插入线程和移除线程，元素很快就从插入线程移交给移除线程。也就是说这更像是一种信道（管道），资源从一个方向快速传递到另一方 向。需要特别说明的是，尽管元素在SynchronousQueue
 内部不会“停留”，但是并不意味之SynchronousQueue 内部没有队列。实际上SynchronousQueue 维护者线程队列，也就是插入线程或者移除线程在不同时存在的时候就会有线程队列。既然有队列，同样就有公平性和非公平性特性，公平性保证正在等待的插入线 程或者移除线程以FIFO的顺序传递资源。显然这是一种快速传递元素的方式，也就是说在这种情况下元素总是以最快的方式从插入着（生产者）传递给移除着（消费者），这在多任务队列中是最快处理任务的方式。在线程池的相关章节中还会更多的提到此特性。
       它模拟的功能类似于生活中一手交钱一手交货这种情形，像那种货到付款或者先付款后发货模型不适合使用SynchronousQueue。首先要知道SynchronousQueue没有容纳元素的能力，即它的isEmpty()方法总是返回true，但是给人的感觉却像是只能容纳一个元素。
         下面使用SynchronousQueue模拟只能生产一个产品的生产者-消费者模型。代码如下：
```
package zmx.concurrent.test;
import java.util.Random;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.TimeUnit;
public class SynchronousQueueTest {
	public static void main(String[] args) throws InterruptedException {
		SynchronousQueue<Integer> queue = new SynchronousQueue<Integer>();
	
		new Product(queue).start();
		new Customer(queue).start();
	}
	static class Product extends Thread{
		SynchronousQueue<Integer> queue;
		public Product(SynchronousQueue<Integer> queue){
			this.queue = queue;
		}
		@Override
		public void run(){
			while(true){
				int rand = new Random().nextInt(1000);
				System.out.println("生产了一个产品："+rand);
				System.out.println("等待三秒后运送出去...");
				try {
					TimeUnit.SECONDS.sleep(3);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				try {
					queue.put(rand);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				
				System.out.println(queue.isEmpty());
			}
		}
	}
	static class Customer extends Thread{
		SynchronousQueue<Integer> queue;
		public Customer(SynchronousQueue<Integer> queue){
			this.queue = queue;
		}
		@Override
		public void run(){
			while(true){
				try {
					System.out.println("消费了一个产品:"+queue.take());
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				System.out.println("------------------------------------------");
			}
		}
	}
}
```
运行结果：
```
生产了一个产品：542
等待三秒后运送出去...
true
消费了一个产品:542
生产了一个产品：183
等待三秒后运送出去...
------------------------------------------
true
消费了一个产品:183
------------------------------------------
生产了一个产品：583
等待三秒后运送出去...
true
消费了一个产品:583
------------------------------------------
```
