# Scala  函数 - 命名参数的函数 - Simple 专栏 - CSDN博客
2018年09月15日 00:35:37[Simple_Zz](https://me.csdn.net/love284969214)阅读数：72
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
在正常的函数调用中，调用的参数按照被调用函数定义的参数顺序逐个匹配。命名参数允许您以不同的顺序将参数传递给函数。语法只是每个参数前面都有一个参数名称和一个等号。
尝试以下程序，它是一个简单的例子来显示具有命名参数的函数。
```
object Demo {
   def main(args: Array[String]) {
      printInt(b = 5, a = 7);
   }
   def printInt( a:Int, b:Int ) = {
      println("Value of a : " + a );
      println("Value of b : " + b );
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
Value of a :  7
Value of b :  5
```
