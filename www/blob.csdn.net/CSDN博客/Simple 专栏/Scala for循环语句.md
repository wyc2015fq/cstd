# Scala for循环语句 - Simple 专栏 - CSDN博客
2018年10月03日 11:40:55[Simple_Zz](https://me.csdn.net/love284969214)阅读数：171
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
`for`循环是一种重复控制结构，可以让您有效地编写一个需要执行特定次数的循环。在Scala中有各种形式的`for`循环，如下所述 -
## 使用范围的for循环
Scala中带范围`for`循环的最简单的语法是 -
```
for( var x <- Range ){
   statement(s);
}
```
在这里，`Range`可以是数字范围，并且表示为`i`到`j`。 左箭头`←`运算符被称为生成器，因为它从一个范围中生成单个值。
请尝试以下示例程序来了解Scala编程语言中的循环控制语句(`for`语句)，参考以下示例代码 -
```
object Demo {
   def main(args: Array[String]) {
      var a = 0;
      // for loop execution with a range
      for( a <- 1 to 10){
         println( "Value of a: " + a );
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，并使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
value of a: 1
value of a: 2
value of a: 3
value of a: 4
value of a: 5
value of a: 6
value of a: 7
value of a: 8
value of a: 9
value of a: 10
```
尝试以下示例程序来了解Scala编程语言中循环控制语句(`for`语句)，循环打印范围`i`到`j`。参考以下示例代码 -
```
object Demo {
   def main(args: Array[String]) {
      var a = 0;
      // for loop execution with a range
      for( a <- 1 until 10){
         println( "Value of a: " + a );
      }
   }
}
```
将上述程序保存源文件在：*Demo.scala* 中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
value of a: 1
value of a: 2
value of a: 3
value of a: 4
value of a: 5
value of a: 6
value of a: 7
value of a: 8
value of a: 9
```
可以在`for`循环中使用由分号(`;`)分隔的多个范围，在这种情况下，循环将遍历给定范围所有可能的计算。以下是仅使用两个范围的示例，也可以使用两个以上的范围。
```
object Demo {
   def main(args: Array[String]) {
      var a = 0;
      var b = 0;
      // for loop execution with a range
      for( a <- 1 to 3; b <- 1 to 3){
         println( "Value of a: " + a );
         println( "Value of b: " + b );
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，并使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
Value of a: 1
Value of b: 1
Value of a: 1
Value of b: 2
Value of a: 1
Value of b: 3
Value of a: 2
Value of b: 1
Value of a: 2
Value of b: 2
Value of a: 2
Value of b: 3
Value of a: 3
Value of b: 1
Value of a: 3
Value of b: 2
Value of a: 3
Value of b: 3
```
## 循环与集合
循环与集合的语法如下 -
```
for( var x <- List ){
   statement(s);
}
```
这里，`List`变量是一个具有元素列表的集合类型，循环遍历所有元素，它一次返回一个元素到`x`变量中。
尝试以下示例程序来了解一个数字集合的循环。 这里我们使用`List()`创建了这个集合。 我们将在另一章中学习集合。以下是在Scala编程语言中循环控制语句(`for`语句)遍历集合的示例代码。
```
object Demo {
   def main(args: Array[String]) {
      var a = 0;
      val numList = List(1,2,3,4,5,6);
      // for loop execution with a collection
      for( a <- numList ){
         println( "Value of a: " + a );
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，并使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
value of a: 1
value of a: 2
value of a: 3
value of a: 4
value of a: 5
value of a: 6
```
## for循环与过滤器
Scala的`for`循环允许使用一个或多个`if`语句过滤掉一些元素。以下是`for`循环与过滤器的语法。 要为`for`表达式添加多个过滤器，请使用分号(`;`)分隔过滤器。
```
for( var x <- List
      if condition1; if condition2...
   ){
   statement(s);
}
```
尝试以下示例程序来了解具有过滤器的循环。
**示例**
```
object Demo {
   def main(args: Array[String]) {
      var a = 0;
      val numList = List(1,2,3,4,5,6,7,8,9,10);
      // for loop execution with multiple filters
      for( a <- numList
           if a != 3; if a < 8 ){
         println( "Value of a: " + a );
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，并使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
value of a: 1
value of a: 2
value of a: 4
value of a: 5
value of a: 6
value of a: 7
```
## for循环与yield
您可以将变量中的`for`循环存储返回值，或通过函数返回。为此，您可以通过关键字`yield`来为`for`表达式的正文添加前缀。以下是语法 -
```
var retVal = for{ var x <- List
   if condition1; if condition2...
}
yield x
```
> 
注意 - 大括号已被用来保留变量和条件，并且`retVal`是一个变量，其中`x`的所有值将以集合的形式存储。
尝试以下示例程序来了解`for`循环与`yield`的使用。
```
object Demo {
   def main(args: Array[String]) {
      var a = 0;
      val numList = List(1,2,3,4,5,6,7,8,9,10);
      // for loop execution with a yield
      var retVal = for{ a <- numList if a != 3; if a < 8 }yield a
      // Now print returned values using another loop.
      for( a <- retVal){
         println( "Value of a: " + a );
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，并使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
value of a: 1
value of a: 2
value of a: 4
value of a: 5
value of a: 6
value of a: 7
```
