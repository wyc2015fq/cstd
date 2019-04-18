# BlockingQueue的使用 - z69183787的专栏 - CSDN博客
2015年09月01日 10:57:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：724
个人分类：[Java-并发集合](https://blog.csdn.net/z69183787/article/category/5783255)
本例介绍一个特殊的队列:BlockingQueue,如果BlockQueue是空的,从BlockingQueue取东西的操作将会被阻断进入等待状态,直到BlockingQueue进了东西才会被唤醒.同样,如果BlockingQueue是满的,任何试图往里存东西的操作也会被阻断进入等待状态,直到BlockingQueue里有空间才会被唤醒继续操作. 
    使用BlockingQueue的关键技术点如下: 
    1.BlockingQueue定义的常用方法如下: 
        1)add(anObject):把anObject加到BlockingQueue里,即如果BlockingQueue可以容纳,则返回true,否则报异常 
        2)offer(anObject):表示如果可能的话,将anObject加到BlockingQueue里,即如果BlockingQueue可以容纳,则返回true,否则返回false.
        3)put(anObject):把anObject加到BlockingQueue里,如果BlockQueue没有空间,则调用此方法的线程被阻断直到BlockingQueue里面有空间再继续. 
        4)poll(time):取走BlockingQueue里排在首位的对象,若不能立即取出,则可以等time参数规定的时间,取不到时返回null 
        5)take():取走BlockingQueue里排在首位的对象,若BlockingQueue为空,阻断进入等待状态直到Blocking有新的对象被加入为止 
    2.BlockingQueue有四个具体的实现类,根据不同需求,选择不同的实现类 
        1)ArrayBlockingQueue:规定大小的BlockingQueue,其构造函数必须带一个int参数来指明其大小.其所含的对象是以FIFO(先入先出)顺序排序的. 
        2)LinkedBlockingQueue:大小不定的BlockingQueue,若其构造函数带一个规定大小的参数,生成的BlockingQueue有大小限制,若不带大小参数,所生成的BlockingQueue的大小由Integer.MAX_VALUE来决定.其所含的对象是以FIFO(先入先出)顺序排序的 
        3)PriorityBlockingQueue:类似于LinkedBlockQueue,但其所含对象的排序不是FIFO,而是依据对象的自然排序顺序或者是构造函数的Comparator决定的顺序. 
        4)SynchronousQueue:特殊的BlockingQueue,对其的操作必须是放和取交替完成的. 
    3.LinkedBlockingQueue和ArrayBlockingQueue比较起来,它们背后所用的数据结构不一样,导致LinkedBlockingQueue的数据吞吐量要大于ArrayBlockingQueue,但在线程数量很大时其性能的可预见性低于ArrayBlockingQueue.      
　　下面是两个使用BlockingQueue的例子：  
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package com.thread;
 2 import java.util.concurrent.ArrayBlockingQueue;
 3 import java.util.concurrent.BlockingQueue;
 4 
 5 public class BlockingQueueTest {
 6     public static void main(String[] args) {
 7         final BlockingQueue queue = new ArrayBlockingQueue(3);
 8         for(int i=0;i<2;i++){
 9             new Thread(){
10                 public void run(){
11                     while(true){
12                         try {
13                             Thread.sleep((long)(Math.random()*1000));
14                             System.out.println(Thread.currentThread().getName() + "准备放数据!");                            
15                             queue.put(1);
16                             System.out.println(Thread.currentThread().getName() + "已经放了数据，" +                             
17                                         "队列目前有" + queue.size() + "个数据");
18                         } catch (InterruptedException e) {
19                             e.printStackTrace();
20                         }
21 
22                     }
23                 }
24                 
25             }.start();
26         }
27         
28         new Thread(){
29             public void run(){
30                 while(true){
31                     try {
32                         //将此处的睡眠时间分别改为100和1000，观察运行结果
33                         Thread.sleep(1000);
34                         System.out.println(Thread.currentThread().getName() + "准备取数据!");
35                         queue.take();
36                         System.out.println(Thread.currentThread().getName() + "已经取走数据，" +                             
37                                 "队列目前有" + queue.size() + "个数据");                    
38                     } catch (InterruptedException e) {
39                         e.printStackTrace();
40                     }
41                 }
42             }
43             
44         }.start();            
45     }
46 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package com.thread;
 2 import java.util.concurrent.ArrayBlockingQueue;
 3 import java.util.concurrent.BlockingQueue;
 4 import java.util.concurrent.ExecutorService;
 5 import java.util.concurrent.Executors;
 6 import java.util.concurrent.locks.Condition;
 7 import java.util.concurrent.locks.Lock;
 8 import java.util.concurrent.locks.ReentrantLock;
 9 
10 public class BlockingQueueCondition {
11 
12     public static void main(String[] args) {
13         ExecutorService service = Executors.newSingleThreadExecutor();
14         final Business3 business = new Business3();
15         service.execute(new Runnable(){
16 
17             public void run() {
18                 for(int i=0;i<50;i++){
19                     business.sub();
20                 }
21             }
22             
23         });
24         
25         for(int i=0;i<50;i++){
26             business.main();
27         }
28     }
29 
30 }
31 
32 class Business3{
33     BlockingQueue subQueue = new ArrayBlockingQueue(1);
34     BlockingQueue mainQueue = new ArrayBlockingQueue(1);
35     //这里是匿名构造方法，只要new一个对象都会调用这个匿名构造方法，它与静态块不同，静态块只会执行一次，
36     //在类第一次加载到JVM的时候执行
37     //这里主要是让main线程首先put一个，就有东西可以取，如果不加这个匿名构造方法put一个的话程序就死锁了
38     {
39         try {
40             mainQueue.put(1);
41         } catch (InterruptedException e) {
42             e.printStackTrace();
43         }
44     }
45     public void sub(){
46         try
47         {
48             mainQueue.take();
49             for(int i=0;i<10;i++){
50                 System.out.println(Thread.currentThread().getName() + " : " + i);
51             }
52             subQueue.put(1);
53         }catch(Exception e){
54 
55         }
56     }
57     
58     public void main(){
59         
60         try
61         {
62             subQueue.take();
63             for(int i=0;i<5;i++){
64                 System.out.println(Thread.currentThread().getName() + " : " + i);
65             }
66             mainQueue.put(1);
67         }catch(Exception e){
68         }        
69     }
70 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
