# Windows Route 路由表命令整理 - DoubleLi - 博客园






Windows Route 路由表命令 
在本地 IP 路由表中显示和修改条目。 

语法 
route [-f] [-p] 
[Command [Destination] [mask Netmask] [Gateway] [metric Metric]] [if Interface]] 


参数 
-f 
清除所有不是主路由（网掩码为 255.255.255.255 的路由）、环回网络路由（目标为 
127.0.0.0，网掩码为 255.255.255.0 的路由）或多播路由（目标为 224.0.0.0，网掩码为 240.0.0.0 
的路由）的条目的路由表。如果它与命令之一（例如 add、change 或 delete）结合使用，表会在运行命令之前清除。 
-p 
与 add 
命令共同使用时，指定路由被添加到注册表并在启动 TCP/IP 协议的时候初始化 IP 路由表。默认情况下，启动 TCP/IP 协议时不会保存添加的路由。与 
print 命令一起使用时，则显示永久路由列表。所有其它的命令都忽略此参数。永久路由存储在注册表中的位置是 
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters\PersistentRoutes。 


Command 
指定要运行的命令。下表列出了有效的命令。 命令 目的 
add 添加路由 
change 更改现存路由 

delete 删除路由 
print 打印路由Destination 
指定路由的网络目标地址。目标地址可以是一个 IP 
网络地址（其中网络地址的主机地址位设置为 0），对于主机路由是 IP 地址，对于默认路由是 0.0.0.0。 

mask subnetmask 

指定与网络目标地址相关联的网掩码（又称之为子网掩码）。子网掩码对于 IP 网络地址可以是一适当的子网掩码，对于主机路由是 255.255.255.255 
，对于默认路由是 0.0.0.0。如果忽略，则使用子网掩码 
255.255.255.255。定义路由时由于目标地址和子网掩码之间的关系，目标地址不能比它对应的子网掩码更为详细。换句话说，如果子网掩码的一位是 
0，则目标地址中的对应位就不能设置为 1。 

Gateway 
指定超过由网络目标和子网掩码定义的可达到的地址集的前一个或下一个跃点 IP 
地址。对于本地连接的子网路由，网关地址是分配给连接子网接口的 IP 
地址。对于要经过一个或多个路由器才可用到的远程路由，网关地址是一个分配给相邻路由器的、可直接达到的 IP 地址。 

metric Metric 

为路由指定所需跃点数的整数值（范围是 1 ~ 
9999），它用来在路由表里的多个路由中选择与转发包中的目标地址最为匹配的路由。所选的路由具有最少的跃点数。跃点数能够反映跃点的数量、路径的速度、路径可靠性、路径吞吐量以及管理属性。 

if Interface 
指定目标可以到达的接口的接口索引。使用 route print 
命令可以显示接口及其对应接口索引的列表。对于接口索引可以使用十进制或十六进制的值。对于十六进制值，要在十六进制数的前面加上 0x。忽略 if 
参数时，接口由网关地址确定。 
/? 
在命令提示符显示帮助。 
注释 
路由表中 跃点数 一列的值较大是由于允许 TCP/IP 
根据每个 LAN接口的 IP 
地址、子网掩码和默认网关的配置自动确定路由表中路由的跃点数造成的。默认启动的自动确定接口跃点数确定了每个接口的速度，调整了每个接口的路由跃点数，因此最快接口所创建的路由具有最低的跃点数。要删除大跃点数，请在每个 
LAN 连接的 TCP/IP 协议的高级属性中禁用自动确定接口跃点数。 

如果在 systemroot\System32\Drivers\Etc 
文件夹的本地网络文件中存在适当的条目，名称可以用于 Destination。只要名称可以通过“域名系统” (DNS) 查询这样的标准主机名解析技术分解为 IP 
地址，就可以将其用于 Gateway，DNS 查询使用存储在 systemroot\System32\Drivers\Etc 文件夹下的本地主机文件和 
NetBIOS 名称解析。 

如果是 print 或 delete 命令，可以忽略 Gateway 
参数，使用通配符来表示目标和网关。Destination 的值可以是由星号 (*) 指定的通配符。如果指定目标含有一个星号 (*) 或问号 
(?)，它被看作是通配符，只打印或删除匹配的目标路由。星号代表任意一字符序列，问号代表任一字符。例如， 10.*.1, 192.168.*、 127.* 和 
*224* 都是星号通配符的有效使用。 

使用了无效的目标和子网掩码（网掩码）值的组合，会显示“Route:bad gateway address 
netmask”错误消息。目标中有一位或多位设置为 1，而其在子网掩码中的对应位设置为 0 
时会发生这个错误。可以通过二进制表示法表示目标和子网掩码来检查这种情况。以二进制表示的子网掩码包括表示目标网络地址部分的一连串的 1 
和表示目标主机地址部分的一连串的 0 两个部分。查看目标以确定目标的主机地址部分（由子网掩码所定义）是否有些位设置成了 1。 

只有 
Windows NT 4.0、Windows 2000、Windows Millennium Edition 和 Windows XP 的 route 命令支持 
-p 参数。Windows 95 或 Windows 98 的 route 命令不支持该参数。 


只有当网际协议 (TCP/IP) 协议在 
网络连接中安装为网络适配器属性的组件时，该命令才可用。 

范例 
要显示 IP 路由表的完整内容，请键入： 

route 
print 

要显示 IP 路由表中以 10. 开始的路由，请键入： 

route print 10.* 


要添加默认网关地址为 192.168.12.1 的默认路由，请键入： 

route add 0.0.0.0 mask 
0.0.0.0 192.168.12.1 

要添加目标为 10.41.0.0，子网掩码为 255.255.0.0，下一个跃点地址为 
10.27.0.1 的路由，请键入： 

route add 10.41.0.0 mask 255.255.0.0 10.27.0.1 


要添加目标为 10.41.0.0，子网掩码为 255.255.0.0，下一个跃点地址为 10.27.0.1 的永久路由，请键入： 


route -p add 10.41.0.0 mask 255.255.0.0 10.27.0.1 

要添加目标为 
10.41.0.0，子网掩码为 255.255.0.0，下一个跃点地址为 10.27.0.1，跃点数为 7 的路由，请键入： 

route add 
10.41.0.0 mask 255.255.0.0 10.27.0.1 metric 7 

要添加目标为 10.41.0.0，子网掩码为 
255.255.0.0，下一个跃点地址为 10.27.0.1，接口索引为 0x3 的路由，请键入： 

route add 10.41.0.0 
mask 255.255.0.0 10.27.0.1 if 0x3 

要删除目标为 10.41.0.0，子网掩码为 255.255.0.0 
的路由，请键入： 

route delete 10.41.0.0 mask 255.255.0.0 

要删除 IP 路由表中以 
10. 开始的所有路由，请键入： 

route delete 10.* 

要将目标为 10.41.0.0，子网掩码为 
255.255.0.0 的路由的下一个跃点地址由 10.27.0.1 更改为 10.27.0.25，请键入： 

route change 
10.41.0.0 mask 255.255.0.0 10.27.0.25 

route 的命令 
routing ip 
add/delete/set/show interface 在指定接口上添加、删除、配置或显示常规 IP 路由设置。 
routing ip 
add/delete/set/show filter 在指定接口上添加、删除、配置或显示 IP 数据包筛选器。 
routing ip 
add/delete/show boundary 在指定接口上添加、删除或显示多播边界设置。 
routing ip add/set ipiptunnel 
添加或配置 IP 中的 IP 接口。 
routing ip add/delete/set/show rtmroute 
添加、配置或显示不持续的路由表管理器路由。 
routing ip add/delete/set/show persistentroute 
添加、删除、配置或显示持续路由。 
routing ip add/delete/set/show preferenceforprotocol 
添加、删除、配置或显示路由协议的优先级。 
routing ip add/delete/set/show scope 添加、删除或显示多播作用域。 

routing ip set/show loglevel 配置或显示全局 IP 记录等级。 
routing ip show helper 显示 
IP 的所有 Netsh 实用程序子环境。 
routing ip show protocol 显示所有正在运行的 IP 路由协议。 

routing ip show mfe 显示多播转发项。 
routing ip show mfestats 显示多播转发项统计。 

routing ip show boundarystats 显示 IP 多播边界。 
routing ip show 
rtmdestinations 显示路由表管理器路由表中的目标。 
routing ip show rtmroutes 显示路由表管理器路由表中的路由。 

routing ip nat set/show global 配置或显示全局网络地址转换 (NAT) 设置。 
routing ip nat 
add/delete/set/show interface 添加、删除、配置或显示指定接口的 NAT 设置。 
routing ip nat 
add/delete addressrange 在 NAT 接口公用地址池中添加或删除一个地址范围。 
routing ip nat add/delete 
addressmapping 添加或删除 NAT 地址映射。 
routing ip nat add/delete portmapping 添加或删除 
NAT 端口映射。 
routing ip autodhcp set/show global 配置或显示全局 DHCP 分配器参数。 

routing ip autodhcp set/show interface 配置或显示指定接口的 DHCP 分配器设置。 
routing ip 
autodhcp add/delete exclusion 在 DHCP 分配器地址范围中添加或删除一个排除范围。 
routing ip 
dnsproxy set/show global 配置或显示全局 DNS 代理参数。 
routing ip dnsproxy set/show 
interface 配置或显示指定接口的 DNS 代理参数。 
routing ip igmp set/show global 配置或显示 IGMP 
全局设置。 
routing ip igmp add/delete/set/show interface 在指定接口上添加、删除、配置或显示 IGMP。 

routing ip igmp add/delete staticgroup 添加或删除指定接口的静态多播组。 
routing ip igmp 
show grouptable 显示 IGMP 主机组表。 
routing ip igmp show ifstats 显示每个接口的 IGMP 统计。 

routing ip igmp show iftable 显示每个接口的 IGMP 主机组。 
routing ip igmp show 
proxygrouptable 显示 IGMP 代理接口的 IGMP 组表。 
routing ip igmp show rasgrouptable 
显示远程访问服务器所使用的 Internet 接口的组表。 
routing ip ospf set/show global 配置或显示全局 OSPF 
设置。 
routing ip ospf add/delete/set/show interface 在指定接口上添加、删除、配置或显示 OSPF。 

routing ip ospf add/delete/set/show area 添加、删除、配置或显示 OSPF 区域。 
routing ip 
ospf add/delete/show range 在指定的 OSPF 区域上添加、删除、配置或显示范围。 
routing ip ospf 
add/delete/set/show virtif 添加、删除、配置或显示 OSPF 虚拟接口。 
routing ip ospf 
add/delete/show neighbor 添加、删除、配置或显示 OSPF 邻居。 
routing ip ospf 
add/delete/show protofilter 添加、删除、配置或显示 OSPF 外部路由的路由信息源。 
routing ip ospf 
add/delete/show routefilter 添加、删除、配置或显示 OSPF 外部路由的路由筛选。 
routing ip ospf show 
areastats 显示 OSPF 区域统计。 
routing ip ospf show lsdb 显示 OSPF 链接状态数据库。 

routing ip ospf show virtifstats 显示 OSPF 虚拟链接统计。 
routing ip relay set 
global 配置“DHCP 中继代理程序”的全局设置。 
routing ip relay add/delete/set interface 
在指定接口上添加、删除或配置“DHCP 中继代理程序”设置。 
routing ip relay add/delete dhcpserver 在 DHCP 
服务器地址列表中添加或删除 DHCP 服务器的 IP 地址。 
routing ip relay show ifbinding 显示接口的 IP 
地址绑定。 
routing ip relay show ifconfig 显示每个接口的“DHCP 中继代理程序”配置。 
routing ip 
relay show ifstats 显示每个接口的 DHCP 统计。 
routing ip rip set/show global 配置 IP 的 
RIP 全局设置。 
routing ip rip add/delete/set/show interface 在指定接口上添加或配置 IP 的 RIP 
设置。 
routing ip rip add/delete peerfilter 添加或删除 RIP 对等筛选器。 
routing ip rip 
add/delete acceptfilter 在接受的路由列表中添加或删除 RIP 路由筛选器。 
routing ip rip add/delete 
announcefilter 在公布的路由列表中添加或删除 RIP 路由筛选器。 
routing ip rip add/delete/show 
neighbor 添加或删除 RIP 邻居。 
routing ip rip set/show flags 在指定接口上配置 IP RIP 高级设置。 

routing ip rip show globalstats 显示全局 RIP 参数。 
routing ip rip show 
ifbinding 显示接口的 IP 地址绑定。 
routing ip rip show ifstats 显示每个接口的 RIP 统计。 


IPX netsh 路由命令 
routing ipx add/set staticroute 在 IPX 路由表中添加或配置静态 IPX 
路由。 
routing ipx add/set staticservice 在 SAP 服务表中添加或配置静态 SAP 服务。 
routing 
ipx add/set filter 在指定的接口上添加或配置 IPX 数据包筛选器。 
routing ipx add/set interface 
在请求拨号接口上启用 IPX 路由，或在指定的接口上配置 IPX 设置。 
routing ipx set global 配置全局 IPX 路由设置。 

routing ipx rip add/set filter 添加和配置 RIP 路由筛选器。 
routing ipx rip set 
global 配置全局 IPX 的 RIP 设置。 
routing ipx rip set interface 在指定接口上配置 IPX 的 RIP 
设置。 
routing ipx sap add/set filter 添加或配置 SAP 服务筛选器。 
routing ipx sap set 
global 配置全局 IPX 的 SAP 设置。 
routing ipx sap set interface 在指定接口上配置 IPX 的 SAP 
设置。 
routing ipx netbios add nbname 将静态 NETBIOS 名称添加到 IPX NetBIOS 名称表中。 

routing ipx netbios set interface 在指定接口上配置基于 IPX 的 NetBIOS 设置。 

route 
英式说明帮助 

Manipulates network routing tables. 

ROUTE [-f] [-p] 
[command [destination] 
[MASK netmask] [gateway] [METRIC metric] [IF 
interface] 

-f Clears the routing tables of all gateway entries. If this 
is 
used in conjunction with one of the commands, the tables are 
cleared 
prior to running the command. 
-p When used with the ADD command, makes a 
route persistent across 
boots of the system. By default, routes are not 
preserved 
when the system is restarted. Ignored for all other commands, 

which always affect the appropriate persistent routes. This 
option is 
not supported in Windows 95. 
command One of these: 
PRINT Prints a route 

ADD Adds a route 
DELETE Deletes a route 
CHANGE Modifies an existing 
route 
destination Specifies the host. 
MASK Specifies that the next 
parameter is the 'netmask' value. 
netmask Specifies a subnet mask value for 
this route entry. 
If not specified, it defaults to 255.255.255.255. 

gateway Specifies gateway. 
interface the interface number for the 
specified route. 
METRIC specifies the metric, ie. cost for the destination. 


All symbolic names used for destination are looked up in the network 
database 
file NETWORKS. The symbolic names for gateway are looked up in the 
host name 
database file HOSTS. 

If the command is PRINT or DELETE. 
Destination or gateway can be a wildcard, 
(wildcard is specified as a star 
'*'), or the gateway argument may be omitted. 

If Dest contains a * or ?, 
it is treated as a shell pattern, and only 
matching destination routes are 
printed. The '*' matches any string, 
and '?' matches any one char. Examples: 
157.*.1, 157.*, 127.*, *224*. 

The PRINT command will show both IPv4 and 
IPv6 routes, but the ADD, DELETE, 
and CHANGE commands work only for IPv4 
routes. For IPv6 routes, use 
the 'interface ipv6' context in netsh.exe. 


Diagnostic Notes: 
Invalid MASK generates an error, that is when 
(DEST & MASK) != DEST. 
Example> route ADD 157.0.0.0 MASK 155.0.0.0 
157.55.80.1 IF 1 
The route addition failed: The specified mask parameter is 
invalid. 
(Destination & Mask) != Destination. 

Examples: 


> route PRINT 
> route ADD 157.0.0.0 MASK 255.0.0.0 157.55.80.1 
METRIC 3 IF 2 
destination^ ^mask ^gateway metric^ ^ 
Interface^ 
If IF 
is not given, it tries to find the best interface for a given 
gateway. 

> route PRINT 
> route PRINT 157* .... Only prints those matching 
157* 
> route CHANGE 157.0.0.0 MASK 255.0.0.0 157.55.80.5 METRIC 2 IF 2 


CHANGE is used to modify gateway and/or metric only. 
> route 
PRINT 
> route DELETE 157.0.0.0 
> route PRINT 









