# 深入浅出Win32多线程程序设计之线程通信 - 深之JohnChen的专栏 - CSDN博客

2006年03月31日 11:24:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3371标签：[多线程																[winapi																[attributes																[semaphore																[null																[access](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=semaphore&t=blog)](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
个人分类：[多线程编程](https://blog.csdn.net/byxdaz/article/category/560198)


**深入浅出Win32多线程程序设计之线程通信**

作者：宋宝华出处：天极开发责任编辑：[方舟](http://comments.yesky.com/t/%B7%BD%D6%DB/6,324/2237333.shtml)[ 2005-12-16 08:48 ]

线程之间通信的两个基本问题是互斥和同步。

**简介**　　线程之间通信的两个基本问题是互斥和同步。

　　线程同步是指线程之间所具有的一种制约关系，一个线程的执行依赖另一个线程的消息，当它没有得到另一个线程的消息时应等待，直到消息到达时才被唤醒。

　　线程互斥是指对于共享的操作系统资源（指的是广义的"资源"，而不是Windows的.res文件，譬如全局变量就是一种共享资源），在各线程访问时的排它性。当有若干个线程都要使用某一共享资源时，任何时刻最多只允许一个线程去使用，其它要使用该资源的线程必须等待，直到占用资源者释放该资源。

　　线程互斥是一种特殊的线程同步。

　　实际上，互斥和同步对应着线程间通信发生的两种情况：

[（1）](http://www.yesky.com/key/3342/163342.html)当有多个线程访问共享资源而不使资源被破坏时；

　　（2）当一个线程需要将某个任务已经完成的情况[通知](http://www.yesky.com/key/4176/164176.html)另外一个或多个线程时。

　　在[WIN32](http://www.yesky.com/key/305/170305.html)中，同步机制主要有以下几种：

　　（1）事件(Event);

　　（2）信号量(semaphore);

　　（3）互斥量(mutex);

　　（4）临界区([Critical](http://www.yesky.com/key/4073/169073.html) section)。

**全局变量**

　　因为进程中的所有线程均可以访问所有的全局变量，因而全局变量成为Win32多线程通信的最简单方式。例如：

int var; //全局变量
UINT ThreadFunction(LPVOIDpParam)
{
　var = 0;
　while (var < MaxValue)
　{
　　//线程处理
　　::InterlockedIncrement(long*) &var);
　}
　return 0;
}
请看下列程序：
int globalFlag = false; 
DWORD WINAPI ThreadFunc(LPVOID n)
{
　Sleep(2000);
　globalFlag = true;

　return 0;
}

int main()
{
　HANDLE hThrd;
　DWORD threadId;

　hThrd = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &threadId);
　if (hThrd)
　{
　　printf("Thread launched/n");
　　CloseHandle(hThrd);
　}

　while (!globalFlag)
　;
　printf("exit/n");
}
　　上述程序中使用全局变量和while循环查询进行线程间同步，实际上，这是一种应该避免的方法，因为： 

　　（1）当主线程必须使自己与ThreadFunc函数的完成运行实现同步时，它并没有使自己进入睡眠状态。由于主线程没有进入睡眠状态，因此操作系统继续为它调度C P U时间，这就要占用其他线程的宝贵时间周期；

　　（2）当主线程的优先级高于执行ThreadFunc函数的线程时，就会发生globalFlag永远不能被赋值为true的情况。因为在这种情况下，系统决不会将任何时间片分配给ThreadFunc线程。

**事件**

　　事件(Event)是WIN32提供的最灵活的线程间同步方式，事件可以处于激发状态(signaled or true)或未激发状态(unsignal or false)。根据状态变迁方式的不同，事件可分为两类：

　　（1）手动设置：这种对象只可能用程序手动设置，在需要该事件或者事件发生时，采用SetEvent及ResetEvent来进行设置。

　　（2）自动恢复：一旦事件发生并被处理后，自动恢复到没有事件状态，不需要再次设置。

　　创建事件的函数原型为：

HANDLE CreateEvent(
　LPSECURITY_ATTRIBUTES lpEventAttributes,
　// [SECURITY](http://www.yesky.com/key/1881/161881.html)_ATTRIBUTES结构指针，可为NULL
　BOOL bManualReset, 
　// 手动/自动
　// TRUE：在WaitForSingleObject后必须手动调用ResetEvent清除信号
　// FALSE：在WaitForSingleObject后，系统自动清除事件信号
　BOOL bInitialState, //初始状态
　LPCTSTR lpName //事件的名称
);
　　使用"事件"机制应注意以下事项：

　　（1）如果跨进程访问事件，必须对事件命名，在对事件命名的时候，要注意不要与系统命名空间中的其它全局命名对象冲突；

　　（2）事件是否要自动恢复；

　　（3）事件的初始状态设置。

　　由于event对象属于内核对象，故进程B可以调用OpenEvent函数通过对象的名字获得进程A中event对象的句柄，然后将这个句柄用于ResetEvent、SetEvent和WaitForMultipleObjects等函数中。此法可以实现一个进程的线程控制另一进程中线程的运行，例如： 

HANDLE hEvent=OpenEvent(EVENT_ALL_ACCESS,true,"MyEvent"); 
ResetEvent(hEvent);

**临界区**　　定义临界区变量

CRITICAL_SECTION gCriticalSection;
　　通常情况下，CRITICAL_SECTION结构体应该被定义为全局变量，以便于进程中的所有线程方便地按照变量名来引用该结构体。

　　初始化临界区

VOID WINAPI InitializeCriticalSection(
　LPCRITICAL_SECTION lpCriticalSection
　//指向程序员定义的CRITICAL_SECTION变量
);
　　该函数用于对pcs所指的CRITICAL_SECTION结构体进行初始化。该函数只是设置了一些成员变量，它的运行一般不会失败，因此它采用了VOID类型的返回值。该函数必须在任何线程调用EnterCriticalSection函数之前被调用，如果一个线程试图进入一个未初始化的CRTICAL_SECTION，那么结果将是很难预计的。

　　删除临界区

VOID WINAPI DeleteCriticalSection(
　LPCRITICAL_SECTION lpCriticalSection
　//指向一个不再需要的CRITICAL_SECTION变量
);
　　进入临界区

VOID WINAPI EnterCriticalSection(
　LPCRITICAL_SECTION lpCriticalSection
　//指向一个你即将锁定的CRITICAL_SECTION变量
);
　　离开临界区

VOID WINAPI LeaveCriticalSection(
　LPCRITICAL_SECTION lpCriticalSection
　//指向一个你即将离开的CRITICAL_SECTION变量
);
　　使用临界区编程的一般方法是：

void UpdateData()
{
　EnterCriticalSection(&gCriticalSection);
　...//do something
　LeaveCriticalSection(&gCriticalSection);
}
　　关于临界区的使用，有下列注意点：

　　（1）每个共享资源使用一个CRITICAL_SECTION变量；

　　（2）不要长时间运行关键代码段，当一个关键代码段长时间运行时，其他线程就会进入等待状态，这会降低应用程序的运行性能；

　　（3）如果需要同时访问多个资源，则可能连续调用EnterCriticalSection；

　　（4）Critical Section不是OS核心对象，如果进入临界区的线程"挂"了，将无法释放临界资源。这个缺点在Mutex中得到了弥补。

**互斥**

　　互斥量的作用是保证每次只能有一个线程获得互斥量而得以继续执行，使用CreateMutex函数创建： 

HANDLE CreateMutex(
　LPSECURITY_ATTRIBUTES lpMutexAttributes,
　// 安全属性结构指针，可为NULL
　BOOL bInitialOwner, 
　//是否占有该互斥量，TRUE：占有，FALSE：不占有
　LPCTSTR lpName 
　//信号量的名称
);

　　Mutex是核心对象，可以跨进程访问，下面的代码给出了从另一进程访问命名Mutex的例子：

HANDLE hMutex;
hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"mutexName"); 
if (hMutex){
　… 
｝
else{
　…
}
　　相关API：

BOOL WINAPI ReleaseMutex(
　HANDLE hMutex
);
　　使用互斥编程的一般方法是：

void UpdateResource()
{
　WaitForSingleObject(hMutex,…);
　...//do something
　ReleaseMutex(hMutex);
}
　　互斥(mutex)内核对象能够确保线程拥有对单个资源的互斥访问权。互斥对象的行为特性与临界区相同，但是互斥对象属于内核对象，而临界区则属于用户方式对象，因此这导致mutex与Critical Section的如下不同：

　　（1） 互斥对象的运行速度比关键代码段要慢；

　　（2） 不同进程中的多个线程能够访问单个互斥对象；

　　（3） 线程在等待访问资源时可以设定一个超时值。

　　下图更详细地列出了互斥与临界区的不同：

![](http://soft.yesky.com/imagelist/05/12/98khj15k3n37.jpg)

**信号量**　　信号量是维护0到指定最大值之间的同步对象。信号量状态在其计数大于0时是有信号的，而其计数是0时是无信号的。信号量对象在控制上可以支持有限数量共享资源的访问。

　　信号量的特点和用途可用下列几句话定义：

　　（1）如果当前资源的数量大于0，则信号量有效；

　　（2）如果当前资源数量是0，则信号量无效；

　　（3）系统决不允许当前资源的数量为负值；

　　（4）当前资源数量决不能大于最大资源数量。

　　创建信号量

HANDLE CreateSemaphore (
　PSECURITY_ATTRIBUTE psa,
　LONG lInitialCount, //开始时可供使用的资源数
　LONG lMaximumCount, //最大资源数
PCTSTR pszName);
　　释放信号量

　　通过调用ReleaseSemaphore函数，线程就能够对信标的当前资源数量进行递增，该函数原型为：

BOOL WINAPI ReleaseSemaphore(
　HANDLE hSemaphore,
　LONG lReleaseCount, //信号量的当前资源数增加lReleaseCount
　LPLONG lpPreviousCount
);
　　打开信号量

　　和其他核心对象一样，信号量也可以通过名字跨进程访问，打开信号量的API为：

HANDLE OpenSemaphore (
　DWORD fdwAccess,
　BOOL bInherithandle,
　PCTSTR pszName
);
**互锁访问**

　　当必须以原子操作方式来修改单个值时，互锁访问函数是相当有用的。所谓原子访问，是指线程在访问资源时能够确保所有其他线程都不在同一时间内访问相同的资源。

　　请看下列代码：

int globalVar = 0;

DWORD WINAPI ThreadFunc1(LPVOID n)
{
　globalVar++;
　return 0;
}
DWORD WINAPI ThreadFunc2(LPVOID n)
{
　globalVar++;
　return 0;
}
　　运行ThreadFunc1和ThreadFunc2线程，结果是不可预料的，因为globalVar++并不对应着一条机器指令，我们看看globalVar++的反汇编代码：

00401038 mov eax,[globalVar (0042d3f0)]
0040103D add eax,1
00401040 mov [globalVar (0042d3f0)],eax
　　在"mov eax,[globalVar (0042d3f0)]" 指令与"add eax,1" 指令以及"add eax,1" 指令与"mov [globalVar (0042d3f0)],eax"指令之间都可能发生线程切换，使得程序的执行后globalVar的结果不能确定。我们可以使用InterlockedExchangeAdd函数解决这个问题：

int globalVar = 0;

DWORD WINAPI ThreadFunc1(LPVOID n)
{
　InterlockedExchangeAdd(&globalVar,1);
　return 0;
}
DWORD WINAPI ThreadFunc2(LPVOID n)
{
　InterlockedExchangeAdd(&globalVar,1);
　return 0;
}
　　InterlockedExchangeAdd保证对变量globalVar的访问具有"原子性"。互锁访问的控制速度非常快，调用一个互锁函数的CPU周期通常小于50，不需要进行用户方式与内核方式的切换（该切换通常需要运行1000个CPU周期）。

　　互锁访问函数的缺点在于其只能对单一变量进行原子访问，如果要访问的资源比较复杂，仍要使用临界区或互斥。

　　可等待定时器

　　可等待定时器是在某个时间或按规定的间隔时间发出自己的信号通知的内核对象。它们通常用来在某个时间执行某个操作。

　　创建可等待定时器

HANDLE CreateWaitableTimer(
　PSECURITY_ATTRISUTES psa,
　BOOL fManualReset,//人工重置或自动重置定时器
PCTSTR pszName);
　　设置可等待定时器

　　可等待定时器对象在非激活状态下被创建，程序员应调用 SetWaitableTimer函数来界定定时器在何时被激活：

BOOL SetWaitableTimer(
　HANDLE hTimer, //要设置的定时器
　const LARGE_INTEGER *pDueTime, //指明定时器第一次激活的时间
　LONG lPeriod, //指明此后定时器应该间隔多长时间激活一次
　PTIMERAPCROUTINE pfnCompletionRoutine,
　PVOID PvArgToCompletionRoutine,
BOOL fResume);
　　取消可等待定时器

BOOl Cancel WaitableTimer(
　HANDLE hTimer //要取消的定时器
);
　　打开可等待定时器

　　作为一种内核对象，WaitableTimer也可以被其他进程以名字打开：

HANDLE OpenWaitableTimer (
　DWORD fdwAccess,
　BOOL bInherithandle,
　PCTSTR pszName
);
**实例**

　　下面给出的一个程序可能发生死锁现象：

#include <windows.h>
#include <stdio.h>
CRITICAL_SECTION cs1, cs2;
long WINAPI ThreadFn(long);
main()
{
　long iThreadID;
　InitializeCriticalSection(&cs1);
　InitializeCriticalSection(&cs2);
　CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFn, NULL, 0,&iThreadID));
　while (TRUE)
　{
　　EnterCriticalSection(&cs1);
　　printf("/n线程1占用临界区1");
　　EnterCriticalSection(&cs2);
　　printf("/n线程1占用临界区2");

　　printf("/n线程1占用两个临界区");

　　LeaveCriticalSection(&cs2);
　　LeaveCriticalSection(&cs1);

　　printf("/n线程1释放两个临界区");
　　Sleep(20);
　};
　return (0);
}

long WINAPI ThreadFn(long lParam)
{
　while (TRUE)
　{
　　EnterCriticalSection(&cs2);
　　printf("/n线程2占用临界区2");
　　EnterCriticalSection(&cs1);
　　printf("/n线程2占用临界区1");

　　printf("/n线程2占用两个临界区");

　　LeaveCriticalSection(&cs1);
　　LeaveCriticalSection(&cs2);

　　printf("/n线程2释放两个临界区");
　　Sleep(20);
　};
}
　　运行这个程序，在中途一旦发生这样的输出：

　　线程1占用临界区1

　　线程2占用临界区2

　　或

　　线程2占用临界区2

　　线程1占用临界区1

　　或

　　线程1占用临界区2

　　线程2占用临界区1

　　或

　　线程2占用临界区1

　　线程1占用临界区2

　　程序就"死"掉了，再也运行不下去。因为这样的输出，意味着两个线程相互等待对方释放临界区，也即出现了死锁。

　　如果我们将线程2的控制函数改为：

long WINAPI ThreadFn(long lParam)
{
　while (TRUE)
　{
　　EnterCriticalSection(&cs1);
　　printf("/n线程2占用临界区1");
　　EnterCriticalSection(&cs2);
　　printf("/n线程2占用临界区2");

　　printf("/n线程2占用两个临界区");

　　LeaveCriticalSection(&cs1);
　　LeaveCriticalSection(&cs2);

　　printf("/n线程2释放两个临界区");
　　Sleep(20);
　};
}
　　再次运行程序，死锁被消除，程序不再挡掉。这是因为我们改变了线程2中获得临界区1、2的顺序，消除了线程1、2相互等待资源的可能性。

　　由此我们得出结论，在使用线程间的同步机制时，要特别留心死锁的发生。


