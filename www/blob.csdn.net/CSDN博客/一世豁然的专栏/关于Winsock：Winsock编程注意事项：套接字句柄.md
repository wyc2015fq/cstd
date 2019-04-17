# 关于Winsock：Winsock编程注意事项：套接字句柄 - 一世豁然的专栏 - CSDN博客





2018年10月10日 16:23:57[一世豁然](https://me.csdn.net/Explorer_day)阅读数：101








套接字句柄可以选择是Windows套接字2中的文件句柄.Winsock提供程序的套接字句柄可以与其他非Winsock函数一起使用，例如ReadFile，WriteFile，ReadFileEx和WriteFileEx。

提供程序的协议信息结构中的XP1_IFS_HANDLES成员确定来自提供程序的套接字句柄是否是可安装文件系统（IFS）句柄。 可以使用作为IFS句柄的套接字句柄，而不会对其他非Winsock函数（例如，ReadFile和WriteFile）造成性能损失。 当与非Winsock函数（例如，ReadFile和WriteFile）一起使用时，任何非IFS套接字句柄都会导致提供程序与文件系统之间的交互，其中涉及额外的处理开销，这可能导致显着的性能损失。 使用具有非Winsock函数的套接字句柄时，从基本文件系统传播的错误代码并不总是映射到Winsock错误代码。 因此，建议套接字句柄仅用于Winsock函数。

套接字句柄不应与DuplicateHandle函数一起使用。 分层服务提供程序（LSP）的存在可能导致此操作失败，并且目标进程无法导入套接字句柄。

> 
注意

分层服务提供商已弃用。 从Windows 8和Windows Server 2012开始，使用Windows筛选平台。


Windows套接字2扩展了某些使用句柄在套接字之间传输数据的功能。 这些函数提供了特定于用于传输数据的套接字的优点，包括WSARecv，WSASend和WSADuplicateSocket。



