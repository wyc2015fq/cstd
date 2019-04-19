# 线程池ThreadPool - 三少GG - CSDN博客
2013年10月28日 22:40:54[三少GG](https://me.csdn.net/scut1135)阅读数：1639
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
**开源实现：**
**1. 成熟：  [http://threadpool.sourceforge.net/index.html](http://threadpool.sourceforge.net/index.html)**
**(代码分析文章： [http://blog.csdn.net/byxdaz/article/details/6299020](http://blog.csdn.net/byxdaz/article/details/6299020)）**
**2. 代码量小： [https://github.com/mbrossard/threadpool](https://github.com/mbrossard/threadpool)**
**(代码分析文章： [http://swind.code-life.info/posts/c-thread-pool.html](http://swind.code-life.info/posts/c-thread-pool.html) ）**
*************************************************************************************************************************************************************************************
# 必看文章：1.[IBM：线程池的介绍及简单实现](http://www.ibm.com/developerworks/cn/java/l-threadPool/)
# 必看文章：2.
Threads are very useful but potentially very tricky constructs in computer programming. They are also generally hard to get right. Building a deadlock-free and correct multi-threaded software requires great care.
And then threads are expensive. The processor needs to do a context switch to jump between threads. Each context switch means saving the state of the current executing thread and then loading the thread selected for execution. Creating a thread for every I/O
 operation or lengthy computation can push the machine to a halt if the number of requests for I/O and/or computation is too high.
**The midway between creating too many threads for all requests for service and doing everything in one thread is to create a pool of threads and reuse a thread as soon as it is done servicing a request.**C++
 does not have a built-in thread pool library (it even didn't have threading support prior to C++ 11 standard that came out last year).**Java has theExecutor interface for this.**With C++, some people use the Boost threading library or theBoost
 Asio library for achieving performance gains in their applications.
In this article we will design a very simple thread pool library using C++ and POSIX threads(also known as pthreads). We will not use any other external libraries like Boost.
The core idea of a thread pool is to have a number of worker threads always ready for accepting work from the main process. The process receives requests for work and schedule those requests as doable tasks for the threads. This pattern resembles the well known
 Readers-writers problem in Computer Science. There is a queue that is populated with tasks as they arrive in the process. The request processing part is the writer and the threads are the readers. Request processor will insert the work items as they arrive
 and the threads will pick up one item at a time from the queue in a First-In-First-Out fashion.
So a thread pool basically consists of the three primary actors:
1. A number of threads either waiting for or executing tasks.
2. A number of service requests to the server from clients. Each request is considered a task to the threads.
3. A queue holding the incoming service requests.
![](https://img-blog.csdn.net/20131028222028437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGt1ZWVjc2Vy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 必看文章：3
[http://cplus.abang.com/od/cplus/a/threadpoolana1.htm](http://cplus.abang.com/od/cplus/a/threadpoolana1.htm)
在平时项目中我们经常需要多线程编程，提高系统的CPU利用率，但是如果需要用多线程处理类似轻量级、频率高的任务，因为创建一个线程或者删除一个线程是需要开销的;如果线程数量过大的话，cpu就会浪费很大的精力做线程切换，和创建、删除线程，甚至在系统开销中占很大的一部分，反而会大大降低系统的性能，碰到这种问题怎么办？自然而然，我们想到了使用线程池——ThreadPool。线程池的原理是首先创建一定量的线程，如果有任务需要线程进行计算，则从线程池中取出线程，调用线程执行该任务，执行完任务，将线程重新置于线程池ThreadPool中。  本文将会就C++实现的一个简单的线程池为Demo，阐述并分析线程池的基本的实现方法，让大家能就ThreadPool能有一个更直观的印象，带大家探索ThreadPool的奥妙。  从上图（线程池流程图）中我们可以看到线程池ThreadPool主要分为以下三个类：WorkItem: 任务对象，标识需要实现的具体的工作 WorkItemQueue: 任务对象队列，当用户向ThreadPool提交任务时，必须先将该任务WorkItem压入任务对象队列 ThreadPool：负责创建，分配线程对象
线程池流程图具体如下： 1、创建线程池ThreadPool, ThreadPool负责创建n个线程，每个线程都处于Waiting状态。 2、用户调用AddWorkItem(workitem)，sem_wait(availableQueueSlots)——如果任务队列WorkItemQueue已满，则等待信号量availableQueueSlots; 如果WorkItemQueue未满，则sem_post(availableWorkItem), 对信号量availableWorkItem加一，最后向WorkItemQueue中加入任务workitem. 3、线程池中的Waiting线程sem_wait(availableWorkItem)——如果任务队列中没有任务，则线程继续处于等待availableWorkItem;如果任务队列中存在任务，则某等待线程从任务对象队列WorkItemQueue中取出workitem, 执行该线程，并sem_post(availabelQueueSlots)——因为从队列中已经取走该workitem，此时线程处于Running状态 4、当线程池中Running线程执行完对应任务workitem，则该线程状态转为Waiting,继续等待任务队列中是否有任务——sem_wait(availableWorkItem) 

关于高级线程池的探讨
简单线程池存在一些问题，比如如果有大量的客户要求服务器为其服务，但由于线程池的工作线程是有限的，服务器只能为部分客户服务，其它客户提交的任务，只能在任务队列中等待处理。一些系统设计人员可能会不满这种状况，因为他们对服务器程序的响应时间要求比较严格，所以在系统设计时可能会怀疑线程池技术的可行性，但是线程池有相应的解决方案。调整优化线程池尺寸是高级线程池要解决的一个问题。主要有下列解决方案：
方案一：动态增加工作线程
在一些高级线程池中一般提供一个可以动态改变的工作线程数目的功能，以适应突发性的请求。一旦请求变少了将逐步减少线程池中工作线程的数目。当然线程增加可以采用一种超前方式，即批量增加一批工作线程，而不是来一个请求才建立创建一个线程。批量创建是更加有效的方式。该方案还有应该限制线程池中工作线程数目的上限和下限。否则这种灵活的方式也就变成一种错误的方式或者灾难，因为频繁的创建线程或者短时间内产生大量的线程将会背离使用线程池原始初衷--减少创建线程的次数。
举例：Jini中的TaskManager，就是一个精巧线程池管理器，它是动态增加工作线程的。SQL Server采用单进程(Single Process)多线程(Multi-Thread)的系统结构，1024个数量的线程池，动态线程分配，理论上限32767。
方案二：优化工作线程数目
如果不想在线程池应用复杂的策略来保证工作线程数满足应用的要求，你就要根据统计学的原理来统计客户的请求数目，比如高峰时段平均一秒钟内有多少任务要求处理，并根据系统的承受能力及客户的忍受能力来平衡估计一个合理的线程池尺寸。线程池的尺寸确实很难确定，所以有时干脆用经验值。
举例：在MTS中线程池的尺寸固定为100。
方案三：一个服务器提供多个线程池
在一些复杂的系统结构会采用这个方案。这样可以根据不同任务或者任务优先级来采用不同线程池处理。
举例：COM+用到了多个线程池。
这三种方案各有优缺点。在不同应用中可能采用不同的方案或者干脆组合这三种方案来解决实际问题。
[回页首](http://www.ibm.com/developerworks/cn/java/l-threadPool/#ibm-pcon)
线程池技术适用范围及应注意的问题
下面是我总结的一些线程池应用范围,可能是不全面的。
线程池的应用范围：
- 
需要大量的线程来完成任务，且完成任务的时间比较短。 WEB服务器完成网页请求这样的任务，使用线程池技术是非常合适的。因为单个任务小，而任务数量巨大，你可以想象一个热门网站的点击次数。 但对于长时间的任务，比如一个Telnet连接请求，线程池的优点就不明显了。因为Telnet会话时间比线程的创建时间大多了。
- 
对性能要求苛刻的应用，比如要求服务器迅速相应客户请求。
- 
接受突发性的大量请求，但不至于使服务器因此产生大量线程的应用。突发性大量客户请求，在没有线程池情况下，将产生大量线程，虽然理论上大部分操作系统线程数目最大值不是问题，短时间内产生大量线程可能使内存到达极限，并出现"OutOfMemory"的错误。
[回页首](http://www.ibm.com/developerworks/cn/java/l-threadPool/#ibm-pcon)
结束语
本文只是简单介绍线程池技术。可以看出线程池技术对于服务器程序的性能改善是显著的。线程池技术在服务器领域有着广泛的应用前景。希望这项技术能够应用到您的多线程服务程序中。
****************************************************************************************************************************************************************************************
**评论：**
《编写高质量代码：改善C#程序的157个建议》第6章异步、多线程、任务和并行，本章将通过具体的实例，让我们熟悉异步、多线程、任务和并行。我们会了解到异步的实质、任务的实质，以及为什么有了任务还需要一个并行类（Parallel）等问题。同时，本章内容还会告诉我们如何优雅地控制线程，并且处理任务和并行中的异常。本节为大家介绍建议79：使用ThreadPool或BackgroundWorker代替Thread。
由于要进行如此多的工作，所以创建和销毁一个线程就意味着代价“昂贵”。为了避免程序员无节制地使用线程，微软开发了“线程池”技术。简单来说，线程池就是替开发人员管理工作线程。当一项工作完毕时，CLR不会销毁这个线程，而是会保留这个线程一段时间，看是否有别的工作需要这个线程。至于何时销毁或新起线程，由CLR根据自身的算法来做这个决定。所以，如果我们要多线程编码，不应想到：
- Thread t = new Thread(() =>
-     {  
-         //工作代码  
-     });  
- t.Start(); 
应该首先想到依赖线程池：- ThreadPool.QueueUserWorkItem((objState) =>
- {  
-     //工作代码  
- }, null); 
线程池技术能让我们重点关注业务的实现，而不是线程的性能测试。
建议80：用Task代替ThreadPool （1）
ThreadPool相对于Thread来说具有很多优势，但是ThreadPool在使用上却存在一定的不方便。比如：
ThreadPool不支持线程的取消、完成、失败通知等交互性操作。
ThreadPool不支持线程执行的先后次序。
以往，如果开发者要实现上述功能，需要完成很多额外的工作。现在，FCL中提供了一个功能更强大的概念：Task。Task在线程池的基础上进行了优化，并提供了更多的API。在FCL 4.0中，如果我们要编写多线程程序，Task显然已经优于传统的方式了。
**链接：**
1.  [浅谈线程池（上）：线程池的作用及CLR线程池](http://blog.zhaojie.me/2009/07/thread-pool-1-the-goal-and-the-clr-thread-pool.html)
2.  [http://ifeve.com/tag/threadpool/](http://ifeve.com/tag/threadpool/)[Java线程池架构(一)原理和源码解析](http://ifeve.com/java-threadpoolexecutor/)
3. [](http://blog.csdn.net/axman/article/details/1481197)
### 深入研究线程池
"生成需要的工作线程.由于创建线程需要一定的开销,一定要注意所创建的所有线程不能超一个设定
的最大值.**建议最大值不要超25**."
