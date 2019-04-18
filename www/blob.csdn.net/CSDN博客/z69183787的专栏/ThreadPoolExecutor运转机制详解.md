# ThreadPoolExecutor运转机制详解 - z69183787的专栏 - CSDN博客
2015年07月21日 14:48:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：697
个人分类：[定时任务调度](https://blog.csdn.net/z69183787/article/category/2175967)

最近发现几起对ThreadPoolExecutor的误用，其中包括自己，发现都是因为没有仔细看注释和内部运转机制，想当然的揣测参数导致，先看一下新建一个ThreadPoolExecutor的构建参数：
**[java]**[view
 plain](http://blog.csdn.net/cutesource/article/details/6061229#)[copy](http://blog.csdn.net/cutesource/article/details/6061229#)
- public ThreadPoolExecutor(int corePoolSize,  
- int maximumPoolSize,  
- long keepAliveTime,  
-                           TimeUnit unit,  
-                           BlockingQueue<Runnable> workQueue,  
-                           ThreadFactory threadFactory,  
-                           RejectedExecutionHandler handler)  
看这个参数很容易让人以为是线程池里保持corePoolSize个线程，如果不够用，就加线程入池直至maximumPoolSize大小，如果还不够就往workQueue里加，如果workQueue也不够就用RejectedExecutionHandler来做拒绝处理。
但实际情况不是这样，具体流程如下：
1）当池子大小小于corePoolSize就新建线程，并处理请求
2）当池子大小等于corePoolSize，把请求放入workQueue中，池子里的空闲线程就去从workQueue中取任务并处理
3）当workQueue放不下新入的任务时，新建线程入池，并处理请求，如果池子大小撑到了maximumPoolSize就用RejectedExecutionHandler来做拒绝处理
4）另外，当池子的线程数大于corePoolSize的时候，多余的线程会等待keepAliveTime长的时间，如果无请求可处理就自行销毁
内部结构如下所示：
![](http://hi.csdn.net/attachment/201012/7/0_12917139691L3R.gif)
从中可以发现ThreadPoolExecutor就是依靠BlockingQueue的阻塞机制来维持线程池，当池子里的线程无事可干的时候就通过workQueue.take()阻塞住。
其实可以通过Executes来学学几种特殊的ThreadPoolExecutor是如何构建的。
**[java]**[view
 plain](http://blog.csdn.net/cutesource/article/details/6061229#)[copy](http://blog.csdn.net/cutesource/article/details/6061229#)
- publicstatic ExecutorService newFixedThreadPool(int nThreads) {  
- returnnew ThreadPoolExecutor(nThreads, nThreads,  
-                                   0L, TimeUnit.MILLISECONDS,  
- new LinkedBlockingQueue<Runnable>());  
- }  
newFixedThreadPool就是一个固定大小的ThreadPool
**[java]**[view
 plain](http://blog.csdn.net/cutesource/article/details/6061229#)[copy](http://blog.csdn.net/cutesource/article/details/6061229#)
- publicstatic ExecutorService newCachedThreadPool() {  
- returnnew ThreadPoolExecutor(0, Integer.MAX_VALUE,  
-                                   60L, TimeUnit.SECONDS,  
- new SynchronousQueue<Runnable>());  
- }  
newCachedThreadPool比较适合没有固定大小并且比较快速就能完成的小任务，没必要维持一个Pool，这比直接new Thread来处理的好处是能在60秒内重用已创建的线程。
其他类型的ThreadPool看看构建参数再结合上面所说的特性就大致知道它的特性
线程池的作用就是用尽可能少的线程来执行尽可能多的Runnable，以实现对线程的充分利用。
先从ThreadPoolExecutor类的构造方法说起：
- ThreadPoolExecutor
- publicThreadPoolExecutor(int corePoolSize,// 核心线程数
- int maximumPoolSize,// 最大线程数
- long keepAliveTime,// 生存时间
- TimeUnit unit,// keepAliveTime值的度量单位
- BlockingQueue<Runnable> workQueue)// 阻塞队列
我们通过ThreadPoolExecutor类的execute()方法添加我们要执行的任务。下面通过实例讲解ThreadPoolExecutor类的使用。
任务类Task：将会被线程池执行的任务类
- classTask{
- publicvoid fun(){
- for(int i =0; i <10; i++){
- System.out.println(Thread.currentThread().getName()+" 执行 - "+ i);
- }
- }
- }
在创建线程池之前，我们先了解一下BlockingQueue，在构造线程池的时候，需要为其指定一个BlockingQueue，可用于传输和保持提交的任务。可以使用此队列与池大小进行交互。
## BlockingQueue(阻塞队列):
如果运行的线程少于 corePoolSize，则 Executor始终首选添加新的线程，而不进行排队(不添加进阻塞队列)。
如果运行的线程等于或多于 corePoolSize，则 Executor 始终首选将请求加入队列，而不添加新的线程。
如果无法将请求加入队列，则创建新的线程，除非创建此线程超出 maximumPoolSize，在这种情况下，任务将被拒绝。
## 排队有三种通用策略：
1：直接提交SynchronousQueue
它将任务直接提交给线程而不保存它们。在此，如果不存在可用于立即运行任务的线程，
则试图把任务加入队列将失败，因此会构造一个新的线程。此策略可以避免在处理可能具有内部依赖性的请求集时出现锁。
直接提交通常要求无界 maximumPoolSizes 以避免拒绝新提交的任务。
当命令以超过队列所能处理的平均数连续到达时，此策略允许无界线程具有增长的可能性。  SynchronousQueue线程安全的Queue，可以存放若干任务（但当前只允许有且只有一个任务在等待），
其中每个插入操作必须等待另一个线程的对应移除操作，也就是说A任务进入队列，B任务必须等A任务被移除之后才能进入队列，否则执行异常策略。
你来一个我扔一个，所以说SynchronousQueue没有任何内部容量。
比如：核心线程数为2，最大线程数为3；使用SynchronousQueue。
当前有2个核心线程在运行，又来了个A任务，两个核心线程没有执行完当前任务，根据如果运行的线程等于或多于 corePoolSize，
则 Executor 始终首选将请求加入队列，而不添加新的线程。所以A任务被添加到队列，此时的队列是SynchronousQueue，
当前不存在可用于立即运行任务的线程，因此会构造一个新的线程，此时又来了个B任务，两个核心线程还没有执行完。
新创建的线程正在执行A任务，所以B任务进入Queue后，最大线程数为3，发现没地方仍了。就只能执行异常策略(RejectedExecutionException)。
代码示例：
- publicclassPoolExecutorTest{
- publicstaticvoid main(String[] args){
- finalTask task =newTask();
- ThreadPoolExecutor pool =
- newThreadPoolExecutor(2,3,3L,
- TimeUnit.SECONDS,newSynchronousQueue<Runnable>(),newThreadPoolExecutor.AbortPolicy());
- for(int i =0; i <4; i++){
-             pool.execute(newRunnable(){
- @Override
- publicvoid run(){
-                     task.fun();
- System.out.println("run");
- }
- });
- }
-         pool.shutdown();
- }
- }// 在添加第4个任务的时候，程序抛出 java.util.concurrent.RejectedExecutionException
2：无界队列如LinkedBlockingQueue
使用无界队列（例如，不具有预定义容量的 LinkedBlockingQueue）将导致在所有核心线程都在忙时新任务在队列中等待。
这样，创建的线程就不会超过 corePoolSize。（因此，maximumPoolSize 的值也就没意义了。）也就不会有新线程被创建，都在那等着排队呢。
如果未指定容量，则它等于 Integer.MAX_VALUE。
如果设置了Queue预定义容量，则当核心线程忙碌时，新任务会在队列中等待，直到超过预定义容量(新任务没地方放了)，才会执行异常策略。
你来一个我接一个，直到我容不下你了。FIFO，先进先出。
比如：核心线程数为2，最大线程数为3；使用LinkedBlockingQueue(1)，设置容量为1。
当前有2个核心线程在运行，又来了个A任务，两个核心线程没有执行完当前任务，根据如果运行的线程等于或多于 corePoolSize，
则 Executor 始终首选将请求加入队列，而不添加新的线程。所以A任务被添加到队列，此时的队列是LinkedBlockingQueue，
此时又来了个B任务，两个核心线程没有执行完当前任务，A任务在队列中等待，队列已满。所以根据如果无法将请求加入队列，则创建新的线程，
B任务被新创建的线程所执行，此时又来个C任务，此时maximumPoolSize已满，队列已满，只能执行异常策略(RejectedExecutionException)。
代码示例：
- publicclassPoolExecutorTest{
- publicstaticvoid main(String[] args){
- finalTask task =newTask();
- ThreadPoolExecutor pool =
- newThreadPoolExecutor(2,3,3L,
- TimeUnit.SECONDS,newLinkedBlockingQueue<Runnable>(1),newThreadPoolExecutor.AbortPolicy());
- for(int i =0; i <5; i++){
- 			pool.execute(newRunnable(){
- @Override
- publicvoid run(){
- 					task.fun();
- System.out.println("run");
- }
- });
- }
- 		pool.shutdown();
- }
- }
- // 在添加第5个任务的时候，程序抛出 java.util.concurrent.RejectedExecutionException
无界的话要防止任务增长的速度远远超过处理任务的速度。
3：有界队列如ArrayBlockingQueue
操作模式跟LinkedBlockingQueue查不多，只不过必须为其设置容量。所以叫有界队列。
new ArrayBlockingQueue<Runnable>(Integer.MAX_VALUE) 跟 new LinkedBlockingQueue(Integer.MAX_VALUE)效果一样。
LinkedBlockingQueue 底层是链表结构
ArrayBlockingQueue  底层是数组结构
你来一个我接一个，直到我容不下你了。FIFO，先进先出。
代码示例：
- publicclassPoolExecutorTest{
- publicstaticvoid main(String[] args){
- finalTask task =newTask();
- ThreadPoolExecutor pool =
- newThreadPoolExecutor(2,3,3L,
- TimeUnit.SECONDS,newArrayBlockingQueue<Runnable>(1),newThreadPoolExecutor.AbortPolicy());
- for(int i =0; i <5; i++){
- 			pool.execute(newRunnable(){
- @Override
- publicvoid run(){
- 					task.fun();
- System.out.println("run");
- }
- });
- }
- 		pool.shutdown();
- }
- }
- // 在添加第5个任务的时候，程序抛出 java.util.concurrent.RejectedExecutionException
关于keepAliveTime：
JDK解释：当线程数大于核心时，此为终止前多余的空闲线程等待新任务的最长时间。
也就是说啊，线程池中当前的空闲线程服务完某任务后的存活时间。如果时间足够长，那么可能会服务其它任务。
这里的空闲线程包不包括后来新创建的服务线程呢？我的理解是包括的。
关于handler有四个选择：
ThreadPoolExecutor.AbortPolicy()  抛出java.util.concurrent.RejectedExecutionException异常  ThreadPoolExecutor.CallerRunsPolicy()  重试添加当前的任务，他会自动重复调用execute()方法  ThreadPoolExecutor.DiscardOldestPolicy()  抛弃旧的任务   ThreadPoolExecutor.DiscardPolicy()  抛弃当前的任务 
小的分析：
使用无界队列，要防止任务增长的速度远远超过处理任务的速度，控制不好可能导致的结果就是内存溢出。
使用有界队列，关键在于调节线程数和Queue大小 ，线程数多，队列容量少，资源浪费。线程数少，队列容量多，性能低，还可能导致内存溢出。
