# Scala  throw  throws 关键字 - Simple 专栏 - CSDN博客
2018年09月17日 03:45:27[Simple_Zz](https://me.csdn.net/love284969214)阅读数：313
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala提供了`throw`关键字来抛出异常。 `throw`关键字主要用于抛出自定义异常。同时也提供了`throws`关键字来声明异常。可以使用方法定义声明异常。 它向调用者函数提供了此方法可能引发此异常的信息。 它有助于调用函数处理并将该代码包含在`try-catch`块中，以避免程序异常终止。在scala中，可以使用`throws`关键字或`throws`注释来声明异常。
**Scala Throw示例**
```
class ExceptionExample2{  
    def validate(age:Int)={  
        if(age<18)  
            throw new ArithmeticException("You are not eligible")  
        else println("You are eligible")  
    }  
}  
object MainObject{  
    def main(args:Array[String]){  
        var e = new ExceptionExample2()  
        e.validate(10)  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
java.lang.ArithmeticException: You are not eligible
```
**Scala Throws示例**
```
class ExceptionExample4{  
    @throws(classOf[NumberFormatException])  
    def validate()={  
        "abc".toInt  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var e = new ExceptionExample4()  
        try{  
            e.validate()  
        }catch{  
            case ex : NumberFormatException => println("Exception handeled here")  
        }  
        println("Rest of the code executing...")  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo.scal
Exception handeled here
Rest of the code executing...
```
