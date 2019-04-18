# Winsocket编程之TCP/IP体系结构 - 深之JohnChen的专栏 - CSDN博客

2005年12月11日 19:17:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1689


**一、TCP/IP 体系结构与特点**
　　1、TCP/IP体系结构

　　TCP/IP协议实际上就是在物理网上的一组完整的网络协议。其中TCP是提供传输层服务，而IP则是提供网络层服务。TCP/IP包括以下协议：（结构如图1.1）

![](http://www.yesky.com/image20010518/54678.gif)
(图1.1) 

　　IP： 网间协议(Internet Protocol) 负责主机间数据的路由和网络上数据的存储。同时为ICMP，TCP，　　　UDP提供分组发送服务。用户进程通常不需要涉及这一层。

　　ARP： 地址解析协议(Address Resolution Protocol)
　　　此协议将网络地址映射到硬件地址。

　　RARP： 反向地址解析协议(Reverse Address Resolution Protocol)
　　　此协议将硬件地址映射到网络地址

　　ICMP： 网间报文控制协议(Internet Control Message Protocol)
　　　此协议处理信关和主机的差错和传送控制。

　　TCP： 传送控制协议(Transmission Control Protocol)
　　　这是一种提供给用户进程的可靠的全双工字节流面向连接的协议。它要为用户进程提供虚电路服务，并为数据可靠传输建立检查。（注：大多数网络用户程序使用TCP）

　　UDP： 用户数据报协议(User Datagram Protocol)
　　　这是提供给用户进程的无连接协议，用于传送数据而不执行正确性检查。

　　FTP： 文件传输协议(File Transfer Protocol)
　　　允许用户以文件操作的方式（文件的增、删、改、查、传送等）与另一主机相互通信。

　　SMTP： 简单邮件传送协议(Simple Mail Transfer Protocol)
　　　SMTP协议为系统之间传送电子邮件。

　　TELNET：终端协议(Telnet Terminal Procotol)
　　　允许用户以虚终端方式访问远程主机

　　HTTP： 超文本传输协议(Hypertext Transfer Procotol)

　　TFTP: 简单文件传输协议(Trivial File Transfer Protocol)

　　2、TCP/IP特点

　　TCP/IP协议的核心部分是传输层协议(TCP、UDP)，网络层协议(IP)和物理接口层，这三层通常是在操作系统内核中实现。因此用户一般不涉及。编程时，编程界面有两种形式：一、是由内核心直接提供的系统调用；二、使用以库函数方式提供的各种函数。前者为核内实现，后者为核外实现。用户服务要通过核外的应用程序才能实现，所以要使用套接字(socket)来实现。

　　图1.2是TCP/IP协议核心与应用程序关系图。

![](http://www.yesky.com/image20010518/54679.gif)
(图1.2)

**二、专用术语**

　　1、套接字

　　套接字是网络的基本构件。它是可以被命名和寻址的通信端点，使用中的每一个套接字都有其类型和一个与之相连听进程。套接字存在通信区域（通信区域又称地址簇）中。套接字只与同一区域中的套接字交换数据（跨区域时，需要执行某和转换进程才能实现）。WINDOWS 中的套接字只支持一个域——网际域。套接字具有类型。

　　WINDOWS SOCKET 1.1 版本支持两种套接字：流套接字(SOCK_STREAM)和数据报套接字(SOCK_DGRAM) 

　　2、WINDOWS SOCKETS 实现

　　一个WINDOWS SOCKETS 实现是指实现了WINDOWS SOCKETS规范所描述的全部功能的一套软件。一般通过DLL文件来实现

　　3、阻塞处理例程

　　阻塞处理例程(blocking hook,阻塞钩子)是WINDOWS SOCKETS实现为了支持阻塞套接字函数调用而提供的一种机制。

　　4、多址广播（multicast，多点传送或组播）

　　是一种一对多的传输方式，传输发起者通过一次传输就将信息传送到一组接收者，与单点传送
(unicast)和广播(Broadcast)相对应。

