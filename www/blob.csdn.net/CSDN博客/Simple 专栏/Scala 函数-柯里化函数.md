# Scala  函数 - 柯里化函数 - Simple 专栏 - CSDN博客
2018年09月15日 00:44:02[Simple_Zz](https://me.csdn.net/love284969214)阅读数：83
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
柯里化(*Currying*)函数是一个带有多个参数，并引入到一个函数链中的函数，每个函数都使用一个参数。 柯里化(*Currying*)函数用多个参数表定义，如下所示：
```
def strcat(s1: String)(s2: String) = s1 + s2
```
或者，还可以使用以下语法定义柯里化(*Currying*)函数 -
```
def strcat(s1: String) = (s2: String) => s1 + s2
```
以下是调用柯里化(*Currying*)函数的语法 -
```
strcat("foo")("bar")
```
您可以根据需要在柯里化(*Currying*)函数上定义两个以上的参数。尝试下面一个简单的示例程序用来了解如何使用柯里化(*Currying*)函数 -
```
object Demo {
   def main(args: Array[String]) {
      val str1:String = "Hello, "
      val str2:String = "Scala!"
      println( "str1 + str2 = " +  strcat(str1)(str2) )
   }
   def strcat(s1: String)(s2: String) = {
      s1 + s2
   }
}
```
将上述程序保存在源文件：*Demo.scala* 中，使用以下命令编译和执行此程序。
```
D:\> scalac Demo.scala
D:\> scala Demo
str1 + str2 = Hello, Scala!
```
