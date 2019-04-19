# Scala  函数 - 部分应用函数 - Simple 专栏 - CSDN博客
2018年09月15日 00:43:17[Simple_Zz](https://me.csdn.net/love284969214)阅读数：75
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
当在调用一个函数时，把这个函数应用到参数中。 如果您传递所有预期的参数，则表示您已完全应用它。 如果只传递几个参数并不是全部参数，那么将返回部分应用的函数。这样就可以方便地绑定一些参数，其余的参数可稍后填写补上。
尝试以下，下面是一个简单的示例程序用来演示如何使用部分应用函数 -
```
import java.util.Date
object Demo {
   def main(args: Array[String]) {
      val date = new Date
      log(date, "message1" )
      Thread.sleep(1000)
      log(date, "message2" )
      Thread.sleep(1000)
      log(date, "message3" )
   }
   def log(date: Date, message: String)  = {
      println(date + "----" + message)
   }
}
```
将上述程序保存在源文件：*Demo.scala* 中，使用以下命令编译和执行此程序。
```
D:/> scalac Demo.scala
D:/> scala Demo
Mon Dec 02 12:52:41 CST 2018----message1
Mon Dec 02 12:52:41 CST 2018----message2
Mon Dec 02 12:52:41 CST 2018----message3
```
这里，`log()`方法有两个参数：`date`和`message`。 我们想要多次调用该方法，具有相同的日期值，但不同的消息值。可以通过将参数部分地应用到`log()`方法来消除将日期传递给每个调用的干扰。为此，首先将值绑定到`date`参数，并将第二个参数绑定到其位置。 结果是存储在变量中的部分应用函数。
尝试以下示例程序以仅使用未绑定的参数消息来调用此新方法。
```
import java.util.Date
object Demo {
   def main(args: Array[String]) {
      val date = new Date
      val logWithDateBound = log(date, _ : String)
      logWithDateBound("message1" )
      Thread.sleep(1000)
      logWithDateBound("message2" )
      Thread.sleep(1000)
      logWithDateBound("message3" )
   }
   def log(date: Date, message: String) = {
      println(date + "----" + message)
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令来编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Mon Dec 02 12:53:56 CST 2018----message1
Mon Dec 02 12:53:56 CST 2018----message2
Mon Dec 02 12:53:56 CST 2018----message3
```
