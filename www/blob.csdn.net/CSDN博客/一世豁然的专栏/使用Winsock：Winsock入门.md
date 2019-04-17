# 使用Winsock：Winsock入门 - 一世豁然的专栏 - CSDN博客





2018年10月16日 15:36:06[一世豁然](https://me.csdn.net/Explorer_day)阅读数：311








以下是Windows套接字编程入门的分步指南。 它旨在提供对基本Winsock函数和数据结构的理解，以及它们如何协同工作。

用于说明的客户端和服务器应用程序是一个非常基本的客户端和服务器。 Microsoft Windows软件开发工具包（SDK）附带的示例中包含更高级的代码示例。

客户端和服务器应用程序的前几个步骤相同。
- 关于服务器和客户端
- 创建一个基本的Winsock应用程序
- 正在初始化Winsock

以下部分描述了创建Winsock客户端应用程序的其余步骤。
- 为客户端创建套接字
- 连接到插座
- 在客户端上发送和接收数据
- 断开客户端连接

以下部分描述了创建Winsock服务器应用程序的其余步骤。
- 为服务器创建套接字
- 绑定套接字
- 听一个插座
- 接受连接
- 在服务器上接收和发送数据
- 断开服务器连接

这些基本示例的完整源代码。
- 运行Winsock客户端和服务器代码示例
- 完整的Winsock客户端代码
- 完整的Winsock服务器代码



**高级Winsock实例**

Windows SDK附带了几个更高级的Winsock客户端和服务器示例。 默认情况下，Winsock示例源代码由Windows SDK for Windows 7安装在以下目录中：

*C:\Program Files\Microsoft SDKs\Windows\v7.0\Samples\NetDs\winsock*

在早期版本的Windows SDK中，上述路径中的版本号将更改。 例如，Winsock示例源代码由Windows SDK for Windows Vista安装在以下默认目录中

*C:\Program Files\Microsoft SDKs\Windows\v6.0\Samples\NetDs\winsock*

以下列出的高级样本按从高到低的顺序排列，可在以下目录中找到：
- iocp

该目录包含三个使用I / O完成端口的示例程序。 这些程序包括一个使用WSAAccept函数的Winsock服务器（iocpserver），一个使用AcceptEx函数的Winsock服务器（iocpserverex），以及一个用于测试这些服务器中的任何一个的简单多线程Winsock客户端（iocpclient）。 服务器程序支持多个客户端通过TCP / IP连接并发送任意大小的数据缓冲区，然后服务器回送给客户端。 为方便起见，开发了一个简单的客户端程序iocpclient，用于连接并不断向服务器发送数据，以便使用多个线程对其进行压力。 使用I / O完成端口的Winsock服务器提供最强大的性能。
- overlap

此目录包含使用重叠I / O的示例服务器程序。 示例程序使用AcceptEx函数和重叠I / O来有效地处理来自客户端的多个异步连接请求。 服务器使用AcceptEx函数在单线程Win32应用程序中复用不同的客户端连接。 使用重叠I / O可实现更高的可伸缩性。
- WSAPoll

该目录包含一个演示WSAPoll函数使用的基本示例程序。 组合的客户端和服务器程序是非阻塞的，并使用WSAPoll函数来确定何时可以无阻塞地发送或接收。 此示例更多用于说明，而不是高性能服务器。‘
- simple

该目录包含三个基本示例程序，用于演示服务器使用多个线程。 这些程序包括一个简单的TCP / UDP服务器（简单），一个仅使用TCP的服务器（simples_ioctl），它使用Win32控制台应用程序中的select函数来支持多个客户端请求，以及一个客户端TCP / UDP程序（simplec），用于测试服务器。 服务器演示了使用多个线程来处理多个客户端请求。 此方法具有可伸缩性问题，因为为每个客户端请求创建了单独的线程。
- accept

该目录包含基本样本服务器和客户端程序。 服务器演示如何使用select函数使用非阻塞接受或使用WSAAsyncSelect函数使用异步接受。 此示例更多用于说明，而不是高性能服务器。’





一、关于服务器和客户端

有两种不同类型的套接字网络应用程序：服务器和客户端。

服务器和客户端有不同的行为; 因此，创建它们的过程是不同的。 以下是创建流式TCP / IP服务器和客户端的一般模型。



一）、服务器
- 初始化Winsock。
- 创建一个套接字。
- 绑定套接字。
- 听取客户端的套接字。
- 接受来自客户端的连接。
- 接收和发送数据。
- 断开链接。



二）、客户端
- 初始化Winsock。
- 创建一个套接字。
- 连接到服务器。
- 发送和接收数据。
- 断开链接。



> 
注意

对于客户端和服务器，某些步骤是相同的。 这些步骤几乎完全相同。 本指南中的某些步骤将特定于正在创建的应用程序类型。






二、创建一个基本的Winsock应用程序

1、为了创建基本的Winsock应用程序

2、创建一个新的空项目。

3、将空C ++源文件添加到项目中。

4、确保构建环境引用Microsoft Windows软件开发工具包（SDK）或早期平台软件开发工具包（SDK）的Include，Lib和Src目录。确保构建环境链接到Winsock库文件Ws2_32.lib。 使用Winsock的应用程序必须与Ws2_32.lib库文件链接。 #pragma注释向链接器指示需要Ws2_32.lib文件。

5、开始编写Winsock应用程序。 通过包含Winsock 2头文件来使用Winsock API。 Winsock2.h头文件包含大多数Winsock函数，结构和定义。 Ws2tcpip.h头文件包含在针对TCP / IP的WinSock 2协议特定附件文档中引入的定义，其中包括用于检索IP地址的较新功能和结构。

> 
注意

Stdio.h用于标准输入和输出，特别是printf（）函数。


```cpp
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
  return 0;
}
```

> 
注意

如果应用程序正在使用IP Helper API，则需要Iphlpapi.h头文件。 当需要Iphlpapi.h头文件时，Winsock2.h头文件的#include行应该放在Iphlpapi.h头文件的#include行之前。

Winsock2.h头文件内部包含来自Windows.h头文件的核心元素，因此Winsock应用程序中的Windows.h头文件通常没有#include行。 如果Windows.h头文件需要#include行，则应在#define WIN32_LEAN_AND_MEAN宏之前添加#include行。 由于历史原因，Windows.h标头默认包含Windows套接字1.1的Winsock.h头文件。 Winsock.h头文件中的声明将与Windows Sockets 2.0所需的Winsock2.h头文件中的声明冲突。 WIN32_LEAN_AND_MEAN宏可防止Windows.h标头包含Winsock.h。 示例说明如下所示。


```cpp
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {
  return 0;
}
```





三、初始化Winsock

调用Winsock函数的所有进程（应用程序或DLL）必须在进行其他Winsock函数调用之前初始化Windows Sockets DLL的使用。 这也确保系统支持Winsock。

为了初始化Winsock

1、创建一个名为wsaData的WSADATA对象。

```cpp
WSADATA wsaData;
```

2、调用WSAStartup并将其值作为整数返回并检查错误。

```cpp
int iResult;

// Initialize Winsock
iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
}
```

调用WSAStartup函数以启动WS2_32.dll的使用。

WSADATA结构包含有关Windows套接字实现的信息。 WSAStartup的MAKEWORD（2,2）参数在系统上发出对Winsock 2.2版的请求，并将传递的版本设置为调用者可以使用的最高版本的Windows套接字支持。





四、Winsock客户端应用程序

以下部分描述了创建Winsock客户端应用程序的其余步骤。 以下是创建流式TCP / IP客户端的一般模型。
- 为客户端创建套接字
- 连接到插座
- 在客户端上发送和接收数据
- 断开客户端连接



一）、为客户端创建套接字

初始化之后，必须实例化SOCKET对象以供客户端使用。

为了创建套接字

1、声明包含sockaddr结构的addrinfo对象并初始化这些值。 对于此应用程序，未指定Internet地址系列，以便可以返回IPv6或IPv4地址。 应用程序请求套接字类型为TCP协议的流套接字。

```cpp
struct addrinfo *result = NULL,
                *ptr = NULL,
                hints;

ZeroMemory( &hints, sizeof(hints) );
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;
```

2、调用getaddrinfo函数，请求在命令行上传递的服务器名称的IP地址。 客户端将连接到的服务器上的TCP端口在此示例中由DEFAULT_PORT定义为27015。 getaddrinfo函数将其值返回为检查错误的整数。

```cpp
#define DEFAULT_PORT "27015"

// Resolve the server address and port
iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
if (iResult != 0) {
    printf("getaddrinfo failed: %d\n", iResult);
    WSACleanup();
    return 1;
}
```

3、创建一个名为ConnectSocket的SOCKET对象。

```cpp
SOCKET ConnectSocket = INVALID_SOCKET;
```

4、调用套接字函数并将其值返回到ConnectSocket变量。 对于此应用程序，请使用调用getaddrinfo返回的第一个IP地址，该地址与hints参数中指定的地址系列，套接字类型和协议相匹配。 在此示例中，指定了TCP流套接字，其套接字类型为SOCK_STREAM，协议为IPPROTO_TCP。 地址系列未指定（AF_UNSPEC），因此返回的IP地址可以是服务器的IPv6或IPv4地址。

如果客户端应用程序只想使用IPv6或IPv4进行连接，则需要在hints参数中将地址族设置为IPv6的AF_INET6或IPv4的AF_INET。

```cpp
// Attempt to connect to the first address returned by
// the call to getaddrinfo
ptr=result;

// Create a SOCKET for connecting to server
ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
    ptr->ai_protocol);
```

5、检查错误以确保套接字是有效的套接字。

```cpp
if (ConnectSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    return 1;
}
```

传递给套接字函数的参数可以针对不同的实现进行更改。

错误检测是成功的网络代码的关键部分。 如果套接字调用失败，则返回INVALID_SOCKET。 上一代码中的if语句用于捕获创建套接字时可能发生的任何错误。 WSAGetLastError返回与上次发生的错误关联的错误号。

> 
注意

根据应用，可能需要进行更广泛的错误检查。


WSACleanup用于终止WS2_32 DLL的使用。



二）、连接到套接字

要使客户端在网络上进行通信，它必须连接到服务器。‘

为了链接套接字

调用connect函数，将创建的套接字和sockaddr结构作为参数传递。

```cpp
// Connect to server.
iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
if (iResult == SOCKET_ERROR) {
    closesocket(ConnectSocket);
    ConnectSocket = INVALID_SOCKET;
}

// Should really try the next address returned by getaddrinfo
// if the connect call failed
// But for this simple example we just free the resources
// returned by getaddrinfo and print an error message

freeaddrinfo(result);

if (ConnectSocket == INVALID_SOCKET) {
    printf("Unable to connect to server!\n");
    WSACleanup();
    return 1;
}
```

’getaddrinfo函数用于确定sockaddr结构中的值。 在此示例中，getaddrinfo函数返回的第一个IP地址用于指定传递给connect的sockaddr结构。 如果连接调用未能通过第一个IP地址，则尝试从getaddrinfo函数返回的链表中的下一个addrinfo结构。

sockaddr结构中指定的信息包括：
- 客户端将尝试连接的服务器的IP地址。
- 客户端将连接到的服务器上的端口号。 当客户端调用getaddrinfo函数时，此端口被指定为端口27015。



三）、在客户端上发送和接收数据

以下代码演示了建立连接后客户端使用的send和recv函数。

客户端

```cpp
#define DEFAULT_BUFLEN 512

int recvbuflen = DEFAULT_BUFLEN;

char *sendbuf = "this is a test";
char recvbuf[DEFAULT_BUFLEN];

int iResult;

// Send an initial buffer
iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
if (iResult == SOCKET_ERROR) {
    printf("send failed: %d\n", WSAGetLastError());
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
}

printf("Bytes Sent: %ld\n", iResult);

// shutdown the connection for sending since no more data will be sent
// the client can still use the ConnectSocket for receiving data
iResult = shutdown(ConnectSocket, SD_SEND);
if (iResult == SOCKET_ERROR) {
    printf("shutdown failed: %d\n", WSAGetLastError());
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
}

// Receive data until the server closes the connection
do {
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0)
        printf("Bytes received: %d\n", iResult);
    else if (iResult == 0)
        printf("Connection closed\n");
    else
        printf("recv failed: %d\n", WSAGetLastError());
} while (iResult > 0);
```

send和recv函数分别返回发送或接收的字节数的整数值或错误。 每个函数也采用相同的参数：活动套接字，字符缓冲区，要发送或接收的字节数，以及要使用的任何标志。



四）、断开客户端连接

一旦客户端完成发送和接收数据，客户端将断开与服务器的连接并关闭套接字。

为了断开连接并关闭套接字

1、当客户端完成向服务器发送数据时，可以调用shutdown函数指定SD_SEND以关闭套接字的发送端。 这允许服务器释放此套接字的一些资源。 客户端应用程序仍然可以在套接字上接收数据。

```cpp
// shutdown the send half of the connection since no more data will be sent
iResult = shutdown(ConnectSocket, SD_SEND);
if (iResult == SOCKET_ERROR) {
    printf("shutdown failed: %d\n", WSAGetLastError());
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
}
```

2、当客户端应用程序完成接收数据时，将调用closesocket函数来关闭套接字。

使用Windows套接字DLL完成客户端应用程序时，将调用WSACleanup函数以释放资源。

```cpp
// cleanup
closesocket(ConnectSocket);
WSACleanup();

return 0;
```





五、Winsock服务器应用程序

以下部分描述了创建Winsock服务器应用程序的其余步骤。 以下是创建流式TCP / IP服务器的一般模型。
- 为服务器创建套接字
- 绑定套接字
- 听一个插座
- 接受连接
- 在服务器上接收和发送数据
- 断开服务器连接



一）、为服务器创建套接字

初始化之后，必须实例化SOCKET对象以供服务器使用。

为服务器创建套接字

1、getaddrinfo函数用于确定sockaddr结构中的值：
- AF_INET用于指定IPv4地址族。
- SOCK_STREAM用于指定流套接字。
- IPPROTO_TCP用于指定TCP协议。
- AI_PASSIVE标志表示调用者打算在调用bind函数时使用返回的套接字地址结构。 当设置AI_PASSIVE标志并且getaddrinfo函数的nodename参数是NULL指针时，套接字地址结构的IP地址部分对于IPv4地址设置为INADDR_ANY，对于IPv6地址设置为IN6ADDR_ANY_INIT。
- 27015是与客户端将连接到的服务器关联的端口号。

addadfo结构由getaddrinfo函数使用。

```cpp
#define DEFAULT_PORT "27015"

struct addrinfo *result = NULL, *ptr = NULL, hints;

ZeroMemory(&hints, sizeof (hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = IPPROTO_TCP;
hints.ai_flags = AI_PASSIVE;

// Resolve the local address and port to be used by the server
iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
if (iResult != 0) {
    printf("getaddrinfo failed: %d\n", iResult);
    WSACleanup();
    return 1;
}
```

2、为服务器创建一个名为ListenSocket的SOCKET对象以侦听客户端连接。

```cpp
SOCKET ListenSocket = INVALID_SOCKET;
```

3、调用套接字函数并将其值返回给ListenSocket变量。 对于此服务器应用程序，请使用调用getaddrinfo返回的第一个IP地址，该地址与hints参数中指定的地址系列，套接字类型和协议相匹配。 在此示例中，请求IPv4的TCP流套接字，其地址系列为IPv4，套接字类型为SOCK_STREAM，协议为IPPROTO_TCP。 因此，为ListenSocket请求了一个IPv4地址。

如果服务器应用程序想要侦听IPv6，则需要在hints参数中将地址族设置为AF_INET6。 如果服务器想要同时侦听IPv6和IPv4，则必须创建两个侦听套接字，一个用于IPv6，另一个用于IPv4。 这两个插座必须由应用程序单独处理。

Windows Vista及更高版本提供了创建单个IPv6套接字的功能，该套接字处于双堆栈模式以侦听IPv6和IPv4。 有关此功能的更多信息，请参阅双栈套接字。

```cpp
// Create a SOCKET for the server to listen for client connections

ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
```

4、检查错误以确保套接字是有效的套接字。

```cpp
if (ListenSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    return 1;
}
```



二）、绑定套接字

要使服务器接受客户端连接，必须将其绑定到系统中的网络地址。 以下代码演示了如何将已创建的套接字绑定到IP地址和端口。 客户端应用程序使用IP地址和端口连接到主机网络。

为了绑定套接字

sockaddr结构保存有关地址系列，IP地址和端口号的信息。

调用bind函数，将从getaddrinfo函数返回的创建的socket和sockaddr结构作为参数传递。 检查一般错误。

```cpp
// Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
```

调用bind函数后，不再需要getaddrinfo函数返回的地址信息。 调用freeaddrinfo函数以释放getaddrinfo函数为此地址信息分配的内存。

```cpp
freeaddrinfo(result);
```



三）、监听套接字

套接字绑定到系统上的IP地址和端口后，服务器必须侦听该IP地址和端口以获取传入的连接请求。

为了监听套接字

调用listen函数，将创建的套接字作为参数传递，并将backlog的值，待接受的挂起队列的最大长度作为接受。 在此示例中，backlog参数设置为SOMAXCONN。 此值是一个特殊常量，它指示Winsock提供程序为此套接字允许队列中最大合理数量的挂起连接。 检查一般错误的返回值。

```cpp
if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
    printf( "Listen failed with error: %ld\n", WSAGetLastError() );
    closesocket(ListenSocket);
    WSACleanup();
    return 1;
}
```



四）、接受连接

一旦套接字正在侦听连接，程序必须处理该套接字上的连接请求。

为了接受套接字上的连接

1、创建一个名为ClientSocket的临时SOCKET对象，用于接受来自客户端的连接。

```cpp
SOCKET ClientSocket;
```

2、通常，服务器应用程序将被设计为侦听来自多个客户端的连接。 对于高性能服务器，通常使用多个线程来处理多个客户端连接。

使用Winsock有几种不同的编程技术可用于侦听多个客户端连接。 一种编程技术是创建一个连续循环，使用listen函数检查连接请求（请参阅侦听套接字）。 如果发生连接请求，应用程序将调用accept，AcceptEx或WSAAccept函数，并将工作传递给另一个线程来处理请求。 其他几种编程技术也是可能的。

请注意，此基本示例非常简单，不使用多个线程。 该示例也只是侦听并接受单个连接。

```cpp
ClientSocket = INVALID_SOCKET;

// Accept a client socket
ClientSocket = accept(ListenSocket, NULL, NULL);
if (ClientSocket == INVALID_SOCKET) {
    printf("accept failed: %d\n", WSAGetLastError());
    closesocket(ListenSocket);
    WSACleanup();
    return 1;
}
```

3、当客户端连接被接受时，服务器应用程序通常会将接受的客户端套接字（上面示例代码中的ClientSocket变量）传递给工作线程或I / O完成端口，并继续接受其他连接。 在此基本示例中，服务器继续执行下一步。

有许多其他编程技术可用于侦听和接受多个连接。 这些包括使用select或WSAPoll函数。 Microsoft Windows软件开发工具包（SDK）附带的Advanced Winsock示例中说明了这些各种编程技术中的一些示例。

> 
注意

在Unix系统上，服务器的通用编程技术是用于监听连接的应用程序。 当接受连接时，父进程将调用fork函数来创建一个新的子进程来处理客户端连接，从父进程继承套接字。 Windows不支持此编程技术，因为不支持fork函数。 此技术通常也不适用于高性能服务器，因为创建新进程所需的资源远远大于线程所需的资源。




五）、在服务器上接收和发送数据

以下代码演示了服务器使用的recv和send函数。

为了在套接字上接收和发送数据

```cpp
#define DEFAULT_BUFLEN 512

char recvbuf[DEFAULT_BUFLEN];
int iResult, iSendResult;
int recvbuflen = DEFAULT_BUFLEN;

// Receive until the peer shuts down the connection
do {

    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
        printf("Bytes received: %d\n", iResult);

        // Echo the buffer back to the sender
        iSendResult = send(ClientSocket, recvbuf, iResult, 0);
        if (iSendResult == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
        printf("Bytes sent: %d\n", iSendResult);
    } else if (iResult == 0)
        printf("Connection closing...\n");
    else {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

} while (iResult > 0);
```

send和recv函数分别返回发送或接收的字节数的整数值或错误。 每个函数也采用相同的参数：活动套接字，字符缓冲区，要发送或接收的字节数，以及要使用的任何标志。



六）、断开服务器连接

一旦服务器完成从客户端接收数据并将数据发送回客户端，服务器将断开与客户端的连接并关闭套接字。

为了断开服务器连接

1、当服务器完成向客户端发送数据时，可以调用shutdown函数指定SD_SEND以关闭套接字的发送端。 这允许客户端释放此套接字的一些资源。 服务器应用程序仍然可以在套接字上接收数据。

```cpp
// shutdown the send half of the connection since no more data will be sent
iResult = shutdown(ClientSocket, SD_SEND);
if (iResult == SOCKET_ERROR) {
    printf("shutdown failed: %d\n", WSAGetLastError());
    closesocket(ClientSocket);
    WSACleanup();
    return 1;
}
```

2、当客户端应用程序完成接收数据时，将调用closesocket函数来关闭套接字。

使用Windows套接字DLL完成客户端应用程序时，将调用WSACleanup函数以释放资源。

```cpp
// cleanup
closesocket(ClientSocket);
WSACleanup();

return 0;
```





六、运行Winsock客户端和服务器代码示例

本节包含TCP / IP客户端和服务器应用程序的完整源代码：
- 完整的Winsock客户端代码
- 完整的Winsock服务器代码

应在启动客户端应用程序之前启动服务器应用程序。

要执行服务器，请编译完整的服务器源代码并运行可执行文件。 服务器应用程序侦听TCP端口27015以供客户端连接。 一旦客户端连接，服务器就会从客户端接收数据，并将收到的数据回送（发送）回客户端。 当客户端关闭连接时，服务器会关闭客户端套接字，关闭套接字并退出。

要执行客户端，请编译完整的客户端源代码并运行可执行文件。 客户端应用程序要求在执行客户端时将运行服务器应用程序的计算机的名称或IP地址作为命令行参数传递。 如果客户端和服务器在示例计算机上执行，则可以按如下方式启动客户端：

客户端localhost

客户端尝试在TCP端口27015上连接到服务器。客户端连接后，客户端将数据发送到服务器并接收从服务器发回的任何数据。 然后客户端关闭套接字并退出。



一）、完整的Winsock客户端代码

以下是基本Winsock TCP / IP客户端应用程序的完整源代码。

**Winsock客户端源代码**

```cpp
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
            printf("Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while( iResult > 0 );

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
```



二）、完整的Winsock服务器代码

以下是基本Winsock TCP / IP Server应用程序的完整源代码。

**Winsock服务器源代码**

```cpp
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int __cdecl main(void) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

        // Echo the buffer back to the sender
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
```





