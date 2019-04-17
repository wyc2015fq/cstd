# 线程本地存储器——Windows核心编程学习手札之二十一 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年09月08日 17:10:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2599








# 线程本地存储器

## ——Windows核心编程学习手札之二十一

C/C++运行期库使用线程本地存储器，运行期库是在多线程应用程序出现前设计的，因此运行期库里的大多数函数是用于单线程应用程序的。函数strtok就是如此，函数初次调用strtok时，该函数传递一个字符串地址，并将字符串地址保存在自己的静态变量中，当再次调用该函数并传递NULL时，函数就引用保存的字符串地址。在多线程环境下，一个线程可以调用strtok，然后在其再次调用strtok之前，另一个线程也调用了strtok，这种情况下，第二个线程会在第一个线程不知情情况下，让strtok用一个新地址改写strtok的静态变量，而第一个线程再次调用strtok时将使用第二个线程的字符串，这样会导致难以发现和排除的错误。

为解决上面问题， C/C++运行期库使用了TLS，每个线程均被赋予自己的字符串指针，供strtok函数使用，类似的C/C++运行期库函数还有asctime和gmtime。如应用程序严重依赖全局变量和静态变量，那TLS能够解决问题，但编程应尽量减少对这些变量的使用，而更多地依赖自动（基于堆栈的）变量和通过函数的参数传递的数据，因为基于堆栈的变量总是和特定的线程联系的。

动态TLS：

先调用DWORD TlsAlloc()告诉系统对进程中的位标志进行扫描，找出FREE标志，然后系统将标志由FREE改为INUSE，并且TlsAlloc返回位数组中的标志的索引。

将一个值放入线程的数组中，调用

BOOL TlsSetValue(

DWORD dwTlsIndex,

PVOID pvTlsValue);

函数将PVOID值放入线程的数组中，由dwTlsIndex参数标识的索引处。

若要从线线程的数组中检索一个值，调用

PVOID TlsGetVluae(DWORD dwTlsIndex)；

当所有线程中不再需要保留TLS时隙的位置时，调用BOOL TlsFree(DWORD dwTlsIndex)告诉系统该时隙不再需要保留。

静态TLS：

静态TLS将数据和线程联系起来，只需要进行如下声明：

__declspec(thread) DWORD gt_dwStartTime=0；

__declspec(thread)告诉编译器，对应的变量应该放入可执行文件或DLL文件中自己的节中，后面的变量必声明为函数中（或函数外）的一个全局变量或静态变量。



