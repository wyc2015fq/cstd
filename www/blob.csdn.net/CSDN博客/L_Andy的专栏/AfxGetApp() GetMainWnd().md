# AfxGetApp() GetMainWnd() - L_Andy的专栏 - CSDN博客

2012年09月26日 16:06:15[卡哥](https://me.csdn.net/L_Andy)阅读数：645标签：[application																[function																[access																[object																[class																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)


## AfxGetApp() GetMainWnd()的使用

卡哥（张明奇）

**AfxGetApp( )是全局的。AfxGetApp( )这个函数可以得到当前应用进程的指针，是CWinApp*类型的，通过这个指针可以访问到这个进程中的对象。比如在全局函数中要向对话框中的列表写数据。**

**void writeString(char* pString)**

**｛**

**   CWnd* pWnd = AfxGetApp()->GetMainWnd();   CMyDlg * pDlg;   pDlg=(CMyDlg *) pWnd;   pDlg->ShowMsg(pString);**

**｝**

**AfxGetApp()得到进程指针CWinApp*，通过这个指针可以得到pWnd。要不在全局函数里你怎么对已存在的对话框操作呢。**

**AfxGetApp( )这个函数可以得到当前引用的指针CWinApp*，通过这个指针可以访问到这个进程中的对象。**

**==============================================================================**

**我们在任何一个VC工程中都会看到一个类似于：**

**//Test.cpp文件中 **

**CTestApp theApp;（工程名为Test）**

**的语句。它是指向工程对象的一个指针，你可以用它方向Test工程的成员函数和变量。**

**如果工程中添加了一个CConfig类，在CConfig中想调用Test工程中的成员函数和变量，则可以这样调用之：**

**CTestApp *pApp=(CTestApp *)AfxGetApp( );**

**如果在CTest类中我们声明如下一些变量：**

**//Test.cpp文件**

**class CTestApp : public CWinApp{public:**

**CTestApp( );**

**CString str; //添加的对话框变量**

**...**

**}**

**//Test.cpp文件**

**CTestApp::CTestApp()**

**{**

**str="abc";**

**}**

**好了，现在我们就在CConfig类中调用str吧！**

**//Config.cpp文件**

**CTestApp *pApp=(CTestApp *)AfxGetApp( );**

**AfxMessageBox(pApp->str);**

**这样，就实现了对CTestApp类中变量的调用。**

**================================================================**

**afxgetapp    --    取应用程序实例指针   getmainwnd   --    取主窗口对象指针   这两个函数可以合成一个: afxgetmainwnd();   我通常把一些重要的工程一开始就需要初始化的并且在其它地方类中都要用到的变量或函数定义在C***App类中，然通过此函数获得这些变量或函数。AfxGetAppThe pointer returned by this function can be used to access applicationinformation such as the main message-dispatch code or the topmost window.**

**CWinApp* AfxGetApp();Return ValueA pointer to the single CWinApp object for the application.**

**Example// Print the application's executable filename.TRACE("Executable filename = %s\n", AfxGetApp()->m_pszExeName);**

**RequirementsWindows CE versions: 2.0 and laterHeader file: Declared in Afxwin.hPlatform: H/PC Pro, Palms-size PC, Pocket PC－－－－如果你定义为取全局变量，只需在需要的地方用extern声名即可如果把变量在你的C*App类中定义,就可以通过通过AfxGetApp()获取C*App的指针,这样就可以调用它的变量了.如果是**

**全局变量,这不需要用这个函数调用它AfxGetApp()的返回值是指向theApp的指针，与全局变量无关**


