# Scala  try-catch 语句 - Simple 专栏 - CSDN博客
2018年09月17日 03:41:27[Simple_Zz](https://me.csdn.net/love284969214)阅读数：234
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala提供`try`和`catch`块来处理异常。`try`块用于包含可疑代码。`catch`块用于处理`try`块中发生的异常。可以根据需要在程序中有任意数量的`try...catch`块。
**Scala try catch示例1**
在下面的程序中，我们将可疑代码封装在`try`块中。 在`try`块之后使用了一个`catch`处理程序来捕获异常。如果发生任何异常，`catch`处理程序将处理它，程序将不会异常终止。
```
class ExceptionExample{  
    def divide(a:Int, b:Int) = {  
        try{  
            a/b  
        }catch{  
            case e: ArithmeticException => println(e)  
        }  
        println("Rest of the code is executing...")  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var e = new ExceptionExample()  
        e.divide(100,0)  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
java.lang.ArithmeticException: / by zero
Rest of the code is executing...
```
**Scala Try Catch示例2**
在这个例子中，`catch`处理程序有两种情况。 第一种情况将只处理算术类型异常。 第二种情况有`Throwable`类，它是异常层次结构中的超类。第二种情况可以处理任何类型的异常在程序代码中。有时当不知道异常的类型时，可以使用超类 - `Throwable`类。
```
class ExceptionExample{  
    def divide(a:Int, b:Int) = {  
        try{  
            a/b  
            var arr = Array(1,2)  
            arr(10)  
        }catch{  
            case e: ArithmeticException => println(e)  
            case ex: Throwable =>println("found a unknown exception"+ ex)  
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
found a unknown exceptionjava.lang.ArrayIndexOutOfBoundsException: 10
Rest of the code is executing...
```
