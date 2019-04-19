# C#面向对象：组件、程序集与反射 - 文章 - 伯乐在线
原文出处： [老张的菠菜](http://blog.csdn.net/zhangchen124/article/details/51287715)
# 组件
组件技术是软件领域一项非常大的成就。组件技术像搭积木游戏一样，在搭积木时，我们知道并不是任何的两个积木都可以组合，能组合的积木之间必定存在可以衔接的接口，同时，已经搭好的积木可以作为一个整体的大积木，被用来与其他积木组合。
在软件领域，这些“积木”模型有微软的COM，Sun的JavaBean、.Net的程序集等。
组件可以提高子程序的重用性和灵活性，并便于子程序的发布。也可以节省开发时间，提高程序的可靠性。
例如，Shapes类包含代表圆、三角形或其他图形的对象。它的方法可用于计算图形面积或执行图形的其他操作。许多程序都要使用Shape类，完成如下功能：加色/绘图程序、Visio/PPT/Word画图、建筑/施工设计、CAD、游戏及其他程序。
如果把Shapes设计成组件，仅定义一次，被在多个程序重用性，岂不美哉？（注意：不是在多个程序中Ctrl+C、Ctrl+V该类，这就太代码工人了。）
在.Net编程模型中，我们使用程序集来实现组件技术的使用。
# 程序集
对于.Net来讲，无论C#、VB、J#那种语言的应用程序编译的最终结果都是一个程序集。编译而成的程序集包括我们提到的Exe文件或DLL文件。.Net编译生成的Exe文件和常规我们见的二进制可执行文件（exe文件,比如说QQ.exe）是不一样的，它是由中间语言(IL)组成的可执行文件，必须由CLR来执行。
程序集具有以下特点：
· 程序集作为 .exe 或 .dll 文件实现。
· 通过将程序集放在全局程序集缓存中，可在多个应用程序之间共享程序集。
· 要将程序集包含在全局程序集缓存中，必须对程序集进行强命名。
· 程序集仅在需要时才加载到内存中。
· 可以使用反射以编程方式获取关于程序集的信息。
创建与使用程序集
第1步：建立控制台项目，给项目改名为“CalcShapes”,如下图：
![1](http://jbcdn2.b0.upaiyun.com/2016/06/28c8edde3d61a0411511d3b1866f06364.jpg)
![2](http://jbcdn2.b0.upaiyun.com/2016/06/665f644e43731ff9db3d341da5c827e14.jpg)
第3步：查看解决方案，并修改Shapes类库项目中的“Class1.cs”名为“Circle.cs”
![3](http://jbcdn2.b0.upaiyun.com/2016/06/38026ed22fc1a91d92b5d2ef93540f203.jpg)
第4步：编写Circle类的代码:

C#
```
usingSystem;
usingSystem.Collections.Generic;
usingSystem.Text;
namespace Shapes
{
public classCircle
{
double radius;
public doubleRadius
{
get{ return radius; }
set{ radius = value; }
}
public Circle()
{
radius = 0;
}
public Circle(doublegivenRadius)
{
radius = givenRadius;
}
public doublegetArea()
{
returnSystem.Math.PI * (radius * radius);
}
}
}
```
第5步：在控制台程序使用Shapes项目中的程序集,在CalcShapes项目的“引用”上右键单击“添加引用”，然后弹出下面的对话框，选择“项目”选项卡，选择“Shapes”项目，然后点击“确定”按钮即可。
第6步：在控制台程序中编写代码，并查看运行效果；

C#
```
usingSystem;
usingSystem.Collections.Generic;
usingSystem.Text;
usingShapes; //1.引用命名空间;
namespaceCalcShapes
{
class Program
{
static voidMain(string[]args)
{
Circlecircle1 = new Circle();
circle1.Radius = 3.3; //设置圆周率
Console.WriteLine(circle1.getArea().ToString()); //现实圆面积
}
}
}
```
> 
项目1：使用Winform来添加Shapes类引用
项目背景：针对我们已经写好的程序集文件，方便我们在以后在各种程序中使用，所以，我们应该从各个方面将该组件淋漓尽致使用的“压榨干净”。在上述基础上，添加新项目
解决方案：学习添加Winform项目
步骤1：添加Winform新项目到刚才的项目,如下图：
![6](http://jbcdn2.b0.upaiyun.com/2016/06/3d2f8900f2e49c02b481c2f717aa90203.jpg)
![0](http://jbcdn2.b0.upaiyun.com/2016/06/dcfcd07e645d245babe887e5e2daa016.jpg)
![11](http://jbcdn2.b0.upaiyun.com/2016/06/7bfc85c0d74ff05806e0b5a0fa0c1df12.jpg)
步骤4：编写代码：

C#
```
usingSystem;
usingSystem.Collections.Generic;
usingSystem.ComponentModel;
usingSystem.Data;
usingSystem.Drawing;
usingSystem.Text;
usingSystem.Windows.Forms;
usingShapes; //1.引用命名空间
namespaceWinCalc
{
public partialclass Form1 : Form
{
public Form1()
{
InitializeComponent();
}
private voidbutton1_Click(object sender, EventArgs e)
{
doubleradious = 0.0;
Circlecircle1 = new Circle();
circle1.Radius = Convert.ToDouble(radious);
MessageBox.Show("圆的面积为：" +circle1.getArea().ToString());
}
}
}
```
步骤5：设置WinCalc为启动项目，如下图，然后运行（F5）:
![7](http://jbcdn2.b0.upaiyun.com/2016/06/cd7fd1517e323f26c6f1b0b6b96e3b3d2.jpg)
步骤6：出现运行界面，输入半径，如下图：
![12](http://jbcdn2.b0.upaiyun.com/2016/06/c8b2f17833a4c73bb20f88876219ddcd2.jpg)
思考与扩充，在.Net中程序集组件引用的本质是多加几个类库项目，而是引用的其中的核心组件是什么？
请注意添加引用的本质是类库项目所对应的程序集文件，如Shapes\bin\DebugShapes.dll文件。
【扩充在VB下使用C#的dll文件】
1. 界面设计省略同C#
2.代买比较痛苦： PublicClass Form1

C#
```
Private Sub Button1_Click(ByVal sender AsSystem.Object, ByVal e As System.EventArgs) Handles Button1.Click
Dim circle As Shapes.Circle
circle = New Shapes.Circle()
circle.Radius = 3.3
MessageBox.Show("VB圆面积" +circle.getArea().ToString())
End Sub
End Class
```
2. 效果如下：
![8](http://jbcdn2.b0.upaiyun.com/2016/06/815e6212def15fe76ed27cec7a393d592.jpg)
深入研究程序集
对于可执行的程序集来讲，包含五部分的信息：
## n PE32头
这是标准的Windows可执行文件头，它可以引导操作系统对此文件进行执行。二进制的可执行文件也必须有这个头操作系统才能执行。PE头里包含了标识文件类型的信息：Exe或Dll；除此之外里面还包括其生成时间以及入口方法地址（Main()）等。操作系统可以通过读取这个头将托管模块加载到内存中然后调用CLR从入口方法开始执行。
## n 元数据
元数据是托管模块中最重要的数据。元数据用来表述托管模块中包含的类型信息，这些信息包括类、接口、方法、字段、属性、事件等代码元素的定义信息。每个程序集都包含有元数据表，元数据表通常有两种，一种是表述代码中的类型或成员的列表；另外一种是表述源代码中引用的类型和成员的表。在模块或程序集中定义和引用的每个类型和成员都将在元数据中进行说明。当执行程序时，运行库将加载元数据，并可以通过它来发现有关代码的类、成员、继承等信息。
## n 中间语言
.Net编译器将编程语言的源代码编译后生成的中间语言代码，在执行时，CLR将这些中间语言代码编译成CLR执行。
## n 资源
资源是指程序在运行时所需要的资源，包括图片、字符串等。在默认情况下，一个程序集可以包含资源也可以不包含资源。如果程序集包含资源，那么程序集中会有一个叫做资源清单的结构专门来描述和连接这些资源文件，公共语言运行库在运行时会管理或加载这些资源文件。
## n 程序集清单
程序集清单包含描述该程序集中各元素彼此如何关联的数据集合。这些数据包括：程序集名称、版本号、区域性、程序集中文件所有文件的列表、类型引用信息、有关被引用程序集的信息等。其中微软规定的前四项（程序集名称、版本号、区域性和强名称信息）构成了程序集的标识。查看.Net创建的任何项目拟都会发现一个叫“AssemblyInfo”的文件，这个文件就是用来设置程序集清单的，一般在创建文件时会被自动生成。
**反射及其使用**
提到反射，首先我们会想到中学物理，光的反射是成像的基础。如果没有光的反射，你将看不到任何东西。如果没有反射，你也不能照镜子，就算你再帅再靓，你也不知道。
对于物体来说，你可以直接查看该物体，也可以通过镜子来查看该物体。这些概念和软件技术中反射的概念很相似。在软件技术中，反射代表一种方法，一种通过间接途径来查看和访问程序集中元素的方法。
**反射本质**
C#项目在编译后生成程序集，而程序集通过记录了代码中的所有类型。假如说，我们目前有某个组件的程序集，而没有源代码，我们能不能通过一种机制从程序集中取出各种类型进行操作呢？答案是肯定的，在.Net中可以通过反射机制从程序中提取任何存在的类型，包括类、结构、委托、接口和枚举等，提取出类型后，利用反射机制我们甚至可以重新生成这些类型的对象，然后使用这些类型的成员，包括方法、属性、事件、构造方法等。在一般情况下，我们可以使用反射来执行以下任务：
n 获取程序集中有关公共类型及其公共成员的信息
n 了解代码所在的模块和程序集
n 调用程序集中类型对象公共属性或方法
要使用反射操作某个程序集中的类型，基本分为两步走：
1. 加载要反射的程序集；
2. 然后通过Type类的方法对程序集中的类型进行枚举或调用。
因为对非公共信息的访问将带来安全风险，所以.Net反射机制在默认情况下只能对程序集中的公共类型的信息或成员进行反射。
**使用反射**
在使用反射之前，我们首先需要加载程序集。需要使用System.Reflection命名空间下的Assembly类的Load方法或LoadFrom方法。这两个方法都是静态方法：
Assembly.Load(String 程序集名称)
例如：Assembly assA=Assembly.Load(“Shapes”);
Assembly.LoadFrom(String 程序集路径)
例如：Assembly assB=Assembly.LoadFrom(@”F:\MyShapes\bin\Debug\MyShaps.dll”)
接下来，通过Assembly的GetType和GetTypes方法来获得加载程序集的类型了。前者只能获得程序集里一个类型，后者可以获得程序集里所有类型，原型如下：

C#
```
public Type GetType(string name) //根据类名获得该类型
public Type[] GetTypes() //获得程序集的所有类型
```
一旦获得程序集中的类型，就可以重建类型对应的对象。有关Type类的常用方法如下：
|**Type方法**|**返回类型**|**操作说明**|
|----|----|----|
|GetMethod/GetMethods|MethodInfo|使用MethodInfo对象封装的方法名称、返回类型、参数、访问修饰符等|
|GetField/GetFields|FieldInfo|使用FieldInfo封装类型的字段名称、访问修饰符等|
|GetEvent/GetEvents|EventInfo|使用EventInfo封装类型的字段名称、事件处理程序、数据类型、自定义属性、声明类型和反射类型等|
|GetProperty/GetProperties|PropertyInfo|使用PropertyInfo封装属性的名称、数据类型、声明类型等|
|GetConstructor/GetConstructors|ConstructorInfo|使用ConstrucorInfo封装构造方法的名称、参数、访问修饰符等信息|
> 
小结：
在本章中，我们主要学习了：
u 组件的历史和应用
u .Net中的组件技术
u 反射技术的使用
u 序列化
英语词汇：
英文                         中文
Attribute               属性
Positional              状态的
Element                 元素
Description           描述
Temperature        温度
Fahrenheit            华氏温度
Service                  服务
Import                   导入
Inherited               继承的
All                           所有的
Assembly              装配、集合、组件
Module                 模块
Struct                    结构
Specify                  指定
练习项目：
1.试将以前我们写的计算器的计算类生成.dll程序集，供其他地方使用。
