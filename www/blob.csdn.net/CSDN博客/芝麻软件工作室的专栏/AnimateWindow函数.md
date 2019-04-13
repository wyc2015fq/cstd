
# AnimateWindow函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:05:30[seven-soft](https://me.csdn.net/softn)阅读数：194标签：[api																](https://so.csdn.net/so/search/s.do?q=api&t=blog)[qq																](https://so.csdn.net/so/search/s.do?q=qq&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=api&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



当你开发一款年轻人使用的软件时，肯定想用一点动感的特性来吸引他们。比如QQ软件就有这个特性，当你把它放到桌面边上时，就会自动隐藏起来，当你的鼠标放到那里时，就会自动慢慢移动出来，副有动感的特性，肯定让年轻人喜欢上它的。还有当你开发软件时，想先动态地显示公司的商标，或者公司的宣传材料时，就会使用到闪屏的效果。其实这两个特性都可以使用API函数AnimateWindow来实现的，下面就来介绍怎么使用它。
函数AnimateWindow声明如下：
\#if(WINVER >= 0x0500)
WINUSERAPI
BOOL
WINAPI
AnimateWindow(
__in HWND hWnd,
__in DWORD dwTime,
__in DWORD dwFlags);
\#endif /* WINVER >= 0x0500 */
hWnd是窗口的句柄。
dwTime是动态出现的时间。
dwFlags是显示效果的标志设置。
调用这个函数的例子如下：
\#001 //消息处理函数。
\#002 //
\#003 //
\#004 //添加动画窗口显示和隐藏。
\#005 //
\#006 LRESULT CCaiWinMsg::OnMessage(UINT nMessage,
\#007                                       WPARAM wParam, LPARAM lParam)
\#008 {
\#009  //
\#010  switch(nMessage)
\#011  {
\#012  case WM_PAINT:
\#013         return OnPaint(wParam,lParam);
\#014         break;
\#015  case WM_CREATE:
\#016
\#017         break;
\#018  case WM_DESTROY:
\#019         return OnDestroy(wParam,lParam);
\#020         break;
\#021  case WM_ACTIVATEAPP:
\#022         if (wParam == TRUE)
\#023         {
**\#024              //****窗口从上到下显示出来。**
**\#025              AnimateWindow(m_hWnd,1000,AW_SLIDE|AW_VER_POSITIVE);**
\#026         }
\#027         else
\#028         {
**\#029              //****窗口从下到上隐藏起来。**
**\#030              AnimateWindow(m_hWnd,1000,AW_HIDE|AW_VER_NEGATIVE);**
\#031         }
\#032         break;
\#033  }
\#034
\#035  return CCaiWin::OnMessage(nMessage,wParam,lParam);
\#036 }
\#037


