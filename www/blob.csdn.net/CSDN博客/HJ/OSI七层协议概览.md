# OSI七层协议概览 - HJ - CSDN博客
2018年06月30日 19:08:53[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：40
个人分类：[计算机网络](https://blog.csdn.net/feizaoSYUACM/article/category/7349917)
|协议名称|数据传输单元|典型设备|典型协议|主要功能描述|
|----|----|----|----|----|
|物理层|数据位（bit）|集线器、中继器、电缆、发送器和接收器|V.35、EIA/TIA-232|物理接口规范，传输比特流；|
|数据链路层|数据帧（frame）|交换机、网桥、网卡|IEEE 802.1、IEEE 802.2、IEEE 802.3、IEEE 802.11|封装成帧，保证帧的无差错传输；|
|网络层|数据包（packet）|路由器、网桥路由器|IP、IPX、APPLETALK、ICMP|形成IP包，把逻辑网络地址转换成物理地址，路由选择、流量控制；|
|传输层|数据段（segment）|网关|TCP、UDP|设置端口地址，进行端到端控制，差错校验，流量控制；|
|会话层|数据（data）|略|组织两个会话进程之间的通信；| |
|表示层|略|通信于不同操作系统之间；| | |
|应用层|DNS、HTTP、HTTPS、FTP、TFTP、Telnet、TFTP、SNMP、SMTP、POP3、DHCP、SSL|为操作系统或网络应用程序提供访问网络服务的接口，对应于各个应用软件；| | |
