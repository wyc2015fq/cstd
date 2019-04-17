# DLL的高级操作技术——Windows核心编程学习手札之二十 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年09月08日 09:53:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2511标签：[dll																[windows																[编程																[thread																[microsoft																[winapi](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)
个人分类：[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)





# DLL的高级操作技术

## ——Windows核心编程学习手札之二十

显示加载DLL模块：

HINSTANCE LoadLibrary(PCTSTR pszDLLPathName);

HINSTANCE LoadLibraryEx(

PCTSTR pszDLLPathName,

HANDLE hFile,

DWORD dwFlags);

上面两个函数找出用户系统上的文件映像，并将DLL的文件映像映射到调用进行的地址空间中，函数返回的HINSTANCE值用于标识文件映像映射到的虚拟内存地址，如果DLL不能被映射到进程的地址空间，则返回NULL，调用GetLastError可了解详细错误信息。LoadLibraryEx函数中的参数hFile保留使用，现在传递NULL；参数dwFlags有如下设置意义：

1）DON’T_RESOLVE_DLL_REFERENCES

传递该参数告诉系统将DLL映射到调用进程的地址空间中，有两点作用：一是DLL被映射到进程的地址空间中，通常要调用DLL的入口函数DllMain，该标志则告诉系统不需要调用DllMain；二是当DLL内部还需要调用另一个DLL，系统一般会自动加载这些被该DLL所调用的DLL，该标志告诉系统不自动将其他DLL加载到进程的地址空间中。

2）LOAD_LIBIRARY_AS_DATAFILE

该标志在下面情况是有用的：只包含资源不包含函数的DLL，传递该标志下使DLL的文件映像能够映射到进程的地址空间中。

3）LOAD_WITH_ALTERED_SEARCH_PATH

设置该标志按照下面顺序搜索文件：pszDLLPathName参数中设定的目录，进程的当前目录，Windows的系统目录，Windows目录，Path环境变量中列出的目录；

显示卸载DLL模块：

当进程中的线程不再需要DLL中的引用符号时，可以从进程的地址空间中显示卸载DLL：

BOOL FreeLibrary(HINSTANCE hinstDll);

传递HINSTANCE值，以便标识要卸载的DLL，也通过调用下面的函数从进程的地址空间中卸载DLL：

VOID FreeLibraryAndExitThread(

HINSTANCE hinstDll,

DWORD dwExitCode);

该函数是在Kernel32.dll中实现的，如下：

VOID FreeLibraryAndExitThread(HINSTANCE hinstDll,

DWORD dwExitCode){

FreeLibrary(hinstDll);

ExitThread(dwExitCode);}

该函数适用于下面情形：编写一个DLL，当它被初次映射到进程的地址空间中，该DLL便创建一个线程，当所创建的线程完成操作时，通过调用FreeLibrary函数从进程的地址空间卸载DLL，并且终止运行，然后立即调用ExitThread，如果线程分开调用FreeLibrary和ExitThread，就会出现问题，问题是调用FreeLibrary会立即从进程的地址空间中卸载DLL，返回时，包含对ExitThread调用的代码就不可以使用，因此线程将无法执行任何代码，导致访问违规，终止整个进程。如果线程调用FreeLibraryAndExitThread，函数调用FreeLibrary使DLL被卸载，而下步执行指令是在Kernel32.dll中（不是刚被卸载的DLL），意味着线程能够继续执行下去调用ExitThread终止线程且不返回。

显示链接到一个输出符号：

一旦DLL模块被显示加载，线程就要获取引用的符号的地址，方法是：

FARPROC GetProcAddress(

HINSTANCE hinstDll,

PCSTR pszSymbolName);

如：FARPROC pfn=GetProcAddress(hinstDll,”someFunInDll”);

参数pszSymbolName的原型是PCSTR而非PCTSTR，因此只接受ANSI字符串而不能将Unicode字符串传递给该函数，因为编译器和链接程序总是将符号名作为ANSI字符串存储在DLL的输出节中，参数pszSymbolName也可以指明想要的地址的符号的序号，如：

FARPROC pfn=GetProcAddress(hinstDll,MAKEINTRESOURCE(2));

假设知道调用的函数序号是被赋予了2的值。

DLL的进入点函数：

一个DLL拥有单个进入点函数：

BOOL WINAPI DllMain(HINSTANCE hinstDll,DWORD fdwReason,PVOID fImpLoad){

switch(fdwReason){

case DLL_PROCESS_ATTACH:

break;

case DLL_THREAD_ATTACH:

break;

case DLL_THREAD_DETACH:

break;

case DLL_PROCESS_DETACH:

break;

}

Return TRUE;

}

1）DLL_PROCESS_ATTACH通知

当DLL被初次映射到进程的地址空间时，系统将调用该DLL的DllMain函数，传递fdwReason的值即为DLL_PROCESS_ATTACH。

2）DLL_PROCESS_DETACH通知

DLl从进程的地址空间中被卸载时，系统将调用DLL的DllMain函数，fdwReason即传递该值。

3）DLL_THREAD_ATTACH通知

当在一个进程中创建线程时，系统要查看当前映射到该进程的地址空间中的所有DLL文件映像，并调用每个文件映像带有DLL_THREAD_ATTACH值DllMain函数，告诉所有的DLL执行每个线程的初始化操作，新创建的线程负责执行DLL的所有DllMain函数中的代码，只有当所有的DLL都有机会处理该通知时，系统才运行新线程开始执行其线程函数。

4）DLL_THREAD_DETACH通知

线程终止的首选是使其线程函数返回，调用ExitThread函数告诉系统，线程要终止运行，但系统并不立即撤消，相反系统取出即将被撤消的线程，并让它调用已经映射的DLL的所有带有DLL_THREAD_DETACH值DllMain函数，通知所有的DLL执行每个线程的清除操作。

DllMain与C/C++运行期库：

如使用Microsoft的Visual C++编译器来创建DLL，需要得到C/C++运行期库的初始帮助，如创建的DLL中包含一个全局变量，而这个全局变量是个C++类的实例，在DllMain函数使用该全局变量之前，需调用其构造函数，这个工作由C/C++运行期库的DLL启动代码来完成。当链接DLL时，链接程序将DLL的进入点函数嵌入产生的DLL文件映像，使用链接程序的/ENTRY开关来设定该函数的地址。按照默认设置，使用Microsoft的链接程序并设定/DLL开关时，链接程序假设进入点函数称为_DllMainCRTStartup，该函数包含在C/C++运行期的库文件中，并且在链接DLL时被静态链接到DLL的文件映像中。当DLL文件映像被映射到进程的地址空间中时，系统实际先调用_DllMainCRTStartup函数，而不是调用DllMain函数。_DllMainCRTStartup函数负责对C/C++运行期库进行初始化，并且确保_DllMainCRTStartup收到DLL_PROCESS_ATTACH通知时创建任何全局或静态C++对象，当执行任何C/C++运行期初始化时，_DllMainCRTStartup函数将调用DllMain函数。当DLL调用DLL_PROCESS_DETACH通知时，系统再次调用_DllMainCRTStartup函数，该函数即调用DllMain函数，当DllMain返回时，_DllMainCRTStartup就为DLL中的任何全局或静态C++对象调用析构函数。

延迟加载DLL：

延迟加载DLL是个隐含连接的DLL，是等到代码引用DLL中包含的一个符号时才进行加载，对下列情况比较有用：

1）应用程序使用若干DLL，初始化时间就比较长，此时在进程运行时分开加载各个DLL，则不需要一次将所有DLL映射到进程地址空间中；

2）如果调用代码中的一个新函数，在无该函数的旧版本系统上运行，加载程序会报告错误，并且不允许应用程序运行。延迟加载可以先让应用程序运行，运行时发现新函数在旧版本系统上无法运行，则不调用该函数。

延迟加载都像平常一样创建DLL模块及其可执行模块，但需修改两个链接程序开关，并且重新链接可执行模块：

/Lib:DelayImp.lib

/DelayLoad:MyDll.dll

Lib开关告诉链接程序将一个特殊函数delayLoadHelper嵌入可执行模块，DelayLoad开关则告诉链接程序：

1）从可执行模块的输入节中删除MyDll.dll，当进程初始化时，操作系统的加载程序就不会显示加载DLL；

2）将新的Delay Import（延迟输入）节，也称为.tidata嵌入可执行模块，以指明那些函数正在从MyDll.dll中输入；

3）通过转移到对delayLoadHelper函数的调用，转换到对延迟加载函数的调用；

若要卸载延迟加载的DLL，首先创建可执行文件时，设定另一个链接开关程序/delay:unload，其次，修改源代码，在想要卸载DLL是调用：

BOOL __FunLoadDelayLoadedDLL(PCSTR szDll)；

链接程序开关/delay:unload该素链接程序将另一个节放入文件中，该节包含了清除已经调用的函数时需要的信息，可以再次调用delayLoadHelper函数。当调用__FunLoadDelayLoadedDLL时，想要卸载的延迟加载的DLL名字传递，该函数进入文件中的未卸载节，并清除DLL的所有函数地址，然后__FunLoadDelayLoadedDLL调用FreeLibrary卸载该DLL。注意，不能自己调用FreeLibrary来卸载DLL，否则函数的地址将不会被清除，这样，当下次调用DLL中的函数时，会导致访问违规。注意二，调用__FunLoadDelayLoadedDLL时，传递的DLL名字不应该包含路径，名字中的字母应和将DLL名字传递给/delayload链接程序开关时使用的字母大小写相同。注意三，如果不打算卸载延迟加载的DLL，则不要设置/delay:unload链接程序开关，且可执行文件的长度应该较小。](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)




