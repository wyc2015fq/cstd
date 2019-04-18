# VC编译选项 MT MTd MD MDd 全面分析 - 长歌行 - CSDN博客





2012年02月03日 12:21:54[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1908标签：[多线程																[dll																[编译器																[thread																[library																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)





**VC编译选项**

多线程    (/MT)

多线程调试(/MTd)

多线程 DLL(/MD)

多线程调试 DLL (/MDd)
**C 运行时库                        库文件**

Single thread(static link) ML           libc.lib

Debug single thread(static link) MLd       
 libcd.lib

MultiThread(static link) MT            libcmt.lib

Debug multiThread(static link) MTd           
libcmtd.lib

MultiThread(dynamic link) MD            msvert.lib

Debug multiThread(dynamic link) MDd       msvertd.lib 


**各种 C 运行时库的区别**

（ 1 ）静态链接的单线程库 Single thread(static link) ML

静态链接的单线程库只能用于单线程的应用程序， C 运行时库的目标代码最终被编译在应用程序的二进制文件中。通过 /ML 编译选项可以设置 Visual C++ 使用静态链接的单线程库。



（ 2 ）静态链接的多线程库  MultiThread(static link) MT

静态链接的多线程库的目标代码也最终被编译在应用程序的二进制文件中，但是它可以在多线程程序中使用。通过 /MT 编译选项可以设置 Visual C++ 使用静态链接的多线程库。



（ 3 ）动态链接的运行时库  

动态链接的运行时库将所有的 C 库函数保存在一个单独的动态链接库 MSVCRTxx.DLL 中， MSVCRTxx.DLL 处理了多线程问题。使用 /MD 编译选项可以设置 Visual C++ 使用动态链接的运行时库。

/MDd 、 /MLd 或 /MTd 选项使用 Debug runtime library( 调试版本的运行时刻函数库 ) ，与 /MD 、 /ML 或 /MT 分别对应。 Debug 版本的 Runtime Library 包含了调试信息，并采用了一些保护机制以帮助发现错误，加强了对错误的检测，因此在运行性能方面比不上 Release 版本。 



**下面是msdn关于Visual C++ 编译器选项的说明：**

这些选项选择单线程或多线程运行时例程，指示多线程模块是否为 DLL，并选择运行时库的发布版本或调试版本。

选项     说明

/MD     定义 _MT 和 _DLL 以便同时从标准 .h 文件中选择运行时例程的多线程特定版本和 DLL 特定版本。此选项还使编译器将库名 MSVCRT.lib 放入 .obj 文件中。

用此选项编译的应用程序静态链接到 MSVCRT.lib。该库提供允许链接器解析外部引用的代码层。实际工作代码包含在 MSVCR71.DLL 中，该库必须在运行时对于与 MSVCRT.lib 链接的应用程序可用。

当在定义了 _STATIC_CPPLIB (/D_STATIC_CPPLIB) 的情况下使用 /MD 时，它将导致应用程序通过静态多线程标准 C++ 库 (libcpmt.lib) 而非动态版本 (msvcprt.lib) 进行链接，同时仍通过 msvcrt.lib 动态链接到主 CRT。

/MDd     定义 _DEBUG、_MT 和 _DLL，以便从标准 .h 文件中选择运行时例程的调试多线程特定版本和 DLL 特定版本。它还使编译器将库名 MSVCRTD.lib 放入 .obj 文件中。

/ML     使编译器将库名 LIBC.lib 放入 .obj 文件中，以便链接器使用 LIBC.lib 解析外部符号。这是编译器的默认操作。LIBC.lib 不提供多线程支持。

/MLd     定义 _DEBUG 并使编译器将库名 LIBCD.lib 放入 .obj 文件中，以便链接器使用 LIBCD.lib 解析外部符号。LIBCD.lib 不提供多线程支持。

/MT     定义 _MT，以便从标准头 (.h) 文件中选择运行时例程的多线程特定版本。此选项还使编译器将库名 LIBCMT.lib 放入 .obj 文件中，以便链接器使用 LIBCMT.lib 解析外部符号。创建多线程程序需要 /MT     或 /MD（或它们的调试等效选项 /MTd 或 /MDd）。

/MTd     定义 _DEBUG 和 _MT。定义 _MT 会导致从标准 .h 文件中选择运行时例程的多线程特定版本。此选项还使编译器将库名 LIBCMTD.lib 放入 .obj 文件中，以便链接器使用 LIBCMTD.lib 解析外部符号。创    建多线程程序需要 /MTd 或 /MDd（或它们的非调试等效选项 /MT 或 MD）。

/LD     创建 DLL。

将 /DLL 选项传递到链接器。链接器查找 DllMain 函数，但并不需要该函数。如果没有编写 DllMain 函数，链接器将插入返回 TRUE 的 DllMain 函数。

链接 DLL 启动代码。

如果命令行上未指定导出 (.exp) 文件，则创建导入库 (.lib)；将导入库链接到调用您的 DLL 的应用程序。

将 /Fe 解释为命名 DLL 而不是 .exe 文件；默认程序名成为基名称.dll 而不是基名称.exe。

如果还未显式指定 /M 选项之一，则将默认运行时库支持更改为 /MT。

/LDd     创建调试 DLL。定义 _DEBUG。

    警告   不要混合使用运行时库的静态版本和动态版本。在一个进程中有多个运行时库副本会导致问题，因为副本中的静态数据不与其他副本共享。链接器禁止在 .exe 文件内部既使用静态版本又使用动态版本链接，但您仍可以使用运行时库的两个（或更多）副本。例如，当与用动态 (DLL) 版本的运行时库链接的 .exe 文件一起使用时，用静态（非 DLL）版本的运行时库链接的动态链接库可能导致问题。（还应该避免在一个进程中混合使用这些库的调试版本和非调试版本）。](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)




