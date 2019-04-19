# DLL的进入退出——由DLL_PROCESS_ATTACH而联系搜索起来 - xqhrs232的专栏 - CSDN博客
2014年01月25日 10:16:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：692
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/lzjsky/articles/1776260.html](http://www.cnblogs.com/lzjsky/articles/1776260.html)
相关文章
1、进程线程及堆栈关系的总结----[http://hi.baidu.com/nash635/item/658aa9f645bf2011d6ff8c72](http://hi.baidu.com/nash635/item/658aa9f645bf2011d6ff8c72)
**线程可以调用GetModuleHandle函数来判断一个DLL是否被载入了进程的地址空间，**
**HINSTANCE GetModuleHandle(LPCTSTR lpszModuleName);**
**例子：**
**HINSTANCE hinstDLL;**
**hinstDLL = GetModuleHandle(“SomeDLL.dll”);**
**If (hinstDLL == NULL){**
**hinstDLL = LoadLibrary(“SomeDLL.dll”);**
**}**
**如果有了DLL的HINSTANCE值就可以使用GetModuleFileName来得到DLL的全路径名，**
**DWORD GetModuleFileName(HINSTANCE hinstModule,**
**LPTSTR lpszPath,DWORD cchPath);**
**参数说明：**
**hinstModule是DLL或EXE的HINSTANCE值，lpszPath是返回的DLL的全路径名，cchPath指定缓冲区字符大小。**
**13.2 DLL的进入/退出函数**
**这些函数常常被DLL用来执行线程级或进程级的初始化或清理工作。**
如果DLL中有这个进入/退出函数，那么这个函数必须是下面这个形式的：
**BOOL WINAPI DLLMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID fImpLoad){**
**Switch(fdwReason){**
**Case DLL_PROCESS_ATTACH:**
**//当这个DLL被映射到了进程的地址空间时**
**break;**
**case DLL_THREAD_ATTACH:**
**//一个线程正在被创建**
**break;**
**case DLL_THREAD_DETACH:**
**//线程终结**
**break;**
**case DLL_PROCESS_DETACH:**
**//这个DLL从进程的地址空间中解除映射**
**break;**
**}**
**return(TRUE);**
**}**
**参数说明：**
**hinstDLL包含DLL句柄。该值表示DLL被映射到进程地址空间内的虚拟地址。**
**fImpLoad当隐式加载时该参数非零，当DLL被显示加载时为零。**
**fdwReason：该参数指出系统为什么调用该函数。该参数有四个可能值：DLL_PROCESS_ATTACH、DLL_PROCESS_DETACH、DLL_THREAD_ATTACH、DLL_THREAD_DETACH。下面分别说明这四个可能值的作用：**
**DLL_PROCESS_ATTACH：当一个DLL被首次载入进程地址空间时，系统会调用该DLL的DLLMain函数，传递的参数fdwReason为DLL_PROCESS_ATTACH。这种情况只有在首次映射DLL时才发生。当DLLMain处理DLL_PROCESS_ATTACH时，DLLMain函数的返回值表示DLL的初始化是否成功。成功返回TRUE，否则返回FALSE。举一个在DLL_PROCESS_ATTACH通知中简单的初始化例子：使用HeapCreate来创建一个DLL要使用的堆，当然这个堆是在进程的地址空间上的。(现在描述一下隐式载入DLL时都发生了什么：当新建一个进程时，系统为该进程分配了地址空间，之后将EXE文件和所需要的DLL文件都映射到进程的地址空间。之后系统创建了进程的主线程，并使用这个主线程来调用每一个DLL中的DLLMain函数，传递给DLLMain函数的参数fdwReason的值是DLL_PROCESS_ATTACH。在所有的DLL都响应了DLL_PROCESS_ATTACH通知后，系统让进程的主线程执行EXE的C运行时启动代码，而后调用EXE文件的WinMain函数。如果有一个DLL的DLLMain函数的返回值为FALSE，说明DLL的初始化没有成功，系统就会终结整个进程，去掉所有文件映象，之后显示一个对话框告诉用户进程不能启动。再说明一下显示载入DLL时都发生了什么：首先线程(A)调用LoadLibrary或LoadLibraryEx来载入一个DLL，之后系统让线程A来调用DLL中的DLLMain函数，并传递参数fdwReason值为DLL_PROCESS_ATTACH，当DLL中的DLLMain处理完DLL_PROCESS_ATTACH通知后，线程就会从LoadLibrary返回，继续执行线程中LoadLibrary下面的代码。如果DLL中的DLLMain返回FALSE，说明初始化不成功，系统将DLL自动解除映射，使用对LoadLibrary或LoadLibraryEx的调用返回NULL。)**
**DLL_PROCESS_DETACH：当DLL从进程的地址空间解除映射时，参数fdwReason被传递的值为DLL_PROCESS_DETACH。当DLL处理DLL_PROCESS_DETACH时，DLL应该处理与进程相关的清理操作。举个例子：可以在DLL_PROCESS_DETACH阶段使用HeapDestroy来释放在DLL_PROCESS_DETACH阶段创建的堆。**
**如果进程的终结是因为系统中有某个线程调用了TerminateProcess来终结的，那么系统就不会用DLL_PROCESS_DETACH来调用DLL中的DLLMain函数来执行进程的清理工作。这样就会造成数据丢失。所以万不得以不要使用TerminateProcess来终结进程。**
**DLL_THREAD_ATTACH：该通知告诉所有的DLL执行线程的初始化。当进程创建一个新线程时，系统会查看进程地址空间中所有的DLL文件映射，之后用DLL_THREAD_ATTACH来调用DLL中的DLLMain函数。要注意：系统不会为进程的主线程使用值DLL_THREAD_ATTACH来调用DLL中的DLLMain函数。**
**DLL_THREAD_DETACH：该通知告诉所有的DLL执行线程的清理工作。注意如果线程的终结是使用TerminateThread来完成的，那么系统将不会使用值DLL_THREAD_DETACH来执行线程的清理工作，这也就是说可能会造成数据丢失，所以万不得已不要使用TerminateThread来终结线程。**
注：在编写DLL时不必一定实现DLLMain函数，当DLL源码中没有DLLMain函数时，C运行时库有它自己的一个DLLMain函数。
**13.3 从DLL中输出函数和变量**
** 在C中输出函数和变量的方法：**
** 举例子说明：如何输出函数add和变量g_nUsageCount，**
**  _declspec(dllexport) int Add(){**
**  //函数Add的函数体**
**}**
**  _declspec(dllexport) int g_nUsageCount = 0;**
** 下面说明一下从DLL中输出函数和变量时链接器所要做的一些事：首先在链接器链接DLL时，链接器检查到了关于输出函数和变量的信息，之后链接器就自动产生了一个包含DLL输出符号的LIB文件，同时还在生成的DLL文件中嵌入了一个输出符号表(这个表中包含DLL中要输出的函数和变量的名字和在DLL中对应的地址)。**
**13.4 在EXE中使用DLL输出的函数和变量**
** 下面举例子说明如何在EXE中调用从DLL中输出的函数Add和变量g_nUsageCount，**
** _declspec(dllimport) int Add();**
** _declspec(dllimport) int g_nUsageCount;**
**线程可以使用GetProcAddress函数来得到从DLL中输出的函数或变量的地址，**
** FARPROC GetProcAddress(HINSTANCE hinstDLL,LPCSTR lpszProc);**
** 参数说明：hinstDLL是DLL的句柄，lpszProc可以是DLL中函数或变量的名字，也可以是对应DLL中函数或变量的序号。**
** 举例子：GetProcAddress(hinst,”SomeDLLName”);//直接使用函数名**
**  GetProcAddress(hinst,MakeIntResource(2));//使用函数的序号**
**13.5 在EXE或DLL的多个实例中共享数据**
** 13.5.1 首先说明一下EXE或DLL中的节：**
** 每一个EXE或DLL文件都是由节的集合组成的，每个节由“.”开始，例如：编译器把所有代码都放在.text节中。如下表所示为EXE或DLL中的常用节：**
|**节名**|**内容**|
|----|----|
|**.text**|应用程序或DLL的代码|
|**.bss**|**未初始化的数据**|
|**.rdata**|**只读的运行时数据**|
|**.rsrc**|**资源**|
|**.edata**|**输出名字表**|
|**.data**|**初始化的数据**|
|**.xdata**|**异常处理表**|
|**.idata**|**引入名字表**|
|**.CRT**|只读的C运行时数据|
|**.reloc**|**修正表信息**|
|**.debug**|**调试信息**|
|**.tls**|**线程局部存储**|
**每个节都有如下访问权限：**
|**属性**|**含义**|
|----|----|
|**READ**|**该节中的字节可读取**|
|**WRITE**|**该节中的字节可写入**|
|**EXECUTE**|**该节中的字节可执行**|
|**SHARED**|**该节中的字节可被多个实例共享**|
**在代码中使用节的方法举例：**
** #pragma comment(linker,”/SECTION:SHARED,RWS”);//使节SHARED可以读、写、可共享。**
