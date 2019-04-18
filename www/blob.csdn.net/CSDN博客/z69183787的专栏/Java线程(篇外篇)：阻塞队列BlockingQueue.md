# Java线程(篇外篇)：阻塞队列BlockingQueue - z69183787的专栏 - CSDN博客
2014年06月05日 21:25:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1481
       好久没有写文章了，这段时间事情比较杂，工作也比较杂乱，上周日刚搬完家，从自建房搬到了楼房，提升了一层生活品质，哈哈！不过昨天晚上在公交车上钱包被偷了，前段时间还丢个自行车，不得不感叹，京城扒手真多，还无人处理。言归正传，这一段时间我的工作主要是改进公司的调度器，调度器调度线程池执行任务，生产者生产任务，消费者消费任务，那么这时就需要一个任务队列，生产者向队列里插入任务，消费者从队列里提取任务执行，调度器里是通过BlockingQueue实现的队列，随后小查一下，下面看看BlockingQueue的原理及其方法。
       BlockingQueue最终会有四种状况，抛出异常、返回特殊值、阻塞、超时，下表总结了这些方法：
> 
||*抛出异常*|*特殊值*|*阻塞*|*超时*|
|----|----|----|----|----|
|**插入**|[`add(e)`](http://blog.csdn.net/ghsau/article/details/8108292)|[`offer(e)`](http://blog.csdn.net/ghsau/article/details/8108292)|[`put(e)`](http://blog.csdn.net/ghsau/article/details/8108292)|[`offer(e, time, unit)`](http://blog.csdn.net/ghsau/article/details/8108292)|
|**移除**|[`remove()`](http://blog.csdn.net/ghsau/article/details/8108292)|[`poll()`](http://blog.csdn.net/ghsau/article/details/8108292)|[`take()`](http://blog.csdn.net/ghsau/article/details/8108292)|[`poll(time, unit)`](http://blog.csdn.net/ghsau/article/details/8108292)|
|**检查**|[`element()`](http://blog.csdn.net/ghsau/article/details/8108292)|[`peek()`](http://blog.csdn.net/ghsau/article/details/8108292)|*不可用*|*不可用*|
       BlockingQueue是个接口，有如下实现类：
       1. ArrayBlockQueue：一个由数组支持的有界阻塞队列。此队列按 FIFO（先进先出）原则对元素进行排序。创建其对象必须明确大小，像数组一样。
       2. LinkedBlockQueue：一个可改变大小的阻塞队列。此队列按 FIFO（先进先出）原则对元素进行排序。创建其对象如果没有明确大小，默认值是Integer.MAX_VALUE。链接队列的吞吐量通常要高于基于数组的队列，但是在大多数并发应用程序中，其可预知的性能要低。 
       3. PriorityBlockingQueue：类似于LinkedBlockingQueue，但其所含对象的排序不是FIFO，而是依据对象的自然排序顺序或者是构造函数所带的Comparator决定的顺序。
       4. SynchronousQueue：同步队列。同步队列没有任何容量，每个插入必须等待另一个线程移除，反之亦然。
       下面使用ArrayBlockQueue来实现之前实现过的[生产者消/费者模式](http://blog.csdn.net/ghsau/article/details/7433673)，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/8108292#)[copy](http://blog.csdn.net/ghsau/article/details/8108292#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126819/fork)
- /** 定义一个盘子类，可以放鸡蛋和取鸡蛋 */
- publicclass BigPlate {  
- 
- /** 装鸡蛋的盘子，大小为5 */
- private BlockingQueue<Object> eggs = new ArrayBlockingQueue<Object>(5);  
- 
- /** 放鸡蛋 */
- publicvoid putEgg(Object egg) {  
- try {  
-             eggs.put(egg);// 向盘子末尾放一个鸡蛋，如果盘子满了，当前线程阻塞
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }  
- 
- // 下面输出有时不准确，因为与put操作不是一个原子操作
-         System.out.println("放入鸡蛋");  
-     }  
- 
- /** 取鸡蛋 */
- public Object getEgg() {  
-         Object egg = null;  
- try {  
-             egg = eggs.take();// 从盘子开始取一个鸡蛋，如果盘子空了，当前线程阻塞
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }  
- 
- // 下面输出有时不准确，因为与take操作不是一个原子操作
-         System.out.println("拿到鸡蛋");  
- return egg;  
-     }  
- 
- /** 放鸡蛋线程 */
- staticclass AddThread extends Thread {  
- private BigPlate plate;  
- private Object egg = new Object();  
- 
- public AddThread(BigPlate plate) {  
- this.plate = plate;  
-         }  
- 
- publicvoid run() {  
-             plate.putEgg(egg);  
-         }  
-     }  
- 
- /** 取鸡蛋线程 */
- staticclass GetThread extends Thread {  
- private BigPlate plate;  
- 
- public GetThread(BigPlate plate) {  
- this.plate = plate;  
-         }  
- 
- publicvoid run() {  
-             plate.getEgg();  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         BigPlate plate = new BigPlate();  
- // 先启动10个放鸡蛋线程
- for(int i = 0; i < 10; i++) {  
- new Thread(new AddThread(plate)).start();  
-         }  
- // 再启动10个取鸡蛋线程
- for(int i = 0; i < 10; i++) {  
- new Thread(new GetThread(plate)).start();  
-         }  
-     }  
- }  
       执行结果：
**[plain]**[view
 plain](http://blog.csdn.net/ghsau/article/details/8108292#)[copy](http://blog.csdn.net/ghsau/article/details/8108292#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126819/fork)
- 放入鸡蛋  
- 放入鸡蛋  
- 放入鸡蛋  
- 放入鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 拿到鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 放入鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 拿到鸡蛋  
- 拿到鸡蛋  
- 拿到鸡蛋  
- 拿到鸡蛋  
       从结果看，启动10个放鸡蛋线程和10个取鸡蛋线程，前5个放入鸡蛋的线程成功执行，到第6个，发现盘子满了，阻塞住，这时切换到取鸡蛋线程执行，成功实现了生产者/消费者模式。java.util.concurrent包是个强大的包！
        本文来自：[高爽|Coder](http://blog.csdn.net/ghsau)，原文地址：[](http://blog.csdn.net/ghsau/article/details/7433673)[http://blog.csdn.net/ghsau/article/details/8108292](http://blog.csdn.net/ghsau/article/details/8108292)。
