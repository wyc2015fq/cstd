# 堆栈——Windows核心编程学习手札之十八 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年09月04日 17:25:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2169








# 堆栈

## ——Windows核心编程学习手札之十八

堆栈可以用来分配许多较小的数据块，可以不考虑分配粒度和页面边界之类的问题，但分配和释放内存块的速度比其他机制要慢，并且无法直接控制物理存储器的提交和回收。堆栈是保留的地址空间的一个区域，开始时，保留区域中的大多数页面没有被提交物理存储器，当从堆栈中进行越来越多的内存分配时，堆栈管理器将把更多的物理存储器提交给堆栈，物理存储器总是从系统的页文件中分配的，当释放堆栈中的内存块时，堆栈管理器将收回这些物理存储器。

进程初始化时，系统在进程的地址空间中创建一个堆栈，称为进程的默认堆栈，默认大小是1MB，但系统可以扩大进程的默认堆栈，使它大于其默认值，当创建应用程序时，可以使用/HEAP链接开关，改变堆栈的1MB默认区域大小。/HEAP链接开关的句法：

/HEAP:reserve[.commit]

可通过调用GetProcessHeap函数获取进程默认堆栈的句柄：

HANDLE GetProcessHeap()；

除进程的默认堆栈外，可以在进程的地址空间中创建辅助堆栈，基于保护组件、更加有效地进行内存管理、进行本地访问、减少线程同步的开销、迅速释放等原因，需要创建辅助堆栈。线程调用HeapCreate函数可创建辅助堆栈：

HANDLE HeapCreate(

DWORD fdwOptions,

SIZE_T dwInitialSize,

SIZE_T dwMaximumSize);

第一个参数fdwOptions用于修改如何在堆栈上执行各种操作，可以设定0、HEAP_NO_SERIALIZE、HEAP_GENERATE_EXCEPTIONS或者组合标志。按照默认设置，堆栈将顺序访问自己，这样多个线程就能够分配和释放堆栈中的内存块而不至于破坏堆栈，但试图从堆栈中分配一个内存块时，HeapAlloc函数必须执行下列操作：

1）遍历分配的和释放的内存块的链接表；

2）寻找一个空闲内存块的地址；

3）通过将空闲内存块标记为“已分配”分配新内存块；

4）将新内存块添加给内存块链接表；

从堆栈中分配内存块，调用HeapAlloc函数：

PVOID HeapAlloc(

HANDLE hHeap,

DWORD fdwFlags,

SIZE_T dwBytes);

第一个参数hHeap用于标识分配的内存块来自的堆栈的句柄，dwBytes参数用于设定从堆栈中分配的内存块的字节数。

调用HeapReAlloc函数来改变内存块的大小：

PVOID HeapReAlloc(

HANDLE hHeap,

DWORD fdwFlags,

PVOID pvMem,

SIZE_T dwBytes)；

当内存块分配后，可调用HeapSize函数来检索内存块的实际大小：

SIZE_T HeapSize(

HANDLE hHeap,

DWORD fdwFlags,

LPCVOID pvMem);

参数hHeap用于标识堆栈，参数pvMem用于指明内存块的地址，参数fdwFlags既可以是0，也可以是HEAP_NO_SERIALIZE。

当不再需要内存块时，可以调用HeapFree函数将其释放：

BOOL HeapFree(

HANDLE hHeap,

DWORD fdwFlags,

PVOID pvMem);

HeapFree函数用于释放内存块，如运行成功，则返回TRUE。

如果应用程序不再需要它创建的堆栈，可通过调用HeapDestroy函数将其撤消：

BOOL HeapDestroy(HANLDE hHeap);

调用HeapDestroy函数可以释放堆栈中包含的所有内存块，也可以将堆栈占用的物理存储器和保留的地址空间区域重新返回给系统。



