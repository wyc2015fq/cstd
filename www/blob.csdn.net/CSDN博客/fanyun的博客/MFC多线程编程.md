# MFC多线程编程 - fanyun的博客 - CSDN博客
2016年10月30日 20:50:45[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：842
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
MFC多线程编程 
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
1、HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes,
                 DWORD dwStackSize,
                 LPTHREAD_START_ROUTINE lpStartAddress,
                 LPVOID lpParameter,
                 DWORD dwCreationFlags,
                 LPDWORD lpThreadId);
该函数在其调用进程的进程空间里创建一个新的线程，并返回已建线程的句柄，其中各参数说明如下： 
•	lpThreadAttributes：指向一个 SECURITY_ATTRIBUTES 结构的指针，该结构决定了线程的安全属性，一般置为 NULL； 
•	dwStackSize：指定了线程的堆栈深度，一般都设置为0； 
•	lpStartAddress：表示新线程开始执行时代码所在函数的地址，即线程的起始地址。一般情况为(LPTHREAD_START_ROUTINE)ThreadFunc，ThreadFunc 是线程函数名； 
•	lpParameter：指定了线程执行时传送给线程的32位参数，即线程函数的参数； 
•	dwCreationFlags：控制线程创建的附加标志，可以取两种值。如果该参数为0，线程在被创建后就会立即开始执行；如果该参数为CREATE_SUSPENDED,则系统产生线程后，该线程处于挂起状态，并不马上执行，直至函数ResumeThread被调用； 
•	lpThreadId：该参数返回所创建线程的ID； 
如果创建成功则返回线程的句柄，否则返回NULL。 
2、DWORD SuspendThread(HANDLE hThread);
该函数用于挂起指定的线程，如果函数执行成功，则线程的执行被终止。 
3、DWORD ResumeThread(HANDLE hThread);
该函数用于结束线程的挂起状态，执行线程。 
4、VOID ExitThread(DWORD dwExitCode);
该函数用于线程终结自身的执行，主要在线程的执行函数中被调用。其中参数dwExitCode用来设置线程的退出码。 
5、BOOL TerminateThread(HANDLE hThread,DWORD dwExitCode);
　　一般情况下，线程运行结束之后，线程函数正常返回，但是应用程序可以调用TerminateThread强行终止某一线程的执行。各参数含义如下： 
•	hThread：将被终结的线程的句柄； 
•	dwExitCode：用于指定线程的退出码。 
　　使用TerminateThread()终止某个线程的执行是不安全的，可能会引起系统不稳定；虽然该函数立即终止线程的执行，但并不释放线程所占用的资源。因此，一般不建议使用该函数。 
6、BOOL PostThreadMessage(DWORD idThread,
			UINT Msg,
			WPARAM wParam,
			LPARAM lParam);
该函数将一条消息放入到指定线程的消息队列中，并且不等到消息被该线程处理时便返回。 
•	idThread：将接收消息的线程的ID； 
•	Msg：指定用来发送的消息； 
•	wParam：同消息有关的字参数； 
•	lParam：同消息有关的长参数； 
调用该函数时，如果即将接收消息的线程没有创建消息循环，则该函数执行失败。
四、Win32 API多线程编程例程
例程1 MultiThread1
1.	建立一个基于对话框的工程MultiThread1，在对话框IDD_MULTITHREAD1_DIALOG中加入两个按钮和一个编辑框，两个按钮的ID分别是IDC_START，IDC_STOP ，标题分别为“启动”，“停止”，IDC_STOP的属性选中Disabled；编辑框的ID为IDC_TIME ，属性选中Read-only；
　 
2.	在MultiThread1Dlg.h文件中添加线程函数声明： 
3.	void ThreadFunc();
注意，线程函数的声明应在类CMultiThread1Dlg的外部。 在类CMultiThread1Dlg内部添加protected型变量： 
	HANDLE hThread;
	DWORD ThreadID;
分别代表线程的句柄和ID。 
　 
4.	在MultiThread1Dlg.cpp文件中添加全局变量m_bRun ： 
5.	volatile BOOL m_bRun;
m_bRun 代表线程是否正在运行。
你要留意到全局变量 m_bRun 是使用 volatile 修饰符的，volatile 修饰符的作用是告诉编译器无需对该变量作任何的优化，即无需将它放到一个寄存器中，并且该值可被外部改变。对于多线程引用的全局变量来说，volatile 是一个非常重要的修饰符。
编写线程函数： 
void ThreadFunc()
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
双击IDC_STOP按钮，完成该按钮的消息函数： 
void CMultiThread1Dlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	m_bRun=FALSE;
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
}
编译并运行该例程，体会使用Win32 API编写的多线程。 
例程2 MultiThread2
　　该线程演示了如何传送一个一个整型的参数到一个线程中，以及如何等待一个线程完成处理。
1.	建立一个基于对话框的工程MultiThread2，在对话框IDD_MULTITHREAD2_DIALOG中加入一个编辑框和一个按钮，ID分别是IDC_COUNT，IDC_START ，按钮控件的标题为“开始”； 
2.	在MultiThread2Dlg.h文件中添加线程函数声明： 
3.	void ThreadFunc(int integer);
注意，线程函数的声明应在类CMultiThread2Dlg的外部。
在类CMultiThread2Dlg内部添加protected型变量: 
	HANDLE hThread;
	DWORD ThreadID;
分别代表线程的句柄和ID。
　 
4.	打开ClassWizard，为编辑框IDC_COUNT添加int型变量m_nCount。在MultiThread2Dlg.cpp文件中添加：
5.	void ThreadFunc(int integer)
6.	{
7.		int i;
8.		for(i=0;i<integer;i++)
9.		{
10.			Beep(200,50);
11.			Sleep(1000);
12.		}
13.	} 
双击IDC_START按钮，完成该按钮的消息函数： 
void CMultiThread2Dlg::OnStart() 
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
顺便说一下WaitForSingleObject函数，其函数原型为：
DWORD WaitForSingleObject(HANDLE hHandle,DWORD dwMilliseconds);
o	hHandle为要监视的对象（一般为同步对象，也可以是线程）的句柄； 
o	dwMilliseconds为hHandle对象所设置的超时值，单位为毫秒； 
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
创建线程时
CreateThread(NULL,0,threadFunc,pMyType,…);
在threadFunc函数内部，可以使用“强制转换”：
int intValue=((pMyType)lpvoid)->firstArgu;
long longValue=((pMyType)lpvoid)->seconddArgu;
……
例程3 MultiThread3将演示如何传送一个指向结构体的指针参数。 
1.	建立一个基于对话框的工程MultiThread3，在对话框IDD_MULTITHREAD3_DIALOG中加入一个编辑框IDC_MILLISECOND，一个按钮IDC_START，标题为“开始” ，一个进度条IDC_PROGRESS1； 
2.	打开ClassWizard，为编辑框IDC_MILLISECOND添加int型变量m_nMilliSecond，为进度条IDC_PROGRESS1添加CProgressCtrl型变量m_ctrlProgress； 
3.	在MultiThread3Dlg.h文件中添加一个结构的定义： 
4.	struct threadInfo
5.	{
6.		UINT nMilliSecond;
7.		CProgressCtrl* pctrlProgress;
8.	};
线程函数的声明： 
UINT ThreadFunc(LPVOID lpParam);
注意，二者应在类CMultiThread3Dlg的外部。 
在类CMultiThread3Dlg内部添加protected型变量: 
HANDLE hThread;
DWORD ThreadID;
分别代表线程的句柄和ID。 
9.	在MultiThread3Dlg.cpp文件中进行如下操作：
定义公共变量 threadInfo Info；
双击按钮IDC_START，添加相应消息处理函数：
10.	void CMultiThread3Dlg::OnStart() 
11.	{
12.		// TODO: Add your control notification handler code here
13.	
14.		UpdateData(TRUE);
15.		Info.nMilliSecond=m_nMilliSecond;
16.		Info.pctrlProgress=&m_ctrlProgress;
17.	
18.		hThread=CreateThread(NULL,
19.			0,
20.			(LPTHREAD_START_ROUTINE)ThreadFunc,
21.			&Info,
22.			0,
23.			&ThreadID);
24.	/*
25.		GetDlgItem(IDC_START)->EnableWindow(FALSE);
26.		WaitForSingleObject(hThread,INFINITE);
27.		GetDlgItem(IDC_START)->EnableWindow(TRUE);
28.	*/
29.	}
在函数BOOL CMultiThread3Dlg::OnInitDialog()中添加语句： 
{
	……
	
	// TODO: Add extra initialization here
	m_ctrlProgress.SetRange(0,99);
	m_nMilliSecond=10;
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}
添加线程处理函数：UINT ThreadFunc(LPVOID lpParam) 
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
　　顺便补充一点，如果你在void CMultiThread3Dlg::OnStart() 函数中添加/* */语句，编译运行你就会发现进度条不进行刷新，主线程也停止了反应。什么原因呢？这是因为WaitForSingleObject函数等待子线程（ThreadFunc）结束时，导致了线程死锁。因为WaitForSingleObject函数会将主线程挂起（任何消息都得不到处理），而子线程ThreadFunc正在设置进度条，一直在等待主线程将刷新消息处理完毕返回才会检测通知事件。这样两个线程都在互相等待，死锁发生了，编程时应注意避免。 
例程4 MultiThread4 
该例程测试在Windows下最多可创建线程的数目。 
1.	建立一个基于对话框的工程MultiThread4，在对话框IDD_MULTITHREAD4_DIALOG中加入一个按钮IDC_TEST和一个编辑框IDC_COUNT，按钮标题为“测试” ， 编辑框属性选中Read-only； 
2.	在MultiThread4Dlg.cpp文件中进行如下操作：
添加公共变量
volatile BOOL m_bRunFlag=TRUE; 
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
