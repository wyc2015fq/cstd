# VC程序调试 - 深之JohnChen的专栏 - CSDN博客

2009年11月11日 12:40:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4900



调试是一个程序员最基本的技能，其重要性甚至超过学习一门语言。不会调试的程序员就意味着他即使会一门语言，却不能编制出任何好的软件。

一、VC程序调试方法

VC的调试功能：首先，再次强调要用Go命令运行一个将要调试的程序；如果要中止调试状态下的运行程序可以点击Stop Debugging命令，还可以通过Break选项以可恢复方式中断调试程序的运行流程（用Restart选项可以重新开始运行程序）；Step Into选项表示每次只执行一行语句（单步执行），但如果当前代码是调用一个函数，那么Step Into表示进入该函数，全部函数语句执行完后返回，而Step Over则是跳出这个函数；Step To Cursor选项表示程序将执行到光标所在的可执行语句行上；在调试多线程程序时，可以在线程函数或主应用程序线程中设置断点，还可以用Break选项结束线程后用Threads选项查看运行线程列表，也可以选择悬挂和恢复每个线程；在设置断点后，在VC "查看"菜单的"调试窗口"中可以查看变量、[内存](http://product.yesky.com/catalog/219/)、调用堆栈、寄存器以及反汇编语句。在程序中设置断点的方法是，点击要设置的代码行并点击设置代码的工具栏按钮，会出现在代码行最左边的一个小黑点即是断点标志，这时再选Go程序会在执行到端点处停下来，如果要继续执行可以再选Go。
　　通过选择VC"工具"菜单下的"源浏览器"可以生成一个.BSC文件，使用浏览器可以从中发现多种信息：程序中任何一个变量、函数、类或宏在何处定义及引用；可以列出所有声明的函数类、变量、宏；可以发现调用一个指定函数的所有函数；可以找到一个指定类的派生来源或者它派生出哪些类。

在使用微软程序开发库MSDN时，我们会发现其中的VC示例经常采用看似多余的ASSERT语句，其作用就是使程序具有"维护"性。对于Debug版本的VC程序，在遇到布尔值为FALSE的ASSERT语句处停止，并显示Assertion Failed对话；如果设置为发布版，所有ASSERT语句都会被预处理程序删除。ASSERT宏在发行版本中不起作用，而应该用VERIFY来进行发行版的调试。如果发行版本运行有问题，可以先禁止所有代码优化再进行调试。

系统提供一系列特殊的函数或者宏来处理Debug版本相关的信息，如下：

|宏名/函数名|说明|
|----|----|
|TRACE|使用方法和printf完全一致，他在output框中输出调试信息|
|ASSERT|它接收一个表达式，如果这个表达式为TRUE，则无动作，否则中断当前程序执行。对于系统中出现这个宏导致的中断，应该认为你的函数调用未能满足系统的调用此函数的前提条件。例如，对于一个还没有创建的窗口调用SetWindowText等。|
|VERIFY|和ASSERT功能类似，所不同的是，在Release版本中，ASSERT不计算输入的表达式的值，而VERIFY计算表达式的值。|

　　在安装好VC系统之后，在VC之外的程序组中有一个程序Tracer是一个跟踪工具，在激活它后使用Go运行VC代码，在输出窗口就能够看到程序运行过程中的内部过程，包括DLL调用等，你如果看不到任何输出，可以转到菜单"查看"（View）点击"输出"（Output）。

　　其实，MFC自身就提供有错误查找和TRACE语句，而TRACE语句的语法与printf非常类似，所以我们可以在程序中直接加入这条跟踪命令，如下所示：

// Example for TRACE
int i = 1;
char sz[] = "one";
TRACE( "Integer = %d, String = %s/n", i, sz );
// Output: 'Integer = 1, String = one'

　　在Developer Studio中还提供了一个ERRLOOK工具，程序员只要输入错误号就能得到系统出错信息或模块错误内容。

MFC从Cobject派生的每个类都包含一个Dump函数，该函数可把当前状态转储（Dumping）到输出窗口，这在某些调试过程中会有用，以下代码是Dump函数的用法：

// Example for CObject::Dump
void CAge::Dump( CDumpContext &dc ) const
{
CObject::Dump( dc );
dc << "Age = " << m_years;
}

　　在MFC中还有一个非常有用的类是CMemoryState，我们可以在程序的任何部分使用这个类检测内存冲突，并得到内存冲突的确切位置。CMemoryState类有3个成员函数：CheckPoint可将堆的当前状态存入类的实体；Difference可以比较两个实体包含的堆之间的差异；DumpStatistics用于标准化转储所有被CheckPoint捕获后分配到堆的对象，如CheckPoint未被调用实体未被初始化时，该函数将转储当前堆的所有内容。以下代码表示了CMemoryState类的使用方法：

// Example for CMemoryState::CMemoryState，
// Includes all CMemoryState functions
CMemoryState msOld, msNew, msDif;
msOld.Checkpoint();
CAge* page1 = new CAge( 21 );
CAge* page2 = new CAge( 22 );
msOld.DumpAllObjectsSince();
msNew.Checkpoint();
msDif.Difference( msOld, msNew );
msDif.DumpStatistics();

　　代码运行的结果为:

Dumping objects ->
{2} a CObject at $190A
{1} a CObject at $18EA
Object dump complete.
0 bytes in 0 Free Blocks
8 bytes in 2 Object Blocks
0 bytes in 0 Non-Object Blocks
Largest number used: 8 bytes
Total allocations: 8 bytes 

　　在MFC类和VC中本身就有"异常情况"这个概念，并在此基础上形成它们处理系统错误和意外的主要机制。比如当系统内存分配殆尽时，你的运行程序就会收到内存异常的消息。这样就给了程序员消除异常的机会。

MFC中的异常情况主要有：CArchiveException表示档案文件载入或保存时出错，CDBException属于数据库错误，CFileException为文件错误，CMemoryException为调用new时发生分配错误，CNotSupportedException表示指定操作不被支持，COleException表示在调用OLE操作时出错，COleDispatchException表示在OLE自动操作时出错，CResourceException表示资源找不到或无法创建，CUserException用于通知用户错误。
MFC还包含一系列以Afx－为词头的调试函数：AfxAbort可以在发生致命错误时异常终止程序，AfxCheckMemory可以检查堆和剩余缓冲池的受损部分；AfxDoForAllClasses重声明所有CObject的派生类；AfxDoForAllObject重声明堆上所有CObject派生的对象；AfxEnableMemoryTracking启用或禁止内存追踪；AfxIsMemoryBlock用于确认指针所指内存有效；AfxIsValidAddress用于确认地址是驻留在程序的内存区域内；AfxIsValidString用于确认地址所指字符串有效；AfxSetAllocHook用于内存分配前进行检测；AfxTraceEnabled启动或禁止输出跟踪，AfxTraceFlags则进一步定制跟踪特征。

二、VC6.0与VC2008 Release版本调试参数设置比较

VC6.0Release版本调试参数设置

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20091111/1.JPG)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20091111/2.JPG)


VC2008 Release版本调试参数设置

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20091111/3.JPG)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20091111/4.JPG)



![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20091111/5.JPG)

三、注意细节

1、对于所有有返回值的函数，都应该检查返回值，除非你确信这个函数调用绝对不会出错，或者不关心它是否出错。

一些函数返回错误，需要用其他函数获得错误的具体信息。例如accept返回INVALID_SOCKET表示accept失败，为了查明具体的失败原因，应该立刻用WSAGetLastError获得错误码，并针对性的解决问题。

2、有些函数通过异常机制抛出错误，应该用TRY-CATCH语句/TRY-CATCH-finally来检查错误。程序员对于能处理的错误，应该自己在底层处理，对于不能处理的，应该报告给用户让他们决定怎么处理。如果程序出了异常，却不对返回值和其他机制返回的错误信息进行判断，只能是加大了找错误的难度。

3、VC中要编制程序不应该一开始就写cpp/h文件，而应该首先创建一个合适的工程。因为只有这样，VC才能选择合适的编译、连接选项。对于加入到工程中的cpp文件，应该检查是否在第一行显式的包含stdafx.h头文件，这是Microsoft Visual Studio为了加快编译速度而设置的预编译头文件。在这个#include "stdafx.h"行前面的所有代码将被忽略，所以其他头文件应该在这一行后面被包含。

对于.c文件，由于不能包含stdafx.h，因此可以通过Project settings把它的预编译头设置为“不使用”，方法是：

弹出Project settings对话框

选择C/C++ 

Category选择Precompilation Header 选择不使用预编译头。

4、用GetLastError来检测返回值，通过得到错误代码来分析错误原因

5、把错误信息记录到文件中

6、查看API的错误码，在watch窗口输入@err可以查看或者@err,hr，其中”,hr”表示错误码的说明。

7、查看数组中的值时，在watch窗口输入数组名称,数组大小就可以查看里面的内容值。

8、Set Next Statement：该功能可以直接跳转到指定的代码行执行，一般用来测试异常处理的代码。

9、使用映射文件调试

a)、创建映射文件：Project settings中link项，选中Generate mapfile，输出程序代码地址：/MAPINFO: LINES，得到引出序号：/MAPINFO: EXPORTS。

b)、程序发布时，应该把所有模块的映射文件都存档。

c)、查看映射文件：见” 通过崩溃地址找出源代码的出错行”文件。

