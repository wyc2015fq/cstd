# 重拾C#教程：多态性 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月10日 13:04:06[boonya](https://me.csdn.net/boonya)阅读数：48








原文地址：[http://www.runoob.com/csharp/csharp-polymorphism.html](http://www.runoob.com/csharp/csharp-polymorphism.html)




**多态性**意味着有多重形式。在面向对象编程范式中，多态性往往表现为"一个接口，多个功能"。

多态性可以是静态的或动态的。在**静态多态性**中，函数的响应是在编译时发生的。在**动态多态性**中，函数的响应是在运行时发生的。


## 静态多态性

在编译时，函数和对象的连接机制被称为早期绑定，也被称为静态绑定。C# 提供了两种技术来实现静态多态性。分别为：
- 函数重载
- 运算符重载

运算符重载将在下一章节讨论，接下来我们将讨论函数重载。

## 函数重载

您可以在同一个范围内对相同的函数名有多个定义。函数的定义必须彼此不同，可以是参数列表中的参数类型不同，也可以是参数个数不同。不能重载只有返回类型不同的函数声明。

下面的实例演示了几个相同的函数 **print()**，用于打印不同的数据类型：
usingSystem;namespacePolymorphismApplication{classPrintdata{voidprint(int i){Console.WriteLine("Printing int: {0}", i );}voidprint(double f){Console.WriteLine("Printing float: {0}", f);}voidprint(string s){Console.WriteLine("Printing string: {0}", s);}staticvoidMain(string[] args){Printdata p =newPrintdata();// 调用 print 来打印整数
         p.print(5);// 调用 print 来打印浮点数
         p.print(500.263);// 调用 print 来打印字符串
         p.print("Hello C++");Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
Printingint:5Printingfloat:500.263Printingstring:Hello C++
## 动态多态性

C# 允许您使用关键字 **abstract** 创建抽象类，用于提供接口的部分类的实现。当一个派生类继承自该抽象类时，实现即完成。**抽象类**包含抽象方法，抽象方法可被派生类实现。派生类具有更专业的功能。

请注意，下面是有关抽象类的一些规则：
- 您不能创建一个抽象类的实例。
- 您不能在一个抽象类外部声明一个抽象方法。
- 通过在类定义前面放置关键字 **sealed**，可以将类声明为**密封类**。当一个类被声明为 **sealed** 时，它不能被继承。抽象类不能被声明为 sealed。

下面的程序演示了一个抽象类：
usingSystem;namespacePolymorphismApplication{abstractclassShape{publicabstractint area();}classRectangle:Shape{privateint length;privateint width;publicRectangle(int a=0,int b=0){
         length = a;
         width = b;}publicoverrideint area (){Console.WriteLine("Rectangle 类的面积：");return(width * length);}}classRectangleTester{staticvoidMain(string[] args){Rectangle r =newRectangle(10,7);double a = r.area();Console.WriteLine("面积： {0}",a);Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
Rectangle类的面积：面积：70
当有一个定义在类中的函数需要在继承类中实现时，可以使用**虚方法**。虚方法是使用关键字 **virtual** 声明的。虚方法可以在不同的继承类中有不同的实现。对虚方法的调用是在运行时发生的。

动态多态性是通过 **抽象类** 和 **虚方法** 实现的。

下面的程序演示了这点：
usingSystem;namespacePolymorphismApplication{classShape{protectedint width, height;publicShape(int a=0,int b=0){
         width = a;
         height = b;}publicvirtualint area(){Console.WriteLine("父类的面积：");return0;}}classRectangle:Shape{publicRectangle(int a=0,int b=0):base(a, b){}publicoverrideint area (){Console.WriteLine("Rectangle 类的面积：");return(width * height);}}classTriangle:Shape{publicTriangle(int a =0,int b =0):base(a, b){}publicoverrideint area(){Console.WriteLine("Triangle 类的面积：");return(width * height /2);}}classCaller{publicvoidCallArea(Shape sh){int a;
         a = sh.area();Console.WriteLine("面积： {0}", a);}}classTester{staticvoidMain(string[] args){Caller c =newCaller();Rectangle r =newRectangle(10,7);Triangle t =newTriangle(10,5);
         c.CallArea(r);
         c.CallArea(t);Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
Rectangle类的面积：面积：70Triangle类的面积：面积：25
## C# 多态性

多态：一个接口多个功能。

静态多态性：编译时发生函数响应（调用）；

动态多态性：运行时发生函数响应。

静态绑定（早期绑定）：编译时函数和对象的连接机制。

两种技术实现静态多态性：函数重载/运算符重载。

函数重载：在同一范围内对相同函数名有多个定义，可以是参数类型或参数个数的不同，但不许只有返回值类型不同。

运算符重载：

关键字 abstract 声明抽象类：用于接口部分类的实现（派生类继承抽象类时，实现完成）。抽象类包含抽象方法，抽象方法可被派生类实现。

抽象类规则：
-  1.不能创建抽象类的实例
- 2.不能在抽象类外定义抽象方法
- 3.不能把抽象类声明为sealed（类前带关键字sealed代表该类是密封类，不能被继承）

关键字virtual声明虚方法:用于方法在继承类中的实现（在不同的继承类中有不同的实现）。

抽象类和虚方法共同实现动态多态性。

注：继承类中的重写虚函数需要声明关键字 override，在方法参数传入中写（类名 形参名）例如 public void CallArea(Shape sh)，意思是传入一个 shape 类型的类。

## virtual 和 abstract

virtual和abstract都是用来修饰父类的，通过覆盖父类的定义，让子类重新定义。
-  1.virtual修饰的方法必须有实现（哪怕是仅仅添加一对大括号),而abstract修饰的方法一定不能实现。
-  2.virtual可以被子类重写，而abstract必须被子类重写。
-  3.如果类成员被abstract修饰，则该类前必须添加abstract，因为只有抽象类才可以有抽象方法。
-  4.无法创建abstract类的实例，只能被继承无法实例化。

重载和重写




重载(overload)是提供了一种机制, 相同函数名通过不同的返回值类型以及参数来表来区分的机制。

> 
见：[函数重载](http://www.runoob.com/csharp/csharp-polymorphism.html#overload)


重写(override)是用于重写基类的虚方法,这样在派生类中提供一个新的方法。

> 
见：[动态多态性](http://www.runoob.com/csharp/csharp-polymorphism.html#override)





