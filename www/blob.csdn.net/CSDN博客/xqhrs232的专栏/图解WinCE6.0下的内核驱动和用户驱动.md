# 图解WinCE6.0下的内核驱动和用户驱动 - xqhrs232的专栏 - CSDN博客
2013年11月01日 14:25:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：771
原文地址::[http://www.cnblogs.com/we-hjb/archive/2010/12/30/1922563.html](http://www.cnblogs.com/we-hjb/archive/2010/12/30/1922563.html)
相关文章
1、[WinCE驱动程序的分类](http://www.cnblogs.com/we-hjb/archive/2008/11/23/1339603.html)----[http://www.cnblogs.com/we-hjb/archive/2008/11/23/1339603.html](http://www.cnblogs.com/we-hjb/archive/2008/11/23/1339603.html)
2、WinCE6.0内核模式驱动程序下的缓冲区问题----[http://bbs.csdn.net/topics/340104628](http://bbs.csdn.net/topics/340104628)

   在《[WinCE驱动程序的分类](http://www.cnblogs.com/we-hjb/archive/2008/11/23/1339603.html)》中曾提到，WinCE6.0的流驱动既可以加载到内核态也可以加载到用户态。下面通过一组图片简单说明一下这两种驱动的关系。
      首先编写一个流驱动WCEDrv，代码如下。
```
#include <windows.h>
extern "C"
BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
    UNREFERENCED_PARAMETER(lpvReserved);
    switch(dwReason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls((HMODULE) hinstDLL);
        break;
    case DLL_PROCESS_DETACH:
        break;
    };
    return TRUE;
}
extern "C"
DWORD Init(LPCTSTR pContext, DWORD dwBusContext)
{
    RETAILMSG(1,(_T("Init(%s, %x)"),pContext,dwBusContext));
    
    PBYTE pBuffer = new BYTE[4096*1024];     
    RETAILMSG(1,(TEXT("pBuffer(%x)\r\n"),pBuffer));
    return  (DWORD)pBuffer;
}
extern "C"
BOOL Deinit(DWORD hDeviceContext)
{
    RETAILMSG(1,(_T("Deinit(%x)\r\n"),hDeviceContext));
    
    PBYTE pBuffer = (PBYTE)hDeviceContext;
    if (pBuffer)
    {
        delete[] pBuffer;
    }    
    return TRUE;
}
extern "C"
void PowerUp(DWORD hDeviceContext)
{
}
extern "C"
void PowerDown(DWORD hDeviceContext)
{
}
extern "C"
DWORD Open(DWORD hDeviceContext, DWORD AccessCode, DWORD ShareMode)
{
    RETAILMSG(1,(_T("Open(%x, 0x%x, 0x%x)\r\n"),hDeviceContext, AccessCode, ShareMode));
    return hDeviceContext;
}
extern "C"
BOOL Close(DWORD hOpenContext)
{
    RETAILMSG(1,(_T("Close(%x)\r\n"),hOpenContext));
    return TRUE;
}
extern "C"
BOOL IOControl(DWORD hOpenContext, DWORD dwCode, PBYTE pBufIn, DWORD dwLenIn
               , PBYTE pBufOut, DWORD dwLenOut, PDWORD pdwActualOut)
{
    UNREFERENCED_PARAMETER(hOpenContext);
    UNREFERENCED_PARAMETER(dwCode);
    UNREFERENCED_PARAMETER(pBufIn);
    UNREFERENCED_PARAMETER(dwLenIn);
    UNREFERENCED_PARAMETER(pBufOut);
    UNREFERENCED_PARAMETER(dwLenOut);
    UNREFERENCED_PARAMETER(pdwActualOut);
    SetLastError(ERROR_INVALID_FUNCTION);
    return FALSE;
}
extern "C"
DWORD Read(DWORD hOpenContext, LPVOID pBuffer, DWORD Count)
{
    RETAILMSG(1,(_T("Read(%x, %x, 0x%x)\r\n"),hOpenContext, pBuffer, Count));
    return TRUE;
}
extern "C"
DWORD Write(DWORD hOpenContext, LPCVOID pBuffer, DWORD Count)
{
    RETAILMSG(1,(_T("Write(%x, %x, 0x%x)\r\n"),hOpenContext, pBuffer, Count));
    return TRUE;
}
extern "C"
DWORD Seek(DWORD hOpenContext, long Amount, WORD Type)
{
    UNREFERENCED_PARAMETER(hOpenContext);
    UNREFERENCED_PARAMETER(Amount);
    UNREFERENCED_PARAMETER(Type);
    SetLastError(ERROR_NOT_SUPPORTED);
    return -1;
}
```
       其对应的注册表文件内容如下。
```
[HKEY_LOCAL_MACHINE\Drivers\WCEDrv1]
    "Prefix"="AAA"
    "Dll"="WCEDrv.dll"
    "Index"=dword:1
    "Flags"=dword:8        ; DEVFLAGS_NAKEDENTRIES
    "Order"=dword:0
[HKEY_LOCAL_MACHINE\Drivers\WCEDrv2]
    "Prefix"="BBB"
    "Dll"="WCEDrv.dll"
    "Index"=dword:1
    "Flags"=dword:8        ; DEVFLAGS_NAKEDENTRIES
    "Order"=dword:0
[HKEY_LOCAL_MACHINE\Drivers\WCEDrv3]
    "Prefix"="CCC"
    "Dll"="WCEDrv.dll"
    "Index"=dword:1
    "Flags"=dword:18    ; DEVFLAGS_LOAD_AS_USERPROC | DEVFLAGS_NAKEDENTRIES
    "Order"=dword:0
[HKEY_LOCAL_MACHINE\Drivers\WCEDrv4]
    "Prefix"="DDD"
    "Dll"="WCEDrv.dll"
    "Index"=dword:1
    "Flags"=dword:18    ; DEVFLAGS_LOAD_AS_USERPROC | DEVFLAGS_NAKEDENTRIES
    "Order"=dword:0
```
       通过驱动调试助手动态加载该驱动，在加载时，根据注册表中的设置，分别加载两个到内核空间和用户空间。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/201012/20101230223914226.png)
                                   通过驱动调试助手导入注册表文件
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/201012/201012302239172503.png)
                                    用户态和内核态分别加载两个，共加载四个驱动
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/201012/201012302239186516.png)
                                 加载驱动过程时的串口打印
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/201012/201012302239203809.png)
                加载驱动成功后，HKEY_LOCAL_MACHINE\Drivers\Active下内核驱动对应的键
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/201012/201012302239226926.png)
                     加载驱动成功后，HKEY_LOCAL_MACHINE\Drivers\Active下用户驱动对应的键      ![image](http://images.cnblogs.com/cnblogs_com/we-hjb/201012/20101230223927509.png)
                          通过远程堆查看器，查看内存的分配情况
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/201012/201012302239348179.png)
                      通过远程进程查看器 ，查看wcedrv.dll加载为内核态驱动 
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/201012/201012302239441267.png)
                          通过远程进程查看器，查看wcedrv.dll加载为用户态驱动1
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/201012/201012302239484152.png)
                      通过远程进程查看器，查看wcedrv.dll加载为用户态驱动2
      WCEDRV的源代码下载地址：[http://files.cnblogs.com/we-hjb/WCEDrv.rar](http://files.cnblogs.com/we-hjb/WCEDrv.rar)
//===========================================================================
备注：：
1》内核态驱动加载在NK.EXE中，用户态驱动加载在udevice.exe中
2》通过改变驱动加载的注册表的flag值可以改变驱动加载的模式----内核态/用户态
3》怎么用户态的驱动写注册表的方式怎么老加载不了？注册表不包括BuiltIn也还是不行---BSP模式下
4》他这篇文章显示的驱动不是一个流接口的驱动，看下面的注册表情况，流接口驱动一般为[HKEY_LOCAL_MACHINE\Drivers\BuiltIn\XXX]，
流接口驱动都在BuiltIn主键下！！！
[HKEY_LOCAL_MACHINE\Drivers\WCEDrv1]
"Prefix"="AAA"
"Dll"="WCEDrv.dll"
"Index"=dword:1
"Flags"=dword:8        ; DEVFLAGS_NAKEDENTRIES
"Order"=dword:0
 5》ctrl+alt+a弹出截屏的界面，然后在所截的屏幕可以增加说明文字/箭头----这个功能对于写文档很有作用
