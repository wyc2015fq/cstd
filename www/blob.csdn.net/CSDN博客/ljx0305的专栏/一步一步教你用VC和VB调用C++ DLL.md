# 一步一步教你用VC和VB调用C++ DLL - ljx0305的专栏 - CSDN博客
2009年06月09日 17:05:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1476标签：[vb																[c++																[dll																[api																[thread																[preprocessor](https://so.csdn.net/so/search/s.do?q=preprocessor&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=vb&t=blog)
个人分类：[DLL																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403907)
你好，Step by Step: Calling C++ DLLs from VC++ and VB 
一步一步教你用VC和VB调用C++ DLL. 
作者 Hans Dietrich 翻译烟灰 
介绍 
本系列教程讨论了普通情况下4种使用DLL的方法 
Part 1 
从VC++应用程序调用C++ DLL的函数 
从VC++应用程序调用C++ DLL的类 
Part 2 
从VB应用程序调用C++ DLL的函数 
Part 3 
从VB应用程序调用C++ DLL的类 
Part 4 
从VC++应用程序动态的调用C++ DLL的函数 
从VC++应用程序调用C++ DLL的函数 
Visual Studio 6 使创建包含函数或类的动态连接库(DLL) 变得非常容易. 
第一步 
打开 Visual Studio 然后选择 File | New菜单项: 
选择 Win32 Dynamic Link Library, 输入工程名, 敲 OK. 
选择 A DLL that exports some symbols 并单击Finish.在File View里你会看到如下的工程文件: 
第二步 
在Test.cpp里,你将看到如下代码: 
// Test.cpp : Defines the entry point for the DLL application.//#include "stdafx.h"#include "Test.h"BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){ switch (ul_reason_for_call) { case DLL_PROCESS_ATTACH: case DLL_THREAD_ATTACH: case DLL_THREAD_DETACH: case DLL_PROCESS_DETACH: break; } return TRUE;}// This is an example of an exported variableTEST_API int nTest=0;// This is an example of an exported function.TEST_API int fnTest(void){ return 42;}// This is the constructor of a class that has been exported.// see Test.h for the class definitionCTest::CTest(){ return; } 
Test.cpp 包含了 fnTest 和 CTest::CTest.如果你现在编译Test.dll, 你将会得到一个可以被其他VC++应用程序直接调用的DLL. 允许其他VC++程序调用的关键机制?( key mechanism)就包含在 Test.h中: 
// The following ifdef block is the standard way of creating macros// which make exporting from a DLL simpler. All files within this DLL// are compiled with the TEST_EXPORTS symbol defined on the command line.// This symbol should not be defined on any project that uses this DLL.// This way any other project whose source files include this file see // TEST_API functions as being imported from a DLL, whereas this DLL// sees symbols defined with this macro as being exported.#ifdef TEST_EXPORTS#define TEST_API __declspec(dllexport)#else#define TEST_API __declspec(dllimport)#endif// This class is exported from the Test.dllclass TEST_API CTest {public: CTest(void); // TODO: add your methods here.};extern TEST_API int nTest;TEST_API int fnTest(void); 
这里面发生了什么? #ifdef TEST_EXPORTS是什么意思? TEST_EXPORTS又是在哪定义的? 
TEST_EXPORTS 如果被定义, 那么TEST_API将会被定义为 __declspec(dllexport) (DLL导出), 否则,将会被定义为__declspec(dllimport)(DLL导入). 这将影响到后边定义的Ctest类是导出类还是导入类. 这意味着如果我们需要导出的时候,我们就得定义TEST_EXPORTS. 当一个VC++应用程序要访问这个DLL的时候,可以将Test.lib链接进去,它包含了DLL的导出符号. 
第三步 
TEST_EXPORTS 在哪里被定义了呢? DLL wizard干了一件我讨厌的事,它把TEST_EXPORTS放到了命令行里. 选择 Project | Settings | C/C++ | General, 你将看到工程选项: 
当然了,这个办法是可行的. 但是它却容易让人忽计,并且可能导致维护上的麻烦. 我比较喜欢清楚明白的定义TEST_EXPORTS : 从项目选项里边去掉/D "TEST_EXPORTS",然后在Test.cpp 里来定义它: 
// Test.cpp : Defines the entry point for the DLL application. 
// 
#include "stdafx.h" 
#define TEST_EXPORTS // <=== ADD THIS LINE 
#include "Test.h" 
BOOL APIENTRY DllMain( HANDLE hModule, 
DWORD ul_reason_for_call, 
LPVOID lpReserved) 
{ 
. 
. 
. 
注意 #define TEST_EXPORTS 在 #include "Test.h"前边. 所以,它定义要在头文件里.现在,可以像先前那样重新编译我们的Test.dll, 我们将得到一个可以被其他VC应用程序所调用的DLL. 
第四步 
我们如何调用DLL里的函数呢? 举个例子吧, 我用VS创建一个示例. 选MFC AppWizard (exe),输入项目名字,然后点OK. 选择基于对话框. 然后点Finish. 打开 XXXDlg.cpp(XXX是你的工程名字.) 找到OnInitDialog()成员函数, 敲进去如下的代码: 
. . . // Set the icon for this dialog. The framework does this automatically // when the application's main window is not a dialog SetIcon(m_hIcon, TRUE); // Set big icon SetIcon(m_hIcon, FALSE); // Set small icon // code to test Test.dll function: int n = fnTest(); // <=== ADD THIS LINE // code to test the CTest class: CTest test; // <=== ADD THIS LINE return TRUE; // return TRUE unless you set the focus to a control} 
第五步 
我们还没写完代码呢, 现在要把 Test.h这个DLL的头文件包含进去: 
// TestExeDlg.cpp : implementation file//#include "stdafx.h"#include "TestExe.h"#include "TestExeDlg.h"#include "Test.h" // <=== ADD THIS LINE... 
第六步 
如果你想赶时间做一个演示的话, 你可能会尝试只是拷贝DLL的 test.h 到你的项目目录里去,那么编译器会找到它. 但是当项目很大的时候,这可不是个好主意, 因为当你更新你的DLL文件时,可能会遇上危险.比如忘了拷贝它到你的exe的目录中去. 这里有个简单的方法来解决这个问题 : 选择Project | Settings | C/C++ | Settings | Preprocessor, 并且添加Additional include directories: (DLL工程的目录) 
提示 这样做实际上是假设DLL项目和EXE项目拥有同一个项目目录. 
现在当我编译的时候, 我得到了一个 linker errors!! 
Deleting intermediate files and output files for project 'TestExe - Win32 Debug'. 
--------------------Configuration: TestExe - Win32 Debug-------------------- 
Compiling resources... 
Compiling... 
StdAfx.cpp 
Compiling... 
TestExe.cpp 
TestExeDlg.cpp 
Generating Code... 
Linking... 
TestExeDlg.obj : error LNK2001: unresolved external symbol "__declspec(dllimport) 
public: __thiscall CTest::CTest(void)" ([__imp_??0CTest@@QAE@XZ](mailto:__imp_??0CTest@@QAE@XZ)) 
TestExeDlg.obj : error LNK2001: unresolved external symbol "__declspec(dllimport) 
int __cdecl fnTest(void)" ([__imp_?fnTest@@YAHXZ](mailto:__imp_?fnTest@@YAHXZ)) 
Debug/TestExe.exe : fatal error LNK1120: 2 unresolved externals 
Error executing link.exe. 
TestExe.exe - 3 error(s), 0 warning(s) 
第七步 
虽然我已经告诉编译器DLL符号啦,但是链接器还不知道. 所以我们必须告诉链接器.. 选择Project | Settings | Link,把DLL的lib文件加到Object/library modules里边去: 
---------------------------------------------- 
第八步 
好啦,现在编译通过. 在我们运行程序前,别忘了一件事: 把Test.dll 拷贝到EXE的目录里去. 
第九步 
接下来,可以放一个 断点到OnInitDialog()函数里去, 点 GO(F5)调试运行: 
可以看到, fnTest 返回了42, 和我们预测的一样. CTest 类也可以用类似的方法来测试. 
要点. 
VS的工程向导为我们创建VC++DLL提供了很好的开始. 
函数,类, 和变量 可以从DLL中导出. 
使用 #define 预处理器定义, 一个头文件将可以被DLL 和应用程序共同使用. 
DLL导出它的符号,并且应用程序导入这个DLL符号. 当编译应用程序时,编译器通过头文件看到的DLL符号, 当链接应用程序时, 链接器通过导入库(Test.lib)看到DLL符号. 
当执行应用程序时,DLL必须放到和EXE相同的目录中去. DLL也可以放到 windows或者system目录中,这样也是可行的, 但是它经常引起某些问题, 所以应该避免这样使用 
注释: 
再实际工作中,我很少用到 第七步中的方法. 这样做的话,在大的工程中,DLL和Lib文件将经常变得很难管理.我们会想到要建立一个lib目录和一个 bin目录,在这里边放进去所有我们要使用的lib文件 , dll文件 和exe文件. 如果这样做的话,我们怎么告诉链接器找到lib文件呢? 有两种方法来做: 
1. 选择Tools | Options | Directories and set Show directories for 为"Library files". 在下边添加上我们工程所使用的Lib文件的路径. 
2. 另一种办法是,选择 Project | Settings | Link, 选category为 Input ,在下边的 Additional library path 筐里输入工程使用的lib文件的所在路径. 
哪种方法更好一点呢?这取决于你的开发环境. 第一种方法要求整个工程要共享的设置目录路径, 并且所有要求所有的开发者的VS都必须设置到这些路径. 
第二种方法允许每个工程定制自己的路径,并且在工程中被储存,如果开发者的计算机上存放了同样的目录,那么每个开发者都可以签出工程并且设计. ,这样可以避免在每台机器上都去设置同样的路径. 
到现在,我还没有说怎样指定要使用的LIB文件, 我的办法是在DLL的Test.h中添加两行,现在它看起来像下边的样子: 
#ifdef TEST_EXPORTS #define TEST_API __declspec(dllexport)#else #define TEST_API __declspec(dllimport) #pragma message("automatic link to Test.lib") // <== add this line #pragma comment(lib, "Test.lib") // <== add this line#endif// This class is exported from the Test.dllclass TEST_API CTest {public: CTest(void);};extern TEST_API int nTest;TEST_API int fnTest(void); 
这样做,保证了当工程包含DLL的头文件时, 它会自动的把DLL 的lib链接进去,我们可以从VS的OUTPUT窗口看到#pragma message给我们的传达的"automatic link to Test.lib"这个消息. 2792希望对你有帮助
VC在DLL中创建线程的问题
a> 
不要在DllMain创建线程，因为在DllMain里的代码会被反复执行。 
可以单独使用个函数来启动线程，下面是我写的一个让PC喇叭每隔1秒响一次的DLL代码，你可以参考。 
//全局变量 
BOOL api_nThreadFlag = TRUE;//线程运行标志 
DWORD __stdcall Thread(LPVOID wParam); 
BOOL APIENTRY DllMain( HANDLE hModule, 
DWORD ul_reason_for_call, 
LPVOID lpReserved 
) 
{ 
switch (ul_reason_for_call) 
{ 
case DLL_PROCESS_ATTACH: 
case DLL_THREAD_ATTACH: 
case DLL_THREAD_DETACH: 
case DLL_PROCESS_DETACH: 
break; 
} 
return TRUE; 
} 
__declspec(dllexport) void WINAPI StartThread() 
{ 
//启动线程 
DWORD dwThreadId; 
api_nThreadFlag = TRUE; 
HANDLE hThread; 
hThread = CreateThread(NULL, 0, Thread, NULL, 0, &dwThreadId); 
CloseHandle(hThread); 
} 
__declspec(dllexport) void WINAPI EndThread() 
{ 
api_nThreadFlag = FALSE; 
} 
DWORD __stdcall Thread(LPVOID wParam) 
{ 
while(api_nThreadFlag) 
{ 
Beep(1000,1000); 
Sleep(1000); 
} 
return 0; 
} 
引用：[http://bbs.zdnet.com.cn/thread-876508-1-15.html](http://bbs.zdnet.com.cn/thread-876508-1-15.html)
本贴来自ZDNetChina中文社区 [http://bbs.zdnet.com.cn](http://bbs.zdnet.com.cn) ,本贴地址：[http://bbs.zdnet.com.cn/viewthread.php?tid=876508](http://bbs.zdnet.com.cn/viewthread.php?tid=876508)

