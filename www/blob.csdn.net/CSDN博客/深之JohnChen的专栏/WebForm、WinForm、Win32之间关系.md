# WebForm、WinForm、Win32之间关系 - 深之JohnChen的专栏 - CSDN博客

2009年01月15日 11:38:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4235


**WebForm、WinForm、Win32之间关系**

WebForm是.net中用于开发web应用的一系列技术的通称。特点是使用统一的UI,就是web页面，统一的client端，浏览器。没有平台和浏览器限制（因为webform产生的页面是html3.2兼容的）。WebForm页面构架是一个可升级的公用语言运行程序模型，被用来在服务器端动态地建立Web页面。WebForm是纯文本文件，以.aspx为文件扩展名。当客户端浏览器请求一个.aspx文件时，Asp.net将解析并编译目标文件为.net构架类，接着这个类动态处理客户端的请求。相对Asp,Asp.net引入了全新的的HTML编程模式，包括全新的服务器端控件，表单的事件驱动，数据绑定等。

WebForm的出现在很大程度上为了实现web上RAD(快速应用程序开发)开发，传统的b/s程序中程序员要做界面时往往要与网页制做人员结合，现在ms给了你一个和vb或delphi一样所见即所得的web开发界面，它是运行在.net架构之上.

WinForm是.Net开发平台中对Windows Form的一种称谓。.Net 为开发WinForm的应用程序提供了丰富的Class Library（类库）。这些WinFrom 类库支持RAD(快速应用程序开发)，这些类库被封装在一个名称空间之中，这个名称空间就是System.Windows.Forms。在此名称空间中定义了许多类，在开发基于.Net的GUI应用程序的时候，就是通过继承和扩展这些类才使得我们的程序有着多样的用户界面。本文就试图通过一个最基本的类--Form.

Win32是建立 Win32 Windows 应用程序，包括主控台应用程序、可执行的 Windows 架构应用程序、动态链接库 (DLL) 以及静态链接库。

webform 和 winform的最大区别是:

WebForm应用程序部署在服务器端，用户通过浏览器与其交互；WinForm应用程序需要部署到客户端，用户通过专门为该程序打造的用户界面与其交互。

WebForm是基于Web浏览器的网络应用程序窗体,winform是基于Windows桌面应用程序窗体.

参考资料：（来源MSDN）

**Windows ****架构应用程序概观**

Microsoft Visual Studio 2005/2008 提供几种开发 Windows 架构应用程序的方式，这些程序会在使用者的本机计算机上执行。您可以使用 Windows Form 建立 Windows 架构应用程序和使用者接口 (UI)。您也可以使用 Visual Studio 或 Microsoft .NET Framework version 2.0 软件开发套件 (SDK) 建立 Windows 服务应用程序，并且可以使用 Visual Studio [项目精灵] 建立 Windows Win32 架构应用程序。

**Visual Studio Windows ****架构应用程序**

Visual Studio Windows 架构应用程序是环绕 .NET Framework 所建置，这是可以用来开发复杂应用程序的丰富类别组。您可以使用任何能够搭配 .NET Framework (Visual Basic、C#、Visual J#、C++ 和许多其它语言) 以及 .NET Framework 侦错工具的程序设计语言建立 Windows 架构应用程序。使用 .NET Framework 类别建立的 Windows 架构应用程序会提供其它好处。您可以存取操作系统服务，并享受使用者运算环境所提供的其它好处。您可以使用 ADO.NET 存取资料。如需详细信息，请参阅[用 Visual Studio 建立数据应用程序](http://msdn.microsoft.com/zh-tw/library/0wxwcakt(VS.80).aspx)。GDI+ 能够让您在窗体内执行进阶绘制。如需详细信息，请参阅[Windows Form 中的图形和绘图](http://msdn.microsoft.com/zh-tw/library/a36fascx(VS.80).aspx)。Windows 架构应用程序可以呼叫透过 XML Web Service 公开的所有方法，让您能充分运用各种不同来源和合作伙伴所提供的信息和运算资源的好处。如需详细信息，请参阅[以 Managed 程序代码建立之 XML Web Service 的程序设计简介](http://msdn.microsoft.com/zh-tw/library/yzbxwf53(VS.80).aspx)

**Windows ****架构应用程序开发的**** Visual Studio ****工具**

正如其它 .NET Framework 应用程序，您可以在文字编辑器中建立 Windows 架构应用程序、呼叫 .NET Framework 方法和类别、在命令列编译应用程序，以及散发产生的可执行程序。

或者您也可以使用 Visual Studio 2005。当您使用 Visual Studio 2005 建立 Windows 架构应用程序时，基本上所建立的应用程序与手动撰写程序代码建立的并无不同。使用 Visual Studio 2005 的优点是它提供让应用程序开发更为快速、简单及可靠的工具。下列清单显示一些更重要的 Windows 架构应用程序开发工具：
- 具有拖放控件的 Windows Form 可视化设计工具。如需详细信息，请参阅[Windows Form 设计工具](http://msdn.microsoft.com/zh-tw/library/e06hs424(VS.80).aspx)。
- 程序代码感知编辑器，其中包括陈述式完成、语法检查和其它 IntelliSense 功能。
- 整合式编译和侦错。
- 建立和管理包括本机部署、跨内部网络、或是跨因特网的应用程序档案之项目管理工具。

如果您曾经使用过 Visual Studio，这些功能看起来都很熟悉。其类似旧版 Visual Basic 和 Visual C++ 中能够使用的功能。Visual Studio 2005 会扩充这些功能以提供开发 Windows 架构应用程序的丰富工具集合。

**Windows ****架构应用程序种类**

您可以使用 Visual Studio 开发几种不同的 Windows 架构应用程序。您可以建立使用 .NET Framework 功能的 Windows Form 和 Windows 服务应用程序，也可以使用 Visual C++ 建立 Windows Win32 架构应用程序。

**Windows Form**

WebForm是.net中用于开发web应用的一系列技术的通称。特点是使用统一的UI,就是web页面，统一的client端，浏览器。没有平台和浏览器限制。

**Windows ****服务应用程序**

您可以使用 Visual Studio 2005 或 Microsoft .NET Framework version 2.0 软件开发套件 (SDK) 建立安装为服务的应用程序以建立服务。这种应用程序称为 Windows 服务。您可以使用 .NET Framework 功能建立服务、安装服务以及启动、停止和控制它们的行为。

如需详细信息，请参阅[Windows 服务应用程序](http://msdn.microsoft.com/zh-tw/library/y817hyb6(VS.80).aspx)。

**Win32 ****专案**

您可以使用 [项目精灵] 建立 Win32 项目类型，包括主控台应用程序、可执行的 Windows 架构应用程序、动态链接库 (DLL) 以及静态链接库。

如需详细信息，请参阅[建立 Win32 Windows 应用程序](http://msdn.microsoft.com/zh-tw/library/2eezscy8(VS.80).aspx)。

