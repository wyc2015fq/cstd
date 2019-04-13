
# UpdateStateByKey操作 - leofionn的博客 - CSDN博客


2019年03月17日 11:02:52[leofionn](https://me.csdn.net/qq_36142114)阅读数：115


官网原话：
`updateStateByKey`操作允许您在使用新信息不断更新时保持任意状态。要使用它，您必须执行两个步骤。
定义状态 - 状态可以是任意数据类型。
定义状态更新功能 - 使用函数指定如何使用先前状态和输入流中的新值更新状态。
在每个批处理中，Spark都会对所有现有key应用状态更新功能，无论它们是否在批处理中都有新数据。如果更新函数返回，`None`则将删除key-valu。
`/**
   * Return a new "state" DStream where the state for each key is updated by applying
   * the given function on the previous state of the key and the new values of each key.
   * Hash partitioning is used to generate the RDDs with Spark's default number of partitions.
   * @param updateFunc State update function. If `this` function returns None, then
   *                   corresponding state key-value pair will be eliminated.
   * @tparam S State type
   */
  def updateStateByKey[S: ClassTag](
      updateFunc: (Seq[V], Option[S]) => Option[S]
    ): DStream[(K, S)] = ssc.withScope {
    updateStateByKey(updateFunc, defaultPartitioner())
  }`计算需要Partitioner。因为Hash高效率且不做排序，所以默认Partitioner是HashPartitoner。
![](https://img-blog.csdnimg.cn/20190317103952216.jpg)
源码中发现有七种重载函数，自行翻阅。[github源码](https://github.com/apache/spark/blob/5264164a67df498b73facae207eda12ee133be7d/streaming/src/main/scala/org/apache/spark/streaming/dstream/PairDStreamFunctions.scala)
`package com.ruozedata.G5
import org.apache.spark.streaming.{Seconds, StreamingContext}
import org.apache.spark.{HashPartitioner, SparkConf, SparkContext}
import org.apache.log4j.Level
import org.apache.log4j.Logger
object updateStateByKey_test {
  Logger.getLogger("org.apache.spark").setLevel(Level.ERROR)
  val updateFunc = (iter: Iterator[(String, Seq[Int], Option[Int])]) => {
    //iter.flatMap(k=>Some(k._2.sum + k._3.getOrElse(0)).map(x=>(k._1,x)))
    //iter.map{case(x,y,z)=>Some(y.sum + z.getOrElse(0)).map(m=>(x, m))}
    //iter.map(k => (k._1, k._2.sum + k._3.getOrElse(0)))
    iter.map{ case(word, current_count, history_count) => (word, current_count.sum + history_count.getOrElse(0)) }
  }
  def main(args: Array[String]) {
   // LoggerLevels.setStreamingLogLevels()
    //StreamingContext
    val conf = new SparkConf().setAppName("test_update").setMaster("local[2]")
    val sc = new SparkContext(conf)
    //updateStateByKey必须设置setCheckpointDir
    sc.setCheckpointDir("Z://check")
    val ssc = new StreamingContext(sc, Seconds(5))
    val ds = ssc.socketTextStream("hadoop001", 9998)
    val result = ds.flatMap(_.split(" ")).map((_, 1)).updateStateByKey(
      updateFunc, new HashPartitioner(sc.defaultParallelism), true)
    result.print()
    ssc.start()
    ssc.awaitTermination()
  }
}`例子如上所示。配合nc -lk 9999使用。
# 由于统计全局，所以需要checkpoint数据会占用较大的存储。而且效率也不高。数据很多的时候
# 不建议
# 使用updateStateByKey

