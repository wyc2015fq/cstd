# Windows Socket五种I/O模型 - 深之JohnChen的专栏 - CSDN博客

2008年03月02日 16:23:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2116标签：[socket																[windows																[asynchronous																[winapi																[struct																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=asynchronous&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[网络编程](https://blog.csdn.net/byxdaz/article/category/166215)


**Windows Socket五种I/O模型**

如果你想在Windows平台上构建服务器应用，那么I/O模型是你必须考虑的。Windows操作系统提供了选择（Select）、异步选择（WSAAsyncSelect）、事件选择（WSAEventSelect）、重叠I/O（Overlapped I/O）和完成端口（Completion Port)共五种I/O模型。每一种模型均适用于一种特定的应用场景。程序员应该对自己的应用需求非常明确，而且综合考虑到程序的扩展性和可移植性等因素，作出自己的选择。

我会以一个回应反射式服务器（与《Windows网络编程》第八章一样）来介绍这五种I/O模型。
我们假设客户端的代码如下（为代码直观，省去所有错误检查，以下同）：
#include <WINSOCK2.H>
#include <stdio.h>
#define SERVER_ADDRESS "137.117.2.148"
#define PORT           5150
#define MSGSIZE        1024
#pragma comment(lib, "ws2_32.lib")

int main()
{
WSADATA     wsaData;
SOCKET      sClient;
SOCKADDR_IN server;
char        szMessage[MSGSIZE];
int         ret;

// Initialize Windows socket library
WSAStartup(0x0202, &wsaData);

// Create client socket
sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Connect to server
memset(&server, 0, sizeof(SOCKADDR_IN));
server.sin_family = AF_INET;
server.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS);
server.sin_port = htons(PORT);

connect(sClient, (struct sockaddr *)&server, sizeof(SOCKADDR_IN));

while (TRUE)
{
    printf("Send:");
   gets(szMessage);

    // Send message
    send(sClient, szMessage, strlen(szMessage), 0);

    // Receive message
    ret = recv(sClient, szMessage, MSGSIZE, 0);
    szMessage[ret] = '/0';

    printf("Received [%d bytes]: '%s'/n", ret, szMessage);
}

// Clean up
closesocket(sClient);
WSACleanup();
return 0;
}

客户端所做的事情相当简单，创建套接字，连接服务器，然后不停的发送和接收数据。

比较容易想到的一种服务器模型就是采用一个主线程，负责监听客户端的连接请求，当接收到某个客户端的连接请求后，创建一个专门用于和该客户端通信的套接字和一个辅助线程。以后该客户端和服务器的交互都在这个辅助线程内完成。这种方法比较直观，程序非常简单而且可移植性好，但是不能利用平台相关的特性。例如，如果连接数增多的时候（成千上万的连接），那么线程数成倍增长，操作系统忙于频繁的线程间切换，而且大部分线程在其生命周期内都是处于非活动状态的，这大大浪费了系统的资源。所以，如果你已经知道你的代码只会运行在Windows平台上，建议采用Winsock I/O模型。

一.选择模型
Select（选择）模型是Winsock中最常见的I/O模型。之所以称其为“Select模型”，是由于它的“中心思想”便是利用select函数，实现对I/O的管理。最初设计该模型时，主要面向的是某些使用UNIX操作系统的计算机，它们采用的是Berkeley套接字方案。Select模型已集成到Winsock 1.1中，它使那些想避免在套接字调用过程中被无辜“锁定”的应用程序，采取一种有序的方式，同时进行对多个套接字的管理。由于Winsock 1.1向后兼容于Berkeley套接字实施方案，所以假如有一个Berkeley套接字应用使用了select函数，那么从理论角度讲，毋需对其进行任何修改，便可正常运行。（节选自《Windows网络编程》第八章)
下面的这段程序就是利用选择模型实现的Echo服务器的代码（已经不能再精简了）：

#include <winsock.h>
#include <stdio.h>

#define PORT       5150
#define MSGSIZE    1024

#pragma comment(lib, "ws2_32.lib")

int    g_iTotalConn = 0;
SOCKET g_CliSocketArr[FD_SETSIZE];

DWORD WINAPI WorkerThread(LPVOID lpParameter);

int main()
{
WSADATA     wsaData;
SOCKET      sListen, sClient;
SOCKADDR_IN local, client;
int         iaddrSize = sizeof(SOCKADDR_IN);
DWORD       dwThreadId;

// Initialize Windows socket library
WSAStartup(0x0202, &wsaData);

// Create listening socket
sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Bind
local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
local.sin_family = AF_INET;
local.sin_port = htons(PORT);
bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

// Listen
listen(sListen, 3);

// Create worker thread
CreateThread(NULL, 0, WorkerThread, NULL, 0, &dwThreadId); 

while (TRUE)
{
    // Accept a connection
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d/n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // Add socket to g_CliSocketArr
    g_CliSocketArr[g_iTotalConn++] = sClient;
}

return 0;
}

DWORD WINAPI WorkerThread(LPVOID lpParam)
{
int            i;
fd_set         fdread;
int            ret;
struct timeval tv = {1, 0};
char           szMessage[MSGSIZE];

while (TRUE)
{
    FD_ZERO(&fdread);
    for (i = 0; i < g_iTotalConn; i++)
    {
      FD_SET(g_CliSocketArr[i], &fdread);
    }

    // We only care read event
    ret = select(0, &fdread, NULL, NULL, &tv);

    if (ret == 0)
    {
      // Time expired
      continue;
    }

    for (i = 0; i < g_iTotalConn; i++)
    {
      if (FD_ISSET(g_CliSocketArr[i], &fdread))
      {
        // A read event happened on g_CliSocketArr[i]
        ret = recv(g_CliSocketArr[i], szMessage, MSGSIZE, 0);
     if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
     {
      // Client socket closed
          printf("Client socket %d closed./n", g_CliSocketArr[i]);
      closesocket(g_CliSocketArr[i]);
      if (i < g_iTotalConn - 1)
          {            
            g_CliSocketArr[i--] = g_CliSocketArr[--g_iTotalConn];
          }
        }
     else
     {
      // We received a message from client
          szMessage[ret] = '/0';
      send(g_CliSocketArr[i], szMessage, strlen(szMessage), 0);
        }
      }
    }
}

return 0;
}

服务器的几个主要动作如下：
1.创建监听套接字，绑定，监听；
2.创建工作者线程；
3.创建一个套接字数组，用来存放当前所有活动的客户端套接字，每accept一个连接就更新一次数组；
4.接受客户端的连接。这里有一点需要注意的，就是我没有重新定义FD_SETSIZE宏，所以服务器最多支持的并发连接数为64。而且，这里决不能无条件的accept,服务器应该根据当前的连接数来决定是否接受来自某个客户端的连接。一种比较好的实现方案就是采用WSAAccept函数，而且让WSAAccept回调自己实现的Condition Function。如下所示：

int CALLBACK ConditionFunc(LPWSABUF lpCallerId,LPWSABUF lpCallerData, LPQOS lpSQOS,LPQOS lpGQOS,LPWSABUF lpCalleeId, LPWSABUF lpCalleeData,GROUP FAR * g,DWORD dwCallbackData)
{
if (当前连接数 < FD_SETSIZE)
   return CF_ACCEPT;
else
   return CF_REJECT;
}

工作者线程里面是一个死循环，一次循环完成的动作是：
1.将当前所有的客户端套接字加入到读集fdread中；
2.调用select函数；
3.查看某个套接字是否仍然处于读集中，如果是，则接收数据。如果接收的数据长度为0，或者发生WSAECONNRESET错误，则表示客户端套接字主动关闭，这时需要将服务器中对应的套接字所绑定的资源释放掉，然后调整我们的套接字数组（将数组中最后一个套接字挪到当前的位置上）

除了需要有条件接受客户端的连接外，还需要在连接数为0的情形下做特殊处理，因为如果读集中没有任何套接字，select函数会立刻返回，这将导致工作者线程成为一个毫无停顿的死循环，CPU的占用率马上达到100%。

二.异步选择
Winsock提供了一个有用的异步I/O模型。利用这个模型，应用程序可在一个套接字上，接收以Windows消息为基础的网络事件通知。具体的做法是在建好一个套接字后，调用WSAAsyncSelect函数。该模型最早出现于Winsock的1.1版本中，用于帮助应用程序开发者面向一些早期的16位Windows平台（如Windows for Workgroups），适应其“落后”的多任务消息环境。应用程序仍可从这种模型中得到好处，特别是它们用一个标准的Windows例程（常称为"WndProc"），对窗口消息进行管理的时候。该模型亦得到了Microsoft Foundation Class（微软基本类，MFC）对象CSocket的采纳。（节选自《Windows网络编程》第八章)
我还是先贴出代码，然后做详细解释：
#include <winsock.h>
#include <tchar.h>

#define PORT      5150
#define MSGSIZE   1024
#define WM_SOCKET WM_USER+0

#pragma comment(lib, "ws2_32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
static TCHAR szAppName[] = _T("AsyncSelect Model");
HWND         hwnd ;
MSG          msg ;
WNDCLASS     wndclass ;

wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
wndclass.lpfnWndProc   = WndProc ;
wndclass.cbClsExtra    = 0 ;
wndclass.cbWndExtra    = 0 ;
wndclass.hInstance     = hInstance ;
wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
wndclass.lpszMenuName = NULL ;
wndclass.lpszClassName = szAppName ;

if (!RegisterClass(&wndclass))
{
    MessageBox (NULL, TEXT ("This program requires Windows NT!"), szAppName, MB_ICONERROR) ;
    return 0 ;
}

hwnd = CreateWindow (szAppName,                  // window class name
                       TEXT ("AsyncSelect Model"), // window caption
                       WS_OVERLAPPEDWINDOW,        // window style
                       CW_USEDEFAULT,              // initial x position
                       CW_USEDEFAULT,              // initial y position
                       CW_USEDEFAULT,              // initial x size
                       CW_USEDEFAULT,              // initial y size
                       NULL,                       // parent window handle
                       NULL,                       // window menu handle
                       hInstance,                  // program instance handle
                       NULL) ;                     // creation parameters

ShowWindow(hwnd, iCmdShow);
UpdateWindow(hwnd);

while (GetMessage(&msg, NULL, 0, 0))
{
    TranslateMessage(&msg) ;
    DispatchMessage(&msg) ;
}

return msg.wParam;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
WSADATA       wsd;
static SOCKET sListen;
SOCKET        sClient;
SOCKADDR_IN   local, client;
int           ret, iAddrSize = sizeof(client);
char          szMessage[MSGSIZE];

switch (message)
{
case WM_CREATE:
    // Initialize Windows Socket library
   WSAStartup(0x0202, &wsd);

   // Create listening socket
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   // Bind
    local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
   local.sin_family = AF_INET;
   local.sin_port = htons(PORT);
   bind(sListen, (struct sockaddr *)&local, sizeof(local));

   // Listen
    listen(sListen, 3);

    // Associate listening socket with FD_ACCEPT event
   WSAAsyncSelect(sListen, hwnd, WM_SOCKET, FD_ACCEPT);
   return 0;

case WM_DESTROY:
    closesocket(sListen);
    WSACleanup();
    PostQuitMessage(0);
    return 0;

case WM_SOCKET:
    if (WSAGETSELECTERROR(lParam))
    {
      closesocket(wParam);
      break;
    }

    switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_ACCEPT:
      // Accept a connection from client
      sClient = accept(wParam, (struct sockaddr *)&client, &iAddrSize);

      // Associate client socket with FD_READ and FD_CLOSE event
      WSAAsyncSelect(sClient, hwnd, WM_SOCKET, FD_READ | FD_CLOSE);
      break;

    case FD_READ:
      ret = recv(wParam, szMessage, MSGSIZE, 0);

      if (ret == 0 || ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET)
      {
        closesocket(wParam);
      }
      else
      {
        szMessage[ret] = '/0';
        send(wParam, szMessage, strlen(szMessage), 0);
      }
      break;

    case FD_CLOSE:
      closesocket(wParam);      
      break;
    }
    return 0;
}

return DefWindowProc(hwnd, message, wParam, lParam);
}

在我看来，WSAAsyncSelect是最简单的一种Winsock I/O模型（之所以说它简单是因为一个主线程就搞定了）。使用Raw Windows API写过窗口类应用程序的人应该都能看得懂。这里，我们需要做的仅仅是：
1.在WM_CREATE消息处理函数中，初始化Windows Socket library，创建监听套接字，绑定，监听，并且调用WSAAsyncSelect函数表示我们关心在监听套接字上发生的FD_ACCEPT事件；
2.自定义一个消息WM_SOCKET，一旦在我们所关心的套接字（监听套接字和客户端套接字）上发生了某个事件，系统就会调用WndProc并且message参数被设置为WM_SOCKET；
3.在WM_SOCKET的消息处理函数中，分别对FD_ACCEPT、FD_READ和FD_CLOSE事件进行处理；
4.在窗口销毁消息(WM_DESTROY)的处理函数中，我们关闭监听套接字，清除Windows Socket library

下面这张用于WSAAsyncSelect函数的网络事件类型表可以让你对各个网络事件有更清楚的认识：
表1

|FD_READ|应用程序想要接收有关是否可读的通知，以便读入数据|
|----|----|
|FD_WRITE|应用程序想要接收有关是否可写的通知，以便写入数据|
|FD_OOB|应用程序想接收是否有带外（OOB）数据抵达的通知|
|FD_ACCEPT|应用程序想接收与进入连接有关的通知|
|FD_CONNECT|应用程序想接收与一次连接或者多点join操作完成的通知|
|FD_CLOSE|应用程序想接收与套接字关闭有关的通知|
|FD_QOS|应用程序想接收套接字“服务质量”（QoS）发生更改的通知|
|FD_GROUP_QOS|应用程序想接收套接字组“服务质量”发生更改的通知（现在没什么用处，为未来套接字组的使用保留）|
|FD_ROUTING_INTERFACE_CHANGE|应用程序想接收在指定的方向上，与路由接口发生变化的通知|
|FD_ADDRESS_LIST_CHANGE|应用程序想接收针对套接字的协议家族，本地地址列表发生变化的通知|

三.事件选择
Winsock提供了另一个有用的异步I/O模型。和WSAAsyncSelect模型类似的是，它也允许应用程序在一个或多个套接字上，接收以事件为基础的网络事件通知。对于表1总结的、由WSAAsyncSelect模型采用的网络事件来说，它们均可原封不动地移植到新模型。在用新模型开发的应用程序中，也能接收和处理所有那些事件。该模型最主要的差别在于网络事件会投递至一个事件对象句柄，而非投递至一个窗口例程。（节选自《Windows网络编程》第八章)
还是让我们先看代码然后进行分析：
#include <winsock2.h>
#include <stdio.h>

#define PORT    5150
#define MSGSIZE 1024

#pragma comment(lib, "ws2_32.lib")

int      g_iTotalConn = 0;
SOCKET   g_CliSocketArr[MAXIMUM_WAIT_OBJECTS];
WSAEVENT g_CliEventArr[MAXIMUM_WAIT_OBJECTS];

DWORD WINAPI WorkerThread(LPVOID);
void Cleanup(int index);

int main()
{
WSADATA     wsaData;
SOCKET      sListen, sClient;
SOCKADDR_IN local, client;
DWORD       dwThreadId;
int         iaddrSize = sizeof(SOCKADDR_IN);

// Initialize Windows Socket library
WSAStartup(0x0202, &wsaData);

// Create listening socket
sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Bind
local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
local.sin_family = AF_INET;
local.sin_port = htons(PORT);
bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

// Listen
listen(sListen, 3);

// Create worker thread
CreateThread(NULL, 0, WorkerThread, NULL, 0, &dwThreadId);

while (TRUE)
{
    // Accept a connection
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d/n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // Associate socket with network event
    g_CliSocketArr[g_iTotalConn] = sClient;
    g_CliEventArr[g_iTotalConn] = WSACreateEvent();
    WSAEventSelect(g_CliSocketArr[g_iTotalConn],
                   g_CliEventArr[g_iTotalConn],
                   FD_READ | FD_CLOSE);
    g_iTotalConn++;
}
}

DWORD WINAPI WorkerThread(LPVOID lpParam)
{
int              ret, index;
WSANETWORKEVENTS NetworkEvents;
char             szMessage[MSGSIZE];

while (TRUE)
{
    ret = WSAWaitForMultipleEvents(g_iTotalConn, g_CliEventArr, FALSE, 1000, FALSE);
    if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
    {
      continue;
    }

    index = ret - WSA_WAIT_EVENT_0;
    WSAEnumNetworkEvents(g_CliSocketArr[index], g_CliEventArr[index], &NetworkEvents);

    if (NetworkEvents.lNetworkEvents & FD_READ)
    {
      // Receive message from client
      ret = recv(g_CliSocketArr[index], szMessage, MSGSIZE, 0);
      if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
      {
        Cleanup(index);
      }
      else
      {
        szMessage[ret] = '/0';
        send(g_CliSocketArr[index], szMessage, strlen(szMessage), 0);
      }
    }

    if (NetworkEvents.lNetworkEvents & FD_CLOSE)
   {
    Cleanup(index);
   }
}
return 0;
}

void Cleanup(int index)
{
closesocket(g_CliSocketArr[index]);
WSACloseEvent(g_CliEventArr[index]);

if (index < g_iTotalConn - 1)
{
   g_CliSocketArr[index] = g_CliSocketArr[g_iTotalConn - 1];
   g_CliEventArr[index] = g_CliEventArr[g_iTotalConn - 1];
}

g_iTotalConn--;
}

事件选择模型也比较简单，实现起来也不是太复杂，它的基本思想是将每个套接字都和一个WSAEVENT对象对应起来，并且在关联的时候指定需要关注的哪些网络事件。一旦在某个套接字上发生了我们关注的事件（FD_READ和FD_CLOSE），与之相关联的WSAEVENT对象被Signaled。程序定义了两个全局数组，一个套接字数组，一个WSAEVENT对象数组，其大小都是MAXIMUM_WAIT_OBJECTS（64），两个数组中的元素一一对应。
同样的，这里的程序没有考虑两个问题，一是不能无条件的调用accept，因为我们支持的并发连接数有限。解决方法是将套接字按MAXIMUM_WAIT_OBJECTS分组，每MAXIMUM_WAIT_OBJECTS个套接字一组，每一组分配一个工作者线程；或者采用WSAAccept代替accept，并回调自己定义的Condition Function。第二个问题是没有对连接数为0的情形做特殊处理，程序在连接数为0的时候CPU占用率为100%。

四.重叠I/O模型
Winsock2的发布使得Socket I/O有了和文件I/O统一的接口。我们可以通过使用Win32文件操纵函数ReadFile和WriteFile来进行Socket I/O。伴随而来的，用于普通文件I/O的重叠I/O模型和完成端口模型对Socket I/O也适用了。这些模型的优点是可以达到更佳的系统性能，但是实现较为复杂，里面涉及较多的C语言技巧。例如我们在完成端口模型中会经常用到所谓的“尾 随数据”。

1.用事件通知方式实现的重叠I/O模型
#include <winsock2.h>
#include <stdio.h>

#define PORT    5150
#define MSGSIZE 1024

#pragma comment(lib, "ws2_32.lib")

typedef struct
{
WSAOVERLAPPED overlap;
WSABUF        Buffer;
char          szMessage[MSGSIZE];
DWORD         NumberOfBytesRecvd;
DWORD         Flags;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

int                     g_iTotalConn = 0;
SOCKET                  g_CliSocketArr[MAXIMUM_WAIT_OBJECTS];
WSAEVENT                g_CliEventArr[MAXIMUM_WAIT_OBJECTS];
LPPER_IO_OPERATION_DATA g_pPerIODataArr[MAXIMUM_WAIT_OBJECTS];

DWORD WINAPI WorkerThread(LPVOID);
void Cleanup(int);

int main()
{
WSADATA     wsaData;
SOCKET      sListen, sClient;
SOCKADDR_IN local, client;
DWORD       dwThreadId;
int         iaddrSize = sizeof(SOCKADDR_IN);

// Initialize Windows Socket library
WSAStartup(0x0202, &wsaData);

// Create listening socket
sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Bind
local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
local.sin_family = AF_INET;
local.sin_port = htons(PORT);
bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

// Listen
listen(sListen, 3);

// Create worker thread
CreateThread(NULL, 0, WorkerThread, NULL, 0, &dwThreadId);

while (TRUE)
{
    // Accept a connection
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d/n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    g_CliSocketArr[g_iTotalConn] = sClient;

    // Allocate a PER_IO_OPERATION_DATA structure
    g_pPerIODataArr[g_iTotalConn] = (LPPER_IO_OPERATION_DATA)HeapAlloc(
      GetProcessHeap(),
      HEAP_ZERO_MEMORY,
      sizeof(PER_IO_OPERATION_DATA));
    g_pPerIODataArr[g_iTotalConn]->Buffer.len = MSGSIZE;
    g_pPerIODataArr[g_iTotalConn]->Buffer.buf = g_pPerIODataArr[g_iTotalConn]->szMessage;
    g_CliEventArr[g_iTotalConn] = g_pPerIODataArr[g_iTotalConn]->overlap.hEvent = WSACreateEvent();

    // Launch an asynchronous operation
    WSARecv(
      g_CliSocketArr[g_iTotalConn],
      &g_pPerIODataArr[g_iTotalConn]->Buffer,
      1,
      &g_pPerIODataArr[g_iTotalConn]->NumberOfBytesRecvd,
      &g_pPerIODataArr[g_iTotalConn]->Flags,
      &g_pPerIODataArr[g_iTotalConn]->overlap,
      NULL);

    g_iTotalConn++;
}

closesocket(sListen);
WSACleanup();
return 0;
}

DWORD WINAPI WorkerThread(LPVOID lpParam)
{
int   ret, index;
DWORD cbTransferred;

while (TRUE)
{
    ret = WSAWaitForMultipleEvents(g_iTotalConn, g_CliEventArr, FALSE, 1000, FALSE);
    if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
    {
      continue;
    }

    index = ret - WSA_WAIT_EVENT_0;
    WSAResetEvent(g_CliEventArr[index]);

    WSAGetOverlappedResult(
      g_CliSocketArr[index],
      &g_pPerIODataArr[index]->overlap,
      &cbTransferred,
      TRUE,
      &g_pPerIODataArr[g_iTotalConn]->Flags);

    if (cbTransferred == 0)
    {
      // The connection was closed by client
      Cleanup(index);
    }
    else
    {
      // g_pPerIODataArr[index]->szMessage contains the received data
      g_pPerIODataArr[index]->szMessage[cbTransferred] = '/0';
      send(g_CliSocketArr[index], g_pPerIODataArr[index]->szMessage,/
        cbTransferred, 0);

      // Launch another asynchronous operation
      WSARecv(
        g_CliSocketArr[index],
        &g_pPerIODataArr[index]->Buffer,
        1,
        &g_pPerIODataArr[index]->NumberOfBytesRecvd,
        &g_pPerIODataArr[index]->Flags,
        &g_pPerIODataArr[index]->overlap,
        NULL);
    }
}

return 0;
}

void Cleanup(int index)
{
closesocket(g_CliSocketArr[index]);
WSACloseEvent(g_CliEventArr[index]);
HeapFree(GetProcessHeap(), 0, g_pPerIODataArr[index]);

if (index < g_iTotalConn - 1)
{
    g_CliSocketArr[index] = g_CliSocketArr[g_iTotalConn - 1];
    g_CliEventArr[index] = g_CliEventArr[g_iTotalConn - 1];
    g_pPerIODataArr[index] = g_pPerIODataArr[g_iTotalConn - 1];
}

g_pPerIODataArr[--g_iTotalConn] = NULL;
}

这个模型与上述其他模型不同的是它使用Winsock2提供的异步I/O函数WSARecv。在调用WSARecv时，指定一个 WSAOVERLAPPED结构，这个调用不是阻塞的，也就是说，它会立刻返回。一旦有数据到达的时候，被指定的WSAOVERLAPPED结构中的 hEvent被Signaled。由于下面这个语句
g_CliEventArr[g_iTotalConn] = g_pPerIODataArr[g_iTotalConn]->overlap.hEvent；
使 得与该套接字相关联的WSAEVENT对象也被Signaled，所以WSAWaitForMultipleEvents的调用操作成功返回。我们现在应 该做的就是用与调用WSARecv相同的WSAOVERLAPPED结构为参数调用WSAGetOverlappedResult，从而得到本次I/O传 送的字节数等相关信息。在取得接收的数据后，把数据原封不动的发送到客户端，然后重新激活一个WSARecv异步操作。

2.用完成例程方式实现的重叠I/O模型
#include <WINSOCK2.H>
#include <stdio.h>

#define PORT    5150
#define MSGSIZE 1024

#pragma comment(lib, "ws2_32.lib")

typedef struct
{
WSAOVERLAPPED overlap;
WSABUF        Buffer;
char          szMessage[MSGSIZE];
DWORD         NumberOfBytesRecvd;
DWORD         Flags; 
SOCKET        sClient;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

DWORD WINAPI WorkerThread(LPVOID);
void CALLBACK CompletionROUTINE(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);

SOCKET g_sNewClientConnection;
BOOL   g_bNewConnectionArrived = FALSE;

int main()
{
WSADATA     wsaData;
SOCKET      sListen;
SOCKADDR_IN local, client;
DWORD       dwThreadId;
int         iaddrSize = sizeof(SOCKADDR_IN);

// Initialize Windows Socket library
WSAStartup(0x0202, &wsaData);

// Create listening socket
sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Bind
local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
local.sin_family = AF_INET;
local.sin_port = htons(PORT);
bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

// Listen
listen(sListen, 3);

// Create worker thread
CreateThread(NULL, 0, WorkerThread, NULL, 0, &dwThreadId);

while (TRUE)
{
    // Accept a connection
    g_sNewClientConnection = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    g_bNewConnectionArrived = TRUE;
    printf("Accepted client:%s:%d/n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
}
}

DWORD WINAPI WorkerThread(LPVOID lpParam)
{
LPPER_IO_OPERATION_DATA lpPerIOData = NULL;

while (TRUE)
{
    if (g_bNewConnectionArrived)
    {
      // Launch an asynchronous operation for new arrived connection
      lpPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(
        GetProcessHeap(),
        HEAP_ZERO_MEMORY,
        sizeof(PER_IO_OPERATION_DATA));
      lpPerIOData->Buffer.len = MSGSIZE;
      lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
      lpPerIOData->sClient = g_sNewClientConnection;

      WSARecv(lpPerIOData->sClient,
        &lpPerIOData->Buffer,
        1,
        &lpPerIOData->NumberOfBytesRecvd,
        &lpPerIOData->Flags,
        &lpPerIOData->overlap,
        CompletionROUTINE);      

      g_bNewConnectionArrived = FALSE;
    }

    SleepEx(1000, TRUE);
}
return 0;
}

void CALLBACK CompletionROUTINE(DWORD dwError,
                                DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped,
                                DWORD dwFlags)
{
LPPER_IO_OPERATION_DATA lpPerIOData = (LPPER_IO_OPERATION_DATA)lpOverlapped;

if (dwError != 0 || cbTransferred == 0)
{
    // Connection was closed by client
   closesocket(lpPerIOData->sClient);
   HeapFree(GetProcessHeap(), 0, lpPerIOData);
}
else
{
    lpPerIOData->szMessage[cbTransferred] = '/0';
    send(lpPerIOData->sClient, lpPerIOData->szMessage, cbTransferred, 0);

    // Launch another asynchronous operation
    memset(&lpPerIOData->overlap, 0, sizeof(WSAOVERLAPPED));
    lpPerIOData->Buffer.len = MSGSIZE;
    lpPerIOData->Buffer.buf = lpPerIOData->szMessage;   

    WSARecv(lpPerIOData->sClient,
      &lpPerIOData->Buffer,
      1,
      &lpPerIOData->NumberOfBytesRecvd,
      &lpPerIOData->Flags,
      &lpPerIOData->overlap,
      CompletionROUTINE);
}
}

用 完成例程来实现重叠I/O比用事件通知简单得多。在这个模型中，主线程只用不停的接受连接即可；辅助线程判断有没有新的客户端连接被建立，如果有，就为那 个客户端套接字激活一个异步的WSARecv操作，然后调用SleepEx使线程处于一种可警告的等待状态，以使得I/O完成后 CompletionROUTINE可以被内核调用。如果辅助线程不调用SleepEx，则内核在完成一次I/O操作后，无法调用完成例程（因为完成例程 的运行应该和当初激活WSARecv异步操作的代码在同一个线程之内）。
完成例程内的实现代码比较简单，它取出接收到的数据，然后将数据原封不动 的发送给客户端，最后重新激活另一个WSARecv异步操作。注意，在这里用到了“尾随数据”。我们在调用WSARecv的时候，参数 lpOverlapped实际上指向一个比它大得多的结构PER_IO_OPERATION_DATA，这个结构除了WSAOVERLAPPED以外，还 被我们附加了缓冲区的结构信息，另外还包括客户端套接字等重要的信息。这样，在完成例程中通过参数lpOverlapped拿到的不仅仅是 WSAOVERLAPPED结构，还有后边尾随的包含客户端套接字和接收数据缓冲区等重要信息。这样的C语言技巧在我后面介绍完成端口的时候还会使用到。

五.完成端口模型
“完 成端口”模型是迄今为止最为复杂的一种I/O模型。然而，假若一个应用程序同时需要管理为数众多的套接字，那么采用这种模型，往往可以达到最佳的系统性 能！但不幸的是，该模型只适用于Windows NT和Windows 2000操作系统。因其设计的复杂性，只有在你的应用程序需要同时管理数百乃至上千个套接字的时候，而且希望随着系统内安装的CPU数量的增多，应用程序 的性能也可以线性提升，才应考虑采用“完成端口”模型。要记住的一个基本准则是，假如要为Windows NT或Windows 2000开发高性能的服务器应用，同时希望为大量套接字I/O请求提供服务（Web服务器便是这方面的典型例子），那么I/O完成端口模型便是最佳选择！ （节选自《Windows网络编程》第八章）
完成端口模型是我最喜爱的一种模型。虽然其实现比较复杂（其实我觉得它的实现比用事件通知实现的重叠 I/O简单多了），但其效率是惊人的。我在T公司的时候曾经帮同事写过一个邮件服务器的性能测试程序，用的就是完成端口模型。结果表明，完成端口模型在多 连接（成千上万）的情况下，仅仅依靠一两个辅助线程，就可以达到非常高的吞吐量。下面我还是从代码说起：
#include <WINSOCK2.H>
#include <stdio.h>

#define PORT    5150
#define MSGSIZE 1024

#pragma comment(lib, "ws2_32.lib")

typedef enum
{
RECV_POSTED
}OPERATION_TYPE;

typedef struct
{
WSAOVERLAPPED overlap;
WSABUF         Buffer;
char           szMessage[MSGSIZE];
DWORD          NumberOfBytesRecvd;
DWORD          Flags;
OPERATION_TYPE OperationType;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

DWORD WINAPI WorkerThread(LPVOID);

int main()
{
WSADATA                 wsaData;
SOCKET                  sListen, sClient;
SOCKADDR_IN             local, client;
DWORD                   i, dwThreadId;
int                     iaddrSize = sizeof(SOCKADDR_IN);
HANDLE                  CompletionPort = INVALID_HANDLE_VALUE;
SYSTEM_INFO             systeminfo;
LPPER_IO_OPERATION_DATA lpPerIOData = NULL;

// Initialize Windows Socket library
WSAStartup(0x0202, &wsaData);

// Create completion port
CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

// Create worker thread
GetSystemInfo(&systeminfo);
for (i = 0; i < systeminfo.dwNumberOfProcessors; i++)
{
    CreateThread(NULL, 0, WorkerThread, CompletionPort, 0, &dwThreadId);
}

// Create listening socket
sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Bind
local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
local.sin_family = AF_INET;
local.sin_port = htons(PORT);
bind(sListen, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));

// Listen
listen(sListen, 3);

while (TRUE)
{
    // Accept a connection
    sClient = accept(sListen, (struct sockaddr *)&client, &iaddrSize);
    printf("Accepted client:%s:%d/n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // Associate the newly arrived client socket with completion port
    CreateIoCompletionPort((HANDLE)sClient, CompletionPort, (DWORD)sClient, 0);

    // Launch an asynchronous operation for new arrived connection
    lpPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(
      GetProcessHeap(),
      HEAP_ZERO_MEMORY,
      sizeof(PER_IO_OPERATION_DATA));
    lpPerIOData->Buffer.len = MSGSIZE;
    lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
    lpPerIOData->OperationType = RECV_POSTED;
    WSARecv(sClient,
      &lpPerIOData->Buffer,
      1,
      &lpPerIOData->NumberOfBytesRecvd,
      &lpPerIOData->Flags,
      &lpPerIOData->overlap,
      NULL);
}

PostQueuedCompletionStatus(CompletionPort, 0xFFFFFFFF, 0, NULL);
CloseHandle(CompletionPort);
closesocket(sListen);
WSACleanup();
return 0;
}

DWORD WINAPI WorkerThread(LPVOID CompletionPortID)
{
HANDLE                  CompletionPort=(HANDLE)CompletionPortID;
DWORD                   dwBytesTransferred;
SOCKET                  sClient;
LPPER_IO_OPERATION_DATA lpPerIOData = NULL;

while (TRUE)
{
    GetQueuedCompletionStatus(
      CompletionPort,
      &dwBytesTransferred,
      &sClient,
      (LPOVERLAPPED *)&lpPerIOData,
      INFINITE);
    if (dwBytesTransferred == 0xFFFFFFFF)
    {
      return 0;
    }

    if (lpPerIOData->OperationType == RECV_POSTED)
    {
      if (dwBytesTransferred == 0)
      {
        // Connection was closed by client
        closesocket(sClient);
        HeapFree(GetProcessHeap(), 0, lpPerIOData);        
      }
      else
      {
        lpPerIOData->szMessage[dwBytesTransferred] = '/0';
        send(sClient, lpPerIOData->szMessage, dwBytesTransferred, 0);

        // Launch another asynchronous operation for sClient
        memset(lpPerIOData, 0, sizeof(PER_IO_OPERATION_DATA));
        lpPerIOData->Buffer.len = MSGSIZE;
        lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
        lpPerIOData->OperationType = RECV_POSTED;
        WSARecv(sClient,
          &lpPerIOData->Buffer,
          1,
          &lpPerIOData->NumberOfBytesRecvd,
          &lpPerIOData->Flags,
          &lpPerIOData->overlap,
          NULL);
      }
    }
}
return 0;
}

首先，说说主线程：
1.创建完成端口对象
2.创建工作者线程（这里工作者线程的数量是按照CPU的个数来决定的，这样可以达到最佳性能）
3.创建监听套接字，绑定，监听，然后程序进入循环
4.在循环中，我做了以下几件事情：
(1).接受一个客户端连接
(2). 将该客户端套接字与完成端口绑定到一起(还是调用CreateIoCompletionPort，但这次的作用不同)，注意，按道理来讲，此时传递给 CreateIoCompletionPort的第三个参数应该是一个完成键，一般来讲，程序都是传递一个单句柄数据结构的地址，该单句柄数据包含了和该 客户端连接有关的信息，由于我们只关心套接字句柄，所以直接将套接字句柄作为完成键传递；
(3).触发一个WSARecv异步调用，这次又用到了“尾随数据”，使接收数据所用的缓冲区紧跟在WSAOVERLAPPED对象之后，此外，还有操作类型等重要信息。

在工作者线程的循环中，我们
1.调用GetQueuedCompletionStatus取得本次I/O的相关信息（例如套接字句柄、传送的字节数、单I/O数据结构的地址等等）
2.通过单I/O数据结构找到接收数据缓冲区，然后将数据原封不动的发送到客户端
3.再次触发一个WSARecv异步操作

六.五种I/O模型的比较
我会从以下几个方面来进行比较
*有无每线程64连接数限制
如 果在选择模型中没有重新定义FD_SETSIZE宏，则每个fd_set默认可以装下64个SOCKET。同样的，受 MAXIMUM_WAIT_OBJECTS宏的影响，事件选择、用事件通知实现的重叠I/O都有每线程最大64连接数限制。如果连接数成千上万，则必须对 客户端套接字进行分组，这样，势必增加程序的复杂度。
相反，异步选择、用完成例程实现的重叠I/O和完成端口不受此限制。

*线程数
除了异步选择以外，其他模型至少需要2个线程。一个主线程和一个辅助线程。同样的，如果连接数大于64，则选择模型、事件选择和用事件通知实现的重叠I/O的线程数还要增加。

*实现的复杂度
我的个人看法是，在实现难度上，异步选择<选择<用完成例程实现的重叠I/O<事件选择<完成端口<用事件通知实现的重叠I/O

*性能
由 于选择模型中每次都要重设读集，在select函数返回后还要针对所有套接字进行逐一测试，我的感觉是效率比较差；完成端口和用完成例程实现的重叠I/O 基本上不涉及全局数据，效率应该是最高的，而且在多处理器情形下完成端口还要高一些；事件选择和用事件通知实现的重叠I/O在实现机制上都是采用 WSAWaitForMultipleEvents，感觉效率差不多；至于异步选择，不好比较。所以我的结论是:选择<用事件通知实现的重叠 I/O<事件选择<用完成例程实现的重叠I/O<完成端口。


