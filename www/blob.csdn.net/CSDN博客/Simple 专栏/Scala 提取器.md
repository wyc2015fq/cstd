# Scala  提取器 - Simple 专栏 - CSDN博客
2018年09月17日 03:26:22[Simple_Zz](https://me.csdn.net/love284969214)阅读数：92标签：[Scala](https://so.csdn.net/so/search/s.do?q=Scala&t=blog)
个人分类：[Scala](https://blog.csdn.net/love284969214/article/category/8057598)
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
Scala中的提取器是一个拥有称为`unapply`方法，并使用`unapply`方法作为其成员之一的对象。 该`unapply`方法的目的是匹配一个值并将其分开。 通常，提取器对象还定义了一种用于构建值的双重方法，但这不是必需的。
**示例**
下面举个例子来定义`apply`和`unapply`方法。`apply`方法与`Test`转换为可应用于括号中的参数的对象，与`apply`方法相同。所以可以写`Test(“Maxsu”，“gmail.com”)`来构造字符串`“Maxsu@gmail.com”`。
`unapply`方法是将`Test`类转换为提取器，并反转了应用的构建过程。在应用两个字符串，并形成一个电子邮件地址字符串，取消应用接收电子邮件地址并返回潜在的两个字符串：地址的用户和域。
**语法**
```
unapply("Maxsu@gmail.com") equals Some("Maxsu", "gmail.com")
unapply("Maxsu Ali") equals None
```
以下示例程序显示电子邮件地址的提取对象。
```
object Demo {
   def main(args: Array[String]) {
      println ("Apply method : " + apply("Maxsu", "gmail.com"));
      println ("Unapply method : " + unapply("Maxsu@gmail.com"));
      println ("Unapply method : " + unapply("Maxsu Ali"));
   }
   // The injection method (optional)
   def apply(user: String, domain: String) = {
      user +"@"+ domain
   }
   // The extraction method (mandatory)
   def unapply(str: String): Option[(String, String)] = {
      val parts = str split "@"
      if (parts.length == 2){
         Some(parts(0), parts(1)) 
      } else {
         None
      }
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
Apply method : Maxsu@gmail.com
Unapply method : Some((Maxsu,gmail.com))
Unapply method : None
```
## 抽取器模式匹配
当一个类的实例后跟括号为零个或多个参数的列表时，编译器会调用该实例上的`apply`方法。 可以在对象和类中定义`apply`方法。
如上所述，`unapply`方法的目的是提取正在寻找的特定值。当使用`match`语句比较提取器对象时，将自动执行`unapply`方法。
尝试以下示例程序 -
```
object Demo {
   def main(args: Array[String]) {
      val x = Demo(5)
      println(x)
      x match {
         case Demo(num) => println(x+" is bigger two times than "+num)
         //unapply is invoked
         case _ => println("i cannot calculate")
      }
   }
   def apply(x: Int) = x*2
   def unapply(z: Int): Option[Int] = if (z%2==0) Some(z/2) else None
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
D:\>scalac Demo.scala
D:\>scala Demo
10
10 is bigger two times than 5
```
