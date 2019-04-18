# 内存管理：_CrtDumpMemoryLeaks和_CrtSetBreakAlloc - tifentan的专栏 - CSDN博客

2017年07月03日 19:27:40[露蛇](https://me.csdn.net/tifentan)阅读数：132



1._CrtDumpMemoryLeaks

确定自程序开始执行以来是否发生过内存泄漏，如果发生过，则转储所有已分配对象。如果已使用 _CrtSetDumpClient 安装了挂钩函数，那么，_CrtDumpMemoryLeaks每次转储 _CLIENT_BLOCK 块时，都会调用应用程序所提供的挂钩函数。 

CrtDumpMemoryLeaks()就是显示当前的内存泄漏。 注意是“当前”，也就是说当它执行时，所有未销毁的对象均会报内存泄漏。因此尽量让这条语句在程序的最后执行。它所反映的是检测到泄漏的地方。

一般用在MFC中比较准确，在InitInstance里面调用_CrtDumpMemoryLeaks

2.信息输出

Detected memory leaks!

Dumping objects ->

{52} normal block at 0x006D2498, 512 bytes long.

?Data: <??????????????? > 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

{51} normal block at 0x006D2440, 24 bytes long.

?Data: < 4????????????? > 10 34 14 00 FF FF FF FF 00 00 00 00 00 00 00 00 

Object dump complete.

3._CrtSetBreakAlloc

知道某个错误分配块的分配请求编号后，可以将该编号传递给 _CrtSetBreakAlloc 以创建一个断点

_CrtSetBreakAlloc(51);这样可以快速在{51}次内存泄漏处设上断点。

/*****************************************************************************************************/

最快速度找到内存泄漏

许式伟

2006年11月某日

内存管理是C++程序员的痛。我的《内存管理变革》系列就是试图讨论更为有效的内存管理方式，以杜绝（或减少）内存泄漏，减轻C++程序员的负担。由于工作忙的缘故，这个系列目前未完，暂停。

这篇短文我想换个方式，讨论一下如何以最快的速度找到内存泄漏。

确认是否存在内存泄漏

我们知道，MFC程序如果检测到存在内存泄漏，退出程序的时候会在调试窗口提醒内存泄漏。例如：

class CMyApp : public CWinApp

{

public:

   BOOL InitApplication()

   {

       int* leak = new int[10];

       return TRUE;

   }

};

产生的内存泄漏报告大体如下：

Detected memory leaks!

Dumping objects ->

c:\work\test.cpp(186) : {52} normal block at 0x003C4410, 40 bytes long.

 Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD 

Object dump complete.

这挺好。问题是，如果我们不喜欢MFC，那么难道就没有办法？或者自己做？

呵呵，这不需要。其实，MFC也没有自己做。内存泄漏检测的工作是VC++的C运行库做的。也就是说，只要你是VC++程序员，都可以很方便地检测内存泄漏。我们还是给个样例：

#include <crtdbg.h>

inline void EnableMemLeakCheck()

{

   _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

}

void main()

{

   EnableMemLeakCheck();

   int* leak = new int[10];

}

 运行（提醒：不要按Ctrl+F5，按F5），你将发现，产生的内存泄漏报告与MFC类似，但有细节不同，如下：

Detected memory leaks!

Dumping objects ->

{52} normal block at 0x003C4410, 40 bytes long.

 Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD 

Object dump complete.

为什么呢？看下面。

定位内存泄漏由于哪一句话引起的

你已经发现程序存在内存泄漏。现在的问题是，我们要找泄漏的根源。

一般我们首先确定内存泄漏是由于哪一句引起。在MFC中，这一点很容易。你双击内存泄漏报告的文字，或者在Debug窗口中按F4，IDE就帮你定位到申请该内存块的地方。对于上例，也就是这一句：

   int* leak = new int[10];

这多多少少对你分析内存泄漏有点帮助。特别地，如果这个new仅对应一条delete（或者你把delete漏写），这将很快可以确认问题的症结。

我们前面已经看到，不使用MFC的时候，生成的内存泄漏报告与MFC不同，而且你立刻发现按F4不灵。那么难道MFC做了什么手脚？

其实不是，我们来模拟下MFC做的事情。看下例：

inline void EnableMemLeakCheck()

{

   _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

}

#ifdef _DEBUG

#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif

void main()

{

   EnableMemLeakCheck();

   int* leak = new int[10];

}

再运行这个样例，你惊喜地发现，现在内存泄漏报告和MFC没有任何分别了。

快速找到内存泄漏

单确定了内存泄漏发生在哪一行，有时候并不足够。特别是同一个new对应有多处释放的情形。在实际的工程中，以下两种情况很典型：

创建对象的地方是一个类工厂（ClassFactory）模式。很多甚至全部类实例由同一个new创建。对于此，定位到了new出对象的所在行基本没有多大帮助。 

COM对象。我们知道COM对象采用Reference Count维护生命周期。也就是说，对象new的地方只有一个，但是Release的地方很多，你要一个个排除。 

那么，有什么好办法，可以迅速定位内存泄漏？

答：有。

在内存泄漏情况复杂的时候，你可以用以下方法定位内存泄漏。这是我个人认为通用的内存泄漏追踪方法中最有效的手段。

我们再回头看看crtdbg生成的内存泄漏报告：

Detected memory leaks!

Dumping objects ->

c:\work\test.cpp(186) : {52} normal block at 0x003C4410, 40 bytes long.

 Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD 

Object dump complete. 

除了产生该内存泄漏的内存分配语句所在的文件名、行号为，我们注意到有一个比较陌生的信息：{52}。这个整数值代表了什么意思呢？

其实，它代表了第几次内存分配操作。象这个例子，{52}代表了第52次内存分配操作发生了泄漏。你可能要说，我只new过一次，怎么会是第52次？这很容易理解，其他的内存申请操作在C的初始化过程调用的呗。:)

有没有可能，我们让程序运行到第52次内存分配操作的时候，自动停下来，进入调试状态？所幸，crtdbg确实提供了这样的函数：即 long _CrtSetBreakAlloc(long nAllocID)。我们加上它：

inline void EnableMemLeakCheck()

{

   _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

}

#ifdef _DEBUG

#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif

void main()

{

   EnableMemLeakCheck();

   _CrtSetBreakAlloc(52);

   int* leak = new int[10];

}

你发现，程序运行到 int* leak = new int[10]; 一句时，自动停下来进入调试状态。细细体会一下，你可以发现，这种方式你获得的信息远比在程序退出时获得文件名及行号有价值得多。因为报告泄漏文件名及行号，你获得的只是静态的信息，然而_CrtSetBreakAlloc则是把整个现场恢复，你可以通过对函数调用栈分析（我发现很多人不习惯看函数调用栈，如果你属于这种情况，我强烈推荐你去补上这一课，因为它太重要了）以及其他在线调试技巧，来分析产生内存泄漏的原因。通常情况下，这种分析方法可以在5分钟内找到肇事者。

当然，_CrtSetBreakAlloc要求你的程序执行过程是可还原的（多次执行过程的内存分配顺序不会发生变化）。这个假设在多数情况下成立。不过，在多线程的情况下，这一点有时难以保证。

附加说明：

对“内存管理”相关的技术感兴趣？这里可以看到我的所有关于内存管理的文章。

/****************************************************************************************************************/

                                                  VC使用CRT调试功能来检测内存泄漏

                                                                     作者：JerryZ

C/C++ 编程语言的最强大功能之一便是其动态分配和释放内存，但是中国有句古话：“最大的长处也可能成为最大的弱点”，那么 C/C++ 应用程序正好印证了这句话。在 C/C++ 应用程序开发过程中，动态分配的内存处理不当是最常见的问题。其中，最难捉摸也最难检测的错误之一就是内存泄漏，即未能正确释放以前分配的内存的错误。偶尔发生的少量内存泄漏可能不会引起我们的注意，但泄漏大量内存的程序或泄漏日益增多的程序可能会表现出各种 各样的征兆：从性能不良（并且逐渐降低）到内存完全耗尽。更糟的是，泄漏的程序可能会用掉太多内存，导致另外一个程序垮掉，而使用户无从查找问题的真正根源。此外，即使无害的内存泄漏也可能殃及池鱼。

　　幸运的是，Visual Studio 调试器和 C 运行时 (CRT) 库为我们提供了检测和识别内存泄漏的有效方法。下面请和我一起分享收获——如何使用 CRT 调试功能来检测内存泄漏？

一、如何启用内存泄漏检测机制

　　VC++ IDE 的默认状态是没有启用内存泄漏检测机制的，也就是说即使某段代码有内存泄漏，调试会话的 Output 窗口的 Debug 页不会输出有关内存泄漏信息。你必须设定两个最基本的机关来启用内存泄漏检测机制。

　　一是使用调试堆函数：

#define _CRTDBG_MAP_ALLOC 

#include<stdlib.h>

#include<crtdbg.h>

　　注意：#include 语句的顺序。如果更改此顺序，所使用的函数可能无法正确工作。

　　通过包含 crtdbg.h 头文件，可以将 malloc 和 free 函数映射到其“调试”版本 _malloc_dbg 和 _free_dbg，这些函数会跟踪内存分配和释放。此映射只在调试（Debug）版本（也就是要定义 _DEBUG）中有效。发行版本（Release）使用普通的 malloc 和 free 函数。#define 语句将 CRT 堆函数的基础版本映射到对应的“调试”版本。该语句不是必须的，但如果没有该语句，那么有关内存泄漏的信息会不全。

　　二是在需要检测内存泄漏的地方添加下面这条语句来输出内存泄漏信息：

_CrtDumpMemoryLeaks();

　　当在调试器下运行程序时，_CrtDumpMemoryLeaks 将在 Output 窗口的 Debug 页中显示内存泄漏信息。比如： Detected memory leaks!

Dumping objects ->

C:\Temp\memleak\memleak.cpp(15) : {45} normal block at 0x00441BA0, 2 bytes long.

Data: <AB> 41 42

c:\program files\microsoft visual studio\vc98\include\crtdbg.h(552) : {44} normal 

block at 0x00441BD0, 33 bytes long.

Data: < C > 00 43 00 CD CD CD CD CD CD CD CD CD CD CD CD CD

c:\program files\microsoft visual studio\vc98\include\crtdbg.h(552) : {43} normal 

block at 0x00441C20, 40 bytes long.

Data: < C > 08 02 43 00 16 00 00 00 00 00 00 00 00 00 00 00

Object dump complete.

　　如果不使用 #define _CRTDBG_MAP_ALLOC 语句，内存泄漏的输出是这样的：

Detected memory leaks!

Dumping objects ->

{45} normal block at 0x00441BA0, 2 bytes long.

Data: <AB> 41 42

{44} normal block at 0x00441BD0, 33 bytes long.

Data: < C > 00 43 00 CD CD CD CD CD CD CD CD CD CD CD CD CD

{43} normal block at 0x00441C20, 40 bytes long.

Data: < C > C0 01 43 00 16 00 00 00 00 00 00 00 00 00 00 00

Object dump complete.

　　根据这段输出信息，你无法知道在哪个源程序文件里发生了内存泄漏。下面我们来研究一下输出信息的格式。第一行和第二行没有什么可说的，从第三行开始：

xx}：花括弧内的数字是内存分配序号，本文例子中是 {45}，{44}，{43}；

block：内存块的类型，常用的有三种：normal（普通）、client（客户端）或 CRT（运行时）；本文例子中是：normal block； 

用十六进制格式表示的内存位置，如：at 0x00441BA0 等；

以字节为单位表示的内存块的大小，如：32 bytes long； 

前 16 字节的内容（也是用十六进制格式表示），如：Data: 41 42 等；

　　仔细观察不难发现，如果定义了 _CRTDBG_MAP_ALLOC ，那么在内存分配序号前面还会显示在其中分配泄漏内存的文件名，以及文件名后括号中的数字表示发生泄漏的代码行号，比如：

C:\Temp\memleak\memleak.cpp(15)

　　双击 Output 窗口中此文件名所在的输出行，便可跳到源程序文件分配该内存的代码行（也可以选中该行，然后按 F4，效果一样） ，这样一来我们就很容易定位内存泄漏是在哪里发生的了，因此，_CRTDBG_MAP_ALLOC 的作用显而易见。

使用 _CrtSetDbgFlag

　　如果程序只有一个出口，那么调用 _CrtDumpMemoryLeaks 的位置是很容易选择的。但是，如果程序可能会在多个地方退出该怎么办呢？在每一个可能的出口处调用 _CrtDumpMemoryLeaks 肯定是不可取的，那么这时可以在程序开始处包含下面的调用：_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );这条语句无论程序在什么地方退出都会自动调用 _CrtDumpMemoryLeaks。注意：这里必须同时设置两个位域标志：_CRTDBG_ALLOC_MEM_DF
 和 _CRTDBG_LEAK_CHECK_DF。

设置 CRT 报告模式

　　默认情况下，_CrtDumpMemoryLeaks 将内存泄漏信息 dump 到 Output 窗口的 Debug 页， 如果你想将这个输出定向到别的地方，可以使用 _CrtSetReportMode 进行重置。如果你使用某个库，它可能将输出定向到另一位置。此时，只要使用以下语句将输出位置设回 Output 窗口即可：

_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

　　有关使用 _CrtSetReportMode 的详细信息，请参考 MSDN 库关于 _CrtSetReportMode 的描述。

二、解释内存块类型

　　前面已经说过，内存泄漏报告中把每一块泄漏的内存分为 normal（普通块）、client（客户端块）和 CRT 块。事实上，需要留心和注意的也就是 normal 和 client，即普通块和客户端块。

　　1.normal block（普通块）：这是由你的程序分配的内存。

　　2.client block（客户块）：这是一种特殊类型的内存块，专门用于 MFC 程序中需要析构函数的对象。MFC new 操作符视具体情况既可以为所创建的对象建立普通块，也可以为之建立客户块。

　　3.CRT block（CRT 块）：是由 C RunTime Library 供自己使用而分配的内存块。由 CRT 库自己来管理这些内存的分配与释放，我们一般不会在内存泄漏报告中发现 CRT 内存泄漏，除非程序发生了严重的错误（例如 CRT 库崩溃）。

　　除了上述的类型外，还有下面这两种类型的内存块，它们不会出现在内存泄漏报告中：

　　1.free block（空闲块）：已经被释放(free)的内存块。 

　　2.Ignore block（忽略块）：这是程序员显式声明过不要在内存泄漏报告中出现的内存块。

三、如何在内存分配序号处设置断点

　　在内存泄漏报告中，的文件名和行号可告诉分配泄漏的内存的代码位置，但仅仅依赖这些信息来了解完整的泄漏原因是不够的。因为一个程序在运行时，一段分配内存的代码可能会被调用很多次，只要有一次调用后没有释放内存就会导致内存泄漏。为了确定是哪些内存没有被释放，不仅要知道泄漏的内存是在哪里分配的，还要知道泄漏产生的条件。这时内存分配序号就显得特别有用——这个序号就是文件名和行号之后的花括弧里的那个数字。

　　例如，在本文例子代码的输出信息中，“45”是内存分配序号，意思是泄漏的内存是你程序中分配的第四十五个内存块：

Detected memory leaks!

Dumping objects ->

C:\Temp\memleak\memleak.cpp(15) : {45} normal block at 0x00441BA0, 2 bytes long.

Data: <AB> 41 42

......

Object dump complete.

　　CRT 库对程序运行期间分配的所有内存块进行计数，包括由 CRT 库自己分配的内存和其它库（如 MFC）分配的内存。因此，分配序号为 N 的对象即为程序中分配的第 N 个对象，但不一定是代码分配的第 N 个对象。（大多数情况下并非如此。）这样的话，你便可以利用分配序号在分配内存的位置设置一个断点。方法是在程序起始附近设置一个位置断点。当程序在该点中断时，可以从 QuickWatch（快速监视）对话框或 Watch（监视）窗口设置一个内存分配断点：

　　例如，在 Watch 窗口中，在 Name 栏键入下面的表达式：

_crtBreakAlloc

　　如果要使用 CRT 库的多线程 DLL 版本（/MD 选项），那么必须包含上下文操作符，像这样：

{,,msvcrtd.dll}_crtBreakAlloc

　　现在按下回车键，调试器将计算该值并把结果放入 Value 栏。如果没有在内存分配点设置任何断点，该值将为 –1。

　　用你想要在其位置中断的内存分配的分配序号替换 Value 栏中的值。例如输入 45。这样就会在分配序号为 45 的地方中断。

　　在所感兴趣的内存分配处设置断点后，可以继续调试。这时，运行程序时一定要小心，要保证内存块分配的顺序不会改变。当程序在指定的内存分配处中断时，可以查看 Call Stack（调用堆栈）窗口和其它调试器信息以确定分配内存时的情况。如果必要，可以从该点继续执行程序，以查看对象发生了什么情况，或许可以确定未正确释放对象的原因。

　　尽管通常在调试器中设置内存分配断点更方便，但如果愿意，也可在代码中设置这些断点。为了在代码中设置一个内存分配断点，可以增加这样一行（对于第四十五个内存分配）：

_crtBreakAlloc = 45;

　　你还可以使用有相同效果的 _CrtSetBreakAlloc 函数：

_CrtSetBreakAlloc(45);

四、如何比较内存状态

　　定位内存泄漏的另一个方法就是在关键点获取应用程序内存状态的快照。CRT 库提供了一个结构类型 _CrtMemState。你可以用它来存储内存状态的快照：

_CrtMemState s1, s2, s3;

　　若要获取给定点的内存状态快照，可以向 _CrtMemCheckpoint 函数传递一个 _CrtMemState 结构。该函数用当前内存状态的快照填充此结构：

_CrtMemCheckpoint( &s1 );

　　通过向 _CrtMemDumpStatistics 函数传递 _CrtMemState 结构，可以在任意地方 dump 该结构的内容：

_CrtMemDumpStatistics( &s1 );

　　该函数输出如下格式的 dump 内存分配信息：

0 bytes in 0 Free Blocks.

75 bytes in 3 Normal Blocks.

5037 bytes in 41 CRT Blocks.

0 bytes in 0 Ignore Blocks.

0 bytes in 0 Client Blocks.

Largest number used: 5308 bytes.

Total allocations: 7559 bytes.

　　若要确定某段代码中是否发生了内存泄漏，可以通过获取该段代码之前和之后的内存状态快照，然后使用 _CrtMemDifference 比较这两个状态：

_CrtMemCheckpoint( &s1 );// 获取第一个内存状态快照

// 在这里进行内存分配

_CrtMemCheckpoint( &s2 );// 获取第二个内存状态快照

// 比较两个内存快照的差异

if ( _CrtMemDifference( &s3, &s1, &s2) )

_CrtMemDumpStatistics( &s3 );// dump 差异结果

　　顾名思义，_CrtMemDifference 比较两个内存状态（前两个参数），生成这两个状态之间差异的结果（第三个参数）。在程序的开始和结尾放置 _CrtMemCheckpoint 调用，并使用 _CrtMemDifference 比较结果，是检查内存泄漏的另一种方法。如果检测到泄漏，则可以使用 _CrtMemCheckpoint 调用通过二进制搜索技术来分割程序和定位泄漏。

五、结论

　　尽管 VC ++ 具有一套专门调试 MFC 应用程序的机制，但本文上述讨论的内存分配很简单，没有涉及到 MFC 对象，所以这些内容同样也适用于 MFC 程序。在 MSDN 库中可以找到很多有关 VC++ 调试方面的资料，如果你能善用 MSDN 库，相信用不了多少时间你就有可能成为调试高手。

