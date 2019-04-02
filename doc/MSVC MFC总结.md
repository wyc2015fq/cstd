# MSVC MFC总结



# 关于编译、链接、装载、库

**转载自http://www.cnblogs.com/qinfengxiaoyue/archive/2013/02/07/2909051.html**



## **1、Run-Time Library（运行时库）**

Run-Time Library是编译器提供的标准库，提供一些基本的库函数和系统调用。

我们一般使用的Run-Time Library是C Run-Time Libraries。当然也有Standard C++ libraries。

C Run-Time Libraries实现ANSI C的标准库。VC安装目录的CRT目录有C Run-Time库的大部分源代码。

C Run-Time Libraries有静态库版本，也有动态链接库版本；有单线程版本，也有多线程版本；还有调试和非调试版本。

可以在"project"-"settings"-"C/C++"-"Code Generation"中选择Run-Time Library的版本。

动态链接库版本：

/MD Multithreaded DLL 使用导入库MSVCRT.LIB，对应MSVCRT.DLL

/MDd Debug Multithreaded DLL 使用导入库MSVCRTD.LIB，对应MSVCRTD.DLL

静态库版本：

/MT Multithreaded 使用静态库LIBCMT.LIB

/MTd Debug Multithreaded 使用静态库LIBCMTD.LIB

**注**：

1.单线程的运行库已废止.

2.标准C++库的选择同于CRT的选择，但是由编译器默认添加的。即，若使用了动态版本的CRT且使用了标准C++库中的相关函数，则编译器默认添加使用标准C++库的动态版本：msvcprt.lib（对应msvcpr.dll）；否则使用标准C++库的静态版本libcpmt.lib.

C Run-Time Library的标准io部分与操作系统的关系很密切，在Windows上，CRT的io部分代码只是一个包装，底层要用到

操作系统内核kernel32.dll中的函数，在编译时使用导入库kernel32.lib。这也就是为什么在嵌入式环境中，我们一般不能直接

使用C标准库。

在Linux环境当然也有C标准库，例如：

ld -o output /lib/crt0.o hello.o -lc

参数"-lc"就是在引用C标准库libc.a。猜一猜"-lm"引用哪个库文件？



## **2、常见的编译参数**

VC建立项目时总会定义"Win32"。控制台程序会定义"_CONSOLE"，否则会定义"_WINDOWS"。Debug版定义"_DEBUG"，

Release版定义"NDEBUG"。

与MFC DLL有关的编译常数包括：

_WINDLL 表示要做一个用到MFC的DLL

_USRDLL 表示做一个用户DLL（相对MFC扩展DLL而言）

_AFXDLL 表示使用MFC动态链接库

_AFXEXT 表示要做一个MFC扩展DLL

所以：

Regular, statically linked to MFC ：_WINDLL,_USRDLL

Regular, using the shared MFC DLL： _WINDLL,_USRDLL,_AFXDLL

Extension DLL ：_WINDLL,_AFXDLL,_AFXEXT

CL.EXE编译所有源文件，LINK.EXE链接EXE和DLL，LIB.EXE产生静态库。



**附**（转自：TN011: [Using MFC as Part of a DLL](http://technet.microsoft.com/zh-cn/subscriptions/zfz4xb9a(v=vs.71).aspx) ）：

When compiling regular DLLs that statically link to MFC, the symbols "**_USRDLL**" and "**_WINDLL**" must be defined. Your DLL code must also be compiled with the following compiler switches:

- /D_WINDLL signifies the compilation is for a DLL
- /D_USRDLL specifies you are building a regular DLL

When compiling regular DLLs that dynamically link to MFC, you must define the above symbols and use the above compiler switches. Additionally, the symbol "**_AFXDLL**" must be defined and your DLL code must be compiled with:

- /D_AFXDLL specifies that you are building a regular DLL that dynamically links to MFC

The interfaces (APIs) between the application and the DLL must be explicitly exported. It is recommended that you define your interfaces to be low bandwidth, sticking to C interfaces where possible. More direct C interfaces are easier to maintain than more complex C++ classes.

Place your APIs in a separate header that can be included by both C and C++ files (that way you won't limit your DLL customers to C++ programmers). See the header ScreenCap.h in the MFC Advanced Concepts sample [DLLScreenCap](http://technet.microsoft.com/zh-cn/subscriptions/ms385284(v=vs.71).aspx) for an example. To export your functions, enter them in the **EXPORTS** section of your module definition file (.DEF) or include **__declspec(dllexport)** on your function definitions. Use **__declspec(dllimport)** to import these functions into the client executable.

You must add the **AFX_MANAGE_STATE** macro at the beginning of all the exported functions in regular DLLs that dynamically link to MFC to set the current module state to the one for the DLL. This is done by adding the following line of code to the beginning of functions exported from the DLL:

``

```
AFX_MANAGE_STATE(AfxGetStaticModuleState( ))
```





## **3、subsystem和可执行文件的启动**

LINK的时候需要指定/subsystem，这个链接选项告诉Windows如何运行可执行文件。

控制台程序是/subsystem:"console"

其它程序一般都是/subsystem:"windows "

将 subsystem 选成"console"后，Windows在进入可执行文件的代码前（如mainCRTStartup），就会产生一个控制台窗

口。

如果选择"windows"，操作系统就不产生console窗口,该类型应用程序的窗口由用户自己创建。

可执行文件都有一个Entry Point，LINK时可以用/entry指定。

缺省情况下，如果subsystem是“console”，Entry Point是 mainCRTStartup(ANSI）或 wmainCRTStartuup(UNICODE)，即：

/subsystem:"console" /entry:"mainCRTStartup" (ANSI)

/subsystem:"console" /entry:"wmainCRTStartuup" (UNICODE)

mainCRTStartup 或 wmainCRTStartuup 会调用main或wmain。

值得一提的是，在进入应用程序的Entry Point前，Windows的装载器已经做过C变量的初始化，有初值的全局变量拥有了它们

的初值，没有初值的变量被设为0。

如果subsystem是“windows”，Entry Point是WinMain(ANSI)或wWinMain(UINCODE)，即：

/subsystem:"windows" /entry:"WinMainCRTStartup" (ANSI)

/sbusystem:"windows" /entry:"wWinMainCRTStartup" (UINCODE)

WinMainCRTStartup 或 wWinMainCRTStartup 会调用 WinMain 或 wWinMain。

这些入口点函数，在CRT目录都可以看到源代码，例如（为了简洁，我删除了原代码的一些条件编译）：

```cpp

void mainCRTStartup(void)
{
	int mainret;
	/* Get the full Win32 version */
	_osver = GetVersion();
	_winminor = (_osver >> 8) & 0x00FF ;
	_winmajor = _osver & 0x00FF ;
	_winver = (_winmajor << 8) + _winminor;
	_osver = (_osver >> 16) & 0x00FFFF ;

#ifdef _MT
	if ( !_heap_init(1) ) /* initialize heap */
#else /* _MT */
	if ( !_heap_init(0) ) /* initialize heap */
#endif /* _MT */
		fast_error_exit(_RT_HEAPINIT); /* write message and die */
#ifdef _MT
	if( !_mtinit() ) /* initialize multi-thread */
		fast_error_exit(_RT_THREAD); /* write message and die */
#endif /* _MT */
	__try {
		_ioinit(); /* initialize lowio */
		_acmdln = (char *)GetCommandLineA(); /* get cmd line info */
		_aenvptr = (char *)__crtGetEnvironmentStringsA(); /* get environ info */
		_setargv();
		_setenvp();
		__initenv = _environ;
		mainret = main(__argc, __argv, _environ);
		exit(mainret);
	}
	__except ( _XcptFilter(GetExceptionCode(), GetExceptionInformation()) )
	{
		_exit( GetExceptionCode() ); /* Should never reach here */
	} /* end of try - except */
} 


```

如果使用MFC框架，WinMain也会被埋藏在MFC库中（APPMODUL.CPP）：



```cpp

extern "C" int WINAPI
	_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,	LPTSTR lpCmdLine, int nCmdShow)
{
	// call shared/exported WinMain
	return AfxWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
```

对于ANSI版本，"_tWinMain"就是"WinMain"；对于UINCODE版本，"_tWinMain"就是"wWinMain"。可参见



afx.h：

```cpp

#ifdef _UNICODE

#define _tmain wmain

#define _tWinMain wWinMain

#else

#define _tmain main

#define _tWinMain WinMain

#endif

```



全局C++对象的构造函数是在什么地方调用的？答案是在进入应用程序的Entry Point后，在调用main函数前的初始化操作

中。所以MFC的theApp的构造函数是在_tWinMain之前调用的。

## **4、不显示Console窗口的Console程序**

在默认情况下/subsystem 和/entry开关是匹配的,也就是：

"console"对应"mainCRTStartup"或者"wmainCRTStartup".

"windows"对应"WinMain"或者"wWinMain".

我们可以通过手动修改的方法使他们不匹配。例如：

```cpp

#include "windows.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址 
void main(void)
{
	MessageBox(NULL, "hello", "Notice", MB_OK);
}
```

这个Console程序就不会显示Console窗口。如果选/MLd的话，这个程序只需要链接LIBCD.LIB user32.lib kernel32.lib。

其实如果不想看到Console窗口，还有一个更直接的方法：那就是直接在EXE文件中将PE文件头的Subsystem从3改成2。在

EXE文件中，PE文件头的偏移地址是0x3c，Subsystem是一个WORD，它在PE文件头中的偏移是0x5c。



## **5、MFC的库文件**

MFC的库可以静态链接，也可以动态链接。静态库和动态库又有Debug和Release，ANSI和Unicode版本之分。

静态MFC库主要有：

ANSI Debug   : NAFXCWD.LIB

ANSI Release : NAFXCW.LIB

Unicode Debug  : UAFXCWD.LIB

Unicode Release: UAFXCW.LIB

动态链接库主要有:

ANSI Debug:

MFCxxD.LIB (core，MFCxxD.DLL),

MFCOxxD.LIB (OLE，MFCOxxD.DLL),

MFCDxxD.LIB (database，MFCDxxD.DLL),

MFCNxxD.LIB (network，MFCNxxD.DLL),

MFCSxxD.LIB (static)

ANSI Release:

MFCxx.LIB (combined，MFCxx.DLL)

MFCSxx.LIB (static)

Unicode Debug:

MFCxxUD.LIB (core，MFCxxUD.DLL),

MFCOxxUD.LIB (OLE，MFCOxxUD.DLL),

MFCDxxUD.LIB (database，MFCDxxUD.DLL),

MFCNxxUD.LIB (network，MFCNxxUD.DLL),

MFCSxxUD.LIB (static)

Unicode Release:

MFCxxU.LIB(combined，MFCxxU.DLL),

MFCSxxU.LIB (static)



上面的LIB文件除了MFCSxx(D、U、UD).LIB以外都是导入库。

MFC动态链接库版本也需要静态链接一些文件，这些文件就放在MFCSxx(D、U、UD).LIB中。例如包含_tWinMain的

appmodul.cpp。



## **6、结束语**

研究这些问题的动机是想弄清楚我们的程序是如何装载、运行的。但是，由于Windows不是开源平台，我也只能研究到PE文件

（Windows上可执行文件的格式）。entry point、subsystem都是PE文件头的一部分。

Windows在进入PE文件的 entry point 之前做了些什么，就看不到了，只能大概推测：应该是创建一个进程，装载PE文件和

所有需要的DLL，初始化C变量，然后从某个起点函数开始运行。不同的subsystem，应该有不同的起点。调用这个起点函数

时应该传入PE文件的entry point地址。