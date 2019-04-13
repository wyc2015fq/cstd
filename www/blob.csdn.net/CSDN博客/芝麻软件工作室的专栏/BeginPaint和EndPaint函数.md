
# BeginPaint和EndPaint函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 11:00:43[seven-soft](https://me.csdn.net/softn)阅读数：282标签：[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[callback																](https://so.csdn.net/so/search/s.do?q=callback&t=blog)[command																](https://so.csdn.net/so/search/s.do?q=command&t=blog)[游戏																](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)[struct																](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=command&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=command&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=callback&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=callback&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=windows&t=blog)

当人们使用软件时，大多数是想看到自己所需要的结果，比如玩RPG游戏，就是想看到自己所操作的主角做各种各样的事情。在2D的RPG游戏里，其实做的事情，就是不断地更新画面，也就是不断地显示BMP的图片。在普通的程序里，大多也是显示各种文本和图片的，但是有一种类型的应用程序是不怎么显示结果的，那就是服务程序。不管怎么样，只要我们想看到程序所执行后的结果，就需要在程序里显示出来。也就是需要调用BeginPaint和EndPaint函数。BeginPaint函数的作用是告诉Windows系统，要开始向显示卡输出内容了，把这次显示的操作请求放到系统显示队列里。由于系统上的显示卡往往只有一个，那么这种资源是独占的，所以操作系统会让显示操作线性化，保证每个窗口的显示是独立进行的，而不是A窗口显示一部份，或者B窗口显示一部份，而是A窗口显示完成后再让B窗口显示。因此，BeginPaint函数就是跟操作系统说，我需要显示了，你安排好吧。当BeginPaint返回时，就获取到系统的显示资源句柄，这样就可以调GDI一大堆函数来操作了。显示完成后，一定要记得调用函数EndPaint，因为使用BeginPaint函数请求了独占的显示资源后，如果不释放回去，就会让其它程序永远获取不到显示资源了，这样系统就死锁了。如果你有空仔细地查看一下Windows源程序，就会发现BeginPaint函数和EndPaint函数怎样构成的。比如在调用BeginPaint函数时先把光标隐藏起来，接着再显示用户显示的东西，最后调用EndPaint函数后，又把隐藏的光标显示出来。
函数BeginPaint函数和EndPaint函数声明如下：
WINUSERAPI
HDC
WINAPI
BeginPaint(
__in HWND hWnd,
__out LPPAINTSTRUCT lpPaint);
WINUSERAPI
BOOL
WINAPI
EndPaint(
__in HWND hWnd,
__in CONST PAINTSTRUCT *lpPaint);
**hWnd**是窗口句柄。
**lpPaint**是获取显示参数。它的结构定义如下：
typedef struct tagPAINTSTRUCT {
HDC hdc;
BOOL fErase;
RECT rcPaint;
BOOL fRestore;
BOOL fIncUpdate;
BYTE rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT;
**hdc**是获取设备句柄。
**fErase**是否擦新背景。
**rcPaint**是显示的窗口大小。
**fRestore**、**fIncUpdate**、**rgbReserved**是保留使用的参数。
BeginPaint函数的返回值也是显示设备的句柄。
调用这个函数的例子如下：
\#001 //
\#002 //目的:处理主窗口的消息.
\#003 //
\#004 //
\#005 //
\#006 LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
\#007 {
\#008  int wmId, wmEvent;
\#009  PAINTSTRUCT ps;
\#010  HDC hdc;
\#011
\#012  switch (message)
\#013  {
\#014  case WM_COMMAND:
\#015         wmId    = LOWORD(wParam);
\#016         wmEvent = HIWORD(wParam);
\#017         //菜单选项命令响应:
\#018         switch (wmId)
\#019         {
\#020         case IDM_ABOUT:
\#021               DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
\#022               break;
\#023         case IDM_EXIT:
\#024               DestroyWindow(hWnd);
\#025               break;
\#026         default:
\#027               return DefWindowProc(hWnd, message, wParam, lParam);
\#028         }
\#029         break;
\#030  case WM_PAINT:
**\#031        hdc = BeginPaint(hWnd, &ps);**
**\#032        //**
**\#033         EndPaint(hWnd, &ps);**
\#034         break;
\#035  case WM_DESTROY:
\#036         PostQuitMessage(0);
\#037         break;
\#038  default:
\#039         return DefWindowProc(hWnd, message, wParam, lParam);
\#040  }
\#041  return 0;
\#042 }
第31行调用函数BeginPaint。
第33行调用函数EndPaint。


