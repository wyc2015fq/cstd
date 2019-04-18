# 线程池ThreadPoolExecutor使用简介 - z69183787的专栏 - CSDN博客
2016年11月06日 17:27:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1051
一、简介 
线程池类为 java.util.concurrent.ThreadPoolExecutor，常用构造方法为： 
ThreadPoolExecutor(int corePoolSize, int maximumPoolSize, 
long keepAliveTime, TimeUnit unit, 
BlockingQueue workQueue, 
RejectedExecutionHandler handler) 
corePoolSize： 线程池维护线程的最少数量 
maximumPoolSize：线程池维护线程的最大数量 
keepAliveTime： 线程池维护线程所允许的空闲时间 
unit： 线程池维护线程所允许的空闲时间的单位 
workQueue： 线程池所使用的缓冲队列 
handler： 线程池对拒绝任务的处理策略 
一个任务通过 execute(Runnable)方法被添加到线程池，任务就是一个 Runnable类型的对象，任务的执行方法就是 Runnable类型对象的run()方法。 
当一个任务通过execute(Runnable)方法欲添加到线程池时： 
如果此时线程池中的数量小于corePoolSize，即使线程池中的线程都处于空闲状态，也要创建新的线程来处理被添加的任务。 
如果此时线程池中的数量等于 corePoolSize，但是缓冲队列 workQueue未满，那么任务被放入缓冲队列。 
如果此时线程池中的数量大于corePoolSize，缓冲队列workQueue满，并且线程池中的数量小于maximumPoolSize，建新的线程来处理被添加的任务。 
如果此时线程池中的数量大于corePoolSize，缓冲队列workQueue满，并且线程池中的数量等于maximumPoolSize，那么通过 handler所指定的策略来处理此任务。 
也就是：处理任务的优先级为： 
核心线程corePoolSize、任务队列workQueue、最大线程maximumPoolSize，如果三者都满了，使用handler处理被拒绝的任务。 
当线程池中的线程数量大于 corePoolSize时，如果某线程空闲时间超过keepAliveTime，线程将被终止。这样，线程池可以动态的调整池中的线程数。 
unit可选的参数为java.util.concurrent.TimeUnit中的几个静态属性： 
NANOSECONDS、MICROSECONDS、MILLISECONDS、SECONDS。 
workQueue我常用的是：java.util.concurrent.ArrayBlockingQueue 
handler有四个选择： 
ThreadPoolExecutor.AbortPolicy() 
抛出java.util.concurrent.RejectedExecutionException异常 
ThreadPoolExecutor.CallerRunsPolicy() 
重试添加当前的任务，他会自动重复调用execute()方法 
ThreadPoolExecutor.DiscardOldestPolicy() 
抛弃旧的任务 
ThreadPoolExecutor.DiscardPolicy() 
抛弃当前的任务 
二、一般用法举例 
点击(此处)折叠或打开
- package demo;
- 
- importjava.io.Serializable;
- importjava.util.concurrent.ArrayBlockingQueue;
- importjava.util.concurrent.ThreadPoolExecutor;
- importjava.util.concurrent.TimeUnit;
- 
- publicclass TestThreadPool2
- {
- privatestaticint produceTaskSleepTime = 2;
- privatestaticint produceTaskMaxNumber = 10;
- 
- publicstaticvoid main(String[] args)
- {
- // 构造一个线程池
- ThreadPoolExecutor threadPool =newThreadPoolExecutor(2, 4, 3,TimeUnit.SECONDS,newArrayBlockingQueue<Runnable>(3),
- newThreadPoolExecutor.DiscardOldestPolicy());
- 
- for(int i = 1; i <= produceTaskMaxNumber; i++)
- {
- try
- {
- // 产生一个任务，并将其加入到线程池
- String task ="task@ "+ i;
- System.out.println("put
 "+ task);
-                 threadPool.execute(new ThreadPoolTask(task));
- 
- // 便于观察，等待一段时间
- Thread.sleep(produceTaskSleepTime);
- }
- catch(Exception e)
- {
-                 e.printStackTrace();
- }
- }
- }
- }
- 
- /**
-  * 线程池执行的任务
-  */
- class ThreadPoolTask implementsRunnable,Serializable
- {
- privatestaticfinallong serialVersionUID = 0;
- privatestaticint consumeTaskSleepTime = 2000;
- // 保存任务所需要的数据
- privateObject threadPoolTaskData;
- 
-     ThreadPoolTask(Object tasks)
- {
- this.threadPoolTaskData = tasks;
- }
- 
- publicvoidrun()
- {
- // 处理一个任务，这里的处理方式太简单了，仅仅是一个打印语句
- System.out.println(Thread.currentThread().getName());
- System.out.println("start
 .."+ threadPoolTaskData);
- 
- try
- {
- // //便于观察，等待一段时间
- Thread.sleep(consumeTaskSleepTime);
- }
- catch(Exception e)
- {
-             e.printStackTrace();
- }
-         threadPoolTaskData =null;
- }
- 
- publicObject getTask()
- {
- returnthis.threadPoolTaskData;
- }
- }
说明： 
1、在这段程序中，一个任务就是一个Runnable类型的对象，也就是一个ThreadPoolTask类型的对象。 
2、一般来说任务除了处理方式外，还需要处理的数据，处理的数据通过构造方法传给任务。 
3、在这段程序中，main()方法相当于一个残忍的领导，他派发出许多任务，丢给一个叫 threadPool的任劳任怨的小组来做。 
这个小组里面队员至少有两个，如果他们两个忙不过来，任务就被放到任务列表里面。 
如果积压的任务过多，多到任务列表都装不下(超过3个)的时候，就雇佣新的队员来帮忙。但是基于成本的考虑，不能雇佣太多的队员，至多只能雇佣 4个。 
如果四个队员都在忙时，再有新的任务，这个小组就处理不了了，任务就会被通过一种策略来处理，我们的处理方式是不停的派发，直到接受这个任务为止(更残忍！呵呵)。 
因为队员工作是需要成本的，如果工作很闲，闲到 3SECONDS都没有新的任务了，那么有的队员就会被解雇了，但是，为了小组的正常运转，即使工作再闲，小组的队员也不能少于两个。 
4、通过调整 produceTaskSleepTime和 consumeTaskSleepTime的大小来实现对派发任务和处理任务的速度的控制，改变这两个值就可以观察不同速率下程序的工作情况。 
5、通过调整4中所指的数据，再加上调整任务丢弃策略，换上其他三种策略，就可以看出不同策略下的不同处理方式。 
6、对于其他的使用方法，参看jdk的帮助，很容易理解和使用。 
另一个例子: 
点击(此处)折叠或打开
- package demo;
- 
- importjava.util.Queue;
- importjava.util.concurrent.ArrayBlockingQueue;
- importjava.util.concurrent.ThreadPoolExecutor;
- importjava.util.concurrent.TimeUnit;
- 
- publicclass ThreadPoolExecutorTest
- {
- 
- privatestaticint queueDeep = 4;
- 
- publicvoid createThreadPool()
- {
- /* 
-          * 创建线程池，最小线程数为2，最大线程数为4，线程池维护线程的空闲时间为3秒， 
-          * 使用队列深度为4的有界队列，如果执行程序尚未关闭，则位于工作队列头部的任务将被删除， 
-          * 然后重试执行程序（如果再次失败，则重复此过程），里面已经根据队列深度对任务加载进行了控制。 
-          */ 
- ThreadPoolExecutor tpe =newThreadPoolExecutor(2, 4, 3,TimeUnit.SECONDS,newArrayBlockingQueue<Runnable>(queueDeep),
- newThreadPoolExecutor.DiscardOldestPolicy());
- 
- // 向线程池中添加 10 个任务
- for(int i = 0; i < 10; i++)
- {
- try
- {
- Thread.sleep(1);
- }
- catch(InterruptedException e)
- {
-                 e.printStackTrace();
- }
- while(getQueueSize(tpe.getQueue())>= queueDeep)
- {
- System.out.println("队列已满，等3秒再添加任务");
- try
- {
- Thread.sleep(3000);
- }
- catch(InterruptedException e)
- {
-                     e.printStackTrace();
- }
- }
-             TaskThreadPool ttp =new TaskThreadPool(i);
- System.out.println("put
 i:"+ i);
-             tpe.execute(ttp);
- }
- 
-         tpe.shutdown();
- }
- 
- privatesynchronizedint getQueueSize(Queuequeue)
- {
- returnqueue.size();
- }
- 
- publicstaticvoid main(String[] args)
- {
-         ThreadPoolExecutorTest test =new ThreadPoolExecutorTest();
-         test.createThreadPool();
- }
- 
- class TaskThreadPool implementsRunnable
- {
- privateint index;
- 
- public TaskThreadPool(int index)
- {
- this.index = index;
- }
- 
- publicvoidrun()
- {
- System.out.println(Thread.currentThread()+"
 index:"+ index);
- try
- {
- Thread.sleep(3000);
- }
- catch(InterruptedException e)
- {
-                 e.printStackTrace();
- }
- }
- }
- }
