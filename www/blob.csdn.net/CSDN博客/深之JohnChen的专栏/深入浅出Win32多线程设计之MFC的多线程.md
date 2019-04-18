# 深入浅出Win32多线程设计之MFC的多线程 - 深之JohnChen的专栏 - CSDN博客

2006年03月31日 11:38:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4479标签：[多线程																[mfc																[attributes																[null																[initialization																[thread](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[多线程编程](https://blog.csdn.net/byxdaz/article/category/560198)


**深入浅出Win32多线程设计之MFC的多线程**

作者：宋宝华出处：天极开发责任编辑：[方舟](http://comments.yesky.com/t/%B7%BD%D6%DB/6,324/2280390.shtml)[ 2006-01-19 13:42 ]

在MFC程序中创建一个线程，宜调用AfxBeginThread函数

1、创建和终止线程

　　在MFC程序中创建一个线程，宜调用AfxBeginThread函数。该函数因参数不同而具有两种重载版本，分别对应工作者线程和用户接口（UI）线程。

　　工作者线程

CWinThread *AfxBeginThread(
　AFX_THREADPROC pfnThreadProc, //控制函数
　LPVOID pParam, //传递给控制函数的参数
　int nPriority = THREAD_PRIORITY_NORMAL, //线程的优先级
　UINT nStackSize = 0, //线程的堆栈大小
　DWORD dwCreateFlags = 0, //线程的创建标志
　LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL //线程的安全属性
);
　　工作者线程编程较为简单，只需编写线程控制函数和启动线程即可。下面的代码给出了定义一个控制函数和启动它的过程：

//线程控制函数
UINT MfcThreadProc(LPVOID lpParam)
{
　CExampleClass *lpObject = (CExampleClass*)lpParam;
　if (lpObject == NULL || !lpObject->IsKindof(RUNTIME_CLASS(CExampleClass)))
　　return - 1; //输入参数非法 
　//线程成功启动
　while (1)
　{
　　...//
　}
　return 0;
}

//在MFC程序中启动线程
AfxBeginThread(MfcThreadProc, lpObject);
　　UI线程

　　创建用户界面线程时，必须首先从CWinThread 派生类，并使用 DECLARE_DYNCREATE 和 IMPLEMENT_DYNCREATE 宏声明此类。

　　下面给出了CWinThread类的原型（添加了关于其重要函数功能和是否需要被继承类重载的注释）：

class CWinThread : public CCmdTarget
{
　DECLARE_DYNAMIC(CWinThread)

　public:
　　// Constructors
　　CWinThread();
　　BOOL CreateThread(DWORD dwCreateFlags = 0, UINT nStackSize = 0,
LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);

　　// Attributes
　　CWnd* m_pMainWnd; // main window (usually same AfxGetApp()->m_pMainWnd)
　　CWnd* m_pActiveWnd; // active main window (may not be m_pMainWnd)
　　BOOL m_bAutoDelete; // enables 'delete this' after thread termination

　　// only valid while running
　　HANDLE m_hThread; // this thread's HANDLE
　　operator HANDLE() const;
　　DWORD m_nThreadID; // this thread's ID

　　int GetThreadPriority();
　　BOOL SetThreadPriority(int nPriority);

　　// Operations
　　DWORD SuspendThread();
　　DWORD ResumeThread();
　　BOOL PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam);

　　// Overridables
　　//执行线程实例初始化，必须重写
　　virtual BOOL InitInstance();

　　// running and idle processing
　　//控制线程的函数，包含消息泵，一般不重写
　　virtual int Run();

　　//消息调度到TranslateMessage和DispatchMessage之前对其进行筛选，
　　//通常不重写
　　virtual BOOL PreTranslateMessage(MSG* pMsg);

　　virtual BOOL PumpMessage(); // low level message pump

　　//执行线程特定的闲置时间处理，通常不重写
　　virtual BOOL OnIdle(LONG lCount); // return TRUE if more idle processing
　　virtual BOOL IsIdleMessage(MSG* pMsg); // checks for special messages

　　//线程终止时执行清除，通常需要重写
　　virtual int ExitInstance(); // default will 'delete this'

　　//截获由线程的消息和命令处理程序引发的未处理异常，通常不重写
　　virtual LRESULT ProcessWndProcException(CException* e, const MSG* pMsg);

　　// Advanced: handling messages sent to message filter hook
　　virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);

　　// Advanced: virtual access to m_pMainWnd
　　virtual CWnd* GetMainWnd();

　　// Implementation
　public:
　　virtual ~CWinThread();
　　#ifdef _DEBUG
　　　virtual void AssertValid() const;
　　　virtual void Dump(CDumpContext& dc) const;
　　　int m_nDisablePumpCount; // Diagnostic trap to detect illegal re-entrancy
　　#endif
　　void CommonConstruct();
　　virtual void Delete();
　　// 'delete this' only if m_bAutoDelete == TRUE

　　// message pump for Run
　　MSG m_msgCur; // current message

　public:
　　// constructor used by implementation of AfxBeginThread
　　CWinThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam);

　　// valid after construction
　　LPVOID m_pThreadParams; // generic parameters passed to starting function
　　AFX_THREADPROC m_pfnThreadProc;

　　// set after OLE is initialized
　　void (AFXAPI* m_lpfnOleTermOrFreeLib)(BOOL, BOOL);
　　COleMessageFilter* m_pMessageFilter;

　protected:
　　CPoint m_ptCursorLast; // last mouse position
　　UINT m_nMsgLast; // last mouse message
　　BOOL DispatchThreadMessageEx(MSG* msg); // helper
　　void DispatchThreadMessage(MSG* msg); // obsolete
};

　　启动UI线程的AfxBeginThread函数的原型为：

CWinThread *AfxBeginThread(
　//从CWinThread派生的类的 RUNTIME_CLASS
　CRuntimeClass *pThreadClass, 
　int nPriority = THREAD_PRIORITY_NORMAL, 
　UINT nStackSize = 0, 
　DWORD dwCreateFlags = 0,
　LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL
); 
　　我们可以方便地使用VC++ 6.0类向导定义一个继承自CWinThread的用户线程类。下面给出产生我们自定义的CWinThread子类CMyUIThread的方法。

　　打开VC++ 6.0类向导，在如下窗口中选择Base Class类为CWinThread，输入子类名为CMyUIThread，点击"OK"按钮后就产生了类CMyUIThread。

![](http://dev.yesky.com/imagelist/06/01/575l04oac886.jpg)

　　其源代码框架为：

/////////////////////////////////////////////////////////////////////////////
// CMyUIThread thread

class CMyUIThread : public CWinThread
{
　DECLARE_DYNCREATE(CMyUIThread)
　protected:
　　CMyUIThread(); // protected constructor used by dynamic creation

　　// Attributes
　public:

　　// Operations
　public:

　　// Overrides
　　// ClassWizard generated virtual function overrides
　　//{{AFX_VIRTUAL(CMyUIThread)
　　public:
　　　virtual BOOL InitInstance();
　　　virtual int ExitInstance();
　　//}}AFX_VIRTUAL

　　// Implementation
　protected:
　　virtual ~CMyUIThread();

　　// Generated message map functions
　　//{{AFX_MSG(CMyUIThread)
　　　// NOTE - the ClassWizard will add and remove member functions here.
　　//}}AFX_MSG

　DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMyUIThread

IMPLEMENT_DYNCREATE(CMyUIThread, CWinThread)

CMyUIThread::CMyUIThread()
{}

CMyUIThread::~CMyUIThread()
{}

BOOL CMyUIThread::InitInstance()
{
　// TODO: perform and per-thread initialization here
　return TRUE;
}

int CMyUIThread::ExitInstance()
{
　// TODO: perform any per-thread cleanup here
　return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyUIThread, CWinThread)
//{{AFX_MSG_MAP(CMyUIThread)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
　　使用下列代码就可以启动这个UI线程：

CMyUIThread *pThread;
pThread = (CMyUIThread*)
AfxBeginThread( RUNTIME_CLASS(CMyUIThread) );
　　另外，我们也可以不用AfxBeginThread 创建线程，而是分如下两步完成：

　　（1）调用线程类的构造函数创建一个线程对象；

　　（2）调用CWinThread::CreateThread函数来启动该线程。

　　在线程自身内调用AfxEndThread函数可以终止该线程：

void AfxEndThread(
　UINT nExitCode //the exit code of the thread
);
　　对于UI线程而言，如果消息队列中放入了WM_QUIT消息，将结束线程。

　　关于UI线程和工作者线程的分配，最好的做法是：将所有与UI相关的操作放入主线程，其它的纯粹的运算工作交给独立的数个工作者线程。

　　候捷先生早些时间喜欢为MDI程序的每个窗口创建一个线程，他后来澄清了这个错误。因为如果为MDI程序的每个窗口都单独创建一个线程，在窗口进行切换的时候，将进行线程的上下文切换！

2.线程间通信

　　MFC中定义了继承自CSyncObject类的CCriticalSection 、CCEvent、CMutex、CSemaphore类封装和简化了WIN32 API所提供的临界区、事件、互斥和信号量。使用这些同步机制，必须包含"Afxmt.h"头文件。下图给出了类的继承关系：

![](http://dev.yesky.com/imagelist/06/01/1pzf0afz2ais.jpg)

　　作为CSyncObject类的继承类，我们仅仅使用基类CSyncObject的接口函数就可以方便、统一的操作CCriticalSection 、CCEvent、CMutex、CSemaphore类，下面是CSyncObject类的原型：

class CSyncObject : public CObject
{
　DECLARE_DYNAMIC(CSyncObject)

　// Constructor
　public:
　　CSyncObject(LPCTSTR pstrName);

　　// Attributes
　public:
　　operator HANDLE() const;
　　HANDLE m_hObject;

　　// Operations
　　virtual BOOL Lock(DWORD dwTimeout = INFINITE);
　　virtual BOOL Unlock() = 0;
　　virtual BOOL Unlock(LONG /* lCount */, LPLONG /* lpPrevCount=NULL */)
　　{ return TRUE; }

　　// Implementation
　public:
　　virtual ~CSyncObject();
　　#ifdef _DEBUG
　　　CString m_strName;
　　　virtual void AssertValid() const;
　　　virtual void Dump(CDumpContext& dc) const;
　　#endif
　　friend class CSingleLock;
　　friend class CMultiLock;
};
　　CSyncObject类最主要的两个函数是Lock和Unlock，若我们直接使用CSyncObject类及其派生类，我们需要非常小心地在Lock之后调用Unlock。

　　MFC提供的另两个类CSingleLock（等待一个对象）和CMultiLock（等待多个对象）为我们编写应用程序提供了更灵活的机制，下面以实际来阐述CSingleLock的用法：

class CThreadSafeWnd
{
　public:
　　CThreadSafeWnd(){}
　　~CThreadSafeWnd(){}
　　void SetWindow(CWnd *pwnd)
　　{
　　　m_pCWnd = pwnd;
　　}
　　void PaintBall(COLORREF color, CRect &rc);
　private:
　　CWnd *m_pCWnd;
　　CCriticalSection m_CSect;
};

void CThreadSafeWnd::PaintBall(COLORREF color, CRect &rc)
{
　CSingleLock csl(&m_CSect);
　//缺省的Timeout是INFINITE，只有m_Csect被激活，csl.Lock()才能返回
　//true，这里一直等待
　if (csl.Lock())
;
　{
　　// not necessary
　　//AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
　　CDC *pdc = m_pCWnd->GetDC();
　　CBrush brush(color);
　　CBrush *oldbrush = pdc->SelectObject(&brush);
　　pdc->Ellipse(rc);
　　pdc->SelectObject(oldbrush);
　　GdiFlush(); // don't wait to update the display
　}
}
　　上述实例讲述了用CSingleLock对Windows GDI相关对象进行保护的方法，下面再给出一个其他方面的例子：

int array1[10], array2[10];
CMutexSection section; //创建一个CMutex类的对象

//赋值线程控制函数
UINT EvaluateThread(LPVOID param) 
{
　CSingleLock singlelock; 
　singlelock(§ion);

　//互斥区域
　singlelock.Lock();
　for (int i = 0; i < 10; i++)
　　array1[i] = i;
　singlelock.Unlock();
}
//拷贝线程控制函数
UINT CopyThread(LPVOID param) 
{
　CSingleLock singlelock;
　singlelock(§ion);

　//互斥区域
　singlelock.Lock();
　for (int i = 0; i < 10; i++)
　　array2[i] = array1[i];
　singlelock.Unlock();
}
}

AfxBeginThread(EvaluateThread, NULL); //启动赋值线程
AfxBeginThread(CopyThread, NULL); //启动拷贝线程
　　上面的例子中启动了两个线程EvaluateThread和CopyThread，线程EvaluateThread把10个数赋值给数组array1[]，线程CopyThread将数组array1[]拷贝给数组array2[]。由于数组的拷贝和赋值都是整体行为，如果不以互斥形式执行代码段：

for (int i = 0; i < 10; i++)
array1[i] = i;
　　和

for (int i = 0; i < 10; i++)
array2[i] = array1[i];
　　其结果是很难预料的！

　　除了可使用CCriticalSection、CEvent、CMutex、CSemaphore作为线程间同步通信的方式以外，我们还可以利用PostThreadMessage函数在线程间发送消息：

BOOL PostThreadMessage(DWORD idThread, // thread identifier
UINT Msg, // message to post
WPARAM wParam, // first message parameter
LPARAM lParam // second message parameter
);
3.线程与消息队列

　　在WIN32中，每一个线程都对应着一个消息队列。由于一个线程可以产生数个窗口，所以并不是每个窗口都对应着一个消息队列。下列几句话应该作为"定理"被记住：

　　"定理" 一

　　所有产生给某个窗口的消息，都先由创建这个窗口的线程处理；

　　"定理" 二

　　Windows屏幕上的每一个控件都是一个窗口，有对应的窗口函数。

　　消息的发送通常有两种方式，一是SendMessage，一是PostMessage，其原型分别为：

LRESULT SendMessage(HWND hWnd, // handle of destination window
　UINT Msg, // message to send
　WPARAM wParam, // first message parameter
　LPARAM lParam // second message parameter
);
BOOL PostMessage(HWND hWnd, // handle of destination window
　UINT Msg, // message to post
　WPARAM wParam, // first message parameter
　LPARAM lParam // second message parameter
);
　　两个函数原型中的四个参数的意义相同，但是SendMessage和PostMessage的行为有差异。SendMessage必须等待消息被处理后才返回，而PostMessage仅仅将消息放入消息队列。SendMessage的目标窗口如果属于另一个线程，则会发生线程上下文切换，等待另一线程处理完成消息。为了防止另一线程当掉，导致SendMessage永远不能返回，我们可以调用SendMessageTimeout函数：

LRESULT SendMessageTimeout(
　HWND hWnd, // handle of destination window
　UINT Msg, // message to send
　WPARAM wParam, // first message parameter
　LPARAM lParam, // second message parameter
　UINT fuFlags, // how to send the message
　UINT uTimeout, // time-out duration
　LPDWORD lpdwResult // return value for synchronous call
);
　　4. MFC线程、消息队列与MFC程序的"生死因果"

　　分析MFC程序的主线程启动及消息队列处理的过程将有助于我们进一步理解UI线程与消息队列的关系，为此我们需要简单地叙述一下MFC程序的"生死因果"（侯捷：《深入浅出MFC》）。

　　使用VC++ 6.0的向导完成一个最简单的单文档架构MFC应用程序MFCThread：

　　（1） 输入MFC EXE工程名MFCThread；

　　（2） 选择单文档架构，不支持Document/View结构；

　　（3） ActiveX、3D container等其他选项都选择无。

　　我们来分析这个工程。下面是产生的核心源代码：

　　MFCThread.h 文件

class CMFCThreadApp : public CWinApp
{
　public:
　　CMFCThreadApp();

　　// Overrides
　　// ClassWizard generated virtual function overrides
　　//{{AFX_VIRTUAL(CMFCThreadApp)
　　　public:
　　　　virtual BOOL InitInstance();
　　//}}AFX_VIRTUAL

　　// Implementation

　public:
　　//{{AFX_MSG(CMFCThreadApp)
　　　afx_msg void OnAppAbout();
　　　// NOTE - the ClassWizard will add and remove member functions here.
　　　// DO NOT EDIT what you see in these blocks of generated code !
　　//}}AFX_MSG
　DECLARE_MESSAGE_MAP()
};
　　MFCThread.cpp文件

CMFCThreadApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMFCThreadApp initialization

BOOL CMFCThreadApp::InitInstance()
{
　…
　CMainFrame* pFrame = new CMainFrame;
　m_pMainWnd = pFrame;

　// create and load the frame with its resources
　pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,NULL);
　// The one and only window has been initialized, so show and update it.
　pFrame->ShowWindow(SW_SHOW);
　pFrame->UpdateWindow();

　return TRUE;
}
　　MainFrm.h文件

#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
　public:
　　CMainFrame();
　protected: 
　　DECLARE_DYNAMIC(CMainFrame)

　　// Attributes
　public:

　　// Operations
　public:
　　// Overrides
　　// ClassWizard generated virtual function overrides
　　//{{AFX_VIRTUAL(CMainFrame)
　　　virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
　　　virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
　　//}}AFX_VIRTUAL

　　// Implementation
　public:
　　virtual ~CMainFrame();
　　#ifdef _DEBUG
　　　virtual void AssertValid() const;
　　　virtual void Dump(CDumpContext& dc) const;
　　#endif
　　CChildView m_wndView;

　　// Generated message map functions
　protected:
　//{{AFX_MSG(CMainFrame)
　　afx_msg void OnSetFocus(CWnd *pOldWnd);
　　// NOTE - the ClassWizard will add and remove member functions here.
　　// DO NOT EDIT what you see in these blocks of generated code!
　//}}AFX_MSG
　DECLARE_MESSAGE_MAP()
};
　　MainFrm.cpp文件

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
　//{{AFX_MSG_MAP(CMainFrame)
　　// NOTE - the ClassWizard will add and remove mapping macros here.
　　// DO NOT EDIT what you see in these blocks of generated code !
　　ON_WM_SETFOCUS()
　//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
　// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
　if( !CFrameWnd::PreCreateWindow(cs) )
　　return FALSE;
　　// TODO: Modify the Window class or styles here by modifying
　　// the CREATESTRUCT cs

　cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
　cs.lpszClass = AfxRegisterWndClass(0);
　return TRUE;
}
　　ChildView.h文件

// CChildView window

class CChildView : public CWnd
{
　// Construction
　public:
　　CChildView();

　　// Attributes
　public:
　　// Operations
　public:
　　// Overrides
　　// ClassWizard generated virtual function overrides
　　//{{AFX_VIRTUAL(CChildView)
　　　protected:
　　　　virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
　　//}}AFX_VIRTUAL

　　// Implementation
　public:
　　virtual ~CChildView();

　　// Generated message map functions
　protected:
　　//{{AFX_MSG(CChildView)
　　　afx_msg void OnPaint();
　　//}}AFX_MSG
　DECLARE_MESSAGE_MAP()
};

ChildView.cpp文件
// CChildView

CChildView::CChildView()
{}

CChildView::~CChildView()
{}

BEGIN_MESSAGE_MAP(CChildView,CWnd )
//{{AFX_MSG_MAP(CChildView)
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
　if (!CWnd::PreCreateWindow(cs))
　　return FALSE;

　cs.dwExStyle |= WS_EX_CLIENTEDGE;
　cs.style &= ~WS_BORDER;
　cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,::LoadCursor(NULL, IDC_ARROW),
HBRUSH(COLOR_WINDOW+1),NULL);

　return TRUE;
}

void CChildView::OnPaint() 
{
　CPaintDC dc(this); // device context for painting

　// TODO: Add your message handler code here
　// Do not call CWnd::OnPaint() for painting messages
}
　　文件MFCThread.h和MFCThread.cpp定义和实现的类CMFCThreadApp继承自CWinApp类，而CWinApp类又继承自CWinThread类（CWinThread类又继承自CCmdTarget类），所以CMFCThread本质上是一个MFC线程类，下图给出了相关的类层次结构：

![](http://dev.yesky.com/imagelist/06/01/g705u0arabo5.jpg)

　我们提取CWinApp类原型的一部分：

class CWinApp : public CWinThread
{
　DECLARE_DYNAMIC(CWinApp)
　public:
　　// Constructor
　　CWinApp(LPCTSTR lpszAppName = NULL);// default app name
　　// Attributes
　　// Startup args (do not change)
　　HINSTANCE m_hInstance;
　　HINSTANCE m_hPrevInstance;
　　LPTSTR m_lpCmdLine;
　　int m_nCmdShow;
　　// Running args (can be changed in InitInstance)
　　LPCTSTR m_pszAppName; // human readable name
　　LPCTSTR m_pszExeName; // executable name (no spaces)
　　LPCTSTR m_pszHelpFilePath; // default based on module path
　　LPCTSTR m_pszProfileName; // default based on app name

　　// Overridables
　　virtual BOOL InitApplication();
　　virtual BOOL InitInstance();
　　virtual int ExitInstance(); // return app exit code
　　virtual int Run();
　　virtual BOOL OnIdle(LONG lCount); // return TRUE if more idle processing
　　virtual LRESULT ProcessWndProcException(CException* e,const MSG* pMsg);

　public:
　　virtual ~CWinApp();
　protected:
　　DECLARE_MESSAGE_MAP()
};
　　SDK程序的WinMain 所完成的工作现在由CWinApp 的三个函数完成：

virtual BOOL InitApplication();
virtual BOOL InitInstance();
virtual int Run();
　　"CMFCThreadApp theApp;"语句定义的全局变量theApp是整个程式的application object，每一个MFC 应用程序都有一个。当我们执行MFCThread程序的时候，这个全局变量被构造。theApp 配置完成后，WinMain开始执行。但是程序中并没有WinMain的代码，它在哪里呢？原来MFC早已准备好并由Linker直接加到应用程序代码中的，其原型为（存在于VC++6.0安装目录下提供的APPMODUL.CPP文件中）：

extern "C" int WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
LPTSTR lpCmdLine, int nCmdShow)
{
　// call shared/exported WinMain
　return AfxWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
　　其中调用的AfxWinMain如下（存在于VC++6.0安装目录下提供的WINMAIN.CPP文件中）：

int AFXAPI AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
LPTSTR lpCmdLine, int nCmdShow)
{
　ASSERT(hPrevInstance == NULL);

　int nReturnCode = -1;
　CWinThread* pThread = AfxGetThread();
　CWinApp* pApp = AfxGetApp();

　// AFX internal initialization
　if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
　　goto InitFailure;

　// App global initializations (rare)
　if (pApp != NULL && !pApp->InitApplication())
　　goto InitFailure;

　// Perform specific initializations
　if (!pThread->InitInstance())
　{
　　if (pThread->m_pMainWnd != NULL)
　　{
　　　TRACE0("Warning: Destroying non-NULL m_pMainWnd/n");
　　　pThread->m_pMainWnd->DestroyWindow();
　　}
　　nReturnCode = pThread->ExitInstance();
　　goto InitFailure;
　}
　nReturnCode = pThread->Run();

　InitFailure:
　#ifdef _DEBUG
　　// Check for missing AfxLockTempMap calls
　　if (AfxGetModuleThreadState()->m_nTempMapLock != 0)
　　{
　　　TRACE1("Warning: Temp map lock count non-zero (%ld)./n",
AfxGetModuleThreadState()->m_nTempMapLock);
　　}
　　AfxLockTempMaps();
　　AfxUnlockTempMaps(-1);
　#endif

　AfxWinTerm();
　return nReturnCode;
}
　　我们提取主干，实际上，这个函数做的事情主要是：

CWinThread* pThread = AfxGetThread();
CWinApp* pApp = AfxGetApp();
AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
pApp->InitApplication()
pThread->InitInstance()
pThread->Run();
　　其中，InitApplication 是注册窗口类别的场所；InitInstance是产生窗口并显示窗口的场所；Run是提取并分派消息的场所。这样，MFC就同WIN32 SDK程序对应起来了。CWinThread::Run是程序生命的"活水源头"（侯捷：《深入浅出MFC》，函数存在于VC++ 6.0安装目录下提供的THRDCORE.CPP文件中）：

// main running routine until thread exits
int CWinThread::Run()
{
　ASSERT_VALID(this);

　// for tracking the idle time state
　BOOL bIdle = TRUE;
　LONG lIdleCount = 0;

　// acquire and dispatch messages until a WM_QUIT message is received.
　for (;;)
　{
　　// phase1: check to see if we can do idle work
　　while (bIdle && !::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
　　{
　　　// call OnIdle while in bIdle state
　　　if (!OnIdle(lIdleCount++))
　　　　bIdle = FALSE; // assume "no idle" state
　　}

　　// phase2: pump messages while available
　　do
　　{
　　　// pump message, but quit on WM_QUIT
　　　if (!PumpMessage())
　　　　return ExitInstance();

　　　// reset "no idle" state after pumping "normal" message
　　　if (IsIdleMessage(&m_msgCur))
　　　{
　　　　bIdle = TRUE;
　　　　lIdleCount = 0;
　　　}

　　} while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE));
　}
　ASSERT(FALSE); // not reachable
}
　　其中的PumpMessage函数又对应于：

/////////////////////////////////////////////////////////////////////////////
// CWinThread implementation helpers

BOOL CWinThread::PumpMessage()
{
　ASSERT_VALID(this);

　if (!::GetMessage(&m_msgCur, NULL, NULL, NULL))
　{
　　return FALSE;
　}

　// process this message
　if(m_msgCur.message != WM_KICKIDLE && !PreTranslateMessage(&m_msgCur))
　{
　　::TranslateMessage(&m_msgCur);
　　::DispatchMessage(&m_msgCur);
　}
　return TRUE;
}
　　因此，忽略IDLE状态，整个RUN的执行提取主干就是：

do {
　::GetMessage(&msg,...);
　PreTranslateMessage{&msg);
　::TranslateMessage(&msg);
　::DispatchMessage(&msg);
　...
} while (::PeekMessage(...));
　　由此，我们建立了MFC消息获取和派生机制与WIN32 SDK程序之间的对应关系。下面继续分析MFC消息的"绕行"过程。

　　在MFC中，只要是CWnd 衍生类别，就可以拦下任何Windows消息。与窗口无关的MFC类别（例如CDocument 和CWinApp）如果也想处理消息，必须衍生自CCmdTarget，并且只可能收到WM_COMMAND消息。所有能进行MESSAGE_MAP的类都继承自CCmdTarget，如：

![](http://dev.yesky.com/imagelist/06/01/4bidfnchk995.jpg)![](http://dev.yesky.com/imagelist/06/01/eqq6knx8w7z0.jpg)![](http://dev.yesky.com/imagelist/06/01/m7pbiwap6un1.jpg)

　　MFC中MESSAGE_MAP的定义依赖于以下三个宏：

DECLARE_MESSAGE_MAP()

BEGIN_MESSAGE_MAP( 
　theClass, //Specifies the name of the class whose message map this is
　baseClass //Specifies the name of the base class of theClass
)

END_MESSAGE_MAP()
　　我们程序中涉及到的有：MFCThread.h、MainFrm.h、ChildView.h文件

DECLARE_MESSAGE_MAP()
MFCThread.cpp文件
BEGIN_MESSAGE_MAP(CMFCThreadApp, CWinApp)
//{{AFX_MSG_MAP(CMFCThreadApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
// NOTE - the ClassWizard will add and remove mapping macros here.
// DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
MainFrm.cpp文件
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
// NOTE - the ClassWizard will add and remove mapping macros here.
// DO NOT EDIT what you see in these blocks of generated code !
ON_WM_SETFOCUS()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
ChildView.cpp文件
BEGIN_MESSAGE_MAP(CChildView,CWnd )
//{{AFX_MSG_MAP(CChildView)
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
　　由这些宏，MFC建立了一个消息映射表（消息流动网），按照消息流动网匹配对应的消息处理函数，完成整个消息的"绕行"。

　　看到这里相信你有这样的疑问：程序定义了CWinApp类的theApp全局变量，可是从来没有调用AfxBeginThread或theApp.CreateThread启动线程呀，theApp对应的线程是怎么启动的？

　　答：MFC在这里用了很高明的一招。实际上，程序开始运行，第一个线程是由操作系统（OS）启动的，在CWinApp的构造函数里，MFC将theApp"对应"向了这个线程，具体的实现是这样的：

CWinApp::CWinApp(LPCTSTR lpszAppName)
{
　if (lpszAppName != NULL)
　　m_pszAppName = _tcsdup(lpszAppName);
　else
　　m_pszAppName = NULL;

　// initialize CWinThread state
　AFX_MODULE_STATE *pModuleState = _AFX_CMDTARGET_GETSTATE();
　AFX_MODULE_THREAD_STATE *pThreadState = pModuleState->m_thread;
　ASSERT(AfxGetThread() == NULL);
　pThreadState->m_pCurrentWinThread = this;
　ASSERT(AfxGetThread() == this);
　m_hThread = ::GetCurrentThread();
　m_nThreadID = ::GetCurrentThreadId();

　// initialize CWinApp state
　ASSERT(afxCurrentWinApp == NULL); // only one CWinApp object please
　pModuleState->m_pCurrentWinApp = this;
　ASSERT(AfxGetApp() == this);

　// in non-running state until WinMain
　m_hInstance = NULL;
　m_pszHelpFilePath = NULL;
　m_pszProfileName = NULL;
　m_pszRegistryKey = NULL;
　m_pszExeName = NULL;
　m_pRecentFileList = NULL;
　m_pDocManager = NULL;
　m_atomApp = m_atomSystemTopic = NULL; //微软懒鬼？或者他认为 
　//这样连等含义更明确？
　m_lpCmdLine = NULL;
　m_pCmdInfo = NULL;

　// initialize wait cursor state
　m_nWaitCursorCount = 0;
　m_hcurWaitCursorRestore = NULL;

　// initialize current printer state
　m_hDevMode = NULL;
　m_hDevNames = NULL;
　m_nNumPreviewPages = 0; // not specified (defaults to 1)

　// initialize DAO state
　m_lpfnDaoTerm = NULL; // will be set if AfxDaoInit called

　// other initialization
　m_bHelpMode = FALSE;
　m_nSafetyPoolSize = 512; // default size
}
　　很显然，theApp成员变量都被赋予OS启动的这个当前线程相关的值，如代码：

m_hThread = ::GetCurrentThread();//theApp的线程句柄等于当前线程句柄 
m_nThreadID = ::GetCurrentThreadId();//theApp的线程ID等于当前线程ID
　　所以CWinApp类几乎只是为MFC程序的第一个线程量身定制的，它不需要也不能被AfxBeginThread或theApp.CreateThread"再次"启动。这就是CWinApp类和theApp全局变量的内涵！如果你要再增加一个UI线程，不要继承类CWinApp，而应继承类CWinThread。而参考第1节，由于我们一般以主线程（在MFC程序里实际上就是OS启动的第一个线程）处理所有窗口的消息，所以我们几乎没有再启动UI线程的需求！ 

共4页。 [9](http://dev.yesky.com/390/2280390.shtml)[7](http://dev.yesky.com/390/2280390_2.shtml)[**1**](http://dev.yesky.com/390/2280390.shtml)[**2**](http://dev.yesky.com/390/2280390_1.shtml)[**3**](http://dev.yesky.com/390/2280390_2.shtml)[**4**](http://dev.yesky.com/390/2280390_3.shtml)


