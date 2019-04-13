
# SetWindowText函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:08:07[seven-soft](https://me.csdn.net/softn)阅读数：761


在开发软件里，有时候需要改变窗口上标题的文字。比如按钮上的文字，为了适应多国语言的显示，就需要改变它显示的内容。这时就需要使用SetWindowText函数来改它的内容。下面的例子就实现改变按钮的标题。
函数SetWindowText声明如下：
WINUSERAPI
BOOL
WINAPI
SetWindowTextA(
__in HWND hWnd,
__in_opt LPCSTR lpString);
WINUSERAPI
BOOL
WINAPI
SetWindowTextW(
__in HWND hWnd,
__in_opt LPCWSTR lpString);
\#ifdef UNICODE
\#define SetWindowText SetWindowTextW
\#else
\#define SetWindowText SetWindowTextA
\#endif // !UNICODE
**hWnd**是窗口的句柄。
**lpString**是要需要显示的文字。
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
**\#027                   SetWindowText(m_hBtn,_T("****改变它****"));**
\#028               }
\#029               else
\#030               {
\#031                    //设置按钮的文字。
**\#032                   SetWindowText(m_hBtn,_T("****按钮****"));**
\#033               }
\#034
\#035               //每一次都改变。
\#036               bChangeText = !bChangeText;
\#037         }
\#038         break;
\#039  default:
\#040         return CCaiWin::OnCommand(nID,nEvent);
\#041  }
\#042
\#043  return 1;
\#044 }


