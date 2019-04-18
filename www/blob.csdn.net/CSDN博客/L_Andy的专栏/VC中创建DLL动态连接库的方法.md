# VC中创建DLL动态连接库的方法 - L_Andy的专栏 - CSDN博客

2012年10月15日 14:58:53[卡哥](https://me.csdn.net/L_Andy)阅读数：1454标签：[dll																[hook																[library																[callback																[mfc																[keyboard](https://so.csdn.net/so/search/s.do?q=keyboard&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=hook&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)


一.Win32动态链接库

　　1.制作的步骤：

　　(1)新建WIN32 Dynamic-link Library工程，工程名为MyDll，选择A simple DLL project类型。

　　(2)MyDll.h的内容如下：

　　extern "C" _declspec(dllexport) int sum(int a,int b);//本文所有的例子只有一个sum即加法函数。

　　(3)MyDll.cpp的内容如下:

　　#include "stdafx.h" 

　　#include "windows.h" 

　　#include "MyDll.h"

　　BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)

　　{ 

　　   return TRUE; 

　　} 

　　extern "C" _declspec(dllexport)int sum(int a, int b) 

　　{ 

　　   return a+b; 

　　}

　　(4)编译之后产生了MyDll.lib与MyDll.dll两个文件。

　　2.使用方法：

　　(1).隐式调用法： 将MyDll.lib和MyDll.h拷贝到需要应用该DLL的工程的目录下，将MyDll.dll拷贝到产生的应用程序的目录下，并在需要应用该DLL中的函数的CPP文件中添加如下几行：

　　#include "MyDll.h" 

　　#pragma comment(lib,"MyDll");

　　(2).显示调用法：将MyDll.lib和MyDll.h拷贝到需要应用该DLL的工程的目录下，将MyDll.dll拷贝到产生的应用程序的目录下，并在需要应用该DLL中的函数的CPP文件中包含头文件，如：

　　#include "MyDll.h" 

　　同时还需要在Project->Setting->Link->Object/library modules的框中增加MyDll.lib这个库。

二.MFC动态链接库

　　1.制作的步骤：

　　(1)新建MFC AppWizard(dll)工程，工程名为MFCDll，选择Regular DLL using shared MFC DLL类型。

　　(2)在生成的MFCDll.cpp文件后面增加下面几行：

　　int sum(int a, int b) 

　　{ 

　　 return a+b; 

　　}

　　(3)在生成的MFCDll.def文件后面增加如下：

　　sum @1 ;表示第一个导出函数是sum

　　(4)编译后会产生两个文件MFCDll.lib,MFCDll.dll

　　2.使用方法

　　(1)隐式调用法： 将MFCDll.lib拷贝到需要应用该DLL的工程的目录下，将MyDll.dll拷贝到产生的应用程序的目录下，并在需要应用该DLL中的函数的CPP文件中添加如下几行：

　　//注意这里没有在MFCDll.h中声明函数，所以不能直接包含MFCDll.h来声明函数。

　　#pragma comment(lib,"MFCDll"); 

　　int sum(int a, int b);

　　//当然如果你的DLL中有很多函数，那可以另外写个MFCDll.h，包含所有的函数声明，然后直接将头文件包含进去

　　(2)显示调用法：与Win32的调用方法一样，不需要#pragma comment(lib,"MFCDll");，但是需要在Project->Setting->Link->Object/library modules的框中增加MFCDll.lib这个库。

下面创建一个简单的Hook的Dll：

(1)新建WIN32 Dynamic-link Library工程，工程名为Hook，选择An empty DLL project类型。

(2)添加Hook.cpp文件：

#include <windows.h>

HHOOK g_hMouse=NULL;

HHOOK g_hKeyboard=NULL;

HWND g_hWnd=NULL;

LRESULT CALLBACK KeyboardProc(

int code,       // hook code

WPARAM wParam, // virtual-key code

LPARAM lParam   // keystroke-message information

)

{

if(VK_F2==wParam) //按F2，退出，解除钩子

{

   SendMessage(g_hWnd,WM_CLOSE,0,0);

   UnhookWindowsHookEx(g_hMouse);

   UnhookWindowsHookEx(g_hKeyboard);

}

return 1;

}

LRESULT CALLBACK MouseProc(

int nCode,      // hook code

WPARAM wParam, // message identifier

LPARAM lParam   // mouse coordinates

)

{

return 1; //返回非0值，系统不会将这个消息传递给目标窗口了，相当于屏蔽掉了所有的鼠标消息

}

void SetHook(HWND hwnd) //设置全局钩子，兼听键盘和鼠标

{

g_hWnd=hwnd;

g_hMouse=SetWindowsHookEx(WH_MOUSE,MouseProc,GetModuleHandle("Hook"),0);

g_hKeyboard=SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,GetModuleHandle("Hook"),0);

}

(3)添加Hook.def文件：

LIBRARY Hook

EXPORTS

SetHook   @2

其中的LIBRARY来指定动态链接库的名称，该名称一定要与生成的动态链接库匹配。

EXPORTS语句指出DLL要导出的函数。

使用DLL导出函数有个序号，如果不想用系统指定的序号，可以用@指定一个。

(4)创建一个对话框工程来测试：

在调用DLL的程序中这样申明导出函数：

_declspec(dllimport) void SetHook(HWND hwnd);


