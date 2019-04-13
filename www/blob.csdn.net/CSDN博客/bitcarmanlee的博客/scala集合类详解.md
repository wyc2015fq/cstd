
# scala集合类详解 - bitcarmanlee的博客 - CSDN博客


2017年05月28日 21:59:14[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：9280


对scala中的集合类虽然有使用，但是一直处于一知半解的状态。尤其是与java中各种集合类的混合使用，虽然用过很多次，但是一直也没有做比较深入的了解与分析。正好趁着最近项目的需要，加上稍微有点时间，特意多花了一点时间对scala中的集合类做个详细的总结。
## 1.数组Array
在说集合类之前，先看看scala中的数组。与Java中不同的是，Scala中没有数组这一种类型。在Scala中，Array类的功能就与数组类似。
与所有数组一样，Array的长度不可变，里面的数据可以按索引位置访问。
```python
def
```
```python
test() = {
```
```python
val
```
```python
array1 =
```
```python
new
```
```python
Array[Int](
```
```python
5
```
```python
)
    array1(
```
```python
1
```
```python
) =
```
```python
1
```
```python
println(array1(
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
array2 = Array(
```
```python
0
```
```python
,
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
)
    println(array2(
```
```python
3
```
```python
))
  }
```
上面的demo就演示了Array的简单用法。
## 2.集合类的大致结构
盗用网上的一张图，scala中集合类的大体框架如下图所示。
![这里写图片描述](https://img-blog.csdn.net/20170528205649796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
特意查了下scala的源码，贴上几张图，可以对应到上面的这幅继承关系图。
![这里写图片描述](https://img-blog.csdn.net/20170528210018311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
根据图以及源码可以很清晰地看出scala中的集合类可以分为三大类：
1.Seq，是一组有序的元素。
2.Set，是一组没有重复元素的集合。
3.Map，是一组k-v对。
## 3.Seq分析
Seq主要由两部分组成：IndexedSeq与LinearSeq。现在我们简单看下这两种类型。
首先看IndexedSeq，很容易看出来这种类型的主要访问方式是通过索引，默认的实现方式为vector。
```python
def test() = {
    val
```
```python
x
```
```python
= IndexedSe
```
```python
q(1,2,3)
```
```python
println(
```
```python
x
```
```python
.getClass)
    println(
```
```python
x
```
```python
(
```
```python
0
```
```python
))
    val
```
```python
y
```
```python
= Range(
```
```python
1
```
```python
,
```
```python
5
```
```python
)
    println(
```
```python
y
```
```python
)
  }
```
将以上函数运行起来以后，输出如下：
```python
class scala
```
```python
.collection
```
```python
.immutable
```
```python
.Vector
```
```python
1
```
```python
Range(
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
)
```
而作为LinearSeq，主要的区别在于其被分为头与尾两部分。其中，头是容器内的第一个元素，尾是除了头元素以外剩余的其他所有元素。LinearSeq默认的实现是List。
```python
def test() = {
    val
```
```python
x
```
```python
= collection.immutable.LinearSe
```
```python
q("a", "b", "c")
```
```python
val head =
```
```python
x
```
```python
.head
    println(
```
```python
s
```
```python
"head is:
```
```python
$head
```
```python
"
```
```python
)
    val
```
```python
y
```
```python
=
```
```python
x
```
```python
.tail
    println(
```
```python
s
```
```python
"tail of y is:
```
```python
$y
```
```python
"
```
```python
)
  }
```
将上面的代码运行起来以后，得到的结果如下：
```python
head
```
```python
is
```
```python
: a
tail
```
```python
of
```
```python
y
```
```python
is
```
```python
: List(b, c)
```
## 4.Set
与其他任何一种编程语言一样，Scala中的Set集合类具有如下特点：
1.不存在有重复的元素。
2.集合中的元素是无序的。换句话说，不能以索引的方式访问集合中的元素。
3.判断某一个元素在集合中比Seq类型的集合要快。
Scala中的集合分为可变与不可变两种，对于Set类型自然也是如此。先来看看示例代码：
```python
def
```
```python
test() = {
```
```python
val
```
```python
x = immutable.HashSet[String](
```
```python
"a"
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
"b"
```
```python
)
```
```python
//x.add("d")无法使用，因为是不可变集合，没有add方法。
```
```python
val
```
```python
y = x +
```
```python
"d"
```
```python
+
```
```python
"f"
```
```python
// 增加新的元素，生成一个新的集合
```
```python
val
```
```python
z = y -
```
```python
"a"
```
```python
// 删除一个元素，生成一个新的集合
```
```python
val
```
```python
a = Set(
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
)
```
```python
val
```
```python
b = Set(
```
```python
1
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
)
```
```python
val
```
```python
c = a ++ b
```
```python
// 生成一个新的集合，增加集合
```
```python
val
```
```python
d = a -- b
```
```python
// 生成一个新的集合，去除集合
```
```python
val
```
```python
e = a & b
```
```python
// 与操作
```
```python
val
```
```python
f = a | b
```
```python
// 或操作
```
```python
}
```
因为上面代码里的集合类型都是不可变类型，所以所有语句结果其实都是生成一个新的集合。
```python
def test() = {
    val x =
```
```python
new
```
```python
mutable.HashSet[String]()
    x +=
```
```python
"a"
```
```python
// 添加一个新的元素。注意此时没有生成一个新的集合
```
```python
x.
```
```python
add
```
```python
(
```
```python
"d"
```
```python
)
```
```python
//因为是可变集合，所以有add方法
```
```python
x ++= Set(
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
)
```
```python
// 添加一个新的集合
```
```python
x.foreach(
```
```python
each
```
```python
=> println(
```
```python
each
```
```python
))
    x -=
```
```python
"b"
```
```python
// 删除一个元素
```
```python
println()
    x.foreach(
```
```python
each
```
```python
=> println(
```
```python
each
```
```python
))
    println()
    val flag = x.
```
```python
contains
```
```python
(
```
```python
"a"
```
```python
)
```
```python
// 是否包含元素
```
```python
println(flag)
  }
```
将上面这段代码运行起来以后，得到的结果如下：
```python
c
d
```
```python
a
```
```python
b
c
d
```
```python
a
```
```python
true
```
## 5.Map
Map这种数据结构是日常开发中使用非常频繁的一种数据结构。Map作为一个存储键值对的容器（key－value），其中key值必须是唯一的。  默认情况下，我们可以通过Map直接创建一个不可变的Map容器对象，这时候容器中的内容是不能改变的。示例代码如下。
```python
def test() = {
    val peoples = Map(
```
```python
"john"
```
```python
->
```
```python
19
```
```python
,
```
```python
"Tracy"
```
```python
->
```
```python
18
```
```python
,
```
```python
"Lily"
```
```python
->
```
```python
20
```
```python
)
```
```python
//不可变
```
```python
// people.put("lucy",15) 会出错，因为是不可变集合。
```
```python
//遍历方式1
```
```python
for
```
```python
(p <- peoples) {
```
```python
print
```
```python
(p +
```
```python
"  "
```
```python
)
```
```python
// (john,19)  (Tracy,18)  (Lily,20)
```
```python
}
```
```python
//遍历方式2
```
```python
peoples.
```
```python
foreach
```
```python
(x => {val (k, v) = x;
```
```python
print
```
```python
(k +
```
```python
":"
```
```python
+ v +
```
```python
"  "
```
```python
)})
```
```python
//john:19  Tracy:18  Lily:20
```
```python
//遍历方式3
```
```python
peoples.
```
```python
foreach
```
```python
({
```
```python
case
```
```python
(k, v) =>
```
```python
print
```
```python
(s
```
```python
"key: $k, value: $v  "
```
```python
)})
```
```python
//key: john, value: 19  key: Tracy, value: 18  key: Lily, value: 20
```
```python
}
```
上面代码中的hashMap是不可变类型。
如果要使用可变类型的map，可以使用mutable包中的map相关类。
```python
def test() = {
    val
```
```python
map
```
```python
=
```
```python
new
```
```python
mutable
```
```python
.HashMap[String, Int]()
```
```python
map
```
```python
.put(
```
```python
"john"
```
```python
,
```
```python
19
```
```python
)
```
```python
// 因为是可变集合，所以可以put
```
```python
map
```
```python
.put(
```
```python
"Tracy"
```
```python
,
```
```python
18
```
```python
)
```
```python
map
```
```python
.contains(
```
```python
"Lily"
```
```python
)
```
```python
//false
```
```python
val res = getSome(
```
```python
map
```
```python
.get(
```
```python
"john"
```
```python
))
    println(res)
```
```python
//Some(19)
```
```python
}
  def getSome(x:Option[Int]) : Any = {
    x match {
```
```python
case
```
```python
Some(s) => s
```
```python
case
```
```python
None =>
```
```python
"None"
```
```python
}
  }
```
## 6.可变数组ArrayBuffer
特意将ArrayBuffer单独拎出来，是因为ArrayBuffer类似于Java中的ArrayList。而ArrayList在Java中是用得非常多的一种集合类。
ArrayBuffer与ArrayList不一样的地方在于，ArrayBuffer的长度是可变的。与Array一样，元素有先后之分，可以重复，可以随机访问，但是插入的效率不高。
```python
def test() = {
```
```python
val
```
```python
arrayBuffer =
```
```python
new
```
```python
mutable
```
```python
.ArrayBuffer[Int]()
    arrayBuffer.append(
```
```python
1
```
```python
)
```
```python
//后面添加元素
```
```python
arrayBuffer.append(
```
```python
2
```
```python
)
    arrayBuffer +=
```
```python
3
```
```python
//后面添加元素
```
```python
4
```
```python
+=: arrayBuffer
```
```python
//前面添加元素
```
```python
}
```
## 7.java与scala集合的相互转换
scala最大的优势之一就是可以使用JDK上面的海量类库。实际项目中，经常需要在java集合类与scala集合类之间做转化。具体的转换对应关系如下：
scala.collection.Iterable <=> Java.lang.Iterable
scala.collection.Iterable <=> Java.util.Collection
scala.collection.Iterator <=> java.util.{ Iterator, Enumeration }
scala.collection.mutable.Buffer <=> java.util.List
scala.collection.mutable.Set <=> java.util.Set
scala.collection.mutable.Map <=> java.util.{ Map, Dictionary }
scala.collection.mutable.ConcurrentMap <=> java.util.concurrent.ConcurrentMap
scala.collection.Seq         => java.util.List
scala.collection.mutable.Seq => java.util.List
scala.collection.Set         => java.util.Set
scala.collection.Map         => java.util.Map
java.util.Properties         => scala.collection.mutable.Map[String, String]
在使用这些转换的时候，只需要scala文件中引入scala.collection.JavaConversions._  即可。
一般比较多件的场景是在scala中调用java方法。如前面所讲，jdk的类库太丰富了，在scala中会经常有调用java方法的需求。给个简单的例子：
假设有如下java代码：
```python
public
```
```python
class
```
```python
TestForScala {
```
```python
public
```
```python
static
```
```python
<T>
```
```python
void
```
```python
printCollection
```
```python
(List<T> list) {
```
```python
for
```
```python
(T t: list) {
            System.
```
```python
out
```
```python
.println(t);
        }
    }
}
```
我们想在scala代码中调用TestForScala类中的printCollection方法。可以这么写：
```python
def
```
```python
test() = {
```
```python
val
```
```python
raw = Vector(
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
)
    TestForScala.printCollection(raw)
  }
```
java方法中需要的参数是个List，参照我们前面的转换关系，scala.collection.Seq可以自动转化为java中的List，而Vector就是scala中Seq的实现，所以可以直接传入到printCollection方法中！

