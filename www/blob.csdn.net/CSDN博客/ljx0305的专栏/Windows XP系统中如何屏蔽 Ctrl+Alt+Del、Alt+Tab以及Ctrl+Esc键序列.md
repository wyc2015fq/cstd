# Windows XP系统中如何屏蔽 Ctrl+Alt+Del、Alt+Tab以及Ctrl+Esc键序列 - ljx0305的专栏 - CSDN博客
2008年02月15日 17:42:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：768标签：[windows																[xp																[任务																[dll																[command																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=xp&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
**Windows XP系统中如何****屏蔽 Ctrl+Alt+Del、Alt+Tab以及Ctrl+Esc键序列**
编译/[northtibet](mailto:northtibet@sohu.com)
关键字：Ctrl+Alt+Del，Alt+Tab，Ctrl+Esc，VK_LWIN，VK_RWIN，Task bar，Task Manager，任务栏，任务管理器。 
下载本文源代码: [TrapKeys.zip](http://www.vckbase.com/code/winsys/hook/trapkeys.zip) (95KB) 
    对于用过Windows的人，几乎没有人不知道Ctrl+Alt+Del组合键，尤其是在使用经常死机的Windows9x时，使用它的频率更高，这一组合键是专门为了系统安全起见提供的紧急出口。VC知识库在线杂志第11期，ac952_z_cn在他的个人专栏中写过一篇关于这方面的文章：“[WINDOWS NT/2000下如何屏蔽CTRL+ALT+DEL](http://www.vckbase.com/document/viewdoc.asp?id=242)”。因此本文侧重于介绍在Windows XP中如何实现屏蔽CTRL+ALT+DEL组合键，也就是任务管理器，任务切换组合键（Alt+Tab），任务栏和“开始”菜单（Ctrl+Esc，VK_LWIN，VK_RWIN）。这个方法也能应用于Windows 2000环境。
    在Windows 9x/Me系统中，屏蔽Ctrl+Alt+Del和各种任务开关键的方法是通过下面的方法实现的：
BOOL bOldState;
SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, TRUE, &bOldState, 0);      
    MS大佬认为这种方法很业余，所以在Windows NT/2000/XP中对此进行了修改。在这些较新的Windows版本中用户登陆使用Winlogon和GINA——Graphical Identification and Authentication，意思是图形化的身份认证，挺吓唬人的是不是！其实就那么回事。Winlogon是Windows系统的一部分，它专门提供交互式登陆支持，而GINA则是Winlogon用来实现认证的一个DLL——这个DLL就是msgina.dll。WlxInitialize、WlxActivateUserShell便是其中输出，当然不知这两个，还有别的。前者进行自身的初始化，后者激活用户的外壳程序。Windows就是用这个DLL来实现用户名+口令的身份认证的，但是开发人员可以用自己的GINA代替msgina.dll。例如，实现智能卡、视网膜扫描仪、DNA检查等等认证机制来代替输入用户名+口令形式的身份检查。 下面的表格中列出了与GINA有关的全部函数。其中有一个是WlxLoggedOnSAS，当按下Ctrl+Alt+Del 键时，Winlogon便调用这个函数。 
（表一）GINA 函数一览表
|函数|描述|
|----|----|
|WlxActivateUserShell|激活用户外壳程序|
|WlxDisplayLockedNotice|允许GINA DLL 显示锁定信息|
|WlxDisplaySASNotice|当没有用户登陆时，Winlogon调用此函数|
|WlxDisplayStatusMessage|Winlogon 用一个状态信息调用此函数进行显示|
|WlxGetConsoleSwitchCredentials|Winlogon调用此函数读取当前登陆用户的信任信息，并透明地将它们传到目标会话|
|WlxGetStatusMessage|Winlogon 调用此函数获取当前状态信息|
|WlxInitialize|针对指定的窗口位置进行GINA DLL初始化|
|WlxIsLockOk|验证工作站正常锁定|
|WlxIslogoffOk|验证注销正常|
|WlxLoggedOnSAS|用户已登陆并且工作站没有被加锁，如果此时接收到SAS事件，则Winlogon 调用此函数|
|WlxLoggedOutSAS|没有用户登陆，如果此时收到SAS事件，则Winlogon 调用此函数|
|WlxLogoff|请求注销操作时通知GINA DLL|
|WlxNegotiate|表示当前的Winlogon版本是否能使用GINA DLL|
|WlxNetworkProviderLoad|在加载网络服务提供程序收集了身份和认证信息后，Winlogon 调用此函数|
|WlxRemoveStatusMessage|Winlogon 调用此函数告诉GINA DLL 停止显示状态信息|
|WlxScreensaverNotify|允许GINA与屏幕保护操作交互|
|WlxShutdown|在关闭之前Winlogon 调用此函数，允许GINA实现任何关闭任务，例如从读卡器中退出智能卡|
|WlxStartApplication|当系统需要在用户的上下文中启动应用程序时调用此函数|
|WlxWkstaLockedSAS|当工作站被锁定，如果接收到一个SAS，则Winlogon 调用此函数|
    在默认情况下，GINA显示登陆对话框，用户输入用户名及口令。所以要想屏蔽掉Ctrl+Alt+Del，则可以写一个新的MyGina.dll，其中提供接口调用msgina.dll的函数WlxLoggedOnSAS，从而实现Ctrl+Alt+Del屏蔽。或者编写一个键盘驱动程序来实现。 
    难道屏蔽Ctrl+Alt+Del真的象上述所说的那么麻烦吗？有没有更好的方法呢？答案是肯定的。所以忘掉GINA吧，使用操作系统的策略设置完全可以搞掂这个问题。方法是进入"开始"菜单，选择"运行"，然后在运行对话框中输入"gpedit.msc"，启动Windows系统的组策略编辑器。在左边窗格查看"用户配置|管理模板|系统|登录/注销"，则在右边窗格策略里不难发现"禁用任务管理器"一项。如图二所示： 
![](http://www.vckbase.com/document/journal/vckbase15/images/cfig020902_1.gif)
图一 组策略编辑器 
通过对这个策略的设置可以屏蔽掉Ctrl+Alt+Del。如果要通过编写代码来实现，则必须操作下面的注册表项：
HKCU/
 Software/
  Microsoft/
   Windows/
    CurrentVersion/
     Policies/
      System/DisableTaskMgr = dword:1
      
如此设置之后，则在Windows XP中，如果用户按下Ctrl+Alt+Del，则会弹出一个出错对话框，如图二所示： 
![](http://www.vckbase.com/document/journal/vckbase15/images/cfig020902_2.jpg)
图二 错误信息 
注意这里假设在控制面板中“用户帐号”管理的“选择登录和注销选项”设置启用了“使用欢迎屏幕”一项。如图三所示： 
![](http://www.vckbase.com/document/journal/vckbase15/images/cfig020902_3.jpg)
图三 登录选项 
    否则，XP将使用Windows的传统登录模式，要求用户输入帐户名。并且Ctrl+Alt+Del组合键的 行为也和传统的行为一样，注册表中DisableTaskMgr的设置也只是将登录/注销对话框中的任务管理器按钮屏蔽或置灰。 有人可能会问，有关任务管理器的文档又没有明确说明，那你是怎么知道DisableTaskMgr是用来禁用任务管理器的呢？告诉你吧， 我是在使用GPEDIT时发现的。GPEDIT是一个非常有用的工具，不仅可以用它来编辑策略，还可以用它来发现策略。利用这个工具 可以轻松控制Windows的许多东西，从许可权限的存取到是否使用IE的传统外观，从是否显示对话框中的Places Bar到是否用Ctrl+Alt+Del 启动任务管理器。总之用它可以配置上百个界面行为，因此它是一个足以让系统管理员垂延三尺的工具。 一旦找到了感兴趣的策略，那如何知道相应的注册表位置呢？有两种方法。第一种是比较粗鲁的办法：在修改策略的前后将注册表输出到 一个.reg文件，然后比较它们有什么不同。所有的策略无外乎以下的四个注册表键：
// 用户指定
HKEY_CURRENT_USER/Software/Policies 
HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Policies
// 机器指定 
HKEY_LOCAL_MACHINE/Software/Policies 
HKEY_LOCAL_MACHINE/Software/Microsoft/Windows/CurrentVersion/Policies      
    第二种方法是直捣信息源头--检查描述策略的管理模板文件（.adm）。下面是Windows XP的system.adm文件对 DisableTaskMgr的描述：（Windows 2000对此的描述稍有不同，其细节请参考Windows 2000的资源开发包）
CATEGORY !!CADOptions
    #if version >= 4
        EXPLAIN !!CADOptions_Help
    #endif
    KEYNAME "Software/Microsoft/Windows/CurrentVersion/Policies/System"
    POLICY !!DisableTaskMgr    
        #if version >= 4
            SUPPORTED !!SUPPORTED_Win2k
        #endif
        EXPLAIN !!DisableTaskMgr_Help
        VALUENAME "DisableTaskMgr"
    END POLICY
    ;
    ; More Ctrl+Alt+Del policies here...
    ;
END CATEGORY ; Ctrl+Alt+Del options
……
……
DisableTaskMgr_Help="防止用户启动''任务管理器''(Taskmgr.exe)。/n/n如果该设置被启用，并且用户试图启动任务管理器，系统
会显示消息，解释是一个策略禁止了这个操作。/n/n任务管理器让用户启动或终止程序，监视计算机性能，查看及监视计算机上所有运行
中的程序 (包含系统服务)， 搜索程序的执行文件名，及更改程序运行的优先顺序。"
DisableTaskMgr="删除任务管理器" 
以上是DisableTaskMgr的描述片断 
    正是在这段描述中KEYNAME 和VALUENAME指定了注册表的键值对。利用这种方法，你可以为自己的应用程序创建管理模板和策略，但编辑和浏览.adm模板文件的编辑器必须支持Unicode字符。如Notepad或者WordPad等都可以。此外，使用管理模板文件，系统管理员可以用它为整个组织配置需要的策略——由此可以看出，此文件在系统中的地位举足轻重！有关模板管理文件格式的详细信息请参考平台SDK。最后需要强调的是DisableTaskMgr只是禁用Ctrl+Alt+Del的功能。下面我们来讨论如何捕获它的按键序列。要想截获Ctrl+Alt+Del，有三种可选择的方法：
- 1、 编写一个GINA代理；此方法我们在以后的文章中介绍。实际上，ac952_z_cn的个人专栏文章：“[WINDOWS NT/2000下如何屏蔽CTRL+ALT+DEL](http://www.vckbase.com/document/viewdoc.asp?id=242)”使用的就是这种方法。 
- 2、 编写一个键盘驱动程序；本文例子程序使用的方法。 
- 3、 用自己的程序代替任务管理器程序TaskMgr.exe。 
屏蔽Ctrl+Alt+Del解决方案的具体实现细节请参考本文的例子代码。 
    下面让我们来解决屏蔽任务切换键序列的问题，这些键序列包括Alt+Tab、Ctrl+Esc、Alt+Esc、VK_LWIN/VK_RWIN以及任务栏。在很早以前的Window 3.1年代，处理这个问题的方法是通过WM_SYSKEYDOWN实现。到了Windows 9x时期，本文前面提到过对这个问题的处理方法，使用SPI_SETSCREENSAVERRUNNING。但是进入Windows NT 4.0 （SP3 +），Windows 2000以及Windows XP时代，对这个问题的处理已经有所不同，必须写一个低级的键盘驱动钩子。不要怕，因为要实现这个钩子并不是很难。本文下面会介绍如何实现这个键盘钩子。一般来讲，系统级钩子必须是一个DLL。下面是本文提供的一个键盘钩子DLL的源代码片断（TaskKeyHook.dll）：
头文件
////////////////////////////////////////////////////////////////
//TaskKeyHook.h
//
#define DLLIMPORT __declspec(dllimport)
DLLIMPORT BOOL DisableTaskKeys(BOOL bEnable, BOOL bBeep);
DLLIMPORT BOOL AreTaskKeysDisabled();
实现文件
////////////////////////////////////////////////////////////////
// TaskKeyHook.cpp
//
#define _WIN32_WINNT 0x0500 // for KBDLLHOOKSTRUCT
#include <afxwin.h>         // MFC core and standard components
#define DLLEXPORT __declspec(dllexport)
//////////////////
// App (DLL) object
//
class CTaskKeyHookDll : public CWinApp {
public:
   CTaskKeyHookDll()  { }
   ~CTaskKeyHookDll() { }
} MyDll;
////////////////////////////////////////////////
// 下面的代码表示这一部分在此DLL所有实例之间共享
// 低级键盘钩子一定是系统级的钩子
// 
#pragma data_seg (".mydata")
HHOOK g_hHookKbdLL = NULL; // 钩子句柄
BOOL  g_bBeep = FALSE;     // 按下非法键时蜂鸣响铃
#pragma data_seg ()
#pragma comment(linker, "/SECTION:.mydata,RWS") // 告诉链接器：建立数据共享段
//////////////////////////////////
// 低级键盘钩子
// 截获任务转换键：不传递直接返回
//
LRESULT CALLBACK MyTaskKeyHookLL(int nCode, WPARAM wp, LPARAM lp)
{
   KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *) lp;
   if (nCode==HC_ACTION) {
      BOOL bCtrlKeyDown =
         GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1);
      if ((pkh->vkCode==VK_ESCAPE && bCtrlKeyDown) || // Ctrl+Esc
          // Alt+TAB
          (pkh->vkCode==VK_TAB && pkh->flags & LLKHF_ALTDOWN) ||   
          // Alt+Esc
          (pkh->vkCode==VK_ESCAPE && pkh->flags & LLKHF_ALTDOWN)|| 
          (pkh->vkCode==VK_LWIN || pkh->vkCode==VK_RWIN)) { // 开始菜单
         if (g_bBeep && (wp==WM_SYSKEYDOWN||wp==WM_KEYDOWN))
            MessageBeep(0); // 蜂鸣
         return 1; // 不再往CallNextHookEx传递，直接返回
      }
   }
   return CallNextHookEx(g_hHookKbdLL, nCode, wp, lp);
}
////////////////////////////////////////////////
// 是否屏蔽任务键序列——也就是说键盘钩子是否安装？
// 注：这里假设没有其它钩子做同样的事情
//
DLLEXPORT BOOL AreTaskKeysDisabled()
{
   return g_hHookKbdLL != NULL;
}
////////////////////////////////////////////////
// 屏蔽任务键：安装低级键盘构
// 返回当前是否屏蔽标志（TRUE/FALSE）
//
DLLEXPORT BOOL DisableTaskKeys(BOOL bDisable, BOOL bBeep)
{
   if (bDisable) {
      if (!g_hHookKbdLL) {
         g_hHookKbdLL = SetWindowsHookEx(WH_KEYBOARD_LL,
            MyTaskKeyHookLL, MyDll.m_hInstance, 0);
      }
   } else if (g_hHookKbdLL != NULL) {
      UnhookWindowsHookEx(g_hHookKbdLL);
      g_hHookKbdLL = NULL;
   }
   g_bBeep = bBeep;
   return AreTaskKeysDisabled();
}      
    TaskKeyHook 输出两个函数：DisableTaskKeys 和 AreTaskKeysDisabled。前者安装WH_KEYBOARD_LL 钩子；后者判断这个钩子是否安装。此键盘钩子的处理思路是截获Alt+Tab，Ctrl+Esc，Alt+Esc以及Windows 键VK_LWIN/VK_RWIN，关于这两个键，稍候会有详细描述。当钩子碰到这些键时，它直接返回到调用者，而不是将处理传递给CallNextHookEx 。
LRESULT CALLBACK MyTaskKeyHookLL(...)
{
   if (/* 任务键*)
      return 1; // 立即返回
   return CallNextHookEx(...);
}      
    TaskKeyHook的大部分实现都很简单。只有一个地方用到了一点小技巧：既使用#pragma data_seg 命名包含全程数据的数据段，并且用#pragma comment (linker...)告诉链接器让这个数据段为共享段。实现细节请参考源代码。 本文附带的例子程序（TrapKeys.exe）汇集了上述几个有关屏蔽键盘按键序列的功能，除此之外，它还有一个功能就是禁用任务栏。因为既然禁用了任务转换键，那么一般来说，也必然要禁用任务栏，否则禁用任务转换键就没有意义了。禁用任务栏的具体方法如下：
HWND hwnd = FindWindow("Shell_traywnd", NULL);//找到任务栏
EnableWindow(hwnd, FALSE); // 禁用任务栏
如图四是例子程序运行画面： 
![](http://www.vckbase.com/document/journal/vckbase15/images/cfig020902_4.gif)
图四 TrapKeys程序运行画面 
以下是TrapKeys程序的实现代码：
/////////////////////////////////////////////////
// TrapKeys.cpp
//
#include "stdafx.h"
#include "resource.h"
#include "StatLink.h"
#include "TaskKeyMgr.h"
////////////////////
// 主对话框
//
class CMyDialog : public CDialog {
public:
   CMyDialog(CWnd* pParent = NULL) : CDialog(IDD_MYDIALOG, pParent) { }
protected:
   HICON m_hIcon;
   CStaticLink m_wndLink1;
   CStaticLink m_wndLink2;
   CStaticLink m_wndLink3;
   virtual BOOL OnInitDialog();
   // 命令/UI 的更新处理
   afx_msg void OnDisableTaskMgr();
   afx_msg void OnDisableTaskKeys();
   afx_msg void OnDisableTaskbar();
   afx_msg void OnUpdateDisableTaskMgr(CCmdUI* pCmdUI);
   afx_msg void OnUpdateDisableTaskKeys(CCmdUI* pCmdUI);
   afx_msg void OnUpdateDisableTaskbar(CCmdUI* pCmdUI);
   afx_msg LRESULT OnKickIdle(WPARAM,LPARAM);
   DECLARE_MESSAGE_MAP()
};
///////////////////////////////////////////////////////
// 标准的MFC 对话框应用类代码。
//
class CMyApp : public CWinApp {
public:
   virtual BOOL InitInstance() {
      // 初始化app：运行对话框
      CMyDialog dlg;
      m_pMainWnd = &dlg;
      dlg.DoModal();
      return FALSE;
   }
   virtual int ExitInstance() {
      // 为了按全起见，在退出程序的时候，将所有禁用的项目复原
      CTaskKeyMgr::Disable(CTaskKeyMgr::ALL, FALSE);
      return 0;
   }
} theApp;
BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
   ON_COMMAND(IDC_DISABLE_TASKKEYS,OnDisableTaskKeys)
   ON_COMMAND(IDC_DISABLE_TASKBAR, OnDisableTaskbar)
   ON_COMMAND(IDC_DISABLE_TASKMGR, OnDisableTaskMgr)
   ON_UPDATE_COMMAND_UI(IDC_DISABLE_TASKKEYS, OnUpdateDisableTaskKeys)
   ON_UPDATE_COMMAND_UI(IDC_DISABLE_TASKBAR, OnUpdateDisableTaskbar)
   ON_UPDATE_COMMAND_UI(IDC_DISABLE_TASKMGR, OnUpdateDisableTaskMgr)
   ON_MESSAGE(WM_KICKIDLE,OnKickIdle)
END_MESSAGE_MAP()
///////////////////////////////////////////////
// 初始化对话框：子类化超链接柄加栽图标
//
BOOL CMyDialog::OnInitDialog()
{
   CDialog::OnInitDialog();
   // 初始化超链接
   m_wndLink1.SubclassDlgItem(IDC_EMAIL,this);
   m_wndLink2.SubclassDlgItem(IDC_VCKBASEURL,this);
   m_wndLink3.SubclassDlgItem(IDC_VCKBASELINK,this);
   // 自己设置对话框图标。MFC不会为对话框应用程序设置它
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
   SetIcon(m_hIcon, TRUE);       // 打图标
   SetIcon(m_hIcon, FALSE);      // 小图标
   
   return TRUE;
}
////////////////////////////////////////////////////////
// 命令/UI 更新处理：写这些东西应该很轻松。
void CMyDialog::OnDisableTaskKeys()
{
   CTaskKeyMgr::Disable(CTaskKeyMgr::TASKKEYS,
      !CTaskKeyMgr::AreTaskKeysDisabled(), TRUE); // 蜂鸣
}
void CMyDialog::OnUpdateDisableTaskKeys(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(CTaskKeyMgr::AreTaskKeysDisabled());
}
void CMyDialog::OnDisableTaskbar()
{
   CTaskKeyMgr::Disable(CTaskKeyMgr::TASKBAR,
      !CTaskKeyMgr::IsTaskBarDisabled());
}
void CMyDialog::OnUpdateDisableTaskbar(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(CTaskKeyMgr::IsTaskBarDisabled());
}
void CMyDialog::OnDisableTaskMgr()
{
   CTaskKeyMgr::Disable(CTaskKeyMgr::TASKMGR,
      !CTaskKeyMgr::IsTaskMgrDisabled());
}
void CMyDialog::OnUpdateDisableTaskMgr(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(CTaskKeyMgr::IsTaskMgrDisabled());
}
////////////////////////////////////////////////////////
// 要想让ON_UPDATE_COMMAND_UI正常工作，这是必需的。
// 
LRESULT CMyDialog::OnKickIdle(WPARAM wp, LPARAM lCount)
{
   UpdateDialogControls(this, TRUE);
   return 0;
}      
   按上述方法尽管禁用了任务栏，但是还有一个机关没有处理，那就是按下Windows键仍然可以弹出“开始”菜单。显然在处理VK_LWIN之前，任务栏不会检查是否被启用。一般来讲，如果某个窗口被屏蔽掉，那么它就不再会处理用户在这个窗口的输入——这就是所谓的禁用（Disable）的含义。通常调用EnableWindow(FALSE)后自然就达到了这个目的。但是处理VK_LWIN/VK_RWIN按键的代码决不会去检查任务栏启用/禁用状态。对此，本文的处理办法仍然是利用键盘钩子。修改一下TaskKeyHook实现，增加对Windows键的捕获。这样按下“开始”菜单键之后什么也不会发生。希望没有漏掉其它的按键。如果哪位读者发现漏掉了什么键，请和我联系，以便把它加到键盘钩子中去。为了简单起见，我在类CTaskKeyMgr中封装了所有禁用的函数。下面是这个类的定义击实现文件：
TaskKeyMgr
////////////////////////////////////////
// TaskKeyMgr.h
//
#pragma once
#include "TaskKeyHook.h"
/////////////////////////////////////////////////////////////////////
// 使用这个类禁用任务键，任务管理器或任务栏。
// 用相应的标志调用Disable，如：CTaskMgrKeys::Disable(CTaskMgrKeys::ALL)；
// 
class CTaskKeyMgr {
public:
   enum {
      TASKMGR  = 0x01,  // 禁用任务管理器(Ctrl+Alt+Del)
      TASKKEYS = 0x02,  //禁用任务转换键(Alt-TAB, etc)
      TASKBAR  = 0x04,  //禁用任务栏
      ALL=0xFFFF        //禁用所有东西L
   };
   static void Disable(DWORD dwItem,BOOL bDisable,BOOL bBeep=FALSE);
   static BOOL IsTaskMgrDisabled();
   static BOOL IsTaskBarDisabled();
   static BOOL AreTaskKeysDisabled() {
      return ::AreTaskKeysDisabled(); // 调用 DLL 
   }
};
CPP实现
////////////////////////////////////////////////////////////////
// TaskKeyMgr.cpp
//
#include "StdAfx.h"
#include "TaskKeyMgr.h"
#define HKCU HKEY_CURRENT_USER
// 用于禁用任务管理器策略的注册表键值对
LPCTSTR KEY_DisableTaskMgr =
   "Software//Microsoft//Windows//CurrentVersion//Policies//System";
LPCTSTR VAL_DisableTaskMgr = "DisableTaskMgr";
///////////////////////////////////////////
// 禁用相关的任务键
// 
// dwFlags   =  表示禁用什么
// bDisable   = 禁用为 (TRUE) ，否则为启用 (FALSE)
// bBeep    =  按下非法键是否蜂鸣（指针对任务键）
//
void CTaskKeyMgr::Disable(DWORD dwFlags, BOOL bDisable, BOOL bBeep)
{
   // 任务管理器 (Ctrl+Alt+Del)
   if (dwFlags & TASKMGR) {
      HKEY hk;
      if (RegOpenKey(HKCU, KEY_DisableTaskMgr,&hk)!=ERROR_SUCCESS)
         RegCreateKey(HKCU, KEY_DisableTaskMgr, &hk);
      if (bDisable) { // 禁用任务管理器（disable TM）： set policy = 1
         DWORD val=1;
         RegSetValueEx(hk, VAL_DisableTaskMgr, NULL,
            REG_DWORD, (BYTE*)&val, sizeof(val));
      } else { // 启用任务管理器（enable TM）
         RegDeleteValue(hk,VAL_DisableTaskMgr);
      }
   }
   // 任务键 (Alt-TAB etc)
   if (dwFlags & TASKKEYS)
      ::DisableTaskKeys(bDisable,bBeep); // 安装键盘钩
   // 任务栏
   if (dwFlags & TASKBAR) {
      HWND hwnd = FindWindow("Shell_traywnd", NULL);
      EnableWindow(hwnd, !bDisable);
   }
}
BOOL CTaskKeyMgr::IsTaskBarDisabled()
{
   HWND hwnd = FindWindow("Shell_traywnd", NULL);
   return IsWindow(hwnd) ? !IsWindowEnabled(hwnd) : TRUE;
}
BOOL CTaskKeyMgr::IsTaskMgrDisabled()
{
   HKEY hk;
   if (RegOpenKey(HKCU, KEY_DisableTaskMgr, &hk)!=ERROR_SUCCESS)
      return FALSE; // 没有此键，不禁用
   DWORD val=0;
   DWORD len=4;
   return RegQueryValueEx(hk, VAL_DisableTaskMgr,
      NULL, NULL, (BYTE*)&val, &len)==ERROR_SUCCESS && val==1;
}      
    这个类中的函数都是静态的，实际上CTaskKeyMgr完全就是一个名字空间。你可以在自己的程序中随心所欲地使用它。例如，禁用任务转换按键和任务栏，但是不禁用Ctrl+Alt+Del：
CTaskKeyMgr::Disable(CTaskKeyMgr::TASKKEYS |
                     CTaskKeyMgr::TASKBAR, TRUE);      
    此外，还有几个函数是用来检查当前禁用了哪些东西，甚至可以在用户按下禁用键时发出蜂鸣声……自己去享受Paul的源代码吧！ 
转自 [http://www.vckbase.com/document/viewdoc/?id=424](http://www.vckbase.com/document/viewdoc/?id=424)
