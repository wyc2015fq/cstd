# Scala  函数 - Simple 专栏 - CSDN博客
2018年09月14日 17:09:41[Simple_Zz](https://me.csdn.net/love284969214)阅读数：64
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
函数是一组执行的语句。您可以将代码按功能分成一个个单独的函数。 如何在不同函数之间划分你的代码取决于你，但从逻辑上讲，通常每个函数执行一个特定的任务。
Scala具有两种函数，术语 - 方法和函数是可以互换的。 Scala方法是具有名称，签名，可选地一些注释和一些字节码的类的一部分，作为Scala中的函数是可以分配给变量的完整对象。 换句话说，定义为某个对象的成员的函数称为方法。
函数定义可以出现在源文件的任何位置，Scala允许嵌套函数定义，也就是其他函数定义中的函数定义。有一点要注意的是，Scala函数的名称可以包含符号，如：`+`，`++`，`~`，`&`，`-` ， `--` ，`\`，`/`，`:`等的字符。
## 函数声明
Scala函数声明具有以下形式 -
```
def functionName ([list of parameters]) : [return type]
```
如果不使用等号和方法体，则隐式声明抽象(`abstract`)方法。
## 函数定义
Scala函数定义具有以下形式 -
**语法**
```
def functionName ([list of parameters]) : [return type] = {
   function body
   return [expr]
}
```
这里，返回类型可以是任何有效的Scala数据类型，参数列表将是由逗号分隔的变量列表，参数列表和返回类型是可选的。与Java非常相似，返回语句可以与表达式一起使用，以防函数返回值。 以下是将两个整数相加并返回其总和的函数，
**语法**
```
object add {
   def addInt( a:Int, b:Int ) : Int = {
      var sum:Int = 0
      sum = a + b
      return sum
   }
}
```
一个不返回任何东西的函数可以返回一个类似在Java中的`void`类型，并表示该函数不返回任何内容。 在Scala中不返回任何东西的函数称为过程。
**语法**
```
object Hello{
   def printMe( ) : Unit = {
      println("Hello, Scala!")
   }
}
```
## 调用函数
Scala为调用方法提供了许多句法变体。以下是调用方法的标准方法 -
```
functionName( list of parameters )
```
如果使用对象的实例调用函数，那么可使用与Java类似的点符号，如下所示：
```
[instance.]functionName( list of parameters )
```
尝试以下示例程序来定义并调用相同的函数 -
示例 -
```
object Demo {
   def main(args: Array[String]) {
      println( "Returned Value : " + addInt(5,7) );
   }
   def addInt( a:Int, b:Int ) : Int = {
      var sum:Int = 0
      sum = a + b
      return sum
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令编译和执行此程序。
```
$ scalac Demo.scala
$ scala Demo
Returned Value : 12
```
Scala函数是Scala编程的核心，因此Scala被认为是函数式编程语言。以下是与Scala函数相关的几个重要概念，Scala程序员应该要理解。
- **[按名称调用函数](https://blog.csdn.net/love284969214/article/details/82706029)**
- **[命名参数的函数](https://blog.csdn.net/love284969214/article/details/82710326)**
- **[可变参数的函数](https://blog.csdn.net/love284969214/article/details/82710329)**
- **[递归函数](https://blog.csdn.net/love284969214/article/details/82710333)**
- **[默认参数值函数](https://blog.csdn.net/love284969214/article/details/82710336)**
- **[高阶函数](https://blog.csdn.net/love284969214/article/details/82710341)**
- **[嵌套函数](https://blog.csdn.net/love284969214/article/details/82710346)**
- **[匿名函数](https://blog.csdn.net/love284969214/article/details/82710348)**
- **[部分应用函数](https://blog.csdn.net/love284969214/article/details/82710354)**
- **[柯里化函数](https://blog.csdn.net/love284969214/article/details/82710356)**
