# WinCE控制面板添加应用程序----CPL文件格式 - xqhrs232的专栏 - CSDN博客
2013年01月23日 23:19:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：744
原文地址::[http://blog.csdn.net/nanjianhui/article/details/4336897](http://blog.csdn.net/nanjianhui/article/details/4336897)
相关网帖
1、透析WINCE的控制面板----[http://bbs.dzsc.com/space/viewspacepost.aspx?postid=91930](http://bbs.dzsc.com/space/viewspacepost.aspx?postid=91930)
2、[详解WINCE的控制面板](http://blog.csdn.net/norains/article/details/1189143)----[http://blog.csdn.net/norains/article/details/1189143](http://blog.csdn.net/norains/article/details/1189143)
作者：ARM-WinCE
WinCE系统中的控制面板和Windows系统中的控制面板原理是一样的，里面就是包含了一些应用程序。WinCE系统的控制面板由Ctlpnl.exe，Control.exe和一些.cpl文件组成，其中Ctlpnl.exe和Control.exe用于控制控制面板的文件夹显示和架构，而.cpl文件和控制面板中的实际应用程序相对应。
WinCE的控制面板中的每个应用程序都由两部分组成：应用程序本身和所对应的.cpl文件。实际上.cpl文件就是一个dll，在该dll中会导出CPLApplet函数，该函数会处理来自控制面板的消息(CPL_INIT, CPL_DBCLK, CPL_STOP等)，然后根据相应的消息来调用应用程序。
先来看一下CPlApplet函数，如下：
**LONG CPlApplet(HWND hwndCPl, UINT msg, LPARAM lParam1, LPARAM lParam2)**
**hwndCPl**：控制面板窗口的句柄
**msg**：发给控制面应用程序的消息
**lParam1**：消息参数1
**lParam2**：消息参数2
该函数会根据msg参数传入的消息进行相应的处理，WinCE中所支持的控制面板消息如下：
**CPL_INIT：**被首次加载的时候会收到该消息，也是第一个消息，用于初始化控制面板应用程序，比如内存分配等。
**CPL_GETCOUNT**：第二个被收到的消息，该消息用于获得该控制面板应用程序中的组件数，因为.cpl文件中可能包含多个Applet程序。
**CPL_NEWINQUIRE**：查询组件信息，如果该.cpl中包含多个组件，那么lParam1表示组件号，lParam2是一个指向NEWCPLINFO结构的指针，其中NEWCPLINFO结构用于描述组件信息。
**CPL_DBCLK**：用户在控制面板界面中双击某个应用时，会收到该消息，在该消息中执行对应的应用程序。如果包含多个组件，那么lParam1表示组建号，lParam2为传给应用程序的数据。
**CPL_STOP**：关闭控制面应用程序时，收到该消息，用于释放资源。如果包含多个组件，那么lParam1表示组件号，lParam2为传给应用程序的数据。
**CPL_EXIT**：在CPL_STOP消息之后，控制面板释放该应用程序时，收到该消息。
在CPlApplet中收到CPL_NEWINQUIRE消息时，会初始化NEWCPLINFO结构来描述组件信息，该结构定义如下：
typedef struct tagNEWCPLINFO {
  DWORD dwSize;
  DWORD dwFlags;
  DWORD dwHelpContext;
  LONG lData;
  HICON hIcon;
  TCHAR szName[32];
  TCHAR szInfo[64];
  TCHAR szHelpFile[128];
} NEWCPLINFO;
**dwSize**：该结构的信息
**dwFlags**：忽略
**dwHelpContext**：忽略
**lData**：传给组建程序的数据
**hIcon**：显示在控制面板中的图标的句柄
**szName**：显示在控制面板中的组件的名字
**szInfo**：显示在控制面板中的描述信息
**szHelpFile**：忽略
前面介绍了控制面板的基础知识，下面就开始添加应用程序到WinCE控制面板中，步骤如下：
**1. 创建一个WinCE的工程，然后添加一个应用程序：**
首先要有一个WinCE的工程，然后点击File->New->Subproject，然后选择WCE Application，然后可以选择Hello World应用程序。
**2. 为应用程序创建CPL工程：**
同样点击File->New->Subproject，然后选择WCE Dynamic-Link Library，工程名为HelloCPL，然后点击Next，在Auto-generated subproject files页面中选择A Dll that exports some symbols，然后点击Finish完成。
**3. 添加CPlApplet函数：**
打开HelloCPL工程，并打开HelloCPL.cpp文件，添加如下头文件：
#include <tchar.h>
#include “cpl.h”
然后添加如下代码：
#define lengthof(exp) ((sizeof((exp)))/sizeof((*(exp))))
HMODULE g_hModule = NULL;   // Handle to the DLL.
BOOL APIENTRY DllMain( HANDLE hModule,
DWORD  ul_reason_for_call,
LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
                        {
                                    g_hModule = (HMODULE) hModule;
                        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// The entry point to the Control Panel application.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
extern "C" HelloCPL_API LONG CALLBACK CPlApplet(HWND hwndCPL,
                  UINT message, LPARAM lParam1, LPARAM lParam2)
{
  switch (message)
  {
    case CPL_INIT:
      // Perform global initializations, especially memory
      // allocations, here.
      // Return 1 for success or 0 for failure.
      // Control Panel does not load if failure is returned.
      return 1;
    case CPL_GETCOUNT:
      // The number of actions supported by this Control
      // Panel application.
      return 1;
    case CPL_NEWINQUIRE:
      // This message is sent once for each dialog box, as
      // determined by the value returned from CPL_GETCOUNT.
      // lParam1 is the 0-based index of the dialog box.
      // lParam2 is a pointer to the NEWCPLINFO structure.
    {
      ASSERT(0 == lParam1);
      ASSERT(lParam2);
      NEWCPLINFO* lpNewCplInfo = (NEWCPLINFO *) lParam2;
      if (lpNewCplInfo)
      {
         lpNewCplInfo->dwSize = sizeof(NEWCPLINFO);
         lpNewCplInfo->dwFlags = 0;
         lpNewCplInfo->dwHelpContext = 0;
         lpNewCplInfo->lData = IDI_HELLO;
         // The large icon for this application. Do not free this
         // HICON; it is freed by the Control Panel infrastructure.
         lpNewCplInfo->hIcon = LoadIcon(g_hModule,
                                  MAKEINTRESOURCE(IDI_HELLO));
          LoadString(g_hModule, IDS_APP_TITLE, lpNewCplInfo->szName,
                     lengthof(lpNewCplInfo->szName));
          LoadString(g_hModule, IDC_Hello, lpNewCplInfo->szInfo,
                     lengthof(lpNewCplInfo->szInfo));
          _tcscpy(lpNewCplInfo->szHelpFile, _T(""));
          return 0;
      }
      return 1;  // Nonzero value means CPlApplet failed.
    }
    case CPL_DBLCLK:
    {
      // The user has double-clicked the icon for the
      // dialog box in lParam1 (zero-based).
      PROCESS_INFORMATION pi = {0};
      if (CreateProcess(_T("//Windows//Hello.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))
      {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return 0;
      }
      return 1;     // CPlApplet failed.
    }
    case CPL_STOP:
      // Called once for each dialog box. Used for cleanup.
    case CPL_EXIT:
      // Called only once for the application. Used for cleanup.
    default:
      return 0;
  }
  return 1;  // CPlApplet failed.
}  // CPlApplet
具体不做解释了，相信自己看一下都能看明白。在上面的代码中，处理消息CPL_NEWINQUIRE的时候，加载了IDI_HELLO，IDS_APP_TITLE和IDC_Hello三个资源，分别是一个图标和两个字符串。为工程添加rc资源文件和resource.h头文件，导入图标资源和字符串资源。资源的导入和定义比较简单，所以不介绍具体步骤了。
**4. 修改HelloCPL工程配置：**
打开HelloCPL.bib文件，添加如下内容：
**MODULES**
**HelloCPL.cpl  $(_FLATRELEASEDIR)/HelloCPL.cpl               NK**
右击HelloCPL工程，选择Properities，选择General页面，在Custom Variables项中添加变量，变量名字为CPL，值为1。
再次右击HelloCPL工程，选择Properities，选择C/C++页面，确认Additional Macro Definitions的值为$(CDEFINES) -DHelloCPL_EXPORTS。设置DLL Entry Point项为DllMain。在Include Directories项中添加路径$(_PROJECTROOT)/cesysgen/oak/inc。
**5. 编译Hello应用程序和HelloCPL工程：**
编译开始创建的Hello应用程序和HelloCPL工程，在WinCE6.0中，编译完成后会自动Makeimg操作。
通过上面的步骤，可以把应用程序添加到WinCE系统的控制面板中，最后编译成功以后，就可以下载运行了，在此我添加了一个Hello的应用程序，名字叫Hello application，运行结果如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20090710/controlpanel_1.JPG)
//==============================================================================================
备注::
1>#include “cpl.h”-----这个文件的路径为D:\WINCE600\PUBLIC\COMMON\OAK\INC
2>这个例子提到的工程建立都是在OS project里面增加子工程的方式！！！，不知道能不能像一般的应用那样创建！！！
3>按上面的方法我怎么还是在控制面板里面看不到对应的图标程序呢？在WINDOWS目录下已经看到了Hello.exe和HelloCPL.CPL这两个问题了。不知道卡在那里，
觉得系统还没给我写的这个HelloCPL.CPL分配编号啊！象电源管理是第5个，在开始菜单的运行中输入"ctlpnl.exe
 /windows/cplmain.cpl,5"运行起来
4>原来是g_hModule
 = (HMODULE) hModule;这句没抄到，看来抄别人的东西也还有学问的！
5>现在是在控制面板可以看到我加的cpl图标了，但双击是对应的EXE跑不起来！！！----原来if
 (CreateProcess(_T("//Windows//Hello.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))这句有错-----修改为_T("\\Windows\\Hello.exe").WINCE下的路径表示方式问题！！！
