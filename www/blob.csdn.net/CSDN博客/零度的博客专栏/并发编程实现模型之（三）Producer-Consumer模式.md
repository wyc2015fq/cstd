# 并发编程实现模型之（三）Producer-Consumer模式 - 零度的博客专栏 - CSDN博客
2017年11月13日 10:27:00[零度anngle](https://me.csdn.net/zmx729618)阅读数：1076
生产者-消费模式，通常有两类线程，即若干个生产者线程和若干个消费者线程。生产者线程负责提交用户请求，消费者线程负责具体处理生产者提交的任务。两者之间通过共享内存缓冲去进行通信。
**一、架构模式图：**
![](https://img-blog.csdn.net/20140930200739929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG1kY3N6aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
类图：
![](https://img-blog.csdn.net/20140930201427417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG1kY3N6aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
生产者：提交用户请求，提取用户任务，并装入内存缓冲区；
消费者：在内存缓冲区中提取并处理任务；
内存缓冲区：缓存生产者提交的任务或数据，供消费者使用；
任务：生产者向内存缓冲区提交的数据结构；
Main：使用生产者和消费者的客户端。
**二、代码实现一个基于生产者-消费者模式的求整数平方的并行计算：**
（1）Producer生产者线程：
**[java]**[view plain](http://blog.csdn.net/lmdcszh/article/details/39699261#)[copy](http://blog.csdn.net/lmdcszh/article/details/39699261#)[print](http://blog.csdn.net/lmdcszh/article/details/39699261#)[?](http://blog.csdn.net/lmdcszh/article/details/39699261#)
- package ProducerConsumer;  
- 
- import java.util.Random;  
- import java.util.concurrent.BlockingQueue;  
- import java.util.concurrent.TimeUnit;  
- import java.util.concurrent.atomic.AtomicInteger;  
- 
- publicclass Producer  implements Runnable{  
- 
- //Volatile修饰的成员变量在每次被线程访问时，都强迫从共享内存中重读该成员变量的值。
- //而且，当成员变量发生变化时，强迫线程将变化值回写到共享内存。
- //这样在任何时刻，两个不同的线程总是看到某个成员变量的同一个值。
- privatevolatileboolean isRunning= true;  
- 
- //内存缓冲区
- private BlockingQueue<PCData> queue;  
- 
- //总数，原子操作
- privatestatic AtomicInteger count = new AtomicInteger();  
- 
- privatestaticfinalint SLEEPTIME=1000;  
- 
- 
- public Producer(BlockingQueue<PCData> queue) {  
- 
- this.queue = queue;  
-     }  
- 
- 
- 
- 
- @Override
- publicvoid run() {  
-         PCData data=null;  
-         Random r  = new Random();  
-         System.out.println("start producer id = "+ Thread .currentThread().getId());  
- try{  
- while(isRunning){  
-                 Thread.sleep(r.nextInt(SLEEPTIME));  
- //构造任务数据
-                 data= new PCData(count.incrementAndGet());  
-                 System.out.println("data is put into queue ");  
- //提交数据到缓冲区
- if(!queue.offer(data,2,TimeUnit.SECONDS)){  
-                     System.out.println("faile to  put data:  "+ data);  
-                 }  
-             }  
-         }catch (InterruptedException e){  
-             e.printStackTrace();  
-             Thread.currentThread().interrupt();  
- 
-         }  
- 
- 
-     }  
- 
- publicvoid stop(){  
- 
-         isRunning=false;  
-     }  
- 
- 
- }  
- 
（2）Consumer消费者线程：
**[java]**[view plain](http://blog.csdn.net/lmdcszh/article/details/39699261#)[copy](http://blog.csdn.net/lmdcszh/article/details/39699261#)[print](http://blog.csdn.net/lmdcszh/article/details/39699261#)[?](http://blog.csdn.net/lmdcszh/article/details/39699261#)
- package ProducerConsumer;  
- 
- import java.text.MessageFormat;  
- import java.util.Random;  
- import java.util.concurrent.BlockingQueue;  
- 
- publicclass Consumer implements Runnable {  
- //缓冲区   
- private BlockingQueue<PCData> queue;  
- privatestaticfinalint SLEEPTIME=1000;  
- 
- 
- public Consumer(BlockingQueue<PCData> queue) {          
- this.queue = queue;  
-     }  
- 
- 
- @Override
- publicvoid run() {  
-         System.out.println("start Consumer id= "+ Thread .currentThread().getId());  
-         Random r = new Random();  
- 
- try {  
- //提取任务
- while(true){  
-                     PCData data= queue.take();  
- if(null!= data){  
- //计算平方
- int re= data.getData()*data.getData();  
-                         System.out.println(MessageFormat.format("{0}*{1}={2}",  
-                                     data.getData(),data.getData(),re  
-                                 ));  
-                         Thread.sleep(r.nextInt(SLEEPTIME));  
- 
-                     }  
-                 }  
-             } catch (InterruptedException e) {                
-                 e.printStackTrace();  
-                 Thread.currentThread().interrupt();  
-             }  
- 
- 
- 
-     }  
- 
- 
- 
- 
- 
- }  
- 
（3）PCData共享数据模型：
**[java]**[view plain](http://blog.csdn.net/lmdcszh/article/details/39699261#)[copy](http://blog.csdn.net/lmdcszh/article/details/39699261#)[print](http://blog.csdn.net/lmdcszh/article/details/39699261#)[?](http://blog.csdn.net/lmdcszh/article/details/39699261#)
- package ProducerConsumer;  
- 
- publicfinalclass PCData {  
- 
- privatefinalint intData;  
- 
- public PCData(int d) {  
-         intData=d;  
-     }  
- 
- public PCData(String  d) {  
-         intData=Integer.valueOf(d);  
-     }  
- 
- publicint getData(){  
- 
- return intData;  
- 
-     }  
- @Override
- public String toString(){  
- return"data:"+ intData ;  
-     }  
- 
- }  
- 
（4）Main函数：
**[java]**[view plain](http://blog.csdn.net/lmdcszh/article/details/39699261#)[copy](http://blog.csdn.net/lmdcszh/article/details/39699261#)[print](http://blog.csdn.net/lmdcszh/article/details/39699261#)[?](http://blog.csdn.net/lmdcszh/article/details/39699261#)
- package ProducerConsumer;  
- 
- import java.util.concurrent.BlockingQueue;  
- import java.util.concurrent.Executor;  
- import java.util.concurrent.ExecutorService;  
- import java.util.concurrent.Executors;  
- import java.util.concurrent.LinkedBlockingDeque;  
- 
- publicclass Main {  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args)  throws InterruptedException{  
- //建立缓冲区
-         BlockingQueue<PCData> queue = new LinkedBlockingDeque<PCData>(10);  
- //建立生产者
-         Producer producer1 = new Producer(queue);  
-         Producer producer2 = new Producer(queue);  
-         Producer producer3 = new Producer(queue);  
- 
- //建立消费者
-         Consumer consumer1 = new Consumer(queue);  
-         Consumer consumer2 = new Consumer(queue);  
-         Consumer consumer3 = new Consumer(queue);         
- 
- //建立线程池
-         ExecutorService service = Executors.newCachedThreadPool();  
- 
- //运行生产者
-         service.execute(producer1);  
-         service.execute(producer2);  
-         service.execute(producer3);  
- //运行消费者
-         service.execute(consumer1);  
-         service.execute(consumer2);  
-         service.execute(consumer3);  
- 
-         Thread.sleep(10*1000);  
- 
- //停止生产者
-         producer1.stop();  
-         producer2.stop();  
-         producer3.stop();  
- 
-         Thread.sleep(3000);  
-         service.shutdown();  
-     }  
- 
- }  
- 
**三、注意：**
    volatile关键字：Volatile修饰的成员变量在每次被线程访问时，都强迫从共享内存中重读该成员变量的值。而且，当成员变量发生变化时，强迫线程将变化值回写到共享内存。这样在任何时刻，两个不同的线程总是看到某个成员变量的同一个值。
    生产-消费模式的核心组件是共享内存缓冲区，是两者的通信桥梁，起到解耦作用，优化系统整体结构。
    由于缓冲区的存在，生产者和消费者，无论谁在某一局部时间内速度相对较高，都可以使用缓冲区得到缓解，保证系统正常运行，这在一定程度上缓解了性能瓶颈对系统系能的影响。
