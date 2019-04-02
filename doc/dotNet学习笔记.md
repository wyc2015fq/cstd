# dotNet学习笔记



# （一）.NET平台结构

2007年11月17日 21:34:00 [cc_net](https://me.csdn.net/cc_net) 阅读数：2969



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/cc_net/article/details/1890461

为了得到一些东西，我们必须放弃一些东西，在每个人的人生天平上，孰重孰轻，只有我们自己知道，但是一旦选择了，我们就要尽量去做好它，保留我们内心中那少少值得骄傲的地方。

​                                                                                                                             -------------------------------学习题记

看到小麟写的很好，就拿过来。大家一起共勉。毕业到工作，有4个月了，突然发现有些不是自己想要的。即便现在的环境和安逸和轻松。谈到.net，或许自己连个菜鸟都算不上。但，为了做自己喜欢的事，从现在起，我努力把自己变为一只菜鸟。《.net框架程序设计》是一本非常好的书，我通过学习此书，顺便做点笔记，尽量用自己的理解。笔记中或许会有许多错误，有许多不正确的认识。也请新人，菜鸟，老鸟，高手们多多指教。

 

## 一：平台介绍

NET Framework 具有两个主要组件：公共语言运行库和 .NET Framework 类库。

公共语言运行库是 .NET Framework 的基础。公共语言运行库管理内存、线程执行、代码执行、代码安全验证、编译以及其他系统服务。这些功能是在公共语言运行库上运行的托管代码所固有的。代码管理的概念是运行库的基本原则。以运行库为目标的代码称为托管代码，而不以运行库为目标的代码称为非托管代码。

.NET Framework 的另一个主要组件是类库，它是一个综合性的面向对象的可重用类型集合，您可以使用它开发多种应用程序，这些应用程序包括传统的命令行或图形用户界面 (GUI) 应用程序，也包括基于 ASP.NET 所提供的最新创新的应用程序（如 Web 窗体和 XML Web services）。

 [![img](http://msdn.microsoft.com/library/CHS/cpguide/html/circle.gif)](http://msdn.microsoft.com/library/CHS/cpguide/html/circle.gif)

## 二：将源文件编译为托管模块

.net实现了跨平台，语言无关，这些都是因为他采用了CLR，无论你使用VB还是C#或是其他CLR所支持的语言，都会通过相应的编译器把源文编译为一个中间代码，MSIL。MSIL是一种接近汇编的语言。通过编译的模块是一个托管模块（MSIL只是一部分）。托管模块是一个需要CLR才能运行的标准WINDOWS可移植可执行文件（PE文件）。如果想在其他OS上也能运行，那么就需要其他系统也能支持这种PE文件。

一个PE文件包括了：

PE表头：它主要描述的是这个PE文件的类型，包括CUI，GUI，DLL（这里的DLL不同于传统的动态连接库，他是.net中的程序集的概念），文件创建的时间，以及一些本地的CPU信息。

CLR表头：因为这个PE文件需要CLR才能执行，所以包括了托管模块的一些信息，如需要CLR的版本号，托管模块代码的入口点（Main方法）的标记，模块的源数据，资源，强命名等信息。

元数据：元数据是以表格的形式存在的，用来描述程序中所定义和引用的各种类型，变量，方法，事件。它使得程序的代码有了自描述性，他是。他省去了编译时（JIT编译）对头文件和库文件的需求，因为编译器可以直接从元数据表中获得需要引用的类型和成员。而在写程序时的 智能感知技术也是通过分析元数据表实现的。利用它。垃圾回收器可以跟踪对象。（常用的元数据表可以自己查下，这里就不列出来了）。

IL代码：这快就是通过CLR所支持的编译器，把源代码编译成IL代码，也就是实际的程序代码，但这个代码要运行，就需要上面的信息。在执行此PE文件时，CLR会把IL代码编译成真正的CPU指令，进行执行。

 

##  三：将托管模块组合为程序集

实际上CLR处理的不是托管模块，而是处理程序集。所谓的程序集，我个人理解就是，一个或多个托管模块，类型定义文件和资源的文件集合。可以认为程序集是对托管模块的封装，封装的同时加入了另一个新的元数据表集----清单。他描述了程序集中的一些信息，包括程序集的名称，版本，语言标记，包含的资源，包括的各个模块信息等等。使用程序集，有助于部署程序，还可以把资源同代码放在一起，而不必欠到程序中，还可以把不同语言的模块放在一起。

让我们回记下，之前先把源代码编译成模块，模块中元数据描述了模块中定义和引用的类型等信息。然后把模块组成一个程序集，程序集中包括的模块，以及描述其中的模块信息和程序集的信息。

在使用VS进行编译时，是直接编译成程序集的。让我们在看看前面编译成托管模块的时候，模块中不包含 清单元数据。因此他是不能被CLR执行的，而只有连接成程序集后，有了清单，才能被执行。而且VS下是不能把多个模块和资源编译到一个程序集合里的，也就是说不支持多文件程序集，所以我们要利用一些工具。

方法1：用CSC.EXE把托管模块加入到一个程序集中，

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//CSC编译器可以把源文件直接便宜为一个程序集，也可以把他编译为一个模块
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//支持把一个模块添加到一个程序集中
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)csc /t:module ex1.cs   //把ex1.cs源文件编译为一个模块（而非程序集），得到的模块名为ex1.netmodule
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)csc /out:ex.dll /t:library /addmodule:ex1.netmodule ex2.cs       //这里是把ex2.cs编译为程序集，同时把模块ex1,加入其中。
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

 

通过上面，一共生成了2个文件，ex1.netmodule模块和ex.dll的程序集（只是和DLL扩展名相同）。实际上把模块加入程序集的过程，只是把模块的信息加入到了程序集的清单中，而不是把整个模块放入到其中。这样做的好处就是，可以把不常用的代码放到ex1中，这样在不使用时是不会调用ex1模块的，减少了部署程序时文件大小，特别是网络部署时，如不使用，可以不下载ex1模块。

方法2：用AL.EXE把托管模块组合成程序集

这就需要用到程序集连接器AL。AL作用是只生成清单而不包括其他任何类容的DLL和EXE文件。但大多数情况是生成DLL文件，因为生成的EXE文件中，没有Main函数入口。虽然可以通过AL.EXE /main命令行来，但不是很有用。

 

```shell
//把前面的ex1,ex2放在一个程序集里

//生成2个托管模块（不包含清单，不能被CLR运行）
csc /t:module ex1.cs
csc /t:module ex2.cs 

//利用AL，连接2个模块，并产生清单
al /out:ex.dll /t:library ex1.netmoudle ex2.netmoudle
```

 

经过上面一共产生了3个文件，ex1模块，ex2模块和ex程序集合。在程序集中不包含任何IL代码，他没有把2个模块放入程序集，而只是在程序集清单中记录了模块的信息。CLR能处理的是ex.dll，通过清单找到2个模块进行调用。

通过上面，看到了什么是程序集，他和托管模块的区别。对于只包含一个托管模块的程序集而言，他就是在模块中加入了一个清单，使他成为了一个程序集，而非用个把清单和模块装在程序集（可以看作一个外壳）里。而对于包含多个模块的程序集，他则是把多个模块和资源还有清单放到程序集（外壳）中。一个托管模块是一个PE，当然一个程序集也是一个PE。


当然，程序集中也可以放一些资源文件，可以使用al /line,al /embed，来添加资源的引用或是把文件直接欠入到程序集中。

下面是分别用AL和CSC把2个CS文件（ex1.cs ,ex2.cs）编译到一个程序集中，大家可以看下差别：

​           ![img](http://www.best4c.com/Best4cUserFiles/20071220/17143_1198125475275.jpg)    （左图）![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/%E7%BB%98%E5%9B%BE2.jpg)（右图）

上面左图是使用CSC直接把2个源文件编译到同一个程序集中，于是其中包含了2个文件IL代码和元数据，然后元数据中还有一个清单，是存储整个程序集相关信息的。

在看看右图，右边是先把2个源文件编译成2个模块，其中模块中包含了个自的IL代码和元数据，然后通过AL，把2个连接起来，可以看带程序集里没有任何IL和模块的元数据，只有一个清单，包含了程序集信息，以及连接的2个模块信息。

具体不同，通过ILDASM来查看元数据就很清楚了。

ps:以上工具默认在 C:/WINDOWS/Microsoft.NET/Framework/.net版本目录

 

## 三：运行可托管代码

当源程序被编译成中间代码，成为程序集后（exe文件），就可以被CLR运行了。因为程序集是需要CLR的，所以，在机器上运行他，必须安装CLR。也就是为什么要装.net 框架才能运行.net程序。微软提供了NET Framework 下载，装了SDK或VS的会自动安装。

当生成exe程序集时，编译器/连接器会产生一些特殊信息，嵌入到程序集的PE文件表头及各个组成文件的.text部分。当EXE文件被调用时，这些信息将导致CLR被加载并初始化。CLR确定程序集的入口点，在MAIN函数被执行之前，CLR检查MAIN中代码应用到的所有类型。这会导致CLR分配一个内部存储结构，用于管理对所引用的类型的访问。其中每个条目保存了每个方法实现代码的地址。CLR初始化时，把这些条目设置为一个函数。

然后开始执行MAIN，要调用其中的一个方法时，找到他对应的函数条目，函数把他对应的实现代码转为本地的CPU指令。这个编译过程是又JIT编译器来完成的。本地CPU指令保存在一个动态分配的内存区域中。然后这个函数会跳转到此区域，来执行这些CPU指令，执行完成后，跳转回MAIN函数。而当第二次在执行这个方法时，就会直接执行CPU指令，而不需要在进行编译了。但因为是临时区域，一旦程序关闭，这些指令也会丢失。

而在编译成本地CPU指令的同时，会对IL代码进行安全检查和验证。主要检查的内容包括：不能从为分配的内存区域中读取数据，每个方法必须传入正确的参数个数，类型也需要匹配，每个方法返回值必须被正确使用等等。托管模块中的元数据包含了所有需要被验证的方法和类型信息，如果IL代码被认为‘不安全’，系统会抛出一个验证不合法的异常，阻止程序继续进行。（注意：C#，VB.NET编译器产生的代码都是安全的，这些代码是可以被验证++的。但在其他语言中，如C++可以利用unsafe关键字来创建不能被CLR验证的代码，在系统配置中可以选择也把不可验证的代码编译成本地CPU指令）

 

具体的运行过程大家可以自行查阅。

 

## 四：ILDASM

原文件经过编译，成为了MSIL代码。而IL代码是可以通过ILDASM这样一个工具进行反编译，来查看程序集其中包括类型，方法，元数据等信息。

这些工具的使用大家可以自行查阅
```csharp
//以下为一个简单的程序的元数据信息。

//程序
public class app
{
    static public void Main(System.String[] args)
    {
        System.Console.WriteLine("Hello");
    }
}



//利用ildasm /adv App.exe，查看到的元数据信息
//托管模块清单
copeName : App.exe
MVID      : {0B76BBCA-3B45-4328-97C1-D261BB06A449}
===========================================================
Global functions
-------------------------------------------------------

Global fields
-------------------------------------------------------

Global MemberRefs
-------------------------------------------------------

TypeDef #1  
-------------------------------------------------------
​    TypDefName: app  (02000002)
​    Flags     : [Public] [AutoLayout] [Class] [AnsiClass]  (00100001)
​    Extends   : 01000001 [TypeRef] System.Object
    Method #1 [ENTRYPOINT]
    -------------------------------------------------------
​        MethodName: Main (06000001)
​        Flags     : [Public] [Static] [HideBySig] [ReuseSlot]  (00000096)
​        RVA       : 0x00002050
​        ImplFlags : [IL] [Managed]  (00000000)
​        CallCnvntn: [DEFAULT]
​        ReturnType: Void
​        1 Arguments
​            Argument #1:  SZArray String
​        1 Parameters
​            (1) ParamToken : (08000001) Name : args flags: [none] (00000000)

    Method #2 
    -------------------------------------------------------
​        MethodName: .ctor (06000002)
​        Flags     : [Public] [HideBySig] [ReuseSlot] [SpecialName] [RTSpecialName] [.ctor]  (00001886)
​        RVA       : 0x00002068
​        ImplFlags : [IL] [Managed]  (00000000)
​        CallCnvntn: [DEFAULT]
​        hasThis 
​        ReturnType: Void
​        No arguments.


TypeRef #1 (01000001)
-------------------------------------------------------
Token:             0x01000001
ResolutionScope:   0x23000001
TypeRefName:       System.Object
    MemberRef #1
    -------------------------------------------------------
​        Member: (0a000003) .ctor: 
​        CallCnvntn: [DEFAULT]
​        hasThis 
​        ReturnType: Void
​        No arguments.

TypeRef #2 (01000002)
-------------------------------------------------------
Token:             0x01000002
ResolutionScope:   0x23000001
TypeRefName:       System.Diagnostics.DebuggableAttribute
    MemberRef #1
    -------------------------------------------------------
​        Member: (0a000001) .ctor: 
​        CallCnvntn: [DEFAULT]
​        hasThis 
​        ReturnType: Void
​        2 Arguments
​            Argument #1:  Boolean
​            Argument #2:  Boolean

TypeRef #3 (01000003)
-------------------------------------------------------
Token:             0x01000003
ResolutionScope:   0x23000001
TypeRefName:       System.Console
    MemberRef #1
    -------------------------------------------------------
​        Member: (0a000002) WriteLine: 
​        CallCnvntn: [DEFAULT]
​        ReturnType: Void
​        1 Arguments
​            Argument #1:  String

//程序集清单

Assembly
-------------------------------------------------------
​    Token: 0x20000001
​    Name : App
​    Public Key    :
​    Hash Algorithm : 0x00008004
​    Major Version: 0x00000000
​    Minor Version: 0x00000000
​    Build Number: 0x00000000
​    Revision Number: 0x00000000
​    Locale: <null>
​    Flags : [SideBySideCompatible]  (00000000)
    CustomAttribute #1 (0c000001)
    -------------------------------------------------------
​        CustomAttribute Type: 0a000001
​        CustomAttributeName: System.Diagnostics.DebuggableAttribute :: instance void .ctor(bool,bool)
​        Length: 6
​        Value : 01 00 00 01 00 00                                >                <
​        ctor args: ( <can not decode> )


AssemblyRef #1
-------------------------------------------------------
​    Token: 0x23000001
​    Public Key or Token: b7 7a 5c 56 19 34 e0 89 
​    Name: mscorlib
​    Major Version: 0x00000001
​    Minor Version: 0x00000000
​    Build Number: 0x00001388
​    Revision Number: 0x00000000
​    Locale: <null>
​    HashValue Blob:
​    Flags: [none] (00000000)


User Strings
-------------------------------------------------------
70000001 : ( 5) L"Hello"
```
 补充：

1：何防止别人通过反向工程获得我的代码？
目前唯一的办法是运行带有 /owner 选项的 ilasm。这样生成的元件的 IL 不能通过 ildasm 来查看。然而，意志坚定的代码破译者能够破解 ildasm 或者编写自己的 ildasm 版本，所以这种方法只能吓唬那些业余的破译者。 
不幸的事，目前的 .NET 编译器没有 /owner 选项，所以要想保护你的 C# 或 VB.NET 元件，你需要像下面那样做：

csc helloworld.cs
ildasm /out=temp.il helloworld.exe
ilasm /owner temp.il


2：我能直接用 IL 编程吗？
是的。Peter Drayton 在 DOTNET 邮件列表里贴出了这个简单的例子：


.assembly MyAssembly ...{}
.class MyApp ...{
.method static void Main() ...{
.entrypoint
ldstr "Hello, IL!"
call void System.Console::WriteLine(class System.Object)
ret
}
}

将其放入名为 hello.il 的文件中，然后运行 ilasm hello.il，将产生一个 exe 元件。

3：ilasm和csc

前者是对il文件进行汇编，生成可执行的PE文件，而后者是对cs文件进行编译，生成PE文件。具体的区别好象就这，其他区别还不清楚

## 五：通用系统类型和语言规范

.NET之所以能够实现多种语言集成，是因为CLR。微软顶制定了一个CTS来描述类型的定义和行为，制定了CLS通用语言规范。而所有语言之间想要无缝集成，就必须使用满足CLS。CLR/CTS是一个大的集合，C#，VB.NET这些语言提的功能是CLR/CTS的子集，想要使用CLR所有的功能，只有通过IL了。而CLS则是这些语言的一个交集。要编写一个可以被其他语言使用的程序，都必须满足CLS的规范。、

 

## 六：程序部署

写好的程序，要部署到客户机，以往的程序需要安装到文件夹，写注册表，活动目录等等。而.net程序，因为他拥有元数据，可以自我描述，所以不需要进行这些操作。直接把文件复制到硬盘中就可以执行。而卸载时直接删除就摁可以。.net部署主要有私有部署和全局部署。

私有部署的程序集，把程序集和应用程序放在同一个目录下成为私有部署。应用程序是绑定到和他同时生成的类型上的，所以私有部署的程序集只能被一起编译时的应用程序使用，而应用程序也只能使用此程序集。CLR不会调用一个仅仅是同名的程序集。

而全局部署的则解决了程序集共享的问题。,NET FCL就是使用的这种方法。需要共享程序集必须指定一个搜索的目录，这个目力是c:/windows/assemably/GAC。只要把程序集部署到这个地方，就可以被其他程序加载了。而进行全局部署的程序集需要是‘强命名程序集’，这种程序集的特点是，需要一个唯一的标识。他是通过程序集名、版本号、语言文化标志，公有密钥标记来区分的。而这些信息被插入到程序集的清单中。

全局部署方便共享，但却破坏了.NET所说的，简单‘拷贝安装’，‘删除卸载’。而只有很多程序需要使用时，才考虑上面的方式。为了解决部署方便和私有部署（非强命名程序集）的‘DLL HELL’的问题，可以让强命名程序集部署为私有程序集。这样做到了不同版本的共存，也使得部署和删除都很方便。但每次使用这个程序集时都会对他进行安全检查，防止被篡改过。而全局部署，只会在第一次使用时检查。

具体的部署知识大家可以自己查阅，这里只是简单说下。

 

## 七：性能

.NET的编译过程类似于JAVA的虚拟机，其实基本是一样的。2次编译，在运行速度上可能会比普通WIN32要慢一些。因为他需要对执行的环境了解的更多。但JIT编译也有一些好处，他可以针对CPU进行代码优化；可以不对那些总是返回错误的BOOL表达试进行编译，减少代码；在运行时，CLR能评估和分析代码执行情况，重新组织以提高分支预测成功率。具体性能差距如何，我想应该不会很大的。

 

## 八：总结

通过看书，自己对.net平台有了一个大概的了解，以上内容也基本是用自己的话组织起来的。通过学习这个平台结构，让我对平台有了比较深入的认识。.net平台解决了以前COM中存在的一些问题。他使得语言之间可以无缝集成，而且提供了统一的模型，简化了COM，WIN32中复杂的基础构造。提供了广泛的平台支持，方便部署，解决了DLL HELL问题（以前不同厂家的同名DLL无法共存，导致1方程序出错，而通过强命名程序集解决了这个问题），实现了自动垃圾回收等等。然后了解了整个编译以及运行的过程。

 

## 九 .net类库和WIN32 API

前段时间在CSDN上问了.net的类库是否是封装的WIN32 API。现在总算有了个结论。之前自己对跨平台认识不是很深。对于WINDOWS来说有一个WIN32的框架，对于Linux有一个Linux的框架。他们下面的实现肯定不一样，都是调用系统功能，但上层提供的接口是一样的。在WINDOWS上的.net,他的类库也是封装了WIN32 API的。需要跨平台的话就要使用跨平台的库。而能在linux上使用的话，linux平台的.net的类库也是封装的linux的API。

 

![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/%E5%9B%BE4.jpg)

 上面是我自己理解后画的一个图，不知道对不对。

在.net中不是不使用WIN32 API,而是通过自己的类库来调用API。比如程序总调用了某个类的方法A，他内部通过调用WIN32 API实现。但我们把此程序放到其他平台的.net框架上运行时，这个类名和方法A的名字没有变，但不同平台上的.net 框架调用的系统API不同。也就是所有平台上.net的FCL是相同的，但框架调用的系统API不同。

不知道我上面的理解对不对。







# （二）系统类型和通用操作 （上）

2007年11月29日 11:22:00 [cc_net](https://me.csdn.net/cc_net) 阅读数：2424



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/cc_net/article/details/1906752

这段时间在学习的过程中，也看了其他不少人写的读书笔记，感觉每个地方都有好多东西可以写很多内容。但鉴于目前自己是在第一次学习阶段，很多地方无法弄的太深。此笔记也主要是对每一块内容学习的一个总结,每次在写笔记时，我觉得自己都会有新的收获，了解的更清楚。希望随着学习的深入也能写一些有深度的东西。最近这几天晚上上不了网，而且太累，导致耳鸣了一两天，所以今天才写笔记。以后每天还是早点睡觉，身体是革命的本钱啊！

## 一 类型基础

在.net里，FCL中定义了很多的类型，CLR的要求是每个类型都要继承自System.Object这个类型。在我们定义一个类行的时候，往往是隐式继承于Object的。Object这个类型定义了四个公有的实例方法和两个受保护的方法，而系统中所有类型都能使用这些通用的方法。
Equals: 此方法是判断两个对象的值是否相同的。在Object中的实现是判断两个对象是否指向同一个对象。而在派生类中，主要用于判断值是否相等。其中引用类型和值类型是不同的。自己定义的类型要判断时需要重写此函数
。
GetHashCode:这个方法是返回对象的散列码。如果一个对象被用作散列表的一个键值，那么该对象的类型应该重写此方法。

Tostring:此方法默认情况下是返回一个类型的全名。另一种常见的用法就是重写该方法让它返回一个表示对象状态的字符串。还可以通过重写他来得到一个表示对象字段值的字符串。

GetType:方法返回一个类型为继承自Type的对象实例，标识了该方法所属对象的类型。此方法是一个非虚方法，可以防止派生类重写此方法而隐瞒实际的类型，破坏类型安全。

MemberwiseClone:这也是个非虚方法，他是创建一个新的类型实例，并将去字段设置为和this对象的字段相同。最后返回创建实例引用。后面的深拷贝时回用到此方法。

Finalize:这是一个虚方法，当垃圾回收齐判定某个对象为可回收的垃圾时，垃圾回收器回在对象被回收前调用此方法。此方法很重要。后面学习中还会具体涉及。

CLR要求每个对象都需要用new来创建，new的话系统会执行一系列的内存等分配工作。但要注意的是CLR中没有提供delete这样一个关键字来手动的释放内存，因为这些都是由垃圾回收器来完成的。也许有人会奇怪，我们平时定义一些简单的数值变量的时候并没有用new，只是在定义类的时候才用。这就引出了后面的话题。.net中的数据类型 

 

## 二 数据类型


.net框架中几种数据类型：基元类型，值类型和引用类型。不同的类型有不同的作用，不同的创建方法，销毁方法和不同的内存分配。通过这一部分的学习，使我对这些都有了一定的了解。我自己也觉得这块比较有用。在后面还会继续深入的学习。

### 1：基元类型

编译器直接支持的数据类型称为基元类型。这就是基元类型的定义。比如C#支持的int,char,long,string等等，这些都称为基元类型。对于这种编译器直接支持的类型，都允许我们不使用new,而使用更直接，更方便的方式来创建他们的对象。也就是我们平时定义这些对象用的方法：int x = 0; 而不是用System.Int32 a = new System.Int32()

实际上这2种方法是一样的。为什么会这样呢？我们知道.NET的结构，他是一个可以兼容不同语言的平台，不同的编译器的基元类型可能不同，但他们都和FCL中的类型有直接映射的关系。如C#中int直接映射为System.Int32类型，后者称为.net的内置类型。这样在不同编译器开发的程序，最后还是使用FCL中的类型来表示的。而也适用与其他编译器的基元类型就称为与CLS兼容。比如C#的int型，在VB中也可以使用。而uint就不行。

有时候在写程序时不知道是用int还是Int32,是用string还是String,实际上他们是一样的，不同的是前面的是c#中的类型，而后者是FCL中的类型。他们之间有一个映射的关系。但建议使用FCL中的类型。他可以避免不同语言中的不一致性，也体现了语言无关性，当然还是根基个人习惯决定。下面4种方法都是对的。具体语言的的基元类型参见MSDN

```csharp
int a  = 0;
System32.Int32 a = 0;
int a = new int();
System.Int32 a = new System.Int32()
```



### 2：值类型和引用类型

上面所讲的基元类型，并不是独立与这两种类型存在的第三种类型。基元类型可以是值类型，也可以是引用类型。比如int就是值类型，而string就是引用类型。由上面我们可以知道基元类型定义对象的时候可以不使用new，所以不能以是否用new创建对象来判断此对象是否为引用类型（或许很多人以前会认为用new创建的就是引用类型，现在应该明白了把。何况CLR是要求所有类型都用new创建的）

那么值类型和引用类型主要有那些区别呢？

对于引用类型来说，再创建对象的时候，new操作符号会成托管堆中分配一个空间用来存储对象；然后初始化对象的附加成员，第一个成员是指向类型方法的方法表。第二个成员是SyncBlockIndexs。CLR用这2个成员管理对象实例。接着会传入new操作符指定的参数，调用类型的构造函数。最后会返回一个指向新创建的对象的引用。这个引用存在线程的堆栈上。这个对象最后会由垃圾回收器进行回收。

由此可见，引用类型的性能不高。因为每使用一个对象，都要进行内存分配这些操作，初始化额外的2个成员，使用垃圾回收。这些都会使性能大打折扣。为了提供使用常用类型的性能，就引入了值类型。

值类型是一种轻量级的类型。它通常被分配到线程的堆栈上。它本身包含的不是对象的引用，而是对象实际所包含的所有字段的值（这里可能会不理解字段的意思。确实我们常用的int,char等值类型，没有字段，但struct类型也是值类型，他就可以包含多个字段）。所以在使用此对象时也不用解析指针引用。而且值类型不收垃圾回收器的管理，减少了托管堆的压力。

在.NET框架中明确指出了那些是引用类型，那些是值类型。见下图，具体参见MSDN

![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/%E5%9B%BE1.jpg)

​     ![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/%E5%9B%BE2.jpg)

其中值类型全部继承于System.ValueType类型，而他又继承与System.Object.所以所有的值类型必须继承与System.ValueType类型。所以我们在定义自己的值类型时不能为其选择任何基类型。但我们可以为它实现哟个或多个接口。CLR还规定，值类型不能做为任何其他类型的基类，因为值类型都是密封的，不能被继承。目前C#中的所有值类型都是基元类型。

引用类型和值类型内存布局图如下：

 ![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/%E5%9B%BE3.jpg)

值类型的优点是代价小，性能高。但相比引用类型也有些限制。值类型对象有2种表示：装箱和未装箱。而引用类型总是装箱的。对于值类

型，我们不能为它添加虚方法，它不能被继承。


关于值类型和引用类型具体的内存分配可以参见：<http://www.cnblogs.com/anytao/archive/2007/05/23/must_net_08.html>
这里分析的比较详细。

 

## 三 类型转换


类型转换是我们总回碰到的一个问题，他会出现在很多地方。CLR一个重要的特性就是类型安全。CLR在运行时总能知道一个对象的类型，我们可以利用GetTyoe方法来得到对象的准确类型。前面说了他是一个非虚方法，所以我们无法重写它来隐藏实际的类型。对于类型转换，CLR允许我们把对象转换为其原来的类型或它的任何一个基类型。各种编程语言自己决定如何提供这些转类型的操作，C#中是用隐式转换的。但把某个类型转换为他的派生类是就需要显式转换。而在运行时CLR会检查转类型操作，已确保总是将对象转为它的实际类型、或者它的任何基类型。

看下面的列子：

```csharp
Employee a = new Employee();   //隐式的转换为本身
Object o = a;       //隐式的把Employee转为Object，转到基类
Employee b = (Int32)o;     //显式把Object转为Employee,转到派生类
DtaeTime yy = (DataTime)o;     //编译时正确，运行时系统抛出异常（InvalidCastExpection）。
```


3和4好象没什么区别，都是显式的转换到派生类。为什么第4个就错了呢，而且是运行时报错。因为在编译时，系统检查o为Object型，而第四个也是显式的转换到派生类。所以不会报错。但在运行时，CLR会检查o所指向的引用，发现引用类型是一个Employee型的，而不是DtaeTime本身或任何DtaeTime的派生类型。所以在运行时回抛出异常。如果CLR可以进行这种转换，就失去了安全性，想想一个DtaeTime类型值为其他类型，在程序中会怎么样呢？而类型安全正是.NET框架中非常重要的一部分。

在我们进行类型转换时，有时因为复杂的继承关系，可能不太清楚某个类型和某个类型是否兼容（能否进行转换）。这个时候我们可以使用系统提供的2个操作符号：is 和 as.关于这两个操作符，网上讨论非常的多，我这里就简单介绍下，要注意的是2个操作符号只能用于引用转换、装箱转换和取消装箱转换。expression is/as type,其中： expression 引用类型的表达式。 type为类型。

is主要是判断某个对象或引用是否属于某个类型，如果是就返回true.不是就返回false.如果对象或引用为NULL，也返回false

```csharp
Employee a = new Employee();   
Object o = a;  
//返回true，执行转换
if (o is Employee)
{
  Employee b = (Employee)o;
}
//返回false，不执行转换
if (o is DtaeTime) 
{
  DtaeTime yy = (DataTime)o;
}  
```

在上面的例子中看到，进行转换的话进行了2次兼容性检查（if中用is来检查。转换时CLR进行的检查）。下面是用as进行的检查，相比较as更简洁，效率更高。但要注意的是用as进行检查后，需要加上一个if判断，如果没有判断直接使用了指向空的引用，会引发一个NullReferenceException的异常。使用as时，只有CLR对类型进行了一次检查。后面只需要检查是否为空就行了。

```csharp
Employee a = new Employee();   
Object o = a;  

Employee b = o as Employee;   //这里CLR检查o所引用的对象是不是Employee,是则返回所指的对象的非空指针。
if (b==BULL)                  //检查是否为空
{...}
else
{...}

DtaeTime yy = o as DataTime;  //这里CLR检查o所引用的对象是不是DataTime,不是则返回NULL，那么yy的引用为空。
if (yy==BULL)                //检查是否为空
{...}
else
{...}
```
上面主要讲的是引用类型的转化，而对于一些值类型和基元类型的转化会简单一些。这里基元类型主要针对值类型。

```csharp
Int32 a = 5;
Int64 b = a;
Int16 c = (Int16)a;
```

如果按前面讲的，这个地方可能无法转化成功，因为a,b,c的类型都不同，而且没有继承关系（值类型不能被继承）。但实际上他是可以执行的。因为对于这些C#编译器熟悉这些基元类型，在编译时回安装自己的规则。也就是说编译器能事变一些通用的编程模式，产生必要的IL代码来使之按期望运行。

如果两个类型间是安全的，那么是可以进行隐式的转换，如果是不安全的，也就是可能导致丢失精度或者数量级的丢失，就需要显式的来转换了。对于小数转正数，C#是直接丢弃小数部分的。关于基元类型的转换规则可以参考C#语法。

对于编译器没有把某些类型当作基元来支持，我们就不能直接进行如上转型的操作，我们可以使用System.Convert类型的静态方法，在不同的对象类型之间进行转换。Convert知道如何在FCL提供的核心类型中间进行转换如（DtaeTime,String,Int32,Int64,Single,Double,decimal等等）。

顺便提下，对于许多基元类型运算操作可能会导致溢出，而CLLR只在32位或64位的数值上进行运算。不同的编译器对溢出处理是不同的。

CLR提供了检查和不检查溢出的运算操作码（如add,add.ovf）。在C#中可以通过check和uncheck来控制溢出时是否抛出异常。

类型转换是个非常复杂也很重要的的东西，我觉得要理解了不同的数据类型之间的关系，才能比较好的理解理解转型这个地方。

  

## 四 装箱与拆箱


前面主要都讨论的是同类型直接的转换，引用到引用，值类型到值类型。还有一种用的非常多的就是引用类型和值类型之间的转换，比如传递参数时，值类型存储到一个引用类型时。也就是经常可以听到的装箱和拆箱操作。这确实是个非常复杂的地方。因为引用类型和值类型的内存空间是不同的，也就导致了许多性能问题和拷贝使用等问题。对于每一种值类型，运行库都提供一种相应的已装箱类型，这是与值类型有着相同状态和行为的类。当需要已装箱的类型时，某些语言要求使用特殊的语法（如C++要用关键字）；而另外一些语言会自动使用已装箱的类型（C#是自动的）。在定义值类型时，需要同时定义已装箱和未装箱的类型。

由前面我们可以知道，值类型是分配在线程的堆栈上，而引用类型是分配在托管堆上的。所谓的‘装箱’也就是把值类型转化为一个引用类型的过程。实际上也就是把分配在堆栈上的对象，装箱（可能说打包比较形象，前面说过分配在托管堆的对象回又个附加成员），然后重新分配到托管堆上的。装箱操作通常由以下几步组成：
1：在托管堆上为新生成的引用类型分配内存空间。这个空间大小为值类型本身大小和附加成员（方法表指针和SyncBlockIndex）
2：将值类型实例的字段拷贝到托管堆上新分配对象的内存中。
3：返回托管堆中新分配对象的地址。这个地址就是一个指向对象的引用。值类型实例也就变成了一个引用类型对象。 

```csharp
//声名一个值类型
struct Point
{
  public Int32 x,y;
}


class app
{
 static void Main()
 {
   ArrayList a = new ArraryList();
   Point p ;            //值类型分配到线程堆栈上
   for(Int32 i = 0; i < 10 ;i++)
   {
      p.x = p.y = 1;    //初始化值类型成员
      a.Add(p);         //Add(Object obj)方法要接受一个引用类型，所以会对p进行装箱操作。
   }
 }
}
```

上面的代码中，会先在托管堆上为point分配一个内存空间，然后把p中字段传到新的内存空间中去。然后返回这个新内存空间的地址，也就是指向这个新分配的对象的地址给add方法。这时传个add方法的就是托管堆上新对象了。这个装箱过程对于C#来说是自动完成的。这个新的对象会最后会被垃圾回收器回收掉。而这个时候，之前的分配在线程堆栈上的对象p则可以被重用或者释放。经过装箱后的值类型的生存期超过了未装箱的值类型。

但我们想从ArrayList中取得这个新的Point对象时，因为他是值类型，我们就必须把他从托管堆上拷贝到线程堆栈上进行使用。这就涉及到拆箱的操作。拆箱操作是指获取已装箱值类型的对象的地址的过程。这也就是说拆箱和装箱不是想对的。往往在拆箱后还要把此对象的字段拷贝到线程堆栈上的值类型实例中。可见拆箱的代价很下，他只获得对象的地址。要注意的是，在拆箱后进行转型时，必须先转为他原来未装箱时的类型。

```csharp
Int32 x = 5;
Object o = x ;
Int16 y =(Int16)o;    //错误
Int16 y =(Int32)(Int16)o; //正确
```

在装箱和拆箱的过程中，会从速度和内存两方面损伤应用程序的性能。所以我们应该清楚编译器何时产生这些操作的指令，并在程序中尽可能的减少这种情况。看下面的代码，为了修改P，两次装箱代价非常大。

```csharp
Point p;
p.x = p.y = 1;
Object o = p;    //p第一次装箱

p = (Point) o;   //拆箱并拷贝
p.x = 2;     
o = p;           //第二次装箱，又会分配一个新的内存空间。
```

下面看一些列子：

```csharp
using System;

class ex1
{
    public static void Main()
    {
        Int32 v = 5 ;
        Object o = v;   
        v = 123;
        Console.WriteLine (v + "," + (Int32)o);
    }
}
```

这样的程序进行了几次装箱操作呢？答案是3次。可以通过看IL代码来了解：

```asm
.method public hidebysig static void  Main() cil managed
{
  // 代码大小       46 (0x2e)
  .maxstack  3
  .locals init (int32 V_0,
           object V_1)
  //把5入栈
  IL_0000:  ldc.i4.5
  IL_0001:  stloc.0
  //装箱
  IL_0002:  ldloc.0
  IL_0003:  box        [mscorlib]System.Int32    
  IL_0008:  stloc.1

  //把123存到v中
  IL_0009:  ldc.i4.s   123
  IL_000b:  stloc.0
  //对V装箱
  IL_000c:  ldloc.0
  IL_000d:  box        [mscorlib]System.Int32  

  //把,入栈 
  IL_0012:  ldstr      ","
  //对o拆箱
  IL_0017:  ldloc.1
  IL_0018:  unbox      [mscorlib]System.Int32
  IL_001d:  ldind.i4

  //对o装箱,可以看到调用了String::Concat方法都接受Object型
  IL_001e:  box        [mscorlib]System.Int32    
  IL_0023:  call       string [mscorlib]System.String::Concat(object,
                                                              object,
                                                              object)
  IL_0028:  call       void [mscorlib]System.Console::WriteLine(string)
  IL_002d:  ret
} // end of method ex1::Main
```

第一次装箱和你好看出，而后两次是发生在Console.WriteLine方法中的，对于连接多个字符串时，他接受的类型是Object型，看到上面的方法中第一个参数v和最后一个(Int32)o，这两个地方发生了装箱。

对于这个Console.WriteLine方法可以改为Console.WriteLine (v + "," + o)，这样的话，最后一个参数o已经是引用参数了，就会减少一次装箱。重新编译后查看IL代码会发现 代码大小变小了。

查阅MSDN可以发现Console.WriteLine方法有多种重载的方法，比如为Int32,Char,Single等基元类型提供了重载，这样在单独输出这些类型的时候就避免的装箱的操作。其他一些类型也有类似重载方法，目的就是为了减少值类型装箱操作，提高效率。我们自己在写程序时，如果一个类型可能导致系统对他反复装箱，我们就应该自己为他装箱

```csharp
Int32 v = 5 ;
Console.WriteLine ("{0},{1},{2}",v,v,v); //3次装箱

Int32 v = 5 ;
Object o = v;    //1次装箱    
Console.WriteLine ("{0},{1},{2}",o,o,o); 
```

 

## 五 继承和重写通用操作


前面提到过Object类型有一些公有的方法，而所有的类型都继承与Object类型，所以他们都能继承这些通用操作方法。当然Object中实现的这些方法可能不能满足我们的类型的需求，这个时候我们可以对他进行重写，来实现自己的方法。

比如Equals方法，在Object中实现是用来判断2个引用类型是否指向同一个对象。这对于我们要判断2个对象的值相等时就显得不够了，所以我们需要对他进行重写。对于重写了Equals方法的类型，一般也要求重写他的GetHashCode方法。这里Equlas方法的重写比较麻烦，要分为引用类型和值类型的重写。而引用类型中又分为基类重写了此方法和没有重写此方法两钟。对Equals方法重新写主要是比较：
1：对象是否为空
2：调用基类此方法判断（对于引用类型的基类重写了此方法的需要判断，如果调用基类会导致调用Object.Equlas是就不调用）
3：2个对象类型是否相同
4：2个对象中的所有字段是否相等
5：最后需要对 ==，!=，这些操作符进行重载

对于值类型来说，因为所有值类型是继承ValueType,而他又是继承Object的。ValueType重写了Equlas方法。他在内部使用了反射，然后比较每个字段。这样效率比较低，但所有的值类型都能继承。

另外一个常用的方法就是MemberwiseClone。他是创建一个新的类型实例，并将去字段设置为和this对象的字段相同。最后返回创建实例引用。在系统中拷贝一个对象会经常用到，其中拷贝有两种，浅拷贝和深拷贝。利用MemberwiseClone来进行的是深拷贝，因为他创建一个新的对象，然后把他的字段和被拷贝的对象置成相同的。而浅拷贝，是指当对象的字段值被拷贝时，字段引用的对象不会拷贝。也就是说只拷贝堆栈上的数据，而不拷贝所指向托管堆栈的对象。

而一个对象是否允许被Clone就要看他是否实现了ICloneable接口。

```csharp
Public interface ICloneable
{ 
  Object Clone();
}

//定义一个类型
Class MyClass
{
  ArrayList set;
  MyClass();
}

//浅拷贝
MyClass o1 = new MyClass();
MyClass o2 = o1； 


//要想实现深拷贝，在Clone方法中调用MemberwiseClone就行了
Class MyClass : ICloneable
{
 public Object Clone()
 {
   return MemberwiseClone();
 }
}
MyClass o1 = new MyClass();
MyClass o2 = o1.Clone();

 

//另一种方法就是自己分配一个新对象，实现深拷贝，不调用MemberwiseClone。
Class MyClass : ICloneable
{
 ArrayList set;

 //定义Clone方法的私有构造器
 private MyClass(ArrayList set)
 {
   this.set = (ArrayList)set.Clone()
 }

 public Object Clone()
 {
   return new MyClass(set);  //构造一个新对象，构造器参数为原来对象中使用的ArrayList。
 }
}

MyClass o1 = new MyClass();
MyClass o2 = o1.Clone();
```





上面讲的都是引用类型，而对于值类型他本身就支持浅拷贝。如果我们自己定义的一个值类型，也希望实现深拷贝，就需要我们自己来实现ICloneable接口，最好不要使用MemberwiseClone方法，而是分配一个对象，自己实现深拷贝。

 这里是关于MemberwiseClone方法：

```csharp
using System;
using System.Collections;

class ex :ICloneable
{
    internal int x;
    internal ex1 y;
    public ex()
    {
        x = 0;
        y = new ex1();
    }

    public Object Clone()
    {
        return MemberwiseClone();
    }
}

class ex1
{
    internal int n;
    public ex1()
    {
        n = 0;
    }

    public ex1(int n)
    {
        this.n = n;
    }
}

class examlpe
{
    static void Main()
    {
        ex e1 = new ex();
        ex e2 = (ex)e1.Clone();
        e1.x = 1;
        e1.y.n = 5;
        Console.WriteLine(e1.x);
        Console.WriteLine(e1.y.n);
        Console.WriteLine(e2.x);
        Console.WriteLine(e2.y.n);
    }
}
```




输出结果入下：

```
1
5
0
5
```

从结果可以看出，实例的int字段指向不同地址，修改了e1.x不会修改e2.x。而对于引用字段y，在Clone时，只复制了y的引用，ex1.y和ex2.y指向同一个地址。所以修改了一个会影响另一个。

MemberwiseClone是浅拷贝。对于浅拷贝是把值拷到新空间中，而深拷贝把引用拷到新的空间中。而和是否创建新的实例没有关系。因为以前一直受下面这个影响，所以对浅拷贝理解的比较浅。

```csharp
ex e1 = new ex();
ex e2 = e1;
```


一直以为只有这一种浅拷贝方式，2个对象指向同一个实例就是签拷贝。但MemberwiseClone方法可以看出。创建了新的实例，2个对象指向不同的实例，也可能是浅拷贝。因为实例中如果是引用字段，复制的时候只是复制的引用地址。所以看到底是深拷贝还是浅拷贝，还是要看实现的时候对引用对象是复制地址还是值。而MemberwiseClone是复制引用地址，所以是浅拷贝。

 

## 六 命名空间和程序集


这个在书中出现了就顺便提下。很多人可能弄不清楚命名空间和程序集的关系。用一句比较通俗的说法就是，程序集是类型的物理存储形式，他表示某个类型在哪个文件中。而命名空间则是类型的逻辑存储形式。就好比学校里的学生就是类型，他们住的寝室好比程序集，而他们的班级是命名空间。不同程序集的类型可能属于同一个命名空间。一个程序集中的类型也可能包含属于多个命名空间。







# （四） 类型设计

 

 

 这段时间工作有点忙，系统刚上线，也有点累。这段时间主要看到了书中.net类型设计这一部分，这一部分主要介绍的是如何用不同的成员来设计一个类型。

## 一：类型成员介绍

面向对象的语言中，最重要的一个特点就是封装，说到最多的一个词就是类。什么是类，类是定义同一类所有对象的变量和方法的蓝图或原型。让数据同操作分离开。人类是一个类，而我们每个人就是这个类的一个实例，也就这个类的一个对象。 我们这里说的类型，并不是程序中的类Class,因为我们知道了类型有，值类型，引用类型。这里用类Class主要是帮助大家理解，毕竟它是用的最多。

一个类型，他可以包括以下一个或多个成员：

常数：常数是一个表示恒定不变的数值符号。常数 总是和类型而非他们实例相关联，所以他们可以说总是静态的。

字段：字段表示一个数据的值，他可以是只读的，也可以是可读写的。字段分为动态字段和实例字段。一般字段声名为私有，这也是面向对象中推荐的作法，防止字段被随意操作。也就是我们类中定义的数据成员。

方法：方法是一个函数，用来改变或查询一个类型，或者一个对象的状态。也就是我们类中定义的成员函数。

属性：属性也是一种方法，当他比较特殊，他主要是用来设置和保护类中的数据成员。

事件：用通俗的话来说，就是你操作某个对象时，执行的某一个方法。他只在特定的时间，由指定的对象来执行指定的方法。

除了上面介绍的这些，一个类型中还包括，构造器（类型初始化用），重载操作符，转换操作符等。对于一个类型，内部可以嵌套定义其他类型。这可以使得复杂的类型划分为小的代码块，简化实现。

下面是类型成员的限定修符的解释：

 ![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/1.jpg)

图1

![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/2.jpg)

图2

当需要限制一个类型（比如Console和Math），不能被继承也不能被实例化的时候希望可以使用Abstract和Sealed，但是CLR并不支持，所以

如果想实在C#中现这样一个类型，可以把类型限制为Sealed，然后定义一个私有(Private)无参数构造器。这样可以防止编译器自动产生一

个公有的构造器。没有构造器可访问，也就无法实例化。

当源代码被编译时，编译器会检查他们是否正确的引用了类型和成员。如果源代码中非法引用了一些类型或成员，编译器会产生相应的错误

信息。在JIT对IL代码进行编译时也会进行检查，如有非法的引用则会抛出异常，这样就保证了代码能安全的执行。

 

## 二：数据成员

一个类型中的数据成员主要包括常数和字段两种。

对于一个常数，我们必须在编译的时候就确定它的值。编译后他的枝保存在元数据中，也就意味着常数的类型只能是那些被编译器认为是基元类型的（只有基元类型才能利用文本常数初始化，而其他类型需要用构造器初始化）。使用常数的好处是他内嵌在代码中不需要在为它分

配内存空间，但同时也带来了版本问题。字段是静态的，直接通过类型名来访问。

```csharp
public class Component
{
  public const Int32 MaxLixt = 20;  //C#不允许为常数指定static关键字，因为常数隐藏为static
}


class App
{
  static void Main()
  {
      Console.WriteLine(Component.MaxList);
  }
}
```


看上面的代码，在编译时，在App的元数据中，已经有了MaxList的值20，所以App虽然引用了Component，但实际运行时可以完全不需要它。

即便重新编译了Component也改变不了App输出的值。只有一起重新编译。所以如果要求一个模块在运行时获得另一个模块的数据，最好用不

要用常数，而是用只读字段。对于那些不会更改的，使用常数可以减少内存空间。

对于字段他有静态和实例两种。字段会在该类型被加载进一个应用程序域时为其分配内存，这通常发生在引用该类型的方法第一次被JIT编

译时。对于实例字段，系统在该类型的实例被构建时动态分配。

如果把上面Component程序中的定义改为 public static readonly Int32 MaxList =20;就解决了版本问题。这里定义静态字段需要显式的

定义。而readonly试其不可修改。这时只需要重新编译Component就可以改变App的输出了（前提是编译后的DLL程序集是非强命名程序集）

，虽然性能可能会有一点影响。

可以Component中的定义，是使用内联方式（声明字段的同时进行初始化赋值）进行初始化的，这是C#提供的一种简便的方法，而实际上他

们是初始化都是在构造器类完成的。

 

## 三：方法

 方法决定了一个类型所具有的功能，其中除了我们自己定义的方法外，用的最多的就是类型的构造方法、操作符重载方法、转换操作符方

法。

谈到构造器，他的作用是非常大的。他负责把类型成员初始化到一个良好的状态。构造器包括实例构造器和类型构造器，从名字也看的出他

们的不同。前者主要是针对实例成员，而后者主要针对类型成员。

对于可验证代码，CLR要求每个类至少定义一个实例构造器。在创建一个引用类型的实例时系统执行3个步骤：

1：为该实例分配内存
2：初始化对象的附加成员（方法表指针和一个ScBlockIndex)
3：调用类型的实例构造器设置对象的初始状态（在调用构造器之前系统为该对象分配的内存总是被设置为0）

PS：关于类型的内存分配问题，之前的学习笔记中已经介绍过了，但介绍的比较粗略。特别是方法表这块，目前还不是太清楚，比如装箱类

型通过方法表指针来访问自己的方法，但方法表存放在那呢？是什么形式呢？非装箱类型有没有方法表呢？又是如何访问的呢？在加上虚方

法，继承，多态等，非常复杂，暂时就不深究了。以后在专门研究了。懂的朋友也可以告诉我。在网上找了相关的资料大家可以看下：

<http://www.cnblogs.com/blusehuang/archive/2007/10/23/833593.html>

具体的构造方法就看一些例子把

1：使用默认构造器。

```csharp
class example
{
    int a;
    static int b;

}

//对应默认构造器的IL代码
.method public hidebysig specialname rtspecialname 
        instance void  .ctor() cil managed
{
  // 代码大小       7 (0x7)
  .maxstack  1
  IL_0000:  ldarg.0
  IL_0001:  call       instance void [mscorlib]System.Object::.ctor()
  IL_0006:  ret
} // end of method example::.ctor
```

2：显式定义构造器：

```csharp
class example
{
    int a;
    static int b;

    public example()
    {
        a  = 0;
      }

    public example(int a)
    {
        this.a = a;
    }
}

//系统产生了两个构造器的IL代码

//无参数的构造器
.method public hidebysig specialname rtspecialname 
        instance void  .ctor() cil managed
{
  // 代码大小       14 (0xe)
  .maxstack  2
  IL_0000:  ldarg.0
  IL_0001:  call       instance void [mscorlib]System.Object::.ctor()
  IL_0006:  ldarg.0
  IL_0007:  ldc.i4.0
  IL_0008:  stfld      int32 example::a
  IL_000d:  ret
} // end of method example::.ctor

//带参数的构造器
.method public hidebysig specialname rtspecialname 
        instance void  .ctor(int32 a) cil managed
{
  // 代码大小       14 (0xe)
  .maxstack  2
  IL_0000:  ldarg.0
  IL_0001:  call       instance void [mscorlib]System.Object::.ctor()
  IL_0006:  ldarg.0
  IL_0007:  ldarg.1
  IL_0008:  stfld      int32 example::a
  IL_000d:  ret
} // end of method example::.ctor
```

可见对字段a进行的初始化，之前仍旧调用了基类的构造方法。两个构造器的IL基本一样，但实际还是有点小区别。要注意的是,一旦自己申明了一个含参实例构造器,那么系统就不会在自动生成一个默认的无参构造器。如果申明一个public example(){}的构造器，这样一个构造器是默认的构造器是完全一样的。

3：以内联方式初始化变量：

```csharp
class example
{
    int a = 1;
    static int b;

}

.method public hidebysig specialname rtspecialname 
        instance void  .ctor() cil managed
{
  // 代码大小       14 (0xe)
  .maxstack  2
  IL_0000:  ldarg.0
  IL_0001:  ldc.i4.1
  IL_0002:  stfld      int32 example::a
  IL_0007:  ldarg.0
  IL_0008:  call       instance void [mscorlib]System.Object::.ctor()
  IL_000d:  ret
} // end of method example::.ctor
```

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

我们在程序中直接给a进行了赋值，但如前面提到过的，实际这只是一种简化方式，实际上，他也是在构造器中进行初始化的。首先给a赋值，然后调用了基类的构造器。如果我定义了多个构造器，每个构造器都会先把a初始化。这个和第一个例子基本一样，但可以发现这种方式使得代码大小大了很多。所以尽量避免这种方式。在看下面

```csharp
class example
{
    int a = 1;
    static int b;

    public example()
    {
      a = 0;
    }

}

.method public hidebysig specialname rtspecialname 
        instance void  .ctor() cil managed
{
  // 代码大小       21 (0x15)
  .maxstack  2
  IL_0000:  ldarg.0
  IL_0001:  ldc.i4.1
  IL_0002:  stfld      int32 example::a
  IL_0007:  ldarg.0
  IL_0008:  call       instance void [mscorlib]System.Object::.ctor()
  IL_000d:  ldarg.0
  IL_000e:  ldc.i4.0
  IL_000f:  stfld      int32 example::a
  IL_0014:  ret
} // end of method example::.ctor
```

这个使用了内联定义的，然后又显式的使用了构造器，最后a的值应该是0。可见这样让代码变的更多了。

4：内联构造器的问题

```csharp
//使用内联方式初始化a,b
class example
{
    int a = 0;
    int b = 1;
    string s;

    
    public example(string s)
    {
        this.s = s; 
    }
}

.method public hidebysig specialname rtspecialname 
        instance void  .ctor(string s) cil managed
{
  // 代码大小       28 (0x1c)
  .maxstack  2
  IL_0000:  ldarg.0
  IL_0001:  ldc.i4.0
  IL_0002:  stfld      int32 example::a
  IL_0007:  ldarg.0
  IL_0008:  ldc.i4.1
  IL_0009:  stfld      int32 example::b
  IL_000e:  ldarg.0
  IL_000f:  call       instance void [mscorlib]System.Object::.ctor()
  IL_0014:  ldarg.0
  IL_0015:  ldarg.1
  IL_0016:  stfld      string example::s
  IL_001b:  ret
} // end of method example::.ctor

//使用一个构造器来初始化a,b
class example2
{
    int a ;
    int b;
                     string s;
 //定义一个默认构造器
    public example2()
    {
      a = 0;
      b = 1;
    }
//调用默认构造器初始化a,b否则系统自动调用基类的构造函数初始化a,b
    public example2(string s):this()
    {
        this.s = s;
    }
}

.method public hidebysig specialname rtspecialname 
        instance void  .ctor() cil managed
{
  // 代码大小       21 (0x15)
  .maxstack  2
  IL_0000:  ldarg.0
  IL_0001:  call       instance void [mscorlib]System.Object::.ctor()
  IL_0006:  ldarg.0
  IL_0007:  ldc.i4.0
  IL_0008:  stfld      int32 example2::a
  IL_000d:  ldarg.0
  IL_000e:  ldc.i4.1
  IL_000f:  stfld      int32 example2::b
  IL_0014:  ret
} // end of method example2::.ctor

.method public hidebysig specialname rtspecialname 
        instance void  .ctor(string s) cil managed
{
  // 代码大小       14 (0xe)
  .maxstack  2
  IL_0000:  ldarg.0
  IL_0001:  call       instance void example2::.ctor()
  IL_0006:  ldarg.0
  IL_0007:  ldarg.1
  IL_0008:  stfld      string example2::s
  IL_000d:  ret
} // end of method example2::.ctor
```




通过上面我们看到，可以发现上面2个对S字段初始化的IL代码，采用内联方式的代码大小为28，而后面一种方法为14。小了很多。可能有人会说，那时因为对a,b初始化在默认构造器内，但你想想当我多几个字段，多几个重载的构造器会怎么样？这里用到了 public example2(string s):this()，用this()表示在初始化s的时候调用默认的构造函数初始化a,b，否则系统会自动调用基类的默认构造器。

所以如果我么有一些需要初始化的字段和许多重载的构造器方法，最好显式的在默认构造器内初始化，然后在初始化其他字段的构造器中调用这个默认构造器。这也相当于一个重用。

5：值类型构造器

CLR并不要求值类型必须定义构造器方法，实际上C#编译器也不会为值类型产生默认的无参构造器。但CLR允许我们为值类型定义构造器。当我们使用构造器，只有我们显式调用构造器才会被执行，用new来创建一个值类型时，只是调用的他的构造器（而引用类型因为是分配在托管堆上，new的时候有3个步骤，前面介绍过）。如果不使用new来创建，那么值类型的字段都会保持为0，因为前面介绍过，在调用构造器之前系统为该对象分配的内存总是被设置为0。

需要注意的时，不能给值类型定义一个无参构造器，所以也不能在值类型中直接给变量复制，必须通过构造器。严格的说只有内嵌于引用类型的值类型字段，CLR才会保证字段被初始化为0或null,基于堆栈的值类型字段无法保证。但因为CLR的代码可验证机制，要求所有基于堆栈的值类型字段在使用前都必须被赋值，而编译器可以保证这一点，这就保证了运行时不会出现异常。

```csharp
struct example
{
    int a ;
    int b ;
    public example(int a ,int b)
    {
        this.a = a;
        this.b = b;
    }
    
}

.method public hidebysig specialname rtspecialname 
        instance void  .ctor(int32 a,
                             int32 b) cil managed
{
  // 代码大小       15 (0xf)
  .maxstack  2
  IL_0000:  ldarg.0
  IL_0001:  ldarg.1
  IL_0002:  stfld      int32 example::a
  IL_0007:  ldarg.0
  IL_0008:  ldarg.2
  IL_0009:  stfld      int32 example::b
  IL_000e:  ret
} // end of method example::.ctor
```

6：类型构造器

除了上面介绍的，用的最多的实例构造器外，还有类型构造器。他主要左右是初始化类型中的静态字段。默认的类型中没有定义类型构造器，要定义的话，也只能定义一个，并且不能有任何参数。

```csharp
class example
{
    static int a ;
    static int b = 1 ;
    static example()
    {
        a = 2;
        b = 1;
    }
    
}

.method private hidebysig specialname rtspecialname static 
        void  .cctor() cil managed
{
  // 代码大小       19 (0x13)
  .maxstack  1
  IL_0000:  ldc.i4.1
  IL_0001:  stsfld     int32 example::b
  IL_0006:  ldc.i4.2
  IL_0007:  stsfld     int32 example::a
  IL_000c:  ldc.i4.1
  IL_000d:  stsfld     int32 example::b
  IL_0012:  ret
} // end of method example::.cctor

.method public hidebysig specialname rtspecialname 
        instance void  .ctor() cil managed
{
  // 代码大小       7 (0x7)
  .maxstack  1
  IL_0000:  ldarg.0
  IL_0001:  call       instance void [mscorlib]System.Object::.ctor()
  IL_0006:  ret
} // end of method example::.ctor
```

可以看到和实例类型差不多，只是他构造器的方法是cctor()，而且不调用任何基类的类型构造器。实际上也不能这么做，因为静态字段并不从基类继承。还可以发现后面还有个ctor的实例构造器，这是系统自动为引用类型产生的，而如果是值类型就不会有这个实例构造器。

通过上面，应该是对构造器有了个大概的了解，通过IL代码也了解了构造的过程，下面列出要注意的地方

a：对于引用类型系统会自动产生实例构造器。实例构造器在访问基类字段之前都必须调用基类的构造器。编译器会自动产生对基类默认构造器的访问代码。每一个构造器最后总会调用到system.object的公有无参构造器。

b：构造器可以采用内联的方式来初始化，但这样会导致代码量的增加。

c：对于值类型，系统不会自动产生一个默认的构造器。我们可以自己定义，但不允许定义一个无参的值类型实d例构造器。而且对于手动定义的构造器必须初始化值类型中所有的字段。

d：对于类型构造器，类型中默认是没有定义的。我们只能定义一个没有参数的类型构造器。而且构造器类型必须为static。默认为私有，并且不允许我们显式定义。类型构造器不应该调用其基类的类型构造器。

 

 四：重载符和转换操作符

1：重载操作符

操作符号重载是用的比较多的，比如对于string类型来说，为什么可以用'+' 来连接2个字符串，用=，！=来判断字符串是否相等？实际上CLR对于这些符号一无所知。但他去归法了语言怎样提供操作符重载。

```csharp
class String 
{
    public static String operator+(String s1,String s2){...........}
}
```

比如String类型中定义了2个操作符的表现行为。在编译的时候，系统会产生名为op_Addition的方法，并且该方法有一个specialname标记。当编译器在代码中碰到+操作符时，它们会去看其中的操作数类型中有那一个定义了参数类型和操作数类型兼容、名为op_Addition的specialname方法。如果存在就调用此方法，否则出现编译错误。

对于一些核心类型（Int32,Int64等），他们没有定义任何操作符重载方法，因为CLR提供了直接操作这些类型实例的IL指令（如Add）。如果为他们提供的重载方法，编译器产生的代码实际也是调用了这些IL指令，那么性能就会有所损失。所以说如果某个编程语言不支持某个FCL核心类型，那么我们不能在其实例上进行任何操作。比如VB就不支持无符号整数。

不是所有的语言都支持操作符重载，所以如果我们在VB上对一个非基元类型使用+操作符时，编译器将产生一个错误。所以在VB上，我们只能定义一个与+功能相同的方法来实现。下面看看C#和VB操作符的互操作。

 

```csharp
//C#定义+操作符方法
public class CSharpex
{
  public static CSharpex operator+(example e1,example e2)

}


//VB中使用
Public Class VBex
   Public Shared Sub Main()
     Dim cs as new CSharpex()
     ’需要注销掉这句，因为VB不支持，他不知道如何把+翻译成op_Addition方法
     ‘cs = cs + cs   
      ‘用这种方式可以显式使用+操作符的方法，只是没有用+ 好看
     cs = CSharpex.op_Addition(cs,cs)
  End Sub
End Class


//在VB中只能这样定义操作符号，而不能用+
Public Class VBex
   Public Shared Function op_Addition(a as VBex, b as VBex)
      Return Nothing
  End Sub
End Class

//在C#中调用时，也只能使用op_Addition，而不能使用+，因为VB编译器不会给此方法加上specailname标记。
```

 

2：转换操作符

前面几次我们讲过了类型转换，那么他们是怎么实现的呢？如果想把一个自己定义的类型转换为Int型要怎么做呢？通过定义下面的转换操作符的方法，我们可以把一个类型转换为另一个类型。

```csharp
using System;


public class Rational
{

    //由Int32构造
    public Rational(Int32 num){}
    
    //由Single构造
    public Rational(Single value){}
    
    //转化为Int32
    public Int32 ToInt32(){}
    
    //转换为Single
    public Singal ToSignal(){}

    //由Int32隐式构造一个Rational并返回
    public static implicit operator Rational(Int32 num)
    {return new Rational(num);}

    //由Single隐式构造一个Rational并返回
    public static implicit operator Rational(Single value)
    {return new Rational(value);}

    //由Rational显式返回一个Int32
    public static implicit operator Int32(Rational r)
    {return r.ToInt32();}

    //由Rational显式返回一个Single
    public static implicit operator Single(Rational r)
    {return r.ToSingle();}
}
```



和重载操作符一样，转换操作符方法也必须为public 和static。不同的是，我们还要告诉编译器是显式还是隐式的进行操作。其中implicit告诉编译器需要隐式的进行转换，而explicit为显式的进行转换。后面的operator告诉编译器该方法是一个转换操作符。对于转换过程中不可能丢失精度或数量及的转换，应该定为隐式的；而对于会丢失精度或数量及的应该定义为显式转换。

编译器在编译时，会产生如下的方法：

```
public static Rational op_Implicit(Int32 num)
public static Rational op_Implicit(Single value)

public static Int32 op_Explicit(Rational r)
public static Single op_Explicit(Rational r)
```

可以看到最后两个方法的IL代码只有返回类型不同，CLR是支持只有返回类型不同的方法的，但C#，VB这些语言都不支持的。C#中不能为一种转换操作同时定义隐式转换操作和显式转换操作。

3：关于类型转换：

类型转换确实是个很复杂的地方。昨天后来大概看了下.C#里的类型转换，主要是使用强制转换，使用Convert,使用类型的ToXXX()方法。下面就讲下我的理解。

1：对于基元类型间进行转换时，CLR会产生支持的IL指令，而使用Convert时是调用相应的方法

```csharp
public class example
{
    public static void Main()
    {
        float a = 10.0f;
        int b = (int)a;
        int c = Convert.ToInt32(a);
    }
}
.method public hidebysig static void  Main() cil managed
{
  // 代码大小       17 (0x11)
  .maxstack  1
  .locals init (float32 V_0,
           int32 V_1,
           int32 V_2)
  IL_0000:  ldc.r4     10.
  IL_0005:  stloc.0
  IL_0006:  ldloc.0
  IL_0007:  conv.i4                 
  IL_0008:  stloc.1
  IL_0009:  ldloc.0
  IL_000a:  call       int32 [mscorlib]System.Convert::ToInt32(float32)
  IL_000f:  stloc.2
  IL_0010:  ret
} // end of method example::Main
```




对于强制类型转换，使用了conv.i4的指令，使用Convert时是调用Convert::ToInt32(float32)。可见对基元类型之间使用强制转换的效率是最高的。我想使用Convert在内部应该也是调用此指令。

2：对于其他一些非基元类型的转换

```csharp
public class example
{
    public static void Main()
    {
        float a = 10.0f;
        
        decimal d = (decimal)a;
        decimal e = Convert.ToDecimal(a);
    }
    
}

.method public hidebysig static void  Main() cil managed
{
  // 代码大小       22 (0x16)
  .maxstack  1
  .locals init (float32 V_0,
           valuetype [mscorlib]System.Decimal V_1,
           valuetype [mscorlib]System.Decimal V_2)
  IL_0000:  ldc.r4     10.
  IL_0005:  stloc.0
  IL_0006:  ldloc.0
  IL_0007:  conv.r4
  IL_0008:  call       valuetype [mscorlib]System.Decimal [mscorlib]System.Decimal::op_Explicit(float32)
  IL_000d:  stloc.1
  IL_000e:  ldloc.0
  IL_000f:  call       valuetype [mscorlib]System.Decimal [mscorlib]System.Convert::ToDecimal(float32)
  IL_0014:  stloc.2
  IL_0015:  ret
} // end of method example::Main
```

Decimal是个比较特殊的类型，他不同于其他基元类型，可以看到Decimal类型中定义了转换操符号，在强制转换时，调用了此方法，而没有对应的IL指令。可见非基元类型之间的转换的效率会低一些。在看看使用Convet进行转换的，他使用的是Convert类型中的ToDecimal方法。

3：关于Convert类

这个类将一个基本数据类型转换为另一个基本数据类型。我们知道，基元类型基本都继承了IConvertible 接口。此接口提供特定的方法，用以将实现类型的实例值转换为具有等效值的公共语言运行库类型。

通常，公共语言运行库通过 [Convert](ms-help://MS.VSCC.2003/MS.MSDNQTR.2003FEB.2052/cpref/html/frlrfsystemconvertclasstopic.htm) 类公开 **IConvertible** 接口。基元使用 [Convert](ms-help://MS.VSCC.2003/MS.MSDNQTR.2003FEB.2052/cpref/html/frlrfsystemconvertclasstopic.htm) 类进行转换，而不是使用此类型的 **IConvertible** 显式接口成员实现。也就是说，调用[Convert](ms-help://MS.VSCC.2003/MS.MSDNQTR.2003FEB.2052/cpref/html/frlrfsystemconvertclasstopic.htm) 类的静态方法，和类型本身实现的 **IConvertible** 接口方法是一样的。只是基元类型没有公开自己实现的**IConvertible** 接口。所以我们不能使用Int32.ToSingle(),这样的方法，而要使用Convert.ToSingle(Int32).

看看下面的例子：

```csharp
public class example
{
    public static void Main()
    {
        float a = 10.0f;
        decimal d = (decimal)a;
        decimal e = Convert.ToDecimal(a);
        int f =decimal.ToInt32(d);
        int g = Convert.ToInt32(d);
        int h = (int)d;
    }
    
}

.method public hidebysig static void  Main() cil managed
{
  // 代码大小       45 (0x2d)
  .maxstack  1
  .locals init (float32 V_0,
           valuetype [mscorlib]System.Decimal V_1,
           valuetype [mscorlib]System.Decimal V_2,
           int32 V_3,
           int32 V_4,
           int32 V_5)
  IL_0000:  ldc.r4     10.
  IL_0005:  stloc.0
  IL_0006:  ldloc.0
  IL_0007:  conv.r4
  IL_0008:  call       valuetype [mscorlib]System.Decimal [mscorlib]System.Decimal::op_Explicit(float32)
  IL_000d:  stloc.1
  IL_000e:  ldloc.0
  IL_000f:  call       valuetype [mscorlib]System.Decimal [mscorlib]System.Convert::ToDecimal(float32)
  IL_0014:  stloc.2
  IL_0015:  ldloc.1
  IL_0016:  call       int32 [mscorlib]System.Decimal::ToInt32(valuetype [mscorlib]System.Decimal)
  IL_001b:  stloc.3
  IL_001c:  ldloc.1
  IL_001d:  call       int32 [mscorlib]System.Convert::ToInt32(valuetype [mscorlib]System.Decimal)
  IL_0022:  stloc.s    V_4
  IL_0024:  ldloc.1
  IL_0025:  call       int32 [mscorlib]System.Decimal::op_Explicit(valuetype [mscorlib]System.Decimal)
  IL_002a:  stloc.s    V_5
  IL_002c:  ret
} // end of method example::Main
```

看下这段代码就明白了，最后三个转换，使用了三种方法：

一是直接调用Decimal类型继承**IConvertible** 接口的静态实现方法。
二是通过Convert来实现，从IL代码来看也看的出区别。为什么他可以用，而Int32这些不行呢，因为他公开了接口实现，所以可以使用decimal.ToInt32(d)。
三是使用强制转换，用到了转换操作符。

总结：

总的说类型转换（.net1.1 C#）主要是上面3种方法：

1：使用强制类型转换，对于基元类型，系统会自动产生它对应的IL指令，而其他类型会去检查是否定义了转换操作符。
2：使用类型继承**IConvertible** 接口的实现的转换方法。这只对非基元有效，并且开放了这些方法的访问。
3：使用Convert类来实现。主要是对基元来使用，因为他们都不开放**IConvertible** 接口的实现，所以只能使用Convert来实现。
4：Decimal是个列外，他又开放了**IConvertible** 接口的实现，也可以用Convert来实现，所以它是不是基元类型很难说。在C#中是的，但在其他语言中就不一定了。
5：关于值类型和String类型的转换，比较复杂，但我们可以看到，基元类型都继承并重写了ToString()的实例方法。所以我们可以直接使用int32.ToString()来实现。而类型中的Parse的静态方法则提供了把String型转换为本类型的功能。

 

疑问：基元类型为什么不和Decimal一样提供ToXXX()方法呢？他内部是否实现了这些方法？

解答：通过用Reflector，我看了下他们

1：Int32里是类似下面的方法，可以发现实际是调用了Convert.ToInt16里的实现 
并且是受保护的方法。内部也没有转换操作符 。

```csharp
short   IConvertible.ToInt16 (IFormatProvider   provider) 
{ 
        return   Convert.ToInt16(this.m_value); 
}
```


2：decimal里面也与这样的受保护的实现方法

```csharp
short   IConvertible.ToInt16 (IFormatProvider   provider) 
{ 
        return   Convert.ToInt16(this); 
} 
```


但也还有如下的   静态方法。

```csharp
public   static   short   ToInt16(decimal   value) 
{ 
        int   num   =   ToInt32(value); 
        if   ((num   <   -32768)   ¦ ¦   (num   >   0x7fff)) 
        { 
                throw   new   OverflowException(Environment.GetResourceString ("Overflow_Int16")); 
        } 
        return   (short)    num; 
} 
```


看来因为类型都继承了接口，内部是必须实现接口的，但接口都是调用Conver类中的实现。 
而对于基元类型，没有定义 ToXXX()方法，所以只能通过调用Conver类中的方法。 
对于其他如Decimal类型，因为内部提供了公有静态方法，所以可以直接通过 类型调用ToXXX()方法实现。 

3：在看下Convert中的实现，实际他是调用decimal内部实现

```csharp
public   static   short   ToInt16(decimal   value) 
{ 
        return   decimal.ToInt16 (decimal.Round (value,   0)); 
}
```


4：而对于那些内部没有实现ToXXX方法的，是在Convert中实现的。

```csharp
public   static   short   ToInt16(int   value) 
{ 
        if   ((value   <   -32768)   ¦ ¦   (value   >   0x7fff)) 
        { 
                throw   new   OverflowException(Environment.GetResourceString ("Overflow_Int16")); 
        } 
        return   (short)    value; 
} 
```



看完上面基本明白了， 
1：对IConvertible接口方法的实现，都是调用Convert类中的方法。 
2：基元类型 的转换的实现基本都在Convert里面，而内部没有实现，所以要通过Convert实现转换。 
3：对于内部实现了的ToXXX()方法的（比如 Decimal），我们可以直接调用此静态方法，也可以通过Convert的方法（其实他也是调用内部实现了的ToXXX()方法） 
4：对于采用 强制类型转换，基元类型会自动产生IL指令，而其他类型则调用转换操作符方法。

关于为什么不在各自类型中直接实现IConvertible接口，直接提供接口调用，而是在Convert类中提供了方法。我觉得是因为接口是引 用类型，将一个未装箱的值类型实例转型为一个该实例实现的接口类型也需要对该实例进行装箱。如果直接使用接口，会因为装箱导致性 能损失。而使用Convert类来实现，就解决了这个问题。可能会有人问那为什么要实现IConvertible接口，这个是因为接口定义了一组功能 ，要实现这些功能就必须实现这些接口。所以继承这些接口是绝对没问题的。而提供Conver类应该是为了优化性能。对于decimal，实现方 法没有放在Convert里，而是放在自己类型中的TOxxx()方法中的，这个我想应该是为了方便使用，通过Conver或直接用decimal都可以调用 。

以上是我的理解，请大家指教。

 

## 五：方法的参数

前面我门谈到了类型中的方法，方法包括了修饰符，返回类型，方法名字和参数。这里我们谈谈参数。说到参数，大家第一个想到的例 子或许就是C语言里面交换两个数字。什么值传递，地址传递，当时听到老师讲，实在是让我头晕。到了后来，C++中为了解决这个方法参 数传递又引入了引用。当然现在对于这些已经有了比较清楚的理解，也通过前面的学习，对不同对象的分配方式有了了解，所以如果从内 存分配模型上来看更好理解。

在.NET平台路CLR里方法的参数默认都是值传递的。对于引用类型，传递时传的是指向对象的引用（引用本身是按值传递的），对于值 传递，传递给方法的是值类型实例的一个拷贝，所以直接没有关系。在方法类修改形参数是不回影响实参。而对于传引用，实参和形参指 向同一个对象，所以会相互影响。这就是传值和传引用（地址）的区别。

### 1：引用参数

CLR允许我们通过关键字（out,ref）来定义参数通过引用方式传递（这里引用方式传递叫引用参数，他和引用类型参数是不同的）。这 两个关键字会告诉编译器，产生额外的元数据来表示参数是按照引用方式传递。两个关键字的区别在与out的参数在使用前可以不被初始化 ，并且被调用方法不能直接读取参数的值，它必须在返回前给；而ref要求参数在使用前被初始化，被调用的方法可以任意读取该参数。

out方法：

```
class ex1
{
    static void Main()
    {
        Int32 x;
        SetVal (x);              //x不需要被初始化
        Console.WriteLine(x);   //显示“10”

    } 
    static void SetVal(out Int32 v) 
    {
        v =10;                 //此方法必须初始化v 
    }
}
```

 

ref方法:

 

```
class ex2
{
    static void Main()
    {
        Int32 x = 5;
        AddVal(ref x);           //x需要被初始化
        Console.WriteLine (x);   //显示 “15”

    }
    static void AddVal(ref Int32 v) 
    {
        v+=10;                 //可以直接初始化v 
    }
}
```

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

 

通过上面可以看到，两个关键字的功能基本是一样的，不同的是编译器会根据他们选择不同的机制来确保我们代码的正确运行。CLR还 允许我们使用ref和out来重载方法：

```
static void add(Point p){.....}

static void add(out Point p){.....}
static void add(ref Point p){.....} //但这2个关键的不能同时存在
```

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

在值类型上使用这2个参数可能没太大区别，所以只有在引用类型上使用，才会比较有意义。可以通过ref参数来要求参数在使用前必须 被初始化或进行一些必要的操作；而使用out可以让参数在方法退出前被赋值，这样可以用在参数被返回后使用时是正常的。有一点要注意 的时，在使用关键字的时候，实参数类型必须和形参类型完全一样，而不仅仅是兼容类型。这也是为了保证类型的安全。

```
class cc
{
    public Int32 val;
}

class ex
{
    static void Main()
    {
        cc c ;
        change(out c);        //error CS1503: 参数“1” : 无法从“out cc”转换为“out object”
        Console.WriteLine(c.val);
        
    } 

    static void change(out Object o)
    {
        o = new String('x',100);
    }
}
```



如上，否则可以通过把cc类型的c传给change（out Object o）方法，而在方法类 o = new string('x',100)这个时候o的类型就被改变 了。所以通过完全匹配类型避免的这个安全漏洞。他会产生一个编译时错误。

最后要说到的就是引用类型使用ref参数。一般情况引用类型，参数传递时传递的是引用，形参和实参是指向同一个对象的。但是有个特殊情况，如果我在定义的方法中，给形参分配了新的内存空间，那么就无法返回这个新的对象了。看下面的例子：

```
class PassingRefByVal 
{
    static void Change(int[] pArray)
    {
        pArray[0] = 888;  // This change affects the original element.
        pArray = new int[5] {-3, -1, -2, -3, -4};   // This change is local.
        System.Console.WriteLine("Inside the method, the first element is: {0}", pArray[0]);
    }

    static void Main() 
    {
        int[] arr = {1, 4, 5};
        System.Console.WriteLine("Inside Main, before calling the method, the first element is: {0}", arr [0]);

        Change(arr);
        System.Console.WriteLine("Inside Main, after calling the method, the first element is: {0}", arr [0]);
    }
}
```

输出：

```
Inside Main, before calling the method, the first element is: 1
Inside the method, the first element is: -3
Inside Main, after calling the method, the first element is: 888
```

在上个示例中，数组 `arr` 为引用类型，在未使用 ref 参数的情况下传递给方法。在此情况下，将向方法传递指向 `arr` 的引用的一个副本。输出显示方法有可能更改数组元素的内容，在这种情况下，从 `1` 改为 `888`。但是，在 `Change` 方法内使用 [new](http://msdn2.microsoft.com/zh-cn/library/51y09td4(VS.80).aspx) 运算符来分配新的内存部分，将使变量 `pArray` 引用新的数组。因此，这之后的任何更改都不会影响原始数组 `arr`（它是在 `Main` 内创建的）。实际上，本示例中创建了两个数组，一个在 `Main` 内，一个在 `Change` 方法内。

在看看使用ref参数来返回这个新的数组

```
class PassingRefByRef 
{
    static void Change(ref int[] pArray)
    {
        // Both of the following changes will affect the original variables:
        pArray[0] = 888;
        pArray = new int[5] {-3, -1, -2, -3, -4};
        System.Console.WriteLine("Inside the method, the first element is: {0}", pArray[0]);
    }
        
    static void Main() 
    {
        int[] arr = {1, 4, 5};
        System.Console.WriteLine("Inside Main, before calling the method, the first element is: {0}", arr[0]);

        Change(ref arr);
        System.Console.WriteLine("Inside Main, after calling the method, the first element is: {0}", arr[0]);
    }
}
```

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

 输出

```
Inside Main, before calling the method, the first element is: 1
Inside the method, the first element is: -3
Inside Main, after calling the method, the first element is: -3
```

方法内发生的所有更改都影响 `Main` 中的原始数组。实际上，使用 **new** 运算符对原始数组进行了重新分配，这个时候形参和实参还是指向同一个对象，而不是想第一个例子那样，2个数组相互不影响了。因此，调用 `Change` 方法后，对 `arr`的任何引用都将指向 `Change` 方法中创建的五个元素的数组。

对于String类型使用ref参数是有用的，因为String是不可变的，修改一个String对象后，都会重新分配一个新的内存空间。所以要实现交换2个字符串，就必须使用ref。具体例子会在后面的String章节中介绍。

2：可变数目参数

有时候我门并不能确定有多少个参数，这个时候就可以使用可变参数：

```
static Int32 Add(pararms Int32[] values) 
{
  Int32 sum = 0;
  for (Int32 x = 0; x<values.Length;x++)
  {
     sum += values[x];
  } 
  return sum;
}
```

这个里方法参数中有一个params关键字。这个关键字告诉编译器在指定参数上使用System.ParamsArrayAttribute定制特性。在编译时 如果有这个特性，就会构造一个数组和用指定的元素填充数组，然后调用这个方法。

需要注意的是，参数中只有最后一个参数才能使用这个特性。而且这个参数必须为一个一维数组，类型任意。可以传递null或者0长数 组给此参数。当要编写接受多个任意类型参数的方法时，只需要把参数类型改为Object就可以了。

3：虚方法

在面向对象语言中，最重要的就是虚方法。正式因为有了他，才实现了程序的多态。通过对基类中虚方法的重写或隐藏，可以实现自己类型的方法。具体这个以后在多态中在慢慢研究，这里就不废话了。

下面是一张类型的内存分配模型：

![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/fig09.gif)

 

## 六 ：属性

当在类型中把一个字段定义为私有类型时，他无法在其他类型中直接访问，这就确保了数据封装的安全性。而有时我们还希望在其他类型中访问和修改这些字段。但因为是私有的，我们无法实现；如果定义为公有，又破坏了封装的安全性；为了解决在其他类型中访问类型的私有字段，我么可以使用访问器方法来实现。

### 1：无参属性

反问器方法是类型中的一个方法，他是公有的，可以在其他类型中被访问。所以我们可以把对私有字段的操作放在这个方法中，其他类型通过调用这个方法来访问私有字段。在方法器中我们还可以进行一些检查和额外的操作

```
class ex
{   
    int n;
    public ex()
    {
        n = 1;
    }
    
    public void SetVal (int n)
    {
        if(n<0)
            Console.WriteLine("n要大于等于0");
        else
            this.n = n;
    }
    
    public int GetVal()
    {
        return this.n;
    }

}

class examlpe
{
    static void Main()
    {
        ex o = new ex();
        Console.WriteLine(o.GetVal());
        o.SetVal(10);
        Console.WriteLine(o.GetVal());
        o.SetVal(-10);
        Console.WriteLine(o.GetVal());

    }
}
```

从上面的程序可以看出来，通过GetVal和SetVal两个访问器方法实现了对私有字段的访问和修改，也保证了数据的安全，而且还在方法中增加了额外的操作和检查。

但这种方法存在的问题是：
1：要写一些额外的代码来访问。
2：只能通过方法名来访问字段，没有直接访问直观。

CLR提供的属性，来缓解了第一个问题和完全解决了第二个问题。修改后的代码如下。

```
class ex
{   
    int n;             //定义的私有字段用''避免命名冲突
    public ex()
    {
        _n = 1;
    }
    
    public int n  
    {
        get{ return _n; }  
        set
        {
            if(value<0)
                Console.WriteLine("n需要大于等于0");
            else
                _n = value;
        }
    }
} 

class examlpe
{
    static void Main()
    {
        ex o = new ex();
        Console.WriteLine(o.n);
        o.n = 10;
        Console.WriteLine(o.n);
        o.n =- 10;
        Console.WriteLine(o.n);

    }
}
```

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

每个属性都要有一个类型（不能为void）和一个名称,属性不能被重载。我们定义属性时为他指定get和set两个方法，缺少get则为只写，缺少set为只读。当这段代码被编译时，系统会自动产生2个方法，自动在属性前加get_和set_,这个和之前的操作符和转换符的重载比较类似，也有一个specialname 标记。除此之外系统还会在元数据内产生一些定义。

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif).method public hidebysig specialname instance int32  get_n() cil managed
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif).method public hidebysig specialname instance void set_n(int32 'value') cil managed

在使用属性时，系统实际上会调用这2个方法，如果使用语言不支持属性，我们可以通过前面访问器的方法来访问，只是没有使用属性看起来方便。对于简单的get和set访问器方法，系统会使用内联的方式处理，所以不会有性能损失。有些比较复杂的计算或操作，我们不应该放在属性的访问器方法中，而应该用方法来完成。

2：含参属性

前面看到的属性get访问器方法不接受任何参数。如果接受一个或多个参数，在C#中我们把他称为索引器，也就是含参数属性。C#中索引器是类似数组的语法来访问。我们也可以将索引器看做是重载[ ] 操作符的一种方式。

```
class BitArray
{   
    //保存位的私有字节数组
    private Byte[] byteArray;
    private Int32 numBits;

    //构造函数分配字节数组，所有位置0
    piblic BitArray(Int32 numBits)
    {
        if (numBits<=0)
            throw new ArgumentOutRangeRxception("numBits must be>0");
        this.numBits = numBits;
        byteArray = new Byte[(numBits + 7)/8];
    }

    //索引器
    public Boolean this[Int32 bitPos]
    {
        get
        {
            //检查访问数组位数的有效性
            if((bitPos < 0) || (bitPos >= numBits))
                throw new IndexOutOfRangeException();
            //返回指定索引上的位状态
            return (byteArray[bitPos / 8] & (1 << (bitPos % 8))) != 0;
        }
        set
        {
            if ((bitPos < 0) || (bitPos >= numBits))
                throw new IndexOutOfRangeException();

            if(value)
                byteArray[bitPos / 8] = (Byte)(byteArray[bitPos / 8] | (1 << (bitPos% 8)));
            else
                byteArray[bitPos / 8] = (Byte)(byteArray[bitPos / 8] & ~(1 << (bitPos% 8)));

        }
    }
    
} 

class examlpe
{
    static void Main()
    {
        //分配一个含14个位的BitArray
        BitArray ba = new BitArray(14);

        //调用set访问器
        for(Int32 x = 0;x < 14;x++)
            ba[x] = (x % 2 ==0);
        

        //调用get访问器
        for(Int32 x = 0;x < 14;x++)
            Console.WriteLine("Bit" + x + "is " + (ba[x] ? "On" : "Off"));

    }
}
```

 

上面的例子可以看出，索引器接受一个或多个参数，这些参数和返回值可以为任何类型。创建一个Object类型的索引器是十分常见的。对于CLR来说，有参和无参属性对它来说没有什么区别，因为他们都只是一个方法。C#中要求我们使用 this[.....]的格式来创建有参属性，所以我们只能在对象实例上创建索引器，而不能创建静态的索引器，索然CLR支持。

同无参属性一样，有参属性在编译时也回产生两个get_和set_的方法和相关的元数据，但可以注意到，有参属性是不允许我们给索引器命名的，生成方法时系统会自动生成get_Item ,set_Item这样两个方法。SortedList类型就提供了一个名为Item的公有属性，属性就是SortedList的一个索引器。当然我们可以通过在索引器上应用如下特性来实现改变方法的名字：

```
Public class BitArray
{
   [System.Runtime.ComplierServices.IndexerName("Bit")]
   Public Boolen this[Int32 bitPos]
}

Console.WriteLine(ba[2]);
Console.WriteLine(ba.Bit[2]);
```

这样我们就可以使用2种方法来访问索引器了。String类型就是一个改变了索引器名字的例子。String把索引器命名为Chars而非Item,该属性允许我们得到一个字符串中的单个字符。对于不使用[ ]操作符号语法来访问含参属性的编程语言来讲，Chars是一个有意义的名称。当编译器遇到读取或设置索引器的代码时，会自动访问get或set方法。但要注意的是在C#，不允许使用索引器的名字来访问，只能通过[ ]来访问。我们还可以给属性的get和set方法设置访问属性,比如private。









# （六） ------事件

2008年01月10日 13:46:00 [cc_net](https://me.csdn.net/cc_net) 阅读数：1928



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/cc_net/article/details/2033596

事件其实也是属于类型设计里的内容，不过因为他比较重要，也比较难理解，我就把他单独拿出来学习。

## 1：下面是一个我们在程序中最常见的代码：

```csharp
private void button1_Click(object sender, System.EventArgs e)
{
          //按妞被点击后执行的操作；    
}
```

他的意思是当你点击一个按钮时，他就会执行这个方法里的代码，产生相应的操作。这个时候我们就可以说‘单击’button1这个事件发生的时候，他会自动去执行这个方法。这看起来很简单。点一个按钮，然后执行代码。这在这种基于驱动的程序中很常见。但有个问题不知道大家有没有想过。

这段代码写在那？一般是写在一个from类里，我点这个按钮属于button类。我实际实在button对象里调用了form对象里的方法。那为什么在点按钮的时候他会执行这个方法呢？

说简单点，就是其实就是在一个对象在执行一个方法时调用其他对象的方法。按事件来说就是一个对象的某个事件被触发时，通知其他对象，其他对象执行相应的一些操作。这里就是button的click事件触发后，通知form对象，执行button1_click（）方法。那么当事件触发后是如何通知其他对象的呢？

```csharp
// button1
// 登记方法到button对象
this.button1.Click += new System.EventHandler(this.button1_Click);
```

在程序中可以看到button1在设置时有这样一句话，他的基本意思就是把button1_click（）方法登记到button1.Click对事件象上，这样当触发click事件时就会调用button1_click（）方法。也就达到了通知的目的。其中button1.Click又是一个EventHandler类型，这是一个委托类型！什么是委托类型？

 

## 2：MSDN中对事件和委托的定义

关于委托可以看另外一篇介绍委托的 [ .NET学习笔记（八） ------委托（上）](http://blog.csdn.net/cc_net/archive/2008/05/21/2467717.aspx)

事件是对象发送的消息，以发信号通知操作的发生。操作可能是由用户交互（例如鼠标单击）引起的，也可能是由某些其他的程序逻辑触发的。引发（触发）事件的对象叫做事件发送方。捕获事件并对其作出响应的对象叫做事件接收方。

在事件通信中，事件发送方类不知道哪个对象或方法将接收到（处理）它引发的事件。所需要的是在源和接收方之间存在一个媒介（或类似指针的机制）。.NET Framework 定义了一个特殊的类型（Delegate），该类型提供函数指针的功能。

委托是一个可以对方法进行引用的类。与其他的类不同，委托类具有一个签名，并且它只能对与其签名匹配的方法进行引用。这样，委托就等效于一个类型安全函数指针或一个回调。虽然委托具有许多其他的用途，但这里只讨论委托的事件处理功能。

 

## 3：事件完整的例子

下面是一个完整的例子，一个邮件接受类MailManager，当接受到邮件的时候，就会通知Fax类，Fax就回进行一些操作。2个类型在后面简单的称为A，B。

```
   //A：

class MailManager
{
    //定义MailMsgEventArgs类型
    public class MailMsgEventArgs : EventArgs
    {
        //1.传递给事件接受者类型定义信息
        public readonly String from,to,subject,body;
        
        public MailMsgEventArgs(String from,String to,String subject,String body)
        {
            this.from = from;
            this.to = to;
            this.subject = subject;
            this.body = body;
        }
    }

    //2.下面委托类型定义了接受者必须实现的回调方法原型
    public delegate void MailMsgEventHandler(Object sender,MailMsgEventArgs args);

    //3.事件成员
    public event MailMsgEventHandler MailMsg;

    //4.下面的受保护虚方法负责通知事件的登记对象
    protected virtual void OnMailMsg(MailMsgEventArgs e)
    {
        //有对象登记事件？
        if(MailMsg != null)
        {
            //如果有，则通知委托链表上的所有对象
            MailMsg(this,e);
        }

    }

    //5.下面的方法将输入转化为期望的事件，该方法在新的电子邮件信息到达时被调用
    public void SimulaterArrivingMsg(String from,String to,String subject,String body)
    {
        //构造一个对象保存希望传递给同志接受者的信息
        MailMsgEventArgs e = new MailMsgEventArgs(from,to,subject,body);
        //调用虚方法同志对象事件已发生
        //如果派生类型没有重写该虚方法，对象将通知所有登记事件监听者
        OnMailMsg(e);
    }

}

```

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

A这个类型是一个事件的发送方，其中定义了事件成员和其他一些方法。他就相当于前面说到的button类，当这个类型中的事件被触发是，他就通过委托来通知登记的对象。

```
class Fax
{
    //1：将MailManager对象传递给构造器
    public Fax(MailManager mm)
    {
        //构造一个指向FaxMsg回调方法的MailMsgEventHandler
        //委托实例，然后登记MailManager的MailMsg事件
        mm.MailMsg +=new MailManager.MailMsgEventHandler(FaxMsg);

    }

    //2：MailManager将调用该方法来通知Fax对象收到一个新的电子邮件消息
    private void FaxMsg(Object sender,MailManager.MailMsgEventArgs e)
    {
        //参数sender表示MailManager对象，如果期望和事件的触发者通信，将会用到该参数
        //参数e表示MailManager对象希望提供的一些附加事件信息
        Console.WriteLine("Faxing mail message:");
        Console.WriteLine("From:{0}  To:{1}  Subject:{2}  Body:{3} ",e.from,e.to,e.subject,e.body);
    }
 

       //3：取消登记
    public void Unregister(MailManager mm)
    {
        MailManager.MailMsgEventHandler callback = new MailManager.MailMsgEventHandler(FaxMsg);
        mm.MailMsg -=callback;
    }
}

```

B这个类型是一个事件接受者，我们看到在内部构造的时候，他已经把自己的方法登记到前面的事件发送者上了。这样在当A中的事件被触发时，B就会接到通知，因为B把自己的方法委托给了A，所以A还会调用B中的方法。

 

## 4：下面是具体对事件模型的讨论

事件发送者：
1：在类型中定义一个EventArgs类型：用来保存要发送给事件接受者的附加信息。如果没有要传递的信息，可以直接使用系统的EventArgs.Emepty而不用在构造新的EventArgs对象。

2：定义一个委托类型：指定事件触发时接受者中被调用的方法的原形。如果我们定义的事件没有需要传递给事件接收者的附加信息，我们便不必定义新的委托类型。直接使用FCL中的System.EventHandler,并将EventArgs.Emepty传递给第2个参数。

3：定义一个事件成员：他是EventHandler委托类型。定义他意味着，事件的接受者想要处理这个事件，他们对应必须具有和委托类型相同签名的回调方法。

4：定义一个虚方法：当事件发生时调用这个方法，这个方法负责通知登记的接收者对象。该方法接收一个EventArgs参数。这个方法先检查是否有对象登了事件，也就是委托类型的事件成员是不是为空。如果不为空，就触发事件，然后登记的接收者对象的一个方法会被调用。这个方法必须和第2步中定义的委托类型的方法有完全匹配的签名。
这个方法是一个虚方法，所以他的继承类可以根据需要重写此方法，使得派生类可以控制事件的触发。通常派生类应该先调用基类的此方法，这样可以使所有已经登记事件的对象都能接受到通知。但是派生类也可以决定不让事件继续传递下去。

5：最后还需要定义一个方法，把外部的输入转换为触发事件的动作。当程序调用这个方法时，这个方法内部会构造EventArgs来产生要触发事件时要传递的信息，并调用上面的虚方法来触发事件。从而通知登记的对象，执行接受者中的回调方法。此方法也可以被派生类重写。


事件接受者：
1：事件接受者在事件发生时能接受到通知，然后执行自己的一个方法。这个方法必须和事件发送者中委托类型的签名完全匹配，才能在接受到通知时被调用。

2：事件接受者想要能接受事件发送者发送来的通知，并在通知后被调用，需要定义一个指向回调方法的委托对象，然后把此对象登记事件发送者中的事件上。

3：如果在事件发生时就不想被通知和调用回掉方法，同样也需要定义一个指向回调方法的委托对象，然后把此对象从事件发送者中的事件上注销。

从上面可以看出，事件中最关键的就是发送者和接受者之间的沟通问题。前面说过了，在事件通信中，事件发送方类不知道哪个对象或方法将接收到（处理）它引发的事件。所需要的是在源和接收方之间存在一个媒介（或类似指针的机制）。.NET Framework 定义了一个特殊的类型（Delegate），该类型提供函数指针的功能。通过委托进行登记和注销，从而把发送和接受双方联结起来。

 

## 5：下面看看是怎么通过委托把两者联系起来的

当编译上面这句时间成员定义时，会产生3个构造方法

```
//1:构造一个委托字段
private MailMsgEventHandler MailMsg = null;

//2:向这个委托链表上添加委托对象
private add_MailMsg(MailMsgEventHandler handler)
{
   MailMsg = (MailMsgEventHandler)Delegate.Combine(MailMsg,handler)
}

//3:从这个委托链表上移除委托对象
private remove_MailMsg(MailMsgEventHandler handler)
{
  MailMsg = (MailMsgEventHandler)Delegate.Remove(MailMsg,handler)
}
```

第一个构造是一个委托类型的字段。这个字段引用的是一个委托链表的首部，链表中包含了那些希望在事件发生时被通知的委托对象。也就是在事件接受者中定义的指向回掉方法的委托对象。一开始这个链表是空的。而后两个构造方法是允许委托对象登记和注销事件的方法。他们通过add_,remove_前缀来命名此方法。调用这些方法时会向链表上添加或移除委托对象。在他们的内部也都是调用System.Delegate的Combine和Remove方法。

在程序中我们见到的却是：

```
mm.MailMsg +=new MailManager.MailMsgEventHandler(FaxMsg);
mm.MailMsg -=new MailManager.MailMsgEventHandler(FaxMsg);
```

因为C#编译器对事件提供了内置支持，所以会自动把他转换为上面的add和remove方法。对于那些不支持+=,-=的语言，可以直接调用上面的构造方法。而C#中则要求我们使用这2个操作符，而不允许直接调用方法。

最后要注意的是，只要一个对象仍然登记有另一个对象的事件，该对象就不可能执行垃圾收集。如果我们的类型实现了IDisposable接口的Dispose方法，，我们应该在内部注销其登记的所有事件。

 

## 6：显式的构造委托成员

以上演示的是在自定义的类中来定义和使用事件。可以发现，其中最重要的就是通过定义一个委托类型的事件成员把发送和接受者连接起来。上面也谈到了事件成员的3个构造方法。

但在.NET的设计中，控件的事件非常多，如果每个时间都自动生成这3个构造函数，那么会是非常的庞大的代码。这里简单介绍下显式的构造委托成员。





# （七） ------字符串，枚举类型

2008年01月31日 20:43:00 [cc_net](https://me.csdn.net/cc_net) 阅读数：1129



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/cc_net/article/details/2075896

​         和往常一样，开篇还是介绍下最近情况了。今天是农历12月24，我们那边今天是过小年。总算是进了年关了，也给大家摆个早年了。不过最近火车成了大问题，能不能顺利回家还是个问题。自己也没啥心情看书了。哎~~还是不能堕落了。而且住的附近无线网用不了了，很久没有写了。书到是看到最后几章了。不过越到后面就越觉得有些复杂了。特别是有些地方以前很少接触用的也少，比如特性 ，枚举，字符的本地化这些。而且目前有好多笔记没写。所以决定用的不多的和很复杂的地方暂时就不去深研了。毕竟是第一轮学习。所以就把实际用的比较多的拿出来写下。其他那些就大概的照本宣科好了。

一 字符











# （八） ------委托（上）



过年后就一直没写学习笔记了，书第一边已经看完了，后面从事件开始的章节有些复杂，牵扯的知识也很 多。而且最近工作也很忙，所以没有时间来写。这段时间感觉自己对委托又有了一定的认识，所以打算来聊聊 .net里的委托。

 

## 一：什么是委托

MSDN中给出的定义：

委托是用来处理其他语言（如 C++、Pascal 和 Modula）需用函数指针来处理的情况的。不过与 C++ 函数 指针不同，委托是完全面对对象的；另外，C++ 指针仅指向成员函数，而委托同时封装了对象实例和方法。

简单的说委托就是对函数指针的一个封装。所谓的函数指针，了解C++的人都应该知道，函数指针中存放的 是函数的地址，当调用是，使用这个指针就可以调用到函数。而委托只不过是给这个指针穿的件漂亮的衣服， 使他变的更安全更强大。

**delegate** 声明定义一种引用类型，该类型可用于将方法用特定的签名封装。委托实例 封装静态方法或实例方法。委托大致类似于 C++ 中的函数指针；但是，委托是类型安全和可靠的。

```
delegate  void  DeletgateEx (int x);  //声名一个委托
DeletgateEx dx =new dx(print);   //实例化一个委托并绑定一个方法  

void print (int x)
{
  Console.WriteLine(x.ToString());
}
```

**delegate** 是声明委托的关键字

1：定义一个委托类型，这看起来有点奇怪，和一般的定义不一样？好象是在声明一个方法。要注意，委托 是对函数指针的封装，所以你就把委托看成一个存放函数的对象。但是委托不是任何函数都可以存放，他只能 存放和他声明的函数拥有相同签名的函数。从例子可以看出，这个委托只能存放一个没有返回值，有一个int 形参数的方法。

2：实例化这个委托，和其他引用对象一样，使用new创建。它需要一个参数，那就是要与此委托绑定的方法。

3：任何与委托的签名相同的方法，都可以绑定到他上面。（具 体怎么绑定后面介绍）

 

## 二：委托的作用

了解了什么是委托，以及如何建立一个委托我们已经知道了。但是委托有什么用呢？为什么要用委托呢？ 前面提到了，委托是用来用来处理其他语言需用函数指针来处理的情况的。这是他作用之一，总的说来在.NET 平台上委托有三大作用：实现委托功能，实现回调函数功能，实现事件功能。

### 1：委托功能

委托不仅仅是一个对象，你还应该把他看做一种功能，刚开始的时候我也困惑于这种区别。把他当一个对 象来看时，他是用于封装函数指针。然后提供函数指针可以提供的各种功能（当然也包括委托的功能）。而把 他当作一种功能时就更好理解了。比如日常生活做，你有些事情不会做，或不想做，你可以委托其他人帮你做 。这就是委托的功能。而在我们程序里表现的就是，比如一个下载程序，他有UI类和download两个类。在 download类中我想边下载边显示进度，但为了减少耦合，我不希望在download类中实现显示，那么我可以通过 委托这个对象，使用委托功能，来委托UI类做显示的工作。有点扰，但我想我应该还是说明白了委托对象和委 托功能的关系。（刚开始接触委托，我就是无法弄清这2个关系，所以觉得很乱）

```
class download
{
   public void down( );  //下载的功能
   
  //我的 download想用绘画界面的功能
  //但我不自 己实现，而是委托UI类来实现
  //而我通过 委托去调用这个方法

 delegate void DelegatePrint( );  //定义一个和要调用方法相同签名的委托


 public static void Main()
 { 
    down(); 
    DelegatePrint dp = new DelegatePrint( new UI ().printUI);    //绑定方法
    dp ();      //调用委托，实际是调用了和委托绑定的方法（通过委托对象dp，实现了委托的功能 ）
 }
 
} 


class UI
{
   public void printUI( );  //绘画界面的功能
}
```







上面就是一个委托功能的实现，当然你也可以在download中建一个UI对象，然后直接调用他的printUI的方 法。所以这个用法并不常见。但是可以通过使用委托，进行异步的处理，也就是让下载和绘画异步进行，这样 就不会出现下载过程中界面失去响应的问题，只有在这个时候，在一个函数中用委托的方式使用另一个函数才 有意义。关于多线程和异步调用，在后面专门文件介绍。

而通常情况，委托功能是提供给某个类，但他无法实现某个功能的时候，通过委托去调用其他的类的功能 。比如有一个容器类，他负责存放2个对象，并比较这两个对象。当我们设计这样一个类的时候，我门无法知 道它以后将要存储什么类型的数据，我们也没有必要知道。那么我们如何实现未知类型对象的比较呢？

用重载？用泛行？是个办法，但是如果存放的是用户定义的类型呢？难道我们能预测用户的类型？是的， 看起来好象没有办法做下去了。但你是否想到了委托？让别人帮我进行比较。让谁呢？当然是要存放到容器的 对象自己。我们只需在容器类中提供一个比较函数的委托，我们不需要知道，也无法知道运行时的数据类型。 但我们可以把比较的方法委托给数据类型对象自己去做。也就是调用数据对象中的比较的函数。

```
class Container
{
    private Object ob1,ob2;  //存放2个对象

    public void Container(Object o1,object o2 )
   {
       ob1 = o1;
       ob2 = o2;
   }

 //  public int compare(Object o1,object o2 )
 //   {
 / /    //不知道如何比较这2个对象
 //  } 

  //通过委托 ,那么存放在容器中的对象必须提供一个与此想匹配的方法供调用
   delegate int compareDelegate (Object o1,object o2 )
  
 //容器类的比较方法。参数是一个委托，他指向了容器中存放的对象自己实现的比较方法
 //而容器类并不需要知道比较对象的类型，如果你愿意，你甚至可以让这个比较方法执行其他操作

    //这完全由你决定，因为可以绑定任意匹配的方法 
  public void comapreObject(compareDelegate cm)
 {
    if(cm != null)    
          cm(ob1,ob2);    
 }
} 


class UserType( )
{
   private int x ,y;
   public void UserType( int x,int y)
  {
     this.x =x;
     this y =y;
  }

 public static int compare(UserType a,UserType b )
 { 
    //比较大小
 }

} 


class Test
{
   statoc void Main()
   {
      //创建2个用户对象
       UserType  u1 = new UserType(1,2);
       UserType  u2 = new UserType(1,3);
      //把2个对象存放到容器
       Container cn = new Container(u1,u2);

        //下面要执行容器对象的比较，没有调用public int compare() 这个方法，因为这个方法无法知道如何比较
      

         //我们来看看使用委托实现运行时指定方法：

         //实例化一个委托，绑定要比较的对象所提供的比较方法（在这里确定这个委托指向那么个方法）
      Container.comapreObject  cd &n bsp;=  new Container.comapreObject(UserType.compare); 
      //调用容器类的比较方法，传递一个委托参数（运行是执行委托的方法->UserType.compare)
       cn.comapreObject(cd);
   }
}

```

看到上面的程序，容器类的比较方法，并没有自己去实现比较，内部而是通过委托去调用了其他的方法。也就是说他在编译时并不知道自己要干什么，而是在运行时，根据委托绑定的方法和对象，自己去找实现比较的方法。这就实现了运行时去确定一个方法。这里意思就是，定义一个委托，告诉容器类，通过我去找方法。而到底找什么方法，那就是我们自己来定了。

注意：上面的容器类应该用范性来定义，否则委托的参数类型是不匹配的。

看了委托对象的委托功能，应该了解在什么时候使用委托功能了，主要是在异步调用和编译时无法确定要运行的方法时使用。下面看一下委托另外两种最重要的功能

 

### 2：通过委托实现事件功能

关于事件的介绍请看  [.NET学习笔记（六） ------事件 ](http://blog.csdn.net/cc_net/archive/2008/01/10/2033596.aspx) 

关于事件我这里在进行一下总结，事件其实是对WINDOWS消息机制的一种封装。我们都知道WINDOWS系统是 基于驱动的。你点一个窗体，系统就向这个窗体发送一个消息。而在.NET中则被封装为了一个click事件。他 的含义就是我在点窗体的时候，通知窗体，然后窗体进行响应。

用个例子来说就是：电灯和开关的关系。我的电灯只负责发光，而开关只负责开关电源。为什么开了开关 电灯就会亮？因为开关向电灯发送了一个消息，告诉电灯，我把电源打开了。他只负责通知，而并不需要知道 他开了电源，电灯会干什么，当然他也不可能知道。而电灯作为一个接受者，他只接受到消息后就来做自己想 做的时，是选择亮，不亮，还是烧毁自己呢？

那说到这里和委托有什么关系？委托就是实现两者之间联系的纽带，就好比电线。没有电线，电流不知道 往那跑。而且一个开关可以控制多个电灯。这也就是常说的多播委托。就好比程序中你点一个安妞，可以触发 多个事件。

下面还是举个例子

```
//定义一个开关类
class Switch 
...{
   

    //委托类型定义了接受者必须实现的回调方法原型
    public delegate void SwitchEventHandler (Object sender, EventArgs args);

    //定义一个委托对象
    public  SwitchEventHandler  SwitchOn;


    //定义一个方法通知电灯
    protected virtual void OnSwitchOn(EventArgs e)
    ...{
        //有对象登记事件？
        if(SwitchOn!= null)
        ...{
            //如果有，则通知委托链表上的所有对象 （电线上的所有电灯）
            SwitchOn (this,e);
        }

    }

    //调用此方法，触发事件
    public void pushSwitch( )
    {
   
        OnSwitchOn(e);
    }

}

//定义电灯类
class Light
...{
    public Light(Switch s)
   {

        s.SwitchOn += new SwitchEventHandler(TurnOn)  //绑定方法 

    }


  private void TurnOn(Object sender,EventArgs e)
  {
        Console.WriteLine("turn on");
    }
 

} 

class Test()
{
   public static voId Main()
   {
       Switch sw = new Switch();
       Light  L1 = new Light(sw);
       Light  L2 = new Light(sw);
       sw.pushSwitch();
        
   }
}
```

 



上面定义了一个开关类，里面有一个委托，他和电灯类的方法是相同的签名。所以可以用来装电灯的方法 。在Light类的构造函数中，把委托和电灯的turnOn方法绑定起来，相当于把电灯和开关用电线（委托）连接 起来了。当我执行了SWitch的pushSwitch操作，就触发了SwitchOn这个事件。于是开关通过委托告诉电灯，我 被按了,而且是通知所有在委托上的电灯，这里是2个。

而这个时候电灯收到消息，就会开始执行相应的方法。这个方法是在Light构造函数中通过和委托绑定方法 指定的。你也可以把他和其他的方法绑定。这些都是自己Light决定的。而开关只负责告诉你，我被按了，具 体你执行什么操作，就在这里设置。

程序有问题？那里？

为什么Light中绑定方法使用的是‘+=’而不是‘=’？？？恩，这是个问题，但不 是这里的问题。前面说了委托可以装函数，而且可以装多个函数。他就好象链子一样，把自己绑定的方法连起 来。就好象电线把电灯都连起来一样。关于委托链我们后面介绍。这里我们要注意的是下面的问题

仔细的朋友一定看到了：

```
//定义一个委托对象
public  SwitchEventHandler  SwitchOn;
```

是啊，我这里定义的是一个委托而不是一个事件。那程序为什么也正常的。既然正常，我们为什么还要事 件呢？事件和委托到底是什么关系呢？一些列的问题又来了，下面就一一回答。

先给出事件中正确的定义方法:

```
    //定义一个委托对象
    public  event SwitchEventHandler  SwitchOn;
```

两者的差别就在于这一个event关键字，加了一个event他到底做了什么，大家可以看前面事件那片文章， 里面有详细的介绍。用event他产生了3个构造方法，其中SwitchOn成了私有类型，而通过add和remove方法对 他进行操作，这就意味着，我们不能对SwitchOn直接进行操作了。那和上面没event的定义有什么区别呢？

如果没有event，就不回产生3个构造方法，SwitchOn就是公有的，那会出现什么情况？我可以直接对 SwitchOn进行设置。如果我不小心把上面Light的构造函数写成了

```
public Light(Switch s)
   ...{
         s.SwitchOn = new SwitchEventHandler(TurnOn)  //绑定方法 
    }
```

这个有错吗？没错，委托就是这么绑定方法的。但这样的结果是什么。委托上只会绑定这一个方法。我就 无法把多个方法绑定到委托上，委托上永远只有这一个方法。那么事件发生时我就无法通知所有的注册的对象 了。而用了evevt关键字，我们就只能对他进行add,remove操作了。这也符合的事件的设计初衷。

事件我们就介绍到这里，我想通过了解委托，大家应该对事件了解的更清楚了。而事件还用到了委托链。 我们在下一篇问文章中介绍。

 

### 3：回调函数功能

 什么是回调函数？网上也有各种对回调函数的定义。比较通俗的理解就是：一个函数被当作一个变量传递给一个方法，这个方法在执行是会使用到这个函数，那么这个函数就被叫做回调函数。而传递这个函数的地址是通过函数指针传递的。那么在.NET平台中，我们就是通过传递一个委托，来实现回调函数的功能。

那么回调函数到底有什么用呢？还记得上面一个比较对象的例子吗？我们本是想调用容器类的方法来比较自定义对象的大小，但容器类实际却调用了我们自定义对象的方法来进行比较。被调用者调用调用者提供的函数的过程就叫回调。

操作系统拥有更广泛意义上的回调，那就是消息机制。消息本是 Windows 的基本控制手段，乍看与函数调用无关，其实是一种变相的函数调用。发送消息的目的是通知收方运行一段预先准备好的代码，相当于调用一个函数。比如程序中使用了一个系统定时器，在程序中调用了这个定时器的方法，而当到了指定时间，定时间器会给程序发消息，要我执行我的一断代码。这就是广意上的回调。这更多的用在C/C++中，而在.NET中，这种消息机制被进行了封装，成了事件event。这里不过多的谈论回调函数，具体可以参见文章：[关于回调函数](http://akerlee.bokee.com/4343352.html) ，这里说的比较详细，但是我还是看的很晕。下面我们主要看看在.NET中如何通过委托实现回调函数。

系统中有许多API函数需要我们提供一个回调方法，下面是MSDN上的一个例子:

```csharp
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyDelegate
{
    委托实现回调函数
}
```

可以看到，使用的API函数 EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam)，他第一参数表示需要传递一个函数给他。而传递函数地址在.net中通过委托来实现，于是我们定义了一个委托，把他和要传递的函数Report绑定。然后把委托传递给API函数，这样就实现了一个回调函数的传递。

上面是在托管平台调用非托管DLL中需要回调函数方法的情况，当然还有前的事件，也需要回调功能，还有在委托功能做提到过的异步调用。

委托、接口和事件允许提供回调功能。每个类型都有自己特定的使用特性，使其更适合特定的情况。

**事件**

如果下列条件为真，请使用事件：

- 方法预先注册回调函数，一般通过单独的 **Add** 和 **Remove** 方法。
- 一般情况下，一个以上的对象需要事件通知。
- 希望最终用户能够轻松地将侦听器添加到可视化设计器的通知中。

**委托**

如果下列条件为真，请使用委托：

- 需要 C 语言样式的函数指针。
- 需要单个回调函数。
- 希望注册在调用中或构造时发生，而不是通过单独的 **Add** 方法。

**接口**

如果回调函数要求复杂的行为，请使用接口。

 

## 三 小结

通过上面的内容对委托的功能和使用应该有一定了解。简单来说委托就是一个容器，用来存放函数的地址。委托对象可以当参数传递，于是也就传递了函数。

因为WINDOWS是基于消息机制的，消息机制是一种广义的回调，所以关键的关键是回调函数。在C/C++中 函数指针来实现回调函数功能，然后应用到消息机制上去。而在.NET中，委托是对函数指针的封装，事件是对消息的封装。所以在.NET中委托的功能是实现了回调函数（说是封装回调函数更便于理解），然后又通过实现的回调函数实现了事件等操作。

说到这里已似乎成了讨论消息机制和回调函数了。而我对这也不了解，所以无法在说下去。还是回到.NET的委托上把。知道可以用委托来传递函数，实现事件模型已经异步调用就可以了。

在下一章将进一步介绍委托的内部实现，他是如何绑定一个方法，并且在运行时找到这个方法并执行的，他是如何同时绑定多个方法的，事件中如何合理的设置委托。

 





# （九） ------委托（下）

2008年05月22日 23:18:00 [cc_net](https://me.csdn.net/cc_net) 阅读数：1597



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/cc_net/article/details/2471007

这一篇主要介绍委托的一些内部结构，想要了解委托的使用和功能可以看前一篇文章：[.NET学习笔记（八） ------委托（上）](http://blog.csdn.net/cc_net/archive/2008/05/21/2467717.aspx)

 前天不小心感冒了，吃了药但还是晕晕的，还是继续写把。。。

前面介绍了委托的功能和使用方法。委托实际是对一个函数指针的封装，那么他是如何指向一个函数，有是如何把多个方法都绑定起来的呢？

## 一 委托的内部结构

下面是简单的一个定义委托方法，写起来很简单，当编译器编译这句话的时候做了什么呢？

```
//申明一个委托要绑定的方法签名
public deletgate void ExampleDeletgate(int x);
```

通过ILDASM我们可以看到，系统产生了以下代码:

```
//构造器有2个参数为了方便后面使用，我重新定义这2个变量名字
//public ExamplDelegate(object target,int methodPtr)
.method public hidebysig specialname rtspecialname  
       instance void  .ctor(object 'object', native int 'method') runtime managed
{
}

//下面两个是委托的一个异步回调用方法，我们暂时不关注他们
.method public hidebysig newslot virtual 
        instance class [mscorlib]System.IAsyncResult 
        BeginInvoke(int32 x,class [mscorlib]System.AsyncCallback callback,object 'object') runtime managed
{
}

.method public hidebysig newslot virtual 
        instance void  EndInvoke(class [mscorlib]System.IAsyncResult result) runtime managed
{
}

//这个方法和委托指定的方法原形是一样的,我同样改写下
//public void virtual Invoke(int32 x);
.method public hidebysig virtual instance void Invoke(int32 x) runtime managed
{
}
```

上面的代码有点乱，但是应该还是可以看出点头绪来。编译器在编译到这句的时候实际是产生了一个名字为ExampleDelegate的类，并且生成了4个方法。其中第一个是一个类型构造器，他需要2个参数。看到我重新命名的参数，大家应该猜测到他的作用了把。最后一个是一个Invoke方法，这个词是调用的意思。而且他的类型和申明的方法原形签名是相同的。大家也应该可以猜到。委托实际调用的是这个方法。而另外2个方法是和Invoke没什么区别，只是异步调用。后面文章会介绍异步调用的知识。

上面说了编译器的工作，下面我们具体来看下这个类，和其中的Invoke方法。ExampleDelegate成为了一个类，还记得定义委托对象是如何做的吗？

```
ExampleDelegate ed1= new ExampleDelegate(Class.method) //实例方法
ExampleDelegate ed2 = new ExampleDelegate(method) //静态方法
```

在定义一个ExampleDelegate类型的对象的时候，我们给他传递了一个方法。这里大家肯定觉得奇怪，上面的构造方法可是需要2个参数的。我们先看看这2个参数，target 是一个object对象，他表示帮定方法的对象。而method则是表示绑定的方法。编译的时候一个参数确实也通过了。这是因为编译器知道我们在构造一个委托。所以他会自动分析我们传递给他的方法的对象。

在ExampleDelegate类中有3个私有的成员变量，其中前两个提供了Target和Method 2个属性。我们可以通过这2个属性判断委托是否与一个方法绑定。

 看完了上面的3个字段应该就明白了，在定义一个委托的时候，我们的委托对象已经在内部记录了绑定的对象和要执行的方法。下面看下通过委托调用这个方法的情况

```csharp
object _target;   //指向绑定方法的对象，如果是实例方法则为调用方法的对象，若为静态方法则为空

MulticaseDelegate  _prev ;  //这也是一个委托类型，用与之向下一个委托，初始为null，在.net2.0中这个字段是object类型

int _method,     //这个字段是从元数据中获得的一个值，改值是绑定的方法在元数据中的一个标识值

ed1(x); //通过委托调用绑定的方法

//下面是ILDASM中看到的
 IL_000c:  stloc.0
 IL_000d:  ldloc.0
 IL_000e:  ldc.i4.3
 IL_000f:  callvirt   instance void delegateEx.Class1/ExampleDelegate::Invoke(int32)  //实际上是调用ExampleDelegate对象的Invoke方法
 IL_0014:  ret
```

看到编译后的代码实际是调用了内部的Inovke函数。当调用这个方法时，会用到_target和_method字段，这就让代理在指定的对象上调用期望的方法。

 

## 二 委托链

在前一片文章中，我们提到过委托链这个概念，而上面内部结构中有个_prev字段就是用来指向下一个委托对象的。这就使得委托可以指向其他对象，组成一个链表结构。

 在Delegate类中提供了几个静态方法，来往一个委托链上添家或移除委托对象。

```
public static Delegate Combine(Delegate a, Delegate b){}
public static Delegate Combine(params Delegate[] delegates){ }
public static Delegate Remove(Delegate source, Delegate value){ }
public static Delegate RemoveAll(Delegate source, Delegate value){ }
```

![img](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)

下面是操作的代码：

```csharp
ExampleDelegate ed1= new ExampleDelegate(method1) 
ExampleDelegate ed2 = new ExampleDelegate(method2) 
ExampleDelegate ed3 = new ExampleDelegate(method3) 
//把2个委托组成串
ExampleDelegate ed = (ExampleDelegate)Delegate.Combine(ed1,ed2);
ed = (ExampleDelegate)Dleegati.Combine(ed,ed3);
```

要注意这里ed相当于链表的头接点,而我们插入的时候使用的是头插发，就是插入到头节点后面，而不是链尾巴。所以上面最后的结果是ed--> ed3-->ed2-->ed1-->null.委托链执行的时候仍旧是执行各个委托对象内部记录的方法，只是按顺序执行。下面就看下更具体的Invoke方法

```
public void Invoke(int x )
{
  if (_prve != null) _prve.Invoke(x);  //如果委托后面还有其他委托对象则执行下一个对象的Invoke方法

  _target.method(x);  //如果委托后面没有其他委托对象，则通过内部的2个字段来执行期望的方法
}
```

可以看到Invoke内部是这样一个样子，所以实际上，方法是从委托链的末尾开始执行的。但要注意的是，如果委托的方法有返回值，那么只有最后一个执行的委托的值会被返回，而其他的委托的值都被丢弃。如何返回所有委托的值在后面会有介绍。

这里要注意的是当使用combine方法时，会构造一个新的委托对象，他的_target,_method字段和原委托相同，但_prve被重新设置。比如Combine(ed1,ed2);ed2会新建立一个委托对象来添加到委托链上，他的_prve指向ed1。因为委托对象一旦被构造就认为是恒定不变的，_prev =  null,不会改变。

关于委托比较：

所以2个ed2指向的不是同一个对象，通过ReferenceEquals方法可以看出来。这里简单介绍下委托对象的比较，委托MulticastDelegate类重写了Equals方法。在这个方法中，是通过比较委托对象的3个私有字段判断的，只有这3个字段全部相同时才相等。而在Delegate类中重写的Equals只比较了_target,_method2个字段。

下面看下把委托从委托链移除的例子

```csharp
ExampleDelegate ed1= new ExampleDelegate(method1) 
ExampleDelegate ed2 = new ExampleDelegate(method2) 
//把2个委托组成串
ExampleDelegate ed = (ExampleDelegate)Delegate.Combine(ed1,ed2);

//移除
//方法1：
//ExampleDelegate ed = (ExampleDelegate)Delegate.Remove(ed1,ed2);

//方法2：
ExampleDelegate ed = (ExampleDelegate)Delegate.Remove(ed1,new ExampleDelegate(method2) );
```

上面有2种方法，但第一种被注释掉了，因为他是错误的，而应该使用第2重，这看起来有些奇怪。但要记得前面说了委托的恒定性，委托一旦建立，他的_prev字段是无法修改的。所以如果你使用了第一种方法，那么ed2的_prev对象还是指向ed1的。所以，我们必须构建一个新的ed2对象，并且他的_prev =null，这样才是把ed2从委托链上移除了。

在移除过程中，Remove方法会扫描委托链，然后比较委托链上是否存在和新建的问题相同的委托。这里进行比较使用的是Delegate.Equals（）方法，因为他只比较_target和_method两个字段（框架程序设计中译者写这里不是使用Delegate.Equals，而是使用的一个内部方法比较，但确实只比较这2个字段，我用Reflector看了下，方法如下：）

```csharp
protected virtual Delegate RemoveImpl(Delegate d)
{
    if ((this.target == d.target) && this.Method.Equals(d.Method))
    {
        return null;
    }
    return this;
}
```



如果发现相同的委托，则移除这个委托，方法是修改这个委托前一个对象的_prev指针。如果找不到他也不会进行任何操作也不会返回异常。而remove每次只能移除一个委托对象。如果委托链上有2个ed2委托对象，必须使用两次remove。

注意：.net2.0中的委托链有所区别

在2.0中，委托有了一定的修改，上面介绍的是1.1的。但基本都是相同的，只是委托中的_prev字段发生了变化。在2.0中，这个字段成了Object类型，他不在是负责指向下一个委托，而是负责一个委托对象数组，通过这个数组来访问委托对象。



![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/net1.1.jpg)     ![img](https://p-blog.csdn.net/images/p_blog_csdn_net/cc_net/.net2.0.jpg)

.net1.1中的委托链 (ed2,ed3应该指向新建构造的ed2,ed3两个对象)                                                                       .net2.0中的委托链

 

 从上面的图就可以看到双方的区别的，在2.0中_prev不在直接指向下一个委托对象，而是通过控制一个数组来实现。建立了ed他指向空，当把ed1加到链上时，直接用ed-->ed1,当把ed2加到链上时，发现已经有了一个委托对象，于是建立一个新的委托对象，并初始化他的3个字段，而他的_prev字段是一个对象数组，_prev[0]指向ed1,而_prev[1]指向ed2。当把ed3加到链上时，_prev一共指向3个对象了。ed指向新的委托对象。而之前的委托对象会被垃圾回收器回收掉。

这样的改进，访问时不需要遍历整个链，只需要遍历头结点的_prev数组。为什么怎么改进，我也不太清楚。

在C#中可以直接使用 +=，-=来操作委托链。前面的示例中就可以看到了。因为C#中这2个操作符号进行了重载，可以通过ILDASM工具查看。另外在.net2.0中c#还提供了一些使用委托上的便利，比如匿名方法，这里就不在详细说明了。

 

## 三 对委托进行更多的控制

从前面可以看到，在访问委托链的时候，都是遍历链，按照顺序来访问，如果其中一个委托发生错误，就会影响后面的委托对象。而且只返回最后一个委托对象的值。

当然我们可以自己来显式的访问每个委托对象，而访问之前，我们必须获得链上所有的委托对象。因为委托的_prev是一个私有字段，所以我们想操作一个链表一样来访问每个对象，但可以使用MulticastDelegate提供的一个获得链中的所有委托

```csharp
//获得一个委托数组，每个元素是ed链中的委托
Delegate arrayofDelegate = ed.GetInvocationList();

foreach(ExampleDelegate eds in arrayofDelegate )
{
    eds(10); //显示的调用委托，可以通过控制语句进行其他控制
}
```



这样，当有委托发生异常时，可以进行处理，保证其他委托正常运行，而且可以获得每个委托返回的值（如果有返回值）

关于事件中的委托：

时间和委托是联系最紧密的，前面我们也说了，事件模型，是通过委托来实现的。消息发布者和订购者都是通过委托来联系的。但是不同的是，在定义委托对象时，加入了event 关键字。这个关键字使得委托的对象在创建被定义为了私有类型，也就是不能象这里，直接对委托对象进行操作。而只能使用+=,-=两个重载的操作符，来操作委托链。这样就避免的对委托的错误操作。其实这个也可以看做是对委托的一种控制。

 

## 四 总结

通过上面的讲解，对委托的内部结构和实现应该有了一个比较清楚的认识。委托实际是对一个函数指针的封装，通过一个链，把多个委托连接起来。就好象链表一样。委托的用途很广泛，无论是事件，回调函数还是异步编程。而且使用委托也是降低模块间耦合性的一个办法。当然还可以通过接口等其他方法来实现其中一些功能。所以理解委托是十分有必要的。自己也只是了解了一些以后，写成笔记和大家分享，有很多东西也不知道，所以无法说的太深，有任何错误和不恰当的地方也希望大家指出。

