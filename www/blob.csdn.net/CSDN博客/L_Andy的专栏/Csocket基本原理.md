# Csocket基本原理 - L_Andy的专栏 - CSDN博客

2015年01月15日 10:14:11[卡哥](https://me.csdn.net/L_Andy)阅读数：668



我通过几个采用 CSocket 类编写并基于 Client/Server （客户端 / 服务端）的网络聊天和传输文件的程式  ，在调试这些程式的过程中，追踪深入至 CSocket 类核心源码 SockCore.cpp ， 对于CSocket 类的运行机制可谓是一览无遗，并且对于阻塞和非阻塞方式下的 socket 程式的编写也是稍有体会。

阅读本文请先注意 :

•  这里的阻塞和非阻塞的概念仅适用于 Server 端 socket 程式。

•  socket 意为套接字，他和 Socket 不同，请注意首字母的大小写。

说明：客户端和服务端的通信简单来讲：服务端 socket 负责监听，应答，接收和发送消息，而客户端 socket 只是连接，应答，接收，发送消息。此外，如果你对于采用 CSocket 类编写 Client/Server 网络程式的原理不是非常了解，请先查询一下（ **详见：参考书籍和在线帮助 **）。

在此之前，有必要先讲述一下： 网络传输服务提供者， ws2_32.dll ， socket 事件 和 socket window 。

**1. 网络传输服务提供者（网络传输服务进程）， Socket 事件， Socket Window**

网络传输服务提供者 （ transport service provider ）是以 DLL 的形式存在的，在 windows 操作系统启动时由服务进程 svchost.exe 加载。当 socket 被创建时，调用 API 函数 Socket （在 ws2_32.dll 中）， Socket 函数会传递三个参数 : 地址族，套接字类型 ( **注 2 **) 和协议，这三个参数决定了是由哪一个类型的 网络传输服务提供者 来启动网络传输服务功能。所有的网络通信正是由网络传输服务提供者完成
 , 这里将 网络传输服务提供者 称为 网络传输服务进程 更有助于理解，因为前文已提到 网络传输服务提供者 是由 svchost.exe 服务进程所加载的。

当 Client 端 socket 和 Server 端 socket 相互通信时，两端均会触发 socket 事件 :

这里仅简要说明两个 socket 事件 ：

FD_CONNECT: 连接事件 , 通常 Client 端 socket 调用 socket API 函数 Connect 时所触发，这个事件发生在 Client 端。

FD_ACCEPT ：正在引入的连接事件，通常 Server 端 socket 正在接收来自 Client 端 socket 连接时触发，这个事件发生在 Server 端。

网络传输服务进程 将 socket 事件 保存至 socket 的事件队列中。

此外， 网络传输服务进程 还会向 socket window 发送消息 WM_SOCKET_NOTIFY , 通知有 socket 事件 产生，见下文对 socket window 的周详说明：

调用 CSocket::Create 函数后， socket 被创建。 socket 创建过程中调用 CAsyncSocket::AttachHandle(SOCKET hSocket, CAsyncSocket* pSocket, BOOL bDead) 。该函数的作用是：

a. 将 socket 实例句柄和 socket 指针添加至 当前模块状态 （ **注 1** ）的一个映射表变量 m_pmapSocketHandle 中。

b. 在 AttachHandle 过程中，会 new 一个 CSocketWnd 实例 ( 基于 CWnd 派生 ) ，这里将这个实例称之为 socket window ，进一步理解为他是存放所有 sockets 的消息池 （ window 消息），请仔细查看，这里 socket 后多加了一个 s ，表示创建的多个 socket 将共享一个 消息池 。

c. 当 Client 端 socket 和 Server 端相互通信时 , 此时 网络传输服务进程 向 socket window 发送消息 WM_SOCKET_NOTIFY ，需要说明的是 CSocketWnd 窗口句柄保存在 当前模块状态 的 m_hSocketWindow 变量中。

**2. 阻塞模式**

阻塞模式下 Server 端和 Client 端之间的通信处于同步状态下。

在 Server 端直接实例化 CSocket 类，调用 Create 方法创建 socket ，然后调用方法 Listen 开始侦听，最后用一个 while 循环阻塞调用 Accept 函数用于等待来自 Client 端的连接，如果这个 socket 在主线程（主程式）中运行，这将导致主线程的阻塞。因此，需要创建一个新的线程以运行 socket 服务。

调试跟踪至 CSocket::Accept 函数源码： 
while(!Accept(...)) 

{ 

    if (GetLastError() == WSAEWOULDBLOCK) //  The socket is marked as nonblocking and no connections are present to be accepted. 

        PumpMessage(FD_ACCEPT); 

    else 

        return FALSE; 

}

他不断调用 CAsyncSocket::Accept （ CSocket 派生自 CAsyncSocket 类）判断 Server 端 socket 的事件队列中是否存在正在引入的连接事件 - FD_ACCEPT （见 1 ），换句话说，就是判断是否有来自 Client 端 socket 的连接请求。

如果当前 Server 端 socket 的事件队列中存在正在引入的连接事件， Accept 返回一个非 0 值。否则， Accept 返回 0 ，此时调用 GetLastError 将返回错误代码 WSAEWOULDBLOCK ，表示队列中无所有连接请求。

注意到在循环体内有一句代码：

PumpMessage(FD_ACCEPT);

PumpMessage 作为一个消息泵使得 socket window 中的消息能够维持在活动状态。

实际跟踪进入 PumpMessage 中，发现这个消息泵和 Accept 函数的调用并不相关，他只是使非常少的 socket window 消息（典型的是 WM_PAINT 窗口重绘消息）处于活动状态，而绝大部分的 socket window 消息被阻塞，被阻塞的消息中含有 WM_SOCKET_NOTIFY 。

非常显然，如果没有来自 Client 端 socket 的连接请求， CSocket 就会不断调用 Accept 产生循环阻塞，直到有来自 Client 端 socket 的连接请求而解除阻塞。

阻塞解除后，表示 Server 端 socket 和 Client 端 socket 已成功连接， Server 端和 Client 端彼此相互调用 Send 和 Receive 方法开始通信。

**3. 非阻塞模式**

在非阻塞模式下 利用 socket 事件 的消息机制， Server 端和 Client 端之间的通信处于异步状态下。

通常需要从 CSocket 类派生一个新类，派生新类的目的是重载 socket 事件 的消息函数，然后在 socket 事件 的消息函数中添入合适的代码以完成 Client 端和 Server 端之间的通信，和阻塞模式相比，非阻塞模式无需创建一个新线程。

这里将讨论当 Server 端 socket 事件 － FD_ACCEPT 被触发后，该事件的处理函数 OnAccept 是怎么进一步被触发的。其他事件的处理函数如 OnConnect, OnReceive 等的触发方式和此类似。

在 1 中已提到 Client/Server 端通信时， Server 端 socket 正在接收来自 Client 端 socket 连接请求，这将会触发 FD_ACCEPT 事件，同时 Server 端的 网络传输服务进程 向 Server 端的 socket window (CSocketWnd ）发送事件通知消息 WM_SOCKET_NOTIFY , 通知有 FD_ACCEPT 事件产生 , CsocketWnd 在收到事件通知消息后，调用消息处理函数 OnSocketNotify: 

LRESULT CSocketWnd::OnSocketNotify(WPARAM wParam, LPARAM lParam) 

{ 

 CSocket::AuxQueueAdd(WM_SOCKET_NOTIFY, wParam, lParam); 

 CSocket::ProcessAuxQueue(); 

 return 0L ; 

}

消息参数 wParam 是 socket 的句柄， lParam 是 socket 事件 。这里稍作解释一下， CSocketWnd 类是作为 CSocket 类的 友元类 ，这意味着他能访问 CSocket 类中的保护和私有成员函数和变量， AuxQueueAdd 和 ProcessAuxQueue 是 CSocket 类的静态成员函数，如果你对友元不熟悉，请迅速找本有关 C++ 书看一下友元的使用方法吧！ 

ProcessAuxQueue 是实质处理 socket 事件 的函数，在该函数中有这样一句代码：

CAsyncSocket* pSocket = CAsyncSocket::LookupHandle((SOCKET)wParam, TRUE);

其实也就是由 socket 句柄得到发送事件通知消息的 socket 指针 pSocket ：从 m_pmapSocketHandle 中查找（见 1 ）！

最后， WSAGETSELECTEVENT(lParam) 会取出事件类型，在一个简单的 switch 语句中判断事件类型并调用事件处理函数。 

在这里，事件类型是 FD_ACCEPT ，当然就调用 pSocket->OnAccept ！

**本文的结束：**

Server 端 socket 处于阻塞调用模式下，他必须在一个新创建的线程中工作，防止主线程被阻塞。

当有多个 Client 端 socket 和 Server 端 socket 连接及通信时， Server 端采用阻塞模式就显得不适合了，应该采用非阻塞模式 , 利用 socket 事件 的消息机制来接受多个 Client 端 socket 的连接请求并进行通信。

在非阻塞模式下，利用 CSocketWnd 作为所有 sockets 的消息池，是实现 socket 事件 的消息机制的关键技术。

文中存在用词不妥和可能存在的技术问题，请大家原谅，也请批评指正，谢谢！ 

**注：**

1. 当前模块状态

用于保存当前线程和模块状态的一个结构，能通过 AfxGetThreadModule() 获得。 AFX_MODULE_THREAD_STATE 在 CSocket 重新定义为 _AFX_SOCK_THREAD_STATE 。

2. socket 类型

在 TCP/IP 协议中， Client/Server 网络程式采用 TCP 协议：即 socket 类型为 SOCK_STREAM ，他是可靠的连接方式。在这里不采用 UDP 协议：即 socket 类型为 SOCK_DGRAM ，他是不可靠的连接方式。

