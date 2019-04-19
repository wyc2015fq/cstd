# Scala  异常处理 - Simple 专栏 - CSDN博客
2018年09月17日 03:24:01[Simple_Zz](https://me.csdn.net/love284969214)阅读数：153
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala的异常处理类似许多其他语言(如Java)。它不是以正常方式返回值，方法可以通过抛出异常来终止。 但是，Scala实际上并没有检查异常。
当您想要处理异常时，要像Java一样使用`try {...} catch {...}`块，除了`catch`块使用匹配来识别和处理异常。
## 引发异常
抛出异常看起来与Java中的异常一样。创建一个异常对象，然后使用`throw`关键字将其抛出如下。
```
throw new IllegalArgumentException
```
## 捕获异常
Scala允许在单个块中`try/catch`任何异常，然后使用`case`块对其进行模式匹配。尝试以下示例程序来处理异常。
```
import java.io.FileReader
import java.io.FileNotFoundException
import java.io.IOException
object Demo {
   def main(args: Array[String]) {
      try {
         val f = new FileReader("input.txt")
      } catch {
         case ex: FileNotFoundException =>{
            println("Missing file exception")
         }
         case ex: IOException => {
            println("IO Exception")
         }
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Missing file exception
```
`try-catch`表达式的行为与其他具有异常的语言相同。它在主体中执行，如果它抛出一个异常，则会依次尝试每个`catch`子句。
## finally子句
如果希望引发一些代码执行，无论表达式如何终止，都会执行在`finally`子句包装表达式。尝试以下程序。
```
import java.io.FileReader
import java.io.FileNotFoundException
import java.io.IOException
object Demo {
   def main(args: Array[String]) {
      try {
         val f = new FileReader("input.txt")
      } catch {
         case ex: FileNotFoundException => {
            println("Missing file exception")
         }
         case ex: IOException => {
            println("IO Exception")
         }
      } finally {
         println("Exiting finally...")
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Missing file exception
Exiting finally...
```
