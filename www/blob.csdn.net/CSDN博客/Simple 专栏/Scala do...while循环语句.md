# Scala do...while循环语句 - Simple 专栏 - CSDN博客
2018年10月03日 11:38:52[Simple_Zz](https://me.csdn.net/love284969214)阅读数：109标签：[Scala																[编程语言](https://so.csdn.net/so/search/s.do?q=编程语言&t=blog)](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
与在循环顶部测试循环条件的`while`循环语句不同，`do...while`循环检查循环底部的条件。 一个`do...while`循环类似于`while`循环，除了`do...while`循环保证至少执行一次。
## 语法
以下是`do...while`循环的语法。
```
do {
   statement(s);
} while( condition );
```
请注意，条件(`condition`)表达式出现在循环的末尾，因此循环中的语句在测试条件之前执行一次。 如果条件为真，则控制流程跳回来执行，并且循环中的语句再次执行。 该过程重复，直到给定条件变为假。
**流程图**
![](http://www.yiibai.com/uploads/images/201708/2908/679090837_92138.png)
尝试以下示例程序来了解Scala编程语言中的`do...while`循环控制语句。
```
object Demo {
   def main(args: Array[String]) {
      // Local variable declaration:
      var a = 10;
      // do loop execution
      do {
         println( "Value of a: " + a );
         a = a + 1;
      }
      while( a < 20 )
   }
}
```
将上述程序保存在源文件：*Demo.scala* 中，使用以下命令编译和执行此程序。
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
