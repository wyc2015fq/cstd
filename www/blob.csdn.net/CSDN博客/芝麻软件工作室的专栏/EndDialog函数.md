
# EndDialog函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:57:20[seven-soft](https://me.csdn.net/softn)阅读数：355标签：[callback																](https://so.csdn.net/so/search/s.do?q=callback&t=blog)[command																](https://so.csdn.net/so/search/s.do?q=command&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=callback&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



上一次介绍了怎么样显示对话框的函数，那么怎么样关闭对话框呢？这就需要使用到函数EndDialog。这个函数只能在对话框的消息处理函数里使用，并且这个函数调用之后，没有立即就删除对话框的，而是设置了操作系统里的结束标志。当操作系统查检到有这个标志时，就去删除对话框的消息循环，同时也去释放对话框占用的资源。其实对话框的生命周期是这样的，先由函数DialogBox创建对话框，这样函数DialogBox完成创建对话框但还没有显示前会发出消息WM_INITDIALOG，让对话框有机会初始化上面所有窗口或控件的显示，比如设置文本框的字符串等。最后当用户点出确定或者取消的按钮，就收到两个命令IDOK或IDCANCEL，这时就可以调用函数EndDialog来结束对话框的生命。
函数EndDialog声明如下：
WINUSERAPI
BOOL
WINAPI
EndDialog(
__in HWND hDlg,
__in INT_PTR nResult);
**hDlg**是对话框窗口的句柄。
**nResult**是设置给函数DialogBox的返回值。
调用这个函数的例子如下：
\#001 //显示关于对话框。
\#002 //
\#003 //
\#004 //
\#005 INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
\#006 {
\#007  UNREFERENCED_PARAMETER(lParam);
\#008  switch (message)
\#009  {
\#010  case WM_INITDIALOG:
\#011         return (INT_PTR)TRUE;
\#012
\#013  case WM_COMMAND:
\#014         if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
\#015         {
**\#016              EndDialog(hDlg, LOWORD(wParam));**
\#017               return (INT_PTR)TRUE;
\#018         }
\#019         break;
\#020  }
\#021  return (INT_PTR)FALSE;
\#022 }


