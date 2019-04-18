# 关于Winsock：Winsock编程注意事项 - 一世豁然的专栏 - CSDN博客





2018年10月10日 16:15:17[一世豁然](https://me.csdn.net/Explorer_day)阅读数：39标签：[Windows Sockets2](https://so.csdn.net/so/search/s.do?q=Windows Sockets2&t=blog)
个人分类：[Windows](https://blog.csdn.net/Explorer_day/article/category/6772518)









Windows套接字2在许多方面扩展了Windows套接字1.1的功能。 下表总结了一些主要功能更改。
|特性|描述|
|----|----|
|[Windows Sockets 2 Architecture](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/windows-sockets-2-architecture-2)|Windows Sockets 2体系结构的说明。|
|[Socket handles](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/socket-handles-2)|套接字句柄可以选择是Windows套接字2中的文件句柄。可以使用带有标准Windows文件I / O函数的套接字句柄。|
|[Simultaneous access to multiple transport protocols](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/simultaneous-access-to-multiple-transport-protocols-2)|允许应用程序使用熟悉的套接字接口来同时访问许多已安装的传输协议。|
|[Protocol-independent name resolution](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/protocol-independent-name-resolution-2)|包括一组标准化的功能，用于查询和使用当前存在的无数名称解析域（例如DNS，SAP和X.500）。|
|[Protocol-independent multicast and multipoint](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/protocol-independent-multicast-and-multipoint-2)|应用程序发现传输提供的多点或多播功能的类型，并以通用方式使用这些功能。|
|[Overlapped I/O](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/overlapped-i-o-and-event-objects-2)|根据Windows环境中建立的模型，将套接字I / O的重叠范例合并在一起。|
|[Scatter/gather I/O](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/scatter-gather-i-o-2)|根据Windows环境中建立的模型，将分散/收集功能与套接字I / O的重叠范例相结合。|
|[Quality of Service](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/flow-specification-quality-of-service-2) (QoS)|建立应用程序用于协商带宽和延迟等参数所需服务级别的约定。 其他与QoS相关的增强功能包括针对特定于网络的服务质量扩展的机制。|
|[Provider-Specific Extension Mechanism](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/provider-specific-extension-mechanism-2)|WSAIoctl功能使服务提供商能够提供特定于提供商的功能扩展。|
|[Shared Sockets](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/shared-sockets-2)|引入了WSADuplicateSocket函数以启用跨进程的套接字共享。|
|[Connection Setup and Teardown](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/connection-setup-and-teardown-2)|在决定是否接受传入连接请求之前，应用程序可以获取呼叫者信息，例如呼叫者标识符和服务质量。 也可以（对于支持此协议的协议）在连接拆除时在端点之间交换用户数据。|
|[Graceful Shutdown, Linger Options, and Socket Closure](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/graceful-shutdown-linger-options-and-socket-closure-2)|应用程序有几个关闭套接字连接的选项（关闭序列）。|
|[Protocol-Independent Out-of-Band Data](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/protocol-independent-out-of-band-data-2)|流套接字抽象包括带外（OOB）数据的概念。|
|[Debug and Trace Facilities](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/debug-and-trace-facilities-2)|Windows套接字2支持特别设计的Ws2_32.dll版本和单独的调试/跟踪DLL。|
|[Windows Sockets Compatibility Issues](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/windows-sockets-compatibility-issues-2)|Windows套接字2继续支持所有Windows套接字1.1语义和函数调用，除了处理伪阻塞的那些。|
|[Handling Winsock Errors](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/handling-winsock-errors)|如何通过应用程序检索和处理Winsock错误。|



