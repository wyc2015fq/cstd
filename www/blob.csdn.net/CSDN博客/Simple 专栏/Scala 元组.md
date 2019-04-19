# Scala  元组 - Simple 专栏 - CSDN博客
2018年09月16日 00:50:06[Simple_Zz](https://me.csdn.net/love284969214)阅读数：54标签：[Scala](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala元组将固定数量的项目组合在一起，以便它们可以作为一个整体传递。 与数组或列表不同，元组可以容纳不同类型的对象，但它们也是不可变的。
以下是一个存有整数，字符串和控制台(`console`)的元组的示例。
```
val t = (1, "hello", Console)
```
上面是以下语法的简写 -
```
val t = new Tuple3(1, "hello", Console)
```
元组的实际类型取决于它包含的数量和元素以及这些元素的类型。 因此，`(99，"Luftballons")`的类型是`Tuple2 [Int，String]`。 `('u'，'r'，“the”，1,4，"me")`是`Tuple6 [Char，Char，String，Int，Int，String]`。
元组是类型`Tuple1`，`Tuple2`，`Tuple3`等等。目前在Scala中只能有`22`个上限，如果您需要更多个元素，那么可以使用集合而不是元组。 对于每个`TupleN`类型，其中上限为`1 <= N <= 22`，Scala定义了许多元素访问方法。给定以下定义 -
```
val t = (4,3,2,1)
```
要访问元组`t`的元素，可以使用`t._1`方法访问第一个元素，`t._2`方法访问第二个元素，依此类推。 例如，以下表达式计算`t`的所有元素的总和 -
```
val sum = t._1 + t._2 + t._3 + t._4
```
可以使用`Tuple`以及采用`List [Double]`来编写一个方法，并返回在三元组元组`Tuple3 [Int，Double，Double]`中返回的计数，总和和平方和。它们也可用于将数据值列表作为并发编程中的参与者之间的消息传递。
请尝试以下示例程序。 它显示了如何使用元组。
```
object Demo {
   def main(args: Array[String]) {
      val t = (4,3,2,1)
      val sum = t._1 + t._2 + t._3 + t._4
      println( "Sum of elements: "  + sum )
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Sum of elements: 10
```
## 迭代元组
可以使用`Tuple.productIterator()`方法遍历元组的所有元素。
尝试以下示例程序来遍历元组。
**示例**
```
object Demo {
   def main(args: Array[String]) {
      val t = (4,3,2,1)
      t.productIterator.foreach{ i =>println("Value = " + i )}
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Value = 4
Value = 3
Value = 2
Value = 1
```
## 转换为字符串
可以使用`Tuple.toString()`方法将元组的所有元素连接成字符串。尝试以下示例程序转换为`String`。
以下是将元组转换为字符串的例子 -
```
object Demo {
   def main(args: Array[String]) {
      val t = new Tuple3(1, "hello", Console)
      println("Concatenated String: " + t.toString() )
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Concatenated String: (1,hello,scala.Console$@281acd47)
```
## 交换元素
可以使用`Tuple.swap`方法交换`Tuple2`中的元素。
尝试以下示例程序来交换元素。
```
object Demo {
   def main(args: Array[String]) {
      val t = new Tuple2("Scala", "hello")
      println("Swapped Tuple: " + t.swap )
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Swapped tuple: (hello,Scala)
```
