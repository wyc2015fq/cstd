# Scala break语句 - Simple 专栏 - CSDN博客
2018年10月03日 11:41:55[Simple_Zz](https://me.csdn.net/love284969214)阅读数：81标签：[Scala																[编程语言](https://so.csdn.net/so/search/s.do?q=编程语言&t=blog)](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala中没有内置的`break`语句，但是如果您运行的是*Scala 2.8*版本，则可以使用`break`语句。当循环中遇到`break`语句时，循环将立即终止，程序控制跳到循环之后的下一个语句执行。
**流程图**
![](http://www.yiibai.com/uploads/images/201708/2908/750100835_79266.png)
## 语法
以下是`break`语句的语法 -
```
// import following package
import scala.util.control._
// create a Breaks object as follows
val loop = new Breaks;
// Keep the loop inside breakable as follows
loop.breakable {
   // Loop will go here
   for(...){
      ....
      // Break will go here
      loop.break;
   }
}
```
尝试以下示例程序来理解`break`语句。
```
import scala.util.control._
object Demo {
   def main(args: Array[String]) {
      var a = 0;
      val numList = List(1,2,3,4,5,6,7,8,9,10);
      val loop = new Breaks;
      loop.breakable {
         for( a <- numList){
            println( "Value of a: " + a );
            if( a == 4 ){
               loop.break;
            }
         }
      }
      println( "After the loop" );
   }
}
```
将上述程序保存在源文件：*Demo.scala* 中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
Value of a: 1
Value of a: 2
Value of a: 3
Value of a: 4
After the loop
```
## 中断嵌套循环
在使用嵌套循环时，存在一个问题。为了防止对嵌套循环使用`break`，请参照下面的方法。这是一个中断嵌套循环的示例程序。
**示例**
```
import scala.util.control._
object Demo {
   def main(args: Array[String]) {
      var a = 0;
      var b = 0;
      val numList1 = List(1,2,3,4,5);
      val numList2 = List(11,12,13);
      val outer = new Breaks;
      val inner = new Breaks;
      outer.breakable {
         for( a <- numList1){
            println( "Value of a: " + a );
            inner.breakable {
               for( b <- numList2){
                  println( "Value of b: " + b );
                  if( b == 12 ){
                     inner.break;
                  }
               }
            } // inner breakable
         }
      } // outer breakable.
   }
}
```
将上述程序保存在源文件：*Demo.scala* 中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
Value of a: 1
Value of b: 11
Value of b: 12
Value of a: 2
Value of b: 11
Value of b: 12
Value of a: 3
Value of b: 11
Value of b: 12
Value of a: 4
Value of b: 11
Value of b: 12
Value of a: 5
Value of b: 11
Value of b: 12
```
