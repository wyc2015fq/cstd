# Windows Sockets 2 - 一世豁然的专栏 - CSDN博客





2018年10月10日 15:22:36[一世豁然](https://me.csdn.net/Explorer_day)阅读数：54








一、目的

Windows Sockets 2（Winsock）使程序员能够创建高级Internet，Intranet和其他支持网络的应用程序，以便通过线路传输应用程序数据，而与所使用的网络协议无关。 借助Winsock，程序员可以访问高级Microsoft®Windows®网络功能，例如多播和服务质量（QoS）。

Winsock遵循Windows开放系统架构（WOSA）模型; 它定义了应用程序编程接口（API）与其导出函数和协议栈之间的标准服务提供者接口（SPI）。 它使用了Berkeley Software Distribution（BSD）UNIX首次推广的套接字范例。 它后来适用于Windows Sockets 1.1中的Windows，Windows Sockets 2应用程序向后兼容。 Winsock编程以前以TCP / IP为中心。 一些使用TCP / IP的编程实践不适用于每个协议。 因此，Windows Sockets 2 API会在必要时添加处理多个协议的功能。





二、开发者要求

Windows套接字2旨在供C / C ++程序员使用。 需要熟悉Windows网络。





三、运行时要求

Windows套接字2可用于所有Windows平台。 在确实存在Windows Sockets 2平台限制的某些实现或功能的情况下，文档中会明确指出这些限制。





四、在这个部分
|话题|描述|
|----|----|
|[Windows Sockets](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/what-s-new-for-windows-sockets-2)新特性|有关Windows套接字的新功能的信息。|
|[Windows中的Winsock网络协议支持](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/network-protocol-support-in-windows)|有关不同Windows版本上Windows套接字的网络协议支持的信息。|
|[关于 Winsock](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/about-winsock)|有关Windows套接字编程注意事项，体系结构和开发人员可用功能的一般信息。|
|[使用 Winsock](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/using-winsock)|Windows套接字使用的过程和编程技术。 本节包括基本的Winsock编程技术，例如Winsock入门，以及对经验丰富的Winsock开发人员有用的高级技术。|
|[Winsock](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/winsock-reference)参考|Windows套接字API的文档。|





