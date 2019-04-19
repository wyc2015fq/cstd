# Scala  final关键字 - Simple 专栏 - CSDN博客
2018年09月18日 14:54:13[Simple_Zz](https://me.csdn.net/love284969214)阅读数：337
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
`final`是一个关键字，用于防止超类成员继承为派生类。也可以声明`final`变量，方法和类。
## Scala final变量示例
不能覆盖子类中的`final`变量，我们来看下面一个例子。
**Scala单继承示例**
```
class Vehicle{  
     final val speed:Int = 60  
}  
class Bike extends Vehicle{  
   override val speed:Int = 100  
    def show(){  
        println(speed)  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var b = new Bike()  
        b.show()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
Demo.scala:5: error: overriding value speed in class Vehicle of type Int;
 value speed cannot override final member
   override val speed:Int = 100
                ^
one error found
```
## Scala final方法
在父类中的`final`方法声明不能被覆盖。 如果不想让它被覆盖，则可以把方法定义成为`final`。尝试覆盖`final`方法将导致编译时错误。
**Scala final方法示例**
```
class Vehicle{  
     final def show(){  
         println("vehicle is running")  
     }  
}  
class Bike extends Vehicle{  
   //override val speed:Int = 100  
    override def show(){  
        println("bike is running")  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var b = new Bike()  
        b.show()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
Demo.scala:8: error: overriding method show in class Vehicle of type ()Unit;
 method show cannot override final member
    override def show(){
                 ^
one error found
```
**Scala final类示例**
也可以定义`final`类，`final`类不能继承。 如果定义了一个类为`final`类，那就不能进一步扩展了。
```
final class Vehicle{  
     def show(){  
         println("vehicle is running")  
     }  
}  
class Bike extends Vehicle{  
       override def show(){  
        println("bike is running")  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var b = new Bike()  
        b.show()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
Demo.scala:8: error: illegal inheritance from final class Vehicle
class Bike extends Vehicle{
                   ^
one error found
```
