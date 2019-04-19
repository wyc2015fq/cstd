# 截获 Windows socket API - xqhrs232的专栏 - CSDN博客
2016年12月01日 15:09:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：231
原文地址::[http://blog.csdn.net/glliuxueke/article/details/2702608](http://blog.csdn.net/glliuxueke/article/details/2702608)
相关文章
1、[WindowsHookApi实例](http://blog.csdn.net/lonelyrains/article/details/25076115)----[http://blog.csdn.net/lonelyrains/article/details/25076115](http://blog.csdn.net/lonelyrains/article/details/25076115)
1前言
本文主要介绍了如何实现替换Windows上的API函数，实现Windows API Hook（当然，对于socket的Hook只是其中的一种特例）。这种Hook API技术被广泛的采用在一些领域中，如屏幕取词，个人防火墙等。
这种API Hook技术并不是很新，但是涉及的领域比较宽广，要想做好有一定的技术难度。本文是采集了不少达人的以前资料并结合自己的实验得出的心得体会，在这里进行总结发表，希望能够给广大的读者提供参考，达到抛砖引玉的结果。
2问题
最近和同学讨论如何构建一个Windows上的简单的个人防火墙。后来讨论涉及到了如何让进程关联套接字端口，替换windows API，屏幕取词等技术。
其中主要的问题有：
1) 采用何种机制来截获socket的调用？
一般来说，实现截获socket的方法有很多很多，最基本的，可以写驱动，驱动也有很多种，TDI驱动, NDIS驱动，Mini port驱动…。由于我使用的是Win2000系统，所以截获socket也可以用Windows SPI来进行。另外一种就是Windows API Hook技术。
由于我没什么硬件基础，不会写驱动，所以第一种方法没有考虑，而用SPI相对比较简单。但是后来觉得Windows API Hook适应面更广，而且觉得自己动手能学到不少东西，就决定用Windows API Hook来尝试做socket Hook.
2) API Hook的实现方法？
实际上就是对系统函数的替换，当然实现替换的方法大概不下5，6种吧，可以参考《Windows核心编程》第22章。不过我使用的方法与其不近相同，应该相对比较简单易懂。
3原理
我们知道，系统函数都是以DLL封装起来的，应用程序应用到系统函数时，应首先把该DLL加载到当前的进程空间中，调用的系统函数的入口地址，可以通过 GetProcAddress函数进行获取。当系统函数进行调用的时候，首先把所必要的信息保存下来（包括参数和返回地址，等一些别的信息），然后就跳转到函数的入口地址，继续执行。其实函数地址，就是系统函数“可执行代码”的开始地址。那么怎么才能让函数首先执行我们的函数呢？呵呵，应该明白了吧，把开始的那段可执行代码替换为我们自己定制的一小段可执行代码，这样系统函数调用时，不就按我们的意图乖乖行事了吗？其实，就这么简单。Very
 very简单。 ：P
实际的说，就可以修改系统函数入口的地方，让他调转到我们的函数的入口点就行了。采用汇编代码就能简单的实现Jmp XXXX, 其中XXXX就是要跳转的相对地址。
我们的做法是：把系统函数的入口地方的内容替换为一条Jmp指令，目的就是跳到我们的函数进行执行。而Jmp后面要求的是相对偏移，也就是我们的函数入口地址到系统函数入口地址之间的差异，再减去我们这条指令的大小。用公式表达如下：
int nDelta = UserFunAddr – SysFunAddr - （我们定制的这条指令的大小）;
Jmp nDleta;
为了保持原程序的健壮性，我们的函数里做完必要的处理后，要回调原来的系统函数，然后返回。所以调用原来系统函数之前必须先把原来修改的系统函数入口地方给恢复，否则，系统函数地方被我们改成了Jmp XXXX就会又跳到我们的函数里，死循环了。
那么说一下程序执行的过程。
我们的dll“注射”入被hook的进程 -> 保存系统函数入口处的代码 -> 替换掉进程中的系统函数入口指向我们的函数 -> 当系统函数被调用，立即跳转到我们的函数 -> 我们函数进行处理 -> 恢复系统函数入口的代码 -> 调用原来的系统函数 -> 再修改系统函数入口指向我们的函数（为了下次hook）-> 返回。
于是，一次完整的Hook就完成了。
好，这个问题明白以后，讲一下下个问题，就是如何进行dll“注射”？即将我们的dll注射到要Hook的进程中去呢？
很简单哦，这里我们采用调用Windows提供给我们的一些现成的Hook来进行注射。举个例子，鼠标钩子，键盘钩子，大家都知道吧？我们可以给系统装一个鼠标钩子，然后所有响应到鼠标事件的进程，就会“自动”（其实是系统处理了）载入我们的dll然后设置相应的钩子函数。其实我们的目的只是需要让被注射进程载入我们的dll就可以了，我们可以再dll实例化的时候进行函数注射的，我们的这个鼠标钩子什么都不干的。
4简单的例子OneAddOne
讲了上面的原理，现在我们应该实战一下了。先不要考虑windows系统那些繁杂的函数，我们自己编写一个API函数来进行Hook与被Hook的练习吧，哈哈。
////////////////////
第一步，首先编写一个add.dll，很简单，这个dll只输出一个API函数,就是add啦。
新建一个win32 dll工程，
add.cpp的内容：
#include "stdafx.h"
int WINAPI add(int a,int b){  //千万别忘记声明WINAPI 否则调用的时候回产生声明错误哦！
 return a+b;
}
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
      )
{
    return TRUE;
}
然后别忘了在add.def里面输出函数:
LIBRARY  Add
DESCRIPTION "ADD LA"
EXPORTS
 add  @1;
编译，ok,我们获得了add.dll
////////////////////
第二步，编写1+1主程序
新建一个基于对话框的工程One，
在 OnOK()里面调用add函数：
ConeDlg.h里面加入一些变量的声明：
….
Public:
 HINSTANCE hAddDll;
 typedef int (WINAPI*AddProc)(int a,int b);
 AddProc add;
…
ConeDlg.cpp里进行调用:
void COneDlg::OnOK() 
{
 // TODO: Add extra validation here
 if (hAddDll==NULL)
  hAddDll=::LoadLibrary("add.dll");
 add=(AddProc)::GetProcAddress(hAddDll,"add");
 int a=1;
 int b=2;
 int c=add(a,b);
 CString tem;
 temp.Format("%d+%d=%d",a,b,c);
 AfxMessageBox(temp);
}
OK,编译运行，正确的话就会显示1+2=3咯
////////////////////
第3步，要动手Hook咯，爽阿
新建一个MFC的 dll工程，Hook
在Hook.dll工程里：
添加一个鼠标Hook MouseProc,鼠标hook什么也不做
LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
 LRESULT RetVal= CallNextHookEx(hhk,nCode,wParam,lParam);
 return RetVal;
}
添加鼠标钩子的安装和卸载函数:
BOOL InstallHook()
{
 hhk=::SetWindowsHookEx(WH_MOUSE,MouseProc,hinst,0);
 ….
 return true;
}
void UninstallHook()
{
 ::UnhookWindowsHookEx(hhk);
}
再实例化中获得一些参数
BOOL CHookApp::InitInstance() 
{
 获得dll 实例，进程句柄
hinst=::AfxGetInstanceHandle();
 DWORD dwPid=::GetCurrentProcessId();
 hProcess=OpenProcess(PROCESS_ALL_ACCESS,0,dwPid); 
//调用注射函数
 Inject();
 return CWinApp::InitInstance();
}
好，最重要的注射函数：
void Inject()
{
 if (m_bInjected==false)
 { //保证只调用1次
  m_bInjected=true;
  //获取add.dll中的add()函数
  HMODULE hmod=::LoadLibrary("add.dll");
  add=(AddProc)::GetProcAddress(hmod,"add");
  pfadd=(FARPROC)add;
  if (pfadd==NULL)
  {
   AfxMessageBox("cannot locate add()");
  }
  // 将add()中的入口代码保存入OldCode[]
  _asm 
  { 
   lea edi,OldCode 
   mov esi,pfadd 
   cld 
   movsd 
   movsb 
  }
  NewCode[0]=0xe9;//实际上0xe9就相当于jmp指令
  //获取Myadd()的相对地址
  _asm 
  { 
   lea eax,Myadd
   mov ebx,pfadd 
   sub eax,ebx 
   sub eax,5 
   mov dword ptr [NewCode+1],eax 
  } 
  //填充完毕，现在NewCode[]里的指令相当于Jmp Myadd
  HookOn(); //可以开启钩子了
 }
}
开启钩子的函数
void HookOn() 
{ 
 ASSERT(hProcess!=NULL);
 DWORD dwTemp=0;
 DWORD dwOldProtect;
 //将内存保护模式改为可写,老模式保存入dwOldProtect
 VirtualProtectEx(hProcess,pfadd,5,PAGE_READWRITE,&dwOldProtect); 
     //将所属进程中add()的前5个字节改为Jmp Myadd 
 WriteProcessMemory(hProcess,pfadd,NewCode,5,0);
 //将内存保护模式改回为dwOldProtect
 VirtualProtectEx(hProcess,pfadd,5,dwOldProtect,&dwTemp);
 bHook=true; 
}
关闭钩子的函数
void HookOff()//将所属进程中add()的入口代码恢复
{ 
 ASSERT(hProcess!=NULL);
 DWORD dwTemp=0;
 DWORD dwOldProtect;
 VirtualProtectEx(hProcess,pfadd,5,PAGE_READWRITE,&dwOldProtect); 
 WriteProcessMemory(hProcess,pfadd,OldCode,5,0); 
 VirtualProtectEx(hProcess,pfadd,5,dwOldProtect,&dwTemp); 
 bHook=false; 
}
然后，写我们自己的Myadd()函数
int WINAPI Myadd(int a,int b)
{
 //截获了对add()的调用，我们给a,b都加1
 a=a+1;
 b=b+1;
 HookOff();//关掉Myadd()钩子防止死循环
 int ret;
 ret=add(a,b);
 HookOn();//开启Myadd()钩子
 return ret;
}
然后别忘记在hook.def里面输出 
InstallHook  
 MouseProc
 Myadd
 UninstallHook 
四个函数。
（全部的程序会贴在最后面的）
好到这里基本上大功告成咯
////////////////////
第4步，我们就可以修改前面的One的[测试](http://lib.csdn.net/base/softwaretest)程序了
增加一个安装钩子的函数/按钮
void COneDlg::doHook() 
{
 hinst=LoadLibrary("hook.dll");
 if(hinst==NULL)
 {
  AfxMessageBox("no hook.dll!");
  return;
 }
 typedef BOOL (CALLBACK *inshook)(); 
 inshook insthook;
 insthook=::GetProcAddress(hinst,"InstallHook");
 if(insthook==NULL)
 {
  AfxMessageBox("func not found!");
  return;
 }
 DWORD pid=::GetCurrentProcessId();
 BOOL ret=insthook();
}
别忘了退出时卸掉钩子
void COneDlg::OnCancel() 
{
 // TODO: Add extra cleanup here
 typedef BOOL (CALLBACK *UnhookProc)(); 
 UnhookProc UninstallHook;
 UninstallHook=::GetProcAddress(hinst,"UninstallHook");
 if(UninstallHook==NULL) UninstallHook();
 if (hinst!=NULL)
 {
  ::FreeLibrary(hinst);
 }
 if (hAddDll!=NULL)
 {
  ::FreeLibrary(hAddDll);
 }
 CDialog::OnCancel();
}
////////////////////
好了，大功告成咯，现在运行一下One，
先点Ok，测试1+2=3 没问题，
然后点Hook,安装钩子，，显示installhook ok,然后再点一下ok,
哈哈，发现结果1+2=5 !!!???
Hook成功啦！
附上Hook.cpp的原码
/////////////////////
Hook.cpp
// Hook.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "Hook.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CHookApp
BEGIN_MESSAGE_MAP(CHookApp, CWinApp)
 //{{AFX_MSG_MAP(CHookApp)
  // NOTE - the ClassWizard will add and remove mapping macros here.
  //    DO NOT EDIT what you see in these blocks of generated code!
 //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CHookApp construction
CHookApp::CHookApp()
{
 // TODO: add construction code here,
 // Place all significant initialization in InitInstance
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CHookApp object
CHookApp theApp;
//变量定义
//不同Instance共享的该变量
#pragma data_seg("SHARED")
static HHOOK  hhk=NULL; //鼠标钩子句柄
static HINSTANCE hinst=NULL; //本dll的实例句柄 (hook.dll)
#pragma data_seg()
#pragma comment(linker, "/section:SHARED,rws")
//以上的变量共享哦!
CString temp; //用于显示错误的临时变量
bool bHook=false; //是否Hook了函数
bool m_bInjected=false; //是否对API进行了Hook
BYTE OldCode[5]; //老的系统API入口代码
BYTE NewCode[5]; //要跳转的API代码 (jmp xxxx)
typedef int (WINAPI*AddProc)(int a,int b);//add.dll中的add函数定义
AddProc add; //add.dll中的add函数
HANDLE hProcess=NULL; //所处进程的句柄
FARPROC pfadd;  //指向add函数的远指针
DWORD dwPid;  //所处进程ID
//end of 变量定义
//函数定义
void HookOn(); //开启钩子
void HookOff(); //关闭钩子
LRESULT CALLBACK MouseProc(int nCode,WPARAM wParam,LPARAM lParam); //鼠标钩子函数
void Inject(); //具体进行注射，替换入口的函数
int WINAPI Myadd(int a,int b); //我们定义的新的add()函数
BOOL InstallHook(); //安装钩子函数
void UninstallHook(); //卸载钩子函数
//end of 函数定义
BOOL CHookApp::InitInstance() 
{
 //获取自身dll句柄
 hinst=::AfxGetInstanceHandle();
 //获取所属进程id和句柄
 DWORD dwPid=::GetCurrentProcessId();
 hProcess=OpenProcess(PROCESS_ALL_ACCESS,0,dwid); 
 //调用函数注射
 Inject();
 return CWinApp::InitInstance();
}
int CHookApp::ExitInstance() 
{
 // 如果add钩子开着，则将其关闭
 if (bHook)
  HookOff();
 return CWinApp::ExitInstance();
}
BOOL InstallHook()
{
 hhk=::SetWindowsHookEx(WH_MOUSE,MouseProc,hinst,0);
 if (hhk==NULL)
 {
  DWORD err=::GetLastError();
  temp.Format("hook install failed!:%d",err);
  AfxMessageBox(temp);
  return false;
 }
 AfxMessageBox("hook installed!");
 return true;
}
void UninstallHook()
{
 if (hhk!=NULL)
 ::UnhookWindowsHookEx(hhk);
 AfxMessageBox("Unhooked!");
}
void Inject()
{
 if (m_bInjected==false)
 {
  m_bInjected=true;
  //读取add.dll并查找到add()函数
  HMODULE hmod=::LoadLibrary("add.dll");
  add=(AddProc)::GetProcAddress(hmod,"add");
  pfadd=(FARPROC)add;
  if (pfadd==NULL)
  {
   AfxMessageBox("cannot locate add()");
  }
  // 将add()的入口代码保存到OldCode里
  _asm 
  { 
   lea edi,OldCode 
   mov esi,pfadd 
   cld 
   movsd 
   movsb 
  }
  NewCode[0]=0xe9;//第一个字节0xe9相当于jmp指令
  //获取Myadd()的相对地址
  _asm 
  { 
   lea eax,Myadd
   mov ebx,pfadd 
   sub eax,ebx 
   sub eax,5 
   mov dword ptr [NewCode+1],eax 
  } 
  //填充完毕，现在NewCode[]里面就相当于指令 jmp Myadd
  HookOn();
 }
}
int WINAPI Myadd(int a,int b)
{
 //截获了对add()的调用，我们给a,b都加1
 a=a+1;
 b=b+1;
 HookOff();//关掉Myadd()钩子防止死循环
 int ret;
 ret=add(a,b);
 HookOn();//开启Myadd()钩子
 return ret;
}
LRESULT CALLBACK MouseProc(
  int nCode,      // hook code
  WPARAM wParam,  // message identifier
  LPARAM lParam   // mouse coordinates
  )
{
 LRESULT RetVal= CallNextHookEx(hhk,nCode,wParam,lParam);
 return RetVal;
}
void HookOn() 
{ 
 ASSERT(hProcess!=NULL);
 DWORD dwTemp=0;
 DWORD dwOldProtect;
 //将内存保护模式改为可写,老模式保存入dwOldProtect
 VirtualProtectEx(hProcess,pfadd,5,PAGE_READWRITE,&dwOldProtect); 
    //将所属进程中add的前5个字节改为Jmp Myadd 
 WriteProcessMemory(hProcess,pfadd,NewCode,5,0);
 //将内存保护模式改回为dwOldProtect
 VirtualProtectEx(hProcess,pfadd,5,dwOldProtect,&dwTemp);
 bHook=true; 
}
void HookOff()//将所属进程中add()的入口代码恢复
{ 
 ASSERT(hProcess!=NULL);
 DWORD dwTemp=0;
 DWORD dwOldProtect;
 VirtualProtectEx(hProcess,pfadd,5,PAGE_READWRITE,&dwOldProtect); 
 WriteProcessMemory(hProcess,pfadd,OldCode,5,0); 
 VirtualProtectEx(hProcess,pfadd,5,dwOldProtect,&dwTemp); 
 bHook=false; 
}
//////////////////////
hook.def
LIBRARY      "Hook"
DESCRIPTION  'Hook Windows Dynamic Link Library'
EXPORTS
    ; Explicit exports can [Go](http://lib.csdn.net/base/go) here
 InstallHook  
 MouseProc
 Myadd
 UninstallHook
5 实战Hook Win Socket API
如果对于上面这章的的理解没有什么问题的化，我想对于windows API的Hook应该也就是按部就班的做就可以了，不会有太大的问题，只要足够细心耐心，善于处理发现一些细节问题就行了。
当然，为了使我们的程序使用起来更灵活方便，必须要改进我们的程序结构，因为我们要Hook很多的API，不可能为每个API制定一套注射、截获的函数，
所以，我设计了一个类来储存某个API的一些信息：
class CHookFuncInfo
{
public:
 CHookFuncInfo(LPCTSTR szLibName,LPCTSTR szOldname,LPCTSTR szNewName)
 {
  //构造函数必须设定三个参数
  strcpy(LibraryName,szLibName);
  strcpy(OldFuncName,szOldname);
  strcpy(NewFuncName,szNewName);
  Injected=false;
 }
public:
 HINSTANCE hinstLib; //该函数所在dll模块的实例句柄
 FARPROC fpFunc;  //指向原函数的指针
 BYTE OldCode[5]; //原函数入口处的代码
 BYTE NewCode[5]; //跳转到我们函数入口地址的代码Jmp xxxx
 FARPROC fpMyFunc; //指向我们函数地址的指针
 char LibraryName[256]; //该函数所属的dll模块的名称
 char OldFuncName[256]; //原API函数名
 char NewFuncName[256]; //我们程序中定义的新API函数名
 bool Injected; //该函数是否被Hook
};
对某API函数的注射都可以用Inject()来处理
void Inject(CHookFuncInfo* phinfo);
开/关某个API可以用HookOn/HookOff来处理
void HookOff(CHookFuncInfo * phinfo);
void HookOn(CHookFuncInfo * phinfo);
需要注意的是，在汇编代码里，不可以直接对对象的指针进行调用
即 mov eax, phinfo->fpFunc;诸如此类的代码是不被允许的。
所以我们要加入新的变量，处理完毕，将变量值传回给传入的对象指针：
void Inject(CHookFuncInfo * phinfo)
{
 …….
 FARPROC fpFunc=phinfo->fpFunc;
 BYTE OldCode[5],NewCode[5];
……
 汇编代码
……
  memcpy(phinfo->OldCode,OldCode,5);
  memcpy(phinfo->NewCode,NewCode,5);  
  HookOn(phinfo);//开启钩子
 return;
}
举个例子：
CHookFuncInfo hinfoSendto("wsock32.dll","sendto","MySendto");
//创建一个sendto函数的API钩子
然后注射-〉开启钩子-〉…
下面是我定义的MySendto()的代码：
int WINAPI MySendto(
  SOCKET s,                        
  const char FAR *buf,            
  int len,                         
  int flags,                       
  const struct sockaddr FAR *to,  
  int tolen                        
  )
{
 int ret;
 HookOff(&hinfoSendto);//关钩子
 //下面这段就是通过WM_COPYDATA传递消息的方法，
//将一些sendto()的数据通过字符串方式传递给
//名为”XsockSpy”的窗体。
//XsockSpy窗体只需要响应对WM_COPYDATA的处理和显示就OK了，很简单。
 HWND hwnd=::FindWindow("#32770","XSockSpy");
 CString temp;
 temp.Format("pid:%d sending %d bytes",g_pid,len);
 if(hwnd!=NULL)
 {
  COPYDATASTRUCT cpdata;
  cpdata.dwData=0;
  cpdata.lpData=temp.GetBuffer(0);
  cpdata.cbData=temp.GetLength();
  ::SendMessage(hwnd,WM_COPYDATA,0,(LPARAM)&cpdata);
 }
 //调用真正的sendto()函数
 ret=::sendto(s,buf,len,flags,to,tolen);
 if (ret==SOCKET_ERROR
 {
  temp.Format("send error:%d",WSAGetLastError());
  AfxMessageBox(temp);
 }
 //开钩子
 HookOn(&hinfoSendto);
 return ret;
};
好了，是不是很简单啊？
打得很累了，我想大家基本生可以明白了吧？
如果需要代码的可以向我来索要，发信到 [yoyohon@etang.com](mailto:yoyohon@etang.com)注明“索要代码”就可以了。
不过由于时间关系，截获socket的代码我仅仅实现了sendto()这个函数的截获，而且代码可能也比较临乱吧，大家见谅了！
欢迎大家讨论，发表看法，提出异议！
最后感谢一下对本文又帮助的人：
pingfanxin
Win2K下的Api函数的拦截
[http://www.yourblog.org/Data/20044/62890.html](http://www.yourblog.org/Data/20044/62890.html)
TopLevel
有关API HOOK方面的一些浅释 
[http://dev.csdn](http://dev.csdn.net/develop/article/27/27732.shtm)[.NET](http://lib.csdn.net/base/dotnet)/develop/article/27/27732.shtm
[](http://blog.csdn.net/glliuxueke/article/details/2702608#)[](http://blog.csdn.net/glliuxueke/article/details/2702608#)[](http://blog.csdn.net/glliuxueke/article/details/2702608#)[](http://blog.csdn.net/glliuxueke/article/details/2702608#)[](http://blog.csdn.net/glliuxueke/article/details/2702608#)[](http://blog.csdn.net/glliuxueke/article/details/2702608#)
- 
顶
0
