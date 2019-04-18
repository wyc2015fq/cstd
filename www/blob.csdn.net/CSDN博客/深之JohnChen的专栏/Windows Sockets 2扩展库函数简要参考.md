# Windows Sockets 2扩展库函数简要参考 - 深之JohnChen的专栏 - CSDN博客

2005年12月22日 12:46:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2481标签：[sockets																[windows																[扩展																[socket																[网络																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=sockets&t=blog)
个人分类：[网络编程](https://blog.csdn.net/byxdaz/article/category/166215)


## 1 WSAAccept()

简述：根据条件函数的返回值有条件地接受连接，同时（可选地）创建和/或加入一个套接口组。

SOCKET WSAAPI WSAAccept ( SOCKET s, struct

sockaddr FAR * addr, int FAR * addrlen,

LPCONDITIONPROC lpfnCondition, DWORD

dwCallbackData );

s：标识一个套接口的描述字，该套接口在listen()后监听连接。

addr：（可选）指针，指向存放通讯层所知的连接实体地址的缓冲区。addr参数的具体格式由套接口创建时产生的地址族决定。

addrlen：（可选）指针，指向存放addr地址长度的整形数。

lpfnCondition：（可选的）用户提供的条件函数的进程实例地址。该函数根据参数传入的调用者信息作出接受或拒绝的决定，并通过给结果参数赋予特定的值来（可选地）创建和/或加入一个套接口组。

dwCallbackData：作为条件函数参数返回给应用程序的回调数据。WinSock不分析该参数。

返回值：

若无错误发生，WSAAccept()函数返回所接受套接口的描述字。否则的话，将返回INVALID_SOCKET错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。

addrlen参数引用的整形数初始时包含了addr参数所指向的空间数，在调用返回时包含了返回地址的实际长度。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAECONNREFUSED根据条件函数的返回值（CF_REJECT）强制拒绝连接请求。

WSAENETDOWN网络子系统失效。

WSAEFAULT addrlen参数太小（小于sockaddr结构的大小），或者lpfnCondition并不是用户空间的一部分。

WSAEINTR通过WSACancelBlockingCall()函数取消（阻塞）调用。

WSAEINPROGRESS一个阻塞WinSock调用正在进行。

WSAEINVALWSAAccept()调用前未执行listen()调用；条件函数中的g参数非法；条件函数的返回值非法；套接口处于非法状态。

WSAEMFILEWSAAccept()调用时排队队列非空，且无可用套接口描述字。

WSAENOBUFS无可用缓冲区空间。

WSAENOTSOCK描述字不是一个套接口。

WSAEOPNOTSUPP所引用的套接口不是支持面向连接服务类型的。

WSATRY_AGAIN根据条件函数的返回值(CF_DEFER) ，连接请求被推迟。

WSAEWOULDBLOCK套接口标志为非阻塞，无连接请求供接受。

WSAEACCES被推迟的连接请求超时或撤销。

另请参阅：accept(), bind(), connect(), getsockopt(),listen(), select(), socket(), SAAsyncSelect(),WSAConnect().



## 2 WSACloseEvent()

简述：关闭一个开放的事件对象句柄。

#include <winsock2.h>

BOOL WSAAPI WSACloseEvent( WSAEVENT hEvent );

hEvent：标识一个开放的事件对象句柄。

返回值：

如果函数顺利完成，返回值为真TRUE。如果失败，返回值为假FALSE。可用WSAGetLastError()调用获取更多的错误信息。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSA_INVALID_HANDLEhEvent不是一个合法的事件对象句柄。

另请参阅：WSACreateEvent(), WSAEnumNetworkEvents(),WSAEventSelect(), WSAGetOverlappedResult(),WSARecv(), WSARecvFrom(), WSAResetEvent(),WSASend(), WSASendTo(), WSASetEvent(),WSAWaitForMultipleEvents().



## 3 WSAConnect()

简述：创建一个与远端的连接，交换连接数据，并根据所提供的流描述确定所需的服务质量。

#include <winsock2.h>

int WSAAPI WSAConnect ( SOCKET s, const struct

sockaddr FAR * name,

int namelen, LPWSABUF lpCallerData, LPWSABUF

lpCalleeData,

LPQOS lpSQOS, LPQOS lpGQOS );


s：用于描述一个未连接套接口的描述字。

name：欲与套接口连接的远端名字。

namelen：名字长度。

lpCallerData：指向用户数据的指针，该数据在建立连接时将传送到远端。

lpCalleeData：指向用户数据的指针，该数据在建立连接时将从远端传送回本机。

lpSQOS：指向套接口s流描述的指针，每个方向一个。

lpGQOS：指向套接口组流描述的指针。（如果有套接口组的话）

返回值：

如果无错误发生，WSAConnect()返回0。否则的话，将返回INVALID_SOCKET错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。

对于阻塞套接口来说，返回值表示连接试图是否成功。

对于非阻塞套接口来说，连接试图不一定马上完成。在这种情况下，WSAConnect()返回SOCKET_ERROR，且WSAGetLastError()返回WSAEWOULDBLOCK. 此时应用程序可以：

1。利用select()函数，通过检查套接口是否可写来判断连接请求是否完成。或者，

2。如果应用程序已使用WSAAsyncSelect()函数来确定对连接事件的兴趣，则当连接操作完成时应用程序将收到FD _CONNECT通知。或者，

3。如果应用程序已使用WSAEventSelect()函数来确定对连接事件的兴趣，则当连接操作完成时相应的事件对象将设置信号。

对于一个非阻塞套接口来说，在连接试图完成之前，任何对该套接口的WSAConnect()调用都将以WSAEALREADY错误失败。

如果返回值指出连接试图失败（例如WSAECONNREFUSED, WSAENETUNREACH，WSAETIMEDOUT）则应用程序可对该套接口再次调用WSAConnect()函数。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEADDRINUSE所指地址已被使用。

WSAEINTR通过WSACancelBlockingCall()函数中止了阻塞调用。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAEALREADY在所指定的套接口上正在进行一个非阻塞的connect()或WSAConnect()调用。

WSAEADDRNOTAVAIL本地机器上无法获得所指定的地址。

WSAEAFNOSUPPORT所指定地址族中的地址无法与本套接口一起使用。

WSAECONNREFUSED连接试图被拒绝。

WSAEFAULTname或namelen参数不是用户地址空间的一个有效部分；namelen参数太小； lpCalleeData、 lpSQOS和lpGQOS的缓冲区太小；或者lpCallerData的缓冲区太大。

WSAEINVAL套接口已与一个地址捆绑。

WSAEINVAL套接口未与一个地址捆绑。

WSAEINVALs参数为监听套接口。

WSAEISCONN套接口已经连接（仅适用于面向连接的套接口）。

WSAENETUNREACH当前无法从本主机联系网络。

WSAENOBUFS无可用缓冲区，套接口未连接。

WSAENOTSOCK描述字不是一个套接口。

WSAEOPNOTSUPPlpSQOS和lpGQOS中的流描述无法满足。

WSAEPROTONOSUPPORT服务提供者不支持lpCallerData参数。

WSAETIMEDOUT连接试图超时，连接未建立。

WSAEWOULDBLOCK套接口标志为非阻塞，连接无法立即完成。当套接口用select()函数设置为读时，可调用select()。

WSAEACCES由于setsockopt()时未允许SO_BROADCAST，无法将一个数据报套接口与一个广播地址连接。

另请参阅：accept(), bind(), connect(), getsockname(),getsockopt(), socket(), select(),

WSAAsyncSelect(), WSAEventSelect().



## 4 WSACreateEvent()

简述：创建一个新的事件对象。

#include <winsock2.h>

WSAEVENT WSAAPI WSACreateEvent( VOID );

返回值：

如果函数成功，则返回值即是事件对象的句柄。

如果函数失败，返回WSA_INVALID_EVENT。应用程序可通过调用WSAGetLastError()函数获取进一步的错误信息。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSA_NOT_ENOUGH_MEMORY 无足够内存创建事件对象。

另请参阅：WSACloseEvent(), WSAEnumNetworkEvents(),WSAEventSelect(), WSAGetOverlappedResult(),WSARecv(), WSARecvFrom(), WSAResetEvent(),WSASend(), WSASendTo(), WSASetEvent(),WSAWaitForMultipleEvents().



## 5 WSADuplicateSocket()

简述：为一个共享套接口创建一个新的描述字。

#include <winsock2.h>

SOCKET WSAAPI WSADuplicateSocket ( SOCKET s,

WSATASK hTargetTask );

s：指定本地套接口描述字。

hTargetTask：指定使用共享套接口的目标任务的句柄。

返回值：

若无错误发生，WSADuplicateSocket()返回新的套接口描述字。否则的话，将返回INVALID_SOCKET错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEINVAL参数中有非法值。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAEMFILE无可用套接口描述字。

WSAENOBUFS无可用缓冲区空间，套接口未创建。

WSAENOTSOCK描述字不是一个套接口。

另请参阅：



## 6 WSAEnumNetworkEvents()

简述：检测所指定套接口上网络事件的发生。

#include <winsock2.h>

int WSAAPI WSAEnumNetworkEvents ( SOCKET s,

WSAEVENT hEventObject, LPWSANETWORKEVENTS

lpNetworkEvents,LPINT lpiCount);

s：标识套接口的描述字。

hEventObject：（可选）句柄，用于标识需要复位的相应事件对象。

lpNetworkEvents：一个WSANETWORKEVENTS结构的数组，每一个元素记录了一个网络事件和相应的错误代码。

lpiCount：数组中的元素数目。在返回时，本参数表示数组中的实际元素数目；如果返回值是WSAENOBUFS，则表示为获取所有网络事件所需的元素数目。

返回值：

如果操作成功则返回0。否则的话，将返回INVALID_SOCKET错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEINVAL参数中有非法值。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAENOBUFS所提供的缓冲区太小。

另请参阅：WSAEventSelect()



## 7 WSAEnumProtocols()

简述：获取现有传送协议的相关信息。

#include <winsock2.h>

int WSAAPI WSAEnumProtocols ( LPDWORD

lpdwProtocols, LPVOID lpProtocolBuffer, LPDWORD

lpdwBufferLength);

lpdwProtocols：一个以NULL结尾的协议标识号数组。本参数可选；如果lpdwProtocols为 NULL，则返回所有可用协议的信息，否则的话只返回数组中所开列的协议信息。

lpProtocolBuffer：一个用PROTOCOL_INFO结构填充的缓冲区。参见下文中对PROTOCOL_INFO结构的具体描述。

lpdwBufferLength：输入时，存有传递给WSAEnumProtocols()函数的lpProtocolBuffer缓冲区长度。输出时，表示为获取所有信息需传递给WSAEnumProtocols()函数的缓冲区长度。本函数不能重复调用；传入的缓冲区必须足够大以能存放所有的元素。这个规定降低了该函数的复杂度。由于一个机器上装载的协议数目往往是很小的，所以并不会产生问题。

返回值：

若无错误发生，WSAEnumProtocols()返回协议的数目。否则的话，将返回INVALID_SOCKET错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEINPROGRESS一个阻塞WinSock调用正在进行。

WSAEINVAL参数中有非法值。

WSAENOBUFS缓冲区太小，无法保存所有PROTOCOL_INFO结构及其相关信息。传入的缓冲区大小至少应等于lpdwBufferLength中返回的值。



## 8 WSAEventSelect()

简述：确定与所提供的FD_XXX网络事件集合相关的一个事件对象。

#include <winsock2.h>

int WSAAPI WSAEventSelect ( SOCKET s, WSAEVENT

hEventObject, long lNetworkEvents );


s：一个标识套接口的描述字。

hEventObject：一个句柄，用于标识与所提供的FD_XXX网络事件集合相关的一个事件对象。

lNetworkEvents：一个屏蔽位，用于指定感兴趣的FD_XXX网络事件组合。

返回值：

如果应用程序指定的网络事件及其相应的事件对象成功设置，则返回0。否则的话，将返回INVALID_SOCKET错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。

在使用select()和WSAAsyncSelect()函数时，WSAEventSelect()常用来决定何时进行数据传送操作（如send()或recv()），并期望能立即成功。但是一个稳定的应用程序应该做好这样的准备，即事件对象被设置，并且一个WinSock调用以WSAEWOULDBLOCK立即返回。举例来说，有可能发生下述操作序列：

(i) 套接口s上到达数据；WinSock设置了WSAEventSelect事件对象。

(ii) 应用程序进行其他操作。

(iii) 在进行操作时，应用程序调用了ioctlsocket(s, FIONREAD...)并发现有数据可读。

(iv) 应用程序调用一个recv(s,...)来读取数据。

(v) 最后应用程序等待WSAEventSelect()所指定的数据对象，该数据对象指出数据可读。

(vi) 应用程序调用recv(s,...)，但以WSAEWOULDBLOCK错误失败。

其他的操作序列也是可能的。

成功地记录了网络事件的发生（通过设置内部网络事件记录的相应位），并且将相应的事件对象设置了信号后，不会对该网络事件作进一步的操作，直到应用程序调用了相应的函数显式地重新允许该网络事件及相应事件对象的信号。

网络事件重新允许函数

FD_READrecv() 或 recvfrom()

FD_WRITEsend() 或 sendto()

FD_OOBrecv()

FD_ACCEPTaccept() 或WSAAccept()，直到返回的错误代码为 WSATRY_AGAIN，指明条件函数返回CF_DEFER。

FD_CONNECTNONE

FD_CLOSENONE

FD_QOS用SIO_GET_QOS 命令调用WSAIoctl()。

FD_GROUP_QOS用SIO_GET_GROUP_QOS命令调用WSAIoctl()。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEINVAL参数中有非法值，或者指定的套接口处于非法状态。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAENOTSOCK描述字不是一个套接口。

另请参阅：WSACloseEvent() ,WSACreateEvent(),WSAEnumNetworkEvents(),WSAGetOverlappedResult(),WSAWaitForMultipleEvents().



## 9 WSAGetOverlappedResult()

简述：返回指定套接口上一个重叠操作的结果。

#include <winsock2.h>

BOOL WSAAPI WSAGetOverlappedResult( SOCKET s,

LPWSAOVERLAPPED lpOverlapped, LPDWORD

lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags );

s：标识套接口。这就是调用重叠操作（WSARecv()、 WSARecvFrom()、WSASend()、WSASendTo() 或 WSAIoctl()）时指定的那个套接口。

lpOverlapped：指向调用重叠操作时指定的WSAOVERLAPPED结构。

lpcbTransfer：指向一个32位变量，该变量用于存放一个发送或接收操作实际传送的字节数，或WSAIoctl()传送的字节数。

fWait：指定函数是否等待挂起的重叠操作结束。若为真TRUE则函数在操作完成后才返回。若为假FALSE且函数挂起，则函数返回FALSE，WSAGetLastError()函数返回WSA_IO_INCOMPLETE。

lpdwFlags：指向一个32位变量，该变量存放完成状态的附加标志位。如果重叠操作为WSARecv()或WSARecvFrom()，则本参数包含lpFlags参数所需的结果。

返回值：

如果函数成功，则返回值为真TRUE。它意味着重叠操作已经完成，lpcbTransfer所指向的值已经被刷新。应用程序可调用WSAGetLastError()来获取重叠操作的错误信息。

如果函数失败，则返回值为假FALSE。它意味着要么重叠操作未完成，要么由于一个或多个参数的错误导致无法决定完成状态。失败时，lpcbTransfer指向的值不会被刷新。应用程序可用WSAGetLastError()来获取失败的原因。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAENOTSOCK 描述字不是一个套接口。

WSA_INVALID_HANDLEWSAOVERLAPPED结构的hEvent域未包含一个有效的事件对象句柄。

WSA_INVALID_PARAMETER有不可接受的参数。

WSA_IO_INCOMPLETEfWait假FALSE且输入/输出操作尚未完成。

另请参阅：WSACreateEvent(), WSAWaitForMultipleEvents(),WSARecv(), WSARecvFrom(), WSASend(), WSASendTo(),WSAConnect(), WSAAccept(), WSAIoctl().



## 10 WSAGetQoSByName()

简述：根据一个模板初始化QOS。

#include <winsock2.h>

BOOL WSAAPI WSAGetQOSByName( SOCKET s, LPWSABUF

lpQOSName, LPQOS lpQOS);

s：一个标识套接口的描述字。

lpQOSName：指定QOS模板的名字。

lpQOS：指向待填充QOS结构的指针。

返回值：

如果函数成功，返回真TRUE。如果函数失败，返回假FALSE。可调用WSAGetLastError()来获取进一步的错误信息。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAENOTSOCK描述字不是一个套接口。

WSAEFAULTlpQOS参数不是用户地址空间的一个有效部分，或lpQOS的缓冲区太小。

WSA_INVAL所指定的QOS模板名字非法。

另请参阅：WSAConnect(), WSAAccept(), getsockopt().



## 11 WSAHtonl()

简述：将一个以主机字节顺序表示的无符号长整形数转换为网络字节顺序。

#include <winsock2.h>

u_long WSAAPI WSAHtonl ( SOCKETs, u_long

hostlong );

s：一个标识套接口的描述字。

hostlong：一个用主机字节顺序表示的32位数。

返回值：WSAHtonl()返回以网络字节顺序表示的值。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAENOTSOCK描述字不是一个套接口。

另请参阅：htonl(), htons(), ntohs(), ntohl(), WSAHtons(),WSANtohl(), WSANtohs().



## 12 WSAHtons()

简述：将一个以主机字节顺序表示的无符号短整形数转换为网络字节顺序。

#include <winsock2.h>

u_short WSAAPI WSAHtons (SOCKET sr, u_short

hostshort );

s：一个标识套接口的描述字。

hostshort：一个以主机字节顺序表示的16位数。

返回值：WSAHtons()返回以网络字节顺序表示的值。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN 网络子系统失效。

WSAENOTSOCK 描述字不是一个套接口。

另请参阅：htonl(), htons(), ntohs(), ntohl(), WSAHtonl(),WSANtohl(), WSANtohs().



## 13 WSAIoctl()

简述：控制一个套接口的模式。

#include <winsock2.h>

int WSAAPI WSAIoctl(SOCKET s, DWORD

dwIoControlCode, LPVOID lpvInBuffer, DWORD

cbInBuffer, LPVOID lpvOutBuffer, DWORD

cbOutBuffer, LPDWORD lpcbBytesReturned,

LPWSAOVERLAPPED lpOverlapped,

LPWSAOVERLAPPED_COMPLETION_ROUTINE

lpCompletionRoutine);

s：一个套接口的句柄。

dwIoControlCode：将进行的操作的控制代码。

lpvInBuffer：输入缓冲区的地址。

cbInBuffer：输入缓冲区的大小。

lpvOutBuffer：输出缓冲区的地址。

cbOutBuffer：输出缓冲区的大小。

lpcbBytesReturned：输出实际字节数的地址。

lpOverlapped：WSAOVERLAPPED结构的地址。

lpCompletionRoutine：一个指向操作结束后调用的例程指针。

返回值：

调用成功后，WSAIoctl ()函数返回0。否则的话，将返回INVALID_SOCKET错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEINVALcmd不是一个合法的命令；或者一个输入参数非法；或者命令对于该种类型的套接口不适用。

WSAEINPROGRESS在一个回调函数运行时调用了该函数。

WSAENOTSOCK描述字不是一个套接口。

WSAEOPNOTSUPP指定的ioctl命令无法实现，例如在SIO_SET_QOS或SIO_SET_GROUP_QOS中指定的流描述无法实现。

WSA_IO_PENDING一个重叠操作被成功启动，过后将报告完成情况。

WSAEWOULDBLOCK套接口标志为非阻塞，且所需操作将产生阻塞。

另请参阅：socket(), ioctlsocket(), WSASocket(),setsockopt(), getsockopt().

## 14 WSAJoinLeaf()

简述：将一个叶节点加入一个多点会晤，交换连接数据，根据提供的流描述确定所需的服务质量。

#include <winsock2.h>

SOCKET WSAAPI WSAJoinLeaf ( SOCKET s, const struct

sockaddr FAR * name, int namelen, LPWSABUF

lpCallerData, LPWSABUF lpCalleeData,

LPQOS lpSQOS, LPQOS lpGQOS, int iFlags );


s：标识一个多点套接口的描述字。

name：将与套接口连接的远端名字。

namelen：名字长度。

lpCallerData：一个指针，指向多点会晤创建时传送给远端的用户数据。

lpCalleeData：一个指针，指向多点会晤创建时从远端传送回来的用户数据。

lpSQOS：一个指向套接口s的流描述的指针，每个方向一个。

lpGQOS：一个指向套接口组（如果存在）流描述的指针。

iFlags：标志位，用于指定套接口作为发送者。接收者或身兼二者。

返回值：若无错误发生，WSAJoinLeaf()返回新创建的多点套接口的描述字。否则的话，将返回INVALID_SOCKET错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEADDRINUSE指定的地址已经在使用中。

WSAEINTR通过WSACancelBlockingCall()函数取消（阻塞）调用。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAEALREADY在指定的套接口上正在运行一个非阻塞的WSAJoinLeaf()调用。

WSAEADDRNOTAVAIL本地主机无法获得指定的地址。

WSAEAFNOSUPPORT所指定地址族中的地址无法与本套接口一起使用。

WSAECONNREFUSED加入试图被强制拒绝。

WSAEFAULTname或namelen参数不是用户地址空间的一个有效部分；namelen参数太小；lpCalleeData、 lpSQOS和lpGQOS的缓冲区太小； lpCallerData缓冲区太大。

WSAEINVAL套接口已与一个地址捆绑。

WSAEINVAL套接口未与一个地址捆绑。

WSAEISCONN套接口已是多点会晤的一个成员。

WSAENETUNREACH当前无法从本主机联系网络。

WSAENOBUFS无可用缓冲区空间。套接口无法加入。

WSAENOTSOCK描述字不是一个套接口。

WSAEOPNOTSUPP lpSQOS和lpGQOS中所指定的流描述无法满足。

WSAEPROTONOSUPPORT服务提供者不支持lpCallerData参数。

WSAETIMEDOUT加入试图超时，未建立多点会晤。

WSAEWOULDBLOCK套接口被标志为非阻塞，但多点会晤加入操作无法立即完成。当用select()选为读连接后，可使用select()对套接口进行操作。

另请参阅：accept(), bind(), select(), WSAAccept(),WSAAsyncSelect(), WSAEventSelect(), WSASocket().



## 15 WSANtohl()

简述：将一个以网络字节顺序表示的无符号长整形数转换为主机字节顺序。

#include <winsock2.h>

u_long WSAAPI WSANtohl ( SOCKET s, u_long netlong

);

s：一个标识套接口的描述字。

netlong：一个以网络字节顺序表示的32位数。

返回值：WSANtohl()返回一个以主机字节顺序表示的值。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAENOTSOCK描述字不是一个套接口。

另请参阅：ntohl(), htonl(), htons(), ntohs(), WSAHtonl(),WSAHtons(), WSANtohs().



## 16 WSANtohs()

简述：将一个以网络字节顺序表示的无符号短整形数转换为主机字节顺序。

#include <winsock2.h>

u_short WSAAPI WSANtohs (SOCKET s, u_short

netshort );

s：一个标识套接口的描述字。

netshort：一个以网络字节顺序标识的16位数。

返回值：WSANtohs()返回以主机字节顺序表示的值。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAENOTSOCK描述字不是一个套接口。

另请参阅：htonl(), htons(), ntohs(), ntohl(), WSAHtonl(),WSAHtons(), WSANtohl().



## 17 WSARecv()

简述：从一个套接口接收数据。

#include <winsock2.h>

int WSAAPI WSARecv ( SOCKET s, LPWSABUF lpBuffers,

DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd,

LPINT lpFlags, LPWSAOVERLAPPED lpOverlapped,

LPWSAOVERLAPPED_COMPLETION_ROUTINE

lpCompletionRoutine );


s：一个标识已连接套接口的描述字。

lpBuffers：一个指向WSABUF结构数组的指针。每一个WSABUF结构包含一个缓冲区的指针和缓冲区的长度。

dwBufferCount：lpBuffers数组中WSABUF结构的数目。

lpNumberOfBytesRecvd：如果接收操作立即结束，一个指向本调用所接收的字节数的指针。

lpFlags：一个指向标志位的指针。

lpOverlapped：一个指向WSAOVERLAPPED结构的指针（对于非重叠套接口则忽略）。

lpCompletionRoutine：一个指向接收操作结束后调用的例程的指针（对于非重叠套接口则忽略）。

返回值：

若无错误发生且接收操作立即完成，则WSARecv()函数返回所接收的字节数。如果连接结束，则返回0。请注意在这种情况下完成指示（启动指定的完成例程或设置一个事件对象）将早已发生。否则的话，将返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。错误代码WSA_IO_PENDING表示重叠操作成功启动，过后将有完成指示。任何其他的错误表示重叠操作未能成功地启动，以后也不会有完成指示。

如果设置了MSG_INTERRUPT标志，则返回值的含义变化。零表示成功，具体含义同上。否则的话，返回值直接包含如下所示的错误代码。由于中断环境中无法调用WSAGetLastError()，故是必需的。请注意仅适用于Win16环境，仅适用于PROTOCOL_INFO结构中设置了XP1_INTERRUPT位的协议。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAENOTCONN套接口未连接。

WSAEINTR通过WSACancelBlockingCall()函数取消（阻塞）调用。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAENETRESET由于远端的复位造成连接的中止。

WSAENOTSOCK 描述字不是一个套接口。

WSAEOPNOTSUPP设置了MSG_OOB，但是该套接口不是诸如SOCK_STREAM流类型的，与套接口相关的通讯域不支持带外数据，或者套接口是单向的，只支持发送操作。

WSAESHUTDOWN套接口已经关闭；一个套接口以SD_RECEIVE或SD_BOTH的how参数shutdown()后，无法进行WSARecv()调用。

WSAEWOULDBLOCK重叠套接口：太多重叠的输入/输出请求。非重叠套接口：套接口被标志为非阻塞，但是操作不能立即完成。

WSAEINVAL套接口未用bind()捆绑，或者套接口未用重叠标志创建。

WSAECONNABORTED由于超时或其他错误导致虚电路中止。

WSAECONNRESET虚电路被远端复位。

WSAEDISCON远端优雅的结束了连接。

WSA_IO_PENDING成功启动一个重叠操作，过后将有完成指示。

另请参阅：WSACloseEvent(),WSACreateEvent(),WSAGetOverlappedResult(), WSASocket(),WSAWaitForMultipleEvents()



## 18 WSARecvDisconnect()

简述：中止一个套接口上的接收操作；若套接口为面向连接的，则检索中止连接数据。

#include <winsock2.h>

int WSAAPI WSARecvDisconnect ( SOCKET s, LPWSABUF

lpInboundDisconnectData );

s：一个标识套接口的描述字。

lpInboundDisconnectData：一个指向前来的中止连接数据的中止。

返回值：若无错误发生，WSARecvDisconnect()返回0。否则的话，返回SOCKET_ERROR，应用程序可通过调用WSAGetLastError()函数来获取相应的错误代码。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEFAULTlpInboundDisconnectData参数所提供的缓冲区太小。

WSAENOPROTOOPT指定地址族不支持中止连接数据。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAENOTCONN套接口未连接（仅适用于面向连接的套接口）。

WSAENOTSOCK描述字不是一个套接口。

另请参阅：connect(), socket().

## 19 WSARecvFrom()

简述：接收一个数据报并保存源地址。

#include <winsock2.h>1

int WSAAPI WSARecvFrom ( SOCKET s, LPWSABUF

lpBuffers, DWORD dwBufferCount, LPDWORD

lpNumberOfBytesRecvd, LPINT lpFlags, LPVOID

lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED

lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE

lpCompletionRoutine );

s：一个标识套接口的描述字。

lpBuffers：一个指向WSABUF结构数组的指针。每个WSABUF结构包含缓冲区的指针和缓冲区的大小。

dwBufferCount：lpBuffers数组中WSABUF结构的数目。

lpNumberOfBytesRecvd：如果接收操作立即完成，则为一个指向所接收数据字节数的指针。

lpFlags：一个指向标志位的指针。

lpFrom：（可选）指针，指向重叠操作完成后存放源地址的缓冲区。

lpFromlen：指向from缓冲区大小的指针，仅当指定了lpFrom才需要。

lpOverlapped：指向WSAOVERLAPPED结构的指针（对于非重叠套接口则忽略）。

lpCompletionRoutine：一个指向接收操作完成后调用的完成例程的指针。（对于非重叠套接口则忽略）。

返回值：

若无错误发生且接收操作立即完成，则WSARecvFrom()函数返回所接收的字节数。如果连接结束，则返回0。请注意在这种情况下完成指示（启动指定的完成例程或设置一个事件对象）将早已发生。否则的话，将返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。错误代码WSA_IO_PENDING表示重叠操作成功启动，过后将有完成指示。任何其他的错误表示重叠操作未能成功地启动，以后也不会有完成指示。

如果设置了MSG_INTERRUPT标志，则返回值的含义变化。零表示成功，具体含义同上。否则的话，返回值直接包含如下所示的错误代码。由于中断环境中无法调用WSAGetLastError()，故是必需的。请注意仅适用于Win16环境，仅适用于PROTOCOL_INFO结构中设置了XP1_INTERRUPT位的协议。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEFAULTlpFromlen参数非法；lpFrom缓冲区太小，无法容纳远端地址。

WSAEINTR通过WSACancelBlockingCall()函数取消（阻塞）调用。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAEINVAL套接口未用bind()捆绑，或者套接口未用重叠标志创建。

WSAENETRESET由于远端的复位造成连接的中止。

WSAENOTCONN套接口未连接。（仅适用于面向连接的套接口）。

WSAENOTSOCK描述字不是一个套接口。

WSAEOPNOTSUPP设置了MSG_OOB，但是该套接口不是诸如SOCK_STREAM流类型的，与套接口相关的通讯域不支持带外数据，或者套接口是单向的，只支持发送操作。

WSAESHUTDOWN套接口已经关闭；一个套接口以SD_RECEIVE或SD_BOTH的how参数shutdown()后，无法进行WSARecvFrom()调用。

WSAEWOULDBLOCK重叠套接口：太多重叠的输入/输出请求。非重叠套接口：套接口被标志为非阻塞，但是操作不能立即完成。

WSAEMSGSIZE消息太大无法全部装入指定的缓冲区，故被修剪。

WSAECONNABORTED由于超时或其他错误导致虚电路中止。

WSAECONNRESET虚电路被远端复位。

WSAEDISCON远端优雅地中止了连接。

WSA_IO_PENDING成功启动一个重叠操作，过后将有完成指示。

另请参阅：WSACloseEvent(),WSACreateEvent(),WSAGetOverlappedResult(), WSASocket(),WSAWaitForMultipleEvents()

## 20 WSAResetEvent()

简述：将指定的事件对象状态清除为未置信号。

#include <winsock2.h>

BOOL WSAAPI WSAResetEvent( WSAEVENT hEvent );

hEvent：标识一个开放的事件对象句柄。

返回值：

如果函数成功，返回真TRUE。如果函数失败，返回假FALSE。可调用WSAGetLastError()来获取进一步的错误信息。

错误代码：

WSA_INVALID_HANDLEhEvent不是一个合法的事件对象句柄。

另请参阅：WSACreateEvent(), WSASetEvent(), WSACloseEvent().



## 21 WSASend()

简述：在一个已连接的套接口上发送数据。

#include <winsock2.h>

int WSAAPI WSASend ( SOCKET s, LPWSABUF lpBuffers,

DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent,

int iFlags, LPWSAOVERLAPPED lpOverlapped,

LPWSAOVERLAPPED_COMPLETION_ROUTINE

lpCompletionRoutine );

s：标识一个已连接套接口的描述字。

lpBuffers：一个指向WSABUF结构数组的指针。每个WSABUF结构包含缓冲区的指针和缓冲区的大小。

dwBufferCount：lpBuffers数组中WSABUF结构的数目。

lpNumberOfBytesSent：如果发送操作立即完成，则为一个指向所发送数据字节数的指针。

iFlags：标志位。

lpOverlapped：指向WSAOVERLAPPED结构的指针（对于非重叠套接口则忽略）。

lpCompletionRoutine：一个指向发送操作完成后调用的完成例程的指针。（对于非重叠套接口则忽略）。

返回值：

若无错误发生且发送操作立即完成，则WSASend()函数返回所发送的字节数。（注意该数目可能小于len参数所指定的值）。如果连接结束，则返回0。请注意在这种情况下完成指示（启动指定的完成例程或设置一个事件对象）将早已发生。否则的话，将返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。错误代码WSA_IO_PENDING表示重叠操作成功启动，过后将有完成指示。任何其他的错误表示重叠操作未能成功地启动，以后也不会有完成指示。

如果设置了MSG_INTERRUPT标志，则返回值的含义变化。零表示成功，具体含义同上。否则的话，返回值直接包含如下所示的错误代码。由于中断环境中无法调用WSAGetLastError()，故是必需的。请注意仅适用于Win16环境，仅适用于PROTOCOL_INFO结构中设置了XP1_INTERRUPT位的协议。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEACCES请求地址为广播地址，但相应标志位没有设置。

WSAEINTR 通过WSACancelBlockingCall()函数取消（阻塞）调用。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAEFAULTlpBuffer参数并不是用户地址空间的一个有效部分。

WSAENETRESET由于远端复位造成连接的中止。

WSAENOBUFSWinSock提供者报告一个缓冲区死锁。

WSAENOTCONN套接口未连接。

WSAENOTSOCK描述字不是一个套接口。

WSAEOPNOTSUPP设置了MSG_OOB，但是该套接口不是诸如SOCK_STREAM流类型的，与套接口相关的通讯域不支持带外数据，或者套接口是单向的，只支持接收操作。

WSAESHUTDOWN套接口已经关闭；一个套接口以SD_SEND或SD _BOTH的how参数shutdown()后，无法进行WSASend()调用。

WSAEWOULDBLOCK太多的重叠输入/输出操作。

WSAEMSGSIZE套接口是面向消息的，但是消息大于底层传送的最大值。

WSAEINVAL套接口未用bind()捆绑，或者套接口未用重叠标志位创建。

WSAECONNABORTED由于超时或其他错误导致虚电路中止。

WSAECONNRESET虚电路被远端复位。

WSA_IO_PENDING成功启动重叠操作，过后将有完成指示。

另请参阅：WSACloseEvent(),WSACreateEvent(),WSAGetOverlappedResult(), WSASocket(),WSAWaitForMultipleEvents()



## 22 WSASendDisconnect()

简述：启动套接口连接的中止操作。

#include <winsock2.h>

int WSAAPI WSASendDisconnect ( SOCKET s, LPWSABUF

lpOutboundDisconnectData );

s：一个标识套接口的描述字。

lpOutboundDisconnectData：指向发出的中止连接数据的指针。

返回值：若无错误发生，WSASendDisconnect()返回0。否则的话，将返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN 网络子系统失效。

WSAENOPROTOOPTlpOutboundDisconnectData参数非NULL，复位提供者不支持中止连接数据。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAENOTCONN套接口未连接（仅适用于面向连接的套接口）。

WSAENOTSOCK描述字不是一个套接口。

另请参阅：connect(), socket().



## 23 WSASendTo()

简述：向指定地址发送数据，可能的话使用重叠输入/输出操作。

#include <winsock2.h>

int WSAAPI WSASendTo ( SOCKET s, LPWSABUF

lpBuffers, DWORD dwBufferCount, LPDWORD

lpNumberOfBytesSent, int iFlags, LPVOID lpTo, int

iToLen, LPWSAOVERLAPPED lpOverlapped,

LPWSAOVERLAPPED_COMPLETION_ROUTINE

lpCompletionRoutine );


s：用于标识一个已连接的套接口，该套接口以WSA_FLAG_OVERLAPPED标志调用WSASocket()创建。

lpBuffers：一个指向WSABUF结构数组的指针。每个WSABUF结构包含缓冲区的指针和缓冲区的大小。

dwBufferCount：lpBuffers数组中WSABUF结构的数目。

lpNumberOfBytesSent：如果发送操作立即完成，则为一个指向所发送数据字节数的指针。

iFlags：标志位。

lpTo：（可选）指针，指向目标套接口的地址。

lpTolen：lpTo中地址的大小。

lpOverlapped：指向WSAOVERLAPPED结构的指针（对于非重叠套接口则忽略）。

lpCompletionRoutine：一个指向发送操作完成后调用的完成例程的指针。（对于非重叠套接口则忽略）。

返回值：

若无错误发生且发送操作立即完成，则WSASendTo()函数返回所发送的字节数（请注意它可能小于len所指定的值）。请注意在这种情况下完成指示（启动指定的完成例程或设置一个事件对象）将早已发生。否则的话，将返回SOCKET_ERROR错误，应用程序可通过WSAGetLastError()来获取相应的错误代码。错误代码WSA_IO_PENDING表示重叠操作成功启动，过后将有完成指示。任何其他的错误表示重叠操作未能成功地启动，以后也不会有完成指示。

如果设置了MSG_INTERRUPT标志，则返回值的含义变化。零表示成功，具体含义同上。否则的话，返回值直接包含如下所示的错误代码。由于中断环境中无法调用WSAGetLastError()，故是必需的。请注意仅适用于Win16环境，仅适用于PROTOCOL_INFO结构中设置了XP1_INTERRUPT位的协议。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSAEACCES请求的地址为广播地址，但未设置相应的标志位。

WSAEINTR 通过WSACancelBlockingCall()函数取消（阻塞）调用。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAEFAULTlpBuffer或lpTo参数不是用户地址空间的一部分；或者lpTo参数太小（小于sockaddr结构的大小）。

WSAENETRESET远端主机复位造成连接的中止。

WSAENOBUFSWinSock提供者报告了一个缓冲区死锁。

WSAENOTCONN套接口未连接（仅适用于面向连接的套接口）。

WSAENOTSOCK描述字不是一个套接口。

WSAEOPNOTSUPP设置了MSG_OOB，但是该套接口不是诸如SOCK_STREAM流类型的，与套接口相关的通讯域不支持带外数据，或者套接口是单向的，只支持接收操作。

WSAESHUTDOWN套接口已经关闭；一个套接口以SD_SEND或SD _BOTH的how参数shutdown()后，无法进行WSASendTo()调用。

WSAEWOULDBLOCK太多重叠的输入/输出请求。

WSAEMSGSIZE套接口是面向消息的，且消息大于底层传送所支持的最大长度。

WSAEINVAL套接口未用bind()捆绑，或者套接口未用重叠标志位创建。

WSAECONNABORTED由于超时或其他错误导致虚电路中止。

WSAECONNRESET虚电路被远端复位。

WSAEADDRNOTAVAIL本地主机无法获取所指定的地址。

WSAEAFNOSUPPORT指定地址族中的地址无法与本套接口一起使用。

WSAEDESTADDRREQ需要目的地地址。

WSAENETUNREACH当前无法从本主机联系网络。

WSA_IO_PENDING成功启动一个重叠操作，过后将有完成指示。

另请参阅：WSACloseEvent(),WSACreateEvent(),WSAGetOverlappedResult(), WSASocket(),WSAWaitForMultipleEvents()



## 24 WSASetEvent()

简述：将指定的事件对象状态设置为有信号。

#include <winsock2.h>

BOOL WSAAPI WSASetEvent( WSAEVENT hEvent );

hEvent：标识一个开放的事件对象句柄。

返回值：

如果函数成功，返回真TRUE。

如果函数失败，返回假FALSE。可通过调用WSAGetLastError()来获取进一步的错误信息。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSA_INVALID_HANDLEhEvent不是一个合法的事件对象句柄。

另请参阅：WSACreateEvent(), WSAResetEvent(),WSACloseEvent().



## 25 WSASocket()

简述：创建一个与指定传送服务提供者捆绑的套接口，可选地创建和/或加入一个套接口组。

#include <winsock2.h>

SOCKET WSAAPI WSASocket ( int af, int type, int

protocol, LPPROTOCOL_INFO lpProtocolInfo, Group g,

int iFlags);


af：地址族描述。目前仅支持PF_INET格式，亦即ARPA Internet地址格式。

type：新套接口的类型描述。

protocol：套接口使用的特定协议，如果调用者不愿指定协议则定为0。

lpProtocolInfo：一个指向PROTOCOL_INFO结构的指针，该结构定义所创建套接口的特性。如果本参数非零，则前三个参数（af, type, protocol）被忽略。

g：套接口组的描述字。

iFlags：套接口属性描述。

返回值：

若无错误发生，WSASocket()返回新套接口的描述字。否则的话，返回 INVALID_SOCKET，应用程序可定调用WSAGetLastError()来获取相应的错误代码。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN 网络子系统失效。

WSAEAFNOSUPPORT不支持指定的地址族。

WSAEINPROGRESS一个阻塞的WinSock调用正在进行中，或者服务提供者仍在处理一个回调函数。（参见B.3.6.6节）

WSAEMFILE无可用的套接口描述字。

WSAENOBUFS无可用的缓冲区空间。套接口无法创建。

WSAEPROTONOSUPPORT不支持指定的协议。

WSAEPROTOTYPE指定的协议对于本套接口类型错误。

WSAESOCKTNOSUPPORT本地址族不支持指定的套接口类型。

WSAEINVALg参数非法。

另请参阅：accept(), bind(), connect(), getsockname(),getsockopt(), setsockopt(), listen(), recv(),recvfrom(), select(), send(), sendto(),shutdown(), ioctlsocket().



## 26 WSAWaitForMultipleEvents()

简述：只要指定事件对象中的一个或全部处于有信号状态，或者超时间隔到，则返回。

#include <winsock2.h>

DWORD WSAAPI WSAWaitForMultipleEvents( DWORD

cEvents,

const WSAEVENT FAR * lphEvents, BOOL fWaitAll,

DWORD dwTimeout,

BOOL fAlertable );

cEvents：指出lphEvents所指数组中事件对象句柄的数目。事件对象句柄的最大值为WSA_MAXIMUM_WAIT_EVENTS。

lphEvents：指向一个事件对象句柄数组的指针。

fWaitAll：指定等待类型。若为真TRUE，则当lphEvents数组中的所有事件对象同时有信号时，函数返回。若为假FALSE，则当任意一个事件对象有信号时函数即返回。在后一种情况下，返回值指出是哪一个事件对象造成函数返回。

dwTimeout：指定超时时间间隔（以毫秒计）。当超时间隔到，函数即返回，不论fWaitAll参数所指定的条件是否满足。如果dwTimeout为零，则函数测试指定的时间对象的状态，并立即返回。如果dwTimeout是WSA_INFINITE，则函数的超时间隔永远不会到。

fAlertable：指定当系统将一个输入/输出完成例程放入队列以供执行时，函数是否返回。若为真TRUE，则函数返回且执行完成例程。若为假FALSE，函数不返回，不执行完成例程。请注意在Win16中忽略该参数。

返回值：

如果函数成功，返回值指出造成函数返回的事件对象。

如果函数失败，返回值为WSA_WAIT_FAILED。可调用WSAGetLastError()来获取进一步的错误信息。

错误代码：

WSANOTINITIALISED在调用本API之前应成功调用WSAStartup()。

WSAENETDOWN网络子系统失效。

WSA_NOT_ENOUGH_MEMORY无足够内存完成该操作。

WSA_INVALID_HANDLElphEvents数组中的一个或多个值不是合法的事件对象句柄。

WSA_INVALID_PARAMETERcEvents参数未包含合法的句柄数目。

另请参阅：WSACreateEvent(), WSACloseEvent().


