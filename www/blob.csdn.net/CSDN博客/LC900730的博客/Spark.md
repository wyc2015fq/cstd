# Spark - LC900730的博客 - CSDN博客
2017年12月10日 11:00:46[lc900730](https://me.csdn.net/LC900730)阅读数：136
Spark目标是为基于工作集的应用(即多个并行操作重用中间结果的应用)提供抽象，同时保持MapReduce及其相关模型的优势特性，即自动容错、位置感知性调度和可伸缩性。
每个RDD有5个主要属性 
* 一组分片(Partition):即数据集的基本组成单位。对于RDD来说，每个分片都会被一个计算任务处理，并决定并行计算的粒度。 
    一个分区肯定在一个节点上，一个节点上游很多分区 
    rdd1.map(_*10):即对这个rdd中每个分片都✖️10 
* 一个计算每个分区的函数。每个RDD都会实现compute函数以达到这个目的。 
* RDD之间依赖关系。RDD每次转换都会生成一个新的RDD，所以RDD之间就会形成类似于流水线一样的前后依赖关系。在部分分区数据丢失时，Spark可以通过这个依赖关系重新计算丢失的分区数据，而不是对RDD所有分区进行重新计算。
- 一个Partition，即RDD的分片函数。当前Spark中实现了2种类型的分片函数，一个是基于哈希的HashPartitioner，另一个是基于范围的RangePartitioner。只有基于key-value的RDD，才会有Partitioner，非key-value的RDD的Partitioner的值是None。Partitioner函数不但决定了RDD本身的分片数量，也决定了parent RDD Shuffle输出时的分片数量。 
hash partition决定在哪个分区
- 一个列表，存储存取每个Partitioner的优先位置。对于一个HDFS文件来说，这个列表保存的就是每个Partitioner所有的块的位置。 
    最佳位置。
下载 
rpm 
* [http://www.scala-lang.org/download/](http://www.scala-lang.org/download/) 选择scala-2.12.4.rpm  
* rpm -ivh scala-2.12.4.rpm  
which scala可以看出版本等信息
启动： 
正常启动hadoop后 
单独启动master与worker 
centos701：./start-master.sh 
centos702与centos703：./start-slave.sh spark://centos701:7077 
jps后发现一切正常
shell下启动 
./bin/spark-shell – master spark://centos701:7077
spark-submit –class org.apache.spark.examples.SparkPi –master spark://centos701:7077 /bigdata/spark-1.6.1-bin-hadoop2.6/lib/spark-example-1.6.1-bin-hadoop2.6.0.jar 100
./spark-submit –class org.apache.spark.examples.SparkPi –driver-memory 512m –executor-memory 180m /usr/local/spark-1.6.1-bin-hadoop2.6/lib/spark-examples-1.6.1-hadoop2.6.0.jar 100 
迭代100次计算pi的值。    
