# Scala  函数 - 按名称调用函数 - Simple 专栏 - CSDN博客
2018年09月15日 00:34:22[Simple_Zz](https://me.csdn.net/love284969214)阅读数：153
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
函数的参数是按值参数; 也就是说，参数的值在传递给函数之前确定。 但是，如果我们需要编写一个函数来接受一个表达式作为参数，我们不希望在函数调用之前进行评估怎么办？ 在这种情况下，可使用Scala中提供的名称参数。
一个按名称机制将代码块传递给调用，并且每次调用访问该参数时，代码块被执行并且该值被计算。 在这里，延迟打印一个消息，表明该方法已经输入。 接下来，延迟打印带有其值的消息。 最后，延迟返回 ' t ' 。
以下程序显示如何实现按名称调用函数。
```
object Demo {
   def main(args: Array[String]) {
        delayed(time());
   }
   def time() = {
      println("Getting time in nano seconds")
      System.nanoTime
   }
   def delayed( t: => Long ) = {
      println("In delayed method")
      println("Param: " + t)
   }
}
```
将上述程序保存在源文件：*Demo.scala* 中，使用以下命令编译和执行此程序。
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
In delayed method
Getting time in nano seconds
Param: 11812503856935
```
