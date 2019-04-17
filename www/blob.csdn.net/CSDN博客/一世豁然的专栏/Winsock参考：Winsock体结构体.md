# Winsock参考：Winsock体结构体 - 一世豁然的专栏 - CSDN博客





2018年10月15日 16:28:27[一世豁然](https://me.csdn.net/Explorer_day)阅读数：77








以下列表提供了每个Winsock结构和typedef的简明描述。 有关任何结构或typedef的其他信息，请单击结构或typedef名称。
|结构体|描述|
|----|----|
|[**addrinfo**](https://msdn.microsoft.com/en-us/library/ms737530(v=VS.85).aspx)|由getaddrinfo函数用于保存主机地址信息。|
|[**addrinfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-addrinfow)|由GetAddrInfoW函数用于保存主机地址信息。|
|[**addrinfoex**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-addrinfoexw)|由GetAddrInfoEx函数用于保存主机地址信息。|
|[**addrinfoex2**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-addrinfoex2w)|当请求规范名称和完全限定的域名时，GetAddrInfoEx函数使用它来保存主机地址信息。|
|[**addrinfoex3**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-addrinfoex3)|GetAddrInfoEx函数用于在请求特定网络接口时保存主机地址信息。|
|[**addrinfoex4**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-addrinfoex4)|当请求完全限定域名的句柄时，GetAddrInfoEx函数使用它来保存主机地址信息。|
|[**AFPROTOCOLS**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_afprotocols)|提供应用程序员可以约束查询的协议列表。 仅用于查询目的。|
|[**BLOB**](https://msdn.microsoft.com/en-us/library/ms737551(v=VS.85).aspx)|包含有关数据块的信息。 源自二进制大对象。|
|[**CSADDR_INFO**](https://msdn.microsoft.com/en-us/library/ms737640(v=VS.85).aspx)|包含网络服务或命名空间提供程序的Winsock地址信息。|
|[**fd_set**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-fd_set)|由各种Winsock函数和服务提供者（例如select）用于将套接字放入“set”中以用于各种目的，例如测试给定套接字的可读性。|
|[**GROUP_FILTER**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_filter)|为组播IPv6或IPv4地址提供组播过滤参数。|
|[**GROUP_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_req)|提供IPv6或IPv4地址的组播组信息。|
|[**GROUP_SOURCE_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_source_req)|提供包含源IP地址的IPv6或IPv4地址的组播组信息。|
|[**hostent**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/ns-winsock-hostent)|存储有关给定主机的信息，例如主机名，IP地址等。|
|[**in_addr**](https://msdn.microsoft.com/en-us/library/ms738571(v=VS.85).aspx)|表示IPv4 Internet地址。|
|[**in_pktinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-in_pktinfo)|存储接收到数据包地址信息，并由Windows用于返回有关已接收数据包的信息。|
|[**in6_addr**](https://msdn.microsoft.com/en-us/library/ms738560(v=VS.85).aspx)|表示IPv6 Internet地址。|
|[**in6_pktinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-in6_pktinfo)|存储收到IPv6数据包地址信息，并由Windows用于返回有关已接收数据包的信息。|
|[**INET_PORT_RANGE**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_inet_port_range)|提供SIO_ACQUIRE_PORT_RESERVATION Ioctl使用的输入数据，以获取TCP或UDP端口块的运行时预留。|
|[**INET_PORT_RESERVATION_INSTANCE**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-inet_port_reservation_instance)|包含端口预留和TCP或UDP端口块的标记。|
|[**INET_PORT_RESERVATION_TOKEN**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-inet_port_reservation_token)|包含TCP或UDP端口块的端口预留令牌。|
|[**ip_mreq**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_mreq)|提供IPv4地址的组播组信息。|
|[**ip_mreq_source**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_mreq_source)|提供IPv4地址的组播组和源信息。|
|[**ip_msfilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_msfilter)|提供IPv4地址的组播过滤参数。|
|[**ipv6_mreq**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ipv6_mreq)|提供IPv6地址的组播组信息。|
|[**linger**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/ns-winsock-linger)|维护有关特定套接字的信息，该套接字指定在将数据排队等待发送并在套接字上调用closesocket函数时该套接字应该如何操作。|
|[**NAPI_DOMAIN_DESCRIPTION_BLOB**](https://docs.microsoft.com/en-us/windows/desktop/api/Nsemail/ns-nsemail-napi_domain_description_blob_tag)|描述由名称空间提供程序为NS_EMAIL名称空间处理的域。|
|[**NAPI_PROVIDER_INSTALLATION_BLOB**](https://docs.microsoft.com/en-us/windows/desktop/api/Nsemail/ns-nsemail-napi_provider_installation_blob_tag)|包含为NS_EMAIL命名空间安装命名空间提供程序所需的信息。|
|[**NS_SERVICE_INFO**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/ns-nspapi-_ns_service_infoa)|包含有关指定命名空间或一组默认命名空间的上下文中的网络服务或网络服务类型的信息。|
|[**PROTOCOL_INFO**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/ns-nspapi-_protocol_infoa)|包含有关协议的信息。|
|[**protoent**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/ns-winsock-protoent)|包含与给定协议名称对应的名称和协议编号。|
|[**REAL_TIME_NOTIFICATION_SETTING_INPUT**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_real_time_notification_setting_input)|包含输入设置以应用TCP套接字的REAL_TIME_NOTIFICATION_CAPABILITY传输设置，该套接字与ControlChannelTrigger一起用于在Windows应用商店应用中接收后台网络通知|
|[**REAL_TIME_NOTIFICATION_SETTING_OUTPUT**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_real_time_notification_setting_input)|包含用于TCP套接字的REAL_TIME_NOTIFICATION_CAPABILITY传输设置的查询的输出设置，该套接字与ControlChannelTrigger一起用于在Windows应用商店应用中接收后台网络通知。|
|[**RIO_EXTENSION_FUNCTION_TABLE**](https://msdn.microsoft.com/en-us/library/Hh437226(v=VS.85).aspx)|包含有关实现Winsock注册的I / O扩展的函数的信息。|
|[**RIO_BUF**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsockdef/ns-mswsockdef-_rio_buf)|指定用于通过Winsock注册的I / O扩展发送或接收网络数据的已注册缓冲区的一部分。|
|[**RIO_BUFFERID**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/rio-bufferid)|指定与Winsock注册的I / O扩展一起使用的已注册缓冲区描述符。|
|[**RIO_CQ**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/riocqueue)|通过使用Winsock注册的I / O扩展的发送和接收请求指定用于I / O完成通知的完成队列描述符。|
|[**RIO_NOTIFICATION_COMPLETION**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/ns-mswsock-_rio_notification_completion)|指定与RIONotify函数一起使用的I / O完成方法，用于使用Winsock注册的I / O扩展发送或接收网络数据。|
|[**RIO_RQ**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/riorqueue)|指定发送和接收请求与Winsock注册的I / O扩展使用的套接字描述符。|
|[**RIORESULT**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsockdef/ns-mswsockdef-_rioresult)|包含用于指示与Winsock注册的I / O扩展一起使用的请求完成结果的数据。|
|[**RM_FEC_INFO**](https://docs.microsoft.com/en-us/windows/desktop/api/Wsrm/ns-wsrm-_rm_fec_info)|指定使用可靠多播的前向纠错（FEC）的设置。|
|[**RM_RECEIVER_STATS**](https://docs.microsoft.com/en-us/windows/desktop/api/Wsrm/ns-wsrm-_rm_receiver_stats)|提供可靠多播接收器会话的统计信息。|
|[**RM_SEND_WINDOW**](https://docs.microsoft.com/en-us/windows/desktop/api/Wsrm/ns-wsrm-_rm_send_window)|指定可靠多播发送窗口。|
|[**RM_SENDER_STATS**](https://docs.microsoft.com/en-us/windows/desktop/api/Wsrm/ns-wsrm-_rm_sender_stats)|提供可靠多播发送方会话的统计信息。|
|[**servent**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/ns-winsock-servent)|存储或返回给定服务名称的名称和服务编号。|
|[**SERVICE_ADDRESS**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/ns-nspapi-_service_address)|包含服务的地址信息。|
|[**SERVICE_ADDRESSES**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/ns-nspapi-_service_addresses)|包含一组SERVICE_ADDRESS数据结构。|
|[**SERVICE_INFO**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/ns-nspapi-_service_infoa)|包含有关网络服务或网络服务类型的信息。|
|[**SERVICE_TYPE_INFO_ABS**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/ns-nspapi-_service_type_info_absa)|包含有关网络服务类型的信息。|
|[**SERVICE_TYPE_VALUE_ABS**](https://docs.microsoft.com/en-us/windows/desktop/api/Nspapi/ns-nspapi-_service_type_value_absa)|包含有关网络服务类型值的信息。 该信息可能特定于命名空间。|
|[**sockaddr**](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/sockaddr-2)|包含套接字地址信息。 sockaddrstructure取决于所选的协议。 仅限IPv4;使用struct sockaddr_storage的来代替。|
|[**SOCKADDR_IRDA**](https://docs.microsoft.com/en-us/windows/desktop/api/Af_irda/ns-af_irda-_sockaddr_irda)|与IrDA套接字操作一起使用，由地址族AF_IRDA定义。|
|[**SOCKADDR_STORAGE**](https://msdn.microsoft.com/en-us/library/ms740504(v=VS.85).aspx)|存储套接字地址信息，并且足够大以存储IPv4或IPv6地址信息，从而促进协议族和协议版本的独立性。 使用此结构代替sockaddr结构。|
|[**SOCKET_ADDRESS**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-_socket_address)|存储特定于协议的地址信息。|
|[**SOCKET_ADDRESS_LIST**](https://msdn.microsoft.com/en-us/library/Aa385467(v=VS.85).aspx)|存储包含特定于协议的地址信息的SOCKET_ADDRESS结构数组。|
|[**SOCKET_PEER_TARGET_NAME**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_peer_target_name)|包含对等目标的IP地址和名称以及要在套接字上使用的安全协议的类型。|
|[**SOCKET_SECURITY_QUERY_INFO**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_security_query_info)|包含WSAQuerySocketSecurity函数返回的安全信息。|
|[**SOCKET_SECURITY_QUERY_TEMPLATE**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_security_query_template)|包含WSAQuerySocketSecurity函数使用的安全模板。|
|[**SOCKET_SECURITY_SETTINGS**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_security_settings)|指定套接字的通用安全性要求。|
|[**SOCKET_SECURITY_SETTINGS_IPSEC**](https://docs.microsoft.com/en-us/windows/desktop/api/Mstcpip/ns-mstcpip-_socket_security_settings_ipsec)|指定特定于IPsec的各种安全要求和设置。|
|[**timeval**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/ns-winsock-timeval)|用于指定时间值。 与Berkeley Software Distribution（BSD）文件Time.h相关联。|
|[**TRANSMIT_FILE_BUFFERS**](https://docs.microsoft.com/en-us/windows/desktop/api/mswsock/ns-mswsock-_transmit_file_buffers)|指定在TransmitFile传输操作期间在文件数据之前和之后传输的数据。|
|[**TRANSMIT_PACKETS_ELEMENT**](https://docs.microsoft.com/en-us/windows/desktop/api/Mswsock/ns-mswsock-_transmit_packets_element)|指定TransmitPackets函数要传输的单个数据元素。|
|[**TRANSPORT_SETTING_ID**](https://msdn.microsoft.com/en-us/library/JJ553485(v=VS.85).aspx)|指定SIO_APPLY_TRANSPORT_SETTING和SIO_QUERY_TRANSPORT_SETTING IOCTL用于应用或查询套接字传输设置的传输设置ID。|
|[**WSABUF**](https://docs.microsoft.com/en-us/windows/desktop/api/ws2def/ns-ws2def-_wsabuf)|允许创建或操作数据缓冲区。|
|[**WSACOMPLETION**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsacompletion)|指定对已注册命名空间进行的I / O控制调用的完成通知设置。|
|[**WSADATA**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/ns-winsock-wsadata)|包含有关Windows套接字实现的信息。|
|[**WSAMSG**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2def/ns-ws2def-_wsamsg)|存储有关已连接和未连接套接字的地址和可选控制信息。 与WSARecvMsg函数一起使用。|
|[**WSANAMESPACE_INFO**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsanamespace_infow)|包含命名空间提供程序的注册信息。|
|[**WSANAMESPACE_INFOEX**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsanamespace_infoexw)|包含命名空间提供程序的增强注册信息。|
|[**WSANETWORKEVENTS**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsanetworkevents)|存储套接字有关网络事件的内部信息。|
|[**WSANSCLASSINFO**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsansclassinfow)|为特定的Winsock命名空间提供单独的参数信息。|
|[**WSAOVERLAPPED**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsaoverlapped)|在重叠I / O操作的启动与其后续完成之间提供通信介质。|
|[**WSAPOLLFD**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-pollfd)|存储WSAPollfunction使用的套接字信息。|
|[**WSAPROTOCOL_INFO**](https://msdn.microsoft.com/en-us/library/ms741675(v=VS.85).aspx)|存储或检索给定协议的完整信息。|
|[**WSAPROTOCOL_INFOW**](https://msdn.microsoft.com/en-us/library/ms741675(v=VS.85).aspx)|存储或检索给定协议的完整信息。 协议名称表示为Unicode字符数组。|
|[**WSAPROTOCOLCHAIN**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsaprotocolchain)|包含计算协议链的目录条目标识符的计数列表。|
|[**WSAQUERYSET**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsaquerysetw)|提供有关给定服务的相关信息。|
|[**WSAQUERYSET2**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsaqueryset2w)|提供有关给定服务的相关信息。|
|[**WSASERVICECLASSINFO**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsaserviceclassinfow)|包含有关指定服务类的信息。|
|[**WSAVERSION**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/ns-winsock2-_wsaversion)|在Winsock中提供版本比较。|



