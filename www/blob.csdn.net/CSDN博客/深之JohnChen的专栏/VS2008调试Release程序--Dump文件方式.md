# VS2008调试Release程序--Dump文件方式 - 深之JohnChen的专栏 - CSDN博客

2014年05月15日 10:19:31[byxdaz](https://me.csdn.net/byxdaz)阅读数：12283


在Windows平台下用C++开发应用程序，最不想见到的情况恐怕就是程序崩溃，而要想解决引起问题的bug，最困难的应该就是调试release版本了。因为release版本来就少了很多调试信息，更何况一般都是发布出去由用户使用，crash的现场很难保留和重现。目前有一些方法可以解决：崩溃地址
 + MAP文件；MAP文件；SetUnhandledExceptionFilter + Minidump。本文重点解决Minidump方式。

一、Minidump文件生成

**1、Minidump概念**

    minidump（小存储器转储）可以理解为一个dump文件，里面记录了能够帮助调试crash的最小有用信息。实际上，如果你在系统属性
 -> 高级 -> 启动和故障恢复 ->设置 ->
写入调试信息中选择“小内存转储(64 KB)”的话，当系统意外停止时都会在C:\Windows\Minidump\路径下生成一个.dmp后缀的文件，这个文件就是minidump文件，只不过这个是内核态的minidump。

我们要生成的是用户态的minidump，文件中包含了程序运行的模块信息、线程信息、堆栈调用信息等。而且为了符合其mini的特性，dump文件是压缩过的。

**2、生成minidump文件**

通过drwtsn32、NTSD、CDB等调试工具生成Dump文件，
 drwtsn32存在的缺点虽然NTSD、CDB可以完全解决，但并不是所有的操作系统中都安装了NTSD、CDB等调试工具。根据MiniDumpWriteDump接口，完全可以程序自动生成Dump文件。

**3、  自动生成Minidump文件**

当程序遇到未处理异常(主要指非指针造成)导致程序崩溃死，如果在异常发生之前调用了SetUnhandledExceptionFilter()函数，异常交给函数处理。MSDN中描述为：

Issuing SetUnhandledExceptionFilter replaces the existing top-level exception filter for all existing and all future threads in the calling process.

因而，在程序开始处增加SetUnhandledExceptionFilter()函数，并在函数中利用适当的方法生成Dump文件，即可实现需要的功能。

生成dump文件类(minidump.h)

```cpp
#pragma once

 

#include <windows.h>

#include <imagehlp.h>

#include <stdlib.h>

#pragma comment(lib, "dbghelp.lib")

 

inline BOOL IsDataSectionNeeded(const WCHAR* pModuleName)

{

    if(pModuleName == 0)

    {

       return FALSE;

    }

 

    WCHAR szFileName[_MAX_FNAME] = L"";

    _wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);

    if(wcsicmp(szFileName, L"ntdll") == 0)

       return TRUE;

 

    return FALSE; 

}

 

inline BOOL CALLBACK MiniDumpCallback(PVOID                            pParam, 

                                  const PMINIDUMP_CALLBACK_INPUT   pInput, 

                                  PMINIDUMP_CALLBACK_OUTPUT        pOutput)

{

    if(pInput == 0 || pOutput == 0)

       return FALSE;

 

    switch(pInput->CallbackType)

    {

    case ModuleCallback: 

       if(pOutput->ModuleWriteFlags & ModuleWriteDataSeg) 

 

           if(!IsDataSectionNeeded(pInput->Module.FullPath)) 

 

              pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg); 

 

    case IncludeModuleCallback:

    case IncludeThreadCallback:

    case ThreadCallback:

    case ThreadExCallback:

       return TRUE;

 

    default:;

    }

 

    return FALSE;

}

 

//创建Dump文件

inline void CreateMiniDump(EXCEPTION_POINTERS* pep, LPCTSTR strFileName)

{

    HANDLE hFile = CreateFile(strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))

    {

       MINIDUMP_EXCEPTION_INFORMATION mdei;

       mdei.ThreadId           = GetCurrentThreadId();

       mdei.ExceptionPointers  = pep;

       mdei.ClientPointers     = FALSE;

       MINIDUMP_CALLBACK_INFORMATION mci;

       mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;

       mci.CallbackParam       = 0;

       MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)0x0000ffff;

       MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &mdei, NULL, &mci);

 

       CloseHandle(hFile); 

    }

}

 

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)

{

    return NULL;

}

 

BOOL PreventSetUnhandledExceptionFilter()

{

    HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));

    if (hKernel32 ==   NULL)

       return FALSE;

 

 

    void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");

    if(pOrgEntry == NULL)

       return FALSE;

 

 

    unsigned char newJump[ 100 ];

    DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;

    dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far

 

 

    void *pNewFunc = &MyDummySetUnhandledExceptionFilter;

    DWORD dwNewEntryAddr = (DWORD) pNewFunc;

    DWORD dwRelativeAddr = dwNewEntryAddr -  dwOrgEntryAddr;

 

 

    newJump[ 0 ] = 0xE9;  // JMP absolute

    memcpy(&newJump[ 1 ], &dwRelativeAddr, sizeof(pNewFunc));

    SIZE_T bytesWritten;

    BOOL bRet = WriteProcessMemory(GetCurrentProcess(),    pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);

    return bRet;

}

 

 

LONG WINAPI UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException)

{

    TCHAR szMbsFile[MAX_PATH] = { 0 };

    ::GetModuleFileName(NULL, szMbsFile, MAX_PATH);

    TCHAR* pFind = _tcsrchr(szMbsFile, '\\');

    if(pFind)

    {

       *(pFind+1) = 0;

       _tcscat(szMbsFile, _T("CreateMiniDump.dmp"));

       CreateMiniDump(pException,szMbsFile);

    }

 

 

    // TODO: MiniDumpWriteDump

    FatalAppExit(-1,  _T("Fatal Error"));

    return EXCEPTION_CONTINUE_SEARCH;

}

 

//运行异常处理

void RunCrashHandler()

{

    SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);

    PreventSetUnhandledExceptionFilter();

}
```

//测试实现文件

// 一个有函数调用的类

// 

classCrashTest

{

public:

voidTest()

{ 

Crash();

}

private:

voidCrash()

{ 

strcpy(NULL,"adfadfg");

}

};

int_tmain(intargc, _TCHAR*
argv[])

{

//设置异常处理函数

RunCrashHandler();

CrashTesttest;

test.Test();

getchar();

return 0;

}

注意事项

1、需要配置debug选项，在C/C++选项à常规à调试信息格式(设置为程序数据库(/Zi))；在连接器选项—>调试à生成调试信息(设置为是)；C/C++选项à优化à禁用。(参见下图)

2、可执行文件（exe）必须找到dbghelp.dll，才能生成Dump文件。这个DLL可以从调试工具包中找到。

3、*.exe、*.pdb、*.dump、dbghelp.dll这四个文件需要放在同一目录下才好调试，双击dump文件时，就可以自动关联到出错代码位置。

4、为了获取更多更深入的调试信息，需要把程序优化开关设置成禁用。

5、当异常代码定位成功以后，如果无法阻止异常的产生，可以用 __try结构包装异常代码，__try
和 try不同，前者可以捕获非法指针产生的异常。

__try {

// 会异常的函数

}

__except( EXCEPTION_EXECUTE_HANDLER ){

// 异常处理

}

![](https://img-blog.csdn.net/20140515100029171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20140515100042937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20140515100108906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

<这个优化，不设置，也可以dump到错误。>

如果是dll，设置上面这几个属性就可以了，在.exe主程序中运行异常处理函数RunCrashHandler()。

二、调试Minidump文件
- 双击minidump文件（*.dmp）。默认会启动vs2008。
- 菜单Tools/Options, Debugging/Symbols，增加PDB文件路径。注：如果minidump文件与pdb文件在同一目录，就不用设置这个了。
- 若调试的程序需要微软基础库的PDB信息，可以增加一个路径为：
- [http://msdl.microsoft.com/download/symbols](http://msdl.microsoft.com/download/symbols)
- 在界面下方Cache Symbol From symbol…选择本地存储这些Symbols的路径。 注：如果本地已存储过微软基础库的pdb，就直接按照此步操作设置本地路径，不必执行上一步操作了。
- ![](https://img-blog.csdn.net/20130717160631937)设置代码路径：


设置代码路径：

刚打开的dmp工程，进入解决方案的属性。在这里输入源程序的代码路径。注：一定是sln所在的路径，而不是vcproj的路径！

![](https://img-blog.csdn.net/20130717160712875)


按F5，debug吧。

demo代码下载：[http://download.csdn.net/detail/byxdaz/7349325](http://download.csdn.net/detail/byxdaz/7349325)

