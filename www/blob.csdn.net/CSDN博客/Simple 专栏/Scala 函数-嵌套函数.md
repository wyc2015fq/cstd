# Scala  函数 - 嵌套函数 - Simple 专栏 - CSDN博客
2018年09月15日 00:41:40[Simple_Zz](https://me.csdn.net/love284969214)阅读数：108
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala允许您定义函数内部的函数，而在其他函数中定义的函数称为局部函数。这是一个阶乘计算器的实现，我们使用传统的技术来调用第二个嵌套方法来完成工作。
尝试以下程序来了解如何实现嵌套函数。
**示例**
```
object Demo {
   def main(args: Array[String]) {
      println( factorial(0) )
      println( factorial(1) )
      println( factorial(2) )
      println( factorial(3) )
   }
   def factorial(i: Int): Int = {
      def fact(i: Int, accumulator: Int): Int = {
         if (i <= 1)
            accumulator
         else
            fact(i - 1, i * accumulator)
      }
      fact(i, 1)
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
1
1
2
6
```
像许多语言中的局部变量声明一样，嵌套方法仅在封闭方法中可见。如果您尝试在`factorial()`之外调用`fact()`，则会在编译器时出现错误。
