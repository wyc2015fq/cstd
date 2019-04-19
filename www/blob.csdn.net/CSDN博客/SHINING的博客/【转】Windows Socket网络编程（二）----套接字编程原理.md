# 【转】Windows Socket网络编程（二）----套接字编程原理 - SHINING的博客 - CSDN博客
2010年05月07日 09:00:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：1231标签：[编程																[socket																[网络																[windows																[pascal																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=pascal&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)
个人分类：[VC++/MFC](https://blog.csdn.net/snsn1984/article/category/420804)
**Windows Socket 网络编程(二) —— 套接字编程原理**
作者: [冰点工作室](http://bingdianstudio.yeah.net/)[小鹰](mailto:eagle_21c@sina.com)
![](http://writeblog.csdn.net/image/paragraph.gif)** 一、客户机/服务器模式**
在TCP/IP网络中两个进程间的相互作用的主机模式是客户机/服务器模式(Client/Server model)。该模式的建立基于以下两点：1、非对等作用；2、通信完全是异步的。客户机/服务器模式在操作过程中采取的是主动请示方式：
首先服务器方要先启动，并根据请示提供相应服务：（过程如下）
1、打开一通信通道并告知本地主机，它愿意在某一个公认地址上接收客户请求。
2、等待客户请求到达该端口。
3、接收到重复服务请求，处理该请求并发送应答信号。
4、返回第二步，等待另一客户请求
5、关闭服务器。
客户方：
1、打开一通信通道，并连接到服务器所在主机的特定端口。
2、向服务器发送服务请求报文，等待并接收应答；继续提出请求……
3、请求结束后关闭通信通道并终止。
![](http://writeblog.csdn.net/image/paragraph.gif)** 二、基本套接字**
为了更好说明套接字编程原理，给出几个基本的套接字，在以后的篇幅中会给出更详细的使用说明。
1、创建套接字——socket()
功能：使用前创建一个新的套接字
格式：SOCKET PASCAL FAR socket(int af,int type,int procotol);
参数：af: 通信发生的区域
type: 要建立的套接字类型
procotol: 使用的特定协议
2、指定本地地址——bind()
功能：将套接字地址与所创建的套接字号联系起来。
格式：int PASCAL FAR bind(SOCKET s,const struct sockaddr FAR * name,int namelen);
参数：s: 是由socket()调用返回的并且未作连接的套接字描述符（套接字号）。
其它：没有错误，bind()返回0，否则SOCKET_ERROR
地址结构说明：
struct sockaddr_in
{
short sin_family;//AF_INET
u_short sin_port;//16位端口号，网络字节顺序
struct in_addr sin_addr;//32位IP地址，网络字节顺序
char sin_zero[8];//保留
}
3、建立套接字连接——connect()和accept()
功能：共同完成连接工作
格式：int PASCAL FAR connect(SOCKET s,const struct sockaddr FAR * name,int namelen);
SOCKET PASCAL FAR accept(SOCKET s,struct sockaddr FAR * name,int FAR * addrlen);
参数：同上
4、监听连接——listen()
功能：用于面向连接服务器，表明它愿意接收连接。
格式：int PASCAL FAR listen(SOCKET s, int backlog);
5、数据传输——send()与recv()
功能：数据的发送与接收
格式：int PASCAL FAR send(SOCKET s,const char FAR * buf,int len,int flags);
int PASCAL FAR recv(SOCKET s,const char FAR * buf,int len,int flags);
参数：buf:指向存有传输数据的缓冲区的指针。
6、多路复用——select()
功能：用来检测一个或多个套接字状态。
格式：int PASCAL FAR select(int nfds,fd_set FAR * readfds,fd_set FAR * writefds, 
fd_set FAR * exceptfds,const struct timeval FAR * timeout);
参数：readfds:指向要做读检测的指针
writefds:指向要做写检测的指针
exceptfds:指向要检测是否出错的指针
timeout:最大等待时间
7、关闭套接字——closesocket()
功能：关闭套接字s
格式：BOOL PASCAL FAR closesocket(SOCKET s);
![](http://writeblog.csdn.net/image/paragraph.gif)**三、典型过程图**
2.1 面向连接的套接字的系统调用时序图
2.2 无连接协议的套接字调用时序图
2.3 面向连接的应用程序流程图
原文地址：[http://www.vckbase.com/document/viewdoc/?id=484](http://www.vckbase.com/document/viewdoc/?id=484)
