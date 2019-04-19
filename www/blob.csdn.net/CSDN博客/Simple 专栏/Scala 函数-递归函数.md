# Scala  函数 - 递归函数 - Simple 专栏 - CSDN博客
2018年09月15日 00:37:36[Simple_Zz](https://me.csdn.net/love284969214)阅读数：124
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
递归在纯功能编程中起着重要作用，Scala支持递归函数。 递归表示一个函数可以重复调用自身。
尝试以下程序，它是一个很好的递归示例，它计算给定参数(数字)的阶乘。
**示例**
```
object Demo {
   def main(args: Array[String]) {
      for (i <- 1 to 10)
         println( "Factorial of " + i + ": = " + factorial(i) )
   }
   def factorial(n: BigInt): BigInt = {  
      if (n <= 1)
         1  
      else    
      n * factorial(n - 1)
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
Factorial of 1: = 1
Factorial of 2: = 2
Factorial of 3: = 6
Factorial of 4: = 24
Factorial of 5: = 120
Factorial of 6: = 720
Factorial of 7: = 5040
Factorial of 8: = 40320
Factorial of 9: = 362880
Factorial of 10: = 3628800
```
