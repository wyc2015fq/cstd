# 关于Winsock：Winsock编程注意事项：处理Winsock错误 - 一世豁然的专栏 - CSDN博客





2018年10月11日 11:00:59[一世豁然](https://me.csdn.net/Explorer_day)阅读数：113








函数返回时，大多数Windows Sockets 2函数都不会返回错误的具体原因。 如果成功，某些Winsock函数返回零值。 否则，返回值SOCKET_ERROR（-1），并通过调用WSAGetLastError函数来检索特定的错误号。 对于返回句柄的Winsock函数，返回值INVALID_SOCKET（0xffff）表示错误，并且可以通过调用WSAGetLastError来检索特定错误号。 对于返回指针的Winsock函数，返回值NULL表示错误，并且可以通过调用WSAGetLastError函数来检索特定的错误号。

可以使用HRESULT_FROM_WIN32将Winsock错误代码转换为HRESULT，以便在远程过程调用（RPC）中使用。 在早期版本的平台软件开发工具包（SDK）中，HRESULT_FROM_WIN32被定义为Winerror.h头文件中的宏。 在Microsoft Windows软件开发工具包（SDK）中，HRESULT_FROM_WIN32被定义为Winerror.h头文件中的内联函数。



