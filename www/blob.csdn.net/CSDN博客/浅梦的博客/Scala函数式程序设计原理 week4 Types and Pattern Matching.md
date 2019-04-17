# Scala函数式程序设计原理 week4 Types and Pattern Matching - 浅梦的博客 - CSDN博客





2017年09月18日 17:59:16[浅梦s](https://me.csdn.net/u012151283)阅读数：272
个人分类：[Scala](https://blog.csdn.net/u012151283/article/category/7178055)









# 函数作为对象

事实上，类型`A=>B`是类`scala.Function[A,B]`的缩写，定义如下

```python
package scala
trait Function[A,B]{
def apply(x:A):B
}
```

所以函数是带有`apply`方法的对象。 

匿名函数
`(x:Int)=>x*x`
可以展开为

```
{
class AnonFun extends Function[Int,Int]{
  def apply(x: Int) = x * x
}
new AnonFun
}
```

或者，使用匿名类语法

```
new Function[Int,Int]{
def apply(x:Int) = x * x
}
```

# 类型变量界定
`def assertAllPos[S<:IntSet](r:S):S=...`
`<: IntSet`是类型参数S的上届。意味着S只能是IntSet的子类型。 

混合界定
`[S>:NonEmpty <: Intset]`
# 型变

通常来说，一种能够接受元素改变的类型是不应该协变的。 

但是不可变类型可以协变，只要方法满足一些条件。 
![这里写图片描述](https://img-blog.csdn.net/20170918173514440?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 函数的子类型化规则

一般来说，如果A2 <: A1并且B1 <: B2 

那么，A1=>B1 <: A2 => B2 

函数和他们的参数是逆变的，和返回类型是协变的。 

这引出了如下的修正定义
```python
package scala
trait Function[-T,+U]{
  def apply(x:T): U
}
```

# 析构

# 匹配语法

## match表达式
- `match`后跟随一系列`cases`，pat => expr
- 每一个样例关联一个表达式和模式
- 若没有匹配到的模式会抛出`MatchError`异常

```php
e match{
case pat => expr
case pat => expt
case _ if expr => expr
case _ => expt
}
```

`case _` 捕获所有。 

match是表达式，不是语句。 

模式可以有以下几种类型构成- 构造器
- 变量
- 通配符
- 常数

变量必须以小写字母开头，常量以大写字母开头，除了保留字null,true,false

## 守卫

守卫可以是任何Boolean条件

## 样例类

样例类是一种特殊的类，它们经过优化以被用于模式匹配。

```
abstract class Amount
case class Dollar(value:Double) extends Amount
case class Currency(value:Double,unit:String) extends Amount
```

当有一个类型为Amount的对象时，可以用模式匹配来匹配到它的类型，并将属性值绑定到变量。

```
amt match{
case Dollar(v) => "$" + v
case Currency(_,u) => "I got" + u
case Nothing => ""
}
```

# 偏函数

被包在括号内的一组case语句是一个**偏函数**，一个并非对所有输入值都有定义的函数。他是`PartialFunction[A,B]`的一个实例。该类有两个方法，apply方法从匹配到的模式计算函数值，而isDefinedAt方法在输入至少匹配其中一个模式时返回true。

```
trait PartialFunction[-A,+R] extends Function[-A,+B]{
def apply(x:A)R
def isDefinedAt(x:A):Boolean
}
```

![这里写图片描述](https://img-blog.csdn.net/20170920095102259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170920095317389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上面这个例子结果为`true`。`isDefinedAt`只保证偏函数最外层的模式匹配块可以匹配，但不保证不会抛出`matcherror`。






