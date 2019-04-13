
# SetWindowLongPtr和GetWindowLongPtr函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:51:14[seven-soft](https://me.csdn.net/softn)阅读数：366


在软件开发里，大家一直对着这个问题是执着的，那是“复用”。总想自己写出来的代码，可以适应性很强，到那里都可以落地生根。因此，面向对象的语言就层出不穷，每个都坚称可以最大地复用代码。在面向对象里，C＋＋是非常强大的。下面就来用C＋＋把上面介绍的程序封装起来，这样可以复用，或者说条理更加清晰。
\#001
\#002 int APIENTRY _tWinMain(HINSTANCE hInstance,
\#003                       HINSTANCE hPrevInstance,
\#004                       LPTSTR    lpCmdLine,
\#005                       int       nCmdShow)
\#006 {
\#007  UNREFERENCED_PARAMETER(hPrevInstance);
\#008  UNREFERENCED_PARAMETER(lpCmdLine);
\#009
\#010  CCaiWin caiWin;
\#011
\#012  caiWin.MyRegisterClass(hInstance);
\#013  if (!caiWin.InitInstance(hInstance,nCmdShow))
\#014  {
\#015         return 0;
\#016  }
\#017
\#018  return caiWin.RunMessage();
\#019 }
这段代码跟前面介绍的调用，就是不一样了。
第10行创建了一个CCaiWin的对象caiWin。
第12行调用对象CCaiWin里的注册函数MyRegisterClass来注册一个窗口。
第13行就是初始化一个窗口的创建。
第18行就是调用对象caiWin的消息处理函数RunMessage。
这样就制定了一个基本应用的框架，可以任意修改对象里的内容，都不会影响这个函数里的调用，也就是说，只要不改那几个函数就可以永远不用修改WinMain函数里的内容了。
接着下来，再来看看类CCaiWin是怎么样编写的。它的类定义如下：
\#001 \#include <string>
\#002
\#003 //
\#004 //封装一个窗口类。
\#005 //
\#006 //
\#007 class CCaiWin
\#008 {
\#009 public:
\#010  CCaiWin(void);
\#011  virtual ~CCaiWin(void);
\#012
\#013  ATOM MyRegisterClass(HINSTANCE hInstance);
\#014  bool InitInstance(HINSTANCE hInstance, int nCmdShow);
\#015  int RunMessage(void);
\#016  HINSTANCE GetAppInstance(void)
\#017   {
\#018         return m_hInstance;
\#019  }
\#020 protected:
\#021  static LRESULT CALLBACK WndProc(HWND hWnd,
\#022         UINT message, WPARAM wParam, LPARAM lParam);
\#023  static INT_PTR CALLBACK About(HWND hDlg,
\#024         UINT message, WPARAM wParam, LPARAM lParam);
\#025 protected:
\#026  HINSTANCE m_hInstance;
\#027  HWND m_hWnd;
\#028
\#029  std::wstring m_strWindowClass;
\#030  std::wstring m_strTitle;
\#031 };
第7行定义类CCaiWin。
第13行是声明MyRegisterClass注册函数。
第14行是声明InitInstance初始化窗口函数。
第15行是声明RunMessage消息处理函数。
第16行是定义GetAppInstance函数获取应用程序句柄。
第21行是声明窗口的消息处理函数。它是静态成员函数，所以它有全局的地址，因此它是没有this指针的，不能直接地访问这个类里的成员变量。需要使用其它方法给它传递。
第23行是关于对话框的消息处理函数。
第26行是保存应用程序句柄。
第27行是保存主窗口的句柄。
第29行是保存注册窗口名称。
第30行是保存窗口显示的标题。
下面再来仔细地查看类的实现文件。
\#001 //
\#002 //函数: InitInstance(HINSTANCE, int)
\#003 //
\#004 //目的:保存程序实例句柄，并创建窗口显示。
\#005 //
\#006 //
\#007 //
\#008 bool CCaiWin::InitInstance(HINSTANCE hInstance, int nCmdShow)
\#009 {
\#010  //
\#011   m_hInstance = hInstance;
\#012
\#013   m_hWnd = CreateWindow(m_strWindowClass.c_str(), m_strTitle.c_str(),
\#014         WS_OVERLAPPEDWINDOW,
\#015         CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
\#016
\#017  if (!m_hWnd)
\#018  {
\#019         return false;
\#020  }
\#021
\#022  //保存类的指针到窗口GWL_USERDATA字段，
\#023  //以便消息函数里可以获取类指针。
\#024  SetWindowLongPtr(m_hWnd, GWL_USERDATA,(LONG)(LONG_PTR)this);
\#025
\#026  ShowWindow(m_hWnd, nCmdShow);
\#027  UpdateWindow(m_hWnd);
\#028
\#029  return true;
\#030 }
这里创建窗口，跟以前创建窗口，只有一个地方不一样，那就是在第24行里调用SetWindowLongPtr函数保存对象指针到窗口用户自定义数据里，这样做就是让后面的静态成员函数WndProc可以访问类成员。如下：
\#001 //
\#002 //函数: WndProc(HWND, UINT, WPARAM, LPARAM)
\#003 //
\#004 //目的:处理主窗口的消息.
\#005 //
\#006 //
\#007 //
\#008 LRESULT CALLBACK CCaiWin::WndProc(HWND hWnd, UINT message,
\#009
\#010 WPARAM wParam, LPARAM lParam)
\#011 {
**\#012  //****获取窗口对应的类指针。**
**\#013  LONG_PTR plptrWin = GetWindowLongPtr(hWnd,GWLP_USERDATA);**
**\#014  if (plptrWin == NULL)**
**\#015  {**
**\#016        return DefWindowProc(hWnd, message, wParam, lParam);**
**\#017  }**
**\#018**
**\#019  //**
**\#020  CCaiWin* pWin = reinterpret_cast<CCaiWin*>(plptrWin);**
**\#021**
\#022  int wmId, wmEvent;
\#023  PAINTSTRUCT ps;
\#024  HDC hdc;
\#025
\#026  switch (message)
\#027  {
\#028  case WM_COMMAND:
\#029         wmId    = LOWORD(wParam);
\#030         wmEvent = HIWORD(wParam);
\#031         //菜单选项命令响应:
\#032         switch (wmId)
\#033         {
\#034         case IDM_ABOUT:
\#035               DialogBox(pWin->GetAppInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX),
\#036                    hWnd, CCaiWin::About);
\#037               break;
\#038         case IDM_EXIT:
\#039               DestroyWindow(hWnd);
\#040               break;
\#041         default:
\#042               return DefWindowProc(hWnd, message, wParam, lParam);
\#043         }
\#044         break;
\#045  case WM_PAINT:
\#046         {
\#047               hdc = BeginPaint(hWnd, &ps);
\#048               //
\#049               std::wstring strShow(_T("C++窗口类的实现,2007-07-27"));
\#050               TextOut(hdc,10,10,strShow.c_str(),(int)strShow.length());
\#051
\#052               //
\#053               EndPaint(hWnd, &ps);
\#054         }
\#055         break;
\#056  case WM_DESTROY:
\#057         //设置窗口类指针为空。
\#058         SetWindowLongPtr(hWnd, GWL_USERDATA,NULL);
\#059
\#060         PostQuitMessage(0);
\#061         break;
\#062  default:
\#063         return DefWindowProc(hWnd, message, wParam, lParam);
\#064  }
\#065  return 0;
\#066 }
上面第13行就是获取窗口里保存的类对象指针，然后再作类型转换为窗口CCaiWin的指针，这样就可以使用类的成员了，比如在第35行里的调用pWin->GetAppInstance()。
其实在封装静态成员函数这里，就有三种方法传递类指针，上面介绍这种是最简单的。一种是MFC里使用的，它是采用一个窗口和类指针映射数组来实现的。一种是WTL里使用叫做THUNK代码实现窗口与静态函数的关联。像上面这种方法，在游戏Second
 Life的源程序就使用它，如果是一般的应用程序，而不是大框架，使用这种简单的方法，就是最好的。
函数GetWindowLongPtr和SetWindowLongPtr声明如下：
WINUSERAPI
LONG
WINAPI
GetWindowLongA(
__in HWND hWnd,
__in int nIndex);
WINUSERAPI
LONG
WINAPI
GetWindowLongW(
__in HWND hWnd,
__in int nIndex);
\#ifdef UNICODE
\#define GetWindowLong GetWindowLongW
\#else
\#define GetWindowLong GetWindowLongA
\#endif // !UNICODE
WINUSERAPI
LONG
WINAPI
SetWindowLongA(
__in HWND hWnd,
__in int nIndex,
__in LONG dwNewLong);
WINUSERAPI
LONG
WINAPI
SetWindowLongW(
__in HWND hWnd,
__in int nIndex,
__in LONG dwNewLong);
\#ifdef UNICODE
\#define SetWindowLong SetWindowLongW
\#else
\#define SetWindowLong SetWindowLongA
\#endif // !UNICODE
\#define GetWindowLongPtrA   GetWindowLongA
\#define GetWindowLongPtrW   GetWindowLongW
\#ifdef UNICODE
\#define GetWindowLongPtr GetWindowLongPtrW
\#else
\#define GetWindowLongPtr GetWindowLongPtrA
\#endif // !UNICODE
\#define SetWindowLongPtrA   SetWindowLongA
\#define SetWindowLongPtrW   SetWindowLongW
\#ifdef UNICODE
\#define SetWindowLongPtr SetWindowLongPtrW
\#else
\#define SetWindowLongPtr SetWindowLongPtrA
\#endif // !UNICODE
**hWnd**是窗口句柄。
**nIndex**是访问窗口对象数据的索引值。比如像GWLP_USERDATA、GWLP_WNDPROC。
**dwNewLong**是设置的新值。

