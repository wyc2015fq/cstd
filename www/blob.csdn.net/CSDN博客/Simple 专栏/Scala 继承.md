# Scala  继承 - Simple 专栏 - CSDN博客
2018年09月18日 14:52:24[Simple_Zz](https://me.csdn.net/love284969214)阅读数：110标签：[Scala](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
继承是面向对象的概念，用于代码的可重用性。可以通过使用`extends`关键字来实现继承。 为了实现继承，一个类必须扩展到其他类，被扩展类称为超类或父类。扩展的类称为派生类或子类。
**示例**
```
class SubClassName extends SuperClassName(){  
    /* Write your code  
     *  methods and fields etc. 
     */  
 }
```
## 继承的简单例子
![](http://www.yiibai.com/uploads/images/201709/0409/351150949_88354.png)
**Scala单继承示例**
```
class Employee{  
    var salary:Float = 11100  
}  
class Programmer extends Employee{  
    var bonus:Int = 5100  
    println("Salary = "+salary)  
    println("Bonus = "+bonus)  
}  
object Demo{  
    def main(args:Array[String]){  
        new Programmer()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
Salary = 11100.0
Bonus = 5100
```
## Scala继承类型
Scala支持各种类型的继承，包括单一，多层次，多重和混合。可以在类中使用单一，多层次和层次结构。多重和混合只能通过使用特征来实现。在这里，通过使用图形表示所有类型的继承。
![](https://img-blog.csdn.net/20180918145156355?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
class A{  
    var salary1 = 10000  
}  
class B extends A{  
    var salary2 = 20000  
}  
class C extends B{  
    def show(){  
        println("salary1 = "+salary1)  
        println("salary2 = "+salary2)  
    }  
}  
object Demo{  
    def main(args:Array[String]){
        var c = new C()  
        c.show()  
    }  
}
```
将上面代码保存到源文件：*Demo.scala*中，使用以下命令编译并执行代码 -
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
salary1 = 10000
salary2 = 20000
```
