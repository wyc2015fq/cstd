# 孙鑫-MFC笔记一--Windows程序内部运行机制 - 工作笔记 - CSDN博客





2012年05月02日 08:58:27[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5824








    对于一个初学者来说，常常会用到各种库函数，例如printf等等，这些库函数是由你所使用的编译器厂商提供的，在Windows操作系统下，开发的应用程序， 也有这样的库函数，不同的是，这样的库函数，是有windows操作系统提供的。windows操作系统，提供给用户开发window应用程序的这些应用程序编程的接口，就是API函数，API函数库包含帮助文档，使用手册，辅助工具等资源后，就组成了SDK。

     windows程序设计是种事件驱动方式的程序设计，主要基于消息的。当用户需要完成某种功能时，需要调用OS某种支持，然后OS将用户的需要包装成消息，并投入到消息队列中，最后应用程序从消息队列中取走消息并进行响应。

1 消息结构：

typedef struct tagMSG {     // msg 

    HWND   hwnd;     //接收消息的窗口句柄。和哪个窗口相关联。

    UINT   message;  //消息标识。消息本身是什么。

    WPARAM wParam;   //第三，第四个消息的附加信息。具体取决于消息本身。  

    LPARAM lParam; 

    DWORD  time;     //消息投递时间。 

    POINT  pt;       //消息投递时，光标在屏幕上的位置。 

} MSG;


2 消息队列 

每个应用程序OS都为它建立一个消息队列，消息队列是个先进先出的缓冲区，其中每个元素都是一个消息，OS将生成的每个消息按先后顺序放进消息队列中，应用程序总是取走当前消息队列中的第一条消息，应用程序取走消息后便知道用户的操作和程序的状态，然后对其处理即消息响应，消息响应通过编码实现。

消息分进队消息和不进队消息。进队消息是操作系统将其放入消息队列。然后由引用程序取出并发送，不进队消息则是由系统调用窗口过程时，直接发送给窗口。

不管是进队消息，还是不进队消息，最后都调用窗口过程函数对消息进行处理。

3 Windows程序入口 

int WINAPI WinMain(

  HINSTANCE hInstance,  // 当前事例句柄。

  HINSTANCE hPrevInstance,  // 先前事例句柄。常为空

  LPSTR lpCmdLine,      // 命令行指针

  int nCmdShow          // （窗口）显示的状态

);

说明：WinMain函数是Windows程序入口点函数，由OS调用，当OS启动应用程序的时候，winmain函数的参数由OS传递的。

4 创建一个完整的窗口需要经过下面四个操作步骤：

一，设计一个窗口类；如：WNDCLASS wndcls;

二，注册窗口类；    如：RegisterClass(&wndcls);

三，创建窗口；      如：CreateWindow(),CreateWindowEX();

四，显示及更新窗口。如：ShowWindow(),UpdateWindow();

说明：创建窗口的时候一定要基于已经注册的窗口类.

5. Windows提供的窗口类：

typedef struct _WNDCLASS { 

    UINT    style;        //窗口的类型

    WNDPROC lpfnWndProc;  //窗口过程函数指针（回调函数）

    int     cbClsExtra; //窗口类附加字节，为该类窗口所共享。通常0。

    int     cbWndExtra; //窗口附加字节。通常设为0。

    HANDLE  hInstance;  //当前应用程序事例句柄。

    HICON   hIcon;      //图标句柄 LoadIcon();

    HCURSOR hCursor;    //光标句柄 LoadCursor();

    HBRUSH  hbrBackground; //画刷句柄 (HBRUSH)GetStockObject();

    LPCTSTR lpszMenuName;  //菜单名字

    LPCTSTR lpszClassName; //类的名字 

} WNDCLASS; 

6,窗口类注册：

ATOM RegisterClass(

  CONST WNDCLASS *lpWndClass   //

);



7，创建窗口：

HWND CreateWindow(

  LPCTSTR lpClassName,  // 指定窗口类的名称

  LPCTSTR lpWindowName, // 窗口名，即标题栏

  DWORD dwStyle,        // 窗口类型

  int x,                // 窗口的左上角的x，y值

  int y,                // 

  int nWidth,           // 窗口宽度

  int nHeight,          // 窗口高度

  HWND hWndParent,      // 父窗口句柄

  HMENU hMenu,          // 子窗口或者菜单句柄

  HANDLE hInstance,     // 应用程序实例

  LPVOID lpParam        // WM_CREAT消息的附加参数lParam传递的数据信息。单文档为null。多文档指向CLIENTCREATSTRUCT结构体。

);

8 显示和更新窗口窗口：

BOOL ShowWindow(

  HWND hWnd,     // 窗口句柄

  int nCmdShow   // 显示状态（最大化，最小化，正常）

);

BOOL UpdateWindow(

  HWND hWnd   // 窗口句柄

);

9 消息循环：

MSG msg;

while(GetMessage(&msg,...))    //从消息队列中取出一条消息

{

 TranslateMessage(&msg); //进行消息（如键盘消息）转换

 DispatchMessage(&msg); //分派消息到窗口的回调函数处理,（OS调用窗口回调函数进行处理）。

}

其中：

BOOL GetMessage(

  LPMSG lpMsg,         // 消息结构体

  HWND hWnd,           // 窗口句柄

  UINT wMsgFilterMin,  // 获取消息的最小值，为0

  UINT wMsgFilterMax   // 获取消息队列的最大值，都为0时，表示获取所有消息

);

BOOL TranslateMessage(

  CONST MSG *lpMsg   // 消息结构体地址);

//The DispatchMessage function dispatches a message to a window procedure. 

LONG DispatchMessage(

  CONST MSG *lpmsg   // 消息结构体);



10 关于DC句柄获取：

a)使用BeginPaint(),EndPaint()对。注意只能在响应WM_PAINT消息时使用。

b)使用GetDc(),ReleaseDC()对。注意他们不能在响应WM_PAINT中使用。

11 WM_CLOSE与WM_DESTROY区别

DestroyWindow(hwnd)函数销毁窗口，但程序没有退出，返回一个WM_DESTROY；

通常，在响应WM_CLOSE时，判断是否退出程序，除非有特殊意义（销毁窗口，程序不退出）

PostQuitMessage(0)，投递WM_QUIT消息给线程，终止程序。



两种函数调用约定（__stdcall 和 __cdecl）：

#define CALLBACK    __stdcall 

//__stdcall 标准调用预定，是PASCAL 调用约定，象DELPHI使用的就是标准调用约定

#define WINAPIV     __cdecl  

// __cdecl 是C 语言形式的调用约定。



主要区别：函数参数传递顺序 和 对堆栈的清除上。

问题：除了那些可变参数的函数调用外，其余的一般都是__stdcall约定。但 
C/C++编译默认的是__cdecl约定。所以如果在VC等环境中调用__stdcall约定的函数，必须要在函数声明的时加上 __stdcall 修饰符，以便对这个函数的调用是使用__stdcall约定（如使用DELPHI编写的DLL时候）。

（VC中可通过这途径修改：project|settings..|c/c++|.代码生成)












