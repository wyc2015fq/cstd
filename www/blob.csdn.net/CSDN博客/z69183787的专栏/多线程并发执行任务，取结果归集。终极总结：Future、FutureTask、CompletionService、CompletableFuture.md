# 多线程并发执行任务，取结果归集。终极总结：Future、FutureTask、CompletionService、CompletableFuture - z69183787的专栏 - CSDN博客
2018年04月08日 18:36:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：117
个人分类：[多线程																[多线程-专栏																[多线程-Java并发包学习](https://blog.csdn.net/z69183787/article/category/6243160)](https://blog.csdn.net/z69183787/article/category/6816123)](https://blog.csdn.net/z69183787/article/category/2176989)
[https://www.cnblogs.com/dennyzhangdd/p/7010972.html](https://www.cnblogs.com/dennyzhangdd/p/7010972.html)
**目录**
- [1.Futrue](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label0)- [原理:](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label0_0)
- [demo:](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label0_1)
- [建议：此种方法可实现基本目标，任务并行且按照提交顺序获取结果。使用很普遍，老少皆宜，就是CPU有消耗，可以使用！](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label0_2)
- [2.FutureTask](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label1)- [原理:](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label1_0)
- [demo:](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label1_1)
- [建议：demo1在特定场合例如有十分耗时的业务但有依赖于其他业务不一定非要执行的，可以尝试使用。demo2多线程并发执行并结果归集，这里多套一层FutureTask比较鸡肋（直接返回Future简单明了）不建议使用。](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label1_2)
- [3.CompletionService：](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label2)
- [4.CompletableFuture](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label3)- [原理：](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label3_0)
- [5.总结：](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_label4)
**正文**
开启线程执行任务，不管是使用Runnable（无返回值不支持上报异常）还是Callable（有返回值支持上报异常）接口，都可以轻松实现。那么如果**是开启线程池并需要获取结果归集**的情况下，如何实现，以及优劣，**老司机直接看总结即可。**
任务执行完，结果归集时，几种方式：
[回到顶部](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_labelTop)
## 1.Futrue
### 原理:
如下图，Future接口封装了取消，获取线程结果，以及状态判断是否取消，是否完成这几个方法，都很有用。
![](https://images2015.cnblogs.com/blog/584866/201706/584866-20170616075627181-1145635502.png)
### demo:
使用线程池提交Callable接口任务，返回Future接口，添加进list,最后遍历FutureList且内部使用while轮询,并发获取结果
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 package thread.future;
 2 
 3 import java.util.ArrayList;
 4 import java.util.Date;
 5 import java.util.List;
 6 import java.util.concurrent.Callable;
 7 import java.util.concurrent.ExecutorService;
 8 import java.util.concurrent.Executors;
 9 import java.util.concurrent.Future;
10 
11 /**
12  * 
13  * @ClassName: FutureDemo
14  * @Description: Future多线程并发任务结果归集
15  * @author denny.zhang
16  * @date 2016年11月4日 下午1:50:32
17  *
18  */
19 public class FutureDemo{
20 
21     public static void main(String[] args)  {
22         Long start = System.currentTimeMillis();
23         //开启多线程
24         ExecutorService exs = Executors.newFixedThreadPool(10);
25         try {
26             //结果集
27             List<Integer> list = new ArrayList<Integer>();
28             List<Future<Integer>> futureList = new ArrayList<Future<Integer>>();
29             //1.高速提交10个任务，每个任务返回一个Future入list
30             for(int i=0;i<10;i++){
31                 futureList.add(exs.submit(new CallableTask(i+1)));
32             }
33             Long getResultStart = System.currentTimeMillis();
34             System.out.println("结果归集开始时间="+new Date());
35             //2.结果归集，遍历futureList,高速轮询（模拟实现了并发）获取future状态成功完成后获取结果，退出当前循环
36             for (Future<Integer> future : futureList) {
37                 while (true) {//CPU高速轮询：**每个future都并发轮循，判断完成状态然后获取结果，这一行，是本实现方案的精髓所在。即有10个future在高速轮询，完成一个future的获取结果，就关闭一个轮询**
38                     if (future.isDone()&& !future.isCancelled()) {//获取future成功完成状态，如果想要限制每个任务的超时时间，取消本行的状态判断+future.get(1000*1, TimeUnit.MILLISECONDS)+catch超时异常使用即可。
39                         Integer i = future.get();//获取结果
40                         System.out.println("任务i="+i+"获取完成!"+new Date());
41                         list.add(i);
42                         break;//当前future获取结果完毕，跳出while
43                     } else {
44                         Thread.sleep(1);//每次轮询休息1毫秒（CPU纳秒级），避免CPU高速轮循耗空CPU---》新手别忘记这个
45                     }
46                 }
47             }
48             System.out.println("list="+list);
49             System.out.println("总耗时="+(System.currentTimeMillis()-start)+",取结果归集耗时="+(System.currentTimeMillis()-getResultStart));
50         } catch (Exception e) {
51             e.printStackTrace();
52         } finally {
53             exs.shutdown();
54         }
55     }
56     static class CallableTask implements Callable<Integer>{
57         Integer i;
58         
59         public CallableTask(Integer i) {
60             super();
61             this.i=i;
62         }
63 
64         @Override
65         public Integer call() throws Exception {
66             if(i==1){
67                 Thread.sleep(3000);//任务1耗时3秒
68             }else if(i==5){
69                 Thread.sleep(5000);//任务5耗时5秒
70             }else{
71                 Thread.sleep(1000);//其它任务耗时1秒
72             }
73             System.out.println("task线程："+Thread.currentThread().getName()+"任务i="+i+",完成！");  
74             return i;
75         }
76     }
77 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
下面我会根据线程池的线程数量+配合每个线程设定不同的耗时时间，来看一下咱们的线程执行细节。（不关心的可以直接跳过~）
1.开启定长为10的线程池，ExecutorService exs = Executors.newFixedThreadPool(10);+任务1耗时3秒，任务5耗时5秒，其他1秒。控制台打印如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
结果归集开始时间=Fri Jun 16 08:38:10 CST 2017
task线程：pool-1-thread-3任务i=3,完成！+Fri Jun 16 08:38:11 CST 2017
task线程：pool-1-thread-4任务i=4,完成！+Fri Jun 16 08:38:11 CST 2017
task线程：pool-1-thread-2任务i=2,完成！+Fri Jun 16 08:38:11 CST 2017
task线程：pool-1-thread-7任务i=7,完成！+Fri Jun 16 08:38:11 CST 2017
task线程：pool-1-thread-10任务i=10,完成！+Fri Jun 16 08:38:11 CST 2017
task线程：pool-1-thread-8任务i=8,完成！+Fri Jun 16 08:38:11 CST 2017
task线程：pool-1-thread-9任务i=9,完成！+Fri Jun 16 08:38:11 CST 2017
task线程：pool-1-thread-6任务i=6,完成！+Fri Jun 16 08:38:11 CST 2017
task线程：pool-1-thread-1任务i=1,完成！+Fri Jun 16 08:38:13 CST 2017---》任务1执行耗时3秒
任务i=1获取完成!Fri Jun 16 08:38:13 CST 2017
任务i=2获取完成!Fri Jun 16 08:38:13 CST 2017
任务i=3获取完成!Fri Jun 16 08:38:13 CST 2017
任务i=4获取完成!Fri Jun 16 08:38:13 CST 2017
task线程：pool-1-thread-5任务i=5,完成！+Fri Jun 16 08:38:15 CST 2017---》任务5执行耗时5秒
任务i=5获取完成!Fri Jun 16 08:38:15 CST 2017
任务i=6获取完成!Fri Jun 16 08:38:15 CST 2017
任务i=7获取完成!Fri Jun 16 08:38:15 CST 2017
任务i=8获取完成!Fri Jun 16 08:38:15 CST 2017
任务i=9获取完成!Fri Jun 16 08:38:15 CST 2017
任务i=10获取完成!Fri Jun 16 08:38:15 CST 2017
list=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
总耗时=5006,取结果归集耗时=5002---》符合逻辑，10个任务，定长10线程池，其中一个任务耗时3秒，一个任务耗时5秒，由于并发高速轮训，耗时取最长5秒
![复制代码](https://common.cnblogs.com/images/copycode.gif)
2.开启定长为5的线程池，ExecutorService exs = Executors.newFixedThreadPool(5);+任务1耗时3秒，任务5耗时5秒，其他1秒。控制台打印如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
结果归集开始时间=Fri Jun 16 08:42:45 CST 2017
task线程：pool-1-thread-4任务i=4,完成！+Fri Jun 16 08:42:46 CST 2017---》线程2,3,4第1波执行任务234，耗时1秒【当前任务】，总耗时1秒【全部任务】---》注：这里的总耗时指的是最终完成全部10个任务的耗时时间线。
task线程：pool-1-thread-3任务i=3,完成！+Fri Jun 16 08:42:46 CST 2017
task线程：pool-1-thread-2任务i=2,完成！+Fri Jun 16 08:42:46 CST 2017
task线程：pool-1-thread-3任务i=6,完成！+Fri Jun 16 08:42:47 CST 2017---》线程2,3,4第2波执行任务678，耗时1秒，总耗时2秒
task线程：pool-1-thread-2任务i=7,完成！+Fri Jun 16 08:42:47 CST 2017
task线程：pool-1-thread-4任务i=8,完成！+Fri Jun 16 08:42:47 CST 2017
task线程：pool-1-thread-1任务i=1,完成！+Fri Jun 16 08:42:48 CST 2017---》线程1，第1波执行任务1，耗时3秒，总耗时3秒
task线程：pool-1-thread-3任务i=9,完成！+Fri Jun 16 08:42:48 CST 2017---》线程3，第3波执行任务9，耗时1秒，线程3总耗时3秒
任务i=1获取完成!Fri Jun 16 08:42:48 CST 2017
task线程：pool-1-thread-2任务i=10,完成！+Fri Jun 16 08:42:48 CST 2017
任务i=2获取完成!Fri Jun 16 08:42:48 CST 2017
任务i=3获取完成!Fri Jun 16 08:42:48 CST 2017
任务i=4获取完成!Fri Jun 16 08:42:48 CST 2017
task线程：pool-1-thread-5任务i=5,完成！+Fri Jun 16 08:42:50 CST 2017---》任务5，第一波执行任务5耗时5秒，总耗时5秒
任务i=5获取完成!Fri Jun 16 08:42:50 CST 2017
任务i=6获取完成!Fri Jun 16 08:42:50 CST 2017
任务i=7获取完成!Fri Jun 16 08:42:50 CST 2017
任务i=8获取完成!Fri Jun 16 08:42:50 CST 2017
任务i=9获取完成!Fri Jun 16 08:42:50 CST 2017
任务i=10获取完成!Fri Jun 16 08:42:50 CST 2017
list=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
总耗时=5006,取结果归集耗时=5002------》符合逻辑，10个任务，定长5的线程池，由于线程1阻塞3秒，线程5阻塞5秒，由于并发高速轮训，总耗时取最长5秒
![复制代码](https://common.cnblogs.com/images/copycode.gif)
3.开启定长为5的线程池且把线程sleep时间全部设定为1秒，即ExecutorService exs = Executors.newFixedThreadPool(5);Thread.sleep(1000);控制台打印如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 结果归集开始时间=Fri Jun 16 08:44:51 CST 2017
 2 task线程：pool-1-thread-4任务i=4,完成！+Fri Jun 16 08:44:52 CST 2017
 3 task线程：pool-1-thread-3任务i=3,完成！+Fri Jun 16 08:44:52 CST 2017
 4 task线程：pool-1-thread-5任务i=5,完成！+Fri Jun 16 08:44:52 CST 2017
 5 task线程：pool-1-thread-2任务i=2,完成！+Fri Jun 16 08:44:52 CST 2017
 6 task线程：pool-1-thread-1任务i=1,完成！+Fri Jun 16 08:44:52 CST 2017
 7 任务i=1获取完成!Fri Jun 16 08:44:52 CST 2017
 8 任务i=2获取完成!Fri Jun 16 08:44:52 CST 2017
 9 任务i=3获取完成!Fri Jun 16 08:44:52 CST 2017
10 任务i=4获取完成!Fri Jun 16 08:44:52 CST 2017
11 任务i=5获取完成!Fri Jun 16 08:44:52 CST 2017
12 task线程：pool-1-thread-4任务i=6,完成！+Fri Jun 16 08:44:53 CST 2017
13 task线程：pool-1-thread-3任务i=7,完成！+Fri Jun 16 08:44:53 CST 2017
14 task线程：pool-1-thread-1任务i=10,完成！+Fri Jun 16 08:44:53 CST 2017
15 task线程：pool-1-thread-2任务i=9,完成！+Fri Jun 16 08:44:53 CST 2017
16 任务i=6获取完成!Fri Jun 16 08:44:53 CST 2017
17 task线程：pool-1-thread-5任务i=8,完成！+Fri Jun 16 08:44:53 CST 2017
18 任务i=7获取完成!Fri Jun 16 08:44:53 CST 2017
19 任务i=8获取完成!Fri Jun 16 08:44:53 CST 2017
20 任务i=9获取完成!Fri Jun 16 08:44:53 CST 2017
21 任务i=10获取完成!Fri Jun 16 08:44:53 CST 2017
22 list=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
23 总耗时=2006,取结果归集耗时=2002------》符合逻辑，10个任务，定长5的线程池，执行2波，耗时2*1秒=2秒
![复制代码](https://common.cnblogs.com/images/copycode.gif)
4.如果开启定长为10的线程池且每个任务耗时一秒，即ExecutorService exs = Executors.newFixedThreadPool(10);Thread.sleep(1000);控制台打印如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
结果归集开始时间=Fri Jun 16 08:47:36 CST 2017
task线程：pool-1-thread-8任务i=8,完成！+Fri Jun 16 08:47:37 CST 2017
task线程：pool-1-thread-2任务i=2,完成！+Fri Jun 16 08:47:37 CST 2017
task线程：pool-1-thread-9任务i=9,完成！+Fri Jun 16 08:47:37 CST 2017
task线程：pool-1-thread-5任务i=5,完成！+Fri Jun 16 08:47:37 CST 2017
task线程：pool-1-thread-6任务i=6,完成！+Fri Jun 16 08:47:37 CST 2017
task线程：pool-1-thread-1任务i=1,完成！+Fri Jun 16 08:47:37 CST 2017
task线程：pool-1-thread-10任务i=10,完成！+Fri Jun 16 08:47:37 CST 2017
task线程：pool-1-thread-4任务i=4,完成！+Fri Jun 16 08:47:37 CST 2017
task线程：pool-1-thread-7任务i=7,完成！+Fri Jun 16 08:47:37 CST 2017
task线程：pool-1-thread-3任务i=3,完成！+Fri Jun 16 08:47:37 CST 2017
任务i=1获取完成!Fri Jun 16 08:47:37 CST 2017
任务i=2获取完成!Fri Jun 16 08:47:37 CST 2017
任务i=3获取完成!Fri Jun 16 08:47:37 CST 2017
任务i=4获取完成!Fri Jun 16 08:47:37 CST 2017
任务i=5获取完成!Fri Jun 16 08:47:37 CST 2017
任务i=6获取完成!Fri Jun 16 08:47:37 CST 2017
任务i=7获取完成!Fri Jun 16 08:47:37 CST 2017
任务i=8获取完成!Fri Jun 16 08:47:37 CST 2017
任务i=9获取完成!Fri Jun 16 08:47:37 CST 2017
任务i=10获取完成!Fri Jun 16 08:47:37 CST 2017
list=[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
总耗时=1009,取结果归集耗时=1005------》符合逻辑，10个任务，定长10的线程池，执行1波，耗时1秒
![复制代码](https://common.cnblogs.com/images/copycode.gif)
### 建议：**此种方法可实现基本目标，任务并行且按照提交顺序获取结果。**使用很普遍，老少皆宜，就是CPU有消耗，**可以使用**！
[回到顶部](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_labelTop)
## 2.FutureTask
### 原理:
是接口RunnableFuture的唯一实现类。类图如下（网上截取来的。。。我的eclipse类图插件还没装好）：
 ![](https://images2015.cnblogs.com/blog/584866/201706/584866-20170616093910009-1554310667.png)
如上图，可见RunnableFuture接口继承自Future<V>+Runnable:
1.Runnable接口，可开启单个线程执行。
2.Future<v>接口，可接受Callable接口的返回值，futureTask.get()阻塞获取结果。
FutureTask的构造方法有两种，其实最终都是赋值callable。如下图：
![](https://images2015.cnblogs.com/blog/584866/201706/584866-20170616094731587-2131717441.png)
### demo:
#### demo1:两个步骤：1.开启单个线程执行任务，2.阻塞等待执行结果，分离这两步骤，可在这两步中间穿插别的相关业务逻辑。
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 /**
 2  * 
 3  * @ClassName:FutureTaskDemo
 4  * @Description:FutureTask弥补了Future必须用线程池提交返回Future的缺陷，实现功能如下：
 5  * 1.Runnable接口，可开启线程执行。
 6  * 2.Future<v>接口，可接受Callable接口的返回值，futureTask.get()阻塞获取结果。
 7  * 这两个步骤：一个开启线程执行任务，一个阻塞等待执行结果，分离这两步骤，可在这两步中间穿插别的相关业务逻辑。
 8  * @author diandian.zhang
 9  * @date 2017年6月16日上午10:36:05
10  */
11 public class FutureTaskContorlDemo {
12     
13     public static void main(String[] args)  {
14         try {
15             System.out.println("=====例如一个统计公司总部和分部的总利润是否达标100万==========");
16             //利润
17             Integer count = 0;
18             //1.定义一个futureTask，假设去远程http获取各个分公司业绩.
19             FutureTask<Integer> futureTask = new FutureTask<Integer>(new CallableTask());
20             Thread futureTaskThread =  new Thread(futureTask);
21             futureTaskThread.start();
22             System.out.println("futureTaskThread start！"+new Date());
23             
24             //2.主线程先做点别的事
25             System.out.println("主线程查询总部公司利润开始时间："+new Date());
26             Thread.sleep(5000);
27             count+=10;//北京集团总部利润。
28             System.out.println("主线程查询总部公司利润结果时间："+new Date());
29             
30             //总部已达标100万利润，就不再继续执行获取分公司业绩任务了
31             if(count>=100){
32                 System.out.println("总部公司利润达标，取消futureTask！"+new Date());
33                 futureTask.cancel(true);//不需要再去获取结果，那么直接取消即可
34             }else{
35                 System.out.println("总部公司利润未达标，进入阻塞查询分公司利润！"+new Date());
36                 //3总部未达标.阻塞获取，各个分公司结果
37                 Integer i = futureTask.get();//真正执行CallableTask
38                 System.out.println("i="+i+"获取到结果!"+new Date()+new Date());
39             }
40         } catch (Exception e) {
41             e.printStackTrace();
42         }
43     }
44 
45     /**
46      * 
47      * @ClassName:CallableTask
48      * @Description:一个十分耗时的任务
49      * @author diandian.zhang
50      * @date 2017年6月16日上午10:39:04
51      */
52     static class CallableTask implements Callable<Integer>{
53         @Override
54         public Integer call() throws Exception {
55             System.out.println("CallableTask-call，查询分公司利润，执行开始！"+new Date());
56             Thread.sleep(10000);
57             System.out.println("CallableTask-call，查询分公司利润，执行完毕！"+new Date());
58             return 10;
59         }
60     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
执行结果如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
=====例如一个统计公司总部和分部的总利润是否达标100万==========
futureTaskThread start！Fri Jun 16 11:14:54 CST 2017----》futureTaskThread 已开始运行
CallableTask-call，查询分公司利润，执行开始！Fri Jun 16 11:14:54 CST 2017
主线程查询总部公司利润开始时间：Fri Jun 16 11:14:54 CST 2017------》主线程耗时5秒
主线程查询总部公司利润结果时间：Fri Jun 16 11:14:59 CST 2017
总部公司利润未达标，进入阻塞查询分公司利润！Fri Jun 16 11:14:59 CST 2017
CallableTask-call，查询分公司利润，执行完毕！Fri Jun 16 11:15:04 CST 2017----》futureTaskThread 执行完毕，耗时10秒
i=10获取到结果!Fri Jun 16 11:15:04 CST 2017Fri Jun 16 11:15:04 CST 2017
![复制代码](https://common.cnblogs.com/images/copycode.gif)
如上图，分离之后，futureTaskThread耗时10秒期间，主线程还穿插的执行了耗时5秒的操作，大大减小总耗时。且可根据业务逻辑实时判断是否需要继续执行futureTask。
#### demo2：当然FutureTask一样可以并发执行任务并获取结果，如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 package thread.future;
 2 
 3 import java.util.ArrayList;
 4 import java.util.Date;
 5 import java.util.List;
 6 import java.util.concurrent.Callable;
 7 import java.util.concurrent.ExecutorService;
 8 import java.util.concurrent.Executors;
 9 import java.util.concurrent.FutureTask;
10 
11 /**
12  * 
13  * @ClassName:FutureTaskDemo
14  * @Description:FutureTask实现多线程并发执行任务并取结果归集
15  * @author diandian.zhang
16  * @date 2017年6月16日上午10:36:05
17  */
18 public class FutureTaskDemo {
19     
20     public static void main(String[] args)  {
21         Long start = System.currentTimeMillis();
22         //开启多线程
23         ExecutorService exs = Executors.newFixedThreadPool(5);
24         try {
25             //结果集
26             List<Integer> list = new ArrayList<Integer>();
27             List<FutureTask<Integer>> futureList = new ArrayList<FutureTask<Integer>>();
28             //启动线程池，10个任务固定线程数为5
29             for(int i=0;i<10;i++){
30                  FutureTask<Integer> futureTask = new FutureTask<Integer>(new CallableTask(i+1));
31                 //提交任务，添加返回
32                 exs.submit(futureTask);//Runnable特性
33                 futureList.add(futureTask);//Future特性
34             }
35             Long getResultStart = System.currentTimeMillis();
36             System.out.println("结果归集开始时间="+new Date());
37             //结果归集
38             for (FutureTask<Integer> future : futureList) {
39                 while (true) {
40                     if (future.isDone()&& !future.isCancelled()) {
41                         Integer i = future.get();//Future特性
42                         System.out.println("i="+i+"获取到结果!"+new Date());
43                         list.add(i);
44                         break;
45                     }else {
46                         Thread.sleep(1);//避免CPU高速轮循，可以休息一下。
47                     }
48                 }
49             }
50             System.out.println("list="+list);
51             System.out.println("总耗时="+(System.currentTimeMillis()-start)+",取结果归集耗时="+(System.currentTimeMillis()-getResultStart));
52         } catch (Exception e) {
53             e.printStackTrace();
54         } finally {
55             exs.shutdown();
56         }
57         
58     }
59 
60     static class CallableTask implements Callable<Integer>{
61         Integer i;
62         
63         public CallableTask(Integer i) {
64             super();
65             this.i=i;
66         }
67 
68         @Override
69         public Integer call() throws Exception {
70             if(i==1){
71                 Thread.sleep(3000);//任务1耗时3秒
72             }else if(i==5){
73                 Thread.sleep(5000);//任务5耗时5秒
74             }else{
75                 Thread.sleep(1000);//其它任务耗时1秒
76             }
77             System.out.println("线程：["+Thread.currentThread().getName()+"]任务i="+i+",完成！"+new Date());  
78             return i;
79 
80         }
81         
82     }
83 
84 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
运行结果就不打印了，和Future一样的。因为结果归集用了Future<v>特性。
### 建议：demo1在特定场合例如有十分耗时的业务但有依赖于其他业务不一定非要执行的，可以尝试使用。demo2多线程并发执行并结果归集，这里多套一层FutureTask比较鸡肋（直接返回Future简单明了）不建议使用。
[回到顶部](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_labelTop)
## 3.CompletionService：
原理:内部通过阻塞队列+FutureTask，实现了**任务先完成可优先获取到，即结果按照完成先后顺序排序。**
#### demo:
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 package thread.future;
 2 
 3 import java.util.ArrayList;
 4 import java.util.Date;
 5 import java.util.List;
 6 import java.util.concurrent.Callable;
 7 import java.util.concurrent.CompletionService;
 8 import java.util.concurrent.ExecutorCompletionService;
 9 import java.util.concurrent.ExecutorService;
10 import java.util.concurrent.Executors;
11 import java.util.concurrent.Future;
12 
13 /**
14  * 
15  * @ClassName: CompletionServiceDemo
16  * @Description: CompletionService多线程并发任务结果归集
17  * @author denny.zhang
18  * @date 2016年11月4日 下午1:50:32
19  *
20  */
21 public class CompletionServiceDemo{
22 
23     public static void main(String[] args)  {
24         Long start = System.currentTimeMillis();
25         //开启3个线程
26         ExecutorService exs = Executors.newFixedThreadPool(5);
27         try {
28             int taskCount = 10;
29             //结果集
30             List<Integer> list = new ArrayList<Integer>();
31             //1.定义CompletionService
32             CompletionService<Integer> completionService = new ExecutorCompletionService<Integer>(exs);  
33             List<Future<Integer>> futureList = new ArrayList<Future<Integer>>();
34             //2.添加任务
35             for(int i=0;i<taskCount;i++){
36                 futureList.add(completionService.submit(new Task(i+1)));
37             }
38             //==================结果归集===================
39             //方法1：future是提交时返回的，遍历queue则按照任务提交顺序，获取结果
40 //            for (Future<Integer> future : futureList) {
41 //                System.out.println("====================");
42 //                Integer result = future.get();//线程在这里阻塞等待该任务执行完毕,按照
43 //                System.out.println("任务result="+result+"获取到结果!"+new Date());
44 //                list.add(result);
45 //            }
46 
47 //            //方法2.使用内部阻塞队列的take()
48             for(int i=0;i<taskCount;i++){
49                 Integer result = completionService.take().get();//采用completionService.take()，内部维护阻塞队列，任务先完成的先获取到
50                 System.out.println("任务i=="+result+"完成!"+new Date());
51                 list.add(result);
52             }
53             System.out.println("list="+list);
54             System.out.println("总耗时="+(System.currentTimeMillis()-start));
55         } catch (Exception e) {
56             e.printStackTrace();
57         } finally {
58             exs.shutdown();//关闭线程池
59         }
60         
61     }
62 
63     static class Task implements Callable<Integer>{
64         Integer i;
65         
66         public Task(Integer i) {
67             super();
68             this.i=i;
69         }
70 
71         @Override
72         public Integer call() throws Exception {
73             if(i==5){
74                 Thread.sleep(5000);
75             }else{
76                 Thread.sleep(1000);
77             }
78             System.out.println("线程："+Thread.currentThread().getName()+"任务i="+i+",执行完成！");  
79             return i;
80         }
81         
82     }
83 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
打印结果如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
线程：pool-1-thread-3任务i=3,执行完成！
线程：pool-1-thread-1任务i=1,执行完成！
线程：pool-1-thread-4任务i=4,执行完成！
线程：pool-1-thread-2任务i=2,执行完成！
任务i==3完成!Fri Jun 16 11:39:17 CST 2017
任务i==1完成!Fri Jun 16 11:39:17 CST 2017
任务i==4完成!Fri Jun 16 11:39:17 CST 2017
任务i==2完成!Fri Jun 16 11:39:17 CST 2017
线程：pool-1-thread-4任务i=8,执行完成！
线程：pool-1-thread-3任务i=7,执行完成！
线程：pool-1-thread-1任务i=6,执行完成！
线程：pool-1-thread-2任务i=9,执行完成！
任务i==8完成!Fri Jun 16 11:39:18 CST 2017
任务i==7完成!Fri Jun 16 11:39:18 CST 2017
任务i==6完成!Fri Jun 16 11:39:18 CST 2017
任务i==9完成!Fri Jun 16 11:39:18 CST 2017
线程：pool-1-thread-3任务i=10,执行完成！
任务i==10完成!Fri Jun 16 11:39:19 CST 2017
线程：pool-1-thread-5任务i=5,执行完成！
任务i==5完成!Fri Jun 16 11:39:21 CST 2017
list=[3, 1, 4, 2, 8, 7, 6, 9, 10, 5]---》这里证实了确实按照执行完成顺序排序
总耗时=5004---》符合逻辑，10个任务，定长5线程池执行，取最长时间。
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 
#### 建议：使用率也挺高，而且能按照完成先后排序，建议**如果有排序需求的优先使用**。只是多线程并发执行任务结果归集，也可以使用。
[回到顶部](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_labelTop)
## 4.CompletableFuture
### 原理：
**4.1.从注释看**：
JDK1.8才新加入的一个实现类，实现了Future<T>, CompletionStage<T>2个接口，JDK注释如下图：
![](https://images2015.cnblogs.com/blog/584866/201706/584866-20170616125956743-898483095.png)
译文（没兴趣的可以跳过）：
当一个Future可能需要显示地完成时，使用CompletionStage接口去支持完成时触发的函数和操作。当2个以上线程同时尝试完成、异常完成、取消一个CompletableFuture时，只有一个能成功。
CompletableFuture实现了CompletionStage接口的如下策略：
1.为了完成当前的CompletableFuture接口或者其他完成方法的回调函数的线程，提供了非异步的完成操作。
2.没有显式入参Executor的所有async方法都使用`ForkJoinPool.commonPool()`为了简化监视、调试和跟踪，所有生成的异步任务都是标记接口`AsynchronousCompletionTask`的实例。
3.所有的CompletionStage方法都是独立于其他共有方法实现的，因此一个方法的行为不会受到子类中其他方法的覆盖。
CompletableFuture实现了Futurre接口的如下策略：
1.CompletableFuture无法直接控制完成，所以cancel操作被视为是另一种异常完成形式。方法`isCompletedExceptionally`可以用来确定一个CompletableFuture是否以任何异常的方式完成。
2.以一个CompletionException为例，方法get()和get(long,TimeUnit)抛出一个ExecutionException，对应CompletionException。为了在大多数上下文中简化用法，这个类还定义了方法join()和getNow，而不是直接在这些情况中直接抛出CompletionException。
**4.2.CompletionStage接口实现流式编程：**
JDK8新增接口，此接口包含38个方法...是的，你没看错，就是38个方法。这些方法主要是为了支持函数式编程中流式处理。
**4.3.CompletableFuture中4个异步执行任务静态方法：**
![](https://images2015.cnblogs.com/blog/584866/201706/584866-20170619100623476-158522093.png)
如上图，其中supplyAsync用于有返回值的任务，runAsync则用于没有返回值的任务。Executor参数可以手动指定线程池，否则默认`ForkJoinPool.commonPool()系统级公共线程池，注意：这些线程都是Daemon线程，主线程结束Daemon线程不结束，只有JVM关闭时，生命周期终止`。
**4.4.组合CompletableFuture：**
**thenCombine()： ****先完成当前CompletionStage和other 2个**CompletionStage任务**，然后把结果传参给BiFunction进行结果合并操作。**
**![](https://images2015.cnblogs.com/blog/584866/201706/584866-20170619104403945-714822781.png)**
**thenCompose()：第一个CompletableFuture执行完毕后，传递给下一个CompletionStage作为入参进行操作。**
![](https://images2015.cnblogs.com/blog/584866/201706/584866-20170619104428866-1469016511.png)
#### demo:
JDK CompletableFuture  自带多任务组合方法allOf和anyOf
**allOf**是等待所有任务完成，构造后CompletableFuture完成
**anyOf**是只要有一个任务完成，构造后CompletableFuture就完成
![](https://images2015.cnblogs.com/blog/584866/201706/584866-20170619183059085-459009717.png)
方式一：循环创建CompletableFuture list,调用sequence()组装返回一个有返回值的CompletableFuture，返回结果get()获取
方式二：全流式处理转换成CompletableFuture[]+**allOf**组装成一个无返回值CompletableFuture，join等待执行完毕。返回结果whenComplete获取。---》推荐
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
  1 package thread.future;
  2 
  3 import java.util.ArrayList;
  4 import java.util.Date;
  5 import java.util.List;
  6 import java.util.concurrent.CompletableFuture;
  7 import java.util.concurrent.ExecutorService;
  8 import java.util.concurrent.Executors;
  9 import java.util.stream.Collectors;
 10 import java.util.stream.Stream;
 11 
 12 import com.google.common.collect.Lists;
 13 
 14 /**
 15  * 
 16  * @ClassName:CompletableFutureDemo
 17  * @Description:多线程并发任务,取结果归集
 18  * @author diandian.zhang
 19  * @date 2017年6月14日下午12:44:01
 20  */
 21 public class CompletableFutureDemo {
 22     public static void main(String[] args) {
 23         Long start = System.currentTimeMillis();
 24         //结果集
 25         List<String> list = new ArrayList<String>();
 26         List<String> list2 = new ArrayList<String>();
 27         //定长10线程池
 28         ExecutorService exs = Executors.newFixedThreadPool(10);
 29         //List<CompletableFuture<String>> futureList = new ArrayList<>();
 30         List<Integer> taskList = Lists.newArrayList(2,1,3,4,5,6,7,8,9,10);
 31         try {
 32             //方式一：循环创建CompletableFuture list,调用sequence()组装返回一个有返回值的CompletableFuture，返回结果get()获取
 33 //            for(int i=0;i<taskList.size();i++){
 34 //                final int j=i+1;
 35 //                CompletableFuture<String> future = CompletableFuture.supplyAsync(()->calc(j), exs)//异步执行
 36 //                        .thenApply(e->Integer.toString(e))//Integer转换字符串    thenAccept只接受不返回不影响结果
 37 ////                        .whenComplete((v, e) -> {//如需获取任务完成先手顺序，此处代码即可
 38 ////                            System.out.println("任务"+v+"完成!result="+v+"，异常 e="+e+","+new Date());
 39 ////                            list2.add(v);
 40 ////                        })
 41 //                        ;
 42 //                futureList.add(future);
 43 //            }
 44 //            //流式获取结果
 45 //            list = sequence(futureList).get();//[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]此处不理解为什么是这样的顺序？谁知道求告知
 46             
 47             //方式二：全流式处理转换成CompletableFuture[]+组装成一个无返回值CompletableFuture，join等待执行完毕。返回结果whenComplete获取
 48             @SuppressWarnings("rawtypes")
 49             CompletableFuture[] cfs = taskList.stream().map(object-> CompletableFuture.supplyAsync(()->calc(object), exs)
 50                     .thenApply(h->Integer.toString(h))
 51                     .whenComplete((v, e) -> {//如需获取任务完成先手顺序，此处代码即可
 52                         System.out.println("任务"+v+"完成!result="+v+"，异常 e="+e+","+new Date());
 53                         list2.add(v);
 54                     }))
 55                     .toArray(CompletableFuture[]::new);
 56             CompletableFuture.allOf(cfs).join();//封装后无返回值，必须自己whenComplete()获取
 57             System.out.println("list2="+list2+"list="+list+",耗时="+(System.currentTimeMillis()-start));
 58         } catch (Exception e) {
 59             e.printStackTrace();
 60         }finally {
 61             exs.shutdown();
 62         }
 63     }
 64     
 65     public static Integer calc(Integer i){
 66         try {
 67             if(i==1){
 68                 Thread.sleep(3000);//任务1耗时3秒
 69             }else if(i==5){
 70                 Thread.sleep(5000);//任务5耗时5秒
 71             }else{
 72                 Thread.sleep(1000);//其它任务耗时1秒
 73             }
 74             System.out.println("task线程："+Thread.currentThread().getName()+"任务i="+i+",完成！+"+new Date());  
 75         } catch (InterruptedException e) {
 76             e.printStackTrace();
 77         }
 78         return i;
 79     }
 80     
 81     /**
 82      * 
 83      * @Description 组合多个CompletableFuture为一个CompletableFuture,所有子任务全部完成，组合后的任务才会完成。带返回值，可直接get.
 84      * @param futures List
 85      * @return
 86      * @author diandian.zhang
 87      * @date 2017年6月19日下午3:01:09
 88      * @since JDK1.8
 89      */
 90     public static <T> CompletableFuture<List<T>> sequence(List<CompletableFuture<T>> futures) {
 91         //1.构造一个空CompletableFuture，子任务数为入参任务list size
 92         CompletableFuture<Void> allDoneFuture = CompletableFuture.allOf(futures.toArray(new CompletableFuture[futures.size()]));
 93         //2.流式（每个子任务join操作后转换为list）往空CompletableFuture中添加结果
 94         return allDoneFuture.thenApply(v -> futures.stream().map(CompletableFuture::join).collect(Collectors.<T>toList()));
 95     }
 96    
 97     /**
 98      * 
 99      * @Description Stream流式类型futures转换成一个CompletableFuture,所有子任务全部完成，组合后的任务才会完成。带返回值，可直接get.
100      * @param futures Stream
101      * @return
102      * @author diandian.zhang
103      * @date 2017年6月19日下午6:23:40
104      * @since JDK1.8
105      */
106     public static <T> CompletableFuture<List<T>> sequence(Stream<CompletableFuture<T>> futures) {
107         List<CompletableFuture<T>> futureList = futures.filter(f -> f != null).collect(Collectors.toList());
108         return sequence(futureList);
109     }
110 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
方式二返回结果：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
task线程：pool-1-thread-1任务i=2,完成！+Mon Jun 19 18:26:17 CST 2017
task线程：pool-1-thread-9任务i=9,完成！+Mon Jun 19 18:26:17 CST 2017
task线程：pool-1-thread-6任务i=6,完成！+Mon Jun 19 18:26:17 CST 2017
task线程：pool-1-thread-8任务i=8,完成！+Mon Jun 19 18:26:17 CST 2017
任务6完成!result=6，异常 e=null,Mon Jun 19 18:26:17 CST 2017
task线程：pool-1-thread-4任务i=4,完成！+Mon Jun 19 18:26:17 CST 2017
task线程：pool-1-thread-7任务i=7,完成！+Mon Jun 19 18:26:17 CST 2017
任务4完成!result=4，异常 e=null,Mon Jun 19 18:26:17 CST 2017
task线程：pool-1-thread-3任务i=3,完成！+Mon Jun 19 18:26:17 CST 2017
任务3完成!result=3，异常 e=null,Mon Jun 19 18:26:17 CST 2017
task线程：pool-1-thread-10任务i=10,完成！+Mon Jun 19 18:26:17 CST 2017
任务10完成!result=10，异常 e=null,Mon Jun 19 18:26:17 CST 2017
任务7完成!result=7，异常 e=null,Mon Jun 19 18:26:17 CST 2017
任务8完成!result=8，异常 e=null,Mon Jun 19 18:26:17 CST 2017
任务2完成!result=2，异常 e=null,Mon Jun 19 18:26:17 CST 2017
任务9完成!result=9，异常 e=null,Mon Jun 19 18:26:17 CST 2017
task线程：pool-1-thread-2任务i=1,完成！+Mon Jun 19 18:26:19 CST 2017---》任务1耗时3秒
任务1完成!result=1，异常 e=null,Mon Jun 19 18:26:19 CST 2017
task线程：pool-1-thread-5任务i=5,完成！+Mon Jun 19 18:26:21 CST 2017---》任务5耗时5秒
任务5完成!result=5，异常 e=null,Mon Jun 19 18:26:21 CST 2017
list2=[6, 4, 3, 10, 7, 8, 2, 9, 1, 5]list=[],耗时=5076---》符合逻辑，10个任务，10个线程并发执行，其中任务1耗时3秒，任务5耗时5秒，耗时取最大值。
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 
#### 建议：CompletableFuture满足并发执行，顺序完成先手顺序获取的目标。而且支持每个任务的异常返回，配合流式编程，用起来速度飞起。JDK源生支持，API丰富，推荐使用。
[回到顶部](https://www.cnblogs.com/dennyzhangdd/p/7010972.html#_labelTop)
## 5.总结：
本文从原理、demo、建议三个方向分析了常用多线程并发，取结果归集的几种实现方案，希望对大家有所启发，整理表格如下：
|###|### Futrue|### FutureTask|### CompletionService|### **CompletableFuture**|
|----|----|----|----|----|
|**原理**|Futrue接口|接口RunnableFuture的唯一实现类，RunnableFuture接口继承自Future<V>+Runnable:|内部通过阻塞队列+FutureTask接口|JDK8实现了Future<T>, CompletionStage<T>2个接口|
|**多任务并发执行**|支持|支持|支持|支持|
|**获取任务结果的顺序**|按照提交顺序获取结果|未知|支持任务完成的先后顺序|支持任务完成的先后顺序|
|**异常捕捉**|自己捕捉|自己捕捉|自己捕捉|源生API支持，返回每个任务的异常|
|**建议**|CPU高速轮询，耗资源，可以使用，但不推荐|功能不对口，并发任务这一块多套一层，不推荐使用。|推荐使用，没有JDK8**CompletableFuture**之前最好的方案，没有质疑|**API极端丰富，配合流式编程，速度飞起，推荐使用！**|
```
```
```
```
```
```
```
```
```
```
```
```
