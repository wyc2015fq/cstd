# SynchronousQueue的简单应用2 - z69183787的专栏 - CSDN博客
2015年09月01日 10:58:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：449
个人分类：[Java-并发集合](https://blog.csdn.net/z69183787/article/category/5783255)

SynchronousQueue<E>的定义如下
public class SynchronousQueue<E>
extends AbstractQueue<E>
implements BlockingQueue<E>, Serializable
从上面可以看出，它实现BlockingQueue<E>，所以是阻塞队列，从名字看，它又是同步的。
它模拟的功能类似于生活中一手交钱一手交货这种情形，像那种货到付款或者先付款后发货模型不适合使用SynchronousQueue。
首先要知道SynchronousQueue没有容纳元素的能力，即它的isEmpty()方法总是返回true，但是给人的感觉却像是只能容纳一个元素。
另外在创建SynchronousQueue时可以传递一个boolean参数来指定它是否是访问它的线程按遵守FIFO顺序处理，true表示遵守FIFO。
下面使用SynchronousQueue模拟只能生产一个产品的生产者-消费者模型
**[java]**[view
 plain](http://blog.csdn.net/wxwzy738/article/details/8502882#)[copy](http://blog.csdn.net/wxwzy738/article/details/8502882#)
- import java.util.Random;  
- import java.util.concurrent.SynchronousQueue;  
- import java.util.concurrent.TimeUnit;  
- 
- publicclass Test19 {  
- publicstaticvoid main(String[] args) {  
-         SynchronousQueue<Integer> queue = new SynchronousQueue<Integer>();  
- new Customer(queue).start();  
- new Product(queue).start();  
-     }  
- staticclass Product extends Thread{  
-         SynchronousQueue<Integer> queue;  
- public Product(SynchronousQueue<Integer> queue){  
- this.queue = queue;  
-         }  
- @Override
- publicvoid run(){  
- while(true){  
- int rand = new Random().nextInt(1000);  
-                 System.out.println("生产了一个产品："+rand);  
-                 System.out.println("等待三秒后运送出去...");  
- try {  
-                     TimeUnit.SECONDS.sleep(3);  
-                 } catch (InterruptedException e) {  
-                     e.printStackTrace();  
-                 }  
-                 queue.offer(rand);  
-             }  
-         }  
-     }  
- staticclass Customer extends Thread{  
-         SynchronousQueue<Integer> queue;  
- public Customer(SynchronousQueue<Integer> queue){  
- this.queue = queue;  
-         }  
- @Override
- publicvoid run(){  
- while(true){  
- try {  
-                     System.out.println("消费了一个产品:"+queue.take());  
-                 } catch (InterruptedException e) {  
-                     e.printStackTrace();  
-                 }  
-                 System.out.println("------------------------------------------");  
-             }  
-         }  
-     }  
- /**
-      * 运行结果：
-      *  生产了一个产品：464
-         等待三秒后运送出去...
-         消费了一个产品:773
-         ------------------------------------------
-         生产了一个产品：547
-         等待三秒后运送出去...
-         消费了一个产品:464
-         ------------------------------------------
-         生产了一个产品：87
-         等待三秒后运送出去...
-         消费了一个产品:547
-         ------------------------------------------
-      */
- }  
- 
