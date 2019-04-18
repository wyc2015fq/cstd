# java创建多线程使用Executors创造ExecutorService - z69183787的专栏 - CSDN博客
2014年01月16日 12:54:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3051
Executors提供了一些方便创建ThreadPoolExecutor的常用方法，主要有以下几个：
**1、 Executors.newFixedThreadPool(int nThreads);创建固定大小(nThreads,大小不能超过int的最大值)的线程池**
//线程数量
int nThreads = 20;
//创建executor 服务
ExecutorService executor = Executors.newFixedThreadPool(nThreads) ;
重载后的版本，需要多传入实现了ThreadFactory接口的对象。
`ExecutorService executor =` Executors. `newFixedThreadPool(`nThreads,threadFactory);
说明：创建固定大小(nThreads,大小不能超过int的最大值)的线程池，缓冲任务的队列为LinkedBlockingQueue,大小为整型的最大数，当使用此线程池时，在同执行的任务数量超过传入的线程池大小值后，将会放入LinkedBlockingQueue，在LinkedBlockingQueue中的任务需要等待线程空闲后再执行，如果放入LinkedBlockingQueue中的任务超过整型的最大数时，抛出RejectedExecutionException。
**2、Executors.newSingleThreadExecutor()：创建大小为1的固定线程池。**
ExecutorService executor = Executors.newSingleThreadExecutor();
重载后的版本，需要多传入实现了ThreadFactory接口的对象。
`ExecutorService executor =` Executors. `newSingleThreadScheduledExecutor(ThreadFactory threadFactory)`
说明：创建大小为1的固定线程池，同时执行任务(task)的只有一个,其它的（任务）task都放在LinkedBlockingQueue中排队等待执行。
3、**Executors**.**newCachedThreadPool**()；创建corePoolSize为0，最大线程数为整型的最大数，线程keepAliveTime为1分钟，缓存任务的队列为SynchronousQueue的线程池。
ExecutorService executor = Executors.newCachedThreadPool()；
当然也可以以下面的方式创建，重载后的版本，需要多传入实现了ThreadFactory接口的对象。
`ExecutorService executor =` Executors.`newCachedThreadPool(ThreadFactory threadFactory)` ;
说明：使用时，放入线程池的task任务会复用线程或启动新线程来执行，注意事项：启动的线程数如果超过整型最大值后会抛出RejectedExecutionException异常，启动后的线程存活时间为一分钟。
**4、Executors.newScheduledThreadPool(int corePoolSize):创建corePoolSize大小的线程池。**
//线程数量
int corePoolSize= 20;
//创建executor 服务
ExecutorService executor = Executors.newScheduledThreadPool(corePoolSize) ;
重载后的版本，需要多传入实现了ThreadFactory接口的对象。
`ExecutorService executor = Executors.newScheduledThreadPool`(corePoolSize
```
,
 threadFactory)
```
；
说明：线程keepAliveTime为0，缓存任务的队列为DelayedWorkQueue，注意不要超过整型的最大值。
区别： 
一：
newCachedThreadPool
 (1)缓存型池子，先查看池中有没有以前建立的线程，如果有，就reuse，如果没有，就建立一个新的线程加入池中；  
 (2)缓存型池子，通常用于执行一些生存周期很短的异步型任务；因此一些面向连接的daemon型server中用得不多；  
 (3)能reuse的线程，必须是timeout IDLE内的池中线程，缺省timeout是60s,超过这个IDLE时长，线程实例将被终止及移出池。  
 (4)注意，放入CachedThreadPool的线程不必担心其结束，超过TIMEOUT不活动，其会自动被终止
二：
newFixedThreadPool
 (1)newFixedThreadPool与cacheThreadPool差不多，也是能reuse就用，但不能随时建新的线程  
 (2)其独特之处:任意时间点，最多只能有固定数目的活动线程存在，此时如果有新的线程要建立，只能放在另外的队列中等待，直到当前的线程中某个线程终止直接被移出池子
 (3)和cacheThreadPool不同，FixedThreadPool没有IDLE机制（可能也有，但既然文档没提，肯定非常长，类似依赖上层的TCP或UDP IDLE机制之类的），所以FixedThreadPool多数针对一些很稳定很固定的正规并发线程，多用于服务器 
 (4)从方法的源代码看，cache池和fixed 池调用的是同一个底层池，只不过参数不同: fixed池线程数固定，并且是0秒IDLE（无IDLE）cache池线程数支持
0-Integer.MAX_VALUE(显然完全没考虑主机的资源承受能力），60秒IDLE 
三：
ScheduledThreadPool
（1）调度型线程池 
（2）这个池子里的线程可以按schedule依次delay执行，或周期执行 
四：
SingleThreadExecutor
（1）单例线程，任意时间池中只能有一个线程 
（2）用的是和cache池和fixed池相同的底层池，但线程数目是1-1,0秒IDLE（无IDLE）
