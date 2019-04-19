# 【深度学习&分布式】Parameter Server 详解 - 数据之美的博客 - CSDN博客
2018年03月13日 10:28:19[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1510
个人分类：[深度学习/机器学习](https://blog.csdn.net/oppo62258801/article/category/6739087)

目录[(?)](http://blog.csdn.net/cyh_24/article/details/50545780)[[-]](http://blog.csdn.net/cyh_24/article/details/50545780#)
- [Parameter Server 详解](http://blog.csdn.net/cyh_24/article/details/50545780#t0)
- [Background](http://blog.csdn.net/cyh_24/article/details/50545780#t1)
- [Related Work](http://blog.csdn.net/cyh_24/article/details/50545780#t2)
- [Parameter Server 优势](http://blog.csdn.net/cyh_24/article/details/50545780#t3)
- [Parameter Server 系统架构](http://blog.csdn.net/cyh_24/article/details/50545780#t4)- [key valueRange Push and Pull](http://blog.csdn.net/cyh_24/article/details/50545780#t5)
- [Asynchronous Tasks and Dependency](http://blog.csdn.net/cyh_24/article/details/50545780#t6)
- [Implementation](http://blog.csdn.net/cyh_24/article/details/50545780#t7)- [Vector Clock](http://blog.csdn.net/cyh_24/article/details/50545780#t8)
- [Messages](http://blog.csdn.net/cyh_24/article/details/50545780#t9)
- [Replication and Consistency](http://blog.csdn.net/cyh_24/article/details/50545780#t10)
- [Server Management](http://blog.csdn.net/cyh_24/article/details/50545780#t11)
- [参考文献](http://blog.csdn.net/cyh_24/article/details/50545780#t12)
# Parameter Server 详解
> 
本博客仅为作者记录笔记之用，不免有很多细节不对之处。
还望各位看官能够见谅，欢迎批评指正。
更多相关博客请猛戳：[http://blog.csdn.net/cyh_24](http://blog.csdn.net/cyh_24)
如需转载，请附上本文链接：[http://blog.csdn.net/cyh_24/article/details/50545780](http://blog.csdn.net/cyh_24/article/details/50545780)
[MXNet](https://github.com/dmlc/mxnet) 是李沐和陈天奇等各路英雄豪杰打造的开源深度学习框架（最近不能更火了），其中最吸引我的是它的分布式训练的特性；而提供支持其分布式训练特性的正是当年李少帅和 Alex Smola 等人开发的 [parameter server](http://parameterserver.org/).
本博客以“*Scaling Distributed Machine Learning with the Parameter Server*” 为主，从易用性、通信高效性、可扩展性等角度介绍 parameter server .
# Background
在机器学习和深度学习领域，分布式的优化已经成了一种先决条件。因为单机已经解决不了目前快速增长的数据和参数了。 
现实中，训练数据的数量可能达到1TB到1PB之间，而训练过程中的参数可能会达到109109到10121012。而往往这些模型的参数需要被所有的worker节点频繁的访问，这就会带来很多问题和挑战：
> - 访问这些巨量的参数，需要大量的网络带宽支持；
- 很多机器学习算法都是连续型的，只有上一次迭代完成（各个worker都完成）之后，才能进行下一次迭代，这就导致了如果机器之间性能差距大（木桶理论），就会造成性能的极大损失；
- 在分布式中，容错能力是非常重要的。很多情况下，算法都是部署到云环境中的（这种环境下，机器是不可靠的，并且job也是有可能被抢占的）；
# Related Work
对于机器学习分布式优化，有很多大公司在做了，包括：Amazon，Baidu，Facebook，Google，Microsoft 和 Yahoo。也有一些开源的项目，比如：YahooLDA 和 Petuum 和Graphlab。 总结一下：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-openps.png)
李少帅的这个ParameterServer 属于第三代的parameter server。
第一代 parameter server：缺少灵活性和性能 —— 仅使用memcached(key, value) 键值对存储作为同步机制。 *YahooLDA* 通过改进这个机制，增加了一个专门的服务器，提供用户能够自定义的更新操作(set, get, update)。 
第二代 parameter server：用bounded delay模型来改进YahooLDA，但是却进一步限制了worker线程模型。 
第三代 parameter server 能够解决这些局限性。
> 
首先来比较一下parameter server 跟通用的分布式系统之间的差别吧。
通用的分布式系统通常都是：每次迭代都强制同步，通常在几十个节点上，它们的性能可以表现的很好，但是在大规模集群中，这样的每次迭代强制同步的机制会因为木桶效应变得很慢。 
Mahout 基于 Hadoop，MLI 基于 Spark，它们（Spark与MLI）采用的都是 Iterative MapReduce 的架构。它们能够保持迭代之间的状态，并且执行策略也更加优化了。但是，由于这两种方法都采用同步迭代的通信方式，使得它们很容易因为个别机器的低性能导致全局性能的降低。
直观感受一下，当其中一个节点运行时间过长会发生什么：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-synchronous.gif)
为了解决这个问题，Graphlab 采用图形抽象的方式进行异步调度通信。但是它缺少了以 MapReduce 为基础架构的弹性扩展性，并且它使用粗粒度的snapshots来进行恢复，这两点都会阻碍到可扩展性。parameter server 正是吸取Graphlab异步机制的优势，并且解决了其在可扩展性方面的劣势。
看看异步迭代是如何提高性能的：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-asynchronous.gif)
# Parameter Server 优势
说完了其他的分布式系统的缺点，该回到本博客的主题了(夸ps)，parameter server 有哪些features？
> 
1. Efficient communication：
由于是异步的通信，因此，不需要停下来等一些机器执行完一个iteration（除非有必要），这大大减少了延时。为机器学习任务做了一些优化(后续会细讲)，能够大大减少网络流量和开销；
2. Flexible consistency models：
宽松的一致性要求进一步减少了同步的成本和延时。parameter server 允许算法设计者根据自身的情况来做算法收敛速度和系统性能之间的trade-off。
3. Elastic Scalability：
使用了一个分布式hash表使得新的server节点可以随时动态的插入到集合中；因此，新增一个节点不需要重新运行系统。
4. Fault Tolerance and Durability：
我们都知道，节点故障是不可避免的，特别是在大规模商用服务器集群中。从非灾难性机器故障中恢复，只需要1秒，而且不需要中断计算。Vector clocks 保证了经历故障之后还是能运行良好；
5. Ease of Use
全局共享的参数可以被表示成各种形式：vector，matrices 或者相应的sparse类型，这大大方便了机器学习算法的开发。并且提供的线性代数的数据类型都具有高性能的多线程库。
# Parameter Server 系统架构
在parameter server中，每个 server 实际上都只负责分到的部分参数（servers共同维持一个全局的共享参数），而每个 work 也只分到部分数据和处理任务；
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-communication.gif)
上图中，每个子节点都只维护自己分配到的参数（图中的黑色），自己部分更新之后，将计算结果（例如：梯度）传回到主节点，进行全局的更新（比如平均操作之类的），主节点再向子节点传送新的参数；
servers 与 workers 之间的通信如下：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-ps-arch.png)
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-arch.png)
server 节点可以跟其他 server 节点通信，每个server负责自己分到的参数，server group 共同维持所有参数的更新。 
server manager node 负责维护一些元数据的一致性，比如各个节点的状态，参数的分配情况等； 
worker 节点之间没有通信，只跟自己对应的server进行通信。每个worker group有一个task scheduler，负责向worker分配任务，并且监控worker的运行情况。当有新的worker加入或者退出，task scheduler 负责重新分配任务。
## (key, value)，Range Push and Pull
parameter server 中，参数都是可以被表示成(key, value)的集合，比如一个最小化损失函数的问题，key就是feature ID，而value就是它的权值。对于稀疏参数，不存在的key，就可以认为是0.
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-kv.png)
把参数表示成k-v， 形式更自然， 易于理，更易于编程解；
workers 跟 servers 之间通过 push 跟 pull 来通信。worker 通过 push 将计算好的梯度发送到server，然后通过 pull 从server更新参数。为了提高计算性能和带宽效率，parameter server 允许用户使用 Range Push 跟 Range Pull 操作；
假设 R 是需要push或pull的 key 的range，那么可以进行如下操作：
```
w.push(R, dest)
w.pull(R, dest)
```
- 1
- 2
意思应该很明显吧，就是发送和接送特定Range中的w.
## Asynchronous Tasks and Dependency
体会一下Asynchronous Task 跟 Synchronous Task 的区别：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-asysy.png)
如果 iter1 需要在 iter0 computation，push 跟 pull 都完成后才能开始，那么就是Synchronous，反之就是Asynchronous.
Asynchronous Task：能够提高系统的效率（因为节省了很多等待的过程），但是，它的缺点就是容易降低算法的收敛速率；
所以，系统性能跟算法收敛速率之间是存在一个trade-off的，你需要同时考虑：
> - 算法对于参数非一致性的敏感度；
- 训练数据特征之间的关联度；
- 硬盘的存储容量；
考虑到用户使用的时候会有不同的情况，parameter server 为用户提供了多种任务依赖方式：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-consistency.png)
Sequential： 这里其实是 synchronous task，任务之间是有顺序的，只有上一个任务完成，才能开始下一个任务； 
Eventual： 跟 sequential 相反，所有任务之间没有顺序，各自独立完成自己的任务， 
Bounded Delay： 这是sequential 跟 eventual 之间的trade-off，可以设置一个 τ 作为最大的延时时间。也就是说，只有 >τ 之前的任务都被完成了，才能开始一个新的任务；极端的情况： 
0τ=0，情况就是 Sequential； 
τ=∞，情况就是 Eventual；
看一个bounded delay 的 PGD (proximal gradient descent)算法的系统运行流程：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-blog-dbpg_2.jpg)
注意：τ 不是越大越好的，具体要取多大，得看具体情况，贴一张李少帅做的实验（Ad click prediction）：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-bounded-delay.png)
# Implementation
servers 使用 一致性hash 来存储参数k-v键值对。用链式复制方式来提高系统容错性。使用 range based communication 和 range based vector clocks 来进一步优化系统；
## Vector Clock
parameter server 使用 vector clock 来记录每个节点中参数的时间戳，能够用来跟踪状态或避免数据的重复发送。但是，假设有n个节点，m个参数，那么vector clock的空间复杂度就是O(n∗m). 显然，当有几千个节点和几十亿的参数时，对于内存和带宽来说都是不可实现的。
好在，parameter server 在push跟pull的时候，都是rang-based，这就带来了一个好处：这个range里面的参数共享的是同一个时间戳，这显然可以大大降低了空间复杂度。
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-vector-clock.png)
每次从一个range里再提取一个range，最多会生成3个新的 vector clocks（一分为三） . 假设k是算法中产生的所有的range，那么空间复杂度就变成了O(k∗m)，因为k 远小于参数个数，所以空间复杂度大大降低了；
## Messages
一条 message 包括：时间戳，len(range)对k-v. 
111[vc(R),(k1,v1),...,(kp,vp)]kj∈Randj∈{1,...p}
这是parameter server 中最基本的通信格式，不仅仅是共享的参数才有，task 的message也是这样的格式，只要把这里的(key, value) 改成 (task ID, 参数/返回值)。
由于机器学习问题通常都需要很高的网络带宽，因此信息的压缩是必须的。
key的压缩： 因为训练数据通常在分配之后都不会发生改变，因此worker没有必要每次都发送相同的key，只需要接收方在第一次接收的时候缓存起来就行了。第二次，worker不再需要同时发送key和value，只需要发送value 和 key list的hash就行。这样瞬间减少了一般的通信量。 
value的压缩： 假设参数时稀疏的，那么就会有大量的0存在。因此，为了进一步压缩，我们只需要发送非0值。parameter server使用 *Snappy* 快速压缩库来压缩数据、高效去除0值。
> 
key 的压缩和 value 的压缩可以同时进行。
用户自定义过滤：
对于机器学习优化问题比如梯度下降来说，并不是每次计算的梯度对于最终优化都是有价值的，用户可以通过自定义的规则过滤一些不必要的传送，再进一步压缩带宽cost： 
1. 发送很小的梯度值是低效的：
因此可以自定义设置，只在梯度值较大的时候发送； 
2. 更新接近最优情况的值是低效的：
因此，只在非最优的情况下发送，可通过KKT来判断；
## Replication and Consistency
parameter server 在数据一致性上，使用的是传统的一致性哈希算法，参数key与server node id被插入到一个hash ring中。
具体的一致性hash算法不是本文的重点，这里不过多介绍了，不清楚的同学建议看看其他文献熟悉一下。 
只要知道它的作用是在分布式系统中，动态增加和移除节点的同时还能保证系统存储与key分配的性能效率；
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-hash-ring.png)
从上图可以看出，每个节点都复制了它逆时钟方向的k个节点中的key。图中，2k=2，1S1 赋值了 2S2 和 3S3 内的key。
两种方式保证slave跟master之间的数据一致性：
1.默认的复制方式: Chain replication (强一致性, 可靠)：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-chain.png)
a. 更新：只能发生在数据头节点,然后更新逐步后移，直到更新到达尾节点，并由尾节点向客户确认更新成功； 
b. 查询：为保证强一致性，客户查询只能在尾节点进行；
2.Replication after Aggregation：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-agregation.png)
两个worker 节点分别向server传送x和y。server 首先通过一定方式（如：f(x+y) ）进行aggregate，然后再进行复制操作；
当有n个worker的时候，复制只需要k/n的带宽。通常来说，k（复制次数）是一个很小的常数，而n的值大概是几百到几千；
## Server Management
要想实现系统的容错以及动态的扩展系统规模，必须要求系统能够支持动态添加和移除节点。
> 
当有一个 server节点添加 进来的时候会发生什么呢？
1. server manager 会对新的节点分配一些range 的key，这会造成其他server节点的key的变化； 
2. 新节点会获取数据做为训练用，另外会复制k份到slave。 
3. server manager 将节点的变化情况广播出去。接收方可能会移除不再属于自己的数据，并且将未完成的任务提交给新来的节点；
当有一个 worker节点（W）添加 进来的时候会发生什么呢？ 跟server差不多，相对更简单一些： 
1. task scheduler 为W分配数据； 
2. 这个 worker 节点通过网络或者文件系统得到分配到的训练数据。接着，W会从服务器pull参数； 
3. task scheduler 会广播节点的变化情况，可能会使一些节点释放一部分训练数据；
# 参考文献
【1】Mu Li. *Scaling Distributed Machine Learning with the Parameter Server*. 
【2】CMU. [http://parameterserver.org/](http://parameterserver.org/)
【3】Joseph E.Gonzalez. *Emerging Systems For Large-scale Machine Learning*.
