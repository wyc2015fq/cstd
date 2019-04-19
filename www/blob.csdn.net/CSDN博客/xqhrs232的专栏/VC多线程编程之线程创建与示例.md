# VC多线程编程之线程创建与示例 - xqhrs232的专栏 - CSDN博客
2013年01月19日 10:28:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：683
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://haobinnan.blog.51cto.com/775253/658446](http://haobinnan.blog.51cto.com/775253/658446)
一、问题的提出
编写一个耗时的单线程程序：
　　新建一个基于对话框的应用程序SingleThread，在主对话框IDD_SINGLETHREAD_DIALOG添加一个按钮，ID为IDC_SLEEP_SIX_SECOND，标题为“延时6秒”，添加按钮的响应函数，代码如下：
void CSingleThreadDlg::OnSleepSixSecond() 
{
    Sleep(6000); //延时6秒
}
　　编译并运行应用程序，单击“延时6秒”按钮，你就会发现在这6秒期间程序就象“死机”一样，不在响应其它消息。为了更好地处理这种耗时的操作，我们有必要学习——多线程编程。
二、多线程概述
　　进程和线程都是操作系统的概念。进程是应用程序的执行实例，每个进程是由私有的虚拟地址空间、代码、数据和其它各种系统资源组成，进程在运行过程中创建的资源随着进程的终止而被销毁，所使用的系统资源在进程终止时被释放或关闭。
　　线程是进程内部的一个执行单元。系统创建好进程后，实际上就启动执行了该进程的主执行线程，主执行线程以函数地址形式，比如说main或WinMain函数，将程序的启动点提供给Windows系统。主执行线程终止了，进程也就随之终止。
　　每一个进程至少有一个主执行线程，它无需由用户去主动创建，是由系统自动创建的。用户根据需要在应用程序中创建其它线程，多个线程并发地运行于同一个进程中。一个进程中的所有线程都在该进程的虚拟地址空间中，共同使用这些虚拟地址空间、全局变量和系统资源，所以线程间的通讯非常方便，多线程技术的应用也较为广泛。
　　多线程可以实现并行处理，避免了某项任务长时间占用CPU时间。要说明的一点是，目前大多数的计算机都是单处理器（CPU）的，为了运行所有这些线程，操作系统为每个独立线程安排一些CPU时间，操作系统以轮换方式向线程提供时间片，这就给人一种假象，好象这些线程都在同时运行。由此可见，如果两个非常活跃的线程为了抢夺对CPU的控制权，在线程切换时会消耗很多的CPU资源，反而会降低系统的性能。这一点在多线程编程时应该注意。
　　Win32 SDK函数支持进行多线程的程序设计，并提供了操作系统原理中的各种同步、互斥和临界区等操作。Visual C++ 6.0中，使用MFC类库也实现了多线程的程序设计，使得多线程编程更加方便。
三、Win32 API对多线程编程的支持
　　Win32 提供了一系列的API函数来完成线程的创建、挂起、恢复、终结以及通信等工作。下面将选取其中的一些重要函数进行说明。
1. HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes,
                  DWORD dwStackSize,
                  LPTHREAD_START_ROUTINE lpStartAddress,
                  LPVOID lpParameter,
                  DWORD dwCreationFlags,
                  LPDWORD lpThreadId);
     该函数在其调用进程的进程空间里创建一个新的线程，并返回已建线程的句柄，其中各参数说明如下：
lpThreadAttributes：指向一个 SECURITY_ATTRIBUTES 结构的指针，该结构决定了线程的安全属性，一般置为 NULL； 
dwStackSize：指定了线程的堆栈深度，一般都设置为0； 
lpStartAddress：表示新线程开始执行时代码所在函数的地址，即线程的起始地址。一般情况为(LPTHREAD_START_ROUTINE)ThreadFunc，ThreadFunc 是线程函数名； 
lpParameter：指定了线程执行时传送给线程的32位参数，即线程函数的参数； 
dwCreationFlags：控制线程创建的附加标志，可以取两种值。如果该参数为0，线程在被创建后就会立即开始执行；如果该参数为CREATE_SUSPENDED,则系统产生线程后，该线程处于挂起状态，并不马上执行，直至函数ResumeThread被调用； 
lpThreadId：该参数返回所创建线程的ID； 
如果创建成功则返回线程的句柄，否则返回NULL。
2. DWORD SuspendThread(HANDLE hThread);
该函数用于挂起指定的线程，如果函数执行成功，则线程的执行被挂起。 
3. DWORD ResumeThread(HANDLE hThread);
该函数用于结束线程的挂起状态，执行线程。
4. VOID ExitThread(DWORD dwExitCode);
该函数用于线程终结自身的执行，主要在线程的执行函数中被调用。其中参数dwExitCode用来设置线程的退出码。
5. BOOL TerminateThread(HANDLE hThread,DWORD dwExitCode);
一般情况下，线程运行结束之后，线程函数正常返回，但是应用程序可以调用TerminateThread强行终止某一线程的执行。各参数含义如下：
      hThread：将被终结的线程的句柄； 
      dwExitCode：用于指定线程的退出码。 
　　使用TerminateThread()终止某个线程的执行是不安全的，可能会引起系统不稳定；虽然该函数立即终止线程的执行，但并不释放线程所占用的资源。因此，一般不建议使用该函数。
6. BOOL PostThreadMessage(DWORD idThread,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam);
该函数将一条消息放入到指定线程的消息队列中，并且不等到消息被该线程处理时便返回。
idThread：将接收消息的线程的ID； 
Msg：指定用来发送的消息； 
wParam：同消息有关的字参数； 
lParam：同消息有关的长参数； 
调用该函数时，如果即将接收消息的线程没有创建消息循环，则该函数执行失败。
四、Win32 API多线程编程例程
例程1 MultiThread1
建立一个基于对话框的工程MultiThread1，在对话框IDD_MULTITHREAD1_DIALOG中加入两个按钮和一个编辑框，两个按钮的ID分别是IDC_START，IDC_STOP ，标题分别为“启动”，“停止”，IDC_STOP的属性选中Disabled；编辑框的ID为IDC_TIME ，属性选中Read-only；
在MultiThread1Dlg.h文件中添加线程函数声明： void ThreadFunc();
注意，线程函数的声明应在类CMultiThread1Dlg的外部。 在类CMultiThread1Dlg内部添加protected型变量：   HANDLE hThread;
DWORD ThreadID;
分别代表线程的句柄和ID。 
在MultiThread1Dlg.cpp文件中添加全局变量m_bRun ： volatile BOOL m_bRun;
m_bRun 代表线程是否正在运行。
你要留意到全局变量 m_bRun 是使用 volatile 修饰符的，volatile 修饰符的作用是告诉编译器无需对该变量作任何的优化，即无需将它放到一个寄存器中，并且该值可被外部改变。对于多线程引用的全局变量来说，volatile 是一个非常重要的修饰符。
编写线程函数： void ThreadFunc()
{
CTime time;
CString strTime;
m_bRun=TRUE;
while(m_bRun)
{
   time=CTime::GetCurrentTime();
   strTime=time.Format("%H:%M:%S");
   ::SetDlgItemText(AfxGetMainWnd()->m_hWnd,IDC_TIME,strTime);
   Sleep(1000);
}
}
该线程函数没有参数，也不返回函数值。只要m_bRun为TRUE，线程一直运行。
双击IDC_START按钮，完成该按钮的消息函数：
void CMultiThread1Dlg::OnStart() 
{
// TODO: Add your control notification handler code here
hThread=CreateThread(NULL,
   0,
   (LPTHREAD_START_ROUTINE)ThreadFunc,
   NULL,
   0,
   &ThreadID);
GetDlgItem(IDC_START)->EnableWindow(FALSE);
GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
}
双击IDC_STOP按钮，完成该按钮的消息函数： void CMultiThread1Dlg::OnStop() 
{
// TODO: Add your control notification handler code here
m_bRun=FALSE;
GetDlgItem(IDC_START)->EnableWindow(TRUE);
GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
}
编译并运行该例程，体会使用Win32 API编写的多线程。
例程2 MultiThread2
　　该线程演示了如何传送一个一个整型的参数到一个线程中，以及如何等待一个线程完成处理。
      建立一个基于对话框的工程MultiThread2，在对话框IDD_MULTITHREAD2_DIALOG中加入一个编辑框和一个按钮，ID分别是IDC_COUNT，IDC_START ，按钮控件的标题为“开始”； 
在MultiThread2Dlg.h文件中添加线程函数声明： void ThreadFunc(int integer);
注意，线程函数的声明应在类CMultiThread2Dlg的外部。
在类CMultiThread2Dlg内部添加protected型变量:   HANDLE hThread;
DWORD ThreadID;
分别代表线程的句柄和ID。
打开ClassWizard，为编辑框IDC_COUNT添加int型变量m_nCount。在MultiThread2Dlg.cpp文件中添加：void ThreadFunc(int integer)
{
int i;
for(i=0;i<integer;i++)
{
   Beep(200,50);
   Sleep(1000);
}
}
双击IDC_START按钮，完成该按钮的消息函数： void CMultiThread2Dlg::OnStart() 
{
UpdateData(TRUE);
int integer=m_nCount;
hThread=CreateThread(NULL,
   0,
   (LPTHREAD_START_ROUTINE)ThreadFunc,
   (VOID*)integer,
   0,
   &ThreadID);
GetDlgItem(IDC_START)->EnableWindow(FALSE);
WaitForSingleObject(hThread,INFINITE);
GetDlgItem(IDC_START)->EnableWindow(TRUE);
}
顺便说一下WaitForSingleObject函数，其函数原型为：DWORD WaitForSingleObject(HANDLE hHandle,DWORD dwMilliseconds);
hHandle为要监视的对象（一般为同步对象，也可以是线程）的句柄； 
dwMilliseconds为hHandle对象所设置的超时值，单位为毫秒； 
　　当在某一线程中调用该函数时，线程暂时挂起，系统监视hHandle所指向的对象的状态。如果在挂起的dwMilliseconds毫秒内，线程所等待的对象变为有信号状态，则该函数立即返回；如果超时时间已经到达dwMilliseconds毫秒，但hHandle所指向的对象还没有变成有信号状态，函数照样返回。参数dwMilliseconds有两个具有特殊意义的值：0和INFINITE。若为0，则该函数立即返回；若为INFINITE，则线程一直被挂起，直到hHandle所指向的对象变为有信号状态时为止。
　　本例程调用该函数的作用是按下IDC_START按钮后，一直等到线程返回，再恢复IDC_START按钮正常状态。编译运行该例程并细心体会。
例程3 MultiThread3 
传送一个结构体给一个线程函数也是可能的，可以通过传送一个指向结构体的指针参数来完成。先定义一个结构体：
typedef struct
{
int firstArgu,
long secondArgu,
…
}myType,*pMyType;
创建线程时CreateThread(NULL,0,threadFunc,pMyType,…);
在threadFunc函数内部，可以使用“强制转换”：
int intValue=((pMyType)lpvoid)->firstArgu;
long longValue=((pMyType)lpvoid)->seconddArgu;
……
例程3 MultiThread3将演示如何传送一个指向结构体的指针参数。
建立一个基于对话框的工程MultiThread3，在对话框IDD_MULTITHREAD3_DIALOG中加入一个编辑框IDC_MILLISECOND，一个按钮IDC_START，标题为“开始” ，一个进度条IDC_PROGRESS1； 
打开ClassWizard，为编辑框IDC_MILLISECOND添加int型变量m_nMilliSecond，为进度条IDC_PROGRESS1添加CProgressCtrl型变量m_ctrlProgress； 
在MultiThread3Dlg.h文件中添加一个结构的定义： 
struct threadInfo
{
UINT nMilliSecond;
CProgressCtrl* pctrlProgress;
};
线程函数的声明： UINT ThreadFunc(LPVOID lpParam);
注意，二者应在类CMultiThread3Dlg的外部。
在类CMultiThread3Dlg内部添加protected型变量: HANDLE hThread;
DWORD ThreadID;
分别代表线程的句柄和ID。 
在MultiThread3Dlg.cpp文件中进行如下操作：
定义公共变量 threadInfo Info；
双击按钮IDC_START，添加相应消息处理函数：
void CMultiThread3Dlg::OnStart() 
{
// TODO: Add your control notification handler code here
UpdateData(TRUE);
Info.nMilliSecond=m_nMilliSecond;
Info.pctrlProgress=&m_ctrlProgress;
hThread=CreateThread(NULL,
   0,
   (LPTHREAD_START_ROUTINE)ThreadFunc,
   &Info,
   0,
   &ThreadID);
/*
GetDlgItem(IDC_START)->EnableWindow(FALSE);
WaitForSingleObject(hThread,INFINITE);
GetDlgItem(IDC_START)->EnableWindow(TRUE);
*/
}
在函数BOOL CMultiThread3Dlg::OnInitDialog()中添加语句： {
……
// TODO: Add extra initialization here
m_ctrlProgress.SetRange(0,99);
m_nMilliSecond=10;
UpdateData(FALSE);
return TRUE;   // return TRUE   unless you set the focus to a control
}
添加线程处理函数：
UINT ThreadFunc(LPVOID lpParam) {
threadInfo* pInfo=(threadInfo*)lpParam;
for(int i=0;i<100;i++)
{
   int nTemp=pInfo->nMilliSecond;
   pInfo->pctrlProgress->SetPos(i);
   Sleep(nTemp);
}
return 0;
}
　　顺便补充一点，如果你在void CMultiThread3Dlg::OnStart() 函数中添加/* */语句，编译运行你就会发现进度条不进行刷新，主线程也停止了反应。什么原因呢？这是因为WaitForSingleObject函数等待子线程（ThreadFunc）结束时，导致了线程死锁。因为WaitForSingleObject函数会将主线程挂起（任何消息都得不到处理），而子线程ThreadFunc正在设置进度条，一直在等待主线程将刷新消息处理完毕返回才会检测通知事件。这样两个线程都在互相等待，死锁发生了，编程时应注意避免。 
例程4 MultiThread4
该例程测试在Windows下最多可创建线程的数目。
建立一个基于对话框的工程MultiThread4，在对话框IDD_MULTITHREAD4_DIALOG中加入一个按钮IDC_TEST和一个编辑框IDC_COUNT，按钮标题为“测试” ， 编辑框属性选中Read-only； 
在MultiThread4Dlg.cpp文件中进行如下操作：
添加公共变量volatile BOOL m_bRunFlag=TRUE; 
该变量表示是否还能继续创建线程。
添加线程函数：
DWORD WINAPI threadFunc(LPVOID threadNum)
{
while(m_bRunFlag)
{
   Sleep(3000);
}
return 0;
}
只要 m_bRunFlag 变量为TRUE，线程一直运行。
双击按钮IDC_TEST，添加其响应消息函数：
void CMultiThread4Dlg::OnTest() 
{
DWORD threadID;
GetDlgItem(IDC_TEST)->EnableWindow(FALSE);
long nCount=0;
while(m_bRunFlag)
{
   if(CreateThread(NULL,0,threadFunc,NULL,0,&threadID)==NULL)
   {
    m_bRunFlag=FALSE;
    break;
   }
   else
   {
    nCount++;
   }
}
    //不断创建线程，直到再不能创建为止
m_nCount=nCount;
UpdateData(FALSE);
Sleep(5000);
    //延时5秒，等待所有创建的线程结束
GetDlgItem(IDC_TEST)->EnableWindow(TRUE);
     m_bRunFlag=TRUE;
}
五、MFC对多线程编程的支持
　　MFC中有两类线程，分别称之为工作者线程和用户界面线程。二者的主要区别在于工作者线程没有消息循环，而用户界面线程有自己的消息队列和消息循环。
      工作者线程没有消息机制，通常用来执行后台计算和维护任务，如冗长的计算过程，打印机的后台打印等。用户界面线程一般用于处理独立于其他线程执行之外的 用户输入，响应用户及系统所产生的事件和消息等。但对于Win32的API编程而言，这两种线程是没有区别的，它们都只需线程的启动地址即可启动线程来执 行任务。
      在MFC中，一般用全局函数AfxBeginThread()来创建并初始化一个线程的运行，该函数有两种重载形式，分别用于创建工作者线程和用户界面线程。两种重载函数原型和参数分别说明如下：
(1) CWinThread* AfxBeginThread(AFX_THREADPROC pfnThreadProc,
                       LPVOID pParam,
                       nPriority=THREAD_PRIORITY_NORMAL,
                       UINT nStackSize=0,
                       DWORD dwCreateFlags=0,
                       LPSECURITY_ATTRIBUTES lpSecurityAttrs=NULL);
PfnThreadProc:指向工作者线程的执行函数的指针，线程函数原型必须声明如下： UINT ExecutingFunction(LPVOID pParam);
请注意，ExecutingFunction()应返回一个UINT类型的值，用以指明该函数结束的原因。一般情况下，返回0表明执行成功。 
pParam：传递给线程函数的一个32位参数，执行函数将用某种方式解释该值。它可以是数值，或是指向一个结构的指针，甚至可以被忽略； 
nPriority：线程的优先级。如果为0，则线程与其父线程具有相同的优先级； 
nStackSize:线程为自己分配堆栈的大小，其单位为字节。如果nStackSize被设为0，则线程的堆栈被设置成与父线程堆栈相同大小； 
dwCreateFlags：如果为0，则线程在创建后立刻开始执行。如果为CREATE_SUSPEND，则线程在创建后立刻被挂起； 
lpSecurityAttrs：线程的安全属性指针，一般为NULL； 
(2) CWinThread* AfxBeginThread(CRuntimeClass* pThreadClass,
                       int nPriority=THREAD_PRIORITY_NORMAL,
                       UINT nStackSize=0,
                       DWORD dwCreateFlags=0,
                       LPSECURITY_ATTRIBUTES lpSecurityAttrs=NULL);
pThreadClass 是指向 CWinThread 的一个导出类的运行时类对象的指针，该导出类定义了被创建的用户界面线程的启动、退出等；其它参数的意义同形式1。使用函数的这个原型生成的线程也有消息 机制，在以后的例子中我们将发现同主线程的机制几乎一样。
下面我们对CWinThread类的数据成员及常用函数进行简要说明。
m_hThread：当前线程的句柄； 
m_nThreadID:当前线程的ID； 
m_pMainWnd：指向应用程序主窗口的指针 
BOOL CWinThread::CreateThread(DWORD dwCreateFlags=0,
UINT nStackSize=0,
LPSECURITY_ATTRIBUTES lpSecurityAttrs=NULL);
　　该函数中的dwCreateFlags、nStackSize、lpSecurityAttrs参数和API函数CreateThread中的对应参数有相同含义，该函数执行成功，返回非0值，否则返回0。
      一般情况下，调用AfxBeginThread()来一次性地创建并启动一个线程，但是也可以通过两步法来创建线程：首先创建CWinThread类的一个对象，然后调用该对象的成员函数CreateThread()来启动该线程。
virtual BOOL CWinThread::InitInstance();
　　重载该函数以控制用户界面线程实例的初始化。初始化成功则返回非0值，否则返回0。用户界面线程经常重载该函数，工作者线程一般不使用InitInstance()。 virtual int CWinThread::ExitInstance();
　　在线程终结前重载该函数进行一些必要的清理工作。该函数返回线程的退出码，0表示执行成功，非0值用来标识各种错误。同InitInstance()成员函数一样，该函数也只适用于用户界面线程。 
六、MFC多线程编程实例
　 　在Visual C++ 6.0编程环境中，我们既可以编写C风格的32位Win32应用程序，也可以利用MFC类库编写C++风格的应用程序，二者各有其优缺点。基于Win32 的应用程序执行代码小巧，运行效率高，但要求程序员编写的代码较多，且需要管理系统提供给程序的所有资源；而基于MFC类库的应用程序可以快速建立起应用 程序，类库为程序员提供了大量的封装类，而且Developer Studio为程序员提供了一些工具来管理用户源程序，其缺点是类库代码很庞大。由于使用类库所带来的快速、简捷和功能强大等优越性，因此除非有特殊的需要，否则Visual
 C++推荐使用MFC类库进行程序开发。
我们知道，MFC中的线程分为两种：用户界面线程和工作者线程。我们将分别举例说明。
用 MFC 类库编程实现工作者线程
例程5 MultiThread5
为了与Win32 API对照，我们使用MFC 类库编程实现例程3 MultiThread3。
建立一个基于对话框的工程MultiThread5，在对话框IDD_MULTITHREAD5_DIALOG中加入一个编辑框IDC_MILLISECOND，一个按钮IDC_START，标题为“开始” ，一个进度条IDC_PROGRESS1； 
打开ClassWizard，为编辑框IDC_MILLISECOND添加int型变量m_nMilliSecond，为进度条IDC_PROGRESS1添加CProgressCtrl型变量m_ctrlProgress； 
在MultiThread5Dlg.h文件中添加一个结构的定义： struct threadInfo
{
UINT nMilliSecond;
CProgressCtrl* pctrlProgress;
};
线程函数的声明：UINT ThreadFunc(LPVOID lpParam); 
注意，二者应在类CMultiThread5Dlg的外部。
在类CMultiThread5Dlg内部添加protected型变量：
CWinThread* pThread; 
在MultiThread5Dlg.cpp文件中进行如下操作：定义公共变量：threadInfo Info; 
双击按钮IDC_START，添加相应消息处理函数：
void CMultiThread5Dlg::OnStart() 
{
// TODO: Add your control notification handler code here
UpdateData(TRUE);
Info.nMilliSecond=m_nMilliSecond;
Info.pctrlProgress=&m_ctrlProgress;
pThread=AfxBeginThread(ThreadFunc, &Info);
}
在函数BOOL CMultiThread3Dlg::OnInitDialog()中添加语句： {
……
// TODO: Add extra initialization here
m_ctrlProgress.SetRange(0,99);
m_nMilliSecond=10;
UpdateData(FALSE);
return TRUE;   // return TRUE   unless you set the focus to a control
}
添加线程处理函数： UINT ThreadFunc(LPVOID lpParam)
{
threadInfo* pInfo=(threadInfo*)lpParam;
for(int i=0;i<100;i++)
{
   int nTemp=pInfo->nMilliSecond;
   pInfo->pctrlProgress->SetPos(i);
   Sleep(nTemp);
}
return 0;
}
用 MFC 类库编程实现用户界面线程
创建用户界面线程的步骤：
使用ClassWizard创建类CWinThread的派生类（以CUIThread类为例） class CUIThread : public CWinThread
{
DECLARE_DYNCREATE(CUIThread)
protected:
CUIThread();            // protected constructor used by dynamic creation
// Attributes
public:
// Operations
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CUIThread)
public:
virtual BOOL InitInstance();
virtual int ExitInstance();
//}}AFX_VIRTUAL
// Implementation
protected:
virtual ~CUIThread();
// Generated message map functions
//{{AFX_MSG(CUIThread)
   // NOTE - the ClassWizard will add and remove member functions here.
//}}AFX_MSG
DECLARE_MESSAGE_MAP()
};
重载函数InitInstance()和ExitInstance()。 BOOL CUIThread::InitInstance()
{
CFrameWnd* wnd=new CFrameWnd;
wnd->Create(NULL,"UI Thread Window");
wnd->ShowWindow(SW_SHOW);
wnd->UpdateWindow();
m_pMainWnd=wnd;
return TRUE;
}
创建新的用户界面线程 void CUIThreadDlg::OnButton1() 
{
CUIThread* pThread=new CUIThread();
pThread->CreateThread();
}
请注意以下两点：
A、在UIThreadDlg.cpp的开头加入语句： #include "UIThread.h"
B、把UIThread.h中类CUIThread()的构造函数的特性由 protected 改为 public。 
用户界面线程的执行次序与应用程序主线程相同，首先调用用户界面线程类的InitInstance()函数，如果返回TRUE，继续调用线程的Run ()函数，该函数的作用是运行一个标准的消息循环，并且当收到WM_QUIT消息后中断，在消息循环过程中，Run()函数检测到线程空闲时（没有消 息），也将调用OnIdle()函数，最后Run()函数返回，MFC调用ExitInstance()函数清理资源。
你可以创建一个没有界 面而有消息循环的线程，例如：你可以从CWinThread派生一个新类，在InitInstance函数中完成某项任务并返回FALSE，这表示仅执行 InitInstance函数中的任务而不执行消息循环，你可以通过这种方法，完成一个工作者线程的功能。
例程6 MultiThread6
建立一个基于对话框的工程MultiThread6，在对话框IDD_MULTITHREAD6_DIALOG中加入一个按钮IDC_UI_THREAD，标题为“用户界面线程” 
右击工程并选中“New Class…”为工程添加基类为CWinThread派生线程类CUIThread。 
给工程添加新对话框IDD_UITHREADDLG，标题为“线程对话框”。 
为 对话框IDD_UITHREADDLG创建一个基于CDialog的类CUIThreadDlg。使用ClassWizard为CUIThreadDlg 类添加WM_LBUTTONDOWN消息的处理函数OnLButtonDown，如下： void CUIThreadDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
AfxMessageBox("You Clicked The Left Button!");
CDialog::OnLButtonDown(nFlags, point);
}
在UIThread.h中添加 #include "UIThreadDlg.h"
并在CUIThread类中添加protected变量CUIThread m_dlg： class CUIThread : public CWinThread
{
DECLARE_DYNCREATE(CUIThread)
protected:
CUIThread();            // protected constructor used by dynamic creation
// Attributes
public:
// Operations
public:
// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CUIThread)
public:
virtual BOOL InitInstance();
virtual int ExitInstance();
//}}AFX_VIRTUAL
// Implementation
protected:
CUIThreadDlg m_dlg;
virtual ~CUIThread();
// Generated message map functions
//{{AFX_MSG(CUIThread)
   // NOTE - the ClassWizard will add and remove member functions here.
//}}AFX_MSG
DECLARE_MESSAGE_MAP()
};
分别重载InitInstance()函数和ExitInstance()函数： BOOL CUIThread::InitInstance()
{
m_dlg.Create(IDD_UITHREADDLG);
m_dlg.ShowWindow(SW_SHOW);
m_pMainWnd=&m_dlg;
return TRUE;
}
int CUIThread::ExitInstance()
{
m_dlg.DestroyWindow();
return CWinThread::ExitInstance();
}
双击按钮IDC_UI_THREAD，添加消息响应函数： void CMultiThread6Dlg::OnUiThread() 
{
CWinThread *pThread=AfxBeginThread(RUNTIME_CLASS(CUIThread));
}
并在MultiThread6Dlg.cpp的开头添加： #include "UIThread.h"
　　好了，编译并运行程序吧。每单击一次“用户界面线程”按钮，都会弹出一个线程对话框，在任何一个线程对话框内按下鼠标左键，都会弹出一个消息框。
七、线程间通讯
　　一般而言,应用程序中的一个次要线程总是为主线程执行特定的任务,这样,主线程和次要线程间必定有一个信息传递的渠道,也就是主线程和次要线程间要进行通信。这种线程间的通信不但是难以避免的，而且在多线程编程中也是复杂和频繁的，下面将进行说明。
使用全局变量进行通信
由 于属于同一个进程的各个线程共享操作系统分配该进程的资源，故解决线程间通信最简单的一种方法是使用全局变量。对于标准类型的全局变量，我们建议使用 volatile 修饰符，它告诉编译器无需对该变量作任何的优化，即无需将它放到一个寄存器中，并且该值可被外部改变。如果线程间所需传递的信息较复杂，我们可以定义一个 结构，通过传递指向该结构的指针进行传递信息。

