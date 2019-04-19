# 禁止窗口移动----API及MFC实现方法及原理 - xqhrs232的专栏 - CSDN博客
2014年11月05日 15:00:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2285
原文地址::[http://blog.163.com/jiang891203@126/blog/static/8704411820111013102351582/](http://blog.163.com/jiang891203@126/blog/static/8704411820111013102351582/)
相关文章
1、如何禁止拖动窗口？----[http://bbs.csdn.net/topics/290020376](http://bbs.csdn.net/topics/290020376)
2、对话框有标题栏，怎么禁止鼠标拖动窗口----[http://bbs.csdn.net/topics/350059939](http://bbs.csdn.net/topics/350059939)
3、[MFC对话框禁止移动 使标题栏不响应鼠标消息 通过客户区移动窗体 OnNcHitTest()的实现](http://blog.csdn.net/yue7603835/article/details/6658382)----[http://blog.csdn.net/yue7603835/article/details/6658382](http://blog.csdn.net/yue7603835/article/details/6658382)

一、C API实现方法
思路：如果用户想通过鼠标移动窗口，一定会按下鼠标左键，那么我们只要捕获到鼠标左键按下的消息，然后删除它即可。鼠标左键按下会产生两种类型的消息：WM_LBUTTONDOWN和WM_NCLBUTTONDOWN消息，那么WM_LBUTTONDOWN和WM_NCLBUTTONDOWN的区别是什么呢？先看看MSDN怎么说：
The WM_LBUTTONDOWN message is posted when the user presses the left mouse button while the cursor is in the client area of a window.
 If the mouse is not captured, the message is posted to the window beneath the cursor. Otherwise, the message is posted to the window that has captured the mouse.
The WM_NCLBUTTONDOWN message is posted when the user presses the left mouse button while the cursor is within the nonclient area of
 a window. This message is posted to the window that contains the cursor. If a window has captured the mouse, this message is not posted.
由此可以知道：
WM_LBUTTONDOWN是在左击客户区时响应； WM_NCLBUTTONDOWN是在左击非客户区时响应。
那么什么是客户区？什么是非客户区？
标题栏，菜单就是非客户区,标题栏、菜单等以外的窗体就是客户区。
所以我们应该处理非客户区的WM_NCLBUTTONDOWN消息：
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  ...
  switch (message)
 {
  case WM_COMMAND:
   wmId    = LOWORD(wParam);
   wmEvent = HIWORD(wParam);
   // Parse the menu selections:
   switch (wmId)
   {
     ...
caseWM_NCLBUTTONDOWN:
MSG msg;
PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE);
break;
    ...
  default:
   return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
二、接下来看看在MFC中如何实现的呢，同样也是处理非客户区的消息WM_NCLBUTTONDOWN，那么怎么去响应这个消息呢？
虚函数OnNcLButtonDown(UINT   nHitTest,   CPoint   point) 即是用来响应这个消息的，添加步骤与平时的有些不同：
Ctrl+W -> MFC ClassWizard,在出现的对话框，切换到Class Info 选项卡，Class name中指定类，再在Message filter中选择Windows,点击OK。在Class name中指定类中右击Add Windows Message Headle，在New Windows message/events中选定WM_NCLBUTTONDOWN，再点击Add and Edit 就会出现OnNcLButtonDown。
添加绿色部分代码即可实现。
void CTestThreadDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
 // TODO: Add your message handler code here and/or call default
 if (HTCAPTION == nHitTest) {
  return;
 }
 CDialog::OnNcLButtonDown(nHitTest, point);
}
HTCAPTION 的意思是the cursorIn's location is a title-bar area.
详细的可以查看MSDN中说明。
//=============================================================================================================
备注：：
1》WINCE下不支持WM_NCLBUTTONDOWN和WM_NCHITTEST这2个消息，但用ModifyStyleEx(0,   WS_EX_NODRAG,   0);是可以禁止对话框程序移动的！！！
# 如何
# //////////
# 禁止拖动窗口？
# 如何禁止拖动窗口？
