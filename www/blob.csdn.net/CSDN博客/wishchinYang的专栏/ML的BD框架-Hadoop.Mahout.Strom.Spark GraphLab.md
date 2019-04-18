# ML的BD框架-Hadoop.Mahout.Strom.Spark/GraphLab - wishchinYang的专栏 - CSDN博客
2014年12月28日 18:23:51[wishchin](https://me.csdn.net/wishchin)阅读数：1109
个人分类：[BigDataMini																[AI/ES																[IAAS																[Spark](https://blog.csdn.net/wishchin/article/category/2802245)](https://blog.csdn.net/wishchin/article/category/2802193)](https://blog.csdn.net/wishchin/article/category/1730787)](https://blog.csdn.net/wishchin/article/category/1834057)
基于Java.Haddop的：  Hadoop.Mahout.Strom.SPark
1.[Spark GraphX 在淘宝的实践](http://www.csdn.net/article/2014-08-07/2821097)
2.[使用Storm进行大数据实时分析](http://www.csdn.net/article/2012-12-24/2813117-storm-realtime-big-data-analysis)
2.1. Storm的淘宝技术博客：[http://www.searchtb.com/2012/09/introduction-to-storm.html](http://www.searchtb.com/2012/09/introduction-to-storm.html)
基于C++图的：GraphLab
GraphLab: [GraphLab是一种新的面向机器学习的并行框架](http://androidfuture.com/blog/?p=215)
## 1.1 GraphLab简介
       在海量数据盛行的今天，大规模并行计算已经随处可见，尤其是MapReduce框架的出现，促进了并行计算在互联网海量数据处理中的广泛应用。而针对海量数据的机器学习对并行计算的性能、开发复杂度等提出了新的挑战。
       机器学习的算法具有下面两个特点：数据依赖性强，运算过程各个机器之间要进行频繁的数据交换；流处理复杂，整个处理过程需要多次迭代，数据的处理条件分支多。
       而MapReduce是典型的SIMD模型，Map阶段集群的各台机器各自完成负载较重的计算过程，数据并行度高，适合完成类似矩阵运算、数据统计等数据独立性强的计算，而对于机器学习类算法并行性能不高。
        另一个并行实现方案就是采用纯MPI（Native MPI）的方式。纯MPI实现通过精细的设计将并行任务按照MPI协议分配到集群机器上，并根据具体应用，在计算过程中进行机器间的数据通信和同步。纯MPI的优点是，可以针对具体的应用，进行深度优化，从而达到很高的并行性能。但纯MPI存在的问题是，针对不同的机器学习算法，需要重写其数据分配、通信等实现细节，代码重用率低，机器拓展性能差，对编程开发人员的要求高，而且优化和调试成本高。因而，纯MPI不适合敏捷的互联网应用。
        为解决机器学习的流处理，Google提出了Pregel框架，Pregel是严格的BSP模型，采用“计算-通信-同步”的模式完成机器学习的数据同步和算法迭代。Goolge曾称其80%的程序使用MapReduce完成，20%的程序使用Pregel实现。因而，Pregel是很成熟的机器学习流处理框架，但Google一直没有将Pregel的具体实现开源，外界对Pregel的模仿实现在性能和稳定性方面都未能达到工业级应用的标准。
        2010年，CMU的Select实验室提出了GraphLab框架，GraphLab是面向机器学习的流处理并行框架[1]。同年， GraphLab基于最初的并行概念实现了1.0版本，在机器学习的流处理并行性能方面得到很大的提升，并引起业界的广泛关注，在2012年GraphLab升级到2.1版本，进一步优化了其并行模型，尤其对自然图的并行性能得到显著改进。
        在本章的余下章节，将详细介绍GraphLab的并行框架和具体的源码实现。
## 1.2 GraphLab并行框架
       GraphLab将数据抽象成Graph结构，将算法的执行过程抽象成Gather、Apply、Scatter三个步骤。其并行的核心思想是对顶点的切分，以下面的例子作为一个说明。
![](http://androidfuture.com/blog/wp-content/uploads/2012/10/GraphLab%E6%A1%86%E6%9E%B6.png)
**图1. Graph对并行思想**
        示例中，需要完成对V0邻接顶点的求和计算，串行实现中，V0对其所有的邻接点进行遍历，累加求和。而GraphLab中，将顶点V0进行切分，将V0的边关系以及对应的邻接点部署在两台处理器上，各台机器上并行进行部分求和运算，然后通过master顶点和mirror顶点的通信完成最终的计算。
### 1.2.1 数据模型：Graph
       顶点是其最小并行粒度和通信粒度，边是机器学习算法中数据依赖性的表现方式。
       对于某个顶点，其被部署到多台机器，一台机器作为master顶点，其余机器上作为mirror。Master作为所有mirror的管理者，负责给mirror安排具体计算任务;mirror作为该顶点在各台机器上的代理执行者，与master数据的保持同步。
       对于某条边，GraphLab将其唯一部署在某一台机器上，而对边关联的顶点进行多份存储，解了边数据量大的问题。
       同一台机器上的所有edge和vertex构成local graph,在每台机器上，存在本地id到全局id的映射表。vertex是一个进程上所有线程共享的，在并行计算过程中，各个线程分摊进程中所有顶点的gather->apply->scatter操作。
       下面这个例子说明，GraphLab是怎么构建Graph的。
![](http://androidfuture.com/blog/wp-content/uploads/2012/10/GraphLab%E5%9B%BE%E7%9A%84%E5%88%87%E5%89%B2.png)
**图2 Graph的构建形式**
### 1.2.2 执行模型：Gather-Apply-Scatter
         每个顶点每一轮迭代经过gather->apple->scatter三个阶段。
1)       Gather阶段
工作顶点的边 (可能是所有边，也有可能是入边或者出边)从领接顶点和自身收集数据，记为gather_data_i，各个边的数据graphlab会求和，记为sum_data。这一阶段对工作顶点、边都是只读的。
2)       Apply阶段
Mirror将gather计算的结果sum_data发送给master顶点，master进行汇总为total。Master利用total和上一步的顶点数据，按照业务需求进行进一步的计算，然后更新master的顶点数据，并同步mirror。Apply阶段中，工作顶点可修改，边不可修改。
3)       Scatter阶段
        工作顶点更新完成之后，更新边上的数据，并通知对其有依赖的邻结顶点更新状态。这scatter过程中，工作顶点只读，边上数据可写。
        在执行模型中，graphlab通过控制三个阶段的读写权限来达到互斥的目的。在gather阶段只读，apply对顶点只写，scatter对边只写。并行计算的同步通过master和mirror来实现，mirror相当于每个顶点对外的一个接口人，将复杂的数据通信抽象成顶点的行为。
        下面这个例子说明GraphLab的执行模型：
![](http://androidfuture.com/blog/wp-content/uploads/2012/10/GraphLab%E6%89%A7%E8%A1%8C%E6%A1%86%E6%9E%B6.png)
***图3. Gather-Apply-Scatter***
## 1.3 GraphLab的源码实现
        Graphlab的实现可以分为四层：基础组件层，抽象层，引擎层，应用层。
![](http://androidfuture.com/blog/wp-content/uploads/2012/10/GraphLab%E6%BA%90%E7%A0%81%E7%BB%93%E6%9E%84.png)
**图4. GraphLab源码结构**
### 1.3.1 基础组件层
       提供Graphlab数据传输、多线程管理等基础并行结构的组件模块，下面将主要介绍其通信、数据序列化、数据交换、多线程管理四个功能模块。
1)       通信(dc_tcp_comm.cpp)
Graphlab基于TCP协议的长连接在机器之间进行数据通信。在Graphlab初始化阶段，所有机器建立连接，将socket数据存储在std::vector<socket_info> sock 结构中。
Graphlab使用单独的线程来接收和发送数据，其中接收或发送都可以配置多个线程，默认每个线程中负责与64台机器进行通信。在接收连接中，tcp_comm基于libevent采用epoll的方式获取连接到达的通知，效率高。将这部分抽象成以下伪代码：
```cpp
listen（）；
for(size_t i = 0;i < nprocs; ++i)
    connect(i);
while{
wait_for_connect（）;
   }
```
in_thread_num=machine_num / proc_per_thread;
- out_thread_num= machine_num / proc_per_thread;
```cpp
for(每一个线程)
{
event_add（）；
}
for(每一个线程)
{
event_add（）；
}
for(每一个线程)
{
In_thread.launch(receive_loop);
}
for(每一个线程)
{
In_thread.launch(send_loop)
}
```
需要补充的是，Graphlab在数据通信中，并没有采用MPI的接口，但在源码中封装了MPI_tools，其用途是在distributed_control::init时，获取系统参数(包括机器IP和端口)提供两种方式，一种是系统配置中初始化，一种是通过MPI接口实现（dc_init_from_mpi::init_param_from_mpi）。
2)       数据序列化(oarchive & iarchive)
Oarchive通过重载操作符>>将对象序列化后写入ostream中，在Graphlab中对于POD( *Plain Old Data*)和非POD数据区分对待， POD类型的数据直接转为为char*写入ostream, 而非POD数据需要用户实现save方法，否则将抛出异常。iarchive的过程与oarchive的过程相反。
所有通过rpc传输的数据都通过oarchive和iarchive转化为stream，比如vertex_program, vertex_data。
![](http://androidfuture.com/blog/wp-content/uploads/2012/10/stream-300x56.png)
**图5. 数据序列化**
3)       数据传输流(buffered_stream_send2.cpp)
Oarchive，iarchive是数据序列化的工具， 在实际的传输过程中，数据并没有立即发送出去，而是缓存在buffered_stream_send。
4)       Pthread_tools：
Thread类封装了lpthread的方法
提供thread_group管理线程队列
封装了锁、信号量、条件变量等同步方法。
### 1.3.2 抽象层
1)      dc_dist_object是GraphLab对所有分布式对象的一个抽象，其目标是将分布式处理的数据对象对用户抽象成普通对象，以希望在使用的时候不需要关心其分布式细节。
2)      buffer_exchange是基于dc_dist_object对需要在顶点间交换的数据提供一个容器。
3)      distribute_controller是基于dc_dist_object实现的一个整个分布式系统的控制器，提供了机器数据、顶点关系等全局信息。
### 1.3.3引擎层
#### 1.3.3.1同步引擎
![](http://androidfuture.com/blog/wp-content/uploads/2012/10/%E5%90%8C%E6%AD%A5-%E5%BC%95%E6%93%8E.png)
**图6. 同步引擎**
1) Excange message阶段，master接受来⾃自mirror的消息；
2) Receive Message阶段，master接收上一轮Scatter发送的消息和mirror发送的消息，将有message的master激活, 对于激活的顶点，master通知mirror激活，并将vectex_program同步到mirrors；
3) Gather阶段，多线程并行gather， 谁先完成，多线程并行localgraph中的顶点，mirror将gather的结果到master；
4) Apply阶段，master执行apply（apply()），并将apply的结果同步到mirror (sync_vertex_data()).
5）Scatter阶段，master和mirror基于新的顶点数据，更新边上数据，并以signal的形式通知相邻顶点。
下面这个例子形象地说明了同步引擎的工作过程：
![](http://androidfuture.com/blog/wp-content/uploads/2012/10/%E5%90%8C%E6%AD%A5%E6%A8%A1%E5%9E%8B%E6%89%A7%E8%A1%8C%E7%A4%BA%E4%BE%8B.png)
**图7. 顶点2的GraphLab执行过程**
#### 1.3.3.2异步引擎
![](http://androidfuture.com/blog/wp-content/uploads/2012/10/%E5%BC%82%E6%AD%A5%E5%BC%95%E6%93%8E.png)
**图8. master和mirror状态转移过程**
      异步引擎中，每个顶点是消息驱动的状态机。
1) 在每一轮执行开始时，Master从全局的调度器(Sceduler)获取消息，获取消息后，master获得锁，并进入Locking状态。同时，master通知mirror获取锁，进入Locking状态。
2) master和mirror分别进行Gathering操作，mirror将gathering结果汇报给master，由master完成汇总。
3) master完成applying之后，将结果同步到mirror上。
4) master和mirror独立的执行scattering，执行完成之后释放锁进入None状态，等待新的任务到来。
5) mirror在scattering状态时，可能再次接收到来自master的locking请求，这种情况下，mirror在完成scattering之后将不会释放锁，而直接进入下一轮任务中。

