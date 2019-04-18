# 关于winsock：Internet协议版本6（IPv6） - 一世豁然的专栏 - CSDN博客





2018年10月12日 11:49:04[一世豁然](https://me.csdn.net/Explorer_day)阅读数：405标签：[Windows Sokets2](https://so.csdn.net/so/search/s.do?q=Windows Sokets2&t=blog)
个人分类：[Windows](https://blog.csdn.net/Explorer_day/article/category/6772518)









确保新的Winsock应用程序以及现有应用程序与IPv6完全兼容非常重要。 2011年，亚洲和太平洋地区用于新IPv4地址分配的IPv4地址空间的可用性已经用尽。预计世界其他地区将在几年内耗尽。

使用IPv6地址可以获得越来越多的新网站和服务。 新兴市场中的许多互联网用户依赖IPv6进行互联网访问。

微软长期致力于支持IPv6。 Windows XP Service Pack 1（SP1）及更高版本提供完整的IPv6支持。

本文档提供有关Winsock对IPv6的支持的信息：
- 使用IPv6
- 使用Internet Explorer访问IPv6网站
- 推荐的IPv6配置
- 其他IPv6主题

有关向Windows套接字应用程序添加IPv6功能的其他信息，请参阅“适用于Windows套接字应用程序的IPv6指南”。

有关IPv6协议的介绍以及有关部署和IPv6过渡技术的概述，可在Microsoft Internet协议版本6（IPv6）的Technet上获得。





一、使用IPv6

在Windows XP上，稍后会提供一个新的命令行工具来配置和管理IPv4。 此工具使用“netsh interface ip”命令配置和管理IPv4。

在带有Service Pack 1（SP1）及更高版本的Windows XP上，此新命令行工具已得到增强，可支持IPv6的配置和管理。 这个增强的工具是“netsh interface ipv6”命令。 使用Netsh.exe命令进行的配置更改是永久性的，并且在重新启动计算机或IPv6协议时不会丢失。

在Windows XP SP1及更高版本上可以使用以下命令在本地计算机上查询和配置IPv6：
- [Netsh.exe](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/netsh-exe)

在带有Service Pack 1（SP1）的Windows XP之前，IPv6配置和管理使用了几个较旧的命令行工具来配置和管理IPv6。 使用这些较旧的工具，IPv6更改不是永久性的，并且在重新启动计算机或IPv6协议时会丢失。

Windows XP上提供以下旧命令
- [Net.exe](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/net-exe-2)
- [Ipv6.exe](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/ipv6-exe-2)
- [Ipsec6.exe](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/ipsec6-exe-2)

这些旧工具也在用于Windows 2000的IPv6技术预览中提供

使用这些旧工具进行的配置更改可以通过将它们作为命令行放在重新启动计算机或IPv6协议后运行的命令脚本文件（.cmd）中来维护。 要在重新启动后自动恢复配置更改，可以使用Windows计划任务在计算机启动时运行.cmd文件。

Windows Server 2003及更高版本不提供这些旧工具。 “netsh interface ipv6”工具用于在Windows Server 2003及更高版本上从命令行配置和管理IPv6。



一）、netsh.exe

IPv6的Netsh命令提供了一个命令行工具，可用于查询和配置IPv6接口，地址，高速缓存和路由。 Windows XP Service Pack 1（SP1）及更高版本支持Netsh接口IPv6命令。

Netsh.exe有许多IPv6子命令。 可以从Windows XP SP1及更高版本的命令提示符处获得Netsh Interface IPv6的完整选项列表，方法是键入以下内容：

**netsh interface ipv6 /?**

有关IPv6的所有netsh命令的文档也可在Technet上在线获取。 有关Windows Server 2008上netsh的更多信息，请参阅接口的Netsh命令（IPv4和IPv6）。 有关Windows Server 2003上的netsh的更多信息，请参阅接口IPv6的Netsh命令。

以下是一些常用的IPv6命令，但支持许多其他命令：

**netsh interface ipv6 add address**

将IPv6地址添加到本地计算机上的特定接口。 此命令具有必须指定的子选项参数。

**netsh interface ipv6 add dns**

将DNS服务器IPv6地址添加到本地计算机上指定接口的静态配置的DNS服务器列表中。 此命令具有必须指定的子选项参数。

**netsh interface ipv6 add route**

在本地计算机上添加指定IPv6前缀地址的路由。 此命令具有必须指定的子选项参数。

**netsh interface ipv6 delete address**

从本地计算机上的特定接口删除指定的IPv6地址。 此命令具有必须指定的子选项参数。

**netsh interface ipv6 delete dns**

从静态配置的本地计算机上指定接口的DNS服务器列表中删除DNS服务器地址。 此命令具有必须指定的子选项参数。

**netsh interface ipv6 delete route**

删除本地计算机上指定IPv6前缀地址的路由。 此命令具有必须指定的子选项参数。

**netsh interface ipv6 dump**

创建一个脚本，其中包含用于创建当前配置的命令。 如果保存到文件，则此脚本可用于恢复更改的配置设置。

**netsh interface ipv6 install**

在本地计算机上安装IPv6协议。

**netsh interface ipv6 renew**

重新启动本地计算机上的IPv6接口。

**netsh interface ipv6 reset**

重置本地计算机上的IPv6配置状态。

**netsh interface ipv6 show global**

显示本地计算机上IPv6的全局配置参数。

**netsh interface ipv6 show address**

显示本地计算机上特定接口上的所有IPv6地址或所有IPv6地址。 此命令具有可能需要指定的子选项参数。

**netsh interface ipv6 uninstall**

在本地计算机上卸载IPv6协议。



**IPv4的Netsh命令**

类似的Netsh命令可用于IPv4。 可以从Windows XP SP1及更高版本的命令提示符处获得用于IPv4的Netsh命令的完整选项列表，方法是键入以下内容：

**netsh interface ip /?**

有关IPv4的所有Netsh命令的文档也可在Technet上在线获取。 有关更多信息，请参阅接口IP的Netsh命令



二）、net.exe

Net.exe可用于停止和启动IPv6协议。 重新启动IPv6会重新初始化协议，就像计算机正在重新启动一样，这可能会更改接口号。 Net.exe有许多子命令。 以下命令与IPv6相关：

**net stop tcpip6**

停止IPv6协议并从内存中卸载它。 如果打开任何IPv6套接字，此命令将失败。

**net start tcpip6**

如果已停止，则启动IPv6协议。 如果％systemroot％\ System32 \ Drivers目录中存在新的Tcpip6.sys驱动程序文件，则会加载该驱动程序文件。



三）、IPv6.exe

所有IPv6配置都使用Ipv6.exe工具完成。 该工具主要用于查询和配置IPv6接口，地址，缓存和路由。 以下是IPv6子命令：

**ipv6 if** [if#]

显示有关接口的信息。 如果指定了接口编号，则仅显示有关该接口的信息。 否则，将显示有关所有接口的信息。 输出包括接口的链路层地址和分配给接口的IPv6地址列表，包括接口的当前MTU和接口可以支持的最大（真实）MTU。

接口＃1是用于环回的伪接口。 接口＃2是用于配置隧道，自动隧道和6to4隧道的伪接口。 其他接口按创建顺序依次编号。 此订单因计算机而异。

aa-bb-cc-dd-ee-ff格式的链路层地址是以太网地址。 a.b.c.d形式的链路层地址是6-over-4接口。 有关6-over-4的更多信息，请参阅RFC 2529。

这两个伪接口不使用IPv6邻居发现。

**ipv6 ifc** if# [forwards] [advertises] [-forwards] [-advertises] [mtu #bytes] [site site-identifier]

控制界面属性。 接口可以转发，在这种情况下，它们转发目的地址未分配给接口的数据包。 接口可以是广告，在这种情况下，它们发送路由器广告。 这些属性可以独立控制。 接口发送路由器请求并接收路由器通告，或接收路由器请求并发送路由器通告。

由于两个伪接口不使用邻居发现，因此无法将它们配置为发送路由器通告。

前锋可以缩写为forw，并作为adv公布。

也可以设置接口的MTU。 新MTU必须小于或等于链路的最大（真实）MTU（由ipv6 if报告）并且大于或等于最小IPv6 MTU（1280字节）。

接口的站点标识符也可以更改。 站点标识符在sin6_scope_id字段中与站点本地地址一起使用。

**ipv6 ifd** if#

删除接口。 无法删除环回和隧道伪接口。

**ipv6 nc** [if# [address]]

显示邻居缓存的内容。 如果指定了接口编号，则仅显示该接口的邻居缓存的内容。 否则，将显示所有接口的邻居缓存的内容。 如果指定了接口，则可以指定IPv6地址以仅显示该邻居缓存条目。

对于每个邻居缓存条目，将显示接口，IPv6地址，链路层地址和到达状态。

**ipv6 ncf** [if# [address]]

刷新指定的邻居缓存条目。 仅清除没有引用的邻居缓存条目。 由于路由缓存条目包含对邻居缓存条目的引用，因此应首先刷新路由缓存。 路由表条目还可以保存对邻居缓存条目的引用。

**ipv6 rc** [if# address]

显示路由缓存的内容。 路由缓存是目标缓存的Microsoft IPv6实现名称。 如果指定了接口和地址，则显示通过接口到达地址的路由缓存条目。 否则，将显示所有路由缓存条目。

对于每个路由缓存条目，将显示IPv6地址和当前下一跳接口以及邻居地址。 还显示与此目标一起使用的首选源地址，通过接口到达此目的地的当前路径MTU，以及确定这是否是接口特定路由缓存条目。 还显示目的地地址的任何转交地址（用于移动性）。

目标地址可以有多个路由缓存条目 - 每个传出接口多达一个。 但是，目标地址最多可以包含一个非特定于接口的路由缓存条目。 仅当应用程序明确指定传出接口时，才使用接口特定路由高速缓存条目。

**ipv6 rcf** [if# [address]]

刷新指定的路由缓存条目。

**ipv6 bc**

显示绑定缓存的内容，该缓存包含移动IPv6的归属地址和转交地址之间的绑定。

对于每个绑定，显示归属地址，转交地址，绑定序列号和生存期。

**ipv6 adu** if#/address [lifetime VL[/PL]] [anycast] [unicast]

在接口上添加或删除单播或任播地址分配。 除非指定任播，否则将对单播地址采取行动。

如果没有指定，寿命是无限的。 如果仅指定了有效生命周期，则首选生命周期等于有效生命周期。 可以指定无限寿命，或以秒为单位的有限值。 首选生命周期必须小于或等于有效生命周期。 指定生命周期为零会导致地址被删除。

你可以将生命缩短为生命。

对于任播地址，唯一有效的生命周期值为零和无限。

**ipv6 spt**

显示站点前缀表的当前内容。

对于每个站点前缀，该命令显示前缀，应用站点前缀的接口以及以秒为单位的前缀生存期。

站点前缀通常由路由器通告自动配置。 它们在getaddrinfo函数中用于过滤不适当的站点本地地址。

**ipv6 spu** prefix if# [lifetime L]

添加，删除或更新站点前缀表中的前缀。

前缀和接口号是必需的。 如果未指定，则以秒为单位指定的站点前缀生存期默认为无限。 指定生命周期为零会删除站点前缀。

对于主机或路由器的正常配置，此命令是不必要的。

**ipv6 rt**

显示路由表的当前内容。

对于每个路由表条目，该命令显示路由前缀，接口上的链路上接口或下一跳邻居，首选项值（首选较小）和生命周期（秒）。

路由表条目还可以具有发布和老化属性。 默认情况下，它们会老化（生命周期倒计时，而不是保持不变）并且不会发布（不用于构建路由器通告）。

在主机上，路由表条目通常是从路由器通告自动配置的。

**ipv6 rtu** prefix if#[/nexthop] [lifetime L] [preference P] [publish] [age] [spl site-prefix-length]

在路由表中添加或删除路由。 路由前缀是必需的。 前缀可以链接到指定接口，也可以是接口上使用邻居地址指定的下一跳。 路径可以具有以秒为单位的生命周期，默认为无限，以及首选项，默认值为零，或者最优选。 指定生命周期为零会删除路由。

如果路由指定为已发布，表明它将用于构建路由器通告，则它不会老化。 路由的生命周期不会倒计时，因此实际上是无限的，但该值用于路由器通告。 可选地，可以将路线指定为也已老化的已发布路线。 默认情况下，未发布的路由始终会老化。

可选的spl子选项可用于指定与路由关联的站点前缀长度。 站点前缀长度仅在发送路由器通告时使用。

生命周期可以缩写为生命，偏好作为pref，并发布为pub。



四）、ipsec6.exe

使用Ipsec6.exe配置IPv6的IPSec策略和安全关联。 以下是Ipsec6.exe子命令：

**ipsec6 sp [***Interface***]**

显示活动安全策略。 或者，显示特定接口的活动安全策略。

**ipsec6 sa**

显示活动的安全关联。

**ipsec6 c [***FileName(without extension)***]**

创建用于配置安全策略和安全关联的文件。 为安全策略创建FileName.spd，为安全关联创建FileName.sad。 使用创建的文件作为模板来配置安全策略或安全关联。 可以使用文本编辑器修改文件。 要调用SPD和SAD文件中包含的配置，请使用ipsec6 a命令。

**ipsec6 a [***FileName(without extension)***]**

将FileName.spd中的安全策略和FileName.sad中的安全关联添加到活动安全策略和安全关联列表中。

**ipsec6 i [***Policy***] [***FileName(without extension)***]**

将FileName.spd中的安全策略和FileName.sad中的安全关联添加到策略编号引用的活动安全策略和安全关联列表中。

**ipsec6 d [type = sp sa] [***IndexNumber***]**

从活动安全策略和安全关联列表中删除安全策略或安全关联，由其索引号引用（使用ipsec6 sp或ipsec6 sa显示）。





二、使用Internet Explorer访问IPv6网站

在满足以下条件时，Microsoft Internet Explorer支持使用IPv6连接和访问启用IPv6的站点（例如，Web服务器和FTP服务器）：
- 运行Internet Explorer的主机必须已安装并启用IPv6。

连接到本地子网外部的主机时，提供连接的接口必须分配可路由的IPv6地址。

连接到IPv6环回地址（:: 1）或链接本地目标时，不需要可路由的IPv6地址。
- 如果请求的URL包含名称（例如，www.contoso.com），则该名称的域名系统（DNS）查询必须返回一个或多个IPv6地址。
- 如果请求的URL包含IPv6地址（例如:: 1），则IPv6地址必须用方括号括起来（http：// \ [:: 1）。 作为IPv6地址的一部分，支持作用域ID（有时称为区域索引）。 范围ID用于确定在具有多个网络接口的计算机上发送请求时要使用的网络接口。 范围ID使用主IPv6地址后面的'％'字符指定（例如，fe80 :: 1％11）。 但是，必须在Internet Explorer使用的URL中转义'％'字符。 例如，链接本地地址上的范围ID 11将在使用Internet Explorer时指定为http：// \ [fe80 :: 1％2511。 Internet Explorer 7及更高版本支持在URL中使用IPv6地址的功能。

> 
注意

如果Internet Explorer配置为使用代理服务器，则代理服务器必须分配IPv6地址，并且代理服务器必须能够代理IPv6地址。 代理服务器将用于使用IPv6连接到外部主机。 通常会绕过IPv6环回地址和IPv6链路本地地址来代理服务器。






三、推荐的IPv6配置

以下是测试IPv6网络应用程序的推荐配置：
- 具有链路本地地址的单个子网
- IPv4互联网络的不同子网上节点之间的IPv6流量（6to4）
- 在两个本地链接主机之间使用IPsec



一）、配置1：具有链路本地地址的单个子网

除安装Microsoft IPv6技术预览协议外，第一个配置不需要其他配置。 此配置由同一子网上的至少两个节点组成。 在IPv6术语中，两个节点在同一链路上，没有中间路由器。

下图显示了使用链路本地地址在单个子网上配置两个节点。

![](https://img-blog.csdn.net/20181012105852647?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

默认情况下，IPv6为与已安装的以太网适配器对应的每个接口配置链路本地IP地址。 链路本地地址的前缀为fe80 :: / 64。 IPv6地址的最后64位称为接口标识符，它是从网络适配器的48位MAC地址派生的。

要从48位（6字节）以太网MAC地址创建IPv6接口标识符：
- 十六进制数字0xff-fe插入在MAC地址的第三和第四字节之间。
- 通用/本地位（MAC地址的第一个字节的第二个低位）被补充。 如果是1，则变为0，如果为0，则变为1。

例如，对于MAC地址00-60-08-52-f9-d8：
- 十六进制数字0xff-fe插入在MAC地址的0x08（第三字节）和0x52（第四字节）之间，形成64位地址00-60-08-ff-fe-52-f9-d8。
- 通用/本地位，MAC地址的第二个低位0x00（第一个字节）被补充。 0x00的第二个低位为0，当补码时，它变为1.结果是对于第一个字节，0x00变为0x02。

因此，对应于以太网MAC地址00-60-08-52-f9-d8的IPv6接口标识符为02-60-08-ff-fe-52-f9-d8。

节点的链路本地地址是前缀fe80 :: / 64和以IPv6冒号 - 十六进制表示法表示的64位接口标识符的组合。 因此，具有前缀fe80 :: / 64和接口标识符02-60-08-ff-fe-52-f9-d8的该示例节点的链路本地地址是fe80 :: 260：8ff：fe52：f9d8。

您可以使用ipv6查看链接本地地址，如下例所示：

**ipv6 if**

```
Interface 4 (site 1): Local Area Connection
  uses Neighbor Discovery
  link-level address: 00-10-5a-aa-20-a2
    preferred address fe80::210:5aff:feaa:20a2, infinite/infinite
    multicast address ff02::1, 1 refs, not reportable
    multicast address ff02::1:ffaa:20a2, 1 refs, last reporter
  link MTU 1500 (true link MTU 1500)
  current hop limit 128
  reachable time 43500ms (base 30000ms)
  retransmission interval 1000ms
  DAD transmits 1
Interface 3 (site 1): 6-over-4 Virtual Interface
  uses Neighbor Discovery
  link-level address: 10.0.0.2
    preferred address fe80::a00:2, infinite/infinite
    multicast address ff02::1, 1 refs, not reportable
    multicast address ff02::1:ff00:2, 1 refs, last reporter
  link MTU 1280 (true link MTU 65515)
  current hop limit 128
  reachable time 34000ms (base 30000ms)
  retransmission interval 1000ms
  DAD transmits 1
Interface 2 (site 0): Tunnel Pseudo-Interface
  does not use Neighbor Discovery
  link-level address: 0.0.0.0
    preferred address ::10.0.0.2, infinite/infinite
  link MTU 1280 (true link MTU 65515)
  current hop limit 128
  reachable time 0ms (base 0ms)
  retransmission interval 0ms
  DAD transmits 0
Interface 1 (site 0): Loopback Pseudo-Interface
  does not use Neighbor Discovery
  link-level address:
    preferred address ::1, infinite/infinite
  link MTU 1500 (true link MTU 1500)
  current hop limit 1
  reachable time 0ms (base 0ms)
  retransmission interval 0ms
  DAD transmits 0
```

接口4是对应于安装的以太网适配器的接口，其链路本地地址为fe80 :: 210：5aff：feaa：20a2。

有关IPv6寻址的更多信息以及IPv6概念的概述，请参阅IPv6简介白皮书。



**测试两个链路本地主机之间的连接**

您可以在两个链接本地主机之间使用IPv6执行简单的ping（交换ICMPv6 Echo请求和Echo Reply消息）。

在两个链路本地主机之间使用IPv6进行ping操作

1、在同一链接（子网）上的两个Windows主机（主机A和主机B）上安装Microsoft IPv6技术预览版Windows。

2、在Host A上使用ipv6获取以太网接口的链路本地地址。

示例：主机A的链路本地地址为fe80 :: 210：5aff：feaa：20a2。

3、在Host B上使用ipv6获取以太网接口的链路本地地址。

示例：主机B的链路本地地址为fe80 :: 260：97ff：fe02：6ea5。

4、从主机A，使用Ping6.exe ping主机B.

示例：ping6 fe80 :: 260：97ff：fe02：6ea5

要指定发送Echo Request消息的源地址，还可以使用Ping6.exe -s选项。 例如，要从fe80 :: 210：5aff：feaa：20a2的IPv6地址向主机B发送Echo请求，请使用以下命令：

**ping6 -s fe80::210:5aff:feaa:20a2%4 fe80::260:97ff:fe02:6ea5**

在ping本地链接或站点本地地址时，建议指定范围ID以使目标地址明确。 指定范围ID的表示法是地址％scope-ID。 对于链路本地地址，scope-ID等于ipv6 if命令中显示的接口号。 对于站点本地地址，scope-ID等于ipv6 if命令中显示的站点号。 例如，要使用范围ID 4向主机B发送Echo Request消息，请使用以下命令：

**ping6 fe80::260:97ff:fe02:6ea5%4**



二）、配置2：IPv4网络中不同子网上节点之间的IPv6流量（6to4）

6to4是一种通过现有IPv4 Internet基础结构连接IPv6主机或站点的方法。 它使用唯一的地址前缀为隔离的IPv6站点提供自己的IPv6地址空间。 6to4就像一个提供IPv6连接的“伪ISP”。 您可以使用6to4直接与其他6to4站点进行通信。 6to4不需要使用IPv6路由器，其IPv6流量使用IPv4报头封装。

下图显示了使用6to4通过IPv4路由器进行通信的不同子网上的两个节点的配置。

![](https://img-blog.csdn.net/20181012110752740?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0V4cGxvcmVyX2RheQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用6to4的主要要求是为您的站点提供一个可全局路由的IPv4地址。 假设您的站点包含您管理的一组IPv6计算机（一些运行Microsoft IPv6协议，另一些运行其他IPv6实现）。 还假设所有IPv6计算机都使用以太网或4-over-4直接连接。 必须将全局可路由的IPv4地址分配给运行Microsoft IPv6协议的其中一台计算机。 这台电脑将是你的6to4网关。

如果您的IPv4地址属于专用地址空间（10.0.0.0/8,172.16.0.0/12或192.168.0.0/16）或自动专用IP寻址（APIPA）地址空间为169.254.0.0/ 在Windows 2000中使用的是16，它不是全局可路由的。 否则，它可能是公共IP地址，并且可以全局路由。 有关确定ISP连接是否支持6to4的更多帮助，请参阅本文档中的“调试6to4配置”主题。



**6to4cfg.exe工具**

6to4cfg.exe工具可自动执行6to4配置，自动发现您的全局可路由IPv4地址并创建6to4前缀。 它可以直接执行配置，也可以编写一个可以检查并稍后运行的配置脚本。

基本的6to4cfg.exe命令语法如下。
`6to4cfg [-r] [-s] [-u] [-R relay] [-b] [-S address] [filename]`
[*filename*]

如果指定文件名，请将配置脚本写入文件。 配置脚本使用Ipv6.exe。

您可以为文件名指定con以将配置脚本写入控制台输出，这对于查看6to4cfg.exe在测试方案中将执行的操作非常有用。

如果未指定文件名，则6to4cfg.exe会直接更新计算机上的IPv6配置。

-r

成为本地网络的6to4网关路由器，支持在所有接口上路由并分配子网前缀。

-s

在6to4站点中启用站点本地寻址。 此命令仅在与-r一起使用时才有用。

-u

指定反转6to4配置。 因此6to4cfg -u逆转了6to4cfg和6to4cfg -r -u的效果，反转了6to4cfg -r的效果，依此类推。

-R *relay*

*指定6to4中继路由器的名称或IPv4地址。 默认名称为6to4.ipv6.microsoft.com，Internet上的6to4中继路由器。*

-b

指定6to4cfg.exe选择“最佳”中继地址而不是第一个。

-S *address*

*指定6to4前缀的本地IPv4地址。*



***手动6to4配置***

*在此示例中，6to4网关的地址为172.31.42.239。 您需要自己的全局可路由IPv4地址才能使用6to4。*

> 
注意

IP地址172.31.42.239仅用于示例目的。 这是一个私有地址，在Internet上无法全局路由。


32位全局可路由IPv4地址与16位前缀2002 :: / 16组合，形成站点的48位IPv6地址前缀。 在此示例中，6to4站点前缀为2002：ac1f：2aef :: / 48。 请注意，ac1f：2aef是172.31.42.239的十六进制编码（当然，您将根据自己的全局可路由IPv4地址使用不同的前缀）。 使用6to4站点前缀，您可以在站点内分配地址和子网前缀。

此示例假定您使用子网0在6to4网关计算机上手动配置6to4地址，并使用子网1自动配置以太网网段上的地址。 但是，其他选择也是可能的。

1、使用Ipv6.exe在6to4网关计算机上启用6to4：

**ipv6 rtu 2002::/16 2**

ipv6 rtu命令执行路由表更新。 它可用于添加，删除或更新路由。 在这种情况下，它启用6to4。

2002 :: / 16参数是路由的前缀，指定唯一的6to4前缀。

2参数指定此前缀的on-link接口。 接口＃2是用于配置隧道，自动隧道和6to4的“伪接口”。 当IPv6目标地址与2002 :: / 16前缀匹配时，将提取目标地址中前缀后面的32位以形成IPv4目标地址。 数据包使用IPv4标头封装并发送到IPv4目标地址。

2、在6to4网关计算机上配置6to4地址：

**ipv6 adu 2/2002:ac1f:2aef::ac1f:2aef**

ipv6 adu命令执行地址更新。 它可用于添加，删除或更新接口上的地址。 在这种情况下，它正在配置计算机的6to4地址。

2/2002：ac1f：2aef :: ac1f：2aef参数指定接口和地址。 它需要在接口＃2上配置地址2002：ac1f：2aef :: ac1f：2aef。 使用站点前缀2002：ac1f：2aef :: / 48，子网0创建地址以提供子网前缀2002：ac1f：2aef :: / 64，以及64位接口标识符。 演示的约定使用计算机的IPv4地址作为分配给接口＃2的地址的接口标识符。 为了您的使用，ac1f：2aef应该替换为您自己的全局可路由IPv4地址的十六进制编码。

前面两个命令足以允许与其他6to4站点进行通信。 例如，您可以尝试ping Microsoft 6to4站点：

**ping6 2002:836b:9820::836b:9820**

要启用与6bone的通信，必须为6to4中继创建默认配置的隧道。 您可以使用Microsoft 6to4中继路由器，131.107.152.32：

**ipv6 rtu ::/0 2/::131.107.152.32 pub life 1800**

ipv6 rtu命令执行路由表更新，在此实例中建立到6to4中继的默认路由。

:: / 0参数是路由前缀。 零长度前缀表示它是默认路由。

2 / :: 131.107.152.32参数指定此前缀的下一跳邻居。 它要求使用接口＃2将匹配前缀的数据包转发到地址:: 131.107.152.32。 在接口＃2上将数据包转发到:: 131.107.152.32会导致它被封装为v4标头并发送到131.107.152.32。

pub参数使这成为已发布的路由。 因为这仅与路由器相关，所以在启用路由之前它不起作用。 同样，30分钟的生命周期仅适用于启用路由的情况。

您应该能够访问6bone站点以及6to4站点。 您可以使用以下命令来测试：

**ping6 3ffe:1cff:0:f5::1**

最后一步是在6to4网关上启用路由。 此示例假定您的网关计算机上的接口＃3是以太网接口，接口＃4是4-over-4。 您的计算机可能会以不同的方式编号 以下两个命令将子网前缀分配给两个链接。 子网前缀源自站点的6to4前缀2002：ac1f：2aef :: / 48：

**ipv6 rtu 2002:ac1f:2aef:1::/64 3 pub life 1800**

**ipv6 rtu 2002:ac1f:2aef:2::/64 4 pub life 1800**

ipv6 rtu命令指定前缀2002：ac1f：2aef：1 :: / 64与接口＃3链接。 它在以太网接口上配置第一个子网前缀。 该路线的发布寿命为30分钟。

类似地，2002：ac1f：2aef：2 :: / 64前缀在6-over-4接口上配置。

接下来的三个命令使6to4网关计算机可以用作路由器：

**ipv6 ifc 2 forw**

**ipv6 ifc 3 forw adv**

**ipv6 ifc 4 forw adv**

ipv6 ifc命令控制接口的属性。 路由器转发数据包并发送路由器通告。 在Microsoft IPv6实现中，这些每个接口属性是分开控制的。

广告不需要接口＃2，因为它是伪接口。

如果您的计算机具有其他接口，则还应将它们配置为转发和广告。

运行这些命令后，Microsoft IPv6协议将使用相应的子网前缀自动配置接口＃3和＃4上的地址，并且这两个接口将以大约3到10分钟的间隔开始发送路由器通告。

接收这些路由器通告的主机将自动使用默认路由和从其链路的子网前缀派生的6to4地址进行自我配置。 他们将通过网关计算机与其他6to4站点和6bone进行通信。



**调试6to4配置**

1、检查您与6to4中继路由器的IPv4连接：

**ping 6to4.ipv6.microsoft.com**

如果此操作失败，则表示您没有全局Internet连接。

2、使用自动隧道检查IPv6封装：

**ping6 ::131.107.152.32**

如果此操作失败，则您的计算机和Internet之间可能有防火墙或网络地址转换程序（NAT）。 如果成功，则您的Internet连接可以支持6to4。

3、检查ipv6 rt命令的显示。 您应该看到2002 :: / 16 - > 2路线。 检查ipv6 if 2命令的显示。 您应该看到带有2002 :: / 16前缀的首选地址。

> 
注意

131.107.152.32的Microsoft 6to4中继路由器的IPv4地址可能会更改。 如果上面的步骤2不起作用，请检查步骤1中ping命令的输出，以验证Microsoft 6to4中继路由器的IPv4地址。




三）、配置3：在两个本地链路主机之间使用IPsec

此配置在同一子网上的两台主机之间创建IPsec安全关联（SA），以使用身份验证标头（AH）和消息摘要5（MD5）散列算法执行身份验证。 在此示例中，显示的配置可保护两个相邻主机之间的所有流量：主机1，链路本地地址FE80 :: 2AA：FF：FE53：A92C，主机2，链路本地地址FE80 :: 2AA：FF：FE92：D0F1。

在两个本地链接主机之间使用IPsec

1、在主机1上，使用ipsec6 c命令创建空白安全关联（SAD）和安全策略（SPD）文件。 在此示例中，Ipsec6.exe命令是ipsec6 c test。 这将创建两个文件来手动配置安全关联（Test.sad）和安全策略（Test.spd）。

2、在主机1上，编辑SPD文件以添加安全策略，以保护主机1和主机2之间的所有流量。

下表显示在此示例的第一个条目之前添加到Test.spd文件的安全策略（未修改Test.spd文件中的第一个条目）。
|SPD file field name|Example value|
|----|----|
|**Policy**|2|
|**RemoteIPAddr**|**FE80::2AA:FF:FE92:D0F1**|
|**LocalIPAddr**|*|
|**RemotePort**|*|
|**Protocol**|*|
|**LocalPort**|*|
|**IPSecProtocol**|**AH**|
|**IPSecMode**|**TRANSPORT**|
|**RemoteGWIPAddr**|*|
|**SABundleIndex**|**NONE**|
|**Direction**|**BIDIRECT**|
|**Action**|**APPLY**|
|**InterfaceIndex**|0|

在配置此安全策略的行末尾放置一个分号。 必须按递减的数字顺序放置策略条目。

3、在主机1上，编辑SAD文件，添加SA条目以保护主机1和主机2之间的所有流量。必须创建两个安全关联，一个用于到主机2的流量，一个用于来自主机2的流量。

下表显示了为此示例添加到Test.sad文件的第一个SA条目（用于到主机2的流量）。
|SAD file field name|Example value|
|----|----|
|**SAEntry**|2|
|**SPI**|3001|
|**SADestIPAddr**|**FE80::2AA:FF:FE92:D0F1**|
|**DestIPAddr**|**POLICY**|
|**SrcIPAddr**|**POLICY**|
|**Protocol**|**POLICY**|
|**DestPort**|**POLICY**|
|**SrcPort**|**POLICY**|
|**AuthAlg**|**HMAC-MD5**|
|**KeyFile**|**Test.key**|
|**Direction**|**OUTBOUND**|
|**SecPolicyIndex**|2|

在配置此SA的行的末尾放置一个分号。

下表显示了为此示例添加到Test.sad文件的第二个SA条目（来自主机2的流量）。
|SAD file field name|Example value|
|----|----|
|**SAEntry**|1|
|**SPI**|3000|
|**SADestIPAddr**|**FE80::2AA:FF:FE53:A92C**|
|**DestIPAddr**|**POLICY**|
|**SrcIPAddr**|**POLICY**|
|**Protocol**|**POLICY**|
|**DestPort**|**POLICY**|
|**SrcPort**|**POLICY**|
|**AuthAlg**|**HMAC-MD5**|
|**KeyFile**|**Test.key**|
|**Direction**|**INBOUND**|
|**SecPolicyIndex**|2|

在配置此SA的行的末尾放置一个分号。 SA条目必须按递减的数字顺序放置。

4、在主机1上，创建一个文本文件，其中包含用于对使用主机2创建的SA进行身份验证的文本字符串。在此示例中，创建文件Test.key，其内容为“This is a test”。 您必须在密钥字符串周围包含双引号，以便ipsec6工具读取密钥。

Microsoft IPv6技术预览版仅支持手动配置的密钥，用于验证IPsec SA。 通过创建包含手动密钥的文本字符串的文本文件来配置手动密钥。 在此示例中，SA的相同密钥用于两个方向。 您可以通过创建不同的密钥文件并使用SAD文件中的KeyFile字段引用它们，为入站和出站SA使用不同的密钥。

5、在主机2上，使用ipsec6 c命令创建空白安全关联（SAD）和安全策略（SPD）文件。 在此示例中，Ipsec6.exe命令是ipsec6 c test。 这将创建两个带有空白条目的文件，用于手动配置安全关联（Test.sad）和安全策略（Test.spd）。

为简化示例，在主机2上使用SAD和SPD文件的相同文件名。您可以选择在每个主机上使用不同的文件名。

6、在主机2上，编辑SPD文件以添加安全策略，以保护主机2和主机1之间的所有流量。

下表显示了在此示例的第一个条目Test.spd文件之前添加的安全策略条目（Test.spd文件中的第一个条目未被修改）。
|SPD file field name|Example value|
|----|----|
|**Policy**|2|
|**RemoteIPAddr**|**FE80::2AA:FF:FE53:A92C**|
|**LocalIPAddr**|*|
|**RemotePort**|*|
|**Protocol**|*|
|**LocalPort**|*|
|**IPSecProtocol**|**AH**|
|**IPSecMode**|**TRANSPORT**|
|**RemoteGWIPAddr**|*|
|**SABundleIndex**|**NONE**|
|**Direction**|**BIDIRECT**|
|**Action**|**APPLY**|
|**InterfaceIndex**|0|

在配置此安全策略的行末尾放置一个分号。 必须按递减的数字顺序放置策略条目。

7、在主机2上，编辑SAD文件，添加SA条目以保护主机2和主机1之间的所有流量。必须创建两个安全关联 - 一个用于到主机1的流量，一个用于来自主机1的流量。

下表显示了为此示例添加到Test.sad文件的第一个SA（对于来自主机1的流量）。
|SAD file field name|Example value|
|----|----|
|**SAEntry**|2|
|**SPI**|3001|
|**SADestIPAddr**|**FE80::2AA:FF:FE92:D0F1**|
|**DestIPAddr**|**POLICY**|
|**SrcIPAddr**|**POLICY**|
|**Protocol**|**POLICY**|
|**DestPort**|**POLICY**|
|**SrcPort**|**POLICY**|
|**AuthAlg**|**HMAC-MD5**|
|**KeyFile**|**Test.key**|
|**Direction**|**INBOUND**|
|**SecPolicyIndex**|2|

在配置此SA的行的末尾放置一个分号。

下表显示了为此示例添加到Test.sad文件的第二个SA条目（用于到主机1的流量）。
|SAD file field name|Example value|
|----|----|
|**SAEntry**|1|
|**SPI**|3000|
|**SADestIPAddr**|**FE80::2AA:FF:FE53:A92C**|
|**DestIPAddr**|**POLICY**|
|**SrcIPAddr**|**POLICY**|
|**Protocol**|**POLICY**|
|**DestPort**|**POLICY**|
|**SrcPort**|**POLICY**|
|**AuthAlg**|**HMAC-MD5**|
|**KeyFile**|**Test.key**|
|**Direction**|**OUTBOUND**|
|**SecPolicyIndex**|2|

在配置此SA的行的末尾放置一个分号。 SA条目必须按递减的数字顺序放置。

8、在主机2上，创建一个文本文件，其中包含用于对使用主机1创建的SA进行身份验证的文本字符串。在此示例中，创建文件Test.key，其内容为“This is a test”。 您必须在密钥字符串周围包含双引号，以便ipsec6工具读取密钥。

9、在主机1上，使用ipsec6 a命令从SPD和SAD文件添加已配置的安全策略和SA。 在此示例中，ipsec6 a test命令在主机1上运行。

10、在主机2上，使用ipsec6 a命令从SPD和SAD文件添加已配置的安全策略和SA。 在此示例中，ipsec6 a test命令在主机2上运行。

11、使用ping6命令从主机2 ping主机1。

如果使用Microsoft网络监视器或其他数据包嗅探器捕获流量，您应该看到在IPv6标头和ICMPv6标头之间使用Authentication Header交换ICMPv6 Echo Request和Echo Reply消息。





四、其他IPv6主题

以下主题提供了与Microsoft IPv6技术预览协议相关的其他信息。
- IPv6路由器广告
- IPv6站点前缀
- IPv6链路本地和站点本地地址
- IPv6多播目标地址
- IPv6自动隧道和6to4
- IPv6转发隧道数据包



一）、IPv6路由器广告

IPv6路由器通告的内容自动从路由表中的已发布路由派生。 非发布路由用于路由，但在构造路由器通告时会被忽略。

IPv6的路由器通告始终包含源链路层地址选项和MTU选项。 MTU选项的值取自发送接口的当前链路MTU。 可以使用ipv6 ifc mtu命令更改此值。

如果存在已发布的默认路由，则路由器通告仅具有非零路由器生存期。 默认路由是零长度前缀的路由。

发布的链路上路由导致路由器通告中的前缀信息选项。 如果on-link前缀有64位，则前缀信息选项同时设置L和A位，接收它的主机将自动配置地址。

发送路由器通告的接口还将根据其发送的前缀信息选项自动为自身配置地址。

建议在所有已发布路线上使用有限的非工作寿命（例如，30分钟）。 如果您决定撤消路线，则可以更改路线以使其具有老化生命周期。 该路由将在几个路由器通告的过程中老化，然后从路由器和接收路由器通告的任何主机中消失。

主机通过路由器通告查找的路由已老化且未发布。 地址自路由器广告时自动配置的地址。



二）、IPv6站点前缀

ipv6 rtu命令允许使用站点前缀长度配置已发布的on-link前缀。 如果指定，则将站点前缀长度放入路由器通告中的前缀信息选项中。 例如：
`ipv6 rtu 2002:836b:9820:2::/64 4 pub life 1800 spl 48`
指定与接口＃4链接的前缀。 发布前缀，这意味着如果接口＃4是广告接口，则它包含在路由器通告中。 前缀信息选项中的生命周期为1800秒（30分钟）。 前缀信息选项中的站点前缀长度为48。

接收指定站点前缀的前缀信息选项会导致在站点前缀表中创建条目，可以使用ipv6 spt命令显示该条目。 站点前缀表用于从getaddrinfo和相关函数返回的地址中删除不适当的站点本地地址。



三）、IPv6链路本地和站点本地地址

IPv6链路本地和站点本地地址称为范围地址。 Windows套接字（Winsock）API支持sockaddr_in6结构中的sin6_scope_id成员，以便与作用域地址一起使用。 对于IPv6链路本地地址（fe80 :: / 10前缀），sockaddr_in6结构中的sin6_scope_id成员是接口号。 对于IPv6站点本地地址（fec0 :: / 10前缀），sockaddr_in6结构中的sin6_scope_id成员是站点标识符。

接口＃5上的链路本地IPv6地址示例如下：
`fe80::208:74ff:feda:625c%5`
Windows XP Service Pack 1（SP1）及更高版本上可以使用以下命令在本地计算机上查询和配置IPv6：
- [Netsh.exe](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/netsh-exe)

使用Netsh.exe命令进行的配置更改是永久性的，并且在重新启动计算机或IPv6协议时不会丢失。

在带有Service Pack 1（SP1）的Windows XP之前，IPv6配置和管理使用了几个较旧的命令行工具（Net.exe，Ipv6.exe和Ipsec6.exe）来配置和管理IPv6。 使用这些较旧的工具，IPv6更改不是永久性的，并且在重新启动计算机或IPv6协议时会丢失。 这些旧的命令行工具仅在Windows XP上受支持。

在带有SP1的Windows XP上，以下命令将显示本地计算机上的IPv6接口列表，包括接口索引，接口名称和各种其他接口属性。

**netsh interface ipv6 show interface**

在带有SP1的Windows XP上，以下命令将更改与接口索引关联的站点标识符。

**netsh interface ipv6 set interface siteid=value**

在Windows XP上，以下较旧的命令还会将与站点本地地址关联的站点标识符更改为3。

**ipv6 rtu fec0::/10 3**

如果要发送或连接到作用域地址，则sockaddr_in6结构中的sin6_scope_id成员可以保留未指定（零），这表示不明确的作用域地址。 例如，以下链接本地地址不明确：
`fe80::10`
如果绑定到作用域地址，则sockaddr_in6结构中的sin6_scope_id成员必须包含非零值，该值指定链接本地地址的有效接口编号或站点本地地址的站点标识符。



**模糊的范围地址**

如果要发送或连接到作用域地址并且未在sockaddr_in6结构中指定sin6_scope_id成员，则作用域地址不明确。 要解决此问题，IPv6协议首先确定您是否已将套接字绑定到源地址。 如果是这样，绑定的源地址通过提供接口号或站点标识符来解决歧义。

如果要发送或连接到作用域地址并且既未指定sin6_scope_id成员也未绑定源地址，则IPv6协议将检查路由表。 例如，以下命令将在本地计算机上显示IPv6路由表：

**netsh interface ipv6 show route**

```
No   Manual   256  fe80::/64      13  Local Area Connection
No   Manual   256  fe80::/64      14  Wireless Network Connection
```

这表示默认情况下，链路本地地址被视为与接口＃13和＃14的链接。

当本地计算机具有多个网络适配器时，会出现歧义。 例如，上面的netsh命令表示有两个网络接口（本地连接和无线网络连接）。 当应用程序指定没有作用域ID的目标链接本地地址（例如，fe80 :: 10）时，不清楚用于发送数据包的适配器。 只有链路本地单播（fe80 :: / 64）或链路范围多播（ff00 :: / 8）IPv6目标地址在发送数据包时可能会遇到范围ID。



## 邻居发现

如果尚未在sockaddr_in6结构中指定sin6_scope_id成员，未绑定源地址，并且未指定链路本地地址的路由，则IPv6协议将尝试使用邻居发现来解析目标链路本地地址。对于发送的给定分组，尝试一个接口。尝试的第一个接口被认为是最受欢迎的接口。如果邻居发现无法解析接口上的链路本地地址，则将丢弃要发送的数据包，并且系统会记住无法通过该接口访问目标链路本地地址。在所有相同条件下要发送的下一个数据包上，尝试使用不同的接口进行邻居发现。对于每个新数据包，此过程将继续通过本地计算机上的每个接口，直到邻居发现响应目标链路本地地址或所有可能的接口已尝试失败。每次尝试解析邻居失败时，都会为该邻居删除一个接口。

如果目标链路本地地址解析，则该接口用于发送当前数据包。 此接口还用于发送到同一链路本地目标地址的任何后续模糊范围的数据包。

如果Neighbor Discovery无法解析所有接口上的目标链路本地地址，则系统会尝试在最喜欢的接口上发送数据包（尝试第一个接口）。 网络堆栈不断尝试在最优选的接口上解析目标链路本地地址。 在所有接口上的邻居发现失败后一段时间后，网络堆栈将再次重新启动该进程并尝试解析所有接口上的目标链路本地地址。 目前，在所有接口上再次尝试邻居发现的时间间隔为60秒。 但是，此时间间隔可能会在Windows版本上发生更改，并且不应由应用程序承担。

> 
注意

如果应用程序在邻居发现解析了链路本地地址后将相同的链路本地地址绑定到不同的接口，则不会使用邻居发现返回的链路本地目标地址覆盖该接口。


有关IP版本6的邻居发现的更多信息，请参阅IETF发布的RFC4861。



四）、IPv6多播目标地址

发送到多播目标地址时，Microsoft IPv6协议通常要求应用程序指定了传出接口。 这是通过IPV6_MULTICAST_IF套接字选项或通过将套接字绑定到特定源地址来完成的。

您还可以为特定多播地址，整个多播地址范围或所有多播地址指定默认接口。 这是通过将组播前缀on-link放置到所需的出接口的路由来完成的。 以下示例显示了如何指定：

```
ipv6 rtu ff02::5/128 3
ipv6 rtu ff0e::/16 3
ipv6 rtu ff00::/8 3
```



五）、IPv6自动隧道和6to4

使用IPv4兼容地址和6to4的自动隧道通过路由连接到接口＃2的前缀。 提取前缀后面的32位，并将其用作封装数据包的IPv4目标地址。

自动隧道使用:: / 96前缀，默认情况下启用。 可以通过删除:: / 96的路由来禁用它。

6to4使用2002 :: / 16前缀，默认情况下不启用。



六）、IPv6转发隧道数据包

接收封装（隧道）数据包并将它们解复用到特定接口的代码存在限制。 如果要转发通过已配置隧道接收的隧道数据包，则必须在任何6-over-4接口和接口＃2上启用转发。 只在接口＃2上启用转发将不起作用。 通常在配置路由器时，您将在除环回之外的所有接口上启用转发。



