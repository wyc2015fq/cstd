# 结合源码彻底讲解Aggregate vs treeAggregate - Spark高级玩法 - CSDN博客
2018年01月14日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：405
**Aggregate**
本文主要是讲解两个常见的聚合操作：aggregate vs treeAggregate
首先讲解aggregate，该函数的方法具体名称如下：
def aggregate[U: ClassTag](zeroValue: U)(seqOp: (U, T) => U, combOp: (U, U) => U): U = withScope {
// Clone the zero value since we will also be serializing it as part of tasks
var jobResult = Utils.clone(zeroValue, sc.env.serializer.newInstance())
val cleanSeqOp = sc.clean(seqOp)
val cleanCombOp = sc.clean(combOp)
val aggregatePartition = (it: Iterator[T]) => it.aggregate(zeroValue)(cleanSeqOp, cleanCombOp)
val mergeResult = (index: Int, taskResult: U) => jobResult = combOp(jobResult, taskResult)
  sc.runJob(this, aggregatePartition, mergeResult)
  jobResult
}
**参数定义：**
首先可以看到，有个U类型的参数叫做zeroValue，然后有两个方法参数，第一个是seqOp: (U, T) => U将U和T类型的数据转化为T类型的数据，第二个函数combOp: (U, U) => U将两个U类型的数据转化为U类型，返回的是一个U类型的数据。
**参数作用：**
zeroValue是给定的初始值，该值将会在seqOp和combOp两个函数中都使用。
seqOp在Executor端对每个分区进行操作，会用到初始值zeroValue。
combOp在driver端执行，也会用到初始值。
**源码简介：**
**片段一：**
val aggregatePartition = (it: Iterator[T]) => it.aggregate(zeroValue)(cleanSeqOp, cleanCombOp)
这个源码就是针对每个RDD分区，进行执行的时候的函数，因为实际上每个分区最终都是一个迭代器，然后执行迭代器的aggregate，参数也是我们给定的参数。Iterator 的aggregate方法实际上三个参数是没有用到的，也即CombOp没有用到。
**片段二：**
val mergeResult = (index: Int, taskResult: U) => jobResult = combOp(jobResult, taskResult)
该段代码是在Driver端执行combOp操作。
具体的执行逻辑不是本文要讲解的主要内容，后面有机会浪尖会逐步给大家分析。
由上面我们可以总结，aggregate执行结构图，如下：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWx2o1f2jiawVeYcicOJMUSibiaelNkxD2sJh6QeefBhVA4SJ1GKKQ2vVSbfy4aBxBkjSBStxv8VyZy9Q/?wx_fmt=png)
**这种聚合操作是有缺陷的，就是所有SeqOp操作对分区的执行结果都只能全部返回给Driver端，然后在对返回的结果和初始值执行CombOp操作，这样数据量大的时候很容易导致Driver端内存溢出，所以，就出现了优化函数treeAggregate。**
**treeAggregate**
**treeAggregate函数**的具体内容如下：
def treeAggregate[U: ClassTag](zeroValue: U)(
    seqOp: (U, T) => U,
combOp: (U, U) => U,
depth: Int = 2): U = withScope {
require(depth >= 1, s"Depth must be greater than or equal to 1 but got $depth.")
if (partitions.length == 0) {
    Utils.clone(zeroValue, context.env.closureSerializer.newInstance())
  } else {
val cleanSeqOp = context.clean(seqOp)
val cleanCombOp = context.clean(combOp)
val aggregatePartition =
      (it: Iterator[T]) => it.aggregate(zeroValue)(cleanSeqOp, cleanCombOp)
var partiallyAggregated = mapPartitions(it => Iterator(aggregatePartition(it)))
var numPartitions = partiallyAggregated.partitions.length
val scale = math.max(math.ceil(math.pow(numPartitions, 1.0 / depth)).toInt, 2)
// If creating an extra level doesn't help reduce
    // the wall-clock time, we stop tree aggregation.
    // Don't trigger TreeAggregation when it doesn't save wall-clock time
while (numPartitions > scale + math.ceil(numPartitions.toDouble / scale)) {
      numPartitions /= scale
val curNumPartitions = numPartitions
      partiallyAggregated = partiallyAggregated.mapPartitionsWithIndex {
        (i, iter) => iter.map((i % curNumPartitions, _))
      }.reduceByKey(new HashPartitioner(curNumPartitions), cleanCombOp).values
    }
    partiallyAggregated.reduce(cleanCombOp)
  }
}
**参数定义：**
首先可以看到，有个U类型的参数叫做zeroValue，然后有两个方法参数，第一个是seqOp: (U, T) => U将U和T类型的数据转化为T类型的数据，第二个函数combOp: (U, U) => U将两个U类型的数据转化为U类型，返回的是一个U类型的数据。
**参数作用：**
zeroValue是给定的初始值，该值将会在seqOp和combOp两个函数中都使用。
seqOp在Executor端对每个分区进行操作，会用到初始值zeroValue。
combOp在Executor端和driver端都会执行，不会用到初始值。
**源码简介：**
**片段一：**
val aggregatePartition =
  (it: Iterator[T]) => it.aggregate(zeroValue)(cleanSeqOp, cleanCombOp)
var partiallyAggregated = mapPartitions(it => Iterator(aggregatePartition(it)))
在Executor端执行的第一层任务，主要操作是对源数据和初始值zeroValue执行seqOp操作。
**片段二：**
var numPartitions = partiallyAggregated.partitions.length
val scale = math.max(math.ceil(math.pow(numPartitions, 1.0 / depth)).toInt, 2)
while (numPartitions > scale + math.ceil(numPartitions.toDouble / scale)) {
  numPartitions /= scale
val curNumPartitions = numPartitions
  partiallyAggregated = partiallyAggregated.mapPartitionsWithIndex {
    (i, iter) => iter.map((i % curNumPartitions, _))
  }.reduceByKey(new HashPartitioner(curNumPartitions), cleanCombOp).values
}
在执行完成第一层任务之后，执行combOp操作，主要是逐渐降低分区数，来逐层进行combOp操作，该操作是在Executor端执行，并且该操作并未用到初始值。
**片段三：**
partiallyAggregated.reduce(cleanCombOp)
在Executor端初步聚合后，对结果数据使用combOp操作再次执行reduce操作。
由上面我们可以总结，aggregate执行结构图，如下：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWx2o1f2jiawVeYcicOJMUSibia8KRQDOiaQsRvUlwYnw44rzCsqjZnHQSogqTAn339UFoytVzJSIicAyIA/?wx_fmt=png)
**aggregate VS treeAggregate**
1, aggregate和treeAggregate的作用一样，最终的结果区别是treeAggregate执行combOp并没有用到初始值zeroValue。
2，treeAggregate比aggregate多执行了n次任务，n可计算。
3，treeAggregate降低了aggregate在driver端内存溢出的风险。
可以举个例子：
def seq(a:Int,b:Int):Int={
println("seq:"+a+":"+b)
  a+b
}
def comb(a:Int,b:Int):Int={
println("comb:"+a+":"+b)
  a+b
}
val res = sc.parallelize(List(1,2,4,5,8,9,7,2),3)
res.aggregate(1)(seq,comb)
res.treeAggregate(1)(seq,comb)
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWx2o1f2jiawVeYcicOJMUSibia7DSdmUMYYzuicCdgZFf3aHYXgyKPgEfue6ngGjNvkyXFYbtgics0gq9Q/?wx_fmt=png)
aggregate结果应该是：1+2+4+5+8+9+7+2+3*1 +1=42
treeAggregate结果应该是：1+2+4+5+8+9+7+2+3*1=41
**推荐阅读：**
1，[用java提交一个Spark应用程序](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484324&idx=1&sn=9d68b15a297c965c24ccece28fa6cc00&chksm=9f38e08ca84f699ab7f50d02bdfecc8da9c697c672e5764dd1333ca0cf028712b5d556006435&scene=21#wechat_redirect)
2，[请别再问我Spark的MLlib和ML库的区别](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484273&idx=1&sn=830eee29f7078dd85d6b3156a2880acd&chksm=9f38e059a84f694f99480c6e831d788187d1943704ca69f881721bf40dbf9745c8701902c4db&scene=21#wechat_redirect)
3，[hive的分区和分桶](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484404&idx=1&sn=88c0418a866572c1dd85ced93d1168f0&chksm=9f38e0dca84f69ca45cd77722839004db02019297c8608835114e382d1b85c6bb6c1fe920934&scene=21#wechat_redirect)
4，[Hbase源码系列之scan源码解析及调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483739&idx=1&sn=22647805307d21d6f893bea04c78153c&chksm=9f38e273a84f6b65be8cac2cc619a65e80f31235b43779b056db0b220b91559cf4a477dbb8bb&scene=21#wechat_redirect)
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
