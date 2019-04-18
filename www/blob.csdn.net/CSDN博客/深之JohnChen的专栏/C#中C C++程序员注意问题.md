# C#中C/C++程序员注意问题 - 深之JohnChen的专栏 - CSDN博客

2010年01月14日 21:27:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：11379


1、C/C++程序员请注意，不能在case语句不为空时“向下执行”。

2、值类型和引用类型之间的区别：C#的基本类型（int，char等）都是值类型，是在栈中创建的。而对象是引用类型，创建于堆中，需要使用关键字new。

3、在C#中通过实例访问静态方法或成员变量是不合法的，会生成编译器错误。但是我们可以通过声明他们的类的名字来发访问静态成员。

4、C#派生类中，没有私有或者保护继承。关于这一点C++程序员注意了。

5、C#多态性,C++程序员注意,必须显式用关键自override标记重新定义了虚方法的方法声明。

6、抽象类与密封类，抽象类用来被派生，并为其子类提供遵循模版；而密封类则完全不允许被派生。抽象类关键字是abstract；密封类关键字为sealed

7、C#中结构体的定义与C++的非常不同。在C++中，结构体与类除了默认的可见性（一个是public，一个是privae）之外完全一样。而在C#中，结构体是值类型，而类是引用类型。

8、C#中委托相当于C++中的方法指针，关键字为delegate。

9、.NET用来创建网页程序的技术是ASP.NET2.0，它在命名空间System.Web和System.Web.UI中为创建网页程序提供了一套丰富的类型。

10、C#线程三种同步机制：互锁类（Interlocked），C# lock语句，以及监控器对象（Monitor class）

11、C#中要使用值变量之前，需要事先声明，并且要在使用之前先设置初始值，永远不可为null；引用类型变量，存储在heap中，不允许在初始化之前使用这个变量。

12、C#中switch-case语句与C/C++不同地方在于C#中不允许case判断式“silent fall through”。也就是说，当找到符合case标记后的常数后，只会执行此case区块中的程序代码，不会再执行其他的case判断式进行判断的动作。

13、参数传入机制

![](http://i3.6.cn/cvbnm/df/62/13/e4123c0fbb94ba9b3db8cead52a53cf6.jpg)

14、C#中使用数组要注意两件事情：数组名称不可置于中括号前方；声明数组变量时，不能够指定数组的大小。

15、类

C#的成员访问修饰符包含以下几种：

Public

任何外部的类都可以不受限制的存取这个类的方法和数据成员。

Private

类中的所有方法与数据成员只能在此类中使用，外部无法存取。

Protected

除了让本身的类可以使用之外，任何继承自此类的子类都可以存取。

Internal

在同一个.NET Assembly（Application或Library，也就是说，同一个EXE或同一个DLL中的成员）中都可以存取。

Protected internal

只限定目前的项目，或是继承自此类的成员才可以存取。

结构与类的差异：

![](http://i3.6.cn/cvbnm/ac/ff/42/7c99a7eb543f77ccf6ad73f7d55682ac.jpg)

16、值类型与引用类型之间的互换称为Boxing与Unboxing

17、在C#中，对象都是使用new保留字建立的，建立对象时分两个步骤：

一、向系统要求内存。

二、调用构造器进行初始化。

18、C#中多态性，虚拟方法用来指明扩展类可以改写基础类方法的实现部分。多态性用override、new来实现，在C++中用virtual来实现。

Override方法则是应用在扩展类当中，可使用override保留字指明欲改写基础类的virtual成员之实现部分。换言之，virtual方法是在基础类中定义的，而override是在扩展类中定义的。

方法声明前加上了virtual 修饰符，我们称之为虚方法，反之为非虚。使用了virtual 修饰符后，不允许再有static, abstract, 或override 修饰符。

示例：

class B 

{ public virtual void foo () {} }

class D : B

{

public override void foo () {}

}

class N : D 

{

public new void foo () {}

public static void Main() {

N n = new N ();

n.foo( ) ; // 调用N的foo

((D)n).foo( ) ; // 调用D的foo

((B)n).foo( ) ; // 调用D的foo

}

}

19、接口（Interface）和类（Class）非常相似，唯一的不同点在于接口中不包含任何实现程序代码。注意：定义在接口中的方法隐含都是属于public，因此不能够在方法之前加public修饰符。

20、attribute属性

Attribute不过是一个声明式的标记，能在执行时期将一些信息，如类、数据结构、enum以及assembly等信息，传送给.NET执行时期引擎，以动态个更改对象执行时期的行为。不妨将attribute想象成一个可供程序保存、然后使用的注释。

21、反射

反射提供了封装程序集、模块和类型的对象（Type 类型）。可以使用反射动态创建类型的实例，将类型绑定到现有对象，或从现有对象获取类型并调用其方法或访问其字段和属性。如果代码中使用了属性，可以利用反射对它们进行访问。

反射在下列情况下很有用：

当需要访问程序元数据中的属性时。请参见主题使用反射访问属性。

检查和实例化程序集中的类型。

在运行时构建新类型。使用 System.Reflection.Emit 中的类。

执行后期绑定，访问在运行时创建的类型的方法。请参见主题动态加载和使用类型。

[http://msdn.microsoft.com/zh-cn/library/ms173183.aspx](http://msdn.microsoft.com/zh-cn/library/ms173183.aspx)


