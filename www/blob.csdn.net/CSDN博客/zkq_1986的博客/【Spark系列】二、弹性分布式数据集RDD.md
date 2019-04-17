# 【Spark系列】二、弹性分布式数据集RDD - zkq_1986的博客 - CSDN博客





2017年01月26日 20:03:57[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：336








## 3.1RDD基本概念

### （1）RDD的两种创建方式

1）外部文件创建，如HDFS、本地文件。

2）RDD转换得到新的RDD。

### （2）RDD的两种操作算子

对于RDD可以有两种计算操作算子：Transformation（变换）与Action（行动）。

只有行动（Action）算子才会触发作业（Job）提交。

### （3）RDD的重要内部属性

1）分区列表。

2）计算每个分片的函数。

3）对父RDD的依赖列表。

4）对Key-Value对数据类型RDD的分区器，控制分区策略和分区数。

5）每个数据分区的地址列表（如HDFS上的数据块的地址）。

### （4）窄依赖和宽依赖

窄依赖指，子RDD分区只由一个或多个父RDD中的一个分区转换而来。map操作就是一个父RDD的一个分区，union操作就是两个父RDD的一个分区。

宽依赖指，子RDD的分区由父RDD的所有分区转换而来，即经过过shuffle操作。，如如reduceByKey，groupByKey等。



## 3.2RDD与分布式共享内存的异同

分布式共享内存(Distributed Shared Memory，DSM)是一种**通用**的**内存数据抽象**，这种通用性同时也使其在商用集群上实现有效的**容错**性和**一致**性更加困难。

区别：

表3-1　RDD与DSM的对比
|对比项目|RDD|DSM|
|----|----|----|
|读|批量或细粒度读操作|细粒度读操作|
|写|批量转换操作|细粒度转换操作|
|一致性|不重要（RDD是不可更改的）|取决于应用程序或运行时|
|容错性|细粒度，低开销使用lineage（血统）|需要检查点操作和程序回滚|
|落后任务的处理|任务备份，重新调度执行|很难处理|
|任务安排|基于数据存放的位置自动实现|取决于应用程序|

此外，RDD对于扫描类型操作，如果内存不足以缓存整个RDD，就进行部分缓存，将内存容纳不下的分区存储到磁盘上。

## 3.4　Spark算子分类及功能

### 3.4.1．Saprk算子的作用

图3-3描述了Spark的输入、运行转换、输出。在运行转换中通过算子对RDD进行转

换。算子是RDD中定义的函数，可以对RDD中的数据进行转换和操作。



图3-3　Spark算子和数据空间

1）输入：在Spark程序运行中，数据从外部数据空间（如分布式存储：textFile读取

HDFS等，parallelize方法输入Scala集合或数据）输入Spark，数据进入Spark运行时数据空间，转化为Spark中的数据块，通过BlockManager进行管理。

2）运行：在Spark数据输入形成RDD后便可以通过变换算子，如fliter等，对数据进行操作并将RDD转化为新的RDD，通过Action算子，触发Spark提交作业。如果数据需

要复用，可以通过Cache算子，将数据缓存到内存。

3）输出：程序运行结束数据会输出Spark运行时空间，存储到分布式存储中（如saveAsTextFile输出到HDFS），或Scala数据或集合中（collect输出到Scala集合，count返

回Scalaint型数据）。

Spark的核心数据模型是RDD，但RDD是个抽象类，具体由各子类实现，如MappedRDD、ShuffledRDD等子类。Spark将常用的大数据操作都转化成为RDD的子类。

### 3.4.2 算子分类

算子大致可以分为三大类。

1）Value型的Transformation 算子，这种变换并不触发提交作业，针对处理的数据项是Value 型的数据。

2） Key-Value型的Transfromation算子，这种变换并不触发提交作业，针对处理的数据项是Key-Value 型的数据对。

3）Action 算子，这类算子会触发SparkContext提交作业Job。

### 3.4.3 Value 型Transformation 算子

处理数据类型为Value 型的Transformation 算子可以根据RDD 变换算子的输入分区与输出分区关系分为以下几种类型。

1）输入分区与输出分区一对一型。

2）多对一型。

3）多对多型。

4）输出分区为输入分区子集型。

5）还有一种特殊的输入与输出分区一对一的算子类型：Cache 型。Cache 算子对RDD

分区进行缓存。



**·输入分区与输出分区一对一型**

（1）map

**数据对数据直接转换（通过一种函数）。**



图3-4　map 算子对RDD 转换

（2）flatMap

**数据对数据直接转换（通过一种函数），与map的区别在于RDD中的集合内元素会合并为一个集合**



图3-5　flapMap算子对RDD 转换

（3）mapPartitions

**对分区内的元素按一定函数方式进行转换。**

mapPartitions是map的一个变种。map的输入函数是应用于RDD中**每个元素**，而mapPartitions的输入函数是应用于每个分区，也就是把每个分区中的内容作为整体来处理的。

它的函数定义为：

def mapPartitions[U: ClassTag](f: Iterator[T] => Iterator[U],preservesPartitioning: Boolean = false): RDD[U]

f即为输入函数，它处理每个分区里面的内容。每个分区中的内容将以Iterator[T]传递给输入函数f，f的输出结果是Iterator[U]。最终的RDD由所有分区经过输入函数处理后的结果合并起来的。



图3-6　mapPartitions算子对RDD 转换

（4）glom（一瞥）

将分区内的集合元素形成数组，以进行一瞥的查看。



图3-7　glom 算子对RDD 转换

**·输入分区与输出分区多对一型**

（1）union

使用union 函数时需要保证两个RDD 元素的数据类型相同，返回的RDD 数据类型和被合并的RDD 元素数据类型相同，并不进行去重操作，保存所有元素。如果想去重，可以使用distinct()。++ 符号相当于union 函数操作。



图3-8　union 算子对RDD 转换

（2）cartesian

对两个RDD 内的所有元素进行笛卡尔积操作。操作后，内部实现返回CartesianRDD.



图3-9　cartesian算子对RDD 转换

**·输入分区与输出分区多对多型**

groupBy：将元素通过函数生成相应的Key，数据就转化为Key-Value 格式，之后将Key 相同的元素合为一组。



图3-10　groupBy算子对RDD 转换

**·输出分区为输入分区子集型**

（1）filter

filter 的功能是对元素进行过滤，对每个元素应用f 函数，返回值为true 的元素在RDD

中保留，返回为false 的将过滤掉。内部实现相当于生成FilteredRDD(this，sc.clean(f))。



图3-11　filter 算子对RDD 转换

（2）distinct

distinct 将RDD 中的元素进行去重操作.



图3-12　distinct去重算子对RDD 转换

（3）subtract

subtract 相当于进行集合的差操作，结果为RDD 1中出现的元素而RDD 2 中不出现。

（4）sample

sample 将RDD 这个集合内的元素进行采样，获取所有元素的子集。用户可以设定是否有放回的抽样、百分比、随机种子，进而决定采样方式。内部实现是生成SampledRDD(withReplacement, fraction, seed)。



图3-14　sample 算子对RDD 转换

（5）takeSample

takeSample() 函数和上面的sample 函数是一个原理，但是不使用相对比例采样，而是按设定的采样个数进行采样，同时返回结果不再是RDD，而是相当于对采样后的数据进行Collect()，返回结果的集合为单机的数组。



图3-15　takeSample算子对RDD 转换

**·Cache 型**

（1）cache

cache 将RDD 元素从磁盘缓存到内存，相当于persist(MEMORY_ONLY) 函数的功能。



图3-16　cache 算子对RDD 转换

（2）persist

persist 函数对RDD 进行缓存操作。数据缓存在哪里由StorageLevel枚举类型确定。有以下几种类型的组合：DISK 代表磁盘，MEMORY 代表内存，SER 代表数据是否进行序列化存储。

### 3.3.4　Key-Value 型Transformation 算子

Key-Value 型算子又大致可以分为3 种类型：输入分区与输出分区一对一、聚集和连接操作。

·输入分区与输出分区一对一

mapValues：针对（Key, Value）型数据中的 Value 进行Map 操作，而不对Key 进行

处理。



图3-19　mapValues算子RDD 对转换

·对单个RDD 或两个RDD 聚集

#### （1）单个RDD 聚集

##### 1）combineByKey



图3-20　comBineByKey算子对RDD 转换

##### 2）reduceByKey。

reduceByKey是更简单的一种情况，只是两个值合并成一个值，所以createCombiner

很简单，就是直接返回v.



图3-21　reduceByKey算子对RDD 转换

##### 3）partitionBy。

partitionBy函数对RDD 进行分区操作。如果原有RDD 的分区器和现有分区器（partitioner）一致，则不重分区，如果不一致，则相当于根据分区器生成一个新的ShuffledRDD。通过新的分区策略将原来在不同分区的V1、V2 数据都合并到了一个分区。



图3-22　partitionBy算子对RDD 转换



#### （2）对两个RDD 进行聚集

cogroup函数将两个RDD 进行协同划分. 下图将RDD1 中的数据（U1, 1）、（U1, 2）和 RDD2 中的数据（U1, 2）合并为（U1,（（1, 2）,（ 2）））。



图3-23　Cogroup算子对RDD 转换

**·连接**

##### （1）join

join 对两个需要连接的RDD 进行cogroup函数操作。cogroup操作之后形成的新RDD，对每个key 下的元素进行笛卡尔积操作，返回的结果再展平，对应Key下的所有元组形成一个集合，最后返回RDD[(K, (V, W))]。



图3-24　join 算子对RDD 转换

##### （2）leftOutJoin和rightOutJoin

LeftOutJoin（左外连接）和RightOutJoin（右外连接）相当于在join 的基础上先判断一

侧的RDD 元素是否为空，如果为空，则填充为空。如果不为空，则将数据进行连接运算，并返回结果。

### 3.3.5　Actions 算子

Actions 算子通过SparkContext执行提交作业的runJob操作，触发了RDDDAG(有向无环图)执行。

根据Action 算子的输出空间将Action 算子进行分类：无输出、HDFS、Scala集合和数据类型。

#### 1．无输出

##### （1）foreach

对RDD 中的每个元素都应用f 函数操作，不返回RDD 和Array，而是返回Uint。



图3-25　foreach算子对RDD 转换

#### 2．HDFS

##### （1）saveAsTextFile

函数将数据输出，存储到HDFS 的指定目录。



图3-26　saveAsHadoopFile算子对RDD 转换

##### （2）saveAsObjectFile

saveAsObjectFile将分区中的每10 个元素组成一个**Array**，然后将这个Array 序列化，映射为(Null,BytesWritable(Y)) 的元素，写入HDFS 为SequenceFile的格式。



图3-27　saveAsObjectFile算子对RDD 转换

#### 3．Scala集合和数据类型

##### （1）collect

collect 相当于toArray，toArray已经过时不推荐使用，collect 将分布式的RDD 返回为一个**单机**的scala Array 数组。在这个数组上运用scala的函数式操作。



图3-28　Collect 算子对RDD 转换

##### （2）collectAsMap

collectAsMap对(K, V) 型的RDD 数据返回一个单机HashMap。对于重复K 的RDD 元素，后面的元素覆盖前面的元素。



图3-29　collectAsMap算子对RDD 转换

##### （3）reduceByKeyLocally

实现的是先reduce 再collectAsMap的功能，先对RDD 的整体进行reduce 操作，然后再收集所有结果返回为一个HashMap。

##### （4）lookup

Lookup 函数对(Key, Value) 型的RDD 操作，返回指定Key 对应的元素形成的Seq。



图3-30　lookup 对RDD 转换

##### （5）count

count 返回整个RDD 的元素个数。



图3-31　count 对RDD 转换

##### （6）top

top 可返回最大的k 个元素。例：

result.top(10)(Ordering.by[(String, Int), Int](_._2))

(String, Int), Int：输入类型，排序类型

##### (7) takeOrdered

取最小的几个数，与top相反。

sc.parallelize(Seq(2, 3, 4, 5, 6)).takeOrdered(2)

returns Array(2, 3)

##### （8）reduce

reduce 函数相当于对RDD 中的元素进行reduceLeft函数的操作。

reduceLeft先对两个元素<K，V>进行reduce 函数操作，然后将结果和迭代器取出的下一个元素<k，V>进行reduce 函数操作，直到迭代器遍历完所有元素，得到最后结果。

reduce函数内的方法f是由用户自定义得到。



##### （9）fold

fold 和reduce 的原理相同，但是与reduce 不同，相当于每个reduce 时，迭代器取的第一个元素是zeroValue。



图3-33　fold 算子对RDD 转换

##### （10）aggregate

aggregate 先对每个分区的所有元素进行aggregate 操作，再对分区的结果进行fold操作。aggreagate与fold 和reduce 的不同之处在于，aggregate 相当于采用归并的方式进行数据聚集，这种聚集是并行化的。而在fold 和reduce 函数的运算过程中，每个分区中需要进行串行处理，每个分区串行计算完结果，结果再按之前的方式进行聚集，并返回最终聚集结果。

### 3.3.6 广播（broadcast）变量

广播（broadcast）变量，相当于Hadoop的distributed cache，其广泛用于广播Map SideJoin 中的小表，以及广播大变量等场景。这些数据集合在单节点内存能够容纳，不需要像RDD 那样在节点之间打散存储。Spark
**运行时**把**广播变量**数据发到**各个节点**，并保存下来，后续计算可以复用。相比Hadoop的distributed cache，广播的**内容**可以**跨作业共享**。Broadcast的底层实现采用了**BT**。

### 3.3.7 accumulator 变量

accumulator 变量：允许做全局累加操作，广泛应用于记录当前运行指标的场合。





