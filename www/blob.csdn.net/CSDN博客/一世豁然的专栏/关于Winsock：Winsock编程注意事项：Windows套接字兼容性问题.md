# 关于Winsock：Winsock编程注意事项：Windows套接字兼容性问题 - 一世豁然的专栏 - CSDN博客





2018年10月11日 10:59:32[一世豁然](https://me.csdn.net/Explorer_day)阅读数：49








Windows套接字2继续支持所有Windows套接字1.1语义和函数调用，除了处理伪阻塞的那些。 由于Windows套接字2仅在32位预抢占调度环境中运行，因此无需实现Windows套接字1.1中的伪阻塞。 这意味着永远不会指示WSAEINPROGRESS错误代码，并且以下Windows套接字1.1函数不适用于Windows套接字2应用程序：
- WSACancelBlockingCall
- WSAIsBlocking
- WSASetBlockingHook
- WSAUnhookBlockingHook

为利用伪阻塞而编写的Windows Sockets 1.1程序将继续正常运行，因为它们链接到Winsock.dll或Wsock32.dll。 两者都继续支持整套Windows Sockets 1.1功能。 为了使程序成为Windows Sockets 2应用程序，必须进行一些代码修改。 在大多数情况下，可以替换线程的明智使用以适应使用阻塞钩子函数完成的处理。



**套接字重叠属性的默认状态**

套接字函数创建了套接字，默认情况下在第一个Wsock32.dll（32位版本的Windows套接字1.1）中设置了重叠属性。 为了确保与当前部署的Wsock32.dll实现的向后兼容性，Windows Sockets 2也将继续如此。 也就是说，在Windows套接字2中，使用套接字函数创建的套接字将具有重叠属性。 但是，为了与Windows API的其余部分更兼容，使用WSASocket创建的套接字不会默认具有重叠属性。 仅当设置了WSA_FLAG_OVERLAPPED位时，才会应用此属性。



**Windows套接字1.1阻止例程和EINPROGRESS**

将应用程序从Berkeley套接字环境移植到Windows环境的一个主要问题涉及阻塞; 也就是说，调用一个在关联操作完成之前不会返回的函数。 当操作花费任意长的时间来完成时出现问题：示例是recv函数，其可能阻塞直到从对等系统接收到数据。 Berkeley套接字模型中的默认行为是套接字以阻塞模式运行，除非程序员明确请求将操作视为非阻塞。 Windows套接字1.1环境无法假定抢占式调度。 因此，强烈建议程序员使用非阻塞（异步）操作，如果可能的话，使用Windows Sockets 1.1。 因为这并非总是可行，所以提供了下面描述的伪阻塞设施。

> 
注意

Windows套接字2仅在抢占式32位操作系统上运行，其中死锁不是问题。 Windows套接字2中不需要为Windows套接字1.1推荐的编程实践。


即使在阻塞套接字上，一些函数 - 例如bind，getsockopt和getpeername - 立即完成。 这些功能的阻塞和非阻塞操作之间没有区别。 其他操作（例如recv）可以立即完成或者需要任意时间来完成，具体取决于各种运输条件。 当应用于阻塞套接字时，这些操作称为阻塞操作。 以下功能可以阻止：
- [**recv**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-recv)
- [**recvfrom**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-recvfrom)
- [**send**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-send)
- [**sendto**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-sendto)

对于16位Windows套接字1.1，无法立即完成的阻塞操作由伪阻塞处理，如下所示。

服务提供程序启动操作，然后进入一个循环，在该循环中调度任何Windows消息（如果需要，将处理器交给另一个线程），然后检查Windows套接字函数是否完成。 如果函数已完成，或者已调用WSACancelBlockingCall，则阻塞函数将以适当的结果完成。

服务提供者必须允许安装不处理消息的阻塞钩子函数，以避免在阻塞操作未完成时可能出现重入消息。 最简单的阻塞钩子函数将返回FALSE。 如果Windows套接字DLL依赖于内部操作的消息，它可以在执行应用程序阻塞挂钩之前执行PeekMessage（hMyWnd ...），以便它可以在不影响系统其余部分的情况下获取其消息。

在16位Windows套接字1.1环境中，如果收到阻止操作正在进行的进程的Windows消息，则应用程序可能会尝试发出另一个Windows套接字调用。 由于难以安全地管理此条件，Windows套接字1.1不支持此类应用程序行为。 不允许应用程序进行多个嵌套的Windows套接字函数调用。 对于特定任务，只允许一个未完成的函数调用。 唯一的例外是在这种情况下为程序员提供的两个函数：WSAIsBlocking和WSACancelBlockingCall。

可以随时调用WSAIsBlocking函数来确定是否正在阻止Windows Sockets 1.1调用。 同样，可以随时调用WSACancelBlockingCall函数来取消正在进行的阻塞调用。 Windows套接字函数的任何其他嵌套都会失败，并显示错误WSAEINPROGRESS。

应该强调的是，此限制适用于阻塞和非阻塞操作。 对于在调用WSAStartup时协商版本2.0或更高版本的Windows套接字2应用程序，不会限制操作的嵌套。 操作可以在极少数情况下嵌套，例如在WSAAccept条件接受回调期间，或者服务提供者依次调用Windows套接字2函数。


尽管此机制对于简单应用程序已足够，但它无法支持更高级应用程序（例如，使用MDI模型的应用程序）的复杂消息调度要求。 对于此类应用程序，Windows套接字API包含函数WSASetBlockingHook，它允许应用程序指定可以调用的特殊例程，而不是前面讨论中描述的默认消息调度例程。

仅当满足以下所有条件时，Windows套接字提供程序才会调用阻塞挂钩：
- 例程是定义为能够阻止的例程。
- 指定的套接字是阻塞套接字。
- 请求无法立即完成。

默认情况下，套接字设置为阻塞，但具有FIONBIO IOCTL或WSAAsyncSelect函数的ioctlsocket函数可以将套接字设置为非阻塞模式。

永远不会调用阻塞钩子，如果应用程序遵循这些准则，应用程序不需要关心阻塞钩子可能引入的重入问题：
- 它仅使用非阻塞套接字。
- 它使用WSAAsyncSelect和/或WSAAsyncGetXByY例程而不是select和getXbyY例程。

如果Windows套接字1.1应用程序调用异步或非阻塞操作，该操作将指向内存对象（例如缓冲区或全局变量）的指针作为参数，则应用程序有责任确保该对象可用于 整个操作过程中的Windows套接字。 应用程序不得调用任何可能影响所涉及内存的映射或地址可行性的Windows函数。





