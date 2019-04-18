# 进程——Windows核心编程学习手札系列之四 - 专注于数据挖掘算法研究和应用 - CSDN博客





2008年12月15日 17:16:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2368








# 进程

### ——Windows核心编程学习手札系列之四

进程是一个正在运行的程序的实例，有两个部分组成：一个是操作系统用来管理进程的内核对象，内核对象是系统用来存放关于进程的统计信息的地方；另一个是地址空间，它包含所有可执行模块或DLL模块的代码和数据，还包含动态内存分配的空间，如线程栈和堆分配空间。进程必须拥有一个在它的环境中运行的线程，负责执行包含在进程的地址空间中的代码。单个进程可能包含若干个线程，所有这些线程都“同时”执行进程地址空间中的代码，每个线程有自己的一组CPU寄存器和堆栈。每个进程至少拥有一个线程来执行进程地址空间中的代码，进程创建时系统会自动创建一个主线程，后该线程可以创建其他的线程。

Windows支持两种类型的应用程序，一种是基于图形用户界面GUI的应用程序，另一种是基于控制台用户界面CUI的应用程序。基于GUI的应用程序有一个图形前端程序，可创建窗口，拥有菜单，可通过对话框与用户交互，并使用所有的标准Windows组件。基于控制台的应用程序属于文本操作的应用程序，通常不创建窗口或处理消息，不需要图形用户界面，包含在屏幕上的窗口中，窗口只包含文本，命令外壳程序CMD.EXE（用于Windows2000）和COMMAND.COM（用于Windows98）就是典型的基于CUI应用程序。

Microsoft Visual C++创建应用程序时，可通过设置链接程序开关选择执行GUI或CUI，用于CUI应用程序的开关是/SUBSYSTEM:CONSOLE，GUI应用程序则是/SUBSYSTEM:WINDOWS，应用程序的开关设置在Microsoft Visual C++6.0中Project Settings的Link页。当用户运行一个程序时，操作系统的加载程序会查看可执行图形程序的标题，抓取该子系统的值，如果该值指明是CUI应用程序窗口，则加载程序会为该应用程序创建文本控制台窗口；如果是GUI应用程序，加载程序不创建控制台窗口而加载应用程序，启动后，操作系统不需要考虑应用程序提供什么样的用户界面。

Windows阴功程序必须拥有一个在应用程序启动运行时调用的进入点函数，有四个：

int WINAPI WinMain(

HINSTANCE hinstExe,

HINSTANCE,

PSTR pszCmdLine,

Int nCmdShow);

int WINAPI wWinMain(

HINSTANCE hinstExe,

HINSTANCE,

PWSTR pszCmdLine,

int nCmdShow);

int __cdecl main(

int argc,

char *argv[],

char *envp[]);

int __cdecl wmain(

int argc,

wchar_t *argv[],

wchar_t *envp[]);

操作系统并不直接执行这四个进入点函数，而是调用C/C++运行期启动函数。C/C++运行期启动函数负责对C/C++运行期库进行初始化，可调用malloc和free之类的函数，确保已经声明的任何全局对象和静态C++对象能够在代码执行以前正确地创建。下面给出了源代码中选择进入点及何时使用进入点。

======================================================================


应用程序类型进入点嵌入可执行文件的启动函数


需要ANSI字符和字符串的GUI应用程序WinMianWinMainCRTStartup

需要UNICODE字符和字符串的GUI应用程序wWinMain wWinMainCRTStartup

需要ANSI字符和字符串的CUI应用程序mainmainCRTStartup


需要UNICODE字符和字符串的CUI应用程序wmainwmainCRTStartup


链接程序负责在连接可执行文件时选择相应的C/C++运行期启动函数。如设置了/SUBSYSTEM:WINDOWS链接程序开关，那链接程序期望找到WinMain或wWinMain的进入点函数，如不存在，链接程序返回一个“未转换的外部符号”的错误消息，否则分别选择WinMainCRTStartup或wWinMainCRTStartup函数。如果应用程序中删除/SUBSYSTEM链接程序开关，链接程序会自动寻找这四个函数（Winmain/wWinMain/main/wmain）以确定进入那个子系统和可执行程序应该嵌入到那个C/C++启动函数。

上面四个C/C++运行期启动函数在执行有何区别呢？基本作用是相同的，差别是处理ANSI或UNICODE字符串，以及选择哪个进入点函数。Visual C++有C运行期库的源代码，可以在CRt0.c文件找到这四个启动函数代码，主要功能如下：

1）检索指向新进程的完整命令行的指针；

2）检索指向新进程的环境变量的指针；

3）对C/C++运行期库的全局变量进行初始化，如包含了StdLib.h文件，代码就能访问该文件里的全局变量；

4）对C运行期内存单元分配函数（malloc和calloc）和其他低层输入输出例程使用的内存栈进行初始化；

5）为所有全局和静态C++类对象调用构造函数。

这些初始化操作完成手，C/C++启动函数就调用应用程序的进入点函数。当进入点函数返回时，启动函数便调用C运行期的exit函数，将返回值（nMainRetVal）传递给Exit函数，并完成下面操作：

1）调用由_onexit函数的调用而注册的任何函数；

2）为所有全局的和静态的C++类对象调用析构函数；

3）调用操作系统的ExitProcess函数，将nMainRetVal传递进去，使操作系统能够撤消进程并设置exit代码。

加载到进程地址空间的每个可执行文件或DLL文件均被赋予一个唯一的实例句柄，可执行文件的实例作为进入点函数（w）WinMain的第一个参数hinstExe来传递。（w）WinMain的hinstExe参数的实际值是系统将可执行文件的映象加载到进程的地址空间时使用的基本地址空间。如系统打开了可执行文件并且将它的内容加载到地址0x00400000中，那么（w）WinMain的hinstExe参数的值就是0x00400000。可执行文件的映象加载到的基地址是有链接程序决定的。不同的链接程序可以使用不同的默认基地址。Visual C++链接程序使用的默认基地址是0x00400000，这是运行Windows98时可执行文件的映象可以加载到的最低地址，可以改变应用程序加载到的基地址，使用Microsoft的链接程序中的/BASE:address链接程序开关。函数GetModuleHandle返回可执行文件或DLL文件加载到进程的地址空间时所用的句柄或基地址：

HMODULE GetModuleHandle(PCTSTR pszModule);

调用该函数，传递一个以0结尾的字符串，用于设定加载到调用进程的地址空间的可执行文件或DLL文件的名字。如果系统找到了指定的可执行文件或DLL文件名，GetModuleHandle将返回该可执行文件或DLL文件映象加载到的基地址，如没有找到该文件则返回NULL。也可为pszModule参数传递NULL，则GetModuleHandle将返回调用的可执行文件的基地址，这是C运行期启动代码调用（w）WinMain函数时候代码执行的操作。该函数只查看调用进程的地址空间。

当一个进程创建时，要传递一个命令行，进程能够接收由单个字符组成的命令行，即字符串结尾处的零。当C运行期的启动代码开始运行的时候，要检索进程的命令行，跳过可执行文件的名字，将指向命令行其余部分的指针传递传递给WinMain的pszCmdLine参数。pszCmdLine参数总是指向一个ANSI字符串，如将WinMain改成wWinMain就能够访问进程的Unicode版本命令行。应用程序可以按照它选择的方法来分析和转换命令行字符串，可以写入pszCmdLine参数指向的内存缓存，但在任何情况下都不应该写到缓存外面去，因为仅是只读缓存。如果想修改命令行，首先将命令行拷贝到应用程序的本地缓存然后修改本地缓存。获得一个指向进程的完整命令行的指针，可调用方法：

PTSTR GetCommanLine();

该函数返回一个指向包含完整命令行的缓存的指针，命令行包括执行文件的完整路径名。

每个进程都有一个与它相关的环境块，环境块是进程地址空间中分配的内存块，每个环境块都包含一组字符串，同时需将一个0字符置于所有环境变量的结尾处，以表示环境块的结束。使用GetEinvironmentVariable函数可确定某个环境变量是否存在以及它的值：

DWORD GetEnvironmentVariable(

PCTSTR pszName,

PTSTR pszValue,

DWORD cchValue);

其中，pszName指向需要的变量名，pszValue指向用于存放变量值缓存，cchValue用于指明缓存的大小（用字符数表示），该函数可以返回拷贝到缓存的字符数，如果在环境块中找不到该变量，可返回0。可以通过SetEnvironmentVariable函数来添加、删除、修改变量的值：

BOOL SetEnvironmentVariable(

PCTSTR pszName,

PCTSTR pszValue);

该函数可以使环境变量保持有序排列。

创建进程的函数CreatProcess，系统会调用个亿进程内核对象，其初始使用计数是1，进程内核对象不是进程本身，是操作系统管理进程时使用的一个较小数据机构；系统还会为进程分配一个虚拟地址空间，并将可执行文件或任何必要的DLL文件的代码和数据加载到进程的地址空间中；接着，系统为新进程的主线程创建一个线程内核对象（其使用计数为1），也是操作系统管理线程的小型数据结构；通过执行C/C++运行期启动代码，主线程便开始运行，如成功创建了新进程和主线程CreateProcess边返回TRUE。具体函数如下：

BOOL CreateProcess(

PCTSTR pszApplicationName,

PTSTR pszCommandLine,

PSECURITY_ATTRIBUTES psaProcess,

PSECURITY_ATTRIBUTES psaThread,

BOOL bInheritHandles,

DWORD fdwCreate,

PVOID pvEnvironment,

PCTSTR pszCurDir,

PSTARTUPINFO psiStartInfo,

PPROCESS_INFOMATION ppiProcInfo);

pszApplicationName和pszCommandLine参数分别用于设定新进程将要使用的可执行文件的名字和传递给新进程的命令行字符串；fdwCreate参数用于标识标志，以便用于规定如何来创建新进程；pvEnvironment参数用于指向包含新进程将要使用的环境字符串的内存块；pszCurDir参数允许父进程设置子进程的当前驱动器和目录。

终止进程的运行的方法：

1）主线程的进入点函数返回（最好使用这个方法）；

2）进程中的一个线程调用ExitProcess函数（应用避免使用这个方法）；

3）另一个进程中的线程调用TerminateProcess函数（应用避免使用这个方法）；

4）进程中的所有线程自行终止运行（比较少发生）；

设计应用程序应在主线程的进入点函数返回时，进程才终止运行，可保证所有线程资源能够得到正确清除：

1）该线程创建的任何C++对象将能使用它们的析构函数正确地撤消；

2）操作系统将正确地释放该线程的堆栈使用的内存；

3）系统将进程的退出码（在进程的内核对象中维护）设置为进入点函数的返回值；

4）系统将进程的内核对象的返回值递减1；

函数ExitProcess可终止进程运行：

VOID ExitProcess(UINT fuExitCode);

该函数终止进程的运行，并将进程的退出码设置为fuExitCode，函数不返回任何值，因为进程已终止运行，如在ExitProcess之后又增加处理代码，则代码将不会被执行。当主线程的进入点函数（WinMain、wWinMain、main、wmain）返回时，将返回给C/C++运行期启动代码，可正确清除该进程使用的所有C运行期资源。当C运行期资源被释放之后，C运行期启动代码就显示调用ExitProcess，并将进入点函数返回的值传递给它。进程中运行的任何其他线程都随着进程终止运行，如果如果在进入点函数中调用ExitThread，而不是调用ExitProcess或者仅仅是返回，那应用程序的主线程将停止运行，但进程如仍有线程在运行那么将不会终止。只有从进入点函数返回，C/C++运行期就能执行它的清除操作，并正确撤消任何或所有C++对象。

函数TerminateProcess也能终止进程运行：

BOOL TerminateProcess（

HANDLE hProcess,

UINT fuExitCode);

任何线程都可以调用TerminateProcess来终止另一进程或自己进程的运行。hProcess参数用于标识要终止运行进程的句柄，当进程终止运行时，它的退出代码将作为fuExitCode参数的值来传递。该函数是异步运行的函数，只有其他方法无法终止进程时，才使用该函数，因为终止运行的进程得不到关于它将终止运行的任何通知，因此无法正确清除并不能避免自己被撤消（除非通过正常的安全机制），例如进程将无法将内存中它拥有的任何信息迅速送往磁盘。

当进程终止运行时，下面操作将启动运行：

1）进程中剩余的所有线程全部终止运行；

2）进程指定的所有用户对象和GDI对象均被释放，所有内核对象均被关闭（如果没有其他进程打开它们的句柄，那这些内核对象将被撤消）；

3）进程的退出代码将从STILL_ACTIVE改为传递给ExitProcess或TerminateProcess的代码；

4）进程内核对象的状态变成收到通知状态，系统中其他线程可以挂起，直到进程终止运行；

5）进程内核对象使用计数递减1；

本学习手札给出了几种类不同枚举系统进程的代码：

头文件包含：#include"tlhelp32.h"

具体代码：

LPPROCESSENTRY32pinfo=(LPPROCESSENTRY32)malloc(sizeof(PROCESSENTRY32));

pinfo->dwSize=sizeof(PROCESSENTRY32);

HANDLEhProcess=(HANDLE)CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

BOOL report=Process32First(hProcess,pinfo);

while(report)

{

CString strFile=pinfo->szExeFile;

int iProID=pinfo->th32ProcessID;

printf("进程名称：%s-----进程ID：%d /n",strFile,iProID);

report=Process32Next(hProcess, pinfo);

}

free(pinfo);

如非2008-12-1



