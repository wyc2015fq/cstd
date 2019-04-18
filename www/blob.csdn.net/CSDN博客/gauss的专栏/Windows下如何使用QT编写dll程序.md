# Windows下如何使用QT编写dll程序 - gauss的专栏 - CSDN博客
2013年01月03日 18:18:57[gauss](https://me.csdn.net/mathlmx)阅读数：263
来自：http://blog.csdn.net/tingsking18/article/details/4967172
Windows下如何使用QT编写dll程序
因为QT必须有调用QApplication的exec方法，这样才能产生消息循环，QT的程序才可以运行。所以说如果我们使用了QT编写了dll程序，在普通的windows程序中是不能调用的。在调用的时候会出现错误。当然QT提供了解决方法：那就是[QTWinmigrate](ftp://ftp.trolltech.com/qt/solutions/lgpl/qtwinmigrate-2.8-opensource.zip)
这里是QT官方网站对QTWinmigrate的介绍：
[http://qt.nokia.com/products/appdev/add-on-products/catalog/4/Windows/qtwinmigrate](http://qt.nokia.com/products/appdev/add-on-products/catalog/4/Windows/qtwinmigrate)
下面我来介绍一下使用QTWinmigrate来编写dll的方法。
首先，我们要重写DllMain函数：
**[cpp]**[view plain](http://blog.csdn.net/tingsking18/article/details/4967172#)[copy](http://blog.csdn.net/tingsking18/article/details/4967172#)
- #include <qtwinmigrate/qmfcapp.h>  
- #include <qtwinmigrate/qwinwidget.h>  
- #include <qmessagebox.h>  
- #include <windows.h>  
- BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved )  
- {  
-     static bool ownApplication = FALSE;  
-     if ( dwReason == DLL_PROCESS_ATTACH )  
-         ownApplication = QMfcApp::pluginInstance( hInstance );  
-     if ( dwReason == DLL_PROCESS_DETACH && ownApplication )  
-         delete qApp;  
-     return TRUE;  
- }  
大家都知道DllMain函数是windows动态库的入口函数，如果在dll中使用了QT的ui界面前，全局的QApplication必须首先要创建，并且应用程序必须创建EventLoop。
进入到QmfcApp::pluginInstance方法中去，
**[cpp]**[view plain](http://blog.csdn.net/tingsking18/article/details/4967172#)[copy](http://blog.csdn.net/tingsking18/article/details/4967172#)
- bool QMfcApp::pluginInstance(Qt::HANDLE plugin)  
- {  
-     if (qApp)  
-     return FALSE;  
-     QT_WA({  
-     hhook = SetWindowsHookExW(WH_GETMESSAGE, QtFilterProc, 0, GetCurrentThreadId());  
-     }, {  
-     hhook = SetWindowsHookExA(WH_GETMESSAGE, QtFilterProc, 0, GetCurrentThreadId());  
-     });  
-     int argc = 0;  
-     (void)new QApplication(argc, 0);  
-     if (plugin) {  
-     char filename[256];  
-     if (GetModuleFileNameA((HINSTANCE)plugin, filename, 255))  
-         LoadLibraryA(filename);  
-     }  
- 
-     return TRUE;  
- }  
我们可以看到：Qapplication被创建了出来。QmfcApp::pluginInstanc是为了保证进程中存在一个Qapplication对象，并且dll要把这个Qapplication的实例加载到内存中。
下面是dll中的导出函数：
**[cpp]**[view plain](http://blog.csdn.net/tingsking18/article/details/4967172#)[copy](http://blog.csdn.net/tingsking18/article/details/4967172#)
- extern "C" __declspec(dllexport) bool showDialog( HWND parent )  
- {  
-     QWinWidget win( parent );  
-     win.showCentered();  
-     QMessageBox::about( &win, "About QtMfc", "QtMfc Version 1.0/nCopyright (C) 2003" );  
- 
-     return TRUE;  
- }  
dll中的导出函数要用extern "C"形式，QwinWidget为native win32窗口提供堆栈等等。
这样还没有写完程序。不行你拿这个程序来
qmake -project
qmake
nmake
这样是无论如何也编译不过的。
如果你仔细看qtwinmigrate的example的话，你就会注意到：
include(D:/qt4.4.3/qtwinmigrate-2.8-opensource/src/qtwinmigrate.pri)
编译的时候一定要在*.pro文件中加上这一句！切记，切记！
