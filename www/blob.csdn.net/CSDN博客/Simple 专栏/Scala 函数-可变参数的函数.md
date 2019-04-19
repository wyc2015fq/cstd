# Scala  函数 - 可变参数的函数 - Simple 专栏 - CSDN博客
2018年09月15日 00:36:49[Simple_Zz](https://me.csdn.net/love284969214)阅读数：258
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala允许指定函数的最后一个参数可重复。 这允许客户端将可变长度参数列表传递给函数。 这里，打印字符串函数里面的`args`类型，被声明为类型 `String *`，实际上是 `Array [String]`。
尝试以下程序，这是一个简单的例子来演示如何使用带有可变参数的函数。
```
object Demo {
   def main(args: Array[String]) {
      printStrings("Hello", "Scala", "Python");
   }
   def printStrings( args:String* ) = {
      var i : Int = 0;
      for( arg <- args ){
         println("Arg value[" + i + "] = " + arg );
         i = i + 1;
      }
   }
}
```
将上述程序保存在源文件:*Demo.scala* 中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
Arg value[0] = Hello
Arg value[1] = Scala
Arg value[2] = Python
```
