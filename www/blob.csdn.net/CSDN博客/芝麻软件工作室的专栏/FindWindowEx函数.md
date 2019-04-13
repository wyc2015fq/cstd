
# FindWindowEx函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:03:33[seven-soft](https://me.csdn.net/softn)阅读数：411


当你想控制一个现有的窗口程序时，就需要获取那个程序的窗口句柄。比如有一些黑客软件需要查找到窗口，然后修改窗口的标题。在外挂流行的今天，惊奇地发现它们也可以修改输入窗口的文字。这其中，就需要使用到FindWindowEx函数来定位窗口。下面就来使用这个函数来实现控制Windows里带的计算器程序。打开计算器程序，最小化在状态下面，运行本例子，点击创建按钮后，就可以点按钮，就会把计算器显示在最前面。
函数FindWindowEx声明如下：
\#if(WINVER >= 0x0400)
WINUSERAPI
HWND
WINAPI
FindWindowExA(
__in_opt HWND hWndParent,
__in_opt HWND hWndChildAfter,
__in_opt LPCSTR lpszClass,
__in_opt LPCSTR lpszWindow);
WINUSERAPI
HWND
WINAPI
FindWindowExW(
__in_opt HWND hWndParent,
__in_opt HWND hWndChildAfter,
__in_opt LPCWSTR lpszClass,
__in_opt LPCWSTR lpszWindow);
\#ifdef UNICODE
\#define FindWindowEx FindWindowExW
\#else
\#define FindWindowEx FindWindowExA
\#endif // !UNICODE
**hWndParent**是找查窗口的父窗口句柄，如果父窗口是桌面，就可以设置为NULL。
**hWndChildAfter**是子窗口开始位置。
**lpszClass**是窗口注册的类型。
**lpszWindow**是窗口的标题。

调用这个函数的例子如下：
\#001 //
\#002 //响应命令.
\#003 //
\#004 //
\#005 LRESULT CCaiWinMsg::OnCommand(int nID,int nEvent)
\#006 {
\#007  //菜单选项命令响应:
\#008  switch (nID)
\#009  {
\#010  case IDC_CREATEBTN:
\#011         //显示一个按钮。
\#012         if (!m_hBtn)
\#013         {
\#014                m_hBtn = CreateWindow(_T("BUTTON"),_T("按钮"),
\#015                    WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,
\#016                    50,50,100,32,
\#017                     m_hWnd,(HMENU)IDC_BTN,m_hInstance,NULL);
\#018         }
\#019         break;
\#020  case IDC_BTN:
\#021         {
\#022**//****查找计算器的窗口。**
**\#023              HWND hWnd = FindWindowEx(NULL, NULL,NULL,_T("****计算器****"));**
\#024               if (hWnd != NULL)
\#025               {
\#026                    //窗口是否最小化。
\#027                    if (IsIconic(hWnd))
\#028                    {
\#029                          //恢复窗口。
\#030                          ShowWindow(hWnd,SW_RESTORE);
\#031                    }
\#032                    else
\#033                    {
\#034                          //显示窗口。
\#035                          ShowWindow(hWnd,SW_SHOWNORMAL);
\#036                    }
\#037
\#038                    //把窗口显示到最前面。
\#039                    BringWindowToTop(hWnd);
\#040
\#041                    OutputDebugString(_T("按钮按下/r/n"));
\#042               }
\#043         }
\#044         break;
\#045  default:
\#046         return CCaiWin::OnCommand(nID,nEvent);
\#047  }
\#048
\#049  return 1;
\#050 }


