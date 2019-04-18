# Winsock参考：Winsock函数 - 一世豁然的专栏 - CSDN博客





2018年10月15日 16:08:10[一世豁然](https://me.csdn.net/Explorer_day)阅读数：155








以下列表提供了每个Winsock函数的简明描述。 有关任何函数的其他信息，请单击函数名称。
|函数|描述|
|----|----|
|[**accept**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-accept)|允许在套接字上进行传入连接尝试。|
|[**AcceptEx**](https://msdn.microsoft.com/en-us/library/ms737524(v=VS.85).aspx)|接受新连接，返回本地和远程地址，并接收客户端应用程序发送的第一个数据块。|
|[**bind**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-bind)|将本地地址与套接字关联。|
|[**closesocket**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-closesocket)|关闭现有套接字。|
|[**connect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-connect)|建立与指定套接字的连接。|
|[**ConnectEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/nc-mswsock-lpfn_connectex)|建立与指定套接字的连接，并可选择在建立连接后发送数据。 仅在面向连接的套接字上受支持。|
|[**DisconnectEx**](https://msdn.microsoft.com/en-us/library/ms737757(v=VS.85).aspx)|关闭套接字上的连接，并允许重用套接字句柄。|
|[**EnumProtocols**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/nf-nspapi-enumprotocolsa)|检索有关在本地主机上处于活动状态的指定网络协议集的信息。|
|[**freeaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-freeaddrinfo)|检索有关在本地主机上处于活动状态的指定网络协议集的信息|
|[**FreeAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-freeaddrinfoex)|释放GetAddrInfoEx函数在addrinfoex结构中动态分配的地址信息。|
|[**FreeAddrInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-freeaddrinfow)|释放GetAddrInfoW函数在addrinfoW结构中动态分配的地址信息。|
|[**gai_strerror**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-gai_strerrora)|根据getaddrinfo函数返回的EAI_ *错误，帮助打印错误消息。|
|[**GetAcceptExSockaddrs**](https://msdn.microsoft.com/en-us/library/ms738516(v=VS.85).aspx)|解析从调用AcceptEx函数获得的数据。|
|[**GetAddressByName**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/nf-nspapi-getaddressbynamea)|查询命名空间或一组默认命名空间，以检索指定网络服务的网络地址信息。 此过程称为服务名称解析。 网络服务还可以使用该函数来获取可以与绑定功能一起使用的本地地址信息。|
|[**getaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfo)|提供从ANSI主机名到地址的协议无关转换。|
|[**GetAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexa)|提供与协议无关的名称解析以及其他参数，以限定哪些名称空间提供程序应处理请求。|
|[**GetAddrInfoExCancel**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexcancel)|通过GetAddrInfoExfunction取消异步操作。|
|[**GetAddrInfoExOverlappedResult**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexoverlappedresult)|获取GetAddrInfoEx函数的异步操作使用的OVERLAPPED结构的返回码。|
|[**GetAddrInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfow)|提供从Unicode主机名到地址的独立于协议的转换。|
|[**gethostbyaddr**](https://msdn.microsoft.com/en-us/library/ms738521(v=VS.85).aspx)|检索与网络地址对应的主机信息。|
|[**gethostbyname**](https://msdn.microsoft.com/en-us/library/ms738524(v=VS.85).aspx)|从主机数据库中检索与主机名对应的主机信息。 不推荐使用：改用getaddrinfo。|
|[**gethostname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-gethostname)|检索本地计算机的标准主机名。|
|[**GetHostNameW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-gethostnamew)|以Unicode字符串形式检索本地计算机的标准主机名。|
|[**getipv4sourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getipv4sourcefilter)|检索IPv4套接字的多播过滤器状态。|
|[**GetNameByType**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/nf-nspapi-getnamebytypea)|检索指定服务类型的网络服务的名称。|
|[**getnameinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getnameinfo)|提供从IPv4或IPv6地址到ANSI主机名的名称解析，以及从端口号到ANSI服务名称的名称解析。|
|[**GetNameInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getnameinfow)|提供从IPv4或IPv6地址到Unicode主机名以及从端口号到Unicode服务名称的名称解析。|
|[**getpeername**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getpeername)|检索套接字连接的对等体的地址。|
|[**getprotobyname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getprotobyname)|检索与协议名称对应的协议信息。|
|[**getprotobynumber**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getprotobynumber)|检索与协议号对应的协议信息。|
|[**getservbyname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getservbyname)|检索与服务名称和协议对应的服务信息。|
|[**getservbyport**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getservbyport)|检索与端口和协议对应的服务信息。|
|[**GetService**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/nf-nspapi-getservicea)|在一组默认命名空间或指定命名空间的上下文中检索有关网络服务的信息。|
|[**getsockname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getsockname)|检索套接字的本地名称。|
|[**getsockopt**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getsockopt)|检索套接字选项。|
|[**getsourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getsourcefilter)|检索IPv4或IPv6套接字的多播过滤器状态。|
|[**GetTypeByName**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/nf-nspapi-gettypebynamea)|检索由name指定的网络服务的服务类型GUID。|
|[**htond**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htond)|将double从主机转换为TCP / IP网络字节顺序（这是big-endian）。|
|[**htonf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htonf)|将浮动从主机转换为TCP / IP网络字节顺序（这是big-endian）。|
|[**htonl**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-htonl)|将u_long从主机转换为TCP / IP网络字节顺序（这是big-endian）。|
|[**htonll**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htonll)|将无符号__int64从主机转换为TCP / IP网络字节顺序（这是big-endian）。|
|[**htons**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-htons)|将u_short从主机转换为TCP / IP网络字节顺序（这是big-endian）。|
|[**inet_addr**](https://msdn.microsoft.com/en-us/library/ms738563(v=VS.85).aspx)|将包含（Ipv4）Internet协议虚线地址的字符串转换为in_addr结构的正确地址。|
|[**inet_ntoa**](https://msdn.microsoft.com/en-us/library/ms738564(v=VS.85).aspx)|将（IPv4）Internet网络地址转换为Internet标准点分格式的字符串。|
|[**InetNtop**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-inetntopw)|将IPv4或IPv6 Internet网络地址转换为Internet标准格式的字符串。 此函数的ANSI版本是inet_ntop。|
|[**InetPton**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-inetptonw)|将标准文本表示形式的IPv4或IPv6 Internet网络地址转换为其数字二进制形式。 此函数的ANSI版本为inet_pton。|
|[**ioctlsocket**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-ioctlsocket)|控制套接字的I / O模式。|
|[**listen**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-listen)|将套接字置于正在侦听传入连接的状态。|
|[**ntohd**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohd)|将无符号__int64从TCP / IP网络顺序转换为主机字节顺序（在Intel处理器上为little-endian）并返回double。|
|[**ntohf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohf)|将无符号__int32从TCP / IP网络顺序转换为主机字节顺序（在Intel处理器上为little-endian）并返回浮点数。|
|[**ntohl**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-ntohl)|将u_long从TCP / IP网络顺序转换为主机字节顺序（在Intel处理器上为little-endian）。|
|[**ntohll**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohll)|将无符号__int64从TCP / IP网络顺序转换为主机字节顺序（在Intel处理器上为little-endian）。|
|[**ntohs**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-ntohs)|将u_short从TCP / IP网络字节顺序转换为主机字节顺序（在Intel处理器上为little-endian）。|
|[**recv**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-recv)|从连接或绑定的套接字接收数据。|
|[**recvfrom**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-recvfrom)|接收数据报并存储源地址。|
|[**RIOCloseCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh448837(v=VS.85).aspx)|通过使用Winsock注册的I / O扩展发送和接收请求，关闭用于I / O完成通知的现有完成队列。|
|[**RIOCreateCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh448841(v=VS.85).aspx)|创建特定大小的I / O完成队列，以便与Winsock注册的I / O扩展一起使用。|
|[**RIOCreateRequestQueue**](https://msdn.microsoft.com/en-us/library/Hh448843(v=VS.85).aspx)|使用指定的套接字和I / O完成队列创建已注册的I / O套接字描述符，以便与Winsock注册的I / O扩展一起使用。|
|[**RIODequeueCompletion**](https://msdn.microsoft.com/en-us/library/Hh448845(v=VS.85).aspx)|从I / O完成队列中删除条目以与Winsock注册的I / O扩展一起使用。|
|[**RIODeregisterBuffer**](https://msdn.microsoft.com/en-us/library/Hh448847(v=VS.85).aspx)|取消注册与Winsock注册的I / O扩展一起使用的注册缓冲区。|
|[**RIONotify**](https://msdn.microsoft.com/en-us/library/Hh437191(v=VS.85).aspx)|使用I / O完成队列注册用于通知行为的方法，以便与Winsock注册的I / O扩展一起使用。|
|[**RIOReceive**](https://msdn.microsoft.com/en-us/library/Hh437193(v=VS.85).aspx)|在连接的已注册I / O TCP套接字或绑定的已注册I / O UDP套接字上接收网络数据，以便与Winsock注册的I / O扩展一起使用。|
|[**RIOReceiveEx**](https://msdn.microsoft.com/en-us/library/Hh437196(v=VS.85).aspx)|接收已连接的已注册I / O TCP套接字或绑定的已注册I / O UDP套接字上的网络数据，以及用于Winsock注册的I / O扩展的其他选项。|
|[**RIORegisterBuffer**](https://msdn.microsoft.com/en-us/library/Hh437199(v=VS.85).aspx)|使用指定的缓冲区注册RIO_BUFFERID（已注册的缓冲区描述符），以便与Winsock注册的I / O扩展一起使用。|
|[**RIOResizeCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh437202(v=VS.85).aspx)|将I / O完成队列的大小调整为更大或更小，以便与Winsock注册的I / O扩展一起使用。|
|[**RIOResizeRequestQueue**](https://msdn.microsoft.com/en-us/library/Hh437204(v=VS.85).aspx)|将请求队列的大小调整为更大或更小，以便与Winsock注册的I / O扩展一起使用。|
|[**RIOSend**](https://msdn.microsoft.com/en-us/library/Hh437213(v=VS.85).aspx)|在连接的已注册I / O TCP套接字或绑定的已注册I / O UDP套接字上发送网络数据，以便与Winsock注册的I / O扩展一起使用。|
|[**RIOSendEx**](https://msdn.microsoft.com/en-us/library/Hh437216(v=VS.85).aspx)|在连接的已注册I / O TCP套接字或绑定的已注册I / O UDP套接字上发送网络数据，以及用于Winsock注册的I / O扩展的其他选项。|
|[**select**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-select)|确定一个或多个套接字的状态，必要时等待执行同步I / O.|
|[**send**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-send)|在连接的套接字上发送数据。|
|[**sendto**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-sendto)|将数据发送到特定目的地。|
|[**SetAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-setaddrinfoexa)|使用特定命名空间提供程序注册主机和服务名称以及关联的地址。|
|[**setipv4sourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-setipv4sourcefilter)|设置IPv4套接字的多播过滤器状态。|
|[**SetService**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/nf-nspapi-setservicea)|在注册表中注册或删除一个或多个名称空间内的网络服务。 还可以在一个或多个名称空间中添加或删除网络服务类型。|
|[**SetSocketMediaStreamingMode**](https://docs.microsoft.com/en-us/windows/desktop/api/Socketapi/nf-socketapi-setsocketmediastreamingmode)|指示网络是否用于传输需要服务质量的流媒体。|
|[**setsockopt**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-setsockopt)|设置套接字选项。|
|[**setsourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-setsourcefilter)|设置IPv4或IPv6套接字的多播过滤器状态。|
|[**shutdown**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-shutdown)|禁用套接字上的发送或接收。|
|[**socket**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-socket)|创建绑定到特定服务提供者的套接字。|
|[**TransmitFile**](https://msdn.microsoft.com/en-us/library/ms740565(v=VS.85).aspx)|通过连接的套接字句柄传输文件数据。|
|[**TransmitPackets**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/nc-mswsock-lpfn_transmitpackets)|通过连接的套接字传输内存数据或文件数据。|
|[**WSAAccept**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaaccept)|有条件地接受基于条件函数的返回值的连接，提供服务质量流规范，并允许传输连接数据。|
|[**WSAAddressToString**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaaddresstostringa)|将sockaddr结构的所有组件转换为地址的人类可读字符串表示形式。|
|[**WSAAsyncGetHostByAddr**](https://msdn.microsoft.com/en-us/library/ms741519(v=VS.85).aspx)|异步检索与地址对应的主机信息。|
|[**WSAAsyncGetHostByName**](https://msdn.microsoft.com/en-us/library/ms741522(v=VS.85).aspx)|异步检索与主机名对应的主机信息。|
|[**WSAAsyncGetProtoByName**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsaasyncgetprotobyname)|异步检索与协议名称对应的协议信息。|
|[**WSAAsyncGetProtoByNumber**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsaasyncgetprotobynumber)|异步检索与协议号对应的协议信息。|
|[**WSAAsyncGetServByName**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsaasyncgetservbyname)|异步检索与服务名称和端口对应的服务信息。|
|[**WSAAsyncGetServByPort**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsaasyncgetservbyport)|异步检索与端口和协议对应的服务信息。|
|[**WSAAsyncSelect**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsaasyncselect)|请求基于Windows消息的基于消息的网络事件通知。|
|[**WSACancelAsyncRequest**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsacancelasyncrequest)|取消不完整的异步操作。|
|[**WSACleanup**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsacleanup)|终止使用Ws2_32.DLL。|
|[**WSACloseEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsacloseevent)|关闭一个打开的事件对象句柄。|
|[**WSAConnect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnect)|建立与另一个套接字应用程序的连接，交换连接数据，并根据指定的FLOWSPEC结构指定所需的服务质量。|
|[**WSAConnectByList**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnectbylist)|建立与一组目标地址（主机名和端口）所代表的可能端点集合中的一个的连接。|
|[**WSAConnectByName**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnectbynamea)|在指定的主机和端口上建立与另一个套接字应用程序的连接|
|[**WSACreateEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsacreateevent)|创建一个新的事件对象。|
|[**WSADeleteSocketPeerTargetName**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsadeletesocketpeertargetname)|删除对等目标名称与套接字IP地址之间的关联。|
|[**WSADuplicateSocket**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaduplicatesocketa)|返回可用于为共享套接字创建新套接字描述符的结构。|
|[**WSAEnumNameSpaceProviders**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnamespaceprovidersa)|检索有关可用命名空间的信息。|
|[**WSAEnumNameSpaceProvidersEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnamespaceprovidersexa)|检索有关可用命名空间的信息。|
|[**WSAEnumNetworkEvents**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnetworkevents)|发现指示套接字的网络事件的发生，清除内部网络事件记录和重置事件对象（可选）。|
|[**WSAEnumProtocols**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumprotocolsa)|检索有关可用传输协议的信息。|
|[**WSAEventSelect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaeventselect)|指定与指定的FD_XXX网络事件集关联的事件对象。|
|[**__WSAFDIsSet**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-__wsafdisset)|指定套接字是否包含在一组套接字描述符中。|
|[**WSAGetLastError**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsagetlasterror)|返回上次失败操作的错误状态。|
|[**WSAGetOverlappedResult**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsagetoverlappedresult)|检索指定套接字上的重叠操作的结果。|
|[**WSAGetQOSByName**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsagetqosbyname)|基于命名模板初始化QOS结构，或者它提供缓冲区以检索可用模板名称的枚举。|
|[**WSAGetServiceClassInfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsagetserviceclassinfoa)|从指定的命名空间提供程序检索与指定服务类有关的类信息（模式）。|
|[**WSAGetServiceClassNameByClassId**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsagetserviceclassnamebyclassida)|检索与指定类型关联的服务的名称。|
|[**WSAHtonl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsahtonl)|将u_long从主机字节顺序转换为网络字节顺序。|
|[**WSAHtons**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsahtons)|将u_short从主机字节顺序转换为网络字节顺序。|
|[**WSAImpersonateSocketPeer**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsaimpersonatesocketpeer)|用于模拟与套接字对等体对应的安全主体，以便执行应用程序级授权。|
|[**WSAInstallServiceClass**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsainstallserviceclassa)|在命名空间内注册服务类模式。|
|[**WSAIoctl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaioctl)|控制套接字的模式。|
|[**WSAJoinLeaf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsajoinleaf)|将叶节点连接到多点会话，交换连接数据，并根据指定的结构指定所需的服务质量。|
|[**WSALookupServiceBegin**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupservicebegina)|启动受WSAQUERYSET结构中包含的信息约束的客户端查询。|
|[**WSALookupServiceEnd**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupserviceend)|释放先前调用WSALookupServiceBegin和WSALookupServiceNext所使用的句柄。|
|[**WSALookupServiceNext**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupservicenexta)|检索所请求的服务信息。|
|[**WSANSPIoctl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsanspioctl)|开发人员对已注册的命名空间进行I / O控制调用。|
|[**WSANtohl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsantohl)|将u_long从网络字节顺序转换为主机字节顺序。|
|[**WSANtohs**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsantohs)|将u_short从网络字节顺序转换为主机字节顺序。|
|[**WSAPoll**](https://msdn.microsoft.com/en-us/library/ms741669(v=VS.85).aspx)|确定一个或多个套接字的状态。|
|[**WSAProviderConfigChange**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaproviderconfigchange)|更改提供程序配置时通知应用程序。|
|[**WSAQuerySocketSecurity**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsaquerysocketsecurity)|查询有关应用于套接字连接的安全性的信息。|
|[**WSARecv**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsarecv)|从连接的套接字接收数据。|
|[**WSARecvDisconnect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsarecvdisconnect)|终止套接字上的接收，并在套接字面向连接时检索断开连接数据。|
|[**WSARecvEx**](https://msdn.microsoft.com/en-us/library/ms741684(v=VS.85).aspx)|从连接的套接字接收数据。|
|[**WSARecvFrom**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsarecvfrom)|接收数据报并存储源地址。|
|[**WSARecvMsg**](https://msdn.microsoft.com/en-us/library/ms741687(v=VS.85).aspx)|从连接和未连接的套接字接收数据和可选控制信息。|
|[**WSARemoveServiceClass**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaremoveserviceclass)|从注册表中永久删除服务类架构。|
|[**WSAResetEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaresetevent)|将指定事件对象的状态重置为无信号。|
|[**WSARevertImpersonation**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsarevertimpersonation)|终止套接字对等体的模拟。|
|[**WSASend**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasend)|在连接的套接字上发送数据。在连接的套接字上发送数据。|
|[**WSASendDisconnect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasenddisconnect)|启动套接字连接的终止并发送断开连接数据。|
|[**WSASendMsg**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock2/nf-winsock2-wsasendmsg)|从连接和未连接的套接字发送数据和可选控制信息。|
|[**WSASendTo**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasendto)|使用重叠的I / O将数据发送到特定目标。|
|[**WSASetEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasetevent)|设置要发信号的指定事件对象的状态。|
|[**WSASetLastError**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsasetlasterror)|设置错误代码。|
|[**WSASetService**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasetservicea)|在注册表中注册或删除一个或多个名称空间中的服务实例。|
|[**WSASetSocketPeerTargetName**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsasetsocketpeertargetname)|用于指定与对等IP地址对应的对等目标名称（SPN）。 此目标名称应由客户端应用程序指定，以安全地标识应进行身份验证的对等方。|
|[**WSASetSocketSecurity**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsasetsocketsecurity)|启用并应用套接字的安全性。|
|[**WSASocket**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasocketa)|创建绑定到特定传输服务提供程序的套接字。|
|[**WSAStartup**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsastartup)|通过进程启动WS2_32.DLL的使用。|
|[**WSAStringToAddress**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsastringtoaddressa)|将数字字符串转换为sockaddr结构。|
|[**WSAWaitForMultipleEvents**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsawaitformultipleevents)|当一个或所有指定的事件对象处于信号状态或超时间隔到期时返回。|



