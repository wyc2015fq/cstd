# 扩展RDD API三部曲第一部回顾基础 - Spark高级玩法 - CSDN博客
2018年12月11日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：131
扩展RDD API三部曲，主要是帮助大家掌握如下三个内容：
1). 回顾一下RDD的基础
2). 扩展Action
3). 扩展 transform及自定义RDD
**1. RDD的五大特性**
*1  - A list of partitions2  - A function for computing each split3  - A list of dependencies on other RDDs4  - Optionally, a Partitioner for key-value RDDs (e.g. to say that the RDD is hash-partitioned)5  - Optionally, a list of preferred locations to compute each split on (e.g. block locations for   an HDFS file)*
翻译成中文:
1,一个分片列表。也即RDD可以进行分片。
2,每个分片都有计算函数。分片是计算的最小单位
3,一个RDD会依赖于一系列的RDD。并不是所有的RDD都是有依赖。
所有的转换操作都会生成新的RDD，所以就形成了RDD的血缘关系，一个RDD计算失败可以利用其血缘关系进行恢复。
4,可选项。针对 key-values类型的RDD才有的分区器。
5,最佳运行位置 或者 叫偏向运行位置 或者 叫数据的本地性。
**自定义RDD的话，必须是要实现三种必须的特性。**
**2. RDD算子**
**1).Transformation(变换)**
Transformation操作会由一个RDD生成一个新的 RDD。Transformation操作是延迟计算的，也就是说从一个RDD转换生成另一个RDD的转换操作不是马上执行，需要等到Actions操作时，才真正开始运算。
例如，根据谓词匹配情况筛选数据就是一个常见的转化操作。
testlines = lines.filter(line=>line.contains("spark"))
在transformations算子中再将数据类型细分为value数据类型和key-value对数据类型的transformations算子。
1）Value型数据的算子封装在RDD类中可以直接使用。
2）Key-value对数据类型的算子封装于PairRDDFunctions类中，用户需要引入import org.apache.spark.SparkContext._才能够使用。
**2).Action(行动)**
Action操作会对 RDD 计算出一个结果，并把结果返回到驱动器程序中，或把结果存储到外部存储系统（如 HDFS）中。
例如，first() 就是我们之前调用的一个行动操作，它会返回 RDD 的第一个元素。
result = testlines.first()
transformations操作和Action操作的区别在于Spark计算RDD 的方式不同。对于在任何时候得到的新的RDD，Spark只会惰性计算。只有在一个行动操作中用到时，才会真正计算。这种策略也是spark性能高的部分原因。
比如，我们读取一个文本文件创建一个RDD，然后把其中包含spark的行筛选出来。如果Spark在我们运行lines = sc.textFile(test.txt) 时就把文件中所有的行都读取到内存中并存储起来，内存开销会很大，而我们接下来的操作会筛选掉其中的很多数据。相反， 如果Spark 在知道了完整的转化操作链之后，它就可以只计算求结果时真正需要的数据。
事实上，在执行行动操作 first()时，Spark也只是扫描文件直到找到第一个匹配的行为止，而不是读取整个文件。
**推荐阅读：**
[spark streaming性能分析经验](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486052&idx=1&sn=7e1537fcbc93f54dc0b4dc9510f35695&chksm=9f38e94ca84f605a16be7582a6a9870a4e771162cd2478a5f1145441dadebeb53bd1bdae43cf&scene=21#wechat_redirect)
[Spark SQL如何实现mysql的union操作](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486038&idx=1&sn=09fd8886fcc456139d3265e7c2baa69a&chksm=9f38e97ea84f606802417d69ddbc645e0993337083c168b2b9c2bb7b6a1c85ad0121950a1947&scene=21#wechat_redirect)
[Spark SQL用UDF实现按列特征重分区](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485842&idx=1&sn=cf7331c3a082a141366e5771640c5e52&chksm=9f38eabaa84f63acec983e9e15691b01239349f568bd8cb9725fbc42a01aa721e97145f8622c&scene=21#wechat_redirect)
[Kafka源码系列之分组消费的再平衡策略](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483700&idx=1&sn=711baf7347f468af4b5b0b9ed05caab0&chksm=9f38e21ca84f6b0ac6f2f693530994b8dbb651ffa74ae759337fbd9c079122a03981fa993926&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640)
