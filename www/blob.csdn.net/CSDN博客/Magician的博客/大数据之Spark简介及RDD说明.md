
# 大数据之Spark简介及RDD说明 - Magician的博客 - CSDN博客


2018年04月07日 18:01:07[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：138所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)
[前言：](#前言)
[1、Spark概述](#1spark概述)[1.1、什么是Spark（官网：http://spark.apache.org）](#11什么是spark官网httpsparkapacheorg)
[1.2、为什么要学Spark](#12为什么要学spark)
[1.3、Spark特点](#13spark特点)
[2、RDD概述](#2rdd概述)[2.1、什么是RDD](#21什么是rdd)
[2.2、RDD的属性](#22rdd的属性)
[2.3、创建RDD的两种方式](#23创建rdd的两种方式)
[2.4、RDD编程API](#24rdd编程api)
[2.5、RDD的依赖关系](#25rdd的依赖关系)
[2.6、RDD的缓存](#26rdd的缓存)
[2.7、DAG的生成](#27dag的生成)
[总结：](#总结)

# 目录
# 前言：
本篇文章只是简单介绍下Spark，然后对Spark的RDD在做一个全面的介绍。由于博主知识有限，这里只是做一个简单的介绍。若有些地方有问题，请大家及时指出。后续随着深入的学习，会再进一步总结自己的学习成果。
# 1、Spark概述
## 1.1、什么是Spark（官网：
## [http://spark.apache.org](http://spark.apache.org)
## ）
![这里写图片描述](https://img-blog.csdn.net/2018040717401451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/2018040717401451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Spark是一种快速、通用、可扩展的大数据分析引擎，2009年诞生于加州大学伯克利分校AMPLab，2010年开源，2013年6月成为Apache孵化项目，**2014年2月成为Apache顶级项目**。目前，Spark生态系统已经发展成为一个包含多个子项目的集合，其中包含**SparkSQL、Spark Streaming、GraphX、MLlib等子项目**，**Spark是基于内存计算的大数据并行计算框架**。**Spark基于内存计算，提高了在大数据环境下数据处理的实时性，同时保证了高容错性和高可伸缩性，允许用户将Spark部署在大量廉价硬件之上，形成集群**。Spark得到了众多大数据公司的支持，这些公司包括Hortonworks、IBM、Intel、Cloudera、MapR、Pivotal、百度、阿里、腾讯、京东、携程、优酷土豆。当前百度的Spark已应用于凤巢、大搜索、直达号、百度大数据等业务；阿里利用GraphX构建了大规模的图计算和图挖掘系统，实现了很多生产系统的推荐算法；腾讯Spark集群达到8000台的规模，是当前已知的世界上最大的Spark集群。
[

](https://img-blog.csdn.net/2018040717401451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 1.2、为什么要学Spark
[
](https://img-blog.csdn.net/2018040717401451?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**中间结果输出：基于MapReduce的计算引擎通常会将中间结果输出到磁盘上，进行存储和容错。**出于任务管道承接的，考虑，当一些查询翻译到MapReduce任务时，往往会产生多个Stage，而这些串联的Stage又依赖于底层文件系统（如HDFS）来存储每一个Stage的输出结果
**Spark是MapReduce的替代方案，而且兼容HDFS、Hive，可融入Hadoop的生态系统，以弥补MapReduce的不足。**
## 1.3、Spark特点
**1.3.1、快**
**与Hadoop的MapReduce相比，Spark基于内存的运算要快100倍以上，基于硬盘的运算也要快10倍以上**。Spark实现了高效的DAG执行引擎，可以通过基于内存来高效处理数据流。
![这里写图片描述](https://img-blog.csdn.net/20180407174345477?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180407174345477?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**1.3.2、易用**
Spark支持Java、Python和Scala的API，还支持超过80种高级算法，使用户可以快速构建不同的应用。而且Spark支持交互式的Python和Scala的shell，可以非常方便地在这些shell中使用Spark集群来验证解决问题的方法。
![这里写图片描述](https://img-blog.csdn.net/20180407174356739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180407174356739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**1.3.3、通用**
Spark提供了统一的解决方案。**Spark可以用于批处理、交互式查询（Spark SQL）、实时流处理（Spark Streaming）、机器学习（Spark MLlib）和图计算（GraphX）**。这些不同类型的处理都可以在同一个应用中无缝使用。Spark统一的解决方案非常具有吸引力，毕竟任何公司都想用统一的平台去处理遇到的问题，减少开发和维护的人力成本和部署平台的物力成本。
**1.3.4、兼容性**
Spark可以非常方便地与其他的开源产品进行融合。比如，Spark可以使用Hadoop的YARN和Apache Mesos作为它的资源管理和调度器，并且可以处理所有Hadoop支持的数据，包括HDFS、HBase和Cassandra等。这对于已经部署Hadoop集群的用户特别重要，因为不需要做任何数据迁移就可以使用Spark的强大处理能力。Spark也可以不依赖于第三方的资源管理和调度器，它实现了Standalone作为其内置的资源管理和调度框架，这样进一步降低了Spark的使用门槛，使得所有人都可以非常容易地部署和使用Spark。此外，Spark还提供了在EC2上部署Standalone的Spark集群的工具。
![这里写图片描述](https://img-blog.csdn.net/20180407174420592?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 2、RDD概述
## 2.1、什么是RDD
RDD（Resilient Distributed Dataset）叫做分布式数据集，是Spark中最基本的数据抽象，它代表一个不可变、可分区、里面的元素可并行计算的集合。RDD具有数据流模型的特点：自动容错、位置感知性调度和可伸缩性。RDD允许用户在执行多个查询时显式地将工作集缓存在内存中，后续的查询能够重用工作集，这极大地提升了查询速度。
## 2.2、RDD的属性
![这里写图片描述](https://img-blog.csdn.net/20180407174600653?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一组分片（Partition），即数据集的基本组成单位。对于RDD来说，每个分片都会被一个计算任务处理，并决定并行计算的粒度。用户可以在创建RDD时指定RDD的分片个数，如果没有指定，那么就会采用默认值。默认值就是程序所分配到的CPU Core的数目。
一个计算每个分区的函数。Spark中RDD的计算是以分片为单位的，每个RDD都会实现compute函数以达到这个目的。compute函数会对迭代器进行复合，不需要保存每次计算的结果。
RDD之间的依赖关系。*RDD的每次转换都会生成一个新的RDD，所以RDD之间就会形成类似于流水线一样的前后依赖关系。在部分分区数据丢失时，Spark可以通过这个依赖关系重新计算丢失的分区数据，而不是对RDD的所有分区进行重新计算。*
一个Partitioner，即RDD的分片函数。当前Spark中实现了两种类型的分片函数，一个是基于哈希的HashPartitioner，另外一个是基于范围的RangePartitioner。只有对于于key-value的RDD，才会有Partitioner，非key-value的RDD的Parititioner的值是None。Partitioner函数不但决定了RDD本身的分片数量，也决定了parent RDD Shuffle输出时的分片数量。
一个列表，存储存取每个Partition的优先位置（preferred location）。对于一个HDFS文件来说，这个列表保存的就是每个Partition所在的块的位置。按照“移动数据不如移动计算”的理念，Spark在进行任务调度的时候，会尽可能地将计算任务分配到其所要处理数据块的存储位置。

## 2.3、创建RDD的两种方式
由一个已经存在的Scala集合创建。
val rdd1 = sc.parallelize(Array(1,2,3,4,5,6,7,8))
由外部存储系统的数据集创建，包括本地的文件系统，还有所有Hadoop支持的数据集，比如HDFS、Cassandra、HBase等
val rdd2 = sc.textFile(“hdfs://node1.itcast.cn:9000/words.txt”)
2.4、RDD编程API
2.4.1、Transformation
RDD中的所有转换都是延迟加载的，也就是说，它们并不会直接计算结果。相反的，它们只是记住这些应用到基础数据集（例如一个文件）上的转换动作。只有当发生一个要求返回结果给Driver的动作时，这些转换才会真正运行。这种设计让Spark更加有效率地运行。
常用的Transformation：
转换                               含义map(func)返回一个新的RDD，该RDD由每一个输入元素经过func函数转换后组成filter(func)返回一个新的RDD，该RDD由经过func函数计算后返回值为true的输入元素组成flatMap(func)类似于map，但是每一个输入元素可以被映射为0或多个输出元素（所以func应该返回一个序列，而不是单一元素）mapPartitions(func)类似于map，但独立地在RDD的每一个分片上运行，因此在类型为T的RDD上运行时，func的函数类型必须是Iterator[T] => Iterator[U]mapPartitionsWithIndex(func)类似于mapPartitions，但func带有一个整数参数表示分片的索引值，因此在类型为T的RDD上运行时，func的函数类型必须是(Int,Interator[T])=> Iterator[U]sample(withReplacement, fraction, seed)根据fraction指定的比例对数据进行采样，可以选择是否使用随机数进行替换，seed用于指定随机数生成器种子union(otherDataset)对源RDD和参数RDD求并集后返回一个新的RDDintersection(otherDataset)对源RDD和参数RDD求交集后返回一个新的RDDdistinct([numTasks])) 对源RDD进行去重后返回一个新的RDDgroupByKey([numTasks])在一个(K,V)的RDD上调用，返回一个(K,Iterator[V])的RDDreduceByKey(func, [numTasks])在一个(K,V)的RDD上调用，返回一个(K,V)的RDD，使用指定的reduce函数，将相同key的值聚合到一起，与groupByKey类似，reduce任务的个数可以通过第二个可选的参数来设置aggregateByKey(zeroValue)(seqOp, combOp, [numTasks])sortByKey([ascending], [numTasks])在一个(K,V)的RDD上调用，K必须实现Ordered接口，返回一个按照key进行排序的(K,V)的RDDsortBy(func,[ascending], [numTasks])与sortByKey类似，但是更灵活join(otherDataset, [numTasks])在类型为(K,V)和(K,W)的RDD上调用，返回一个相同key对应的所有元素对在一起的(K,(V,W))的RDDcogroup(otherDataset, [numTasks])在类型为(K,V)和(K,W)的RDD上调用，返回一个(K,(Iterable<V>,Iterable<W>))类型的RDDcartesian(otherDataset)笛卡尔积pipe(command, [envVars])coalesce(numPartitions)repartition(numPartitions)repartitionAndSortWithinPartitions(partitioner)2.4.2、 Action
动作                        含义reduce(func)通过func函数聚集RDD中的所有元素，这个功能必须是课交换且可并联的collect()在驱动程序中，以数组的形式返回数据集的所有元素count()返回RDD的元素个数first()返回RDD的第一个元素（类似于take(1)）take(n)返回一个由数据集的前n个元素组成的数组takeSample(withReplacement,num, [seed])返回一个数组，该数组由从数据集中随机采样的num个元素组成，可以选择是否用随机数替换不足的部分，seed用于指定随机数生成器种子takeOrdered(n, [ordering])saveAsTextFile(path)将数据集的元素以textfile的形式保存到HDFS文件系统或者其他支持的文件系统，对于每个元素，Spark将会调用toString方法，将它装换为文件中的文本saveAsSequenceFile(path)将数据集中的元素以Hadoopsequencefile的格式保存到指定的目录下，可以使HDFS或者其他Hadoop支持的文件系统。saveAsObjectFile(path)countByKey()针对(K,V)类型的RDD，返回一个(K,Int)的map，表示每一个key对应的元素个数。foreach(func)在数据集的每一个元素上，运行函数func进行更新。2.5、RDD的依赖关系
RDD和它依赖的父RDD（s）的关系有两种不同的类型，即窄依赖（narrow dependency）和宽依赖（wide dependency）。
2.5.1、窄依赖
窄依赖指的是每一个父RDD的Partition最多被子RDD的一个Partition使用
总结：窄依赖我们形象的比喻为独生子女
2.5.2、宽依赖
宽依赖指的是多个子RDD的Partition会依赖同一个父RDD的Partition
总结：宽依赖我们形象的比喻为超生
![这里写图片描述](https://img-blog.csdn.net/20180407175434238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.6、RDD的缓存
Spark速度非常快的原因之一，就是在不同操作中可以在内存中持久化或缓存个数据集。当持久化某个RDD后，每一个节点都将把计算的分片结果保存在内存中，并在对此RDD或衍生出的RDD进行的其他动作中重用。这使得后续的动作变得更加迅速。RDD相关的持久化和缓存，是Spark最重要的特征之一。可以说，缓存是Spark构建迭代式算法和快速交互式查询的关键。
2.6.1、RDD缓存方式
RDD通过persist方法或cache方法可以将前面的计算结果缓存，但是并不是这两个方法被调用时立即缓存，而是触发后面的action时，该RDD将会被缓存在计算节点的内存中，并供后面重用。
![这里写图片描述](https://img-blog.csdn.net/20180407175713487?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180407175713487?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
通过查看源码发现cache最终也是调用了persist方法，默认的存储级别都是仅在内存存储一份，Spark的存储级别还有好多种，存储级别在object StorageLevel中定义的。
![这里写图片描述](https://img-blog.csdn.net/2018040717572143?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/2018040717572143?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
缓存有可能丢失，或者存储存储于内存的数据由于内存不足而被删除，RDD的缓存容错机制保证了即使缓存丢失也能保证计算的正确执行。通过基于RDD的一系列转换，丢失的数据会被重算，由于RDD的各个Partition是相对独立的，因此只需要计算丢失的部分即可，并不需要重算全部Partition。
[

](https://img-blog.csdn.net/2018040717572143?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)2.7、DAG的生成
[
](https://img-blog.csdn.net/2018040717572143?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)DAG(Directed Acyclic Graph)叫做有向无环图，原始的RDD通过一系列的转换就就形成了DAG，根据RDD之间的依赖关系的不同将DAG划分成不同的Stage，对于窄依赖，partition的转换处理在Stage中完成计算。对于宽依赖，由于有Shuffle的存在，只能在parent RDD处理完成后，才能开始接下来的计算，因此宽依赖是划分Stage的依据。
![这里写图片描述](https://img-blog.csdn.net/20180407175832677?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
总结：
关于本篇文章，你需要了解以下几个知识点：
1、Spark是什么、用来干什么的、有什么特点。
2、RDD是什么、常用的RDD有哪些对应的作用是什么、RDD的依赖关系有哪些。
3、DAG是什么。

