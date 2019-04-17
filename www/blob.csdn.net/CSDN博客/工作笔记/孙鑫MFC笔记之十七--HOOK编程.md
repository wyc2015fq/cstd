# 孙鑫MFC笔记之十七--HOOK编程 - 工作笔记 - CSDN博客





2012年07月02日 09:36:49[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：9011








通过安装Hook过程，可以用来屏蔽消息队列中某些消息

HHOOK SetWindowsHookEx(

int idHook,//钩子过程的类型

HOOKPROC lpfn,//钩子过程，如果dwThreadId 为0 ，或者指向一个其他进程创建的线程，则该参数必须指向一个位于动态链接库的钩子过程。否则，指向当前进程相关的代码中定义的钩子过程。

HINSTANCE hMod,//动态链接库句柄。如果dwThreadId 指定当前线程定义钩子过程，则该参数为NULL

DWORD dwThreadId   //为零表示和所有安装的线程相关

);



一、下面我们来创建一个屏蔽鼠标过程的hook:

1.创建基于MFC的一个InnerHook工程项目

2.在BOOL CInnerHookerDlg::OnInitDialog()中添加hook

SetWindowsHookEx(WH_MOUSE,MouseProc,NULL,GetCurrentThreadId());//当前线程的钩子过程安装。

要获得当前线程句柄，使用函数DWORD GetCurrentThreadId(void);

3.实现鼠标过程MouseProc为：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/teshorse/snap253.jpg)

LRESULT CALLBACK MouseProc(

int nCode,//如何处理当前消息

WPARAM wParam,//鼠标消息的信息

LPARAM lParam

)

{

return 1;  //返回值为一表示屏蔽鼠标过程

}



二、如果要屏蔽键盘消息，可以添加如下代码

1.在CPP文件中添加一个变量：HHOOK g_hKeyBoard;

2.在CInnerHookerDlg::OnInitDialog()中添加hook

g_hKeyBoard=SetWindowsHookEx(WH_KEYBOARD,KeyBoardProc,NULL,GetCurrentThreadId());



3.实现键盘过程keybroadProc为（只屏蔽空格键）：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/teshorse/snap254.jpg)

4.下边添加代码使程序在F2键按下后退出。

要关闭窗口，首先要获得窗口的句柄，先声明一个全局变量Hwnd g_hWnd，

在OnInitDialog()中把窗口句柄传给它:

g_hWnd=m_hWnd;

接下来为键盘钩子过程添加代码：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/teshorse/snap255.jpg)

这时我们只能屏蔽主线程的键盘消息，如果要屏蔽所有消息，就得把代码放到动态链接库中实现。

三、屏蔽所有线程的消息

首先要创建一个动态链接库

1.新建一个Win32 Dynamic-Link Library项目工程Hook

  得到动态链接库模块的句柄有两种方式：

     方法1。DllMain函数方式：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/teshorse/snap256.jpg)

HMODULE和HINSTANCE可以通用

     方法2。GetModuleHandle函数方式

SetWindowsHookEx(WH_MOUSE,MouseProc,GetModuleHandle("Hook"),0);

![](https://p-blog.csdn.net/images/p_blog_csdn_net/teshorse/snap257.jpg)

这样我们所安装的钩子过程就和运行在同一个桌面上的所有进程相关了

2.编写MouseProc()

![](https://p-blog.csdn.net/images/p_blog_csdn_net/teshorse/snap258.jpg)

3.之后新建一个模块文件Hook.def,添加代码:

LIBRARY Hook

EXPORTS

SetHook  @2   //@2用来指定序号。

4.编译生成dll文件



接下来新建一个工程，用来测试刚才的DLL

    首先安装一个鼠标Hook屏蔽所有的鼠标消息。

1.新建一个基于MFC对话框的项目工程HookTest

2.在BOOL CHookTestDlg::OnInitDialog()前声明SetHook函数

_declspec(dllimport) void SetHook();

3.在Setting对话框的Link选项卡的，添加库文件：../Hook/Debug/Hook.lib

4.在OnInitDialog()中调用 SetHook();

5.将生成好的动态链接库拷贝到测试程序项目工程目录下面。



调试运行，你会发现你的鼠标坏了，所有的鼠标操作都被屏蔽了。





    然后安装一个键盘Hook，我们可以按照刚才所做键盘Hook的过程在动态链接库中也做一个Hook，

    这是需要给SetHook带上参数HWND hwnd.

    在测试程序中要把函数也带上参数，并给SetHook传入窗口句柄 SetHook(m_hWnd)。





    接着，让程序窗口始终在其他窗口之前，而且将它最大化，从而使用户不能切换到窗口。

可以使用SetWindowPos函数

BOOL SetWindowPos(

HWND hWndInsertAfter,

int X,

int Y,

int cx,

int cy,

UINT uFlags

);

A window can be moved to the top of the Z-order either by setting the pWndInsertAfter parameter to &wndTopMost and ensuring that the SWP_NOZORDER flag is not set or by setting a window’s Z-order so that it is above any existing topmost windows. When a nontopmost
 window is made topmost, its owned windows are also made topmost. Its owners are not changed.



得到窗口的大小，可以使用函数GetSystemMetrics

int GetSystemMetrics(int nIndex);

代码：

int cxScreen,cyScreen;

cxScreen=GetSystemMetrics(SM_CXSCREEN);

cyScreen=GetSystemMetrics(SM_CYSCREEN);

SetWindowPos(&wndTopMost,0,0,cxScreen,cyScreen,SWP_SHOWWINDOW);



SetHook(m_hWnd);

因为第一个参数设置为&wndTopMost，这时程序始终处于顶层窗口，

不管怎样切换窗口，我们的窗口显示在最前面。







四、如何实现在切换到其他线程时，也能响应F2退出程序

在程序中，我们屏蔽了鼠标和键盘，但是我们留下了一个退出程序的后门（F2）。

前面讲过动态链接库共享性的原理，多个进程可以共享同一份代码与数据页，

按道理切换到其它线程之后，按下F2应该也可以退出程序才对，

但是发现当切换到其他程序后，再按F2 程序不会退出，

这是因为系统的页面拷贝机制，如果系统发现被某线程要修改某个数据页面，

它就会先拷贝一份页面数据，再对新的页面数据进行修改，

其它没有更新数据的线程继续使用旧的页面数据。

比如：SetHook(HWND hwnd)中将形参传递给了一个全局变量g_hWnd

，那么调用SetHook的线程将使用新的存放了hwnd的数据页面，

而其它的线程继续使用旧的数据页面，所以在其它线程成为活动窗口的时候，

按下F2时，因为没有g_hWnd没有传递到hwnd窗口，所以按下F2没有反应。



我们可以通过创建一个新的节，将全局变量放到这个节当中，然后将这个节设置为一个共享的节，

这样全局变量就可以在多个线程间共享，从而使切换到其他线程时也能按下F2退出程序。

要显示动态链接库的节，可以使用命令行：  dumpbin  -headers  Hook.dll



如何创建一个新的节？

如果确实想在其他程序窗口下关闭我们的程序窗口，可以把共享窗口句柄，使系统不再进行页面拷贝，方法是使用下面语句把窗口句柄设为共享：



#pragma data_seg("MySec")//MySec是新创建的节的名字（不能超过8个字节）

HWND g_hWnd=NULL;  //新变量必须初始化，否则没有新建节的信息

#pragma  data_seg()   //以上为新建节



新创建的节共享以后才有效，共享节有两种方法：

1.#pragma comment(linker,"/section:MySec,RWS")  //设置节的属性，读，写，共享

2.也可以把#pragma comment(linker,"/section:MySec,RWS")省略。

在Hook.def中添加如下代码：

SEGMENTS

MySec read write shared

也能对节的属性进行设置



把SetWindowsHookEx函数的第一个参数设为WH_GETMESSAGE，能够破解密码。

使用Hook时要小心。





