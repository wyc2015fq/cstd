# 视频：RDD的特性介绍及源码阅读必备基础 - Spark高级玩法 - CSDN博客
2018年06月27日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：240
**本节课主要讲：**
RDD相关的基础知识
讲解几种常见的RDD及其分区策略：
textFile
sequenceFile
JDBCRDD
HBaseRDD
**RDD的基础讲解**
**RDD的五大特性**
*1  - A list of partitions2  - A function for computing each split3  - A list of dependencies on other RDDs4  - Optionally, a Partitioner for key-value RDDs (e.g. to say that the RDD is hash-partitioned)5  - Optionally, a list of preferred locations to compute each split on (e.g. block locations for    an HDFS file)*
翻译成中文:
1,一个分片列表。也即RDD可以进行分片。
2,每个分片都有计算函数。分片是计算的最小单位
3,一个RDD会依赖于一系列的RDD。并不是所有的RDD都是有依赖。
所有的转换操作都会生成新的RDD，所以就形成了RDD的血缘关系，一个RDD计算失败可以利用其血缘关系进行恢复。
4,可选项。针对 key-values类型的RDD才有的分区器。
5,最佳运行位置 或者 叫偏向运行位置 或者 叫数据的本地性。
**RDD的操作**
转换(Transformations)(如：map, filter, groupBy, join等)，Transformations操作是Lazy的，也就是说从一个RDD转换生成另一个RDD的操作不是马上执行，Spark在遇到Transformations操作时只会记录需要这样的操作，并不会去执行，需要等到有Actions操作的时候才会真正启动计算过程进行计算。
操作(Actions)(如：count, collect, save等)，Actions操作会返回结果或把RDD数据写到存储系统中。Actions是触发Spark启动计算的动因。
**RDD在集群的使用及工作原理**
RDD的声明周期可以分为四步:
**1,创建RDD。**
RDD创建方式
1）从Hadoop文件系统（如HDFS、Hive、HBase）输入创建。
2）从父RDD转换得到新RDD。
3）通过parallelize或makeRDD将单机数据创建为分布式RDD。
4）基于DB(Mysql)、NoSQL(HBase)、S3(SC3)、数据流创建。
**2,构建执行计划。**
- 
,RDD在调用转化算子和action算子后会构成一个RDD链条，也即是RDD的血缘关系。
- 
,DAGScheduler会根据RDD之间的依赖关系进行Stage划分，最终封装成TaskSetManager根据不同的调度模型加入不同的调度队列。
**3,调度执行任务。**
由TaskScheduler和TaskSetManager对TaskSet进行进一步资源封装可最佳位置计算，然后进行调度到相应的Executor上去执行。
**4,结果返回。**
最终的执行结果返回给Driver或者输出到指定的位置。
RDD计算链条
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXWDUBPadOp1tpibZCr97ibp77MdIaHVQIJX7CgalaJwyFT3uwM7h6JibA8Gdr192OKDHsibiaj4aCBr5g/640?wx_fmt=png)
**RDD的依赖关系**
窄依赖：父RDD的一个分区只会被一个子RDD继承：NarrowDependency
宽依赖：父RDD的一个分区可以被若干个子RDD继承：ShuffleDependency
对比,宽依赖，窄依赖：
1，窄依赖允许在一个集群节点上以流水线的方式（pipeline）计算所有父分区。例如，逐个元素地执行map、然后filter操作；宽依赖则需要首先计算好所有父分区数据，然后在节点之间进行Shuffle，与MapReduce类似；
2，窄依赖能够更有效地进行失效节点的恢复，即只需重新计算丢失RDD分区的父分区，而且不同节点之间可以并行计算；而对于一个宽依赖关系的Lineage图，单个节点失效可能导致这个RDD的所有祖先丢失部分分区，因而需要整体重新计算。
**RDD数据分区**
1,通过控制分区数可以。可以控制task数目。
2,所有key-value类型的RDD都支持分区。HashPartitioner是最常用的。也可以自定义分区策略。
3,RDD的很多操作算子都会导致Shuffle，比如ReduceBykey，GroupByKey等。
**Shuffle**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXWDUBPadOp1tpibZCr97ibp7940G1OHvQSJS9ibwAbO6drjcpmFyNpqTiaxl7Kw6hJyNqfrBy6XiaDL7Q/640?wx_fmt=png)
**Hadoop文件的压缩**
|压缩格式|工具|算法|扩展名|Splitable|
|----|----|----|----|----|
|DEFATE|N/A|DEFLATE|.deflate|No|
|Gzip|gzip|DEFLATE|.gz|No|
|Bzip2|Bzip2|Bzip2|.bz2|Yes|
|Lzo|lzop|LZO|.lzo|No|
|LZ4|N/A|LZ4|.lz4|No|
|Snappy|N/A|Snappy|.snappy|No|
更多源码视频，请加入知识星球获取：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFU7Oic5ibSSV5sJGB2RgzSeqQpuBicxOPgCBglwQPt9rnatXN0VblKhw9WicqSGsuTU9o0Sj12698QpuQ/640?wx_fmt=jpeg)
