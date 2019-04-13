
# SparkRDD常用算子实践（附运行效果图） - Magician的博客 - CSDN博客


2017年12月14日 20:27:15[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：1384所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



[目录](#目录)[1、简单算子说明](#1简单算子说明)
[2、复杂算子说明](#2复杂算子说明)


# 目录
**SparkRDD算子分为两类：Transformation与Action.**
Transformation：即延迟加载数据，Transformation会记录元数据信息，当计算任务触发Action时，才会真正开始计算。
Action：即立即加载数据，开始计算。
**创建RDD的方式有两种：**
1、通过sc.textFile(“/root/words.txt”)从文件系统中创建 RDD。
2、\#通过并行化scala集合创建RDD：val rdd1 = sc.parallelize(Array(1,2,3,4,5,6,7,8))
## 1、简单算子说明
这里先说下简单的Transformation算子
//通过并行化scala集合创建RDD
val rdd1 =**sc.parallelize**(Array(1,2,3,4,5,6,7,8))
//查看该rdd的分区数量
**rdd1.partitions.length**
//**map**方法同scala中的一样，将List中的每个数据拿出来做函数运算。
//**sortBy**：将数据进行排序
val rdd2 = sc.parallelize(List(5,6,4,7,3,8,2,9,1,10)).map(_*2).sortBy(x=>x,true)
//**filter**：将List中的每个数据进行函数造作，挑选出大于10的值。
val rdd3 = rdd2.filter(_>10)
//**collect**：将最终结果显示出来
//**flatMap**:对数据先进行map操作，再进行flat（碾压）操作。
rdd4.flatMap(_.split(’ ‘)).collect
**运行效果图**
![这里写图片描述](https://img-blog.csdn.net/20171214194143919?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
val rdd1 = sc.parallelize(List(5,6,4,7,3,8,2,9,1,10))
val rdd2 = sc.parallelize(List(5,6,4,7,3,8,2,9,1,10)).map(_*2).sortBy(x=>x,true)
val rdd3 = rdd2.filter(_>10)
val rdd2 = sc.parallelize(List(5,6,4,7,3,8,2,9,1,10)).map(_*2).sortBy(x=>x+”“,true)
val rdd2 = sc.parallelize(List(5,6,4,7,3,8,2,9,1,10)).map(_*2).sortBy(x=>x.toString,true)
![这里写图片描述](https://img-blog.csdn.net/20171214193847730?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
//**intersection求交集**
val rdd9 = rdd6.intersection(rdd7)
val rdd1 = sc.parallelize(List((“tom”, 1), (“jerry”, 2), (“kitty”, 3)))
val rdd2 = sc.parallelize(List((“jerry”, 9), (“tom”, 8), (“shuke”, 7)))
![这里写图片描述](https://img-blog.csdn.net/20171214195017839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
//**join**
val rdd3 = rdd1.join(rdd2)
![这里写图片描述](https://img-blog.csdn.net/20171214195105315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171214195105315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
val rdd3 = rdd1.leftOuterJoin(rdd2)
![这里写图片描述](https://img-blog.csdn.net/20171214195131899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171214195131899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
val rdd3 = rdd1.rightOuterJoin(rdd2)
[
](https://img-blog.csdn.net/20171214195131899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
//**union**：求并集，注意类型要一致
val rdd6 = sc.parallelize(List(5,6,4,7))
val rdd7 = sc.parallelize(List(1,2,3,4))
val rdd8 = rdd6.union(rdd7)
rdd8.distinct.sortBy(x=>x).collect
![这里写图片描述](https://img-blog.csdn.net/20171214195145577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
//**groupByKey**
val rdd3 = rdd1 union rdd2
rdd3.groupByKey
rdd3.groupByKey.map(x=>(x._1,x._2.sum))
![这里写图片描述](https://img-blog.csdn.net/20171214195208307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
//**cogroup**
val rdd1 = sc.parallelize(List((“tom”, 1), (“tom”, 2), (“jerry”, 3), (“kitty”, 2)))
val rdd2 = sc.parallelize(List((“jerry”, 2), (“tom”, 1), (“shuke”, 2)))
val rdd3 = rdd1.cogroup(rdd2)
val rdd4 = rdd3.map(t=>(t._1, t._2._1.sum + t._2._2.sum))
![这里写图片描述](https://img-blog.csdn.net/20171214195249096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
//cartesian笛卡尔积
val rdd1 = sc.parallelize(List(“tom”, “jerry”))
val rdd2 = sc.parallelize(List(“tom”, “kitty”, “shuke”))
val rdd3 = rdd1.cartesian(rdd2)
![这里写图片描述](https://img-blog.csdn.net/20171214195319281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**接下来说下简单的Action算子**
val rdd1 = sc.parallelize(List(1,2,3,4,5), 2)
**\#collect**
rdd1.collect
**\#reduce**
val rdd2 = rdd1.reduce(*+*)
**\#count**
rdd1.count
**\#top**
rdd1.top(2)
**\#take**
rdd1.take(2)
**\#first(similer to take(1))**
rdd1.first
**\#takeOrdered**
rdd1.takeOrdered(3)
![这里写图片描述](https://img-blog.csdn.net/20171214195517877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2、复杂算子说明
**mapPartitionsWithIndex**: 把每个partition中的分区号和对应的值拿出来, 看源码
val func = (index: Int, iter: Iterator[(Int)]) => {
iter.toList.map(x => “[partID:” +  index + “, val: ” + x + “]”).iterator
}
val rdd1 = sc.parallelize(List(1,2,3,4,5,6,7,8,9), 2)
rdd1.mapPartitionsWithIndex(func).collect
![这里写图片描述](https://img-blog.csdn.net/20171214195758590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**aggregate**
def func1(index: Int, iter: Iterator[(Int)]) : Iterator[String] = {
iter.toList.map(x => “[partID:” +  index + “, val: ” + x + “]”).iterator
}
val rdd1 = sc.parallelize(List(1,2,3,4,5,6,7,8,9), 2)
rdd1.mapPartitionsWithIndex(func1).collect
**\#\#\#是action操作, 第一个参数是初始值, 二:是2个函数(第一个函数:先对个个分区进行合并, 第二个函数:对个个分区合并后的结果再进行合并)**
**\#\#\#0 + (0+1+2+3+4   +   0+5+6+7+8+9)**
```python
rdd1.
```
```python
aggregate
```
```python
(
```
```python
0
```
```python
)(_+_, _+_)
```
![这里写图片描述](https://img-blog.csdn.net/20171214200258748?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
rdd1.aggregate(0)(math.max(*,*), _ + _)
**\#\#\#0分别与0和1分区的List元素对比得到每个分区中的最大值，在这里分别是3和7，然后将0+3+7=10**
![这里写图片描述](https://img-blog.csdn.net/20171214200316260?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**\#\#\#5和1比, 得5再和234比得5 –> 5和6789比,得9 –> 5 + (5+9)**
rdd1.aggregate(5)(math.max(*,*), _ + _)
---
val rdd3 = sc.parallelize(List(“12”,”23”,”345”,”4567”),2)
rdd3.aggregate(“”)((x,y) => math.max(x.length, y.length).toString, (x,y) => x + y)
**\#\#\#\#\#\#\#\#\#    “”.length分别与两个分区元素的length进行比较得到0分区为字符串”2”，1分区为字符串”4”，然而结果返回不分先后，所以结果是24或42**
![这里写图片描述](https://img-blog.csdn.net/20171214200703333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
val rdd4 = sc.parallelize(List(“12”,”23”,”345”,”“),2)
rdd4.aggregate(“”)((x,y) => math.min(x.length, y.length).toString, (x,y) => x + y)
**\#\#\#\#\#\#\#\#  “”.length的为0，与“12”比较后得到字符串“0”，然后字符串“0”再与“23”比较得到最小值为1.**
![这里写图片描述](https://img-blog.csdn.net/20171214200745504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**aggregateByKey**
val pairRDD = sc.parallelize(List( (“cat”,2), (“cat”, 5), (“mouse”, 4),(“cat”, 12), (“dog”, 12), (“mouse”, 2)), 2)
def func2(index: Int, iter: Iterator[(String, Int)]) : Iterator[String] = {
iter.toList.map(x => “[partID:” +  index + “, val: ” + x + “]”).iterator
}
pairRDD.mapPartitionsWithIndex(func2).collect
![这里写图片描述](https://img-blog.csdn.net/20171214200831855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
pairRDD.aggregateByKey(0)(math.max(*,*), _ + _).collect
**\#\#\#\#\#\#\#\#\#\#    先对0号分区中的各个数据进行操作（拿初始值和各个数据进行比较）得到（cat,5）(mouse,4).然后再对1号分区中的数据进行操作得到（cat，12）（dog,12）(mouse,2)。然后再对两个分区的数据进行相加得到最终结果**
![这里写图片描述](https://img-blog.csdn.net/20171214200841917?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**coalesce**
**\#coalesce(2, false)代表将数据重新分成2个区，不进行shuffle（将数据重新进行随机分配，数据通过网络可分配在不同的机器上）**
val rdd1 = sc.parallelize(1 to 10, 10)
val rdd2 = rdd1.coalesce(2, false)
rdd2.partitions.length
![这里写图片描述](https://img-blog.csdn.net/20171214201121934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**repartition**
repartition效果等同于coalesce(x, true)
---
**collectAsMap : Map(b -> 2, a -> 1)**
val rdd = sc.parallelize(List((“a”, 1), (“b”, 2)))
rdd.collectAsMap
![这里写图片描述](https://img-blog.csdn.net/20171214201131597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**combineByKey : 和reduceByKey是相同的效果**
**\#\#\#第一个参数x:原封不动取出来, 第二个参数:是函数, 局部运算, 第三个:是函数, 对局部运算后的结果再做运算**
**\#\#\#每个分区中每个key中value中的第一个值, (hello,1)(hello,1)(good,1)–>(hello(1,1),good(1))–>x就相当于hello的第一个1, good中的1**
val rdd1 = sc.textFile(“hdfs://master:9000/wordcount/input/”).flatMap(*.split(” “)).map((*, 1))
val rdd2 = rdd1.combineByKey(x => x, (a: Int, b: Int) => a + b, (m: Int, n: Int) => m + n)
rdd1.collect
![这里写图片描述](https://img-blog.csdn.net/20171214201513479?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**\#\#\#当input下有3个文件时(有3个block块分三个区, 不是有3个文件就有3个block, ), 每个会多加3个10**
val rdd3 = rdd1.combineByKey(x => x + 10, (a: Int, b: Int) => a + b, (m: Int, n: Int) => m + n)
rdd3.collect
![这里写图片描述](https://img-blog.csdn.net/20171214201525379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
val rdd4 = sc.parallelize(List(“dog”,”cat”,”gnu”,”salmon”,”rabbit”,”turkey”,”wolf”,”bear”,”bee”), 3)
val rdd5 = sc.parallelize(List(1,1,2,2,2,1,2,2,2), 3)
val rdd6 = rdd5.zip(rdd4)
![这里写图片描述](https://img-blog.csdn.net/20171214201557927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**//第一个参数List(_)代表的是将第一个元素转换为一个List，第 二个参数x: List[String], y: String) => x :+ y，代表将元素y加入到这个list中。第三个参数:(m: List[String], n: List[String]) => m ++ n),代表将两个分区的各个list合并成新的List。**
val rdd7 = rdd6.combineByKey(List(_), (x: List[String], y: String) => x :+ y, (m: List[String], n: List[String]) => m ++ n)
![这里写图片描述](https://img-blog.csdn.net/20171214201622367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**countByKey**
val rdd1 = sc.parallelize(List((“a”, 1), (“b”, 2), (“b”, 2), (“c”, 2), (“c”, 1)))
rdd1.countByKey
rdd1.countByValue
![这里写图片描述](https://img-blog.csdn.net/20171214201844935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**filterByRange**
val rdd1 = sc.parallelize(List((“e”, 5), (“c”, 3), (“d”, 4), (“c”, 2), (“a”, 1)))
val rdd2 = rdd1.filterByRange(“b”, “d”)
rdd2.collect
![这里写图片描述](https://img-blog.csdn.net/20171214201857115?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**flatMapValues**:  Array((a,1), (a,2), (b,3), (b,4))
val rdd3 = sc.parallelize(List((“a”, “1 2”), (“b”, “3 4”)))
val rdd4 = rdd3.flatMapValues(_.split(” “))
rdd4.collect
![这里写图片描述](https://img-blog.csdn.net/20171214201904488?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**foldByKey**
val rdd1 = sc.parallelize(List(“dog”, “wolf”, “cat”, “bear”), 2)
val rdd2 = rdd1.map(x => (x.length, x))
val rdd3 = rdd2.foldByKey(“”)(*+*)
![这里写图片描述](https://img-blog.csdn.net/20171214201914777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**keyBy : 以传入的参数做key**
val rdd1 = sc.parallelize(List(“dog”, “salmon”, “salmon”, “rat”, “elephant”), 3)
val rdd2 = rdd1.keyBy(_.length)
rdd2.collect
![这里写图片描述](https://img-blog.csdn.net/20171214202022811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
**keys values**
val rdd1 = sc.parallelize(List(“dog”, “tiger”, “lion”, “cat”, “panther”, “eagle”), 2)
val rdd2 = rdd1.map(x => (x.length, x))
rdd2.keys.collect
rdd2.values.collect
![这里写图片描述](https://img-blog.csdn.net/20171214202030055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---
以下是一些方法的英文解释
\#
map(func)
Return a new distributed dataset formed by passing each element of the source through a function func.
filter(func)
Return a new dataset formed by selecting those elements of the source on which func returns true.
flatMap(func)（内部执行顺序是从右往左，先执行Map再执行Flat）
Similar to map, but each input item can be mapped to 0 or more output items (so func should return a Seq rather than a single item).
mapPartitions(func)
Similar to map, but runs separately on each partition (block) of the RDD, so func must be of type Iterator => Iteratorwhen running on an RDD of type T.
mapPartitionsWithIndex(func)
Similar to mapPartitions, but also provides func with an integer value representing the index of the partition, so func must be of type (Int, Iterator) => Iteratorwhen running on an RDD of type T.
sample(withReplacement, fraction, seed)
Sample a fraction fraction of the data, with or without replacement, using a given random number generator seed.
union(otherDataset)
Return a new dataset that contains the union of the elements in the source dataset and the argument.
intersection(otherDataset)
Return a new RDD that contains the intersection of elements in the source dataset and the argument.
distinct([numTasks]))
Return a new dataset that contains the distinct elements of the source dataset.
groupByKey([numTasks])
When called on a dataset of (K, V) pairs, returns a dataset of (K, Iterable) pairs.
reduceByKey(func, [numTasks])
When called on a dataset of (K, V) pairs, returns a dataset of (K, V) pairs where the values for each key are aggregated using the given reduce function func, which must be of type (V,V) => V. Like in groupByKey, the number of reduce tasks is configurable through an optional second argument.
aggregateByKey(zeroValue)(seqOp, combOp, [numTasks])
When called on a dataset of (K, V) pairs, returns a dataset of (K, U) pairs where the values for each key are aggregated using the given combine functions and a neutral “zero” value. Allows an aggregated value type that is different than the input value type, while avoiding unnecessary allocations. Like in groupByKey, the number of reduce tasks is configurable through an optional second argument.
sortByKey([ascending], [numTasks])
When called on a dataset of (K, V) pairs where K implements Ordered, returns a dataset of (K, V) pairs sorted by keys in ascending or descending order, as specified in the boolean ascending argument.
join(otherDataset, [numTasks])
When called on datasets of type (K, V) and (K, W), returns a dataset of (K, (V, W)) pairs with all pairs of elements for each key. Outer joins are supported through leftOuterJoin, rightOuterJoin, and fullOuterJoin.
cogroup(otherDataset, [numTasks])
When called on datasets of type (K, V) and (K, W), returns a dataset of (K, (Iterable, Iterable)) tuples. This operation is also called groupWith.
cartesian(otherDataset)
When called on datasets of types T and U, returns a dataset of (T, U) pairs (all pairs of elements).
pipe(command, [envVars])
Pipe each partition of the RDD through a shell command, e.g. a Perl or bash script. RDD elements are written to the process’s stdin and lines output to its stdout are returned as an RDD of strings.
coalesce(numPartitions)
Decrease the number of partitions in the RDD to numPartitions. Useful for running operations more efficiently after filtering down a large dataset.
repartition(numPartitions)
Reshuffle the data in the RDD randomly to create either more or fewer partitions and balance it across them. This always shuffles all data over the network.
repartitionAndSortWithinPartitions(partitioner)
Repartition the RDD according to the given partitioner and, within each resulting partition, sort records by their keys. This is more efficient than calling repartition and then sorting within each partition because it can push the sorting down into the shuffle machinery.
(K,(Iterable,Iterable))

