# 诊断Windows CE下的内存泄露(转载) - xqhrs232的专栏 - CSDN博客
2009年11月12日 14:48:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1154

[版权声明](http://creativecommons.org/licenses/by/3.0/deed.zh)：转载时请以超链接形式标明文章原始出处和作者信息及[本声明](http://bangzhuzhongxin.blogbus.com/logs/11205960.html)
[http://omale.blogbus.com/logs/38322194.html](http://omale.blogbus.com/logs/38322194.html)
这篇文章是直接从Get Embedded专栏翻译的。其中LMEMDEBUG还是第一次听说，感觉还是有些用处的，有可能利用LMEMDEBUG的功能开发Windows CE下的LeakDiag或者BoundsChecker。以后会写一篇文章深入的讨论LMEMDEBUG的机制。
英文原文：![::URL::](http://style.blogcn.com/blogcnpage/style/images/images/aurl.gif)[http://msdn.microsoft.com/library/en-us/dnembedded/html/embedded10152002.asp?frame=true
概述: 本文介绍Microsoft Windows CE.NET 4.1中自带的可以用来在自定义的操作系统中跟踪内存泄露的工具。特别的，Debug Zones，远程性能监视器以及LMEMDEBUG。
很希望你能在你的Windows CE项目上进展顺利。如果是这样，你应该非常舒服的构建操作系统映像，你也可能写了一些在Windows CE上面跑的代码——或许是应用程序或许是驱动。这个月的文章主要介绍内存泄露和如何跟踪内存泄露。
我知道你们都是非常优秀的开发人员。你们的代码一直都逻辑清晰并且所有细枝末节都含有注释。代码非常容易维护并且没有内存泄露和Bug……好了，让我们回到现实世界中来。
我们在我们的代码中都会犯错误，尤其是当我们被项目的时间所迫或者深夜在键盘上敲击代码的时候。当我们的代码完成的时候，我们如何检测代码中是否包含内存泄露呢](http://msdn.microsoft.com/library/en-us/dnembedded/html/embedded10152002.asp?frame=true)？更进一步，我们如何跟踪到源代码中然后修复它们呢？本月的文章中我们将会看一下debug zone，远程性能监视器和LMEMDEBUG。
我们会使用一个基于命令行的简单示例程序来帮助演示Windows CE中的一些工具和特性。这个例子叫memLeak。顾名思义，它泄漏内存。这个程序有两个线程，主线程有一个含有Sleep的循环以保证程序不会结束，子线程每500毫秒分配分配一次内存。我们使用这个程序来演示如何使用debug zone，远程性能监视器和LMEMDEBUG。memLeak程序中调用了一系列函数。它们是：
•AllocateMemory( ) 
•UseMemory( ) 
•FreeMemory( ) 
从函数的名字中你就可以猜到它们的功能了。AllocateMemory检查现在的内存负载，如果负载低于60%，我们就申请2048*TCHAR然后继续。UseMemory对内存做一些有用的事情，FreeMemory希望释放我们已经申请的内存。
这是应用程序的完整源代码：
// memLeak.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <celog.h>
#define MAX_LOADSTRING 100
        // Definitions for our debug zones                               
        #define ZONEID_INIT      0
        #define ZONEID_TRACE     1
        #define ZONEID_MEMORY    2
        #define ZONEID_RSVD3     3
        #define ZONEID_RSVD4     4
        #define ZONEID_RSVD5     5
        #define ZONEID_RSVD6     6
        #define ZONEID_RSVD7     7
        #define ZONEID_RSVD8     8
        #define ZONEID_RSVD9     9
        #define ZONEID_RSVD10   10
        #define ZONEID_RSVD11   11
        #define ZONEID_RSVD12   12
        #define ZONEID_RSVD13   13        
        #define ZONEID_WARN     14
        #define ZONEID_ERROR    15
        // These masks are useful for initialization of dpCurSettings
        #define ZONEMASK_INIT      (1<<ZONEID_INIT) 
        #define ZONEMASK_TRACE     (1<<ZONEID_TRACE) 
        #define ZONEMASK_MEMORY    (1<<ZONEID_MEMORY) 
        #define ZONEMASK_RSVD3     (1<<ZONEID_RSVD3) 
        #define ZONEMASK_RSVD4     (1<<ZONEID_RSVD4) 
        #define ZONEMASK_RSVD5     (1<<ZONEID_RSVD5) 
        #define ZONEMASK_RSVD6     (1<<ZONEID_RSVD6) 
        #define ZONEMASK_RSVD7     (1<<ZONEID_RSVD7) 
        #define ZONEMASK_RSVD8     (1<<ZONEID_RSVD8) 
        #define ZONEMASK_RSVD9     (1<<ZONEID_RSVD9) 
        #define ZONEMASK_RSVD10    (1<<ZONEID_RSVD10)
        #define ZONEMASK_RSVD11    (1<<ZONEID_RSVD11)
        #define ZONEMASK_RSVD12    (1<<ZONEID_RSVD12)
        #define ZONEMASK_RSVD13    (1<<ZONEID_RSVD13)        
        #define ZONEMASK_WARN      (1<<ZONEID_WARN )  
        #define ZONEMASK_ERROR     (1<<ZONEID_ERROR) 
#ifdef DEBUG
        // These macros are used as the first arg to DEBUGMSG
        #define ZONE_INIT       DEBUGZONE(ZONEID_INIT)
        #define ZONE_TRACE      DEBUGZONE(ZONEID_TRACE)
        #define ZONE_MEMORY     DEBUGZONE(ZONEID_MEMORY)
        #define ZONE_RSVD3      DEBUGZONE(ZONEID_RSVD3)
        #define ZONE_RSVD4      DEBUGZONE(ZONEID_RSVD4)
        #define ZONE_RSVD5      DEBUGZONE(ZONEID_RSVD5)
        #define ZONE_RSVD6      DEBUGZONE(ZONEID_RSVD6)
        #define ZONE_RSVD7      DEBUGZONE(ZONEID_RSVD7)
        #define ZONE_RSVD8      DEBUGZONE(ZONEID_RSVD8)
        #define ZONE_RSVD9      DEBUGZONE(ZONEID_RSVD9)
        #define ZONE_RSVD10     DEBUGZONE(ZONEID_RSVD10)
        #define ZONE_RSVD11     DEBUGZONE(ZONEID_RSVD11)
        #define ZONE_RSVD12     DEBUGZONE(ZONEID_RSVD12)
        #define ZONE_RSVD13     DEBUGZONE(ZONEID_RSVD13)
        #define ZONE_WARN       DEBUGZONE(ZONEID_WARN )
        #define ZONE_ERROR      DEBUGZONE(ZONEID_ERROR)
#endif
DBGPARAM dpCurSettings = {
    TEXT("MemLeak"), {
        TEXT("Init"),TEXT("Trace Fn( );"),TEXT("Memory"),TEXT(""),
        TEXT(""),TEXT(""),TEXT(""),TEXT(""),
        TEXT(""),TEXT(""),TEXT(""),TEXT(""),
        TEXT(""),TEXT(""),TEXT(""),TEXT("")},
    // By default, turn on the zones for init and errors.
    ZONEMASK_INIT    
}; 
DWORD WINAPI MemoryThread(LPVOID lpParameter);
void AllocateMemory( );
void FreeMemory( );
void StartAllocation( );
void UseMemory( );
MEMORYSTATUS g_MemStatus;
HLOCAL g_tcTemp,g_tc_Temp;
DWORD dwThreadID;
TCHAR szMessage[256];
int WINAPI main (HINSTANCE hInstance, 
HINSTANCE hInstPrev, LPWSTR pCmdLine, int nCmdShow)
{
   OutputDebugString(L"leakApp Starting/n");
   DEBUGREGISTER(NULL);      // Register the debug zones
   StartAllocation( );
   while(true) {
      Sleep(1000);
   }
   return 0;
}
void StartAllocation( )
{
   g_tcTemp=NULL;
   g_tc_Temp=NULL;
   OutputDebugString(L"Creating Thread.../n");
   CreateThread(NULL,0,
(LPTHREAD_START_ROUTINE)MemoryThread,
(LPVOID)0,0,&dwThreadID);
}
DWORD WINAPI MemoryThread(LPVOID lpParameter)
{
   while(TRUE) {
      Sleep(500);
      DEBUGMSG (ZONE_TRACE, (TEXT("-------------------------/n")));
      AllocateMemory( );
      UseMemory( );
      FreeMemory( );
   }
}
void AllocateMemory( )
{
   DEBUGMSG (ZONE_TRACE, (TEXT("Enter - AllocateMemory( ) Function/n")));
   DEBUGMSG (ZONE_MEMORY, (TEXT("Check GlobalMemoryStatus( )/n")));
   memset(&g_MemStatus,0x00,sizeof(g_MemStatus));
   g_MemStatus.dwLength=sizeof(g_MemStatus);
   GlobalMemoryStatus(&g_MemStatus);
   DEBUGMSG (ZONE_MEMORY, 
  (TEXT("Memory Load %d%%/n"),
  g_MemStatus.dwMemoryLoad));
   CELOGDATA(TRUE, CELID_RAW_LONG, &g_MemStatus.dwMemoryLoad, 
(WORD) (sizeof(DWORD)), 1, CELZONE_MISC);
   if (g_MemStatus.dwMemoryLoad < 60) {
      DEBUGMSG (ZONE_MEMORY, 
       (TEXT("Allocate TCHAR *2048 (4096 UNICODE Characters)/n")));
      g_tcTemp=LocalAlloc(LPTR,(2048*sizeof(TCHAR)));   
      DEBUGMSG (ZONE_MEMORY, (TEXT("Pointer 0x%lx/n"),g_tcTemp));
   } else {
      DEBUGMSG (ZONE_MEMORY, 
               (TEXT("Memory Load too high - not allocating memory /n"),
               g_MemStatus.dwMemoryLoad));
   }
   DEBUGMSG (ZONE_TRACE, (TEXT("Leave - AllocateMemory( ) Function/n")));
}
void FreeMemory( )
{
   DEBUGMSG (ZONE_TRACE, (TEXT("Enter - FreeMemory( ) Function/n")));
   DEBUGMSG (ZONE_MEMORY, (TEXT("Free Pointer 0x%lx/n"),g_tc_Temp));
   LocalFree(g_tc_Temp);
   DEBUGMSG (ZONE_TRACE, (TEXT("Leave - FreeMemory( ) Function/n")));
}
void UseMemory( )
{
   DEBUGMSG (ZONE_TRACE, (TEXT("Enter - UseMemory( ) Function/n")));
   DEBUGMSG (ZONE_MEMORY, (TEXT("Do Something Interesting here./n")));
   DEBUGMSG (ZONE_TRACE, (TEXT("Leave - UseMemory( ) Function/n")));
}
有很多方法可以跟踪正在运行的程序的流程。或许最简单的方法是使用OutputDebugString( )。这个函数可以用来输出代码中任何有用的信息。不幸的是，不管我们想不想要，我们都会看到这些调试信息，这有可能导致信息过量。
当我们需要的时候再打开调试信息或许比较有用。例如，在程序运行的时候跟踪我们代码中函数的入口和出口可以显示代码流程。我们或许不需要这个功能随时在运行，但是当我们跟踪泄漏或者崩溃的时候确定代码流程就非常有用。那么，我们如何来启用它呢？
答案是debug zone。Windows CE操作系统中的大多数模块都启用了debug zone。看一下下面的Platform Builder的截屏。（在Platform Builder中选择Target | CE Debug Zones会显示该对话框）它显示了GWES.exe所暴露的debug zone。你可以看到一共暴露了16个zone（0-15），默认的，zone 6，Warnings被启动了。
![](http://msdn.microsoft.com/library/en-us/dnembedded/html/embedded10152002_fig1.gif)
图1. Platform Builder中的Debug Zones 对话框
Debug zone提供了可选的打开和关闭代码中的调试信息的功能。这样做允许你在不挂起操作系统执行的情况下跟踪代码的执行。跟踪可以在不导致操作系统停止响应的情况下抓到代码中的问题的简单方法。Debug Zone可以通过Target Control（Target | CE Target Control）或者Platform Builder的IDE（Target | CE Debug Zones）来启动。
每一个应用程序或模块（驱动，DLL或其它）可以包含16个debug zone。每个zone的目的不是固定的。你可以编程来使每个zone显示对你当前的项目合适的信息。你或许在你的应用程序/驱动程序中只需要一个或者两个zone。可以用它来跟踪函数的进入和离开（代码跟踪）。你或许也对内存申请/释放感兴趣。代码跟踪或内存申请可以在你的应用或驱动中被写成debug zone。
Debug Zone通过声明DBGPARAM结构体来实现，它在dbgapi.h中定义。DBGPARAM结构包含三个元素：模块名，zone名和显示默认那些zone被启用的位掩码。下面显示了在memLeak程序中是如何填充DBGPARAM结构体的：
DBGPARAM dpCurSettings = {
    TEXT("MemLeak"), {
        TEXT("Init"),TEXT("Trace Fn( );"),TEXT("Memory"),TEXT(""),
        TEXT(""),TEXT(""),TEXT(""),TEXT(""),
        TEXT(""),TEXT(""),TEXT(""),TEXT(""),
        TEXT(""),TEXT(""),TEXT(""),TEXT("")},
    // By default, turn on the zones for init and errors.
    ZONEMASK_INIT    
}; 
我们可以看到模块的名字是“memLeak”，然后是16个字符串，定义了代码中的debug zone的人类可以阅读的名字。（这是Platform Builder的’Debug Zone’对话框中显示的名字）这样的话，我们暴露了三个zone：Init，Trace FN()和Memory。DEGPARM结构的第三部分定义了哪些zone默认是开启的——我们开启了Init Zone。
那么，我们的程序中如何启用debug zone呢？简单，我们定义一个名为dpCurSetings的DBGPARAM结构，然后在我们的程序或者模块启动的时候，我们调用DEBUGREGISTER()。DEBUGREGISTER宏的语法是DEBUGREGISTER(hMod | NULL)。如果你正在调试.dll，使用合适的hModule调用DEBUGREGISTER，如果你正在构建应用程序，就给宏传递NULL。
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
                     )
目前为止还一切都很好，我们知道了如何填写结构体和如何向调试器注册debug zone。然后就可以在Platform Builder中简单的显示debug zone了（菜单项Target | CE Debug Zone）。那么我们的代码中如何使用debug zone呢？一样，很简单。我们看一下Trace Fn()调试信息还有他是如何被启用的。这里有一行从memLeak应用程序中取出的代码：
   DEBUGMSG (ZONE_TRACE, (TEXT("Enter - AllocateMemory( ) Function/n")));
可以看到，调用了DEBUGMSG( )函数。这个函数有两个参数：一个位测试和一个字符串。这个位测试有不同的使用方式。如果你想让这条消息永远都出现，你可以这样指定DEBUGMSG(1,L"String")。注意，数字是第一个参数——或者，我们可以使用类似这样的语句DEBUGZONE(ZONEID_INIT,L"String")来测试某个debug zone是否被启用。——或者使用位测试，或者使用1来做为DEBUGZONE()的第一个参数。我们只是需要一种方式来确定是否某个debug zone被启用了。如果被启用了，就输出调试字符串。
那么这个能如何帮助我们跟踪内存泄露呢？我们可以在我们的程序中包含一个MemoryAllocate和MemoryFree函数。这样（最基本的）就可以跟踪内存分配的数量，声明一个全局变量，分配内存时增加全局变量的值，然后把全局变量的值作为调试信息输出。显然，MemoryFree需要减少这 ...
