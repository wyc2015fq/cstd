# C#学习笔记8——C# 中的委托和事件 - 深之JohnChen的专栏 - CSDN博客

2011年09月01日 00:03:40[byxdaz](https://me.csdn.net/byxdaz)阅读数：1565


**NET委托和事件**

**一、委托**

### 1、委托概念

OOP中具有相同属性的对象抽象后成为类型（**class**）。那么，具有相同属性的函数或方法（也称具有相同的函数签名）：
- 返回类型相同
- 参数类型、参数顺序及参数个数相同

抽象后又是什么概念？例如，1到n之间每个数的平方后求和函数intSquareSum(int n)和立方后求和函数intCubeSum(int n)，它们具有相同的函数签名：返回类型int、参数只有一个且是int类型。
static private int SquareSum(int n)
{
int m = 0;
for (int k = 1; k <= n; k++)
{
m += k * k;
}
return m;
}static private int CubeSum(int n)
{
int m = 0;
for (int k = 1; k <= n; k++)
{
m += k * k * k;
}
return m;
}
这些相同属性的函数抽象，就是.NET提出的一个新的类型概念——委托，关键字为delegate。

### 2、委托声明及特点

与C/C++/C#的函数声明相同，声名一个委托需要有：委托名、返回类型、参数及类型。例如，声明前面定义的两个函数的委托PowerSum如下： 
public delegate int PowerSum(int n);
特别地，一类通用的事件处理委托EventHandler声明如下：
public delegate void EventHandler(object sender, EventArgs e)
显然，与类定义不同，委托声名不需要定义成员，它只起一个表示作用（delegate就是代表的意思）。此外，delegate也是类，其基类是MulticastDelegate，再上层类是Delegate，顶层类是object。

C# 2.0 提供了更简单的方法来编写在以前的声明，如下面的示例所示。

//1、声明委托
public delegate void PublishBook();
//2、声明一个和签名和委托一样的方法
public static void ReadCSharpBook()
{
Console.WriteLine("Im Reading C Sharp Book Now!!");
}
//3、创建一个委托实例
PublishBook pck = ReadCSharpBook;
在 C# 2.0 和更高版本中，也可能是到下面的示例所示声明并初始化一个委托，使用匿名方法。

PublishBookpck= delegate()
{ 
Console.WriteLine("Im Reading C Sharp Book Now!!");
};
在 C# 3.0 和更高版本，委托可还声明和实例化使用一个 lambda 表达式，如下面的示例所示。

Publisher.PublishBookpck = ()=>
{
Console.WriteLine("Im Reading C Sharp Book Now!!");
};

### 3、委托揭秘

标题“揭秘”是借用了Jeffrey Richeter的《.框架设计(第2版)CLR Via C#》的一句原话。前面定义的委托 PowerSum 的类层次如下图，其中省略了许多方法与方法参数，图也不十分规范（BeginInvoke()的参数有省略号）。            

![](http://hi.csdn.net/attachment/201108/31/0_1314806644W619.gif)
从上图可以看出：
- 委托内含了一个链表，可以 Combine() 和 Remove() 实例方法
- 委托链提供了委托对象的访问接口
- 委托是异步编程的基础，BeginInvoke() 和 EndInvoke() 两个方法是通用的异步调用函数

### 4、委托应用
- Microsoft .NET     Framework 通过委托向外提供一种函数回调机制。——《框架设计(第2版)CLR Via C#》Jeffrey Richeter
- 是一种类型安全的方法引用，可以把它看成一个类型安全的C函数指针。——《.NET组件编程》Juval Lowy
- 要把方法传送给其它方法时需要委托。与C函数指针不同，.NET委托是类型安全的。——《C#高级编程》Christian Nagel

从上面名著的描述可以看出，.NET委托的主要用途是三个：1）函数回调；2）传递方法；3）事件机制。

### 4.1委托举例1：传递方法

委托作为方法传递时，有两种方式。第一种，直接传递方法，这种方式称为**委托推断**；第二种，创建委托对象后传递，这种方式是常规方式。应用前面定义的委托，现定义一个调用方法的函数：intGetPowerSum(PowerSum ps)如下，该函数用于计算1到10的指数和。
static private int GetPowerSum(PowerSum ps)
{
return ps(10);
}
采用委托推断方式调用代码如下：
int p2 = GetPowerSum(SquareSum);
int p3 = GetPowerSum(CubeSum);
采用创建委托对象方式调用代码如下：
PowerSum ps2 = new PowerSum(SquareSum);
PowerSum ps3 = new PowerSum(CubeSum);

p2 = ps2(10);
p3 = ps3(10);
### 4.2委托举例2：函数回调

最常见的回调应用之一，是计时器到点时调用的函数。涉及到的类型如下（.NET有三个计时器类型，这个是线程名称空间System.Threading里的Timer）：
public sealed Timer(TimerCallBack callback, object state, int dueTime, int period)；
public delegate void TimeCallBack(object state);- Timer类型中，callback是一个委托TimerCallBack的对象；state是调用时的状态参数，可以灵活应用；dueTime是计时器开始计时的等待时间；period是计时周期，每完成一个周期就调用方法callback
- 回调函数CalllBack的委托定义表明，计时器类Timer到点时回调的函数不能有返回类型，但必须有一个参数object型的参数。注意，此处委托的所谓逆变不能用了

现定义一个到点回调函数，即到点就输出字符串信息如下：
static private void TimeClick(object state)
{
Console.WriteLine("time click 500ms");
}
那么500ms报时的计时器应用代码如下：
System.Threading.TimerCallback callBack = new System.Threading.TimerCallback(TimeClick);
System.Threading.Timer timer = new System.Threading.Timer(callBack, null, 0, 500);
由于回调函数比较简单，可以使用匿名委托，代码如下
System.Threading.TimerCallback callBack = new System.Threading.TimerCallback
(
delegate(object state) 
{ 
Console.WriteLine("time click 500ms"); 
}
);

System.Threading.timer = new System.Threading.Timer(callBack, null, 0, 500);
5、**多路广播委托**
　　　前面使用的委托只包含一个方法调用。调用委托的次数与调用方法的次数相同。如果要调用多个方法，就需要多次显示调用这个委托。其实委托也可以包含多个方法，这种委托就是多路广播委托。多路广播委托派生于System.MulticastDelegate，它的Combine方法允许把多个方法调用链接在一起，我们可以通过+=来向委托添加调用方法，也可以用-=删除其中的调用方法。

**为什么要用委托**　　使用委托使程序员可以将方法引用封装在委托对象内。然后可以将该委托对象传递给可调用所引用方法的代码，而不必在编译时知道将调用哪个方法。与C或C++中的函数指针不同，委托是面向对象，而且是类型安全的。

**委托使用的注意事项**

1)在C＃中，所有的委托都是从System.MulticastDelegate类派生的。

2)委托隐含具有sealed属性，即不能用来派生新的类型。

3)委托最大的作用就是为类的事件绑定事件处理程序。

4)在通过委托调用函数前，必须先检查委托是否为空（null），若非空，才能调用函数。

5)在委托实例中可以封装静态的方法也可以封装实例方法。

6)在创建委托实例时，需要传递将要映射的方法或其他委托实例以指明委托将要封装的函数原型（.NET中称为方法签名：signature）。注意，如果映射的是静态方法，传递的参数应该是类名.方法名，如果映射的是实例方法，传递的参数应该是实例名.方法名。

### 二、事件

### 1、概念

·[Visual Studio 2008](http://msdn.microsoft.com/zh-cn/library/awbftdfh(v=VS.90).aspx)

·[Visual Studio 2005](http://msdn.microsoft.com/zh-cn/library/awbftdfh(v=VS.80).aspx)

[类](http://msdn.microsoft.com/zh-cn/library/0b0thckt.aspx)或对象可以通过事件向其他类或对象通知发生的相关事情。发送（或引发）事件的类称为“发行者”，接收（或处理）事件的类称为“订户”。

在典型的 C#Windows 窗体或 Web 应用程序中，可订阅由控件（如按钮和列表框）引发的事件。可使用 Visual C# 集成开发环境 (IDE) 来浏览控件发布的事件，选择要处理的事件。IDE 会自动添加空事件处理程序方法和订阅事件的代码。Event，它封装了委托类型的变量，使得在类的内部，不管你声明它是public还是protected，它总是private的。在类的外部，注册“+=”和注销“-=”的访问限定符与你在声明事件时使用的访问符相同。

### 2、事件具有的特点

·              发行者确定何时引发事件，订户确定执行何种操作来响应该事件。

·              一个事件可以有多个订户。一个订户可处理来自多个发行者的多个事件。

·              没有订户的事件永远也不会引发。

·              事件通常用于通知用户操作，例如，图形用户界面中的按钮单击或菜单选择操作。

·              如果一个事件有多个订户，当引发该事件时，会同步调用多个事件处理程序。要异步调用事件，请参见[使用异步方式调用同步方法](http://msdn.microsoft.com/zh-cn/library/2e08f6yc.aspx)。

·              在 .NET Framework 类库中，事件是基于 EventHandler委托和 EventArgs 基类的。

### 3、事件描述

我们可以把事件编程简单地分成两个部分：事件发生的类（事件发生器）和事件接收处理的类。事件发生的类就是说在这个类中触发了一个事件，但这个类并不知道哪个个对象或方法将会加收到并处理它触发的事件。所需要的是在发送方和接收方之间存在一个媒介。这个媒介在.NET Framework中就是委托(delegate)。在事件接收处理的类中，我们需要有一个处理事件的方法。好了，我们就按照这个顺序来实现一个捕获键盘按键的程序，来一步一步说明如何编写事件应用程序。

1）、首先创建一个自己的EventArgs类。
EventArgs是包含事件数据的类的基类，此类不包含事件数据，在事件引发时不向事件处理程序传递状态信息的事件会使用此类。如果事件处理程序需要状态信息，则应用程序必须从此类派生一个类来保存数据。
因为在我们键盘按键事件中要包含按键信息，所以要派生一个KeyEventArgs类，来保存按键信息，好让后面知道按了哪个键。 

 internal class KeyEventArgs : EventArgs
 {
private char keyChar;
public KeyEventArgs( char keyChar ) : base()
 {
this.keyChar = keyChar;
}

public char KeyChar
 {
get
 {
return keyChar;
}
}
}

2）、再创建一个事件发生的类KeyInputMonitor，这个类用于监控键盘按键的输入并触发一个事件：

internal class KeyInputMonitor
 {
// 创建一个委托，返回类型为void，两个参数
public delegate void KeyDown( objectsender, KeyEventArgs e );
// 将创建的委托和特定事件关联,在这里特定的事件为OnKeyDown
public event KeyDown OnKeyDown;

public void Run()
 {
bool finished = false;
do
 {
Console.WriteLine( "Input achar " );
string response = Console.ReadLine();
char responseChar = ( response == "") ? ' ' : char.ToUpper( response[0] );
switch( responseChar )
 {
case 'X':
finished = true;
break;
default:
// 得到按键信息的参数
KeyEventArgskeyEventArgs = new KeyEventArgs( responseChar );
// 触发事件
OnKeyDown( this,keyEventArgs );
break;
}
}while( !finished );
}
}

这里注意OnKeyDown( this,KeyEventArgs );一句，这就是触发事件的语句，并将事件交由KeyDown这个委托来处理，委托指定事件处理方法去处理事件，这就是事件接收方的类的事情了。参数 this是指触发事件的对象就是本身这个对象，keyEventArgs包含了按键信息。

3）、最后创建一个事件接收方的类，这个类先产生一个委托实例，再把这个委托实例添加到产生事件对象的事件列表中去，这个过程又叫订阅事件。然后提供一个方法回显按键信息。

 internal class EventReceiver
 {
public EventReceiver( KeyInputMonitor monitor )
 {
// 产生一个委托实例并添加到KeyInputMonitor产生的事件列表中
monitor.OnKeyDown += newKeyInputMonitor.KeyDown( this.Echo );
}
private void Echo(objectsender, KeyEventArgs e)
 {
// 真正的事件处理函数
Console.WriteLine( "Capture key: {0}", e.KeyChar );
}
}

4）、看一下如何调用

 public class MainEntryPoint
 {
public static void Start()
 {
// 实例化一个事件发送器
KeyInputMonitormonitor = new KeyInputMonitor();
// 实例化一个事件接收器
EventReceivereventReceiver = new EventReceiver( monitor );
// 运行
monitor.Run();
}
}

**事件使用的注意事项**

C#中使用事件需要的步骤：
1）.创建一个委托
2）.将创建的委托与特定事件关联(.Net类库中的很多事件都是已经定制好的，所以他们也就有相应的一个委托，在编写关联事件处理程序--也就是当有事件发生时我们要执行的方法的时候我们需要和这个委托有相同的签名)
3）.编写事件处理程序
4）.利用编写的事件处理程序生成一个委托实例
5）.把这个委托实例添加到产生事件对象的事件列表中去，这个过程又叫订阅事件

参考文献 

1、[http://msdn.microsoft.com/zh-cn/vstudio/ms173176(en-us,VS.85).aspx](http://msdn.microsoft.com/zh-cn/vstudio/ms173176(en-us,VS.85).aspx)

