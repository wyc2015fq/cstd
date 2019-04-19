# WinSock TCP,UDP使用总结 - fanyun的博客 - CSDN博客
2017年06月30日 19:57:39[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：688
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
Windows下网络编程的规范-Windows Sockets是Windows下得到广泛应用的、开放的、支持多种协议的网络编程接口。
       TCP（Transmission Control Protocol,传输控制协议）是基于连接的协议，在正式收发数据前，必须和对方建立可靠的连接。一个TCP连接必须要经过三次“对话”才能建立起来。
　　UDP（User Data Protocol,用户数据报协议）是与TCP相对应的协议。它是面向非连接的协议，它不与对方建立连接，而是直接就把数据包发送过去。
从上述描述我们可以看出TCP是面向连接的可靠的协议，而UDP是面向非连接的不可靠的协议，两者从传送速度上相比较，UDP的传输速度较TCP更高，更加符合网络视频监控系统对于数据传输实时性的要求。
         WinSock提供了对UDP（用户数据报协议）的支持，通过UDP协议我们可以向指定IP地址的主机发送数据，同时也可以从指定IP地址的主机接收数据，发送和接收方处于相同的地位没有主次之分。利用CSocket操纵无连接的数据发送很简单，首先生成一个本地套接口（需要指明SOCK_DGRAM标记），然后利用
intCAsyncSocket::SendTo( const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTRlpszHostAddress = NULL, int nFlags = 0 )发送数据，
intCAsyncSocket::ReceiveFrom( void* lpBuf, int nBufLen, CString&rSocketAddress, UINT& rSocketPort, int nFlags = 0 )接收数据。函数调用顺序如图。
利用UDP协议发送和接收都可以是双向的，就是说任何一个主机都可以发送和接收数据。但是UDP协议是无连接的，所以发送的数据不一定能被接收，此外接收的顺序也有可能与发送顺序不一致。下面是相关代码：
/*
发送方在端口6800上向接收方端口6801发送数据
*/
//发送方代码：
```cpp
BOOLCMy62_s1_clientDlg::OnInitDialog()
{
         CDialog::OnInitDialog();
 
         //创建本地套接口
         m_sockSend.Create(6800,SOCK_DGRAM,NULL);
         //绑定本地套接口
         m_sockSend.Bind(6800,"127.0.0.1");
         //创建一个定时器定时发送
         SetTimer(1,3000,NULL);
...
}
voidCMy62_s1_clientDlg::OnTimer(UINT nIDEvent)
{
         static iIndex=0;
         char szSend[20];
         sprintf(szSend,"%010d",iIndex++);
         //发送UDP数据
         int iSend=m_sockSend.SendTo(szSend,10,6801,"127.0.0.1",0);
         TRACE("sent %dbyte\n",iSend);
...
}
```
//接收方代码
```cpp
BOOLCMy62_s1_serverDlg::OnInitDialog()
{
         CDialog::OnInitDialog();
 
         //创建本地套接口
         m_sockRecv.Create(6801,SOCK_DGRAM,"127.0.0.1");
         //绑定本地套接口
         m_sockRecv.Bind(6801,"127.0.0.1");
         //创建一个定时器定时读取
         SetTimer(1,3000,NULL);
...
}
voidCMy62_s1_serverDlg::OnTimer(UINT nIDEvent)
{
         char szRecv[20];
         CString szIP("127.0.0.1");
         UINT uPort=6800;
         //接收UDP数据
         int iRecv=m_sockRecv.ReceiveFrom(szRecv,10,szIP,uPort,0);
         TRACE("received %dbyte\n",iRecv);
...
}
/*
```
接收方采用同步读取数据的方式，所以没有读到数据函数调用将不会返回
