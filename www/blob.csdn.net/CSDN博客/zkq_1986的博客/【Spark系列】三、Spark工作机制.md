# 【Spark系列】三、Spark工作机制 - zkq_1986的博客 - CSDN博客





2017年01月26日 20:06:35[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：1380








# Spark工作机制
|Client|
|----|


|Driver程序|
|----|


|Spark Context|
|----|


|RDD DAG|
|----|


|DAGSchedular|
|----|


|TaskSchedular|
|----|


|SparkEnv|
|----|


|Worker Node|
|----|


|Executor|
|----|


|Task|
|----|


|Task|
|----|


|Cache|
|----|


|Worker Node|
|----|


|Executor|
|----|


|Task|
|----|


|Task|
|----|


|Cache|
|----|


|Cluster Manager|
|----|



图　Spark架构图

## 4.1应用程序执行流程

应用程序的执行流程为： 

1）写好的应用程序，打包成jar文件。然后通过客户端上传到集群。根据Driver的配置模式，要么运行在客户端，要么由master指定worker启动driver进程，并对整个应用程序进行监控和管理。接着，配置一些上下文环境。然后顺序执行代码。

2）RDD的算子包括两大类：一是转换算子，二是行动算子。只有Action算子才会触发Job的提交，也就是说，Spark采用的是惰性机制，在碰到行动算子的时候，才提交作业。接着生成RDD有向无环图DAG，由DAG调度器DAGScheduler转化为阶段Stage DAG，每个阶段Stage中产生相应的任务Task集合，任务调度器TaskScheduler将任务分发到worker上的Executor执行。每个任务对应一个数据块，使用用户定义的函数处理数据块。如图：





### 4.1.1　应用提交与执行方式

Driver配置（deploy-mode）模式包含以下两种方式。

·Driver进程运行在客户端，对应用进行管理监控。（为默认项）

·主节点指定某个Worker节点启动Driver，对应用进行监控管理。





图4-4　Spark Driver位于Client   图4-5　Spark Driver位于Worker节点的应用提交与执行

## 4.2　Spark任务调度模式

Spark有多种运行模式，如单机（Local）模式、Standalone模式、YARN模式、Mesos模式。

### 4.2.1　Spark应用程序之间的调度

一个Executor在一个时间段内只能给一个应用使用。

### 4.2.2　作业调度

不同线程提交的作业Job可以并行运行。一个作业分为多个Stage。整个RDD DAG为一个Job。action算子中的本质是调用Spark上下文（SparkContext）中的runJob提交了Job。

作业的调度主要有FIFO和FAIR两种模式。

**FIFO模式**

FIFO(先进先出)。

**fair模式**

在fair共享模式调度下，多个作业以轮询（round robin）方式为分配资源。考虑到长任务和短任务问题，这样长任务在前，短任务在后，短任务也可以获得不错的响应时间。

### 4.2.3　阶段(Stage)调度

Action算子触发作业的提交，并形成RDD DAG。DAG Scheduler(调度器)负责将RDD DAG转化为Stage（阶段）DAG。Stage的DAG通过最后执行的Stage为根进行广度优先遍历，遍历到最开始执行的Stage并执行，如果提交的Stage仍有未完成的父母Stage，则Stage需要等待其父Stage执行完才能执行。

waitingStages中记录仍有未执行的父母Stage，防止过早执行。runningStages中保存正在执行的Stage，防止重复执行。failedStages中保存执行失败的Stage，需要重新执行，这里的设计是出于容错的考虑。

### 4.2.4　任务(Task)调度

一个应用只有一个任务调度器（TaskScheduler）。所有TaskSetManager都是由这个TaskScheduler调度。一个Stage对也只有一个TaskSetManager。TaskSetManager通过一定次序放入调度池pool中。在调度池中，这些TaskSetMananger又会根据Job ID排序，先提交的Job的TaskSetManager优先调度，然后一个Job内的TaskSetManager ID小的先调度。

**在执行任务时，任务分配规则:**

按照“尽量将任务分配到数据块所存储的位置”原则分配任务。数据块的存储位置请见4.3.3节。

执行地点的选取：

1）  如果是调用过cache（）方法的RDD，则读取内存缓存中分区的数据。

2）  如果在磁盘中，通常最开始的RDD会有相应信息，例如，从HDFS上读取的数据，HDFS分区就是最好的执行地点。

3）  如果不是上面两种情况，将遍历RDD DAG获取第一个窄依赖的父亲RDD对应分区的执行地点。

## 4.3　Spark I/O机制

### 4.3.1　序列化 

序列化是将对象转换为字节流，本质上可以理解为将链表存储的非连续空间的数据存储转化为连续空间存储的数组中。这样就可以将数据进行流式传输或者块存储。

### 4.3.2　压缩 

当大片连续区域进行数据存储并且存储区域中数据重复性高的状况下，数据适合进行压缩。数组或者对象序列化后的数据块可以考虑压缩。所以序列化后的数据可以压缩，使数据紧缩，减少空间开销。

Snappy提供了更高的压缩速度，LZF提供了更高的压缩比，用户可以根据具体需求选择压缩方式。

### 4.3.3　Spark存储系统

可以从以下几个维度理解整个存储系统：类接口、数据读写流程和数据通信。

**（1）     类接口。**

所有外部类都通过块管理器接口（BlockManager）对存储模块（storage）进行操作。

**（2）     数据读写流程**

数据存储分为3个层次：内存、本地磁盘和远程磁盘。在diskManager中，存储块ID(blockId)和文件路径映射。

·数据读取流程

在RDD类中，通过compute方法调用迭代器(iterator)读取某个分区（Partition）的数据。分区是逻辑概念。一个分区对应物理上的一个块（block）。一个Executor负责若干个分区。查看数据存储位置的优先级是：

1）  内存；

2）  Tachyon；

3）  本地磁盘；

4）  远程磁盘

在获取远程数据时，先得到远程数据路径，然后通过块管理器工作机创建通信管理器,并从远程读取数据。

·数据写入流程



数据写入流程主要分为以下几个步骤。

1）在RDD类中，通过调用compute方法计算要写到哪个分区。

2）然后通过缓存管理器（CacheManager）调用块管理器（BlockManager），判断数据是否已经写入，如果未写则写入。

3）块管理器（BlockManager）根据指定的存储层次向相应块写入数据。并向主节点汇报存储状态。

·MemoryStore：提供Block在内存中的Block读写功能。 

·DiskStore：提供Block在磁盘上以文件形式读写的功能。

·BlockManagerWorker：对远端数据的异步传输进行管理。

·ConnectionManager：提供本地机器和远端节点进行网络传输Block的功能。 

**（3）     数据通信**

主节点和从节点之间通过Actor传送信息。

·BlockManagerMasterActor：在主节点创建，所有从节点都用于这个Actor引用，并通过这个Actor的引用向主节点传递信息。

·BlockManagerSlaveActor：在从节点创建，主节点拥有所有从节点的这个Actor引用，通过这个Actor引用向从节点传递控制信息（命令）。



块管理器（BlockManager）在内部封装块管理器Master（BlockManagerMaster），并通过BlockManagerMaster对Actor通信进行管理。各从节点的块管理器（BlockManager）对象在Spark上下文环境中（SparkEnv）中创建。在SparkEnv中也会创建其他管理组件，例如connectionManager、broadcastManager、cacheManager等。

##### 4.3.3.2 Spark的数据存储



图3-2　RDD数据管理模型

在物理上，RDD对象实质上是一个元数据结构，存储着块、节点（Block、Node）等的映射关系，以及其他的元数据信息。数据块Block对应一个分区，若干个分区组成一个RDD。

分区是逻辑概念，变换前后的分区在物理上可能处在同一块内存。这是很重要的优化，以防止函数式数据不变性（immutable）导致的内存需求无限扩张。如果要重复使用数据（机器学习中多次迭代），可以调用cache()方法缓存数据。图3-2为RDD的数据存储模型。

## 4.4　Spark通信模块

下面介绍分布式通信的几种方式。 

（1）       RPC（Remote Produce Call）

RPC是远程过程调用协议，基于C/S模型调用。过程大致可以理解为本地分布式对象向本机发请求，不用自己编写底层通信本机。通过网络向服务器发送请求，服务器对象接收参数后，进行处理，再把处理后的结果发送回客户端。

（2）       RMI（Remote MethodInvocation） 

RMI（远程方法调用）和RPC一样都是调用远程的方法，可以把RMI看做是用Java语言实现了RPC协议。RPC不支持对象通信，支持对象传输，这也是RMI相比于RPC的优越之处。


（3）       JMS（Java Message Service）

JMS, java消息服务是Java平台中关于面向消息中间件（MOM）的API，用于在两个应用程序之间，或分布式系统中发送消息，进行异步通信。其支持P2P和pub/stub两种消息模型，即点对点和发布订阅两种模型。其优点在于：支持异步通信、消息生产者和消费者耦合度低。应用程序通过读写队列消息（针对应用程序的数据）来通信，而无须专用连接来连接它们。

### 4.4.1　通信框架AKKA Spark

Spark模块间通信使用AKKA框架。AKKA是用Scala开发的一个库，用于编写Actor模型应用。Actor是一些包含状态和行为的对象。每一个actor拥有自己的属性和方法，从而使得Actor模型容易并发执行。

Actor通过**消息邮件队列**通信。发送端通过“！”符号发送消息，接收端通过receive方法中的case模式匹配接收消息，并进行相应处理。这些通信是异步的。

通常一个**Actor系统**是一个重量级结构。它会分配多个线程。所以对于每**一个应用**，一般只要**一个Actor系统**。



AKKAActor树形结构

一个Actor会创建**多个可子Actor**，并**负责监督**这些子Actor，让这些子Actor完成小的任务。同时，子Actor又可下分为多个子Actor。

### 4.4.2　Client、Master和Worker间的通信

在Standalone模式下，存在三个角色： client、master、worker。 

·Client：提交作业。

·Master：负责接收作业，并启动Driver，管理Worker和Executor。 

·Worker：期性地通过beatheart向Master发送状态信息。当master向它传来启动executor命令的时候，它就启动Executor进行计算。

## 4.5　容错机制 

一般来说，分布式数据集的容错性有两种方式：数据**检查点**和**记录数据的更新**。数据检查点操作成本很高，因此，Spark选择**记录更新**的方式。RDD只支持**粗粒度**转换，是对**全局数据**做同样的**重做**进而恢复数据。

## 4.6　Shuffle机制

Shuffle的本义是洗牌、混洗，即把一组有一定规则的数据打散重新组合转换成一组无规则随机数据分区。Spark中的Shuffle更像是洗牌的逆过程，把一组无规则的数据尽量转换成一组具有一定规则的数据，Spark中的Shuffle和MapReduce中的Shuffle思想相同。

4.7 union, aggregate, join, concatenation区别

·union合并,例：

1,2

3,4  

union  

4,2

1,3  =》                               1,2

                                                          3,4

                                                          4,2

1,2

·aggregate 聚集

1,2

3,4  

union  

4,2

1,3  =》                               1,2

1,2



                                                          3,4

                                                          4,2

与union合并相比多了一个排序

·join 联接，类似数据库的联接操作（通过关键词联接）

数据1：

1,23,23

2,23,12

3,333，112

…

数据2：

1，  we,asd

2，  sd,asd

3，  llksd,asd

数据1 join 数据2：

1,23,23，we,asd

2,23,12，sd,asd

3,333，112，llksd,asd

·concatenation结合，连接（英语，共同迎合）



