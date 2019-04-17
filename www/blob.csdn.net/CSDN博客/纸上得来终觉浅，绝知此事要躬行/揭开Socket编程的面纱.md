# 揭开Socket编程的面纱 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年02月01日 19:07:03[boonya](https://me.csdn.net/boonya)阅读数：1006








原文地址：http://goodcandle.cnblogs.com/archive/2005/12/10/294652.aspx

 对TCP/IP、UDP、Socket编程这些词你不会很陌生吧？随着网络技术的发展，这些词充斥着我们的耳朵。那么我想问：
1.         什么是TCP/IP、UDP？
2.         Socket在哪里呢？
3.         Socket是什么呢？
4.         你会使用它们吗？


#### **什么是TCP/IP、UDP？**

TCP/IP（Transmission Control Protocol/Internet Protocol）即传输控制协议/网间协议，是一个工业标准的协议集，它是为广域网（WANs）设计的。
UDP（User Data Protocol，用户数据报协议）是与TCP相对应的协议。它是属于TCP/IP协议族中的一种。
        这里有一张图，表明了这些协议的关系。


![](http://images.cnblogs.com/cnblogs_com/goodcandle/socket1.jpg)
                                                                        图1

       TCP/IP协议族包括运输层、网络层、链路层。现在你知道TCP/IP与UDP的关系了吧。


#### **Socket在哪里呢？**
在图1中，我们没有看到Socket的影子，那么它到底在哪里呢？还是用图来说话，一目了然。






![](http://images.cnblogs.com/cnblogs_com/goodcandle/socket2.jpg)

图2

       原来Socket在这里。


#### **Socket是什么呢？**
Socket是应用层与TCP/IP协议族通信的中间软件抽象层，它是一组接口。在设计模式中，Socket其实就是一个门面模式，它把复杂的TCP/IP协议族隐藏在Socket接口后面，对用户来说，一组简单的接口就是全部，让Socket去组织数据，以符合指定的协议。

#### **你会使用它们吗？**
前人已经给我们做了好多的事了，网络间的通信也就简单了许多，但毕竟还是有挺多工作要做的。以前听到Socket编程，觉得它是比较高深的编程知识，但是只要弄清Socket编程的工作原理，神秘的面纱也就揭开了。
       一个生活中的场景。你要打电话给一个朋友，先拨号，朋友听到电话铃声后提起电话，这时你和你的朋友就建立起了连接，就可以讲话了。等交流结束，挂断电话结束此次交谈。    生活中的场景就解释了这工作原理，也许TCP/IP协议族就是诞生于生活中，这也不一定。


![](http://images.cnblogs.com/cnblogs_com/goodcandle/socket3.jpg)

图3

       先从服务器端说起。服务器端先初始化Socket，然后与端口绑定(bind)，对端口进行监听(listen)，调用accept阻塞，等待客户端连接。在这时如果有个客户端初始化一个Socket，然后连接服务器(connect)，如果连接成功，这时客户端与服务器端的连接就建立了。客户端发送数据请求，服务器端接收请求并处理请求，然后把回应数据发送给客户端，客户端读取数据，最后关闭连接，一次交互结束。
       在这里我就举个简单的例子，我们走的是TCP协议这条路（见图2）。例子用MFC编写，运行的界面如下：




![](http://images.cnblogs.com/cnblogs_com/goodcandle/socket4.jpg)

图4




![](http://images.cnblogs.com/cnblogs_com/goodcandle/socket5.jpg)

图5

       在客户端输入服务器端的IP地址和发送的数据，然后按发送按钮，服务器端接收到数据，然后回应客户端。客户端读取回应的数据，显示在界面上。
       下面是接收数据和发送数据的函数：



```
int    Receive(SOCKET fd,char *szText,int len)

{
       int cnt;
       int rc;
       cnt=len;

       while(cnt>0)
       {
              rc=recv(fd,szText,cnt,0);
              if(rc==SOCKET_ERROR)
              {
                     return -1;
             }

             if(rc==0)

                     return len-cnt;

              szText+=rc;

              cnt-=rc;

       }

       return len;

}

int Send(SOCKET fd,char *szText,int len)
{

       int cnt;

       int rc;

       cnt=len;

       while(cnt>0)

       {

              rc=send(fd,szText,cnt,0);

              if(rc==SOCKET_ERROR)

              {

                     return -1;

              }

              if(rc==0)

                     return len-cnt;

              szText+=rc;

              cnt-=rc;

       }

       return len;

}
```

服务器端：
       在服务器端，主要是启动Socket和监听线程。



```
#define DEFAULT_PORT      2000

void CServerDlg::OnStart()

{

       sockaddr_in local;

       DWORD dwThreadID = 0;

      

       local.sin_family=AF_INET;

       //设置的端口为DEFAULT_PORT。

       local.sin_port=htons(DEFAULT_PORT);

       //IP地址设置成INADDR_ANY,让系统自动获取本机的IP地址。

       local.sin_addr.S_un.S_addr=INADDR_ANY;

 

       //初始化Socket

       m_Listening = socket(AF_INET,SOCK_STREAM,0);

       if(m_Listening == INVALID_SOCKET)

       {

              return ;

       }

       //将本地地址绑定到所创建的套接字上

       if(bind(m_Listening,(LPSOCKADDR)&local,sizeof(local)) == SOCKET_ERROR )

       {

              closesocket(m_Listening);

              return ;

       }

       //创建监听线程，这样也能响应界面上操作。

       m_hListenThread = ::CreateThread(NULL,0,ListenThread,this,0,&dwThreadID);

       m_StartBtn.EnableWindow(FALSE);

       m_StopBtn.EnableWindow(TRUE);

}
```

监听线程函数：


```
DWORD WINAPI CServerDlg::ListenThread(LPVOID lpparam)
{

       CServerDlg* pDlg = (CServerDlg*)lpparam;

       if(pDlg == NULL)

              return 0;

 

       SOCKET  Listening = pDlg->m_Listening;

       //开始监听是否有客户端连接。

       if(listen(Listening,40) == SOCKET_ERROR)

       {

              return 0;

       }

       char szBuf[MAX_PATH];

       //初始化

       memset(szBuf,0,MAX_PATH);

       while(1)

       {

              SOCKET ConnectSocket;

              sockaddr_in    ClientAddr;

              int                  nLen = sizeof(sockaddr);

              //阻塞直到有客户端连接，不然多浪费CPU资源。

              ConnectSocket = accept(Listening,(sockaddr*)&ClientAddr,&nLen);

              //都到客户端的IP地址。

              char *pAddrname = inet_ntoa(ClientAddr.sin_addr);

              pDlg->Receive(ConnectSocket,szBuf,100);

              //界面上显示请求数据。

              pDlg->SetRequestText(szBuf);

              strcat(szBuf," :我是老猫，收到（");

              strcat(szBuf,pAddrname);

              strcat(szBuf,"）");

              //向客户端发送回应数据

              pDlg->Send(ConnectSocket,szBuf,100);

       }

       return 0;

}
```

       服务器端一直在监听是否有客户端连接，如有连接，处理客户端的请求，给出回应，然后继续监听。

**客户端：**

       客户端的发送函数：



```
#define DEFAULT_PORT      2000

void CClientDlg::OnSend()

{

       DWORD dwIP = 0;      

       TCHAR szText[MAX_PATH];

       memset(szText,0,MAX_PATH);

       m_IP.GetWindowText(szText,MAX_PATH);

       //把字符串形式的IP地址转成IN_ADDR结构需要的形式。

       dwIP = inet_addr(szText);

       m_RequestEdit.GetWindowText(szText,MAX_PATH);

 

       sockaddr_in local;

       SOCKET socketTmp;

       //必须是AF_INET,表示该socket在Internet域中进行通信

       local.sin_family=AF_INET;

       //端口号

       local.sin_port=htons(DEFAULT_PORT);

       //服务器的IP地址。

       local.sin_addr.S_un.S_addr=dwIP;

      

       ////初始化Socket

       socketTmp=socket(AF_INET,SOCK_STREAM,0);

       //连接服务器

       if(connect(socketTmp,(LPSOCKADDR)&local,sizeof(local)) < 0)

       {

              closesocket(socketTmp);

              MessageBox("连接服务器失败。");

              return ;

       }

       //发送请求，为简单只发100字节，在服务器端也规定100字节。

       Send(socketTmp,szText,100);

       //读取服务器端返回的数据。

       memset(szText,0,MAX_PATH);

       //接收服务器端的回应。

       Receive(socketTmp,szText,100);

 

       TCHAR szMessage[MAX_PATH];

       memset(szMessage,0,MAX_PATH);

       strcat(szMessage,szText);

       //界面上显示回应数据。

       m_ReplyBtn.SetWindowText(szMessage);

       closesocket(socketTmp);

}
```


       客户端就一个函数完成了一次通信。在这里IP地址为何用127.0.0.1呢？使用这个IP地址，服务器端和客户端就能运行在同一台机器上，这样调试方便多了。当然你可以在你朋友的机器上运行Server程序(本人在局域网中测试过)，在自己的机器上运行Client程序，当然输入的IP地址就该是你朋友机器的IP地址了。

       简单的理论和实践都说了，现在Socket编程不神秘了吧？希望对你有些帮助。

注： 主要是看看流程图，实际代码不是问题，何必纠结于语言。








