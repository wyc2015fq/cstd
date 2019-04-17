# Winsock参考：Winsock追踪事件 - 一世豁然的专栏 - CSDN博客





2018年10月16日 11:12:17[一世豁然](https://me.csdn.net/Explorer_day)阅读数：109








本节介绍有关特定Winsock跟踪事件详细信息的详细信息。

Winsock跟踪是一种故障排除功能，可以在零售二进制文件中启用，以最小的开销跟踪某些Windows套接字事件。 此功能为开发人员和产品支持提供了更好的诊断功能。 Winsock网络事件跟踪支持跟踪IPv4和IPv6应用程序的套接字操作。 Winsock目录更改跟踪支持跟踪分层服务提供程序（LSP）对Winsock目录所做的更改。

> 
注意

分层服务提供商已弃用。 从Windows 8和Windows Server 2012开始，使用Windows筛选平台。


Winsock跟踪使用Windows事件跟踪（ETW），这是一种由操作系统提供的通用高速跟踪工具。 ETW为用户模式应用程序和内核模式设备驱动程序引发的事件提供跟踪机制。 ETW可以动态启用和禁用日志记录，从而可以轻松地在生产环境中执行详细跟踪，而无需重新启动或重新启动应用程序。 在Windows Vista及更高版本上添加了使用ETW支持Winsock跟踪。 有关ETW的一般信息，请参阅使用ETW改进调试和性能调整。

以下列表提供了每个Winsock跟踪事件的详细信息。 有关任何事件的其他信息，请单击事件名称。
|事件名|描述|
|----|----|
|[**AFD_EVENT_CREATE**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/afd-event-create)|用于套接字创建操作的Winsock网络跟踪事件。|
|[**AFD_EVENT_CLOSE**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/afd-event-close)|用于套接字关闭操作的Winsock网络跟踪事件。|
|[**WINSOCK_WS2HELP_LSP_INSTALL**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/winsock-ws2help-lsp-install)|用于分层服务提供程序（LSP）安装操作的Winsock目录更改事件。|
|[**WINSOCK_WS2HELP_LSP_REMOVE**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/winsock-ws2help-lsp-remove)|用于分层服务提供程序（LSP）删除操作的Winsock目录更改事件。|
|[**WINSOCK_WS2HELP_LSP_DISABLE**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/winsock-ws2help-lsp-disable)|Winsock目录更改事件，用于分层服务提供程序（LSP）禁用操作。|
|[**WINSOCK_WS2HELP_LSP_RESET**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/winsock-ws2help-lsp-reset)|Winsock目录重置操作的Winsock目录更改事件。|



