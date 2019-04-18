# VC笔记（二） - L_Andy的专栏 - CSDN博客

2012年11月15日 15:55:16[卡哥](https://me.csdn.net/L_Andy)阅读数：788


VC++ -----------学习笔记一  张明奇（卡哥）

1.每个MFC应用程序都包含如下6个部分

·从CWinApp派生一个新类作为该应用程序的应用类，这个类应该重载InitInstance（）函数

·将该应用程序类声明为全局变量，CWinApp的构造函数将自动调用InitInstance（）函数

·从CFrameWnd（或CMDIFrameWnd或CMINIFramdWnd）派生一个类作为屏幕上的主窗口

·创建该窗口的InitInstance（）函数

·创建该窗口的构造函数

·创建该窗口的析构函数

2.MFC程序运行和启动的过程：

·应用程序对象产生（theApp）,配置内存，设定初值

·调用AfxWinMain()函数，执行AfxWinInit()函数，后者又调用AfxInitThread（）函数，把消息队列尽量加大到96

·AfxWinMain（）函数执行InitApplication（）函数

·AfxWinMain（）函数执行InitInstance（）函数，他是CwinApp的虚函数，必须重载并修改它

·InitInstance（）函数创建了一个CMyFramWnd对象

·CFramWnd构造函数调用Create（）函数，产生主窗口，在生成窗口前，Create（）函数注册有关窗口类

·回到InitInstance（）函数中继续执行ShowWindow（）函数显示窗口

·执行UpdataWindow（）函数，发出WM_PAINT消息

·回到AfxWinMain（）函数，执行Run（）函数，进入消息循环

Windows应用程序具有相对固定的基本结构，由WinMain函数和WndProc函数构成基本框架。WinMain函数是所有Windows函数的入口，类似C语言的main函数，其功能时完成一系列的定义和初始化工作，并产生消息循环。消息循环是整个程序运行的核心。

WinMain函数主要由一下几个部分组成

·定义并注册窗口类；

·建立窗口

·产生消息循环

窗口函数WndProc定义了应用程序对收到的不同消息的相应，其中包含了应用程序对各种可能收到的消息的处理过程，是消息处理分支控制语句的集合。通常，窗口函数由一个或多个switch…case语句构成，每一个语句对应一种消息代码处理模块，当应用程序接收到一个消息时，相应的case语句就被激活并执行相应的模块。

3.应用类及其主要成员函数

每个应用程序从类CWinApp派生出自己的应用程序类，并定义一个全局对象。

CWinApp所包含的数据成员主要用于存放和控制应用程序本身的信息

·Instance（）函数

主要完成设置注册数据库，载入标准设置（最近打开文件列表等），注册文档模板等工作，其中注册文档模板过称中隐含地创建了主窗口。接着，处理命令行参数，显示窗口然后返回进入消息循环。

·Run()函数 WinMain（）函数在初始化应用程序实例后，就调用Run ()函数处理消息循环

等待消息-处理消息-空闲任务-等待消息

程序结束时调用ExitInstance（）函数使应用程序推出。

很少被重载，也可以重载提供特殊功能

·ExitInstance（）函数

用户推出应用程序的运行实例时由Run（）函数调用。

·OnIdle（）函数

在应用程序的消息队列中没有消息时候Run()函数调用的

由于只有在OnIdle返回后应用程序才能处理用户输入，因此在OnIdle中不应执行较长的任务

4.文档类和视图类 从CDocument派生 定义在MFCDoc.h，实现在MFCDoc.h

若要使用AppWinzard提供的文档类，必须执行下列操作。

·为每个文档类型从Cdocument中派生一类

·添加成员变量一存储每个文档的数据

·重写CDocumet的Serialize成员函数，Serialize用于从磁盘读取稳定那个的数据和将文档的数据写入磁盘。

·经常需要重写OnNewDocument和OnNewDocument（）用于文档对象的初始化和Serialize（）用于实现序列化，和DeleteContents以动态销毁分配的数据。

视图类产生的视图类名为MFCView，职责时以图形方式将文档数据显示给用户，以及接受用户输入并将用户输入解释为对文档的操作。编写视图类的任务通常包括

·编写视图类的OnDraw成员函数来呈现文档的数据

·将适当的Windows消息和用户界面对象（如菜单项）链接到视图类中的消息处理成员函数。用以实现处理程序解释用户输入。

可能重写的函数

·OnInitialUpdate（）函数为视图执行特殊的初始化

·OnUpdate（）函数以执行在视图重绘自己钱所需的任何特殊处理

·对于多页文档，可能还必须重写OnPreparePrinting（）函数，用要打印的页数和其他信息初始化打印对话框

·OnDraw（）的重载

通过调用文档成员函数获取数据

通过调用框架传递给OnDraw（）函数的设备上下文对象的成员函数来显示数据

当文档的数据以某种方式更改后，必须重绘视图以反映该更改，一般情况下，文档数据的更改在用户通过视图对文档进行更改时发生。在此情况下，视图调用文档的UpdateAllViews（）成员函数，通知同一文档上的所有视图对自身进行更新，UpdateAllViews（）函数调用每个视图的OnUpdate（）成员函数，使视图的整个工作区无效。可以重写OnUpdate，仅使工作区中映射到文档修改部分的那些区域无效。

文档和视图的关系：

文档是数据源（磁盘文件，串口，网络，摄像机），文档对象负责来自所有数据源的数据的管理。

视图是数据的显示窗口，为用户提供了文档的可视的数据显示，和用户数据交互界面。

MFC的文档视图结构机制把数据同它的显示以及用户对数据的操作分离开来，所有对数据的修改由文档来完成，视图调用这个对象的方法来访问和更新数据。

CDocument类的UpdateAllViews（）成员函数和CView类的OnUpdate（）成员函数传递描述被修改的文档部分的信息。上述机制可限制视图必须重绘的区域。当视图变得无效时，Windows将WM-PAINT消息发给它，该视图的OnPaint（）函数处理函数通过创建CPaintDC类设备上下文对象来相应该消息并调用视图的OnDraw（）成员函数，通常不必编写重写的OnPaint（）处理成员函数。设备上下文时包含设备（显示器，打印机等）绘图属性信息的Windows数据结构，所有的绘图调用都通过设备上下文对象进行。对于屏幕上的绘图，CPaintDC对象传递给OnDraw（）函数。对于打印机上的绘图，为当前打印机设置的CDC对象传递给OnDraw（）函数。视图中的绘图代码首先检索指向文档的指针，然后通过设备上下文进行绘图调用.

滚动：自定义或CScrollView类

CScrollView在默认情况下执行下列操作

·管理窗口和书图区大小以及映射模式

·相应滚动条消息进行自动滚动

.技巧：使MDI启动时不建立一个新文件

使用AppWizard工具生成的MDI程序启动时会默认创建一个空白文件，这有时候带来麻烦。为了使MDI启动时不建立一个新文件，需要修改InitInstance（）函数如下：

 CCommandLineInfo cmdInfo;

if(cmdInfo.m_nShellCommand ==CCommandLineInfo::FileNew)

{

  cmdInfo.m_nShellCommand =CCommandLineInfo::FileNothing;

}

.关于CCommandLineInfo类

一个MFC应用程序会在它的应用对象的CWinApp：：InitInstance（）函数中典型的产生一个这个类的本地实例，这个对象然后被送到CWinApp：：ParseCommandLine（），它重复调用ParaseParam（）来充实CCommandLinInfo对象。这个对象然后被送到CWinApp：：ProcessShellCommand来处理command-line的参数和标识。

*******************************************************************************

