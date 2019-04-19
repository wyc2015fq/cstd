# Scala  单例和伴生对象 - Simple 专栏 - CSDN博客
2018年09月18日 14:42:27[Simple_Zz](https://me.csdn.net/love284969214)阅读数：149
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
单例(*Singleton*)对象是一个通过使用`object`关键字而不是使用`class`关键字声明的对象。其它对象不需要调用在单例对象中声明的方法。
在scala中，没有静态的概念。 所以scala创建一个单例对象来为程序的执行提供入口点。
如果不创建单例对象，代码也会成功编译，但不会产生任何输出。单例对象中声明的方法可以全局访问。单例对象可以扩展类和性状。
**Scala单例对象示例**
```
object Singleton{  
    def main(args:Array[String]){  
        SingletonObject.hello()         // No need to create object.  
    }  
}  
object SingletonObject{  
    def hello(){  
        println("Hello, This is Singleton Object")  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
Hello, This is Singleton Object
```
## Scala伴生对象
在scala中，当有一个与单例(*singleton*)对象同名的类时，它被称为伴生(*companion*)类，单例(*singleton*)对象调用伴生对象。
伴生类及其伴随对象必须在同一个源文件中定义。
**Scala伴生对象示例**
```
class ComapanionClass{  
    def hello(){  
        println("Hello, this is Companion Class.")  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        new ComapanionClass().hello()  
        println("And this is Companion Object.")  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
Hello, this is Companion Class.
And this is Companion Object.
```
