# Scala  循环语句 - Simple 专栏 - CSDN博客
2018年09月14日 17:05:53[Simple_Zz](https://me.csdn.net/love284969214)阅读数：58
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
本章将介绍Scala编程语言中的循环控制结构。
可能会有一种情况，当你需要执行一段代码多次。 一般来说，语句依次执行：函数中的第一个语句先执行，后跟第二个语句，依此类推。
编程语言提供了允许更复杂的执行路径的各种控制结构。
循环语句允许我们多次执行一个语句或一组语句，以下是大多数编程语言中循环语句的一般形式 -
![](http://www.yiibai.com/uploads/images/201708/2908/289080846_34200.png)
Scala编程语言提供以下类型的循环来处理循环需求。 单击以下表中的链接来查看其详细信息。
|序号|循环类型|描述|
|----|----|----|
|1|[while循环](https://blog.csdn.net/love284969214/article/details/82930824)|在给定条件为真时，重复执行一个语句或一组语句。它在执行循环体之前测试条件。|
|2|[do…while循环](https://blog.csdn.net/love284969214/article/details/82930834)|像`while`语句一样，除了它循环体结尾测试条件。|
|3|[for循环](https://blog.csdn.net/love284969214/article/details/82930841)|多次执行一系列语句，并缩写管理循环变量的代码。|
## 循环控制语句
循环控制语句从其正常顺序更改执行。当执行离开范围时，在该范围内创建的所有自动对象都将被销毁。Scala不支持像Java那样的`break`或`continue`语句，但是从*Scala 2.8*版本开始，加入了一种打断循环的方法。点击以下链接查看详细信息。
|序号|循环类型|描述|
|----|----|----|
|1|[break语句](https://blog.csdn.net/love284969214/article/details/82930855)|终止循环语句并将执行转移到循环之后的语句。|
## 无限循环
如果一个条件永远不会变成`false`，循环将成为一个无限循环。 如果您使用Scala，`while`循环是实现无限循环的最佳方法。
以下程序实现无限循环 -
```
object Demo {
   def main(args: Array[String]) {
      var a = 10;
      // An infinite loop.
      while( true ){
         println( "Value of a: " + a );
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala* 中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
Value of a: 10
Value of a: 10
Value of a: 10
Value of a: 10
…………….
```
**如果您执行上述代码，它将进入无限循环，可以通过按*Ctrl + C*键终止。**
