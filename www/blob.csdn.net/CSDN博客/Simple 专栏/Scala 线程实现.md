# Scala  线程实现 - Simple 专栏 - CSDN博客
2018年09月17日 03:38:26[Simple_Zz](https://me.csdn.net/love284969214)阅读数：380
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
在Scala中，创建线程有两种方法：
- 通过扩展 `Thread `类
- 通过实现 `Runnable `接口
**通过扩展Thread类的Scala线程示例**
以下示例扩展了`Thread`类并覆盖了`run`方法，`start()`方法用于启动线程。
```
class ThreadExample extends Thread{  
    override def run(){  
    println("Thread is running?");  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var t = new ThreadExample()  
        t.start()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
Thread is running...
```
**通过扩展Runnable接口的Scala线程示例**
以下示例实现了`Runnable`接口并覆盖了`run`方法。 `start()`方法用于启动线程。
```
class ThreadExample extends Runnable{  
    override def run(){  
        println("Thread is running...")  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var e = new ThreadExample()  
        var t = new Thread(e)  
        t.start()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
Thread is running...
```
