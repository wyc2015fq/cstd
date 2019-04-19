# GPRS开发系列文章之实战篇 - xqhrs232的专栏 - CSDN博客
2013年01月09日 22:39:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：586
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/jsjkandy/archive/2008/08/12/1266345.html](http://www.cnblogs.com/jsjkandy/archive/2008/08/12/1266345.html)
**一、前言**
在前篇《GPRS开发系列文章之进阶篇》里，我主要详细讲解了客户端进行GPRS连接的常用API，并对GPRSdemo测试程序中的连接类ConnManager中的一些重要函数做了说明，最后稍微提及了下服务器端要用到的一些类库。今天，在这篇实战篇中，我将在理解前两篇的基础上，结合客户端与服务器端，向大家介绍这篇GPRS开发之实战篇，向大家演示如何利用GPRS开发一个客户端与服务器端互相通信的程序，主要介绍SOCKET编程的原理和SOCKET应用API，并在最后提供本实战篇的源代码下载。最后还是那句老话，欢迎指点，共同提高！
**二、实战系列篇详解**
**1.       开发环境**
a)       客户端：EVC4;
b)       服务器端：VS2005(C#);
**2.       运行环境**
a)       客户端：ppc 2003(winCE4.2) for mobile或以上版本；
b)       服务器端：WINXP/SERVER 2003等
**3.       客户端和服务器端通信详解**本文章的主要目的是利用GPRS连接编写一个利用TCP协议进行通信的程序，而在上篇文章已解决了GPRS连接的问题，因此剩下的主要就是我们都比较熟悉的SOCKET编程了，由于客户端和服务器端进行SOCKET通信的原理相同，所以我将他们放到一起进行讲解，主要介绍SOCKET编程的一些原理及要点，然后贴出部分比较重要的代码供大家参考。
   首先，介绍些要了解SOCKET编程的一些核心**概念**：
   我们知道在这种通信程序中，一般客户端和服务器端是分开的（本机通信可以看作是一种特例），客户端一旦和服务器端建立连接成功后就可以透明的传输数据和接收数据了。那么我们的程序在建立了GPRS连接到Internet后是如何访问到我们指定的服务器的呢？通信过程又是怎么控制的呢？那么首先看第一个概念,进程通信。
**进程通信：**这里的进程通信包括两种情况，一种是同一机器的不同进程之间的通信，另一种是在同一网络中（不同网络通过路由进行连接还是可以看成同一网络）的不同机器的不同进程之间的通信。在同一台机器中的进程通信问题，由于每个进程都在自己的地址范围内运行，为保证两个相互通信的进程之间既互不干扰又协调一致工作，操作系统为进程通信提供了相应设施，如管道（pipe）、命名管道（named pipe）和信号量（semaphore)等。各个进程要进行通信首先要解决进程的标识问题，在同一机器中，可用process
 ID来唯一标识每个单独的进程，我们可以在任务管理器中进行查看，每个进行都有自己唯一的标志符。如果没有看到的，可以在任务管理器中点击“查看”，然后点击“选择列”，在出现的对话框中选中“PID(进程标志符)”这一栏，确定后我们就可以看到每个进程的PID了，。而在网络中的不同电脑要进行通信，首先要经过网络间的协议转换然后再寻址找到我们的目的机器，最后根据特定标志符找到特定的进程，于是我们的客户端进程就可以和服务器进程进行网间进程通信了，在这一过程中扮演着重要角色的就是TCP/IP协议
**TCP/IP协议**：TCP/IP是一个协议簇，它包括网络接口层，网络层、传输层和应用层，网络层中有负责因特网地址(IP地址)与底层网络地址之间进行转换的地址解析协议ARP和反向地址解析协议RARP。同时也包括对主机和网关进行差错报告、控制和进行请求/应答的IGMP协议和网络层的核心协议IP协议。在TCP/IP协议簇中的传输层中，提供了进程间的通信的TCP和UDP协议，这两个协议分别提供了了可靠的面向连接的传输服务和简单高效的无连接传输服务，我们最需要了解的就是传输层中的这两个协议。
** IP地址**：因特网的IP协议提供了一种整个互联网中通用的地址格式，并在同一管理下进行IP地址的分配并保证其唯一性，以确保每台因特网主机（路由器）对应一个IP地址。
**端口**：网络中可以被命名和寻址的通信端口，是操作系统可分配的一种资源。按照OSI七层协议的描述，传输层与网络层在功能上的最大区别是传输层提供进程通信能力。从这个意义上讲，网络通信的最终地址就不仅仅是主机地址了，还包括可以描述进程的某种标识符。为此，TCP/IP协议提出了协议端口（protocol port，简称端口）的概念，用于标识通信的进程。 
    端口是一种抽象的软件结构（包括一些数据结构和I/O缓冲区）。应用程序（即进程）通过系统调用与某端口建立连接（binding）后，传输层传给该端口的数据都被相应进程所接收，相应进程发给传输层的数据都通过该端口输出。在TCP/IP协议的实现中，端口类似于一般的I/O操作，进程获取一个端口，相当于获取本地唯一的I/O文件，可以用一般的读写原语访问之，如我们通过指定端口读取GPS信息等。 
    类似于文件描述符，每个端口都拥有一个叫端口号（port number）的整数型标识符，用于区别不同端口。由于TCP/IP传输层的两个协议TCP和UDP是完全独立的两个软件模块，因此各自的端口号也相互独立，如TCP有一个255号端口，UDP也可以有一个255号端口，二者并不冲突。**因此当我们通过指定的IP地址和端口号就可以找到唯一标志我们的进程了。**
    在了解了上述基础知识后，我们可以简单做个回顾，总结下整个连接的过程。本文介绍的客户端与服务器端通信是典型的C/S模式，客户端在请求服务器端提供特定服务后，服务器端接收请求并提供相应服务。在TCP/IP网络应用中，C/S模式中服务器端是采取主动的方式，首先启动，并根据请求提供相应服务。
     服务器端：
   1. 打开一通信通道并告知本地主机，它愿意在某一公认地址上接收客户请求； 
   2. 等待客户请求到达该端口； 
   3. 接收到服务请求，处理该请求并发送应答信号
   4. 返回第二步，等待另一客户请求。 
   5. 关闭服务器
    客户端： 
   1. 打开一通信通道，并连接到服务器所在主机的特定端口； 
   2. 向服务器发服务请求报文，等待并接收应答；继续提出请求...... 
   3. 请求结束后关闭通信通道并终止。客户端主界面如图所示：**![](http://images.cnblogs.com/cnblogs_com/jsjkandy/GPRS/testClint.JPG)【代码部分】客户端**主要功能为建立服务器的连接，和服务器互相通信(发送数据和接收数据)，其中用到的关键的核心类为CConnectionManager类和CTCPClient_CE类，而服务器端主要负责侦听同时也发送数据给客户端，用到的核心类为ConnectionManager，客户端和服务器利用socket通信步骤如下：
第一步：实例化套接字。
用WINSOCK API方式如下（客户端）：
[]()
bool CTCPClient_CE::Connect()
{
struct sockaddr_in addr;
int err;
   addr.sin_family = AF_INET;
   addr.sin_port = htons(m_port);
//此处要将双字节转换成单字节
char ansiRemoteHost[255];
    ZeroMemory(ansiRemoteHost,255);
    WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_remoteHost,wcslen(m_remoteHost)
        ,ansiRemoteHost,wcslen(m_remoteHost),NULL,NULL);
   addr.sin_addr.s_addr=inet_addr(ansiRemoteHost);
//创建TCP套接字 
   m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if (m_socket == INVALID_SOCKET)
   {
return FALSE;
   }
//此时采用同步连接方式,connect直接返回成功或是失败
   err = connect(m_socket,(struct sockaddr *)&addr,sizeof(addr));
if (err == SOCKET_ERROR) 
   {
return FALSE;
   }
//设置通讯模式为异步模式
   DWORD ul=1;
   ioctlsocket(m_socket,FIONBIO,&ul);
return TRUE;
}
[]()
服务器端用.net平台如下：
[]()
privatevoid StartToListen(object sender, DoWorkEventArgs e)
        {
try
            {
this.listenerSocket =new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
this.listenerSocket.Bind(new IPEndPoint(this.serverIP, this.serverPort));
this.listenerSocket.Listen(200);
while (bListen)
this.CreateNewClientManager(this.listenerSocket.Accept());
            }
catch(SocketException ex)
            {
if (ex.ErrorCode ==10004)
return;
else
                {
throw ex;
                }
            }
        }
[]()
第二步，进行侦听。获取数据，发送数据。
客户端发送数据：
[]()
bool CTCPClient_CE::SendData(constchar* buf , int len)
{
int nBytes =0;
int nSendBytes=0;
while (nSendBytes < len)
    {
        nBytes = send(m_socket,buf+nSendBytes,len-nSendBytes,0);
if (nBytes==SOCKET_ERROR )
        {
int iErrorCode = WSAGetLastError();
//触发socket的Error事件
            OnError(m_pOwnerWnd,iErrorCode);
//触发与服务器端断开连接事件
            OnDisConnect(m_pOwnerWnd);
//关闭socket
            Close();
return FALSE;
        }
        nSendBytes = nSendBytes + nBytes;
if (nSendBytes < len)
        {
            Sleep(1000);
        }
    } 
return TRUE; 
}
[]()
              服务器端发送数据:
[]()
privatebool SendCommandToClient(Command cmd)
        {
try
            {
                semaphor.WaitOne();
string strSentInfo =string.Empty;
                strSentInfo =string.Format("发送者：{0}{1}内容：{2}", cmd.SenderName, Environment.NewLine, cmd.MetaData);
byte[] buffer =newbyte[256];
                buffer = System.Text.Encoding.Default.GetBytes(strSentInfo);
this.networkStream.Write(buffer, 0, buffer.GetLength(0));
this.networkStream.Flush();               
                semaphor.Release();
returntrue;
            }
catch
            {
                semaphor.Release();
returnfalse;
            }
        }
[]()
可以看出，虽然他们语法不相同，语义却相同。在实例化一个套接字对象socket时，我们都要指定协议簇，套接字类型(有流式套接字、数据报套接字和原始套接字等类型)和传输协议，成功获取套接字后服务器端要与指定端口绑定(Bind)，然后进行监听(Listen)，并调用accept ()方法。Accept()以同步方式从侦听套接字的连接请求队列中提取第一个挂起的连接请求，然后创建并返回新的**Socket****，**而客户端完成套接字的实例化后，开始调用Select()函数判断是否有读事件发生，如果有则调用Recv()函数获取从服务器端发来的数据或者调用Send()函数来向服务器发送数据。
**客户端主要函数为：**
bool Open(CWnd * pWnd);
bool Connect();
bool SendData(const char * buf , int len);
bool Close();**服务器端主要函数为：**void StartToListen(objectsender, DoWorkEventArgse);
void CreateNewClientManager(Socketsocket);
voidStartReceive(objectsender, DoWorkEventArgse);
void SendCommandToClient(Commandcmd);
**三、引用（参考）文章**
**1.****[http://bbs.chinaunix.net/viewthread.php?tid=198859](http://bbs.chinaunix.net/viewthread.php?tid=198859)（socket编程原理-很不错）；**
**2.[http://msdn.microsoft.com/zh-cn/library/system.net.sockets.socket_methods.aspx](http://msdn.microsoft.com/zh-cn/library/system.net.sockets.socket_methods.aspx)(msdn 开发中心socket部分)**
**3.实战篇源代码下载：[客户端](http://files.cnblogs.com/jsjkandy/TestClient.rar)[服务器端](http://files.cnblogs.com/jsjkandy/GRRSServer.rar)**
**作者**：[彭立云](http://www.cnblogs.com/hanchan)
**出处**：[http://www.cnblogs.com/hanchan](http://www.cnblogs.com/hanchan)
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文连接，否则保留追究法律责任的权利。
