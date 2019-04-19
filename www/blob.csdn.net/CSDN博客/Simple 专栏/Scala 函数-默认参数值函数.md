# Scala  函数 - 默认参数值函数 - Simple 专栏 - CSDN博客
2018年09月15日 00:38:20[Simple_Zz](https://me.csdn.net/love284969214)阅读数：380
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala允许您指定函数参数的默认值。 这样一个参数可以从函数调用中选择性地省略，在这种情况下，相应的参数值将使用默认值。如果指定其中一个参数，则使用该参数将传递第一个参数，第二个参数将从默认值中获取。
尝试以下示例，它是为函数指定默认参数的示例 -
**示例**
```
object Demo {
   def main(args: Array[String]) {
      println( "Returned Value : " + addInt() );
   }
   def addInt( a:Int = 5, b:Int = 7 ) : Int = {
      var sum:Int = 0
      sum = a + b
      return sum
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
Returned Value : 12
```
