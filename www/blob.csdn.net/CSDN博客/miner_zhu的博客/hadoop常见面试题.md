# hadoop常见面试题 - miner_zhu的博客 - CSDN博客





2018年07月13日 09:01:53[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：619











1.什么是hadoop

Hadoop 是一个开源软件框架，用于存储大量数据，并发处理/查询在具有多个商用硬件（即低成本硬件）节点的集群上的那些数据。

         HDFS（HadoopDistributed File System，Hadoop 分布式文件系统）：HDFS 允许你以一种分布式和冗余的方式存储大量数据。例如，1 GB（即 1024 MB）文本文件可以拆分为 16 * 128MB 文件，并存储在 Hadoop 集群中的 8 个不同节点上。每个分裂可以复制 3 次，以实现容错，以便如果 1 个节点故障的话，也有备份。HDFS 适用于顺序的“一次写入、多次读取”的类型访问。

         MapReduce：一个计算框架。它以分布式和并行的方式处理大量的数据。当你对所有年龄> 18 的用户在上述 1 GB 文件上执行查询时，将会有“8 个映射”函数并行运行，以在其 128 MB 拆分文件中提取年龄> 18 的用户，然后“reduce”函数将运行以将所有单独的输出组合成单个最终结果。



**2.**请列出正常工作的hadoop集群中hadoop都需要启动哪些进程，他们的作用分别是什么？

*1) NameNode: HDFS*的守护进程，负责记录文件是如何分割成数据块，以及这些数据块分别被存储到那些数据节点上，它的主要功能是对内存及*IO*进行集中管理

*2) Secondary NameNode*：辅助后台程序，与*NameNode*进行通信，以便定期保存*HDFS*元数据的快照。

*3) DataNode*：负责把*HDFS*数据块读写到本地的文件系统。

*4) JobTracker*：负责分配*task*，并监控所有运行的*task*。

*5) TaskTracker*：负责执行具体的*task*，并与*JobTracker*进行交互。



**3.**请列出你所知道的hadoop调度器，并简要说明其工作方法？

比较流行的三种调度器有：默认调度器*FIFO*，计算能力调度器*CapacityScheduler*，公平调度器*Fair Scheduler*

*1) *默认调度器*FIFO*

*hadoop*中默认的调度器，采用先进先出的原则

*2) *计算能力调度器*CapacityScheduler*

选择占用资源小，优先级高的先执行

*3) *公平调度器*FairScheduler*

同一队列中的作业公平共享队列中所有资源





4.简答说一下hadoop的map-reduce编程模型

首先map task会从本地文件系统读取数据，转换成key-value形式的键值对集合

使用的是hadoop内置的数据类型，比如longwritable、text等

将键值对集合输入mapper进行业务处理过程，将其转换成需要的key-value在输出

之后会进行一个partition分区操作，默认使用的是hashpartitioner，可以通过重写hashpartitioner的getpartition方法来自定义分区规则

之后会对key进行进行sort排序，grouping分组操作将相同key的value合并分组输出，在这里可以使用自定义的数据类型，重写WritableComparator的Comparator方法来自定义排序规则，重写RawComparator的compara方法来自定义分组规则

之后进行一个combiner归约操作，其实就是一个本地段的reduce预处理，以减小后面shufle和reducer的工作量

reduce task会通过网络将各个数据收集进行reduce处理，最后将数据保存或者显示，结束整个job



4、为什么要用flume导入hdfs，hdfs的构架是怎样的

flume可以实时的导入数据到hdfs中，当hdfs上的文件达到一个指定大小的时候会形成一个文件，或者超过指定时间的话也形成一个文件

文件都是存储在datanode上面的，namenode记录着datanode的元数据信息，而namenode的元数据信息是存在内存中的，所以当文件切片很小或者很多的时候会卡死



5、map-reduce程序运行的时候会有什么比较常见的问题

比如说作业中大部分都完成了，但是总有几个reduce一直在运行

这是因为这几个reduce中的处理的数据要远远大于其他的reduce，可能是因为对键值对任务划分的不均匀造成的数据倾斜

解决的方法可以在分区的时候重新定义分区规则对于value数据很多的key可以进行拆分、均匀打散等处理，或者是在map端的combiner中进行数据预处理的操作



7、Hive中存放是什么？

表。 
存的是和hdfs的映射关系，hive是逻辑上的数据仓库，实际操作的都是hdfs上的文件，HQL就是用sql语法来写的mr程序。



8、Hive与关系型数据库的关系？

没有关系，hive是数据仓库，不能和数据库一样进行实时的CURD操作。 
是一次写入多次读取的操作，可以看成是ETL工具。



10、Sqoop工作原理是什么？

hadoop生态圈上的数据传输工具。 
可以将关系型数据库的数据导入非结构化的hdfs、hive或者bbase中，也可以将hdfs中的数据导出到关系型数据库或者文本文件中。 
使用的是mr程序来执行任务，使用jdbc和关系型数据库进行交互。 
import原理：通过指定的分隔符进行数据切分，将分片传入各个map中，在map任务中在每行数据进行写入处理没有reduce。 
export原理：根据要操作的表名生成一个java类，并读取其元数据信息和分隔符对非结构化的数据进行匹配，多个map作业同时执行写入关系型数据库







16、Hadoop性能调优？

调优可以通过系统配置、程序编写和作业调度算法来进行。 
hdfs的block.size可以调到128/256（网络很好的情况下，默认为64） 
调优的大头：mapred.map.tasks、mapred.reduce.tasks设置mr任务数（默认都是1） 
mapred.tasktracker.map.tasks.maximum每台机器上的最大map任务数 
mapred.tasktracker.reduce.tasks.maximum每台机器上的最大reduce任务数 
mapred.reduce.slowstart.completed.maps配置reduce任务在map任务完成到百分之几的时候开始进入 
这个几个参数要看实际节点的情况进行配置，reduce任务是在33%的时候完成copy，要在这之前完成map任务，（map可以提前完成） 
mapred.compress.map.output,mapred.output.compress配置压缩项，消耗cpu提升网络和磁盘io 
合理利用combiner 
注意重用writable对象



12、Spark Streaming和Storm有何区别？

一个实时毫秒一个准实时亚秒，不过storm的吞吐率比较低。



28、ALS算法原理？

答：对于user-product-rating数据，als会建立一个稀疏的评分矩阵，其目的就是通过一定的规则填满这个稀疏矩阵。 
als会对稀疏矩阵进行分解，分为用户-特征值，产品-特征值，一个用户对一个产品的评分可以由这两个矩阵相乘得到。 
通过固定一个未知的特征值，计算另外一个特征值，然后交替反复进行最小二乘法，直至差平方和最小，即可得想要的矩阵。

29、kmeans算法原理？

随机初始化中心点范围，计算各个类别的平均值得到新的中心点。 
重新计算各个点到中心值的距离划分，再次计算平均值得到新的中心点，直至各个类别数据平均值无变化。

30、canopy算法原理？

根据两个阈值来划分数据，以随机的一个数据点作为canopy中心。 
计算其他数据点到其的距离，划入t1、t2中，划入t2的从数据集中删除，划入t1的其他数据点继续计算，直至数据集中无数据。

31、朴素贝叶斯分类算法原理？

对于待分类的数据和分类项，根据待分类数据的各个特征属性，出现在各个分类项中的概率判断该数据是属于哪个类别的。

32、关联规则挖掘算法apriori原理？

一个频繁项集的子集也是频繁项集，针对数据得出每个产品的支持数列表，过滤支持数小于预设值的项，对剩下的项进行全排列，重新计算支持数，再次过滤，重复至全排列结束，可得到频繁项和对应的支持数。



