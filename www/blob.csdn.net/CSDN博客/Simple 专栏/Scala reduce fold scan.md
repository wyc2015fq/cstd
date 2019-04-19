# Scala reduce fold scan - Simple 专栏 - CSDN博客
2018年12月28日 18:42:30[Simple_Zz](https://me.csdn.net/love284969214)阅读数：47
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
# reduce 化简
**将二元函数引用于集合中的函数。**
```
// 源码 reduce
def reduce[A1 >: A](op: (A1, A1) => A1): A1 = reduceLeft(op)
// 源码 reduceLeft
override /*TraversableLike*/
def reduceLeft[B >: A](f: (B, A) => B): B =
if (isEmpty) throw new UnsupportedOperationException("empty.reduceLeft")
else tail.foldLeft[B](head)(f)
// 源码 reduceRight
override /*IterableLike*/
def reduceRight[B >: A](op: (A, B) => B): B =
if (isEmpty) throw new UnsupportedOperationException("Nil.reduceRight")
else if (tail.isEmpty) head
else op(head, tail.reduceRight(op))
```
需求：val list = List(1, 2, 3, 4 ,5) , 求出list的和。
分析：上面的问题当然可以使用遍历list方法来解决，这里我们使用scala的化简(reduce)方式来完成。
```
val list = List(1, 2, 3, 4, 5)
//sum 返回两个数的和
def sum(num1: Int, num2: Int): Int = {
  num1 + num2
}
//分析执行流程. 1, 2, 3, 4, 5
//1.(1,2),3,4,5
//2.(3,3),4,5
//3.(6,4),5
//4.(10,5)
//5.15
//((((1, 2), 3, 4), 5)
println(list.reduce(sum))   // 15
//((((1, 2), 3, 4), 5)
println(list.reduceLeft(sum))   // 15
//(1, (20, (30, (4, 5))))
println(list.reduceRight(sum))  // 15
```
reduceLeft()这个函数的执行逻辑如图
![](https://img-blog.csdnimg.cn/20181228120006740.png)
reduceRight() 和上面一样，只是从右边开始进行简化。
# fold 折叠
fold函数将上一步返回的值作为函数的第一个参数继续传递参与运算，直到list中的所有元素被遍历。
```
// 源码 fold
def fold[A1 >: A](z: A1)(op: (A1, A1) => A1): A1 = foldLeft(z)(op)
// 源码 foldLeft
override /*TraversableLike*/
def foldLeft[B](z: B)(@deprecatedName('f) op: (B, A) => B): B = {
  var acc = z
  var these = this
  while (!these.isEmpty) {
    acc = op(acc, these.head)
    these = these.tail
  }
  acc
}
// 源码 foldRight
override /*IterableLike*/
def foldRight[B](z: B)(@deprecatedName('f) op: (A, B) => B): B =
if (this.isEmpty) z
else op(head, tail.foldRight(z)(op))
```
- 可以把 reduceLeft 看做简化版的 foldLeft。
- 可以看到 reduceLeft 就是调用的 foldLeft[B](head)，并且是默认从计划的**head****元素**开始操作的。
```
// 折叠
val list = List(1, 2, 3, 4)
def minus( num1 : Int, num2 : Int ): Int = {
  num1 - num2
}
// ((((5,1),2),3),4)
println(list.fold(5)(minus))  // -5
// ((((5,1),2),3),4)
println(list.foldLeft(5)(minus))  // -5
// (1,(2,(3,(4,5))))
println(list.foldRight(5)(minus)) // 3
// (20)(1,2,3,4)
println((20 /: list)(minus))  // 10
// (1,2,3,4)(20)
println((list :\ 20 )(minus)) // 18
```
# scan 扫描
扫描，即对某个集合的所有元素做 fold 操作，但是会把**产生的所有中间结果**放置于一个**集合**中保存。
```
// 源码 scan
def scan[B >: A, That](z: B)(op: (B, B) => B)(implicit cbf: CanBuildFrom[Repr, B, That]): That = scanLeft(z)(op)
// 源码 scanLeft
def scanLeft[B, That](z: B)(op: (B, A) => B)(implicit bf: CanBuildFrom[Repr, B, That]): That = {
  val b = bf(repr)
  b.sizeHint(this, 1)
  var acc = z
  b += acc
  for (x <- this) { acc = op(acc, x); b += acc }
  b.result
}
// 源码 scanRight
def scanRight[B, That](z: B)(op: (A, B) => B)(implicit bf: CanBuildFrom[Repr, B, That]): That = {
  var scanned = List(z)
  var acc = z
  for (x <- reversed) {
    acc = op(x, acc)
    scanned ::= acc
  }
  val b = bf(repr)
  for (elem <- scanned) b += elem
  b.result
}
```
```
val list = List(1, 2, 3, 4, 5)
def minus(num1: Int, num2: Int): Int = {
  num1 - num2
}
// 5 (1, 2, 3, 4, 5) => (5, 4, 2, -1, -5, -10)
println(list.scan(5)(minus))
// 5 (1, 2, 3, 4, 5) => (5, 4, 2, -1, -5, -10)
println(list.scanLeft(5)(minus))
// (1, 2, 3, 4, 5) 5 => (-2, 3, -1, 4, 0, 5)
println(list.scanRight(5)(minus))
```
