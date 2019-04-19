# MFC应用程序中添加控制台窗口 - 三少GG - CSDN博客
2012年04月17日 15:29:17[三少GG](https://me.csdn.net/scut1135)阅读数：4632
在MFC程序中输出调试信息的方法有两种，一种是使用TRACE宏，可以向Output窗口输出调试信息；另一种是用MessageBox，弹出消息框来输出调试信息，但会影响程序的运行。
其实有一种方法可以更为方便的输出调试信息，就是输出到控制台(Console)中，即不影响程序运行，又便于查看调试信息。方法如下：
**1. ** 使用时略显麻烦。需要建立全局的instance，使用时需要p->print.  不能格式输出，不太方便。 推荐指数：★★★
![](http://www.cppblog.com/images/cppblog_com/automateprogram/a.png)
在写MFC这个是一种调试的方法。 
[代码下载](http://www.cppblog.com/Files/AutomateProgram/MFCConsoleOut.rar)
++++++++++++++++++++++
[MFC程序利用控制台输出调试信息](http://www.diybl.com/course/3_program/c++/cppjs/200865/122072.html)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/cooltalk/mfcconsole.JPG)
        近日研究师兄的一个MFC程序，见其会生成一个控制台窗口输出信息，就如同ANSYS的Output窗口，觉得这个功能实在有用。 
        于是研究了一下他的代码，不过因为其为DLL工程，又可怜我的VC6打不开他的VS2005的Test工程，原先的工程没有办法编通过。 
        于是想把实现此功能的类拆出来，仔细研究之后，做了个Test工程，居然不好用，不知道哪里出了问题。 
        于是google之，有几个结果可供参考： 
(1)Creating
 a console for your MFC app's debug output[http://www.codeproject.com/KB/debug/mfcconsole.aspx?df=100&forumid=822&exp=0&select=2656] 
(2)MFC 利用控制台输出调试信息   [http://blog.csdn.net/wang23432/article/details/5747897](http://blog.csdn.net/wang23432/article/details/5747897)
(3) MFC/DLL 编程时用独立的控制台窗口显示用户自定义调试信息[这篇文章因为被转载多次，找不到出处，google标题即可找到] 
(4) GUI程序也能使用控制台窗口[http://bbs.51cto.com/thread-396954-1-1.html] 
        综合一下，其实现的方法基本一致。 
                1)调用`AllocConsole()`函数，创建一个Console； 
                2)调用`_cprintf()`函数，输出字符串； 
                        或者，重定向输出流（详见下文）； 
                3)调用`FreeConsole()`函数，释放Console。 
        为了使用方便，参考了师兄使用的“单例模式”，写了下面的class。 
```cpp
//.h
class CConsolePrinter  
{
public:
	void print(const char* str);
	static void Destroy();
	static CConsolePrinter* Instance();
	
	
	virtual ~CConsolePrinter();
protected:
	CConsolePrinter();
private:
	static CConsolePrinter* _instance;
	FILE *file;
};
//.cpp
CConsolePrinter* CConsolePrinter::_instance = 0;
CConsolePrinter::CConsolePrinter()
{
	// create a new console to the process
    AllocConsole();
	
    int hCrun;    
    hCrun = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    file  = _fdopen(hCrun, "w");
	
    // use default stream buffer
    setvbuf(file, NULL, _IONBF, 0);
    *stdout = *file;
	std::cout << "Ready!\n";
}
CConsolePrinter::~CConsolePrinter()
{
	FreeConsole();
	fclose(file);
}
CConsolePrinter* CConsolePrinter::Instance()
{
	if (_instance == 0)
	{
		_instance = new CConsolePrinter;
	}
	return _instance;
}
void CConsolePrinter::Destroy()
{
	if (_instance)
	{
		delete _instance;
	}
	_instance = 0;
}
void CConsolePrinter::print(const char *str)
{
	std::cout << str << std::endl;
}
  
    使用方法也很简单。
    首先在应用程序类的InitInstance()函数中添加CConsolePrinter::Instance();，例如：
BOOL CMfcWithConsoleApp::InitInstance()
{
	// details omitted
	// allocate a console
#ifdef _DEBUG
	CConsolePrinter::Instance();
#endif
                     // details omitted
	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}
  
    第二步在应用程序类的ExitInstance()函数中添加CConsolePrinter::Destroy();，例如：
int CMfcWithConsoleApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef _DEBUG
	CConsolePrinter::Destroy();
#endif
	
	return CWinApp::ExitInstance();
}
    然后就是在需要输出字符串信息的地方调用CConsolePrinter::Instance()->print();函数。
void CMfcWithConsoleDoc::OnEditCopy() 
{	// TODO: Add your command handler code here
#ifdef _DEBUG
	CConsolePrinter::Instance()->print("CMfcWithConsoleDoc::OnEditCopy() ");
#endif	
}
    需要注意的是，如果关闭控制台窗口会导致主程序退出。
```
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**2.**[MFC/DLL
 编程时用独立的控制台窗口显示用户自定义调试信息](http://www.moon-soft.com/doc/22467.htm)**推荐指数：****★★★★**
**支持格式输出！只需在待输出文件include "DBWindow.h",用DBWindowWrite 代替printf**
** 不好的地方是程序开始时就会弹出console，显得不太高级~**
我将这个功能封装为两个文件,请将此文本内容下载到本地，保存为正确的文件名后使用。在不改变源代码的情况下实现，Debug版显示调试信息窗口，在Release版下不现实调试窗口且不增加程序负担。
###############DBWindow.h##################
/********************************************************************
 创建日期: 2004/09/02
 文件名称: DBWindow.h
      作者: 刘磊(vietor)
      版本: 2.0
      邮箱: [liuleilover@163.com](mailto:liuleilover@163.com)
  创建目的:
      用VC MFC/DLL编程时通常Debug版需要将测试信息通过控制台输出,而
      编译成Release版时需要将这些输出调试信息的代码给注释掉，通
      常这些代码较多操作比较麻烦。
      这个程序的作用就是为了程序员在进行MFC/DLL软件开发时Debug版可以
      在一个单独的控制台窗口进行调试程序的输出，而做成Release版时
      不必手工将这些代码注释掉，由此程序自动完成。
      注意：在一个进程之内只存在一个控制台窗口，对于多个可能同时调试的DLL
      请用颜色识别。
  版权声明:
      您可以随意拷贝和使用这个程序的副本，但请保证所有文件的完整和
      不被修改，如果您有修改意见，请与作者联系。
*********************************************************************/
#ifndef _DBWINDOW_
#define _DBWINDOW_
#include <windows.h>
//控制台输出时的文本颜色
#define WDS_T_RED    FOREGROUND_RED
#define WDS_T_GREEN  FOREGROUND_GREEN
#define WDS_T_BLUE  FOREGROUND_BLUE
//控制台输出时的文本背景颜色
#define WDS_BG_RED  BACKGROUND_RED
#define WDS_BG_GREEN BACKGROUND_GREEN
#define WDS_BG_BLUE  BACKGROUND_BLUE
#ifdef _DEBUG
//设置控制台输出窗口标题
BOOL DBWindowTile(LPCTSTR tile);
//格式化文本输出
BOOL DBWindowWrite(LPCTSTR fmt,...);
//带颜色格式化文本输出
BOOL DBWindowWrite(WORD Attrs,LPCTSTR fmt,...);
#else
#define DBWindowTile
#define DBWindowWrite
#endif
#endif
###############DBWindow.cpp################
/********************************************************************
 创建日期: 2004/09/02
 文件名称: DBWindow.cpp
      作者: 刘磊(vietor)
      版本: 2.0
      邮箱: [liuleilover@163.com](mailto:liuleilover@163.com)
  创建目的:
      用VC MFC/DLL编程时通常Debug版需要将测试信息通过控制台输出,而
      编译成Release版时需要将这些输出调试信息的代码给注释掉，通
      常这些代码较多操作比较麻烦。
      这个程序的作用就是为了程序员在进行MFC/DLL软件开发时Debug版可以
      在一个单独的控制台窗口进行调试程序的输出，而做成Release版时
      不必手工将这些代码注释掉，由此程序自动完成。
      注意：在一个进程之内只存在一个控制台窗口，对于多个可能同时调试的DLL
      请用颜色识别。
  版权声明:
      您可以随意拷贝和使用这个程序的副本，但请保证所有文件的完整和
      不被修改，如果您有修改意见，请与作者联系。
*********************************************************************/
#include "stdafx.h"
#include "DBWindow.h"
#ifdef _DEBUG
#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>
#define CONSOLE_TILE _T("DBWindow Ver 2.0 by LiuLei")
class ConsoleWindow  
{
public:
  ConsoleWindow();
  virtual ~ConsoleWindow();
  BOOL SetTile(LPCTSTR lpTile);
  BOOL WriteString(LPCTSTR lpString);
  BOOL WriteString(WORD Attrs,LPCTSTR lpString);
private:
  HANDLE m_hConsole;
  BOOL   m_bCreate;
  BOOL   m_bAttrs;
  WORD   m_OldColorAttrs; 
};
ConsoleWindow::ConsoleWindow()
{
 m_hConsole=NULL;
 m_bCreate=FALSE;
 if(AllocConsole())
 {
  m_hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTitle(CONSOLE_TILE);
  SetConsoleMode(m_hConsole,ENABLE_PROCESSED_OUTPUT);
  m_bCreate=TRUE;
 }
 else{
  m_hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
  if(m_hConsole==INVALID_HANDLE_VALUE)
   m_hConsole=NULL;
 }
 if(m_hConsole)
 {
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
  if(GetConsoleScreenBufferInfo(m_hConsole, &csbiInfo))
  {
   m_bAttrs=TRUE;
   m_OldColorAttrs = csbiInfo.wAttributes;      
  }
  else{
   m_bAttrs=FALSE;
   m_OldColorAttrs = 0;
  }
 }
}
ConsoleWindow::~ConsoleWindow()
{
 if(m_bCreate)
  FreeConsole();
}
BOOL ConsoleWindow::SetTile(LPCTSTR lpTile)
{
  return SetConsoleTitle(lpTile);
}
BOOL ConsoleWindow::WriteString(LPCTSTR lpString)
{
 BOOL ret=FALSE;
 if(m_hConsole)
 {
  ret=WriteConsole(m_hConsole,lpString,_tcslen(lpString),NULL,NULL);
 }
 return ret;
}
BOOL ConsoleWindow::WriteString(WORD Attrs,LPCTSTR lpString)
{
  BOOL ret=FALSE;
  if(m_hConsole)
  {
    if(m_bAttrs)SetConsoleTextAttribute(m_hConsole,Attrs);
    ret=WriteConsole(m_hConsole,lpString,_tcslen(lpString),NULL,NULL);
    if(m_bAttrs)SetConsoleTextAttribute(m_hConsole,m_OldColorAttrs);
  }
  return ret; 
}
ConsoleWindow  ConWindow;
#define MAX_BUF_LEN 4096
BOOL DBWindowTile(LPCTSTR tile)
{
  return ConWindow.SetTile(tile);
}
BOOL DBWindowWrite(LPCTSTR fmt,...)
{
  TCHAR   message[MAX_BUF_LEN];
  va_list cur_arg;
  va_start(cur_arg,fmt);
  _vsntprintf(message,MAX_BUF_LEN,fmt,cur_arg);
  va_end(cur_arg);
  return ConWindow.WriteString(message);
}
BOOL DBWindowWrite(WORD Attrs,LPCTSTR fmt,...)
{
  TCHAR   message[MAX_BUF_LEN];
  va_list cur_arg;
  va_start(cur_arg,fmt);
  _vsntprintf(message,MAX_BUF_LEN,fmt,cur_arg);
  va_end(cur_arg);
  return ConWindow.WriteString(Attrs,message); 
}
#endif
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**3.**
[VC2010 MFC中实现printf调试功能，即MFC程序利用控制台输出调试信息](http://blog.chinaunix.net/uid-20672257-id-2973396.html) (2011-10-17 16:00)   **推荐指数：**★★★★★
1、在项目自动生成的stdafx.h文件中添加下面头文件
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
2、把下面的函数加到你初始化的地方，然后你就可以使用printf函数了
void InitConsoleWindow()
{
    int nCrt = 0;
    FILE* fp;
    AllocConsole();
    nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    fp = _fdopen(nCrt, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);
}
以下红色部分是我初始化函数中添加的
BOOL CSerialPortptestDlg::OnInitDialog()
{
 CDialogEx::OnInitDialog();
  CString str;
 int    Index;
 HKEY   hKey;
 LONG   ret;   
  OSVERSIONINFO     osvi;   
  BOOL   bOsVersionInfoEx;   
 char   keyinfo[100],comm_name[200],ValueName[200];   
 int   i;   
 DWORD   sType,Reserved,cbData,cbValueName;
 // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
 //  执行此操作
 SetIcon(m_hIcon, TRUE);   // 设置大图标
 SetIcon(m_hIcon, FALSE);  // 设置小图标
InitConsoleWindow();
 printf( "str   =   %s\n ",   "debug");
调用此函数后会弹出一个CONSOLE,然后printf的东西就会出现在上面。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**4.**[MFC
 利用控制台输出调试信息](http://blog.csdn.net/wang23432/article/details/5747897)**推荐指数：****★★★★★★**
第一种方案：
   WINDOWS为你提供了一系列的API来完成这个功能，例如：ReadConsole,WriteConsole等，具体参见MSDN。   
  1、首先调用AllocConsole函数来为你进程创建一个Console，该API是将当前进程Attache到一个新创建的   Console 上。你还可以通过调用SetConsoleTitle(tstrName)来设置Console的Title。   
  2、使用WriteConsoleOutput来将信息输出到Console上；在输出之前，先要得到Console的HANDLE,这通过 GetStdHandle(STD_OUTPUT_HANDLE)来得到，然后将信息组织成Console的格式，然后输出。   
  3、关闭CONSOLE。当不需要这个CONSOLE的时候，调用FreeConsole来将当前进程从Console中Detach中。   
  4、通过创建一个进程来为监视你的CONSOLE输入和输出；你可以创建一个线程然后来，在线程中取得标准输入和输出CONSOLE的HANDLE，然后循环监视其事件，再对不同的事件进行处理。   
第二种方案：   
      在 Project | Setting 中，选项 Post-builder  step 里新建command，输入：     
                             editbin   /SUBSYSTEM:CONSOLE   $(OUTDIR)/filename.exe     
               (其中 filename 为可执行文件名)     
      则可以使用 printf 或者 std::cout在控制台中输出了。
      例如你的可执行文件名为 HelloWorld.exe，则你新建的 command  就为    
               editbin /SUBSYSTEM:CONSOLE $(OUTDIR)/HelloWorld.exe      
需注意空格问题,eg:  editbin   /SUBSYSTEM:CONSOLE  C:/Users/.../Documents/"Visual Studio 2008"/Projects/HelloMFC2/Debug/HelloMFC2.exe
清理工程后重新编译。  **我用了第二种方案，比较快捷而且简单。  **
第三种方案：
      就用TRACE，然后编译debug的工程，直接运行你的可执行文件，可以用Dbgview来获得输出
第四种方案：
[http://community.csdn.net/Expert/FAQ/FAQ_Index.asp](http://community.csdn.net/Expert/FAQ/FAQ_Index.asp) id=186174
第五种方案：
      就是将你的调试信息输出到文件，如 *.txt 文件
+++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++
其余参考:
1.   没看懂： [http://blog.csdn.net/zhuqinglu/article/details/2454524](http://blog.csdn.net/zhuqinglu/article/details/2454524)
2.
# [MFC窗体程序中添加调试控制台](http://www.cnblogs.com/kekec/archive/2010/07/21/1782480.html)
在编写复杂程序的过程中，我们经常需要将一些信息输出到文件或者屏幕上。较控制台应用程序，MFC窗体程序要显得麻烦一些！
下面有2种方法来实现为MFC窗体程序添加调试控制台，方便程序员调试程序和了解当前程序的运行状态。
重要Windows API：AllocConsole();
   //创建Console窗口
FreeConsole();
     //销毁Console窗口
（1）启动控制台窗口
需要包含的头文件
#include <io.h>
#include <cstdio>
#include <FCNTL.H>
```
void CTestDebugConsoleDlg::OnBnClickedButton6()
{
// start
 debugconsole
AllocConsole();
intptr_t handle= (intptr_t)GetStdHandle(STD_OUTPUT_HANDLE);
int hCrt = _open_osfhandle(handle,_O_TEXT);
FILE * hf = _fdopen(
 hCrt, "w" );
*stdout =*hf;
char title[1024] ={0};
sprintf_s(title, 1024, "DebugCosole[%u]",
 (unsigned long)(this->GetSafeHwnd()));
SetConsoleTitle(title);
SetConsoleTextAttribute((HANDLE)handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
HWND hwnd=NULL; 
while(NULL==hwnd)
 hwnd=::FindWindow(NULL,(LPCTSTR)title); 
HMENU hmenu = ::GetSystemMenu
 ( hwnd, FALSE ); 
DeleteMenu ( hmenu, SC_CLOSE, MF_BYCOMMAND );
}
```
MFC窗体测试程序：
![](http://pic002.cnblogs.com/img/kekec/201007/2010072120031177.png)
Console信息显示：
![](http://pic002.cnblogs.com/img/kekec/201007/2010072120034539.png)
代码下载（VS2008）：[http://files.cnblogs.com/kekec/DebugConsole.rar](http://files.cnblogs.com/kekec/DebugConsole.rar)
（2）设计另外一个MFC窗体程序
关于这种方法请详见 Azure Product 的 “[实现一个通用的调试控制台](http://www.azure.com.cn/article.asp?id=417)”。
