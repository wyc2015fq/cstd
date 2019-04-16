# BREW SDK 九大功能之网络服务 - 我相信...... - CSDN博客





2010年10月08日 11:36:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1001








BREW通过ISocket和INetMgr可以实现主要的网络功能。从网络整体来看，Socket是不同主机上应用程序之间的一个虚拟的接口，具有跨平台特性。从程序员角度来看，Socket是应用程序和网络设备的一个接口，特殊的I/O。socket有3种类型：面向连接的字节流套接口(Stream Sockets)，无连接的数据报套接口(Datagram Sockets)和只对Internet协议有效，用于直接访问IP协议原始套接口(Raw Sockets)。socket接口并不仅限于TCP/IP，但由于TCP/IP的广泛使用，它们几乎被等同起来了。

网络服务中的一个重要概念是IP地址/域名，BREW可用gethostbyaddr和getaddrbyhost函数相互转换。另一个重要概念是端口号，TCP和UDP采用16bit的端口号来识别应用程序。服务器一般都是通过周知端口号来识别的，如FTP服务器的TCP端口号是21，TFTP (简单文件传送协议)服务器的UDP端口号是69，等等。端口号用以区别不同的应用，RFC1700规定了由IANA（因特网已分配数值权威机构）定义的端口列表，端口号分三段：周知端口：0～1023，由IANA分配和控制，对普通用户不可用；经注册的端口：1024～49151，不受IANA控制，但由IANA登记并提供它们的使用情况清单；动态或私用端口：49152～65535，IANA不管的端口，也称临时端口。理论上来说，编程能使用的端口值为1024～65535，通用端口的使用需要超级用户(root)权限。

BREW的ISocket派生于标准BSD socket，采用非阻塞异步通信模式，基于回调函数的通知方式，支持TCP和UDP（BREW2.X手机至少同时支持3个TCP，4个UDP）。INetMgr 支持IPV4和PPP的网络属性，管理网络行为，是所有ISocket的始作俑者，在所有操作完成以后释放。BREW 3.X及以上版本中的ISocketPort和INetwork支持了多种移动通信网络和IPV6的功能。

ISocket接口负责具体的网络连接，每个连接需要一个ISocket接口。ISocket接口不能直接创建，而应该通过INETMGR_Opensocket来创建，因此没有INetMg实例，ISocket是不能工作的。INetMgr_opensocket用于打开TCP或UDP套接字。如果成功，该函数返回指向isocket接口实例的指针。可使用该指针调用ISOCKETConnect，以指定要连接套接字的目标IP 地址和端口号。首个套接字连接到设备上后，BREW AEE将构建网络子系统。该子系统的组件包括:CDMA物理层，CDMA设备的无线链路协议(RLP)和点到点协议(PPP) 连接。连接到设备上的所有套接字将共享这些连接。通过调用ISOCKETRelease可关闭套接字。当关闭了设备上连接的最后一个套接字时，BREW AEE将在指定的延迟时间后终止网络子系统的连接。

BREW中非阻塞网络通信的好处是避免了任何用户线程上的长时间运行而导致不能及时响应系统事件，而同时会带来一点处理上的麻烦，即客户端程序可能不能在网络UDP数据包到达的第一时间就将数据从指定的端口取走，从而会造成相同端口上没有及时读取的数据包会被后续到来的数据包覆盖的问题，在设计应用的时候要给予注意。在TCP/IP协议中使用最大报文段长度（MSS，Maximum Segment Size）来表示TCP传往另一端的最大块数据长度。因而了解无线网络上的MSS值对网络应用的开发很有意义，尤其是流媒体服务。将每一帧的大小设置为TCP允许的MSS，可以保证网络的利用率，减少TCP头部的开销。

针对应用广泛的互联网服务，BREW提供了IWeb等一系列接口是开发者方便使用互联网，同时提供了WEB 代理功能。BREW WEB 代理规范易于使用，是一组固定的以 null 结尾的字符串，类似于常见的 URL 语法。BREWWEB代理规范可以为一个处理指定多个代理规范，并按 IWEBOPTS_GetOpt() 顺序检验。 它们具有一套简单但有效的语法，可用于指定代理 URL 的包含和排除。BREW WEB 代理规范无需复制，并可将其当作 WebOpts 管理。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/04/07/5456697.aspx](http://blog.csdn.net/wireless_com/archive/2010/04/07/5456697.aspx)



