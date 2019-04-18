# TCP/IP协议详解卷1学习笔记_TCP协议  - 深之JohnChen的专栏 - CSDN博客

2006年04月13日 10:12:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2945标签：[tcp																[服务器																[socket																[网络																[互联网](https://so.csdn.net/so/search/s.do?q=互联网&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)
个人分类：[网络编程](https://blog.csdn.net/byxdaz/article/category/166215)


TCP协议　

==> TCP首部

    源端口号、目的端口号、位序号、位确认序号、首部长度、标志位、窗口大小、检验和、紧急指针和其它选项。 

    一个IP地址和一个端口号也成为一个插口(socket)。插口对可唯一确定互联网中每个TCP连接的双方。

==> TCP连接的建立与终止

    TCP是一个面向连接的协议，无论哪方向另一方发送数据之前，都必须先在双方之间建立一条连接。
    TCP连接的建立——三次握手。
    TCP连接的终止——四次握手。这是由TCP的半关闭造成的。因为TCP是全双工的，因此每个方向必须单独的进行关闭。

==> 最大报文段长度MSS

    MSS越大，允许每个报文段传递的数据越多，相对TCP和IP的首部有更高的利用率。

    有些情况下，MSS是可以在建立TCP连接时进行协商的选项，但是有些情况下不行 。

    * 如果是本地网络，TCP可以根据网络外出接口处的MTU值减去固定的IP首部(20)和TCP长度(20)，对于以太网，可以达到1460。

    * 如果IP地址为非本地的，则MSS通常定为默认值536字节（允许20字节的IP首部和20字节的TCP首部以适合576字节的IP数据报）。

    MSS让主机限制另一端发送数据的长度，同时也能控制它自己发送数据报的长度，避免较小MTU发生分片。

==> TCP的半关闭

    TCP连接的一端在结束它的发送后还能接收来自另一端数据（直到它也发送FIN）的能力，这就是所谓的半关闭。应用程序很少用到。

==> 复位报文段

    * 不存在的端口（目的端口没有进程监听）。目的主机将对SYN请求返回一个RST报文段。（UDP则将产生一个端口不可达的信息）

    * 异常终止。

    * 检测半打开的连接。

==> TCP服务器的设计

    * 大多数TCP服务器的进程是并发的.

    * 只有处于监听的进程才能处理客户端的连接请求.

    * TCP服务器可以对本地IP地址进行限制,但是一般不能对远程IP地址进行限制.


