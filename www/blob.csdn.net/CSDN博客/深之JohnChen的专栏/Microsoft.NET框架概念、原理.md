# Microsoft.NET框架概念、原理 - 深之JohnChen的专栏 - CSDN博客

2010年01月08日 12:58:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1782标签：[框架																[windows																[语言																[assembly																[forms																[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=forms&t=blog)](https://so.csdn.net/so/search/s.do?q=assembly&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)
个人分类：[.Net & C#](https://blog.csdn.net/byxdaz/article/category/628880)


**NET****框架类库**

FCL(Framework Class Library)含有几千个类型的定义。（它的体系和以前了解的VCL的体系有点类似，不知道其他的类库是否也是以类似的方法来建立？）

1、CLR和FCL可以开发下面几种应用程序：XML Web服务、Web窗体（Web Forms基于HTML的应用程序）、Windows窗体（GUI应用程序）、Windows控制台应用程序（CUI）、Windows服务和组建库

2、FCL波阿汉的类型放在一个由namespace（命名空间）组织的集合中提供给开发人员。

System命名空间包含了Object基本类，所有其他的类型都直接或简介由此继承而来；它还包含了整数、字符、字符串、异常处理……。所有的应用程序都会用到System命名空间中的类型。（这里对命名空间的基本概念没做出解释，如果是以前没接触过这一名词的读者肯定会被糊了）

3、一些通用的FCL命名空间：

System：其中的类型是为所有应用程序使用的一些基本类型；

System.Collections：其中的类型用于管理对象集合；

System.Diagnostics：其中的类型用于帮助诊断和调试应用程序；

System.Drawing：其中的类型用于操作二维图形；

System.EnterpriseServices：其中的类型用于管理事务、队列组件、对象池等等；

System.Globalization：其中的类型用于多国语言支持；

System.IO：其中的类型用于操作I/O流、遍历目录和文件；

System.Xml：其中的类型用于处理XML模式（schema）和数据;

System.Web.Services：其中的类型用于创建XML Web服务；

System. Web.UI：其中的类型用于创建Web窗体；

System. Windows.Forms：其中的类型用于创建Windows GUI应用程序；

System. ServiceProcess：其中的类型用于创建由SCM控制的Windows服务。

4、CLR和FCL支持我们可以开发以下几种应用程序:
(1)XML Web Services
(2)Web Forms
(3)Windows Forms
(4)Windows CUI(控制台应用程序)
(5)Windows服务(Windows SCM-Service Control Manager-所控制的服务程序)
(6)组件库

**通用类型系统CTS**

　　从前面的内容大家可以看到CLR的所有内容都是围绕类型展开。微软制定的规范CTS(Common Type System，即通用类型系统)，就是用来描述类型的定义和行为。

1、CTS规范规定一个类型可以包含下面的0或多个成员：字段、方法、属性和事件

2、CTS还定义了类型可见性和访问类型成员的一些规则，下载是控制对方法或字段的访问选项：

Private:方法只能被同一类型中的其他方法调用；

Family:方法可以被派生类型的代码调用，不管它们是否在同一个程序集中。（类似于许多语言中的protected）;

Family 与assembly:方法只可以被位于同一个程序集中的派生类型中的代码调用。（C#和VB都没有提供这种机制）；

Assembly: 方法可以被统一个程序集中的任何代码调用（类似于internal）；

Family 或 Assembly：方法可以被任何程序集中的派生类型代码调用，也可以被同一程序集中的任何类型调用；（这个和前面的Family区别在哪里？）

Public:方法可以被任何程序集的任何代码调用。

　　另外CTS定义了诸多规则来管理类型继承、虚函数、对象生存期等。我们在编程的时候并不需要了解这些规则，因为在程序被编译成托管代码时会将你所使用的编程语言的语法规则映射为CLR的这些规则（除非你想用IL汇编直接写你选用的语言实现不了的功能）。

CTS不支持多继承，如果你用的C＋＋使用了多继承是不能被编译成托管代码的。

CTS的类型都必须继承自System.Object

**通用语言规范CLS**

CLR提供语言间对象的操作，即不同语言的代码之间进行无缝的集成。为了解决这个问题微软定义了CLS（Common Language Specification,通用语言规范），不同语言的代码只有符合了这个规范才能彼此集成。它是各种高级语言的一个交集。

**代码在****CLR****中的运行过程**

**1****、选择编译器**

首先CLR选择编译器，因为CLR中可以运行不同语言开发出来的代码。

**2****、把代码编译成****MSIL**

CLR利用选择的编译器生成MSIL（Microsoft intermediate language）和metadata把她们加载到PE（portable executable file）。

MSIL：解释语言，包含代码信息，让JIT易于编译成机器语言。与操作系统和编程语言无关。

PE：可执行文件格式，windows就是DLL或EXE了。

**3****、用****JIT****编译器把****MSIL****编译成机器语言**

有两种方式一种是只编译当前执行的代码段，另一种是把整个代码一次性编译。JIT会检测代码的安全性。

**4****、****执行代码**

代码首次调用就会编译成机器代码，再次调用的时候就不再重新编译了。

**与非托管代码互操作**

　　为了使得现存的非.NET代码都可用，微软的设计使得CLR可以在应用程序里同时包含托管和非托管代码。CLR支持三种互操作：

　　托管代码调用DLL中的非托管函数

　　托管代码使用现存的COM组建（非托管组建作为COM服务器）

非托管代码使用托管类型（托管类型作为COM服务器）

**.NET****框架、语言、工具**

**![](http://i3.6.cn/cvbnm/41/b5/0f/1b136d4294116a7fd0c6a5bdf1222b76.jpg)**

**.NET执行与编译过程**

**![](http://i3.6.cn/cvbnm/ef/96/fe/24e6d335debcd8b5497b1aeb55fc95c4.jpg)**


