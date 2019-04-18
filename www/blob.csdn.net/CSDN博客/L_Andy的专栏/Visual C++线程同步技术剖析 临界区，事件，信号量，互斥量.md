# Visual C++线程同步技术剖析:临界区，事件，信号量，互斥量 - L_Andy的专栏 - CSDN博客

2015年01月28日 11:14:10[卡哥](https://me.csdn.net/L_Andy)阅读数：1102


转自：

使线程同步

　　在程序中使用多线程时，一般很少有多个线程能在其生命期内进行完全独立的操作。更多的情况是一些线程进行某些处理操作，而其他的线程必须对其处理结果进行了解。正常情况下对这种处理结果的了解应当在其处理任务完成后进行。

　　如果不采取适当的措施，其他线程往往会在线程处理任务结束前就去访问处理结果，这就很有可能得到有关处理结果的错误了解。例如，多个线程同时访问同一个全局变量，如果都是读取操作，则不会出现问题。如果一个线程负责改变此变量的值，而其他线程负责同时读取变量内容，则不能保证读取到的数据是经过写线程修改后的。

　　为了确保读线程读取到的是经过修改的变量，就必须在向变量写入数据时禁止其他线程对其的任何访问，直至赋值过程结束后再解除对其他线程的访问限制。象这种保证线程能了解其他线程任务处理结束后的处理结果而采取的保护措施即为线程同步。

　　线程同步是一个非常大的话题，包括方方面面的内容。从大的方面讲，线程的同步可分用户模式的线程同步和内核对象的线程同步两大类。用户模式中线程的同步方法主要有原子访问和临界区等方法。其特点是同步速度特别快，适合于对线程运行速度有严格要求的场合。

　　内核对象的线程同步则主要由事件、等待定时器、信号量以及信号灯等内核对象构成。由于这种同步机制使用了内核对象，使用时必须将线程从用户模式切换到内核模式，而这种转换一般要耗费近千个CPU周期，因此同步速度较慢，但在适用性上却要远优于用户模式的线程同步方式。

临界区

　　临界区（Critical Section）是一段独占对某些共享资源访问的代码，在任意时刻只允许一个线程对共享资源进行访问。如果有多个线程试图同时访问临界区，那么在有一个线程进入后其他所有试图访问此临界区的线程将被挂起，并一直持续到进入临界区的线程离开。临界区在被释放后，其他线程可以继续抢占，并以此达到用原子方式操作共享资源的目的。

　　临界区在使用时以CRITICAL_SECTION结构对象保护共享资源，并分别用EnterCriticalSection（）和LeaveCriticalSection（）函数去标识和释放一个临界区。所用到的CRITICAL_SECTION结构对象必须经过InitializeCriticalSection（）的初始化后才能使用，而且必须确保所有线程中的任何试图访问此共享资源的代码都处在此临界区的保护之下。否则临界区将不会起到应有的作用，共享资源依然有被破坏的可能。

![](https://img-blog.csdn.net/20150128111116418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图1
使用临界区保持线程同步

下面通过一段代码展示了临界区在保护多线程访问的共享资源中的作用。通过两个线程来分别对全局变量g_cArray[10]进行写入操作，用临界区结构对象g_cs来保持线程的同步，并在开启线程前对其进行初始化。为了使实验效果更加明显，体现出临界区的作用，在线程函数对共享资源g_cArray[10]的写入时，以Sleep（）函数延迟1毫秒，使其他线程同其抢占CPU的可能性增大。如果不使用临界区对其进行保护，则共享资源数据将被破坏（参见图1（a）所示计算结果），而使用临界区对线程保持同步后则可以得到正确的结果（参见图1（b）所示计算结果）。代码实现清单附下：

// 临界区结构对象

CRITICAL_SECTION g_cs;

// 共享资源

char g_cArray[10];

UINT ThreadProc10(LPVOID pParam)

{
// 进入临界区
EnterCriticalSection(&g_cs);
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[i] = 'a';
Sleep(1);
}
// 离开临界区
LeaveCriticalSection(&g_cs);
return 0;

}

UINT ThreadProc11(LPVOID pParam)

{
// 进入临界区
EnterCriticalSection(&g_cs);
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[10 - i - 1] = 'b';
Sleep(1);
}
// 离开临界区
LeaveCriticalSection(&g_cs);
return 0;

}

……

void CSample08View::OnCriticalSection() 

{
// 初始化临界区
InitializeCriticalSection(&g_cs);
// 启动线程
AfxBeginThread(ThreadProc10, NULL);
AfxBeginThread(ThreadProc11, NULL);
// 等待计算完毕
Sleep(300);
// 报告计算结果
CString sResult = CString(g_cArray);
AfxMessageBox(sResult);

}

　　在使用临界区时，一般不允许其运行时间过长，只要进入临界区的线程还没有离开，其他所有试图进入此临界区的线程都会被挂起而进入到等待状态，并会在一定程度上影响。程序的运行性能。尤其需要注意的是不要将等待用户输入或是其他一些外界干预的操作包含到临界区。如果进入了临界区却一直没有释放，同样也会引起其他线程的长时间等待。换句话说，在执行了EnterCriticalSection（）语句进入临界区后无论发生什么，必须确保与之匹配的LeaveCriticalSection（）都能够被执行到。可以通过添加结构化异常处理代码来确保LeaveCriticalSection（）语句的执行。虽然临界区同步速度很快，但却只能用来同步本进程内的线程，而不可用来同步多个进程中的线程。

MFC为临界区提供有一个CCriticalSection类，使用该类进行线程同步处理是非常简单的，只需在线程函数中用CCriticalSection类成员函数Lock（）和UnLock（）标定出被保护代码片段即可。对于上述代码，可通过CCriticalSection类将其改写如下：

// MFC临界区类对象

CCriticalSection g_clsCriticalSection;

// 共享资源

char g_cArray[10];

UINT ThreadProc20(LPVOID pParam)

{
// 进入临界区
g_clsCriticalSection.Lock();
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[i] = 'a';
Sleep(1);
}
// 离开临界区
g_clsCriticalSection.Unlock();
return 0;

}

UINT ThreadProc21(LPVOID pParam)

{
// 进入临界区
g_clsCriticalSection.Lock();
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[10 - i - 1] = 'b';
Sleep(1);
}
// 离开临界区
g_clsCriticalSection.Unlock();
return 0;

}

……

void CSample08View::OnCriticalSectionMfc() 

{
// 启动线程
AfxBeginThread(ThreadProc20, NULL);
AfxBeginThread(ThreadProc21, NULL);
// 等待计算完毕
Sleep(300);
// 报告计算结果
CString sResult = CString(g_cArray);
AfxMessageBox(sResult);

}

管理事件内核对象

　　在前面讲述线程通信时曾使用过事件内核对象来进行线程间的通信，除此之外，事件内核对象也可以通过通知操作的方式来保持线程的同步。对于前面那段使用临界区保持线程同步的代码可用事件对象的线程同步方法改写如下：

// 事件句柄

HANDLE hEvent = NULL;

// 共享资源

char g_cArray[10];

……

UINT ThreadProc12(LPVOID pParam)

{
// 等待事件置位
WaitForSingleObject(hEvent, INFINITE);
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[i] = 'a';
Sleep(1);
}
// 处理完成后即将事件对象置位
SetEvent(hEvent);
return 0;

}

UINT ThreadProc13(LPVOID pParam)

{
// 等待事件置位
WaitForSingleObject(hEvent, INFINITE);
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[10 - i - 1] = 'b';
Sleep(1);
}
// 处理完成后即将事件对象置位
SetEvent(hEvent);
return 0;

}

……

void CSample08View::OnEvent() 

{
// 创建事件
hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
// 事件置位
SetEvent(hEvent);
// 启动线程
AfxBeginThread(ThreadProc12, NULL);
AfxBeginThread(ThreadProc13, NULL);
// 等待计算完毕
Sleep(300);
// 报告计算结果
CString sResult = CString(g_cArray);
AfxMessageBox(sResult);

}

　　在创建线程前，首先创建一个可以自动复位的事件内核对象hEvent，而线程函数则通过WaitForSingleObject（）等待函数无限等待hEvent的置位，只有在事件置位时WaitForSingleObject（）才会返回，被保护的代码将得以执行。对于以自动复位方式创建的事件对象，在其置位后一被WaitForSingleObject（）等待到就会立即复位，也就是说在执行ThreadProc12（）中的受保护代码时，事件对象已经是复位状态的，这时即使有ThreadProc13（）对CPU的抢占，也会由于WaitForSingleObject（）没有hEvent的置位而不能继续执行，也就没有可能破坏受保护的共享资源。在ThreadProc12（）中的处理完成后可以通过SetEvent（）对hEvent的置位而允许ThreadProc13（）对共享资源g_cArray的处理。这里SetEvent（）所起的作用可以看作是对某项特定任务完成的通知。

　　使用临界区只能同步同一进程中的线程，而使用事件内核对象则可以对进程外的线程进行同步，其前提是得到对此事件对象的访问权。可以通过OpenEvent（）函数获取得到，其函数原型为：

HANDLE OpenEvent(
DWORD dwDesiredAccess, // 访问标志
BOOL bInheritHandle, // 继承标志
LPCTSTR lpName // 指向事件对象名的指针

);

　　如果事件对象已创建（在创建事件时需要指定事件名），函数将返回指定事件的句柄。对于那些在创建事件时没有指定事件名的事件内核对象，可以通过使用内核对象的继承性或是调用DuplicateHandle（）函数来调用CreateEvent（）以获得对指定事件对象的访问权。在获取到访问权后所进行的同步操作与在同一个进程中所进行的线程同步操作是一样的。

　　如果需要在一个线程中等待多个事件，则用WaitForMultipleObjects（）来等待。WaitForMultipleObjects（）与WaitForSingleObject（）类似，同时监视位于句柄数组中的所有句柄。这些被监视对象的句柄享有平等的优先权，任何一个句柄都不可能比其他句柄具有更高的优先权。WaitForMultipleObjects（）的函数原型为：

DWORD WaitForMultipleObjects(
DWORD nCount, // 等待句柄数
CONST HANDLE *lpHandles, // 句柄数组首地址
BOOL fWaitAll, // 等待标志
DWORD dwMilliseconds // 等待时间间隔

);

　　参数nCount指定了要等待的内核对象的数目，存放这些内核对象的数组由lpHandles来指向。fWaitAll对指定的这nCount个内核对象的两种等待方式进行了指定，为TRUE时当所有对象都被通知时函数才会返回，为FALSE则只要其中任何一个得到通知就可以返回。dwMilliseconds在饫锏淖饔糜朐赪aitForSingleObject（）中的作用是完全一致的。如果等待超时，函数将返回WAIT_TIMEOUT。如果返回WAIT_OBJECT_0到WAIT_OBJECT_0+nCount-1中的某个值，则说明所有指定对象的状态均为已通知状态（当fWaitAll为TRUE时）或是用以减去WAIT_OBJECT_0而得到发生通知的对象的索引（当fWaitAll为FALSE时）。如果返回值在WAIT_ABANDONED_0与WAIT_ABANDONED_0+nCount-1之间，则表示所有指定对象的状态均为已通知，且其中至少有一个对象是被丢弃的互斥对象（当fWaitAll为TRUE时），或是用以减去WAIT_OBJECT_0表示一个等待正常结束的互斥对象的索引（当fWaitAll为FALSE时）。下面给出的代码主要展示了对WaitForMultipleObjects（）函数的使用。通过对两个事件内核对象的等待来控制线程任务的执行与中途退出：

// 存放事件句柄的数组

HANDLE hEvents[2];

UINT ThreadProc14(LPVOID pParam)

{ 
// 等待开启事件
DWORD dwRet1 = WaitForMultipleObjects(2, hEvents, FALSE,INFINITE);
// 如果开启事件到达则线程开始执行任务
if (dwRet1 == WAIT_OBJECT_0)
{
AfxMessageBox("线程开始工作!");
while (true)
{
for (int i = 0; i < 10000; i++);
// 在任务处理过程中等待结束事件
DWORD dwRet2 = WaitForMultipleObjects(2, hEvents, FALSE,0);
// 如果结束事件置位则立即终止任务的执行
if (dwRet2 == WAIT_OBJECT_0 + 1)
break;
}
}
AfxMessageBox("线程退出!");
return 0;

}

……

void CSample08View::OnStartEvent() 

{
// 创建线程
for (int i = 0; i < 2; i++)
hEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
// 开启线程
AfxBeginThread(ThreadProc14, NULL);
// 设置事件0(开启事件)
SetEvent(hEvents[0]);

}

void CSample08View::OnEndevent() 

{
// 设置事件1(结束事件)
SetEvent(hEvents[1]);

}

MFC为事件相关处理也提供了一个CEvent类，共包含有除构造函数外的4个成员函数PulseEvent（）、ResetEvent（）、SetEvent（）和UnLock（）。在功能上分别相当与Win32
 API的PulseEvent（）、ResetEvent（）、SetEvent（）和CloseHandle（）等函数。而构造函数则履行了原CreateEvent（）函数创建事件对象的职责，其函数原型为：

CEvent(BOOL bInitiallyOwn = FALSE, BOOL bManualReset = FALSE, LPCTSTR lpszName= NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL );

　　按照此缺省设置将创建一个自动复位、初始状态为复位状态的没有名字的事件对象。封装后的CEvent类使用起来更加方便，图2即展示了CEvent类对A、B两线程的同步过程：

![](https://img-blog.csdn.net/20150128111132033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图2 CEvent类对线程的同步过程示意

B线程在执行到CEvent类成员函数Lock（）时将会发生阻塞，而A线程此时则可以在没有B线程干扰的情况下对共享资源进行处理，并在处理完成后通过成员函数SetEvent（）向B发出事件，使其被释放，得以对A先前已处理完毕的共享资源进行操作。可见，使用CEvent类对线程的同步方法与通过API函数进行线程同步的处理方法是基本一致的。前面的API处理代码可用CEvent类将其改写为：

// MFC事件类对象

CEvent g_clsEvent;

UINT ThreadProc22(LPVOID pParam)

{
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[i] = 'a';
Sleep(1);
}
// 事件置位
g_clsEvent.SetEvent();
return 0;

}

UINT ThreadProc23(LPVOID pParam)

{
// 等待事件
g_clsEvent.Lock();
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[10 - i - 1] = 'b';
Sleep(1);
}
return 0;

}

……

void CSample08View::OnEventMfc() 

{
// 启动线程
AfxBeginThread(ThreadProc22, NULL);
AfxBeginThread(ThreadProc23, NULL);
// 等待计算完毕
Sleep(300);
// 报告计算结果
CString sResult = CString(g_cArray);
AfxMessageBox(sResult);

}

　　信号量内核对象

　　信号量（Semaphore）内核对象对线程的同步方式与前面几种方法不同，它允许多个线程在同一时刻访问同一资源，但是需要限制在同一时刻访问此资源的最大线程数目。在用CreateSemaphore（）创建信号量时即要同时指出允许的最大资源计数和当前可用资源计数。一般是将当前可用资源计数设置为最大资源计数，每增加一个线程对共享资源的访问，当前可用资源计数就会减1，只要当前可用资源计数是大于0的，就可以发出信号量信号。但是当前可用计数减小到0时则说明当前占用资源的线程数已经达到了所允许的最大数目，不能在允许其他线程的进入，此时的信号量信号将无法发出。线程在处理完共享资源后，应在离开的同时通过ReleaseSemaphore（）函数将当前可用资源计数加1。在任何时候当前可用资源计数决不可能大于最大资源计数。

![](https://img-blog.csdn.net/20150128111141659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图3
使用信号量对象控制资源

下面结合图例3来演示信号量对象对资源的控制。在图3中，以箭头和白色箭头表示共享资源所允许的最大资源计数和当前可用资源计数。初始如图（a）所示，最大资源计数和当前可用资源计数均为4，此后每增加一个对资源进行访问的线程（用黑色箭头表示）当前资源计数就会相应减1，图（b）即表示的在3个线程对共享资源进行访问时的状态。当进入线程数达到4个时，将如图（c）所示，此时已达到最大资源计数，而当前可用资源计数也已减到0，其他线程无法对共享资源进行访问。在当前占有资源的线程处理完毕而退出后，将会释放出空间，图（d）已有两个线程退出对资源的占有，当前可用计数为2，可以再允许2个线程进入到对资源的处理。可以看出，信号量是通过计数来对线程访问资源进行控制的，而实际上信号量确实也被称作Dijkstra计数器。

　　使用信号量内核对象进行线程同步主要会用到CreateSemaphore（）、OpenSemaphore（）、ReleaseSemaphore（）、WaitForSingleObject（）和WaitForMultipleObjects（）等函数。其中，CreateSemaphore（）用来创建一个信号量内核对象，其函数原型为：

HANDLE CreateSemaphore(
LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, //
安全属性指针
LONG lInitialCount, // 初始计数
LONG lMaximumCount, // 最大计数
LPCTSTR lpName // 对象名指针

);

　　参数lMaximumCount是一个有符号32位值，定义了允许的最大资源计数，最大取值不能超过4294967295。lpName参数可以为创建的信号量定义一个名字，由于其创建的是一个内核对象，因此在其他进程中可以通过该名字而得到此信号量。OpenSemaphore（）函数即可用来根据信号量名打开在其他进程中创建的信号量，函数原型如下：

HANDLE OpenSemaphore(
DWORD dwDesiredAccess, // 访问标志
BOOL bInheritHandle, // 继承标志
LPCTSTR lpName // 信号量名

);

　　在线程离开对共享资源的处理时，必须通过ReleaseSemaphore（）来增加当前可用资源计数。否则将会出现当前正在处理共享资源的实际线程数并没有达到要限制的数值，而其他线程却因为当前可用资源计数为0而仍无法进入的情况。ReleaseSemaphore（）的函数原型为：

BOOL ReleaseSemaphore(
HANDLE hSemaphore, // 信号量句柄
LONG lReleaseCount, // 计数递增数量
LPLONG lpPreviousCount // 先前计数

);

　　该函数将lReleaseCount中的值添加给信号量的当前资源计数，一般将lReleaseCount设置为1，如果需要也可以设置其他的值。WaitForSingleObject（）和WaitForMultipleObjects（）主要用在试图进入共享资源的线程函数入口处，主要用来判断信号量的当前可用资源计数是否允许本线程的进入。只有在当前可用资源计数值大于0时，被监视的信号量内核对象才会得到通知。

　　信号量的使用特点使其更适用于对Socket（套接字）程序中线程的同步。例如，网络上的HTTP服务器要对同一时间内访问同一页面的用户数加以限制，这时可以为没一个用户对服务器的页面请求设置一个线程，而页面则是待保护的共享资源，通过使用信号量对线程的同步作用可以确保在任一时刻无论有多少用户对某一页面进行访问，只有不大于设定的最大用户数目的线程能够进行访问，而其他的访问企图则被挂起，只有在有用户退出对此页面的访问后才有可能进入。下面给出的示例代码即展示了类似的处理过程：

// 信号量对象句柄

HANDLE hSemaphore;

UINT ThreadProc15(LPVOID pParam)

{ 
// 试图进入信号量关口
WaitForSingleObject(hSemaphore, INFINITE);
// 线程任务处理
AfxMessageBox("线程一正在执行!");
// 释放信号量计数
ReleaseSemaphore(hSemaphore, 1, NULL);
return 0;

}

UINT ThreadProc16(LPVOID pParam)

{ 
// 试图进入信号量关口
WaitForSingleObject(hSemaphore, INFINITE);
// 线程任务处理
AfxMessageBox("线程二正在执行!");
// 释放信号量计数
ReleaseSemaphore(hSemaphore, 1, NULL);
return 0;

}

UINT ThreadProc17(LPVOID pParam)

{ 
// 试图进入信号量关口
WaitForSingleObject(hSemaphore, INFINITE);
// 线程任务处理
AfxMessageBox("线程三正在执行!");
// 释放信号量计数
ReleaseSemaphore(hSemaphore, 1, NULL);
return 0;

}

……

void CSample08View::OnSemaphore() 

{
// 创建信号量对象
hSemaphore = CreateSemaphore(NULL, 2, 2, NULL);
// 开启线程
AfxBeginThread(ThreadProc15, NULL);
AfxBeginThread(ThreadProc16, NULL);
AfxBeginThread(ThreadProc17, NULL);

}

![](https://img-blog.csdn.net/20150128111149490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图4
开始进入的两个线程

![](https://img-blog.csdn.net/20150128111121796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图5
线程二退出后线程三才得以进入

上述代码在开启线程前首先创建了一个初始计数和最大资源计数均为2的信号量对象hSemaphore。即在同一时刻只允许2个线程进入由hSemaphore保护的共享资源。随后开启的三个线程均试图访问此共享资源，在前两个线程试图访问共享资源时，由于hSemaphore的当前可用资源计数分别为2和1，此时的hSemaphore是可以得到通知的，也就是说位于线程入口处的WaitForSingleObject（）将立即返回，而在前两个线程进入到保护区域后，hSemaphore的当前资源计数减少到0，hSemaphore将不再得到通知，WaitForSingleObject（）将线程挂起。直到此前进入到保护区的线程退出后才能得以进入。图4和图5为上述代脉的运行结果。从实验结果可以看出，信号量始终保持了同一时刻不超过2个线程的进入。

　　在MFC中，通过CSemaphore类对信号量作了表述。该类只具有一个构造函数，可以构造一个信号量对象，并对初始资源计数、最大资源计数、对象名和安全属性等进行初始化，其原型如下：

CSemaphore( LONG lInitialCount = 1, LONG lMaxCount = 1, LPCTSTR pstrName =NULL, LPSECURITY_ATTRIBUTES lpsaAttributes = NULL );

　　在构造了CSemaphore类对象后，任何一个访问受保护共享资源的线程都必须通过CSemaphore从父类CSyncObject类继承得到的Lock（）和UnLock（）成员函数来访问或释放CSemaphore对象。与前面介绍的几种通过MFC类保持线程同步的方法类似，通过CSemaphore类也可以将前面的线程同步代码进行改写，这两种使用信号量的线程同步方法无论是在实现原理上还是从实现结果上都是完全一致的。下面给出经MFC改写后的信号量线程同步代码：

// MFC信号量类对象

CSemaphore g_clsSemaphore(2, 2);

UINT ThreadProc24(LPVOID pParam)

{ 
// 试图进入信号量关口
g_clsSemaphore.Lock();
// 线程任务处理
AfxMessageBox("线程一正在执行!");
// 释放信号量计数
g_clsSemaphore.Unlock();
return 0;

}

UINT ThreadProc25(LPVOID pParam)

{
// 试图进入信号量关口
g_clsSemaphore.Lock();
// 线程任务处理
AfxMessageBox("线程二正在执行!");
// 释放信号量计数
g_clsSemaphore.Unlock();
return 0;

}

UINT ThreadProc26(LPVOID pParam)

{
// 试图进入信号量关口
g_clsSemaphore.Lock();
// 线程任务处理
AfxMessageBox("线程三正在执行!");
// 释放信号量计数
g_clsSemaphore.Unlock();
return 0;

}

……

void CSample08View::OnSemaphoreMfc() 

{
// 开启线程
AfxBeginThread(ThreadProc24, NULL);
AfxBeginThread(ThreadProc25, NULL);
AfxBeginThread(ThreadProc26, NULL);

}
　　互斥内核对象

　　互斥（Mutex）是一种用途非常广泛的内核对象。能够保证多个线程对同一共享资源的互斥访问。同临界区有些类似，只有拥有互斥对象的线程才具有访问资源的权限，由于互斥对象只有一个，因此就决定了任何情况下此共享资源都不会同时被多个线程所访问。当前占据资源的线程在任务处理完后应将拥有的互斥对象交出，以便其他线程在获得后得以访问资源。与其他几种内核对象不同，互斥对象在操作系统中拥有特殊代码，并由操作系统来管理，操作系统甚至还允许其进行一些其他内核对象所不能进行的非常规操作。为便于理解，可参照图6给出的互斥内核对象的工作模型：

![](https://img-blog.csdn.net/20150128111204388?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图6 使用互斥内核对象对共享资源的保护
图（a）中的箭头为要访问资源（矩形框）的线程，但只有第二个线程拥有互斥对象（黑点）并得以进入到共享资源，而其他线程则会被排斥在外（如图（b）所示）。当此线程处理完共享资源并准备离开此区域时将把其所拥有的互斥对象交出（如图（c）所示），其他任何一个试图访问此资源的线程都有机会得到此互斥对象。

　　以互斥内核对象来保持线程同步可能用到的函数主要有CreateMutex（）、OpenMutex（）、ReleaseMutex（）、WaitForSingleObject（）和WaitForMultipleObjects（）等。在使用互斥对象前，首先要通过CreateMutex（）或OpenMutex（）创建或打开一个互斥对象。CreateMutex（）函数原型为：

HANDLE CreateMutex(
LPSECURITY_ATTRIBUTES lpMutexAttributes, // 安全属性指针
BOOL bInitialOwner, // 初始拥有者
LPCTSTR lpName // 互斥对象名

);

　　参数bInitialOwner主要用来控制互斥对象的初始状态。一般多将其设置为FALSE，以表明互斥对象在创建时并没有为任何线程所占有。如果在创建互斥对象时指定了对象名，那么可以在本进程其他地方或是在其他进程通过OpenMutex（）函数得到此互斥对象的句柄。OpenMutex（）函数原型为：

HANDLE OpenMutex(
DWORD dwDesiredAccess, // 访问标志
BOOL bInheritHandle, // 继承标志
LPCTSTR lpName // 互斥对象名

);

　　当目前对资源具有访问权的线程不再需要访问此资源而要离开时，必须通过ReleaseMutex（）函数来释放其拥有的互斥对象，其函数原型为：

BOOL ReleaseMutex(HANDLE hMutex);

　　其唯一的参数hMutex为待释放的互斥对象句柄。至于WaitForSingleObject（）和WaitForMultipleObjects（）等待函数在互斥对象保持线程同步中所起的作用与在其他内核对象中的作用是基本一致的，也是等待互斥内核对象的通知。但是这里需要特别指出的是：在互斥对象通知引起调用等待函数返回时，等待函数的返回值不再是通常的WAIT_OBJECT_0（对于WaitForSingleObject（）函数）或是在WAIT_OBJECT_0到WAIT_OBJECT_0+nCount-1之间的一个值（对于WaitForMultipleObjects（）函数），而是将返回一个WAIT_ABANDONED_0（对于WaitForSingleObject（）函数）或是在WAIT_ABANDONED_0到WAIT_ABANDONED_0+nCount-1之间的一个值（对于WaitForMultipleObjects（）函数）。以此来表明线程正在等待的互斥对象由另外一个线程所拥有，而此线程却在使用完共享资源前就已经终止。除此之外，使用互斥对象的方法在等待线程的可调度性上同使用其他几种内核对象的方法也有所不同，其他内核对象在没有得到通知时，受调用等待函数的作用，线程将会挂起，同时失去可调度性，而使用互斥的方法却可以在等待的同时仍具有可调度性，这也正是互斥对象所能完成的非常规操作之一。

　　在编写程序时，互斥对象多用在对那些为多个线程所访问的内存块的保护上，可以确保任何线程在处理此内存块时都对其拥有可靠的独占访问权。下面给出的示例代码即通过互斥内核对象hMutex对共享内存快g_cArray[]进行线程的独占访问保护。下面给出实现代码清单：

// 互斥对象

HANDLE hMutex = NULL;

char g_cArray[10];

UINT ThreadProc18(LPVOID pParam)

{
// 等待互斥对象通知
WaitForSingleObject(hMutex, INFINITE);
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[i] = 'a';
Sleep(1);
}
// 释放互斥对象
ReleaseMutex(hMutex);
return 0;

}

UINT ThreadProc19(LPVOID pParam)

{
// 等待互斥对象通知
WaitForSingleObject(hMutex, INFINITE);
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[10 - i - 1] = 'b';
Sleep(1);
}
// 释放互斥对象
ReleaseMutex(hMutex);
return 0;

}

……

void CSample08View::OnMutex() 

{
// 创建互斥对象
hMutex = CreateMutex(NULL, FALSE, NULL);
// 启动线程
AfxBeginThread(ThreadProc18, NULL);
AfxBeginThread(ThreadProc19, NULL);
// 等待计算完毕
Sleep(300);
// 报告计算结果
CString sResult = CString(g_cArray);
AfxMessageBox(sResult);

}

　　互斥对象在MFC中通过CMutex类进行表述。使用CMutex类的方法非常简单，在构造CMutex类对象的同时可以指明待查询的互斥对象的名字，在构造函数返回后即可访问此互斥变量。CMutex类也是只含有构造函数这唯一的成员函数，当完成对互斥对象保护资源的访问后，可通过调用从父类CSyncObject继承的UnLock（）函数完成对互斥对象的释放。CMutex类构造函数原型为：

CMutex( BOOL bInitiallyOwn = FALSE,LPCTSTR lpszName = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL );

　　该类的适用范围和实现原理与API方式创建的互斥内核对象是完全类似的，但要简洁的多，下面给出就是对前面的示例代码经CMutex类改写后的程序实现清单：

// MFC互斥类对象

CMutex g_clsMutex(FALSE, NULL);

UINT ThreadProc27(LPVOID pParam)

{
// 等待互斥对象通知
g_clsMutex.Lock();
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[i] = 'a';
Sleep(1);
}
// 释放互斥对象
g_clsMutex.Unlock();
return 0;

}

UINT ThreadProc28(LPVOID pParam)

{
// 等待互斥对象通知
g_clsMutex.Lock();
// 对共享资源进行写入操作
for (int i = 0; i < 10; i++)
{
g_cArray[10 - i - 1] = 'b';
Sleep(1);
}
// 释放互斥对象
g_clsMutex.Unlock();
return 0;

}

……

void CSample08View::OnMutexMfc() 

{
// 启动线程
AfxBeginThread(ThreadProc27, NULL);
AfxBeginThread(ThreadProc28, NULL);
// 等待计算完毕
Sleep(300);
// 报告计算结果
CString sResult = CString(g_cArray);
AfxMessageBox(sResult);

}

　　小结

　　线程的使用使程序处理更够更加灵活，而这种灵活同样也会带来各种不确定性的可能。尤其是在多个线程对同一公共变量进行访问时。虽然未使用线程同步的程序代码在逻辑上或许没有什么问题，但为了确保程序的正确、可靠运行，必须在适当的场合采取线程同步措施。

