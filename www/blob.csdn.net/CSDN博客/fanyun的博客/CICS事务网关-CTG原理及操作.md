# CICS事务网关-CTG原理及操作 - fanyun的博客 - CSDN博客
2016年11月24日 16:16:49[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1801
1.1 什么是CTG
1.1.1 概述
CICS系统是IBM大型机上的重要的交易处理系统，并且由于大型机的特殊性，导致CICS的已有应用程序对分布式平台开放不够，难以重用以往的应用程序。因此，CICS与分布式系统的交互，即外界如何访问CICS传统应用，成为一个重要的课题。
    而CTG(CICS Transaction gateway，以下简称为CTG)，CICS交易网关，是分布式平台连接到CICS的首选工具，它具有高性能，高安全性，高可扩展性等特性。
    CTG与CICS的连接属于直接相连的方式，几乎不用修改CICS的已有应用，即可被外界访问。Web Services 同样可以实现与CICS的连接，但是相比于Web Services，CTG有以下优势：
    (1)没有复杂的标准，技术相对成熟。
    (2)利用已有的应用程序接口，很少需要对CICS程序进行修改。
本文中实例所使用的CICS Transaction Gateway软件为V7.2 for Windows版本。
1.1.2 CTG结构图
![图像](https://dw1.s81c.com/developerworks/mydeveloperworks/blogs/cicschina/resource/BLOGS_UPLOADED_IMAGES/1401.jpg)
 相关概念介绍:
1. Gateway Daemon (网关守护程序)：网关守护程序用于监听远端客户程序的请求，并且可以管理线程和网络连接，以达到优化的目的。网关守护程序中的一个重要组件是协议处理器(Protocol Handler)，用于处理远端应用程序的请求，可以支持TCP/IP和SSL协议。
2. Client Daemon (客户机守护程序)：客户机守护程序是开放平台CTG的重要组成部分，它提供一个本地接口，客户应用程序就通过这个接口连接到CICS，支持C，C++，.Net，COBOL等多种语言的应用程序。
3. IPIC driver (IPIC 驱动器)：IPIC驱动器是CTG中的一个独立组件，作用类似于客户机守护程序，专用于IPIC方式连接到CICS。CICS在3.2版本中引入了IPIC(IP Intercommunication)的概念，用于CICS
 region之间的互连，这种连接是基于TCP/IP协议的，具体的IPIC概念请参考其他相关章节。从CTGV7.1版本以后，加入了对IPIC的支持。
4. CTGCFG tool(CTG configuration tool，CTG配置工具)：CTG产品自带的图形化配置工具，用于设置ctg.ini配置文件中的属性，包括网关守护程序和客户机守护程序等的相关参数。配置客户端守护程序界面，如下图所示：
![图像](https://dw1.s81c.com/developerworks/mydeveloperworks/blogs/cicschina/resource/BLOGS_UPLOADED_IMAGES/1402.jpg)其中重要的几个参数描述如下：
(1) 服务器名称：一个8位长的字符串，定义一个独立于协议的服务器名称，客户端应用程序发到CICS的请求都要发到这个服务器。
(2) 协议：指定CTG所支持的各种网络协议，本章重点介绍TCP/IP协议。
(3) 主机名或IP地址：CICS所在的主机地址，可以指定主机名或IP地址。
(4) 端口：CICS端定义的TCPIPService所指定的端口，是客户端请求发送到CICS的接入点。
配置客户端守护程序界面，如下图所示：
![图像](https://dw1.s81c.com/developerworks/mydeveloperworks/blogs/cicschina/resource/BLOGS_UPLOADED_IMAGES/1403.jpg) 选中“启用协议处理程序”就能使图示的网关守护程序生效。
其中重要的几个参数描述如下：
(1)绑定地址：如果在此处指定某IP或主机名，就可以将协议处理程序绑定到上面指定的值；如果需要将协议处理程序绑定到任意地址，可以把此处留空不填。
(2)端口：安装CTG的服务器对外开放的CTG端口，默认为2006。
1.1.3 CTG编程接口
由于分布式系统架构的多样性，CTG支持一下三种连接方式：
1.    External Call Interface (ECI) 外部访问接口
ECI是简单的RPC式访问接口，用来调用(LINK) CICS中的应用程序，可以使用COMMAREA或channel作为数据接口，用于客户应用程序和CICS之间的数据交换。
    CICS将此种类型的访问看成是一种分布式程序调用(DPL : distributed program link )。客户
