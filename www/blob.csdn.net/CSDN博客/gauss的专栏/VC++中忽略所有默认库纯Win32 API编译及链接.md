# VC++中忽略所有默认库纯Win32 API编译及链接 - gauss的专栏 - CSDN博客
2013年01月04日 00:25:59[gauss](https://me.csdn.net/mathlmx)阅读数：298
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
我们在用VC++编写Windows程序的时候可能会发现一般可执行体（.EXE）的文件体积都比较大，于是非常羡慕那些使用Win32汇编编写程 序的人，因为他们编写的可执行文件非常小。其实应用程序的体积是一方面，另外应用程序的部署环境则是需要注意的另一方面，这方面我深有体会，曾经使用 Visual Studio 2008编译过一个C++的Win32程序，本地测试正常，但是部署到客户机时，出现缺少什么动态库，于是还要安装[Visual
 C++ 2008可再发行组件包（Visual C++ 2005 Redistributable Package）](http://www.microsoft.com/downloads/details.aspx?familyid=9B2DA534-3E03-4391-8A4D-074B9F2BC1BF&displaylang=zh-cn)![新窗口打开链接](http://wangye.org/blog/shared/img/external.gif)，这给软件部署带来了一定的麻烦，另外对于一个功能比较简单的程序，安装如此的组件包，可能心里会不好受，我们希望对于一些比较简单的应用程序可以直接调用系统提供的API，从而降低部署程序的复杂度。
其实对于VC++我们可以采用忽略所有默认库的方式避免编译器引入不必要的动态链接库，当然你可以使用如下的预编译宏。
#pragma comment(linker, "/nodefaultlib")
#pragma comment(linker, "/nodefaultlib")
实际上，我们还需要在属性的 连接器->清单文件 将 生成清单 改为 否；然后选择 清单工具->输入和输出 将嵌入清单改为否；在 C/C++中选择代码生成将缓冲区安全检查改为否(/GS-)，否则编译会出现一个错误，设定程序的主入口点。注意上述配置一般在Release下，生成文件也在Release下编译链接，Debug可能无法使用，如果需要防止Debug模式编译，可以使用如下宏命令：
#ifdef _DEBUG 
#error Debug is disabled  
#endif //    _DEBUG  
#ifdef _DEBUG #error Debug is disabled #endif // _DEBUG
另外对于函数入口的重新定义可以使用如下宏以代替属性配置。
#pragma comment(linker,"/ENTRY:wWinMainCRTStartup")  
#pragma comment(linker,"/ENTRY:wWinMainCRTStartup")
由于这里指定使用的宽字符（Unicode）API调用，所以我们将入口点定义为wWinMainCRTStartup，ANSI版建议定义为 WinMainCRTStartup，另外Windows API有两个版本的API接口，ANSI版和Unicode版，ANSI版主要是为了兼容 Windows 98等旧系统，一般ANSI API编译的程序体积比较小，但由于现在的基于NT的新的操作系统基本使用的Unicode API，相对而言，对于这些系统，Unicode API接口的速度要快于ANSI 接口的速度。
入口点做如下定义
#include 
HINSTANCE    g_hInst    = NULL; 
int WINAPI SimpleMain(VOID) 
{ 
return 0; 
} 
VOID WINAPI wWinMainCRTStartup(VOID) 
{ 
    g_hInst = GetModuleHandle(NULL); 
    ExitProcess(SimpleMain()); 
}
#include HINSTANCE g_hInst = NULL; int WINAPI SimpleMain(VOID) { return 0; } VOID WINAPI wWinMainCRTStartup(VOID) { g_hInst = GetModuleHandle(NULL); ExitProcess(SimpleMain()); }
很多朋友可能还需要获得程序的执行命令行，以获得所需要的执行参数，我们可以使用GetCommandLine()这个API获得。这里提供一个我写的分离命令的函数。
VOID WINAPI AnalyseCommandLine(const LPTSTR lpOriginal, LPTSTR lpParameter)   
{   
int i, j, k = 0, nCmdLen = lstrlen(lpOriginal);   
for(i = nCmdLen; i>0; i--)   
    {   
if(*(lpOriginal + i)==' ')   
        {   
for(j=i+1; j < nCmdLen+1; j++)   
            {   
*(lpParameter + k) = *(lpOriginal + j);   
                k++;   
            }   
break;   
        }   
    }   
*(lpParameter+k) = '/0';   
}  
VOID WINAPI AnalyseCommandLine(const LPTSTR lpOriginal, LPTSTR lpParameter) { int i, j, k = 0, nCmdLen = lstrlen(lpOriginal); for(i = nCmdLen; i>0; i--) { if(*(lpOriginal + i)==' ') { for(j=i+1; j < nCmdLen+1; j++) { *(lpParameter + k) = *(lpOriginal + j);
 k++; } break; } } *(lpParameter+k) = '/0'; }
具体使用如下：
[](http://wangye.org/blog/article.asp?pid=7#)
LPTSTR    lpCommand = NULL;   
TCHAR    zCmd[MAX_PATH];   
lpCommand = GetCommandLine();   
AnalyseCommandLine(lpCommand, zCmd);   
LPTSTR lpCommand = NULL; TCHAR zCmd[MAX_PATH]; lpCommand = GetCommandLine(); AnalyseCommandLine(lpCommand, zCmd);
理论上这样可以编译链接了，实际上还有很多错误，主要是链接方面的。由于我们使用了纯Windows API，所以不能使用memset，这时可以调用 FillMemory（RtlFillMemory）、ZeroMemory（RtlZeroMemory）等等，这时编译会出现链接错误 是关于 _memset符号链接的，实际上我们并没有使用memset，那这个错误是怎么产生的呢？其实微软重新定义了RtlFillMemory等API并使它们挂接到memset这个函数下，为了我们能够顺利编译，我们需要在自己的头文件里做如下处理。
#undef RtlFillMemory    
#undef RtlZeroMemory   
extern "C" NTSYSAPI BOOL NTAPI    
RtlFillMemory ( VOID *Source1,DWORD Source2,BYTE Fill );   
extern "C" NTSYSAPI BOOL NTAPI    
RtlZeroMemory( PVOID Destination, SIZE_T Length);   
#define memset(Destination,Fill,Length) RtlFillMemory((Destination),(Length),(Fill)) 
#undef RtlFillMemory #undef RtlZeroMemory extern "C" NTSYSAPI BOOL NTAPI RtlFillMemory ( VOID *Source1,DWORD Source2,BYTE Fill ); extern "C" NTSYSAPI BOOL NTAPI RtlZeroMemory( PVOID Destination, SIZE_T Length); #define memset(Destination,Fill,Length) RtlFillMemory((Destination),(Length),(Fill))
下面我们继续我们的编译，在链接这里又出现错误。
error LNK2001: 无法解析的外部符号 __imp__InitCommonControls@0 
error LNK2001: 无法解析的外部符号 __imp__InitCommonControlsEx@4
跟踪后发现这两个函数InitCommonControls和InitCommonControlsEx由COMCTL32.dll导出，参考网上的解决方案，加入lib库。
#include <commctrl.h> 
#pragma comment(lib, "comctl32.lib")  
#include #pragma comment(lib, "comctl32.lib")
错误依旧！经过仔细寻找发现在属性配置里 链接器->输入 在附加依赖项里填入 comctl32.lib，编译，通过！
另外对于空间分配建议参考HeapAlloc、HeapFree等等API函数。
如果大家在操作过程中遇到什么问题欢迎讨论！
[http://blog.csdn.net/lwglucky/article/details/5347340](http://blog.csdn.net/lwglucky/article/details/5347340)
