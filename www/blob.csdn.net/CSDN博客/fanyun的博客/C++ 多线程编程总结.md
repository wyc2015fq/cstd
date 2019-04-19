# C++ 多线程编程总结 - fanyun的博客 - CSDN博客
2016年12月12日 22:17:19[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4722
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**一.什么是多线程？**
        在计算机编程中，一个基本的概念就是同时对多个任务加以控制。许多[程序设计](http://baike.so.com/doc/3745498-3934974.html)问题都要求程序能够停下手头的工作，改为处理其他一些问题，再返回主[进程](http://baike.so.com/doc/1591750-1682565.html)。可以通过多种途径达到这个目的。最开始的时候，那些掌握机器低级语言的[程序员](http://baike.so.com/doc/4237940-4440011.html)编写一些“中断服务例程”，主进程的暂停是通过[硬件](http://baike.so.com/doc/2368908-2504815.html)级的中断实现的。尽管这是一种有用的方法，但编出的[程序](http://baike.so.com/doc/10037936-10515822.html)很难移植，由此造成了另一类的代价高昂问题。中断对那些实时性很强的[任务](http://baike.so.com/doc/6685928-10509386.html)来说是很有必要的。但对于其他许多问题，只要求将问题划分进入[独立](http://baike.so.com/doc/781877-827271.html)运行的程序片断中，使整个程序能更迅速地响应用户的请求。
       最开始，线程只是用于分配单个处理器的[处理时间](http://baike.so.com/doc/4141131-4340807.html)的一种工具。但假如操作系统本身支持多个处理器，那么每个线程都可分配给一个不同的处理器，真正进入“并行运算”状态。从[程序设计语言](http://baike.so.com/doc/5797287-6010082.html)的角度看，多线程操作最有价值的特性之一就是程序员不必关心到底使用了多少个处理器。程序在[逻辑意义](http://baike.so.com/doc/4988060-5211639.html)上被分割为数个线程；假如机器本身安装了多个处理器，那么程序会运行得更快，毋需作出任何特殊的调校。根据前面的论述，大家可能感觉线程处理非常简单。但必须注意一个问题：共享资源！如果有多个线程同时运行，而且它们试图访问相同的资源，就会遇到一个问题。举个例子来说，两个线程不能将信息同时发送给一台打印机。为解决这个问题，对那些可共享的资源来说（比如打印机），它们在使用期间必须进入锁定状态。所以一个线程可将资源锁定，在完成了它的任务后，再解开（释放）这个锁，使其他线程可以接着使用同样的资源。
**二.多线程使用目的： **
     多线程是为了同步完成多项任务，不是为了提高运行效率，而是为了提高资源使用效率来提高系统的效率。线程是在同一时间需要完成多项任务的时候实现的。在开发C++程序时，一般在吞吐量、并发、实时性上有较高的要求。设计C++程序时，总结起来可以从并发、异步、缓存几点提高效率：
**三.多线程使用情形总结：**
在此先介绍下常用五种使用情形：
第一种使用情形：任务队列
1.以生产者-消费者模型设计任务队列
生产者-消费者模型是人们非常熟悉的模型，比如在某个服务器程序中，当User数据被逻辑模块修改后，就产生一个更新数据库的任务（produce），投递给IO模块任务队列，IO模块从任务队列中取出任务执行sql操作（consume）。
设计通用的任务队列，示例代码如下：
```cpp
void task_queue_t::produce(const task_t& task_)
 {
 lock_guard_t lock(m_mutex); 
 if (m_tasklist->empty())
{//! 条件满足唤醒等待线程 
m_cond.signal(); 
} 
m_tasklist->push_back(task_); 
}
 int task_queue_t::comsume(task_t& task_){
 lock_guard_t lock(m_mutex); 
 while (m_tasklist->empty())//! 当没有作业时，就等待直到条件满足被唤醒
{ 
if (false == m_flag)
{
 return -1;
 }
 m_cond.wait(); 
} 
task_ = m_tasklist->front(); 
m_tasklist->pop_front(); 
return 0;
 }
```
2.任务队列使用技巧
2.1 IO与逻辑分离
比如网络游戏服务器程序中，网络模块收到消息包，投递给逻辑层后立即返回，继续接受下一个消息包。逻辑线程在一个没有io操作的环境下运行，以保障实时性。示例：
```cpp
void handle_xx_msg(long uid, const xx_msg_t& msg){
 logic_task_queue->post(boost::bind(&servie_t::proces, uid, msg));
 }
```
注意，此模式下为单任务队列，每个任务队列单线程。
2.2连接池与异步回调
   比如逻辑Service模块需要数据库模块异步载入用户数据，并做后续处理计算。而数据库模块拥有一个固定连接数的连接池，当执行SQL的任务到来时，选择一个空闲的连接，执行SQL，并把SQL通过回调函数传递给逻辑层。其步骤如下：
(1).预先分配好线程池，每个线程创建一个连接到数据库的连接
(2).为数据库模块创建一个任务队列，所有线程都是这个任务队列的消费者
(3).逻辑层想数据库模块投递sql执行任务，同时传递一个回调函数来接受sql执行结果
示例如下：
```cpp
void db_t:load(long uid_,boost::functionpost(boost::bind(&db_t:load, uid, func));
```
注意，此模式下为单任务队列，每个任务队列多线程。
第二种使用情形：日志
    本文主要讲C++多线程编程，日志系统不是为了提高程序效率，但是在程序调试、运行期排错上，日志是无可替代的工具，相信开发后台程序的朋友都会使用日志。常见的日志使用方式有如下几种：
(1).流式，如logstream<<“start servie time[%d]” << time(0) <<” app name[%s]” <<app_string.c_str() << endl;
(2).Printf格式如：logtrace(LOG_MODULE,“start servie time[%d] app name[%s]”, time(0),app_string.c_str());
二者各有优缺点，流式是线程安全的，printf格式格式化字符串会更直接，但缺点是线程不安全，如果把app_string.c_str()换成app_string（std::string），编译被通过，但是运行期会crash（如果运气好每次都crash，运气不好偶尔会crash）。我个人钟爱printf风格，可以做如下改进：
(3).增加线程安全，利用C++模板的traits机制，可以实现线程安全。示例：
```cpp
template
 void logtrace(const char* module, const char* fmt, ARG1 arg1){
 boost::format s(fmt);
 f % arg1;
 }
```
     这样，除了标准类型+std::string传入其他类型将编译不能通过。这里只列举了一个参数的例子，可以重载该版本支持更多参数，如果你愿意，可以支持9个参数或更多。
(4).为日志增加颜色，在printf中加入控制字符，可以再屏幕终端上显示颜色，Linux下示例：printf(“33[32;49;1m [DONE] 33[39;49;0m”)
(5).每个线程启动时，都应该用日志打印该线程负责什么功能。这样，程序跑起来的时候通过top–H– p pid可以得知那个功能使用cpu的多少。实际上，我的每行日志都会打印线程id，此线程id非pthread_id，而其实是线程对应的系统分配的进程id号。
第三种使用情形：性能监控
        尽管已经有很多工具可以分析c++程序运行性能，但是其大部分还是运行在程序debug阶段。我们需要一种手段在debug和release阶段都能监控程序，一方面得知程序瓶颈之所在，一方面尽早发现哪些组件在运行期出现了异常。
      通常都是使用gettimeofday来计算某个函数开销，可以精确到微妙。可以利用C++的确定性析构，非常方便的实现获取函数开销的小工具,示例如下：
```cpp
struct profiler{
 profiler(const char* func_name){
 gettimeofday(&tv, NULL);
 }
 ~profiler(){
 struct timeval tv2;
 gettimeofday(&tv2, NULL);
 long cost = (tv.tv_sec - tv.tv_sec) * 1000000 + (tv.tv_usec - tv.tv_usec);
 //! post to some manager
 }
 struct timeval tv;
 };
#define PROFILER() profiler(__FUNCTION__)
```
Cost应该被投递到性能统计管理器中，该管理器定时讲性能统计数据输出到文件中。
第四种使用情形： Lambda编程
使用foreach代替迭代器
很多编程语言已经内建了foreach，但是c++还没有。所以建议自己在需要遍历容器的地方编写foreach函数。习惯函数式编程的人应该会非常钟情使用foreach，使用foreach的好处多多少少有些，如：
但主要是编程哲学上层面的。
示例：
void user_mgr_t::foreach(boost::function func_){ for (iterator it = m_users.begin(); it != m_users.end() ++it){ func_(it->second); } }
比如要实现dump接口，不需要重写关于迭代器的代码
```cpp
void user_mgr_t:dump(){
 struct lambda {
 static void print(user_t& user){
 //! print(tostring(user);
 }
 };
 this->foreach(lambda::print);
 }
```
实际上，上面的代码变通的生成了匿名函数，如果是c++ 11标准的编译器，本可以写的更简洁一些：
this->foreach([](user_t& user) {} );
但是我大部分时间编写的程序都要运行在centos上，你知道吗它的gcc版本是gcc 4.1.2，所以大部分时间我都是用变通的方式使用lambda函数。
Lambda函数结合任务队列实现异步
常见的使用任务队列实现异步的代码如下：
```cpp
void service_t:async_update_user(long uid){
 task_queue->post(boost::bind(&service_t:sync_update_user_impl,this, uid));
 }
 voidservice_t:sync_update_user_impl(long uid){
 user_t& user = get_user(uid);
 user.update()
 }
```
这样做的缺点是，一个接口要响应的写两遍函数，如果一个函数的参数变了，那么另一个参数也要跟着改动。并且代码也不是很美观。使用lambda可以让异步看起来更直观，仿佛就是在接口函数中立刻完成一样。示例代码：
```cpp
void service_t:async_update_user(long uid){
 struct lambda {
 static void update_user_impl(service_t*servie, long uid){
 user_t& user = servie->get_user(uid);
 user.update();
 }
 };
 task_queue->post(boost::bind(&lambda:update_user_impl,this, uid));
 }
```
这样当要改动该接口时，直接在该接口内修改代码，非常直观。
第五种情形：利用shared_ptr实现map/reduce
Map/reduce的语义是先将任务划分为多个任务，投递到多个worker中并发执行，其产生的结果经reduce汇总后生成最终的结果。Shared_ptr的语义是什么呢？当最后一个shared_ptr析构时，将会调用托管对象的析构函数。语义和map/reduce过程非常相近。我们只需自己实现讲请求划分多个任务即可。示例过程如下：
(1).定义请求托管对象，加入我们需要在10个文件中搜索“ohnice”字符串出现的次数，定义托管结构体如下：
```cpp
struct reducer{
 voidset_result(int index, long result) {
 m_result[index] = result;
 }
 ~reducer(){
 longtotal = 0;
 for(int i = 0; i < sizeof(m_result); ++i){
 total += m_result[i];
 }
 //!post total to somewhere
 }
 longm_result[10];
 };
```
1).定义执行任务的 worker
```cpp
void worker_t:exe(int index_, shared_ptrret) {
 ret->set_result(index, 100);
 }
```
2).将任务分割后，投递给不同的worker
```cpp
shared_ptr ret(new reducer());
 for(int i = 0; i < 10; ++i) { task_queue[i]->post(boost::bind(&worker_t:exe,i, ret));
 }
```
多线程综合使用实例：
CreateThread.h
```cpp
// MulThreadTest.h : PROJECT_NAME 应用程序的主头文件
//
#pragma once
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif
#include "resource.h"		// 主符号
// CMulThreadTestApp:
// 有关此类的实现，请参阅 MulThreadTest.cpp
//
class CMulThreadTestApp : public CWinApp
{
public:
	CMulThreadTestApp();
// 重写
	public:
	virtual BOOL InitInstance();
// 实现
	DECLARE_MESSAGE_MAP()
};
extern CMulThreadTestApp theApp;
```
MulThreadTest.h
```cpp
// MulThreadTest.h : PROJECT_NAME 应用程序的主头文件
//
#pragma once
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif
#include "resource.h"		// 主符号
// CMulThreadTestApp:
// 有关此类的实现，请参阅 MulThreadTest.cpp
//
class CMulThreadTestApp : public CWinApp
{
public:
	CMulThreadTestApp();
// 重写
	public:
	virtual BOOL InitInstance();
// 实现
	DECLARE_MESSAGE_MAP()
};
extern CMulThreadTestApp theApp;
```
MulThreadTestDlg.h
```cpp
// MulThreadTestDlg.h : 头文件
//
#pragma once
// CMulThreadTestDlg 对话框
class CMulThreadTestDlg : public CDialog
{
// 构造
public:
	CMulThreadTestDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_MULTHREADTEST_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
```
NewWinThread.h
```cpp
#pragma once
// CNewWinThread
class CNewWinThread : public CWinThread
{
	DECLARE_DYNCREATE(CNewWinThread)
protected:
	CNewWinThread();           // protected constructor used by dynamic creation
	virtual ~CNewWinThread();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	DECLARE_MESSAGE_MAP()
};
```
Resource.h
```cpp
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by MulThreadTest.rc
//
#define IDD_MULTHREADTEST_DIALOG        102
#define IDI_ICON1                       127
#define IDR_MAINFRAME                   128
#define IDD_DIALOG1                     129
#define IDI_ICON2                       131
#define IDI_ICON3                       132
#define IDI_ICON4                       133
#define IDC_PROGRESS1                   1000
// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        134
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
```
stdafx.h
```cpp
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
#pragma once
#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif
// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif
#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						
#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif
#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的
// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
```
CreateThread.cpp
```cpp
// CreateThread.cpp : implementation file
//
#include "stdafx.h"
#include "MulThreadTest.h"
#include "CreateThread.h"
extern UINT n;
// CCreateThread dialog
IMPLEMENT_DYNAMIC(CCreateThread, CDialog)
UINT CCreateThread::MulThreadFunc(LPVOID lpParam)							//线程函数,属于CCreateThread类的成员函数
{
	m_data* plp=(m_data *)lpParam;										//参数类型转化
	while(1)
	{
	  for(int pos=plp->m_pro.GetPos();pos<100;pos++)
		{
			(plp->m_pro).SetPos(pos);
			Sleep(100);
			if(plp->b)
			{
				ExitThread(0);
			}
		}	
	  plp->m_pro.SetPos(0);
	}
	return 0;
}
CCreateThread::CCreateThread(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateThread::IDD, pParent)
{
	check = FALSE;
}
CCreateThread::~CCreateThread()
{
}
void CCreateThread::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_d.m_pro);
}
BEGIN_MESSAGE_MAP(CCreateThread, CDialog)
	ON_BN_CLICKED(IDOK, &CCreateThread::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()
// CCreateThread message handlers
void CCreateThread::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_d.b = check;
	if(!check)
	{
		pThread=AfxBeginThread(MulThreadFunc,&m_d,0,0,4);				//创建新的线程	
		pThread->ResumeThread();										//开始执行
		GetDlgItem(IDOK)->SetWindowText(TEXT("暂停"));
		SetIcon(AfxGetApp()->LoadIcon(IDI_ICON2), FALSE);
	}
	else
	{
		GetDlgItem(IDOK)->SetWindowText(TEXT("继续"));
		SetIcon(AfxGetApp()->LoadIcon(IDI_ICON3), FALSE);
	}
	check = !check;
}
void CCreateThread::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CString showwin;
	showwin.Format(_T("Thread: %d"),n);
	SetWindowText(showwin);
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON4), FALSE);
}
```
MulThreadTest.cpp
```cpp
// MulThreadTest.cpp : 定义应用程序的类行为。
//
#include "stdafx.h"
#include "MulThreadTest.h"
#include "MulThreadTestDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CMulThreadTestApp
BEGIN_MESSAGE_MAP(CMulThreadTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()
// CMulThreadTestApp 构造
CMulThreadTestApp::CMulThreadTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}
// 唯一的一个 CMulThreadTestApp 对象
CMulThreadTestApp theApp;
// CMulThreadTestApp 初始化
BOOL CMulThreadTestApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	CMulThreadTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
```
MulThreadTestDlg.cpp
```cpp
// MulThreadTestDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "MulThreadTest.h"
#include "MulThreadTestDlg.h"
#include "CreateThread.h"
#include "NewWinThread.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CMulThreadTestDlg 对话框
UINT n=0;
CMulThreadTestDlg::CMulThreadTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMulThreadTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}
void CMulThreadTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CMulThreadTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMulThreadTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()
// CMulThreadTestDlg 消息处理程序
BOOL CMulThreadTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CMulThreadTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CMulThreadTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CMulThreadTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CCreateThread *pDlg;
	//pDlg = new CCreateThread;
	//pDlg->Create(IDD_DIALOG1);								//创建非模态对话框
	//pDlg->ShowWindow(SW_SHOW);								//显示对话
	n++;
	CWinThread *pthread=AfxBeginThread(RUNTIME_CLASS(CNewWinThread));
}
```
NewWinThread.cpp
```cpp
// NewWinThread.cpp : implementation file
//
#include "stdafx.h"
#include "MulThreadTest.h"
#include "NewWinThread.h"
#include "CreateThread.h"
// CNewWinThread
IMPLEMENT_DYNCREATE(CNewWinThread, CWinThread)
CNewWinThread::CNewWinThread()
{
}
CNewWinThread::~CNewWinThread()
{
}
BOOL CNewWinThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	CCreateThread *pDlg;
	pDlg = new CCreateThread;
	pDlg->Create(IDD_DIALOG1);								//创建非模态对话框
	pDlg->ShowWindow(SW_SHOW);								//显示对话
	return TRUE;
}
int CNewWinThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	delete this;
	return CWinThread::ExitInstance();
}
BEGIN_MESSAGE_MAP(CNewWinThread, CWinThread)
END_MESSAGE_MAP()
// CNewWinThread message handlers
```
stdafx.cpp
```cpp
// stdafx.cpp : 只包括标准包含文件的源文件
// MulThreadTest.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息
#include "stdafx.h"
```
        本程序对线程创建做了一些控制，可以很直观的了解线程创建和销毁的过程，若有不足，还请批评指正！


﻿﻿
﻿﻿
