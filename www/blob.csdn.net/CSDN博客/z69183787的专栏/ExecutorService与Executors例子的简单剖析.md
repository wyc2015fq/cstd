# ExecutorService与Executors例子的简单剖析 - z69183787的专栏 - CSDN博客
2016年11月06日 17:06:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1445
 对于多线程有了一点了解之后，那么来看看java.lang.concurrent包下面的一些东西。在此之前，我们运行一个线程都是显式调用了Thread的start()方法。我们用concurrent下面的类来实现一下线程的运行，而且这将成为以后常用的方法或者实现思路。 
        看一个简单的例子： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- publicclass CacheThreadPool {  
- publicstaticvoid main(String[] args) {  
-         ExecutorService exec=Executors.newCachedThreadPool();  
- for(int i=0;i<5;i++)  
-             exec.execute(new LiftOff());  
-         exec.shutdown();//并不是终止线程的运行，而是禁止在这个Executor中添加新的任务
-     }  
- }  
        这个例子其实很容易看懂，ExecutorService中有一个execute方法，这个方法的参数是Runnable类型。也就是说，将一个实现了Runnable类型的类的实例作为参数传入execute方法并执行，那么线程就相应的执行了。 
一、ExecutorService
        先看看ExecutorService，这是一个接口，简单的列一下这个接口： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- publicinterface ExecutorService extends Executor {  
- 
- void shutdown();  
- 
-     List<Runnable> shutdownNow();  
- 
- boolean isShutdown();  
- 
- boolean isTerminated();  
- 
- boolean awaitTermination(long timeout, TimeUnit unit)  
- 
-     <T> Future<T> submit(Callable<T> task);  
- 
-     <T> Future<T> submit(Runnable task, T result);  
- 
-     Future<?> submit(Runnable task);  
- 
-     <T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks)  
- 
-     <T> List<Future<T>> invokeAll(Collection<? extends Callable<T>> tasks, long timeout, TimeUnit unit)  
- 
-     <T> T invokeAny(Collection<? extends Callable<T>> tasks)  
- 
-     <T> T invokeAny(Collection<? extends Callable<T>> tasks,  
- long timeout, TimeUnit unit)  
- }  
        ExecuteService继承了Executor，Executor也是一个接口，里面只有一个方法： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- void execute(Runnable command)  
二、Executors
        Executors是一个类，直接援引JDK文档的说明来说一下这个类的作用： 
- 
        Factory and utility methods for Executor, ExecutorService, ScheduledExecutorService, ThreadFactory, and Callable classes defined in this package. This class supports the following kinds of methods: 
- Methods that create and return an ExecutorService set up with commonly useful configuration settings.       
- Methods that create and return a ScheduledExecutorService set up with commonly useful configuration settings.       
- Methods that create and return a "wrapped" ExecutorService, that disables reconfiguration by making implementation-specific methods inaccessible.       
- Methods that create and return a ThreadFactory that sets newly created threads to a known state.       
- Methods that create and return a Callable out of other closure-like forms, so they can be used in execution methods requiring Callable.       
        在上面的例子中，我们用到了newCachedThreadPool()方法。看一下这个方法： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- publicstatic ExecutorService newCachedThreadPool() {  
- returnnew ThreadPoolExecutor(0, Integer.MAX_VALUE,  
-                                       60L, TimeUnit.SECONDS,  
- new SynchronousQueue<Runnable>());  
-     }  
        在源码中我们可以知道两点，1、这个方法返回类型是ExecutorService；2、此方法返回值实际是另一个类的实例。看一下这个类的信息： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- publicclass ThreadPoolExecutor extends AbstractExecutorService {  
-     ..........  
- privatefinal BlockingQueue<Runnable> workQueue;//这个变量在下面会提到
-     ..........  
- }  
        ThreadPoolExecutor继承了AbstractExecutorService,而AbstractExecutorService又实现了ExecutorService接口。所以，根据多态，ThreadPoolExecutor可以看作是ExecutorService类型。 
        线程执行的最关键的一步是执行了executor方法，根据java的动态绑定，实际执行的是ThreadPoolExecutor所实现的executor方法。看看源码： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- publicclass ThreadPoolExecutor extends AbstractExecutorService {  
-     ..........  
- publicvoid execute(Runnable command) {  
- if (command == null)  
- thrownew NullPointerException();  
- if (poolSize >= corePoolSize || !addIfUnderCorePoolSize(command)) {  
- if (runState == RUNNING && workQueue.offer(command)) {  
- if (runState != RUNNING || poolSize == 0)  
-                     ensureQueuedTaskHandled(command);  
-             }  
- elseif (!addIfUnderMaximumPoolSize(command))  
-                 reject(command); // is shutdown or saturated
-         }  
-     }  
-     ..........  
- }  
        根据程序正常执行的路线来看，这个方法中比较重要的两个地方分别是： 
        1、workQueue.offer(command) 
        workQueue在上面提到过，是BlockingQueue<Runnable>类型的变量，这条语句就是将Runnable类型的实例加入到队列中。 
        2、ensureQueuedTaskHandled(command) 
        这个是线程执行的关键语句。看看它的源码： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- publicclass ThreadPoolExecutor extends AbstractExecutorService {  
-     ..........  
- privatevoid ensureQueuedTaskHandled(Runnable command) {  
- final ReentrantLock mainLock = this.mainLock;  
-         mainLock.lock();  
- boolean reject = false;  
-         Thread t = null;  
- try {  
- int state = runState;  
- if (state != RUNNING && workQueue.remove(command))  
-                 reject = true;  
- elseif (state < STOP &&  
-                      poolSize < Math.max(corePoolSize, 1) &&  
-                      !workQueue.isEmpty())  
-                 t = addThread(null);  
-         } finally {  
-             mainLock.unlock();  
-         }  
- if (reject)  
-             reject(command);  
- elseif (t != null)  
-             t.start();  
-     }  
-     ..........  
- }  
        在这里我们就可以看到最终执行了t.start()方法来运行线程。在这之前的重点是t=addThread(null)方法，看看addThread方法的源码： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- publicclass ThreadPoolExecutor extends AbstractExecutorService {  
-     ..........  
- private Thread addThread(Runnable firstTask) {  
-         Worker w = new Worker(firstTask);  
-         Thread t = threadFactory.newThread(w);  
- if (t != null) {  
-             w.thread = t;  
-             workers.add(w);  
- int nt = ++poolSize;  
- if (nt > largestPoolSize)  
-                 largestPoolSize = nt;  
-         }  
- return t;  
-     }  
-     ..........  
- }  
        这里两个重点，很明显： 
        1、Worker w = new Worker(firstTask) 
        2、Thread t = threadFactory.newThread(w) 
        先看Worker是个什么结构： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- publicclass ThreadPoolExecutor extends AbstractExecutorService {  
-     ..........  
- privatefinalclass Worker implements Runnable {  
-         ..........  
-         Worker(Runnable firstTask) {  
- this.firstTask = firstTask;  
-         }  
- 
- private Runnable firstTask;  
-         ..........  
- 
- publicvoid run() {  
- try {  
-                 Runnable task = firstTask;  
-                 firstTask = null;  
- while (task != null || (task = getTask()) != null) {  
-                     runTask(task);  
-                     task = null;  
-                 }  
-             } finally {  
-                 workerDone(this);  
-             }  
-         }  
-     }  
- 
-     Runnable getTask() {  
- for (;;) {  
- try {  
- int state = runState;  
- if (state > SHUTDOWN)  
- returnnull;  
-                 Runnable r;  
- if (state == SHUTDOWN)  // Help drain queue
-                     r = workQueue.poll();  
- elseif (poolSize > corePoolSize || allowCoreThreadTimeOut)  
-                     r = workQueue.poll(keepAliveTime, TimeUnit.NANOSECONDS);  
- else
-                     r = workQueue.take();  
- if (r != null)  
- return r;  
- if (workerCanExit()) {  
- if (runState >= SHUTDOWN) // Wake up others
-                         interruptIdleWorkers();  
- returnnull;  
-                 }  
- // Else retry
-             } catch (InterruptedException ie) {  
- // On interruption, re-check runState
-             }  
-         }  
-     }  
-     }  
-     ..........  
- }  
        Worker是一个内部类。根据之前可以知道，传入addThread的参数是null，也就是说Work中firstTask为null。
        在看看newThread是一个什么方法： 
Java代码  ![收藏代码](http://paddy-w.iteye.com/images/icon_star.png)
- publicclass Executors {  
-     ..........  
- staticclass DefaultThreadFactory implements ThreadFactory {  
-         ..........  
- public Thread newThread(Runnable r) {  
-             Thread t = new Thread(group, r,  
-                                   namePrefix + threadNumber.getAndIncrement(),  
- 0);  
- if (t.isDaemon())  
-                 t.setDaemon(false);  
- if (t.getPriority() != Thread.NORM_PRIORITY)  
-                 t.setPriority(Thread.NORM_PRIORITY);  
- return t;  
-         }  
-         ..........  
-     }  
-     ..........  
- }  
        通过源码可以得知threadFactory的实际类型是DefaultThreadFactory，而DefaultThreadFactory是Executors的一个嵌套内部类。 
        之前我们提到了t.start()这个方法执行了线程。那么现在从头顺一下，看看到底是执行了谁的run方法。首先知道，t=addThread(null)，而addThread内部执行了下面三步，Worker w = new Worker(null);Thread t = threadFactory.newThread(w);return
 t;这里两个t是一致的。 
        从这里可以看出，t.start()实际上执行的是Worker内部的run方法。run()内部会在if条件里面使用“短路”：判断firstTask是否为null，若不是null则直接执行firstTask的run方法；如果是null，则调用getTask()方法来获取Runnable类型实例。从哪里获取呢？workQueue！在execute方法中，执行ensureQueuedTaskHandled(command)之前就已经把Runnable类型实例放入到workQueue中了，所以这里可以从workQueue中获取到。
