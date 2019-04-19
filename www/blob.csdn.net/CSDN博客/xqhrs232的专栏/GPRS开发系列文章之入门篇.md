# GPRS开发系列文章之入门篇 - xqhrs232的专栏 - CSDN博客
2013年01月09日 22:35:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：504
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://www.cnblogs.com/jsjkandy/archive/2008/08/04/1260490.html](http://www.cnblogs.com/jsjkandy/archive/2008/08/04/1260490.html)
要做GPRS方面的应用软件开发，除了必备的一些程序设计知识之外，对计算机网络通信这些专业术语的了解可是一个都不能少，本人并不是专职于移动开发，所以在搞开发时也遇到了很多困难。出于这种考虑，我也想写个GPRS开发系列入门文章，服务于各位还正在搜索资料的同仁们（开发者抑或PDA爱好者等都行），同时欢迎各位移动开发的高手指点，共同提高。
接下来的文章依次为：
[GPRS开发系列文章之进阶篇（主要讲述evc/.NetCF下的开发类库）](http://www.cnblogs.com/jsjkandy/archive/2008/08/06/1262445.html)[GPRS开发系列文章之实战篇（利用类库进行client和server端开发）](http://www.cnblogs.com/jsjkandy/archive/2008/08/12/1266345.html)
**注：第一次在首页发文章，挺紧张的，如果内容不适合，我将第一时间转到新手区去。**
本篇文章的大致路线是这样的，先介绍下一些具体概念，然后再解释下GPRS是如何工作的，这里并不涉及具体开发，主要了解业务逻辑。
n         **入门概念篇**
1.         **GSM**。GSM全名为：Global System for Mobile Communications，中文为全球移动通讯系统，俗称"全球通"，是一种起源于欧洲的移动通信技术标准，它采用电话交换的技术，是第二代移动通信技术，其开发目的是让全球各地可以共同使用一个移动电话网络标准，让用户使用一部手机就能行遍全球。我国于20世纪90年代初引进采用此项技术标准，此前一直是采用蜂窝模拟移动技术，即第一代GSM技术（2001年12月31日我国关闭了模拟移动网络）。目前，中国移动、中国联通各拥有一个GSM网，为世界最大的移动通信网络。GSM系统包括 GSM
 900：900MHz、GSM1800：1800MHz 及 GSM1900：1900MHz等几个频段 。GSM（全球移动通信系统）是一种广泛应用于欧洲及世界其他地方的数字移动电话系统。GSM使用的是时分多址的变体，并且它是目前三种数字无线电话技术（TDMA、GSM和CDMA）中使用最为广泛的一种。GSM将资料数字化，并将数据进行压缩，然后与其它的两个用户数据流一起从信道发送出去，另外的两个用户数据流都有各自的时隙。。GSM实际上是欧洲的无线电话标准，据GSM MoU联合委员会报道，GSM在全球有120亿的用户，并且用户遍布120多个国家。因为许多GSM网络操作员与其他国外操作员有漫游协议，因此当用户到其他国家之后，仍然可以继续使用他们的移动电话。
美国著名通信公司Sprint的一个辅助部门，美国个人通信正在使用GSM作为一种宽带个人通信服务的技术。这种个人通信服务将最终为爱立信、摩托罗拉以及诺基亚现在正在生产的手持机建立400多个基站。手持机包括电话、短信寻呼机和对讲机。
GSM及其他技术是无线移动通信的演进，无线移动通信包括高速电路交换数据、通用无线分组系统、基于GSM网络的数据增强型移动通信技术以及通用移动通信服务
2.         GPRS。GPRS是通用分组无线业务(General Packet Radio Service)的简称，它突破了GSM网只能提供电路交换的思维方式，只通过增加相应的功能实体和对现有的基站系统进行部分改造来实现分组交换，这种改造的投入相对来说并不大，但得到的用户数据速率却相当可观。GPRS(General
 Packet Radio Service)是一种以全球手机系统（GSM）为基础的数据传输技术，可说是GSM的延续。GPRS和以往连续在频道传输的方式不同，是以封包（Packet）式来传输，因此使用者所负担的费用是以其传输资料单位计算，并非使用其整个频道，理论上较为便宜。
GPRS的传输速率可提升至56甚至114Kbps。而且，因为不再需要现行无线应用所需要的中介转换器，所以连接及传输都会更方便容易。如此，使用者既可联机上网，参加视讯会议等互动传播，而且在同一个视讯网络上（VRN）的使用者，甚至可以无需通过拨号上网，而持续与网络连接。
**注意**：GPRS并不是新一代的移动通信系统，GPRS只是一项加快数据传输的服务，在无线电波的传递上，还是以GSM的规格，所以可以说GPRS只是GSM的加强模型而并非新一代通信系统，也有人称之为2.5代通信系统
3.         **电路交换（Circuit Switching）**是两计算机或终端相通信时，使用的同一条实际物理链路，在通信中自始自终使用该贯路传输，且不允许其他计算机或终端同时共亨该链路。电路交换包括公用电话网，公用电报网和电路交换的公用数据网(CSPDN)；
4.         **分组交换网**是继电路交换网和报文交换网之后一种新型交换网络，它主要用于数据通信。分组交换是一种存储转发的交换方式，它将用户的报文划分成一定长度的分组，以分组为存储转发，因此，它比电路交换的利用率高，比报文交换的时延要小，而具有实时通信的能力。分组交换利用统计时分复用原理，将一条数据链路复用成多个逻辑信道，最终构成一条主叫、被叫用户之间的信息传送通路，称之为虚电路(V.C)实现数据的分组传送。
5.         **APN(Access Point Name)**,即接入点，在GPRS网络中引入了APN的概念,用户在激活网络时一般将APN作为参数,对应外部网络的业务提供者。对于一个GGSN(类似路由或网关),APN和业务提供者有着特定的对应关系。用户访问外部网络时,如果用户不指明具体的APN,网络侧将指配给用户一个缺省的APN,用户也可以自行指定具体的APN。
6.         **GGSN**（Gateway GSN，网关GSN）主要是起网关作用，它可以和多种不同的数据网络连接，如ISDN、PSPDN和LAN等。有的文献中，把GGSN称为GPRS路由器。GGSN可以把GSM网中的GPRS分组数据包进行协议转换，从而可以把这些分组数据包传送到远端的TCP／IP或X.25网络。
7.         **SGSN（**Serving GPRS Support Node，服务GPRS支持节点**）**SGSN是GSM/GPRS/EDGE网络的CN(Core
 Network，核心网)的网元。它负责在其服务区内转发MS(Mobile Station，移动台)与外部网络之间的IP数据包。SGSN和MS之间的业务信息还要经过BSC(Base Staion Controller，基站控制器)和BTS(Base Transceiver Station，基站收发信台)的传输
8.         **VPN(**Virtual Private Network，虚拟专用网络) 虚拟专用网(VPN)被定义为通过一个公用网络(通常是因特网)建立一个临时的、安全的连接，是一条穿过混乱的公用网络的安全、稳定的隧道。虚拟专用网是对企业内部网的扩展。
9.         **CMNET与CMWAP **(China Mobile Net与China
 Mobile Wap.) CMNET和CMWAP是中国移动GPRS网络的两个不同APN（Access Point Name接入点名称）。CMNET提供了NAT服务，使用该服务的手机可以直接访问internet。CMWAP只提供了WAP代理和HTTP代理，手机上网能访问的范围有限，接入时只能访问GPRS网络内的IP（10.0.0.172 ）。因此CMWAP主要用于手机WAP上网，CMNET则是用于PC，笔记本电脑，PDA等设备实现GPRS上网。两者手机上网的方式没有差别，如此划分是中国移动对手机上网服务对象的定位不同，所以他们的应用范围和自费标准不同；
10.     **其他常见英文缩写：**
u       BSC 基站控制器 Base Station Controller
u       BSS 基站系统 Base Station System
u       BTS 基站收发系统 Base Transeiver System
u       CDMA1X 码分多址 Code Division Multiple Access
u       DDN 数字数据网 Digital Data Network
u       DHCP 动态主机配置协议 Dynamic Host Configuration Protocol
u       DNS 域名系统 Domain Name System
u       ISDN 综合数字业务网络 Integrated Services Digital Network
u       ISP 互联网业务提供商 Internet Service Provider
u       M2M Machine to Machine 或Mobile to Machine业务
u       NAT 网络地址转换 Network Address Translation
u       RTU 远方终端单元 Remote Terminal Unit
u       SIM 用户标识模块 Subscriber Identify Module
u       SMS 短消息业务Short Message Service
u       TDMA 时分多址 Time Division Multiple Acess
n         **GPRS连接Internet工作原理篇**
    GPRS技术将通信网络和计算机网络结合在一起，向全IP网络的方向发展。GPRS基站与SGSN设备之间的连接一般通过帧中继连接，GGSN与 SGSN设备之间通过IP网络连接。GGSN是GPRS网络的网关和路由器。GGSN可以把GSM网中的GPRS分组数据包进行协议转换，从而可以把这些分组数据包传送到远端的INTERNET或X.25网络. （简单表示如下）
![](http://images.cnblogs.com/cnblogs_com/jsjkandy/GPRS/GPRS.jpg)
    GGSN可以由具有网络地址翻译功能的路由器承担内部IP地址与外部网络IP地址的转换。用户可以访问GPRS内部的网络，也可以通过APN访问外部的INTERNET.
如果用户的IP地址是运营商分配的公有地址，则GGSN不参与用户的论证和鉴权过程。用户可以通过GGSN透明地接入到GPRS内部网络或互联网络，这种方式称为**透明方式**.
**非透明方式**主要是用户通过GPRS网络接入到企业网络或ISP的情形。用户MS的IP地址是由企业网络或ISP分配的私有地址，用户访问该企业网络或ISP时,GGSN需要企业网络或ISP中的专用服务器对该用户进行鉴权或论证。 　　
　　在标识GPRS设备中，如手机MS的标识除了在GSM中使用IMSI,MSISDN等号码外，还需要分配IP地址。网元设备SGSN,GGSN的标识既有7号信令地址，又有数据GGSN的IP地址，网元设备之间的通信采用IP地址，而网元设备与MSC、HLR等实体的通信采用信令地址。在GPRS系统中，有一个重要的数据库记录信息，即用户PDP上下文（分组数据协议上下文），用于管理从手机MS到网关GGSN 及到ISP之间的数据路由信息。当MS访问GPRS内部网络或外部INTERINT网络时,MS提出PDP上下文请求消息,MS可以与运营商签约选择固定服务的GGSN。或由SGSN选择服务的GGSN，SGSN再向GGSN发建立PDP上下文请求消息。GGSN分配MS一个IP地址。在成功地建立和激活PDP上下文后，MS,SGSN和
 GGSN都存储了用户的PDP上下文信息。有了用户的位置信息和数据的路由信息,MS就可以访问该网络的资源。
用户通过GPRS网络接入到互联网、企业内部网或ISP时，需要对用户的身份、服务质量进行鉴权和数据加密等过程，用户MS的动态IP地址的分配可以分别由运营商、企业网或ISP等实现，因此GPRS用户的接入方式有透明接入和非透明接入两种方式
  n         **附PDA开发机利用GPRS上网步骤**（点击“开始”-》“设置”—》“连接”）
如果您的开发机支持GPRS，那么上网(Internet)可以有两种接入点(APN)方式供选择，一种是cmwap,一种是cmnet,他们的设置参数分别为：（其中步骤为）
l         CMWAP方式
1）连接名称: 中国移动梦网（GPRS）
2）数据承载方式: GPRS
3）接入点名称: cmwap
4）用户名: 无
5）密码: 无
6）鉴定: 普通
7）网关IP 地址: 10.0.0.172
8）主页: [http://wap.monternet.com/](http://wap.monternet.com/)
9）连接安全: 关
10）连接类型: 永久
11）可选端口:80/8080/9201 需要设置的主要参数：
l         CMNET 方式
1）连接名称: 中国移动互联网（GPRS）
2）数据承载方式: GPRS
3）接入点名称: cmnet
4）用户名: 无
5）密码: 无
6）鉴定: 普通
7）网关IP 地址: 无
8）连接安全: 关
9）连接类型: 永久
    注：本文除少数文字外，多数都来源于网络，本人只做了整理以供学习和比较之用。
