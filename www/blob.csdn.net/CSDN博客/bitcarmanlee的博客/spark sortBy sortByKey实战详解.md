
# spark sortBy sortByKey实战详解 - bitcarmanlee的博客 - CSDN博客


2018年08月27日 19:13:41[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1171


日常工作中，排序是道绕过不过去的侃，我们每天都会面对各种各样的排序需求。那么在spark中如何排序呢？我们来看一些很有代表性的例子。
## 1.最简单的排序
假设有个`RDD[Int]`类型的数据，需要按数据大小进行排序，那这个排序算最简单的：
```python
sc
```
```python
.parallelize
```
```python
(Array(
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
6
```
```python
,
```
```python
5
```
```python
))
```
```python
.sortBy
```
```python
(
```
```python
x
```
```python
=>
```
```python
x
```
```python
)
```
```python
.collect
```
```python
()
```
代码运行的结果：
```python
Array
```
```python
[
```
```python
Int
```
```python
] =
```
```python
Array
```
```python
(
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
)
```
## 2.kv结构的排序
在kv结构的数据中，按value排序是常见的需求：
```python
sc
```
```python
.parallelize
```
```python
(Array((
```
```python
"a"
```
```python
,
```
```python
1
```
```python
), (
```
```python
"c"
```
```python
,
```
```python
3
```
```python
), (
```
```python
"b"
```
```python
,
```
```python
2
```
```python
), (
```
```python
"d"
```
```python
,
```
```python
4
```
```python
)))
```
```python
.sortBy
```
```python
(_._2)
```
代码运行的结果：
```python
Array
```
```python
[(
```
```python
String
```
```python
,
```
```python
Int
```
```python
)] =
```
```python
Array
```
```python
((a,
```
```python
1
```
```python
), (b,
```
```python
2
```
```python
), (c,
```
```python
3
```
```python
), (d,
```
```python
4
```
```python
))
```
## 3.定制排序规则
有如下结构的数据：
```python
<10 6094308
<100 234975
<20 2286079
<200 1336431
```
希望按照<后面的数字大小排序，得到如下结果：
```python
<10 6094308
<20 2286079
<100 234975
<200 1336431
```
代码如下：
```python
val array = Array((
```
```python
"<10"
```
```python
,
```
```python
6094308
```
```python
), (
```
```python
"<100"
```
```python
,
```
```python
234975
```
```python
), (
```
```python
"<20"
```
```python
,
```
```python
2286079
```
```python
),(
```
```python
"<200"
```
```python
,
```
```python
1336431
```
```python
))
```
```python
;
```
```python
sc
```
```python
.parallelize
```
```python
(array)
```
```python
.sortBy
```
```python
({item => item._1
```
```python
.substring
```
```python
(
```
```python
1
```
```python
, item._1
```
```python
.length
```
```python
)
```
```python
.toInt
```
```python
})
```
```python
.collect
```
```python
()
```
要理解上述代码的原理，我们需要分析一下`sortBy`的源码。
```python
/**
   * Return this RDD sorted by the given key function.
   */
```
```python
def
```
```python
sortBy[K](
      f: (T) => K,
      ascending: Boolean =
```
```python
true
```
```python
,
      numPartitions: Int =
```
```python
this
```
```python
.partitions.length)
      (implicit ord: Ordering[K], ctag: ClassTag[K]): RDD[T] = withScope {
```
```python
this
```
```python
.keyBy[K](f)
        .sortByKey(ascending, numPartitions)
        .values
  }
```
`sortBy`必需传入的一个参数为`f: (T) => K`，`T`为array中的元素类型。
```python
/**
   * Creates tuples of the elements
```
```python
in
```
```python
this RDD by applying `f`.
   */
```
```python
def
```
```python
keyBy
```
```python
[
```
```python
K
```
```python
]
```
```python
(f: T => K)
```
```python
:
```
```python
RDD[(K, T)] = withScope {
    val cleanedF = sc.clean(f)
    map(x => (cleanedF(x), x))
  }
```
传入的`f: (T) => K`作用在`keyBy`方法上，生成了一个`RDD[(K, T)]`的数据。
然后调用`sortByKey`，最后取出里面的`T`，得到的就是原始array中的类型！
## 4.用sortByKey实现上面的功能
我们再来看看`sortByKey`的源码
```python
/**
   * Sort the RDD by key, so that each partition contains a sorted range of the elements. Calling
   * `collect` or `save` on the resulting RDD will return or output an ordered list of records
   * (in the `save` case, they will be written to multiple `part-X` files in the filesystem, in
   * order of the keys).
   */
```
```python
// TODO: this currently doesn't work on P other than Tuple2!
```
```python
def
```
```python
sortByKey(ascending: Boolean =
```
```python
true
```
```python
, numPartitions: Int = self.partitions.length)
      : RDD[(K, V)] = self.withScope
  {
```
```python
val
```
```python
part =
```
```python
new
```
```python
RangePartitioner(numPartitions, self, ascending)
```
```python
new
```
```python
ShuffledRDD[K, V, V](self, part)
      .setKeyOrdering(
```
```python
if
```
```python
(ascending) ordering
```
```python
else
```
```python
ordering.reverse)
  }
```
大家看到`sortByKey`的源码可能会有疑惑，难道`sortByKey`不能指定排序方式么？不能像`sortBy`那样传入一个函数么？
其实是可以的。`sortByKey`位于`OrderedRDDFunctions`类中，`OrderedRDDFunctions`中有一个隐藏变量：
```python
private
```
```python
val ordering = implicitly[Ordering[K]]
```
我们重写这个变量以后，就可以改变排序规则。
以第三部分的需求为例，我们用`sortByKey`可以这么做：
```python
implicit val sortByNum = new Ordering[String] { override
```
```python
def
```
```python
compare
```
```python
(x: String, y: String)
```
```python
:
```
```python
Int = x.substring(
```
```python
1
```
```python
, x.length).toInt.compareTo(y.substring(
```
```python
1
```
```python
, y.length).toInt)};
val array = Array((
```
```python
"<10"
```
```python
,
```
```python
6094308
```
```python
), (
```
```python
"<100"
```
```python
,
```
```python
234975
```
```python
), (
```
```python
"<20"
```
```python
,
```
```python
2286079
```
```python
),(
```
```python
"<200"
```
```python
,
```
```python
1336431
```
```python
));
sc.parallelize(array).sortByKey().collect()
```
最后的输出结果为：
```python
Array
```
```python
[(
```
```python
String
```
```python
,
```
```python
Int
```
```python
)] =
```
```python
Array
```
```python
((<
```
```python
10
```
```python
,
```
```python
6094308
```
```python
), (<
```
```python
20
```
```python
,
```
```python
2286079
```
```python
), (<
```
```python
100
```
```python
,
```
```python
234975
```
```python
), (<
```
```python
200
```
```python
,
```
```python
1336431
```
```python
))
```
同样达到了我们的目的！

