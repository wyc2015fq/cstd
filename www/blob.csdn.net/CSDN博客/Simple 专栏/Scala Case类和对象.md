# Scala  Case类和对象 - Simple 专栏 - CSDN博客
2018年09月18日 14:43:16[Simple_Zz](https://me.csdn.net/love284969214)阅读数：355
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala `Case`类只是常规类，默认情况下是不可变的，可通过模式匹配可分解。
它使用相等(`equal`)方法在结构上比较实例。它不使用`new`关键字实例化对象。
默认情况下，`case`类中列出的所有参数默认使用`public`和`immutable`修辞符。
**语法**
```
case class className(parameters)
```
**Scala Case类示例**
```
case class CaseClass(a:Int, b:Int)  
object Demo{  
    def main(args:Array[String]){  
        var c =  CaseClass(10,10)       // Creating object of case class  
        println("a = "+c.a)               // Accessing elements of case class  
        println("b = "+c.b)  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
a = 10
b = 10
```
`Case`类支持模式匹配。 所以，可以在模式中使用它。以下是`Case`类和模式的示例。
## Scala Case类和模式匹配示例
没有参数的`case`类将被声明为`case`对象而不是`case`类。 默认情况下，`case`对象是可序列化的。
```
trait SuperTrait  
case class CaseClass1(a:Int,b:Int) extends SuperTrait  
case class CaseClass2(a:Int) extends SuperTrait         // Case class  
case object CaseObject extends SuperTrait               // Case object  
object Demo{  
    def main(args:Array[String]){  
        callCase(CaseClass1(10,10))  
        callCase(CaseClass2(10))  
        callCase(CaseObject)  
    }  
    def callCase(f:SuperTrait) = f match{  
        case CaseClass1(f,g)=>println("a = "+f+" b ="+g)  
        case CaseClass2(f)=>println("a = "+f)  
        case CaseObject=>println("No Argument")  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
a = 10 b =10
a = 10
No Argument
```
