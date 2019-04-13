
# SetWindowPos函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:06:28[seven-soft](https://me.csdn.net/softn)阅读数：206标签：[button																](https://so.csdn.net/so/search/s.do?q=button&t=blog)[qq																](https://so.csdn.net/so/search/s.do?q=qq&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=qq&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=qq&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=button&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=button&t=blog)

有一天，用户突然对我说，你这个窗口能不能放到最顶端，这样操作和打开文件就很不方便了。这个功能就需要改变窗口的属性了。比如大家使用QQ时，就有一个功能，设置QQ的窗口在最顶端，不管你选择了什么窗口，QQ的界面永远都在最前面。又像FlashGet的状态查看窗口，一直保持在窗口的最前端，让你看到当前下载的流量情况。现在股票那么火爆，很多人一边工作，一边查看股票，如果错失了机会，又少了很多钱的啊！面对这样的需求，就需要把一些窗口永远摆在最前面，这样起到提示用户的作用。因此，学会使用SetWindowPos函数，就成为能否让软件满足客户需求的关键了。与MoveWindow函数相比，SetWindowPos函数的功能比较强大一点。
函数SetWindowPos声明如下：
WINUSERAPI
BOOL
WINAPI
SetWindowPos(
__in HWND hWnd,
__in_opt HWND hWndInsertAfter,
__in int X,
__in int Y,
__in int cx,
__in int cy,
__in UINT uFlags);
**hWnd**是窗口的句柄。
**hWndInsertAfter**是窗口Z顺序属性。
**X**是窗口在X轴的位置。
**Y**是窗口在Y辆的位置。
**cx**是窗口的宽度。
**cy**是窗口的高度。
**uFlags**是选择设置的标志。
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
\#021         OutputDebugString(_T("按钮按下/r/n"));
\#022         {
\#023               static bool bChangeText = true;
\#024               if (bChangeText)
\#025               {
\#026                    //设置按钮的文字。
\#027                    SetWindowText(m_hBtn,_T("改变它"));
\#028
\#029                    //改变按钮窗口的位置和大小。
\#030                    MoveWindow(m_hBtn,10,10,100,32,TRUE);
\#031
**\#032                   //****改变主窗口为最顶端窗口。**
**\#033               SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);**
\#034               }
\#035               else
\#036               {
\#037                    //设置按钮的文字。
\#038                    SetWindowText(m_hBtn,_T("按钮"));
\#039
\#040                    //改变按钮窗口的位置和大小。
\#041                    MoveWindow(m_hBtn,50,50,100,32,TRUE);
\#042
**\#043                   //****改变主窗口为普通窗口。**
**\#044               SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);**
\#045               }
\#046
\#047               //每一次都改变。
\#048               bChangeText = !bChangeText;
\#049         }
\#050         break;
\#051  default:
\#052         return CCaiWin::OnCommand(nID,nEvent);
\#053  }
\#054
\#055  return 1;
\#056 }


