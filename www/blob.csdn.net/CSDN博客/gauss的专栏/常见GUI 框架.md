# 常见GUI 框架 - gauss的专栏 - CSDN博客
2011年12月11日 10:45:38[gauss](https://me.csdn.net/mathlmx)阅读数：1718
**1：wxWidgets**
wxWidgets是一个开源的跨平台的C++构架库（framework），它可以提供 GUI（图形用户界面）和其它工具。目前的2.x版本支持所有版本的Windows、带GTK+或Motif的Unix和MacOS。一个支持OS/2的 版本正在开发中。
wxWidgets在最开始是由爱丁堡（Edinburgh）大学的人工智能应用学院开发的，主要是内部使用，而在1992年第一次公布。 2.x版本做了很大程度的改良，并且由Julian Smart, Robert Roebling, Vadim Zeitlin, Vaclav Slavik和更多其他的人所编写和维护。
wxWidgets的主体是由C++构建的，但你并不是必需通过C++才能使用wxWidgets.wxWidgets拥有许多其它语言的绑定（binding），使你在用其它语言编写程序的时候也可以使用wxWidgets.
wxwidgets是一个 c++编写的用来提供gui开发的框架。它包含一个可以支持现今几乎所有操作系统（Version 2 currently supports all desktop versions of MS Windows, Unix with GTK+, Unix with Motif, and MacOS. An OS/2 port is in progress.）的GUI库和其他一些很有用的工具，提供了类似MFC的功能。而且，特别要说一下，这个c++lib还的新版本还提供了对掌上电脑的
 支持。当然，说到这里很多人会想到java队多系统的支持，其实这是不一样的，java的跨平台是建立在“中间代码”的基础上的，就是说需要在目标平台上 安装java解释器；但是wxwidgets是c++库，经过编译后，他提供的是native级的机器码，在gui编程方面，这可是意味着很大的不同！
**2：WTL**
Windows
 模板库 (WTL) 是一个用来开发 Windows 应用程序的 C++ 的 UI 组件，它扩展了 ATL (Active Template Library) 提供了一系列的对话框、帧、GDI对象等等.
**3:DirectUI Duilib **
国内首个开源 的directui 界面库，开放，共享，惠众，共赢，遵循bsd协议，可以免费用于商业项目，目前支持Windows 32 、Window CE、Mobile等平台。
Duilib 是一款强大的界面开发工具，可以将用户界面和处理逻辑彻底分离，极大地提高用户界面的开发效率。提供所见即所得的开发工具UIDesigner。使用DirectUI后将使得我们的设计人员彻底解 放，不会受到开发的束缚，可以充分地发挥其设计能力来设计软件界面，并参与到用户界面开发过程中。 Duilib 目前支持Windows 32 、Window CE、Mobile等平台，使用C++开发，遵循BSD协议，可以免费用于商业项目。
DirectUI界面库 取名自微软的一个窗口类名“DirectUIHWND”，意为Paint on parent dc directly。 即子窗口不以窗口句柄的形式创建，只是逻辑上的窗口，绘制在父窗口之上。
DirectUI界面库使用XML来描述界面风格，界面布局，可 以很方便的构建高效，绚丽的，非常易于扩展的界面。从而很好的将界面和逻辑分离，同时易于实现各种超炫的界面效果如换色，换肤，透明等.
**4:QT**
Qt是诺基亚开发的一个跨平台的C++图形用户界面应用程序框架。它提供给应用程序开发者建立艺术级的图形用户界面所需的所用功能。Qt是完全面向对象的，很容易扩展，并且允许真正地组件编程.
- 优良的跨平台特性:
Qt支持下列操作系统: Microsoft Windows 95/98， Microsoft Windows NT， Linux， Solaris， SunOS， HP-UX， Digital UNIX (OSF/1， Tru64)， Irix， FreeBSD， BSD/OS， SCO， AIX， OS390，QNX 等等。
- 面向对象
Qt 的良好封装机制使得 Qt 的模块化程度非常高，可重用性较好，对于用户开发来说是非常 方便的。 Qt 提供了一种称为 signals/slots 的安全类型来替代 callback，这使得各个元件 之间的协同工作变得十分简单。
- 丰富的 API
Qt 包括多达 250 个以上的 C++ 类，还替供基于模板的 collections， serialization， file， I/O device， directory management， date/time 类。甚至还包括正则表达式的处理 功能。
- 支持 2D/3D 图形渲染，支持 OpenGL 
- 大量的开发文档 
- XML 支持 
**5:GTK(GIMP Toolkit)**
**GTK**(GIMP Toolkit)是一套跨多种平台的图形工具包,按LGPL许可协议发布的。虽然最初是为GIMP写的，但目前已发展为一个功能强大、设计灵活的一个通用图形库。特别是被GNOME选中使得GTK+广为流传，成为Linux下开发图形界面的应用程序的主流开发工具之一，当然GTK+并不要求必须在Linux上，事实上，目前GTK+已经有了成功的windows版本。
GTK虽然是用C语言写的，但是您可以使用你熟悉的语言了使用GTK，因为GTK+已经被绑定到几乎所有流行的语言上，如：C++, Guile, Perl, Python, TOM, Ada95, Objective C, Free Pascal, and Eiffel.
**6:kGUI**
kGUI 被设计成为一个跨平台的 GUI 框架，C++语言的。目前只能在 Windows 上使用。kGUI 还提供一些常用的功能包括：哈希表、bsps、异步HTTP下载、线程、xml读写、HTML渲染、报表生成和打印等.
**7:MFC/ATL**
windows下面GUI库，功能比较强大，但也比较繁琐.
