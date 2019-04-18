# LocalAlloc,VirtualAlloc,malloc,new的异同 - ljx0305的专栏 - CSDN博客
2010年05月18日 14:57:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：500标签：[windows																[编程																[编译器																[语言																[delete																[access](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[VC																[MFC](https://blog.csdn.net/ljx0305/article/category/403913)](https://blog.csdn.net/ljx0305/article/category/401790)
1. 首先我们来看HeapAlloc：
MSDN上的解释为：HeapALloc是从堆上分配一块内存，且分配的内存是不可移动的（即如果没有连续的空间能满足分配的大小，程序不能将其他零散的 空间利用起来，从而导致分配失败），该分配方法是从一指定地址开始分配，而不像GloabalAlloc是从全局堆上分配，这个有可能是全局，也有可能是 局部。函数原型为：
LPVOID
HeapAlloc(
    HANDLE hHeap,
    DWORD dwFlags,
   SIZE_T dwBytes
    );
hHeap是进程堆内存开始位置。
dwFlags是分配堆内存的标志。包括HEAP_ZERO_MEMORY，即使分配的空间清零。
dwBytes是分配堆内存的大小。
其对应的释放空间函数为HeapFree。
2. 再看GlobalAlloc：该函数用于从全局堆中分配出内存供程序使用，函数原型为：
HGLOBAL GlobalAlloc(
UINT uFlags,
SIZE_T dwBytes
);
uFlags参数含义
GHND   GMEM_MOVEABLE和GMEM_ZEROINIT的组合
GMEM_FIXED   分配固定内存，返回值是一个指针
GMEM_MOVEABLE   分配活动内存，在Win32中,内存块不能在物理内存中移动，但能在默认的堆中移动。返回值是内存对象的句柄，用函数GlobalLock可将句柄转化为指针
GMEM_ZEROINIT   将内存内容初始化为零
GPTR   GMEM_FIXED和GMEM_ZEROINIT的组合
一般情况下我们在编程的时候，给应用程序分配的内存都是可以移动的或者是可以丢弃的，这样能使有限的内存资源充分利用，所以，在某一个时候我们分配的那块 内存的地址是不确定的，因为他是可以移动的，所以得先锁定那块内存块，这儿应用程序需要调用API函数GlobalLock函数来锁定句柄。如下： lpMem=GlobalLock(hMem); 这样应用程序才能存取这块内存。所以我们在使用GlobalAllock时，通常搭配使用GlobalLock，当然在不使用内存时，一定记得使用 GlobalUnlock，否则被锁定的内存块一直不能被其他变量使用。
GlobalAlloc对应的释放空间的函数为GlobalFree。
3.　LocalAlloc：该函数用于从局部堆中分配内存供程序使用，函数原型为：
HLOCAL LocalAlloc(
UINT uFlags,
SIZE_T uBytes
);
参数同GlobalAlloc。
在16位Windows中是有区别的,因为在16位windows用一个全局堆和局部堆来管理内存，每一个应用程序或dll装入内存时，代码段被装入全局 堆，而系统又为每个实例从全局堆中分配了一个64kb的数据段作为该实例的局部堆，用来存放应用程序的堆栈和所有全局或静态变量。而 LocalAlloc/GlobalAlloc就是分别用于在局部堆或全局堆中分配内存。 
由于每个进程的局部堆很小，所以在局部堆中分配内存会受到空间的限制。但这个堆是每个进程私有的，相对而言分配数据较安全，数据访问出错不至于影响到整个系统。 
而在全局堆中分配的内存是为各个进程共享的，每个进程只要拥有这个内存块的句柄都可以访问这块内存，但是每个全局内存空间需要额外的内存开销，造成分配浪费。而且一旦发生严重错误，可能会影响到整个系统的稳定。 
不过在Win32中，每个进程都只拥有一个省缺的私有堆，它只能被当前进程访问。应用程序也不可能直接访问系统内存。所以在Win32中全局堆和局部堆都 指向进程的省缺堆。用LocalAlloc/GlobalAlloc分配内存没有任何区别。甚至LocalAlloc分配的内存可以被 GlobalFree释放掉。所以在Win32下编程，无需注意Local和Global的区别，一般的内存分配都等效于 HeapAlloc(GetProcessHeap(),...)。
LocalAlloc对应的释放函数为LockFree。
4．　VirtualAlloc：该函数的功能是在调用进程的虚地址空间,预定或者提交一部分页，如果用于内存分配的话,并且分配类型未指定 MEM_RESET,则系统将自动设置为0;其函数原型：
LPVOID VirtualAlloc(
LPVOID lpAddress, // region to reserve or commit
SIZE_T dwSize, // size of region
DWORD flAllocationType, // type of allocation
DWORD flProtect // type of access protection
);
VirtualAlloc可以通过并行多次调用提交一个区域的部分或全部来保留一个大的内存区域。多重调用提交同一块区域不会引起失败。这使得一个应用程 序保留内存后可以随意提交将被写的页。当这种方式不在有效的时候，它会释放应用程序通过检测被保留页的状态看它是否在提交调用之前已经被提交。
VirtualAlloc对应的释放函数为VirtualFree。
5．Malloc：malloc与free是C++/C语言的标准库函数，可用于申请动态内存和释放内存。对于非内部数据类型的对象而言，光用 malloc/free无法满足动态对象的要求。对象在创建的同时要自动执行构造函数，对象在消亡之前要自动执行析构函数。由于malloc/free是 库函数而不是运算符，不在编译器控制权限之内，不能够把执行构造函数和析构函数的任务强加于malloc/free。
6．New：new/delete是C++的运算符。可用于申请动态内存和释放内存。C++语言需要一个能完成动态内存分配和初始化工作的运算符new， 以一个能完成清理与释放内存工作的运算符delete。注意new/delete不是库函数。C++程序经常要调用C函数，而C程序只能用malloc /free管理动态内存。new 是个操作符,和什么"+","-","="...有一样的地位. 
        malloc是个分配内存的函数,供你调用的. 
        new是保留字,不需要头文件支持. 
        malloc需要头文件库函数支持.new 建立的是一个对象, 
        malloc分配的是一块内存. 
        new建立的对象你可以把它当成一个普通的对象,用成员函数访问,不要直接访问它的地址空间 
        malloc分配的是一块内存区域,就用指针访问好了,而且还可以在里面移动指针.
内存泄漏对于malloc或者new都可以检查出来的，区别在于new可以指明是那个文件的那一行，而malloc没有这些信息。new可以认为是 malloc加构造函数的执行。new出来的指针是直接带类型信息的。而malloc返回的都是void指针。
引用：[http://hi.baidu.com/gamedot/blog/item/6fad84a78dbdb19fd0435846.html](http://hi.baidu.com/gamedot/blog/item/6fad84a78dbdb19fd0435846.html)
