# 更改MFC对话框默认的窗口类名 - xqhrs232的专栏 - CSDN博客
2012年06月12日 16:11:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：905标签：[mfc																[dialog																[class																[button																[文本编辑																[wince](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=文本编辑&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/flyoxs/article/details/2812625](http://blog.csdn.net/flyoxs/article/details/2812625)
相关网帖
1.[关于VC中设置窗口类名](http://blog.csdn.net/tianxing009/article/details/5772488)----[http://blog.csdn.net/tianxing009/article/details/5772488](http://blog.csdn.net/tianxing009/article/details/5772488)
2.[修改MFC对话框默认窗口类名（wince+vs2005）](http://www.cnblogs.com/dahai/archive/2011/12/08/2280294.html) ----[http://www.cnblogs.com/dahai/archive/2011/12/08/2280294.html](http://www.cnblogs.com/dahai/archive/2011/12/08/2280294.html)
Windows操作系统中有一个概念——窗口类。窗口类是抽象的，它可以有很多窗口实例，即HWND/CWnd。在一个程序中，你可以定制并注册一个窗口类，然后用它创建窗口，也可以直接用已经注册的窗口类创建我们的窗口。
为了减轻我们的编程负担，也为了隐藏具体的实现，MFC已经为我们定制并注册了很多窗口类，我们可以直接使用它们。比如，对话框的窗口类为“#32770”，按钮的窗口类为“Button”，列表框的窗口类为“ListBox”……等等。
很多时候，我们需要用到FindWindow函数来查找已经存在的窗口，然后给它发送消息。或者查询系统是否已经存在指定的窗口（进程），如果存在，我们就不再创建程序的新进程，而仅仅是激活它。FindWindow函数的声明为：
- HWND FindWindow(      
- LPCTSTR lpClassName,
- LPCTSTR lpWindowName
- );
我们可以通过窗口类名（*lpClassName*）查找，也可以通过窗口标题文本（*lpWindowName*）查找，或者同时使用。窗口的标题文本并不总是十分可靠，很多时候，它是动态变化的。所以通过窗口类名来查找就很有必要了。
那么怎样把MFC提供给我们的对话框的默认窗口类名“#32770“改成我们程序特有的呢？以下是更改步骤：
1.    打开工程的资源视图
2.    更改对话框资源的ClassName属性。有两种方法：
第一种方法，在需要更改类名的对话框资源上右击，选择“属性”，打开属性对话框，可以看到有一个名为“Class Name”的属性，我们在其中输入一个自己定义的名称（如“MyPrivateClassName”）即可。但是，MFC默认设置这个属性选项是灰色不可用的，我们先要开启它。切换到资源视图，右击根节点（如“XxxXxx.rc”），选属性，去掉勾选“**Enable MFC Features**”项（如果是VS.Net，把“**MFC Mode property”**项改为False）。这样就可以修改对话框的Class
 Name属性了。
另一种方法是直接用文本编辑器打开资源文件，修改对话框定义代码，插入CLASS项，如：
- IDD_LIMITDLGINSTANCE_DIALOG DIALOGEX 0, 0, 195, 44
- STYLE DS_MODALFRAME | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU
- EXSTYLE WS_EX_APPWINDOW
- CAPTION "LimitDlgInstance"
- **CLASS "MyPrivateClassName"// Add your class name here!**
- FONT 8, "MS Sans Serif"
- BEGIN
-     DEFPUSHBUTTON   "OK",IDOK,138,7,50,14
-     PUSHBUTTON      "Cancel",IDCANCEL,138,23,50,14
-     PUSHBUTTON      "&Test!",IDC_BUTTON1,48,14,49,15
- END
3.    定制并注册新窗口类。在应用程序类的InitInstance()函数中添加以下代码，以注册资源文件中用到的新窗口类：
-  WNDCLASS wc;
- 
- // 获取窗口类信息。MFC默认的所有对话框的窗口类名为 #32770 
-  ::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);
- 
- // 改变窗口类名 
-  wc.lpszClassName = _T("**MyPrivateClassName**");
- 
- // 注册新窗口类，使程序能使用它 
-  AfxRegisterClass(&wc);
注意：
1.    在调用函数::GetClassInfo()时，请确保传入的第一个参数HINSTANCE是包含你对话框资源所在的dll或exe的进程实例。
2.    请确保资源中指定的Class Name与InitInstance()中指定的窗口类名完全相同，否则程序不能运行。
更改完成，请重建工程，运行程序，使用Spy++查看最终效果吧！
参考资料：[How to provide your own Window class name for an MFC dialog box](http://support.microsoft.com/default.aspx?scid=http://support.microsoft.com:80/support/kb/articles/Q251/0/59.ASP&NoWebContent=1)
 //==================================================================================
//备注::
[修改MFC对话框默认窗口类名（wince+vs2005）](http://www.cnblogs.com/dahai/archive/2011/12/08/2280294.html)
更改对话框资源的ClassName属性
- 使能ClassName属性：Resource view--->XXX.rc属性页--->MFC Mode：False 
- ClassName属性中填入新类名：NewDialog 
注册新的窗口类
- InitInstance()中加入代码（WinCE MFC默认对话框类名为Dialog，windows为#32770）
```
[]()
1      WNDCLASS wc;
2// 获取窗口类信息
3     BOOL b = ::GetClassInfo(AfxGetInstanceHandle(), _T( "Dialog" ), &wc);
4// 改变窗口类名
5     wc.lpszClassName = _T( "NewDialog" );
6// 注册新窗口类
7     AfxRegisterClass(&wc);
[]()
```
用Remote Spy可查看新对话框类名

