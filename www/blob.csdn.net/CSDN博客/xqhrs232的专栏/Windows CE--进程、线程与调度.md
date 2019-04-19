# Windows CE--进程、线程与调度 - xqhrs232的专栏 - CSDN博客
2014年03月08日 10:26:15[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1260
原文地址::[http://blog.sina.com.cn/s/blog_672af14301016pka.html](http://blog.sina.com.cn/s/blog_672af14301016pka.html)
相关文章
1、[wince多线程调度](http://blog.donews.com/staniel/archive/2006/01/18/700445.aspx)----[http://blog.donews.com/staniel/archive/2006/01/18/700445.aspx](http://blog.donews.com/staniel/archive/2006/01/18/700445.aspx)
2、Windows CE系统机制中的调度----[http://bbs.csdn.net/topics/310092589](http://bbs.csdn.net/topics/310092589)
3、wince进程和线程与调度----[http://wenku.baidu.com/view/4000de48f7ec4afe04a1dfcc.html](http://wenku.baidu.com/view/4000de48f7ec4afe04a1dfcc.html)
# Windows CE系统机制中的调度

**进程**
Windows CE最多只支持32个进程同时运行，每个进程占据32MB的虚拟地址空间（也叫做Slot）。在系统启动时，filesys.exe，gwes.exe，device.exe已经占据了多个slot，因此，用户实际可用的进程数不到30个。因此，在构建复杂的应用程序的时候，如果需要同时处理多个任务，应该尽量选择多线程解决方案而不是采用多进程。
此外，Windows CE的进程也不支持环境变量（environment variable）和当前目录。如果在文件的前面不指明路径，Windows CE会先查找”\Windows”目录，然后查找根目录，并不会查找当前目录。查找当前目录首先使用GetModuleFileName()函数得到当前执行文件所在的目录，然后再把要打开的文件拼接到路径中得到完整的路径，参考代码如下：
TCHAR szBuf[MAX_PATH];
DWORD dwPathLen;
TCHAR pStr = NULL;
//得到当前程序可执行文件的完整路径
dwPathLen = GetModuleFileName(NULL, szBuf, MAX_PATH)
if (! dwPathLen)
{
return -1;
}
//除去文件名
While (szBuf[--dwPathLen]!=’\\’)
;
//拼接文件
_tsscat (szBuf, TEXT(“MyFile.txt”));
//如果当前文件位于/Temp，则此时szBuf的内容为”/Temp/MyFile.txt”
用CreateProcess()函数创建一个新的进程，函数原型如下:
BOOL CreatProcess(
LPCWSTR lpszImageName,                               //可执行文件的路径和名字
LPCWSTR lpszCmdLine,                                   //命令行参数
LPSECURITY_ATTRIBUTES lpsaProcess,      //不支持，设置为NULL
LPSECURITY_ATTRIBUTES lpsaThread,              //不支持，设置为NULL
BOOL fInheritHandles,                                       //不支持，设置为NULL
DWORD fdwCreate,                                                 //控制进程创建的附加参数
LPVOID lpvEnvironment,                                  //不支持，设置为NULL
LPWSTR lpszCurDir,                                          //不支持，设置为NULL
LPSTARTUPINFFOW lpsiStartInfo,                 //不支持，设置为NULL
LPPOCESS_INFORMATION lppiProInfo        //返回的进程相关的信息
);
如果进程创建成功，函数返回非0值，如果失败，函数返回FALSE。因为Windows CE并不支持安全和当前目录，也不出来句柄继承，大多数参数必须被设置为NULL或0。如果把所有不支持的参数都去除，函数原型为;
BOOL CreatProcess(
LPCWSTR  lpszImageName,
LPCWSTR  lpszCmdLine,  NULL, NULL, FALSE,
DWORD   fdwCreate,  NULL, NULL, NULL
LPPOCESS_INFORMATION  lppiProInfo)
第一个参数是可执行文件的路径和名称，如果没有明确指定可执行文件的路径，Windows CE会按照如下的顺序搜索:
Windows目录(“\Windows”);
对象存储的根目录(“\”)
OEM所指定的搜索目录(在系统注册表的”HKEY_LOCAL_MACHINE\Loader\SystemPath”下添加，这是一个Multistring的值，因此可以添加多个搜索路径。)
fdwCreate参数指定进程加载后的初始状态：参数如下表所示
|标记|描述|
|----|----|
|0|创建一个常规的进程|
|CREATE_SUSPENDED|进程的主线程初始状态为挂起|
|DEBUG_PROCESS|创建呗调用该API的进程调试的进程|
|DEBUG_ONLY_THIS_PROCESS|创建的进程被调用进程调试，但是该进程所创建的任何子进程都不被调试，这个参数必须与DEBUG_PROCESS同时使用|
|CREAT_NEW_CONSOLE|创建命令行程序|
CreatProcess()函数的最后一个参数lppiProcInfo指向一个PROCESS_INFORMATION结构体，PROCESS_INFORMATION结构体返回进程和主线程的句柄以及ID，如果不希望得到这些信息，也可以把这个参数设置为空。
线程
HANDLE CreateThread(
LPSECURITY_ATTRIBUTES lpsa,   //不支持，设为NULL
DWORD dwStackSize,                     //线程栈的大小，通常被忽略而使用默认值
LPTHREAD_START_ROUTINE lpStartAddr,      //指向线程的执行函数的指针
LPVOID lpvThreadParam,                   //用来为线程传递一个应用程序自定义的值
DWORD dwCreateFlags,                    //控制线程创建的附加参数
LPDWORD lpThreadId);                      //返回新创建线程的ID
如果线程创建成功，函数返回新创建线程的句柄。否则函数返回 NULL。可以把 fdwCreate
参数设置为 CREATE_SUSPENDED 来创建一个起始状态为挂起的线程，否则线程创建结束
后就会立即执行。
如果要结束线程，最好的方法是从线程的执行函数返回。此外，Windows CE 还提供
ExitThread()和 TerminateThread()函数使线程结束执行。
调度
Windows CE 是一个抢占式多任务（Preemptive Multitasks）操作系统。调度程序使用基
于优先级的时间片算法对线程进行调度。
驱 动 程 序 和 应 用 程 序 可 以 使 用 CeGetThreadPriority() 和CeSetThreadPriority()函数来获取和设置线程的优先级
GetThreadPriority()和 SetThreadPriority()函数：
Windows CE 3.0 版本之后为了与以前的版本向下兼容，也提供 GetThreadPriority()和
SetThreadPriority()两个函数。这两个函数也能用来得到和设置线程的优先级。但是使用这两
个 函 数 设 置 的 优 先 级 只 能 是 248 到 255 之 间 的 某 个 值 。 在 函 数 参 数 中 以
THREAD_PRIORITY_TIME_CRITICAL 和 THREAD_PRIORITY_NORMAL 等宏表示。
在线程获得处理器之后，会执行特定的一段时间然后重新调度，这段时间称作时间片大
小（quantum）。每个线程都有一个时间片大小，默认的时间片大小是 100 毫秒，OEM 可以
在内核初始化的时候改变这个值的大小。分配给线程的时间片大小可以用
CeGetThreadQuantum()函数获得，这个函数接受线程的句柄作为参数，返回线程的时间片大
小。
线程的状态可以有以下几种：
运行（Running）：线程正在处理器上执行。
就绪（Ready）：线程可以执行，但是此刻没有占有处理器。如果就绪的线程被调度
程序选中，则占有处理器就进入运行状态。
挂起（ Suspended）：创建线程时指定了 CREATE_SUSPENDED 参数或者调用
SuspendThread 函数都可以导致线程挂起。挂起的线程不能占有处理器。每个线程都
有一个挂起计数，SuspendThread 函数用来增加挂起计数，ResumeThread 函数使线
程的挂起计数减 1。当线程的挂起计数为 0 时，线程转入就绪态。
睡眠（Sleeping）：调用 Sleep 函数可以使线程进入睡眠状态，处于睡眠状态的线程
不能占有处理器。当睡眠时间结束后，线程转入就绪态。
阻塞（Blocked）：如果线程申请的共享资源暂时无法获得，线程就进入阻塞状态，
处于阻塞状态的线程不能占有处理器。当共享资源可以获得后，线程转入就绪态。
终止（Terminated）：线程运行结束。
Sleep(0)                   // 使当前线程放弃剩余的时间片
Sleep(n)                   // 使当前线程睡眠 n 毫秒
Sleep(INFINITE)            // 使线程一直睡眠直到线程被终止/唤醒
SleepTillTick              // 使线程睡眠直到下一个时钟嘀嗒（tick）
SuspendThread              // 增加线程的挂起计数
ResumeThread               // 减少线程的挂起计数
//======================================================================================================================
备注：：
1》wince默认的线程量是100ms线程量是线程在一个时间片的持续时间。可以通过CeSetThreadQuantum函数修改。也可以修改dwDefaultThreadQuantum变量。如果要求实时性强可以把线程量设置小一点。------这个还真没实验过，不知道真假！！！
2》WINCE下确实有dwDefaultThreadQuantum变量和CeSetThreadQuantum，CeGetThreadQuantum函数，WINCE50下在Schedule.cd:\wince500\private\winceos\coreos\nk\Kernel；WINCE60下在Schedule.cd:\wince600\updates\backup\121231_2012\private\winceos\coreos\nk\Kernel。WINCE60打完12年年包就没了？！
3》WINCE下**默认的时间片是**** 100****毫秒，****OEM****可以在****OAL****中重新****设置-----这个还不知道怎么去设置！！！**
