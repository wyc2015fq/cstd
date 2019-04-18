# WinSocket模型的探讨——完成端口模型（一） - ljx0305的专栏 - CSDN博客
2009年04月06日 10:26:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1032标签：[socket																[null																[io																[struct																[winapi																[delete](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[VC																[网络编程](https://blog.csdn.net/ljx0305/article/category/403915)](https://blog.csdn.net/ljx0305/article/category/401790)
众所皆知，完成端口是在WINDOWS平台下效率最高，扩展性最好的IO模型，特别针对于WINSOCK的海量连接时，更能显示出其威力。其实建立一个完成端口的服务器也很简单，只要注意几个函数，了解一下关键的步骤也就行了。
这是篇完成端口入门级的文章，分为以下几步来说明完成端口： 
- 函数 
- 常见问题以及解答 
- 步骤 
- 例程 
**1、函数：**
我们在完成端口模型下会使用到的最重要的两个函数是：
CreateIoCompletionPort、GetQueuedCompletionStatus
CreateIoCompletionPort  的作用是创建一个完成端口和把一个IO句柄和完成端口关联起来：
// 创建完成端口
HANDLE CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
// 把一个IO句柄和完成端口关联起来，这里的句柄是一个socket 句柄
CreateIoCompletionPort((HANDLE)sClient, CompletionPort, (DWORD)PerHandleData, 0);
其中第一个参数是句柄，可以是文件句柄、SOCKET句柄。
第二个就是我们上面创建出来的完成端口，这里就把两个东西关联在一起了。
第三个参数很关键，叫做PerHandleData，就是对应于每个句柄的数据块。我们可以使用这个参数在后面取到与这个SOCKET对应的数据。
最后一个参数给0，意思就是根据CPU的个数，允许尽可能多的线程并发执行。
GetQueuedCompletionStatus 的作用就是取得完成端口的结果：
// 从完成端口中取得结果
GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (LPDWORD)&PerHandleData, (LPOVERLAPPED*)&PerIoData, INFINITE)
第一个参数是完成端口
第二个参数是表明这次的操作传递了多少个字节的数据
第三个参数是OUT类型的参数，就是前面CreateIoCompletionPort传进去的单句柄数据，这里就是前面的SOCKET句柄以及与之相对应的数据，这里操作系统给我们返回，让我们不用自己去做列表查询等操作了。
第四个参数就是进行IO操作的结果，是我们在投递 WSARecv / WSASend 等操作时传递进去的，这里操作系统做好准备后，给我们返回了。非常省事！！
个人感觉完成端口就是操作系统为我们包装了很多重叠IO的不爽的地方，让我们可以更方便的去使用，下篇我将会尝试去讲述完成端口的原理。
**2、常见问题和解答**
a、什么是单句柄数据(PerHandle)和单IO数据(PerIO)
单句柄数据就是和句柄对应的数据，像socket句柄，文件句柄这种东西。
单IO数据，就是对应于每次的IO操作的数据。例如每次的WSARecv/WSASend等等
其实我觉得PER是每次的意思，翻译成每个句柄数据和每次IO数据还比较清晰一点。
在完成端口中，单句柄数据直接通过GetQueuedCompletionStatus 返回，省去了我们自己做容器去管理。单IO数据也容许我们自己扩展OVERLAPPED结构，所以，在这里所有与应用逻辑有关的东西都可以在此扩展。
b、如何判断客户端的断开
我们要处理几种情况
1) 如果客户端调用了closesocket，我们就可以这样判断他的断开：
if(0 == GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, 。。。)
{
}
if(BytesTransferred == 0)
{
    // 客户端断开，释放资源
}
2)　如果是客户端直接退出，那就会出现６４错误，指定的网络名不可再用。这种情况我们也要处理的：
if(0 == GetQueuedCompletionStatus(。。。))
{
   if( (GetLastError() == WAIT_TIMEOUT) || (GetLastError() == ERROR_NETNAME_DELETED) )
   {
        // 客户端断开，释放资源
   }
}
**3、步骤**
编写完成端口服务程序，无非就是以下几个步骤：
  1、创建一个完成端口
  2、根据CPU个数创建工作者线程，把完成端口传进去线程里
  3、创建侦听SOCKET，把SOCKET和完成端口关联起来
  4、创建PerIOData，向连接进来的SOCKET投递WSARecv操作
  5、线程里所做的事情：
 a、GetQueuedCompletionStatus，在退出的时候就可以使用PostQueudCompletionStatus使线程退出
 b、取得数据并处理
**4、例程**
下面是服务端的例程，可以使用《WinSocket模型的探讨——Overlapped模型（一）》中的客户端程序来测试次服务端。稍微研究一下，也就会对完成端口模型有个大概的了解了。
/*
   完成端口服务器
   接收到客户端的信息，直接显示出来
*/
#include "winerror.h"
#include "Winsock2.h"
#pragma comment(lib, "ws2_32")
#include "windows.h"
#include <iostream>
using namespace std;
/// 宏定义
#define PORT 5050
#define DATA_BUFSIZE 8192
#define OutErr(a) cout << (a) << endl /
      << "出错代码：" << WSAGetLastError() << endl /
      << "出错文件：" << __FILE__ << endl  /
      << "出错行数：" << __LINE__ << endl /
#define OutMsg(a) cout << (a) << endl;
/// 全局函数定义
///////////////////////////////////////////////////////////////////////
//
// 函数名       : InitWinsock
// 功能描述     : 初始化WINSOCK
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void InitWinsock()
{
 // 初始化WINSOCK
 WSADATA wsd;
 if( WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
 {
  OutErr("WSAStartup()");
 }
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : BindServerOverlapped
// 功能描述     : 绑定端口，并返回一个 Overlapped 的Listen Socket
// 参数         : int nPort
// 返回值       : SOCKET 
//
///////////////////////////////////////////////////////////////////////
SOCKET BindServerOverlapped(int nPort)
{
 // 创建socket 
 SOCKET sServer = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
 // 绑定端口
 struct sockaddr_in servAddr;
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = htons(nPort);
 servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
 if(bind(sServer, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
 {
  OutErr("bind Failed!");
  return NULL;
 }
 // 设置监听队列为200
 if(listen(sServer, 200) != 0)
 {
  OutErr("listen Failed!");
  return NULL;
 }
 return sServer;
}
/// 结构体定义
typedef struct
{
   OVERLAPPED Overlapped;
   WSABUF DataBuf;
   CHAR Buffer[DATA_BUFSIZE];
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;
typedef struct 
{
   SOCKET Socket;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;
DWORD WINAPI ProcessIO(LPVOID lpParam)
{
 HANDLE CompletionPort = (HANDLE)lpParam;
    DWORD BytesTransferred;
    LPPER_HANDLE_DATA PerHandleData;
    LPPER_IO_OPERATION_DATA PerIoData;
 while(true)
 {
  if(0 == GetQueuedCompletionStatus(CompletionPort, &BytesTransferred, (LPDWORD)&PerHandleData, (LPOVERLAPPED*)&PerIoData, INFINITE))
  {
   if( (GetLastError() == WAIT_TIMEOUT) || (GetLastError() == ERROR_NETNAME_DELETED) )
   {
    cout << "closing socket" << PerHandleData->Socket << endl;
    closesocket(PerHandleData->Socket);
    delete PerIoData;
    delete PerHandleData;
    continue;
   }
   else
   {
    OutErr("GetQueuedCompletionStatus failed!");
   }
   return 0;
  }
  // 说明客户端已经退出
  if(BytesTransferred == 0)
  {
   cout << "closing socket" << PerHandleData->Socket << endl;
   closesocket(PerHandleData->Socket);
   delete PerIoData;
   delete PerHandleData;
   continue;
  }
  // 取得数据并处理
  cout << PerHandleData->Socket << "发送过来的消息：" << PerIoData->Buffer << endl;
  // 继续向 socket 投递WSARecv操作
  DWORD Flags = 0;
  DWORD dwRecv = 0;
  ZeroMemory(PerIoData, sizeof(PER_IO_OPERATION_DATA));
  PerIoData->DataBuf.buf = PerIoData->Buffer;
  PerIoData->DataBuf.len = DATA_BUFSIZE;
  WSARecv(PerHandleData->Socket, &PerIoData->DataBuf, 1, &dwRecv, &Flags, &PerIoData->Overlapped, NULL); 
 }
 return 0;
}
void main()
{
 InitWinsock();
 HANDLE CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
 // 根据系统的CPU来创建工作者线程
 SYSTEM_INFO SystemInfo;
 GetSystemInfo(&SystemInfo);
 for(int i = 0; i < SystemInfo.dwNumberOfProcessors * 2; i++)
 {
  HANDLE hProcessIO = CreateThread(NULL, 0, ProcessIO, CompletionPort, 0, NULL);
  if(hProcessIO)
  {
   CloseHandle(hProcessIO);
  }
 }
 // 创建侦听SOCKET
 SOCKET sListen = BindServerOverlapped(PORT);
 SOCKET sClient;
 LPPER_HANDLE_DATA PerHandleData;
    LPPER_IO_OPERATION_DATA PerIoData;
 while(true)
 {
  // 等待客户端接入
  //sClient = WSAAccept(sListen, NULL, NULL, NULL, 0);
  sClient = accept(sListen, 0, 0);
  cout << "Socket " << sClient << "连接进来" << endl;
  PerHandleData = new PER_HANDLE_DATA();
  PerHandleData->Socket = sClient;
  // 将接入的客户端和完成端口联系起来
  CreateIoCompletionPort((HANDLE)sClient, CompletionPort, (DWORD)PerHandleData, 0);
  // 建立一个Overlapped，并使用这个Overlapped结构对socket投递操作
  PerIoData = new PER_IO_OPERATION_DATA();
  ZeroMemory(PerIoData, sizeof(PER_IO_OPERATION_DATA));
  PerIoData->DataBuf.buf = PerIoData->Buffer;
  PerIoData->DataBuf.len = DATA_BUFSIZE;
  // 投递一个WSARecv操作
  DWORD Flags = 0;
  DWORD dwRecv = 0;
  WSARecv(sClient, &PerIoData->DataBuf, 1, &dwRecv, &Flags, &PerIoData->Overlapped, NULL);
 }
 DWORD dwByteTrans;
 PostQueuedCompletionStatus(CompletionPort, dwByteTrans, 0, 0);
 closesocket(sListen);
}
好了，本篇文章就到此为止，以后有时间再继续写下一篇，下篇就不是这种步骤和函数的说明了，而是包括了封装、架构、应用等等方面的技术。OK，下次再会！
引用：[http://blog.csdn.net/dylgsy/archive/2007/08/02/1722325.aspx](http://blog.csdn.net/dylgsy/archive/2007/08/02/1722325.aspx)
