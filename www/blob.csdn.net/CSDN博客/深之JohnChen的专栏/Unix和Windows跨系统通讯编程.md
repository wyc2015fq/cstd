# Unix和Windows跨系统通讯编程 - 深之JohnChen的专栏 - CSDN博客

2005年12月27日 12:57:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3267


本文介绍了套接字（Socket）的基本概念及编程技术，并结合实例说明在Unix和Windows下如何用套接字实现客户/服务器方式的通讯编程。
**摘　要**　本文介绍了套接字（Socket）的基本概念及编程技术，并结合实例说明在Unix和Windows下如何用套接字实现客户/服务器方式的通讯编程。
**关键词**　Berkeley Sockets Windows Sockets 通讯编程 

**一、 前言**　　随着Internet的不断发展，客户机/服务器模型得到了广泛的应用。客户应用程序向服务器程序请求服务。一个服务程序通常在一个众所周知的端口监听对服务的请求，也就是说，服务进程一直处于休眠状态，直到一个客户对这个服务的地址提出了连接请求。在这个时刻，服务程序被“惊醒”并且为客户提供服务或对客户的请求作出适当的反应。　　 Unix最早是由美国贝尔实验室发明的一种多用户、多任务的通用操作系统，由于 Unix具有技术成熟、可靠性高、网络和数据库功能强、伸缩性突出和开发性好的特色，可满足各行各业的实际需要，特别是企业重要业务的需要，已经成为主要的工作平台和重要的企业操作平台，而微软公司的 Windows操作系统用户界面友好，安装、使用也比较方便，应用软件丰富，在个人PC机上成为主流操作系统。因此服务端使用 Unix，客户端使用 Windows能充分利用它们各自的优点，这也是今后发展的一个趋势。

　　同时，金融行业的中间业务发展迅速，银行主机采用 Unix系统，而各代收代付单位多采用 Windows或 WindowsNT系统，在它们之间传输数据文件也涉及跨系统通讯的问题，通过套接字 Socket能方便地实现 Unix和 Windows的跨系统通讯，本文拟就这一问题作一探讨。

**二、 SOCKET简介**

　　TCP/IP是计算机互连最常使用的网络通讯协议， TCP/IP的核心部分由网络操作系统的内核实现，应用程序通过编程接口来访问 TCP/IP，见下图：

![](http://www.yesky.com/image20010518/42509.gif)

图1　应用程序与Windows Socket关系图

　　七十年代中，美国国防部高研署（DARPA）将TCP/IP的软件提供给加利福尼亚大学Berkeley分校后，TCP/IP很快被集成到Unix中，同时出现了许多成熟的TCP/IP应用程序接口（API）。这个API称为Socket接口。今天，SOCKET接口是TCP/IP网络最为通用的API，也是在INTERNET上进行应用开发最为通用的API。

　　九十年代初，由Microsoft联合了其他几家公司共同制定了一套WINDOWS下的网络编程接口，即Windows Sockets规范。它是Berkeley Sockets的重要扩充，主要是增加了一些异步函数，并增加了符合 Windows 消息驱动特性的网络事件异步选择机制。 Windows Sockets规范是一套开放的、支持多种协议的 Windows下的网络编程接口。目前，在实际应用中的Windows Sockets规范主要有1.1版和2.0版。两者的最重要区别是1.1版只支持TCP/IP协议，而2.0版可以支持多协议，2.0版有良好的向后兼容性，目前，Windows下的Internet软件都是基于 WinSock开发的。

　　Socket实际在计算机中提供了一个通信端口，可以通过这个端口与任何一个具有Socket接口的计算机通信。应用程序在网络上传输，接收的信息都通过这个Socket接口来实现。在应用开发中就像使用文件句柄一样，可以对 Socket句柄进行读、写操作。我们将 Socket翻译为套接字，套接字分为以下三种类型：

　　字节流套接字（Stream Socket）　是最常用的套接字类型，TCP/IP协议族中的 TCP 协议使用此类接口。字节流套接口提供面向连接的（建立虚电路）、无差错的、发送先后顺序一致的、无记录边界和非重复的网络信包传输。

　　数据报套接字 （Datagram Socket）　TCP/IP协议族中的UDP协议使用此类接口，它是无连接的服务，它以独立的信包进行网络传输，信包最大长度为32KB，传输不保证顺序性、可靠性和无重复性，它通常用于单个报文传输或可靠性不重要的场合。数据报套接口的一个重要特点是它保留了记录边界。对于这一特点。数据报套接口采用了与现在许多包交换网络（例如以太网）非常类似的模型。

　　原始数据报套接字（Raw Socket）　提供对网络下层通讯协议（如IP协议）的直接访问，它一般不是提供给普通用户的，主要用于开发新的协议或用于提取协议较隐蔽的功能。

**三、 基于SOCKET的应用开发**

![](http://www.yesky.com/image20010518/42511.gif)

图2　面向连接协议的SOCKET编程模型

　　面向连接协议的SOCKET编程模型应用最为广泛，因为面向连接协议提供了一系列的数据纠错功能，可以保证在网络上传输的数据及时、无误地到达对方。基于连接协议（字节流套接字）的服务是设计客户机/服务器应用程序时的标准，其编程模型如下：

　　尽管Windows Sockets和Berkeley Sockets都是TCP/IP应用程序的编程接口，但二者由于分属不同的系统，在某些环节仍有一些差别。Windows Sockets API没有严格地坚持Berkeley传统风格，通常这么做是因为在Windows环境中实现的难度。

　　1.套接口数据类型和错误数值

　　 Windows Sockets规范中定义了一个新的数据类型 SOCKET，这一类型的定义对于将来Windows Sockets规范的升级是必要的。这一类型的定义保证了应用程序向Win32 环境的可移植性。因为这一类型会自动地从16位升级到32位。

　　在 UNIX中，所有句柄包括套接口句柄，都是非负的短整数。 Windows Sockets 句柄则没有这一限制，除了INVALID_SOCKET 不是一个有效的套接口外，套接口可以取从0到 INVALID_SOCKET－1 之间的任意值。因为 SOCKET 类型是unsigned ，所以编译已经存在于UNIX 环境中的应用程序的源代码可能会导致 signed/unsigned 数据类型不匹配的警告。

　　因此，在socket（） 例程和accept（） 例程返回时，检查是否有错误发生就不应该再使用把返回值和－1比较的方法，或判断返回值是否为负（这两种方法在BSD 中都是很普通，很合法的途径）。取而代之的是，一个应用程序应该使用常量INVALID_SOCKET ，该常量已在WINSOCK.H 中定义。

　　例如：
　　BDS 风格
　　m_hSocket=socket（…）；
　　if（m_hSocket=－1）　　 /＊or m_hSocket＜0＊/
　　　　{…}
　　Windows风格：
　　m_hSocket=socket（…）；
　　if（m_hSocket=INVALID_SOCKET）
　　　　{…}

　　2.select（） 函数和FD_＊宏

　　由于一个套接口不再表示为非负的整数，select（） 函数在Windows Sockets API中的实现有一些变化：每一组套接口仍然用 fd_set 类型来代表，但是它并不是一个位掩码。

　　typedef struct fd_set{
　　　u_int fd_count；
　　　SOCKET fd_array；
　　} fd_set；

　　整个组的套接口是用了一个套接口的数组来实现的。为了避免潜在的危险，应用程序应该坚持用FD_XXX 宏（FD_SET,FD_ZERO,FD_CLR,FD_ISSET）来设置，初始化，清除和检查fd_set 结构。

　　3.错误代码－errno,h_errno,WSAGetLastError（）

　　 Windows Sockets 实现所设置的错误代码是无法通过 errno 变量得到的。另外对于 getXbyY（） 这一类的函数，错误代码无法从h _errno 变量得到，错误代码可以使用 WSAGetLastError（）调用得到，这种改进是为了适应多线程程序设计的需要。WSAGetLastError（）允许程序员能够得到对应于每一线程的最近的错误代码。

　　为了保持与 BSD 的兼容性，应用程序可以加入以下一行代码：

　　＃define errno WSAGetLastError（）

　　这就保证了用全程的 errno 变量所写的网络程序代码在单线程环境中可以正确使用。当然，这样做有许多明显的缺点：如果一个源程序对套接口和非套接口函数都用 errno 变量来检查错误，那么这种机制将无法工作。此外，一个应用程序不可能为 errno 赋一个新的值 （在Windows Sockets 中，WSAGetLastError（）函数可以做到这一点）。

　　例如：
　　BSD风格：
　　retcode=recv（…）；
　　if（retcode=－1 ＆＆ errno=EWOULDBLOCK）
　　{…}
　　Windows风格：
　　retcode=recv（…）；
　　if（retcode=－1 ＆＆ WSAGetLastError （）=EWOULDBLOCK）
　　{…}

　　虽然为了兼容性原因，错误常量与4.3BSD 所提供的一致：应用程序应该尽可能地使用“WSA”系列错误代码定义。且常量 SOCKET_ERROR是被用来检查API 调用失败的。虽然对这一常量的使用并不是强制性的，但这是推荐的。例如，上面程序更准确应该是：

　　retcode=recv（…）
　　if（retcode=SOCKET_ERROR＆＆WSAGetLastError（）=
　　WSAEWOULDBLOCK）
　　{…}

　　4.重命名的函数

　　有两种原因Berkeley 套接口中的函数必须重命名，以避免与其他的 API 冲突：

　　① close（） 和closesocket（） 
　　在Berkeley套接口中，套接口出现的形式与标准文件描述字相同，所以 close（） 函数可以用来象关闭文件一样关闭套接口。在Windows Sockets API中，套接字和正常文件句柄不是等同的，例如read（）,write（） 和close（） 在应用于套接口后不能保证正确工作。套接口必须使用 closesocket（）例程来关闭，用close（） 例程来关闭套接口是不正确的。

　　② ioctl（）和iooctlsocket（）
　　Windows Sockets定义ioctlsocket（） 例程，用于实现 BSD中ioctl（） 和fcntl（） 的功能。

　　5.阻塞例程和 EINPROGRESS 宏

　　虽然Windows Sockets 支持关于套接口的阻塞操作，但是这种应用是被强烈反对的，如果程序员被迫使用阻塞模式（例如一个准备移植的已有的程序，BSD 包含了大量的阻塞函数，且其默认的工作方式都是阻塞的），那么他应该清楚地知道Windows Sockets中阻塞操作的语义。

　　6.指针

　　所有应用程序与Windows Sockets 使用的指针都必须是FAR 指针，为了方便应用程序开发者作用，Windows Sockets规范定义了数据类型LPHOSTENT 。

　　7. Windows Sockets支持的最大套接口数目

　　一个Windows Sockets 应用程序可以使用的套接口的最大数目是在编译时由常量 FD_SETSIZE 决定的。这个常量在 select（） 函数中被用来组建fd_set 结构。在WINSOCK.H 中缺省值是64。如果一个应用程序希望能够使用超过64个套接口，则编程人员必须在每一个源文件包含 WINSOCK.H 前定义确切的FD_SETSIZE值。有一种方法可以完成这项工作，就是在工程项目文件中的编译器选项上加入这一定义。 FD_SETSIZE定义的值对Windows Sockets实现所支持的套接口的数目并无任何影响。

　　8.头文件

　　为了方便基于 Berkeley 套接口的已有的源代码的移植， Windows Sockets支持许多 Berkeley头文件。这些 Berkeley头文件被包含在WINSOCK.H中。所以一个 Windows Sockets应用程序只需简单的包含 WINSOCK.H就足够了（这也是一种被推荐使用的方法）。

**四、 跨系统通讯编程实例**　　下面通过一个实例来具体说明 Socket 在Unix 和Windows 跨系统通讯编程中的应用。

　　Internet 上可以提供一种叫 IRC（Internet Relay Chatting,Internet 在线聊天系统）的服务。使用者通过客户端的程序登录到 IRC 服务器上，就可以与登录在同一 IRC 服务器上的客户进行交谈，这也就是平常所说的聊天室。在这里，给出了一个在运行 TCP/IP 协议的网络上实现 IRC 服务的程序。其中，服务器运行在 SCO Open Server 5.0.5上，客户端运行在Windows98 或 Windows NT 上。

　　在一台计算机上运行服务端程序，同一网络的其他计算机上运行客户端程序，登录到服务器上，各个客户之间就可以聊天了。

　　1.服务端

　　服务端用名为 client 的整型数组记录每个客户的已连接套接口描述字，此数组中的所有元素都初始化为－1。同时服务器既要处理监听套接口，又要处理所有已连接套接口，因此需要用到 I/O 复用。通过 select 函数内核等待多个事件中的任一个发生，并仅在一个或多个事件发生或经过某指定时间后才唤醒进程。维护一个读描述字集 rset ，当有客户到达时，在数组 client中的第一个可用条目（即值为－1的第一个条目）记录其已连接套接口的描述字。同时把这个已连接描述字加到读描述字集rset 中，变量maxi 是当前使用的数组 client 的最大下标，而变量 maxfd＋1 是函数 select第一个参数的当前值。如下图：（假设有两个客户与服务器建立连接） 

![](http://www.yesky.com/image20010518/42512.gif)

图3　第二客户建立连接后的TCP服务器

![](http://www.yesky.com/image20010518/42513.gif)

图4　第二客户建立连接后的数据结构

　　服务器在指定的端口上监听，每当一个套接口接收到信息，都将会把接收到的信息发送给每一个Client 。其主要源程序如下：

　　int main（int argc,char ＊＊argv）
　　{
　　int 　　i,maxi,maxfd,listenfd,connfd,sockfd；
　　int 　　nready,client；
　　ssize_t n；
　　fd_set rset,allset；
　　char line［MAXLNE]；
　　socklen_t clilen；
　　struct sockaddr_in cliaddr,servaddr；
　　const int on=1；
　　listenfd=Socket（AF_INET,SOCK_STREAM,0）；
　　if（setsockopt （listenfd,SOL_SOCKET,SO_REUSEADDR,＆on,sizeof（on））=－1）；　　　　err_ret（″setsockopt error″）；
　　bzero（＆servaddr,sizeof（servaddr））；
　　servaddr.sin_family =AF_INET；
　　servaddr.sin_addr.s_addr =htonl（INADDR_ANY）；
　　servaddr.sin_port =htons（SERV_PORT）；
　　Bind（listenfd,（SA＊）＆servaddr,sizeof（servaddr））；
　　Listen（listenfd,LISTENQ）；
　　maxfd =listenfd； //初始化
　　maxi = －1； //最大下标
　　for（i=0；i＜FD_SETSIZE；i＋＋）
　　　　client[i]=－1； //－1表示可用
　　FD_ZERO（＆allset）；
　　FD_SET（listenfd,＆allset）；
　　for（；；）{
　　　rset=allset； //结构赋值
　　　 nready=Select（maxfd＋1,＆rset,NULL,NULL,NULL）；
　　 if （FD_ISSET（listenfd,＆rset））{ //有用户连接
　　　　clilen=sizeof（cliaddr）；
　　 　connfd=Accept（listenfd,（SA＊）＆cliaddr,＆clilen）；
　　　　for（i=0；i＜FD_SETSIZE；i＋＋）
　　　　　 if（client[i]＜ 0 {
　　　　　　 client[i]=connfd；//保存套接字
　　　　　　 break；
　　　　　　}
　　　　if（i=FD_SETSIZE）；
　　 　　err_quit（″too many clients″）；
　　　　FD_SET （connfd，＆allset）； //增加套接字到读描述字集
　　　　if（connfd＞maxfd）；
　　　　　　maxfd=connfd；
　　　　if（i＞maxi）；
　　　　　　maxi=i；
　　　　if（－－nready＜=0）
　　　　　continue；
　　}
　　for （i=0；i＜=maxi；i＋＋）{ //检查所有用户连接
　　　 if（（sockfd=client[i]）＜0）
　　　　　continue；
　　　 if（FD_ISSET（sockfd,＆rset））
{
　　　　　if（（n=Readline（sockfd,line,MAXLINE））=0）
{　　　　　　　　　　　　　　　//用户关闭连接
　　　　　　 Close（sockfd）；
　　　　　　 FD_CLR（sockfd,＆allset）；
　　　　　　 client[i]=－1
　　　　　 }else
　　　　　　　　printf（″％s″,line）；
　　　　　　　Broadcast（client,maxi,line,n）；
　　　　　　　if （－－nready＜=0）
　　　　　　　　　break；
　　　　　　　　}
　　　　　　}
　　　　}
　　}
　　//将聊天内容发送到所有已连接的用户
　　Broadcast（int client,int maxi,char＊str,size_t n）
　　{
　　　　　　int i；
　　　　　　int sockfd；
　　　　　　for（i=0；i＜maxi,i＋＋）{
　　　 　　　if（（sockfd=client[i]）＜0）
　　　　 　　continue；
　　　　　　writen（sockfd,str,n）；
　　　　}
　　}

　　2.客户端

　　为了实现非阻塞通信，利用异步选择函数 WSAAsynSelect（） 将网络事件与 WinSock 消息联系起来，由该函数注册一些用户感兴趣的网络事件（如接收缓冲区满，允许发送数据，请求连接等）。当这些被注册的网络事件发生时，应用程序的相应函数将接收到有关消息。

　　应用程序在使用Windows Sockets DLL 之前必须先调用启动函数WSAStartup（） ，该函数的功能有两点：一是由应用程序指定所要求Windows Sockets DLL 版本；二是获得系统 Windows Sockets DLL的一些技术细节。每一个WSAStartup（）函数必须和一个WSACleanup（）函数对应，当应用程序终止时，必须调用 WSACleanup（）将自己从OLL 中注销。

　　客户端程序用 VC＋＋6.0在Windows98 操作系统下设计，程序主框架由 AppWizard 生成,客户端核心代码在 CTalkDialog类中。只有一个 socket 变量m_hSocket 与服务端进行连接。连接建立好后，通过此 SOCKET发送和接收信息。

　　手工加入CTalkDialog::OnSockConnect（） ，完成基本套接字编程，为客户程序申请一个套接字，并将该套接字与指定服务器绑定，然后向服务器发出连接请求，启动异步选择函数等待服务器的响应。

　　void CTalkDialog:″OnSockConnect（）
{
　　 struct sockaddr_in servaddr；
　　 WSADATA wsaData；
　　 if（WSAStartup（WINSOCK_VERSION,＆wsaData））；
　　 {
　　　　MessageBox（″Could not load Windows Sockets DLL,″,NULL,MB_Ok）；
　　　 return；
　　}
m_hSocket=socket（AF_INET,SOCK_STREAM,0）；
　　 memset（＆servaddr,0，sizeof（servaddr））；
　　 servaddr.sin_family=AF_INET；
　　 //m_iPort,m_csIP 为通过注册对话框返回的端口号和IP地址
　　servaddr.sin_port=htons（m_iport）；
　　servaddr.sin_addr.S_un.S_addr=inet_addr（m_csIP）；
　　if （connect（m_hSocket,（SA＊）＆ servaddr,sizeof （servaddr））!=0）{
　　　　 AfxMessageBox（″连接服务器失败!″）；
　　　GetDlgItem（IDC_BUTTON_OUT）－＞EnableWindow（FALSE）；
　　　　 GetDlgItem（IDC_BUTTON_IN）－＞EnableWindowTRUE
　　　　 GetDlgItem（IDC_EDIT－SEND）－＞EnableWindow（FALSE）；
　　　　 UpdateData（FALSE）；
　　}
　　else{
　　　　 GetDlgItem（IDC_BUTTON_IN）－＞EnableWindow（FALSE）；
　　　GetDlgItem（IDC_BUTTON_OUT）－＞EnableWindow（TRUE）；
　　 GetDlgItem（IDC_EDIT－SEND）－＞EnableWindow（TRUE）；
　　　　 int iErrorCode=WSAAsyncSelect（m_hSocket,m_hWnd,WM_SOCKET_READ,　　 FD_READ）；
　　　　 if（iErrorCode=SOCKET_ERROR）
　　　　　　MessageBox（″WSAAsyncSelect failed on socket″,NULL,MB_OK）；
　　　　}
　　}　　手工加入CTalkDialog::OnSockRead（）, 响应WinSock 发来的消息
　　LRESULT CTalkDialog::OnSockRead（WPARAM wParamLPARAM 1Param）
　　{
　　　　int iRead；
　　　　int iBufferLength；
　　　　int iEnd；
　　　　int iSpaceRemain；
　　　　char chIncomingData[100]；
　　　　iBufferLength=iSpaceRemain=sizeof （chIncomingData）；
　　 iEnd=0；
　　 iSpaceRemain－=iEnd；
　　 iRead=recv（m_hSocket,（LPSTR）（chIncomingData＋iEnd）,iSpaceRemain,0）；
　　 iEnd＋=iRead；
　　 if （iRead=SOCKET_ERROR）
　　 　AfxMessageBox（″接收数据错误!″）；
　　 chIncomingData[iEnd]=‘/0’；
　　 if（lstrlen（chIncomingData）!=0）
{
　　m_csRecv=m_csRecv＋chIncomingData；
　　GetDlgItem（IDC_EDIT_RECV）－＞SetWindowText（（LPCSTR）m_csRecv）；
　　CEdit＊pEdit；
　　pEdit=（CEdit＊）GetDlgItem（IDC_EDIT_RECV）；
　　int i=pEdit－＞GetLineCount（）；
　　　　　pEdit－＞LineScroll（i,0）；
　　　　}
　　　　return（OL）；
　　}
　　手工加入CTalkDialog::OnSend（）,将指定缓冲区中的数据发送出去。
　　void CTalkDialog::OnSend（）
　　 {
　　　 UpdateData（）；
　　　 m_csSend.TrimLeft（）；
　　　 m_csSend.TrimRight（）；
　　　 if（!m_csSend.IsEmpty（））
　　 { 
m_csSend=m_csName＋″:″＋m_csSend＋″/r/n″；
　　 int nCharSend=send（m_hSocket,m_csSend,m_csSend.GetLength（）,0）；
　　 if（nCharSend=SOCKET_ERROR）
　　　　MessageBox（″ 发送数据错误!″,NULL,MB_OK）；
　　　}
　　　m_csSend=″″；
　　　UpdateData（FALSE）；
　　　CWnd ＊pEdit=GetDlgItem（IDC_EDIT_SEND）；
　　　pEdit－＞SetFocus（）；
　　　return；
　　}
　　通过ClassWizard 增加 virtual function PreTranslateMessage（）, 控制当按回车键时调用OnSend（），而不是执行缺省按钮的动作。
　　BOOL CTalkDialog::PreTranslateMessage（MSG＊pMsg）
　　{
if（pMsg－＞ message==WM_KEYDOWN ＆＆ pMsg－＞ wParam=VK_RETURN）{
　　　　　OnSend（）；
　　　　}
　　　　return CDialog::PreTranslateMessage（pMsg）；
　　}

　　为了简化设计，用户名在客户端控制，服务端只进行简单的接收信息和“广播”此信息，不进行名字校验，也就是说，可以有同名客户登录到服务端。这个程序设计虽然简单，但是已经具备了聊天室的最基本的功能。服务端程序在SCO OpenServer 5.0.5 下编译通过，客户端程序在VC＋＋6.0 下编译通过，在使用TCP/IP 协议的局域网上运行良好。

　　上述实例仅用于说明通过 Socket 编程接口能方便地实现跨系统通讯，而这种跨系统通讯在金融行业中有着越来越广泛的应用，利用它可以实现各种多媒体查询，数据传输，网络通讯等功能。因此对跨系统通讯的探讨是非常必要和有意义的，希望上述探讨对大家能有所启发。

**二、 SOCKET简介**

　　TCP/IP是计算机互连最常使用的网络通讯协议， TCP/IP的核心部分由网络操作系统的内核实现，应用程序通过编程接口来访问 TCP/IP，见下图：

![](http://www.yesky.com/image20010518/42509.gif)

图1　应用程序与Windows Socket关系图

　　七十年代中，美国国防部高研署（DARPA）将TCP/IP的软件提供给加利福尼亚大学Berkeley分校后，TCP/IP很快被集成到Unix中，同时出现了许多成熟的TCP/IP应用程序接口（API）。这个API称为Socket接口。今天，SOCKET接口是TCP/IP网络最为通用的API，也是在INTERNET上进行应用开发最为通用的API。

　　九十年代初，由Microsoft联合了其他几家公司共同制定了一套WINDOWS下的网络编程接口，即Windows Sockets规范。它是Berkeley Sockets的重要扩充，主要是增加了一些异步函数，并增加了符合 Windows 消息驱动特性的网络事件异步选择机制。 Windows Sockets规范是一套开放的、支持多种协议的 Windows下的网络编程接口。目前，在实际应用中的Windows Sockets规范主要有1.1版和2.0版。两者的最重要区别是1.1版只支持TCP/IP协议，而2.0版可以支持多协议，2.0版有良好的向后兼容性，目前，Windows下的Internet软件都是基于 WinSock开发的。

　　Socket实际在计算机中提供了一个通信端口，可以通过这个端口与任何一个具有Socket接口的计算机通信。应用程序在网络上传输，接收的信息都通过这个Socket接口来实现。在应用开发中就像使用文件句柄一样，可以对 Socket句柄进行读、写操作。我们将 Socket翻译为套接字，套接字分为以下三种类型：

　　字节流套接字（Stream Socket）　是最常用的套接字类型，TCP/IP协议族中的 TCP 协议使用此类接口。字节流套接口提供面向连接的（建立虚电路）、无差错的、发送先后顺序一致的、无记录边界和非重复的网络信包传输。

　　数据报套接字 （Datagram Socket）　TCP/IP协议族中的UDP协议使用此类接口，它是无连接的服务，它以独立的信包进行网络传输，信包最大长度为32KB，传输不保证顺序性、可靠性和无重复性，它通常用于单个报文传输或可靠性不重要的场合。数据报套接口的一个重要特点是它保留了记录边界。对于这一特点。数据报套接口采用了与现在许多包交换网络（例如以太网）非常类似的模型。

　　原始数据报套接字（Raw Socket）　提供对网络下层通讯协议（如IP协议）的直接访问，它一般不是提供给普通用户的，主要用于开发新的协议或用于提取协议较隐蔽的功能。

**三、 基于SOCKET的应用开发**

![](http://www.yesky.com/image20010518/42511.gif)

图2　面向连接协议的SOCKET编程模型

　　面向连接协议的SOCKET编程模型应用最为广泛，因为面向连接协议提供了一系列的数据纠错功能，可以保证在网络上传输的数据及时、无误地到达对方。基于连接协议（字节流套接字）的服务是设计客户机/服务器应用程序时的标准，其编程模型如下：

　　尽管Windows Sockets和Berkeley Sockets都是TCP/IP应用程序的编程接口，但二者由于分属不同的系统，在某些环节仍有一些差别。Windows Sockets API没有严格地坚持Berkeley传统风格，通常这么做是因为在Windows环境中实现的难度。

　　1.套接口数据类型和错误数值

　　 Windows Sockets规范中定义了一个新的数据类型 SOCKET，这一类型的定义对于将来Windows Sockets规范的升级是必要的。这一类型的定义保证了应用程序向Win32 环境的可移植性。因为这一类型会自动地从16位升级到32位。

　　在 UNIX中，所有句柄包括套接口句柄，都是非负的短整数。 Windows Sockets 句柄则没有这一限制，除了INVALID_SOCKET 不是一个有效的套接口外，套接口可以取从0到 INVALID_SOCKET－1 之间的任意值。因为 SOCKET 类型是unsigned ，所以编译已经存在于UNIX 环境中的应用程序的源代码可能会导致 signed/unsigned 数据类型不匹配的警告。

　　因此，在socket（） 例程和accept（） 例程返回时，检查是否有错误发生就不应该再使用把返回值和－1比较的方法，或判断返回值是否为负（这两种方法在BSD 中都是很普通，很合法的途径）。取而代之的是，一个应用程序应该使用常量INVALID_SOCKET ，该常量已在WINSOCK.H 中定义。

　　例如：
　　BDS 风格
　　m_hSocket=socket（…）；
　　if（m_hSocket=－1）　　 /＊or m_hSocket＜0＊/
　　　　{…}
　　Windows风格：
　　m_hSocket=socket（…）；
　　if（m_hSocket=INVALID_SOCKET）
　　　　{…}

　　2.select（） 函数和FD_＊宏

　　由于一个套接口不再表示为非负的整数，select（） 函数在Windows Sockets API中的实现有一些变化：每一组套接口仍然用 fd_set 类型来代表，但是它并不是一个位掩码。

　　typedef struct fd_set{
　　　u_int fd_count；
　　　SOCKET fd_array；
　　} fd_set；

　　整个组的套接口是用了一个套接口的数组来实现的。为了避免潜在的危险，应用程序应该坚持用FD_XXX 宏（FD_SET,FD_ZERO,FD_CLR,FD_ISSET）来设置，初始化，清除和检查fd_set 结构。

　　3.错误代码－errno,h_errno,WSAGetLastError（）

　　 Windows Sockets 实现所设置的错误代码是无法通过 errno 变量得到的。另外对于 getXbyY（） 这一类的函数，错误代码无法从h _errno 变量得到，错误代码可以使用 WSAGetLastError（）调用得到，这种改进是为了适应多线程程序设计的需要。WSAGetLastError（）允许程序员能够得到对应于每一线程的最近的错误代码。

　　为了保持与 BSD 的兼容性，应用程序可以加入以下一行代码：

　　＃define errno WSAGetLastError（）

　　这就保证了用全程的 errno 变量所写的网络程序代码在单线程环境中可以正确使用。当然，这样做有许多明显的缺点：如果一个源程序对套接口和非套接口函数都用 errno 变量来检查错误，那么这种机制将无法工作。此外，一个应用程序不可能为 errno 赋一个新的值 （在Windows Sockets 中，WSAGetLastError（）函数可以做到这一点）。

　　例如：
　　BSD风格：
　　retcode=recv（…）；
　　if（retcode=－1 ＆＆ errno=EWOULDBLOCK）
　　{…}
　　Windows风格：
　　retcode=recv（…）；
　　if（retcode=－1 ＆＆ WSAGetLastError （）=EWOULDBLOCK）
　　{…}

　　虽然为了兼容性原因，错误常量与4.3BSD 所提供的一致：应用程序应该尽可能地使用“WSA”系列错误代码定义。且常量 SOCKET_ERROR是被用来检查API 调用失败的。虽然对这一常量的使用并不是强制性的，但这是推荐的。例如，上面程序更准确应该是：

　　retcode=recv（…）
　　if（retcode=SOCKET_ERROR＆＆WSAGetLastError（）=
　　WSAEWOULDBLOCK）
　　{…}

　　4.重命名的函数

　　有两种原因Berkeley 套接口中的函数必须重命名，以避免与其他的 API 冲突：

　　① close（） 和closesocket（） 
　　在Berkeley套接口中，套接口出现的形式与标准文件描述字相同，所以 close（） 函数可以用来象关闭文件一样关闭套接口。在Windows Sockets API中，套接字和正常文件句柄不是等同的，例如read（）,write（） 和close（） 在应用于套接口后不能保证正确工作。套接口必须使用 closesocket（）例程来关闭，用close（） 例程来关闭套接口是不正确的。

　　② ioctl（）和iooctlsocket（）
　　Windows Sockets定义ioctlsocket（） 例程，用于实现 BSD中ioctl（） 和fcntl（） 的功能。

　　5.阻塞例程和 EINPROGRESS 宏

　　虽然Windows Sockets 支持关于套接口的阻塞操作，但是这种应用是被强烈反对的，如果程序员被迫使用阻塞模式（例如一个准备移植的已有的程序，BSD 包含了大量的阻塞函数，且其默认的工作方式都是阻塞的），那么他应该清楚地知道Windows Sockets中阻塞操作的语义。

　　6.指针

　　所有应用程序与Windows Sockets 使用的指针都必须是FAR 指针，为了方便应用程序开发者作用，Windows Sockets规范定义了数据类型LPHOSTENT 。

　　7. Windows Sockets支持的最大套接口数目

　　一个Windows Sockets 应用程序可以使用的套接口的最大数目是在编译时由常量 FD_SETSIZE 决定的。这个常量在 select（） 函数中被用来组建fd_set 结构。在WINSOCK.H 中缺省值是64。如果一个应用程序希望能够使用超过64个套接口，则编程人员必须在每一个源文件包含 WINSOCK.H 前定义确切的FD_SETSIZE值。有一种方法可以完成这项工作，就是在工程项目文件中的编译器选项上加入这一定义。 FD_SETSIZE定义的值对Windows Sockets实现所支持的套接口的数目并无任何影响。

　　8.头文件

　　为了方便基于 Berkeley 套接口的已有的源代码的移植， Windows Sockets支持许多 Berkeley头文件。这些 Berkeley头文件被包含在WINSOCK.H中。所以一个 Windows Sockets应用程序只需简单的包含 WINSOCK.H就足够了（这也是一种被推荐使用的方法）。

**四、 跨系统通讯编程实例**　　下面通过一个实例来具体说明 Socket 在Unix 和Windows 跨系统通讯编程中的应用。

　　Internet 上可以提供一种叫 IRC（Internet Relay Chatting,Internet 在线聊天系统）的服务。使用者通过客户端的程序登录到 IRC 服务器上，就可以与登录在同一 IRC 服务器上的客户进行交谈，这也就是平常所说的聊天室。在这里，给出了一个在运行 TCP/IP 协议的网络上实现 IRC 服务的程序。其中，服务器运行在 SCO Open Server 5.0.5上，客户端运行在Windows98 或 Windows NT 上。

　　在一台计算机上运行服务端程序，同一网络的其他计算机上运行客户端程序，登录到服务器上，各个客户之间就可以聊天了。

　　1.服务端

　　服务端用名为 client 的整型数组记录每个客户的已连接套接口描述字，此数组中的所有元素都初始化为－1。同时服务器既要处理监听套接口，又要处理所有已连接套接口，因此需要用到 I/O 复用。通过 select 函数内核等待多个事件中的任一个发生，并仅在一个或多个事件发生或经过某指定时间后才唤醒进程。维护一个读描述字集 rset ，当有客户到达时，在数组 client中的第一个可用条目（即值为－1的第一个条目）记录其已连接套接口的描述字。同时把这个已连接描述字加到读描述字集rset 中，变量maxi 是当前使用的数组 client 的最大下标，而变量 maxfd＋1 是函数 select第一个参数的当前值。如下图：（假设有两个客户与服务器建立连接） 

![](http://www.yesky.com/image20010518/42512.gif)

图3　第二客户建立连接后的TCP服务器

![](http://www.yesky.com/image20010518/42513.gif)

图4　第二客户建立连接后的数据结构

　　服务器在指定的端口上监听，每当一个套接口接收到信息，都将会把接收到的信息发送给每一个Client 。其主要源程序如下：

　　int main（int argc,char ＊＊argv）
　　{
　　int 　　i,maxi,maxfd,listenfd,connfd,sockfd；
　　int 　　nready,client；
　　ssize_t n；
　　fd_set rset,allset；
　　char line［MAXLNE]；
　　socklen_t clilen；
　　struct sockaddr_in cliaddr,servaddr；
　　const int on=1；
　　listenfd=Socket（AF_INET,SOCK_STREAM,0）；
　　if（setsockopt （listenfd,SOL_SOCKET,SO_REUSEADDR,＆on,sizeof（on））=－1）；　　　　err_ret（″setsockopt error″）；
　　bzero（＆servaddr,sizeof（servaddr））；
　　servaddr.sin_family =AF_INET；
　　servaddr.sin_addr.s_addr =htonl（INADDR_ANY）；
　　servaddr.sin_port =htons（SERV_PORT）；
　　Bind（listenfd,（SA＊）＆servaddr,sizeof（servaddr））；
　　Listen（listenfd,LISTENQ）；
　　maxfd =listenfd； //初始化
　　maxi = －1； //最大下标
　　for（i=0；i＜FD_SETSIZE；i＋＋）
　　　　client[i]=－1； //－1表示可用
　　FD_ZERO（＆allset）；
　　FD_SET（listenfd,＆allset）；
　　for（；；）{
　　　rset=allset； //结构赋值
　　　 nready=Select（maxfd＋1,＆rset,NULL,NULL,NULL）；
　　 if （FD_ISSET（listenfd,＆rset））{ //有用户连接
　　　　clilen=sizeof（cliaddr）；
　　 　connfd=Accept（listenfd,（SA＊）＆cliaddr,＆clilen）；
　　　　for（i=0；i＜FD_SETSIZE；i＋＋）
　　　　　 if（client[i]＜ 0 {
　　　　　　 client[i]=connfd；//保存套接字
　　　　　　 break；
　　　　　　}
　　　　if（i=FD_SETSIZE）；
　　 　　err_quit（″too many clients″）；
　　　　FD_SET （connfd，＆allset）； //增加套接字到读描述字集
　　　　if（connfd＞maxfd）；
　　　　　　maxfd=connfd；
　　　　if（i＞maxi）；
　　　　　　maxi=i；
　　　　if（－－nready＜=0）
　　　　　continue；
　　}
　　for （i=0；i＜=maxi；i＋＋）{ //检查所有用户连接
　　　 if（（sockfd=client[i]）＜0）
　　　　　continue；
　　　 if（FD_ISSET（sockfd,＆rset））
{
　　　　　if（（n=Readline（sockfd,line,MAXLINE））=0）
{　　　　　　　　　　　　　　　//用户关闭连接
　　　　　　 Close（sockfd）；
　　　　　　 FD_CLR（sockfd,＆allset）；
　　　　　　 client[i]=－1
　　　　　 }else
　　　　　　　　printf（″％s″,line）；
　　　　　　　Broadcast（client,maxi,line,n）；
　　　　　　　if （－－nready＜=0）
　　　　　　　　　break；
　　　　　　　　}
　　　　　　}
　　　　}
　　}
　　//将聊天内容发送到所有已连接的用户
　　Broadcast（int client,int maxi,char＊str,size_t n）
　　{
　　　　　　int i；
　　　　　　int sockfd；
　　　　　　for（i=0；i＜maxi,i＋＋）{
　　　 　　　if（（sockfd=client[i]）＜0）
　　　　 　　continue；
　　　　　　writen（sockfd,str,n）；
　　　　}
　　}

　　2.客户端

　　为了实现非阻塞通信，利用异步选择函数 WSAAsynSelect（） 将网络事件与 WinSock 消息联系起来，由该函数注册一些用户感兴趣的网络事件（如接收缓冲区满，允许发送数据，请求连接等）。当这些被注册的网络事件发生时，应用程序的相应函数将接收到有关消息。

　　应用程序在使用Windows Sockets DLL 之前必须先调用启动函数WSAStartup（） ，该函数的功能有两点：一是由应用程序指定所要求Windows Sockets DLL 版本；二是获得系统 Windows Sockets DLL的一些技术细节。每一个WSAStartup（）函数必须和一个WSACleanup（）函数对应，当应用程序终止时，必须调用 WSACleanup（）将自己从OLL 中注销。

　　客户端程序用 VC＋＋6.0在Windows98 操作系统下设计，程序主框架由 AppWizard 生成,客户端核心代码在 CTalkDialog类中。只有一个 socket 变量m_hSocket 与服务端进行连接。连接建立好后，通过此 SOCKET发送和接收信息。

　　手工加入CTalkDialog::OnSockConnect（） ，完成基本套接字编程，为客户程序申请一个套接字，并将该套接字与指定服务器绑定，然后向服务器发出连接请求，启动异步选择函数等待服务器的响应。

　　void CTalkDialog:″OnSockConnect（）
{
　　 struct sockaddr_in servaddr；
　　 WSADATA wsaData；
　　 if（WSAStartup（WINSOCK_VERSION,＆wsaData））；
　　 {
　　　　MessageBox（″Could not load Windows Sockets DLL,″,NULL,MB_Ok）；
　　　 return；
　　}
m_hSocket=socket（AF_INET,SOCK_STREAM,0）；
　　 memset（＆servaddr,0，sizeof（servaddr））；
　　 servaddr.sin_family=AF_INET；
　　 //m_iPort,m_csIP 为通过注册对话框返回的端口号和IP地址
　　servaddr.sin_port=htons（m_iport）；
　　servaddr.sin_addr.S_un.S_addr=inet_addr（m_csIP）；
　　if （connect（m_hSocket,（SA＊）＆ servaddr,sizeof （servaddr））!=0）{
　　　　 AfxMessageBox（″连接服务器失败!″）；
　　　GetDlgItem（IDC_BUTTON_OUT）－＞EnableWindow（FALSE）；
　　　　 GetDlgItem（IDC_BUTTON_IN）－＞EnableWindowTRUE
　　　　 GetDlgItem（IDC_EDIT－SEND）－＞EnableWindow（FALSE）；
　　　　 UpdateData（FALSE）；
　　}
　　else{
　　　　 GetDlgItem（IDC_BUTTON_IN）－＞EnableWindow（FALSE）；
　　　GetDlgItem（IDC_BUTTON_OUT）－＞EnableWindow（TRUE）；
　　 GetDlgItem（IDC_EDIT－SEND）－＞EnableWindow（TRUE）；
　　　　 int iErrorCode=WSAAsyncSelect（m_hSocket,m_hWnd,WM_SOCKET_READ,　　 FD_READ）；
　　　　 if（iErrorCode=SOCKET_ERROR）
　　　　　　MessageBox（″WSAAsyncSelect failed on socket″,NULL,MB_OK）；
　　　　}
　　}　　手工加入CTalkDialog::OnSockRead（）, 响应WinSock 发来的消息
　　LRESULT CTalkDialog::OnSockRead（WPARAM wParamLPARAM 1Param）
　　{
　　　　int iRead；
　　　　int iBufferLength；
　　　　int iEnd；
　　　　int iSpaceRemain；
　　　　char chIncomingData[100]；
　　　　iBufferLength=iSpaceRemain=sizeof （chIncomingData）；
　　 iEnd=0；
　　 iSpaceRemain－=iEnd；
　　 iRead=recv（m_hSocket,（LPSTR）（chIncomingData＋iEnd）,iSpaceRemain,0）；
　　 iEnd＋=iRead；
　　 if （iRead=SOCKET_ERROR）
　　 　AfxMessageBox（″接收数据错误!″）；
　　 chIncomingData[iEnd]=‘/0’；
　　 if（lstrlen（chIncomingData）!=0）
{
　　m_csRecv=m_csRecv＋chIncomingData；
　　GetDlgItem（IDC_EDIT_RECV）－＞SetWindowText（（LPCSTR）m_csRecv）；
　　CEdit＊pEdit；
　　pEdit=（CEdit＊）GetDlgItem（IDC_EDIT_RECV）；
　　int i=pEdit－＞GetLineCount（）；
　　　　　pEdit－＞LineScroll（i,0）；
　　　　}
　　　　return（OL）；
　　}
　　手工加入CTalkDialog::OnSend（）,将指定缓冲区中的数据发送出去。
　　void CTalkDialog::OnSend（）
　　 {
　　　 UpdateData（）；
　　　 m_csSend.TrimLeft（）；
　　　 m_csSend.TrimRight（）；
　　　 if（!m_csSend.IsEmpty（））
　　 { 
m_csSend=m_csName＋″:″＋m_csSend＋″/r/n″；
　　 int nCharSend=send（m_hSocket,m_csSend,m_csSend.GetLength（）,0）；
　　 if（nCharSend=SOCKET_ERROR）
　　　　MessageBox（″ 发送数据错误!″,NULL,MB_OK）；
　　　}
　　　m_csSend=″″；
　　　UpdateData（FALSE）；
　　　CWnd ＊pEdit=GetDlgItem（IDC_EDIT_SEND）；
　　　pEdit－＞SetFocus（）；
　　　return；
　　}
　　通过ClassWizard 增加 virtual function PreTranslateMessage（）, 控制当按回车键时调用OnSend（），而不是执行缺省按钮的动作。
　　BOOL CTalkDialog::PreTranslateMessage（MSG＊pMsg）
　　{
if（pMsg－＞ message==WM_KEYDOWN ＆＆ pMsg－＞ wParam=VK_RETURN）{
　　　　　OnSend（）；
　　　　}
　　　　return CDialog::PreTranslateMessage（pMsg）；
　　}

　　为了简化设计，用户名在客户端控制，服务端只进行简单的接收信息和“广播”此信息，不进行名字校验，也就是说，可以有同名客户登录到服务端。这个程序设计虽然简单，但是已经具备了聊天室的最基本的功能。服务端程序在SCO OpenServer 5.0.5 下编译通过，客户端程序在VC＋＋6.0 下编译通过，在使用TCP/IP 协议的局域网上运行良好。

　　上述实例仅用于说明通过 Socket 编程接口能方便地实现跨系统通讯，而这种跨系统通讯在金融行业中有着越来越广泛的应用，利用它可以实现各种多媒体查询，数据传输，网络通讯等功能。因此对跨系统通讯的探讨是非常必要和有意义的，希望上述探讨对大家能有所启发。

