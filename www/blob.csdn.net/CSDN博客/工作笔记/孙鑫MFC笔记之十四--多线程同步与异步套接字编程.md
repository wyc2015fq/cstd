# 孙鑫MFC笔记之十四--多线程同步与异步套接字编程 - 工作笔记 - CSDN博客





2012年06月26日 09:00:21[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7444









多线程同步编程方法1

事件对象


n事件对象也属于内核对象，包含一个使用计数，一个用于指明该事件是一个自动重置的事件还是一个人工重置的事件的布尔值，另一个用于指明该事件处于已通知状态还是未通知状态的布尔值。

n有两种不同类型的事件对象。一种是人工重置的事件，另一种是自动重置的事件。当人工重置的事件得到通知时，等待该事件的所有线程均变为可调度线程。当一个自动重置的事件得到通知时，等待该事件的线程中只有一个线程变为可调度线程。




多线程同步编程方法2

关键代码段


n关键代码段(临界区)工作在用户方式下。 

n关键代码段(临界区)是指一个小代码段，在代码能够执行前，它必须独占对某些资源的访问权。




线程死锁


n哲学家进餐的问题 

n线程1拥有了临界区对象A，等待临界区对象B的拥有权，线程2拥有了临界区对象B，等待临界区对象A的拥有权，就造成了死锁。



3种方法的区别



n互斥对象和事件对象属于内核对象，利用内核对象进行线程同步，速度较慢，但利用互斥对象和事件对象这样的内核对象，可以在多个进程中的各个线程间进行同步。



n关键代码段是工作在用户方式下，同步速度较快，但在使用关键代码段时，很容易进入死锁状态，因为在等待进入关键代码段时无法设定超时值。



异步套接字编程：





nWindows套接字在两种模式下执行I/O操作，阻塞和非阻塞。在阻塞模式下，在I/O操作完成前，执行操作的Winsock函数会一直等待下去，不会立即返回程序(将控制权交还给程序)。而在非阻塞模式下，Winsock函数无论如何都会立即返回。

nWindows Sockets为了支持Windows消息驱动机制，使应用程序开发者能够方便地处理网络通信，它对网络事件采用了基于消息的异步存取策略。

nWindows Sockets的异步选择函数WSAAsyncSelect()提供了消息机制的网络事件选择，当使用它登记的网络事件发生时，Windows应用程序相应的窗口函数将收到一个消息，消息中指示了发生的网络事件，以及与事件相关的一些信息。



WSAAsyncSelect()函数，自动设置套接字为非阻塞模式。

WSAAsyncSelect(

    SOCKET s,//套接字

    HWND hWnd,//窗口句柄

    u_int wMsg,//消息

    long lEvent//事件

    );



int WSAEnumProtocols( LPINT lpiProtocols, LPWSAPROTOCOL_INFO lpProtocolBuffer, ILPDWORD lpdwBufferLength);

nWin32平台支持多种不同的网络协议，采用Winsock2，就可以编写可直接使用任何一种协议的网络应用程序了。通过WSAEnumProtocols函数可以获得系统中安装的网络协议的相关信息。

nlpiProtocols，一个以NULL结尾的协议标识号数组。这个参数是可选的，如果lpiProtocols为NULL，则返回所有可用协议的信息，否则，只返回数组中列出的协议信息。

nlpProtocolBuffer，[out]，一个用WSAPROTOCOL_INFO结构体填充的缓冲区。 WSAPROTOCOL_INFO结构体用来存放或得到一个指定协议的完整信息。

nlpdwBufferLength，[in, out]，在输入时，指定传递给WSAEnumProtocols()函数的lpProtocolBuffer缓冲区的长度；在输出时，存有获取所有请求信息需传递给WSAEnumProtocols ()函数的最小缓冲区长度。这个函数不能重复调用，传入的缓冲区必须足够大以便能存放所有的元素。这个规定降低了该函数的复杂度，并且由于一个 机器上装载的协议数目往往是很少的，所以并不会产生问题。





SOCKET WSASocket( int af, int type, int protocol, LPWSAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags);

n前三个参数和socket()函数的前三个参数含义一样。 

nlpProtocolInfo，一个指向WSAPROTOCOL_INFO结构体的指针，该结构定义了所创建的套接字的特性。如果lpProtocolInfo为NULL，则WinSock2 DLL使用前三个参数来决定使用哪一个服务提供者，它选择能够支持规定的地址族、套接字类型和协议值的第一个传输提供者。如果lpProtocolInfo不为NULL，则套接字绑定到与指定的结构WSAPROTOCOL_INFO相关的提供者。

ng，保留的。 

ndwFlags，套接字属性的描述。





int WSARecvFrom( SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORDlpFlags, struct sockaddr FAR *lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine );

ns，标识套接字的描述符。 

nlpBuffers，[in, out]，一个指向WSABUF结构体的指针。每一个WSABUF结构体包含一个缓冲区的指针和缓冲区的长度。 

ndwBufferCount，lpBuffers数组中WSABUF结构体的数目。 

nlpNumberOfBytesRecvd，[out]，如果接收操作立即完成，则为一个指向本次调用所接收的字节数的指针。 

nlpFlags，[in, out]，一个指向标志位的指针。 

nlpFrom，[out]，可选指针，指向重叠操作完成后存放源地址的缓冲区。 

nlpFromlen，[in, out]，指向from缓冲区大小的指针，仅当指定了lpFrom才需要。 

nlpOverlapped，一个指向WSAOVERLAPPED结构体的指针(对于非重叠套接字则忽略)。 

nlpCompletionRoutine，一个指向接收操作完成时调用的完成例程的指针(对于非重叠套接字则忽略)。 











   int WSASendTo( SOCKET s, LPWSABUFlpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr FAR *lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINElpCompletionRoutine );


ns，标识一个套接字(可能已连接)的描述符。 

nlpBuffers，一个指向WSABUF结构体的指针。每一个WSABUF结构体包含一个缓冲区的指针和缓冲区的长度。 

ndwBufferCount，lpBuffers数组中WSABUF结构体的数目。 

nlpNumberOfBytesSent，[out]，如果发送操作立即完成，则为一个指向本次调用所发送的字节数的指针。 

ndwFlags，指示影响操作行为的标志位。 

nlpTo，可选指针，指向目标套接字的地址。 

niToLen，lpTo中地址的长度。 

nlpOverlapped，一个指向WSAOVERLAPPED结构的指针(对于非重叠套接字则忽略)。 

nlpCompletionRoutine，一个指向接收操作完成时调用的完成例程的指针(对于非重叠套接字则忽略)。 







使用异步套接字编写网络聊天室

  1)加载套接字库，进行版本协商，包含头文件，链接库文件，这次请示的是2.2版本！

  2）在类CChatDlg中增加一个成员变量m_socket,在析构函数中释放这个变量

  3）利用WSASocket()创建套接字（数据报类型的UDP型的）

  4）然后调用WSAAsyncSelect(m_socket,m_hWnd,UM_SOCK,FD_READ)为网络事件定义消息！此时如果发生FD_READ消息，系统会发送UM_SOCK消息给应用程序！程序并不会阻塞在这儿了！

  以上是在BOOL CChatDlg::OnInitDialog()完成

  5）然后完成消息响应！

  头文件中：#define UM_SOCK  WM_USER+1

afx_msg void OnSock(WPARAM,LPARAM);

   源文件中：

    ON_MESSAGE(UM_SOCK,OnSock)

    实现消息响应函数：void CChatDlg::OnSock(WPARAM wParam,LPARAM lParam)

{

switch(LOWORD(lParam))

{

case FD_READ:

  WSABUF wsabuf;

  wsabuf.buf=new char[200];

  wsabuf.len=200;

  DWORD dwRead;

  DWORD dwFlag=0;

  SOCKADDR_IN addrFrom;

  int len=sizeof(SOCKADDR);

  CString str;

  CString strTemp;

  HOSTENT *pHost;

  if(SOCKET_ERROR==WSARecvFrom(m_socket,&wsabuf,1,&dwRead,&dwFlag,

      (SOCKADDR*)&addrFrom,&len,NULL,NULL))

  {

   MessageBox("接收数据失败！");

   return;

  }

  pHost=gethostbyaddr((char*)&addrFrom.sin_addr.S_un.S_addr,4,AF_INET);

  //str.Format("%s说 :%s",inet_ntoa(addrFrom.sin_addr),wsabuf.buf);

  str.Format("%s说 :%s",pHost->h_name,wsabuf.buf);

  str+="\r\n";

  GetDlgItemText(IDC_EDIT_RECV,strTemp);

  str+=strTemp;

  SetDlgItemText(IDC_EDIT_RECV,str);

  break;

}

}

OK！

      6）完成数据发送的功能！

      void CChatDlg::OnBtnSend() 

{

// TOD Add your control notification handler code here

DWORD dwIP;

CString strSend;

WSABUF wsabuf;

DWORD dwSend;

int len;

CString strHostName;

SOCKADDR_IN addrTo;

HOSTENT* pHost;

if(GetDlgItemText(IDC_EDIT_HOSTNAME,strHostName),strHostName=="")

{

  ((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS1))->GetAddress(dwIP);

  addrTo.sin_addr.S_un.S_addr=htonl(dwIP);

}

else

{

  pHost=gethostbyname(strHostName);

  addrTo.sin_addr.S_un.S_addr=*((DWORD*)pHost->h_addr_list[0]);

}


addrTo.sin_family=AF_INET;

addrTo.sin_port=htons(6000);



GetDlgItemText(IDC_EDIT_SEND,strSend);

len=strSend.GetLength();

wsabuf.buf=strSend.GetBuffer(len);

wsabuf.len=len+1;



SetDlgItemText(IDC_EDIT_SEND,"");



if(SOCKET_ERROR==WSASendTo(m_socket,&wsabuf,1,&dwSend,0,

   (SOCKADDR*)&addrTo,sizeof(SOCKADDR),NULL,NULL))

{

  MessageBox("发送数据失败！");

  return;

}



}



      7）完成将主机名转换为IP地址的功能，以前将IP地址转换为主机名的功能

嘿嘿，单线程的聊天室创建完毕！性能并且非常出色！










