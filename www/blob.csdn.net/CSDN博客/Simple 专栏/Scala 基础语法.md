# Scala  基础语法 - Simple 专栏 - CSDN博客
2018年09月14日 16:54:29[Simple_Zz](https://me.csdn.net/love284969214)阅读数：162
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
如果您熟悉Java语言语法和编程，那么学习Scala将会很容易。Scala和Java之间最大的句法差异在于行结束字符的分号(`;`) 是可选的。
当编写Scala程序时，它可以被定义为通过调用彼此的方法进行通信的对象的集合。现在我们简单地看一下类，对象，方法和实例变量的含义。
- **类 - **可以将类定义为描述与该类相关的行为/状态的模板/蓝图。
- **对象 -** 对象有状态和行为。一个对象是类的一个实例。
- **方法 - **一个方法基本上是一种行为。一个类可以包含很多方法。将逻辑代码写入的方法中，数据在方法中操纵并且执行所有的动作。
- **字段 - **每个对象都有其唯一的一组实例变量，称为字段。对象的状态是由分配给这些字段的值来创建的。
- **闭包 -** 闭包是一个函数，其返回值取决于在此函数之外声明的一个或多个变量的值。
- **特征 - **特征(*Traits*)封装了方法和字段定义，然后可以通过将它们混合到类中来重用它们，特征(*Traits*)用于通过指定支持的方法的签名来定义对象类型。
## 第一个Scala项目
我们可以通过两种模式执行*Scala*程序：
- **交互模式**
- **脚本模式**
### 1. 交互模式
打开命令提示符并使用以下命令打开*Scala*。
> 
注意： 需要先下载： *scala-2.12.x.tgz* ，下载地址： [http://downloads.lightbend.com/scala/2.12.3/scala-2.12.3.tgz](http://downloads.lightbend.com/scala/2.12.3/scala-2.12.3.tgz) ，并解压到一个目录，这里解压到：*D:\software\* ，
打开命令提示符进入到目录：*D:\software\scala-2.12.3\bin* 执行如下命令运行程序 -
```
D:\software\scala-2.12.3\bin> scala
Welcome to Scala 2.12.3 (Java HotSpot(TM) 64-Bit Server VM, Java 1.8.0_65).
Type in expressions for evaluation. Or try :help.
scala> println("Hello, Scala!");
Hello, Scala!
scala>
```
Shell
### 2. 脚本模式
使用以下说明在脚本模式下编写Scala程序。打开记事本创建一个源文件：*HelloWorld.scala*，保存在目录：*D:\software\scala-2.12.3\bin* 下，并在其中编写以下代码 -
```
object HelloWorld {
   /* This is my first java program.  
   * This will print 'Hello World' as the output
   */
   def main(args: Array[String]) {
      println("Hello, world!") // prints Hello World
   }
}
```
Scala
打开命令提示符窗口并转到保存程序文件的目录。使用`'scalac'`命令编译Scala程序，它将在当前目录中生成一些类文件。其中一个将被编译生成为`HelloWorld.class`。 这是一个使用`'scala'`命令在Java虚拟机(JVM)上运行的字节码。
使用以下命令来编译和执行Scala程序及输出结果如下 -
```
D:\software\scala-2.12.3\bin>scalac HelloWorld.scala
D:\software\scala-2.12.3\bin>scala HelloWorld
Hello, world!
D:\software\scala-2.12.3\bin>
```
## 基础语法
以下是Scala编程中的基本语法和编码约定。
- **区分大小写** - Scala是区分大小写的，比如标识符`Hello`和`hello`在Scala中表示不同的含义(对象)。
- 
**类名称** - 对于所有类名，第一个字母应为大写。如果使用多个单词来形成类的名称，则每个内部单词的第一个字母是大写。
	示例 - `class MyFirstScalaClass`，`class Employee`*类等。
- 
**方法名称 **- 所有方法名称应以小写字母开头。如果使用多个单词形成方法的名称，则每个内部单词的第一个字母应为大写。
	示例 - `def myMethodName()`，`def getName()`等。
- 
**程序文件名 **- 程序文件的名称应与对象名称完全匹配。保存文件时，您应该使用对象名称保存它(记住Scala是区分大小写的)，并将`.scala`附加到文件名称的末尾。 (如果文件名和对象名不匹配，则程序将不会编译)。
*示例* - 假设`HelloWorld`是对象名称。 那么文件应该保存为`HelloWorld.scala`。
- 
**def main(args：Array [String]) **- Scala程序从`main()`方法开始，这是每个Scala程序的强制性部分。
## Scala标识符
所有Scala组件都需要名称。用于对象，类，变量和方法的名称称为标识符。关键字不能用作标识符，标识符区分大小写。Scala支持四种类型的标识符。
**字母数字标识符**
字母数字标识符以字母或下划线开头，后面可以有更多的字母，数字或下划线。 `$`字符是Scala中的保留关键字，不应在标识符中使用。
以下是合法的字母数字标识符 -
```
age, salary, _value,  __1_value
```
以下是非法标识符 -
```
$salary, 123abc, -salary
```
**运算符标识符**
运算符标识符由一个或多个运算符字符组成。运算符字符是可打印的ASCII字符，如：`+`，`:`，`?`，`~`或`#`。
以下是合法运算符标识符 -
```
+ ++ ::: <?> :>
```
Scala编译器将在内部将操作符标识符转换成具有嵌入式`$`字符的合法Java标识符。例如，标识符`:->`将被内部表示为`$colon$minus$greater`。
**混合标识符**
混合标识符由字母数字标识符组成，后面跟着一个下划线和一个操作符标识符。
以下是合法的混合标识符 -
```
unary_+,  myvar_=
```
这里，一元`_+`用作方法名称定义一个一元的`+`运算符，而`myvar_=`用作方法名定义一个赋值运算符(运算符重载)。
**字面识别符**
一个文字标识符是一个随意的字符串，包含在后面的勾号(`...`)中。
以下是合法的文字标识符 -
```
`x` `<clinit>` `yield`
```
### Scala关键字
以下列表显示*Scala*中的保留字(关键字)，这些保留字不能用作常量或变量或任何其他标识符名称。
|abstract|case|catch|class|
|----|----|----|----|
|def|do|else|extends|
|false|final|finally|for|
|forSome|if|implicit|import|
|lazy|match|new|Null|
|object|override|package|private|
|protected|return|sealed|super|
|this|throw|trait|Try|
|true|type|val|Var|
|while|with|yield||
|-|:|=|=>|
|<-|<:|<%|>:|
|#|@|| |
## Scala注释
Scala支持与Java类似的单行和多行注释。多行注释可以嵌套，但需要正确嵌套。注释中任何字符都被Scala编译器忽略。
```
object HelloWorld {
   /* This is my first java program.  
    * This will print 'Hello World' as the output
    * This is an example of multi-line comments.
    */
   def main(args: Array[String]) {
      // Prints Hello World
      // This is also an example of single line comment.
      println("Hello, world!") 
   }
}
```
**空行和空格**
只包含空格的行(可能带有注释)称为空行，Scala完全忽略它。标记可以由空格字符和/或注释分隔。
## 换行字符
Scala是一种面向行的语言，其语句可能以分号(`;`)或换行符终止。语句结尾处的分号(`;`)通常是可选的。如果需要，您可以输入一个分号(`;`)，但如果语句在单行上自行显示，则不需要输入。 另一方面，如果在单行上写多个语句，则需要分号。 下面的语法是多个语句的用法。
```
val s = "hello"; println(s)
```
### Scala包
一个包是一个命名的代码模块。 例如，`Lift`实用程序包是`net.liftweb.util`。包声明是源文件中的第一个非注释行，如下所示：
```
package com.liftcode.stuff
```
导入Scala软件包，以便在当前的编译范围内引用它们。 以下语句导入`scala.xml`包的内容 -
```
import scala.xml._
```
也可以从`scala.collection.mutable`包导入单个类和对象，例如:`HashMap`，
```
import scala.collection.mutable.HashMap
```
可以从单个包导入多个类或对象，例如，来自`scala.collection.immutable`包的`TreeMap`和`TreeSet` -
```
import scala.collection.immutable.{TreeMap, TreeSet}
```
### 应用动态
一个能够进行动态调用的标记特征。此特征的实例`x`允许方法调用`x.meth(args)`用于任意方法名称和参数列表`args`以及对任意字段名称字段的字段访问`x.field`。此功能在*Scala-2.10*中引入。
如果调用不是由本机支持的(即，如果类型检查失败)，则根据以下规则重写：
```
foo.method("blah") ~~> foo.applyDynamic("method")("blah")
foo.method(x = "blah") ~~> foo.applyDynamicNamed("method")(("x", "blah"))
foo.method(x = 1, 2) ~~> foo.applyDynamicNamed("method")(("x", 1), ("", 2))
foo.field ~~> foo.selectDynamic("field")
foo.varia = 10 ~~> foo.updateDynamic("varia")(10)
foo.arr(10) = 13 ~~> foo.selectDynamic("arr").update(10, 13)
foo.arr(10) ~~> foo.applyDynamic("arr")(10)
```
