# Qt创建窗体的过程 - gauss的专栏 - CSDN博客
2013年01月04日 00:20:24[gauss](https://me.csdn.net/mathlmx)阅读数：351
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
来自：http://blog.csdn.net/tingsking18/article/details/5528666
前言：分析Qt的代码也有一段时间了，以前在进行QT源码解析的时候总是使用ue，一个函数名在QTDIR/src目录下反复的查找，然后分析函数之间的调用关系，效率实在是太低了，最近总结出一个更简便的方法，就是利用Qt Creator这个IDE。
带来的好处是：
1.       Qt Creator可以很方便的跟踪代码的调用，这样大大提高了分析代码的速度。
2.       函数间的调用关系能更加直观的找到。
3.       便于对代码的纵向关系的把握。
带来的坏处：
1.  只是展现了调用到的函数或者类的关系。
2.  缺少对类、某一组类、函数间关系的整体把握。
上面总结一下自己在QT源码解析时候用到的方法，下面开始步入正题。Qt创建窗体的过程，由于我对linux不是很熟悉，下面我所有的分析都是基于windows下的。
关于windows下利用API创建窗体。我这里就不多解释了，直接给出代码，然后结合下面的代码来分析一下Qt创建窗体的过程。
详细的解释请参考：
**[John Chen](http://blog.csdn.net/byxdaz/)**大牛的博文：**[WIN32 SDK界面编程](http://blog.csdn.net/byxdaz/archive/2010/04/20/5507566.aspx)**
**[cpp]**[view plain](http://blog.csdn.net/tingsking18/article/details/5528666#)[copy](http://blog.csdn.net/tingsking18/article/details/5528666#)
- #include <windows.h>  
- LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;  
- int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,  
-                     PSTR szCmdLine, int iCmdShow)  
- {  
-     static TCHAR szAppName[] = TEXT ("HelloWin") ;     
- 
-     HWND         hwnd ;  
-     MSG          msg ;  
- 
-     WNDCLASS     wc ;  
-     wc.style         = CS_HREDRAW | CS_VREDRAW ;  
-     wc.lpfnWndProc   = WndProc ;  
-     wc.cbClsExtra    = 0 ;  
-     wc.cbWndExtra    = 0 ;  
-     wc.hInstance     = hInstance ;  
-     wc.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;  
-     wc.hCursor       = LoadCursor (NULL, IDC_ARROW) ;  
-     wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;  
-     wc.lpszMenuName  = NULL ;  
-     wc.lpszClassName = szAppName ;  
-     if (!RegisterClass (&wc))  
-     {  
-         MessageBox (NULL, TEXT ("This program requires Windows NT!"), szAppName, MB_ICONERROR) ;  
-         return 0 ;  
- 
-     }  
-     hwnd = CreateWindow (szAppName,                  // window class name  
-         TEXT (“hello”), // window caption  
-         WS_OVERLAPPEDWINDOW,        // window style  
-         CW_USEDEFAULT,              // initial x position  
-         CW_USEDEFAULT,              // initial y position  
-         CW_USEDEFAULT,              // initial x size  
-         CW_USEDEFAULT,              // initial y size  
-         NULL,                       // parent window handle  
-         NULL,                       // window menu handle  
-         hInstance,                  // program instance handle  
-         NULL) ;                     // creation parameters  
-     ShowWindow (hwnd, iCmdShow) ;  
-     UpdateWindow (hwnd) ;  
-     while (GetMessage (&msg, NULL, 0, 0))  
-     {  
-         TranslateMessage (&msg) ;  
-         DispatchMessage (&msg) ;  
-     }  
-     return msg.wParam ;  
- 
- }  
- LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  
- {  
-     HDC         hdc ;  
-     PAINTSTRUCT ps ;  
-     RECT        rect ;  
-     switch (message)  
-     {  
-     case WM_PAINT:  
-         hdc = BeginPaint (hwnd, &ps) ;  
-         GetClientRect (hwnd, &rect) ;  
-         DrawText (hdc, TEXT ("the WM_PAINTmessage"), -1, &rect,DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;  
-         EndPaint (hwnd, &ps) ;  
-         return 0 ;  
-     case WM_DESTROY:  
-         PostQuitMessage (0) ;  
-         return 0 ;  
-     }  
-     return DefWindowProc (hwnd, message, wParam, lParam) ;  
- }  
先写一个最简单的Qt程序：
**[cpp]**[view plain](http://blog.csdn.net/tingsking18/article/details/5528666#)[copy](http://blog.csdn.net/tingsking18/article/details/5528666#)
- #include <QtGui/QApplication>  
- #include <QPushButton>  
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication a(argc, argv);  
-     QPushButton w("hello kitty");  
-     w.show();  
-     return a.exec();  
- }  
来分析一下这个窗体程序是如何创建的。
首先关于main函数和winmain函数，为什么Qt的窗口程序是用main函数而非winmain，在我的另外一篇博文中有解释：**[QT源码解析](http://blog.csdn.net/tingsking18/archive/2009/10/28/4737925.aspx)(一) QT创建窗口程序、消息循环和WinMain函数**这里不再解释
Windows窗体创建一定会调用RegisterClass这个函数的，我们在QTDIR/src里面搜索一下，有两个文件有这个函数一个是qapplication_win.cpp另外一个是qeventdispatcher_win.cpp，两个的作用不同，这次我们先研究qapplication_win.cpp中的RegisterClass函数，因为这个是与窗体创建有关的，下一篇[**QT源码解析(八）Qt是如何处理windows消息的**](http://blog.csdn.net/tingsking18/archive/2010/04/26/5528675.aspx)将会介绍qeventdispatcher_win.cpp中的RegisterClass的作用。
我们先将断点设置在qapplication_win.cpp中的 qt_reg_winclass 函数里，然后开始调试，运行到断点，然后我们看一下call stack如下图：
![](http://hi.csdn.net/attachment/201004/26/0_1272267874hmBZ.gif)
下面红色的框中为Call stack，我们可以看到函数调用的顺序，真正的创建QPushButton是在show()方法中，show()方法又调用了setVisible方法…… ……
QtWndProc就是窗体的回调函数，在RegisterClass的时候传给WNDCLASS结构的，QtWndProc同上面的API创建窗体的函数WndProc。
我们看一下QtWndProc的代码，也是一个switch (message) 然后一堆case来处理消息，最后也是调用DefWindowProc将不归他处理的消息交还给系统。
