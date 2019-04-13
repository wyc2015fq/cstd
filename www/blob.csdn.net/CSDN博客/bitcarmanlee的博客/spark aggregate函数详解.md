
# spark aggregate函数详解 - bitcarmanlee的博客 - CSDN博客


2017年09月25日 18:53:39[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：5797


aggregate算是spark中比较常用的一个函数，理解起来会比较费劲一些，现在通过几个详细的例子带大家来着重理解一下aggregate的用法。
## 1.先看看aggregate的函数签名
在spark的源码中，可以看到aggregate函数的签名如下：
```python
def aggregate[
```
```python
U: ClassTag
```
```python
](
```
```python
zeroValue: U
```
```python
)(seqOp: (U, T) => U, combOp: (U, U) => U): U
```
可以看出，这个函数是个柯里化的方法，输入参数分为了两部分：`(zeroValue: U)`与`(seqOp: (U, T) => U, combOp: (U, U) => U)`
## 2.aggregate的用法
函数签名比较复杂，可能有的小伙伴看着就晕菜了。别捉急，我们再来看看函数前面的注释，关于此函数的用法我们就会比较清楚。
```python
/**
   * Aggregate
```
```python
the
```
```python
elements
```
```python
of
```
```python
each partition,
```
```python
and
```
```python
then
```
```python
the
```
```python
results
```
```python
for
```
```python
all
```
```python
the
```
```python
partitions, using
   *
```
```python
given
```
```python
combine functions
```
```python
and
```
```python
a neutral
```
```python
"zero value"
```
```python
. This function can
```
```python
return
```
```python
a different
```
```python
result
```
```python
* type, U, than
```
```python
the
```
```python
type
```
```python
of
```
```python
this RDD, T. Thus, we need one operation
```
```python
for
```
```python
merging a T
```
```python
into
```
```python
an U
   *
```
```python
and
```
```python
one operation
```
```python
for
```
```python
merging two U's,
```
```python
as
```
```python
in
```
```python
scala.TraversableOnce. Both
```
```python
of
```
```python
these functions are
   * allowed
```
```python
to
```
```python
modify
```
```python
and
```
```python
return
```
```python
their
```
```python
first
```
```python
argument
```
```python
instead of
```
```python
creating a new U
```
```python
to
```
```python
avoid memory
   * allocation.
   *
   * @param zeroValue
```
```python
the
```
```python
initial value
```
```python
for
```
```python
the
```
```python
accumulated
```
```python
result
```
```python
of
```
```python
each partition
```
```python
for
```
```python
the
```
```python
*                  `seqOp` operator,
```
```python
and
```
```python
also
```
```python
the
```
```python
initial value
```
```python
for
```
```python
the
```
```python
combine results
```
```python
from
```
```python
*                  different partitions
```
```python
for
```
```python
the
```
```python
`combOp` operator - this will typically be
```
```python
the
```
```python
*                  neutral element (e.g. `Nil`
```
```python
for
```
```python
list
```
```python
concatenation
```
```python
or
```
```python
`
```
```python
0
```
```python
`
```
```python
for
```
```python
summation)
   * @param seqOp an operator used
```
```python
to
```
```python
accumulate results within a partition
   * @param combOp an associative operator used
```
```python
to
```
```python
combine results
```
```python
from
```
```python
different partitions
   */
```
翻译过来就是：aggregate先对每个分区的元素做聚集，然后对所有分区的结果做聚集，聚集过程中，使用的是给定的聚集函数以及初始值”zero value”。这个函数能返回一个与原始RDD不同的类型U，因此，需要一个合并RDD类型T到结果类型U的函数，还需要一个合并类型U的函数。这两个函数都可以修改和返回他们的第一个参数，而不是重新新建一个U类型的参数以避免重新分配内存。
参数zeroValue：`seqOp`运算符的每个分区的累积结果的初始值以及`combOp`运算符的不同分区的组合结果的初始值 - 这通常将是初始元素（例如“Nil”表的列表 连接或“0”表示求和）
参数seqOp： 每个分区累积结果的聚集函数。
参数combOp： 一个关联运算符用于组合不同分区的结果
## 3.求平均值
看来了上面的原理介绍，接下来我们看干货。
首先可以看网上最多的一个例子：
```python
val list =
```
```python
List
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
,
```
```python
7
```
```python
,
```
```python
8
```
```python
,
```
```python
9
```
```python
)
```
```python
val
```
```python
(mul, sum, count)
```
```python
=
```
```python
sc
```
```python
.
```
```python
parallelize
```
```python
(list,
```
```python
2
```
```python
)
```
```python
.
```
```python
aggregate
```
```python
((
```
```python
1
```
```python
,
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
)
```
```python
)
```
```python
(
    (acc, number)
```
```python
=>
```
```python
(acc.
```
```python
_
```
```python
1 * number, acc.
```
```python
_
```
```python
2 + number, acc.
```
```python
_
```
```python
3 +
```
```python
1
```
```python
)
```
```python
,
```
```python
(x, y)
```
```python
=>
```
```python
(x.
```
```python
_
```
```python
1 * y.
```
```python
_
```
```python
1, x.
```
```python
_
```
```python
2 + y.
```
```python
_
```
```python
2, x.
```
```python
_
```
```python
3 + y.
```
```python
_
```
```python
3)
```
```python
)
```
```python
(sum / count, mul)
```
在常见的求均值的基础上稍作了变动，sum是求和，count是累积元素的个数，mul是求各元素的乘积。
解释一下具体过程：
1.初始值是(1, 0 ,0)
2.number是函数中的T，也就是List中的元素，此时类型为Int。而acc的类型为(Int, Int, Int)。acc._1 * num是各元素相乘(初始值为1)，acc._2 + number为各元素相加。
3.sum / count为计算平均数。
## 4.另外的例子
为了加深理解，看另外一个的例子。
```python
val raw = List(
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
"d"
```
```python
,
```
```python
"f"
```
```python
,
```
```python
"g"
```
```python
,
```
```python
"h"
```
```python
,
```
```python
"o"
```
```python
,
```
```python
"q"
```
```python
,
```
```python
"x"
```
```python
,
```
```python
"y"
```
```python
)
        val (biggerthanf, lessthanf) = sc.parallelize(raw,
```
```python
1
```
```python
).aggregate((
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
))(
```
```python
(cc, str)
```
```python
=>
```
```python
{
```
```python
var
```
```python
biggerf = cc._1
```
```python
var
```
```python
lessf = cc._2
```
```python
if
```
```python
(str.compareTo(
```
```python
"f"
```
```python
) >=
```
```python
0
```
```python
) biggerf = cc._1 +
```
```python
1
```
```python
else
```
```python
if
```
```python
(str.compareTo(
```
```python
"f"
```
```python
) <
```
```python
0
```
```python
) lessf = cc._2 +
```
```python
1
```
```python
(biggerf, lessf)
            },
```
```python
(x, y)
```
```python
=>
```
```python
(x._1 + y._1, x._2 + y._2)
        )
```
这个例子中，我们想做的就是统计一下在raw这个list中，比”f”大与比”f”小的元素分别有多少个。代码本身的逻辑也比较简单，就不再更多解释。
## 5.aggregateByKey与combineByKey的比较
aggregate是针对序列的操作，aggregateByKey则是针对k,v对的操作。顾名思义，aggregateByKey则是针对key做aggregate操作。spark中函数的原型如下：
```python
def aggregateByKey[
```
```python
U: ClassTag
```
```python
](
```
```python
zeroValue: U
```
```python
)(seqOp: (U, V) => U,
```
```python
combOp: (U, U) => U): RDD[(K, U)] = self.withScope {
```
```python
aggregateByKey(zeroValue, defaultPartitioner(self))(seqOp, combOp)
```
```python
}
```
都是针对k,v对的操作，spark中还有一个combineByKey的操作：
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
为了看清楚两个的联系，我们再看看 aggregateByKey里面的真正实现：
```python
def aggregateByKey[
```
```python
U
```
```python
: ClassTag]
```
```python
(zeroValue: U, partitioner: Partitioner)(seqOp: (U, V) => U,
      combOp: (U, U) => U)
```
```python
:
```
```python
RDD
```
```python
[
```
```python
(K, U)
```
```python
] =
```
```python
self
```
```python
.
```
```python
withScope
```
```python
{
    //
```
```python
Serialize
```
```python
the
```
```python
zero
```
```python
value
```
```python
to
```
```python
a
```
```python
byte
```
```python
array
```
```python
so
```
```python
that
```
```python
we
```
```python
can
```
```python
get
```
```python
a
```
```python
new
```
```python
clone
```
```python
of
```
```python
it
```
```python
on
```
```python
each
```
```python
key
```
```python
val
```
```python
zeroBuffer
```
```python
=
```
```python
SparkEnv
```
```python
.
```
```python
get
```
```python
.
```
```python
serializer
```
```python
.
```
```python
newInstance
```
```python
()
```
```python
.
```
```python
serialize
```
```python
(zeroValue)
```
```python
val
```
```python
zeroArray
```
```python
=
```
```python
new
```
```python
Array
```
```python
[
```
```python
Byte
```
```python
]
```
```python
(zeroBuffer.limit)
```
```python
zeroBuffer
```
```python
.
```
```python
get
```
```python
(zeroArray)
```
```python
lazy
```
```python
val
```
```python
cachedSerializer
```
```python
=
```
```python
SparkEnv
```
```python
.
```
```python
get
```
```python
.
```
```python
serializer
```
```python
.
```
```python
newInstance
```
```python
()
```
```python
val
```
```python
createZero
```
```python
=
```
```python
()
```
```python
=>
```
```python
cachedSerializer.deserialize[U](ByteBuffer.wrap(zeroArray))
```
```python
//
```
```python
We will clean the combiner closure later
```
```python
in
```
```python
`
```
```python
combineByKey
```
```python
`
    val cleanedSeqOp = self.context.clean(seqOp)
    combineByKeyWithClassTag[U]
```
```python
((v: V) => cleanedSeqOp(createZero(), v),
      cleanedSeqOp, combOp, partitioner)
```
```python
}
```
从上面这段源码可以清晰看出，aggregateByKey调用的就是combineByKey方法。seqOp方法就是mergeValue，combOp方法则是mergeCombiners，cleanedSeqOp(createZero(), v)是createCombiner, 也就是传入的seqOp函数, 只不过其中一个值是传入的zeroValue而已！
因此, 当createCombiner和mergeValue函数的操作相同, aggregateByKey更为合适！

