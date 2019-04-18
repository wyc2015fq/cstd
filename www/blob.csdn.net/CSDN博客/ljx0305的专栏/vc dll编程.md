# vc dll编程 - ljx0305的专栏 - CSDN博客
2009年06月09日 14:24:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：618标签：[dll																[编程																[mfc																[extension																[initialization																[thread](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=extension&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[VC																[DLL																[C++](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/403907)](https://blog.csdn.net/ljx0305/article/category/401790)
在我们实际用软件时，经常可看到许多动态连接库。动态连接库有其自身的优点
如节省内存、支持多语种等功能，而且，当DLL中的函数改变后，只要不是参数的改变
调用起的函数并不需要重新编译。这在编程时十分有用。至于其他妙处，各位在电脑
杂志、书籍中都能看到，我这里再说就是废话了.
这次小弟我所要讲的是如何在VC5.0中如何做自己的Win32 DLLs，各位要做自己的
动态连接库，首先要知道DLL在VC5.0中都有哪几种分类。VC支持三种DLL，它们是:
1.Non-MFC Dlls
2.Regular Dlls
3.Extension Dlls Note:翻译措辞不当，故遇到术语是引用原词
Non-MFC DLL:指的是不用MFC的类库结构，直接用C语言写的DLL，其输出的函数一
般用的是标准C接口，并能被非MFC或MFC编写的应用程序所调用。LL，
Regular DLL:和下述的Extension Dlls一样，是用MFC类库编写的。明显的特点是
在源文件里有一个继承CWinApp的类。其又可细分成静态连接到MFC和动态连接到MFC上
的。但静态连接到MFC的动态连接库只被VC的专业般和企业版所支持。
Extension DLL:用来实现从MFC所继承下来的类的重新利用，也就是说，用这种类
型的动态连接库，可以用来输出一个从MFC所继承下来的类。Extension DLL使用MFC的
动态连接版本所创建的，并且它只被用MFC类库所编写的应用程序所调用。
各位看到这里如果眼有点花或头有点晕，请别泄气，再看两遍，然后继续往下看，
定有收获。
标 题: 关于VC中的DLL的编程[1]
这一节介绍Non-MFC DLLs的编写方法。下面是一个通用的
写法：
BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,
LPVOID lpReserved)
{
switch( ul_reason_for_call ) {
case DLL_PROCESS_ATTACH:
.......
case DLL_THREAD_ATTACH:
.......
case DLL_THREAD_DETACH:
.......
case DLL_PROCESS_DETACH:
.......
}
return TRUE;
}
每一个DLL必须有一个入口点，这就象我们用C编写的应用程序一样，
必须有一个WINMAIN函数一样。
在这个示例中，DllMain是一个缺省的入口函数，你不需要编写自己
的DLL入口函数，并用linker的命令行的参数开关/ENTRY声明。用这个缺
省的入口函数就能使动态连接库被调用时得到正确的初始化，当然了，你
不要在初始化的时候填写使系统崩溃的代码了。
参数中，hMoudle是动态库被调用时所传递来的一个指向自己的句柄
(实际上，它是指向_DGROUP段的一个选择符)
ul_reason_for_call是一个说明动态库被调原因的标志。当进程或线程
装入或卸载动态连接库的时候，操作系统调用入口函数，并说明动态连接库
被调用的原因。它所有的可能值为：
DLL_PROCESS_ATTACH: 进程被调用
DLL_THREAD_ATTACH: 线程被调用
DLL_PROCESS_DETACH: 进程被停止
DLL_THREAD_DETACH: 线程被停止
lpReserved是一个被系统所保留的参数。
入口函数已经写了，盛下的也不难，你可以在文件中加入你所想要输
出的函数或变量或c++类或、或、或、？好象差部多了。Look here!现在就
要加入一个新的输出函数了:
void _declspec(dllexport) JustSoSo()
{
MessageBox(NULL,"It's so easy!","Hahaha......",MB_OK);
}
要输出一个类也可以，如下：
class _declspec(dllexport) Easy
{
//add your class definition...
};
各位一定注意到在输出函数或类是我用到_declspec(dllexport),
这是VC提供的一个关键字，用它可在动态连接库中输出一个数据、
一个函数或一个类。用这个关键字可省你不少事，你不用在.DEF文件
中说明我要输出这个类、那个函数的。
Ok!各位照着上面的例子试着敲敲看，Just so easy!
先说到这了
发信人: dragon (龙), 信区: VC
标 题: 关于VC中的DLL的编程[2]
前面讲到Non-MFC DLL的编法，现在讲讲调用DLL的方法。对DLL的
调用分为两种，一种是显式的调用，一种是隐式的调用。
所谓显式的调用，是指在应用程序中用LoadLibrary或MFC提供的
AfxLoadLibrary显式的将自己所做的动态连接库调近来，动态连接库
的文件名即是上两函数的参数，再用GetProcAddress()获取想要引入
的函数。自此，你就可以象使用如同本应用程序自定义的函数一样来
调用此引入函数了。在应用程序退出之前，应该用FreeLibrary或
MFC提供的AfxLoadLibrary释放动态连接库。
隐式的调用则需要把产生动态连接库时产生的.LIB文件加入到应
用程序的工程中，想使用DLL中的函数时，只须说明以下，如下：说明
上篇的输出函数void JustSoSo();
隐式调用不需要调用LoadLibrary()和FreeLibrary().
由此看来，隐式说明调用的方法比较简单，但DLL改变后，应用程序
须从新编译。并且，所有所调用的DLL在应用程序加载的同时被加载到内
存中，但应用程序调用的DLL比较多时，装入的过程十分慢。隐式的调用
则在应用程序不知道所要装入的DLL或隐式调用不成功，此时，允许用户
指定所要加载的动态连接库，比较灵活
发信人: dragon (龙), 信区: VC
标 题: 关于VC中的DLL的编程[3]
Regular DLL能够被所有支持DLL技术的语言所编写的应用程序
所调用。在这种动态连接库中，它必须有一个从CWinApp继承下来的
类，DllMain函数被MFC所提供，不用自己显式的写出来。下面是一个
例子：
// MyRegularDll.h:main header file for the MYREGULARDLL DLL
#include "resource.h" // main symbols
class CMyRegularDllApp : public CWinApp
{
public:
CMyRegularDllApp();
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CMyRegularDllApp)
//}}AFX_VIRTUAL
//{{AFX_MSG(CMyRegularDllApp)
// NOTE - the ClassWizard will add and
// remove member functions here.
// DO NOT EDIT what you see in these blocks
// of generated code !
//}}AFX_MSG
DECLARE_MESSAGE_MAP()
};
//MyRegularDll.cpp:Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "MyRegularDll.h"
// Note!
//
// If this DLL is dynamically linked against the MFC
// DLLs, any functions exported from this DLL which
// call into MFC must have the AFX_MANAGE_STATE macro
// added at the very beginning of the function.
//
// For example:
//
// extern "C" BOOL PASCAL EXPORT ExportedFunction()
// {
// AFX_MANAGE_STATE(AfxGetStaticModuleState());
// // normal function body here
// }
//
// It is very important that this macro appear in each
// function, prior to any calls into MFC. This means that
// it must appear as the first statement within the
// function, even before any object variable declarations
// as their constructors may generate calls into the MFC
// DLL.
BEGIN_MESSAGE_MAP(CMyRegularDllApp, CWinApp)
//{{AFX_MSG_MAP(CMyRegularDllApp)
// NOTE - the ClassWizard will add
// and remove mapping macros here.
// DO NOT EDIT what you see in these blocks
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////
// CMyRegularDllApp construction
CMyRegularDllApp::CMyRegularDllApp()
{
// TODO: add construction code here,
// Place all significant initialization in InitInstance
}
以上是AppWizard产生的含有主要代码的两个文件，各位可从中
看出和Non-MFC Dlls的区别。但要注意上面的AppWizard的提醒啊。
发信人: dragon (龙), 信区: VC
标 题: 关于VC中的DLL的编程[4]
发信站: 饮水思源站 (Thu Mar 25 00:46:22 1999) , 站内信件
这次要讲的是最后一种动态连接库：Extension Dlls.再次说明，
Extension Dll只被用MFC类库所编写的应用程序所调用.在这种动态
连接库中，你可以从MFC继承你所想要的、更适于你自己用的类，并
把它提供给你的应用程序。你也可随意的给你的应用程序提供MFC或
MFC继承类的对象指针。
Extension DLLs 和Regular DLLs不一样，它没有一个从CWinApp
继承而来的类的对象，所以，你必须为自己DllMain函数添加初始化
代码和结束代码.如下:
#include "stdafx.h"
#include 
static AFX_EXTENSION_MODULE PROJNAMEDLL = { NULL, NULL };
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
if (dwReason == DLL_PROCESS_ATTACH)
{
TRACE0("PROJNAME.DLL Initializing!/n");
// Extension DLL one-time initialization
AfxInitExtensionModule(PROJNAMEDLL,
hInstance);
// Insert this DLL into the resource chain
new CDynLinkLibrary(Dll3DLL);
}
else if (dwReason == DLL_PROCESS_DETACH)
{
TRACE0("PROJNAME.DLL Terminating!/n");
}
return 1; // ok
}
在上面代码中AfxInitExtensionMoudle函数捕捉此动态库模块
用.
在初始化的时NEW一个CDynLinkLibrary对象的目的在于：它
能是Extension DLL想应用程序输出CRuntimeClass对象或资源.
如果此动态连接库被显式的调用，还必须在DLL_PROCESS_DETACH
选择项的执行代码上调用AfxTermEXtensonModule，这保证了当调
用进程与动态连接库分离是正确清理内存中的动态库模块。如果是
隐式的被调用，则此步不是必须的了。 
**作者Blog：**[http://blog.csdn.net/aocanghai/](http://blog.csdn.net/aocanghai/)
引用:http://dev.csdn.net/Develop/article/19/19966.shtm
