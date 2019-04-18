# vc网络编程入门  - 深之JohnChen的专栏 - CSDN博客

2005年11月30日 14:43:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2913


                对于许多初学者来说，网络通信程序的开发，普遍的一个现象就是觉得难以入手。许多概念，诸如：同步(Sync)/异步(Async)，阻塞(Block)/非阻塞(Unblock)等，初学者往往迷惑不清，只知其所以而不知起所以然。 
       同步方式指的是发送方不等接收方响应，便接着发下个数据包的通信方式；而异步指发送方发出数据后，等收到接收方发回的响应，才发下一个数据包的通信方式。 
      阻塞套接字是指执行此套接字的网络调用时，直到成功才返回，否则一直阻塞在此网络调用上，比如调用recv()函数读取网络缓冲区中的数据，如果没有数据到达，将一直挂在recv()这个函数调用上，直到读到一些数据，此函数调用才返回；而非阻塞套接字是指执行此套接字的网络调用时，不管是否执行成功，都立即返回。比如调用recv()函数读取网络缓冲区中数据，不管是否读到数据都立即返回，而不会一直挂在此函数调用上。在实际Windows网络通信软件开发中，异步非阻塞套接字是用的最多的。平常所说的C/S（客户端/服务器）结构的软件就是异步非阻塞模式的。 
       对于这些概念，初学者的理解也许只能似是而非，我将用一个最简单的例子说明异步非阻塞Socket的基本原理和工作机制。目的是让初学者不仅对Socket异步非阻塞的概念有个非常透彻的理解，而且也给他们提供一个用Socket开发网络通信应用程序的快速入门方法。操作系统是Windows 98（或NT4.0），开发工具是Visual C++6.0。 
       MFC提供了一个异步类CAsyncSocket，它封装了异步、非阻塞Socket的基本功能，用它做常用的网络通信软件很方便。但它屏蔽了Socket的异步、非阻塞等概念，开发人员无需了解异步、非阻塞Socket的原理和工作机制。因此，建议初学者学习编网络通信程序时，暂且不要用MFC提供的类，而先用Winsock2  API，这样有助于对异步、非阻塞Socket编程机制的理解。 
       为了简单起见，服务器端和客户端的应用程序均是基于MFC的标准对话框，网络通信部分基于Winsock2 API实现。 
       先做服务器端应用程序。 
       用MFC向导做一个基于对话框的应用程序SocketSever，注意第三步中不要选上Windwos Sockets选项。在做好工程后，创建一个SeverSock，将它设置为异步非阻塞模式，并为它注册各种网络异步事件，然后与自定义的网络异步事件联系上，最后还要将它设置为监听模式。在自定义的网络异步事件的回调函数中，你可以得到各种网络异步事件，根据它们的类型，做不同的处理。下面将详细介绍如何编写相关代码。 
       在SocketSeverDlg.h文件的类定义之前增加如下定义： 
      #define  NETWORK_EVENT  WM_USER+166  file://定义网络事件 

       SOCKET ServerSock; file://服务器端Socket 
      在类定义中增加如下定义： 
      class CSocketSeverDlg : CDialog 
     { 
                 … 
      public: 
         SOCKET ClientSock[CLNT_MAX_NUM]; file://存储与客户端通信的Socket的数组 

         /*各种网络异步事件的处理函数*/ 
       void OnClose(SOCKET CurSock);   file://对端Socket断开 
        void OnSend(SOCKET CurSock);   file://发送网络数据包 
       void OnReceive(SOCKET CurSock); file://网络数据包到达 
       void OnAccept(SOCKET CurSock);  file://客户端连接请求 

        BOOL InitNetwork();  file://初始化网络函数 
         void OnNetEvent(WPARAM wParam, LPARAM lParam); file://异步事件回调函数 
                … 
     }; 

      在SocketSeverDlg.cpp文件中增加消息映射，其中OnNetEvent是异步事件回调函数名： 
               ON_MESSAGE(NETWORK_EVENT,OnNetEvent) 
      定义初始化网络函数，在SocketSeverDlg.cpp文件的OnInitDialog（）中调此函数即可。 
      BOOL CSocketSeverDlg::InitNetwork() 
      { 
           WSADATA wsaData; 

          file://初始化TCP协议 
          BOOL ret = WSAStartup(MAKEWORD(2,2), &wsaData); 
         if(ret != 0) 
          { 
                MessageBox("初始化网络协议失败!"); 
               return FALSE; 
          } 

          file://创建服务器端套接字 
          ServerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
           if(ServerSock == INVALID_SOCKET) 
          { 
               MessageBox("创建套接字失败!"); 
              closesocket(ServerSock); 
              WSACleanup(); 
               return FALSE; 
          } 

          file://绑定到本地一个端口上 
          sockaddr_in localaddr; 
          localaddr.sin_family = AF_INET; 
          localaddr.sin_port = htons(8888);  file://端口号不要与其他应用程序冲突 
          localaddr.sin_addr.s_addr = 0; 
         if(bind(ServerSock ,(struct sockaddr*)&localaddr,sizeof(sockaddr)) 
                                          = = SOCKET_ERROR) 
         { 
               MessageBox("绑定地址失败!"); 
               closesocket(ServerSock); 
               WSACleanup(); 
               return FALSE; 
         } 

         file://将SeverSock设置为异步非阻塞模式，并为它注册各种网络异步事件，其 中   m_hWnd       
         file://为应用程序的主对话框或主窗口的句柄 
        if(WSAAsyncSelect(ServerSock, m_hWnd, NETWORK_EVENT, 
              FD_ACCEPT | FD_CLOSE | FD_READ | FD_WRITE) == SOCKET_ERROR) 
         { 
               MessageBox("注册网络异步事件失败!"); 
               WSACleanup(); 
               return FALSE; 
         } 
         listen(ServerSock, 5); file://设置侦听模式 
        return TRUE; 
     } 

     下面定义网络异步事件的回调函数 
     void CSocketSeverDlg::OnNetEvent(WPARAM wParam, LPARAM lParam) 
     { 
        file://调用Winsock API函数，得到网络事件类型 
        int iEvent = WSAGETSELECTEVENT(lParam); 

        file://调用Winsock API函数，得到发生此事件的客户端套接字 
        SOCKET CurSock= (SOCKET)wParam; 

        switch(iEvent) 
        { 
        case FD_ACCEPT:      file://客户端连接请求事件 
                OnAccept(CurSock); 
                break; 
        case FD_CLOSE:       file://客户端断开事件: 
                OnClose(CurSock); 
                break; 
        case FD_READ:        file://网络数据包到达事件 
                OnReceive(CurSock); 
                break; 
         case FD_WRITE:      file://发送网络数据事件 
                OnSend(CurSock); 
                break; 
         default: break; 
         } 
     } 

      以下是发生在相应Socket上的各种网络异步事件的处理函数，其中OnAccept传进来的参数是服务器端创建的套接字，OnClose()、OnReceive()和OnSend()传进来的参数均是服务器端在接受客户端连接时新创建的用与此客户端通信的Socket。 
    void CSocketSeverDlg::OnAccept(SOCKET CurSock) 
    { 
         file://接受连接请求，并保存与发起连接请求的客户端进行通信Socket 
     file://为新的socket注册异步事件，注意没有Accept事件 
    } 

    void CSocketSeverDlg::OnClose(SOCET CurSock) 
    { 
        file://结束与相应的客户端的通信，释放相应资源 
    } 

    void CSocketSeverDlg::OnSend(SOCET CurSock) 
    { 
        file://在给客户端发数据时做相关预处理 
    } 

    void CSocketSeverDlg::OnReceive(SOCET CurSock) 
    { 
        file://读出网络缓冲区中的数据包 
    }         

       用同样的方法建立一个客户端应用程序。初始化网络部分，不需要将套接字设置为监听模式。注册异步事件时，没有FD_ACCEPT，但增加了FD_CONNECT事件，因此没有OnAccept()函数，但增加了OnConnect()函数。向服务器发出连接请求时，使用connect()函数，连接成功后，会响应到OnConnect()函数中。下面是OnConnect()函数的定义，传进来的参数是客户端Socket和服务器端发回来的连接是否成功的标志。 
void CSocketClntDlg::OnConnect(SOCKET CurSock, int error) 
{ 
    if(0 = = error) 
    { 
    if(CurSock = = ClntSock) 
     MessageBox("连接服务器成功!"); 
   } 
} 
    定义OnReceive()函数，处理网络数据到达事件; 
    定义OnSend()函数，处理发送网络数据事件; 
    定义OnClose()函数，处理服务器的关闭事件。 

       以上就是用基于Windows消息机制的异步I/O模型做服务器和客户端应用程序的基本方法。另外还可以用事件模型、重叠模型或完成端口模型，读者可以参考有关书籍。 
       在实现了上面的例子后，你将对Winsock编网络通信程序的机制有了一定的了解。接下来你可以进行更精彩的编程, 不仅可以在网上传输普通数据，而且还以传输语音、视频数据，你还可以自己做一个网络资源共享的服务器软件，和你的同学在实验室的局域网里可以共同分享你的成果。            

