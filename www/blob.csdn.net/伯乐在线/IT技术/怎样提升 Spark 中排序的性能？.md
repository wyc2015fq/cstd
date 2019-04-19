# 怎样提升 Spark 中排序的性能？ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [drowzju](http://www.jobbole.com/members/drowzju) 翻译，[sunshinebuel](http://www.jobbole.com/members/sunshinebuel) 校稿。未经许可，禁止转载！
英文出处：[Cloudera](http://blog.cloudera.com/blog/2015/01/improving-sort-performance-in-apache-spark-its-a-double/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
**Cloudera和Intel的工程师们正在就提升Spark shuffle过程的可扩展性和可靠性方面展开合作。下面是该方案设计上的细节：**
像MapReduce和[Apache Spark](http://spark.apache.org)(Apache Hadoop的下一代数据处理引擎)这样的计算引擎，和高度并行系统之间的主要区别就是对”多对多”操作的支持。作为分布式引擎，MapReduce和Spark要对分区会跨越集群的数据集合的子分片进行操作。很多操作一次只处理集中的数据，可以完全在分区内部完成。多对多的操作必须把整个数据集视作整体；每个输出记录的内容可能依赖于来自于不同分区的记录。对Spark而言，groupByKey, sorbByKey, 以及 reduceByKey  就是常见的这类操作。
对分布式计算引擎而言，”shuffle”指的是多对多操作中数据的重新分区和汇聚。很容易理解，我们在实际生产环境的Spark部署中观察到的大多的性能、可伸缩性和可靠性问题，都出现在shuffle过程之中。
Cloudera和Intel的工程师们正在就提升Spark的shuffle过程展开合作，以保证可以更快、更可靠地处理大量数据集。尽管Spark在许多方面都拥有对MapReduce的优势，但是在可伸缩性和可靠性方面它还是追赶者。我们从经过实战检验的MapReduce shuffle的实现借用了一些概念，用于改进输出排序数据的shuffle操作。
本文中，我们将纵览当前Spark shuffle实现的工作机制，和我们所推荐的改变，以及这些改变是如何提升了性能。此工作目前还在进行之中:[SPARK-2926](https://issues.apache.org/jira/browse/SPARK-2926).
## 当前事务的状态
一个shuffle过程涉及两个任务集合：产生shuffle数据的阶段所包含的任务，以及消费这些数据的阶段所包含的任务。出于历史原因，产生shuffle数据的任务被称作”map任务”， 而读取shuffle数据的任务被称为”reduce任务”。这两种角色是相对任务中的具体shuffle过程而言的。一个任务可能在一次shuffle中是在读数据，是reduce任务；而它在下一次shuffle中又产生了被下一阶段所消费的数据，它又是map任务。
MapReduce和Spark的shuffle过程采用了”拉”的模型。每个map任务在本地磁盘写入数据，然后reduce任务发起远程请求获取数据。因为shuffle是多对多操作的结果，任一map任务都可能有一组会被任意reduce任务使用的记录。shuffle在map端的工作就是要以这样的方式写入记录：把所有的需要被同一reduce任务访问的记录排列在一起，以保证它们能被更容易地获取。
Spark的原生shuffle(基于HASH的shuffle)通过在map任务中为每一个reduce任务打开一个文件来实现这个目标。这个方案有好处就是简单，不过带来了很多问题。比如，Spark 必须要么使用大量内存缓存每个文件，要么带来大量的随机磁盘I/O。另一个问题是，如果M和R是shuffle过程中map和reduce任务的数目，基于HASH的shuffle 需要总共M*R个中间文件。[Shuffle consolidation work](https://issues.apache.org/jira/browse/SPARK-751)把中间文件的数目减少到了 C * R个，C是map任务同时可以投入运行的数目。不过即使如此，用户在运行拥有相当数目的reducer 的作业时 ，还是会经常遇到”Too many open files”的ulimit限制。
![](http://blog.cloudera.com/wp-content/uploads/2015/01/shuffle-f1.png)
**Single map task in hash-based shuffle**
![](http://blog.cloudera.com/wp-content/uploads/2015/01/shuffle-f2.png)
**Single map task in sort-based shuffle**
为了进一步改进shuffle的可伸缩性和性能，从1.1版本开始，Spark引入了一种”基于排序的shuffle”实现，和MapReduce的map端实现很相似。在这种实现中， 每个map任务的输出记录会保存在内存中直到不能再填入。此时记录会被按照它们所要被投递的reduce任务进行排序并溢写到一个单独的文件。若此过程在一个任务中发生多次，所有溢写的文件在后续会被合并。
在reduce端，一组线程负责获取远端的map输出块。每当有块到达，记录信息会被反序列化，并被传入相应的多对多操作结果的数据结构中。对`groupByKey`, `reduceByKey`, 和`aggregateByKey`这样的汇聚操作，记录会被传入`一个ExternalAppendOnlyMap，它本质上是个HASH 映射，超过内存用量时也会溢写到磁盘。对类似于``sortByKey这样的排序操作，记录会被传入``ExternalSorter，它会对数据排序，很可能会溢写到磁盘，并返回一个针对数据做有序迭代的迭代器。`
## 全排序shuffle
上述方案有两个缺陷：
- 每个Reduce任务同时在内存中持有太多反序列化的记录。大量Java对象给JVM的垃圾回收带来压力，可能导致业务变慢或中止。比序列化的版本多占用了内存，还意味着Spark得更早更频繁地进行溢写，带来更多的磁盘I/O。此外，很难对反序列化对象的内存占用情况做出百分百正确的判定，所以保持越多的对象，就带来越多的内存溢出错误。
- 当需要在分区进行排序操作时，我们面对的是对同样的数据排序两次：第一次是mapper中进行分区时，第二次是reducer中进行按key操作。
我们的修改在map端的分区过程中对记录按key排序过了，所以在reduce端，我们只需要把从各个map 任务过来的排序之后的块合并。我们把块在内存中以序列化的格式保存，在合并时一次反序列化一个记录。因此任何时刻反序列化记录的最大数目都是我们要合并到一起的块的数目。
**![](http://blog.cloudera.com/wp-content/uploads/2015/01/shuffle-f3.png)Single map task in full sort-based shuffle**
单独一个reduce任务就可以从上千个map任务接收块。为了让这个多路合并的过程更高效，特别是在内存不足以放入数据的情况下，我们引入了*tiered merger*。当我们需要合并大量磁盘上的块，tiered merer引导合并块的子集，以最小化磁盘寻址。tiered merger 对`ExternalAppendOnlyMap和``ExternalSorter`的merge步骤也适用，不过我们还没有修改这两处以利用tiered mereger。
## 高性能合并
对每个任务，都有一组线程用于并行获取shuffle的数据。针对每个任务分配一个48M大小的内存池用来做获取数据的着陆点。
我们引入了`SortShuffleReader`，它负责操作块数据，并向用户代码提供一个基于[Key, Value]对的迭代器。
Spark维护了一个主shuffle内存区，所有任务共享，默认大小是整个executor堆的20%。当有数据块到来，`SortShuffleReader`尝试从此主内存区为数据块获取内存。序列化的块往内存中填充，直到尝试申请内存失败。此时我们需要溢写数据到磁盘以释放空间。`SortShuffleReader`合并所有(确切说不算所有；有时候最好只溢写一部分)内存中的块到一个单一的有序的磁盘文件。当磁盘上数据块开始堆积，一个后台线程会进行监控，并在有必要时把它们合并为大的有序磁盘数据块。”最终合并”把最终磁盘数据块的集合和所有还在内存中的块合并到一起，向用户代码返回一个迭代器。
我们如何判断何时需要进行中间的磁盘到磁盘的合并？`spark.shuffle.maxMergeFactor(默认100)`属性控制着单次合并中磁盘数据块的最大数目。当磁盘数据块的数目超过了这个限制，后台线程会运行一次合并过程以降低此数目(不过不是立刻；代码有更多细节)。要决定合并多少块，此线程会首先最小化它进行的合并次数，然后，在此数目内，尝试尽可能少地合并块。因此，如果`spark.shuffle.maxMergeFactor`是100，而磁盘数据块是110，线程只会把11个块合并到一起，即最终磁盘块的数目是100。合并多点块会需要额外的合并过程，合并多的块会导致不必要的磁盘I/O。
![](http://blog.cloudera.com/wp-content/uploads/2015/01/shuffle-f41.png)
**maxMergeWidth=4**时的Tiered merge。每个矩形都是一个磁盘文件。三个文件被合并成一个文件，最终4个文件被合并为一个迭代器，供下次操作使用。
## 和sortByKey的性能对比
我们使用[SparkPerf](https://github.com/databricks/spark-perf)的 sort-by-key 工作负载进行测试，以评估我们所做修改的性能影响。我们选择了两种不同大小的数据集用于比较在内存足以或不足以保存所有shuffle时，我们所做修改的性能收益。
Spark的sortBykey转换导致两种作业和三个阶段。
- 取样阶段：对数据取样，创建范围分区函数，它会产生均匀的分区；
- “Map”阶段： 把数据写入目的shuffle桶，供reduce阶段使用；
- “Reduce”阶段: 获得相关的shuffle输出，并针对数据集的特定分区进行合并/排序
检查程序在一个6节点集群上实施。每个executor有24个核，以及36GB的内存。大的数据集有200亿条记录，经压缩后在HDFS上占用了409.8GB的空间。晓得数据集有20亿条记录，压缩后在HDFS上占用了15.9GB的空间。每条记录是两个10字符的key-value对。每个用例中，排序操作都有超过1000个分区。每个阶段和整个作业的运行时间的图表展示如下：
![](http://blog.cloudera.com/wp-content/uploads/2015/01/shuffle-f5.png)
大的数据集(数值低较好)
**![](http://blog.cloudera.com/wp-content/uploads/2015/01/shuffle-f6.png)**小的数据集(数值低较好)
取样阶段是一样的，因为不涉及shuffle过程。map阶段，因为我们的改进现在会在每个分区内对数据按key排序，所以阶段运行时间有增长(大的数据集增长了37%，小的数据集增长了27%)。尽管如此，reduce阶段还是弥补了map超出的时间且有盈余，因为reduce现在只需要把排序之后的数据进行合并。对大小两个数据集，reduce阶段的持续时间都减少了超过66%，为大的数据集总共带来了超过27%的提升，为小的数据集带来了17%的整体提升。
## 下一步？
SPARK-2926 是对Spark的shuffle机制的一些改进计划之一。要特别说明一下，有很多方式可以让shuffle更好的管理内存:
- [SPARK-4550](https://issues.apache.org/jira/browse/SPARK-4550) 跟踪：把内存缓存的map输出数据作为原始字节保存，而不是Java对象。这样做可以让map的输出数据占据更少内存空间，也就减少了溢写。同时原始字节的比较速度更快。
- [SPARK-4452](https://issues.apache.org/jira/browse/SPARK-4452) 跟踪：更小心地为不同的shuffle数据结构分配内存，还有更快地返还不再需要的内存。
- [SPARK-3461](https://issues.apache.org/jira/browse/SPARK-3461) 跟踪：对传入的groupBy或join的数据按照特定key去处理值，而不是一次性把所有内容加载进内存。
