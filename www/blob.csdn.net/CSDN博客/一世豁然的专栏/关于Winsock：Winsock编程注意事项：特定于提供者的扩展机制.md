# 关于Winsock：Winsock编程注意事项：特定于提供者的扩展机制 - 一世豁然的专栏 - CSDN博客





2018年10月11日 10:13:30[一世豁然](https://me.csdn.net/Explorer_day)阅读数：32标签：[Windows Sockets2](https://so.csdn.net/so/search/s.do?q=Windows Sockets2&t=blog)
个人分类：[Windows](https://blog.csdn.net/Explorer_day/article/category/6772518)









WSAIoctl功能使服务提供商能够提供特定于提供商的功能扩展。 当然，这种机制假定应用程序知道特定扩展并理解所涉及的语义和语法。 这些信息通常由服务提供商供应商提供。

要调用扩展函数，应用程序必须首先请求指向所需函数的指针。 这是通过使用SIO_GET_EXTENSION_FUNCTION_POINTER命令代码的WSAIoctl函数完成的。 WSAIoctl的输入缓冲区包含所需扩展函数的标识符，而输出缓冲区包含函数指针本身。 然后，应用程序可以直接调用扩展函数，而无需通过Ws2_32.dll。

分配给扩展功能的标识符是由服务提供商供应商分配的全局唯一标识符（GUID）。 敦促创建扩展函数的供应商发布有关函数的完整详细信息，包括函数原型的语法。 这使得可以由多个服务提供商供应商提供通用和流行的扩展功能。 应用程序可以获取函数指针并使用该函数，而无需了解实现该函数的特定服务提供者的任何信息。

在Windows Vista及更高版本中，新的Winsock系统扩展直接从Winsock DLL导出，因此不需要WSAIoctl函数来加载这些扩展。 Windows Vista及更高版本中可用的新扩展功能包括从Ws2_32.dll导出的WSAPoll和WSASendMsg函数。



