
# scala 偏函数与 map-collect - Zhang's Wikipedia - CSDN博客


2019年01月30日 19:37:35[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：132个人分类：[Hadoop-Scala-Spark](https://blog.csdn.net/lanchunhui/article/category/6141083)



[https://fangjian0423.github.io/2015/06/14/scala-partial/](https://fangjian0423.github.io/2015/06/14/scala-partial/)
[https://www.jianshu.com/p/fa2ed7ed391e](https://www.jianshu.com/p/fa2ed7ed391e)
## 0. collect 与 map 的区别
由于collect方法接收的是一个偏函数类型，所以它并不能接收一个lambda表达式：
scala> List(1, 3, 5, "seven").collect(i => i + 1)
error: type mismatch;
scala> List(1, 3, 5, "seven").collect{case i => i + 1}
error: type mismatch;
scala> List(1, 3, 5, "seven").collect{case i:Int => i + 1}
List[Int] = List(2, 4, 6)我们在使用collect时，可以利用偏函数的原理，同时实现filter与map的特性。例如：
List(1, 2, 3, 4, 5, 6) collect { case i if i % 2 == 0 => i * i }
// 等价于
List(1, 2, 3, 4, 5, 6).filter(i => i % 2 == 0).map(i => i * i)
## 1. 普通函数与偏函数
普通函数的定义中，有指定的输入类型，所以可以接受任意该类型下的值。比如一个(Int) => String的函数可以接收任意Int值，并返回一个字符串。
偏函数的定义中，也有指定的输入类型，但是偏函数不接受所有该类型下的值。比诶如(Int) => String的偏函数不能接收所有Int值为输入（通过 case 进行指定）。
def one:PartialFunction[Int, String] = {
    case 1 => "one"
}
one(1) // "one"
one(2) // 报错
def two: PartialFunction[Int, String] = {
    case 2 => "two"
}
two(1) // 报错
two(2) // "two"
def wildcard: PartialFunction[Int, String] = {
    case _ => "something else"
}
wildcard(1) // "something else"
wildcard(2) // "something else"
由于偏函数只会接收部分参数，所以可以使用 “orElse” 方法进行组合：
val partial = one orElse two orElse wildcard
partial(1) // "one"
partial(2) // "two"
partial(3) // "something else"
## 2.  偏函数常见成员
orElse
isDefinedAt：判断偏函数是否对参数中的参数有效
one.isDefinedAt(1) // true
one.isDefinedAt(2) // false
case语句其实是偏函数定义的语法糖，当我们编写一个case语句时，其实等同于创建了一个具有apply与isDefineAt方法的偏函数对象。
当我们使用PartialFunction类型定义一个偏函数的时候，scala会被自动转换：
def int2Char: PartialFunction[Int, Char] = {
    case 1 => 'a'
    case 3 => 'c'
}会被转化为：
val int2Char = new PartialFunction[Int, Char] {
    def apply(i: Int) = {
        case 1 => 'a'
        case 3 => 'c'
    }
    def isDefinedAt(i: Int): Boolean = i match {
        case 1 => true
        case 3 => true
        case _ => false
    }
}
## 3. map/collect 的区别
map 与 collect 等价的情况：
val languageToCount = Map("Scala" -> 10, "Java" -> 20, "Ruby" -> 5)
languageToCount map { case (_, count) => count + 1 }
languageToCount collect { case (_, count) => count + 1 }
collect 运行正常，map 运行失败：
List(1, 3, 5, "seven") map { case i: Int => i + 1 } //won't work
//scala.MatchError: seven (of class java.lang.String)
List(1, 3, 5, "seven") collect { case i: Int => i + 1 } //it works
map 与 collect 的 api：
def map[B](f: (A) ⇒ B): List[B]
def collect[B](pf: PartialFunction[A, B]): List[B]两个方法的定义如出一辙，区别就在于前者接收的是一个函数类型的参数，而后者接收的是一个偏函数（partial function）类型的参数：
map: Builds a new collection by applying a function to all elements of this list.
colect: Builds a new collection by applying a partial function to all elements of this list on which the function is defined.
我们可以对比map方法和collect方法的实现：
def map[B, That](f: A => B)(implicit bf: CanBuildFrom[Repr, B, That]): That = {
  def builder = {
      val b = bf(repr)
      b.sizeHint(this)
      b
  }
  val b = builder  
  for (x <- this) b += f(x)  
  b.result
}
def collect[B, That](pf: PartialFunction[A, B])(implicit bf: CanBuildFrom[Repr, B, That]): That = {
   val b = bf(repr)
   for (x <- this) if (pf.isDefinedAt(x)) b += pf(x)  
   b.result
}在调用map方法时，一旦遍历到值"seven"，并调用f(x)，因为类型不符合模式匹配中的Int类型，导致抛出MatchError错误。而collect方法在调用pf(x)之前，调用了pf的isDefinedAt(x)作了一次过滤。


