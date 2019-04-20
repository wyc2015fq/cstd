# 扩展RDD API三部曲之第二部自定义操作算子 - Spark高级玩法 - CSDN博客
2018年12月12日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：193
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWvHlGicg3iaPlH27ibbHfVCkbF41fBv4mLwWJYEibIn1HrTYicMRXRNxeNXL2xgLlcWOmuV5uvIDMVoPg/640)
扩展RDD API三部曲，主要是帮助大家掌握如下三个内容：
1). 回顾一下RDD的基础
2). 扩展Action，也即是自定义RDD算子
3). 扩展 transform及自定义RDD
本文主要是将自定义Spark RDD算子中的Action 类型操作。
**1. 准备阶段**
讲到自定义RDD的action操作，大家首先应该想到的就是那些RDD到key-value算子的隐式转换，具体一点也就是PairRDDFunctions这个类里包含的算子，比如reducebykey等操作算子。
具体实现肯定是要比较了解scala的隐式转换操作，这个浪尖也发过文章了，可以点击下文阅读：
[Scala语法基础之隐式转换](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483864&idx=1&sn=4e1e742e00b2eabcddd24d52e70a5df9&chksm=9f38e2f0a84f6be6b5201ecfead986c83d1f2f031536cc731ef8323f28c2daa92595cabf8d4f&scene=21#wechat_redirect)
首先，我们要进行准备操作，首先定义一个case class
class SalesRecord(val transactionId: String,
                  val customerId: String,
                  val itemId: String,
                  val itemValue: Double) extends Comparable[SalesRecord]
with Serializable {
  override def compareTo(o: SalesRecord): Int = {
returnthis.transactionId.compareTo(o.transactionId)
  }
  override def toString: String = {
    transactionId+","+customerId+","+itemId+","+itemValue
  }
}
然后，定义我们的主要函数：
val sparkConf = new SparkConf().setAppName(this.getClass.getName).setMaster("local[*]")
      .set("yarn.resourcemanager.hostname", "mt-mdh.local")
      .set("spark.executor.instances","2")
      .set("spark.default.parallelism","4")
      .set("spark.sql.shuffle.partitions","4")
      .setJars(List("/opt/sparkjar/bigdata.jar"
        ,"/opt/jars/spark-streaming-kafka-0-10_2.11-2.3.1.jar"
        ,"/opt/jars/kafka-clients-0.10.2.2.jar"
        ,"/opt/jars/kafka_2.11-0.10.2.2.jar"))
    val sc = new SparkContext(sparkConf)
    val dataRDD = sc.textFile("file:///opt/bigdata/src/main/data/sales.csv")
    val salesRecordRDD = dataRDD.map(row => {
      val colValues = row.split(",")
new SalesRecord(colValues(0),colValues(1),colValues(2),colValues(3).toDouble)
    })
这个时候加入我们需要对itemValue字段求和，常见的做法是
salesRecordRDD.map(_.itemValue).sum
其实，sum就是DoubleRDDFunctions内部的算子，也是通过隐式转换实现的。
**2. 自定义算子实现**
然后就是要定义RDD的操作算子本身，也即是一个工具类，我们叫他为CustomFunctions，内部包含求和函数如下：
import org.apache.spark.rdd.RDD
classCustomFunctions(rdd:RDD[SalesRecord]) {
deftotalSales = rdd.map(_.itemValue).sum
defdiscount(discountPercentage:Double) = new DiscountRDD(rdd,discountPercentage)
}
这个仔细读一下上面已有的隐式转换算子，可以发现还不行，需要为自定义RDD的操作算子，自定义一个隐士转换的算子工具，内容如下：
object CustomFunctions {
implicit def addCustomFunctions(rdd: RDD[SalesRecord]) = new CustomFunctions(rdd)
}
**3. 使用算子**
调用我们的转换方法：
println("Spark RDD API : "+salesRecordRDD.map(_.itemValue).sum)
import CustomFunctions._
println("Cunstom RDD API : "+salesRecordRDD.totalSales)
输出结果：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWvHlGicg3iaPlH27ibbHfVCkbJy9tzvZaXouavnFdZy3vicaup8SH17bYa6qO6eQaIjuMjhT8WDjZicxQ/640)
这就是自定义RDD的action操作。
下篇文章为自定义RDD和转换操作，这个就只会在星球里分享了欢迎加入浪尖的知识星球，与近420好友一起学习进步。
**推荐阅读：**
[Spark源码系列之Standalone模式下Spark应用的整个启动过程](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483817&idx=1&sn=3ff54dcd88b44a3522439f46e4c64b5c&chksm=9f38e281a84f6b971208a945ed5cf0a199d17eaa1418504dded52b63c300194670752068a048&scene=21#wechat_redirect)
[Spark源码系列之foreach和foreachPartition的区别](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483766&idx=1&sn=bed469000c668cc65a7329a278dd5c41&chksm=9f38e25ea84f6b48012580bd02227b70a1f6c1fa93ee57a8e6c569686553b22f388554fabb69&scene=21#wechat_redirect)
[Hbase源码系列之BufferedMutator的Demo和源码解析](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483750&idx=1&sn=8f86e86ff26111155d42ba55e23cda8d&chksm=9f38e24ea84f6b58405e0b751bb0d19d31a3adcdf422e6c1bf231d6741ebb2b0ef42661eac26&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640)
