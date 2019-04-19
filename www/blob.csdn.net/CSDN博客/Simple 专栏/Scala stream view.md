# Scala stream view - Simple 专栏 - CSDN博客
2018年12月30日 18:43:32[Simple_Zz](https://me.csdn.net/love284969214)阅读数：41标签：[Scala																[大数据																[编程语言](https://so.csdn.net/so/search/s.do?q=编程语言&t=blog)](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
# Stream是一个集合
**这个集合，可以用于存放，无穷多个元素，但是这无穷个元素并不会一次性生产出来，而是需要用到多大的区间，就会动态的生产，末尾元素遵循lazy规则(即：要使用结果才进行计算的) 。**
**1)  ****使用#::****得到一个stream**
def numsForm(n: BigInt) : Stream[BigInt] = n #:: numsForm(n + 1)
**2)  ****传递一个值，并打印stream****集合**
val tenOrMore = numsForm(10)
println(tenOrMore)
**3)  tail****的每一次使用，都会动态的向stream****集合按照规则生成新的元素**
println(tenOrMore.tail)
println(tenOrMore)
**4)  ****使用map****映射stream****的元素并进行一些计算**
println(numsForm(5).map(x => x * x))
```
def main(args: Array[String]): Unit = {
  //对下面的函数说明
  //Stream 集合存放的数据类型是BigInt
  //numsForm 是自定义的一个函数，函数名是程序员指定的。
  //创建的集合的第一个元素是 n , 后续元素生成的规则是 n + 1
  //后续元素生成的规则是可以程序员指定的 ，比如 numsForm( n * 4)...
  def numsForm(n: BigInt) : Stream[BigInt] = n #:: numsForm(n + 1)
  val stream1 = numsForm(1)
  println("stream1=" + stream1)
  //我希望在取一个流集合数据
  println(stream1.tail) //(2,?)
  println("stream1=" + stream1) // (1,2,?)
  println("stream1.head=" + stream1.head)
  //println("stream1.last" + stream1.last) //死循环
}
```
# View是一个集合
Stream的懒加载特性，也可以对其他集合应用view方法来得到类似的效果。
具有如下特点：
**view****方法产出一个总是被懒执行的集合**。
view不会缓存数据，每次都要重新计算。
```
def main(args: Array[String]): Unit = {
  //函数 将一个数字，原封不动的返回
  def multiple(num: Int): Int = {
    num
  }
  //判断一个数字，交换顺序后，是否相等
  def eq(i: Int): Boolean = {
    i.toString.equals(i.toString.reverse)
  }
  //说明: 没有使用view
  val viewSquares1 = (1 to 100)
    .map(multiple)
    .filter(eq)
  println(viewSquares1) //（1,2,3，。。。。11,22）
  //说明: 使用view
  val viewSquares2 = (1 to 100)
    .view
    .map(multiple)
    .filter(eq)
  println("viewSquares2=" + viewSquares2) //（SeqView）lazy
  for (item <- viewSquares2) {
    print(item + ",")
  }
}
```
