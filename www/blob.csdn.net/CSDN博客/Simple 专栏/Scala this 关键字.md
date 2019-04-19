# Scala  this 关键字 - Simple 专栏 - CSDN博客
2018年09月18日 14:45:44[Simple_Zz](https://me.csdn.net/love284969214)阅读数：362
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
在scala中，`this`是一个关键字，用于引用当前对象。可以使用`this`关键字调用实例变量，方法，构造函数。
## Scala this示例
在以下示例中，这用于调用实例变量和主要构造方法。
```
class ThisExample{  
    var id:Int = 0  
    var name: String = ""  
    def this(id:Int, name:String){  
        this()  
        this.id = id  
        this.name = name  
    }  
    def show(){  
        println(id+" "+name)  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var t = new ThisExample(1010,"Maxsu")  
        t.show()  
    }  
}
```
Scala
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
1010 Maxsu
```
**Scala构造函数使用this关键字调用**
在下面的例子中，使用`this`关键字来调用构造函数。它演示了如何从其他构造函数调用构造函数。必须确保`this`必须放在构造函数中的第一个语句，同时调用其他构造函数，否则编译器会抛出错误。
```
class Student(name:String){  
    def this(name:String, age:Int){  
        this(name)  
        println(name+" "+age)  
    }      
}  
object Demo{  
    def main(args:Array[String]){  
        var s = new Student("Maxsu",1000)  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
Maxsu 1010
```
