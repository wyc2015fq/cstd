# Scala flatMap filter - Simple 专栏 - CSDN博客
2018年11月20日 10:21:59[Simple_Zz](https://me.csdn.net/love284969214)阅读数：119
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
### flatMap 与 filter 这两个都是高阶函数，参数均接收一个函数。
### flatMap：flat即压扁，压平，扁平化。
### 效果就是将集合中的每个元素的子元素映射到某个函数并返回新的集合。
```
object testFlatMap {
  def main(args: Array[String]): Unit = {
    val names = List("About", "Box", "Clear")
    println(names.map(upper))
    println(names.flatMap(upper))
  }
  def upper(s: String): String = {
    s.toUpperCase()
  }
}
// 输出
List(ABOUT, BOX, CLEAR)
List(A, B, O, U, T, B, O, X, C, L, E, A, R)
```
### filter：过滤
### 效果就是将符合要求的数据(筛选)放置到新的集合。
```
object testFilter {
  def main(args: Array[String]): Unit = {
    val names = List("About", "Box", "Clear")
    println(names.filter(startA))
  }
  def startA(str: String): Boolean = {
    str.startsWith("A")
  }
}
// 输出
List(About)
```
