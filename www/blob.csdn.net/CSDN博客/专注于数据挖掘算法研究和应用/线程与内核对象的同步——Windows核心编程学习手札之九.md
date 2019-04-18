# 线程与内核对象的同步——Windows核心编程学习手札之九 - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年01月14日 17:58:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3396








# 线程与内核对象的同步

### ——Windows核心编程学习手札之九

用户方式下的线程同步机制具有速度快的特点，但有其局限性，对于许多应用程序来说，并不合适。例如，互锁函数家族只能在单值上运行，根本无法使线程进入等待状态；关键代码可以使线程进入等待状态，但只能用关键代码段对单个进程中的线程实施同步，使用关键代码段也容易陷入死锁，因为在等待进入关键代码段时无法设定超时值。内核方式下的线程同步机制适应性要优于用户方式下的机制，唯一不足的是内核对象机制的速度比较慢。内核对象机制下线程必须从用户方式转为内核方式，这个转换需要很大代价：往返一次需要占用X86平台上大约1000个CPU周期，当然还不包括执行内核方式代码，即实现线程调用的函数的代码所需时间。

对于线程同步来说，内核对象中的每种对象（包括进程、线程、作业）都可以说是处于已通知或未通知的状态之中，这种状态的切换是由Microsoft为每个对象建立的一套规则来决定。例如，进程内核对象总是未通知的状态中创建的，当进程终止运行时，操作系统自动使该进程的内核对象处于已通知状态，一旦进程内核对象得到通知，它将永远保持这种状态，它的状态永远不会改为未通知状态。当进程正在运行时，进程内核对象处于未通知状态，当进程终止运行时，就变为已通知状态。线程的内核对象如是，因此可以将相同方法应用于程序。下面的内核对象可以处于已通知状态或未通知状态：1）进程；2）文件修改通知；3）线程；4）事件；5）作业；6）可等待定时器；7）文件；8）信标；9）控制台输入；10）互斥对象。线程可以使自己进入等待状态，直到一个对象变为已通知状态。注意，用于控制每个对象的已通知/未通知状态的规则要根据对象类型而定。Windows提供专门实现线程同步的各种内核对象，如事件、等待计数器、信标和互斥对象。

等待函数可使线程资源进入等待状态，直到一个特定的内核对象变为已通知状态为止，这些等待函数中最常用的是WaitForSingleObject：

DWORD WaitForSingleObject(

HANDLE hObject,

DWORD dwMillisecondes)；

当线程调用该函数时，第一个参数hObject标识一个能够支持被通知/未通知的内核对象，第二个参数dwMilliseconds运行线程设置等待的时间长度。

函数WaitForMultipleObjects允许调用线程同时查看若干个内核对象的已通知状态：

DWORD WaitForMultipleObjects(

DWORD dwCount,

CONST HANDLE *phObjects,

BOOL fWailAll,

DWORD dwMilliseconds)；

该函数的参数dwCount用于指明想要让函数查看的内核对象的数量，这个值必须在1和MAXIMUN_WAIT_OBJECTS（在Windows头文件中定义为64）之间；参数phObjects参数是指向内核对象句柄的数组的指针。参数fWaitAll告诉函数是在指定内核对象中任何一个变为已通知状态还是在所有指定的内核对象都变为已通知状态下线程可运行，如fWaitAll传递了TRUE，那么在所有对象变为已通知前，该函数将不运行调用线程运行；dwMilliseconds与WaitForSingleObject作用一样，用于设置等待时长。

事件内核对象是最基本的对象，包含一个使用计数（与所有内核对象一样）、一个用于指明该事件是个自动重置的事件还是一个人工重置的事件的布尔值、用于指明该事件处于已通知状态还是未通知状态的布尔值。事件能够通知一个操作已经完成，有两种不同类型的事件对象，一种是人工重置的事件，另一种是自动重置的事件。当人工重置的事件得到通知时，等待该事件的所有线程均变为可调度的线程；当一个自动重置的事件得到通知时，等待该事件的线程中只有一个线程变为可调度的线程。当一个线程执行初始化操作，然后通知另一个线程执行剩余的操作时，事件使用得最多。事件初始化为未通知状态，在该线程完成它的初始化操作后，它将事件设置为已通知状态，这时，等待该事件的另一个线程发现该事件已经得到通知，此线程序就变成可调度线程。

创建事件内核对象：

HANDLE CreateEvent(

PSECURITY_ATTRIBUTES psa,

BOOL fManualReset,

BOOL fInitialState,

PCTSTR pszName);

其中，fManualReset参数是布尔值，告诉系统创建人工重置的事件（TRUE）或是自动重置事件（FALSE）；fInitialState参数用于指明该事件是要初始化为已通知状态（TRUE）还是未通知状态（FALSE）；当系统创建事件对象后，createEvent就将与进程相关的句柄返回给事件对象，其他进程中的线程可以获得对该对象的访问权，方法是使用在pszName参数中传递的相同值，使用继承性，使用DuplicateHandle函数等来调用CreateEvent，或者调用OpenEvent，在pszName参数中设定一个与调用createEvent时设定的名字相匹配的名字：

HANDLE OpenEvent(

DWORD fdwAccess,

BOOL fInherit,

PCTSTR pszName);

与其他内核对象一样，当不再需要事件内核对象时，应调用CloseHandle函数。

一旦事件已经创建，就可以直接控制它的状态，当调用SetEvent时，可以将事件改为已通知状态：BOOL SetEvent(HANDLE hEvent)；当调用ResetEvent(HANDLE hEvent)函数时，就可以将该事件改为未通知状态。函数BOOL PulseEvent(HANDLE hEvent)使得事件变为已通知状态，然后立即变为未通知状态，如调用SetEvent后又立即调用ResetEvent函数。

下面的代码例子说明了自动重置事件的用法，具体是实现了由主线程向子线程发起请求时设置事件未通知状态，子线程响应主线程序请求：

#include <process.h>

//this event is signaled when the client has a request for server

HANDLE g_hEventRequestSubmitted;

//this event is signaled when the server has a result for the client

HANDLE g_hEventResultReturned;

//the buffer shared between the client an server threads

TCHAR g_szSharedRequestAndResultBuffer[1024];

//server thread to terminate cleanly

TCHAR g_szServerShutdown[]=TEXT("Server Shutdown");



//this is the code executed by the server thread

UINT WINAPI ServerThread(PVOID pvParam)

{

//assume that the server thread is to run forever

BOOL fShutdown=FALSE;

while(!fShutdown)

{

//wait for the client to submit a request

WaitForSingleObject(g_hEventRequestSubmitted,INFINITE);

//check to see if the client wants the server to terminate

fShutdown=(lstrcmpi(g_szSharedRequestAndResultBuffer,g_szServerShutdown)==0);

if(!fShutdown)

{

//process the client's requet(reverse the string)

_tcsrev(g_szSharedRequestAndResultBuffer);

}



//let the client process the request's result

SetEvent(g_hEventResultReturned);

}

return 0;

}



void CEventDemoDlg::OnOK() 

{

// TODO: Add extra validation here

//create & initialize the two nonsignaled ,auto-reset events

g_hEventRequestSubmitted=CreateEvent(NULL,FALSE,FALSE,NULL);

g_hEventResultReturned=CreateEvent(NULL,FALSE,FALSE,NULL);



//spawn the server thread

UINT dwThreadID;

HANDLE hThreadServer=(HANDLE)_beginthreadex(NULL,0,ServerThread,NULL,0,&dwThreadID);



CString strReq;

GetDlgItemText(IDC_EditReq,strReq);

strcpy(g_szSharedRequestAndResultBuffer,strReq);



//let the server thread know that a request is ready in the buffer

SetEvent(g_hEventRequestSubmitted);



//wait for the server to process the request and give us the result

WaitForSingleObject(g_hEventResultReturned,INFINITE);

//let the user know the result

strReq.Format(_T("%s"), g_szSharedRequestAndResultBuffer); 

SetDlgItemText(IDC_EditRes,strReq);





//end 

lstrcpy(g_szSharedRequestAndResultBuffer,g_szServerShutdown);

SetEvent(g_hEventRequestSubmitted);

//wait for the server thread to acknowledge the shutdown and wait for the server thread to fully terminate

HANDLE h[2];

h[0]=g_hEventResultReturned;

h[1]=hThreadServer;

WaitForMultipleObjects(2,h,TRUE,INFINITE);

//properly clean up everything

CloseHandle(hThreadServer);

CloseHandle(g_hEventRequestSubmitted);

CloseHandle(g_hEventResultReturned);

//close the application

//CDialog::OnOK();

AfxMessageBox("close the application");

}

void CEventDemoDlg::OnOK() 是VC6.0中Dialog工程的一个按钮函数，在这里具体实现了事件的状态变化来同步线程的运行。

等待定时器内核对象是在某个时间或按规定的间隔时间发出自己的信号来通知，用在某个时间执行某个操作，其函数是：

HANDLE CreateWaitableTimer(

PSECURITY_ATTRIBUTES psa,

BOOL fManualReset,

PCTSTR pszName);

进程可以获得与自己相关的现有等待定时器的句柄，通过函数：

HANDLE OpenWaitableTimer(

DWORD dwDesiredAccess,

BOOL bInheritHandle,

PCTSTR pszName);

与事件内核对象一样，fManualReset参数用于指明人工重置的定时器或自动重置的定时器。当发出人工重置的定时器信号通知时，等待该定时器的所有线程均变为可调度的线程，当发出自动重置的定时器信号通知时，只有一个等待的线程变为可调度线程。

等待定时器总是在未通知状态中创建，必须调用SetWaitableTimer函数来告诉定时器使其变为已通知状态：

BOOL SetWaitableTimer(

HANDLE hTimer,

Const LARGE_INTEGER *pDueTime,

LONG lPeriod,

PTIMERAPCROUTINE pfnCompletionRoutine,

PVOID pvArgToCompletionRoutine,

BOOL fResume);

其中，参数hTimer用于指明所设置的定时器；PDueTime和lPeriod两个参数是一同使用的，PDueTime指明定时器何时应第一次报时，而lPeriod参数则指明此后定时器应间隔多长时间报时一次，下面代码用于将定时器的第一次报时设置在2009年1月8日的下午一点钟，然后每隔6小时报时一次：

//Declare our local variables.

HANDLE hTimer;

SYSTEMTIME st;

FILETIME ftLocal,ftUTC;

LARGE_INTEGER liUTC;

//create an auto-reset timer.

hTimer=CreateWaitableTimer(NULL,FALSE,NULL);

//first signaling is at January 8, 2009 at 1:00P.M. (local time).

st.wYear=2009;

st.wMonth=1;

st.wDayofWeek=0;

st.wDay=8;

st.wHour=13;

st.wMinute=0;

st.wSeconde=0;

st.wMilliseconds=0;

SystemTimeToFileTime(&st,&ftLocal);

//convert local time to UTC time.

LocalFileTimeToFileTime(&ftLocal,&ftUTC);

//convert FILETIME to LARGE_INTEGER because of different alignment.

liUTC.LowPart=ftUTC.dwLowDateTime;

liUIC.HighPart=ftUTC.dwHighDateTime;

//set the timer.

SetWaitableTimer(hTimer,&liUTC,6*60*60*1000,NULL,NULL,FALSE);

……

首先对SYSTEMTIME结构进行初始化，该结果用于指明定时器何时第一次报时（发出信号通知）。

信标内核对象用于对资源进行计数，与所有内核对象一样，包含一个使用数量，但也包含另外两个带符号的32位值，一个是最大资源数量，一个是当前资源数量，最大资源数量用于标识信标能够控制的资源的最大数量，而当前资源数量则用于标识当前可以使用的资源的数量。信标的使用规则是：1）如果当前资源的数量大于0，则发出信标信号；2）如果当前资源数量是0，则不发出信标信号；3）系统决不允许当前资源的数量为负值；4）当前资源数量决不能大于最大资源数量。当使用信标时，不要将信标对象的使用数量和它的当前资源数量混为一谈。创建信标内核对象的函数是：

HANDLE CreateSemaphore(

PSECURITY_ATTRIBUTE psa,

LONG lInitialCount,

LONG lMaximumCount,

PCTSTR pszName);

通过函数OpenSemaphore函数，其他进程可以获得现有信标有关的句柄：

HANDLE OpenSemaphore(

DWORD fdwAccess,

BOOL bInheritHandle,

PCTSTR pszName);

lMaximumCount参数用于告诉系统，应用程序处理的最大资源数量是多少，该参数是带符号的32位值，因此最多可以拥有2147483647个资源。lInitialCount参数用于指明开始时（当前）这些资源有多少可供使用。信标能以原子操作方式执行测试和设置操作，也就是说，当向信标申请一个资源时，操作系统就要检查是否有这个资源可供使用，同时将可用资源的数量递减，而不让另一个线程加以干扰，只有当资源数量递减后，系统才允许另一个线程申请对资源的访问权。通过调用ReleaseSemaphore函数，线程能够对信标的当前资源数量进行递增：

BOOL ReleaseSemaphore(

HANDLE hsem,

LONG lReleaseCount,

PLONG plPreviousCount);

该函数将lReleaseCount中的值添加给信标的当前资源数量。

互斥对象（mutex）内核对象能够确保线程拥有对单个资源的互斥访问权，包含一个使用数量，一个线程ID和一个递归计数器，互斥对象的行为特性和关键代码段相同，但是互斥对象属于内核对象，而关键代码段则属于用户方式对象，这意味着互斥对象的运行速度比关键代码段要慢，但也意味着不同进程的多个线程能够访问单个互斥对象，并且线程在等待访问资源时可以设定一个超时值。ID用于标识系统中的那个线程当前拥有互斥对象，递归计数器用于指明该线程拥有互斥对象的次数。互斥对象是常用内核对象之一，通常是用于保护由多个线程访问的内存块，互斥对象保证访问内存块的任何线程拥有对该内存块的独占访问权，这样可保证数据的完整性。互斥对象使用规则如下：

1）如果线程ID是0（这是无效的ID值），互斥对象不被任何线程所拥有，并且发出该互斥对象的通知信号；

2）如果ID是非0数字，那么一个线程就拥有互斥对象，并且不发出该互斥对象的通知信号；

3）与所有其他内核对象不同，互斥对象在操作系统中拥有特殊的代码，允许它们违反正常的规则；

若要使用互斥对象，需要调用CreateMutex来创建互斥内核对象：

HANDLE CreateMutex(

PSECURITY_ATTRIBUTES psa,

BOOL fInitialOwner,

PCTSTR pszName);

通过调用OpenMutex，另一个进程可以获得现有互斥对象相关的句柄：

HANDLE OpenMutex(

DWORD fdwAccess,

BOOL bInheritHandle,

PCTSTR pszName);

其中，参数fInitialOwner用于控制互斥对象的初始状态，如果传递FALSE（通常情况下传递的值），那互斥对象的ID和递归计数器被设置为0，这意味着该互斥对象没有被任何线程所拥有，因此要发出它的通知信号。如果fInitialOwner参数传递TRUE，那么该对象的线程ID被设置为调用线程的ID，递归计数器被设置为1，由于ID是个非0数字，因此该互斥对象开始时不发出通知信号。

对于互斥对象来说，正常的内核对象的已通知和未通知规则存在一个特殊的异常情况，比如说：一个线程试图等待一个未通知的互斥对象，在这种情况下，该线程通常被置于等待状态，然而系统要查看试图获取互斥对象的线程ID是否与互斥对象中记录的线程ID相同，如果相同，即使互斥对象处于未通知状态，系统也允许线程保持可调度状态，这种“异常”行为特性不适用于系统中的其他内核对象。每当线程成功地等待互斥对象时，该对象的递归计数器就递增，若要使递归计数器的值大于1，唯一的方法是线程多次等待相同的互斥对象，以便利用这个异常规则。一旦线程成功等待到一个互斥对象，该线程就知道它已经拥有对受保护资源的独占访问权，试图访问该资源的任何其他线程（通过等待相同的互斥对象）均被置于等待状态中。当目前拥有对资源访问权的线程不再需要它的访问权时，需要调用ReleaseMutex函数来释放该互斥对象：

BOOL ReleaseMutex(HANDLE hMutex);

该函数将对象递归技数器递减1，如果线程多次成功等待一个互斥对象，在互斥对象的递归计数器变成0之前，该线程必须以同样次数调用ReleaseMutex函数，当递归计数器到达0时，该线程ID也被置为0，同时该对象变为已通知状态。当一个线程调用ReleaseMutex函数时，函数要查看调用线程的ID是否与互斥对象中的线程ID相匹配，如相同，递归计数器就递减，如不匹配，那函数将不进行任何操作，而是将FALSE（表示失败）返回给调用者，此时调用GetLastError，将返回ERROR_NOT_OWNER（试图释放不是调用者拥有的互斥对象）。如在释放互斥对象之前，拥有互斥对象的线程终止运行（使用ExitThread、TerminateThread、ExitProcess或TerminateProcess函数），系统将该互斥对象视为已经被放弃——拥有互斥的线程决不会释放它，因为该线程已经终止运行。

总结下线程同步对象（内核对象）与线程同步之间的相互关系：

1）对象：进程，何时处于未通知状态：当进程仍然活动时，何时处于已通知状态：当进程终止运行时（ExitProcess,TerminateProcess），成功等待的副作用：无；

2）对象：线程，何时处于未通知状态：当线程仍然活动时，何时处于已通知状态：当线程终止运行时（ExitThread,TerminateThread），成功等待的副作用：无；

3）对象：作业，何时处于未通知状态：当作业的时间尚未结束时，何时处于已通知状态：当作业的时间结束时，成功等待的副作用：无；

4）对象：文件，何时处于未通知状态：当I/O请求正在处理时，何时处于已通知状态：当I/O请求处理完毕时，成功等待的副作用：无；

5）对象：控制台输入，何时处于未通知状态：不存在任何输入，何时处于已通知状态：当存在输入时，成功等待的副作用：无；

6）对象：文件修改通知，何时处于未通知状态：没有任何文件被修改，何时处于已通知状态：当文件系统发现修改时，成功等待的副作用：重置通知；

7）对象：自动重置事件，何时处于未通知状态：ResetEvent,Pulse-Event或等待成功，何时处于已通知状态：当调用SetEvent/PulseEvent时，成功等待的副作用：重置事件；

8）对象：人工重置事件，何时处于未通知状态：ResetEvent,Pulse-Event，何时处于已通知状态：当调用SetEvent/PulseEvent时，成功等待的副作用：无

9）对象：自动重置等待定时器，何时处于未通知状态：CancelWaitableTimer或等待成功，何时处于已通知状态：当时间到时（SetWaitableTimer），成功等待的副作用：重置定时器；

10）对象：人工重置等待定时器，何时处于未通知状态：CancelWaitableTimer，何时处于已通知状态：当时间到时（SetWaitableTimer），成功等待的副作用：无；

11）对象：信标，何时处于未通知状态：等待成功，何时处于已通知状态：当数量>0时（ReleaseSemaphore），成功等待的副作用：数量递减1；

12）对象：互斥对象，何时处于未通知状态：等待成功，何时处于已通知状态：当未被线程拥有时（ReleaseMutex），成功等待的副作用：将所有权赋予线程；

13）对象：关键代码段（用户方式），何时处于未通知状态：等待成功（（Try）EnterCriticalSection），何时处于已通知状态：当未被线程拥有时（LeaveCriticalSection），成功等待的副作用：将所有权赋予线程。

互锁（用户方式）函数决不会导致线程变为非调度状态，它们会改变一个值并立即返回。

线程同步函数WaitForSingleObject和WaitForMultipleObjects是使用最多的函数，不过，Windows还提供了其他函数。异步设备I/O使得线程能够启动一个读操作或写操作，但是不必等待读操作或写操作完成。例如，如果线程需要将一个大文件装入内存，那该线程可以告诉系统将文件装入内存，在系统加载该文件时，线程可以执行其他任务，如创建窗口、对内部数据结构进行初始化等等，当初始化操作完成时，线程可以终止自己的运行，等待系统通知文件已经读取。设备对象是可以同步的内核对象，这意味着可以调用WaitForSingleObject函数，传递文件、套接字和通信端口的句柄。当系统执行异步I/O时，设备对象处于未通知状态，一旦操作完成，系统就将对象的状态改为已通知状态，这样，线程就知道操作已经完成，此时线程可继续运行。线程可以调用WaitForInputIdle来终止自己的运行：

DWORD WaitForInputIdle(

HANDLE hProcess,

DWORD dwMilliseconds);

该函数一直处于等待状态，知道hProcess标识的进程在创建应用程序的第一个窗口的线程中已经没有尚未处理的输入为止。

线程可以调用MsgWaitForMultipleObjects或MsgWaitForMultipleObjectsEx函数，让线程等待它自己的消息：

DWORD MsgWaitForMultipleObjects(

DWORD dwCount,

PHANDLE phObjects,

BOOL fWaitAll,

DWORD dwMilliseconds,

DWORD dwWakeMask);

DWORD MsgWaitForMultipleObjects(

DWORD dwCount,

PHANDLE phObjects,

BOOL fWaitAll,

DWORD dwMilliseconds,

DWORD dwWakeMask,

DWORD dwFlags);

这些函数与WaitForMultipleObjects函数十分相似，差别在于它们允许线程在内核对象变成已通知状态或窗口消息需要调度到调用线程创建的窗口中时被调度。

线程同步函数还有WaitForDebugEvent函数：当调试程序启动运行时，它将自己附加给一个被调试程序；SingleObjectAndWait函数用于在单个原子方式的操作中发出关于内核对象的通知并等待另一个内核对象。

如非

2009-1-15



