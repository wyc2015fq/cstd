
# OutputDebugString函数 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月21日 06:31:23[seven-soft](https://me.csdn.net/softn)阅读数：461


在软件的开发过程里，大部份时间是在做什么呢？其实是在调试代码，毕竟人类的思维方式是从简单到复杂的过程，自然的认知过程也是从简单到复杂。因此，开发软件时，第一个版本总是先写简单的，然后慢慢地添加出错处理上去，也把一些没有想到的情况添加进去，那些没有想到的错误，就叫做BUG。要根除一个软件里的BUG，是一件不容易的事情，不但要对这个软件流程比较熟悉，还要对这个软件的目标比较了解，还需要对这个开发平台了解，当然还需要不断地测试不同的条件是否出错。软件的条件组合是2的N次方，所以不可能把所有组合都测试完成，只能选择比较重要的进行测试，只要不影响软件的使用，这个软件就算是一款能够挣钱的软件了，不要希望这个软件一点BUG都不出现，那是不现实的，或者说付出的代价也太大了，导致这个软件的开发成本过高，公司当然是不能生存下去。因此，怎么样去平衡可使用性与软件的BUG，就成为一件艺术的事情。不同的场合是不同的需求的，一定不要为了完美而完美。下面就介绍怎么样使用OutputDebugString来输出调试信息到开发平台里。
函数OutputDebugString声明如下：
WINBASEAPI
VOID
WINAPI
OutputDebugStringA(
__in LPCSTR lpOutputString
);
WINBASEAPI
VOID
WINAPI
OutputDebugStringW(
__in LPCWSTR lpOutputString
);
\#ifdef UNICODE
\#define OutputDebugString OutputDebugStringW
\#else
\#define OutputDebugString OutputDebugStringA
\#endif // !UNICODE
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
**\#021        OutputDebugString(_T("按钮按下/r/n"));**
\#022         break;
\#023  default:
\#024         return CCaiWin::OnCommand(nID,nEvent);
\#025  }
\#026
\#027  return 1;
\#028 }

