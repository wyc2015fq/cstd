# Scala  函数 - 高阶函数 - Simple 专栏 - CSDN博客
2018年09月15日 00:40:42[Simple_Zz](https://me.csdn.net/love284969214)阅读数：45
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala允许定义高阶函数。它是将其他函数作为参数或其结果是函数的函数。
尝试以下示例程序，`apply()`函数接受另一个函数`f`和值`v`，并将函数`f`应用于`v`。
**示例**
```
object Demo {
   def main(args: Array[String]) {
      println( apply( layout, 10) )
   }
   def apply(f: Int => String, v: Int) = f(v)
   def layout[A](x: A) = "[" + x.toString() + "]"
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
[10]
```
