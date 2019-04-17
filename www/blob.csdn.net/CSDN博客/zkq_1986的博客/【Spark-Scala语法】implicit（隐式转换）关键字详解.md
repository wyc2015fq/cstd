# 【Spark-Scala语法】implicit（隐式转换）关键字详解 - zkq_1986的博客 - CSDN博客





2017年12月19日 14:41:33[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：346








implicit关键字，告诉程序从上下文获取相关的变量或函数。包括隐式参数、隐式转换、隐式函数。




1.隐式参数




当我们在定义方法时，可以把最后一个参数列表标记为implicit，表示该组参数是隐式参数。一个方法只会有一个隐式参数列表，置于方法的最后一个参数列表。如果方法有多个隐式参数，只需一个implicit修饰即可。 当调用包含隐式参数的方法是，如果当前上下文中有合适的隐式值，则编译器会自动为改组参数填充合适的值。如果没有编译器会抛出异常。当然，标记为隐式参数的我们也可以手动为该参数添加默认值。def foo(n: Int)(implicit t1: String, t2: Double = 3.14)

```python
scala > def calcTax(amount: Float)(implicit rate: Float): Float = amount * rate
scala > implicit val currentTaxRate = 0.08F
scala > val tax = calcTax(50000F) // 4000.0
```
- 1
- 2
- 3


如果编译器在上下文没有找到第二行代码会报错

### 2.隐式地转换类型


使用隐含转换将变量转换成预期的类型是编译器最先使用 implicit 的地方。这个规则非常简单，当编译器看到类型X而却需要类型Y，它就在当前作用域查找是否定义了从类型X到类型Y的隐式定义 

例子：

```
scala> val i: Int = 3.5 //直接报错
加上这句：
scala> implicit def double2Int(d: Double) = d.toInt
再运行，没报错
scala> val i: Int = 3.5  //i=3
```
- 1
- 2
- 3
- 4
- 5
- 6

### 3.隐式调用函数


隐式调用函数可以转换调用方法的对象，比如但编译器看到X .method，而类型 X 没有定义 method（包括基类)方法，那么编译器就查找作用域内定义的从 X 到其它对象的类型转换，比如 Y，而类型Y定义了 method 方法，编译器就首先使用隐含类型转换把 X 转换成 Y，然后调用 Y 的 method。 

例子：

```
class SwingType{
  def  wantLearned(sw : String) = println("兔子已经学会了"+sw)
}
object swimming{
  implicit def learningType(s : AminalType) = new SwingType
}
class AminalType
object AminalType extends  App{
  import swimming._
  val rabbit = new AminalType
    rabbit.wantLearned("breaststroke")         //兔子已经学会了breaststroke
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12


上例中编译器在rabbit对象调用时发现对象上并没有wantLearned方法，此时编译器就会在作用域范围内查找能使其编译通过的隐式视图，找到learningType方法后，编译器通过隐式转换将对象转换成具有这个方法的对象，之后调用wantLearned方法。




转载自：http://blog.csdn.net/jameshadoop/article/details/52337949



