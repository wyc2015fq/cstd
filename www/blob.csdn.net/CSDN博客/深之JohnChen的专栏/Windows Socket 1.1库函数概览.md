# Windows Socket 1.1库函数概览 - 深之JohnChen的专栏 - CSDN博客

2005年12月21日 16:35:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2426标签：[windows																[socket																[sockets																[dll																[microsoft																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=sockets&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[网络编程](https://blog.csdn.net/byxdaz/article/category/166215)


## Windows Socket 1.1库函数概览

## 1 套接口函数

Windows Sockets规范包含了以下Berkeley风格的套接口例程：

accept()*响应联结请求，并且新建一个套接口。原来的套接口则返回监听状态。

bind()把一个本地的名字和一个无名的套接口捆绑起来。

closesocket()*把套接口从拥有对象参考表中取消。该函数只有在SO_LINGER被设置时才会阻塞。

connect()*初始化到一个指定套接口上的连接。

getpeername()得到连接在指定套接口上的对等通讯方的名字。

getsockname()得到指定套接口上当前的名字。

getsockopt()得到与指定套接口相关的属性选项。

htonl()把32位的数字从主机字节顺序转换到网络字节顺序。

htons()把16位的数字从主机字节顺序转换到网络字节顺序。

inet_addr()把一个Internet标准的"."记号地址转换成Internet地址数值。

inet_ntoa()把Internet地址数值转换成带"."的ASCII字符串。

ioctlsocket()为套接口提供控制。

listen()监听某一指定套接口上连接请求的到来。

ntohl()把32位数字从网络字节顺序转换为主机字节顺序。

ntons()把16位数字从网络字节顺序转换为主机字节顺序。

recv()*从一个已连接的套接口接收数据。

recvfrom()*从一个已连接的或未连接的套接口接收数据。

select()*执行同步I/O多路复用。

send()*从一已连接的套接口发送数据。

sendto()*从已连接或未连接的套接口发送数据。

setsockopt()设置与指定套接口相关的属性选项。

shutdown()关闭一部分全双工的连接。

socket()创建一个通讯端点并返回一个套接口。

*表示例程在应用于阻塞套接口时会阻塞。

### 1.1 阻塞/非阻塞和数据易失性

阻塞是在把应用程序从Berkeley套接口环境中移植到Windows环境中的一个主要焦点。阻塞是指唤起一个函数，该函数直到相关操作完成时才返回。由于操作可能需要任意长的时间才能完成，于是问题就出现了。最明显的一个例子就是recv()，这个函数会一直处于阻塞状态直到收到对方系统发送的数据。在Berkeley套接口模型中，一个套接口的操作的缺省行为是阻塞方式的，除非程序员显式地请求该操作为非阻塞方式。我们强烈推荐程序员在尽可能的情况下使用非阻塞方式（异步方式）的操作。因为非阻塞方式的操作能够更好地在非占先的Windows环境下工作。程序员应该在绝对必要的时候才采用阻塞方式。而且在你必须使用阻塞方式的操作前仔细阅读并理解这一部分。

即使在阻塞方式下，有些操作（例如bind(),getsockopt(),getpeername()）也会立刻完成。对于这些操作，阻塞方式和非阻塞方式并没有什么两样。其他一些操作（例如recv()）可能立刻完成，也可能需要阻塞一段随机的时间才能完成。这都取决于不同的传输情况。当用于阻塞套接口时，这些操作被认为是工作于阻塞方式的，所有会阻塞的例程在以前或以后的列表中都打上了星号作标记。

在Windows Sockets实现中，一个无法立刻完成的阻塞操作是按如下方式处理的。DLL先初始化操作，然后进入一个循环，在循环中发送收到的任何信息－为了使在必要时把处理器交给其他线程，然后检查Windows Sockets功能是否完成。如果功能完成了，或者WSACancelBlockingCall()被唤起，阻塞操作以一个适当的返回值结束。完整的关于这种机制的描述，请参见5.3.13节，WSASetBlockingHook()，这一部分还包括了对于各种函数伪代码的讨论。

如果一个正在运行某一阻塞操作的进程收到了一个Windows消息，那么应用程序有可能试图发出另一个Windows Sockets调用，由于很难安全地处理这种情形，Windows Sockets规范不支持这种应用程序的工作方式。在这种情况下，有两个函数可以帮助程序员。WSAIsBlocking()可以用来确定在该进程上是否有阻塞的Windows Sockets调用。WSACancelBlookingCall()可以用来取消在线的阻塞调用，如果有的话。任何其他的Windows Sockets函数如果在这种情况下被调用，则会失败并返回错误代码WSAEINPROGRESS。要强调的是，这一限制适用于所有阻塞和非阻塞的操作。

虽然这种机制对于简单的应用程序已经足够了，但这不能支持高级应用程序的复杂的消息发送要求。（例如，那些MDI模型的用户）对于这样的应用程序，Windows Sockets API设计了WSASetBlockingHook()函数，这个函数可以允许程序员定义特殊的阻塞钩子来代替上面讨论的缺省消息发送例程。

只有在以下都为真时，Windows Sockets DLL才调用阻塞钩子函数：例程是被定义为可以阻塞的，指定的套接口也是阻塞套接口，而且请求不能被立刻完成。（套接口是被缺省地设为阻塞方式的，但IOCTL FIONBIO和WSAAsyncSelect()都可以把套接口设置成为非阻塞模式）。如果应用程序只使用非阻塞方式的套接口，而且使用WSAAsyncSelect()和/或WSAAsyncGetXByY()例程，而不是使用select()和/或getXbyY()例程，那么阻塞钩子函数就永远也不会被调用，应用程序也不用再操心由于阻塞钩子函数而带来的重入问题。

如果一个应用程序在唤起异步或非阻塞方式调用时使用了一个内存对象的指针（例如一个缓冲区，或者一个全程变量）作为参数，那么应用程序要保证那个对象在Windows Sockets实现的整个操作中都可得到并使用。应用程序不能再唤起可能影响到内存唤射或寻址能力的其他的Windows函数。在多线程系统中，应用程序也有责任使用某种同步机制来协调对内存对象的存取。Windows Sockets实现不能，也不会提出这种事情。没有遵守这条规则，所可能产生的后果已不在规范讨论的范围之内。

## 2 数据库函数

Windows Sockets规范定义了如下数据库例程。正如我们先前提出的，Windows Sockets提供者有可能不采用依赖于本地数据库的方式来实现这些函数。某些数据库例程返回的指针（例如gethostbyname()）指向的区域是由Windows Sockets函数库分配的。这些指针指向的数据是易失的。它们只在该线程的下一个Windows Sockets API调用前有效。此外，应用程序不应试图修改这个结构，或者释放其中的任何一部分。在一个线程中，这个结构只有一份拷贝。因此，应用程序应该在发出下一个Windows Sockets API调用以前把所需的信息拷贝下来。

gethostbyaddr()*从网络地址得到对应的名字（有可能多个）和地址。

gethostbyname()*从主机名得到对应的名字（有可能多个）和地址。

gethostname()得到本地主机名。

getprotbyname()*从协议名得到对应的协议名和数值。

getservbyname()*从一个服务的名字得到对应的服务名以及端口号。

getservbyport()*从一个端口号得到对应的服务名以及端口号。

*表示例程在某些情况下可能会阻塞。

## 3 针对Microsoft Windows的扩展函数

Windows Sockets规范提供了许多在标准的Berkelet套接口例程之外的扩展函数。本质上，这些扩展的API是为了应用程序能更好地处理基于消息的异步发送的网络事件。虽然基于Windows Sockets的编程并不强制要使用这个扩展的API集（WSAStartup()和WSACleanup()除外）但我们推荐应用程序开发者遵循Microsoft Windows的编程范例。

WSAAsyncGetHostByAddr()一个标准的Berkeley的getXbyY()函数集合的异步版本。例如WSAAsyncGetHostByName()函数提供了一个标准Berkeley的gethostbyname()函数的异步基于消息的实现。

WSAAsyncGetHostByName()

WSAAsyncGetProtoByName()

WSAAsyncGetProtByNumber()

WSAAsyncGetServByName()

WSAAsyncGetServByPort()

WSAAsyncSelect()select()函数的异步版本。

WSACancelAsyncRequest()取消一个未完成的WSAAsyncGetXByY()函数的实例。

WSACancelBlockingCall()取消未完成的阻塞的API调用。

WSACleanup()从底层的Windows Sockets DLL中撤销注册。

WSAGetLastError()得到最近的一个Windows Sockets API调用错误的详细情况。

WSAIsBlocking()确定底层的Windows Sockets DLL是否在该线程已经被一个调用阻塞。

WSASetBlockingHook()为底层的Windows Sockets实现设置阻塞钩子。

WSASetLastError()设置下一次WSAGetLastError()返回的错误信息。

WSAStartup()初始化底层的Windows Sockets DLL。

WSAUnhookBlockingHook()恢复原始的阻塞钩子。

### 3.1 异步选择机制

WSAAsyncSelect()调用允许应用程序程序注册一个或多个感兴趣的网络事件。这一API调用用来取代探寻网络I/O调用。在select()或非阻塞I/O例程（例如send()和recv()）已经被调用或将要被调用的情况下都可以使用WSAAsyncSelect()调用。在这种情况下，在声明感兴趣的网络事件时，你必须提供一个通知时使用的窗口句柄。那么在你声明的感兴趣的网络事件发生时，对应的窗口将收到一个基于消息的通知。

Windows Sockets允许对于一特定的套接口声明如下感兴趣的网络事件：

*套接口已准备读数据。

*套接口已准备写数据。

*带外数据准备好。

*套接口准备接收连接请求。

*非阻塞连接操作结束。

*连接关闭。

### 3.2 异步支持例程

异步数据库函数允许应用程序用异步方式请求信息。某些网络实现和/或配置，需要通过执行网络操作来应答这些请求。WSAAsyncGetXByY()函数允许应用程序开发者不必象在使用Berkeley标准函数时阻塞整个Windows环境。WSACancelAsyncRequest()函数可以允许一个应用程序取消任何未完成的异步的WSAAsyncGetXByY()请求。

### 3.3 阻塞钩子函数方法

正如4.1.1节所讲述的，Windows Sockets实现以这样一种方式阻塞一个操作，Windows消息处理可以继续，发出调用的应用程序仍然可以收到Windows消息。但在某些情况下，应用程序可能希望影响或改变这种伪阻塞的实现方式。WSASetBlockingHook()函数就提供了这样一种功能。它使得应用程序可以替换Windows Sockets实现在“阻塞”操作中放弃处理器时调用的例程。

### 3.4 错误处理

为了与基于线程的环境兼容，API调用的错误细节可以通过WSAGetLastError()调用得到。虽然已经为大家接收的Berkeley风格的机制是通过"errno"得到关于套接口的网络错误的，这种机制不能够保证在多线程环境中错误代码的完整性和正确性。WSAGetLastError()允许程序员能够得到对应于每一线程的最近的错误代码。

WSAGetLastError()所返回的错误代码尽量避免与标准的Microsoft C错误代码冲突。但是某些Windows Sockets例程返回的错误代码是在Microsoft C定义的标准错误代码之内的。如果你使用的应用程序开发环境中的错误代码定义与Microsoft C不同，那么我们建议你使用Windows Sockets错误代码前缀"WSA"来保证准确的检测错误。

这份规范定义了一个推荐的错误代码的集合，而且列举了每一个函数有可能返回的错误。但是某些Windows Sockets实现也有可能返回一些在我们列举之外的错误代码。应用程序应该具备能够处理在每个API描述下列举的错误代码之外的错误的能力。然而Windows Sockets实现不能返回在附录4.1中列举的合法Windows Sockets错误之外的任何数值。

### 3.5 通过中介DLL调用Windows Sockets DLL

Windows Sockets DLL既可以直接从应用程序中调用也可以通过中介DLL调用。通过中介DLL的例子是：使用Windows Sockets为应用程序实现一个提供通用网络功能的虚拟网络API层，这样的DLL可以同时被多个应用程序使用。担这样的DLL必须对WSAStartup()和WSACleanup()这两个函数非常警惕，它们必须保证在任何一个使用Windows Sockets调用的任务前后均调用了WSAStartup()和WSACleanup()。这是因为Windows Sockets DLL需要一个对WSAStartup()的调用来为每个任务初始化其数据结构，也需要一个对WSACleanup()的调用来释放为任务分配的所有资源。

有至少两种方法去完成这一任务。最简单的方法是中介DLL具有与WSAStarup()和WSACleanup()类似的调用提供给应用程序使用，DLL将在这些例程中调用WSAStartup()和WSACleanup()。另一种机制就是中介DLL建立一个任务句柄列表。任务句柄是从GetCurrentTask()这一个Windows API中获得的。在中介DLL的每一个入口处检查对于当前任务WSAStartup()函数是否已被调用，并且在必要的时候调用WSACleanup()函数。

在Windows NT环境中，这一点是没有必要的。因为Windows NT中的DLL结构与流程是与Windows不同的。在Windows NT中，中介DLL只需简单的在它的DLL初始化例程中调用WSAStartup()即可。这个例程将在任何一个新的进程试图使用DLL的开始时刻被执行。

如果中介DLL调用了阻塞操作而又没有安装任何它自己的阻塞钩子，那么DLL作者必须清楚地认识到控制有可能会通过应用程序安装的阻塞钩子或缺省的阻塞钩子回到应用程序手中。这样应用程序有可能通过WSACancelBlockingCall()来取消中介DLL的阻塞操作，如果这种情况发生了，中介DLL的阻塞操作会失败并返回错误代码WSAEINTR。这时候，中介DLL必须尽快地把控制交还给调用它的任务。因为用户有可能按了Cancel或者Close按钮。应用程序正在急切地盼望获得CPU的控制权。我们推荐中介DLL在进行阻塞调用时安装自己的阻塞钩子来防止不可预见的中介DLL和应用程序之间的互相影响。

### 3.6 Windows Sockets实现内部对消息的使用

为了把Windows Sockets实现成一个纯粹的DLL，有时在DLL内部互相发送消息来通讯和定时是必要的。这是非常合法的。但是Windows DLL不应该无缘无故地发送消息给一个由客户打开的窗口句柄，除非应用程序要求这些消息。所以为了自身的目的而需要使用消息的Windows Sockets DLL打开了一个隐藏的窗口，并且发送必要的消息给这个窗口的句柄。

### 3.7 私有的API接口

附录B.3中的WINSOCK.DEF文件列出了Windows Sockets API功能调用的序数。除了已经列出的序数值外，所有小于999的序数都是保留给将来的Windows Sockets使用的。对于一个Windows Sockets实现来说，提供附加的私有的接口也是很方便的。这是完全可以接受的，只要这些调用的序数大于1000，要注意的是，任何使用了某个特定Windows Sockets DLL私有的API的应用程序极有可能在任何其他供应商的Windows Sockets DLL上无法工作。应该注意到，只有使用在这份规范中定义的API才能可以保证每一个Windows Sockets实现都支持。

如果一个应用程序使用了某个供应商的Windows Sockets DLL的特定接口，最好不要把应用程序与DLL静态连接，而通过Windows Sockets例程LoadLibrary()和GetProcAddress()动态载入，这就使得应用程序在其他不支持同样的扩展功能集的Windows Sockets DLL系统上运行时，可以得到适当的错误信息。


