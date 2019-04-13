
# LoadString、LoadIcon和LoadCursor函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:53:16[seven-soft](https://me.csdn.net/softn)阅读数：350标签：[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[windows																](https://so.csdn.net/so/search/s.do?q=windows&t=blog)[交通																](https://so.csdn.net/so/search/s.do?q=交通&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[语言																](https://so.csdn.net/so/search/s.do?q=语言&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=交通&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=交通&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=file&t=blog)

在编写国际化的应用程序里，经常要使用不同语言的字符串。比如中文菜单里叫做“文件”，而在英语里叫做“File”。开发这种软件的功能是一样的，只是界面上显示的文字不一样而已。为了方便这种软件的开发，在Windows里经常使用的方法就是替换掉显示的字符串，比如指定在中文里就显示“文件”，在英语里就显示“File”，是通过函数LoadString从不同的资源里加载不同的字符串显示来实现的。其实所有可变的字符串，都可以使用函数LoadString从资源里加载字符串显示。
图像的信息是非常方便人们记忆的，像交通信号一样，就是使用各种各样的图标，每个人看了就会明白是什么东西，因此在程序里使用图标来标识程序。只要看到这个图标，就知道是这个软件做什么用的。比如在程序的左上角显示的图标，就需要调用函数LoadIcon从资源里加载到内存里，然后再显示出来。
光标更是最常用的图标了，时时刻刻都可以看到它。当你在编辑文件时，光标显示为一个“I”字形图标，当你在桌面操作文件时，显示为一个箭头。这样变化的光标，就是因为设置了不同的图标。光标的使用，就是根据不同的区域来作不同的指示。更换光标的函数是LoadCursor。
函数LoadString声明如下：
WINUSERAPI
int
WINAPI
LoadStringA(
__in_opt HINSTANCE hInstance,
__in UINT uID,
__out_ecount(cchBufferMax) LPSTR lpBuffer,
__in int cchBufferMax);
WINUSERAPI
int
WINAPI
LoadStringW(
__in_opt HINSTANCE hInstance,
__in UINT uID,
__out_ecount(cchBufferMax) LPWSTR lpBuffer,
__in int cchBufferMax);
\#ifdef UNICODE
\#define LoadString  LoadStringW
\#else
\#define LoadString  LoadStringA
\#endif // !UNICODE
**hInstance**是应用程序实例句柄。
**uID**是资源中的字符串编号。
**lpBuffer**是接收从资源里拷贝字符串出来的缓冲区。
**cchBufferMax**是指明缓冲的大小。
函数LoadIcon声明如下：
WINUSERAPI
HICON
WINAPI
LoadIconA(
__in_opt HINSTANCE hInstance,
__in LPCSTR lpIconName);
WINUSERAPI
HICON
WINAPI
LoadIconW(
__in_opt HINSTANCE hInstance,
__in LPCWSTR lpIconName);
\#ifdef UNICODE
\#define LoadIcon  LoadIconW
\#else
\#define LoadIcon  LoadIconA
\#endif // !UNICODE
hInstance是应用程序实例句柄。
lpIconName是指向图标的名称。
函数LoadCursor声明如下：
WINUSERAPI
HCURSOR
WINAPI
LoadCursorA(
__in_opt HINSTANCE hInstance,
__in LPCSTR lpCursorName);
WINUSERAPI
HCURSOR
WINAPI
LoadCursorW(
__in_opt HINSTANCE hInstance,
__in LPCWSTR lpCursorName);
\#ifdef UNICODE
\#define LoadCursor  LoadCursorW
\#else
\#define LoadCursor  LoadCursorA
\#endif // !UNICODE
hInstance是应用程序实例句柄。
lpCursorName是光标的名称。
调用这三个函数的例子如下：
\#001  //
\#002  //函数: MyRegisterClass()
\#003  //
\#004  //目的:注册一个窗口类型.
\#005  //
\#006  //

\#007  //
\#008  ATOM MyRegisterClass(HINSTANCE hInstance)
\#009  {
\#010    WNDCLASSEX wcex;
\#011
\#012    wcex.cbSize = sizeof(WNDCLASSEX);
\#013
\#014    wcex.style             = CS_HREDRAW | CS_VREDRAW;
\#015    wcex.lpfnWndProc  = WndProc;
\#016    wcex.cbClsExtra           = 0;
\#017    wcex.cbWndExtra        = 0;
\#018    wcex.hInstance      = hInstance;
**\#019    wcex.hIcon               = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWIN));**
**\#020    wcex.hCursor      = LoadCursor(NULL, IDC_ARROW);**
\#021    wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
\#022    wcex.lpszMenuName     = MAKEINTRESOURCE(IDC_TESTWIN);
\#023    wcex.lpszClassName     = szWindowClass;
\#024    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
\#025
\#026    return RegisterClassEx(&wcex);
\#027  }
第19行就是加载显示的图标，使用宏MAKEINTRESOURCE来转换为合适的类型，它的定义如下：
\#define IS_INTRESOURCE(_r) ((((ULONG_PTR)(_r)) >> 16) == 0)
\#define MAKEINTRESOURCEA(i) ((LPSTR)((ULONG_PTR)((WORD)(i))))
\#define MAKEINTRESOURCEW(i) ((LPWSTR)((ULONG_PTR)((WORD)(i))))
\#ifdef UNICODE
\#define MAKEINTRESOURCE  MAKEINTRESOURCEW
\#else
\#define MAKEINTRESOURCE  MAKEINTRESOURCEA
\#endif // !UNICODE
第20行是加载箭头的光标来显示。
//加载全局字符串。
LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
LoadString(hInstance, IDC_TESTWIN, szWindowClass, MAX_LOADSTRING);

