# Windows Socket1.1 程序设计 - 深之JohnChen的专栏 - CSDN博客

2005年12月11日 19:22:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1611标签：[windows																[socket																[sockets																[dll																[pascal																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=pascal&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=sockets&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[网络编程](https://blog.csdn.net/byxdaz/article/category/166215)


一、简介

　　Windows Sockets 是从 Berkeley Sockets 扩展而来的，其在继承 Berkeley Sockets 的基础上，又进行了新的扩充。这些扩充主要是提供了一些异步函数，并增加了符合WINDOWS消息驱动特性的网络事件异步选择机制。

　　Windows Sockets由两部分组成：开发组件和运行组件。

　　开发组件：Windows Sockets 实现文档、应用程序接口(API)引入库和一些头文件。

　　运行组件：Windows Sockets 应用程序接口的动态链接库(WINSOCK.DLL)。 

　　二、主要扩充说明

　　1、异步选择机制：

　　Windows Sockets 的异步选择函数提供了消息机制的网络事件选择，当使用它登记网络事件发生时，应用程序相应窗口函数将收到一个消息，消息中指示了发生的网络事件，以及与事件相关的一些信息。

　　Windows Sockets 提供了一个异步选择函数 WSAAsyncSelect()，用它来注册应用程序感兴趣的网络事件，当这些事件发生时，应用程序相应的窗口函数将收到一个消息。

　　函数结构如下：

int PASCAL FAR WSAAsyncSelect(SOCKET s,HWND hWnd,unsigned int wMsg,long lEvent);
　　参数说明：

　　　hWnd：窗口句柄

　　　wMsg：需要发送的消息

　　　lEvent：事件（以下为事件的内容）

|值：|含义：|
|----|----|
|FD_READ|期望在套接字上收到数据（即读准备好）时接到通知|
|FD_WRITE|期望在套接字上可发送数据（即写准备好）时接到通知|
|FD_OOB|期望在套接字上有带外数据到达时接到通知|
|FD_ACCEPT|期望在套接字上有外来连接时接到通知|
|FD_CONNECT|期望在套接字连接建立完成时接到通知|
|FD_CLOSE|期望在套接字关闭时接到通知|
　　例如：我们要在套接字读准备好或写准备好时接到通知，语句如下： 

rc=WSAAsyncSelect(s,hWnd,wMsg,FD_READ|FD_WRITE);
　　如果我们需要注销对套接字网络事件的消息发送，只要将 lEvent 设置为0 

　　2、异步请求函数

　　在 Berkeley Sockets 中请求服务是阻塞的，WINDOWS SICKETS 除了支持这一类函数外，还增加了相应的异步请求函数(WSAAsyncGetXByY();)。 

　　3、阻塞处理方法

　　Windows Sockets 为了实现当一个应用程序的套接字调用处于阻塞时，能够放弃CPU让其它应用程序运行，它在调用处于阻塞时便进入一个叫“HOOK”的例程，此例程负责接收和分配WINDOWS消息，使得其它应用程序仍然能够接收到自己的消息并取得控制权。

　　WINDOWS 是非抢先的多任务环境，即若一个程序不主动放弃其控制权，别的程序就不能执行。因此在设计Windows Sockets 程序时，尽管系统支持阻塞操作，但还是反对程序员使用该操作。但由于 SUN 公司下的 Berkeley Sockets 的套接字默认操作是阻塞的，WINDOWS 作为移植的 SOCKETS 也不可避免对这个操作支持。

　　在Windows Sockets 实现中，对于不能立即完成的阻塞操作做如下处理：DLL初始化→循环操作。在循环中，它发送任何 WINDOWS 消息，并检查这个 Windows Sockets 调用是否完成，在必要时，它可以放弃CPU让其它应用程序执行（当然使用超线程的CPU就不会有这个麻烦了^_^）。我们可以调用 WSACancelBlockingCall() 函数取消此阻塞操作。

　　在 Windows Sockets 中，有一个默认的阻塞处理例程 BlockingHook() 简单地获取并发送 WINDOWS 消息。如果要对复杂程序进行处理，Windows Sockets 中还有 WSASetBlockingHook() 提供用户安装自己的阻塞处理例程能力；与该函数相对应的则是 SWAUnhookBlockingHook()，它用于删除先前安装的任何阻塞处理例程，并重新安装默认的处理例程。请注意，设计自己的阻塞处理例程时，除了函数 WSACancelBlockingHook() 之外，它不能使用其它的 Windows Sockets API 函数。在处理例程中调用 WSACancelBlockingHook()函数将取消处于阻塞的操作，它将结束阻塞循环。

　　4、出错处理

　　Windows Sockets 为了和以后多线程环境（WINDOWS/UNIX）兼容，它提供了两个出错处理函数来获取和设置当前线程的最近错误号。（WSAGetLastEror()和WSASetLastError()）

　　5、启动与终止

　　使用函数 WSAStartup() 和 WSACleanup() 启动和终止套接字。

**三、Windows Sockets网络程序设计核心**

　　我们终于可以开始真正的 Windows Sockets 网络程序设计了。不过我们还是先看一看每个 Windows Sockets 网络程序都要涉及的内容。让我们一步步慢慢走。

　　1、启动与终止

　　在所有 Windows Sockets 函数中，只有启动函数 WSAStartup() 和终止函数 WSACleanup() 是必须使用的。

　　启动函数必须是第一个使用的函数，而且它允许指定 Windows Sockets API 的版本，并获得 SOCKETS的特定的一些技术细节。本结构如下：

int PASCAL FAR WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
　　其中 wVersionRequested 保证 SOCKETS 可正常运行的 DLL 版本，如果不支持，则返回错误信息。
我们看一下下面这段代码，看一下如何进行 WSAStartup() 的调用

WORD wVersionRequested;// 定义版本信息变量
WSADATA wsaData;//定义数据信息变量
int err;//定义错误号变量
wVersionRequested = MAKEWORD(1,1);//给版本信息赋值
err = WSAStartup(wVersionRequested, &wsaData);//给错误信息赋值
if(err!=0)
{
return;//告诉用户找不到合适的版本
}
//确认 Windows Sockets DLL 支持 1.1 版本
//DLL 版本可以高于 1.1
//系统返回的版本号始终是最低要求的 1.1，即应用程序与DLL 中可支持的最低版本号
if(LOBYTE(wsaData.wVersion)!= 1|| HIBYTE(wsaData.wVersion)!=1)
{
WSACleanup();//告诉用户找不到合适的版本
return;
}
//Windows Sockets DLL 被进程接受，可以进入下一步操作
　　关闭函数使用时，任何打开并已连接的 SOCK_STREAM 套接字被复位，但那些已由 closesocket() 函数关闭的但仍有未发送数据的套接字不受影响，未发送的数据仍将被发送。程序运行时可能会多次调用 WSAStartuo() 函数，但必须保证每次调用时的 wVersionRequested 的值是相同的。

　　2、异步请求服务

　　Windows Sockets 除支持 Berkeley Sockets 中同步请求，还增加了了一类异步请求服务函数 WSAAsyncGerXByY()。该函数是阻塞请求函数的异步版本。应用程序调用它时，由 Windows Sockets DLL 初始化这一操作并返回调用者，此函数返回一个异步句柄，用来标识这个操作。当结果存储在调用者提供的缓冲区，并且发送一个消息到应用程序相应窗口。常用结构如下：

HANDLE taskHnd;
char hostname="rs6000";
taskHnd = WSAAsyncBetHostByName(hWnd,wMsg,hostname,buf,buflen); 
　　需要注意的是，由于 Windows 的内存对像可以设置为可移动和可丢弃，因此在操作内存对象是，必须保证 WIindows Sockets DLL 对象是可用的。 

　　3、异步数据传输

　　使用 send() 或 sendto() 函数来发送数据，使用 recv() 或recvfrom() 来接收数据。Windows Sockets 不鼓励用户使用阻塞方式传输数据，因为那样可能会阻塞整个 Windows 环境。下面我们看一个异步数据传输实例：

　　假设套接字 s 在连接建立后，已经使用了函数 WSAAsyncSelect() 在其上注册了网络事件 FD_READ 和 FD_WRITE，并且 wMsg 值为 UM_SOCK，那么我们可以在 Windows 消息循环中增加如下的分支语句：

case UM_SOCK:
switch(lParam)
{
case FD_READ:
len = recv(wParam,lpBuffer,length,0);
break;
case FD_WRITE:
while(send(wParam,lpBuffer,len,0)!=SOCKET_ERROR)
break;
}
break;
　　4、出错处理

　　Windows 提供了一个函数来获取最近的错误码 WSAGetLastError()，推荐的编写方式如下： 

len = send (s,lpBuffer,len,0);
of((len==SOCKET_ERROR)&&(WSAGetLastError()==WSAWOULDBLOCK)){...}
　　二、主要扩充说明

　　1、异步选择机制：

　　Windows Sockets 的异步选择函数提供了消息机制的网络事件选择，当使用它登记网络事件发生时，应用程序相应窗口函数将收到一个消息，消息中指示了发生的网络事件，以及与事件相关的一些信息。

　　Windows Sockets 提供了一个异步选择函数 WSAAsyncSelect()，用它来注册应用程序感兴趣的网络事件，当这些事件发生时，应用程序相应的窗口函数将收到一个消息。

　　函数结构如下：

int PASCAL FAR WSAAsyncSelect(SOCKET s,HWND hWnd,unsigned int wMsg,long lEvent);
　　参数说明：

　　　hWnd：窗口句柄

　　　wMsg：需要发送的消息

　　　lEvent：事件（以下为事件的内容）

|值：|含义：|
|----|----|
|FD_READ|期望在套接字上收到数据（即读准备好）时接到通知|
|FD_WRITE|期望在套接字上可发送数据（即写准备好）时接到通知|
|FD_OOB|期望在套接字上有带外数据到达时接到通知|
|FD_ACCEPT|期望在套接字上有外来连接时接到通知|
|FD_CONNECT|期望在套接字连接建立完成时接到通知|
|FD_CLOSE|期望在套接字关闭时接到通知|
　　例如：我们要在套接字读准备好或写准备好时接到通知，语句如下： 

rc=WSAAsyncSelect(s,hWnd,wMsg,FD_READ|FD_WRITE);
　　如果我们需要注销对套接字网络事件的消息发送，只要将 lEvent 设置为0 

　　2、异步请求函数

　　在 Berkeley Sockets 中请求服务是阻塞的，WINDOWS SICKETS 除了支持这一类函数外，还增加了相应的异步请求函数(WSAAsyncGetXByY();)。 

　　3、阻塞处理方法

　　Windows Sockets 为了实现当一个应用程序的套接字调用处于阻塞时，能够放弃CPU让其它应用程序运行，它在调用处于阻塞时便进入一个叫“HOOK”的例程，此例程负责接收和分配WINDOWS消息，使得其它应用程序仍然能够接收到自己的消息并取得控制权。

　　WINDOWS 是非抢先的多任务环境，即若一个程序不主动放弃其控制权，别的程序就不能执行。因此在设计Windows Sockets 程序时，尽管系统支持阻塞操作，但还是反对程序员使用该操作。但由于 SUN 公司下的 Berkeley Sockets 的套接字默认操作是阻塞的，WINDOWS 作为移植的 SOCKETS 也不可避免对这个操作支持。

　　在Windows Sockets 实现中，对于不能立即完成的阻塞操作做如下处理：DLL初始化→循环操作。在循环中，它发送任何 WINDOWS 消息，并检查这个 Windows Sockets 调用是否完成，在必要时，它可以放弃CPU让其它应用程序执行（当然使用超线程的CPU就不会有这个麻烦了^_^）。我们可以调用 WSACancelBlockingCall() 函数取消此阻塞操作。

　　在 Windows Sockets 中，有一个默认的阻塞处理例程 BlockingHook() 简单地获取并发送 WINDOWS 消息。如果要对复杂程序进行处理，Windows Sockets 中还有 WSASetBlockingHook() 提供用户安装自己的阻塞处理例程能力；与该函数相对应的则是 SWAUnhookBlockingHook()，它用于删除先前安装的任何阻塞处理例程，并重新安装默认的处理例程。请注意，设计自己的阻塞处理例程时，除了函数 WSACancelBlockingHook() 之外，它不能使用其它的 Windows Sockets API 函数。在处理例程中调用 WSACancelBlockingHook()函数将取消处于阻塞的操作，它将结束阻塞循环。

　　4、出错处理

　　Windows Sockets 为了和以后多线程环境（WINDOWS/UNIX）兼容，它提供了两个出错处理函数来获取和设置当前线程的最近错误号。（WSAGetLastEror()和WSASetLastError()）

　　5、启动与终止

　　使用函数 WSAStartup() 和 WSACleanup() 启动和终止套接字。

**三、Windows Sockets网络程序设计核心**

　　我们终于可以开始真正的 Windows Sockets 网络程序设计了。不过我们还是先看一看每个 Windows Sockets 网络程序都要涉及的内容。让我们一步步慢慢走。

　　1、启动与终止

　　在所有 Windows Sockets 函数中，只有启动函数 WSAStartup() 和终止函数 WSACleanup() 是必须使用的。

　　启动函数必须是第一个使用的函数，而且它允许指定 Windows Sockets API 的版本，并获得 SOCKETS的特定的一些技术细节。本结构如下：

int PASCAL FAR WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
　　其中 wVersionRequested 保证 SOCKETS 可正常运行的 DLL 版本，如果不支持，则返回错误信息。
我们看一下下面这段代码，看一下如何进行 WSAStartup() 的调用

WORD wVersionRequested;// 定义版本信息变量
WSADATA wsaData;//定义数据信息变量
int err;//定义错误号变量
wVersionRequested = MAKEWORD(1,1);//给版本信息赋值
err = WSAStartup(wVersionRequested, &wsaData);//给错误信息赋值
if(err!=0)
{
return;//告诉用户找不到合适的版本
}
//确认 Windows Sockets DLL 支持 1.1 版本
//DLL 版本可以高于 1.1
//系统返回的版本号始终是最低要求的 1.1，即应用程序与DLL 中可支持的最低版本号
if(LOBYTE(wsaData.wVersion)!= 1|| HIBYTE(wsaData.wVersion)!=1)
{
WSACleanup();//告诉用户找不到合适的版本
return;
}
//Windows Sockets DLL 被进程接受，可以进入下一步操作
　　关闭函数使用时，任何打开并已连接的 SOCK_STREAM 套接字被复位，但那些已由 closesocket() 函数关闭的但仍有未发送数据的套接字不受影响，未发送的数据仍将被发送。程序运行时可能会多次调用 WSAStartuo() 函数，但必须保证每次调用时的 wVersionRequested 的值是相同的。

　　2、异步请求服务

　　Windows Sockets 除支持 Berkeley Sockets 中同步请求，还增加了了一类异步请求服务函数 WSAAsyncGerXByY()。该函数是阻塞请求函数的异步版本。应用程序调用它时，由 Windows Sockets DLL 初始化这一操作并返回调用者，此函数返回一个异步句柄，用来标识这个操作。当结果存储在调用者提供的缓冲区，并且发送一个消息到应用程序相应窗口。常用结构如下：

HANDLE taskHnd;
char hostname="rs6000";
taskHnd = WSAAsyncBetHostByName(hWnd,wMsg,hostname,buf,buflen); 
　　需要注意的是，由于 Windows 的内存对像可以设置为可移动和可丢弃，因此在操作内存对象是，必须保证 WIindows Sockets DLL 对象是可用的。 

　　3、异步数据传输

　　使用 send() 或 sendto() 函数来发送数据，使用 recv() 或recvfrom() 来接收数据。Windows Sockets 不鼓励用户使用阻塞方式传输数据，因为那样可能会阻塞整个 Windows 环境。下面我们看一个异步数据传输实例：

　　假设套接字 s 在连接建立后，已经使用了函数 WSAAsyncSelect() 在其上注册了网络事件 FD_READ 和 FD_WRITE，并且 wMsg 值为 UM_SOCK，那么我们可以在 Windows 消息循环中增加如下的分支语句：

case UM_SOCK:
switch(lParam)
{
case FD_READ:
len = recv(wParam,lpBuffer,length,0);
break;
case FD_WRITE:
while(send(wParam,lpBuffer,len,0)!=SOCKET_ERROR)
break;
}
break;
　　4、出错处理

　　Windows 提供了一个函数来获取最近的错误码 WSAGetLastError()，推荐的编写方式如下： 

len = send (s,lpBuffer,len,0);
of((len==SOCKET_ERROR)&&(WSAGetLastError()==WSAWOULDBLOCK)){...}

　　二、主要扩充说明

　　1、异步选择机制：

　　Windows Sockets 的异步选择函数提供了消息机制的网络事件选择，当使用它登记网络事件发生时，应用程序相应窗口函数将收到一个消息，消息中指示了发生的网络事件，以及与事件相关的一些信息。

　　Windows Sockets 提供了一个异步选择函数 WSAAsyncSelect()，用它来注册应用程序感兴趣的网络事件，当这些事件发生时，应用程序相应的窗口函数将收到一个消息。

　　函数结构如下：

int PASCAL FAR WSAAsyncSelect(SOCKET s,HWND hWnd,unsigned int wMsg,long lEvent);
　　参数说明：

　　　hWnd：窗口句柄

　　　wMsg：需要发送的消息

　　　lEvent：事件（以下为事件的内容）

|值：|含义：|
|----|----|
|FD_READ|期望在套接字上收到数据（即读准备好）时接到通知|
|FD_WRITE|期望在套接字上可发送数据（即写准备好）时接到通知|
|FD_OOB|期望在套接字上有带外数据到达时接到通知|
|FD_ACCEPT|期望在套接字上有外来连接时接到通知|
|FD_CONNECT|期望在套接字连接建立完成时接到通知|
|FD_CLOSE|期望在套接字关闭时接到通知|
　　例如：我们要在套接字读准备好或写准备好时接到通知，语句如下： 

rc=WSAAsyncSelect(s,hWnd,wMsg,FD_READ|FD_WRITE);
　　如果我们需要注销对套接字网络事件的消息发送，只要将 lEvent 设置为0 

　　2、异步请求函数

　　在 Berkeley Sockets 中请求服务是阻塞的，WINDOWS SICKETS 除了支持这一类函数外，还增加了相应的异步请求函数(WSAAsyncGetXByY();)。 

　　3、阻塞处理方法

　　Windows Sockets 为了实现当一个应用程序的套接字调用处于阻塞时，能够放弃CPU让其它应用程序运行，它在调用处于阻塞时便进入一个叫“HOOK”的例程，此例程负责接收和分配WINDOWS消息，使得其它应用程序仍然能够接收到自己的消息并取得控制权。

　　WINDOWS 是非抢先的多任务环境，即若一个程序不主动放弃其控制权，别的程序就不能执行。因此在设计Windows Sockets 程序时，尽管系统支持阻塞操作，但还是反对程序员使用该操作。但由于 SUN 公司下的 Berkeley Sockets 的套接字默认操作是阻塞的，WINDOWS 作为移植的 SOCKETS 也不可避免对这个操作支持。

　　在Windows Sockets 实现中，对于不能立即完成的阻塞操作做如下处理：DLL初始化→循环操作。在循环中，它发送任何 WINDOWS 消息，并检查这个 Windows Sockets 调用是否完成，在必要时，它可以放弃CPU让其它应用程序执行（当然使用超线程的CPU就不会有这个麻烦了^_^）。我们可以调用 WSACancelBlockingCall() 函数取消此阻塞操作。

　　在 Windows Sockets 中，有一个默认的阻塞处理例程 BlockingHook() 简单地获取并发送 WINDOWS 消息。如果要对复杂程序进行处理，Windows Sockets 中还有 WSASetBlockingHook() 提供用户安装自己的阻塞处理例程能力；与该函数相对应的则是 SWAUnhookBlockingHook()，它用于删除先前安装的任何阻塞处理例程，并重新安装默认的处理例程。请注意，设计自己的阻塞处理例程时，除了函数 WSACancelBlockingHook() 之外，它不能使用其它的 Windows Sockets API 函数。在处理例程中调用 WSACancelBlockingHook()函数将取消处于阻塞的操作，它将结束阻塞循环。

　　4、出错处理

　　Windows Sockets 为了和以后多线程环境（WINDOWS/UNIX）兼容，它提供了两个出错处理函数来获取和设置当前线程的最近错误号。（WSAGetLastEror()和WSASetLastError()）

　　5、启动与终止

　　使用函数 WSAStartup() 和 WSACleanup() 启动和终止套接字。

**三、Windows Sockets网络程序设计核心**

　　我们终于可以开始真正的 Windows Sockets 网络程序设计了。不过我们还是先看一看每个 Windows Sockets 网络程序都要涉及的内容。让我们一步步慢慢走。

　　1、启动与终止

　　在所有 Windows Sockets 函数中，只有启动函数 WSAStartup() 和终止函数 WSACleanup() 是必须使用的。

　　启动函数必须是第一个使用的函数，而且它允许指定 Windows Sockets API 的版本，并获得 SOCKETS的特定的一些技术细节。本结构如下：

int PASCAL FAR WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
　　其中 wVersionRequested 保证 SOCKETS 可正常运行的 DLL 版本，如果不支持，则返回错误信息。
我们看一下下面这段代码，看一下如何进行 WSAStartup() 的调用

WORD wVersionRequested;// 定义版本信息变量
WSADATA wsaData;//定义数据信息变量
int err;//定义错误号变量
wVersionRequested = MAKEWORD(1,1);//给版本信息赋值
err = WSAStartup(wVersionRequested, &wsaData);//给错误信息赋值
if(err!=0)
{
return;//告诉用户找不到合适的版本
}
//确认 Windows Sockets DLL 支持 1.1 版本
//DLL 版本可以高于 1.1
//系统返回的版本号始终是最低要求的 1.1，即应用程序与DLL 中可支持的最低版本号
if(LOBYTE(wsaData.wVersion)!= 1|| HIBYTE(wsaData.wVersion)!=1)
{
WSACleanup();//告诉用户找不到合适的版本
return;
}
//Windows Sockets DLL 被进程接受，可以进入下一步操作
　　关闭函数使用时，任何打开并已连接的 SOCK_STREAM 套接字被复位，但那些已由 closesocket() 函数关闭的但仍有未发送数据的套接字不受影响，未发送的数据仍将被发送。程序运行时可能会多次调用 WSAStartuo() 函数，但必须保证每次调用时的 wVersionRequested 的值是相同的。

　　2、异步请求服务

　　Windows Sockets 除支持 Berkeley Sockets 中同步请求，还增加了了一类异步请求服务函数 WSAAsyncGerXByY()。该函数是阻塞请求函数的异步版本。应用程序调用它时，由 Windows Sockets DLL 初始化这一操作并返回调用者，此函数返回一个异步句柄，用来标识这个操作。当结果存储在调用者提供的缓冲区，并且发送一个消息到应用程序相应窗口。常用结构如下：

HANDLE taskHnd;
char hostname="rs6000";
taskHnd = WSAAsyncBetHostByName(hWnd,wMsg,hostname,buf,buflen); 
　　需要注意的是，由于 Windows 的内存对像可以设置为可移动和可丢弃，因此在操作内存对象是，必须保证 WIindows Sockets DLL 对象是可用的。 

　　3、异步数据传输

　　使用 send() 或 sendto() 函数来发送数据，使用 recv() 或recvfrom() 来接收数据。Windows Sockets 不鼓励用户使用阻塞方式传输数据，因为那样可能会阻塞整个 Windows 环境。下面我们看一个异步数据传输实例：

　　假设套接字 s 在连接建立后，已经使用了函数 WSAAsyncSelect() 在其上注册了网络事件 FD_READ 和 FD_WRITE，并且 wMsg 值为 UM_SOCK，那么我们可以在 Windows 消息循环中增加如下的分支语句：

case UM_SOCK:
switch(lParam)
{
case FD_READ:
len = recv(wParam,lpBuffer,length,0);
break;
case FD_WRITE:
while(send(wParam,lpBuffer,len,0)!=SOCKET_ERROR)
break;
}
break;
　　4、出错处理

　　Windows 提供了一个函数来获取最近的错误码 WSAGetLastError()，推荐的编写方式如下： 

len = send (s,lpBuffer,len,0);
of((len==SOCKET_ERROR)&&(WSAGetLastError()==WSAWOULDBLOCK)){...}

