# Scala while循环语句 - Simple 专栏 - CSDN博客
2018年10月03日 11:37:41[Simple_Zz](https://me.csdn.net/love284969214)阅读数：241
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
当给定条件为真时，`while`循环重复一个语句或一组语句。它在执行循环体之前测试条件状态。 只要给定的条件为真，`while`循环语句重复执行目标语句。
**语法**
以下是`while`循环的语法 -
```
while(condition){
   statement(s);
}
```
在这里，语句(`statement(s)`)可能是一个单一语句或一个语句块。条件(`condition`)可以是任何表达式，`true`是任何非零值。循环在条件为真时循环。当条件(`condition`)变为`false`时，程序控制传递到循环之后的行代码。
![](http://www.yiibai.com/uploads/images/201708/2908/644090821_98852.png)
在这里，`while`循环的关键是循环体可能不会运行。因为当条件被测试并且结果为假时，循环体将被跳过，这时`while`循环之后的第一个语句将被执行。
尝试以下示例程序来了解Scala编程语言中的`while`循环控制语句。
```
object Demo {
   def main(args: Array[String]) {
      // Local variable declaration:
      var a = 10;
      // while loop execution
      while( a < 20 ){
         println( "Value of a: " + a );
         a = a + 1;
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala* 中。使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
value of a: 10
value of a: 11
value of a: 12
value of a: 13
value of a: 14
value of a: 15
value of a: 16
value of a: 17
value of a: 18
value of a: 19
```
