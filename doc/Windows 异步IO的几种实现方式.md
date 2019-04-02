# Windows 异步IO的几种实现方式

2015年03月26日 18:00:30 [zj510](https://me.csdn.net/zj510) 阅读数：10197



Windows上的异步IO有好几种实现方式。



## 设备内核对象

这是最简单的一种了，直接用设备内核对象的状态。比如文件句柄，线程句柄等等，这些内核对象都是有一个触发状态的，比如当一个线程结束后，线程内核对象就会被触发。

对于文件内核对象，如果一个异步IO完成了，就会把文件句柄设置为触发状态，但是有个问题就是：**如果有多个异步io，那么只要有一个异步io完成了，文件句柄就会被设置为触发状态。这样，就不能应用于多个异步io的情况，因为根本不知道是哪个异步io完成了。**

代码例子：



```cpp
/* 设备内核对象
通过设备内核对象来得到异步IO完成通知。
使用很简单。
缺点：如果对一个设备发起了多个io请求，那么这个办法就不行了。
这是因为，多个异步io请求里面任何一个完成了，都会把设备内核对象的状态
设置成触发。这样当WaitForSingleObject返回的时候就无法知道是什么io完成了。
*/
void DeviceObjIO()
{
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);//创建一个文件，设置成异步IO，FILE_FLAG_OVERLAPPED
	BYTE buffer[10] = {'a', 'b', 'c', 'd'};
	OVERLAPPED ol = {0};//初始化OVERLAPPED的结构
	ol.Offset = 2;//表示从第三个字节开始写
 
	BOOL rt = WriteFile(hFile, buffer, 5, NULL, &ol);//发起一个异步写操作
 
	//这里可以做其他事情了，因为WriteFile是异步了，会马上返回。如果是同步的WriteFile，那么假如WriteFile需要10秒钟，WriteFile在10秒内是不会返回的。
 
//	SetFileCompletionNotificationModes(hFile, FILE_SKIP_SET_EVENT_ON_HANDLE);//如果设置了这个标记，那么文件内核对象就不会被触发了。
 
	if (rt == FALSE && GetLastError() == ERROR_IO_PENDING)//检查异步IO是否完成了，可以在其他线程检查。这里只是一个演示。
	{
		WaitForSingleObject(hFile, INFINITE);//等待设备内核对象（文件）被触发。
	}
 
	CloseHandle(hFile);
}
```

## 事件内核对象



事件内核对象比设备内核对象好一点，可以支持多个异步io。每个read或者write里面的overlapped的结构，我们可以设置一个事件内核对象，这样每次io的事件内核对象是不一样的，就可以支持多个异步io了。**但是WaitForMultipleObject也有个限制，就是一次等待的事件内核对象最多只能有64个，当然也可以变通一下，比如把所有的事件内核对象分组等。**



```cpp
/* 事件内核对象
通过一个事件内核对象来获取异步IO完成的通知。
这个比较设备内核对象好一些，可以支持多个异步IO，因为程序可以通过每个异步IO的OVERLAPPED结构里面的事件内核对象来判断
异步IO是否完成了。
*/
void EventObjIO()
{
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//打开前面创建的文件
	BYTE buffer[10] = {0};
	OVERLAPPED ol = {0};
	ol.Offset = 0;
	HANDLE hEvent = CreateEvent(0, FALSE, FALSE, NULL);
	ol.hEvent = hEvent;//传递一个事件对象。
 
	BOOL rt = ReadFile(hFile, buffer, 7, NULL, &ol);//提交一个异步读操作
 
//	DWORD read = 0;
//	GetOverlappedResult(hFile, &ol, &read, TRUE);//也可以使用这个函数来等待执行结果。GetOverlappedResult内部会调用WaitForSingleObject来检查异步IO是否完成了。
 
	if (rt == FALSE && GetLastError() == ERROR_IO_PENDING)
	{
		WaitForSingleObject(ol.hEvent, INFINITE);//等待事件对象被触发。
	}
 
	CloseHandle(hFile);
	CloseHandle(hEvent);
}
```

## **APC 队列**



每个线程都有一个APC队列。曾经微软公司大力推广这个技术，但是有个最大的问题，**就是回调函数只能在调用线程里面被调用。如果调用线程忙，那么就得不到callback的执行。**个人从来没有使用过这种方式。



```cpp
/* 可提醒IO
通过线程的APC队列
当系统创建一个线程的时候，同时会创建一个线程APC队列
APC: Asynchronous Procedure Call
在主调线程里面需要通过可提醒等待函数来等待回调函数的执行。
最大的缺点就是：回调函数将会在主调线程里面。如果主调线程在做其他事情，
就算异步IO已经完成了，而且驱动已经把完成的IO添加到线程的APC队列，线程也没有办法来执行回调函数。
*/
VOID
WINAPI myCallback(
	__in    DWORD dwErrorCode,
	__in    DWORD dwNumberOfBytesTransfered,
	__inout LPOVERLAPPED lpOverlapped
	)
{
	printf("transfered: %d\n", dwNumberOfBytesTransfered);
}
 
void APCIO()
{
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//打开前面创建的文件
	BYTE buffer[10] = {'f', 'f', 'f'};
	OVERLAPPED ol = {0};
	ol.Offset = 5;
 
	BOOL rt = WriteFileEx(hFile, buffer, 3, &ol, myCallback);//跟前面的WriteFile不一样，这里会返回TRUE.
 
	//再写一次
	BYTE buffer2[10] = {'g', 'g', 'g', 'g'};
	OVERLAPPED ol2 = {0};
	ol2.Offset = 8;
 
	rt = WriteFileEx(hFile, buffer2, 4, &ol2, myCallback);
 
//	Sleep(5000);//在这5秒内，就算异步IO完成了，回调函数也不会被执行。因为回调函数只能在当前线程中执行，但是这个线程在做其他事情：睡觉。
 
	DWORD dwError = SleepEx(10000, TRUE);//如果APC被成功调用的话，那么这个函数会返回，同时返回值是WAIT_IO_COMPLETION.SleepEx跟Sleep的区别是SleepEx是可提醒的，如果
										//当前线程的APC队列里面有东西了，那么线程将被唤醒，并且执行回调函数。回调函数结束后，SleepEx也将返回。
 
	if(dwError == WAIT_IO_COMPLETION)
	{
		printf("APC successfully\n");
	}
 
	CloseHandle(hFile);
 
}
```

## 完成端口

异步io里面最高效的一种方式了。

Windows上服务器模型最高效的就是socket + 完成端口了。这里的例子是文件+完成端口。

把文件句柄和完成端口相绑定，然后发起异步io，这样当有异步io完成的时候，这个通知就会把发送到完成端口内核对象的IO完成队列，然后完成端口会找个线程池里面找到一个等待的线程并且唤醒。

线程池里面的线程有程序员创建，一般创建CPU * 2个线程。每个创建的线程调用GetQueuedCompletionStatus(), 这个线程就会被放到完成端口的“等待线程队列”里面。

下面是个简单的例子。



```cpp
/* 完成端口
*/
 
 
class MyOverlapped: public OVERLAPPED
{
public:
	MyOverlapped()
	{
		memset(this, 0, sizeof(MyOverlapped));
 
		m_buf = new BYTE[10];
		memset(m_buf, 0, 10);
	}
 
	~MyOverlapped()
	{
		if(m_buf)
		{
			delete m_buf;
			m_buf = NULL;
		}
	}
 
	BYTE* GetByte() const{return m_buf;}
 
protected:
	BYTE* m_buf;
};
 
void WorkThread(void* pv)
{
	HANDLE hCP = (HANDLE)pv;
 
	while (TRUE)
	{
		DWORD dwTransfer = 0;
		MyOverlapped* ol = NULL;
 
		ULONG_PTR key = 0;
		//查询完成端口上的完成队列，看看是否有已经完成的异步IO.
		BOOL rt = GetQueuedCompletionStatus(hCP, &dwTransfer, &key, (LPOVERLAPPED*)&ol, INFINITE);//第三个参数不可以传0，不然会得到一个998的错误
		DWORD err = GetLastError();
		if (!rt)
		{
			break;
		}
 
		printf("IOCP recv: %d bytes, thread id: %d\n", dwTransfer, GetCurrentThreadId());
	}
}
 
void IOCP()
{
	//完成端口是一个内核对象，但是这个创建函数为什么没有安全参数呢？
	//据说所有的内核对象里面，就是完成端口是个例外。根据书上说的，可能是因为完成端口的设计初衷就是在一个进程里面使用。
	HANDLE hCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);//创建一个完成端口对象，最后一个参数，表示使用默认的并发线程数，CPU的数量。
 
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//打开一个设备
 
	//将这个设备关联到完成端口,这样当设备上的异步IO完成的时候，就会通知完成端口的完成队列。
	HANDLE h = CreateIoCompletionPort(hFile, hCP, NULL, 0);
 
	assert(h == hCP);//返回值肯定和之前已经创建的完成端口对象一样
 
	MyOverlapped ol;//使用一个自定义的OVERLAPPED
	ol.Offset = 4;
 
	BOOL rt = ReadFile(hFile, ol.GetByte(), 4, NULL, &ol);//发起一个异步IO
 
	MyOverlapped ol2;
	ol2.Offset = 5;
 
	rt = ReadFile(hFile, ol2.GetByte(), 5, NULL, &ol2);//再发起一个异步IO
 
	DWORD dwErr = GetLastError();
 
	//一般会创建CPU个数 * 2 个处理线程。
	_beginthread(WorkThread, 0, hCP);//启动一个处理线程。
	_beginthread(WorkThread, 0, hCP);//再启动一个处理线程。
 
	Sleep(2000);
 
	CloseHandle(hFile);
	CloseHandle(hCP);
}
```

## 线程池 + 完成端口

上面的例子里面，线程是自己创建的，需要自己来维护线程的创建和销毁。Windows提供了线程池来和完成端口相配合，这样省去了创建和销毁线程的麻烦。

Windows的线程池有很多功能，比如和timer一起使用，和完成端口一起使用。

CreateThreadPoolIo会创建一个完成端口，把想要绑定的io对象，如文件句柄传给它，这样线程池里面的完成端口对象就和文件句柄绑定了。

简单例子如下：



```cpp
/*线程池 + 完成端口
*/
 
VOID CALLBACK OverlappedCompletionRoutine(PTP_CALLBACK_INSTANCE pInstance,
										  PVOID pvContext,
										  PVOID pOverlapped,
										  ULONG IoResult,
										  ULONG_PTR NumberOfBytesTransferred,
										  PTP_IO pIo)
{
	printf("OverlappedCompletionRoutine, transferred: %d bytes\n", NumberOfBytesTransferred);
}
 
 
 
void IOCP_ThreadPool()
{
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//打开一个设备
 
	PTP_IO pio = CreateThreadpoolIo(hFile, OverlappedCompletionRoutine, NULL, NULL);//将设备对象和线程池的IO完成端口关联起来。
	DWORD err = GetLastError();
 
 
	MyOverlapped ol;//使用一个自定义的OVERLAPPED
	ol.Offset = 4;
 
	StartThreadpoolIo(pio);//每次发起一个异步io请求的时候，都要调一下这个函数，不然在CloseThreadpoolIo()的时候会出异常。
	BOOL rt = ReadFile(hFile, ol.GetByte(), 4, NULL, &ol);//发起一个异步IO
	
 
	MyOverlapped ol2;
	ol2.Offset = 5;
 
	StartThreadpoolIo(pio);
	rt = ReadFile(hFile, ol2.GetByte(), 5, NULL, &ol2);//再发起一个异步IO
 
 
	Sleep(4000);
 
	CloseHandle(hFile);//关闭文件对象
	CloseThreadpoolIo(pio);//关闭线程池io完成对象
}
```



所有异步io里面，个人用的最多的其实是完成端口 + 自己创建线程的方式了。**当然每种方式都有自己的优缺点，具体使用哪种还是需要看应用场景，没有最好的方式，只有合适的方式。**

附：

完整测试例子

```cpp
// AsyncIO.cpp : Defines the entry point for the console application.
//
 
#include "stdafx.h"
#include <Windows.h>
#include <process.h>
#include <assert.h>
 
/* 设备内核对象
通过设备内核对象来得到异步IO完成通知。
使用很简单。
缺点：如果对一个设备发起了多个io请求，那么这个办法就不行了。
这是因为，多个异步io请求里面任何一个完成了，都会把设备内核对象的状态
设置成触发。这样当WaitForSingleObject返回的时候就无法知道是什么io完成了。
*/
void DeviceObjIO()
{
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);//创建一个文件，设置成异步IO，FILE_FLAG_OVERLAPPED
	BYTE buffer[10] = {'a', 'b', 'c', 'd'};
	OVERLAPPED ol = {0};//初始化OVERLAPPED的结构
	ol.Offset = 2;//表示从第三个字节开始写
 
	BOOL rt = WriteFile(hFile, buffer, 5, NULL, &ol);//发起一个异步写操作
 
	//这里可以做其他事情了，因为WriteFile是异步了，会马上返回。如果是同步的WriteFile，那么假如WriteFile需要10秒钟，WriteFile在10秒内是不会返回的。
 
//	SetFileCompletionNotificationModes(hFile, FILE_SKIP_SET_EVENT_ON_HANDLE);//如果设置了这个标记，那么文件内核对象就不会被触发了。
 
	if (rt == FALSE && GetLastError() == ERROR_IO_PENDING)//检查异步IO是否完成了，可以在其他线程检查。这里只是一个演示。
	{
		WaitForSingleObject(hFile, INFINITE);//等待设备内核对象（文件）被触发。
	}
 
	CloseHandle(hFile);
}
 
/* 事件内核对象
通过一个事件内核对象来获取异步IO完成的通知。
这个比较设备内核对象好一些，可以支持多个异步IO，因为程序可以通过每个异步IO的OVERLAPPED结构里面的事件内核对象来判断
异步IO是否完成了。
*/
void EventObjIO()
{
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//打开前面创建的文件
	BYTE buffer[10] = {0};
	OVERLAPPED ol = {0};
	ol.Offset = 0;
	HANDLE hEvent = CreateEvent(0, FALSE, FALSE, NULL);
	ol.hEvent = hEvent;//传递一个事件对象。
 
	BOOL rt = ReadFile(hFile, buffer, 7, NULL, &ol);//提交一个异步读操作
 
//	DWORD read = 0;
//	GetOverlappedResult(hFile, &ol, &read, TRUE);//也可以使用这个函数来等待执行结果。GetOverlappedResult内部会调用WaitForSingleObject来检查异步IO是否完成了。
 
	if (rt == FALSE && GetLastError() == ERROR_IO_PENDING)
	{
		WaitForSingleObject(ol.hEvent, INFINITE);//等待事件对象被触发。
	}
 
	CloseHandle(hFile);
	CloseHandle(hEvent);
}
 
/* 可提醒IO
通过线程的APC队列
当系统创建一个线程的时候，同时会创建一个线程APC队列
APC: Asynchronous Procedure Call
在主调线程里面需要通过可提醒等待函数来等待回调函数的执行。
最大的缺点就是：回调函数将会在主调线程里面。如果主调线程在做其他事情，
就算异步IO已经完成了，而且驱动已经把完成的IO添加到线程的APC队列，线程也没有办法来执行回调函数。
*/
VOID
WINAPI myCallback(
	__in    DWORD dwErrorCode,
	__in    DWORD dwNumberOfBytesTransfered,
	__inout LPOVERLAPPED lpOverlapped
	)
{
	printf("transfered: %d\n", dwNumberOfBytesTransfered);
}
 
void APCIO()
{
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//打开前面创建的文件
	BYTE buffer[10] = {'f', 'f', 'f'};
	OVERLAPPED ol = {0};
	ol.Offset = 5;
 
	BOOL rt = WriteFileEx(hFile, buffer, 3, &ol, myCallback);//跟前面的WriteFile不一样，这里会返回TRUE.
 
	//再写一次
	BYTE buffer2[10] = {'g', 'g', 'g', 'g'};
	OVERLAPPED ol2 = {0};
	ol2.Offset = 8;
 
	rt = WriteFileEx(hFile, buffer2, 4, &ol2, myCallback);
 
//	Sleep(5000);//在这5秒内，就算异步IO完成了，回调函数也不会被执行。因为回调函数只能在当前线程中执行，但是这个线程在做其他事情：睡觉。
 
	DWORD dwError = SleepEx(10000, TRUE);//如果APC被成功调用的话，那么这个函数会返回，同时返回值是WAIT_IO_COMPLETION.SleepEx跟Sleep的区别是SleepEx是可提醒的，如果
										//当前线程的APC队列里面有东西了，那么线程将被唤醒，并且执行回调函数。回调函数结束后，SleepEx也将返回。
 
	if(dwError == WAIT_IO_COMPLETION)
	{
		printf("APC successfully\n");
	}
 
	CloseHandle(hFile);
 
}
 
/* 完成端口
*/
 
 
class MyOverlapped: public OVERLAPPED
{
public:
	MyOverlapped()
	{
		memset(this, 0, sizeof(MyOverlapped));
 
		m_buf = new BYTE[10];
		memset(m_buf, 0, 10);
	}
 
	~MyOverlapped()
	{
		if(m_buf)
		{
			delete m_buf;
			m_buf = NULL;
		}
	}
 
	BYTE* GetByte() const{return m_buf;}
 
protected:
	BYTE* m_buf;
};
 
void WorkThread(void* pv)
{
	HANDLE hCP = (HANDLE)pv;
 
	while (TRUE)
	{
		DWORD dwTransfer = 0;
		MyOverlapped* ol = NULL;
 
		ULONG_PTR key = 0;
		//查询完成端口上的完成队列，看看是否有已经完成的异步IO.
		BOOL rt = GetQueuedCompletionStatus(hCP, &dwTransfer, &key, (LPOVERLAPPED*)&ol, INFINITE);//第三个参数不可以传0，不然会得到一个998的错误
		DWORD err = GetLastError();
		if (!rt)
		{
			break;
		}
 
		printf("IOCP recv: %d bytes, thread id: %d\n", dwTransfer, GetCurrentThreadId());
	}
}
 
void IOCP()
{
	//完成端口是一个内核对象，但是这个创建函数为什么没有安全参数呢？
	//据说所有的内核对象里面，就是完成端口是个例外。根据书上说的，可能是因为完成端口的设计初衷就是在一个进程里面使用。
	HANDLE hCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);//创建一个完成端口对象，最后一个参数，表示使用默认的并发线程数，CPU的数量。
 
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//打开一个设备
 
	//将这个设备关联到完成端口,这样当设备上的异步IO完成的时候，就会通知完成端口的完成队列。
	HANDLE h = CreateIoCompletionPort(hFile, hCP, NULL, 0);
 
	assert(h == hCP);//返回值肯定和之前已经创建的完成端口对象一样
 
	MyOverlapped ol;//使用一个自定义的OVERLAPPED
	ol.Offset = 4;
 
	BOOL rt = ReadFile(hFile, ol.GetByte(), 4, NULL, &ol);//发起一个异步IO
 
	MyOverlapped ol2;
	ol2.Offset = 5;
 
	rt = ReadFile(hFile, ol2.GetByte(), 5, NULL, &ol2);//再发起一个异步IO
 
	DWORD dwErr = GetLastError();
 
	//一般会创建CPU个数 * 2 个处理线程。
	_beginthread(WorkThread, 0, hCP);//启动一个处理线程。
	_beginthread(WorkThread, 0, hCP);//再启动一个处理线程。
 
	Sleep(2000);
 
	CloseHandle(hFile);
	CloseHandle(hCP);
}
 
/*线程池 + 完成端口
*/
 
VOID CALLBACK OverlappedCompletionRoutine(PTP_CALLBACK_INSTANCE pInstance,
										  PVOID pvContext,
										  PVOID pOverlapped,
										  ULONG IoResult,
										  ULONG_PTR NumberOfBytesTransferred,
										  PTP_IO pIo)
{
	printf("OverlappedCompletionRoutine, transferred: %d bytes\n", NumberOfBytesTransferred);
}
 
 
 
void IOCP_ThreadPool()
{
	HANDLE hFile = CreateFileW(L"d:\\test.txt", GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);//打开一个设备
 
	PTP_IO pio = CreateThreadpoolIo(hFile, OverlappedCompletionRoutine, NULL, NULL);//将设备对象和线程池的IO完成端口关联起来。
	DWORD err = GetLastError();
 
 
	MyOverlapped ol;//使用一个自定义的OVERLAPPED
	ol.Offset = 4;
 
	StartThreadpoolIo(pio);//每次发起一个异步io请求的时候，都要调一下这个函数，不然在CloseThreadpoolIo()的时候会出异常。
	BOOL rt = ReadFile(hFile, ol.GetByte(), 4, NULL, &ol);//发起一个异步IO
	
 
	MyOverlapped ol2;
	ol2.Offset = 5;
 
	StartThreadpoolIo(pio);
	rt = ReadFile(hFile, ol2.GetByte(), 5, NULL, &ol2);//再发起一个异步IO
 
 
	Sleep(4000);
 
	CloseHandle(hFile);//关闭文件对象
	CloseThreadpoolIo(pio);//关闭线程池io完成对象
}
 
int _tmain(int argc, _TCHAR* argv[])
{
	DeviceObjIO();
 
	EventObjIO();
 
	APCIO();
 
	IOCP();
 
	IOCP_ThreadPool();
 
	return 0;
}
```