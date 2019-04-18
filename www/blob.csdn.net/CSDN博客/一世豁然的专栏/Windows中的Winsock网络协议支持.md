# Windows中的Winsock网络协议支持 - 一世豁然的专栏 - CSDN博客





2018年10月10日 16:00:51[一世豁然](https://me.csdn.net/Explorer_day)阅读数：137








Internet协议套件是企业网络和Internet上使用的主要网络协议。 Internet协议套件代表了大量分层网络协议。 Internet协议套件通常被称为TCP / IP，它基于套件中包含的两个最重要的协议：Internet协议（IP）和传输控制协议（TCP）。

IPv6和IPv4代表Internet协议的两个可用版本。 TCP是几种重要的网络服务之一，通常称为在IPv6和IPv4网络上运行的IP协议。 用户数据报协议（UDP）和Internet控制消息协议（ICMP）是在IPv6和IPv4网络上使用的其他重要IP协议。 可以在IPv6和IPv4网络上使用许多其他IP协议。

Windows套接字将每个网络协议套件视为唯一的地址族。 因此IPv6协议被认为是AF_INET6地址族，IPv4协议被认为是AF_INET地址族。 IPv6和IPv4协议支持使用各种分层IP协议，如TCP，UDP和ICMP。

Windows套接字最初设计用于向Windows添加对IPv4的支持。 但是，Windows套接字编程接口从一开始就设计为能够支持其他网络协议套件。 随着时间的推移，Windows和相关Windows套接字的版本包括对其他网络协议套件的本机支持（例如，IPX / SPX和AppleTalk）。 作为供应商提供的第三方软件，Windows版本也支持其他网络协议。

在互联网的发展和普及之前，在网络环境中使用各种其他网络协议套件，特别是对于本地内部网。 网络协议套件的选择通常基于网络规模或IT网络人员的专业知识。 凭借今天的全球互联网连接，即使是最小的网络也可以连接到世界其他地方，IPv6和IPv4的网络专业知识对于网络专业人士来说至关重要。 结果，其他先前重要的网络协议套件现在使用非常有限并且已经被消除。 从最近版本的Microsoft Windows中删除了对这些经过验证的网络协议套件（通常称为旧版网络协议）的本机支持。 对这些传统协议中的一些的支持可以作为来自供应商的第三方软件（例如，具有ATM网络硬件的ATM）提供。

下表标识了对常见网络协议套件的本机Windows支持。
|Network Protocol|Windows 7|Windows Server 2008|Windows Vista|Windows Server 2003|Windows XP|Windows 2000|
|----|----|----|----|----|----|----|
|IPv6|支持|支持|支持|支持|支持|Not supported (see Notes)|
|IPv4|支持|支持|支持|支持|支持|支持|
|NetBIOS (see Notes)|支持|支持|支持|支持|支持||
|IrDA (see Notes)|支持|支持|支持|支持|支持|支持|
|Bluetooth (see Notes)|支持|支持|支持|支持|支持|不支持|
|IPX/SPX|不支持|不支持|不支持|支持|支持|支持|
|AppleTalk|不支持|不支持|不支持|支持|支持|支持|
|DLC|不支持|不支持|不支持|不支持 (see Notes)|不支持(see Notes)|支持|
|ATM|不支持|不支持|不支持|支持 (see Notes)|不支持 (see Notes)|支持(see Notes)|
|NetBEUI|不支持|不支持|不支持|不支持|不支持|支持 (see Notes)|

Windows 2000上的IPv6：Windows 2000 Service Pack 1（SP1）以及更高版本的Windows 2000 Microsoft IPv6技术预览版支持IPv6协议。

NetBIOS：NetBIOS协议通常用于Windows上的命名服务。 NetBIOS可以使用多种网络协议套件，包括IP（TCP / IP上的NetBIOS），IPX / SPX和NetBEUI。 Winsock仅支持32位版本的Windows 7，Windows Server 2008和Windows Vista上的TCP / IP上的NetBIOS（通常称为NetBT）。 Winsock在Windows Server 2003和Windows XP上使用IPX支持TCP / IP上的NetBIOS和NetBIOS。 Winsock支持TCP / IP上的NetBIOS，使用IPX的NetBIOS和Windows 2000上使用NetBEUI的NetBIOS。

IrDA：如果计算机安装了红外端口和驱动程序，则支持红外数据协会（IrDA）协议。

蓝牙：Winsock支持蓝牙作为网络协议套件，包括蓝牙个人局域网（PAN）和拨号网络（DUN）配置文件。 Windows中的蓝牙支持还包括使用蓝牙人机接口设备（HID）和其他配置文件连接键盘，指点设备和其他与网络协议无关的输入设备。

Windows 2003和Windows XP上的DLC：安装了Microsoft Host Integration Server 2006，Host Integration Server 2004或Host Integration Server 2000附带的DLC驱动程序时，Windows Server 2003和Windows XP支持数据链接控制（DLC）协议。

Windows 2003，Windows XP和Windows 2000上的ATM：安装ATM网络适配器时，Windows Server 2003，Windows XP和Windows 2000支持异步传输模式（ATM）协议。 经典IP over ATM（有时缩写为CLIP / ATM）的协议在RFC 2225和IETF发布的相关文档中定义。 Windows Server 2003，Windows XP和Windows 2000提供了此标准的完整实现。

Windows 2000上的NetBEUI：Windows套接字不直接支持NetBEUI协议。 但是，可能使用多个网络协议的NetBIOS协议支持在Windows 2000上使用NetBEUI协议。



**相关话题**

[ATM技术参考](http://go.microsoft.com/fwlink/p/?linkid=193472)

蓝牙

[适用于Windows 2000的IPv6技术预览版](http://go.microsoft.com/fwlink/p/?linkid=193479)

[IrDA](https://msdn.microsoft.com/en-us/library/ms691773(v=VS.85).aspx)

[Windows中的NDIS 5.0和ATM支持](http://go.microsoft.com/fwlink/p/?linkid=193474)





