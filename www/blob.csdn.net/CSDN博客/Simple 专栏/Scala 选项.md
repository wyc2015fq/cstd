# Scala  选项 - Simple 专栏 - CSDN博客
2018年09月16日 00:50:12[Simple_Zz](https://me.csdn.net/love284969214)阅读数：45标签：[Scala](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala `Option[T]`是由给定类型的零或一个元素的一种容器。`Option[T]`可以是 `Some [T]`或`None`对象，它代表缺少的值。 例如，如果已找到与给定键对应的值，则Scala的Map的`get`方法会生成`Some(value)`，如果在`Map`中未定义给定的键，则将返回`None`。
`Option`类型在Scala程序中经常使用，可以将其与Java中可用的`null`值进行比较，表示`null`值。 例如，`java.util.HashMap`的`get`方法返回存储在`HashMap`中的值，如果没有找到值，则返回`null`。
假设我们有一种基于主键从数据库中检索记录的方法。
```
def findPerson(key: Int): Option[Person]
```
如果找到记录，该方法将返回`Some [Person]`，如果没有找到该记录，则返回`None`。下面来看看看以下程序代码 -
**示例**
```
object Demo {
   def main(args: Array[String]) {
      val capitals = Map("France" -> "Paris", "Japan" -> "Tokyo")
      println("capitals.get( \"France\" ) : " +  capitals.get( "France" ))
      println("capitals.get( \"India\" ) : " +  capitals.get( "India" ))
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
capitals.get( "France" ) : Some(Paris)
capitals.get( "India" ) : None
```
将可选值分开的最常见方法是通过模式匹配。例如尝试以下程序 -
```
object Demo {
   def main(args: Array[String]) {
      val capitals = Map("France" -> "Paris", "Japan" -> "Tokyo")
      println("show(capitals.get( \"Japan\")) : " + show(capitals.get( "Japan")) )
      println("show(capitals.get( \"India\")) : " + show(capitals.get( "India")) )
   }
   def show(x: Option[String]) = x match {
      case Some(s) => s
      case None => "?"
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
show(capitals.get( "Japan")) : Tokyo
show(capitals.get( "India")) : ?
```
## 使用getOrElse()方法
以下是示例程序，显示如何在没有值的情况下使用`getOrElse()`方法来访问值或默认值。
```
object Demo {
   def main(args: Array[String]) {
      val a:Option[Int] = Some(5)
      val b:Option[Int] = None 
      println("a.getOrElse(0): " + a.getOrElse(0) )
      println("b.getOrElse(10): " + b.getOrElse(10) )
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
a.getOrElse(0): 5
b.getOrElse(10): 10
```
## 使用isEmpty()方法
以下是显示如何使用`isEmpty()`方法检查该选项是否为`None`的示例程序。
```
object Demo {
   def main(args: Array[String]) {
      val a:Option[Int] = Some(5)
      val b:Option[Int] = None 
      println("a.isEmpty: " + a.isEmpty )
      println("b.isEmpty: " + b.isEmpty )
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
a.isEmpty: false
b.isEmpty: true
```
