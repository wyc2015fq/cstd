# Windows Sockets新特性 - 一世豁然的专栏 - CSDN博客





2018年10月10日 15:50:10[一世豁然](https://me.csdn.net/Explorer_day)阅读数：51标签：[Windows Socket2](https://so.csdn.net/so/search/s.do?q=Windows Socket2&t=blog)
个人分类：[Windows](https://blog.csdn.net/Explorer_day/article/category/6772518)









**已针对Windows 8.1和Windows Server 2012 R2进行了更新**

Windows 8.1，Windows Server 2012 R2及更高版本上的Windows应用商店应用程序支持以下功能。 Windows应用商店应用程序需要Microsoft Visual Studio 2013 Update 3或更高版本。
- [**__WSAFDIsSet**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-__wsafdisset)
- [**accept**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-accept)
- [**AcceptEx**](https://msdn.microsoft.com/en-us/library/ms737524(v=VS.85).aspx)
- [**bind**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-bind)
- [**closesocket**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-closesocket)
- [**connect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-connect)
- [**ConnectEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/nc-mswsock-lpfn_connectex)
- [**DisconnectEx**](https://msdn.microsoft.com/en-us/library/ms737757(v=VS.85).aspx)
- [**freeaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-freeaddrinfo)
- [**FreeAddrInfoExW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-freeaddrinfoex)
- [**freeaddrinfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-freeaddrinfo)
- [**GetAcceptExSockaddrs**](https://msdn.microsoft.com/en-us/library/ms738516(v=VS.85).aspx)
- [**getaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfo)
- [**GetAddrInfoExCancel**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexcancel)
- [**GetAddrInfoExOverlappedResult**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexoverlappedresult)
- [**GetAddrInfoExW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexa)
- [**GetAddrInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfow)
- [**gethostbyaddr**](https://msdn.microsoft.com/en-us/library/ms738521(v=VS.85).aspx)
- [**gethostbyname**](https://msdn.microsoft.com/en-us/library/ms738524(v=VS.85).aspx)
- [**gethostname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-gethostname)
- [**GetHostNameW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-gethostnamew)
- [**getipv4sourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getipv4sourcefilter)
- [**getnameinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getnameinfo)
- [**GetNameInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getnameinfow)
- [**getpeername**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getpeername)
- [**getprotobyname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getprotobyname)
- [**getprotobynumber**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getprotobynumber)
- [**getservbyname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getservbyname)
- [**getservbyport**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getservbyport)
- [**getsockname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getsockname)
- [**getsockopt**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getsockopt)
- [**getsourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getsourcefilter)
- [**htond**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htond)
- [**htonf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htonf)
- [**htonl**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-htonl)
- [**htonll**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htonll)
- [**htons**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-htons)
- [**inet_addr**](https://msdn.microsoft.com/en-us/library/ms738563(v=VS.85).aspx)
- [**inet_ntoa**](https://msdn.microsoft.com/en-us/library/ms738564(v=VS.85).aspx)
- [**inet_ntop**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-inetntopw)
- [**inet_pton**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-inetptonw)
- [**InetNtopW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-inetntopw)
- [**InetPtonW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-inetptonw)
- [**ioctlsocket**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-ioctlsocket)
- [**listen**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-listen)
- [**ntohd**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohd)
- [**ntohf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohf)
- [**ntohl**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-ntohl)
- [**ntohll**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohll)
- [**ntohs**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-ntohs)
- [**recv**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-recv)
- [**recvfrom**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-recvfrom)
- [**RIOCloseCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh448837(v=VS.85).aspx)
- [**RIOCreateCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh448841(v=VS.85).aspx)
- [**RIOCreateRequestQueue**](https://msdn.microsoft.com/en-us/library/Hh448843(v=VS.85).aspx)
- [**RIODequeueCompletion**](https://msdn.microsoft.com/en-us/library/Hh448845(v=VS.85).aspx)
- [**RIODeregisterBuffer**](https://msdn.microsoft.com/en-us/library/Hh448847(v=VS.85).aspx)
- [**RIONotify**](https://msdn.microsoft.com/en-us/library/Hh437191(v=VS.85).aspx)
- [**RIOReceive**](https://msdn.microsoft.com/en-us/library/Hh437193(v=VS.85).aspx)
- [**RIOReceiveEx**](https://msdn.microsoft.com/en-us/library/Hh437196(v=VS.85).aspx)
- [**RIORegisterBuffer**](https://msdn.microsoft.com/en-us/library/Hh437199(v=VS.85).aspx)
- [**RIOResizeCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh437202(v=VS.85).aspx)
- [**RIOResizeRequestQueue**](https://msdn.microsoft.com/en-us/library/Hh437204(v=VS.85).aspx)
- [**RIOSend**](https://msdn.microsoft.com/en-us/library/Hh437213(v=VS.85).aspx)
- [**RIOSendEx**](https://msdn.microsoft.com/en-us/library/Hh437216(v=VS.85).aspx)
- [**select**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-select)
- [**send**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-send)
- [**sendto**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-sendto)
- [**SetAddrInfoExW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-setaddrinfoexa)
- [**setipv4sourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-setipv4sourcefilter)
- [**setsockopt**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-setsockopt)
- [**setsourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-setsourcefilter)
- [**shutdown**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-shutdown)
- [**socket**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-socket)
- [**TransmitFile**](https://msdn.microsoft.com/en-us/library/ms740565(v=VS.85).aspx)
- [**TransmitPackets**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/nc-mswsock-lpfn_transmitpackets)
- [**WSAAccept**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaaccept)
- [**WSAAddressToStringW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaaddresstostringa)
- [**WSACleanup**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsacleanup)
- [**WSACloseEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsacloseevent)
- [**WSAConnect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnect)
- [**WSAConnectByList**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnectbylist)
- [**WSAConnectByNameW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnectbynamea)
- [**WSACreateEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsacreateevent)
- [**WSADuplicateSocketW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaduplicatesocketa)
- [**WSAEnumNameSpaceProvidersExW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnamespaceprovidersexa)
- [**WSAEnumNameSpaceProvidersW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnamespaceprovidersa)
- [**WSAEnumNetworkEvents**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnetworkevents)
- [**WSAEnumProtocolsW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumprotocolsa)
- [**WSAEventSelect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaeventselect)
- [**WSAGetLastError**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsagetlasterror)
- [**WSAGetOverlappedResult**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsagetoverlappedresult)
- [**WSAHtonl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsahtonl)
- [**WSAHtons**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsahtons)
- [**WSAIoctl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaioctl)
- [**WSAJoinLeaf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsajoinleaf)
- [**WSALookupServiceBeginW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupservicebegina)
- [**WSALookupServiceEnd**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupserviceend)
- [**WSALookupServiceNextW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupservicenexta)
- [**WSANSPIoctl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsanspioctl)
- [**WSANtohl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsantohl)
- [**WSANtohs**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsantohs)
- [**WSAPoll**](https://msdn.microsoft.com/en-us/library/ms741669(v=VS.85).aspx)
- [**WSAProviderConfigChange**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaproviderconfigchange)
- [**WSARecv**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsarecv)
- [**WSARecvFrom**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsarecvfrom)
- [**WSARecvMsg**](https://msdn.microsoft.com/en-us/library/ms741687(v=VS.85).aspx)
- [**WSAResetEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaresetevent)
- [**WSASend**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasend)
- [**WSASendMsg**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock2/nf-winsock2-wsasendmsg)
- [**WSASendTo**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasendto)
- [**WSASetEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasetevent)
- [**WSASetLastError**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsasetlasterror)
- [**WSASetServiceW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasetservicea)
- [**WSASocketW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasocketa)
- [**WSAStartup**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsastartup)
- [**WSAStringToAddressW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsastringtoaddressa)
- [**WSAWaitForMultipleEvents**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsawaitformultipleevents)





**已针对Windows Phone 8进行了更新**

Windows Phone 8及更高版本上的Windows Phone应用程序支持以下功能。
- [**__WSAFDIsSet**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-__wsafdisset)
- [**AcceptEx**](https://msdn.microsoft.com/en-us/library/ms737524(v=VS.85).aspx)
- [**bind**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-bind)
- [**closesocket**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-closesocket)
- [**connect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-connect)
- [**ConnectEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/nc-mswsock-lpfn_connectex)
- [**DisconnectEx**](https://msdn.microsoft.com/en-us/library/ms737757(v=VS.85).aspx)
- [**freeaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-freeaddrinfo)
- [**getaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfo)
- [**gethostbyaddr**](https://msdn.microsoft.com/en-us/library/ms738521(v=VS.85).aspx)
- [**gethostbyname**](https://msdn.microsoft.com/en-us/library/ms738524(v=VS.85).aspx)
- [**gethostname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-gethostname)
- [**GetHostNameW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-gethostnamew)
- [**getipv4sourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getipv4sourcefilter)
- [**getnameinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getnameinfo)
- [**GetNameInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getnameinfow)
- [**getpeername**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getpeername)
- [**getprotobyname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getprotobyname)
- [**getprotobynumber**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getprotobynumber)
- [**getservbyname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getservbyname)
- [**getservbyport**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getservbyport)
- [**getsockname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getsockname)
- [**getsockopt**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getsockopt)
- [**getsourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getsourcefilter)
- [**htond**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htond)
- [**htonf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htonf)
- [**htonll**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htonll)
- [**inet_addr**](https://msdn.microsoft.com/en-us/library/ms738563(v=VS.85).aspx)
- [**inet_ntoa**](https://msdn.microsoft.com/en-us/library/ms738564(v=VS.85).aspx)
- [**ioctlsocket**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-ioctlsocket)
- [**listen**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-listen)
- [**ntohd**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohd)
- [**ntohf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohf)
- [**ntohll**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohll)
- [**recv**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-recv)
- [**recvfrom**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-recvfrom)
- [**RIOCloseCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh448837(v=VS.85).aspx)
- [**RIOCreateCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh448841(v=VS.85).aspx)
- [**RIOCreateRequestQueue**](https://msdn.microsoft.com/en-us/library/Hh448843(v=VS.85).aspx)
- [**RIODequeueCompletion**](https://msdn.microsoft.com/en-us/library/Hh448845(v=VS.85).aspx)
- [**RIODeregisterBuffer**](https://msdn.microsoft.com/en-us/library/Hh448847(v=VS.85).aspx)
- [**RIONotify**](https://msdn.microsoft.com/en-us/library/Hh437191(v=VS.85).aspx)
- [**RIOReceive**](https://msdn.microsoft.com/en-us/library/Hh437193(v=VS.85).aspx)
- [**RIOReceiveEx**](https://msdn.microsoft.com/en-us/library/Hh437196(v=VS.85).aspx)
- [**RIORegisterBuffer**](https://msdn.microsoft.com/en-us/library/Hh437199(v=VS.85).aspx)
- [**RIOResizeCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh437202(v=VS.85).aspx)
- [**RIOResizeRequestQueue**](https://msdn.microsoft.com/en-us/library/Hh437204(v=VS.85).aspx)
- [**RIOSend**](https://msdn.microsoft.com/en-us/library/Hh437213(v=VS.85).aspx)
- [**RIOSendEx**](https://msdn.microsoft.com/en-us/library/Hh437216(v=VS.85).aspx)
- [**select**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-select)
- [**send**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-send)
- [**sendto**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-sendto)
- [**setipv4sourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-setipv4sourcefilter)
- [**setsockopt**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-setsockopt)
- [**setsourcefilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-setsourcefilter)
- [**shutdown**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-shutdown)
- [**socket**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-socket)
- [**TransmitPackets**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/nc-mswsock-lpfn_transmitpackets)
- [**WSAAccept**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaaccept)
- [**WSAAddressToStringW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaaddresstostringa)
- [**WSACleanup**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsacleanup)
- [**WSACloseEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsacloseevent)
- [**WSAConnect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnect)
- [**WSAConnectByList**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnectbylist)
- [**WSAConnectByNameW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnectbynamea)
- [**WSACreateEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsacreateevent)
- [**WSAEnumNameSpaceProvidersW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnamespaceprovidersa)
- [**WSAEnumNetworkEvents**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnetworkevents)
- [**WSAEnumProtocolsW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumprotocolsa)
- [**WSAEventSelect**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaeventselect)
- [**WSAGetLastError**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsagetlasterror)
- [**WSAGetOverlappedResult**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsagetoverlappedresult)
- [**WSAHtonl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsahtonl)
- [**WSAHtons**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsahtons)
- [**WSAIoctl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaioctl)
- [**WSAJoinLeaf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsajoinleaf)
- [**WSALookupServiceBeginW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupservicebegina)
- [**WSALookupServiceEnd**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupserviceend)
- [**WSALookupServiceNextW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupservicenexta)
- [**WSANSPIoctl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsanspioctl)
- [**WSANtohl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsantohl)
- [**WSANtohs**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsantohs)
- [**WSARecv**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsarecv)
- [**WSARecvFrom**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsarecvfrom)
- [**WSARecvMsg**](https://msdn.microsoft.com/en-us/library/ms741687(v=VS.85).aspx)
- [**WSAResetEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaresetevent)
- [**WSASend**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasend)
- [**WSASendTo**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasendto)
- [**WSASetEvent**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasetevent)
- [**WSASetLastError**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsasetlasterror)
- [**WSASetServiceW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasetservicea)
- [**WSASocketW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasocketa)
- [**WSAStartup**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsastartup)
- [**WSAStringToAddressW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsastringtoaddressa)
- [**WSAWaitForMultipleEvents**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsawaitformultipleevents)





**已针对Windows 8和Windows Server 2012进行了更新**

Microsoft Windows 8和Windows Server 2012引入了新的Windows套接字编程元素。

提供一组高速网络扩展，可提高网络性能，降低延迟和抖动。 这些主要针对服务器应用程序的扩展使用预先注册的数据缓冲区和完成队列来提高性能。

以下是为支持Winsock高速网络注册的I / O扩展而添加的新Windows套接字功能：
- [**RIOCloseCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh448837(v=VS.85).aspx)
- [**RIOCreateCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh448841(v=VS.85).aspx)
- [**RIOCreateRequestQueue**](https://msdn.microsoft.com/en-us/library/Hh448843(v=VS.85).aspx)
- [**RIODequeueCompletion**](https://msdn.microsoft.com/en-us/library/Hh448845(v=VS.85).aspx)
- [**RIODeregisterBuffer**](https://msdn.microsoft.com/en-us/library/Hh448847(v=VS.85).aspx)
- [**RIONotify**](https://msdn.microsoft.com/en-us/library/Hh437191(v=VS.85).aspx)
- [**RIOReceive**](https://msdn.microsoft.com/en-us/library/Hh437193(v=VS.85).aspx)
- [**RIOReceiveEx**](https://msdn.microsoft.com/en-us/library/Hh437196(v=VS.85).aspx)
- [**RIORegisterBuffer**](https://msdn.microsoft.com/en-us/library/Hh437199(v=VS.85).aspx)
- [**RIOResizeCompletionQueue**](https://msdn.microsoft.com/en-us/library/Hh437202(v=VS.85).aspx)
- [**RIOResizeRequestQueue**](https://msdn.microsoft.com/en-us/library/Hh437204(v=VS.85).aspx)
- [**RIOSend**](https://msdn.microsoft.com/en-us/library/Hh437213(v=VS.85).aspx)
- [**RIOSendEx**](https://msdn.microsoft.com/en-us/library/Hh437216(v=VS.85).aspx)

以下是为支持Winsock高速网络注册I / O扩展而添加的新Windows套接字枚举，结构和typedef：
- [**RIO_CQ**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/riocqueue)
- [**RIO_RQ**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/riorqueue)
- [**RIO_BUFFERID**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/rio-bufferid)
- [**RIO_BUF**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsockdef/ns-mswsockdef-_rio_buf)
- [**RIO_NOTIFICATION_COMPLETION**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/ns-mswsock-_rio_notification_completion)
- [**RIO_NOTIFICATION_COMPLETION_TYPE**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/ne-mswsock-_rio_notification_completion_type)
- [**RIORESULT**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsockdef/ns-mswsockdef-_rioresult)

提供了一组用于异步命名支持的增强功能。

以下是为支持异步命名请求而添加的新Windows套接字函数：
- [**GetAddrInfoExCancel**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexcancel)
- [**GetAddrInfoExOverlappedResult**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexoverlappedresult)

以下现有的Winsock函数已经过修订，以支持异步命名请求：
- [**GetAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexa)
- [**NSPv2LookupServiceBegin**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nc-ws2spi-lpnspv2lookupservicebegin)

可以使用一组增强功能来添加对国际化域名（IDN）解析的支持。

以下现有的Winsock函数已经过修订，以支持IDN解析：
- [**getaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfo)
- [**GetAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexa)
- [**GetAddrInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfow)

可以使用Winsock中的命名支持增强功能来支持请求规范名称和完全限定的域名。

以下现有的Winsock函数已经过修订，以支持请求规范名称和完全限定的域名：
- [**GetAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexa)

添加了以下新的Winsock结构以支持请求规范名称和完全限定的域名：
- [**addrinfoex2**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-addrinfoex2w)

添加了以下新的Windows套接字函数以检索Unicode中的本地主机名：
- [**GetHostNameW**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-gethostnamew)

以下是为支持Windows筛选平台（WFP）重定向服务而添加的新Windows套接字Ioctls：
- [**SIO_QUERY_WFP_CONNECTION_REDIRECT_CONTEXT**](https://msdn.microsoft.com/en-us/library/Hh859712(v=VS.85).aspx)
- [**SIO_QUERY_WFP_CONNECTION_REDIRECT_RECORDS**](https://msdn.microsoft.com/en-us/library/Hh859713(v=VS.85).aspx)
- [**SIO_SET_WFP_CONNECTION_REDIRECT_RECORDS**](https://msdn.microsoft.com/en-us/library/Hh859714(v=VS.85).aspx)

以下是为支持Windows筛选平台（WFP）重定向服务而添加的新Windows IP套接字选项：
- [**IP_WFP_REDIRECT_CONTEXT**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/ipproto-ip-socket-options)
- [**IP_WFP_REDIRECT_RECORDS**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/ipproto-ip-socket-options)

以下是一个新的Windows套接字Ioctl和一个相关的结构，用于控制TCP套接字的初始（SYN / SYN + ACK）重传特性：
- [**SIO_TCP_INITIAL_RTO**](https://msdn.microsoft.com/en-us/library/JJ710203(v=VS.85).aspx)
- [**TCP_INITIAL_RTO_PARAMETERS**](https://docs.microsoft.com/en-us/windows/desktop/api/mswsock/ns-mswsock-_transmit_file_buffers)

以下是一个新的Windows套接字Ioctl和一个相关的结构，用于检索套接字和RSS处理器核心与NUMA节点之间的关联：
- [**SIO_QUERY_RSS_PROCESSOR_INFO**](https://msdn.microsoft.com/en-us/library/JJ553482(v=VS.85).aspx)
- [**SOCKET_PROCESSOR_AFFINITY**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-_socket_processor_affinity)

添加以下新的Windows套接字Ioctl以应用和查询套接字上的传输设置：
- [**SIO_APPLY_TRANSPORT_SETTING**](https://msdn.microsoft.com/en-us/library/JJ553481(v=VS.85).aspx)
- [**SIO_QUERY_TRANSPORT_SETTING**](https://msdn.microsoft.com/en-us/library/JJ553483(v=VS.85).aspx)

当前定义的唯一传输设置是针对TCP套接字上的**REAL_TIME_NOTIFICATION_CAPABILITY**功能。 添加了以下新结构和枚举以支持**REAL_TIME_NOTIFICATION_CAPABILITY**：
- [**CONTROL_CHANNEL_TRIGGER_STATUS**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/ne-mswsock-_rio_notification_completion_type)
- [**REAL_TIME_NOTIFICATION_SETTING_INPUT**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_real_time_notification_setting_input)
- [**REAL_TIME_NOTIFICATION_SETTING_INPUT**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_real_time_notification_setting_input)
- [**REAL_TIME_NOTIFICATION_SETTING_OUTPUT**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_real_time_notification_setting_input)
- [**TRANSPORT_SETTING_ID**](https://msdn.microsoft.com/en-us/library/JJ553485(v=VS.85).aspx)

添加了以下新的Windows套接字Ioctl，以便在TCP套接字上启用环回的快速路径。 此功能可以降低延迟并提高使用TCP环回的应用程序（例如金融服务行业使用的应用程序）的性能：
- [**SIO_LOOPBACK_FAST_PATH**](https://msdn.microsoft.com/en-us/library/JJ841212(v=VS.85).aspx)

增强功能以支持传输需要服务质量的流媒体（例如，IP语音）。

以下新的Windows套接字功能支持传输需要服务质量的流媒体：
- [**SetSocketMediaStreamingMode**](https://docs.microsoft.com/en-us/windows/desktop/api/Socketapi/nf-socketapi-setsocketmediastreamingmode)

Windows 8，Windows Server 2012及更高版本上的Windows应用商店应用也支持SetSocketMediaStreamingMode功能。

Winsock2.h头文件中定义的一组内联函数，用于在主机字节顺序和网络字节顺序之间转换float或无符号__int64。
- [**htond**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htond)
- [**htonf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htonf)
- [**htonll**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-htonll)
- [**ntohd**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohd)
- [**ntohf**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohf)
- [**ntohll**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-ntohll)





**已针对Windows 7和Windows Server 2008 R2进行了更新**

Microsoft Windows 7和Windows Server 2008 R2引入了新的Windows套接字编程元素。

以下现有Windows套接字功能已得到增强，可支持其他选项：
- [**getaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfo)
- [**GetAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexa)
- [**GetAddrInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfow)

以下是新的Windows套接字选项：
- [**IP_ORIGINAL_ARRIVAL_IF**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/ipproto-ip-socket-options)
- [**IP_ORIGINAL_ARRIVAL_IF for IPv6**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/ipproto-ipv6-socket-options)

Windows 7和Windows Server 2008 R2上添加了TCP的动态发送缓冲。 因此，仅在特殊情况下才需要使用SIO_IDEAL_SEND_BACKLOG_CHANGE和SIO_IDEAL_SEND_BACKLOG_QUERY IOCTL。 有关更多信息，请参阅SIO_IDEAL_SEND_BACKLOG_QUERY。





**针对Windows Server 2008和带有Service Pack 1（SP1）的Windows Vista进行了更新**

Microsoft Windows Server 2008和带有Service Pack 1（SP1）的Windows Vista引入了新的Windows套接字编程元素。

以下是新的Windows套接字Ioctls：
- [**SIO_IDEAL_SEND_BACKLOG_CHANGE**](https://msdn.microsoft.com/en-us/library/Bb736548(v=VS.85).aspx)
- [**SIO_IDEAL_SEND_BACKLOG_QUERY**](https://msdn.microsoft.com/en-us/library/Bb736549(v=VS.85).aspx)

这些新的Ioctl可以由使用TCP的应用程序使用，以确定要发送的未完成数据量的理想值，以便实现连接的最佳吞吐量。 这称为理想的发送积压（ISB）大小。





**已针对Windows Server 2008进行了更新**

Microsoft Windows Server 2008引入了新的Windows套接字编程元素。

以下是新的Windows套接字选项：
- [**SO_PORT_SCALABILITY**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/so-port-scalability)





**针对Windows Vista进行了更新**

Microsoft Windows Vista引入了新的Windows套接字编程元素。 这些元素扩展了Winsock简化编程和提供IPv6兼容性的能力。 有关将Winsock应用程序移植到IPv6的详细信息，请参阅“适用于Windows套接字应用程序的IPv6指南”。

Windows套接字跟踪是Windows Vista和Windows Server 2008支持的新功能。可以在零售二进制文件中启用此功能，以最小的开销跟踪某些套接字事件。 有关更多信息，请参阅Winsock跟踪。

以下是新的Windows套接字功能：
- [**FreeAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-freeaddrinfoex)
- [**GetAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexa)
- [**InetNtop**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-inetntopw)
- [**InetPton**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-inetptonw)
- [**SetAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-setaddrinfoexa)
- [**WSAConnectByList**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnectbylist)
- [**WSAConnectByName**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaconnectbynamea)
- [**WSADeleteSocketPeerTargetName**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsadeletesocketpeertargetname)
- [**WSAEnumNameSpaceProvidersEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnamespaceprovidersexa)
- [**WSAImpersonateSocketPeer**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsaimpersonatesocketpeer)
- [**WSAPoll**](https://msdn.microsoft.com/en-us/library/ms741669(v=VS.85).aspx)
- [**WSAQuerySocketSecurity**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsaquerysocketsecurity)
- [**WSARevertImpersonation**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsarevertimpersonation)
- [**WSASendMsg**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock2/nf-winsock2-wsasendmsg)
- [**WSASetSocketPeerTargetName**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsasetsocketpeertargetname)
- [**WSASetSocketSecurity**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-wsasetsocketsecurity)

以下是新的Windows套接字结构和枚举：
- [**addrinfoex**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-addrinfoexw)
- [**BLOB**](https://msdn.microsoft.com/en-us/library/ms737551(v=VS.85).aspx)
- [**GROUP_FILTER**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_filter)
- [**GROUP_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_req)
- [**GROUP_SOURCE_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_source_req)
- [**MULTICAST_MODE_TYPE**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ne-ws2ipdef-multicast_mode_type)
- [**NAPI_DOMAIN_DESCRIPTION_BLOB**](https://docs.microsoft.com/en-us/windows/desktop/api/Nsemail/ns-nsemail-napi_domain_description_blob_tag)
- [**NAPI_PROVIDER_INSTALLATION_BLOB**](https://docs.microsoft.com/en-us/windows/desktop/api/Nsemail/ns-nsemail-napi_provider_installation_blob_tag)
- [**NAPI_PROVIDER_LEVEL**](https://docs.microsoft.com/en-us/windows/desktop/api/Nsemail/ne-nsemail-napi_provider_level_tag)
- [**NAPI_PROVIDER_TYPE**](https://docs.microsoft.com/en-us/windows/desktop/api/Nsemail/ne-nsemail-napi_provider_type_tag)
- [**SOCKET_PEER_TARGET_NAME**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_peer_target_name)
- [**SOCKET_SECURITY_PROTOCOL**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ne-mstcpip-_socket_security_protocol)
- [**SOCKET_SECURITY_QUERY_INFO**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_security_query_info)
- [**SOCKET_SECURITY_QUERY_TEMPLATE**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_security_query_template)
- [**SOCKET_SECURITY_SETTINGS**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_security_settings)
- [**SOCKET_SECURITY_SETTINGS_IPSEC**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_security_settings_ipsec)
- [**SOCKET_USAGE_TYPE**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ne-mstcpip-_socket_usage_type)
- [**WSAQUERYSET2**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsaqueryset2w)

Microsoft Windows Vista引入了新的Windows套接字SPI功能，以便为分层服务提供商分类应用程序。 还为命名空间提供程序添加了新函数。

以下是新的Windows套接字SPI函数：
- [**NSPv2Cleanup**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nc-ws2spi-lpnspv2cleanup)
- [**NSPv2ClientSessionRundown**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nc-ws2spi-lpnspv2clientsessionrundown)
- [**NSPv2LookupServiceBegin**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nc-ws2spi-lpnsplookupservicebegin)
- [**NSPv2LookupServiceEnd**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nc-ws2spi-lpnspv2lookupserviceend)
- [**NSPv2LookupServiceNextEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nc-ws2spi-lpnspv2lookupservicenextex)
- [**NSPv2SetServiceEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nc-ws2spi-lpnspv2setserviceex)
- [**NSPv2Startup**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nc-ws2spi-lpnspv2startup)
- [**WSAAdvertiseProvider**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wsaadvertiseprovider)
- [**WSAProviderCompleteAsyncCall**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wsaprovidercompleteasynccall)
- [**WSAUnadvertiseProvider**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wsaunadvertiseprovider)
- [**WSCEnumNameSpaceProvidersEx32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscenumnamespaceprovidersex32)
- [**WSCGetApplicationCategory**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscgetapplicationcategory)
- [**WSCGetProviderInfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscgetproviderinfo)
- [**WSCInstallNameSpaceEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscinstallnamespaceex)
- [**WSCInstallNameSpaceEx32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscinstallnamespaceex32)
- [**WSCSetApplicationCategory**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscsetapplicationcategory)
- [**WSCSetProviderInfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscsetproviderinfo)
- [**WSCSetProviderInfo32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscsetproviderinfo32)

以下是新的Windows套接字SPI结构：
- [**NSPV2_ROUTINE**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/ns-ws2spi-_nspv2_routine)

Microsoft Windows Vista引入了新的Windows套接字编程元素。

以下是新的Windows套接字Ioctls：
- [**SIO_ACQUIRE_PORT_RESERVATION**](https://msdn.microsoft.com/en-us/library/Gg699720(v=VS.85).aspx)
- [**SIO_ASSOCIATE_PORT_RESERVATION**](https://msdn.microsoft.com/en-us/library/Gg699721(v=VS.85).aspx)
- [**SIO_SET_COMPATIBILITY_MODE**](https://msdn.microsoft.com/en-us/library/Cc136103(v=VS.85).aspx)
- [**SIO_RELEASE_PORT_RESERVATION**](https://msdn.microsoft.com/en-us/library/Gg699722(v=VS.85).aspx)

以下是新的Windows套接字选项：
- [**SO_BSP_STATE**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/so-bsp-state)
- [**IP_UNICAST_IF**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/ipproto-ip-socket-options)
- [**IPV6_UNICAST_IF**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/ipproto-ipv6-socket-options)





**更新于2005年4月**

Windows套接字SPI（ws2spi.h）中添加了以下功能，以支持64位平台上的32位进程和目录：
- [**WSCDeinstallProvider32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscdeinstallprovider32)
- [**WSCEnableNSProvider32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscenablensprovider32)
- [**WSCEnumNameSpaceProviders32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscenumnamespaceproviders32)
- [**WSCEnumProtocols32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscenumprotocols32)
- [**WSCGetProviderPath32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscgetproviderpath32)
- [**WSCInstallNameSpace32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscinstallnamespace32)
- [**WSCInstallProvider64_32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscinstallprovider64_32)
- [**WSCInstallProviderAndChains64_32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscinstallproviderandchains64_32)
- [**WSCUnInstallNameSpace32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscuninstallnamespace32)
- [**WSCUpdateProvider32**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2spi/nf-ws2spi-wscupdateprovider32)





**Updated for Windows Server 2003**

Microsoft Windows Server 2003引入了新的Windows套接字编程元素。 这些元素扩展了Winsock简化编程和提供IPv6兼容性的能力。 有关将Winsock应用程序移植到IPv6的详细信息，请参阅“适用于Windows套接字应用程序的IPv6指南”。

以下是新的Windows套接字功能：
- [**ConnectEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/nc-mswsock-lpfn_connectex)
- [**DisconnectEx**](https://msdn.microsoft.com/en-us/library/ms737757(v=VS.85).aspx)
- [**freeaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-freeaddrinfo)
- [**gai_strerror**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-gai_strerrora)
- [**getaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfo)
- [**getnameinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getnameinfo)
- [**TransmitPackets**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/nc-mswsock-lpfn_transmitpackets)
- [**WSANSPIoctl**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsanspioctl)
- [**WSARecvMsg**](https://msdn.microsoft.com/en-us/library/ms741687(v=VS.85).aspx)

以下是新的Windows套接字结构定义：
- [**addrinfo**](https://msdn.microsoft.com/en-us/library/ms737530(v=VS.85).aspx)
- [**in_pktinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-in_pktinfo)
- [**SOCKADDR_STORAGE**](https://msdn.microsoft.com/en-us/library/ms740504(v=VS.85).aspx)
- [**TRANSMIT_PACKETS_ELEMENT**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/ns-mswsock-_transmit_packets_element)
- [**WSAMSG**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-_wsamsg)







