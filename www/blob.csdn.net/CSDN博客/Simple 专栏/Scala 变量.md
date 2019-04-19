# Scala  变量 - Simple 专栏 - CSDN博客
2018年09月14日 17:00:40[Simple_Zz](https://me.csdn.net/love284969214)阅读数：97
所属专栏：[Scala 入门](https://blog.csdn.net/column/details/27500.html)
变量是保存存储值的内存位置的名称。这意味着当创建变量时，可以在内存中保留一些空间。
根据变量的数据类型，编译器分配内存并决定可以存储在预留内存中的内容。因此，通过为变量分配不同的数据类型，可以在这些变量中存储整数，小数或字符。
## 变量声明
Scala具有用于声明变量的不同语法。它们可以被定义为值，即常数或变量。 这里，`myVar`使用关键字`var`声明。 它是一个可以改变值的变量，这称为可变变量。 以下是使用`var`关键字定义变量的语法 -
**语法**
```
var myVar : String = "Foo"
```
下面示例中，`myVal`使用关键字`val`声明。它是一个不能被改变的变量，这被称为不可变变量。 以下是使用`val`关键字定义变量的语法 -
```
val myVal : String = "Foo"
```
## 可变数据类型
变量的类型在变量名称和等号之前指定。可以通过其数据类型来定义任何类型的Scala变量，如下所示：
```
val or val VariableName : DataType = [Initial Value]
```
如果您没有为变量分配任何初始值，则有效的语法如下：
```
var myVar :Int;
val myVal :String;
```
## 可变类型推断
当为变量分配初始值时，Scala编译器可以根据分配给它的值来推断变量的类型。这被称为变量类型推断。 因此，可以编写这样的变量声明 -
```
var myVar = 10;
val myVal = "Hello, Scala!";
```
默认情况下，`myVar`将会被计算推断为`Int`类型，`myVal`将计算推断为`String`类型变量。
## 多个赋值
Scala支持多个赋值。如果代码块或方法返回一个元组(Tuple - 保持不同类型的对象的集合)，则可以将元组分配给一个`val`变量。
> 
注：我们将在随后的章节学习元组。
```
val (myVar1: Int, myVar2: String) = Pair(40, "Foo")
```
类型推断得到正确的类型 -
```
val (myVar1, myVar2) = Pair(40, "Foo")
```
## 示例程序
以下是一个示例程序，用于说明Scala中变量声明的过程。该程序声明了四个变量 - 两个变量用变量声明定义，其余两个变量没有变量声明。
**例子**
```
object Demo {
   def main(args: Array[String]) {
      var myVar :Int = 10;
      val myVal :String = "Hello Scala with datatype declaration.";
      var myVar1 = 20;
      val myVal1 = "Hello Scala new without datatype declaration.";
      println(myVar); println(myVal); println(myVar1); 
      println(myVal1);
   }
}
```
将上述程序保存在源文件：*Demo.scala*中，使用以下命令用于编译和执行此程序。
```
D:\software\scala-2.12.3\bin>scalac Demo.scala
D:\software\scala-2.12.3\bin>scala Demo
10
Hello Scala with datatype declaration.
20
Hello Scala new without datatype declaration.
D:\software\scala-2.12.3\bin>
```
## 变量的作用域
Scala中的变量可以有三个不同的范围，具体取决于它们被使用的位置。它们可以作为字段存在，作为方法参数和局部变量存在。以下是每种类型范围的详细信息。
**字段**
字段是属于对象的变量。这些字段可以从对象中的每个方法的内部访问。根据字段声明的访问修饰符，也可以在对象外部访问字段。 对象字段可以是可变的和不可变的类型，可以使用`var`或`val`来定义。
**方法参数**
方法参数是在调用该方法时用于传递方法中的值的变量。方法参数只能从方法内部访问，但是如果从方法外部引用了对象，则可以从外部访问传入的对象。方法参数始终是不可变的，由`val`关键字定义。
**局部变量**
局部变量是在方法中声明的变量。局部变量只能从方法内部访问，但如果从方法返回，则您创建的对象可能会转义该方法。局部变量可以是可变的和不可变的类型，可以使用`var`或`val`定义。
