# Spark入门 - miner_zhu的博客 - CSDN博客





2018年07月13日 09:04:52[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：66标签：[大数据																[spark](https://so.csdn.net/so/search/s.do?q=spark&t=blog)](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)
个人分类：[笔记](https://blog.csdn.net/miner_zhu/article/category/7799287)











Spark具有如下几个**主要特点**：

1运行速度快：Spark使用先进的DAG（Directed Acyclic Graph，**有向无环图**）执行引擎，以支持循环数据流与内存计算，基于**内存**的执行速度可比HadoopMapReduce快上百倍，基于磁盘的执行速度也能快十倍；
    2容易使用：Spark支持使用Scala、Java、Python和R语言进行编程，简洁的API设计有助于用户轻松构建并行程序，并且可以通过Spark Shell进行交互式编程；
    3通用性：Spark提供了完整而强大的技术栈，包括SQL查询、流式计算、机器学习和图算法组件，这些组件可以无缝整合在同一个应用中，足以应对复杂的计算；
    4运行模式多样：Spark可运行于独立的集群模式中，或者运行于Hadoop中，也可运行于Amazon EC2等云环境中，并且可以访问HDFS、Cassandra、HBase、Hive等多种数据源。



Hadoop存在如下一些**缺点**：

1表达能力有限。计算都必须要转化成Map和Reduce两个操作，但这并不适合所有的情况，难以描述复杂的数据处理过程；
    2磁盘IO开销大。每次执行时都需要从磁盘读取数据，并且在计算完成后需要将中间结果写入到磁盘中，IO开销较大；
    3延迟高。一次计算可能需要分解成一系列按顺序执行的MapReduce任务，任务之间的衔接由于涉及到IO开销，会产生较高延迟。而且，在前一个任务执行完成之前，其他任务无法开始，难以胜任复杂、多阶段的计算任务。



相比于MapReduce，Spark主要具有如下**优点**：

1 Spark的计算模式也属于MapReduce，但不局限于Map和Reduce操作，还提供了多种数据集操作类型，编程模型比MapReduce更灵活；
    2Spark提供了**内存**计算，中间结果直接放到内存中，带来了更高的迭代运算效率；
    3Spark基于**DAG**的任务调度执行机制，要优于MapReduce的迭代执行机制。



Spark最大的特点就是将计算数据、中间结果都存储在内存中，大大减少了IO开销，因而，Spark更适合于迭代运算比较多的数据挖掘与机器学习运算。

尽管Spark相对于Hadoop而言具有较大优势，但Spark并不能完全替代Hadoop，主要用于替代Hadoop中的MapReduce计算模型。实际上，Spark已经很好地融入了Hadoop生态圈，并成为其中的重要一员，它可以借助于YARN实现资源调度管理，借助于HDFS实现分布式存储。此外，Hadoop可以使用廉价的、异构的机器来做分布式存储与计算，但是，Spark对硬件的要求稍高一些，对内存与CPU有一定的要求。



在实际应用中，大数据处理主要包括以下**三个类型**：

复杂的批量数据处理：时间跨度通常在数十分钟到数小时之间；
基于历史数据的交互式查询：时间跨度通常在数十秒到数分钟之间；

基于实时数据流的数据处理：时间跨度通常在数百毫秒到数秒之间。



Spark的生态系统主要包含了Spark Core、Spark SQL、Spark Streaming、MLLib和GraphX 等组件，各个**组件**的具体功能如下：
   Spark Core：Spark Core包含Spark的基本功能，如内存计算、任务调度、部署模式、故障恢复、存储管理等。Spark建立在统一的抽象RDD之上，使其可以以基本一致的方式应对不同的大数据处理场景；通常所说的Apache Spark，就是指Spark Core；
   Spark SQL：Spark SQL允许开发人员直接处理RDD，同时也可查询Hive、HBase等外部数据源。Spark SQL的一个重要特点是其能够统一处理关系表和RDD，使得开发人员可以轻松地使用SQL命令进行查询，并进行更复杂的数据分析；
   Spark Streaming：SparkStreaming支持高吞吐量、可容错处理的实时流数据处理，其核心思路是将流式计算分解成一系列短小的批处理作业。SparkStreaming支持多种数据输入源，如Kafka、Flume和TCP套接字等；
   MLlib（机器学习）：MLlib提供了常用机器学习算法的实现，包括聚类、分类、回归、协同过滤等，降低了机器学习的门槛，开发人员只要具备一定的理论知识就能进行机器学习的工作；
   GraphX（图计算）：GraphX是Spark中用于图计算的API，可认为是Pregel在Spark上的重写及优化，Graphx性能良好，拥有丰富的功能和运算符，能在海量数据上自如地运行复杂的图算法。




