# 虚拟内存——Windows核心编程学习手札之十四 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年06月11日 17:00:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2531








# 虚拟内存

### ——Windows核心编程学习手札之十四

系统信息

有些操作系统的值是根据主机而定的，如页面大小、分配粒度大小等，这些值不用硬编码形式，进程初始化时应检索这些值以使用。函数GetSystemInfo函数用于检索与主机相关的值：

VOID GetSystemInfo(LPSYSTEM_INFO psinf);

传递SYSTEM_INFO结构的地址，该函数将初始化结构成员并返回。

typedef struct _SYSTEM_INFO{

union{

DWORD dwOemId;//obsolete,do not use

Struct{

WORD wProcessorArchitecture;//用于指明处理结构，如Intel、Alpha、Intel64位lpha64位，主要在windows2000中；

WORD wReserved;

};

};

DWORD dwPageSize;//用于显示CPU页面大小，如x86是4096字节；Alpha是8192字节；IA-64是8192字节；

LPVOID lpMinimumApplicationAddress;//用于给出每个进程的可用地址空间的最小内存地址，如Windows98中是4 194 304或0x00400000，因为每个进程的地址空间中下面4MB是不能使用的，用户方式的分区在该地址之上；如windows2000中，这个值是65 536或0x000100000，因为每个进程的地址空间中开头的64KB是空闲的；

LPVOID lpMaximumApplicationAddress;//用户给出每个进程的可用地址空间的最大内存地址，如windows98，这个地址是2 147 483 647或0x7FFFFFFF，因为共享内存映射文件和共享操作系统代码包含在上面的2GB分区中；如windows2000，这个地址是内核方式内存开始的地址，不足64KB；

DWORD_PTR dwActiveProcessorMask;//一个位屏蔽，用于指明哪个CPU是活动的；

DWORD dwNumberOfProcessors;//用于指明计算机中的CPU数目；

DWORD dwProcessorType;//用于指明处理器类型，如Intel386、486/Pentium，主要在windows98中；

DWORD dwAllocationGranularity;//显示保留的地址空间区域的分配粒度，目前，所有windows平台都是65 356；

WORD wProcessorLevel;//用于进一步细分处理器的结构，如设定Intel Pentium Pro或PentiumⅡ，主要用于windows2000；

WORD wProcessorRevision;//用于进一步细分处理器的级别，用于windows2000；

}SYSTEM_INFO,*LPSYSTEM_INFO;

对既定的系统，这些值都是相同的，不需要在既定的进程中多次调用函数，通过GetSystemInfo函数应该程序可以查询关于系统中的这些值。

示例代码：

#include "stdafx.h"

#include "windows.h"

#include "tchar.h"





void ShowCPUInfo(WORD wProcessorArchitecture,WORD wProcessorLevel,WORD wProcessorRevision)

{

TCHAR strInfo[256];

memset(strInfo,0,256);

/*PROCESSOR_ARCHITECTURE_INTEL 

PROCESSOR_ARCHITECTURE_MIPS 

PROCESSOR_ARCHITECTURE_UNKNOWN 

PROCESSOR_ARCHITECTURE_SHX 

PROCESSOR_ARCHITECTURE_ARM */

switch(wProcessorArchitecture)

{

case PROCESSOR_ARCHITECTURE_INTEL:

if(wProcessorLevel==4)

{

wsprintf(strInfo,_T("处理器类型: Intel 80486"));

}

else if(wProcessorLevel==5)

{

wsprintf(strInfo,_T("处理器类型: Pentium"));

}

else

{

wsprintf(strInfo,_T("处理器类型: PROCESSOR_ARCHITECTURE_INTEL"));

}

break;

case PROCESSOR_ARCHITECTURE_MIPS:

switch(wProcessorLevel)

{

case 3:

wsprintf(strInfo,_T("处理器类型: MIPS R3000"));

break;

case 4:

wsprintf(strInfo,_T("处理器类型: MIPS R4000"));

break;

case 5:

wsprintf(strInfo,_T("处理器类型: MIPS R5000"));

break;

default:

wsprintf(strInfo,_T("处理器类型: PROCESSOR_ARCHITECTURE_MIPS"));

break;

}

break;

case PROCESSOR_ARCHITECTURE_SHX:

wsprintf(strInfo,_T("处理器类型: PROCESSOR_ARCHITECTURE_SHX"));

break;

case PROCESSOR_ARCHITECTURE_ARM:

if(wProcessorLevel==4)

{

wsprintf(strInfo,_T("处理器类型: ARM version 4"));

}

else

{

wsprintf(strInfo,_T("处理器类型: PROCESSOR_ARCHITECTURE_ARM"));

}

break;

default:

wsprintf(strInfo,_T("处理器类型: PROCESSOR_ARCHITECTURE_UNKNOWN"));

break;

}

}



int main(int argc, char* argv[])

{

SYSTEM_INFO sinf;

GetSystemInfo(&sinf);



ShowCPUInfo(sinf.wProcessorArchitecture,sinf.wProcessorLevel,sinf.wProcessorRevision);



TCHAR szBuf[256];

wsprintf(szBuf,_T("页面文件大小: %d 或 0x%08x"),sinf.dwPageSize,sinf.dwPageSize);

wsprintf(szBuf,_T("进程的最小内存地址: %d 或 0x%08x"),sinf.lpMinimumApplicationAddress,sinf.lpMinimumApplicationAddress);

wsprintf(szBuf,_T("进程的最大内存地址: %d 或 0x%08x"),sinf.lpMaximumApplicationAddress,sinf.lpMaximumApplicationAddress);

wsprintf(szBuf,_T("当前活动的CPU: %d"),sinf.dwActiveProcessorMask);

return 0;

}

虚拟内存的状态

Windows函数GlobalMemoryStatus可用于检索关于当前内存状态的动态信息：

VOID GlobalMemoryStatus(LPMEMORYSTATUS pmst);

当调用GlobalMemoryStatus时，要传递MEMORYSTATUS结构的地址：

typedef struct _MEMORYSTATUS{

DWORD dwLength;

DWORD dwMemoryLoad;

SIZE_T dwTotalPhys;

SIZE_T dwAvailPhys;

SIZE_T dwTotalPageFile;

SIZE_T dwAvailPageFile;

SIZE_T dwTotalVirtual;

SIZE_T dwAvailVirtual;

}MEMORYSTATUS,*LPMEMORYSTATUS;

调用GlobalMemoryStatus函数之前，将dwLength成员初始化为用字节表示的结构的大小，即一个MEMORYSTATUS结构的大小。

如应用程序在内存大于4GB的计算机上运行，或者合计交换文件的大小大于4GB，可用新的GlobalMemoryStatusEx函数：

BOOL GlobalMemoryStatusEx(LPMEMORYSTATUSEX pmst);

函数传递新的结构MEMORYSTATUSEX：

typedef struct _MEMORYSTATUSEX{

DWORD dwLength;

DWORD dwMemoryLoad;

DWORDLONG ullTotalPhys;

DWORDLONG ullAvailPhys;

DWORDLONG ullTotalPageFile;

DWORDLONG ullAvailPageFile;

DWORDLONG ullTotalVirtual;

DWORDLONG ullAvailVirtual;

DWORDLONG ullAvailExtendedVirtual;

}MEMORYSTATUSEX,*LPMEMORYSTATUSEX;

新结构和原来的MEMORYSTATUS基本相同，差别在于新结构的所有成员的大小都是64位宽，因此它的值可以大于4GB，最后一个成员ullAvailExtendedVirtual，用于指明在调用进程的虚拟地址空间的极大内存（VLM）部分中未保留内存的大小。该VLM部分只适用于某些配置中的某些CPU结构。

Windows提供了一个函数，可以用来查询地址空间中内存地址的某些信息（如大小，存储器类型和保护属性等）。函数VirtualQuery：

DWORD VirtualQuery(

LPCVOID pvAddress,

PMEMORY_BASIC_INFORMATION pmbi,

DWORD dwLength);

Windows也提供了VirualQueryEx函数，用于使一个进程能够查询另一个进程的内存信息：

DWORD VirtualQueryEx(

HANDLE hProcess,

LPCVOID pvAddress,

PMEMORY_BASIC_INFORMATION pmbi,

DWORD dwLength);

结构体MEMORY_BASIC_INFORMATION在WinNT.h中定义：

typedef struct _MEMORY_BASIC_INFORMATION{

PVOID BaseAddress;

PVOID AllocationBase;

PVOID AllocationProtect;

SIZE_T RegionSize;

DWORD State;

DWORD Protect;

DWORD Type;

} MEMORY_BASIC_INFORMATION,* PMEMORY_BASIC_INFORMATION；

函数VirtualQuery的最后一个参数是dwLength，用于设定MEMORY_BASIC_INFORMATION结构的大小。VirtualQuery(Ex)函数返回拷贝到缓存中的字节的数量。根据pvAddress参数中传递的地址，VirtualQuery(Ex)函数将关于共享相同状态、保护属性和类型的相邻页面的范围信息填入MEMORY_BASIC_INFORMATION结构中。





