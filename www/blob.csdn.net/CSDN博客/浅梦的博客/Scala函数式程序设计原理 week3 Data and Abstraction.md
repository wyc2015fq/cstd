# Scala函数式程序设计原理 week3 Data and Abstraction - 浅梦的博客 - CSDN博客





2017年09月18日 16:45:41[浅梦s](https://me.csdn.net/u012151283)阅读数：316
个人分类：[Scala](https://blog.csdn.net/u012151283/article/category/7178055)









# 类的层次

## 抽象类

和Java一样，你可以用`abstract`关键字来标记不能被实例化的类，通常这是因为它的某个或某几个方法没有被完整定义。例如：

```
abstract class Person(val name:String){
def id: Int //没有方法体--这是一个抽象方法
}
```

在子类中重写超类的抽象方法时，不需要使用`override`关键字 

抽象类的对象不能通过`new`操作符创建。

## 扩展类

使用`extend`关键字。 

和java一样，可以将类声明成`final`，这样它就不能被扩展。还可以将单个方法或字段声明为final，确保他们不能被重写。 

在java中，final字段是不可变的，类似scala中的val。
## 基类和子类

在scala中，所有用户定义的类都扩展自另一个类。 

如果没有给出超类，java中的包`java.lang`中的标准类`Object`就是默认的超类。 

一个类的直接或间接超类称为基类。
## 实现和重写

在子类中实现超类中的抽象方法。 

或者使用`override`关键字重定义一个已经存在的非抽象的定义。

## 对象定义和单例模式

Scala中没有静态方法或静态字段，可以用`object`语法结构达到同样目的。 

对象的构造器在该对象第一次被使用时调用。 

对象本质上可以拥有类的所有特性—扩展其他类或特质。只有一个例外：不能提供构造器参数 

在Scala中需要对象实现的场景：- 作为存放工具函数或常量的地方
- 高效地共享单个不可变实例
- 需要用单个实例来协调某个服务时（单例模式）

## 应用程序对象

每个Scala程序都必须从一个对象的`main`方法开始，这个方法的类型名为`Array[String]=>Unit`

```
object Hello{
def main(args:Array[string]) = println("hello")
}
```

当编译完成后，使用 
`>scala Hello`即可运行程序。

## 动态绑定

面向对象设计语言实现了动态方法绑定的功能。这意味着引起方法调用的代码取决于运行时包含该方法的对象的类型。

# 类的组织

## 包

要将一个类或对象放到一个包里，使用一个package语句在源代码的头部。

```
package progfun.examples
object Hello {...}
```

这样就将`Hello`放到了包`progfun.examples`中。 

可以使用完整限定名访问`progfun.examples.Hello`。

## 引入

```java
import week3.Rational
import week3.{Rational,Hello}
import wekk3._
```

最后一个是通配引入

## 自动引入

一些实体被自动引入到所有Scala程序中。
- 包scala的所有成员
- 包java.lang的所有成员
- 包scala.Predef的单例对象

## Scaladoc

www.scala-lang.org/api/current

## 特质

在Java和Scala中，一个类只能又一个超类。 

特质的声明和抽象类类似，关键字是`trait`。

```
trait Planar{
   def height:Int
   def width:Int
   def surface = height * width
   }
```

**类，对象，特质可以继承最多一个类但是任意多的特质。**

例如
`class Square extends Shape with Planar with Movable`
特质类似于java中的接口，但是功能更强大，因为他们可以包括字段和具体方法。 

另一方面，特质不能有值参数，只有类可以有。

## 顶层类型

![这里写图片描述](https://img-blog.csdn.net/20170918160920286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170918161415006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Nothing类型

`Nothing`是所有其他类型的子类，他没有值。 

用来标记异常终止，作为空集合的元素类型。

## 异常
`throw Exc`
这个表达式的类型是`Nothing`

## Null类型

每个引用类类型有一个`null`值。 
`null`值的类型是`Null`。 
`Null`是所有继承自`Object`的类的子类，它和`AnyVal`的子类不兼容。
```
val x = null//x : Null
val y:String = null // y: String
val z: Int = null //error:type mismatch
```

# 多态

## 类型参数

类和特质可以带类型参数。在Scala中，用方括号定义类型参数例如
`class Pair[T,S] (val first:T,val second:s)`
在类的定义中，可以用类型参数来定义变量，方法参数，以及返回值的类型。 
**带有一个或多个类型参数的类是泛型的**，Scala会从构造参数推断出实际类型。

```
val p = new Pair(42,'String')//Pair[Int,String]
val p2 = new Pair[Any,Any](42,'String')//自己指定
```

泛型函数，类型参数放在方法名之后。
`def getMiddle[T](a:Array[T])=2`
## 类型擦除

类型参数不影响取值。 

可以认为所有类型参数在程序取值前都被移除。 

java,Scala，Haskell，ML都是这样的。 

一些其他语言保留类型参数直到运行时，如c++,c#,f#
## 多态
- 函数可以应用到多种类型的参数
- 
类型可以有多种实例类型 

两个主要原则：

- 
子类：子类的实例可以传递给基类

- 泛型：函数或类的实例可以通过参数化的类创建









