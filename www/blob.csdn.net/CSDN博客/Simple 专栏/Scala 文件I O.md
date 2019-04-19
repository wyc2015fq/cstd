# Scala  文件I/O - Simple 专栏 - CSDN博客
2018年09月17日 03:27:43[Simple_Zz](https://me.csdn.net/love284969214)阅读数：65
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala可以使用任何Java对象，而`java.io.File`是Scala编程中可用于读取和写入文件的对象之一。
以下是写入文件的示例程序。
```
import java.io._
object Demo {
   def main(args: Array[String]) {
      val writer = new PrintWriter(new File("test.txt" ))
      writer.write("Hello Scala")
      writer.close()
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Hello Scala
```
## 从命令行读一行
有时需要从屏幕上读取用户输入，然后继续进行进一步的处理。 以下示例程序显示如何从命令行读取输入。
```
object Demo {
   def main(args: Array[String]) {
      print("Please enter your input : " )
      val line = Console.readLine
      println("Thanks, you just typed: " + line)
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Please enter your input : Scala is great
Thanks, you just typed: Scala is great
```
## 阅读文件内容
从文件读取真的很简单。可以使用Scala的`Source`类及其对象来读取文件。以下是从前面创建的`“Demo.txt”`文件中读取的示例。
```
import scala.io.Source
object Demo {
   def main(args: Array[String]) {
      println("Following is the content read:" )
      Source.fromFile("Demo.txt" ).foreach { 
         print 
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Following is the content read:
Hello Scala
```
