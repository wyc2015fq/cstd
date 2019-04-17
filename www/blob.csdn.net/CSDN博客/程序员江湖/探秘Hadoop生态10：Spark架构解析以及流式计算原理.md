# 探秘Hadoop生态10：Spark架构解析以及流式计算原理 - 程序员江湖 - CSDN博客





2017年03月08日 09:46:54[黄小斜](https://me.csdn.net/a724888)阅读数：1091
所属专栏：[后端技术杂谈](https://blog.csdn.net/column/details/25481.html)









# [](http://www.cnblogs.com/zyw-205520/p/4771253.html)



## 导语

spark 已经成为广告、报表以及推荐系统等大数据计算场景中首选系统，因效率高，易用以及通用性越来越得到大家的青睐，我自己最近半年在接触spark以及spark streaming之后，对spark技术的使用有一些自己的经验积累以及心得体会，在此分享给大家。

本文依次从spark生态，原理，基本概念，spark streaming原理及实践，还有spark调优以及环境搭建等方面进行介绍，希望对大家有所帮助。

## spark 生态及运行原理

![](https://blog-10039692.file.myqcloud.com/1493709487351_2754_1493709487748.png)

### Spark 特点
- 
运行速度快 => Spark拥有DAG执行引擎，支持在内存中对数据进行迭代计算。官方提供的数据表明，如果数据由磁盘读取，速度是Hadoop MapReduce的10倍以上，如果数据从内存中读取，速度可以高达100多倍。

- 
适用场景广泛 => 大数据分析统计，实时数据处理，图计算及机器学习

- 
易用性 => 编写简单，支持80种以上的高级算子，支持多种语言，数据源丰富，可部署在多种集群中

- 
容错性高。Spark引进了弹性分布式数据集RDD (Resilient Distributed Dataset) 的抽象，它是分布在一组节点中的只读对象集合，这些集合是弹性的，如果数据集一部分丢失，则可以根据“血统”（即充许基于数据衍生过程）对它们进行重建。另外在RDD计算时可以通过CheckPoint来实现容错，而CheckPoint有两种方式：CheckPoint Data，和Logging The Updates，用户可以控制采用哪种方式来实现容错。


### Spark的适用场景

目前大数据处理场景有以下几个类型：
- 
复杂的批量处理（Batch Data Processing），偏重点在于处理海量数据的能力，至于处理速度可忍受，通常的时间可能是在数十分钟到数小时；

- 
基于历史数据的交互式查询（Interactive Query），通常的时间在数十秒到数十分钟之间

- 
基于实时数据流的数据处理（Streaming Data Processing），通常在数百毫秒到数秒之间


### Spark成功案例

目前大数据在互联网公司主要应用在广告、报表、推荐系统等业务上。在广告业务方面需要大数据做应用分析、效果分析、定向优化等，在推荐系统方面则需要大数据优化相关排名、个性化推荐以及热点点击分析等。这些应用场景的普遍特点是计算量大、效率要求高。

腾讯 / yahoo / 淘宝 / 优酷土豆

## spark运行架构

spark基础运行架构如下所示：

![](https://blog-10039692.file.myqcloud.com/1493710441713_3905_1493710442164.png)

spark结合yarn集群背后的运行流程如下所示：

![](https://blog-10039692.file.myqcloud.com/1493710470901_4658_1493710471619.png)

spark 运行流程：

Spark架构采用了分布式计算中的Master-Slave模型。Master是对应集群中的含有Master进程的节点，Slave是集群中含有Worker进程的节点。
- 
Master作为整个集群的控制器，负责整个集群的正常运行；

- 
Worker相当于计算节点，接收主节点命令与进行状态汇报；

- 
Executor负责任务的执行；

- 
Client作为用户的客户端负责提交应用；

- 
Driver负责控制一个应用的执行。


Spark集群部署后，需要在主节点和从节点分别启动Master进程和Worker进程，对整个集群进行控制。在一个Spark应用的执行过程中，Driver和Worker是两个重要角色。Driver 程序是应用逻辑执行的起点，负责作业的调度，即Task任务的分发，而多个Worker用来管理计算节点和创建Executor并行处理任务。在执行阶段，Driver会将Task和Task所依赖的file和jar序列化后传递给对应的Worker机器，同时Executor对相应数据分区的任务进行处理。
- 
Excecutor /Task 每个程序自有，不同程序互相隔离，task多线程并行

- 
集群对Spark透明，Spark只要能获取相关节点和进程

- 
Driver 与Executor保持通信，协作处理


三种集群模式：

1.Standalone 独立集群

2.Mesos, apache mesos

3.Yarn, hadoop yarn

基本概念：
- 
Application =>Spark的应用程序，包含一个Driver program和若干Executor

- 
SparkContext => Spark应用程序的入口，负责调度各个运算资源，协调各个Worker Node上的Executor

- 
Driver Program => 运行Application的main()函数并且创建SparkContext

- 
Executor => 是为Application运行在Worker node上的一个进程，该进程负责运行Task，并且负责将数据存在内存或者磁盘上。每个Application都会申请各自的Executor来处理任务

- 
Cluster Manager =>在集群上获取资源的外部服务 (例如：Standalone、Mesos、Yarn)

- 
Worker Node => 集群中任何可以运行Application代码的节点，运行一个或多个Executor进程

- 
Task => 运行在Executor上的工作单元

- 
Job => SparkContext提交的具体Action操作，常和Action对应

- 
Stage => 每个Job会被拆分很多组task，每组任务被称为Stage，也称TaskSet

- 
RDD => 是Resilient distributed datasets的简称，中文为弹性分布式数据集;是Spark最核心的模块和类

- 
DAGScheduler => 根据Job构建基于Stage的DAG，并提交Stage给TaskScheduler

- 
TaskScheduler => 将Taskset提交给Worker node集群运行并返回结果

- 
Transformations => 是Spark API的一种类型，Transformation返回值还是一个RDD，所有的Transformation采用的都是懒策略，如果只是将Transformation提交是不会执行计算的

- 
Action => 是Spark API的一种类型，Action返回值不是一个RDD，而是一个scala集合；计算只有在Action被提交的时候计算才被触发。


## Spark核心概念之RDD

![](https://blog-10039692.file.myqcloud.com/1493710739191_5851_1493710739472.png)

## Spark核心概念之Transformations / Actions

![](https://blog-10039692.file.myqcloud.com/1493710771128_2699_1493710771745.png)

Transformation返回值还是一个RDD。它使用了链式调用的设计模式，对一个RDD进行计算后，变换成另外一个RDD，然后这个RDD又可以进行另外一次转换。这个过程是分布式的。 Action返回值不是一个RDD。它要么是一个Scala的普通集合，要么是一个值，要么是空，最终或返回到Driver程序，或把RDD写入到文件系统中。

Action是返回值返回给driver或者存储到文件，是RDD到result的变换，Transformation是RDD到RDD的变换。

只有action执行时，rdd才会被计算生成，这是rdd懒惰执行的根本所在。

## Spark核心概念之Jobs / Stage

Job => 包含多个task的并行计算，一个action触发一个job

stage => 一个job会被拆为多组task，每组任务称为一个stage，以shuffle进行划分

![](https://blog-10039692.file.myqcloud.com/1493710812301_4487_1493710812770.png)

## Spark核心概念之Shuffle

以reduceByKey为例解释shuffle过程。

![](https://blog-10039692.file.myqcloud.com/1493710860785_9941_1493710861234.png)

在没有task的文件分片合并下的shuffle过程如下：（`spark.shuffle.consolidateFiles=false`）

![](https://blog-10039692.file.myqcloud.com/1493710902559_586_1493710903064.png)

fetch 来的数据存放到哪里？

刚 fetch 来的 FileSegment 存放在 softBuffer 缓冲区，经过处理后的数据放在内存 + 磁盘上。这里我们主要讨论处理后的数据，可以灵活设置这些数据是“只用内存”还是“内存＋磁盘”。如果spark.shuffle.spill = false就只用内存。由于不要求数据有序，shuffle write 的任务很简单：将数据 partition 好，并持久化。之所以要持久化，一方面是要减少内存存储空间压力，另一方面也是为了 fault-tolerance。

shuffle之所以需要把中间结果放到磁盘文件中，是因为虽然上一批task结束了，下一批task还需要使用内存。如果全部放在内存中，内存会不够。另外一方面为了容错，防止任务挂掉。

存在问题如下：
- 
产生的 FileSegment 过多。每个 ShuffleMapTask 产生 R（reducer 个数）个 FileSegment，M 个 ShuffleMapTask 就会产生 M * R 个文件。一般 Spark job 的 M 和 R 都很大，因此磁盘上会存在大量的数据文件。

- 
缓冲区占用内存空间大。每个 ShuffleMapTask 需要开 R 个 bucket，M 个 ShuffleMapTask 就会产生 MR 个 bucket。虽然一个 ShuffleMapTask 结束后，对应的缓冲区可以被回收，但一个 worker node 上同时存在的 bucket 个数可以达到 cores R 个（一般 worker 同时可以运行 cores 个 ShuffleMapTask），占用的内存空间也就达到了cores× R × 32 KB。对于 8 核 1000 个 reducer 来说，占用内存就是 256MB。


为了解决上述问题，我们可以使用文件合并的功能。

在进行task的文件分片合并下的shuffle过程如下：（`spark.shuffle.consolidateFiles=true`）

![](https://blog-10039692.file.myqcloud.com/1493710982278_9005_1493710982725.png)

可以明显看出，在一个 core 上连续执行的 ShuffleMapTasks 可以共用一个输出文件 ShuffleFile。先执行完的 ShuffleMapTask 形成 ShuffleBlock i，后执行的 ShuffleMapTask 可以将输出数据直接追加到 ShuffleBlock i 后面，形成 ShuffleBlock i'，每个 ShuffleBlock 被称为 FileSegment。下一个 stage 的 reducer 只需要 fetch 整个 ShuffleFile 就行了。这样，每个 worker 持有的文件数降为 cores× R。FileConsolidation 功能可以通过`spark.shuffle.consolidateFiles=true`来开启。

## Spark核心概念之Cache

```
val rdd1 = ... // 读取hdfs数据，加载成RDD
rdd1.cache

val rdd2 = rdd1.map(...)
val rdd3 = rdd1.filter(...)

rdd2.take(10).foreach(println)
rdd3.take(10).foreach(println)

rdd1.unpersist
```

cache和unpersisit两个操作比较特殊，他们既不是action也不是transformation。cache会将标记需要缓存的rdd，真正缓存是在第一次被相关action调用后才缓存；unpersisit是抹掉该标记，并且立刻释放内存。只有action执行时，rdd1才会开始创建并进行后续的rdd变换计算。

cache其实也是调用的persist持久化函数，只是选择的持久化级别为`MEMORY_ONLY`。

persist支持的RDD持久化级别如下：

![](https://blog-10039692.file.myqcloud.com/1493711054560_6707_1493711054961.png)

需要注意的问题：

Cache或shuffle场景序列化时， spark序列化不支持protobuf message，需要java 可以serializable的对象。一旦在序列化用到不支持java serializable的对象就会出现上述错误。

Spark只要写磁盘，就会用到序列化。除了shuffle阶段和persist会序列化，其他时候RDD处理都在内存中，不会用到序列化。






[Spark Streaming实时计算框架介绍](https://www.cnblogs.com/Leo_wl/p/3530464.html)



阅读目录
- [Spark Streaming实时计算框架介绍](https://www.cnblogs.com/Leo_wl/p/3530464.html#_label0)


[回到目录](https://www.cnblogs.com/Leo_wl/p/3530464.html#_labelTop)

# [Spark Streaming实时计算框架介绍](http://www.cnblogs.com/shenh062326/p/3530092.html)




随着大数据的发展，人们对大数据的处理要求也越来越高，原有的批处理框架MapReduce适合离线计算，却无法满足实时性要求较高的业务，如实时推荐、用户行为分析等。 Spark Streaming是建立在Spark上的实时计算框架，通过它提供的丰富的API、基于内存的高速执行引擎，用户可以结合流式、批处理和交互试查询应用。本文将详细介绍Spark Streaming实时计算框架的原理与特点、适用场景。



## Spark Streaming实时计算框架



    Spark是一个类似于MapReduce的分布式计算框架，其核心是弹性分布式数据集，提供了比MapReduce更丰富的模型，可以在快速在内存中对数据集进行多次迭代，以支持复杂的数据挖掘算法和图形计算算法。Spark Streaming是一种构建在Spark上的实时计算框架，它扩展了Spark处理大规模流式数据的能力。

Spark Streaming的优势在于：
- 
能运行在100+的结点上，并达到秒级延迟。

- 
使用基于内存的Spark作为执行引擎，具有高效和容错的特性。

- 
能集成Spark的批处理和交互查询。

- 
为实现复杂的算法提供和批处理类似的简单接口。


基于云梯Spark on Yarn的Spark Streaming总体架构如图1所示。其中Spark on Yarn的启动流程我的另外一篇文章（《程序员》2013年11月期刊《深入剖析阿里巴巴云梯Yarn集群》）有详细描述，这里不再赘述。Spark on Yarn启动后，由Spark AppMaster把Receiver作为一个Task提交给某一个Spark Executor；Receive启动后输入数据，生成数据块，然后通知Spark AppMaster；Spark AppMaster会根据数据块生成相应的Job，并把Job的Task提交给空闲Spark Executor 执行。图中蓝色的粗箭头显示被处理的数据流，输入数据流可以是磁盘、网络和HDFS等，输出可以是HDFS，数据库等。

![](https://images0.cnblogs.com/blog/287057/201401/221719130226.jpg)

图1 云梯Spark Streaming总体架构

Spark Streaming的基本原理是将输入数据流以时间片（秒级）为单位进行拆分，然后以类似批处理的方式处理每个时间片数据，其基本原理如图2所示。

![](https://images0.cnblogs.com/blog/287057/201401/221719138823.jpg)

图2 Spark Streaming基本原理图

首先，Spark Streaming把实时输入数据流以时间片Δt （如1秒）为单位切分成块。Spark Streaming会把每块数据作为一个RDD，并使用RDD操作处理每一小块数据。每个块都会生成一个Spark Job处理，最终结果也返回多块。

下面介绍Spark Streaming内部实现原理。

使用Spark Streaming编写的程序与编写Spark程序非常相似，在Spark程序中，主要通过操作RDD（Resilient Distributed Datasets弹性分布式数据集）提供的接口，如map、reduce、filter等，实现数据的批处理。而在Spark Streaming中，则通过操作DStream（表示数据流的RDD序列）提供的接口，这些接口和RDD提供的接口类似。图3和图4展示了由Spark Streaming程序到Spark jobs的转换图。

![](https://images0.cnblogs.com/blog/287057/201401/221719142572.jpg)

图3 Spark Streaming程序转换为DStream Graph

![](https://images0.cnblogs.com/blog/287057/201401/221719147728.jpg)

图4 DStream Graph转换为Spark jobs

在图3中，Spark Streaming把程序中对DStream的操作转换为DStream Graph，图4中，对于每个时间片，DStream Graph都会产生一个RDD Graph；针对每个输出操作（如print、foreach等），Spark Streaming都会创建一个Spark action；对于每个Spark action，Spark Streaming都会产生一个相应的Spark job，并交给JobManager。JobManager中维护着一个Jobs队列, Spark job存储在这个队列中，JobManager把Spark job提交给Spark Scheduler，Spark Scheduler负责调度Task到相应的Spark Executor上执行。

Spark Streaming的另一大优势在于其容错性，RDD会记住创建自己的操作，每一批输入数据都会在内存中备份，如果由于某个结点故障导致该结点上的数据丢失，这时可以通过备份的数据在其它结点上重算得到最终的结果。

正如Spark Streaming最初的目标一样，它通过丰富的API和基于内存的高速计算引擎让用户可以结合流式处理，批处理和交互查询等应用。因此Spark Streaming适合一些需要历史数据和实时数据结合分析的应用场合。当然，对于实时性要求不是特别高的应用也能完全胜任。另外通过RDD的数据重用机制可以得到更高效的容错处理。







