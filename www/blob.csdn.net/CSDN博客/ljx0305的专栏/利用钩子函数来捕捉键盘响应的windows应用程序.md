# 利用钩子函数来捕捉键盘响应的windows应用程序  - ljx0305的专栏 - CSDN博客
2009年11月17日 22:15:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：648标签：[windows																[dll																[keyboard																[callback																[thread																[linker](https://so.csdn.net/so/search/s.do?q=linker&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=keyboard&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[VC																[MFC](https://blog.csdn.net/ljx0305/article/category/403913)](https://blog.csdn.net/ljx0305/article/category/401790)
利用钩子函数来捕捉键盘响应的windows应用程序 
=========================================================== 
作者: youhello(http://youhello.itpub.net)
发表于: 2007.11.20 16:54
分类: 程序开发 
出处: http://youhello.itpub.net/post/4630/414477
--------------------------------------------------------------- 
你也许一直对金山词霸的屏幕抓词的实现原理感到困惑，你也许希望将你的键盘，鼠标的活动适时的记录下来，甚至你想知道木马在windows操作系统是怎样进行木马dll的加载的…..其实这些都是用到了windows的钩子函数。因此本文将对钩子函数的相关知识进行阐述。当然，本文的目的并不是想通过此程序让读者去窃取别人的密码，只是由于钩子函数在windows系统中是一个非常重要的系统接口函数，所以想和大家共同的探讨，当然本文也对怎样建立动态连结库（DLL）作了一些简单的描述。（本文的程序为vc6.0的开发环境，语言是：C和win32 api）。
一：引言：
你也许一直对金山词霸的屏幕抓词的实现原理感到困惑，你也许希望将你的键盘，鼠标的活动适时的记录下来，甚至你想知道木马在windows操作系统是怎样进行木马dll的加载的…..其实这些都是用到了windows的钩子函数。因此本文将对钩子函数的相关知识进行阐述。当然，本文的目的并不是想通过此程序让读者去窃取别人的密码，只是由于钩子函数在windows系统中是一个非常重要的系统接口函数，所以想和大家共同的探讨，当然本文也对怎样建立动态连结库（DLL）作了一些简单的描述。（本文的程序为vc6.0的开发环境，语言是：C和win32 api）。
二：钩子概述：
微软的windowsX操作系统是建立在事件驱动的机制上的，也就是通过消息传递来实现。而钩子在windows操作系统中，是一种能在事件（比如：消息、鼠标激活、键盘响应）到达应用程序前中途接获事件的机制。而且，钩子函数还可以通过修改、丢弃等手段来对事件起作用。
Windows 有两种钩子，一种是特定线程钩子（Thread specific hooks），一种是全局系统钩子(Systemwide hooks)。特定线程钩子只是监视指定的线程，而全局系统钩子则可以监视系统中所有的线程。无论是特定线程钩子，还是全局系统钩子，都是通过SetWindowsHookEx ()来设置钩子的。对于特定线程钩子，钩子的函数既可以是包含在一个.exe也可以是一个.dll。但是对于一个全局系统钩子，钩子函数必须包含在独立的dll中，因此，当我们要捕捉键盘响应时，我们必须创建一个动态链接库。但是当钩子函数在得到了控制权，并对相关的事件处理完后，如果想要该消息得以继续的传递，那么则必须调用另一个函数：CallNextHookEx。由于系统必须对每个消息处理，钩子程序因此增加了处理的负担，因此也降低了系统的性能。鉴于这一点，在windows ce中对钩子程序并不支持。所以当程序完成并退出时，应当释放钩子，调用函数：UnhookWindowsHookEx。
下面我们将举一个例子（捕捉键盘）来详细的讲解钩子函数的程序设计。
三：程序的设计：
I:设置钩子
设置钩子是通过SetWindowsHookEx ()的API函数.
原形: HHOOK SetWindowsHookEx(int idHook,HOOKPROC lpfn,HINSTANCE hMod,DWORD dwThreadId)
idhook:装入钩子的类型.
lpfn: 钩子进程的入口地址
hMod: 应用程序的事件句柄
dwThreadId: 装入钩子的线程标示
参数：
idHook:
这个参数可以是以下值：
WH_CALLWNDPROC、WH_CALLWNDPROCRET、WH_CBT、WH_DEBUG、WH_FOREGROUNDIDLE、WH_GETMESSAGE、WH_JOURNALPLAYBACK、WH_JOURNALRECORD、WH_KEYBOARD、WH_KEYBOARD_LL、WH_MOUSE、WH_MOUSE_LL、WH_MSGFILTER、WH_SHELL、WH_SYSMSGFILTER。
对于这些参数，我不想一一加以解释，因为MSDN中有关于他们的详细注解。我只挑选其中的几个加以中文说明。
WH_KEYBOARD：一旦有键盘敲打消息（键盘的按下、键盘的弹起），在这个消息被放在应用程序的消息队列前，WINDOWS将会调用你的钩子函数。钩子函数可以改变和丢弃键盘敲打消息。
WH_MOUSE：每个鼠标消息在被放在应用程序的消息队列前，WINDOWS将会调用你的钩子函数。钩子函数可以改变和丢弃鼠标消息。
WH_GETMESSAGE：每次当你的应用程序调用一个GetMessage()或者一个PeekMessage()为了去从应用程序的消息队列中要求一个消息时，WINDOWS都会调用你的钩子函数。而钩子函数可以改变和丢弃这个消息。
II:释放钩子
钩子的释放使用的是UnhookWindowsHookEx（）函数
原形：BOOL UnhookWindowsHookEx( HHOOK hhk )
UnhookWindowsHookEx（）函数将释放的是钩子链中函数SetWindowsHookEx所装入的钩子进程。
hhk: 将要释放的钩子进程的句柄。
III：钩子进程
钩子进程使用函数HookProc;其实HookProc仅仅只是应用程序定义的符号。比如你可以写成KeyBoardHook.但是参数是不变的。Win32 API提供了诸如：CallWndProc、GetMsgProc、DebugProc、CBTProc、MouseProc、KeyboardProc、MessageProc等函数，对于他们的详细讲解，可以看MSDN我在此只讲解一下KeyBoardHook的含义。
原形：LRESULT CALLBACK KeyBoardHook (int nCode, WPARAM wParam, LPARAM lParam)
说明：钩子进程是一些依附在一个钩子上的一些函数，因此钩子进程只被WINDOWS调用而不被应用程序调用，他们有时就需要作为一个回调函数（CALLBACK）。
参数说明：
nCode:钩子代码，钩子进程使用钩子代码去决定是否执行。而钩子代码的值是依靠钩子的种类来定的。每种钩子种类都有他们自己一系列特性的代码。比如对于WH_KEYBOARD，钩子代码的参数有：HC_ACTION，HC_NOREMOVE。HC_ACTION的意义：参数wParam 和lParam 包含了键盘敲打消息的信息，HC_NOREMOVE的意义：参数wParam 和lParam包含了键盘敲打消息的信息，并且，键盘敲打消息一直没有从消息队列中删除。(应用程序调用PeekMessage函数，并且设置PM_NOREMOVE标志)。也就是说当nCode等于HC_ACTION时，钩子进程必须处理消息。而为HC_NOREMOVE时，钩子进程必须传递消息给CallNextHookEx函数，而不能做进一步的处理，而且必须有CallNextHookEx函数的返回值。 
wParam:键盘敲打所产生的键盘消息，键盘按键的虚拟代码。
lParam:包含了消息细节。
注意:如果钩子进程中nCode小于零，钩子进程必须返回(return) CallNextHookEx(nCode,wParam,lParam);而钩子进程中的nCode大于零，但是钩子进程并不处理消息，作者推荐你调用CallNextHookEx并且返回该函数的返回值。否则，如果另一个应用程序也装入WH_KEYBOARD 钩子，那么该钩子将不接受钩子通知并且返回一个不正确的值。如果钩子进程处理了消息，它可能返回一个非零值去阻止系统传递该信息到其它剩下的钩子或者windows进程。所以最好在钩子进程的最后都返回CallNextHookEx的返回值。 
IV:调用下一个钩子函数
调用下一个钩子函数时使用CallNexHookEx函数。
原形：LRESULT CallNextHookEx( HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam )
CallNexHookEx()函数用于对当前钩子链中的下一个钩子进程传递钩子信息，一个钩子进程既可以在钩子信息处理前，也可以在钩子信息处理后调用该函数。为什么使用该函数已在iii钩子进程中的“注意”中，加以了详细的说明。
hhk: 当前钩子的句柄
nCode: 传送到钩子进程的钩子代码。
wParam:传送到钩子进程的值。
lParam:传送到钩子进程的值。
参数：
hhk: 当前钩子的句柄. 应用程序接受这个句柄，作为先前调用SetWindowsHookE函数的结果 
nCode: 传送到钩子进程的钩子代码，下一个钩子进程使用这个代码以此决定如何处理钩子信息 
wParam:传送给钩子进程的wParam 参数值 ，参数值的具体含义与当前钩子链的挂接的钩子类型有关
lParam : 传送给钩子进程的wParam 参数值 ，参数值的具体含义与当前钩子链的挂接的钩子类型有关 
返回值：返回值是链中下一个钩子进程返回的值，当前钩子进程必须返回这个值，返回值的具体含义与挂接的钩子类型有关，详细信息请参看具体的钩子进程描述。
V 建立一个动态连接库（DLL）
当我们熟悉了以上的各个函数后，现在我们开始编写一个动态连接库（DLL）。在这儿我采用的是WIN32 DLL,而不是MFC DLL。而且以下所有的程序也都是采用C语言去编写。这主要是因为使用WIN32 API能够更详细、更全面的控制程序的如何执行，而使用MFC，一些低级的控制是不可能实现的(当然，仅对该程序来说，也是可以使用MFC的)。
1：建立一个动态连接库的.cpp文件。比如我们现在建立一个名为hookdll.cpp的文件。在hookdll.cpp的文件中加上如下内容：
#include <windows.h>
#include "string.h"
#include "stdio.h" 
HINSTANCE hInst;
#pragma data_seg("hookdata")
HHOOK oldkeyhook=0;
#pragma data_seg()
#pragma comment(linker,"/SECTION:hookdata,RWS")
#define DllExport extern "C"__declspec(dllexport)
DllExport LRESULT CALLBACK KeyBoardProc(int nCode,WPARAM wParam, LPARAM lParam );
DllExport void InstallHook(int nCode);
DllExport void EndHook(void);
BOOL WINAPI DllMain(HINSTANCE hInstance,ULONG What,LPVOID NotUsed)
{
switch(What)
{
case DLL_PROCESS_ATTACH:
hInst = hInstance;
break;
case DLL_PROCESS_DETACH:
break;
case DLL_THREAD_ATTACH:
break;
case DLL_THREAD_DETACH:
break; 
}
return 1;
}
void InstallHook(int nCode)
{
oldkeyhook = SetWindowsHookEx(WH_KEYBOARD,(HOOKPROC)KeyBoardProc,hInst,0);
}
DllExport LRESULT CALLBACK KeyBoardProc(int nCode,WPARAM wParam, LPARAM lParam )
{
WPARAM j;
FILE *fp;
if(lParam&0x80000000)
{
j = wParam;
fp=fopen("c:hookkey.txt","a"); 
fprintf(fp,"%4d",j);
fclose(fp);
}
return CallNextHookEx(oldkeyhook,nCode,wParam,lParam);
}
void EndHook(void)
{
UnhookWindowsHookEx(oldkeyhook);
}
这个动态连接库的源代码hookdll.cpp包含了键盘处理函数，设置钩子，退出钩子函数。并将键盘敲下的键以值的格式存入到c:hookkey.txt文件中。以下是对该文件的详细的解释。
使用包含在DLL的函数，必须将其导入。导入操作时通过dllimport来完成的，dllexport和dllimport都是vc（visual C++）和bc（Borland C++）所支持的扩展的关键字。但是dllexport和dllimport关键字不能被自身所使用，因此它的前面必须有另一个扩展关键字__declspec。通用格式如下：__declspec(specifier)其中specifier是存储类标示符。对于DLL，specifier将是dllexport和dllimport。而且为了简化说明导入和导出函数的语句，用一个宏名来代替__declspec.在此程序中，使用的是DllExport。如果用户的DLL被编译成一个C++程序，而且希望C程序也能使用它，就需要增加“C”的连接说明。#define DllExport extern "C"__declspec(dllexport)，这样就避免了标准C++命名损坏。（当然，如果读者正在编译的是C程序，就不要加入extern “C”,因为不需要它，而且编译器也不接受它）。有了宏定义，现在就可以用一个简单的语句就可以导出函数了，比如：DllExport LRESULT CALLBACK KeyBoardProc(int nCode,WPARAM wParam, LPARAM lParam )；DllExport void InstallHook(int nCode);DllExport void EndHook(void);
第一个#pragma 语句创造数据段，这里命名为hookdata。其实也可以命名为您喜欢的任意的一个名称。#pragma 语句之后的所有初始化的变量都进入hookdata段中。第二个#pragma语句是数据段的结束标志。对变量进行专门的初始化是很重要的，否则编译程序将把它们放在普通的未初始化的段中而不是放在hookdata中。
但是链接程序必须直到有一个hookdata段。我们可以在Project Setting（vc6.0） 对话框中选择Link选项，选中HOOKDLL时在Project Options域（在Release 和Debug配置中均可），包含下面的连接语句：/SECTION:hookdata,RWS字母RWS是表明该段具有读、写、和共享属性。当然，您也可以直接用DLL源代码指定链接程序就像HOOKDLL.c那样：#pragma comment(linker,"/SECTION:hookdata,RWS")。
由于有些DLL需要特殊的启动和终止代码。为此，所有的DLL都有一个名为DllMain()的函数，当初始化或终止DLL时调用该函数。一般在动态连结库的资源文件中定义此函数。不过如果没有定义它，则编译器会自动提供缺省的形式。
原型为：BOOL WINAPI DllMain(HINSTANCE hInstance,ULONG What,LPVOID NotUsed)
参数：
hInstance：DLL实例句柄
What:指定所发生的操作
NotUsed:保留参数
其中What的值可以为以下值：
DLL_PROCESS_ATTACH:进程开始使用DLL
DLL_PROCESS_DETACH:进程正在释放DLL
DLL_THREAD_ATTACH:进程已创建一个新的线程
DLL_THREAD_DETACH:进程已舍弃了一个线程
总的来说，无论何时调用DllMain()函数，都必须根据What的内容来采取适当的动作。这种适当的动作可以什么都不做，但不是返回非零值。 
DllMain()接下来的便是设置钩子，键盘处理，和释放钩子。
2：建立头文件
正如应用程序所使用的其它任何库函数一样，程序也必须包含dll内的函数的原型。所有得Windows程序都必须包含windows.h的原因。所以我们现在建立一个头文件hookdll.h如下：
#define DllImport extern"C"__declspec(dllimport)
DllImport void InstallHook(int nCode);
DllImport LRESULT CALLBACK KeyBoardProc (int nCode,WPARAM wParam, LPARAM lParam );
DllImport void EndHook(void);
使用dllimport主要是为了使代码更高效，因此推荐使用它。但是在导入数据时是需要dllimport的。当完成了上面的程序后，建一个项目工程，不妨为hookdll，然后将hookdll.c插入导项目工程中，编译，则可以生成了hookdll.dll和hookdll.lib。
3：建立程序主文件
我们在上面作的所有得工作都是为现在的主程序打得基础。其实当我们完成了Dll文件后，剩下的就是调用设置钩子函数：InstallHook 。如果你对windows编程十分的熟悉，那么你可以在你任何需要的时候来调用InstallHook。但是在你必须记住在你退出程序的时候你需要调EndHook以便释放你所装入的钩子函数。现在我在建立了一个hookspy.cpp，并将生成好的hookdll.dll和hookdll.lib拷贝到从一个目录下，并建立一个hookspy的项目工程。将hookspy.cpp,hookdll.dll,hookdll.lib,hookdll.h插入到项目工程中去。然后在建立windows窗口时就将钩子设置，在退出程序时退出钩子函数。比如：
case WM_CREATE:
InstallHook(TRUE);
break;
case WM_DESTROY: //terminate the program
EndHook();
PostQuitMessage(0);
break;
当然，如果大家需要我的源代码，可与我联系，但是我认为重要的是技巧和原理，而不是源代码。就像如果你知道了Windows程序设计原理，至于用什么编译工具都是可以的。
[](http://youhello.itpub.net/post/4630/414477)
引用：[http://youhello.itpub.net/post/4630/414477](http://youhello.itpub.net/post/4630/414477)
