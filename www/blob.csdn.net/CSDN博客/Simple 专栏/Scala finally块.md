# Scala  finally块 - Simple 专栏 - CSDN博客
2018年09月17日 03:42:10[Simple_Zz](https://me.csdn.net/love284969214)阅读数：152
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
`finally`块用于在异常时释放资源。资源可能是文件，网络连接，数据库连接等，`finally`块执行代码运行保证。以下程序说明了`finally`块的用法。
**Scala finally块示例**
```
class ExceptionExample{  
    def divide(a:Int, b:Int) = {  
        try{  
            a/b  
            var arr = Array(1,2)  
            arr(10)  
        }catch{  
            case e: ArithmeticException => println(e)  
            case ex: Exception =>println(ex)  
            case th: Throwable=>println("found a unknown exception"+th)  
        }  
        finally{  
            println("Finaly block always executes")  
        }  
        println("Rest of the code is executing...")  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var e = new ExceptionExample()  
        e.divide(100,10)  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
java.lang.ArrayIndexOutOfBoundsException: 10
Finally block always executes
Rest of the code is executing...
```
