# 线程的调度、优先级和亲缘性——Windows核心编程学习手札系列之七 - 专注于数据挖掘算法研究和应用 - CSDN博客





2008年12月22日 17:36:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3418








# 线程的调度、优先级和亲缘性

### ——Windows核心编程学习手札系列之七

每个线程都拥有一个上下文结构，在线程的内核对象中，记录线程上次运行时该线程的CPU寄存器状态。Windows会每隔20ms左右查看当前存在的所有线程内核对象，在这些对象中，选择可调度的对象，将其上下文结构（内核对象中）加载到CPU的寄存器中，其值就是上次保存在线程环境中的值，此为上下文切换。Windows保存了一个记录，说明每个线程获得调度的机会，Microsoft的Spy++工具，可以查看这个。操作系统只调度可以调度的线程，实际中，大多数线程是不可调度的，如暂停的线程（CREATE_SUSPENDED标志）以及等待事件触发的线程等。

线程内核对象的内部有线程的暂停计数值，当调用CreateProcess或CreateThread函数时，就创建了线程的内核对象，且它的暂停计数被初始化为1，防止线程被调度到CPU中，因为线程初始化需要时间，需要在准备好开始执行线程。当线程完全初始化后，CreateProcess或CreateThread要查看是否传递了CREATE_SUSPENDED标志，如果传递了给标志，那函数返回，新线程处于暂停状态；如未传递该标志，那函数将线程的暂停计数递减为0，此时如果线程没有等待事件，那么该线程处在可调度的状态。在暂停状态中创建一个线程，就可以在线程有机会执行前改变线程的运行环境（如优先级）。要恢复线程的可调度性，可调用函数ResumeThread，将调用CreateThread函数时返回的线程句柄传递给它。DWORD ResumeThread(HANDLE hThread)运行成功将返回线程的前一个暂停计数，否则返回0xFFFFFFFF值，单个线程可以暂停若干次，如一个线程序暂停了3次需要恢复3次才可以被分配给一个CPU。创建线程，除传递CREATE_SUSPENDED标志外，还可以调用DWORD SuspendThread(HANDLE hThread)函数来暂停线程的运行。任何线程都可以调用该函数来暂停另一个线程的运行（只要有线程的句柄），线程可以自行暂停运行，但不能自行恢复运行。与ResumeThread一样，SuspendThread返回的是线程的一个暂停计数，暂停计数最多是MAXIMUM_SUSPEND_COUNT次（在WinNT.h中定义为127），SuspendThread与内核方式是异步运行的，但在线程恢复运行之前，不会发生用户方式的执行。使用SuspendThread函数暂停线程应该在确切知道目标线程正在做什么情况下，并采取措施避免因暂停线程的运行而带来的问题或死锁，因为如果线程正试图从堆栈中分配内存，该线程会在该堆栈上设置锁，当其他线程访问该堆栈时将被停止，直到暂停线程的恢复。

Windows中不存在暂停或恢复进程的概念，允许一个进程暂停另一个进程中所有线程的运行，但从事暂停操作的进程必须是个调试程序，也需要调用WaitForDebugEvent和ContinueDebugEvent之类的函数。总言，Windows没有提供方法暂停进程中的所有线程运行。这里有一个作者写的暂停进程函数作为要暂停进程的参考用。

Void SuspendProcess(DWORD dwProcessID,BOOL fSuspend){

//get the list of threads in the system

HANDLE hSnapshot=CreateToolhelp32Snapshot(

TH32CS_SNAPTHREAD,dwProcessID);

if(hSnapshot != INVALID_HANDLE_VALUE){

//walk the list of threads

THREADENTRY32 te={sizeof(te)};

BOOL fOk=Thread32First(hSnapshot,&te);

for(;fOk;fOk=Thread32Next(hSnapshot,&te)){

//Is this thread in the desired process?

if(te.th32OwnerProcessID = = dwProcessID){

//attempt to convert the thread ID into a handle

HANLDE hThread=OpenThread(THREAD_SUSPEND_RESUME,FALSE,

te.th32ThreadID);

if(hThread!=NULL){

//suspend or resume the thread

if(fSuspend)

SuspendThread(hThread);

Else

ResumeThread(hThread);

}

CloseHandle(hThread);

}

}

CloseHandle(hSnapShot);

}

}

该函数使用ToolHelp函数枚举系统中的线程列表，当找到作为指定进程的组成部分的线程时，就调用HANDLE OpenThread(DWORD dwDesiredAccess,BOOL bInheritHandle,DWORD dwThreadID)函数找出匹配线程ID的线程内核对象，对内核对象的使用计数进行递增，然后返回对象的句柄，运用这个句柄可调用SuspendThread或ResumeThread来暂停或恢复线程的运行。

如果线程不想爱某个时间段被调度，可以调用Sleep函数来实现：void sleep(DWORD dwMillisecondes)，这个函数使线程暂停自己运行，知道dwMillisecondes后，该函数需要注意的是：1）调用sleep函数，使线程自愿放弃它的剩余时间片；2）系统将在大约的指定毫秒内使线程不可调度；3）可为sleep函数的参数dwMillisecondes传递INFINITE，告诉系统永不调度该线程，这不提倡，最好让线程退出，还原其堆栈和内核对象；4）可将0传递给sleep，这样调用线程将释放剩余时间片，并迫使系统调度另一个线程。

函数BOOL SwitchToThread()可转换到另一个线程。当调用这个函数时，系统查看是否存在一个迫切需要CPU时间的线程，如没有，SwitchToThread就会立即返回，如果存在，SwitchToThread就对该线程进行调度。这个函数允许一个需要资源的线程强制另一个优先级较低、而目前却拥有该资源的线程放弃该资源。如果调用SwitchToThread函数时没有其他线程能够运行，则返回FALSE，否则返回非0值。与sleep函数相似，区别在于SwitchToThread允许优先级较低的线程运行，即使低优先级线程迫切需要CPU时间，sleep也能够立即对调用线程重新进行调度。

要获取线程的运行时间需要调用GetThreadTimes函数，这个返回线程得到的CPU时间数量。具体实现代码如下：

#include <windows.h>

<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />



__int64 FileTimeToQuadWord(PFILETIME pft)

{

return(Int64ShllMod32(pft->dwHighDateTime,32) | pft->dwLowDateTime);

}





int main(int argc, char* argv[])

{

FILETIME ftKernelTimeStart,ftKernelTimeEnd;

FILETIME ftUserTimeStart,ftUserTimeEnd;

FILETIME ftDummy;

__int64 qwKernelTimeElapsed,qwUserTimeElapsed,qwTotalTimeElapsed;





//Get start times

GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&ftKernelTimeStart,&ftUserTimeStart);





//perform complex algorithm here.

//Get ending times.

GetThreadTimes(GetCurrentThread(),&ftDummy,&ftDummy,&ftUserTimeEnd,&ftUserTimeEnd);





//Get the elapsed kernel and user times by converting the start and end times form FILETIMEs

//to quad words and then subtract the start times from the end times.

qwKernelTimeElapsed=FileTimeToQuadWord(&ftKernelTimeEnd)-FileTimeToQuadWord(&ftKernelTimeStart);





qwUserTimeElapsed=FileTimeToQuadWord(&ftUserTimeEnd)-FileTimeToQuadWord(&ftUserTimeStart);





//Get total time duration by adding the kernel and user times.

qwTotalTimeElapsed=qwKernelTimeElapsed+qwUserTimeElapsed;





// the total elapsed time is in qwTotalTimeElapsed and display in console

printf("The executing times of thread is %d /n",qwTotalTimeElapsed);

//printf("Hello World!/n");

return 0;

}

环境结构使系统保留线程的状态，在下次线程拥有CPU时，能够回到上次中断运行的地方。Windows允许查看线程内核对象的内部情况，以便抓取它当前的一组CPU寄存器，若要执行该项操作，可调用GetThreadContext函数：

BOOL GetThreadContext(HANDLE hThread,PCONTEXT pContext);

调用该函数，只需指定一个CONTEXT结构，对某些标志（该结构中的ContextFlags成员）进行初始化，指明想要收回那些寄存器，并将该结构的地址传递给函数，函数会将数据填入到所要求的成员中。在调用GetThreadContext函数前，应调用SuspendThread，否则线程可能被调度，且线程的环境与所收回的不同。一个线程实际有两个环境，一个是用户方式，一个是内核方式。GetThreadContext只能返回线程的用户方式环境，如调用SuspendThread来停止线程的运行，但该线程目前正运行在内核方式下，那么即使SuspendThread尚未暂停该线程的运行，它的用户方式仍然处于稳定状态。线程在恢复用户方式之前，无法执行更多的用户方式代码，因此可放心将线程视为处于暂停状态，GetThreadContext函数将能正常运行。CONTEXT结构的ContextFlags成员并不与任何CPU寄存器对应。无论是何种CPU结构，该成员存在于CONTEXT结构定义中。ContextFlags成员用于向GetThreadContext函数指明想要检索那些寄存器。如想获得线程的控制寄存器，可以用如下代码：

//Create a CONTEXT structure.

CONTEXT Context;

//Tell the system that we are interested in only the control registers.

Context.ContextFlags=CONTEXT_CONTROL;

//Tell the system to get the registers associated with a thread.

GetThreadContext(hThread,&Context);

在调用GetThreadContext之前，须对CONTEXT结构中的ContextFlags成员进行初始化，如想获得线程的控制寄存器和整数寄存器，需要进行下面的ContextFlags初始化：

Context.ContextFlags=CONTEXT_CONTROL | CONTEXT_INTEGER;

也可以获得线程的所有重要的寄存器（Mcirosoft认为最常用的寄存器）：

Context.ContextFlags=CONTEXT_FULL;

当GetThreadContext返回时，可容易查看线程的任何寄存器值，要编写与CPU相关的代码。Windows可修改CONTEXT结构中的成员，然后通过SetThreadContext将新寄存器值放回线程的内核对象中：

BOOL SetThreadContext(HANDLE hThread,CONST CONTEXT *pContext);

修改其环境的线程前应先暂停线程，否则结果不得而知。下面的代码是演示：

//Create a CONTEXT structure.

CONTEXT Context;

//stop the thead from running

SuspendThread(hThread);



//Get the thread's control registers.

Context.ContextFlags=CONTEXT_CONTROL;

//Tell the system to get the registers associated with a thread.

GetThreadContext(hThread,&Context);





//Make the instruction pointer point to the address of your choice.

//Here I've arbitrarily set the address instruction pointer to 0x00010000

#if defined(_ALPHA_)

Context.Fir=0x00010000;

#elif defined(_X86_)

Context.Eip=0x00010000;

#else

#error Module contains CPU-specific code;modify and recompile.

#endif





//Set the thread's registers to reflect the changed values.

//It's not really necessary to reset the ControlFlags memeber because it was set earlier.

Context.ContextFlags=CONTEXT_CONTROL;

SetThreadContext(hThread,&Context);





//Resuming the thread will cause it to begin execution at address 0x00010000.

RusumeThread(hThread);

如此处理，可能导致远程线程中的违规，向用户显示未处理的异常消息框，同时远程进程终止运行。GetThreadContext和SetThreadContext函数可以对线程进行多方面控制，但要慎用。

线程被赋予不同的优先级，决定系统调度程序选择调度哪个线程来运行（使其拥有CPU）。每个线程都被赋予一个从0（最低）到31（最高）的优先级号码。当系统引导时，会创建一个特殊的线程，称为0页线程，该线程被赋予优先级为0，是整个系统中唯一的一个在优先级0上运行的线程。当系统中没有任何需要执行操作时，0页线程负责将系统中的所有空闲RAM页面置0。Windows支持6个优先级类：空闲、低于正常、正常、高于正常、高和实时，一般程序都处在正常这个级别。Windows Explorer是在高优先级上运行的，大多数时间Explorer线程是暂停的，等待用户按下操作键或点击鼠标按照时被唤醒。当Explorer的线程处于暂停状态时，系统不分配CPU给它的线程，这样次优先级的线程可以得到调度。但一旦用户有按键操作，系统就会唤醒Explorer线程，如果低优先级线程正在运行，系统会立即抢在这些线程之前让Explorer的线程优先运行。应该避免使用实时这个最高的优先级类，因为它可能干涉操作系统任务的运行，可能阻止必要的磁盘I/O信息和网络信息的产生。

当调用CreateProcess时，fdwCreate参数可以传递需要的优先级类。可通过调用SetPriorityClass来改变优先级类：

BOOL SetPriorityClass(HANDLE hProcess,DWORD fdwPriority);

该函数将hProcess标识的优先级改为fdwPriority参数中设定的值。由于该函数带有一个进程句柄，所以只要拥有进程的句柄和足够的访问权，就可以改变系统中运行的任何进程的优先级类。检索进程的优先级类函数：DWORD GetPriorityClass(HANDLE hProcess)。如果使用Start命令来启动程序，可以使用一个开关来设定应用程序的起始优先级，如在命令外壳输入如下命令可使系统启动Calculator，并在开始时按空闲优先级来运行它：

C:/>START /LOW CALC.EXE

Start命令还能识别/BELOWNORMAL、/NORMAL、/ABOVENORMAL、/HIGT和/REALTIME等开关，以便按它们各自的优先级启动执行一个应用程序。当然，一旦应用程序启动运行，可以通过调用SetPriorityClass函数改变自己的优先级。

当系统将线程分配给处理器时，Windows2000使用软亲缘性来进程操作，这意味着如果所有其他因素相同的话，它将设法在它上次运行的哪个处理器上运行线程，让线程留在单个处理器上，有助于重复使用仍然在处理器的内存高速缓存中的数据。Windows2000允许设置进程和线程的亲缘性，可控制哪个CPU运行某些线程，称为硬亲缘性。计算机在引导时，要确定机器中有多少个CPU可供使用。通过调用GetSystemInfo函数，应用程序可查询机器中的CPU数量。按照默认设置，任何线程都可以调度到这些CPU中的任何一个上去运行。为限制在可用CPU的子集上运行的单个进程中的线程数量，可调用：

BOOL SetProcessAffinityMask(HANDLE hProcess,DWORD_PTR dwProcessAffinityMask);

第一个参数hProcess用于指明影响的是哪个进程；第二个参数dwProcessAffinityMask是位屏蔽，用于指明线程可以在那些CPU上运行，如传递0x00000005（二进制0101，0和2位是真值）表示该进程中的线程可以在CPU0和CPU2上运行，但是不能爱CPU1和CPU3至31上运行。子进程可以继承进程的亲缘性。同时可通过下面函数返回进程的亲缘性屏蔽：

BOOL GetProcessAffinityMask(HANDLE hProcess,

PDWORD_PTR pdwProcessAffinityMask,

PDWORD_PTR pdwSystemAffinityMask);

传递亲缘性屏蔽的进程句柄，函数将填入pdwProcessAffinityMask变量，同时返回系统的亲缘性屏蔽（pdwSystemAffinityMask指向的变量中）。系统的亲缘性屏蔽用于指明系统的那个CPU能够处理线程，进程的亲缘性始终是一个系统的亲缘性屏蔽的正确子集。上面谈到的是将进程的多个线程限制到一组CPU上运行，那么同样可以设置将进程中的一个线程限制到一组CPU上去运行。如包含4个线程的进程，在拥有4个CPU的计算机上运行，如为线程中的一个线程（正在执行非常重要的操作）增加某个CPU始终供它使用，则需要对其他三个线程限制不能在CPU0上运行，而只能在CPU1、CPU2、CPU3上运行。

通过调用SetThreadAffinityMask函数，能为各个线程设置亲缘性屏蔽：

DWORD_PTR SetThreadAffinityMask(HANDLE hThread,

DWORD_PTR dwThreadAffinityMask);

函数中hThread参数用于指明要限制的线程，dwThreadAffinityMask用于指明线程能够运行在那个CPU上，dwThreadAffinityMask是进程亲缘性的相应子集，返回值是线程的前一个亲缘性屏蔽。上面例子中将3个线程限制在CPU1、CPU2、CPU3上运行的代码：

//Thread 0 can only on CPU0.

SetThreadAffinityMask(hThread0,0x00000001);

//Threads1/2/3 run on CPUs 1/2/3

SetThreadAffinityMask(hThread1,0x0000000E);

SetThreadAffinityMask(hThread2,0x0000000E);

SetThreadAffinityMask(hThread3,0x0000000E);

如非 <?xml:namespace prefix = st1 ns = "urn:schemas-microsoft-com:office:smarttags" />2008-12-22



