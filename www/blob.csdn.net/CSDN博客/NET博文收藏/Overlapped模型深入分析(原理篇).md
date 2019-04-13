
# Overlapped模型深入分析(原理篇) - .NET博文收藏 - CSDN博客


2008年06月10日 12:52:00[hejishan](https://me.csdn.net/hejishan)阅读数：1458标签：[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[数据结构																](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[asynchronous																](https://so.csdn.net/so/search/s.do?q=asynchronous&t=blog)[callback																](https://so.csdn.net/so/search/s.do?q=callback&t=blog)[工作																](https://so.csdn.net/so/search/s.do?q=工作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=callback&t=blog)[
																															](https://so.csdn.net/so/search/s.do?q=asynchronous&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=file&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=file&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=null&t=blog)
Overlapped I/O模型深入分析
简述：
Overlapped I/O也称Asynchronous  I/O，异步I/O模型。异步I/O和同步I/O不同，同步I/O时，程序被挂起，一直到I/O处理完，程序才能获得控制。异步I/O，调用一个函数告诉OS，进行I/O操作，不等I/O结束就立即返回，继续程序执行，操作系统完成I/O之后，通知消息给你。Overlapped I/O只是一种模型，它可以由内核对象(hand)，事件内核对象(hEvent), 异步过程调用(apcs) 和完成端口(I/O completion)实现。
Overlapped I/O的设计的目的：
取代多线程功能，（多线程存在同步机制，错误处理，在成千上万个线程I/O中，线程上下文切换是十分消耗CPU资源的）。
Overlapped I/O模型是OS为你传递数据，完成上下文切换，在处理完之后通知你。由程序中的处理，变为OS的处理。内部也是用线程处理的。
Overlapped数据结构：
typedef struct _OVERLAPPED {
DWORD   Internal;通常被保留，当GetOverlappedResult()传回False并且GatLastError()并非传回ERROR_IO_PENDINO时，该状态置为系统定的状态。
DWORDInternalHigh;通常被保留，当GetOverlappedResult()传回False时，为
被传输数据的长度。
DWORDOffset;指定文件的位置,从该位置传送数据,文件位置是相对文件开始
处的字节偏移量。调用 ReadFile或WriteFile函数之前调用进
程设置这个成员，读写命名管道及通信设备时调用进程忽略这
个成员;
DWORDOffsetHigh;指定开始传送数据的字节偏移量的高位字,读写命名管道及通
信设备时调用进程忽略这个成员;
HANDLE hEvent;标识事件,数据传送完成时把它设为信号状态,调用ReadFile
WriteFile   ConnectNamedPipe   TransactNamedPipe函数
前,调用进程设置这个成员. 相关函数
CreateEvent  ResetEvent   GetOverlappedResult
WaitForSingleObject   CWinThread   GetLastError
} OVERLAPPED, *LPOVERLAPPED;
二个重要功能：
1．标识每个正在overlapped 的操作。
2．程序和系统之间提供了共享区域。参数可以在区域内双向传递。
OVERLAPPED和数据缓冲区释放问题:
在请求时，不能释放，只有在I/O请求完成之后，才可以释放。如果发出多个overlapped请求，每个overlapped读写操作，都必须包含文件位置（socket），另外，如果有多个磁盘，I/O执行次序无法保证。（每个overlapped都是独立的请求操作）。

**内核对象****(hand)实现：**
例子：用overlapped模型读一个磁盘文件内容。
1．把设备句柄看作同步对象，ReadFile将设备句柄设为无信号。ReadFile 异步I/O字节位置必须在OVERLAPPED结构中指定。
2．完成I/O，设置信息状态。为有信号。
3．WaitForSingleObject或WaitForMultipleObject判断
或者异步设备调用GetOverLappedResult函数。
int main()
{
BOOL rc;
HANDLE hFile;
DWORD numread;
OVERLAPPED overlap;
char buf[READ_SIZE];
char szPath[MAX_PATH];
CheckOsVersion();
GetWindowsDirectory(szPath, sizeof(szPath));
strcat(szPath, "//WINHLP32.EXE");
hFile = CreateFile( szPath,
GENERIC_READ,
FILE_SHARE_READ|FILE_SHARE_WRITE,
NULL,
OPEN_EXISTING,
FILE_FLAG_OVERLAPPED,
NULL
);
if (hFile == INVALID_HANDLE_VALUE)
{
printf("Could not open %s/n", szPath);
return -1;
}
memset(&overlap, 0, sizeof(overlap));
overlap.Offset = 1500;
rc = ReadFile(
hFile,
buf,
READ_SIZE,
&numread,
&overlap
);
printf("Issued read request/n");
if (rc)
{
printf("Request was returned immediately/n");
}
else
{
if (GetLastError() == ERROR_IO_PENDING)
{
printf("Request queued, waiting.../n");
WaitForSingleObject(hFile, INFINITE);
printf("Request completed./n");
rc = GetOverlappedResult(
hFile,
&overlap,
&numread,
FALSE
);
printf("Result was %d/n", rc);
}
else
{
printf("Error reading file/n");
}
}
CloseHandle(hFile);
return EXIT_SUCCESS;
}
**事件内核对象****(hEvent)：**
内核对象(hand)实现的问题：
不能区分那一个overlapped操作，对同一个文件handle，系统有多个异步操作时(一边读文件头，一边写文件尾, 有一个完成，就会有信号，不能区分是那种操作。)，为每个进行中的overlapped调用GetOverlappedResult是不好的作法。
事件内核对象(hEvent)实现方案：
Overlapped成员hEven标识事件内核对象。CreateEvent,为每个请求创建一个事件，初始化每个请求的hEvent成员(对同一文件多个读写请求，每个操作绑定一个event对象)。调用WaitForMultipleObject来等等其中一个（或全部）完成。
另外Event对象必须是手动重置。使用自动重置（在等待event之前设置，WaitForSingleObject()和 WaitForMultipleObjects()函数永不返回）。
自动重置事件
WaitForSingleObject()和 WaitForMultipleObjects()会等待事件到信号状态，随后又自动将其重置为非信号状态，这样保证了等待此事件的线程中只有一个会被唤醒。
手动重置事件
需要用户调用ResetEvent()才会重置事件。可能有若干个线程在等待同一事件，这样当事件变为信号状态时，所有等待线程都可以运行了。 SetEvent()函数用来把事件对象设置成信号状态，ResetEvent()把事件对象重置成非信号状态，两者均需事件对象句柄作参数。
相关例子如下：
int main()
{
int i;
BOOL rc;
char szPath[MAX_PATH];
CheckOsVersion();
GetWindowsDirectory(szPath, sizeof(szPath));
strcat(szPath, "//WINHLP32.EXE");
ghFile = CreateFile( szPath,
GENERIC_READ,
FILE_SHARE_READ|FILE_SHARE_WRITE,
NULL,
OPEN_EXISTING,
FILE_FLAG_OVERLAPPED,
NULL
);
if (ghFile == INVALID_HANDLE_VALUE)
{
printf("Could not open %s/n", szPath);
return -1;
}
for (i=0; i
{
QueueRequest(i, i*16384, READ_SIZE);
}
printf("QUEUED!!/n");
MTVERIFY( WaitForMultipleObjects(
MAX_REQUESTS, ghEvents, TRUE, INFINITE
) != WAIT_FAILED );
for (i=0; i
{
DWORD dwNumread;
rc = GetOverlappedResult(
ghFile,
&gOverlapped[i],
&dwNumread,
FALSE
);
printf("Read \#%d returned %d. %d bytes were read./n",
i, rc, dwNumread);
CloseHandle(gOverlapped[i].hEvent);
}
CloseHandle(ghFile);
return EXIT_SUCCESS;
}
int QueueRequest(int nIndex, DWORD dwLocation, DWORD dwAmount)
{
int i;
BOOL rc;
DWORD dwNumread;
DWORD err;
MTVERIFY(
ghEvents[nIndex] = CreateEvent(
NULL,    // No security
TRUE,    // Manual reset - extremely important!
FALSE,   // Initially set Event to non-signaled state
NULL     // No name
)
);
gOverlapped[nIndex].hEvent = ghEvents[nIndex];
gOverlapped[nIndex].Offset = dwLocation;
for (i=0; i
{
rc = ReadFile(
ghFile,
gBuffers[nIndex],
dwAmount,
&dwNumread,
&gOverlapped[nIndex]
);
if (rc)
{
printf("Read \#%d completed immediately./n", nIndex);
return TRUE;
}
err = GetLastError();
if (err == ERROR_IO_PENDING)
{
// asynchronous i/o is still in progress
printf("Read \#%d queued for overlapped I/O./n", nIndex);
return TRUE;
}
if ( err == ERROR_INVALID_USER_BUFFER ||
err == ERROR_NOT_ENOUGH_QUOTA ||
err == ERROR_NOT_ENOUGH_MEMORY )
{
Sleep(50); // Wait around and try later
continue;
}
break;
}
printf("ReadFile failed./n");
return -1;
}
**异步过程调用****(apcs)：**
事件内核对象(hEvent)的问题：
事件内核对象在使用WaitForMultipleObjects时，只能等待64个对象。需要另建两个数据组，并gOverlapped[nIndex].hEvent = ghEvents[nIndex]绑定起来。
异步过程调用(apcs)实现方案：
异步过程调用，callback回调函数，在一个Overlapped I/O完成之后，系统调用该回调函数。OS在有信号状态下(设备句柄)，才会调用回调函数（可能有很多APCS等待处理了），传给它完成I/O请求的错误码，传输字节数和Overlapped结构的地址。
五个函数可以设置信号状态：
1．SleepEx
2．WaitForSingleObjectEx
3．WaitForMultipleObjectEx
4．SingalObjectAndWait
5．MsgWaitForMultipleObjectsEx
Main函数调用WaitForSingleObjectEx, APCS被处理，调用回调函数
FileIOCompletionRoutine

VOID WINAPI FileIOCompletionRoutine(
DWORD dwErrorCode, // completion code
DWORD dwNumberOfBytesTransfered,    // number of bytes transferred
LPOVERLAPPED lpOverlapped   // pointer to structure with I/O information
)
{
int nIndex = (int)(lpOverlapped->hEvent);
printf("Read \#%d returned %d. %d bytes were read./n",
nIndex,
dwErrorCode,
dwNumberOfBytesTransfered);
if (++nCompletionCount == MAX_REQUESTS)
SetEvent(ghEvent); // Cause the wait to terminate
}
int main()
{
int i;
char szPath[MAX_PATH];
CheckOsVersion();
MTVERIFY(
ghEvent = CreateEvent(
NULL,    // No security
TRUE,    // Manual reset - extremely important!
FALSE,   // Initially set Event to non-signaled state
NULL     // No name
)
);
GetWindowsDirectory(szPath, sizeof(szPath));
strcat(szPath, "//WINHLP32.EXE");
ghFile = CreateFile( szPath,
GENERIC_READ,
FILE_SHARE_READ|FILE_SHARE_WRITE,
NULL,
OPEN_EXISTING,
FILE_FLAG_OVERLAPPED,
NULL
);
if (ghFile == INVALID_HANDLE_VALUE)
{
printf("Could not open %s/n", szPath);
return -1;
}
for (i=0; i
{
QueueRequest(i, i*16384, READ_SIZE);
}
printf("QUEUED!!/n");
for (;;)
{
DWORD rc;
rc = WaitForSingleObjectEx(ghEvent, INFINITE, TRUE );
if (rc == WAIT_OBJECT_0)
break;
MTVERIFY(rc == WAIT_IO_COMPLETION);
}
CloseHandle(ghFile);
return EXIT_SUCCESS;
}
int QueueRequest(int nIndex, DWORD dwLocation, DWORD dwAmount)
{
int i;
BOOL rc;
DWORD err;
gOverlapped[nIndex].hEvent = (HANDLE)nIndex;
gOverlapped[nIndex].Offset = dwLocation;
for (i=0; i
{
rc = ReadFileEx(
ghFile,
gBuffers[nIndex],
dwAmount,
&gOverlapped[nIndex],
FileIOCompletionRoutine
);
if (rc)
{
printf("Read \#%d queued for overlapped I/O./n", nIndex);
return TRUE;
}
err = GetLastError();
if ( err == ERROR_INVALID_USER_BUFFER ||
err == ERROR_NOT_ENOUGH_QUOTA ||
err == ERROR_NOT_ENOUGH_MEMORY )
{
Sleep(50); // Wait around and try later
continue;
}
break;
}
printf("ReadFileEx failed./n");
return -1;
}
**完成端口****(I/O completion)：**
异步过程调用(apcs)问题：
只有发overlapped请求的线程才可以提供callback函数（需要一个特定的线程为一个特定的I/O请求服务）。
完成端口(I/O completion)的优点：
不会限制handle个数，可处理成千上万个连接。I/O completion port允许一个线程将一个请求暂时保存下来，由另一个线程为它做实际服务。
并发模型与线程池：
在典型的并发模型中，服务器为每一个客户端创建一个线程，如果很多客户同时请求，则这些线程都是运行的，那么CPU就要一个个切换，CPU花费了更多的时间在线程切换，线程确没得到很多CPU时间。到底应该创建多少个线程比较合适呢，微软件帮助文档上讲应该是2*CPU个。但理想条件下最好线程不要切换，而又能象线程池一样，重复利用。I/O完成端口就是使用了线程池。
**理解与使用：**
**第一步：**
在我们使用完成端口之前，要调用CreateIoCompletionPort函数先创建完成端口对象。
定义如下：
HANDLE CreateIoCompletionPort(
HANDLE FileHandle,
HANDLE ExistingCompletionPort,
DWORD CompletionKey,
DWORD NumberOfConcurrentThreads
);
FileHandle：
文件或设备的handle, 如果值为INVALID_HANDLE_VALUE则产生一个没有和任何文件handle有关系的port.( 可以用来和完成端口联系的各种句柄，文件，套接字)
ExistingCompletionPort:
NULL时生成一个新port, 否则handle会加到此port上。
CompletionKey：
用户自定义数值，被交给服务的线程。GetQueuedCompletionStatus函数时我们可以完全得到我们在此联系函数中的完成键（申请的内存块）。在GetQueuedCompletionStatus
中可以完封不动的得到这个内存块，并且使用它。
NumberOfConcurrentThreads：
参数NumberOfConcurrentThreads用来指定在一个完成端口上可以并发的线程数量。理想的情况是，一个处理器上只运行一个线程，这样可以避免线程上下文切换的开销。如果这个参数的值为0，那就是告诉系统线程数与处理器数相同。我们可以用下面的代码来创建I/O完成端口。
隐藏在之创建完成端口的秘密：
1．创建一个完成端口
CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, dwNumberOfConcurrentThreads);
2．设备列表，完成端口把它同一个或多个设备相关联。
CreateIoCompletionPort(hDevice, hCompPort, dwCompKey, 0) ;
**第二步：**
根据处理器个数，创建cpu*2个工作线程：
CreateThread(NULL, 0, ServerWorkerThread, CompletionPort,0, &ThreadID))
与此同时，服务器调用WSASocket，bind, listen, WSAAccept,之后，调用
CreateIoCompletionPort((HANDLE) Accept, CompletionPort... )把一个套接字句柄和一个完成端口绑定到一起。完成端口又同一个或多个设备相关联着，所以以套接字为基础，投递发送和请求，对I/O处理。接着，可以依赖完成端口，接收有关I/O操作完成情况的通知。再看程序里：
WSARecv(Accept, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
&(PerIoData->Overlapped), NULL)开始调用，这里象前面讲过的一样，既然是异步I/O，所以WSASend和WSARecv的调用会立即返回。
**系统处理：**
当一个设备的异步I/O请求完成之后，系统会检查该设备是否关联了一个完成端口，如果是，系统就向该完成端口的I/O完成队列中加入完成的I/O请求列。
然后我们需要从这个完成队列中，取出调用后的结果(需要通过一个Overlapped结构来接收调用的结果)。怎么知道这个队列中已经有处理后的结果呢，调用GetQueuedCompletionStatus函数。
工作线程与完成端口：
和异步过程调用不同(在一个Overlapped I/O完成之后，系统调用该回调函数。OS在有信号状态下(设备句柄)，才会调用回调函数（可能有很多APCS等待处理了）)
GetQueuedCompletionStatus
在工作线程内调用GetQueuedCompletionStatus函数。
GetQueuedCompletionStatus(
HANDLE CompletionPort,
LPDWORD lpNumberOfBytesTransferred,
LPDWORD lpCompletionKey,
LPOVERLAPPED *lpOverlapped,
DWORD dwMilliseconds
);
CompletionPort：指出了线程要监视哪一个完成端口。很多服务应用程序只是使用一个I/O完成端口，所有的I/O请求完成以后的通知都将发给该端口。
lpNumberOfBytesTransferred：传输的数据字节数
lpCompletionKey：
完成端口的单句柄数据指针，这个指针将可以得到我们在CreateIoCompletionPort中申请那片内存。
lpOverlapped:
重叠I/O请求结构，这个结构同样是指向我们在重叠请求时所申请的内存块，同时和lpCompletionKey,一样我们也可以利用这个内存块来存储我们要保存的任意数据。
dwMilliseconds:
等待的最长时间(毫秒),如果超时，lpOverlapped被设为NULL，函数返回False.
GetQueuedCompletionStatus功能及隐藏的秘密：
GetQueuedCompletionStatus使调用线程挂起，直到指定的端口的I/O完成队列中出现了一项或直到超时。（I/0完成队列中出现了记录）调用GetQueuedCompletionStatus时，调用线程的ID(cpu*2个线程，每个ServerWorkerThread的线程ID)就被放入该等待线程队列中。
等待线程队列很简单，只是保存了这些线程的ID。完成端口会按照后进先出的原则将一个线程队列的ID放入到释放线程列表中。
这样，I/O完成端口内核对象就知道哪些线程正在等待处理完成的I/O请求。当端口的I/O完成队列出现一项时，完成端口就唤醒（睡眠状态中变为可调度状态）等待线程队列中的一个线程。线程将得到完成I/O项中的信息：传输的字节数，完成键(单句柄数据结构)和Overlapped结构地址，线程是通过GetQueuedCompletionStatus返回这些信息，等待CPU的调度。
GetQueuedCompletionStatus返回可能有多种原因，如果传递无效完成端口句柄，函数返回False，GetLastError返回一个错误(ERROR_INVALID_HANDLE),如果超时，返回False, GetLastError返回WAIT_TIMEOUT, i/o完成队列删除一项，该表项是一个成功完成的I/O请求，则返回True。
调用GetQueuedCompletionStatus的线程是后进先出的方式唤醒的，比如有4个线程等待，如果有一个I/O，最后一个调用GetQueuedCompletionStatus的线程被唤醒来处理。处理完之后，再调用GetQueuedCompletionStatus进入等待线程队列中。
深入分析完成端口线程池调度原理：
假设我们运行在2CPU的机器上。创建完成端口时指定2个并发，创建了4个工作线程加入线程池中等待完成I/O请求，且完成端口队列（先入先出）中有3个完成I/O的请求的情况：
工作线程运行, 创建了4个工作线程，调用GetQueuedCompletionStatus时，该调用线程就进入了睡眠状态，假设这个时候，I/O完成队列出现了三项，调用线程的ID就被放入该等待线程队列中, (如图)：

|等待的线程队列（后进先出）
|
|进队列
|
|出队列
|
|线
|程
|A
|
|线
|程
|B
|
|线
|程
|C
|
|线
|程
|D
|
I/O完成端口内核对象（第3个参数等级线程队列）,因此知道哪些线程正在等待处理完成的I/O请求。当端口的I/O完成队列出现一项时，完成端口就唤醒（睡眠状态中变为可调度状态）等待线程队列中的一个线程(前面讲过等待线程队列是后进先出)。所以线程D将得到完成I/O项中的信息：传输的字节数，完成键(单句柄数据结构)和Overlapped结构地址，线程是通过GetQueuedCompletionStatus返回这些信息。
在前面我们指定了并发线程的数目是2，所以I/O完成端口唤醒2个线程，线程D和线程C，另两个继续休眠（线程B，线程A），直到线程D处理完了，发现表项里还有要处理的，就唤醒同一线程继续处理。

|等待的线程队列（后进先出）
|
|进队列
|
|出队列
|
|线
|程
|A
|
|线
|程
|B
|
|释放线程队列
|
|线
|程
|C
|
|线
|程
|D
|
线程并发量：
并发量限制了与该完成端口相关联的可运行线程的数目, 它类似阀门的作用。 当与该完成端口相关联的可运行线程的总数目达到了该并发量，系统就会阻塞任何与该完成端口相关联的后续线程的执行， 直到与该完成端口相关联的可运行线程数目下降到小于该并发量为止。所以解释了线程池中的运行线程可能会比设置的并发线程多的原因。
它的作用:
最有效的假想是发生在有完成包在队列中等待，而没有等待被满足，因为此时完成端口达到了其并发量的极限。此时，一个正在运行中的线程调用GetQueuedCompletionStatus时，它就会立刻从队列中取走该完成包。这样就不存在着环境的切换，因为该处于运行中的线程就会连续不断地从队列中取走完成包，而其他的线程就不能运行了。
注意：如果池中的所有线程都在忙，客户请求就可能拒绝，所以要适当调整这个参数，获得最佳性能。
**线程并发：**D线程挂起，加入暂停线程，醒来后又加入释放线程队列。

|线
|程
|C
|
|线
|程
|B
|
|线
|程
|A
|
|出队列
|
|进队列
|
|等待的线程队列（后进先出）
|
|释放线程队列
|
|暂停线程
|
|线
|程
|D
|
线程的安全退出：
PostQueudCompletionStatus函数，我们可以用它发送一个自定义的包含了OVERLAPPED成员变量的结构地址，里面包含一个状态变量，当状态变量为退出标志时，线程就执行清除动作然后退出。
完成端口使用需要注意的地方：
1．在执行wsasend和wsarecv操作前，请先将overlapped结构体使用memset进行清零。














