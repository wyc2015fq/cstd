
# scala中sorted,sortWith,sortBy用法详解 - bitcarmanlee的博客 - CSDN博客


2017年07月23日 23:07:51[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：12367


scala的集合中提供了三种排序的方式：sorted,sortWith,sortBy。那么这三种方式有什么不同呢？下面我们结合源码来分析一下
## 1.sorted
先来看看scala中sorted的源码。
```python
def
```
```python
sorted
```
```python
[
```
```python
B
```
```python
>:
```
```python
A](implicit ord: Ordering[B]): Repr = {
    val len = this.length
    val arr = new ArraySeq[A](len)
    var i =
```
```python
0
```
```python
for
```
```python
(x <- this.seq) {
      arr(i) = x
      i +=
```
```python
1
```
```python
}
    java.util.Arrays.sort(arr.array, ord.asInstanceOf[Ordering[Object]])
    val b = newBuilder
    b.sizeHint(len)
```
```python
for
```
```python
(x <- arr) b += x
    b.result
  }
```
源码中有两点值得注意的地方：
1.sorted方法中有个隐式参数ord: Ordering。
2.sorted方法真正排序的逻辑是调用的java.util.Arrays.sort。
## 2.sortBy
看看sortBy的源码，很简单。
```python
def
```
```python
sortBy
```
```python
[
```
```python
B
```
```python
]
```
```python
(f: A => B)
```
```python
(implicit ord: Ordering[B])
```
```python
:
```
```python
Repr = sorted(ord on f)
```
sortBy最后也是调用的sorted方法。不一样的地方在于，sortBy前面还需要提供一个属性。
## 3.sortWith
sortWith的源码如下。
```python
def
```
```python
sortWith
```
```python
(lt:
```
```python
(A, A)
```
```python
=> Boolean)
```
```python
:
```
```python
Repr = sorted(Ordering fromLessThan lt)
```
跟前面两个不同的是，sortWith需要传入一个比较函数用来比较！
## 4.实例
理论部分说完了，下面来干货
```python
object ImplicitValue {
    implicit val KeyOrdering = new Ordering[String] {
        override
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
Int = {
            y.compareTo(x)
        }
    }
}
```
首先定义了一个隐式比较器。
```python
def
```
```python
test1() = {
```
```python
import
```
```python
ImplicitValue.KeyOrdering
```
```python
val
```
```python
list = List(
```
```python
"a"
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
"F"
```
```python
,
```
```python
"B"
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
val
```
```python
sortedList = list.sorted
        println(sortedList)
```
```python
// List(g, c, a, F, B)
```
```python
}
```
注意因为我们将隐式比较器import了进来，这个时候sorted排序的规则是按照我们自定义的比较器进行比较。在我们自定义的比较器中，定义的是按字符串逆序，所以最终的输出结果为字符串按从大到小的顺序排列！
再来看看sortWith的用法。
```python
//
```
```python
忽略大小写排序
    def compareIngoreUpperCase(
```
```python
e1
```
```python
: String,
```
```python
e2
```
```python
: String) : Boolean = {
        e1.toLowerCase < e2.toLowerCase
    }
    def test2() = {
        val list = List(
```
```python
"a"
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
"F"
```
```python
,
```
```python
"B"
```
```python
,
```
```python
"c"
```
```python
)
        val sortWithList1 = list.sortWith(_ < _)
```
```python
//
```
```python
List(B, F, a, c, g)
        val sortwithList2 = list.sortWith
```
```python
((left, right) => left < right)
```
```python
//
```
```python
List
```
```python
(B, F, a, c, g)
```
```python
val
```
```python
sortwithList3
```
```python
=
```
```python
list
```
```python
.
```
```python
sortWith
```
```python
(compareIngoreUpperCase)
```
```python
//
```
```python
List
```
```python
(a, B, c, F, g)
```
```python
println
```
```python
(sortWithList1)
```
```python
println
```
```python
(sortwithList2)
```
```python
println
```
```python
(sortwithList3)
```
```python
}
```
本例中， sortWithList1与sortWithList2最终的结果是一致的，只不过写法不一样而已，都是按字符串从小到大的顺序排列。sortwithList3则是按照传入的compareIngoreUpperCase函数进行排序！
最后看看sortBy的代码
```python
def test3() = {
        val m = Map(
            -
```
```python
2
```
```python
->
```
```python
5
```
```python
,
```
```python
2
```
```python
->
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
->
```
```python
9
```
```python
,
```
```python
1
```
```python
->
```
```python
2
```
```python
,
```
```python
0
```
```python
->
```
```python
-
```
```python
16
```
```python
,
            -
```
```python
1
```
```python
->
```
```python
-
```
```python
4
```
```python
)
```
```python
//
```
```python
按key排序
        m.toList.sorted.foreach{
```
```python
case
```
```python
(key, value)
```
```python
=>
```
```python
println(key +
```
```python
":"
```
```python
+ value)
        }
        println
```
```python
//
```
```python
按value排序
        m.toList.sortBy(_._2).foreach {
```
```python
case
```
```python
(key, value)
```
```python
=>
```
```python
println(key +
```
```python
":"
```
```python
+ value)
        }
    }
```
最后的输出结果为：
```python
-2
```
```python
:5
```
```python
-1
```
```python
:-4
```
```python
0
```
```python
:-16
```
```python
1
```
```python
:2
```
```python
2
```
```python
:6
```
```python
5
```
```python
:9
```
```python
0
```
```python
:-16
```
```python
-1
```
```python
:-4
```
```python
1
```
```python
:2
```
```python
-2
```
```python
:5
```
```python
2
```
```python
:6
```
```python
5
```
```python
:9
```

