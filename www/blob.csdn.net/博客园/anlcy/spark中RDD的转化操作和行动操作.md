
# spark中RDD的转化操作和行动操作 - anlcy - 博客园






# [spark中RDD的转化操作和行动操作](https://www.cnblogs.com/camilla/p/8296704.html)
本文主要是讲解spark里RDD的基础操作。RDD是spark特有的数据模型，谈到RDD就会提到什么弹性分布式数据集，什么有向无环图，本文暂时不去展开这些高深概念，在阅读本文时候，大家可以就把RDD当作一个数组，这样的理解对我们学习RDD的API是非常有帮助的。本文所有示例代码都是使用scala语言编写的。
Spark里的计算都是操作RDD进行，那么学习RDD的第一个问题就是如何构建RDD，构建RDD从数据来源角度分为两类：第一类是从内存里直接读取数据，第二类就是从文件系统里读取，当然这里的文件系统种类很多常见的就是HDFS以及本地文件系统了。
第一类方式从内存里构造RDD，使用的方法：makeRDD和parallelize方法，如下代码所示：
```python
/* 使用makeRDD创建RDD */
```

```python
/* List */
```

```python
val
```
```python
rdd
```
```python
01
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
))
```

```python
val
```
```python
r
```
```python
01
```
```python
=
```
```python
rdd
```
```python
01
```
```python
.map { x
```
```python
=
```
```python
> x * x }
```

```python
println(r
```
```python
01
```
```python
.collect().mkString(
```
```python
","
```
```python
))
```

```python
/* Array */
```

```python
val
```
```python
rdd
```
```python
02
```
```python
=
```
```python
sc.makeRDD(Array(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
))
```

```python
val
```
```python
r
```
```python
02
```
```python
=
```
```python
rdd
```
```python
02
```
```python
.filter { x
```
```python
=
```
```python
> x <
```
```python
5
```
```python
}
```

```python
println(r
```
```python
02
```
```python
.collect().mkString(
```
```python
","
```
```python
))
```

```python
val
```
```python
rdd
```
```python
03
```
```python
=
```
```python
sc.parallelize(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
),
```
```python
1
```
```python
)
```

```python
val
```
```python
r
```
```python
03
```
```python
=
```
```python
rdd
```
```python
03
```
```python
.map { x
```
```python
=
```
```python
> x +
```
```python
1
```
```python
}
```

```python
println(r
```
```python
03
```
```python
.collect().mkString(
```
```python
","
```
```python
))
```

```python
/* Array */
```

```python
val
```
```python
rdd
```
```python
04
```
```python
=
```
```python
sc.parallelize(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
),
```
```python
1
```
```python
)
```

```python
val
```
```python
r
```
```python
04
```
```python
=
```
```python
rdd
```
```python
04
```
```python
.filter { x
```
```python
=
```
```python
> x >
```
```python
3
```
```python
}
```

```python
println(r
```
```python
04
```
```python
.collect().mkString(
```
```python
","
```
```python
))
```
大家看到了RDD本质就是一个数组，因此构造数据时候使用的是List（链表）和Array（数组）类型。
第二类方式是通过文件系统构造RDD，代码如下所示：
```python
val
```
```python
rdd
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
sc.textFile(
```
```python
"file:///D:/sparkdata.txt"
```
```python
,
```
```python
1
```
```python
)
```

```python
val
```
```python
r
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
rdd.flatMap { x
```
```python
=
```
```python
> x.split(
```
```python
","
```
```python
) }
```

```python
println(r.collect().mkString(
```
```python
","
```
```python
))
```
这里例子使用的是本地文件系统，所以文件路径协议前缀是file://。
构造了RDD对象了，接下来就是如何操作RDD对象了，RDD的操作分为转化操作（transformation）和行动操作（action），RDD之所以将操作分成这两类这是和RDD惰性运算有关，当RDD执行转化操作时候，实际计算并没有被执行，只有当RDD执行行动操作时候才会促发计算任务提交，执行相应的计算操作。区别转化操作和行动操作也非常简单，转化操作就是从一个RDD产生一个新的RDD操作，而行动操作就是进行实际的计算。
下面是RDD的基础操作API介绍：
### 常见的转化操作
**示例数据{1，2，3，3}**
map
> 目的：将函数应用于RDD中的每个元素, 将返回值构成新的RDD

> 示例：rdd.map(x=>x+1)

> 结果：{2,3,4,4}
flatMap
> 目的：将函数应用于RDD中的每个元素, 将返回的迭代器的所有内容构成新的RDD。通常用来切分单词

> 示例：rdd.flatMap(x => x.to(3))

> 结果：{1,2,3,2,3,3}
|函数名|目的|示例|结果|备注|
|---|---|---|---|---|
|map()|将函数应用于RDD中的每个元素, 将返回值构成新的RDD|rdd.map(x=>x+1)|{2,3,4,4}|
|flatMap()|将函数应用于RDD中的每个元素, 将返回的迭代器的所有内容构成新的RDD。通常用来切分单词|rdd.flatMap(x => x.to(3))|{1,2,3,2,3,3}|
|filter()|返回一个由通过传给filter()的函数的元素组成的RDD|rdd.filter(x => x!=1)|{2,3,3}|
|distinct()|去重|rdd.distinct()|{1,2,3}|开销很大，因为需要将所有数据通过网络进行混洗|
|sample(withReplacement, fraction, [seed])|对RDD采样，以及是否替换|rdd.sample(false, 0.5)|非确定|
**示例数据{1,2,3}{3,4,5}**
|函数名|目的|示例|结果|备注|
|---|---|---|---|---|
|union()|生成一个包含两个RDD中所有元素的RDD|rdd.union(other)|{1,2,3,3,4,5}|
|intersection()|求两个RDD共同元素的RDD|rdd.intersection(other)|{3}|性能差，需要网络混洗数据来发现共同元素|
|subtract()|移除一个RDD中的内容|rdd.subtract(other)|{1,2}|需要数据混洗|
|cartesian()|与另一个RDD的笛卡尔积|rdd.cartesian(other)|{{1,3},{1,4},...,{3,5}}|求大规模RDD开销巨大|
### 常见的行为操作
**示例数据{1,2,3,3}**
|函数名|目的|示例|结果|备注|
|---|---|---|---|---|
|collect()|返回RDD中的所有元素|rdd.collect()|{1,2,3,3}|
|count()|RDD中的元素个数|rdd.count()|4|
|countByValue()|各元素在RDD中出现的次数|rdd.countByValue()|{(1,1),(2,1),(3,2)}|
|take(num)|从RDD中返回num个元素|rdd.take(2)|{1,2}|
|top(num)|从RDD中返回最前面的num个元素|rdd.top(2)|{3,3}|
|takeOrdered(num)(ordering)|从RDD中按照提供的顺序返回最前面的num个元素|rdd.takeOrdered(2)(myOrdering)|(3,3)|
|takeSample(withReplacement, num, [seed])|从RDD中返回任意一些元素|rdd.takeSample(false, 1)|
|reduce(func)|并行整合RDD中所有数据(例如sum)|rdd.reduce((x,y)=>x+y)|9|
|fold(zero)(func)|和reduce一样，但需要提供初始值|rdd.fold(0)((x,y)=>x+y)|9|
|aggregate(zeroValue)(seqOp, combOp)|和reduce相似，但通常不返回同类型的函数|rdd.aggregate(0,0)((x,y)=>(x._1+y,x._2+1),(x,y)=>(x._1+y._1,x._2+y._2))|{9,4}|
|foreach(func)|对RDD中的每个元素使用给定的函数|rdd.foreach(func)|
### 持久化
`val result``=``input``.``map``(``x``=>``x``*``x``)``result``.``persist``(``StorageLevel``.``DISK_ ONLY``)``println``(``result``.``count``())``println``(``result``.``collect``().``mkString``(``","``))`|级别|使用的空间|CPU时间|是否在内存中|是否在硬盘中|备注|
|---|---|---|---|---|---|
|MEMORY_ONLY|高|低|是|否|
|MEMORY_ONLY_SER|低|高|是|否|
|MEMORY_AND_DISK|高|中等|部分|部分|如果数据在内存中放不下，
|则溢写到磁盘上|
|MEMORY_AND_DISK_SER|低|高|部分|部分|如果数据在内存中放不下，
|则溢写到磁盘上。在内存中存放序列化后的数据|
|DISK_ONLY|低|高|否|是|
下面是以上API操作的示例代码，如下：
转化操作：
```python
val
```
```python
rddInt
```
```python
:
```
```python
RDD[Int]
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
,
```
```python
2
```
```python
,
```
```python
5
```
```python
,
```
```python
1
```
```python
))
```

```python
val
```
```python
rddStr
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
sc.parallelize(Array(
```
```python
"a"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"c"
```
```python
,
```
```python
"d"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"a"
```
```python
),
```
```python
1
```
```python
)
```

```python
val
```
```python
rddFile
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
sc.textFile(path,
```
```python
1
```
```python
)
```

```python
val
```
```python
rdd
```
```python
01
```
```python
:
```
```python
RDD[Int]
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
1
```
```python
,
```
```python
3
```
```python
,
```
```python
5
```
```python
,
```
```python
3
```
```python
))
```

```python
val
```
```python
rdd
```
```python
02
```
```python
:
```
```python
RDD[Int]
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
2
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
1
```
```python
))
```

```python
/* map操作 */
```

```python
println(
```
```python
"======map操作======"
```
```python
)
```

```python
println(rddInt.map(x
```
```python
=
```
```python
> x +
```
```python
1
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======map操作======"
```
```python
)
```

```python
/* filter操作 */
```

```python
println(
```
```python
"======filter操作======"
```
```python
)
```

```python
println(rddInt.filter(x
```
```python
=
```
```python
> x >
```
```python
4
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======filter操作======"
```
```python
)
```

```python
/* flatMap操作 */
```

```python
println(
```
```python
"======flatMap操作======"
```
```python
)
```

```python
println(rddFile.flatMap { x
```
```python
=
```
```python
> x.split(
```
```python
","
```
```python
) }.first())
```

```python
println(
```
```python
"======flatMap操作======"
```
```python
)
```

```python
/* distinct去重操作 */
```

```python
println(
```
```python
"======distinct去重======"
```
```python
)
```

```python
println(rddInt.distinct().collect().mkString(
```
```python
","
```
```python
))
```

```python
println(rddStr.distinct().collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======distinct去重======"
```
```python
)
```

```python
/* union操作 */
```

```python
println(
```
```python
"======union操作======"
```
```python
)
```

```python
println(rdd
```
```python
01
```
```python
.union(rdd
```
```python
02
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======union操作======"
```
```python
)
```

```python
/* intersection操作 */
```

```python
println(
```
```python
"======intersection操作======"
```
```python
)
```

```python
println(rdd
```
```python
01
```
```python
.intersection(rdd
```
```python
02
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======intersection操作======"
```
```python
)
```

```python
/* subtract操作 */
```

```python
println(
```
```python
"======subtract操作======"
```
```python
)
```

```python
println(rdd
```
```python
01
```
```python
.subtract(rdd
```
```python
02
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======subtract操作======"
```
```python
)
```

```python
/* cartesian操作 */
```

```python
println(
```
```python
"======cartesian操作======"
```
```python
)
```

```python
println(rdd
```
```python
01
```
```python
.cartesian(rdd
```
```python
02
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======cartesian操作======"
```
```python
)
```
行动操作代码如下：
```python
val
```
```python
rddInt
```
```python
:
```
```python
RDD[Int]
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
,
```
```python
2
```
```python
,
```
```python
5
```
```python
,
```
```python
1
```
```python
))
```
```python
val
```
```python
rddStr
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
sc.parallelize(Array(
```
```python
"a"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"c"
```
```python
,
```
```python
"d"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"a"
```
```python
),
```
```python
1
```
```python
)
```

```python
/* count操作 */
```

```python
println(
```
```python
"======count操作======"
```
```python
)
```

```python
println(rddInt.count())
```

```python
println(
```
```python
"======count操作======"
```
```python
)
```

```python
/* countByValue操作 */
```

```python
println(
```
```python
"======countByValue操作======"
```
```python
)
```

```python
println(rddInt.countByValue())
```

```python
println(
```
```python
"======countByValue操作======"
```
```python
)
```

```python
/* reduce操作 */
```

```python
println(
```
```python
"======countByValue操作======"
```
```python
)
```

```python
println(rddInt.reduce((x ,y)
```
```python
=
```
```python
> x + y))
```

```python
println(
```
```python
"======countByValue操作======"
```
```python
)
```

```python
/* fold操作 */
```

```python
println(
```
```python
"======fold操作======"
```
```python
)
```

```python
println(rddInt.fold(
```
```python
0
```
```python
)((x ,y)
```
```python
=
```
```python
> x + y))
```

```python
println(
```
```python
"======fold操作======"
```
```python
)
```

```python
/* aggregate操作 */
```

```python
println(
```
```python
"======aggregate操作======"
```
```python
)
```

```python
val
```
```python
res
```
```python
:
```
```python
(Int,Int)
```
```python
=
```
```python
rddInt.aggregate((
```
```python
0
```
```python
,
```
```python
0
```
```python
))((x,y)
```
```python
=
```
```python
> (x.
```
```python
_
```
```python
1
```
```python
+ x.
```
```python
_
```
```python
2
```
```python
,y),(x,y)
```
```python
=
```
```python
> (x.
```
```python
_
```
```python
1
```
```python
+ x.
```
```python
_
```
```python
2
```
```python
,y.
```
```python
_
```
```python
1
```
```python
+ y.
```
```python
_
```
```python
2
```
```python
))
```

```python
println(res.
```
```python
_
```
```python
1
```
```python
+
```
```python
","
```
```python
+ res.
```
```python
_
```
```python
2
```
```python
)
```

```python
println(
```
```python
"======aggregate操作======"
```
```python
)
```

```python
/* foeach操作 */
```

```python
println(
```
```python
"======foeach操作======"
```
```python
)
```

```python
println(rddStr.foreach { x
```
```python
=
```
```python
> println(x) })
```

```python
println(
```
```python
"======foeach操作======"
```
```python
)
```
RDD操作暂时先学习到这里，剩下的内容在下一篇里再谈了，下面我要说说如何开发spark，安装spark的内容我后面会使用专门的文章进行讲解，这里我们假设已经安装好了spark，那么我们就可以在已经装好的spark服务器上使用spark-shell进行与spark交互的shell，这里我们直接可以敲打代码编写spark程序。但是spark-shell毕竟使用太麻烦，而且spark-shell一次只能使用一个用户，当另外一个用户要使用spark-shell就会把前一个用户踢掉，而且shell也没有IDE那种代码补全，代码校验的功能，使用起来很是痛苦。
不过spark的确是一个神奇的框架，这里的神奇就是指spark本地开发调试非常简单，本地开发调试不需要任何已经装好的spark系统，我们只需要建立一个项目，这个项目可以是java的也可以是scala，然后我们将spark-assembly-1.6.1-hadoop2.6.0.jar这样的jar放入项目的环境里，这个时候我们就可以在本地开发调试spark程序了。
大家请看我们装有scala插件的eclipse里的完整代码：
```python
package
```
```python
cn.com.sparktest
```

```python
import
```
```python
org.apache.spark.SparkConf
```

```python
import
```
```python
org.apache.spark.SparkConf
```

```python
import
```
```python
org.apache.spark.SparkContext
```

```python
import
```
```python
org.apache.spark.rdd.RDD
```

```python
object
```
```python
SparkTest {
```

```python
val
```
```python
conf
```
```python
:
```
```python
SparkConf
```
```python
=
```
```python
new
```
```python
SparkConf().setAppName(
```
```python
"xtq"
```
```python
).setMaster(
```
```python
"local[2]"
```
```python
)
```

```python
val
```
```python
sc
```
```python
:
```
```python
SparkContext
```
```python
=
```
```python
new
```
```python
SparkContext(conf)
```

```python
/**
```

```python
* 创建数据的方式--从内存里构造数据(基础)
```

```python
*/
```

```python
def
```
```python
createDataMethod()
```
```python
:
```
```python
Unit
```
```python
=
```
```python
{
```

```python
/* 使用makeRDD创建RDD */
```

```python
/* List */
```

```python
val
```
```python
rdd
```
```python
01
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
))
```

```python
val
```
```python
r
```
```python
01
```
```python
=
```
```python
rdd
```
```python
01
```
```python
.map { x
```
```python
=
```
```python
> x * x }
```

```python
println(
```
```python
"===================createDataMethod:makeRDD:List====================="
```
```python
)
```

```python
println(r
```
```python
01
```
```python
.collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"===================createDataMethod:makeRDD:List====================="
```
```python
)
```

```python
/* Array */
```

```python
val
```
```python
rdd
```
```python
02
```
```python
=
```
```python
sc.makeRDD(Array(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
))
```

```python
val
```
```python
r
```
```python
02
```
```python
=
```
```python
rdd
```
```python
02
```
```python
.filter { x
```
```python
=
```
```python
> x <
```
```python
5
```
```python
}
```

```python
println(
```
```python
"===================createDataMethod:makeRDD:Array====================="
```
```python
)
```

```python
println(r
```
```python
02
```
```python
.collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"===================createDataMethod:makeRDD:Array====================="
```
```python
)
```

```python
/* 使用parallelize创建RDD */
```

```python
/* List */
```

```python
val
```
```python
rdd
```
```python
03
```
```python
=
```
```python
sc.parallelize(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
),
```
```python
1
```
```python
)
```

```python
val
```
```python
r
```
```python
03
```
```python
=
```
```python
rdd
```
```python
03
```
```python
.map { x
```
```python
=
```
```python
> x +
```
```python
1
```
```python
}
```

```python
println(
```
```python
"===================createDataMethod:parallelize:List====================="
```
```python
)
```

```python
println(r
```
```python
03
```
```python
.collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"===================createDataMethod:parallelize:List====================="
```
```python
)
```

```python
/* Array */
```

```python
val
```
```python
rdd
```
```python
04
```
```python
=
```
```python
sc.parallelize(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
),
```
```python
1
```
```python
)
```

```python
val
```
```python
r
```
```python
04
```
```python
=
```
```python
rdd
```
```python
04
```
```python
.filter { x
```
```python
=
```
```python
> x >
```
```python
3
```
```python
}
```

```python
println(
```
```python
"===================createDataMethod:parallelize:Array====================="
```
```python
)
```

```python
println(r
```
```python
04
```
```python
.collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"===================createDataMethod:parallelize:Array====================="
```
```python
)
```

```python
}
```

```python
/**
```

```python
* 创建Pair Map
```

```python
*/
```

```python
def
```
```python
createPairRDD()
```
```python
:
```
```python
Unit
```
```python
=
```
```python
{
```

```python
val
```
```python
rdd
```
```python
:
```
```python
RDD[(String,Int)]
```
```python
=
```
```python
sc.makeRDD(List((
```
```python
"key01"
```
```python
,
```
```python
1
```
```python
),(
```
```python
"key02"
```
```python
,
```
```python
2
```
```python
),(
```
```python
"key03"
```
```python
,
```
```python
3
```
```python
)))
```

```python
val
```
```python
r
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
rdd.keys
```

```python
println(
```
```python
"===========================createPairRDD================================="
```
```python
)
```

```python
println(r.collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"===========================createPairRDD================================="
```
```python
)
```

```python
}
```

```python
/**
```

```python
* 通过文件创建RDD
```

```python
* 文件数据：
```

```python
*    key01,1,2.3
```

```python
key02,5,3.7
```

```python
key03,23,4.8
```

```python
key04,12,3.9
```

```python
key05,7,1.3
```

```python
*/
```

```python
def
```
```python
createDataFromFile(path
```
```python
:
```
```python
String)
```
```python
:
```
```python
Unit
```
```python
=
```
```python
{
```

```python
val
```
```python
rdd
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
sc.textFile(path,
```
```python
1
```
```python
)
```

```python
val
```
```python
r
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
rdd.flatMap { x
```
```python
=
```
```python
> x.split(
```
```python
","
```
```python
) }
```

```python
println(
```
```python
"=========================createDataFromFile=================================="
```
```python
)
```

```python
println(r.collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"=========================createDataFromFile=================================="
```
```python
)
```

```python
}
```

```python
/**
```

```python
* 基本的RDD操作
```

```python
*/
```

```python
def
```
```python
basicTransformRDD(path
```
```python
:
```
```python
String)
```
```python
:
```
```python
Unit
```
```python
=
```
```python
{
```

```python
val
```
```python
rddInt
```
```python
:
```
```python
RDD[Int]
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
,
```
```python
2
```
```python
,
```
```python
5
```
```python
,
```
```python
1
```
```python
))
```

```python
val
```
```python
rddStr
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
sc.parallelize(Array(
```
```python
"a"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"c"
```
```python
,
```
```python
"d"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"a"
```
```python
),
```
```python
1
```
```python
)
```

```python
val
```
```python
rddFile
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
sc.textFile(path,
```
```python
1
```
```python
)
```

```python
val
```
```python
rdd
```
```python
01
```
```python
:
```
```python
RDD[Int]
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
1
```
```python
,
```
```python
3
```
```python
,
```
```python
5
```
```python
,
```
```python
3
```
```python
))
```

```python
val
```
```python
rdd
```
```python
02
```
```python
:
```
```python
RDD[Int]
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
2
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
1
```
```python
))
```

```python
/* map操作 */
```

```python
println(
```
```python
"======map操作======"
```
```python
)
```

```python
println(rddInt.map(x
```
```python
=
```
```python
> x +
```
```python
1
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======map操作======"
```
```python
)
```

```python
/* filter操作 */
```

```python
println(
```
```python
"======filter操作======"
```
```python
)
```

```python
println(rddInt.filter(x
```
```python
=
```
```python
> x >
```
```python
4
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======filter操作======"
```
```python
)
```

```python
/* flatMap操作 */
```

```python
println(
```
```python
"======flatMap操作======"
```
```python
)
```

```python
println(rddFile.flatMap { x
```
```python
=
```
```python
> x.split(
```
```python
","
```
```python
) }.first())
```

```python
println(
```
```python
"======flatMap操作======"
```
```python
)
```

```python
/* distinct去重操作 */
```

```python
println(
```
```python
"======distinct去重======"
```
```python
)
```

```python
println(rddInt.distinct().collect().mkString(
```
```python
","
```
```python
))
```

```python
println(rddStr.distinct().collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======distinct去重======"
```
```python
)
```

```python
/* union操作 */
```

```python
println(
```
```python
"======union操作======"
```
```python
)
```

```python
println(rdd
```
```python
01
```
```python
.union(rdd
```
```python
02
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======union操作======"
```
```python
)
```

```python
/* intersection操作 */
```

```python
println(
```
```python
"======intersection操作======"
```
```python
)
```

```python
println(rdd
```
```python
01
```
```python
.intersection(rdd
```
```python
02
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======intersection操作======"
```
```python
)
```

```python
/* subtract操作 */
```

```python
println(
```
```python
"======subtract操作======"
```
```python
)
```

```python
println(rdd
```
```python
01
```
```python
.subtract(rdd
```
```python
02
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======subtract操作======"
```
```python
)
```

```python
/* cartesian操作 */
```

```python
println(
```
```python
"======cartesian操作======"
```
```python
)
```

```python
println(rdd
```
```python
01
```
```python
.cartesian(rdd
```
```python
02
```
```python
).collect().mkString(
```
```python
","
```
```python
))
```

```python
println(
```
```python
"======cartesian操作======"
```
```python
)
```

```python
}
```

```python
/**
```

```python
* 基本的RDD行动操作
```

```python
*/
```

```python
def
```
```python
basicActionRDD()
```
```python
:
```
```python
Unit
```
```python
=
```
```python
{
```

```python
val
```
```python
rddInt
```
```python
:
```
```python
RDD[Int]
```
```python
=
```
```python
sc.makeRDD(List(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
,
```
```python
4
```
```python
,
```
```python
5
```
```python
,
```
```python
6
```
```python
,
```
```python
2
```
```python
,
```
```python
5
```
```python
,
```
```python
1
```
```python
))
```

```python
val
```
```python
rddStr
```
```python
:
```
```python
RDD[String]
```
```python
=
```
```python
sc.parallelize(Array(
```
```python
"a"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"c"
```
```python
,
```
```python
"d"
```
```python
,
```
```python
"b"
```
```python
,
```
```python
"a"
```
```python
),
```
```python
1
```
```python
)
```

```python
/* count操作 */
```

```python
println(
```
```python
"======count操作======"
```
```python
)
```

```python
println(rddInt.count())
```

```python
println(
```
```python
"======count操作======"
```
```python
)
```

```python
/* countByValue操作 */
```

```python
println(
```
```python
"======countByValue操作======"
```
```python
)
```

```python
println(rddInt.countByValue())
```

```python
println(
```
```python
"======countByValue操作======"
```
```python
)
```

```python
/* reduce操作 */
```

```python
println(
```
```python
"======countByValue操作======"
```
```python
)
```

```python
println(rddInt.reduce((x ,y)
```
```python
=
```
```python
> x + y))
```

```python
println(
```
```python
"======countByValue操作======"
```
```python
)
```

```python
/* fold操作 */
```

```python
println(
```
```python
"======fold操作======"
```
```python
)
```

```python
println(rddInt.fold(
```
```python
0
```
```python
)((x ,y)
```
```python
=
```
```python
> x + y))
```

```python
println(
```
```python
"======fold操作======"
```
```python
)
```

```python
/* aggregate操作 */
```

```python
println(
```
```python
"======aggregate操作======"
```
```python
)
```

```python
val
```
```python
res
```
```python
:
```
```python
(Int,Int)
```
```python
=
```
```python
rddInt.aggregate((
```
```python
0
```
```python
,
```
```python
0
```
```python
))((x,y)
```
```python
=
```
```python
> (x.
```
```python
_
```
```python
1
```
```python
+ x.
```
```python
_
```
```python
2
```
```python
,y),(x,y)
```
```python
=
```
```python
> (x.
```
```python
_
```
```python
1
```
```python
+ x.
```
```python
_
```
```python
2
```
```python
,y.
```
```python
_
```
```python
1
```
```python
+ y.
```
```python
_
```
```python
2
```
```python
))
```

```python
println(res.
```
```python
_
```
```python
1
```
```python
+
```
```python
","
```
```python
+ res.
```
```python
_
```
```python
2
```
```python
)
```

```python
println(
```
```python
"======aggregate操作======"
```
```python
)
```

```python
/* foeach操作 */
```

```python
println(
```
```python
"======foeach操作======"
```
```python
)
```

```python
println(rddStr.foreach { x
```
```python
=
```
```python
> println(x) })
```

```python
println(
```
```python
"======foeach操作======"
```
```python
)
```

```python
}
```

```python
def
```
```python
main(args
```
```python
:
```
```python
Array[String])
```
```python
:
```
```python
Unit
```
```python
=
```
```python
{
```

```python
println(System.getenv(
```
```python
"HADOOP_HOME"
```
```python
))
```

```python
createDataMethod()
```

```python
createPairRDD()
```

```python
createDataFromFile(
```
```python
"file:///D:/sparkdata.txt"
```
```python
)
```

```python
basicTransformRDD(
```
```python
"file:///D:/sparkdata.txt"
```
```python
)
```

```python
basicActionRDD()
```
}
}
Spark执行时候我们需要构造一个SparkContenxt的环境变量，构造环境变量时候需要构造一个SparkConf对象，例如代码：setAppName("xtq").setMaster("local[2]")
appName就是spark任务名称，master为local[2]是指使用本地模式，启动2个线程完成spark任务。
在eclipse里运行spark程序时候，会报出如下错误：
```python
ava.io.IOException
```
```python
:
```
```python
Could not locate executable
```
```python
null
```
```python
\bin\winutils.exe in the Hadoop binaries.
```

```python
at org.apache.hadoop.util.Shell.getQualifiedBinPath(Shell.java
```
```python
:
```
```python
355
```
```python
)
```

```python
at org.apache.hadoop.util.Shell.getWinUtilsPath(Shell.java
```
```python
:
```
```python
370
```
```python
)
```

```python
at org.apache.hadoop.util.Shell.<clinit>(Shell.java
```
```python
:
```
```python
363
```
```python
)
```

```python
at org.apache.hadoop.util.StringUtils.<clinit>(StringUtils.java
```
```python
:
```
```python
79
```
```python
)
```

```python
at org.apache.hadoop.security.Groups.parseStaticMapping(Groups.java
```
```python
:
```
```python
104
```
```python
)
```

```python
at org.apache.hadoop.security.Groups.<init>(Groups.java
```
```python
:
```
```python
86
```
```python
)
```

```python
at org.apache.hadoop.security.Groups.<init>(Groups.java
```
```python
:
```
```python
66
```
```python
)
```

```python
at org.apache.hadoop.security.Groups.getUserToGroupsMappingService(Groups.java
```
```python
:
```
```python
280
```
```python
)
```

```python
at org.apache.hadoop.security.UserGroupInformation.initialize(UserGroupInformation.java
```
```python
:
```
```python
271
```
```python
)
```

```python
at org.apache.hadoop.security.UserGroupInformation.ensureInitialized(UserGroupInformation.java
```
```python
:
```
```python
248
```
```python
)
```

```python
at org.apache.hadoop.security.UserGroupInformation.loginUserFromSubject(UserGroupInformation.java
```
```python
:
```
```python
763
```
```python
)
```

```python
at org.apache.hadoop.security.UserGroupInformation.getLoginUser(UserGroupInformation.java
```
```python
:
```
```python
748
```
```python
)
```

```python
at org.apache.hadoop.security.UserGroupInformation.getCurrentUser(UserGroupInformation.java
```
```python
:
```
```python
621
```
```python
)
```

```python
at org.apache.spark.util.Utils$$anonfun$getCurrentUserName$
```
```python
1
```
```python
.apply(Utils.scala
```
```python
:
```
```python
2160
```
```python
)
```

```python
at org.apache.spark.util.Utils$$anonfun$getCurrentUserName$
```
```python
1
```
```python
.apply(Utils.scala
```
```python
:
```
```python
2160
```
```python
)
```

```python
at scala.Option.getOrElse(Option.scala
```
```python
:
```
```python
120
```
```python
)
```

```python
at org.apache.spark.util.Utils$.getCurrentUserName(Utils.scala
```
```python
:
```
```python
2160
```
```python
)
```

```python
at org.apache.spark.SparkContext.<init>(SparkContext.scala
```
```python
:
```
```python
322
```
```python
)
```

```python
at cn.com.sparktest.SparkTest$.<init>(SparkTest.scala
```
```python
:
```
```python
10
```
```python
)
```

```python
at cn.com.sparktest.SparkTest$.<clinit>(SparkTest.scala)
```

```python
at cn.com.sparktest.SparkTest.main(SparkTest.scala)
```
该错误不会影响程序的运算，但总是让人觉得不舒服，这个问题是因为spark运行依赖于hadoop，可是在window下其实是无法安装hadoop，只能使用cygwin模拟安装，而新版本的hadoop在windows下使用需要使用winutils.exe，解决这个问题很简单，就是下载一个winutils.exe，注意下自己操作系统是32位还是64位，找到对应版本，然后放置在这样的目录下：
D:\hadoop\bin\winutils.exe
然后再环境变量里定义HADOOP_HOME= D:\hadoop
环境变量的改变要重启eclipse，这样环境变量才会生效，这个时候程序运行就不会报出错误了。
转自：http://www.cnblogs.com/sharpxiajun/p/5506822.html








