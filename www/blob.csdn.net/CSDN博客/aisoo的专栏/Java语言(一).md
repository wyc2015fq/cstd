# Java语言(一) - aisoo的专栏 - CSDN博客
2009年11月10日 10:54:00[aisoo](https://me.csdn.net/aisoo)阅读数：300标签：[java																[语言																[class																[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
 一  类 
     类是java中的一种重要的复合数据类型，是组成java程序的基本要素。它封装了一类对象的状态和方法，是这一类对象的原形。一个类的实现包括两个部分：类声明和类体
1．类声明：
　　[public][abstract|final] 
class className [extends superclassName] [implements 
interfaceNameList]
　　{……}
　　其中，修饰符public,abstract,final 
说明了类的属性，className为类名，superclassName为类的父类的名字，interfaceNameList为类所实现的接口列表。
　2．类体
　　类体定义如下：
　　class 
className
　　{[public | protected | private ] [static] 
　　[final] 
[transient] [volatile] 
type
　　variableName;　　　　　　　　　　　　　　　　　//成员变量
　　[public | protected | 
private ] [static]
　　[final | abstract] [native] 
[synchronized]
　　returnType methodName([paramList]) [throws 
exceptionList]
　　　{statements} 　　　　　　　　　　　　　　　 
//成员方法
　　}
　3．成员变量
　　成员变量的声明方式如下：
　　[public | protected | private ] 
[static] 
　　[final] [transient] [volatile] type
　　variableName; 
　　　　　　　　　　　　　　　　//成员变量
　　其中，
　　static: 静态变量（类变量）；相对于实例变量
　　final: 
常量
　　transient: 暂时性变量，用于对象存档，用于对象的串行化，见对象的串行化一节
　　volatile: 
贡献变量，用于并发线程的共享
　4．成员方法
　　方法的实现包括两部分内容：方法声明和方法体。
　　[public | protected | 
private ] [static]
　　[final　| abstract] [native] 
[synchronized]
　　returnType methodName([paramList])
　　[throws 
exceptionList] 　　　　　　　　　　　//方法声明
　　　{statements} 
　　　　　　　　　　　　　　　//方法体
　　方法声明中的限定词的含义：
　　static: 
类方法，可通过类名直接调用
　　abstract: 抽象方法，没有方法体
　　final: 方法不能被重写
　　native: 
集成其它语言的代码
　　synchronized: 控制多个并发线程的访问
　　◇ 
方法声明
　　方法声明包括方法名、返回类型和外部参数。其中参数的类型可以是简单数据类型，也可以是复合数据类型（又称引用数据类型）。
　　对于简单数据类型来说，java实现的是值传递，方法接收参数的值，但不能改变这些参数的值。如果要改变参数的值，则用引用数据类型，因为引用  数据类型传递给方法的是数据在内存中的地址，方法中对数据的操作可以改变数据的值。
  ◇ 
方法体
　　方法体是对方法的实现，它包括局部变量的声明以及所有合法的Java指令。方法体中声明的局部变量的作用域在该方法内部。若局部变量与类的成员变量同名，则类的成员变量被隐藏。
为了区别参数和类的成员变量，我们必须使用this。this-----用在一个方法中引用当前对象，它的值是调用该方法的对象。返回值须与返回类型一致，或者完全相同，或是其子类。当返回类型是接口时，返回值必须实现该接口。
  5．方法重载
　方法重载是指多个方法享有相同的名字，但是这些方法的参数必须不同，或者是参数的个数不同，或者是参数类型不同。返回类型不能用来区分重载的方法。
　　参数类型的区分度一定要足够，例如不能是同一简单类型的参数，如int与long。编译器会根据参数的个数和类型来决定当前所使用的方法。
 6． 
构造方法
　　◇ 构造方法是一个特殊的方法。Java 中的每个类都有构造方法，用来初始化该类的一个对象。
　　◇ 
构造方法具有和类名相同的名称，而且不返回任何数据类型。
　　◇ 重载经常用于构造方法。
　　◇ 构造方法只能由new运算符调用
