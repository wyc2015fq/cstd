
# DestroyWindow函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:54:46[seven-soft](https://me.csdn.net/softn)阅读数：301标签：[callback																](https://so.csdn.net/so/search/s.do?q=callback&t=blog)[command																](https://so.csdn.net/so/search/s.do?q=command&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=callback&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



以前已经介绍过直接点击关闭按钮来关闭应用程序，但想删除一个窗口对象，需要用到什么函数的呢？比如创建了30个窗口，想把第12个窗口关闭删除掉，那就需要使用函数DestroyWindow。当调用DestroyWindow函数后，操作系统就会进行一系列的删除动作，先发送WM_DESTROY消息，接着发送WM_NCDESTROY消息。如果这个窗口还有子窗口或者是其它窗口的所有者，就需要给所有子窗口发送删除消息。
函数DestroyWindow声明如下：
WINUSERAPI
BOOL
WINAPI
DestroyWindow(
__in HWND hWnd);
**hWnd**是要删除的窗口句柄。
调用这个函数的例子如下：
\#001 //
\#002 //函数: WndProc(HWND, UINT, WPARAM, LPARAM)
\#003 //
\#004 //目的:处理主窗口的消息.
\#005 //
\#006 //
\#007 //
\#008 LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
\#009  {
\#010  int wmId, wmEvent;
\#011  PAINTSTRUCT ps;
\#012  HDC hdc;
\#013
\#014  switch (message)
\#015  {
\#016  case WM_COMMAND:
\#017         wmId    = LOWORD(wParam);
\#018         wmEvent = HIWORD(wParam);
\#019         //菜单选项命令响应:
\#020         switch (wmId)
\#021         {
\#022         case IDM_ABOUT:
\#023               DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
\#024               break;
\#025         case IDM_EXIT:
**\#026              DestroyWindow(hWnd);**
\#027               break;
\#028         default:
\#029               return DefWindowProc(hWnd, message, wParam, lParam);
\#030         }
\#031         break;
\#032  case WM_PAINT:
\#033         hdc = BeginPaint(hWnd, &ps);
\#034         //
\#035         EndPaint(hWnd, &ps);
\#036         break;
\#037  case WM_DESTROY:
\#038         PostQuitMessage(0);
\#039         break;
\#040  default:
\#041         return DefWindowProc(hWnd, message, wParam, lParam);
\#042  }
\#043  return 0;
\#044 }
第26行是当收到菜单按钮退出命令的消息，就调用函数DestroyWindow，然后它发出消息WM_DESTROY给第37行那里进行处理。


