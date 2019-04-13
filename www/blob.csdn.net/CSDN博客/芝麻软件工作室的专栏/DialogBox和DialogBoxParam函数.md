
# DialogBox和DialogBoxParam函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:58:49[seven-soft](https://me.csdn.net/softn)阅读数：423


对话框是比较常用的窗口，比如当你想让用户输入一些参数时就可以使用对话框。或者提示一些警告的信息，都是可以使用对话框的。比如当你拷贝文件时，Windows就会提示一个拷贝文件的进度对话框。对话框的使用范围比较广，并且它在设计时就可以看到运行的结果模样，这样方便设计。但对话框又分为两类，一种对话框运行后，一定要用户关闭那个对话框后才能返回到父窗口；一种对话框是不需要关闭后就可以直接返回父窗口。因此，软件开发人员就要考虑这个对话框的结果是否会影响后面的操作，如果这个对话框的结果跟后面的操作没有因果关系的，可以设置为第二种对话框。像拷贝文件的对话框就是第二种的对话框，称作无模式的对话框。如果设置为第一类，非要等那里拷贝文件才可以去操作其它东西，那么Windows就不方便使用了，这样会浪费大量的时间。让人等待，就是一个不好用的软件，所以软件开发人员设计软件时，要站在用户的立场思考问题，在保持软件正确的情况下，不要让人等待，任何让人等待超过20秒以上的软件，会让用户烦躁不安。如果非要等待的话，也要加入进度条对话框提示，这样可以有效地缓解用户烦躁不安的心情。这就跟你去银行排队时，可以坐在那里看着电视，感觉不到时间长的道理一样的。
函数DialogBox函数和DialogBoxParam函数声明如下：
\#define DialogBoxA(hInstance, lpTemplate, hWndParent, lpDialogFunc) /
DialogBoxParamA(hInstance, lpTemplate, hWndParent, lpDialogFunc, 0L)
\#define DialogBoxW(hInstance, lpTemplate, hWndParent, lpDialogFunc) /
DialogBoxParamW(hInstance, lpTemplate, hWndParent, lpDialogFunc, 0L)
\#ifdef UNICODE
\#define DialogBox DialogBoxW
\#else
\#define DialogBox DialogBoxA
\#endif // !UNICODE
WINUSERAPI
INT_PTR
WINAPI
DialogBoxParamA(
__in_opt HINSTANCE hInstance,
__in LPCSTR lpTemplateName,
__in_opt HWND hWndParent,
__in_opt DLGPROC lpDialogFunc,
__in LPARAM dwInitParam);
WINUSERAPI
INT_PTR
WINAPI
DialogBoxParamW(
__in_opt HINSTANCE hInstance,
__in LPCWSTR lpTemplateName,
__in_opt HWND hWndParent,
__in_opt DLGPROC lpDialogFunc,
__in LPARAM dwInitParam);
\#ifdef UNICODE
\#define DialogBoxParam DialogBoxParamW
\#else
\#define DialogBoxParam DialogBoxParamA
\#endif // !UNICODE
**hInstance**是当前应用程序的实例句柄。
**lpTemplateName**是对话框的资源模板。
**hWndParent**是父窗口的句柄。
**lpDialogFunc**是对话框的消息处理函数。
**dwInitParam**是初始化参数，这里缺省设置为0。
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
\#021**DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);**
\#022               break;
\#023         case IDM_EXIT:
\#024               DestroyWindow(hWnd);
\#025               break;
\#026         default:
\#027               return DefWindowProc(hWnd, message, wParam, lParam);
\#028         }
\#029         break;
\#030  case WM_PAINT:
\#031         hdc = BeginPaint(hWnd, &ps);
\#032         //
\#033         EndPaint(hWnd, &ps);
\#034         break;
\#035  case WM_DESTROY:
\#036         PostQuitMessage(0);
\#037         break;
\#038  default:
\#039         return DefWindowProc(hWnd, message, wParam, lParam);
\#040  }
\#041  return 0;
\#042 }
第21行就是调用函数DialogBox来显示对话框窗口。
对话框的模板如下：
/////////////////////////////////////////////////////////////////////////////
//
// Dialog
//
IDD_ABOUTBOX DIALOGEX 22, 17, 230, 75
STYLE DS_SETFONT | DS_MODALFRAME | WS_CAPTION | WS_SYSMENU
CAPTION "关于"
FONT 9, "新宋体", 400, 0, 0x86
BEGIN
ICON            IDI_TESTWIN,IDC_MYICON,14,9,21,21
LTEXT           "TestWin Version 1.0",IDC_STATIC,49,10,119,8,SS_NOPREFIX
LTEXT           "Copyright (C) 2007",IDC_STATIC,49,20,119,8
DEFPUSHBUTTON   "确定",IDOK,185,51,38,16,WS_GROUP
END
IDD_ABOUTBOX是对话框模板的名称。
DIALOGEX是对话框定义的关键字。
22, 17, 230, 75是对话框的坐标和大小。
STYLE是设置对话框的显示类型。
CAPTION是定义标题名称。这里是"关于"。
FONT是定义对话的字体。
ICON是定义一个图标显示。
LTEXT是定义显示静态文本。
DEFPUSHBUTTON是定义一个按钮。


