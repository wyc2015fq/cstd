# dll封装 - 三少GG - CSDN博客
2013年03月04日 22:43:46[三少GG](https://me.csdn.net/scut1135)阅读数：839

视频:孙鑫VC视频教程 动态链接库[http://v.youku.com/v_show/id_XMjE5MDMwODY0.html](http://v.youku.com/v_show/id_XMjE5MDMwODY0.html)
## [VS2010中 C++创建DLL图解.](http://hi.baidu.com/lipeiyi2006/item/df1d035734472d9509be17c0)
**一、DLL的创建**
创建项目: Win32->Win32项目，名称：MyDLL
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/ef662ad9f2d3572c6d1dd6958a13632763d0c330.jpg)
选择DLL (D) ->完成.
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/e2bf60381f30e924c834d6784c086e061c95f71b.jpg)
1、新建头文件**testdll.htestdll.h代码如下：**
```
```cpp
#ifndef TestDll_H_
```
```cpp
#define TestDll_H_
```
```cpp
#ifdef MYLIBDLL
```
```cpp
#define MYLIBDLL extern "C" _declspec(dllimport)
```
```cpp
#else
```
```cpp
#define MYLIBDLL extern "C" _declspec(dllexport)
```
```cpp
#endif
```
```cpp
MYLIBDLL
```
```cpp
int
```
```cpp
Add(
```
```cpp
int
```
```cpp
plus1,
```
```cpp
int
```
```cpp
plus2);
```
```cpp
//You can also write like this:
```
```cpp
//extern "C" {
```
```cpp
//_declspec(dllexport) int Add(int plus1, int plus2);
```
```cpp
//};
```
```cpp
#endif
```
```
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/bc59756d55fbb2fbc8bb125b4f4a20a44723dc1b.jpg)
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/cb6aebcc7cd98d10ce564678213fb80e7aec9035.jpg)
2、新建源文件**testdll.cpptestdll.cpp代码如下：**
```
```cpp
#include "stdafx.h"
```
```cpp
#include "testdll.h"
```
```cpp
#include <iostream>
```
```cpp
using
```
```cpp
namespace
```
```cpp
std;
```
```cpp
int
```
```cpp
Add(
```
```cpp
int
```
```cpp
plus1,
```
```cpp
int
```
```cpp
plus2)
```
```cpp
{
```
```cpp
int
```
```cpp
add_result = plus1 + plus2;
```
```cpp
return
```
```cpp
add_result;
```
```cpp
}
```
```
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/df9e65f5e0fe9925528e231934a85edf8cb17135.jpg)
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/9371efefce1b9d161e57d028f3deb48f8d54641b.jpg)
3、新建模块定义文件**mydll.defmydll.def代码如下：**
```
```cpp
LIBRARY
```
```cpp
"MyDLL"
```
```cpp
EXPORTS
```
```cpp
Add @1
```
```
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/79359958ccbf6c817fa766a1bc3eb13532fa401b.jpg)
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/957873c79f3df8dcc6aec5d8cd11728b4610281b.jpg)
4、vs2010自动创建dllmain.cpp文件，它定义了DLL 应用程序的入口点。
dllmain.cpp代码如下：
// dllmain.cpp : 定义 DLL 应用程序的入口点。
```
```cpp
#include "stdafx.h"
```
```cpp
BOOL
```
```cpp
APIENTRY DllMain(
```
```cpp
HMODULE
```
```cpp
hModule,
```
```cpp
```
```cpp
DWORD
```
```cpp
ul_reason_for_call,
```
```cpp
```
```cpp
LPVOID
```
```cpp
lpReserved
```
```cpp
```
```cpp
)
```
```cpp
{
```
```cpp
switch
```
```cpp
(ul_reason_for_call)
```
```cpp
{
```
```cpp
case
```
```cpp
DLL_PROCESS_ATTACH:
```
```cpp
case
```
```cpp
DLL_THREAD_ATTACH:
```
```cpp
case
```
```cpp
DLL_THREAD_DETACH:
```
```cpp
case
```
```cpp
DLL_PROCESS_DETACH:
```
```cpp
```
```cpp
break
```
```cpp
;
```
```cpp
}
```
```cpp
return
```
```cpp
TRUE;
```
```cpp
}
```
```
最后，编译生成MyDLL.dll文件和MyDLL.lib文件。
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/4175351ed21b0ef4327bd217ddc451da80cb3e1b.jpg)
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/55c9828065380cd72c1b17a4a144ad34588281ae.jpg)
1>------ 已启动生成: 项目: MyDLL, 配置: Debug Win32 ------
1>  dllmain.cpp
========== 生成: 成功 1 个，失败 0 个，最新 0 个，跳过 0 个 ==========
1>------ 已启动生成: 项目: MyDLL, 配置: Debug Win32 ------
1>  stdafx.cpp
1>  testdll.cpp
1>  MyDLL.cpp
1>  正在生成代码...
1>     正在创建库 D:\Visual C++\工程\Libaray\MyDLL\Debug\MyDLL.lib 和对象 D:\Visual C++\工程\Libaray\MyDLL\Debug
![](http://hiphotos.baidu.com/lipeiyi2006/pic/item/f468e1ec8a1363271db5777a918fa0ec09fac753.jpg)

今天教大家在**[VC](http://dev.yesky.com/ysdevvc)中创建DLL文件**的方法步骤，一起来学习一下，并不是很难的，相信聪明的你一看就会。
**一.Win32动态链接库**
**1.制作的步骤：**
　　(1)新建WIN32 [Dynamic](http://www.mydown.com/soft/2/2896.html)-link Library工程，工程名为MyDll，选择A simple DLL project类型。
　　(2)MyDll.h的内容如下：
以下是引用片段：
　　extern "C" _declspec(dllexport) int sum(int a,int b);//本文所有的例子只有一个sum即加法函数。
　　(3)MyDll.cpp的内容如下:
以下是引用片段：
　　#include "stdafx.h" 
　　#include "windows.h" 
　　#include "MyDll.h" 
　　BOOL APIENTRY DllMain( 
　　HANDLE hModule, 
　　DWORD ul_reason_for_call, 
　　LPVOID lpReserved 
　　) 
　　{ 
　　return TRUE; 
　　} 
　　extern "C" _declspec(dllexport)int sum(int a, int b) 
　　{ 
　　return a+b; 
　　}
　　(4)编译之后产生了MyDll.lib与MyDll.dll两个文件。
**2.使用方法：**
　　(1).隐式调用法： 将MyDll.lib和MyDll.h拷贝到需要应用该DLL的工程的目录下，将MyDll.dll拷贝到产生的应用程序的目录下，并在需要应用该DLL中的函数的CPP文件中添加如下几行：
以下是引用片段：
　　#include "MyDll.h" 
　　#pragma comment(lib,"MyDll");
　　(2).显示调用法：将MyDll.lib和MyDll.h拷贝到需要应用该DLL的工程的目录下，将MyDll.dll拷贝到产生的应用程序的目录下，并在需要应用该DLL中的函数的CPP文件中包含头文件，如：
以下是引用片段：
　　#include "MyDll.h"
　　同时还需要在Project->Setting->Link->Object/library modules的框中增加MyDll.lib这个库。
**二.MFC动态链接库**
**1.制作的步骤：**
　　(1)新建MFC AppWizard(dll)工程，工程名为MFCDll，选择Regular DLL using shared MFC DLL类型。
　　(2)在生成的MFCDll.cpp文件后面增加下面几行：
以下是引用片段：
　　int sum(int a, int b) 
　　{ 
　　return a+b; 
　　}
　　(3)在生成的MFCDll.def文件后面增加如下：
以下是引用片段：
　　sum @1 ;表示第一个函数是sum
　　(4)编译后会产生两个文件MFCDll.lib,MFCDll.dll
**2.使用方法**
　　(1)隐式调用法： 将MFCDll.lib拷贝到需要应用该DLL的工程的目录下，将MyDll.dll拷贝到产生的应用程序的目录下，并在需要应用该DLL中的函数的CPP文件中添加如下几行：
　　//注意这里没有在MFCDll.h中声明函数，所以不能直接包含MFCDll.h来声明函数。
以下是引用片段：
　　#pragma comment(lib,"MFCDll"); 
　　int sum(int a, int b);
　　//当然如果你的DLL中有很多函数，那可以另外写个MFCDll.h，包含所有的函数声明，然后直接将头文件包含进去
　　(2)显示调用法：与Win32的调用方法一样，不需要#pragma comment(lib,"MFCDll");，但是需要在Project->Setting->Link->Object/library modules的框中增加MFCDll.lib这个库。
DLL文件与exe文件的区别：
动态链接库（Dynamic Link Library，缩写为DLL）
是一个可以被其它应用程序共享的程序模块，其中封装了一些可以被共享的例程和资源。动态链接库文件的扩展名一般是dll，也有可能是drv、sys和fon，它和可执行文件（exe）非常类似.
区别
DLL中虽然包含了可执行代码却不能单独执行，而应由Windows应用程序直接或间接调用。 
动态链接是相对于静态链接而言的。所谓静态链接是指把要调用的函数或者过程链接到可执行文件中，成为可执行文件的一部分。换句话说，函数和过程的代码就在程序的exe文件中，该文件包含了运行时所需的全部代码。当多个程序都调用相同函数时，内存中就会存在这个函数的多个拷贝，这样就浪费了宝贵的内存资源。
而动态链接所调用的函数代码并没有被拷贝到应用程序的可执行文件中去，而是仅仅在其中加入了所调用函数的描述信息（往往是一些重定位信息）。
仅当应用程序被装入内存开始运行时，在Windows的管理下，才在应用程序与相应的DLL之间建立链接关系。当要执行所调用DLL中的函数时，根据链接产生的重定位信息，Windows才转去执行DLL中相应的函数代码
### [初识dll，在VS2010平台上创建并使用dll（revised）](http://blog.csdn.net/btwsmile/article/details/6676802)
        分类：            [VC++](http://blog.csdn.net/btwsmile/article/category/1283472)[组件技术](http://blog.csdn.net/btwsmile/article/category/1283482)2011-08-10
 14:148069人阅读[评论](http://blog.csdn.net/btwsmile/article/details/6676802#comments)(16)[收藏]()[举报](http://blog.csdn.net/btwsmile/article/details/6676802#report)
**一、为什么需要dll**
代码复用是提高软件开发效率的重要途径。一般而言，只要某部分代码具有通用性，就可将它构造成相对独立的功能模块并在之后的项目中重复使用。比较常见的例子是各种应用程序框架，如ATL、MFC等，它们都以源代码的形式发布。由于这种复用是“源码级别”的，源代码完全暴露给了程序员，因而称之为“白盒复用”。“白盒复用”的缺点比较多，总结起来有4点。
- 暴露了源代码；
- 容易与程序员的“普通”代码发生命名冲突；
- 多份拷贝，造成存储浪费；
- 更新功能模块比较困难。
实际上，以上4点概括起来就是“暴露的源代码”造成“代码严重耦合”。为了弥补这些不足，就提出了“二进制级别”的代码复用。使用二进制级别的代码复用一定程度上隐藏了源代码，对于缓解代码耦合现象起到了一定的作用。这样的复用被称为“黑盒复用”。
在Windows操作系统中有两种可执行文件，其后缀名分别为.exe和.dll。它们的区别在于，.exe文件可被独立的装载于内存中运行；.dll文件却不能，它只能被其它进程调用。然而无论什么格式，它们都是二进制文件。上面说到的“二进制级别”的代码复用，可以使用.dll来实现。
与白盒复用相比，.dll很大程度上弥补了上述4大缺陷。.dll是二进制文件，因此隐藏了源代码；如果采用“显式调用”（后边将会提到），一般不会发生命名冲突；由于.dll是动态链接到应用程序中去的，它并不会在链接生成程序时被原原本本拷贝进去；.dll文件相对独立的存在，因此更新功能模块是可行的。
说明：实现“黑盒复用”的途径不只dll一种，静态链接库甚至更高级的COM组件都是。本文只对dll进行讨论。
**二、创建dll**
接下来用一个简单的例子来说明创建dll的方法。本例采用VS2010，使用C++编程语言，具体操作步骤如下。
通过Start Page或者File菜单栏，新建一个Project，将会弹出新建项目对话框。选择Win32 Project向导，项目名为CreateDLL，解决方案名为DLLTEST（注意Create directories for solution是勾选上的），点击OK，接着点击Next，到Application Settings，**选择应用程序类型为dll，并勾选“Export Symbols”，**点击Finish。完成这一步之后，VS界面上左边的Solution Explorer中将会看到向导自动生成的文件列表，如图1所示。
![](http://hi.csdn.net/attachment/201108/10/0_1312978187lw5O.gif)
图1 wizard自动生成的文件列表
在VS界面的编辑窗口中，展示了自动生成的CreateDLL.cpp的代码。
**[cpp]**[view plain](http://blog.csdn.net/btwsmile/article/details/6676802#)[copy](http://blog.csdn.net/btwsmile/article/details/6676802#)[print](http://blog.csdn.net/btwsmile/article/details/6676802#)[?](http://blog.csdn.net/btwsmile/article/details/6676802#)
- // CreateDLL.cpp : Defines the exported functions for the DLL application.
- //
- 
- #include "stdafx.h"
- #include "CreateDLL.h"
- 
- 
- // This is an example of an exported variable
- CREATEDLL_API int nCreateDLL = 0;  
- 
- // This is an example of an exported function.
- CREATEDLL_API int fnCreateDLL(void)  
- {  
- return 42;  
- }  
- 
- // This is the constructor of a class that has been exported.
- // see CreateDLL.h for the class definition
- CCreateDLL::CCreateDLL()  
- {  
- return;  
- }  
```cpp
// CreateDLL.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "CreateDLL.h"
// This is an example of an exported variable
CREATEDLL_API int nCreateDLL = 0;
// This is an example of an exported function.
CREATEDLL_API int fnCreateDLL(void)
{
	return 42;
}
// This is the constructor of a class that has been exported.
// see CreateDLL.h for the class definition
CCreateDLL::CCreateDLL()
{
	return;
}
```
这里有3种类型的example，分别为导出变量nCreateDLL、导出函数fnCreateDLL以及导出类CCreateDLL。为了简化起见，本例只考虑导出函数。修改CreateDLL.h文件为：
**[cpp]**[view plain](http://blog.csdn.net/btwsmile/article/details/6676802#)[copy](http://blog.csdn.net/btwsmile/article/details/6676802#)[print](http://blog.csdn.net/btwsmile/article/details/6676802#)[?](http://blog.csdn.net/btwsmile/article/details/6676802#)
- #ifdef CREATEDLL_EXPORTS
- #define CREATEDLL_API __declspec(dllexport)
- #else
- #define CREATEDLL_API __declspec(dllimport)
- #endif
- 
- CREATEDLL_API void printMax(int&,int&);  
- CREATEDLL_API void printMax(int&,int&,int&);  
```cpp
#ifdef CREATEDLL_EXPORTS
#define CREATEDLL_API __declspec(dllexport)
#else
#define CREATEDLL_API __declspec(dllimport)
#endif
CREATEDLL_API void printMax(int&,int&);
CREATEDLL_API void printMax(int&,int&,int&);
```
修改CreateDLL.cpp文件为： 
**[cpp]**[view plain](http://blog.csdn.net/btwsmile/article/details/6676802#)[copy](http://blog.csdn.net/btwsmile/article/details/6676802#)[print](http://blog.csdn.net/btwsmile/article/details/6676802#)[?](http://blog.csdn.net/btwsmile/article/details/6676802#)
- CREATEDLL_API void printMax(int& a,int& b)  
- {  
-     std::cout<<"Among ("<<a<<","<<b<<"), the Max Number is "<<(a>b?a:b)<<"\n";  
- }  
- CREATEDLL_API void printMax(int& a,int& b,int& c)  
- {  
-     std::cout<<"Among ("<<a<<","<<b<<","<<c<<"), the Max Number is "<<(((a>b?a:b)>c)?(a>b?a:b):c)<<"\n";  
- }  
```cpp
CREATEDLL_API void printMax(int& a,int& b)
{
	std::cout<<"Among ("<<a<<","<<b<<"), the Max Number is "<<(a>b?a:b)<<"\n";
}
CREATEDLL_API void printMax(int& a,int& b,int& c)
{
	std::cout<<"Among ("<<a<<","<<b<<","<<c<<"), the Max Number is "<<(((a>b?a:b)>c)?(a>b?a:b):c)<<"\n";
}
```
不难发现，printMax函数的作用就是打印出两个整数或三个整数中的最大值。需要说明的是，这里故意使用同名函数是为了引出导出函数的修饰名称，具体将在第四节中阐述。
接下来，选择菜单Build->Build CreateDLL，Output窗口提示CreateDLL.dll文件生成成功，如图2所示。
![](http://hi.csdn.net/attachment/201108/10/0_1312979405lb3m.gif)
图2 CreateDLL.dll成功生成
**三、使用dll**
本例采用“显式调用”的方式使用CreateDLL.dll。显式调用方式相比于”隐式调用“有好有坏。显式调用只需要一个.dll文件就可以了，灵活性更好，更新模块方便；相对的，程序员需要做的事情更多，使用方法更为复杂。
右键单击Solution Explorer中的Solution 'DLLTEST'，在弹出的菜单中选择Add->New Project，选择Win32 Console Application，输入项目名为UseDLL，点击OK，接着点击Next，在Application Settings界面勾选EmptyProject并点击Finish。右键单击项目UseDLL，给它添加源文件UseDLL.cpp。这样操作之后，Solution
 Explorer的信息如图3所示。
![](http://hi.csdn.net/attachment/201108/10/0_13129807837k0O.gif)
图3 向Solution'DLLTEST'添加项目UseDLL
编写UseDLL.cpp的代码为：
**[cpp]**[view plain](http://blog.csdn.net/btwsmile/article/details/6676802#)[copy](http://blog.csdn.net/btwsmile/article/details/6676802#)[print](http://blog.csdn.net/btwsmile/article/details/6676802#)[?](http://blog.csdn.net/btwsmile/article/details/6676802#)
- /*--UseDLL.cpp
-  *Author: ume(李优米)
-  *Use CreateDLL.dll explicitly
-  */
- #include<Windows.h>
- #include<iostream>
- typedefvoid(*FUNA)(int&,int&);  
- typedefvoid(*FUNB)(int&,int&,int&);  
- int main()  
- {  
- constchar* dllName = "CreateDLL.dll";  
- constchar* funName1 = "printMax";  
- constchar* funName2 = "printMax";  
- int x(100), y(100), z(100);  
- HMODULE hDLL = LoadLibrary(dllName);  
- if(hDLL != NULL)  
-     {  
-         FUNA fp1 = FUNA(GetProcAddress(hDLL,funName1));  
- if(fp1 != NULL)  
-         {  
-             std::cout<<"Input 2 Numbers:";  
-             std::cin>>x>>y;  
-             fp1(x,y);  
-         }  
- else
-         {  
-             std::cout<<"Cannot Find Function "<<funName1<<std::endl;  
-         }  
-         FUNB fp2 = FUNB(GetProcAddress(hDLL,funName2));  
- if(fp2 != NULL)  
-         {  
-             std::cout<<"Input 3 Numbers:";  
-             std::cin>>x>>y>>z;  
-             fp2(x,y,z);  
-         }  
- else
-         {  
-             std::cout<<"Cannot Find Function "<<funName2<<std::endl;  
-         }  
-         FreeLibrary(hDLL);  
-     }  
- else
-     {  
-         std::cout<<"Cannot Find "<<dllName<<std::endl;  
-     }  
- return 1;  
- }  
```cpp
/*--UseDLL.cpp
 *Author: ume(李优米)
 *Use CreateDLL.dll explicitly
 */
#include<Windows.h>
#include<iostream>
typedef void(*FUNA)(int&,int&);
typedef void(*FUNB)(int&,int&,int&);
int main()
{
	const char* dllName = "CreateDLL.dll";
	const char* funName1 = "printMax";
	const char* funName2 = "printMax";
	int x(100), y(100), z(100);
	HMODULE hDLL = LoadLibrary(dllName);
	if(hDLL != NULL)
	{
		FUNA fp1 = FUNA(GetProcAddress(hDLL,funName1));
		if(fp1 != NULL)
		{
			std::cout<<"Input 2 Numbers:";
			std::cin>>x>>y;
			fp1(x,y);
		}
		else
		{
			std::cout<<"Cannot Find Function "<<funName1<<std::endl;
		}
		FUNB fp2 = FUNB(GetProcAddress(hDLL,funName2));
		if(fp2 != NULL)
		{
			std::cout<<"Input 3 Numbers:";
			std::cin>>x>>y>>z;
			fp2(x,y,z);
		}
		else
		{
			std::cout<<"Cannot Find Function "<<funName2<<std::endl;
		}
		FreeLibrary(hDLL);
	}
	else
	{
		std::cout<<"Cannot Find "<<dllName<<std::endl;
	}
	return 1;
}
```
代码比较长，但是并不难理解，这里仅说明代码中的一些要点。
- 包含头文件Windows.h，原因在于程序中用到了LoadLibrary、FreeLibrary、GetProcAddress等Win32 API函数。
- FUNA和FUNB是函数指针类型的声明。
- 当程序不再使用dll时，应该调用FreeLibrary及时释放它占用的内存空间。
- 如果在const char* dllName和funName底部出现红色波浪线提示，说明采用的字符集不匹配，需要修改项目UseDLL的属性CharaterSet为Not Set。
- 为方便项目的调试，建议修改解决方案的Startup Project属性为Single startup project并以UseDLL为首选。
然而，这个程序还有错误。编译并运行，结果如图4所示。
![](http://hi.csdn.net/attachment/201108/10/0_1312982369pYNs.gif)
图4 UseDLL的运行结果
这并不是期望中的结果。实际上，正如第二节提到的那样，造成这种错误的原因正是导出函数的修饰名称。虽然在CreateDLL.cpp中两个printMax函数有相同的名称，但在dll二进制文件中，经过编译器的“加工”，它们实际上各自有不同的名称了。这也是函数重载机制得以实现的一个技术支持。
使用VS2010附带工具dumpbin，查看CreateDLL.dll的导出函数名，结果如图5所示。
![](http://hi.csdn.net/attachment/201108/10/0_1312982813H5iR.gif)
图5 查看CreateDLL.dll的导出函数名
观察图5可以发现，CreateDLL.dll导出函数名为?printMax@@YAXAAH00@Z和?printMax@@YAXAAH0@Z。它们分别对应着三个整数的printMax和两个整数的printMax。因此，Use.DLL中funName应当相应修改为：
**[cpp]**[view plain](http://blog.csdn.net/btwsmile/article/details/6676802#)[copy](http://blog.csdn.net/btwsmile/article/details/6676802#)[print](http://blog.csdn.net/btwsmile/article/details/6676802#)[?](http://blog.csdn.net/btwsmile/article/details/6676802#)
- constchar* funName1 = "?printMax@@YAXAAH0@Z";  
- constchar* funName2 = “?printMax@@YAXAAH00@Z”;  
```cpp
const char* funName1 = "?printMax@@YAXAAH0@Z";
const char* funName2 = “?printMax@@YAXAAH00@Z”;
```
修改之后，再次编译运行，结果正确，如图6所示。
![](http://hi.csdn.net/attachment/201108/10/0_1312983332XOzt.gif)
 图6 UseDLL正常运行
**四、dll导出函数名称规范化**
创建、使用dll并不复杂，走过前三节，相信读者肯定有这样的体会。然而，一个问题仍然值得思考：导出函数的修饰名称太“奇怪”，为dll的使用带来了不便，能不能让导出函数的修饰名称规范一些？
答案是肯定的，而且方法至少有两种：一是运用extern "C"修饰printMax；二是运用模块定义文件.def。后者的效果更好，所以本节将使用.def来规范化导出函数的修饰名称。
CreateDLL.dll导出的两个函数功能很简单，根据功能描述，理想的函数名称是pMaxA2和pMaxA3。在CreateDLL项目中添加CreateDLL.def文件：
**[cpp]**[view plain](http://blog.csdn.net/btwsmile/article/details/6676802#)[copy](http://blog.csdn.net/btwsmile/article/details/6676802#)[print](http://blog.csdn.net/btwsmile/article/details/6676802#)[?](http://blog.csdn.net/btwsmile/article/details/6676802#)
- LIBRARY CreateDLL  
- EXPORTS  
- pMaxA2 = ?printMax@@YAXAAH0@Z  
- pMaxA3 = ?printMax@@YAXAAH00@Z  
```cpp
LIBRARY CreateDLL
EXPORTS
pMaxA2 = ?printMax@@YAXAAH0@Z
pMaxA3 = ?printMax@@YAXAAH00@Z
```
重新build项目CreateDLL，使用dumpbin再次查看CreateDLL.dll的导出函数名称，结果如图7所示。
![](http://hi.csdn.net/attachment/201108/10/0_1312984479zzss.gif)
图7 规范化的函数名，奇怪的修饰名称还存在
出现了期望的结果，但仍有小缺憾：奇怪的修饰名称仍然存在。能否去掉这些不太规范的修饰名称呢？当然是可以的。只需要将CreateDLL.h中#define CREATEDLL_API __declspec(dllexport) 修改为#define CREATEDLL_API即可。修改之后重新编译生成CreateDLL.dll，使用dumpbin查看导出函数名称，结果如图8所示。
![](http://hi.csdn.net/attachment/201108/10/0_13129847632P7E.gif)
图8 规范化的函数名，去除了奇怪的修饰名称    
回到UseDLL.cpp，修改funName：
**[cpp]**[view plain](http://blog.csdn.net/btwsmile/article/details/6676802#)[copy](http://blog.csdn.net/btwsmile/article/details/6676802#)[print](http://blog.csdn.net/btwsmile/article/details/6676802#)[?](http://blog.csdn.net/btwsmile/article/details/6676802#)
- constchar* funName1 = "pMaxA2";  
- constchar* funName2 = "pMaxA3";  
```cpp
const char* funName1 = "pMaxA2";
const char* funName2 = "pMaxA3";
```
重新编译运行UseDLL，结果正确，与图6类似。
**五、dll的不足**
动态链接库虽然一定程度上实现了“黑盒复用”，但仍存在着诸多不足，笔者能够想到的有下面几点。
- dll节省了编译期的时间，但相应延长了运行期的时间，因为在使用dll的导出函数时，不但要加载dll，而且程序将会在模块间跳转，降低了cache的命中率。
- **若采用隐式调用，仍然需要.h、.lib、.dll文件（“三件套”），并不能有效支持模块的更新。**
- **显式调用虽然很好地支持模块的更新，但却不能导出类和变量。**
- dll不支持Template。
二进制级别的代码复用相比源码级别的复用已经有了很大的进步，但在二进制级别的代码复用中，dll显得太古老。想真正完美实现跨平台、跨语言的黑盒复用，采用COM才是正确的选择。
