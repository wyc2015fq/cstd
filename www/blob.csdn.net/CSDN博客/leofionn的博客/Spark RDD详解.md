
# Spark RDD详解 - leofionn的博客 - CSDN博客


2018年03月05日 13:51:09[leofionn](https://me.csdn.net/qq_36142114)阅读数：41


# 转载http://blog.csdn.net/wangxiaotongfan/article/details/51395769


# 1、RDD是什么
RDD：Spark的核心概念是RDD (resilientdistributed dataset)，指的是一个只读的，可分区的分布式数据集，这个数据集的全部或部分可以缓存在内存中，在多次计算间重用。
RDD的描述
Internally, each RDD is characterized by five main properties:
A list of partitions
A function for computing each split
A list of dependencies on other RDDs
Optionally, a Partitioner for key-value RDDs (e.g. to say that the RDD is hash-partitioned)
Optionally, a list of preferred locations to compute each split on (e.g. block locations for an HDFS file)
下面对RDD的五个特性进行解释：
1、有一个分片列表。就是能被切分，和hadoop一样的，能够切分的数据才能并行计算。
2、有一个函数计算每一个分片，这里指的是下面会提到的compute函数。
3、对其他的RDD的依赖列表，依赖还具体分为宽依赖和窄依赖，但并不是所有的RDD都有依赖。
4、可选：key-value型的RDD是根据哈希来分区的，类似于mapreduce当中的Paritioner接口，控制key分到哪个reduce。
5、可选：每一个分片的优先计算位置（preferred locations），比如HDFS的block的所在位置应该是优先计算的位置。(存储的是一个表，可以将处理的分区“本地化”)
```python
//只计算一次  
  protected
```
```python
def
```
```python
getPartitions
```
```python
:
```
```python
Array[Partition]  
  //对一个分片进行计算，得出一个可遍历的结果
```
```python
def
```
```python
compute
```
```python
(split: Partition, context: TaskContext)
```
```python
:
```
```python
Iterator[T]
  //只计算一次，计算RDD对父RDD的依赖
  protected
```
```python
def
```
```python
getDependencies
```
```python
:
```
```python
Seq[Dependency[_]] = deps
  //可选的，分区的方法，针对第
```
```python
4
```
```python
点，类似于mapreduce当中的Paritioner接口，控制key分到哪个reduce
```
```python
@transient val partitioner: Option[Partitioner] = None
```
```python
//可选的，指定优先位置，输入参数是split分片，输出结果是一组优先的节点位置
  protected
```
```python
def
```
```python
getPreferredLocations
```
```python
(split: Partition)
```
```python
:
```
```python
Seq[String] = Nil
```
1
2
3
4
5
6
7
8
9
10
## 为什么会产生RDD？
（1）传统的MapReduce虽然具有自动容错、平衡负载和可拓展性的优点，但是其最大缺点是采用非循环式的数据流模型，使得在迭代计算式要进行大量的磁盘IO操作。RDD正是解决这一缺点的抽象方法
（2）RDD的具体描述RDD（弹性分布式数据集）是Spark提供的最重要的抽象的概念，它是一种有容错机制的特殊集合，可以分布在集群的节点上，以函数式编程操作集合的方式，进行各种并行操作。可以将RDD理解为一个具有容错机制的特殊集合，它提供了一种只读、只能由已存在的RDD变换而来的共享内存，然后将所有数据都加载到内存中，方便进行多次重用。
a.它是分布式的，可以分布在多台机器上，进行计算。
b.它是弹性的，计算过程中内存不够时它会和磁盘进行数据交换（缓存管理）。
c.这些限制可以极大的降低自动容错开销
d.实质是一种更为通用的迭代并行计算框架，用户可以显示地控制计算的中间结果，然后将其自由运用于之后的计算。
（3）RDD的容错机制实现分布式数据集容错方法有两种：数据检查点和记录更新，RDD采用记录更新的方式：记录所有更新点的成本很高。所以，RDD只支持粗颗粒变换，即只记录单个块（分区）上执行的单个操作，然后创建某个RDD的变换序列（血统 lineage）存储下来；变换序列指，每个RDD都包含了它是如何由其他RDD变换过来的以及如何重建某一块数据的信息。因此RDD的容错机制又称“血统”容错。 要实现这种“血统”容错机制，最大的难题就是如何表达父RDD和子RDD之间的依赖关系。实际上依赖关系可以分两种，窄依赖和宽依赖。窄依赖：子RDD中的每个数据块只依赖于父RDD中对应的有限个固定的数据块；宽依赖：子RDD中的一个数据块可以依赖于父RDD中的所有数据块。例如：map变换，子RDD中的数据块只依赖于父RDD中对应的一个数据块；groupByKey变换，子RDD中的数据块会依赖于多块父RDD中的数据块，因为一个key可能分布于父RDD的任何一个数据块中， 将依赖关系分类的两个特性：第一，窄依赖可以在某个计算节点上直接通过计算父RDD的某块数据计算得到子RDD对应的某块数据；宽依赖则要等到父RDD所有数据都计算完成之后，并且父RDD的计算结果进行hash并传到对应节点上之后才能计算子RDD。第二，数据丢失时，对于窄依赖只需要重新计算丢失的那一块数据来恢复；对于宽依赖则要将祖先RDD中的所有数据块全部重新计算来恢复。所以在“血统”链特别是有宽依赖的时候，需要在适当的时机设置数据检查点。也是这两个特性要求对于不同依赖关系要采取不同的任务调度机制和容错恢复机制。
（4）RDD内部的设计每个RDD都需要包含以下四个部分：
a.源数据分割后的数据块，源代码中的splits变量
b.关于“血统”的信息，源码中的dependencies变量
c.一个计算函数（该RDD如何通过父RDD计算得到），源码中的iterator(split)和compute函数d.一些关于如何分块和数据存放位置的元信息，如源码中的partitioner和preferredLocations例如：a.一个从分布式文件系统中的文件得到的RDD具有的数据块通过切分各个文件得到的，它是没有父RDD的，它的计算函数知识读取文件的每一行并作为一个元素返回给RDD；b.对与一个通过map函数得到的RDD，它会具有和父RDD相同的数据块，它的计算函数式对每个父RDD中的元素所执行的一个函数
# 2、RDD在Spark中的地位及作用
（1）为什么会有Spark？因为传统的并行计算模型无法有效的解决迭代计算（iterative）和交互式计算（interactive）；而Spark的使命便是解决这两个问题，这也是他存在的价值和理由。
（2）Spark如何解决迭代计算？其主要实现思想就是RDD，把所有计算的数据保存在分布式的内存中。迭代计算通常情况下都是对同一个数据集做反复的迭代计算，数据在内存中将大大提升IO操作。这也是Spark涉及的核心：内存计算。
（3）Spark如何实现交互式计算？因为Spark是用scala语言实现的，Spark和scala能够紧密的集成，所以Spark可以完美的运用scala的解释器，使得其中的scala可以向操作本地集合对象一样轻松操作分布式数据集。
（4）Spark和RDD的关系？可以理解为：RDD是一种具有容错性基于内存的集群计算抽象方法，Spark则是这个抽象方法的实现。
# 3、RDD底层实现原理
RDD是一个分布式数据集，顾名思义，其数据应该分部存储于多台机器上。事实上，每个RDD的数据都以Block的形式存储于多台机器上，下图是Spark的RDD存储架构图，其中每个Executor会启动一个BlockManagerSlave，并管理一部分Block；而Block的元数据由Driver节点的BlockManagerMaster保存。BlockManagerSlave生成Block后向BlockManagerMaster注册该Block，BlockManagerMaster管理RDD与Block的关系，当RDD不再需要存储的时候，将向BlockManagerSlave发送指令删除相应的Block。
![这里写图片描述](https://img-blog.csdn.net/20160513165628668)
# 4、RDD cache的原理
RDD的转换过程中，并不是每个RDD都会存储，如果某个RDD会被重复使用，或者计算其代价很高，那么可以通过显示调用RDD提供的cache()方法，把该RDD存储下来。那RDD的cache是如何实现的呢？
RDD中提供的cache()方法只是简单的把该RDD放到cache列表中。当RDD的iterator被调用时，通过CacheManager把RDD计算出来，并存储到BlockManager中，下次获取该RDD的数据时便可直接通过CacheManager从BlockManager读出。
## What is Data Locality - RDD的位置可见性(location preference)
这个问题就不重复造轮子了，直接引用Quora上的一个问答了:
RDD is a dataset which is distributed, that is, it is divided into “partitions”. Each of these partitions can be present in the memory or disk of different machines. If you want Spark to process the RDD, then Spark needs to launch one task per partition of the RDD. It’s best that each task be sent to the machine have the partition that task is supposed to process. In that case, the task will be able to read the data of the partition from the local machine. Otherwise, the task would have to pull the partition data over the network from a different machine, which is less efficient. This scheduling of tasks (that is, allocation of tasks to machines) such that the tasks can read data “locally” is known as “locality aware scheduling”.
# 5、如何操作RDD？
RDD的所有转换操作都是lazy模式，即Spark不会立刻计算结果，而只是简单的记住所有对数据集的转换操作。这些转换只有遇到action操作的时候才会开始计算
（1）如何获取RDDa.从共享的文件系统获取，（如：HDFS）b.通过已存在的RDD转换c.将已存在scala集合（只要是Seq对象）并行化 ，通过调用SparkContext的parallelize方法实现d.改变现有RDD的之久性；RDD是懒散，短暂的。（RDD的固化：cache缓存至内存； save保存到分布式文件系统）
（2）操作RDD的两个动作a.Actions：对数据集计算后返回一个数值value给驱动程序；例如：Reduce将数据集的所有元素用某个函数聚合后，将最终结果返回给程序。b.Transformation：根据数据集创建一个新的数据集，计算后返回一个新RDD；例如：Map将数据的每个元素经过某个函数计算后，返回一个新的分布式数据集。
在spark新版中，也许会有更多的action和transformation，可以参照spark的主页
（3）Actions具体内容：
|operation|explaination|
|---|---|
|reduce(func)|通过函数func聚集数据集中的所有元素。Func函数接受2个参数，返回一个值。这个函数必须是关联性的，确保可以被正确的并发执行|
|collect()|在Driver的程序中，以数组的形式，返回数据集的所有元素。这通常会在使用filter或者其它操作后，返回一个足够小的数据子集再使用，直接将整个RDD集Collect返回，很可能会让Driver程序OOM|
|count()|返回数据集的元素个数|
|take(n)|返回一个数组，由数据集的前n个元素组成。注意，这个操作目前并非在多个节点上，并行执行，而是Driver程序所在机器，单机计算所有的元素(Gateway的内存压力会增大，需要谨慎使用）|
|first()|返回数据集的第一个元素（类似于take（1）|
|saveAsTextFile(path)|将数据集的元素，以textfile的形式，保存到本地文件系统，hdfs或者任何其它hadoop支持的文件系统。Spark将会调用每个元素的toString方法，并将它转换为文件中的一行文本|
|saveAsSequenceFile(path)|将数据集的元素，以sequencefile的格式，保存到指定的目录下，本地系统，hdfs或者任何其它hadoop支持的文件系统。RDD的元素必须由key-value对组成，并都实现了Hadoop的Writable接口，或隐式可以转换为Writable（Spark包括了基本类型的转换，例如Int，Double，String等等）|
|foreach(func)|在数据集的每一个元素上，运行函数func。这通常用于更新一个累加器变量，或者和外部存储系统做交互|
（4）Transformation具体内容
|operation|explaination|
|---|---|
|map(func)|返回一个新的分布式数据集，由每个原元素经过func函数转换后组成|
|filter(func)|返回一个新的数据集，由经过func函数后返回值为true的原元素组成|
|flatMap(func)|类似于map，但是每一个输入元素，会被映射为0到多个输出元素（因此，func函数的返回值是一个Seq，而不是单一元素）|
|sample(withReplacement, frac, seed)|根据给定的随机种子seed，随机抽样出数量为frac的数据|
|union(otherDataset)|返回一个新的数据集，由原数据集和参数联合而成|
|groupByKey([numTasks])|在一个由（K,V）对组成的数据集上调用，返回一个（K，Seq[V])对的数据集。注意：默认情况下，使用8个并行任务进行分组，你可以传入numTask可选参数，根据数据量设置不同数目的Task|
|reduceByKey(func, [numTasks])|在一个（K，V)对的数据集上使用，返回一个（K，V）对的数据集，key相同的值，都被使用指定的reduce函数聚合到一起。和groupbykey类似，任务的个数是可以通过第二个可选参数来配置的。|
|join(otherDataset, [numTasks])|在类型为（K,V)和（K,W)类型的数据集上调用，返回一个（K,(V,W))对，每个key中的所有元素都在一起的数据集|
|groupWith(otherDataset, [numTasks])|在类型为（K,V)和(K,W)类型的数据集上调用，返回一个数据集，组成元素为（K, Seq[V], Seq[W]) Tuples。这个操作在其它框架，称为CoGroup|
|cartesian(otherDataset)|笛卡尔积。但在数据集T和U上调用时，返回一个(T，U）对的数据集，所有元素交互进行笛卡尔积。|



