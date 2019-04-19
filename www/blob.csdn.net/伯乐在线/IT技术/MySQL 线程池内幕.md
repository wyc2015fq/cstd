# MySQL 线程池内幕 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Float_Lu](http://www.jobbole.com/members/5334670325) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# 摘要
在MySQL中，线程池指的是用来管理处理MySQL客户端连接任务的线程的一种机制，我厂用的percona版本已经是集成了线程池，只需要通过如下参数开启即可。


```
thread_handling=pool-of-threads
```
本文在介绍MySQL线程池核心参数的基础之上对线程池内部实现机制进行进一步介绍。
# 线程池导读
## 线程池概论
在继续了解MySQL线程池之前，我们首先要了解为什么线程池的引入可以帮助MySQL提升性能，除了性能之外线程池还有哪些作用？如果把线程看做系统资源那么线程池本质上是对系统资源的管理，对于操作系统来说线程的创建和销毁是比较消耗系统资源的，频繁的创建与销毁线程必然给系统带来不必要的资源浪费，特别是在负载高的情况下这部分开销严重影响系统的资源使用效率从而影响系统的性能与吞吐量，另一方面过多的线程创建又会造成系统资源的过载消耗，同时带来相对频繁的线程之间上下文切换问题。系统资源是宝贵的，我认为性能与资源的利用率是紧密相关的：
![](http://ww3.sinaimg.cn/mw690/63918611gw1fbjl13q7mxj20or0bkaak.jpg)
**资源利用率与性能的同向性**
他们往往向着一个方向发展，好的资源利用与通常可以带来较优的性能，线程池技术一方面可以减少线程重复创建与销毁这部分开销，从而更好地利用已经创建的线程资源，另一方面也可以控制线程的创建与系统的负载，某些场景对系统起到了保护作用。
## 如何了解MySQL线程池
通过学习掌握MySQL有哪些参数，并深刻理解每个参数的含义以及这些参数是如何影响MySQL的等问题是一种很好的学习MySQL线程池的一种方式，另外在了解MySQL基本实现原理的基础之上再对MySQL线程池不足以及可以改进的地方进行更深层次的思考有利于更好地理解MySQL线程池技术。
# 线程池核心参数
MySQL线程池向用户开放了一些参数，用户可以修改这些参数从而影响线程池的行为，下面分别介绍一下这些核心参数。
## thread_pool_size
这个参数指的是线程组大小，默认是CPU核心数，线程池初始化的时候会根据这个数字来生成线程组，每个线程组初始化一个poolfd句柄。
## thread_pool_stall_limit
Timer Thread迭代的时间间隔，默认是500ms。
## thread_pool_oversubscribe
用于计算线程组是否太过活跃或者太过繁忙，也即系统的负载程度，用于在一定场景决策新的工作线程是否被创建于和任务是否被处理，这个值默认是3。
## thread_pool_max_threads
允许线程池中最大的线程数，默认是10000。
## thread_pool_idle_timeout
工作线程最大空闲时间，工作线程超过这个数还空闲的话就退出，这个值默认是60秒。
## thread_pool_high_prio_mode
这个参数可用于控制任务队列的使用，可取三个值：
- transactions
- statements
- none
当为值为statements的时候则线程组只使用优先队列，当为值为none的时候则只使用普通队列，当值为transactions的时候配合thread_pool_high_prio_tickets参数生效，用于控制任务被放入优先队列的最大次数。
## thread_pool_high_prio_tickets
当thread_pool_high_prio_mode=transactions的时候每个连接的任务最多被放入优先队列thread_pool_high_prio_tickets次，并且每放一次递减，直到小于等于0的时候放入普通队列，这个值默认是4294967295。
# MySQL线程池实现内幕
## 线程池总体架构
与JAVA的线程池不同，JAVA线程池中是工作线程而MySQL线程池有一个线程组的概念，线程组内部层级才是工作线程，先看看MySQL线程池的大致架构：
![](http://ww1.sinaimg.cn/mw690/63918611gw1fbjl192w3bj21ig0z87au.jpg)
**线程池架构**
上图大致可以看出线程池内部的结构，线程组为我们关注的一个较大的组件，线程组内部每个组件的相互协调构成了线程组，每个线程组良好地工作构成了线程池。对于线程池内部，我认为值得了解的内容主要包括下面几个方面：
- 线程组
- Worker线程
- Check Stall机制
- 任务队列
- Listener线程
对于上面列出的几个方面，后文将会展开介绍。
## 线程组
MySQL线程池在初始化的时候根据宿主机的CPU核心数设置thread_pool_size，这也就是线程池的线程组的个数。每个线程组在初始化之后会通过底层的IO库分配一个网络特殊的句柄与之关联，IO库可以通过这个句柄监听与之绑定的socket句柄就绪的IO任务，线程组的结构体定义如下：


```
struct thread_group_t
{
  mysql_mutex_t mutex;
  connection_queue_t queue;//低优先级任务队列
  connection_queue_t high_prio_queue;//高优先级任务队列
  worker_list_t waiting_threads; //代表当前线程没有任务的时候进入等待队里
  worker_thread_t *listener;//读取网络任务线程
  pthread_attr_t *pthread_attr;
  int  pollfd;//特殊的句柄
  int  thread_count;//线程组中的线程数
  int  active_thread_count;//当前活跃的线程
  int  connection_count;//分配给当前线程组的连接
  int  waiting_thread_count;//代表的是当前线程在执行命令的时候处于等待状态
  /* Stats for the deadlock detection timer routine.*/
  int io_event_count;//待处理任务数，从句柄中获取
  int queue_event_count;//从队列移除的网络任务数，意味着网络任务被处理   
  ulonglong last_thread_creation_time;//上一次创建工作线程的时候
  int  shutdown_pipe[2];
  bool shutdown;
  bool stalled;
 } MY_ALIGNED(512);
```
线程池由多个线程组构成，线程池的细节基本都在线程组内。
## worker线程
线程组内有0个或多个线程，这里与Netty有些不同，Netty中有固定的线程用于轮训IO事件，工作线程只负责处理IO任务，而在MySQL线程池中listener只是一种角色，每个线程的角色可以是listener或者是worker，工作线程为listener的时候负责从poolfd中读取就绪IO任务，处于worker角色的时候负责处理这些IO任务，我们需要区分工作线程的以下几种状态状态：
- **活跃状态：**当工作线程处于正在处理任务且的状态且未被阻塞的状态，这意味着工作线程将会消耗CPU，增加系统的负载。如果worker线程将自己设置为listener则不算进线程组的活跃线程状态数。
- **空闲状态：**由于没有任务处理而被处于的空闲状态。
- **等待状态：**如果工作线程在执行命令的过程中由于IO、锁、条件、sleep等需要等待则线程池将被通知并且将这些工作线程记作等待状态。
在线程组中，关于线程的计数有如下关系：


```
thread_count = active_thread_count + waiting_thread_count + waiting_threads.length + listener.length
```
thread_count代表线程组中的总线程数，active_thread_count代表当前正在工作且未被阻塞的线程数，waiting_thread_count代表的是工作线程任务的过程中被阻塞的个数，而waiting_threads代表空闲线程列表。
在MySQL线程池中，线程组中busy的线程数是active_thread_count与waiting_thread_count的总和，因为这些线程此时都不能处理新的任务，因此被认为是**繁忙的**。如果处于busy状态的线程数大于一定值则线程组被任务是**太繁忙（too many active）**了，这会用于决策普通优先级的任务是否能得到及时的处理，这个值被定义为：


```
thread_pool_oversubscribe + 1
```
默认值也就是4。如果active_thread_count数大于等于一定值（同上算法为4），则线程组被认为是**太活跃（too busy）**了，此时意味着可能过饱满的CPU负载，这个指标用于决策线程组是否还继续执行普通优先级的任务，上面的逻辑总结一句话为：
**在正常工作的情况下，当工作线程检索任务的时候，如果线程组太活跃（too many active）则即使有任务工作线程也不会执行，如果不是太繁忙（too busy）才会考虑高优先级的任务，对于低优先级的任务只有当线程组不是太繁忙（too busy）的时候才会执行。**
注意上面的条件，因此线程池对系统负载具有一定的保护作用，那么问题来了，如果存在一些耗时任务（如耗时查询），会不会导致后来任务被延迟处理？会不会有时候觉得SQL写得没问题，但是却莫名其妙的Long SQL？这就是下面要介绍的Check Stall机制。
## Check Stall机制
如果后来的IO任务被前面执行时间过长的任务影响了怎么办？这必然会导致一些无辜的任务（或是一个简单的INSERT操作，之所以举INSERT的例子是因为INSERT通常很快）被影响，结果是有可能会被延迟处理。线程池中有一个Timer Thread，类似我们很多系统里面的Timeout Thread线程，这个线程每隔一定时间间隔就会进行一次迭代，迭代中做的事情包括如下两个部分：
- 检查线程组的负载情况进行工作线程的唤醒与创建。
- 检查与处理超时的客户端连接。
这里主要介绍第一部分工作也就是Check Stall机制。Timer Thread周期性地检查线程组内的线程是否被**阻塞**（stall），所谓阻塞也就是**新来了任务但是线程组内没有线程来处理**。Timer Thread通过queue_event_count和IO任务队列是否为空来判断线程组是否为阻塞状态，每次工作线程检索任务的时候queue_event_count都会累加，累加意味着任务被正常处理，工作线程正常工作，在每一次check_stall之后queue_event_count会被清零，因此**如果在一定时间间隔（stall_limit）后的下一次迭代中，IO任务队列不为空并且queue_event_count为空，则说明这段时间间隔内都没有工作线程来处理IO任务了，那么Check Stall机制会尝试着唤醒或创建一个工作线程**，唤醒线程的逻辑很简单，如果waiting_threads中有空闲线程则唤醒一个空闲线程，否则需要尝试创建一个工作线程，创建线程不一定会创建成功，我们看看创建线程的条件：
- 如果没有空闲线程且没有活跃线程则立马创建，这个时候可能是因为没有任何工作线程或者工作线程都被阻塞了，或者是存在潜在的死锁。
- 否则如果距离上次创建的时间大于一定阈值才创建线程，这个阈值由线程组内的线程数决定。
阈值与线程组内线程数的关系如下：
|线程数|阈值|
|----|----|
|< 4|0|
|< 8|50 * 1000|
|< 16|100 * 1000|
|>= 16|200 * 1000|
**阈值机制**能够有效的防止线程创建过于频繁。这里遗留个问题，为什么阈值依赖于线程池的线程数？阈值是否能依赖于thread_pool_stall_limit的值？Check Stall机制可以被认为**一个专门的线程做专门的事情**，毕竟线程组内部逻辑也是蛮混乱的。
## 任务队列
任务队列也就是listener每次从poolfd轮训出来的就绪任务，分为**优先任务队列**（high_prio_queue）和**普通任务队列**（queue），优先队列中的IO任务会先被处理，然后普通队列中的任务才能够被处理。那么什么样的任务会被认为是优先任务呢？官方列出了两个条件：
- 连接处于事务中。
- 连接关联的priority tickets值大于0。
参数priority tickets（thread_pool_high_prio_tickets）的设计是为了防止高优先级的任务总是被处理，而一些非高优先级的任务处于较长时间的饥饿状态，毕竟工作线程的创建也是有条件的，当高优先级的任务每一次被放入高优先级队列之后都会对priority tickets的值进行减一，因此达到一定次数priority tickets的值必然会小于等于0，因此避免了**永久高优先级的问题**。另外队列的使用受参数thread_pool_high_prio_mode影响，可参考对参数thread_pool_high_prio_mode介绍的部分。当就绪IO任务被轮训出来放入队列之后会对io_event_count进行累加，当IO任务从队列取出处理的时候会对queue_event_coun进行计数。
## **Listener线程**
Listener做的事情主要是从poolfd中轮训与其绑定的socket句柄的就绪IO事件，事件以任务的形式被放入任务队列并做相应处理，如果listener读取了一些IO任务之后，该怎么办呢？下面基于两个问题回答：
- listener应该自己处理这些任务吗？还是将这些任务放入队列让工作线程处理？
- 如果任务队列不为空，我们需要唤醒多少个工作线程？
对于**第一个问题**，通常我们不想经常改变listener的等待和唤醒的状态，因为listener刚被唤醒，因此我们更倾向于让listener利用它的时间片去做一些工作。如果listener不自己处理工作，这意味着其他线程要被唤醒去做这个工作，这显然不是很好。而让listener去做任务潜在的问题是线程组有可能一段时间网络任务无法及时被处理，这不是主要的问题，因为stall将被Timer Thread检查。然而总是依赖Timer Thread也是不好的，因为stall_limit有可能被设置比较长的时间。我们使用下面的策略，如果任务队列不空，我们任务网络任务此时可能比较多，让其他线程来处理任务，否则listener自己处理任务。
对于**第二个问题**，我们通常为每一个线程组保持一个活动线程（活动线程包括正在做任务的线程），因此唤醒一个工作线程的条件为当前活跃前程数为0，如果没有线程被唤醒，在只能依靠Timer Thread来检查stall并进行唤醒了。
上面可以看出，如果任务队列不为空，也不一定会有线程来及时处理任务，这就导致了耗时任务影响了后来任务的执行，未来可能通过摒弃每个线程组只保持一个活跃线程的规则来避免网络任务长时间得不到处理。
# 总结
使用MySQL线程池可以提高数据库的性能，设计者对线程池的创建与任务的处理机制进行精心的设计，然而同时也带来了一些潜在的问题，最明显的就是**耗时任务对其他任务调度的影响**，尽管有不足之处但是使用者仍然可以通过掌握线程池的内部细节以及深刻了解开放参数的含义，通过参数的调整来在一定程度上对MySQL线程池的使用进行优化。学以致用，到这里，您是否能够利用上面介绍的一些知识来解决一些实际问题了呢？
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7cb05a1a5b9ef06cc9d40cc79a096f293.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/12e72c4df391cc981614cc68aedd44085.jpg)
