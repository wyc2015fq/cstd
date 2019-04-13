
# combineByKey实例详解 - bitcarmanlee的博客 - CSDN博客


2017年08月25日 14:22:34[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：567


我们在做数据统计与分析的时候，经常会遇到K-V结构的数据，所以处理这种K-V结构的数据也是非常常见的需求。在Spark中，除了原生的RDD天然有这种K,V结构，API中也包含有javaPairRdd,PairwiseRdd等对应的接口。而对于KV结构的数据处理就有很多种情况了，例如像数据库的group by操作等。今天我们就来说说在spark中一个常用的操作：combineByKey
## 1.combineByKey函数原型
```python
/**
   * Simplified version of combineByKeyWithClassTag that hash-partitions the resulting RDD using the
   * existing partitioner/parallelism level. This method
```
```python
is
```
```python
here
```
```python
for
```
```python
backward compatibility. It
   * does
```
```python
not
```
```python
provide combiner classtag information to the shuffle.
   *
   *
```
```python
@see [[combineByKeyWithClassTag]]
```
```python
*/
```
```python
def
```
```python
combineByKey
```
```python
[
```
```python
C
```
```python
]
```
```python
(
      createCombiner: V => C,
      mergeValue:
```
```python
(C, V)
```
```python
=> C,
      mergeCombiners:
```
```python
(C, C)
```
```python
=> C)
```
```python
:
```
```python
RDD[(K, C)] = self.withScope {
    combineByKeyWithClassTag(createCombiner, mergeValue, mergeCombiners)(null)
  }
```
可以看出，combineByKey是典型的K-V类型的算子，而且是一个transformation操作。与其他transformation操作一样，combineByKey也不会触发作业的提交。combineByKey函数主要有三个参数，而且这三个参数都是函数：
createCombiner: V => C 产生一个combiner的函数，将RDD[K,V]中的V转换成一个新的值C1
mergeValue: (C, V) => C 合并value的函数，将一个C1类型的值与一个V合并成一个新的C类型的值，假设这个新的C类型的值为C2
mergeCombiners: (C, C) => C) 将两个C类型的值合并为一个C类型的值
整个函数最后的输出为RDD[(K, C)]
## 2.看个实际例子
假设hdfs上有个文本，文本有两列：第一列为city城市名，第二列为用户标识uuid，现在想统计每个城市有多少UV并排序，用combineByKey就可以实现上述需求。源码如下：
```python
def t1(sc: SparkContext) = {
        val inputpath =
```
```python
"XXX"
```
```python
sc
```
```python
.textFile
```
```python
(inputpath).
            filter {
```
```python
x
```
```python
=>
                val lines =
```
```python
x
```
```python
.split
```
```python
(
```
```python
"\t"
```
```python
)
                lines
```
```python
.length
```
```python
==
```
```python
2
```
```python
&& lines(
```
```python
1
```
```python
)
```
```python
.length
```
```python
>
```
```python
0
```
```python
}
```
```python
.map
```
```python
{
```
```python
x
```
```python
=>
                val lines =
```
```python
x
```
```python
.split
```
```python
(
```
```python
"\t"
```
```python
)
                val (city, uuid) = (lines(
```
```python
0
```
```python
), lines(
```
```python
1
```
```python
))
                (city, uuid)
            }
```
```python
.combineByKey
```
```python
((v: String) => {
                val
```
```python
set
```
```python
= new java
```
```python
.util
```
```python
.HashSet
```
```python
[String]()
```
```python
set
```
```python
.add
```
```python
(v)
```
```python
set
```
```python
},
                (
```
```python
x
```
```python
: java
```
```python
.util
```
```python
.HashSet
```
```python
[String], v: String) => {
```
```python
x
```
```python
.add
```
```python
(v)
```
```python
x
```
```python
},
                (
```
```python
x
```
```python
: java
```
```python
.util
```
```python
.HashSet
```
```python
[String],
```
```python
y
```
```python
: java
```
```python
.util
```
```python
.HashSet
```
```python
[String]) => {
```
```python
x
```
```python
.addAll
```
```python
(
```
```python
y
```
```python
)
```
```python
x
```
```python
})
```
```python
.map
```
```python
(
```
```python
x
```
```python
=> (
```
```python
x
```
```python
._1,
```
```python
x
```
```python
._2
```
```python
.size
```
```python
()))
```
```python
.sortBy
```
```python
(_._2, false)
```
```python
.take
```
```python
(
```
```python
10
```
```python
)
```
```python
.foreach
```
```python
(println)
    }
```
代码详解：
1.
```python
(v: String) => {
                val
```
```python
set
```
```python
= new java
```
```python
.util
```
```python
.HashSet
```
```python
[String]()
```
```python
set
```
```python
.add
```
```python
(v)
```
```python
set
```
这个函数表示对于每一个city第一次出现的时候，先new一个hashset，并把此时的uuid加入到hashset中。
2.
```python
(x: java.util.HashSet[
```
```python
String
```
```python
], v:
```
```python
String
```
```python
) =>
```
```python
{
                    x.add(v)
                    x
                }
```
这个表示将每一个uuid都merge到已有的combiner中。
3.
```python
(
```
```python
x
```
```python
: java
```
```python
.util
```
```python
.HashSet
```
```python
[String],
```
```python
y
```
```python
: java
```
```python
.util
```
```python
.HashSet
```
```python
[String]) => {
```
```python
x
```
```python
.addAll
```
```python
(
```
```python
y
```
```python
)
```
```python
x
```
```python
}
```
最后一个函数表示将所有city对应的uuid的hashset合并，得到的就是每个city的所有uuid集合，达到了我们最终的目的！

