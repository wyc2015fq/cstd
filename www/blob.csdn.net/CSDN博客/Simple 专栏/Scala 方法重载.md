# Scala  方法重载 - Simple 专栏 - CSDN博客
2018年09月18日 14:44:57[Simple_Zz](https://me.csdn.net/love284969214)阅读数：281
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala提供了方法重载功能，使我们能够定义相同名称但使用不同参数或数据类型的方法。 它有助于优化代码。
**Scala方法通过使用不同的参数重载示例**
在下面的例子中，定义了两个具有不同数量的参数但具有相同数据类型的`add`方法。
```
class Arithmetic{  
    def add(a:Int, b:Int){  
        var sum = a+b  
        println(sum)  
    }  
    def add(a:Int, b:Int, c:Int){  
        var sum = a+b+c  
        println(sum)  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var a  = new Arithmetic();  
        a.add(10,20);  
        a.add(10,20,30);  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
30
60
```
**Scala通过使用不同的数据类型方法重载示例**
在下面的例子中，创建了一个使用两个相同数量的参数但是不同的数据类型的`add`方法。
```
class Arithmetic{  
    def add(a:Int, b:Int){  
        var sum = a+b  
        println(sum)  
    }  
    def add(a:Double, b:Double){  
        var sum = a+b  
        println(sum)  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var b = new Arithmetic()  
        b.add(10,20)  
        b.add(10.0,20.1)  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
30
30.1
```
