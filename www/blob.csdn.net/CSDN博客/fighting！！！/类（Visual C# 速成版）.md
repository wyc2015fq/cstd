# 类（Visual C# 速成版） - fighting！！！ - CSDN博客
2017年11月14日 20:29:51[dujiahei](https://me.csdn.net/dujiahei)阅读数：266
                
类（Visual C# 速成版）
https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx
# 类（Visual C# 速成版）
**Visual Studio 2008**
更新：2007 年 11 月
C# 是一种面向对象的编程语言，与其他现代语言具有相同点。C# 将相关字段、方法、属性和事件分组到称为“类”的数据结构中。
类与对象
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_0)
从根本上而言，类就是自定义数据类型的蓝图。定义类之后，便可通过将类加载到内存中来使用它。已加载到内存中的类称为“对象”或“实例”。可以通过使用 C# 关键字 [new](https://msdn.microsoft.com/zh-cn/library/51y09td4(v=vs.90).aspx) 来创建类的实例。
下面的示例定义一个称为 SampleClass 的类，并创建该类的一个实例，即一个称为 sampleClass1 的对象。因为 C# 要求在类中定义 Main 函数，所以下面的代码还定义一个 Program 类，但是该类并不用于创建对象。
C#
using System;
class SampleClass
{
    publicvoid SayHello()
    {
        Console.WriteLine("Hello, World!");
    }
}
class Program
{
    //Main is the entrypoint, where every C# program startsstaticvoid Main(string[] args)
    {
        SampleClass sampleClass1 = new SampleClass();  // Create an object
        sampleClass1.SayHello();                       // Call a method
    }
}
正如您可以根据同一蓝图建造任意数量的房屋一样，您可以实例化同一个类的任何数量的对象。数组或列表包含同一个类的许多对象的情况很常见。类的每一个实例都占用单独的内存空间，并且其字段（如下所述的静态字段除外）的值也是相互分离、相互独立的。在下面的代码示例中，您可以创建一个 Animal 类型的对象并将其大小设置为 2，然后可以创建另一个大小设置为
 3 的对象。但是，此规则有一个重要例外，那就是“静态成员”。
静态成员与实例成员
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_1)
静态成员是无需引用类的特定实例即可访问的方法或字段。最常见的静态方法是 Main，它是所有 C# 程序的入口点；注意，您不必为调用 Main 方法而创建包含它的类的实例。另一个常用的静态方法是 [Console](https://msdn.microsoft.com/zh-cn/library/system.console(v=vs.90).aspx) 类中的 WriteLine。请注意访问这些静态方法时的语法差别；在点运算符的左边您使用的是类名称而不是实例名称：Console.WriteLine。
当您将一个类字段声明为静态时，该类的所有实例将“共享”该字段。如果将下面的代码示例中的 size 声明为静态的，而一个 Animal 对象更改了该字段的值，则对于 Animal 类型的所有对象，该值都将改变。
静态类是其成员均为静态的类。出于性能和效率方面的考虑，在某些情况下，静态类、方法和字段非常有用。但是，如果您假定一个字段为实例字段，而实际上却是静态字段时，可能会引发难以觉察的错误。有关更多信息，请参见[静态类和静态类成员（C# 编程指南）](https://msdn.microsoft.com/zh-cn/library/79b3xss3(v=vs.90).aspx)。
类与文件
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_2)
每个 C# 程序都至少包含一个类。设计程序时，将一个类保存在一个源代码 (.cs) 文件中是很好的编程习惯，但不是必须这么做。如果使用 C# 集成开发环境创建类，则该环境会同时自动创建一个新的源代码文件。
封装
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_3)
类通常表示某个事物的特性，以及该事物可执行的操作。例如，要将一个动物表示为 C# 类，您可以用数字来表示该动物的大小、速度和力量，并提供一些函数（如 MoveLeft()、MoveRight()、SpeedUp()、Stop() 等），在这些函数中写入代码以使“动物”执行这些动作。在
 C# 中，该动物类的代码可如下所示：
C#
publicclass Animal
    {
        privateint size;
        privatefloat speed;
        privateint strength;
	    publicvoid MoveLeft()  // method
	    {
		    // code goes here...
        }
        // other methods go here...
    }
浏览 .NET Framework Class Library 时，您将看到每个类都表示一个“事物”（如，一个 [XmlDocument](https://msdn.microsoft.com/zh-cn/library/system.xml.xmldocument(v=vs.90).aspx)、一个 [String](https://msdn.microsoft.com/zh-cn/library/system.string(v=vs.90).aspx) 和一个 [Form](https://msdn.microsoft.com/zh-cn/library/system.windows.forms.form(v=vs.90).aspx)），并且每个事物都具有不同的、可以执行的操作（方法），可读取并且也许可以修改的特性（属性）和在执行某项给定操作时发出的通知（事件）。方法、属性、事件，以及所有其他内部变量和常数（字段）都称为类的成员。
将成员一起组合到类中的过程不仅是逻辑上的，同时它还让您可以隐藏那些您不希望其他代码访问的数据和函数。此原则称为“封装”。浏览 .NET Framework 类库时，您只能看到这些类的公共成员。此外，每个类可能还具有该类或与之相关的类在内部使用的私有成员，但并不意味着可以供应用程序使用。创建自己的类时，您将确定哪些成员应为公共成员，哪些成员应为私有成员。
继承
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_4)
类可以继承其他类，这意味着该类包含原始类的所有成员（公共的和私有的）以及该类所定义的其他成员。原始类称为“基类”，而新类称为“派生类”。您可以创建一个派生类来表示基类的一种更为具体的事物。例如，您可以定义一个从 Animal 继承的类 Cat。Cat 可以做 Animal 能够做的所有事情，此外，它还可以执行一个独有的附加操作。相应的
 C# 代码如下所示：
C#
publicclass Cat : Animal
    {
	    publicvoid Purr()
	    {
	    }
    }
Cat : Animal 表示法指示 Cat 从 Animal 继承，因此 Cat 也具有一个 MoveLeft 方法和三个私有变量 size、speed 和 strength。如果定义一个从 Cat继承的 SiameseCat 类，它将具有 Cat 的所有成员，以及 Animal 的所有成员。
多态性
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_5)
在计算机编程领域中，“多态性”指的是派生类重新定义或“重写”它从基类继承的方法的能力。当您需要在一个方法中做一些与基类方法中不同的或在基类方法中没有定义的特定事情，此时就会使用到多态性。例如，由于 Animal.MoveLeft 方法必须非常宽泛以适用于所有动物，因此该方法可能会非常简单：类似于“更改方向以使动物的头现在指向方向
 X”。但是，在 Cat 类中，这样可能还不够。您可能需要指定 Cat 在转向时移动其爪子和尾巴的方式。如果您同时定义了 Fish 类或 Bird 类，则还可能需要以不同的方式为这两个类中的每一个重写 MoveLeft 方法。因为您可以为特定的类自定义 MoveLeft 方法的行为，所以用于创建这样一个类并调用其方法的代码不必针对自然界中的每种动物都单独定义一个方法。只要对象从Amimal 继承，则调用代码只需调用 MoveLeft 方法，就可以调用属于对象自己的版本的方法。
构造函数
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_6)
每个类都具有一个构造函数：一个与类共享相同名称的方法。根据类定义创建对象时会调用构造函数。构造函数通常设置在类中定义的变量的初始值。如果初始值为数值数据类型值 0、布尔型数据类型值 false 或引用类型值 null，则不需要调用构造函数设置初始值，因为这些数据类型会自动初始化。
您可以使用任意数量的参数来定义构造函数。没有参数的构造函数称为“默认构造函数”。 下面的示例使用默认构造函数和采用一个字符串参数的构造函数定义类，然后分别使用：
C#
class SampleClass
{
    string greeting;
    public SampleClass()
    {
        greeting = "Hello, World";
    }
    public SampleClass(string message)
    {
        greeting = message;
    }
    publicvoid SayHello()
    {
        System.Console.WriteLine(greeting);
    }
}
class Program
{
    staticvoid Main(string[] args)
    {
        // Use default constructor.
        SampleClass sampleClass1 = new SampleClass();
        sampleClass1.SayHello();
        // Use constructor that takes a string parameter.
        SampleClass sampleClass2 = new SampleClass("Hello, Mars");
        sampleClass2.SayHello();
    }
}
运算符重载
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_7)
使用相同名称（在上面的示例中为 SampleClass()）创建不同方法的这一过程称为重载。编译器知道要使用哪种方法，因为每次创建对象时都会提供参数列表（如果有）。重载可以增强代码的灵活性和可读性。
析构函数
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_8)
如果您使用过 C++，则您可能已经了解析构函数。因为 C# 中的自动垃圾回收系统，所以不可能出现必须实现一个析造函数的情况，除非类使用的是非托管资源。有关更多信息，请参见 [析构函数（C# 编程指南）](https://msdn.microsoft.com/zh-cn/library/66x5fx1b(v=vs.90).aspx)。
结构
[](https://msdn.microsoft.com/zh-cn/library/ey4ke239(v=vs.90).aspx#Anchor_9)
除不支持继承以外，结构是一种在许多方面都与类相似的类型。有关更多信息，请参见[结构（Visual C# 速成版）](https://msdn.microsoft.com/zh-cn/library/89892kc7(v=vs.90).aspx)。

