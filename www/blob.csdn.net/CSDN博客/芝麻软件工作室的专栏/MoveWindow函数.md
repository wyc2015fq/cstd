
# MoveWindow函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:07:17[seven-soft](https://me.csdn.net/softn)阅读数：397标签：[button																](https://so.csdn.net/so/search/s.do?q=button&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=button&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



当你设计一个对话框的窗口时，就需要布局好所有按钮、文本显示框等等，由于每个按钮都是一个窗口，那么就需要移动这些窗口到合适的位置，这时就需要使用到MoveWindow函数。或者当你的界面需要动态地修改按钮位置，比如窗口放大了，按钮就需要跟着移动，否则按钮还在原来的位置，放大也不会移动按钮的位置，这时也需要使用MoveWindow函数重新设置按钮的位置。只要你想移动窗口，就可以考虑使用这个函数来实现。
函数MoveWindow声明如下：
WINUSERAPI
BOOL
WINAPI
MoveWindow(
__in HWND hWnd,
__in int X,
__in int Y,
__in int nWidth,
__in int nHeight,
__in BOOL bRepaint);
**hWnd**是窗口的句柄。
**X**是窗口在X轴的位置。
**Y**是窗口在Y辆的位置。
**nWidth**是窗口的宽度。
**nHeight**是窗口的高度。
**bRepaint**是设置是否重画窗口和父窗口。当设置为TRUE时，就进行重画。当设置为FALSE时，就不进行重画，需要手功进行更新指定的区域。
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
\#**029                    //****改变按钮窗口的位置和大小。**
**\#030                   MoveWindow(m_hBtn,10,10,100,32,TRUE);**
\#031               }
\#032               else
\#033               {
\#034                    //设置按钮的文字。
\#035                    SetWindowText(m_hBtn,_T("按钮"));
\#036
**\#037                   //****改变按钮窗口的位置和大小。**
**\#038                   MoveWindow(m_hBtn,50,50,100,32,TRUE);**
\#039               }
\#040
\#041               //每一次都改变。
\#042               bChangeText = !bChangeText;
\#043         }
\#044         break;
\#045  default:
\#046         return CCaiWin::OnCommand(nID,nEvent);
\#047  }
\#048
\#049  return 1;
\#050 }


