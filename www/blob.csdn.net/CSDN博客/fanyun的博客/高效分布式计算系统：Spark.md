# 高效分布式计算系统：Spark - fanyun的博客 - CSDN博客
2016年03月18日 11:29:07[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3945
所属专栏：[大数据](https://blog.csdn.net/column/details/bigdata01.html)

### 一. 什么是Spark？
- Spark是UC Berkeley AMP lab所开源的类Hadoop MapReduce的通用的并行计算框架，Spark基于map reduce算法实现的分布式计算，拥有Hadoop MapReduce所具有的优点；但不同于MapReduce的是Job中间输出和结果可以保存在内存中，从而不再需要读写HDFS，因此Spark能更好地适用于数据挖掘与机器学习等需要迭代的map reduce的算法。其架构如下图所示：
### ![spark-framwork](http://jbcdn2.b0.upaiyun.com/2013/09/spark-framwork.jpg)
### 二.Spark与Hadoop的对比
- Spark的中间数据放到内存中，对于迭代运算效率更高。
- Spark更适合于迭代运算比较多的ML和DM运算。因为在Spark里面，有RDD的抽象概念。
- Spark比Hadoop更通用。
- Spark提供的数据集操作类型有很多种，不像Hadoop只提供了Map和Reduce两种操作。比如map, filter, flatMap, sample, groupByKey, reduceByKey, union, join, cogroup, mapValues, sort,partionBy等多种操作类型，Spark把这些操作称为Transformations。同时还提供Count,
 collect, reduce, lookup, save等多种actions操作。
- 这些多种多样的数据集操作类型，给给开发上层应用的用户提供了方便。各个处理节点之间的通信模型不再像Hadoop那样就是唯一的Data Shuffle一种模式。用户可以命名，物化，控制中间结果的存储、分区等。可以说编程模型比Hadoop更灵活。
- 不过由于RDD的特性，Spark不适用那种异步细粒度更新状态的应用，例如web服务的存储或者是增量的web爬虫和索引。就是对于那种增量修改的应用模型不适合。
- 容错性。
- 在分布式数据集计算时通过checkpoint来实现容错，而checkpoint有两种方式，一个是checkpoint data，一个是logging the updates。用户可以控制采用哪种方式来实现容错。
- 可用性。
- Spark通过提供丰富的Scala, Java，Python API及交互式Shell来提高可用性。
### 三.Spark与Hadoop的结合
- Spark可以直接对HDFS进行数据的读写，同样支持Spark on YARN。Spark可以与MapReduce运行于同集群中，共享存储资源与计算，数据仓库Shark实现上借用Hive，几乎与Hive完全兼容。
### 四.Spark的适用场景
- Spark是基于内存的迭代计算框架，适用于需要多次操作特定数据集的应用场合。需要反复操作的次数越多，所需读取的数据量越大，受益越大，数据量小但是计算密集度较大的场合，受益就相对较小
- 由于RDD的特性，Spark不适用那种异步细粒度更新状态的应用，例如web服务的存储或者是增量的web爬虫和索引。就是对于那种增量修改的应用模型不适合。
- 总的来说Spark的适用面比较广泛且比较通用。
### 五.运行模式
- 本地模式
- Standalone模式
- Mesoes模式
- yarn模式
### 六.Spark生态系统
- Shark ( Hive on Spark): Shark基本上就是在Spark的框架基础上提供和Hive一样的H iveQL命令接口，为了最大程度的保持和Hive的兼容性，Shark使用了Hive的API来实现query Parsing和 Logic Plan generation，最后的PhysicalPlan execution阶段用Spark代替Hadoop MapReduce。通过配置Shark参数，Shark可以自动在内存中缓存特定的RDD，实现数据重用，进而加快特定数据集的检索。同时，Shark通过UDF用户自定义函数实现特定的数据分析学习算法，使得SQL数据查询和运算分析能结合在一起，最大化RDD的重复使用。
- Spark streaming: 构建在Spark上处理Stream数据的框架，基本的原理是将Stream数据分成小的时间片断（几秒），以类似batch批量处理的方式来处理这小部分数据。Spark Streaming构建在Spark上，一方面是因为Spark的低延迟执行引擎（100ms+）可以用于实时计算，另一方面相比基于Record的其它处理框架（如Storm），RDD数据集更容易做高效的容错处理。此外小批量处理的方式使得它可以同时兼容批量和实时数据处理的逻辑和算法。方便了一些需要历史数据和实时数据联合分析的特定应用场合。
- Bagel: Pregel on Spark，可以用Spark进行图计算，这是个非常有用的小项目。Bagel自带了一个例子，实现了Google的PageRank算法。
### 七.在业界的使用
- Spark项目在2009年启动，2010年开源, 现在使用的有：Berkeley, Princeton, Klout, Foursquare, Conviva, Quantifind, Yahoo! Research & others, 淘宝等，豆瓣也在使用Spark的python克隆版Dpark。
## 八.Spark核心概念
### Resilient Distributed Dataset (RDD)弹性分布数据集
- RDD是Spark的最基本抽象,是对分布式内存的抽象使用，实现了以操作本地集合的方式来操作分布式数据集的抽象实现。RDD是Spark最核心的东西，它表示已被分区，不可变的并能够被并行操作的数据集合，不同的数据集格式对应不同的RDD实现。RDD必须是可序列化的。RDD可以cache到内存中，每次对RDD数据集的操作之后的结果，都可以存放到内存中，下一个操作可以直接从内存中输入，省去了MapReduce大量的磁盘IO操作。这对于迭代运算比较常见的机器学习算法,
 交互式数据挖掘来说，效率提升比较大。
- RDD的特点：
- 它是在集群节点上的不可变的、已分区的集合对象。
- 通过并行转换的方式来创建如（map, filter, join, etc）。
- 失败自动重建。
- 可以控制存储级别（内存、磁盘等）来进行重用。
- 必须是可序列化的。
- 是静态类型的。
- RDD的好处
- RDD只能从持久存储或通过Transformations操作产生，相比于分布式共享内存（DSM）可以更高效实现容错，对于丢失部分数据分区只需根据它的lineage就可重新计算出来，而不需要做特定的Checkpoint。
- RDD的不变性，可以实现类Hadoop MapReduce的推测式执行。
- RDD的数据分区特性，可以通过数据的本地性来提高性能，这与Hadoop MapReduce是一样的。
- RDD都是可序列化的，在内存不足时可自动降级为磁盘存储，把RDD存储于磁盘上，这时性能会有大的下降但不会差于现在的MapReduce。
- RDD的存储与分区
- 用户可以选择不同的存储级别存储RDD以便重用。
- 当前RDD默认是存储于内存，但当内存不足时，RDD会spill到disk。
- RDD在需要进行分区把数据分布于集群中时会根据每条记录Key进行分区（如Hash 分区），以此保证两个数据集在Join时能高效。
- RDD的内部表示
在RDD的内部实现中每个RDD都可以使用5个方面的特性来表示：
- 分区列表（数据块列表）
- 计算每个分片的函数（根据父RDD计算出此RDD）
- 对父RDD的依赖列表
- 对key-value RDD的Partitioner【可选】
- 每个数据分片的预定义地址列表(如HDFS上的数据块的地址)【可选】
- RDD的存储级别
RDD根据useDisk、useMemory、deserialized、replication四个参数的组合提供了11种存储级别：
```
```
val
 NONE = new StorageLevel(false, false, false)
```
`    `
```
val
 DISK_ONLY = new StorageLevel(true, false, false)
```
`    `
```
val
 DISK_ONLY_2 = new StorageLevel(true, false, false, 2)
```
`    `
```
val
 MEMORY_ONLY = new StorageLevel(false, true, true)
```
`    `
```
val
 MEMORY_ONLY_2 = new StorageLevel(false, true, true, 2)
```
`    `
```
val
 MEMORY_ONLY_SER = new StorageLevel(false, true, false)
```
`    `
```
val
 MEMORY_ONLY_SER_2 = new StorageLevel(false, true, false, 2)
```
`    `
```
val
 MEMORY_AND_DISK = new StorageLevel(true, true, true)
```
`    `
```
val
 MEMORY_AND_DISK_2 = new StorageLevel(true, true, true, 2)
```
`    `
```
val
 MEMORY_AND_DISK_SER = new StorageLevel(true, true, false)
```
`    `
```
val
 MEMORY_AND_DISK_SER_2 = new StorageLevel(true, true, false, 2)
```
```
- RDD定义了各种操作，不同类型的数据由不同的RDD类抽象表示，不同的操作也由RDD进行抽实现。
### 九.RDD的生成
- RDD有两种创建方式：
1、从Hadoop文件系统（或与Hadoop兼容的其它存储系统）输入（例如HDFS）创建。
2、从父RDD转换得到新RDD。
- 下面来看一从Hadoop文件系统生成RDD的方式，如：`val file = spark.textFile("hdfs://...")`，file变量就是RDD（实际是HadoopRDD实例），生成的它的核心代码如下：
```
```
//
 SparkContext根据文件/目录及可选的分片数创建RDD, 这里我们可以看到Spark与Hadoop MapReduce很像
```
`   `
```
//
 需要InputFormat, Key、Value的类型，其实Spark使用的Hadoop的InputFormat, Writable类型。
```
`   `
```
def
 textFile(path: String, minSplits: Int = defaultMinSplits): RDD[String] = {
```
`       `
```
hadoopFile(path,
 classOf[TextInputFormat], classOf[LongWritable],
```
`       `
```
classOf[Text],
 minSplits) .map(pair => pair._2.toString) }
```
`   `
```
//
 根据Hadoop配置，及InputFormat等创建HadoopRDD
```
`   `
```
new
 HadoopRDD(this, conf, inputFormatClass, keyClass, valueClass, minSplits)
```
```
- 对RDD进行计算时，RDD从HDFS读取数据时与Hadoop MapReduce几乎一样的：
```
```
//
 根据hadoop配置和分片从InputFormat中获取RecordReader进行数据的读取。
```
`    `
```
reader
 = fmt.getRecordReader(split.inputSplit.value, conf, Reporter.NULL)
```
`    `
```
val
 key: K = reader.createKey()
```
`    `
```
val
 value: V = reader.createValue()
```
`    `
```
//使用Hadoop
 MapReduce的RecordReader读取数据，每个Key、Value对以元组返回。
```
`    `
```
override
 def getNext() = {
```
`    `
```
try
 {
```
`      `
```
finished
 = !reader.next(key, value)
```
`    `
```
}
 catch {
```
`      `
```
case
 eof: EOFException =>
```
`        `
```
finished
 = true
```
`    ``}`
`      `
```
(key,
 value)
```
`    ``}`
```
### 十.RDD的转换与操作
- 对于RDD可以有两种计算方式：转换（返回值还是一个RDD）与操作（返回值不是一个RDD）。
- 转换(Transformations) (如：map, filter, groupBy, join等)，Transformations操作是Lazy的，也就是说从一个RDD转换生成另一个RDD的操作不是马上执行，Spark在遇到Transformations操作时只会记录需要这样的操作，并不会去执行，需要等到有Actions操作的时候才会真正启动计算过程进行计算。
- 操作(Actions) (如：count, collect, save等)，Actions操作会返回结果或把RDD数据写到存储系统中。Actions是触发Spark启动计算的动因。
- 下面使用一个例子来示例说明Transformations与Actions在Spark的使用。
```
```
val
 sc = new SparkContext(master, "Example", System.getenv("SPARK_HOME"),
```
`        ``Seq(System.getenv("SPARK_TEST_JAR")))`
`    `
```
val
 rdd_A = sc.textFile(hdfs://.....)
```
`    `
```
val
 rdd_B = rdd_A.flatMap((line => line.split("\\s+"))).map(word => (word, 1))
```
`    `
```
val
 rdd_C = sc.textFile(hdfs://.....)
```
`    `
```
val
 rdd_D = rdd_C.map(line => (line.substring(10), 1))
```
`    `
```
val
 rdd_E = rdd_D.reduceByKey((a, b) => a + b)
```
`    `
```
val
 rdd_F = rdd_B.jion(rdd_E)
```
`    ``rdd_F.saveAsSequenceFile(hdfs://....)`
```
![SparkTA11](http://jbcdn2.b0.upaiyun.com/2013/09/SparkTA11.jpg)
### 十一.Lineage（血统）
- 利用内存加快数据加载,在众多的其它的In-Memory类数据库或Cache类系统中也有实现，Spark的主要区别在于它处理分布式运算环境下的数据容错性（节点实效/数据丢失）问题时采用的方案。为了保证RDD中数据的鲁棒性，RDD数据集通过所谓的血统关系(Lineage)记住了它是如何从其它RDD中演变过来的。相比其它系统的细颗粒度的内存数据更新级别的备份或者LOG机制，RDD的Lineage记录的是粗颗粒度的特定数据转换（Transformation）操作（filter,
 map, join etc.)行为。当这个RDD的部分分区数据丢失时，它可以通过Lineage获取足够的信息来重新运算和恢复丢失的数据分区。这种粗颗粒的数据模型，限制了Spark的运用场合，但同时相比细颗粒度的数据模型，也带来了性能的提升。
- RDD在Lineage依赖方面分为两种Narrow Dependencies与Wide Dependencies用来解决数据容错的高效性。Narrow Dependencies是指父RDD的每一个分区最多被一个子RDD的分区所用，表现为一个父RDD的分区对应于一个子RDD的分区或多个父RDD的分区对应于一个子RDD的分区，也就是说一个父RDD的一个分区不可能对应一个子RDD的多个分区。Wide
 Dependencies是指子RDD的分区依赖于父RDD的多个分区或所有分区，也就是说存在一个父RDD的一个分区对应一个子RDD的多个分区。对与Wide Dependencies，这种计算的输入和输出在不同的节点上，lineage方法对与输入节点完好，而输出节点宕机时，通过重新计算，这种情况下，这种方法容错是有效的，否则无效，因为无法重试，需要向上其祖先追溯看是否可以重试（这就是lineage，血统的意思），Narrow Dependencies对于数据的重算开销要远小于Wide Dependencies的数据重算开销。
### 十二.容错
- 在RDD计算，通过checkpint进行容错，做checkpoint有两种方式，一个是checkpoint data，一个是logging the updates。用户可以控制采用哪种方式来实现容错，默认是logging the updates方式，通过记录跟踪所有生成RDD的转换（transformations）也就是记录每个RDD的lineage（血统）来重新计算生成丢失的分区数据。
### 十三.资源管理与作业调度
- Spark对于资源管理与作业调度可以使用Standalone(独立模式)，Apache Mesos及Hadoop YARN来实现。 Spark on Yarn在Spark0.6时引用，但真正可用是在现在的branch-0.8版本。Spark on Yarn遵循YARN的官方规范实现，得益于Spark天生支持多种Scheduler和Executor的良好设计，对YARN的支持也就非常容易，Spark
 on Yarn的大致框架图。 ![Spark架构图](http://jbcdn2.b0.upaiyun.com/2013/09/Spark%E6%9E%B6%E6%9E%84%E5%9B%BE.jpg)
- 让Spark运行于YARN上与Hadoop共用集群资源可以提高资源利用率。
## 十四.编程接口
- Spark通过与编程语言集成的方式暴露RDD的操作，类似于DryadLINQ和FlumeJava，每个数据集都表示为RDD对象，对数据集的操作就表示成对RDD对象的操作。Spark主要的编程语言是Scala，选择Scala是因为它的简洁性（Scala可以很方便在交互式下使用）和性能（JVM上的静态强类型语言）。
- Spark和Hadoop MapReduce类似，由Master(类似于MapReduce的Jobtracker)和Workers(Spark的Slave工作节点)组成。用户编写的Spark程序被称为Driver程序，Dirver程序会连接master并定义了对各RDD的转换与操作，而对RDD的转换与操作通过Scala闭包(字面量函数)来表示，Scala使用Java对象来表示闭包且都是可序列化的，以此把对RDD的闭包操作发送到各Workers节点。
 Workers存储着数据分块和享有集群内存，是运行在工作节点上的守护进程，当它收到对RDD的操作时，根据数据分片信息进行本地化数据操作，生成新的数据分片、返回结果或把RDD写入存储系统。
### ![runtime](http://jbcdn2.b0.upaiyun.com/2013/09/runtime.jpg)
### 十五.Scala
- Spark使用Scala开发，默认使用Scala作为编程语言。编写Spark程序比编写Hadoop MapReduce程序要简单的多，SparK提供了Spark-Shell，可以在Spark-Shell测试程序。写SparK程序的一般步骤就是创建或使用(SparkContext)实例，使用SparkContext创建RDD，然后就是对RDD进行操作。如：
```
`val`
```
sc
```
`=``new`
```
SparkContext(master, appName, [sparkHome], [jars])
```
`val`
```
textFile
```
`=``sc.textFile(``"hdfs://....."``)`
`textFile.map(....).filter(.....).....`
```
### 十六.Java
- Spark支持Java编程，但对于使用Java就没有了Spark-Shell这样方便的工具，其它与Scala编程是一样的，因为都是JVM上的语言，Scala与Java可以互操作，Java编程接口其实就是对Scala的封装。如：
```
```java
JavaSparkContext
 sc =
```
```java
new
```
```java
JavaSparkContext(...);
```
```java
JavaRDD
 lines = ctx.textFile(
```
```java
"hdfs://..."
```
```java
);
```
```java
JavaRDD
 words = lines.flatMap(
```
```java
```
```java
new
```
```java
FlatMapFunction<String,
 String>() {
```
```java
```
```java
public
```
```java
Iterable
 call(String s) {
```
```java
```
```java
return
```
```java
Arrays.asList(s.split(
```
```java
"
 "
```
```java
));
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
);
```
```
### 十七.Python
### 现在Spark也提供了Python编程接口，Spark使用py4j来实现python与java的互操作，从而实现使用python编写Spark程序。Spark也同样提供了pyspark，一个Spark的python shell，可以以交互式的方式使用Python编写Spark程序。 如：
- 
```
```python
from
```
```python
pyspark
```
```python
import
```
```python
SparkContext
```
```python
sc
```
```python
=
```
```python
SparkContext(
```
```python
"local"
```
```python
,
```
```python
"Job
 Name"
```
```python
,
 pyFiles
```
```python
=
```
```python
[
```
```python
'MyFile.py'
```
```python
,
```
```python
'lib.zip'
```
```python
,
```
```python
'app.egg'
```
```python
])
```
```python
words
```
```python
=
```
```python
sc.textFile(
```
```python
"/usr/share/dict/words"
```
```python
)
```
```python
words.
```
```python
filter
```
```python
(
```
```python
lambda
```
```python
w:
 w.startswith(
```
```python
"spar"
```
```python
)).take(
```
```python
5
```
```python
)
```
```
## 使用示例
### 十八.Standalone模式
- 为方便Spark的推广使用，Spark提供了Standalone模式，Spark一开始就设计运行于Apache Mesos资源管理框架上，这是非常好的设计，但是却带了部署测试的复杂性。为了让Spark能更方便的部署和尝试，Spark因此提供了Standalone运行模式，它由一个Spark Master和多个Spark worker组成，与Hadoop MapReduce1很相似，就连集群启动方式都几乎是一样。
- 以Standalone模式运行Spark集群
- 下载Scala2.9.3，并配置SCALA_HOME
- 下载Spark代码（可以使用源码编译也可以下载编译好的版本）这里下载 编译好的版本（`http://spark-project.org/download/spark-0.7.3-prebuilt-cdh4.tgz`）
- 解压spark-0.7.3-prebuilt-cdh4.tgz安装包
- 修改配置（conf/*） slaves: 配置工作节点的主机名 spark-env.sh：配置环境变量。
```
`SCALA_HOME=/home/spark/scala-2.9.3`
`JAVA_HOME=/home/spark/jdk1.6.0_45`
`SPARK_MASTER_IP=spark1            `
`SPARK_MASTER_PORT=30111`
`SPARK_MASTER_WEBUI_PORT=30118`
```
SPARK_WORKER_CORES=2
 SPARK_WORKER_MEMORY=4g
```
`SPARK_WORKER_PORT=30333`
`SPARK_WORKER_WEBUI_PORT=30119`
`SPARK_WORKER_INSTANCES=1`
```
- 把Hadoop配置copy到conf目录下
- 在master主机上对其它机器做ssh无密码登录
- 把配置好的Spark程序使用scp copy到其它机器
- 在master启动集群
```
`$SPARK_HOME/start-all.sh`
```
### 十九.yarn模式
- Spark-shell现在还不支持Yarn模式，使用Yarn模式运行，需要把Spark程序全部打包成一个jar包提交到Yarn上运行。目录只有branch-0.8版本才真正支持Yarn。
- 以Yarn模式运行Spark
- 下载Spark代码.
```
```
git
 clone git://github.com/mesos/spark
```
```
- 切换到branch-0.8
```
```
cd
 spark
```
```
git
 checkout -b yarn --track origin/yarn
```
```
- 使用sbt编译Spark并
```
`$SPARK_HOME/sbt/sbt`
```
>
 package
```
```
>
 assembly
```
```
- 把Hadoop yarn配置copy到conf目录下
- 运行测试
```
`  `
```
SPARK_JAR=./core/target/scala-2.9.3/spark-core-assembly-0.8.0-SNAPSHOT.jar
 \
```
```
./run
 spark.deploy.yarn.Client --jar examples/target/scala-2.9.3/ \
```
```
--class
 spark.examples.SparkPi --args yarn-standalone
```
```
### 二十.使用Spark-shell
- Spark-shell使用很简单，当Spark以Standalon模式运行后，使用`$SPARK_HOME/spark-shell`进入shell即可，在Spark-shell中SparkContext已经创建好了，实例名为sc可以直接使用，还有一个需要注意的是，在Standalone模式下，Spark默认使用的调度器的FIFO调度器而不是公平调度，而Spark-shell作为一个Spark程序一直运行在Spark上，其它的Spark程序就只能排队等待，也就是说同一时间只能有一个Spark-shell在运行。
- 在Spark-shell上写程序非常简单，就像在Scala Shell上写程序一样。
```
```
scala>
 val textFile = sc.textFile("hdfs://hadoop1:2323/user/data")
```
```
textFile:
 spark.RDD[String] = spark.MappedRDD@2ee9b6e3
```
```
scala>
 textFile.count() // Number of items in this RDD
```
```
res0:
 Long = 21374
```
```
scala>
 textFile.first() // First item in this RDD
```
```
res1:
 String = # Spark
```
```
### 二十一.编写Driver程序
- 在Spark中Spark程序称为Driver程序，编写Driver程序很简单几乎与在Spark-shell上写程序是一样的，不同的地方就是SparkContext需要自己创建。如WorkCount程序如下：
```
```
import
 spark.SparkContext
```
```
import
 SparkContext._
```
```
object
 WordCount {
```
`  `
```
def
 main(args: Array[String]) {
```
`    `
```
if
 (args.length ==0 ){
```
`      `
```
println("usage
 is org.test.WordCount ")
```
`    ``}`
`    `
```
println("the
 args: ")
```
`    ``args.foreach(println)`
`    `
```
val
 hdfsPath = "hdfs://hadoop1:8020"
```
`    `
```
//
 create the SparkContext， args(0)由yarn传入appMaster地址
```
`    `
```
val
 sc = new SparkContext(args(0), "WrodCount",
```
`    `
```
System.getenv("SPARK_HOME"),
 Seq(System.getenv("SPARK_TEST_JAR")))
```
`    `
```
val
 textFile = sc.textFile(hdfsPath + args(1))
```
`    `
```
val
 result = textFile.flatMap(line => line.split("\\s+"))
```
`        `
```
.map(word
 => (word, 1)).reduceByKey(_ + _)
```
`    `
```
result.saveAsTextFile(hdfsPath
 + args(2))
```
`  ``}`
`}`
```
