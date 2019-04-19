# Scala  构造函数 - Simple 专栏 - CSDN博客
2018年09月18日 14:44:15[Simple_Zz](https://me.csdn.net/love284969214)阅读数：117
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala默认主构造函数
在scala中，如果不指定主构造函数，编译器将创建一个主构造函数的构造函数。 所有类的主体的声明都被视为构造函数的一部分。它也被称为默认构造函数。
**Scala默认主构造函数示例**
```
class Student{  
    println("Hello from default constructor");  
}
```
## Scala主要构造函数
Scala提供了一个类的主构造函数的概念。如果代码只有一个构造函数，则可以不需要定义明确的构造函数。它有助于优化代码，可以创建具有零个或多个参数的主构造函数。
**Scala主构造函数示例**
```
class Student(id:Int, name:String){  
    def showDetails(){  
        println(id+" "+name);  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var s = new Student(1010,"Maxsu");  
        s.showDetails()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
1010 Maxsu
```
## Scala次要(辅助)构造器
可以在类中创建任意数量的辅助构造函数，必须要从辅助构造函数内部调用主构造函数。`this`关键字用于从其他构造函数调用构造函数。当调用其他构造函数时，要将其放在构造函数中的第一行。
**Scala二次构造函数示例**
```
class Student(id:Int, name:String){  
    var age:Int = 0  
    def showDetails(){  
        println(id+" "+name+" "+age)  
    }  
    def this(id:Int, name:String,age:Int){  
        this(id,name)       // Calling primary constructor, and it is first line  
        this.age = age  
    }  
}  
object Demo{  
    def main(args:Array[String]){  
        var s = new Student(1010,"Maxsu", 25);  
        s.showDetails()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
1010 Maxsu 25
```
## Scala示例：构造器重载
在scala中，可以重载构造函数。下面我们来看一个例子。
```
class Student(id:Int){  
    def this(id:Int, name:String)={  
        this(id)  
        println(id+" "+name)  
    }  
    println(id)  
}  
object Demo{  
    def main(args:Array[String]){  
        new Student(101)  
        new Student(100,"Minsu")  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
101
100
100 Minsu
```
