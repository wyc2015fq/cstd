
# spark的coalesce和repartition算子管理分区 - leofionn的博客 - CSDN博客


2019年02月20日 21:13:21[leofionn](https://me.csdn.net/qq_36142114)阅读数：69


`源码地址https://github.com/apache/spark/blob/v2.4.0/core/src/main/scala/org/apache/spark/rdd/RDD.scala`repartition：
`/ **
   *返回一个具有正确numPartitions分区的新RDD。
   *
   *可以增加或减少此RDD中的并行度。在内部，这使用
   *重新分配数据的随机播放。
   *
   *如果要减少此RDD中的分区数，请考虑使用`coalesce`，
   *可以避免执行随机播放。
   *
   * /
 def repartition(numPartitions: Int)(implicit ord: Ordering[T] = null): RDD[T] = withScope {
    coalesce(numPartitions, shuffle = true)
  }`coalesce
`/ **
   *返回一个新的RDD，它被缩减为`numPartitions`分区。
   *
   *这导致了一个狭窄的依赖，例如，如果你从1000分区
   *到100个分区，不会有一个shuffle，而是每个100个
   *新分区将声明10个当前分区。如果较大的数量请求分区，它将保持当前分区数。
   *
   *但是，如果你正在进行激烈的合并，例如对numPartitions = 1，
   *这可能导致您的计算发生在比节点更少的节点上
   *你喜欢（例如，numPartitions = 1时的一个节点）。要避免这种情况，
   *你可以传递shuffle = true。这将添加一个洗牌步骤，但意味着
   *当前的上游分区将并行执行（无论如何当前分区是什么）。
   *
   * 使用shuffle = true，您实际上可以合并为更大的数字
   *分区。如果您有少量分区，这很有用
   *说100，可能有一些分区异常大。调用
   * coalesce（1000，shuffle = true）将导致1000个分区
   *使用散列分区器分发数据。可选的分区聚结器
   *传入必须是可序列化的。
   * /
 def coalesce(numPartitions: Int, shuffle: Boolean = false,
               partitionCoalescer: Option[PartitionCoalescer] = Option.empty)
              (implicit ord: Ordering[T] = null)
      : RDD[T] = withScope {
    require(numPartitions > 0, s"Number of partitions ($numPartitions) must be positive.")
    if (shuffle) {
      /** Distributes elements evenly across output partitions, starting from a random partition. */
      val distributePartition = (index: Int, items: Iterator[T]) => {
        var position = new Random(hashing.byteswap32(index)).nextInt(numPartitions)
        items.map { t =>
          // Note that the hash code of the key will just be the key itself. The HashPartitioner
          // will mod it with the number of total partitions.
          position = position + 1
          (position, t)
        }
      } : Iterator[(Int, T)]
      // include a shuffle step so that our upstream tasks are still distributed
      new CoalescedRDD(
        new ShuffledRDD[Int, T, T](
          mapPartitionsWithIndexInternal(distributePartition, isOrderSensitive = true),
          new HashPartitioner(numPartitions)),
        numPartitions,
        partitionCoalescer).values
    } else {
      new CoalescedRDD(this, numPartitions, partitionCoalescer)
    }
  }`repartition方法是调用了coalesce方法,shuffle为true的情况
所以使用coalesce默认情况下是不产生shuffle的。
## 分区介绍
当配置文件spark-default.conf中没有配置时，则按照如下规则取值：
1、本地模式（不会启动executor，由SparkSubmit进程生成指定数量的线程数来并发）：
`spark-shell       spark.default.parallelism = 1
spark-shell --master local[N] spark.default.parallelism = N （使用N个核）
spark-shell --master local       spark.default.parallelism = 1`2、伪集群模式（x为本机上启动的executor数，y为每个executor使用的core数，z为每个 executor使用的内存）
`spark-shell --master local-cluster[x,y,z] spark.default.parallelism = x * y`3、其他模式（这里主要指yarn模式，当然standalone也是如此）
`Others: total number of cores on all executor nodes or 2, whichever is larger
spark.default.parallelism =  max（所有executor使用的core总数， 2）`假设使用 spark-shell --master local[3] 启动：
`val x = (1 to 1000).toList
val test_partitionDf = x.toDF(“test_partition”)``scala> test_partitionDf.rdd.partitions.size 
res0: Int = 3`coalesce方法减少了DataFrame中的分区数量。以下是如何合并两个分区中的数据：
`val test_partitionDf2 = test_partitionDf.coalesce(1)`我们可以验证coalesce是否只创建了一个只有一个分区的新DataFrame：
`scala> test_partitionDf2.rdd.partitions.size
res1: Int = 1`使用coalesce来增加分区，但是并不生效：
`val test_partitionDf3 = test_partitionDf.coalesce(4)``scala> test_partitionDf3.rdd.partitions.size
res2: Int = 3`而repartition方法可用于增加或减少DataFrame中的分区数。
增加分区或减少分区
`scala> val test_partition_repartitionDf = test_partitionDf.repartition(6)
scala> test_partition_repartitionDf.rdd.partitions.size
res3: Int = 6
scala> val test_partition_repartitionDf = test_partitionDf.repartition(1)
scala>test_partition_repartitionDf.rdd.partitions.size
res4: Int = 1`重新分区方法可以完全重排数据，因此可以增加或减少分区数。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190220210643373.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0,size_16,color_FFFFFF,t_70)
## coalesce和repartition之间的区别
repartition对数据进行完全重排，并创建相同大小的数据分区。coalesce结合现有分区以避免完全洗牌。
##### 按列repartition
`val color = List((1001,"blue"),(102,"red"),(1555,"blue"),(9,"red"),(1,"blue"))
val colorDf = color.toDF("sum","color")
val test_colorDf = colorDf.repartition($"color")
test_colorDf.rdd.partitions.size
res6: Int = 200`![在这里插入图片描述](https://img-blog.csdnimg.cn/20190220210802628.jpg)
按列分区时，Spark默认会创建至少200个分区。查看分区数据，只有两个分区有数据，且同一个分区中的数据的color字段是一致的。colorDf包含每种color的不同分区，并针对color提取进行了优化。按列分区类似于索引关系数据库中的列。
[
](https://img-blog.csdnimg.cn/20190220210802628.jpg)
## 考虑分区
[
](https://img-blog.csdnimg.cn/20190220210802628.jpg)1）N<M。一般情况下N个分区有数据分布不均匀的状况，利用HashPartitioner函数将数据重新分区为M个，这时需要将shuffle设置为true。
[
](https://img-blog.csdnimg.cn/20190220210802628.jpg)2）如果N>M并且N和M相差不多，(假如N是1000，M是100)那么就可以将N个分区中的若干个分区合并成一个新的分区，最终合并为M个分区，这时可以将shuff设置为false，在shuffl为false的情况下，如果M>N时，coalesce为无效的，不进行shuffle过程，父RDD和子RDD之间是窄依赖关系。
[
](https://img-blog.csdnimg.cn/20190220210802628.jpg)3）如果N>M并且两者相差悬殊，这时如果将shuffle设置为false，父子RDD是窄依赖关系，他们同处在一个Stage中，就可能造成spark程序的并行度不够，从而影响性能，如果在M为1的时候，为了使coalesce之前的操作有更好的并行度，可以将shuffle设置为true。
[
](https://img-blog.csdnimg.cn/20190220210802628.jpg)总之：如果shuff为false时，如果传入的参数大于现有的分区数目，RDD的分区数不变，也就是说不经过shuffle，是无法将RDDde分区数变多的。
[
](https://img-blog.csdnimg.cn/20190220210802628.jpg)参考：[https://blog.csdn.net/u011981433/article/details/50035851](https://blog.csdn.net/u011981433/article/details/50035851)
[https://blog.csdn.net/jiangsanfeng1111/article/details/78191891](https://blog.csdn.net/jiangsanfeng1111/article/details/78191891)
[
						](https://img-blog.csdnimg.cn/20190220210802628.jpg)
[
	](https://img-blog.csdnimg.cn/20190220210802628.jpg)
