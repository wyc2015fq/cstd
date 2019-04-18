# 将自己的类封装为lib的方法 - wishchinYang的专栏 - CSDN博客
2013年09月11日 16:05:38[wishchin](https://me.csdn.net/wishchin)阅读数：717
个人分类：[MFC编程																[C++编程																[C+/代码迁移](https://blog.csdn.net/wishchin/article/category/1552163)](https://blog.csdn.net/wishchin/article/category/1508333)](https://blog.csdn.net/wishchin/article/category/1508409)
**前言：**
Windows API中所有的函数都包含在dll中，其中有3个最重要的DLL。
(1)   Kernel32.dll 它包含那些用于管理内存、进程和线程的函数，例如CreateThread函数；
(2)   User32.dll   它包含那些用于执行用户界面任务(如窗口的创建和消息的传送)的函数，例如CreateWindow函数；
(3)   GDI32.dll    它包含那些用于画图和显示文本的函数。
静态库和动态库
(1)   静态库
         函数和数据被编译进一个二进制文件(通常扩展名为.LIB)。在使用静态库的情况下，在编译链接可执行文件时，链接器从库中复制这些函数和数据并把它们和应用程序的其他模块组合起来创建最终的可执行文件(.Exe文件).当发布产品时，只需要发布这个可执行文件，并不需要发布被使用的静态库。
(2)   动态库
      在使用动态库的时候，往往提供两个文件：一个引入库(.lib)文件和一个DLL(.dll)文件。虽然引入库的后缀名也是”lib”，但是动态库的引入库文件和静态库文件有着本质上的区别，对一个DLL来说，其引入库文件(.lib)包含该DLL导出的函数和变量的符号名，而.dll文件包含该DLL实际的函数和数据。在使用动态库的情况下，在编译链接可执行文件时，只需要链接该DLL的引入库文件，该DLL中的函数代码和数据并不复制到可执行文件中，直到可执行程序运行时，才去加载所需的DLL，将该DLL映射到进程的地址空间外，然后访问DLL中导出的函数。这时，发布产品时，除了发布可执行文件以外，同时还要发布该程序将要调用的动态链接库。
**一、DLL的创建 **
（1）：方法引自于此文: [http://blog.csdn.net/wqvbjhc/article/details/6029168](http://blog.csdn.net/wqvbjhc/article/details/6029168)
              对于图像缺失问题，请访问原文链接.
（2）：创建DLL 此文解释已经相当详细，地址：[http://hi.baidu.com/lipeiyi2006/item/df1d035734472d9509be17c0](http://hi.baidu.com/lipeiyi2006/item/df1d035734472d9509be17c0)
  不过为了防止以后博主删除，还是copy一遍....、
创建项目: Win32->Win32项目，名称：MyDLL
选择DLL (D) ->完成.
1、新建头文件**testdll.htestdll.h代码如下：**
```cpp
#ifndef TestDll_H_
#define TestDll_H_
#ifdef MYLIBDLL
#define MYLIBDLL extern "C" _declspec(dllimport) 
#else
#define MYLIBDLL extern "C" _declspec(dllexport) 
#endif
MYLIBDLL int Add(int plus1, int plus2);
//You can also write like this:
//extern "C" {
//_declspec(dllexport) int Add(int plus1, int plus2);
//};
#endif
```
Ps：__declspec(dllexport)的作用，它就是为了省掉在DEF文件中手工定义导出哪些函数的一个方法。当然，如果你的DLL里全是C++的类的话，你无法在DEF里指定导出的函数，只能用__declspec(dllexport)导出类。
*注：extern "C"是可选的，extern "C"是为了保证导出的DLL函数名不发生变化。如果写了extern "C"，那么导出和导入都要写，要保证一致。
如果要导出的是类，不能在类名前加extern "C"，切记切记！！编译不会通过的。
如果在新建dll的时候选择空白的文件（即不用vs自带的帮你生成）的话，注意一定要自己手动添加def文件。否则生成不了lib文件。
2、新建源文件**testdll.cpp**
```cpp
#include "stdafx.h"
#include "testdll.h"
#include <iostream>
using namespace std;
int Add(int plus1, int plus2)
{
int add_result = plus1 + plus2;
return add_result;
}
```
3、新建模块定义文件**mydll.defmydll.def代码如下：**LIBRARY "MyDLL"EXPORTSAdd @1
PS：这个在VS2012中可能没有，所以要这样操作：
方法是：在所建工程上单击鼠标右键，在弹出的右键菜单中选择“添加-->新建项....---->模块定义文件”，在该模块定义文件中写导出函数表，单击确定
**Vs2012默认生成dll，但不生成Lib文件。这个.def文件可以生成lib文件。**
4、vs2010自动创建dllmain.cpp文件，它定义了DLL 应用程序的入口点。
dllmain.cpp代码如下：
// dllmain.cpp : 定义 DLL 应用程序的入口点。
```cpp
#include "stdafx.h"
BOOL APIENTRY DllMain( HMODULE hModule,
              DWORD  ul_reason_for_call,
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
```
最后，编译生成MyDLL.dll文件和MyDLL.lib文件。
```cpp
1>------ 已启动生成: 项目: MyDLL, 配置: Debug Win32 ------
1>  dllmain.cpp
========== 生成: 成功 1 个，失败 0 个，最新 0 个，跳过 0 个 ==========
1>------ 已启动生成: 项目: MyDLL, 配置: Debug Win32 ------
1>  stdafx.cpp
1>  testdll.cpp
1>  MyDLL.cpp
1>  正在生成代码...
1>     正在创建库 D:\Visual C++\工程\Libaray\MyDLL\Debug\MyDLL.lib 和对象 D:\Visual C++\工程\Libaray\MyDLL\Debug
Ps：errorc2061 语法错误 标识符
```
**错误原因：**是因为头文件的顺序不对。
**解决方法：**调整头文件的顺序。
**转自：**[http://liuyunfeng484.blog.163.com/blog/static/66831715201232810449824/](http://liuyunfeng484.blog.163.com/blog/static/66831715201232810449824/)
**二、DLL的使用（静态链接、隐式链接）**
## 如何在C++中调用DLL中的函数
　　应用程序使用DLL可以采用两种方式：一种是隐式链接，另一种是显式链接。在使用DLL之前首先要知道DLL中函数的结构信息。Visual C++6.0在VC\bin目录下提供了一个名为Dumpbin.exe的小程序，用它可以查看DLL文件中的函数结构。另外，Windows系统将遵循下面的搜索顺序来定位DLL： 1．包含EXE文件的目录，2．进程的当前工作目录， 3．Windows系统目录， 4．Windows目录，5．列在Path环境变量中的一系列目录。
1．隐式链接
　　隐式链接就是在程序开始执行时就将DLL文件加载到应用程序当中。实现隐式链接很容易，只要将导入函数关键字_declspec(dllimport)函数名等写到应用程序相应的头文件中就可以了。下面的例子通过隐式链接调用MyDll.dll库中的Min函数。首先生成一个项目为TestDll，在DllTest.h、DllTest.cpp文件中分别输入如下代码：
```cpp
//Dlltest.h
#pragma comment(lib，"MyDll.lib")
extern"C"_declspec(dllimport) int Max(int a,int b);
extern"C"_declspec(dllimport) int Min(int a,int b);
//TestDll.cpp
#include
#include"Dlltest.h"
void main()
{int a;
a=min(8,10)
printf("比较的结果为%d\n"，a);
}
```
　　在创建DllTest.exe文件之前，要先将MyDll.dll和MyDll.lib拷贝到当前工程所在的目录下面，也可以拷贝到windows的System目录下。如果DLL使用的是def文件，要删除TestDll.h文件中关键字extern "C"。TestDll.h文件中的关键字Progam commit是要Visual C+的编译器在link时，链接到MyDll.lib文件，当然，开发人员也可以不使用#pragma comment(lib，"MyDll.lib")语句，而直接在工程的Setting->Link页的Object/Moduls栏填入MyDll.lib既可。
2．显式链接
　　显式链接是应用程序在执行过程中随时可以加载DLL文件，也可以随时卸载DLL文件，这是隐式链接所无法作到的，所以显式链接具有更好的灵活性，对于解释性语言更为合适。不过实现显式链接要麻烦一些。在应用程序中用LoadLibrary或MFC提供的AfxLoadLibrary显式的将自己所做的动态链接库调进来，动态链接库的文件名即是上述两个函数的参数，此后再用GetProcAddress()获取想要引入的函数。自此，你就可以象使用如同在应用程序自定义的函数一样来调用此引入函数了。在应用程序退出之前，应该用FreeLibrary或MFC提供的AfxFreeLibrary释放动态链接库。下面是通过显式链接调用DLL中的Max函数的例子。
```cpp
#include
#include
void main(void)
{
typedef int(*pMax)(int a,int b);
typedef int(*pMin)(int a,int b);
HINSTANCE hDLL;
PMax Max
HDLL=LoadLibrary("MyDll.dll");//加载动态链接库MyDll.dll文件；
Max=(pMax)GetProcAddress(hDLL,"Max");
A=Max(5,8);
Printf("比较的结果为%d\n"，a);
FreeLibrary(hDLL);//卸载MyDll.dll文件；
}
```
　　在上例中使用类型定义关键字typedef，定义指向和DLL中相同的函数原型指针，然后通过LoadLibray()将DLL加载到当前的应用程序中并返回当前DLL文件的句柄，然后通过GetProcAddress()函数获取导入到应用程序中的函数指针，函数调用完毕后，使用FreeLibrary()卸载DLL文件。在编译程序之前，首先要将DLL文件拷贝到工程所在的目录或Windows系统目录下。
　　使用显式链接应用程序编译时不需要使用相应的Lib文件。另外，使用GetProcAddress()函数时，可以利用MAKEINTRESOURCE()函数直接使用DLL中函数出现的顺序号，如将GetProcAddress(hDLL,"Min")改为GetProcAddress(hDLL, MAKEINTRESOURCE(2))（函数Min()在DLL中的顺序号是2），这样调用DLL中的函数速度很快，但是要记住函数的使用序号，否则会发生错误。
[warning LNK4070](http://blog.csdn.net/clever101/article/details/5898073)：exp : warning LNK4070: .EXP中的
 /OUT:A.dll指令与输出文件名"../outdir/Debug/B.dll"不同；忽略指令
**产生原因是：**这个工程的def文件中LIBRARY字段的值和输出文件不一样造成的。
**解决办法：修改**LIBRARY字段的值，使得和输出文件保持一致。
### 转自：[http://blog.163.com/gost_008/blog/static/87202204200862932528217/](http://blog.163.com/gost_008/blog/static/87202204200862932528217/)
### 三：DLL的模块定义文件（.DEF）
动态连接库函数或者成员的导出可以用
_declspec(dllexport)来实现，比如为了导出voidSayHello()函数，则在DLL文件中这么声明（或者定义）：
_declspec(dllexport)void SayHello();
也可以不用_declspec(dllexport)，而采用.def文件来说明要导出的函数或成员：
.def文件的格式：
LIBRARYABC //ABC为.dll文件的名字，也就是dll工程的名字
EXPORTS
SayHello@1
这样，就从ABC.dll文件中导出了SayHello这个函数，
后面的那个"@1"的含义为：
从动态连接库文件中导出的函数或者成员可以用名字来标识，也可以用序号来表示，一般情况下用名字来标识，序号由系统来设置，但是也可以用"@1"的格式来自己定义，表示SayHello函数的序号为1
不想使用__declspec(dllexport)而单独使用.def文件导出类，不知道如何实现？1，把你要从dll输出的每个类，都设计一个基类，这是个纯虚，即所有函数都是“virtual...=0;”的。把这些写成一个.h；
  2，把dll里的类都从相应的基类派生，并实现每一个纯虚函数。并给dll设计一个可动态导出的函数，这个函数里   return   
 new    CxxxInDll;   
  3，在调用dll的程序里定义纯虚基类的指针，并赋予第二步返回的值。
现在可以动态加载、调用dll里的类了。看看COM原理，你就有启发了。熟练了后会发现这个方法在小型应用里非常有效！（在大型应用里，由于缺乏生存期控制，所以不安全）
需要补充的是：需要为每个类增加定义一个Release函数，函数里delete   this，不要在调用dll的程序里直接delete从第二步获得的指针。
**四：我的方法**
 对于Vs建立DLL文件的方法过程为：
（1）：建立一个空工程，命名
（2）：把工程生成文件设置为DLL
（3）：添加现有项目，把类里面对外展示的Public成员函数，在头文件函数声明前面添加_declspec(dllexport)
（4）：切记：要把构造函数和析构函数前面也加上_declspec(dllexport)
（5）：生成dll文件，在工程的Release或者Debug文件夹里面会生成DLL和LIb文件，然后整个项目.h文件，DLL和LIb文件 为最后需要的文件

