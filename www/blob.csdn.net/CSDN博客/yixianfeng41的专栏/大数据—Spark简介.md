# 大数据—Spark简介 - yixianfeng41的专栏 - CSDN博客
2016年11月01日 20:30:44[第2梦](https://me.csdn.net/yixianfeng41)阅读数：1727
## 1、Spark是什么
![这里写图片描述](https://img-blog.csdn.net/20161101220905448)
Spark是基于内存计算的大数据并行计算框架，是一个用来实现快速而通用的集群计算平台。它替代了广泛使用的MapReduce计算模型，并且支持交互式查询和流处理等其他高效计算模型。
## 2、Spark与Hadoop的关系
Spark是一个计算框架,它只是MapReduce的替代方案，当然随着spark发展壮大，现在spark也形成了它自己的生态系统；而Hadoop是一个生态系统，包含计算框架MapReduce和分布式文件系统HDFS，还有Hbase,Hive等。  
### 2.1、为何Spark比MapReduce快
1、基于MapReduce的计算引擎通常会将中间结果输出到磁盘上，进行存储和容错，磁盘IO开销是造成MapReduce慢的主要原因。
MapReduce将任务划分成map和reduce两个阶段，map产生的中间结果写回磁盘。那么在这两个阶段之间的shuffle操作就需要从网络中的各个节点进行数据拷贝，那么就会有大量的时间耗费在网络磁盘IO中而不是在计算上。
举个例子，例如像逻辑回归这样的迭代式的计算，MapReduce的弊端会显得更加明显。 
![这里写图片描述](https://img-blog.csdn.net/20161101182844832)
 Hadoop磁盘读写 IO开销大
2、而Spark是基于内存的。 
![这里写图片描述](https://img-blog.csdn.net/20161101183126898)
Spark 基于内存读写 IO开销小 
另一方面，Spark将它的执行模型抽象为有向无环图执行计划（DAG）,也是Saprk较快的原因。DAG可以将多Stage的任务串联或者并行执行，无须将Stage中间结果输出到HDFS中。这里有必要介绍下DAG.
### 2.2、DAG介绍
Spark的计算发生在RDD的action操作，而对action之前的所有transformation，Spark只是记录下RDD生成的轨迹，而不会触发真正的计算。
Spark内核会在需要计算发生的时刻绘制一张关于计算路径的有向无环图，也就是DAG。举个例子，在图2中，从输入中逻辑上生成A和C两个RDD，经过一系列transformation操作，逻辑上生成了F，注意，我们说的是逻辑上，因为这时候计算没有发生，Spark内核做的事情只是记录了RDD的生成和依赖关系。当F要进行输出时，也就是F进行了action操作，Spark会根据RDD的依赖生成DAG，并从起点开始真正的计算。 
![这里写图片描述](https://img-blog.csdn.net/20161101202750013)
### 2.3、Spark优势
Spark相比Hadoop MapReduce的优势如下：
- 
中间结果输出 
Spark将执行模型抽象为有向无环图执行计划（DAG）,DAG可以将多Stage的任务串联或者并行执行，无须将Stage中间结果输出到HDFS中； 
MapReduce将任务划分成map和reduce两个阶段，map产生的中间结果写回磁盘。
- 
容错性好 
Spark引进了弹性分布式数据集RDD (Resilient Distributed Dataset) 的抽象，它是分布在一组节点中的只读对象集合，这些集合是弹性的，如果数据集一部分丢失，则可以根据“血统”（即充许基于数据衍生过程）对它们进行重建。另外在RDD计算时可以通过CheckPoint来实现容错，而CheckPoint有两种方式：CheckPoint Data，和Logging The Updates，用户可以控制采用哪种方式来实现容错。
- 
任务调度的开销 
Spark采用了事件驱动的类库AKKA来启动任务，通过线程池复用线程来避免进程或线程启动和切换开销。
- 
Spark更加通用 
Hadoop只提供了Map和Reduce两种操作，Spark提供的数据集操作类型有很多种，大致分为：Transformations和Actions两大类。 
1）Transformations：Map、Filter、FlatMap、Sample、GroupByKey、ReduceByKey、Union、Join、Cogroup、MapValues、Sort和PartionBy等操作； 
2）Actions：Collect、Reduce、Lookup和Save等操作。
## 3、Spark特点
Spark官网首页所列特点如下：
- 
快速
内存：比hadoop快100倍； 
磁盘：比hadoop快10倍。
- 
使用简单
编程语言：java，scala,python或者R语言进行编程； 
操作：saprk提供了80多种操作，使得spark用起来很是方便。
- 
运行模式多样 
Local （用于测试、开发） 
Standlone （独立集群模式） 
Spark on Yarn （Spark在Yarn上） 
Spark on Mesos （Spark在Mesos）
## 4、Spark生态系统BDAS
Spark生态系统，也就是BDAS。其核心框架是Spark，同时BDAS提供机器学习功能的系统MLbase及底层的分布式机器学习库MLlib、并行图计算框架GraphX、流计算框架Spark Streaming、采样近似计算查询引擎BlinkDB、内存分布式文件系统Tachyon、资源管理框架Mesos等子项目。这些子项目在Spark上层提供了更高层、更丰富的计算范式。如下为BDAS的项目结构图，各模块更详细内容等后续介绍。 
![这里写图片描述](https://img-blog.csdn.net/20161101201000405)
Spark生态系统BDAS 
## 参考资料
1）[大数据计算平台Spark内核全面解读(1)](http://developer.51cto.com/art/201502/464742.htm)
2）Spark 大数据处理 技术、应用与性能优化 高彦杰 著
