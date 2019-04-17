# error LNK2005: _DllMain@12 已经在 dllmain.obj 中定义 - DoubleLi - 博客园





error LNK2005: _DllMain@12 已经在 dllmain.obj 中定义
今天遇到了同样的问题，搜索搜到了这里，后来解决了。。。。。。。。
创建解决方案时，用的是WIN32 DLL,添加了MFC ATL的支持，自动生成文件中是没有现成的DllMain函数的。后来需要用到这个入口函数，所以自己人工添加了DllMain函数，调试结果就报错，退重复定义DllMain之类的。。。

从其它地方找到了解决办法，这里给大家分享下：

给项目添加一个dllmian.cpp，里边的内容为
// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

// global data

// The following symbol used to force inclusion of this module for _USRDLL
#ifdef _X86_
extern "C" { int _afxForceUSRDLL; }
#else
extern "C" { int __afxForceUSRDLL; }
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
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

这样问题就 解决了。。最重要的就是
#ifdef _X86_
extern "C" { int _afxForceUSRDLL; }
#else
extern "C" { int __afxForceUSRDLL; }
#endif
这段代码起到了强制执行你自定义的dllmain函数的作用。。








