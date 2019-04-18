# 使用Winsock：组播编程 - 一世豁然的专栏 - CSDN博客





2018年10月17日 22:04:49[一世豁然](https://me.csdn.net/Explorer_day)阅读数：237








通过Windows套接字启用多播编程。 Windows套接字通过使用套接字选项或IOCTL，在IPv6和Internet组管理协议版本2（IGMPv2）和3（IGMPv3）上启用多播侦听器发现（MLD）版本1（MLDv1）和2（MLDv2）。 本节介绍Windows实现，解释如何使用Windows套接字启用多播编程，并提供编程示例以说明其用法。

IGMP的第二版本（以下称为IGMPv2）使主机能够加入和离开由特定网络接口上的IPv4多播地址标识的多播组。 Windows套接字使应用程序能够在特定套接字上加入和离开这些组。 然而，IGMPv2的缺点是加入IGMPv2组的任何IPv4源地址都可以传输给所有成员，可能使该组泛滥并使其不能用于需要主要源的传输，例如因特网无线电台。 IGMPv2的问题在于它无法选择性地选择单个IPv4源地址（甚至几个源），并且无法阻止给定多播组的发送者（例如流氓广播者或拒绝服务的犯罪者）。 IGMPv3解决了这些缺点。

使用Windows套接字和IGMPv3，应用程序可以选择特定的多播IPv4源地址和多播组对。 此外，Windows套接字使开发人员能够有选择地允许给定源/组对中的其他广播公司，或使应用程序能够阻止特定的广播公司。 Windows Vista及更高版本支持IGMPv3。

IPv6上的第一个版本的MLD（称为MLDv1）与IGMPv2非常相似，并且受到相同的限制。 MLDv1允许主机在特定网络接口上加入和离开由IPv6多播地址标识的多播组。 Windows套接字使应用程序能够在特定套接字上加入和离开这些组。 但是，加入MLDv1组的任何IPv6源地址都可以传输给所有成员，可能会使该组泛滥并使其无法用于需要主源的传输。 MLDv1的问题在于它无法选择性地选择单个IPv6源地址（甚至是几个源），并且无法阻止给定多播组的发送者（例如流氓广播者或拒绝服务的犯罪者）。 MLDv2解决了这些缺点。

使用Windows套接字和MLDv2，应用程序可以选择特定的多播IPv6源地址和多播组对。 此外，Windows套接字使开发人员能够有选择地允许给定源/组对中的其他广播公司，或使应用程序能够阻止特定的广播公司。 Windows Vista及更高版本支持MLDv2。

在Windows中开发多播应用程序时，应用程序员可以采用两种方法。 第一种方法是基于变化的; 根据需要，即使在传输过程中，也可以使用套接字选项添加或删除组播源。 第二种方法是基于最终状态; 源地址和任何包含/排除的地址都使用IOCTL指定。 每种方法都是有效的多播实践，但开发人员可能会发现使用套接字选项和基于更改的方法更直观，更灵活。






一、MLD和IGMP使用Windows套接字

Windows套接字通过使用套接字选项和IOCTL，启用IPv6上的多播侦听器发现（MLD）和IPv4上的Internet组管理协议（IGMP）以用于多播应用程序。 此页面描述了启用多播编程的套接字选项，并描述了它们的使用方式。 有关每个套接字选项的定义，请参阅“套接字选项”页面。

有关使用IOCTL进行多播编程的信息，请参阅本节后面的基于最终状态的多播编程。

在Windows Vista及更高版本中，一组套接字选项可用于支持IPv6和IPv4地址的多播编程。 这些套接字选项与IP无关，可以在IPv6和IPv4上使用。 在IPv6上，这些套接字选项使用MLDv2。 在IPv4上，这些套接字选项使用IGMPv3。 这些与IP无关的选项是Windows Vista及更高版本上多播编程的首选套接字选项。 Windows套接字使用以下套接字选项：
|Socket option|Argument type|
|----|----|
|MCAST_BLOCK_SOURCE|[**GROUP_SOURCE_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_source_req) structure|
|MCAST_JOIN_GROUP|[**GROUP_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_req) structure|
|MCAST_JOIN_SOURCE_GROUP|[**GROUP_SOURCE_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_source_req) structure|
|MCAST_LEAVE_GROUP|[**GROUP_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_req) structure|
|MCAST_LEAVE_SOURCE_GROUP|[**GROUP_SOURCE_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_source_req) structure|
|MCAST_UNBLOCK_SOURCE|[**GROUP_SOURCE_REQ**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_source_req) structure|

一组套接字选项可用于支持仅IPv6地址的多播编程。 这些套接字选项使用MLDv1或MLDv2。 使用的MLD版本取决于Windows的版本。 Windows Vista及更高版本支持MLDv2。 Windows套接字使用以下套接字选项：
|Socket option|Argument type|
|----|----|
|IPV6_ADD_MEMBERSHIP|[**ipv6_mreq**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ipv6_mreq) structure|
|IPV6_DROP_MEMBERSHIP|[**ipv6_mreq**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ipv6_mreq) structure|

一组套接字选项可用于支持仅IPv4地址的多播编程。 这些套接字选项使用IGMPv3或IGMPv2。 使用的IGMP版本取决于Windows的版本。 Windows Vista及更高版本支持IGMPv3。 Windows套接字使用以下套接字选项：
|Socket option|Argument type|
|----|----|
|IP_ADD_MEMBERSHIP|[**ip_mreq**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_mreq) structure|
|IP_ADD_SOURCE_MEMBERSHIP|[**ip_mreq_source**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_mreq_source) structure|
|IP_BLOCK_SOURCE|[**ip_mreq_source**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_mreq_source) structure|
|IP_DROP_MEMBERSHIP|[**ip_mreq**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_mreq) structure|
|IP_DROP_SOURCE_MEMBERSHIP|[**ip_mreq_source**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_mreq_source) structure|
|IP_UNBLOCK_SOURCE|[**ip_mreq_source**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_mreq_source) structure|

当IGMPv3可用时，由于路由器可以处理过滤，因此可以更有效地处理IP_ADD_SOURCE_MEMBERSHIP，IP_BLOCK_SOURCE，IP_DROP_SOURCE_MEMBERSHIP和IP_UNBLOCK_SOURCE选项。 只有IGMPv2可用时，主机必须处理过滤。

大多数应用程序可能分为两类：任意源和受控源。
- 任何源应用程序默认接受所有源，允许根据需要关闭各个源。 任何源应用程序的示例是允许所有收件人参与的视频会议呼叫。
- 受控源应用程序将源限制为给定列表，例如Internet广播站，或广播显着事件。 使用套接字选项的过程对于每个选项略有不同。

在Windows Vista及更高版本中，以下步骤适用于任何源应用程序：

使用** MCAST \ _ _ _ JOIN \ _GROUP **加入群组。 如果需要，请使用** MCAST \ _BLOCK \ _SOURCE **关闭给定的源。 如果需要，使用** MCAST \ _UNBLOCK \ _SOURCE **重新允许被阻止的源。 使用** MCAST \ _LEAVE \ _GROUP **离开该组。

在Windows Vista及更高版本中，以下步骤适用于受控源应用程序：

使用** MCAST \ _JOIN \ _SOURCE \ _GROUP **加入每个组/源对。 如果所有源使用相同的组地址，请使用** MCAST \ _LEAVE \ _SOURCE \ _GROUP **离开每个组/源，或使用** MCAST \ _LEAVE \ _GROUP **保留所有源。

以下步骤适用于任何源应用程序：

使用** IP \ _ADD \ _MEMBERSHIP **加入组（** IPV6 \ _ADD \ _MEMBERSHIP **用于IPv6）。 如果需要，使用** IP \ _BLOCK \ _SOURCE **关闭给定的源。 如果需要，使用** IP \ _UNBLOCK \ _SOURCE **重新允许被阻止的源。 使用** IP \ _DROP \ _MEMBERSHIP **离开组（** IPV6 \ _DROP \ _MEMBERSHIP **用于IPv6）。

以下步骤适用于受控源应用程序：

使用** IP \ _ADD \ _SOURCE \ _MEMBERSHIP **加入每个组/源对。 如果所有源使用相同的组地址，请使用** IP \ _DROP \ _SOURCE \ _MEMBERSHIP **保留每个组/源，或使用** IP \ _DROP \ _MEMBERSHIP **保留所有源。

设置这些套接字选项的顺序具有关联规则。 有关多播套接字选项的信息和故障排除信息，请参阅多播套接字选项行为。





二、组播套接字选项行为

此页面描述了基于各种套接字选项设置状态的多播套接字选项的行为。

例如，此页面描述了在已在同一网络接口上使用指定的组/源对设置了IP_ADD_SOURCE_MEMBERSHIP选项的套接字上设置IP_ADD_SOURCE_MEMBERSHIP套接字选项时的行为。 允许在不同网络接口上的同一组上调用IP_ADD_SOURCE_MEMBERSHIP。

此页面有助于正确设计和排除Windows套接字多播应用程序。
|初始化套接字选项|冲突的沟洫套接字选项|错误返回|备注|
|----|----|----|----|
|IP_ADD_MEMBERSHIP${REMOVE}$|IP_ADD_MEMBERSHIP|WSAEADDRNOTAVAIL|不要在同一网络接口上多次使用同一组调用IP_ADD_MEMBERSHIP。|
|IP_ADD_SOURCE_MEMBERSHIP|WSAEADDRNOTAVAIL|不要使用先前在同一网络接口上使用IP_ADD_MEMBERSHIP调用的同一组调用IP_ADD_SOURCE_MEMBERSHIP。| |
|IP_DROP_SOURCE_MEMBERSHIP|WSAEINVAL|请改用IP_BLOCK_SOURCE。| |
|IP_UNBLOCK_SOURCE|WSAEINVAL|尝试取消阻止先前未在同一网络接口上阻止的组/源对时返回错误。| |
|IP_DROP_MEMBERSHIP|Any subsequent call on the same group or group/source pair|WSAEINVAL|对当前不在包含列表中的组或组/源对进行套接字选项调用（由于成员资格丢失或其他原因）导致错误。|
|IP_ADD_SOURCE_MEMBERSHIP${REMOVE}$|IP_ADD_MEMBERSHIP|WSAEADDRNOTAVAIL|不要使用先前在同一网络接口上使用IP_ADD_SOURCE_MEMBERSHIP调用的同一组调用IP_ADD_MEMBERSHIP。|
|IP_ADD_SOURCE_MEMBERSHIP|WSAEADDRNOTAVAIL|不要使用先前在同一网络接口上使用IP_ADD_SOURCE_MEMBERSHIP调用的相同组/源对调用IP_ADD_SOURCE_MEMBERSHIP。| |
|IP_UNBLOCK_SOURCE|WSAEINVAL|尝试取消阻止先前未在同一网络接口上阻止的组/源对时返回错误。| |
|IP_DROP_SOURCE_MEMBERSHIP${REMOVE}$|IP_UNBLOCK_SOURCE|WSAEINVAL|尝试取消阻止先前未在同一网络接口上阻止的组/源对时返回错误。|
|IP_DROP_SOURCE_MEMBERSHIP|WSAEADDRNOTAVAIL|尝试删除不在同一网络接口上的包含列表中的组/源对时返回错误。| |
|IP_BLOCK_SOURCE${REMOVE}$|IP_BLOCK_SOURCE|WSAEADDRNOTAVAIL|尝试阻止已在同一网络接口上阻止的组/源对时返回错误。|
|IP_ADD_SOURCE_MEMBERSHIP|WSAEINVAL|请改用IP_UNBLOCK_SOURCE。| |
|IP_ADD_MEMBERSHIP|WSAEINVAL|请改用IP_UNBLOCK_SOURCE。| |
|IP_UNBLOCK_SOURCE|IP_UNBLOCK_SOURCE|WSAEADDRNOTAVAIL|尝试取消阻止同一网络接口上不在阻止列表中的组/源对时返回错误。|





三、多播编程示例

以下示例代码说明了如何使用套接字选项将多播功能包含到Windows套接字应用程序中。

```cpp
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <mswsock.h>

#define u_int32 UINT32  // Unix uses u_int32

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")


int                  /* OUT: whatever setsockopt() returns */
join_source_group(int sd, u_int32 grpaddr, 
   u_int32 srcaddr, u_int32 iaddr)
{
   struct ip_mreq_source imr; 
   
   imr.imr_multiaddr.s_addr  = grpaddr;
   imr.imr_sourceaddr.s_addr = srcaddr;
   imr.imr_interface.s_addr  = iaddr;
   return setsockopt(sd, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char *) &imr, sizeof(imr));  
}

int
leave_source_group(int sd, u_int32 grpaddr, 
   u_int32 srcaddr, u_int32 iaddr)
{
   struct ip_mreq_source imr;

   imr.imr_multiaddr.s_addr  = grpaddr;
   imr.imr_sourceaddr.s_addr = srcaddr;
   imr.imr_interface.s_addr  = iaddr;
   return setsockopt(sd, IPPROTO_IP, IP_DROP_SOURCE_MEMBERSHIP, (char *) &imr, sizeof(imr));
}
```





四、基于最终状态的组播编程

本节介绍使用IOCTL而不是套接字选项的基于最终状态的多播编程。 有关基于最终状态的多播编程与基于更改的多播编程的不同之处的概述，请参阅多播编程。

下表描述了用于Windows上多播编程的Windows套接字IOCTL。
|IOCTL|Argument type|
|----|----|
|SIOCSMSFILTER|[**GROUP_FILTER**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_filter) structure|
|SIOCGMSFILTER|[**GROUP_FILTER**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-group_filter) structure|
|SIO_GET_MULTICAST_FILTER|[**ip_msfilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_msfilter) structure|
|SIO_SET_MULTICAST_FILTER|[**ip_msfilter**](https://docs.microsoft.com/en-us/windows/desktop/api/Ws2ipdef/ns-ws2ipdef-ip_msfilter) structure|

请注意，SIOCSMSFILTER和SIOCGMSFILTER IOCTLS在Windows Vista及更高版本上可用。

使用这些IOCTL进行多播编程在处理大型源列表时具有性能优势。 有关与使用SIO_GET_MULTICAST_FILTER或SIO_SET_MULTICAST_FILTER相关的参数和设置的更多信息，请参阅GROUP_FILTER参考页。 有关与使用SIO_GET_MULTICAST_FILTER或SIO_SET_MULTICAST_FILTER相关的参数和设置的详细信息，请参阅ip_msfilter参考页。




五、将广播应用程序移植到IPv6

本节介绍将IPv6广播应用程序移植到Windows套接字可用的多播功能的最佳实践。

**将IPv4与IPv6进行比较**

准备将IPv4广播应用程序移植到IPv6时最值得注意的考虑因素是：IPv6没有实现广播的概念。 相反，IPv6使用多播。

IPv6组播可以模拟IPv4中的传统广播功能。 设置IPV6_ADD_MEMBERSHIP套接字选项，并将IPv6地址设置为链路本地范围，所有节点地址（FF02 :: 1）等效于使用SO_BROADCAST套接字选项在IPv4广播地址上进行广播。 该地址有时称为全节点组播组。 对于只想要IPv6广播仿真的应用程序，该方法在操作上是等效的。 然而，与IPv6的一个显着区别是，默认情况下不接收全节点多播组地址上的多播（默认接收IPv4广播）。 应用程序编程人员必须使用IPV6_ADD_MEMBERSHIP套接字选项来启用来自任何源的多播接收，包括全节点多播组地址。

> 
注意

在IPv6中，接口选择在传递给多播套接字选项或IOCTL的参数结构中指定。


但是，为了向多个主机传输更丰富，更强大，更具选择性和更易管理的应用程序，应用程序开发人员应考虑转向多播模型。



**转向多播**

通过多播，应用程序员可以选择性地选择特定的源/组对，从而实现选择性接收模型。 IPv6上的多播侦听器发现（MLD）和IPv4上的Internet组管理协议（IGMPv3）版本3支持多播编程。 此外，多播使应用程序能够专门阻止（或解除阻塞）组内的发送者，从而进一步保护应用程序免受流氓广播公司的攻击。 此功能适用于IPv4（需要IGMPv3）以及IPv6（需要MLDv2）。

应用程序员使用多播有两种主要方案：从IPv4广播（或多播）应用程序移植到IPv6，以及创建新IPv6组播应用程序的方案。

对于移植现有应用程序，有两种选择可以转移到IPv6多播：使用套接字选项和使用IOCTL。
- 使用套接字选项是一种基于更改的方法，它使开发人员能够根据需要更改套接字属性（例如阻止或取消阻止发件人，添加新源等）。 这种方法更直观，也是推荐的方法。 有关基于更改的多播编程方法的更多信息，请参阅使用Windows套接字的MLD和IGMP。
- 使用IOCTL是一种基于最终状态的方法，因为它使开发人员能够通过一次调用提供完全配置的套接字状态，包括包含和排除列表。 有关基于最终状态的方法的更多信息，请参阅基于最终状态的多播编程。
- 

对于那些创建新IPv6组播应用程序的人，建议的做法是使用套接字选项，而不是使用IOCTL。

还有另一种使用IPv6创建多播应用程序的方法，这需要使用WSAJoinLeaf函数。 虽然使用WSAJoinLeaf函数不是推荐的做法，但有些情况可能会指示其使用。 例如，在Windows Server 2003及更早版本上使用套接字选项的一个缺点是它们是特定于IP版本的。 在这些旧版本的Windows上，不同的套接字选项必须适用于IPv6和IPv4。 在Windows Vista及更高版本中，支持可与IPv4和IPv6一起使用的新套接字选项。 相反，WSAJoinLeaf函数是IP版本和协议不可知的，因此它可以是一种有用的方法，用于构建必须在Windows Server 2003及更早版本上使用多个IP版本的应用程序。 在需要协议和IP版本不可知性的某些情况下，使用WSAJoinLeaf函数可能更合适。





