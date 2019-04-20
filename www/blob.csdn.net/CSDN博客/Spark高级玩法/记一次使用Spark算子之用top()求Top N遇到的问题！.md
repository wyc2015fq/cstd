# 记一次使用Spark算子之用top()求Top N遇到的问题！ - Spark高级玩法 - CSDN博客
2018年07月07日 20:59:03[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：723
> 
**需求：使用spark统计词频，并求出现次数最多的10个词以及出现次数**
**问题：用Spark算子top()，求top N的时候结果不准确**
我们用一首被初中生唱收费的《That girl》来做测试： 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU6Z0FCsmiaNrx2rs8lon1kLbyaf6ESNvfibsJicSiaiaffagKLB6SpeIdaxflGDqzSSgmn54BiaUFduuJA/640?wx_fmt=png)
> 
步骤一：使用算子map() 、reduceByKey()、filter()统计词频
defwordcount(): Unit ={
  val  conf = new SparkConf().setAppName("wordcount").setMaster("local[*]")
  val sc = new  SparkContext(conf)
       sc.setLogLevel("ERROR")
  val rdd1 = sc.textFile("song.txt")
  val sortWord = rdd1.flatMap(_.split(" "))
    .map(x => (x,1))
    .reduceByKey((v1,v2) => v1 + v2)
    .filter(x => x._1 != "")
    .foreach(println)
    sc.stop()
}
输出： 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU6Z0FCsmiaNrx2rs8lon1kLAYUtiaDicAmyEChuIMQtQVqC4QYFfbTVY9pOXibKvKIGILr7KGuDD3LVw/640?wx_fmt=png)
> 
步骤二：根据词频倒序排序  
  注意：sortBy(x => x._2,false,1)，需要设置分区数为1，不然会在不同的分区内排序
```
def wordcount(): Unit ={
  val  conf = new SparkConf().setAppName("wordcount").setMaster("local[*]")
  val sc = new  SparkContext(conf)
       sc.setLogLevel("ERROR")
  val rdd1 = sc.textFile("song.txt")
  val sortWord = rdd1.flatMap(_.split(" "))
    .map(x => (x,1))
    .reduceByKey((v1,v2) =>  v1 + v2)
    .filter(x => x._1 != "")
    .sortBy(x => x._2,false,1)
    .foreach(println)
    sc.stop()}
```
输出： 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU6Z0FCsmiaNrx2rs8lon1kLC8xxtnax9O5sll3QRiawk4sYEJHgvatNwpTl9WtjLjsl9FEQRd05pdQ/640?wx_fmt=png)
> 
步骤三：使用top()算子求top 10
```
def wordcount(): Unit ={
  val  conf = new SparkConf().setAppName("wordcount").setMaster("local[*]")
  val sc = new  SparkContext(conf)
       sc.setLogLevel("ERROR")
  val rdd1 = sc.textFile("song.txt")
  val sortWord = rdd1.flatMap(_.split(" "))
    .map(x => (x,1))
    .reduceByKey((v1,v2) => v1 + v2)
    .filter(x => x._1 != "")
    .sortBy(x => x._2,false,1)
    .top(2)
    .foreach(println)
    sc.stop()}
```
输出： 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU6Z0FCsmiaNrx2rs8lon1kLicicZiafYTSzekcBZLO0Q6bVHWCuM0Xov4HjCjUaHiaoXEF1icAHKktURiaQ/640?wx_fmt=png)
> 
注意： 这里问题来了，对比一下第二步和第三步的输出发现，得到的top 10结果并不是倒序排序后得到的前十个词！！！
我门来看看top()函数的源码：
```python
def top(num: Int)(implicit ord: Ordering[T]): Array[T] = withScope {
  takeOrdered(num)(ord.reverse)
}
```
top()调用takeOrdered()的源码：
```python
def takeOrdered(num: Int)(implicit ord: Ordering[T]): Array[T] = withScope { if (num == 0) {
   Array.empty
 } else {
   val mapRDDs = mapPartitions { items =>
     val queue = new BoundedPriorityQueue[T](num)(ord.reverse)
     queue ++= util.collection.Utils.takeOrdered(items, num)(ord)
     Iterator.single(queue)
   }   if (mapRDDs.partitions.length == 0) {
     Array.empty
   } else {
     mapRDDs.reduce { (queue1, queue2) =>
       queue1 ++= queue2
       queue1
     }.toArray.sorted(ord)
   }
 }
}
```
> 
###### 问题分析：
top()算子底层调用了 takeOrdered()这个函数，这个函数也是RDD中的一个算子，来看看上边的源码：
> 
首先takeOrdered()里调用了 mapPartitions()，也就是说使用top()的时候会对我们第二次输出的结果进行分区，默认为2个分区，所以看到第三步的结果应该是每个分区的top(5)（这里我想的对不对，还有待商榷）； 
其次top()会对我之前sortBy()的结果按照key重新排序，所以导致了我们Top N的结果不准确；
###### 解决方案：
> 
方案一：指定top()的排序方法，这里我们直接根据value排序：sortBy(x => x._2,false).top(10)(Ordering.by(e => e._2) 
方案二：不用top()，直接用sortBy(x =>x._2,false).take(10) 
方案三：既然top()底层调用的是takeOrdered()，我们也直接可以用takeOrdered(10)(Ordering.by(e => e._2) 
**思考：方案一中，我们既然指定了top()的排序方式，还需要sortBy()嘛？？？当然可以不用啊！！！所以我们可以去掉sortBy()*
**科普一下：top(10)(Ordering.by(e => e._2) 这种写法叫做函数的柯里化。*
**柯里化(Currying)：把接受多个参数的函数变换成接受一个单一参数(最初函数的第一个参数)的函数，并且返回接受余下的参数且返回结果的新函数的技术。这个技术由 Christopher Strachey 以逻辑学家 Haskell Curry 命名的，尽管它是 Moses Schnfinkel 和 Gottlob Frege 发明的。*
# 使用top():def wordcount(): Unit ={
  val  conf = new SparkConf().setAppName("wordcount").setMaster("local[*]")
  val sc = new  SparkContext(conf)
  sc.setLogLevel("ERROR")
  val rdd1 = sc.textFile("song.txt")
  val sortWord = rdd1.flatMap(_.split(" "))
    .map(x => (x,1))
    .reduceByKey((v1,v2) => v1 + v2)
    .filter(x => x._1 != "")
    //.sortBy(x => x._2,false,1)
    .top(10)(Ordering.by(x => x._2))
    .foreach(println)sc.stop()}
# 使用take()def wordcount(): Unit ={
  val  conf = new SparkConf().setAppName("wordcount").setMaster("local[*]")
  val sc = new  SparkContext(conf)
  sc.setLogLevel("ERROR")
  val rdd1 = sc.textFile("song.txt")
  val sortWord = rdd1.flatMap(_.split(" "))
    .map(x => (x,1))
    .reduceByKey((v1,v2) => v1 + v2)
    .filter(x => x._1 != "")
    .sortBy(x =>x._2,false).take(10)
    .foreach(println)sc.stop()}
两者结果都是一样的： 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU6Z0FCsmiaNrx2rs8lon1kLpa2vmgsa84tQS1wKr3yrh0oKQFC36AsJKs5Tnic5MtNPTtfqtMx119w/640?wx_fmt=png)
本文是一spark爱好者，投稿，在这里再次谢谢他的分享。也欢迎联系浪尖进行投稿。原文阅读，请点击阅读原文。
