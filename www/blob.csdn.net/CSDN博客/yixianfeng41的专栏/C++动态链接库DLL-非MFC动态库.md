# C++动态链接库DLL-非MFC动态库 - yixianfeng41的专栏 - CSDN博客
2015年10月14日 19:46:43[第2梦](https://me.csdn.net/yixianfeng41)阅读数：1302
个人分类：[c++](https://blog.csdn.net/yixianfeng41/article/category/5618607)
所属专栏：[C++编程](https://blog.csdn.net/column/details/14514.html)
### 一、引言
     现在我们编程时，总会用到各种库文件，毫不夸张的说，没有库文件，我们编程做项目几乎寸步难行。而库文件一般又分为静态链接库(.lib)和动态链接库（.DLL)。如果采用静态链接库，则lib中的指令代码都将包含在最终生成的exe文件中。若是使用动态链接库，则其代码不会包含在EXE中，可以动态的装载和卸载DLL文件。静态链接库不讨论，只看看动态链接库。C++支持三种DLL,分别是非MFC动态链接库，MFC规则DLL,MFC扩展DLL,本篇讨论下非MFC动态库。
### 二、非MFC动态库
      非MFC动态链接库不采用MFC类库结构，其导出函数为标准的C接口，能被非MFC或MFC编写的应用程序所调用。
#### 1、声明导出函数
       DLL中导出函数声明有两种方式，一种是在函数声明中加上_declspec(dllexport);另一种是采用模块定义（.def)文件声明，.def文件为连接器提供了有关被链接程序的导出、属性及其他方面的信息。
###### 1.1、在函数声明中加上_declspec(dllexport)
      VS2012新建项目->c++->Win32控制台应用程序->DLL，建一个test.dll。
```cpp
//DLL的头文件test.h
#ifndef _H_TEST
#define _H_TEST
extern "C" int _declspec(dllexport)add(int x,int y);
#endif
//DLL的源文件test.cpp
#include "test.h"
int add(int x,int y)
{
	return x+y;
}
```
生成以下解决方案，Test.dll就生成好了。
接下来建一个控制台项目CallDll，调用前面那个dll。
```cpp
#include<iostream>
#include<Windows.h>
using namespace std;
typedef int(*AddFun) (int,int);
int main()
{
	HINSTANCE hDLL;
	AddFun af;
	hDLL=LoadLibrary(L"TestDll.dll");
	if(hDLL!=NULL)
	{
		af=(AddFun)GetProcAddress(hDLL,"add");
		if(af!=NULL)
		{
			int result=af(2,3);
			cout<<result;
		}
	}
	FreeLibrary(hDLL);
	return 0;
}
```
运行就是输出了5，调用成功。分析一下上述代码，建动态链接库时，我们在add方法前加了extern "c" 以及_declspec(export)。extern "c"无关紧要，就是说修饰的这个函数按照c语言的形式编译和链接。而_declspec(export),则表示这个函数为DLL的导出函数。在一个DLL中，有两种函数，一种是内部函数，一种是导出函数，内部函数只能在DLL内部使用，导出函数是其他程序可以调用的。在调用动态链接库的程序中，我们首先声明了一个跟add方法参数一样的函数指针，虽有在面中定义了函数一个函数指针的实例，然后调用loadlibrary方法获得DLL的句柄；再通过GetProcAddress得到加载的动态链接库中add的地址，然后使用这个方法。
###### 1.2、模块定义文件（.def)声明
在DLL中添加一个.def文件
```cpp
;test.def 
LIBRARY  TestDll
EXPORTS 
add @1
```
注意：1）.def文件中的注释由每个注释行开始处的分号指定，且注释不能与语句在一行。
             2）LIBRARY语句说明.def文件中相应的DLL.
             3)EXPORTS语句后列出要导出函数的名称，可以在.def文件中的导出函数后加上@n,表示要导出函数的序号为n。
#### 2、DLL的调用方式
      Dll的调用有两种方式，动态调用和静态调用。动态调用时完全由编程者用API函数加载和卸载DLL,程序员可以觉得dll文件何时加载，何时卸载; 静态调用是由编译系统完成对DLL的加载和卸载。前面的例子就用的是动态调用，下面我们来看看静态调用。
将生成好的动态链接库的dll以及lib文件拷到当前程序debug目录下。
```cpp
#include<iostream>
using namespace std;
#pragma comment(lib,"TestDll.lib")
extern "C" int _declspec(dllimport)add(int x,int y);
int main()
{
	int result=add(2,3);
	cout<<result;
	return 0;
}
```
静态链接需要注意：
1）程序在生成dll文件时，编译器会自动生成一个对应的lib文件。静态调用时要告诉编译器与DLL相对应的lib文件名与路径，#pragma comment(...)。
2）声明导入函数extern "C”int _declspec(dllimport)add(int x,int y);
程序根据lib中的信息加载dll.
#### 3、_stdcall约定
      如果通过c++编写的DLL欲被其他语言程序调用，应将导出函数以及调用函数声明为_stdcall。
动态链接库头文件，应该为:
```cpp
extern "C" int _declspec(dllexport) _stdcall add(int x,int y);
```
调用时：
```cpp
typedef int(_stdcall *AddFun) (int,int);
```
#### 4、导出全局变量和类
      导出全局变量和类与导出函数大同小异，因为用的少，也不讨论了！
