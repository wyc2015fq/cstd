# VC++钩子DLL框架代码(MFC Extension DLL using shared MFC DLL) - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年11月26日 07:39:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2911








// HookDllDemo.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>



#include <windows.h> //引入windows头文件



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// 建立数据段
#pragma data_seg("HookData")
HHOOK g_hHook = NULL;   // 钩子句柄
HINSTANCE g_hInstDLL = NULL; // 模块句柄
#pragma data_seg()
//设置数据段为可读可写可共享
#pragma comment(linker,"/SECTION:HookData,RWS")



static AFX_EXTENSION_MODULE HookDllDemoDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)//入口函数
{
 // Remove this if you use lpReserved
 UNREFERENCED_PARAMETER(lpReserved);

 if (dwReason == DLL_PROCESS_ATTACH)
 {
  TRACE0("HOOKDLLDEMO.DLL Initializing!/n");

  // Extension DLL one-time initialization
  if (!AfxInitExtensionModule(HookDllDemoDLL, hInstance))
   return 0;

  // Insert this DLL into the resource chain
  // NOTE: If this Extension DLL is being implicitly linked to by
  //  an MFC Regular DLL (such as an ActiveX Control)
  //  instead of an MFC application, then you will want to
  //  remove this line from DllMain and put it in a separate
  //  function exported from this Extension DLL.  The Regular DLL
  //  that uses this Extension DLL should then explicitly call that
  //  function to initialize this Extension DLL.  Otherwise,
  //  the CDynLinkLibrary object will not be attached to the
  //  Regular DLL's resource chain, and serious problems will
  //  result.

  new CDynLinkLibrary(HookDllDemoDLL);



  g_hInstDLL = hInstDLL; // 初始化模块句柄


 }
 else if (dwReason == DLL_PROCESS_DETACH)
 {
  TRACE0("HOOKDLLDEMO.DLL Terminating!/n");
  // Terminate the library before destructors are called
  AfxTermExtensionModule(HookDllDemoDLL);
 }
 return 1;   // ok
}





// 钩子回调函数
LRESULT WINAPI HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
 // TODO: 在这里添加响应动作

 // 将事件传递到下一个钩子
 return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// 安装钩子
extern "C" __declspec(dllexport) DWORD InstallHook(void)
{
 DWORD dwRet = 0;
 /*加载钩子类别*/
 g_hHook = SetWindowsHookEx(WH_MOUSE/*WH_SHELL*/, HookProc, g_hInstDLL, 0); 
 if(g_hHook == NULL)
  dwRet = GetLastError();
 return dwRet;
}

// 卸载钩子
extern "C" __declspec(dllexport) DWORD KillHook(void)
{
 if(UnhookWindowsHookEx(g_hHook))
  return 0;
 else
  return GetLastError();
}



