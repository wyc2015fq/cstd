# 如何优雅地关闭一个socket - DoubleLi - 博客园






最近在windows编程时需要考虑到“如何优雅地关闭一个socket”，查阅了一些资料，现将查到的相关资料做个汇编，希望能对后来者有所帮助（比较懒，所以英文资料没有翻译:-)）

**1. 关闭Socket时究竟做了什么**

    关闭socket分为主动关闭（Active closure）和被动关闭（Passive closure）两种情况。前者是指有本地主机主动发起的关闭；而后者则是指本地主机检测到远程主机发起关闭之后，作出回应，从而关闭整个连接。
    其状态图如下图所示：

![](http://static.flickr.com/50/137183203_0564194f78.jpg?v=0)

    　　起初每个socket都是CLOSED状态，当客户端初使化一个连接，他发送一个SYN包到服务器，客户端进入SYN_SENT状态。
服务器接收到SYN包，反馈一个SYN-ACK包，客户端接收后返馈一个ACK包客户端变成ESTABLISHED状态，如果长时间没收到SYN-ACK包，客户端超时进入CLOSED状态。
　　当服务器绑定并监听某一端口时，socket的状态是LISTEN，当客户企图建立连接时，服务器收到一个SYN包，并反馈SYN-ACK包。服务器状态变成SYN_RCVD，当客户端发送一个ACK包时，服务器socket变成ESTABLISHED状态。

　　当一个程序在ESTABLISHED状态时有两种图径关闭它， 第一是主动关闭，第二是被动关闭。如果你要主动关闭的话，发送一个FIN包。当你的程序closesocket或者shutdown（标记），你的程序发送一个FIN包到peer，你的socket变成FIN_WAIT_1状态。peer反馈一个ACK包，你的socket进入FIN_WAIT_2状态。如果peer也在关闭连接，那么它将发送一个FIN包到你的电脑，你反馈一个ACK包，并转成TIME_WAIT状态。
　　TIME_WAIT状态又号2MSL等待状态。MSL意思是最大段生命周期（Maximum Segment Lifetime)表明一个包存在于网络上到被丢弃之间的时间。每个IP包有一个TTL(time_to_live)，当它减到0时则包被丢弃。每个路由器使TTL减一并且传送该包。当一个程序进入TIME_WAIT状态时，他有２个MSL的时间，这个充许TCP重发最后的ACK，万一最后的ACK丢失了，使得FIN被重新传输。在2MSL等待状态完成后，socket进入CLOSED状态。
　　被动关闭：当程序收到一个FIN包从peer，并反馈一个ACK包，于是程序的socket转入CLOSE_WAIT状态。因为peer已经关闭了，所以不能发任何消息了。但程序还可以。要关闭连接，程序自已发送给自已FIN，使程序的TCP socket状态变成LAST_ACK状态，当程序从peer收到ACK包时，程序进入CLOSED状态。

**2. Winsock2 API中的相关函数**

    先当然是查MSDN，看到winsocks2 API中的相关函数有:closesocket,shutdown,WSASendDisconnect. 我大致说一下，具体详细的资料还请自行查MSDN.

**int****closesocket(****SOCKET***[s](http://writeblog.csdn.net/fckeditor/editor/)***)**的作用是关闭指定的socket，并且回收其所有的资源。
**int****shutdown(****SOCKET***[s](http://writeblog.csdn.net/fckeditor/editor/)***,****int***[how](http://writeblog.csdn.net/fckeditor/editor/)***)**则是禁止在指定的socket s上禁止进行由how指定的操作，但并不对资源进行回收，shutdown之后而closesocket之前s还不能再次connect或者WSAConnect.
**int****WSASendDisconnect(****SOCKET***[s](http://writeblog.csdn.net/fckeditor/editor/)***,****LPWSABUF***[lpOutboundDisconnectData](http://writeblog.csdn.net/fckeditor/editor/)***)**则和shutdown基本类似，稍有不同的就是WSASendDisconnect函数多了一个[*lpOutboundDisconnectData*](http://writeblog.csdn.net/fckeditor/editor/)参数，可以允许发送“断开数据”（disconnect data）.但MSDN上写了“The native implementation of TCP/IP on Windows does not support disconnect data.”，所以一般我们就用shutdown函数就行了。


**3. Socket的优雅关闭**

在MSDN中对shutdown函数中的Remarks部分有下面一段话，指出了如何进行一次优雅的slcket关闭:

To assure that all data is sent and received on a connected socket before it is closed, an application should use **shutdown** to close connection before calling **closesocket**. For example, to initiate a graceful disconnect:


- Call [**WSAAsyncSelect**](ms-help://MS.MSDNQTR.2003FEB.2052/winsock/winsock/wsaasyncselect_2.htm) to register for FD_CLOSE notification.
- Call **shutdown** with *how*=SD_SEND.
- When FD_CLOSE received, call [**recv**](ms-help://MS.MSDNQTR.2003FEB.2052/winsock/winsock/recv_2.htm) until zero returned, or SOCKET_ERROR.
- Call [**closesocket**](ms-help://MS.MSDNQTR.2003FEB.2052/winsock/winsock/closesocket_2.htm).

closesocket的行为也是随setsockopt()中参数的不同而有不同的表现，这里影响它的行为的主要就是那个linger结构。


SO_DONTLINGER 若为真，则SO_LINGER选项被禁止。 
SO_LINGER 延迟关闭连接 struct linger  
上面这两个选项影响close行为 
选项 间隔 关闭方式 等待关闭与否 
SO_DONTLINGER 不关心 优雅 否 
SO_LINGER 零 强制 否 
SO_LINGER 非零 优雅 是 
若设置了SO_LINGER（亦即linger结构中的l_onoff域设为非零），并设置了零超时间隔，则closesocket()不被阻塞立即执行，不论是否有排队数据未发送或未被确认。这种关闭方式称为“强制”或“失效”关闭，因为套接口的虚电路立即被复位，且丢失了未发送的数据。在远端的recv()调用将以WSAECONNRESET出错。 
若设置了SO_LINGER并确定了非零的超时间隔，则closesocket()调用阻塞进程，直到所剩数据发送完毕或超时。这种关闭称为“优雅的”关闭。请注意如果套接口置为非阻塞且SO_LINGER设为非零超时，则closesocket()调用将以WSAEWOULDBLOCK错误返回。 
若在一个流类套接口上设置了SO_DONTLINGER（也就是说将linger结构的l_onoff域设为零），则closesocket()调用立即返回。但是，如果可能，排队的数据将在套接口关闭前发送。请注意，在这种情况下WINDOWS套接口实现将在一段不确定的时间内保留套接口以及其他资源，这对于想用所以套接口的应用程序来说有一定影响。

    所以一般来说，不应该把linger设置为SO_LINGER 并且设置timeout为0，这样的话，当本地主机调用closesocket时将会造成一个“强制”或“失效”的非优雅关闭。可以根据实际情况设置为另外两种情况。









