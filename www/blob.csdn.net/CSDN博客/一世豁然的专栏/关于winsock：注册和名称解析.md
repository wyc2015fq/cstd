# 关于winsock：注册和名称解析 - 一世豁然的专栏 - CSDN博客





2018年10月12日 10:28:24[一世豁然](https://me.csdn.net/Explorer_day)阅读数：48








Windows套接字2是一组功能，用于标准化应用程序访问和使用各种网络命名服务的方式。 使用这些功能时，应用程序无需区分与名称服务相关的广泛不同的协议，如DNS，NIS，X.500，SAP等。要保持与Windows套接字1.1的完全向后兼容性，现有的getXbyY和异步WSAAsyncGetXbyY数据库查找 功能继续受支持，但在新的名称解析功能方面在Windows套接字服务提供程序接口中实现。 有关更多信息，请参阅getservbyname和getservbyport函数。 另请参阅Windows套接字1.1 SPI中的TCP / IP兼容名称解析。

本节介绍Winsock开发人员可用的注册和名称解析功能。 以下列表描述了本节中的主题：
- 与协议无关的名称解析
- Windows套接字1.1 API中TCP / IP的兼容名称解析



**一、与协议无关的名称解析**

在开发独立于协议的客户端/服务器应用程序时，在名称解析和注册方面存在两个基本要求：
- 服务器的一半应用程序（服务）在一个或多个名称空间内注册（或可访问）的能力。
- 客户端应用程序在命名空间内查找服务并获取所需传输协议和寻址信息的能力。

对于那些习惯于开发基于TCP / IP的应用程序的人来说，这似乎只涉及查找主机地址然后使用商定的端口号。 但是，其他网络方案允许在运行时发现服务的位置，用于服务的协议以及其他属性。 为了适应现有名称服务中广泛的功能，Windows Sockets 2接口采用本节主题中描述的模型。

本节介绍Winsock开发人员可用的与协议无关的名称解析功能。 以下列表描述了本节中的主题：
- 名称解析模型
- 名称解析功能摘要
- 名称解析数据结构



一）、名称解析模型

命名空间指的是将网络服务的协议和寻址属性与一个或多个友好名称相关联（至少）的一些功能。 目前，许多名称空间广泛使用，包括Internet的域名系统（DNS），Active Directory域服务，平构数据库，Novell的NetWare目录服务（NDS）和X.500。 这些名称空间在组织和实施方式上差异很大。 从Winsock名称解析的角度来看，它们的一些属性尤为重要。

1、命名空间的类型

有三种不同类型的命名空间可以注册服务：
- 动态
- 静态的
- 一贯

动态命名空间允许服务在运行时向命名空间注册，并允许客户端在运行时发现可用的服务。 动态命名空间经常依赖于广播来指示网络服务的持续可用性。 较旧的动态命名空间示例包括NetWare环境中使用的服务广告协议（SAP）命名空间和AppleTalk使用的名称绑定协议（NBP）命名空间。 Windows上使用的对等名称解析协议（PNRP）命名空间是动态命名空间的最新示例。

静态命名空间要求提前注册所有服务，即创建命名空间时。 静态命名空间的一个示例是大多数TCP / IP实现使用的主机，协议和服务文件。 在Windows上，这些文件通常位于C：\ windows \ system32 \ drivers \ etc文件夹中。

持久命名空间允许服务在运行时向命名空间注册。然而，与动态命名空间不同，持久命名空间将注册信息保留在非易失性存储中，直到服务请求删除它为止。持久命名空间以X.500和NDS（NetWare目录服务）等目录服务为代表。这些环境允许添加，删除和修改服务属性。另外，表示目录服务内的服务的服务对象可以具有与服务相关联的各种属性。客户端应用程序最重要的属性是服务的寻址信息。 DNS是持久命名空间的另一个示例。虽然有一种使用Windows套接字解析DNS名称的编程方法，但Windows中的DNS命名空间提供程序不支持使用Winsock注册新的DNS名称。您必须直接使用DNS功能来注册DNS名称。有关更多信息，请参阅DNS参考。


2、命名空间组织

许多名称空间按层次排列。 某些（例如X.500和NDS）允许无限制嵌套。 其他人允许将服务组合成单个级别的层次结构或组。 这通常称为工作组。 构造查询时，通常需要在命名空间层次结构中建立一个上下文点，搜索将从该层次结构开始。



3、命名空间提供程序架构

当然，用于查询各种类型的命名空间以及在命名空间内注册信息（如果支持）的编程接口差别很大。 命名空间提供程序是本地驻留的软件，它知道如何在Winsock命名空间SPI和某些现有命名空间（可以在本地实现或通过网络访问）之间进行映射。 命名空间提供程序体系结构说明如下：

![](https://img-blog.csdn.net/20181011145007875?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

请注意，给定命名空间（例如DNS）可能在给定计算机上安装了多个命名空间提供程序。

如上所述，通用术语服务指的是客户端/服务器应用程序的服务器的一半。在Winsock中，服务与服务类相关联，并且特定服务的每个实例都具有服务名称，该服务名称在服务类中必须是唯一的。服务类的示例包括FTP服务器，SQL Server，XYZ Corp.员工信息服务器等。如示例试图说明的那样，一些服务类是众所周知的，而其他服务类是特定的垂直应用程序是唯一的和特定的。在任何一种情况下，每个服务类都由类名和类标识符表示。类名不一定必须是唯一的，但类标识符必须是唯一的。全局唯一标识符（GUID）用于表示服务类标识符。对于众所周知的服务，已经预先分配了类名和类标识符（GUID），并且宏可用于在例如TCP端口号（以主机字节顺序）和相应的类标识符GUID之间进行转换。对于其他服务，开发人员选择类名并使用Uuidgen.exe实用程序为类标识符生成GUID。

存在服务类的概念以允许建立由特定服务的所有实例共同保持的一组属性。 在将服务类定义到Winsock时提供这组属性，并将其称为服务类模式信息。 当服务安装并在主机上可用时，该服务被视为实例化，并且其服务名称用于将服务的特定实例与命名空间可能已知的其他实例区分开。

请注意，服务类的安装只需要在执行服务的计算机上进行，而不是在可能使用该服务的所有客户机上进行。 在可能的情况下，Ws2_32.dll在注册服务的实例化或启动服务查询时向命名空间提供程序提供服务类模式信息。 当然，Ws2_32.dll本身并不存储此信息，而是尝试从已指示其提供此数据的能力的命名空间提供程序中检索它。 由于无法保证Ws2_32.dll可以提供服务类模式，因此需要此信息的命名空间提供程序必须具有回退机制才能通过特定于命名空间的方式获取它。

如上所述，因特网采用了可称为以主机为中心的服务模型。 需要定位服务的传输地址的应用程序通常必须首先解析已知承载服务的特定主机的地址。 他们在这个地址添加了众所周知的端口号，从而创建了一个完整的传输地址。 为了便于解析主机名，已预先分配了一个特殊的服务类标识符（SVCID_HOSTNAME）。 如果查询成功，则将SVCID_HOSTNAME指定为服务类并指定服务实例名称的主机名的查询将返回主机地址信息。

在Windows套接字2中，独立于协议的应用程序应该避免需要理解传输地址的内部细节。 因此，首先获得主机地址然后添加端口的需要是有问题的。 为了避免这种情况，查询还可以包括特定服务的众所周知的名称以及服务运行的协议，例如FTP over TCP。 在这种情况下，成功的查询将在指定的主机上返回指定服务的完整传输地址，并且不需要应用程序检查sockaddr结构的内部。

Internet的域名系统没有明确定义的方法来存储服务类架构信息。 因此，Winsock的DNS命名空间提供程序只能容纳已预先分配了服务类GUID的众所周知的TCP / IP服务。

实际上，这不是一个严重的限制，因为服务类GUID已经预先分配给整个TCP和UDP端口集，并且宏可用于检索与任何TCP或UDP端口关联的GUID，其端口以主机字节顺序表示。 因此，所有熟悉的服务，如HTTP，FTP，Telnet，Whois等都得到了很好的支持。

继续我们的服务类示例，FTP服务的实例名称可能是“alder.intel.com”或“ftp.microsoft.com”，而XYZ Corp.员工信息服务器的实例可能被命名为“XYZ Corp.员工信息” 服务器版本3.5“。

在前两种情况下，FTP的服务类GUID和计算机名称（作为服务实例名称提供）的组合唯一地标识所需的服务。 在第三种情况下，可以在服务查询时发现服务所在的主机名，因此服务实例名称不需要包含主机名。



二）、名称解析功能摘要

名称解析功能可以分为三类：服务安装，客户端查询和帮助程序（带宏）。 以下各节确定了每个类别中的功能，并简要描述了它们的用途。 还描述了关键数据结构。



1、服务安装
- [**WSAInstallServiceClass**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsainstallserviceclassa)
- [**WSARemoveServiceClass**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaremoveserviceclass)
- [**WSASetService**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsasetservicea)

当所需的服务类尚不存在时，应用程序使用WSAInstallServiceClass通过提供服务类名称，服务类标识符的GUID和一系列WSANSCLASSINFO结构来安装新的服务类。 这些结构各自特定于特定命名空间，并提供常见值，例如推荐的TCP端口号或NetWare SAP标识符。 可以通过调用WSARemoveServiceClass并提供与类标识符对应的GUID来删除服务类。

存在服务类后，可以通过WSASetService安装或删除特定的服务实例。 此函数将WSAQUERYSET结构作为输入参数以及操作代码和操作标志。 操作代码指示是否正在安装或删除服务。 WSAQUERYSET结构提供有关服务的所有相关信息，包括服务类标识符，服务名称（对于此实例），适用的命名空间标识符和协议信息，以及服务侦听的一组传输地址。 服务在初始化时应调用WSASetService以在动态命名空间中公布它们的存在。


2、客户查询
- [**WSAEnumNameSpaceProviders**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaenumnamespaceprovidersa)
- [**WSALookupServiceBegin**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupservicebegina)
- [**WSALookupServiceNext**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupservicenexta)
- [**WSALookupServiceEnd**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsalookupserviceend)

WSAEnumNameSpaceProviders函数允许应用程序发现可通过Winsock名称解析工具访问哪些名称空间。 它还允许应用程序确定多个命名空间提供程序是否支持给定的命名空间，以及发现任何特定命名空间提供程序的提供程序标识符。 使用提供程序标识符，应用程序可以将查询操作限制为指定的命名空间提供程序。

Winsock命名空间查询操作涉及一系列调用：WSALookupServiceBegin，然后是对WSALookupServiceNext的一次或多次调用，最后是对WSALookupServiceEnd的调用。 WSALookupServiceBegin将WSAQUERYSET结构作为输入来定义查询参数以及一组标志，以提供对搜索操作的额外控制。 它返回一个查询句柄，用于后续调用WSALookupServiceNext和WSALookupServiceEnd。

应用程序调用WSALookupServiceNext来获取查询结果，并在应用程序提供的WSAQUERYSET缓冲区中提供结果。 应用程序继续调用WSALookupServiceNext，直到返回错误代码WSA_E_NO_MORE，表明已检索到所有结果。 然后通过调用WSALookupServiceEnd终止搜索。 当从另一个线程调用时，WSALookupServiceEnd函数还可用于取消当前挂起的WSALookupServiceNext。

在Windows套接字2中，为WSAENOMORE（10102）和WSA_E_NO_MORE（10110）定义了冲突的错误代码。 将来的版本中将删除错误代码WSAENOMORE，并且只保留WSA_E_NO_MORE。 但是，对于Windows套接字2，应用程序应检查WSAENOMORE和WSA_E_NO_MORE，以便尽可能与使用其中任何一个的命名空间提供程序兼容。


3、助手功能
- [**WSAGetServiceClassNameByClassId**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsagetserviceclassnamebyclassida)
- [**WSAAddressToString**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsaaddresstostringa)
- [**WSAStringToAddress**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsastringtoaddressa)
- [**WSAGetServiceClassInfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Winsock2/nf-winsock2-wsagetserviceclassinfoa)

名称解析辅助函数包括一个函数，用于在给定服务类标识符的情况下检索服务类名称，一对用于在SOCKADDR结构和ASCII字符串表示之间转换传输地址的函数，一个用于检索服务类模式信息的函数。 给定的服务类，以及一组用于将众所周知的服务映射到预分配的GUID的宏。

Winsock2.h中的以下宏有助于在众所周知的服务类和这些名称空间之间进行映射：
|宏|描述|
|----|----|
|SVCID_TCP(Port) SVCID_UDP(Port)			SVCID_NETWARE(Object Type)|给定TCP / IP或UDP / IP的端口或NetWare的对象类型，返回GUID（主机顺序的端口号）。|
|IS_SVCID_TCP(GUID)IS_SVCID_UDP(GUID)			IS_SVCID_NETWARE(GUID)|如果GUID在允许范围内，则返回TRUE。|
|SET_TCP_SVCID(GUID, port)SET_UDP_SVCID(GUID, port)|使用与TCP或UDP端口号等效的GUID初始化GUID结构（端口号必须按主机顺序排列）。|
|PORT_FROM_SVCID_TCP(GUID)PORT_FROM_SVCID_UDP(GUID)			SAPID_FROM_SVCID_NETWARE(GUID)|返回与GUID关联的端口或对象类型（主机顺序中的端口号）。|



三）、名称解析数据结构

在名称解析功能中广泛使用了几种重要的数据结构。



1、与查询相关的数据结构

WSAQUERYSET结构用于形成WSALookupServiceBegin的查询，并用于为WSALookupServiceNext提供查询结果。 它是一个复杂的结构，因为它包含指向其他几个结构的指针，其中一些结构还引用了其他结构。 WSAQUERYSET结构与其引用的结构之间的关系如下所示。

![](https://img-blog.csdn.net/20181011150025258?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在WSAQUERYSET结构中，大多数成员都是自解释的，但有些成员应该得到额外的解释。 必须始终使用sizeof（WSAQUERYSET）填充dwSize成员，因为命名空间提供程序使用它来检测和适应可能随时间出现的WSAQUERYSET结构的不同版本。

命名空间提供程序使用dwOutputFlags成员来提供有关查询结果的其他信息。 有关详细信息，请参阅WSALookupServiceNext函数。

lpversion成员引用的WSAECOMPARATOR结构用于查询约束和结果。 对于查询，dwVersion成员指示所需的服务版本。 ecHow成员是枚举类型，它指定如何进行比较。 选项是COMP_EQUALS，它要求版本完全匹配，或者COMP_NOTLESS指定服务的版本号不小于dwVersion成员的值。

dwNameSpace和lpNSProviderId的解释取决于结构的使用方式，并在使用此结构的各个函数描述中进一步描述。

lpszContext成员应用于分层命名空间，并指定查询的起始点或服务所在的层次结构中的位置。 一般规则是：
- 值NULL，空白（“”）在默认上下文中开始搜索。
- 值“\”开始命名空间顶部的搜索。
- 任何其他值都会在指定点开始搜索。

如果指定了除“”或“\”以外的任何内容，则不支持包含的提供程序可能会返回错误。 支持有限限制的提供程序（如组）应接受“”，“\”或指定的点。上下文是特定于命名空间的。如果dwNameSpace成员是NS_ALL，则只应将“”或“\”作为上下文传递 因为这些都被所有名称空间识别。

lpszQueryString成员用于提供其他特定于命名空间的查询信息，例如描述已知服务和传输协议名称的字符串，如“FTP / TCP”中所示。

lpafpProtocols成员引用的AFPROTOCOLS结构仅用于查询目的，并提供协议列表以约束查询。 这些协议表示为（地址族，协议）对，因为协议值仅在地址族的上下文中具有含义。

lpcsaBuffer成员引用的CSADDR_INFO结构数组包含服务用于建立侦听所需的所有信息，或者客户端用于建立与服务的连接所需的所有信息。 LocalAddr和RemoteAddr成员都直接包含SOCKET_ADDRESS结构。

服务将通过使用LocalAddr.lpSockaddr-> sa_family，iSocketType和iProtocol的元组作为参数调用套接字或WSASocket函数来创建套接字。 通过使用LocalAddr.lpSockaddr和LocalAddr.lpSockaddrLength作为参数调用bind函数，服务将套接字绑定到本地地址。

客户端通过使用LocalAddr.lpSockaddr-> sa_family，iSocketType和iProtocol的元组作为参数调用套接字或WSASocket函数来创建其套接字。 在使用connect，ConnectEx或WSAConnect函数建立远程连接时，客户端使用RemoteAddr.lpSockaddr和RemoteAddr.lpSockaddrLength的组合作为参数。



2、服务类数据结构

安装新服务类时，必须准备并提供WSASERVICECLASSINFO结构。 此结构还包含子结构，其中包含一系列适用于特定名称空间的成员。 类信息数据结构如下：

![](https://img-blog.csdn.net/20181011150408370?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对于每个服务类，都有一个WSASERVICECLASSINFO结构。 在WSASERVICECLASSINFO结构中，服务类的唯一标识符包含在lpServiceClassId成员中，并且相关的显示字符串由lpServiceClassName成员引用。 这是WSAGetServiceClassNameByClassId函数返回的字符串。

WSASERVICECLASSINFO结构中的lpClassInfos成员引用WSANSCLASSINFO结构数组，每个结构都提供一个适用于指定命名空间的命名和类型成员。 lpszName成员的值示例包括：“SapId”，“TcpPort”，“UdpPort”等。这些字符串通常特定于dwNameSpace成员中标识的命名空间。 dwValueType成员的典型值可能是REG_DWORD，REG_SZ等.dwValueSize成员指示lpValue指向的数据项的长度。

在调用WSAInstallServiceClass函数时，将为每个命名空间提供程序提供WSASERVICECLASSINFO结构中表示的整个数据集合。 然后，每个单独的命名空间提供程序都会筛选WSANSCLASSINFO结构列表，并保留适用于它的信息。





二、Windows套接字1.1 API中TCP / IP的兼容名称解析

> 
注意

用于名称解析的所有Windows套接字1.1函数都特定于IPv4 TCP / IP网络。 强烈建议应用程序开发人员继续使用仅支持IPv4的这些特定于传输的功能。


应用程序开发人员应使用以下与协议无关的功能，并支持IPv6和IPv4名称解析。
- [**getaddrinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfo)
- [**GetAddrInfoEx**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfoexa)
- [**GetAddrInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getaddrinfow)
- [**getnameinfo**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getnameinfo)
- [**GetNameInfoW**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2tcpip/nf-ws2tcpip-getnameinfow)

Windows套接字1.1定义了许多用于使用TCP / IP（IP版本4）网络进行名称解析的例程。 这些有时称为getXbyY函数，包括以下内容：

[**gethostname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-gethostname)
[**gethostbyaddr**](https://msdn.microsoft.com/en-us/library/ms738521(v=VS.85).aspx)
[**gethostbyname**](https://msdn.microsoft.com/en-us/library/ms738524(v=VS.85).aspx)
[**getprotobyname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getprotobyname)
[**getprotobynumber**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getprotobynumber)
[**getservbyname**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getservbyname)
[**getservbyport**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-getservbyport)

还定义了这些函数的异步版本。

[**WSAAsyncGetHostByAddr**](https://msdn.microsoft.com/en-us/library/ms741519(v=VS.85).aspx)
[**WSAAsyncGetHostByName**](https://msdn.microsoft.com/en-us/library/ms741522(v=VS.85).aspx)
[**WSAAsyncGetProtoByName**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsaasyncgetprotobyname)
[**WSAAsyncGetProtoByNumber**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsaasyncgetprotobynumber)
[**WSAAsyncGetServByName**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsaasyncgetservbyname)
[**WSAAsyncGetServByPort**](https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-wsaasyncgetservbyport)

现在还有两个函数，现在在Winsock2.dll中实现，用于将点分别的Ipv4地址符号分别转换为字符串和二进制表示。

[**inet_addr**](https://msdn.microsoft.com/en-us/library/ms738563(v=VS.85).aspx)
[**inet_ntoa**](https://msdn.microsoft.com/en-us/library/ms738564(v=VS.85).aspx)

为了保持与Windows套接字1.1的严格向后兼容性，只要至少有一个支持AF_INET地址系列的命名空间提供程序（这些功能与IP版本6无关），就会继续支持所有旧的仅支持IPv4的功能。 ，由AF_INET6表示。

Ws2_32.dll使用适当的WSALookupServiceBegin / Next / End函数调用序列，在新的，与协议无关的名称解析工具方面实现这些兼容性函数。 下面提供了如何将getXbyY函数映射到名称解析函数的详细信息。 WSs2_32.dll处理getXbyY函数的异步和同步版本之间的差异，因此仅讨论了同步getXbyY函数的实现。

本节介绍Windows Sockets 1.1 API中TCP / IP的兼容名称解析。 以下列表描述了本节中的主题：
- API中GetXbyY的基本方法
- API中的getprotobyname和getprotobynumber 函数
- API中的getservbyname和getservbyport函数
- API中的gethostbyname 函数
- API中的gethostbyaddr 函数
- API中的gethostname函数



一）、API中GetXbyY的基本方法

大多数getXbyY函数由Ws2_32.dll转换为WSALookupServiceBegin，WSALookupServiceNext和WSALookupServiceEnd序列，该序列使用一组特殊GUID作为服务类。 这些GUID标识正在模拟的getXbyY操作的类型。 该查询仅限于支持AF_INET的名称服务提供程序。 每当getXbyY函数返回HOSTENT或SERVENT结构时，Ws2_32.dll都会在WSALookupServiceBegin中指定LUP_RETURN_BLOB标志，以便名称服务提供程序返回所需的信息。 必须稍微修改这些结构，因为必须用相对于blob数据开始的偏移替换其中包含的指针。 当然，这些指针参数引用的所有值必须完全包含在blob中，并且所有字符串都是ASCII。



二）、API中的getprotobyname和getprotobynumber 函数

getprotobyname和getprotobynumber函数通过查询本地协议数据库在Ws2_32.dll中实现。 它们不会导致任何名称解析查询。



三）、API中的getservbyname和getservbyport函数

getservbyname和getservbyport函数使用WSALookupServiceBegin函数来查询SVCID_INET_SERVICEBYNAME作为服务类GUID。 传递给WSALookupServiceBegin函数的WSAQUERYSET结构中的lpszServiceInstanceName成员引用字符串以指示服务名称或服务端口，以及（可选）服务协议。 字符串的格式说明为FTP或TCP或21 / TCP或只是FTP。 字符串不区分大小写。 斜杠标记（如果存在）将协议标识符与字符串的前一部分分开。 Ws2_32.dll将指定LUP_RETURN_BLOB，命名空间提供程序将在Blob中放置一个SERVENT结构（使用偏移而不是如上所述的指针）。 命名空间提供程序也应该遵循这些其他LUP_RETURN_ *标志。
|Flag|描述|
|----|----|
|LUP_RETURN_NAME|从lpszServiceInstanceName中的SERVENT结构返回s_name成员。|
|LUP_RETURN_TYPE|返回lpServiceClassId中的规范GUID可以理解，根据本地建立的约定，标识为FTP或21的服务可以在另一个端口上。 SERVENT结构的s_port参数应指示在本地环境中可以联系服务的位置。 设置LUP_RETURN_TYPE时返回的规范GUID应该是来自Svcs.h的预定义GUID之一，它对应于SERVENT结构中指示的端口号。|



四）、API中的gethostbyname 函数

gethostbyname函数使用WSALookupServiceBegin函数来查询SVCID_INET_HOSTADDRBYNAME作为服务类GUID。 主机名在传递给WSALookupServiceBegin函数的WSAQUERYSET结构中的lpszServiceInstanceName成员中提供。 Ws2_32.dll指定LUP_RETURN_BLOB，名称服务提供者在blob中放置HOSTENT结构（使用偏移而不是如上所述的指针）。 名称服务提供商也应该遵守这些其他LUP_RETURN_ *标志。
|Flag|Description|
|----|----|
|LUP_RETURN_NAME|从lpszServiceInstanceName中的HOSTENT结构返回h_name成员。|
|LUP_RETURN_ADDR|从CSADDR_INFO结构中的HOSTENT返回寻址信息，端口信息默认为零。 请注意，此例程不会解析由虚线IPv4地址组成的主机名。|



五）、API中的gethostbyaddr 函数

gethostbyaddr函数使用WSALookupServiceBegin函数将SVCID_INET_HOSTNAMEBYADDR作为服务类GUID进行查询。 主机地址在传递给WSALookupServiceBegin函数的WSAQUERYSET结构的lpszServiceInstanceName成员中作为虚线的decimnal IPv4字符串（例如，“192.9.200.120”）提供。 Ws2_32.dll指定LUP_RETURN_BLOB，名称服务提供者在blob中放置HOSTENT结构（使用偏移而不是如上所述的指针）。 名称服务提供商也应该遵守这些其他LUP_RETURN_ *标志。
|Flag|描述|
|----|----|
|LUP_RETURN_NAME|从lpszServiceInstanceName中的HOSTENT结构返回h_name成员。|
|LUP_RETURN_ADDR|从CSADDR_INFO结构中的HOSTENT返回寻址信息，端口信息默认为零。|



六）、API中的gethostname函数

gethostname函数使用WSALookupServiceBegin函数将SVCID_HOSTNAME作为服务类GUID进行查询。如果传递给WSALookupServiceBegin函数的WSAQUERYSET结构的lpszServiceInstanceName成员为NULL或引用NULL字符串（即“。”），则将解析本地主机。否则，将发生对指定主机名的查找。出于模拟gethostname的目的，Ws2_32.dll为lpszServiceInstanceName成员指定NULL指针，并指定LUP_RETURN_NAME以便在lpszServiceInstanceName成员中返回主机名。如果应用程序使用此查询并指定LUP_RETURN_ADDR，则主机地址在CSADDR_INFO结构中提供。此查询未定义LUP_RETURN_BLOB操作。除非传递给WSALookupServiceBegin函数的WSAQUERYSET结构的lpszQueryString成员引用诸如FTP之类的服务，否则端口信息默认为零，在这种情况下，提供指示服务的完整传输地址。




