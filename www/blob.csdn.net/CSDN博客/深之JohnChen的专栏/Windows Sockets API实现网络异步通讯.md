# Windows Sockets API实现网络异步通讯 - 深之JohnChen的专栏 - CSDN博客

2005年12月11日 19:29:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1538标签：[sockets																[网络																[windows																[api																[服务器																[socket](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=sockets&t=blog)
个人分类：[网络编程](https://blog.csdn.net/byxdaz/article/category/166215)

**摘要：**本文对如何使用面向连接的流式套接字实现对网卡的编程以及如何实现异步网络通讯等问题进行了讨论与阐述。 

**一、 引言**

　　在80年代初，美国加利福尼亚大学伯克利分校的研究人员为TCP/IP网络通信开发了一个专门用于网络通讯开发的API。这个API就是Socket接口（套接字）--当今在TCP/IP网络最为通用的一种API，也是在互联网上进行应用开发最为通用的一种API。在微软联合其它几家公司共同制定了一套Windows下的网络编程接口Windows Sockets规范后，由于在其规范中引入了一些异步函数，增加了对网络事件异步选择机制，因此更加符合Windows的消息驱动特性，使网络开发人员可以更加方便的进行高性能网络通讯程序的设计。本文接下来就针对Windows Sockets API进行面向连接的流式套接字编程以及对异步网络通讯的编程实现等问题展开讨论。

**二、 面向连接的流式套接字编程模型的设计**

　　本文在方案选择上采用了在网络编程中最常用的一种模型--客户机/服务器模型。这种客户/服务器模型是一种非对称式编程模式。该模式的基本思想是把集中在一起的应用划分成为功能不同的两个部分，分别在不同的计算机上运行，通过它们之间的分工合作来实现一个完整的功能。对于这种模式而言其中一部分需要作为服务器，用来响应并为客户提供固定的服务；另一部分则作为客户机程序用来向服务器提出请求或要求某种服务。

　　本文选取了基于TCP/IP的客户机/服务器模型和面向连接的流式套接字。其通信原理为：服务器端和客户端都必须建立通信套接字，而且服务器端应先进入监听状态，然后客户端套接字发出连接请求，服务器端收到请求后，建立另一个套接字进行通信，原来负责监听的套接字仍进行监听，如果有其它客户发来连接请求，则再建立一个套接字。默认状态下最多可同时接收5个客户的连接请求，并与之建立通信关系。因此本程序的设计流程应当由服务器首先启动，然后在某一时刻启动客户机并使其与服务器建立连接。服务器与客户机开始都必须调用Windows Sockets API函数socket()建立一个套接字sockets,然后服务器方调用bind()将套接字与一个本地网络地址捆扎在一起，再调用listen()使套接字处于一种被动的准备接收状态，同时规定它的请求队列长度。在此之后服务器就可以通过调用accept()来接收客户机的连接。

　　相对于服务器，客户端的工作就显得比较简单了，当客户端打开套接字之后，便可通过调用connect()和服务器建立连接。连接建立之后，客户和服务器之间就可以通过连接发送和接收资料。最后资料传送结束，双方调用closesocket()关闭套接字来结束这次通讯。整个通讯过程的具体流程框图可大致用下面的流程图来表示：

![](http://www.yesky.com/SoftChannel/72342371928702976/20020910/table.gif)
　　　　　　　　面向连接的流式套接字编程流程示意图 **三、 软件设计要点以及异步通讯的实现**　　根据前面设计的程序流程，可将程序划分为两部分：服务器端和客户端。而且整个实现过程可以大致用以下几个非常关键的Windows Sockets API函数将其惯穿下来：

　　服务器方：

socket()->bind()->listen->accept()->recv()/send()->closesocket()
　　客户机方：

socket()->connect()->send()/recv()->closesocket()
　　有鉴于以上几个函数在整个网络编程中的重要性，有必要结合程序实例对其做较深入的剖析。服务器端应用程序在使用套接字之前，首先必须拥有一个Socket，系统调用socket()函数向应用程序提供创建套接字的手段。该套接字实际上是在计算机中提供了一个通信埠，可以通过这个埠与任何一个具有套接字接口的计算机通信。应用程序在网络上传输、接收的信息都通过这个套接字接口来实现的。在应用开发中如同使用文件句柄一样，可以对套接字句柄进行读写操作：

sock=socket(AF_INET,SOCK_STREAM,0);
　　函数的第一个参数用于指定地址族，在Windows下仅支持AF_INET(TCP/IP地址)；第二个参数用于描述套接字的类型，对于流式套接字提供有SOCK_STREAM；最后一个参数指定套接字使用的协议，一般为0。该函数的返回值保存了新套接字的句柄，在程序退出前可以用 closesocket(sock);函数来将其释放。服务器方一旦获取了一个新的套接字后应通过bind()将该套接字与本机上的一个端口相关联：

sockin.sin_family=AF_INET;
sockin.sin_addr.s_addr=0;
sockin.sin_port=htons(USERPORT);
bind(sock,(LPSOCKADDR)&sockin,sizeof(sockin)));
　　该函数的第二个参数是一个指向包含有本机IP地址和端口信息的sockaddr_in结构类型的指针，其成员描述了本地端口号和本地主机地址，经过bind()将服务器进程在网络上标识出来。需要注意的是由于1024以内的埠号都是保留的埠号因此如无特别需要一般不能将sockin.sin_port的埠号设置为1024以内的值。然后调用listen()函数开始侦听，再通过accept()调用等待接收连接以完成连接的建立：

//连接请求队列长度为1，即只允许有一个请求,若有多个请求,
//则出现错误，给出错误代码WSAECONNREFUSED。
listen(sock,1);
//开启线程避免主程序的阻塞
AfxBeginThread(Server,NULL);
……
UINT Server(LPVOID lpVoid)
{
……
int nLen=sizeof(SOCKADDR);
pView->newskt=accept(pView->sock,(LPSOCKADDR)& pView->sockin,(LPINT)& nLen);
…… 
WSAAsyncSelect(pView->newskt,pView->m_hWnd,WM_SOCKET_MSG,FD_READ|FD_CLOSE);
return 1; 
}

　　这里之所以把accept()放到一个线程中去是因为在执行到该函数时如没有客户连接服务器的请求到来，服务器就会停在accept语句上等待连接请求的到来，这势必会引起程序的阻塞，虽然也可以通过设置套接字为非阻塞方式使在没有客户等待时可以使accept（）函数调用立即返回，但这种轮询套接字的方式会使CPU处于忙等待方式，从而降低程序的运行效率大大浪费系统资源。考虑到这种情况，将套接字设置为阻塞工作方式，并为其单独开辟一个子线程，将其阻塞控制在子线程范围内而不会造成整个应用程序的阻塞。对于网络事件的响应显然要采取异步选择机制，只有采取这种方式才可以在由网络对方所引起的不可预知的网络事件发生时能马上在进程中做出及时的响应处理，而在没有网络事件到达时则可以处理其他事件，这种效率是很高的，而且完全符合Windows所标榜的消息触发原则。前面那段代码中的WSAAsyncSelect()函数便是实现网络事件异步选择的核心函数。
通过第四个参数注册应用程序感兴取的网络事件，在这里通过FD_READ|FD_CLOSE指定了网络读和网络断开两种事件，当这种事件发生时变会发出由第三个参数指定的自定义消息WM_SOCKET_MSG，接收该消息的窗口通过第二个参数指定其句柄。在消息处理函数中可以通过对消息参数低字节进行判断而区别出发生的是何种网络事件：

void CNetServerView::OnSocket(WPARAM wParam,LPARAM lParam)
{
int iReadLen=0;
int message=lParam & 0x0000FFFF;
switch(message)
{ 
case FD_READ://读事件发生。此时有字符到达，需要进行接收处理
char cDataBuffer[MTU*10];
//通过套接字接收信息
iReadLen = recv(newskt,cDataBuffer,MTU*10,0);
//将信息保存到文件
if(!file.Open("ServerFile.txt",CFile::modeReadWrite))
file.Open("E:ServerFile.txt",CFile::modeCreate|CFile::modeReadWrite);
file.SeekToEnd();
file.Write(cDataBuffer,iReadLen);
file.Close(); 
break;
case FD_CLOSE://网络断开事件发生。此时客户机关闭或退出。
……//进行相应的处理
break;
default:
break;
}
}
　　在这里需要实现对自定义消息WM_SOCKET_MSG的响应，需要在头文件和实现文件中分别添加其消息映射关系：

　　头文件：

//{{AFX_MSG(CNetServerView)
//}}AFX_MSG
void OnSocket(WPARAM wParam,LPARAM lParam);
DECLARE_MESSAGE_MAP()
　　实现文件：

BEGIN_MESSAGE_MAP(CNetServerView, CView)
//{{AFX_MSG_MAP(CNetServerView)
//}}AFX_MSG_MAP
ON_MESSAGE(WM_SOCKET_MSG,OnSocket)
END_MESSAGE_MAP()

　　在进行异步选择使用WSAAsyncSelect()函数时，有以下几点需要引起特别的注意：

　　1． 连续使用两次WSAAsyncSelect()函数时，只有第二次设置的事件有效，如：

WSAAsyncSelect(s,hwnd,wMsg1,FD_READ);
WSAAsyncSelect(s,hwnd,wMsg2,FD_CLOSE);
　　这样只有当FD_CLOSE事件发生时才会发送wMsg2消息。

　　2．可以在设置过异步选择后通过再次调用WSAAsyncSelect(s,hwnd,0,0);的形式取消在套接字上所设置的异步事件。

　　3．Windows Sockets DLL在一个网络事件发生后，通常只会给相应的应用程序发送一个消息，而不能发送多个消息。但通过使用一些函数隐式地允许重发此事件的消息，这样就可能再次接收到相应的消息。

　　4．在调用过closesocket()函数关闭套接字之后不会再发生FD_CLOSE事件。

　　以上基本完成了服务器方的程序设计，下面对于客户端的实现则要简单多了，在用socket()创建完套接字之后只需通过调用connect()完成同服务器的连接即可，剩下的工作同服务器完全一样：用send()/recv()发送/接收收据，用closesocket()关闭套接字：

sockin.sin_family=AF_INET; //地址族
sockin.sin_addr.S_un.S_addr=IPaddr; //指定服务器的IP地址
sockin.sin_port=m_Port; //指定连接的端口号
int nConnect=connect(sock,(LPSOCKADDR)&sockin,sizeof(sockin));

　　本文采取的是可靠的面向连接的流式套接字。在数据发送上有write()、writev()和send()等三个函数可供选择，其中前两种分别用于缓冲发送和集中发送，而send()则为可控缓冲发送，并且还可以指定传输控制标志为MSG_OOB进行带外数据的发送或是为MSG_DONTROUTE寻径控制选项。在信宿地址的网络号部分指定数据发送需要经过的网络接口，使其可以不经过本地寻径机制直接发送出去。这也是其同write()函数的真正区别所在。由于接收数据系统调用和发送数据系统调用是一一对应的，因此对于数据的接收，在此不再赘述，相应的三个接收函数分别为：read()、readv()和recv()。由于后者功能上的全面，本文在实现上选择了send()-recv()函数对，在具体编程中应当视具体情况的不同灵活选择适当的发送-接收函数对。

**小结：**TCP/IP协议是目前各网络操作系统主要的通讯协议，也是 Internet的通讯协议，本文通过Windows Sockets API实现了对基于TCP/IP协议的面向连接的流式套接字网络通讯程序的设计，并通过异步通讯和多线程等手段提高了程序的运行效率，避免了阻塞的发生。


