# DLL和exe里的malloc和free不能混用 - gauss的专栏 - CSDN博客
2013年01月04日 00:11:47[gauss](https://me.csdn.net/mathlmx)阅读数：256
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
                
我们在进行VC++编程时可能会经常遇到下面这种提示：
其原因可能是堆被损坏，这说明**.exe中或它加载的任何DLL中有Bug
在网上搜了一下资料，如下说明：
一个模块一个堆，一个线程一个栈。
dll里malloc的内存，在exe里free会出错。
CRT（C运行时期库）不是使用进程缺省的堆来实现malloc（new中调用malloc）的，而是使用一个全局句柄HANDLE _crtheap来分配内存的。这个_crtheap是在XXXCRTStartUp（CRT提供的进口点函数）中创建的。 
由于CRT静态连接，则楼主的DLL里有也有一个CRT，因此也有一个_crtheap。而在dll中的new使用dll中的_crtheap句柄分配堆，在exe中的delete使用exe中的_crtheap释放堆，当然失败！
解决办法：
1。在DLL中输出一个函数给EXE调用，专门用来释放由DLL分配的内存；
2。用GlobalAlloc()代替new，用GlobalFree()代替delete；
3。使用单一的堆，分配内存使用HeapAlloc(GetProcessHeap(),0,size)，释放内存使用HeapFree(GetProcessHeap(),0,p)；
4。把dll和exe的Settings的C/C++选项卡的Code Generation的Use Run-time liberary改成Debug Multithreaded DLL，在Release版本中改成Multithreaded DLL；这样使用一个CRT了——MSVCRT.DLL。
||**C语言**|**C++语言**|**Windows 平台**|**COM**|**IMalloc 接口**|**BSTR**|
|----|----|----|----|----|----|----|
|**申请**|malloc()|new|GlobalAlloc()|CoTaskMemAlloc()|Alloc()|SysAllocString()|
|**重新申请**|realloc()||GlobalReAlloc()|CoTaskRealloc()|Realloc()|SysReAllocString()|
|**释放**|free()|delete|GlobalFree()|CoTaskMemFree()|Free()|SysFreeString()|
　　以上这些函数必须要按类型配合使用（比如：new 申请的内存，则必须用 delete 释放）。在 COM 内部，当然你可以随便使用任何类型的内存分配释放函数，但组件如果需要与客户进行内存的交互，则必须使用上表中的后三类函数族。IMalloc 接口又是对 CoTaskXXX() 函数族的一个包装。包装后，同时增强了一些功能，比如：IMalloc::GetSize()可以取得尺寸，使用 IMallocSpy 可以监视内存的使用。
