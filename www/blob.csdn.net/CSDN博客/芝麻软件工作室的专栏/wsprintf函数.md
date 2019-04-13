
# wsprintf函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 11:00:19[seven-soft](https://me.csdn.net/softn)阅读数：206标签：[winapi																](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[class																](https://so.csdn.net/so/search/s.do?q=class&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=null&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=class&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=class&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=string&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)

接着上面，再继续实现更加强大的线程类。从上面的C＋＋类里可以看到，要在静态函数里使用类的成员就需要获取this指针，也就是通过CreateThread函数里把类的this指针传送进来的，这样在函数ThreadProc里的参数lpParameter就是this指针了。因此把参数lpParameter转换为CThread类指针，这样就可以使用类的成员。在这个例子里使用wsprintf函数来格式化线程ID输出来，下面就来详细地分析例子吧。
函数wsprintf声明如下：
WINUSERAPI
int
WINAPIV
wsprintfA(
__out LPSTR,
__in __format_string LPCSTR,
...);
WINUSERAPI
int
WINAPIV
wsprintfW(
__out LPWSTR,
__in __format_string LPCWSTR,
...);
\#ifdef UNICODE
\#define wsprintf wsprintfW
\#else
\#define wsprintf wsprintfA
\#endif // !UNICODE
**LPWSTR**是格化后输出的缓冲区。
**LPCWSTR**是输入格式化字符串。
**...**是可变参数。
调用这个函数的例子如下：
\#001 \#pragma once
\#002
\#003 //线程类。
\#004 //
\#005 class CThread
\#006 {
\#007 public:
\#008
\#009  CThread(void)
\#010  {
\#011          m_hThread = NULL;
\#012  }
\#013
\#014  virtual ~CThread(void)
\#015  {
\#016         if (m_hThread)
\#017         {
\#018               //删除的线程资源。
\#019               ::CloseHandle(m_hThread);
\#020         }
\#021
\#022  }
\#023
\#024  //创建线程
\#025  HANDLE CreateThread(void)
\#026  {
\#027         //创建线程。
\#028          m_hThread = ::CreateThread(
\#029               NULL,                    //安全属性使用缺省。
\#030               0,                         //线程的堆栈大小。
\#031               ThreadProc,                 //线程运行函数地址。
\#032               this,                      //传给线程函数的参数。
\#033               0,                         //创建标志。
\#034               &m_dwThreadID);        //成功创建后的线程标识码。
\#035
\#036         return m_hThread;
\#037  }
\#038
\#039  //等待线程结束。
\#040  void WaitFor(DWORD dwMilliseconds = INFINITE)
\#041  {
\#042         //等待线程结束。
\#043         ::WaitForSingleObject(m_hThread,dwMilliseconds);
\#044  }
\#045
\#046 protected:
\#047  //
\#048  //线程运行函数。
\#049  //
\#050  //
\#051  static DWORD WINAPI ThreadProc(LPVOID lpParameter)
\#052  {
\#053         //转换传送入来的参数。
\#054         CThread* pThread = reinterpret_cast<CThread *>(lpParameter);
\#055         if (pThread)
\#056         {
\#057               //线程返回码。
\#058               //调用类的线程处理函数。
\#059                return pThread->Run();
\#060         }
\#061
\#062         //
\#063         return -1;
\#064  }
\#065
\#066  //线程运行函数。
\#067  //在这里可以使用类里的成员，也可以让派生类实现更强大的功能。
\#068  //
\#069  virtual DWORD Run(void)
\#070  {
\#071         //输出到调试窗口。
\#072         ::OutputDebugString(_T("Run()线程函数运行/r/n"));
\#073
\#074         TCHAR chTemp[128];
**\#075        wsprintf(chTemp,_T("ThreadID=%d/r/n"),m_dwThreadID);**
\#076         ::OutputDebugString(chTemp);
\#077
\#078         return 0;
\#079  }
\#080
\#081 protected:
\#082  HANDLE m_hThread;         //线程句柄。
\#083  DWORD m_dwThreadID;          //线程ID。
\#084
\#085 };
\#086
在这个例子里主要在第54行获取类的指针，然后在59行调用运行类的成员函数Run()。通过这样的调用后，所有线程的代码都可以写在类CThread里面了，这样就达到编写更简单的代码和复用的目的，因此Run函数设计为虚函数，让派生类可以改写这个类的功能。


