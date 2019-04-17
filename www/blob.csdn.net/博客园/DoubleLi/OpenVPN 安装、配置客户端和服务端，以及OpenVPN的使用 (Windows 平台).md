# OpenVPN 安装、配置客户端和服务端，以及OpenVPN的使用 (Windows 平台) - DoubleLi - 博客园







**OpenVPN 开源，好用，而且免费，感谢 OpenVPN 团队开发此产品。**



**简介**

OpenVPN允许参与创建VPN的单点使用[公开密钥](http://zh.wikipedia.org/wiki/%E5%85%AC%E5%BC%80%E5%AF%86%E9%92%A5)、[电子证书](http://zh.wikipedia.org/wiki/%E9%9B%BB%E5%AD%90%E8%AD%89%E6%9B%B8)、或者用户名／密码来进行身份验证。它大量使用了[OpenSSL](http://zh.wikipedia.org/wiki/OpenSSL)加密库中的[SSL](http://zh.wikipedia.org/wiki/SSL)v3/[TLS](http://zh.wikipedia.org/wiki/TLS)v1协议函数库。目前OpenVPN能在[Solaris](http://zh.wikipedia.org/wiki/Solaris)、[Linux](http://zh.wikipedia.org/wiki/Linux)、[OpenBSD](http://zh.wikipedia.org/wiki/OpenBSD)、[FreeBSD](http://zh.wikipedia.org/wiki/FreeBSD)、[NetBSD](http://zh.wikipedia.org/wiki/NetBSD)、[Mac OS X](http://zh.wikipedia.org/wiki/Mac_OS_X)与[Windows 2000](http://zh.wikipedia.org/wiki/Windows_2000)／[XP](http://zh.wikipedia.org/wiki/Windows_XP)／[Vista](http://zh.wikipedia.org/wiki/Windows_Vista)/[Windows 7](http://zh.wikipedia.org/wiki/Windows_7)以及[Android](http://zh.wikipedia.org/wiki/Android)上运行，并包含了许多安全性的功能。它并不是一个基于Web的VPN软件，也不与[IPsec](http://zh.wikipedia.org/wiki/IPsec)及其他VPN软件包兼容。

## 加密

OpenVPN使用[OpenSSL](http://zh.wikipedia.org/wiki/OpenSSL)库加密数据与控制信息：它使用了OpenSSL的加密以及验证功能，意味着，它能够使用任何OpenSSL支持的[算法](http://lib.csdn.net/base/datastructure)。它提供了可选的数据包[HMAC](http://zh.wikipedia.org/w/index.php?title=HMAC&action=edit&redlink=1)功能以提高连接的安全性。此外，OpenSSL的硬件加速也能提高它的性能。

## 验证

OpenVPN提供了多种[身份验证方式](http://zh.wikipedia.org/wiki/%E8%BA%AB%E4%BB%BD%E9%AA%8C%E8%AF%81%E6%96%B9%E5%BC%8F)，用以确认参与连接双方的身份，包括：预享私钥，第三方证书以及用户名／密码组合。预享密钥最为简单，但同时它只能用于创建点对点的VPN；基于[PKI](http://zh.wikipedia.org/wiki/PKI)的第三方证书提供了最完善的功能，但是需要额外的精力去维护一个PKI证书体系。OpenVPN2.0后引入了用户名／口令组合的身份验证方式，它可以省略客户端证书，但是仍有一份服务器证书需要被用作加密。

## 网络

OpenVPN所有的通信都基于一个单一的[IP](http://zh.wikipedia.org/wiki/IP)[端口](http://zh.wikipedia.org/wiki/%E7%AB%AF%E5%8F%A3)，默认且推荐使用UDP协议通讯，同时TCP也被支持。OpenVPN连接能通过大多数的[代理服务器](http://zh.wikipedia.org/wiki/%E4%BB%A3%E7%90%86%E6%9C%8D%E5%8A%A1%E5%99%A8)，并且能够在[NAT](http://zh.wikipedia.org/wiki/NAT)的环境中很好地工作。服务端具有向客户端“推送”某些网络配置信息的功能，这些信息包括：[IP地址](http://zh.wikipedia.org/wiki/IP%E5%9C%B0%E5%9D%80)、[路由](http://zh.wikipedia.org/wiki/%E8%B7%AF%E7%94%B1)设置等。OpenVPN提供了两种虚拟网络接口：[通用Tun/Tap驱动](http://zh.wikipedia.org/wiki/TUN%E4%B8%8ETAP)，通过它们，可以创建三层IP隧道，或者虚拟二层[以太网](http://zh.wikipedia.org/wiki/%E4%BB%A5%E5%A4%AA%E7%BD%91)，后者可以传送任何类型的二层以太网络数据。传送的数据可通过[LZO](http://zh.wikipedia.org/wiki/LZO)算法压缩。[IANA](http://zh.wikipedia.org/wiki/IANA)（Internet Assigned Numbers Authority）指定给OpenVPN的官方端口为1194。OpenVPN 2.0以后版本每个进程可以同时管理数个并发的隧道。

OpenVPN使用通用网络协议（[TCP](http://zh.wikipedia.org/wiki/TCP)与[UDP](http://zh.wikipedia.org/wiki/UDP)）的特点使它成为[IPsec](http://zh.wikipedia.org/wiki/IPsec)等协议的理想替代，尤其是在[ISP](http://zh.wikipedia.org/wiki/ISP)（Internet service provider）过滤某些特定[VPN](http://zh.wikipedia.org/wiki/VPN)协议的情况下。

## 安全

OpenVPN与生俱来便具备了许多安全特性：它在用户空间运行，无须对内核及网络协议栈作修改；初始完毕后以chroot方式运行，放弃root权限；使用[mlockall](http://www.opengroup.org/onlinepubs/009695399/functions/mlockall.html)以防止敏感数据交换到磁盘。

OpenVPN通过[PKCS#11](http://zh.wikipedia.org/wiki/PKCS11)支持硬件加密标识，如[智能卡](http://zh.wikipedia.org/wiki/%E6%99%BA%E8%83%BD%E5%8D%A1)。



**[以上关于 OpenVPN 的内容转载自维基百科](http://zh.wikipedia.org/wiki/OpenVPN)**



**申明：此文与免流无关，以免流为目的读者请离开。**



**若想了解 FreeBSD 下 OpenVPN 的资讯可以访问 [http://blog.csdn.net/joyous/article/details/38048425](http://blog.csdn.net/joyous/article/details/38048425)**

**有关编译 OpenVPN 的资料在 [http://blog.csdn.net/joyous/article/details/59634058](http://blog.csdn.net/joyous/article/details/59634058)**



题外话：目前[openvpn](http://openvpn.net/) 的官方网站无法正常访问，以至于如此优秀的软件和资料无法通过正常渠道获取，而这并非是OpenVPN官网的问题，而是中囯症腐对其进行了封锁（这导致Google和其他很多网站也存在类似问题），因为OpenVPN的加密通讯功能再配合一台NAT主机，可以绕开GFW（很恶心东西，一个打着以网络安全为幌子的内向防火墙，用于阻碍中囯居民(中囯没有公民)对境外网络的完全访问）。
                                                       ——坦荡不需要遮掩，除非心里揣着脏东西！



另注：目前 OpenVPN 常用的证书验证方式已经被 GFW 攻破，原因是网络**通讯**动态****加密协议都存在一个特征，就是通讯协议初期握手时，IP包特定位置会发送一段明文，而能避免这个明文的出现的就是采用静态验证模式，可以突破 GFW 对 OpenVPN 的围攻。



言归正传，OpenVPN 使用比较简单，流程上稍微注意一下，你一定可以很好的操作它，本文仅以证书及 key 作为验证方式对 OpenVPN 的使用进行介绍，有问题Q群里交流（群号在文章中）。



![](http://img.blog.csdn.net/20131008174119781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvam95b3Vz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

连接模式大致如图所示



　　OpenVPN 可以为1对1或者1对多，甚至多对多的分散计算机构筑安全的互联网络，**简单的说，就是****通过物理网卡的连接，创建虚拟网络，借由虚拟网卡访问虚拟网络，形成一个跨越广域网的虚拟局域网。**

　　相互间的通讯经严格加密，安全可靠，比如远程管理服务器，但又当心3389或者FTP之类的端口被那些草根黑客用字典猜密码，复杂的密码虽然不大可能猜中，但也实在很烦，但真正的黑客我还是很敬仰他们的，那么言归正传。管理这类服务器通过 OpenVPN 的证书认证连接成功后，你和服务器之间如同局域网，对内网再开放端口会安全的多，维护也较容易，防火墙对外网，则只针对例如80 端口和OpenVPN 端口（可以自定，默认是1194）开放，否则就无法访问了。



 因为封锁，下载也很可能随时被封（如果以下下载不能，请到文中Q群**共享**文件****下载）





## OpenVPN 2.4.2 -- released on 2017.05.11
|Source Tarball (gzip)|[**openvpn-2.4.2.tar.gz**](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.tar.gz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.4.2.tar.xz**](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.tar.xz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.tar.xz.asc)|
|Source Zip|[**openvpn-2.4.2.zip**](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.zip)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.zip.asc)|
|Installer, Windows Vista and later|[**openvpn-install-2.4.2-I601.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.4.2-I601.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.4.2-I601.exe.asc)|




**OpenVPN 2.4.0 -- released on 2016.12.27**


|Source Tarball (gzip)|[**openvpn-2.4.0.tar.gz**](https://swupdate.openvpn.org/community/releases/openvpn-2.4.0.tar.gz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.4.0.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.4.0.tar.xz**](https://swupdate.openvpn.org/community/releases/openvpn-2.4.0.tar.xz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.4.0.tar.xz.asc)|
|Source Zip|[**openvpn-2.4.0.zip**](https://swupdate.openvpn.org/community/releases/openvpn-2.4.0.zip)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.4.0.zip.asc)|
|Installer, Windows Vista and later|[**openvpn-install-2.4.0-I602.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.4.0-I602.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.4.0-I602.exe.asc)|




## OpenVPN 2.3.16 (old stable) -- released on 2017.05.19
|Source Tarball (gzip)|[**openvpn-2.3.16.tar.gz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.tar.gz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.3.16.tar.xz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.tar.xz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.tar.xz.asc)|
|Source Zip|[**openvpn-2.3.16.zip**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.zip)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.zip.asc)|
|Installer (32-bit), Windows XP|[**openvpn-install-2.3.16-I001-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I001-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I001-i686.exe.asc)|
|Installer (64-bit), Windows XP|[**openvpn-install-2.3.16-I001-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I001-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I001-x86_64.exe.asc)|
|Installer (32-bit), Windows Vista and later|[**openvpn-install-2.3.16-I601-i686.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I601-i686.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I601-i686.exe.asc)|
|Installer (64-bit), Windows Vista and later|[**openvpn-install-2.3.16-I601-x86_64.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I601-x86_64.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I601-x86_64.exe.asc)|


## OpenVPN 2.3.13-- released on 2016.11.03

|Source Tarball (gzip)|[**openvpn-2.3.13.tar.gz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.13.tar.gz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.13.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.3.13.tar.xz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.13.tar.xz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.13.tar.xz.asc)|
|Source Zip|[**openvpn-2.3.13.zip**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.13.zip)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.13.zip.asc)|
|Installer (32-bit), Windows XP|[**openvpn-install-2.3.13-I001-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.13-I001-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.13-I001-i686.exe.asc)|
|Installer (64-bit), Windows XP|[**openvpn-install-2.3.13-I001-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.13-I001-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.13-I001-x86_64.exe.asc)|
|Installer (32-bit), Windows Vista and later|[**openvpn-install-2.3.13-I601-i686.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.13-I601-i686.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.13-I601-i686.exe.asc)|
|Installer (64-bit), Windows Vista and later|[**openvpn-install-2.3.13-I601-x86_64.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.13-I601-x86_64.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.13-I601-x86_64.exe.asc)|






OpenVPN src & bin released on 2016.8.23


|Source Tarball (gzip)|[**openvpn-2.3.12.tar.gz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.12.tar.gz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.12.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.3.12.tar.xz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.12.tar.xz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.12.tar.xz.asc)|
|Source Zip|[**openvpn-2.3.12.zip**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.12.zip)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.12.zip.asc)|
|Installer (32-bit), Windows XP|[**openvpn-install-2.3.12-I002-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.12-I002-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.12-I002-i686.exe.asc)|
|Installer (64-bit), Windows XP|[**openvpn-install-2.3.12-I002-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.12-I002-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.12-I002-x86_64.exe.asc)|
|Installer (32-bit), Windows Vista and later|[**openvpn-install-2.3.12-I602-i686.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.12-I602-i686.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.12-I602-i686.exe.asc)|
|Installer (64-bit), Windows Vista and later|[**openvpn-install-2.3.12-I602-x86_64.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.12-I602-x86_64.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.12-I602-x86_64.exe.asc)|




OpenVPN src & bin released on 2016.05.10


|Source Tarball (gzip)|[**openvpn-2.3.11.tar.gz**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.11.tar.gz)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.11.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.3.11.tar.xz**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.11.tar.xz)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.11.tar.xz.asc)|
|Source Zip|[**openvpn-2.3.11.zip**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.11.zip)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.11.zip.asc)|
|Installer (32-bit), Windows XP|[**openvpn-install-2.3.11-I001-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.11-I001-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.11-I001-i686.exe.asc)|
|Installer (64-bit), Windows XP|[**openvpn-install-2.3.11-I001-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.11-I001-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.11-I001-x86_64.exe.asc)|
|Installer (32-bit), Windows Vista and later|[**openvpn-install-2.3.11-I601-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.11-I601-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.11-I601-i686.exe.asc)|
|Installer (64-bit), Windows Vista and later|[**openvpn-install-2.3.11-I601-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.11-I601-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.11-I601-x86_64.exe.asc)|




**OpenVPN 2.3.10 src & bin, released on 2016.01.04**


|Source Tarball (gzip)|[**openvpn-2.3.10.tar.gz**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.10.tar.gz)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.10.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.3.10.tar.xz**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.10.tar.xz)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.10.tar.xz.asc)|
|Source Zip|[**openvpn-2.3.10.zip**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.10.zip)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.10.zip.asc)|
|Installer (32-bit), Windows XP|[**openvpn-install-2.3.10-I001-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.10-I001-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.10-I001-i686.exe.asc)|
|Installer (64-bit), Windows XP|[**openvpn-install-2.3.10-I001-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.10-I001-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.10-I001-x86_64.exe.asc)|
|Installer (32-bit), Windows Vista and later|[**openvpn-install-2.3.10-I601-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.10-I601-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.10-I601-i686.exe.asc)|
|Installer (64-bit), Windows Vista and later|[**openvpn-install-2.3.10-I601-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.10-I601-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.10-I601-x86_64.exe.asc)|




**OpenVPN 2.3.8 src & bin, released on 2015.08.04**


|Source Tarball (gzip)|[**openvpn-2.3.8.tar.gz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.8.tar.gz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.8.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.3.8.tar.xz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.8.tar.xz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.8.tar.xz.asc)|
|Source Zip|[**openvpn-2.3.8.zip**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.8.zip)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.8.zip.asc)|
|Installer (32-bit), Windows XP|[**openvpn-install-2.3-I001-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.8-I001-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.8-I001-i686.exe.asc)|
|Installer (64-bit), Windows XP|[**openvpn-install-2.3.8-I001-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.8-I001-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.8-I001-x86_64.exe.asc)|
|Installer (32-bit), Windows Vista and later|[**openvpn-install-2.3.8-I601-i686.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.8-I601-i686.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.8-I601-i686.exe.asc)|
|Installer (64-bit), Windows Vista and later|[**openvpn-install-2.3.8-I601-x86_64.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.8-I601-x86_64.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.8-I601-x86_64.exe.asc)|




OpenVPN 2.3.6 bin & src 下载
|Source Tarball (gzip)|[**openvpn-2.3.6.tar.gz**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.6.tar.gz)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.6.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.3.6.tar.xz**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.6.tar.xz)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.6.tar.xz.asc)|
|Source Zip|[**openvpn-2.3.6.zip**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.6.zip)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.6.zip.asc)|
|Installer (32-bit), Windows XP and later|[**openvpn-install-2.3.6-I001-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.6-I001-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.6-I001-i686.exe.asc)|
|Installer (64-bit), Windows XP and later|[**openvpn-install-2.3.6-I001-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.6-I001-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.6-I001-x86_64.exe.asc)|
|Installer (32-bit), Windows Vista and later|[**openvpn-install-2.3.6-I601-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.6-I601-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.6-I601-i686.exe.asc)|
|Installer (64-bit), Windows Vista and later|[**openvpn-install-2.3.6-I601-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.6-I601-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.6-I601-x86_64.exe.asc)|



**OpenVPN 2.3.5 I001 32/64 bin & src 下载**
|Source Tarball (gzip)|[**openvpn-2.3.5.tar.gz**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.5.tar.gz)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.5.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.3.5.tar.xz**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.5.tar.xz)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.5.tar.xz.asc)|
|Source Zip|[**openvpn-2.3.5.zip**](http://swupdate.openvpn.org/community/releases/openvpn-2.3.5.zip)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-2.3.5.zip.asc)|
|Installer (32-bit), Windows XP and later|[**openvpn-install-2.3.5-I001-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.5-I001-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.5-I001-i686.exe.asc)|
|Installer (64-bit), Windows XP and later|[**openvpn-install-2.3.5-I001-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.5-I001-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.5-I001-x86_64.exe.asc)|
|Installer (32-bit), Windows Vista and later|[**openvpn-install-2.3.5-I601-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.5-I601-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.5-I601-i686.exe.asc)|
|Installer (64-bit), Windows Vista and later|[**openvpn-install-2.3.5-I601-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.5-I601-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.5-I601-x86_64.exe.asc)|



**OpenVPN 2.3.4 I003/I603 下载地址**

Installer (32-bit), Windows XP and later [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I003-i686.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I003-i686.exe)

Installer (64-bit), Windows XP and later [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I003-x86_64.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I003-x86_64.exe)

Installer (32-bit), Windows Vista and later [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I603-i686.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I603-i686.exe)

Installer (64-bit), Windows Vista and later [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I603-x86_64.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I603-x86_64.exe)

Source Tarball (gzip) [http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.tar.gz](http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.tar.gz)

Source Tarball (xz) [http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.tar.xz](http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.tar.xz)

Source Zip [http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.zip](http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.zip)



**OpenVPN 2.3.4 I002 下载地址（已修复心脏出血漏洞）**

Windows Install 32-bit [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I002-i686.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I002-i686.exe)

Windows Install 64-bit [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I002-x86_64.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.4-I002-x86_64.exe)

Source-zip [http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.zip](http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.zip)

Source-gzip [http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.tar.gz](http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.tar.gz)

Source-xz [http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.tar.xz](http://swupdate.openvpn.org/community/releases/openvpn-2.3.4.tar.xz)



**OpenVPN 2.3.3  下载地址**

32位 [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.3-I002-i686.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.3-I002-i686.exe)

64位 [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.3-I002-x86_64.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.3-I002-x86_64.exe)

源代码 [http://swupdate.openvpn.org/community/releases/openvpn-2.3.3.zip](http://swupdate.openvpn.org/community/releases/openvpn-2.3.3.zip)



**OpenVPN 2.3.2 下载地址**

32Bit [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.2-I001-i686.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.2-I001-i686.exe)

64Bit [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.2-I001-x86_64.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.2-I001-x86_64.exe)

Source Code [http://swupdate.openvpn.org/community/releases/openvpn-2.3.2.zip](http://swupdate.openvpn.org/community/releases/openvpn-2.3.2.zip)



**OpenVPN 2.3 下载地址**

32位 [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.0-I005-i686.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.0-I005-i686.exe)

64位 [http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.0-I005-x86_64.exe](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.0-I005-x86_64.exe)

源代码 [http://swupdate.openvpn.org/community/releases/openvpn-2.3.0.zip](http://swupdate.openvpn.org/community/releases/openvpn-2.3.0.zip)



**OpenVPN 2.2 下载地址**

下安装程序：[http://swupdate.openvpn.org/community/releases/openvpn-2.2.2-install.exe](http://swupdate.openvpn.org/community/releases/openvpn-2.2.2-install.exe)

源代码：[http://swupdate.openvpn.org/community/releases/openvpn-2.2.2.zip](http://swupdate.openvpn.org/community/releases/openvpn-2.2.2.zip)



**其它历史发布版本**

含源代码和安装程序 [http://build.openvpn.net/downloads/releases/](http://build.openvpn.net/downloads/releases/)



OpenVPN 的配置不复杂，以 Windows 环境下为例，选择**全部**组件安装，安装完毕后 cmd 控制台进入C:\Program Files\OpenVPN\easy-rsa 目录，我们需要创建服务端和客户端证书。

进入cmd前可能需要将C:\Program Files\OpenVPN\bin添加到环境变量的path中。

OpenVPN 经[测试](http://lib.csdn.net/base/softwaretest)，能运行于32位和64位[操作系统](http://lib.csdn.net/base/operatingsystem)，不过若在Windows 7下安装的话，必须以管理员权限运行。旧版本 OpenVPN 很可能在 Windows 8 运行不正常，其他旧版 Windows 32/64 平台均没问题，最新版已经支持 Windows 10。



**制作证书，运行控制台命令**

#初始化配置

init-config

#复制配置文件为批处理
vars

#清除key文件夹内的文件,文件夹不存在会自动创建
clean-all

#生成证书及key，需要填写一些参数，建议不要默认
build-ca

#创建服务端证书及key，需要填写一些参数，建议不要默认
build-key-server server

#创建一个客户端证书及key，需要填写一些参数，建议不要默认
build-key client1



#再创建一个客户端证书及key，需要填写一些参数，建议不要默认
build-key client2

#再创建一个客户端证书及key，需要填写一些参数，建议不要默认
build-key client3

#生成1024bit的key，并结束此次创建
build-dh



证书制作完毕，得到一些文件，**但如果参数填写不恰当，可能生成一些0字节的crt文件，**那么就得重新生成证书。生成结束后，可按照如下表格分别将文件拷贝到客户端和服务端的 config 目录中（测试时全部放一起也没关系）。




|**      文件名       **|**位置**|**目的**|**  保密**|
|----|----|----|----|
|ca.crt|server + all clients|Root CA certificate|NO|
|ca.key|key signing machine only|Root CA key|YES|
|dh{n}.pem|server only|Diffie Hellman parameters|NO|
|server.crt|server only|Server Certificate|NO|
|server.key|server only|Server Key|YES|
|client1.crt|client1 only|Client1 Certificate|NO|
|client1.key|client1 only|Client1 Key|YES|
|client2.crt|client2 only|Client2 Certificate|NO|
|client2.key|client2 only|Client2 Key|YES|
|client3.crt|client3 only|Client3 Certificate|NO|
|client3.key|client3 only|Client3 Key|YES|







若要增加证书，如果不是第一次创建用户，只需运行

vars

build-key mark



若要撤销证书，则使用以下命令，并得到 **crl.pem** 文件，

vars
revoke-full client2

再在服务端配置中加上

crl-verify crl.pem



下一步是配置文件，在 **C:\Program Files\OpenVPN\sample-config** 目录内有**client.ovpn**、**server.ovpn**，将client.ovpn拷贝到客户端的config，server.ovpn拷贝到服务端的 C:\Program Files\OpenVPN\config 目录。



**配置文件内需要指定证书文件名（默认已存在），请务必确认配置文件内证书文件名描述正确！**

**如果仅仅使用证书配置，除证书文件名描述正确外，只需要修改客户端配置文件指向的服务端IP即可。**

**也就是说，simple-config 内的配置几乎不需要修改，在证书正常的情况下，就可以直接使用！**



修改客户端的 **client.ovpn**的**remote** 项，让它指向**服务端外部 IP 地址**，格式例如：**remote x.x.x.x 1194**，1194是服务端定义的端口，这个可以通过服务端的配置文件修改，服务端和客户端端口必须匹配。

证书：在第92,93行的（大概）位置，**修改指定的 crt 和 key 文件，与之生成的证书文件名匹配。**



修改服务端的 server.ovpn，**其实服务端配置不修改也已经可以使用了**，如果想指定内部地址10.254.254.1可自行调整；默认的**crt**和**key**就是**server，**若名字不同，则需要修改一下。



确保客户端 DHCP Client 服务打开，客户端需要通过这个服务接收服务端分配的 IP。



*~~打开操作系统自带的 DHCP Client 服务（管理工具的服务，但通常是自动），用于获取被分配的IP地址。~~*



部署完毕后，服务端运行 OpenVPN GUI，右键选择 **server** 再选择**connect**，激活服务端连接。注意看日志，或者 控制台运行**openvpn --config server** 也可以，日志将详细显示连接情况，若有问题很容易排查，全部调试通过后建议以自动启动的服务的方式运行（服务内已经被安装了OpenVPN的一项，默认手动），这样不登入桌面也可以使用VPN。



客户端运行 OpenVPN GUI，右键选择 **client1 **在选择**connect**，尝试连接服务端。注意看日志，便于排除问题，这次连接成功后，下次可以使用系统服务来使用OpenVPN，不需要OpenVPN GUI，这样使用更方便。



至此 OpenVPN 连接完成。



*至于看不明白是为什么的，照葫芦画瓢走一两道，应该就明白了。*



**有一点注意，若需要更新证书，自己又处于远程位置，则在服务端控制台将证书全部生成后，先把客户端证书下载好（但不部署），接着部署服务端证书，然后重新启动 OpenVPN 的服务端的服务（这时VPN中断），再将下载好的客户端证书部署好，并重新连接，即完成全部证书更新，不过整个过程需特别谨慎，不能有差错，否则可能被关在服务器外面，或者先把3389打开，再更新证书，但风险会有提高。**



官方推荐使用UDP协议（UDP要比TCP节省资源），不过据我个人测试下来，TCP连接在意外中断后自动重连接速度更快一些，但UDP协议兼容性更高；客户端和服务端的配置文件还有一些其他属性，各位可以参考备注或者下面留的链接。



通过配置文件的 client to client 开关，还能将互联网上的多台终端集合到一个虚拟的局域网中，实现相互访问。



主要参考资料 来自 [https://openvpn.net](https://openvpn.net/) 不过无法通过正常渠道打开，大家可以使用国外免费代'|'理进行访问，或借助一些工具，将更简单，进群寻找答案。

[https://openvpn.net/index.php/open-source/documentation/howto.html](http://openvpn.net/index.php/open-source/documentation/howto.html)

[https://openvpn.net/index.php/open-source/documentation/examples.html](http://openvpn.net/index.php/open-source/documentation/examples.html)



另推荐一本书《OpenVPN 2 Cookbook》，有100例使用配置，通常足够使用了（群共享有下载）。



若需要 [Android](http://lib.csdn.net/base/android) 客户端，可进入

[https://play.google.com/store/apps/details?id=net.openvpn.privatetunnel](https://play.google.com/store/apps/details?id=net.openvpn.privatetunnel)

[https://play.google.com/store/apps/details?id=net.openvpn.openvpn](https://play.google.com/store/apps/details?id=net.openvpn.openvpn)

下载，[android](http://lib.csdn.net/base/android) 4.0 以上支持。



***适当调整服务端配置，并且在加上一个NAT服务，OpenVPN 就是一个FQ利器……***



**注：此文与免流无关，以免流为目的加群者莫自取其辱。免流实为作恶，或盗窃，或销赃，奉劝莫用免流。**

**有问题Q群讨论：236201801**群内共享提供 OpenVPN 下载。****



[FreeBSD 下 OpenVPN 客户端和服务端的安装与配置](http://blog.csdn.net/joyous/article/details/38048425)



# 原文参考，来自 [www.openvpn.net](http://www.openvpn.net/)



## Introduction



OpenVPN is a full-featured SSL VPN which implements OSI layer 2 or 3 secure network extension using the industry standard SSL/TLS protocol, supports flexible client authentication methods based on certificates, smart cards, and/or username/password credentials, and allows user or group-specific access control policies using firewall rules applied to the VPN virtual interface. OpenVPN is not a web application proxy and does not operate through a web browser.



OpenVPN 2.0 expands on the capabilities of [OpenVPN 1.x](http://openvpn.net/index.php/open-source/documentation/miscellaneous/88-1xhowto.html) by offering a scalable client/server mode, allowing multiple clients to connect to a single OpenVPN server process over a single TCP or UDP port. OpenVPN 2.3 includes[a large number of improvements](https://community.openvpn.net/openvpn/wiki/ChangesInOpenvpn23), including full IPv6 support and PolarSSL support.



This document provides step-by-step instructions for configuring an OpenVPN 2.x client/server VPN, including:


- [OpenVPN Quickstart](http://openvpn.net/index.php/open-source/documentation/howto.html#quick).
- [Installing OpenVPN](http://openvpn.net/index.php/open-source/documentation/howto.html#install).
- [Determining whether to use a routed or bridged VPN](http://openvpn.net/index.php/open-source/documentation/howto.html#vpntype).
- [Numbering private subnets](http://openvpn.net/index.php/open-source/documentation/howto.html#numbering).
- [Setting up your own Certificate Authority (CA) and generating certificates and keys for an OpenVPN server and multiple clients](http://openvpn.net/index.php/open-source/documentation/howto.html#pki).
- [Creating configuration files for server and clients](http://openvpn.net/index.php/open-source/documentation/howto.html#config).
- [Starting up the VPN and testing for initial connectivity](http://openvpn.net/index.php/open-source/documentation/howto.html#start).
- [Configuring OpenVPN to run automatically on system startup](http://openvpn.net/index.php/open-source/documentation/howto.html#startup).
- [Controlling a running OpenVPN process](http://openvpn.net/index.php/open-source/documentation/howto.html#control).
- [Expanding the scope of the VPN to include additional machines on either the client or server subnet](http://openvpn.net/index.php/open-source/documentation/howto.html#scope).
- [Pushing DHCP options to clients](http://openvpn.net/index.php/open-source/documentation/howto.html#dhcp).
- [Configuring client-specific rules and access policies](http://openvpn.net/index.php/open-source/documentation/howto.html#policy).
- [Using alternative authentication methods](http://openvpn.net/index.php/open-source/documentation/howto.html#auth).
- [How to add dual-factor authentication to an OpenVPN configuration using client-side smart cards](http://openvpn.net/index.php/open-source/documentation/howto.html#pkcs11).
- [Routing all client traffic (including web-traffic) through the VPN](http://openvpn.net/index.php/open-source/documentation/howto.html#redirect).
- [Running an OpenVPN server on a dynamic IP address](http://openvpn.net/index.php/open-source/documentation/howto.html#dynamic).
- [Connecting to an OpenVPN server via an HTTP proxy](http://openvpn.net/index.php/open-source/documentation/howto.html#http).
- [Connecting to a Samba share over OpenVPN](http://openvpn.net/index.php/open-source/documentation/howto.html#samba).
- [Implementing a load-balancing/failover configuration](http://openvpn.net/index.php/open-source/documentation/howto.html#loadbalance).
- [Hardening OpenVPN Security](http://openvpn.net/index.php/open-source/documentation/howto.html#security).
- [Revoking Certificates](http://openvpn.net/index.php/open-source/documentation/howto.html#revoke).
- [Additional Security Notes](http://openvpn.net/index.php/open-source/documentation/howto.html#secnotes).



The impatient may wish to jump straight to the sample configuration files:


- [Server configuration file](http://openvpn.net/index.php/open-source/documentation/howto.html#server).
- [Client configuration file](http://openvpn.net/index.php/open-source/documentation/howto.html#client).



## Intended Audience



This HOWTO assumes that readers possess a prior understanding of basic networking concepts such as IP addresses, DNS names, netmasks, subnets, IP routing, routers, network interfaces, LANs, gateways, and firewall rules.



## Additional Documentation



### OpenVPN Books



Please take a look at the [OpenVPN books page.](http://openvpn.net/index.php/open-source/books.html)



### OpenVPN 1.x HOWTO



The original [OpenVPN 1.x HOWTO](http://openvpn.net/index.php/open-source/documentation/miscellaneous/88-1xhowto.html) is still available, and remains relevant for point-to-point or static-key configurations.



### OpenVPN Articles



For additional documentation, see the [articles page](http://openvpn.net/index.php/open-source/articles.html) and the [OpenVPN wiki](https://community.openvpn.net/openvpn).



### OpenVPN Quickstart



While this HOWTO will guide you in setting up a scalable client/server VPN using an X509 PKI (public key infrastruction using certificates and private keys), this might be overkill if you are only looking for a simple VPN setup with a server that can handle a single client.



If you would like to get a VPN running quickly with minimal configuration, you might check out the[Static Key Mini-HOWTO](http://openvpn.net/index.php/open-source/documentation/miscellaneous/78-static-key-mini-howto.html).



### Static Key advantages


- Simple Setup
- No X509 PKI (Public Key Infrastructure) to maintain



### Static Key disadvantages


- Limited scalability -- one client, one server
- Lack of *perfect forward secrecy* -- key compromise results in total disclosure of previous sessions
- Secret key must exist in plaintext form on each VPN peer
- Secret key must be exchanged using a pre-existing secure channel









## Installing OpenVPN



OpenVPN source code and Windows installers can be [downloaded here](http://openvpn.net/index.php/open-source/downloads.html). Recent releases (2.2 and later) are also available as Debian and RPM packages; see the[OpenVPN wiki](https://community.openvpn.net/openvpn) for details.



For security, it's a good idea to check the [file release signature](http://openvpn.net/index.php/open-source/documentation/sig.html) after downloading.



The OpenVPN executable should be installed on both server and client machines, since the single executable provides both client and server functions.



### Linux Notes (using RPM package)



If you are using a [Linux](http://lib.csdn.net/base/linux) distribution which supports RPM packages (SuSE, Fedora, Redhat, etc.), it's best to install using this mechanism. The easiest method is to find an existing binary RPM file for your distribution. You can also build your own binary RPM file:



> **rpmbuild -tb openvpn-[version].tar.gz**



Once you have the .rpm file, you can install it with the usual



> **rpm -ivh openvpn-[details].rpm**



or upgrade an existing installation with



> **rpm -Uvh openvpn-[details].rpm**



Installing OpenVPN from a binary RPM package has these dependencies:


- openssl
- lzo
- pam



Furthermore, if you are building your own binary RPM package, there are several additional dependencies:


- openssl-devel
- lzo-devel
- pam-devel



See the [openvpn.spec](http://openvpn.net/index.php/open-source/documentation/install.html#rpm) file for additional notes on building an RPM package for Red Hat [linux](http://lib.csdn.net/base/linux) 9 or building with reduced dependencies.



### Linux Notes (without RPM)



If you are using Debian, Gentoo, or a non-RPM-based Linux distribution, use your distro-specific packaging mechanism such as**apt-get** on Debian or**emerge** on Gentoo.



It is also possible to install OpenVPN on Linux using the universal **./configure** method. First expand the .tar.gz file:



> **tar xfz openvpn-[version].tar.gz**



Then cd to the top-level directory and type:



> **./configuremakemake install**







### Windows Notes



OpenVPN for Windows can be installed from the self-installing exe file on the [OpenVPN download page](http://openvpn.net/index.php/open-source/downloads.html). Remember that OpenVPN will only run on Windows XP or later. Also note that OpenVPN must be installed and run by a user who has administrative privileges (this restriction is imposed by Windows, not OpenVPN). The restriction can be sidestepped by running OpenVPN in the background as a service, in which case even non-admin users will be able to access the VPN, once it is installed.[More discussion on OpenVPN + Windows privilege issues](http://openvpn.se/files/howto/openvpn-howto_run_openvpn_as_nonadmin.html).



Official OpenVPN Windows installers include [OpenVPN-GUI](https://community.openvpn.net/openvpn/wiki/OpenVPN-GUI), which allows managing OpenVPN connections from a system tray applet. Other[GUI](http://openvpn.net/index.php/open-source/documentation/graphical-user-interface.html) applications are also available.



After you've run the Windows installer, OpenVPN is ready for use and will associate itself with files having the**.ovpn** extension. To run OpenVPN, you can:


- Right click on an OpenVPN configuration file (.ovpn) and select **Start OpenVPN on this configuration file**. Once running, you can use the**F4**key to exit.



- Run OpenVPN from a command prompt Window with a command such as:



> **openvpn myconfig.ovpn**

Once running in a command prompt window, OpenVPN can be stopped by the **F4** key.

- Run OpenVPN as a service by putting one or more .ovpn configuration files in **\Program Files\OpenVPN\config** and starting the OpenVPN Service, which can be controlled from Start Menu -> Control Panel -> Administrative Tools -> Services.



[Additional Windows install notes.](http://openvpn.net/index.php/open-source/documentation/install.html?start=1)



### Mac OS X Notes



Angelo Laub and Dirk Theisen have developed an [OpenVPN GUI for OS X](http://code.google.com/p/tunnelblick/).



### Other OSes



Some notes are available in the [INSTALL](http://openvpn.net/index.php/open-source/documentation/install.html?start=1) file for specific OSes. In general, the



> **./configuremakemake install**



method can be used, or you can search for an OpenVPN port or package which is specific to your OS/distribution.









## Determining whether to use a routed or bridged VPN



See [FAQ](http://openvpn.net/index.php/open-source/faq.html#bridge1) for an overview of Routing vs. Ethernet Bridging. See also the OpenVPN [Ethernet Bridging](http://openvpn.net/index.php/open-source/documentation/miscellaneous/76-ethernet-bridging.html) page for more notes and details on bridging.



Overall, routing is probably a better choice for most people, as it is more efficient and easier to set up (as far as the OpenVPN configuration itself) than bridging. Routing also provides a greater ability to selectively control access rights on a client-specific basis.



I would recommend using routing unless you need a specific feature which requires bridging, such as:


- the VPN needs to be able to handle non-IP protocols such as IPX,
- you are running applications over the VPN which rely on network broadcasts (such as LAN games), or
- you would like to allow browsing of Windows file shares across the VPN without setting up a Samba or WINS server.









## Numbering private subnets



Setting up a VPN often entails linking together private subnets from different locations.



The Internet Assigned Numbers Authority (IANA) has reserved the following three blocks of the IP address space for private internets (codified in RFC 1918):


|10.0.0.0|10.255.255.255|(10/8 prefix)|
|----|----|----|
|172.16.0.0|172.31.255.255|(172.16/12 prefix)|
|192.168.0.0|192.168.255.255|(192.168/16 prefix)|



While addresses from these netblocks should normally be used in VPN configurations, it's important to select addresses that minimize the probability of IP address or subnet conflicts. The types of conflicts that need to be avoided are:


- conflicts from different sites on the VPN using the same LAN subnet numbering, or
- remote access connections from sites which are using private subnets which conflict with your VPN subnets.



For example, suppose you use the popular 192.168.0.0/24 subnet as your private LAN subnet. Now you are trying to connect to the VPN from an internet cafe which is using the same subnet for its WiFi LAN. You will have a routing conflict because your machine won't know if 192.168.0.1 refers to the local WiFi gateway or to the same address on the VPN.



As another example, suppose you want to link together multiple sites by VPN, but each site is using 192.168.0.0/24 as its LAN subnet. This won't work without adding a complexifying layer of NAT translation, because the VPN won't know how to route packets between multiple sites if those sites don't use a subnet which uniquely identifies them.



The best solution is to avoid using 10.0.0.0/24 or 192.168.0.0/24 as private LAN network addresses. Instead, use something that has a lower probability of being used in a WiFi cafe, airport, or hotel where you might expect to connect from remotely. The best candidates are subnets in the middle of the vast 10.0.0.0/8 netblock (for example 10.66.77.0/24).



And to avoid cross-site IP numbering conflicts, always use unique numbering for your LAN subnets.









## Setting up your own Certificate Authority (CA) and generating certificates and keys for an OpenVPN server and multiple clients



### Overview



The first step in building an OpenVPN 2.x configuration is to establish a PKI (public key infrastructure). The PKI consists of:


- a separate certificate (also known as a public key) and private key for the server and each client, and
- a master Certificate Authority (CA) certificate and key which is used to sign each of the server and client certificates.



OpenVPN supports bidirectional authentication based on certificates, meaning that the client must authenticate the server certificate and the server must authenticate the client certificate before mutual trust is established.



Both server and client will authenticate the other by first verifying that the presented certificate was signed by the master certificate authority (CA), and then by testing information in the now-authenticated certificate header, such as the certificate common name or certificate type (client or server).



This security model has a number of desirable features from the VPN perspective:


- The server only needs its own certificate/key -- it doesn't need to know the individual certificates of every client which might possibly connect to it.
- The server will only accept clients whose certificates were signed by the master CA certificate (which we will generate below). And because the server can perform this signature verification without needing access to the CA private key itself, it is possible for the CA key (the most sensitive key in the entire PKI) to reside on a completely different machine, even one without a network connection.
- If a private key is compromised, it can be disabled by adding its certificate to a CRL (certificate revocation list). The CRL allows compromised certificates to be selectively rejected without requiring that the entire PKI be rebuilt.
- The server can enforce client-specific access rights based on embedded certificate fields, such as the Common Name.



Note that the server and client clocks need to be roughly in sync or certificates might not work properly.



### Generate the master Certificate Authority (CA) certificate & key



In this section we will generate a master CA certificate/key, a server certificate/key, and certificates/keys for 3 separate clients.



For PKI management, we will use *easy-rsa*, a set of scripts which is bundled with OpenVPN 2.2.x and earlier. If you're using OpenVPN 2.3.x, you need to download easy-rsa separately from[here](https://github.com/OpenVPN/easy-rsa).



If you are using Linux, BSD, or a unix-like OS, open a shell and cd to the **easy-rsa** subdirectory. If you installed OpenVPN from an RPM or DEB file, the easy-rsa directory can usually be found in**/usr/share/doc/packages/openvpn** or**/usr/share/doc/openvpn**(it's best to copy this directory to another location such as**/etc/openvpn**, before any edits, so that future OpenVPN package upgrades won't overwrite your modifications). If you installed from a .tar.gz file, the easy-rsa directory will be in the top level directory of the expanded source tree.



If you are using Windows, open up a Command Prompt window and cd to **\Program Files\OpenVPN\easy-rsa**. Run the following batch file to copy configuration files into place (this will overwrite any preexisting vars.bat and openssl.cnf files):



> **init-config**



Now edit the **vars** file (called **vars.bat** on Windows) and set the KEY_COUNTRY, KEY_PROVINCE, KEY_CITY, KEY_ORG, and KEY_EMAIL parameters. Don't leave any of these parameters blank.



Next, initialize the PKI. On Linux/BSD/Unix:



> **. ./vars./clean-all./build-ca**



On Windows:



> **varsclean-allbuild-ca**



The final command (**build-ca**) will build the certificate authority (CA) certificate and key by invoking the interactive**openssl**command:



> ai:easy-rsa # ./build-ca
Generating a 1024 bit RSA private key
............++++++
...........++++++
writing new private key to 'ca.key'
-----
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [KG]:
State or Province Name (full name) [NA]:
Locality Name (eg, city) [BISHKEK]:
Organization Name (eg, company) [OpenVPN-TEST]:
Organizational Unit Name (eg, section) []:
Common Name (eg, your name or your server's hostname) []:OpenVPN-CA
Email Address [me@myhost.mydomain]:



|Note that in the above sequence, most queried parameters were defaulted to the values set in the**vars** or**vars.bat** files. The only parameter which must be explicitly entered is the**Common Name**. In the example above, I used "OpenVPN-CA".### Generate certificate & key for serverNext, we will generate a certificate and private key for the server. On Linux/BSD/Unix:> **./build-key-server server**On Windows:> **build-key-server server**As in the previous step, most parameters can be defaulted. When the **Common Name** is queried, enter "server". Two other queries require positive responses, "Sign the certificate? [y/n]" and "1 out of 1 certificate requests certified, commit? [y/n]".### Generate certificates & keys for 3 clientsGenerating client certificates is very similar to the previous step. On Linux/BSD/Unix:> **./build-key client1./build-key client2./build-key client3**On Windows:> **build-key client1build-key client2build-key client3**If you would like to password-protect your client keys, substitute the **build-key-pass** script.Remember that for each client, make sure to type the appropriate **Common Name** when prompted, i.e. "client1", "client2", or "client3". Always use a unique common name for each client.### Generate Diffie Hellman parameters[Diffie Hellman](http://www.rsasecurity.com/rsalabs/node.asp?id=2248) parameters must be generated for the OpenVPN server. On Linux/BSD/Unix:> **./build-dh**On Windows:> **build-dh**Output:|
|----|




> ai:easy-rsa # ./build-dh
Generating DH parameters, 1024 bit long safe prime, generator 2
This is going to take a long time
.................+...........................................
...................+.............+.................+.........
......................................



|### Key FilesNow we will find our newly-generated keys and certificates in the **keys**subdirectory. Here is an explanation of the relevant files:|**Filename**|**Needed By**|**Purpose**|**Secret**||----|----|----|----||ca.crt|server + all clients|Root CA certificate|NO||ca.key|key signing machine only|Root CA key|YES||dh{n}.pem|server only|Diffie Hellman parameters|NO||server.crt|server only|Server Certificate|NO||server.key|server only|Server Key|YES||client1.crt|client1 only|Client1 Certificate|NO||client1.key|client1 only|Client1 Key|YES||client2.crt|client2 only|Client2 Certificate|NO||client2.key|client2 only|Client2 Key|YES||client3.crt|client3 only|Client3 Certificate|NO||client3.key|client3 only|Client3 Key|YES|The final step in the key generation process is to copy all files to the machines which need them, taking care to copy secret files over a secure channel.Now wait, you may say. Shouldn't it be possible to set up the PKI without a pre-existing secure channel?The answer is ostensibly yes. In the example above, for the sake of brevity, we generated all private keys in the same place. With a bit more effort, we could have done this differently. For example, instead of generating the client certificate and keys on the server, we could have had the client generate its own private key locally, and then submit a Certificate Signing Request (CSR) to the key-signing machine. In turn, the key-signing machine could have processed the CSR and returned a signed certificate to the client. This could have been done without ever requiring that a secret **.key** file leave the hard drive of the machine on which it was generated.## Creating configuration files for server and clients### Getting the sample config filesIt's best to use the OpenVPN [sample configuration files](http://openvpn.net/index.php/open-source/documentation/howto.html#examples) as a starting point for your own configuration. These files can also be found in- the **sample-config-files** directory of the OpenVPN source distribution- the **sample-config-files** directory in **/usr/share/doc/packages/openvpn**or**/usr/share/doc/openvpn** if you installed from an RPM or DEB package- **Start Menu -> All Programs -> OpenVPN -> OpenVPN Sample Configuration Files** on WindowsNote that on Linux, BSD, or unix-like OSes, the sample configuration files are named**server.conf** and**client.conf**. On Windows they are named**server.ovpn**and**client.ovpn**.### Editing the server configuration fileThe sample server configuration file is an ideal starting point for an OpenVPN server configuration. It will create a VPN using a virtual**TUN** network interface (for routing), will listen for client connections on**UDP port 1194** (OpenVPN's official port number), and distribute virtual addresses to connecting clients from the**10.8.0.0/24** subnet.Before you use the sample configuration file, you should first edit the **ca**, **cert**,**key**, and **dh** parameters to point to the files you generated in the[PKI](http://openvpn.net/index.php/open-source/documentation/howto.html#pki) section above.At this point, the server configuration file is usable, however you still might want to customize it further:- If you are using [Ethernet bridging](http://openvpn.net/index.php/open-source/documentation/miscellaneous/76-ethernet-bridging.html), you must use **server-bridge** and **dev tap** instead of**server** and**dev tun**.- If you want your OpenVPN server to listen on a TCP port instead of a UDP port, use**proto tcp** instead of**proto udp** (If you want OpenVPN to listen on both a UDP and TCP port, you must run two separate OpenVPN instances).- If you want to use a virtual IP address range other than **10.8.0.0/24**, you should modify the**server** directive. Remember that this virtual IP address range should be a private range which is currently unused on your network.- Uncomment out the **client-to-client** directive if you would like connecting clients to be able to reach each other over the VPN. By default, clients will only be able to reach the server.- If you are using Linux, BSD, or a Unix-like OS, you can improve security by uncommenting out the**user nobody** and**group nobody** directives.If you want to run multiple OpenVPN instances on the same machine, each using a different configuration file, it is possible if you:- Use a different **port** number for each instance (the UDP and TCP protocols use different port spaces so you can run one daemon listening on UDP-1194 and another on TCP-1194).- If you are using Windows, each OpenVPN configuration taneeds to have its own TAP-Windows adapter. You can add additional adapters by going to**Start Menu -> All Programs -> TAP-Windows -> Add a new TAP-Windows virtual ethernet adapter**.- If you are running multiple OpenVPN instances out of the same directory, make sure to edit directives which create output files so that multiple instances do not overwrite each other's output files. These directives include**log**,**log-append**,**status**, and **ifconfig-pool-persist**.### Editing the client configuration filesThe sample client configuration file (**client.conf** on Linux/BSD/Unix or**client.ovpn** on Windows) mirrors the default directives set in the sample server configuration file.- Like the server configuration file, first edit the **ca**, **cert**, and **key** parameters to point to the files you generated in the[PKI](http://openvpn.net/howto.html#pki) section above. Note that each client should have its own**cert**/**key** pair. Only the **ca**file is universal across the OpenVPN server and all clients.- Next, edit the **remote**directive to point to the hostname/IP address and port number of the OpenVPN server (if your OpenVPN server will be running on a single-NIC machine behind a firewall/NAT-gateway, use the public IP address of the gateway, and a port number which you have configured the gateway to forward to the OpenVPN server).- Finally, ensure that the client configuration file is consistent with the directives used in the server configuration. The major thing to check for is that the**dev** (tun or tap) and**proto** (udp or tcp) directives are consistent. Also make sure that**comp-lzo** and**fragment**, if used, are present in both client and server config files.## Starting up the VPN and testing for initial connectivity### Starting the serverFirst, make sure the OpenVPN server will be accessible from the internet. That means:- opening up UDP port 1194 on the firewall (or whatever TCP/UDP port you've configured), or- setting up a port forward rule to forward UDP port 1194 from the firewall/gateway to the machine running the OpenVPN server.Next, [make sure that the TUN/TAP interface is not firewalled](http://openvpn.net/faq.html#firewall).To simplify troubleshooting, it's best to initially start the OpenVPN server from the command line (or right-click on the**.ovpn** file on Windows), rather than start it as a daemon or service:> **openvpn [server config file] **A normal server startup should look like this (output will vary across platforms):|**Filename**|**Needed By**|**Purpose**|**Secret**|ca.crt|server + all clients|Root CA certificate|NO|ca.key|key signing machine only|Root CA key|YES|dh{n}.pem|server only|Diffie Hellman parameters|NO|server.crt|server only|Server Certificate|NO|server.key|server only|Server Key|YES|client1.crt|client1 only|Client1 Certificate|NO|client1.key|client1 only|Client1 Key|YES|client2.crt|client2 only|Client2 Certificate|NO|client2.key|client2 only|Client2 Key|YES|client3.crt|client3 only|Client3 Certificate|NO|client3.key|client3 only|Client3 Key|YES|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|**Filename**|**Needed By**|**Purpose**|**Secret**| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|ca.crt|server + all clients|Root CA certificate|NO| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|ca.key|key signing machine only|Root CA key|YES| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|dh{n}.pem|server only|Diffie Hellman parameters|NO| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|server.crt|server only|Server Certificate|NO| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|server.key|server only|Server Key|YES| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|client1.crt|client1 only|Client1 Certificate|NO| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|client1.key|client1 only|Client1 Key|YES| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|client2.crt|client2 only|Client2 Certificate|NO| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|client2.key|client2 only|Client2 Key|YES| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|client3.crt|client3 only|Client3 Certificate|NO| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|client3.key|client3 only|Client3 Key|YES| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |

The final step in the key generation process is to copy all files to the machines which need them, taking care to copy secret files over a secure channel.

Now wait, you may say. Shouldn't it be possible to set up the PKI without a pre-existing secure channel?



The answer is ostensibly yes. In the example above, for the sake of brevity, we generated all private keys in the same place. With a bit more effort, we could have done this differently. For example, instead of generating the client certificate and keys on the server, we could have had the client generate its own private key locally, and then submit a Certificate Signing Request (CSR) to the key-signing machine. In turn, the key-signing machine could have processed the CSR and returned a signed certificate to the client. This could have been done without ever requiring that a secret **.key** file leave the hard drive of the machine on which it was generated.

## Creating configuration files for server and clients

### Getting the sample config files

It's best to use the OpenVPN [sample configuration files](http://openvpn.net/index.php/open-source/documentation/howto.html#examples) as a starting point for your own configuration. These files can also be found in
- the **sample-config-files** directory of the OpenVPN source distribution
- the **sample-config-files** directory in **/usr/share/doc/packages/openvpn**or**/usr/share/doc/openvpn** if you installed from an RPM or DEB package
- **Start Menu -> All Programs -> OpenVPN -> OpenVPN Sample Configuration Files** on Windows

Note that on Linux, BSD, or unix-like OSes, the sample configuration files are named**server.conf** and**client.conf**. On Windows they are named**server.ovpn**and**client.ovpn**.

### Editing the server configuration file

The sample server configuration file is an ideal starting point for an OpenVPN server configuration. It will create a VPN using a virtual**TUN** network interface (for routing), will listen for client connections on**UDP port 1194** (OpenVPN's official port number), and distribute virtual addresses to connecting clients from the**10.8.0.0/24** subnet.

Before you use the sample configuration file, you should first edit the **ca**, **cert**,**key**, and **dh** parameters to point to the files you generated in the[PKI](http://openvpn.net/index.php/open-source/documentation/howto.html#pki) section above.

At this point, the server configuration file is usable, however you still might want to customize it further:
- If you are using [Ethernet bridging](http://openvpn.net/index.php/open-source/documentation/miscellaneous/76-ethernet-bridging.html), you must use **server-bridge** and **dev tap** instead of**server** and**dev tun**.
- If you want your OpenVPN server to listen on a TCP port instead of a UDP port, use**proto tcp** instead of**proto udp** (If you want OpenVPN to listen on both a UDP and TCP port, you must run two separate OpenVPN instances).
- If you want to use a virtual IP address range other than **10.8.0.0/24**, you should modify the**server** directive. Remember that this virtual IP address range should be a private range which is currently unused on your network.
- Uncomment out the **client-to-client** directive if you would like connecting clients to be able to reach each other over the VPN. By default, clients will only be able to reach the server.
- If you are using Linux, BSD, or a Unix-like OS, you can improve security by uncommenting out the**user nobody** and**group nobody** directives.

If you want to run multiple OpenVPN instances on the same machine, each using a different configuration file, it is possible if you:
- Use a different **port** number for each instance (the UDP and TCP protocols use different port spaces so you can run one daemon listening on UDP-1194 and another on TCP-1194).
- If you are using Windows, each OpenVPN configuration taneeds to have its own TAP-Windows adapter. You can add additional adapters by going to**Start Menu -> All Programs -> TAP-Windows -> Add a new TAP-Windows virtual ethernet adapter**.
- If you are running multiple OpenVPN instances out of the same directory, make sure to edit directives which create output files so that multiple instances do not overwrite each other's output files. These directives include**log**,**log-append**,**status**, and **ifconfig-pool-persist**.

### Editing the client configuration files

The sample client configuration file (**client.conf** on Linux/BSD/Unix or**client.ovpn** on Windows) mirrors the default directives set in the sample server configuration file.
- Like the server configuration file, first edit the **ca**, **cert**, and **key** parameters to point to the files you generated in the[PKI](http://openvpn.net/howto.html#pki) section above. Note that each client should have its own**cert**/**key** pair. Only the **ca**file is universal across the OpenVPN server and all clients.



- Next, edit the **remote**directive to point to the hostname/IP address and port number of the OpenVPN server (if your OpenVPN server will be running on a single-NIC machine behind a firewall/NAT-gateway, use the public IP address of the gateway, and a port number which you have configured the gateway to forward to the OpenVPN server).



- Finally, ensure that the client configuration file is consistent with the directives used in the server configuration. The major thing to check for is that the**dev** (tun or tap) and**proto** (udp or tcp) directives are consistent. Also make sure that**comp-lzo** and**fragment**, if used, are present in both client and server config files.




## Starting up the VPN and testing for initial connectivity

### Starting the server

First, make sure the OpenVPN server will be accessible from the internet. That means:
- opening up UDP port 1194 on the firewall (or whatever TCP/UDP port you've configured), or
- setting up a port forward rule to forward UDP port 1194 from the firewall/gateway to the machine running the OpenVPN server.

Next, [make sure that the TUN/TAP interface is not firewalled](http://openvpn.net/faq.html#firewall).

To simplify troubleshooting, it's best to initially start the OpenVPN server from the command line (or right-click on the**.ovpn** file on Windows), rather than start it as a daemon or service:

> **openvpn [server config file] **

A normal server startup should look like this (output will vary across platforms):




> Sun Feb  6 20:46:38 2005 OpenVPN 2.0_rc12 i686-suse-linux [SSL] [LZO] [EPOLL] built on Feb  5 2005
Sun Feb  6 20:46:38 2005 Diffie-Hellman initialized with 1024 bit key
Sun Feb  6 20:46:38 2005 TLS-Auth MTU parms [ L:1542 D:138 EF:38 EB:0 ET:0 EL:0 ]
Sun Feb  6 20:46:38 2005 TUN/TAP device tun1 opened
Sun Feb  6 20:46:38 2005 /sbin/ifconfig tun1 10.8.0.1 pointopoint 10.8.0.2 mtu 1500
Sun Feb  6 20:46:38 2005 /sbin/route add -net 10.8.0.0 netmask 255.255.255.0 gw 10.8.0.2
Sun Feb  6 20:46:38 2005 Data Channel MTU parms [ L:1542 D:1450 EF:42 EB:23 ET:0 EL:0 AF:3/1 ]
Sun Feb  6 20:46:38 2005 UDPv4 link local (bound): [undef]:1194
Sun Feb  6 20:46:38 2005 UDPv4 link remote: [undef]
Sun Feb  6 20:46:38 2005 MULTI: multi_init called, r=256 v=256
Sun Feb  6 20:46:38 2005 IFCONFIG POOL: base=10.8.0.4 size=62
Sun Feb  6 20:46:38 2005 IFCONFIG POOL LIST
Sun Feb  6 20:46:38 2005 Initialization Sequence Completed



|### Starting the clientAs in the server configuration, it's best to initially start the OpenVPN server from the command line (or on Windows, by right-clicking on the**client.ovpn** file), rather than start it as a daemon or service:> **openvpn [client config file] **A normal client startup on Windows will look similar to the server output above, and should end with the**Initialization Sequence Completed** message.Now, try a ping across the VPN from the client. If you are using routing (i.e.**dev tun** in the server config file), try:> **ping 10.8.0.1**If you are using bridging (i.e. **dev tap** in the server config file), try to ping the IP address of a machine on the server's ethernet subnet.If the ping succeeds, congratulations! You now have a functioning VPN.### TroubleshootingIf the ping failed or the OpenVPN client initialization failed to complete, here is a checklist of common symptoms and their solutions:- You get the error message: **TLS Error: TLS key negotiation failed to occur within 60 seconds (check your network connectivity)**. This error indicates that the client was unable to establish a network connection with the server.**Solutions**:- Make sure the client is using the correct hostname/IP address and port number which will allow it to reach the OpenVPN server.- If the OpenVPN server machine is a single-NIC box inside a protected LAN, make sure you are using a correct port forward rule on the server's gateway firewall. For example, suppose your OpenVPN box is at 192.168.4.4 inside the firewall, listening for client connections on UDP port 1194. The NAT gateway servicing the 192.168.4.x subnet should have a port forward rule that says**forward UDP port 1194 from my public IP address to 192.168.4.4**.- Open up the server's firewall to allow incoming connections to UDP port 1194 (or whatever TCP/UDP port you have configured in the server config file).- You get the error message: **Initialization Sequence Completed with errors**-- This error can occur on Windows if (a) You don't have the DHCP client service running, or (b) You are using certain third-party personal firewalls on XP SP2.**Solution**: Start the DHCP client server and make sure that you are using a personal firewall which is known to work correctly on XP SP2.- You get the **Initialization Sequence Completed**message but the ping test fails -- This usually indicates that a firewall on either server or client is blocking VPN network traffic by filtering on the TUN/TAP interface.**Solution**: Disable the client firewall (if one exists) from filtering the TUN/TAP interface on the client. For example on Windows XP SP2, you can do this by going to**Windows Security Center -> Windows Firewall -> Advanced** and unchecking the box which corresponds to the TAP-Windows adapter (disabling the client firewall from filtering the TUN/TAP adapter is generally reasonable from a security perspective, as you are essentially telling the firewall not to block authenticated VPN traffic). Also make sure that the TUN/TAP interface on the server is not being filtered by a firewall (having said that, note that selective firewalling of the TUN/TAP interface on the server side can confer certain security benefits. See the[access policies](http://openvpn.net/howto.html#policy) section below).- The connection stalls on startup when using a **proto udp**configuration, the server log file shows this line:> TLS: Initial packet from x.x.x.x:x, sid=xxxxxxxx xxxxxxxxhowever the client log does not show an equivalent line.**Solution**: You have a one-way connection from client to server. The server to client direction is blocked by a firewall, usually on the client side. The firewall can either be (a) a personal software firewall running on the client, or (b) the NAT router gateway for the client. Modify the firewall to allow returning UDP packets from the server to reach the client.See the [FAQ](http://openvpn.net/index.php/open-source/faq.html) for additional troubleshooting information.## Configuring OpenVPN to run automatically on system startupThe lack of standards in this area means that most OSes have a different way of configuring daemons/services for autostart on boot. The best way to have this functionality configured by default is to install OpenVPN as a package, such as via RPM on Linux or using the Windows installer.### LinuxIf you install OpenVPN via an RPM or DEB package on Linux, the installer will set up an**initscript**. When executed, the initscript will scan for**.conf** configuration files in**/etc/openvpn**, and if found, will start up a separate OpenVPN daemon for each file.### WindowsThe Windows installer will set up a Service Wrapper, but leave it turned off by default. To activate it, [Go](http://lib.csdn.net/base/go) to Control Panel / Administrative Tools / Services, select the OpenVPN service, right-click on properties, and set the Startup Type to Automatic. This will configure the service for automatic start on the next reboot.When started, the OpenVPN Service Wrapper will scan the **\Program Files\OpenVPN\config** folder for**.ovpn** configuration files, starting a separate OpenVPN process on each file.## Controlling a running OpenVPN process### Running on Linux/BSD/UnixOpenVPN accepts several signals:- **SIGUSR1** -- Conditional restart, designed to restart without root privileges- **SIGHUP** -- Hard restart- **SIGUSR2** -- Output connection statistics to log file or syslog- **SIGTERM**, **SIGINT** -- ExitUse the **writepid** directive to write the OpenVPN daemon's PID to a file, so that you know where to send the signal (if you are starting openvpn with an**initscript**, the script may already be passing a**--writepid** directive on the**openvpn** command line).### Running on Windows as a GUISee the [OpenVPN GUI page](http://openvpn.net/index.php/open-source/documentation/graphical-user-interface.html).### Running in a Windows command prompt windowOn Windows, you can start OpenVPN by right clicking on an OpenVPN configuration file (**.ovpn** file) and selecting "Start OpenVPN on this config file".Once running in this fashion, several keyboard commands are available:- **F1** -- Conditional restart (doesn't close/reopen TAP adapter)- **F2** -- Show connection statistics- **F3** -- Hard restart- **F4** -- Exit### Running as a Windows ServiceWhen OpenVPN is started as a service on Windows, the only way to control it is:- Via the service control manager (Control Panel / Administrative Tools / Services) which gives start/stop control.- Via the management interface (see below).### Modifying a live server configurationWhile most configuration changes require you to restart the server, there are two directives in particular which refer to files which can be dynamically updated on-the-fly, and which will take immediate effect on the server without needing to restart the server process.**client-config-dir** -- This directive sets a client configuration directory, which the OpenVPN server will scan on every incoming connection, searching for a client-specific configuration file (see the[the manual page](http://openvpn.net/man.html) for more information). Files in this directory can be updated on-the-fly, without restarting the server. Note that changes in this directory will only take effect for new connections, not existing connections. If you would like a client-specific configuration file change to take immediate effect on a currently connected client (or one which has disconnected, but where the server has not timed-out its instance object), kill the client instance object by using the management interface (described below). This will cause the client to reconnect and use the new**client-config-dir** file.**crl-verify** -- This directive names a *Certificate Revocation List* file, described below in the[Revoking Certificates](http://openvpn.net/howto.html#revoke) section. The CRL file can be modified on the fly, and changes will take effect immediately for new connections, or existing connections which are renegotiating their SSL/TLS channel (occurs once per hour by default). If you would like to kill a currently connected client whose certificate has just been added to the CRL, use the management interface (described below).### Status FileThe default [server.conf](http://openvpn.net/howto.html#server) file has a line> **status openvpn-status.log**which will output a list of current client connections to the file **openvpn-status.log** once per minute.### Using the management interfaceThe [OpenVPN management interface](http://openvpn.net/management.html) allows a great deal of control over a running OpenVPN process. You can use the management interface directly, by telneting to the management interface port, or indirectly by using an[OpenVPN GUI](http://openvpn.net/index.php/open-source/documentation/graphical-user-interface.html) which itself connects to the management interface.To enable the management interface on either an OpenVPN server or client, add this to the configuration file:> **management localhost 7505**This tells OpenVPN to listen on TCP port 7505 for management interface clients (port 7505 is an arbitrary choice -- you can use any free port).Once OpenVPN is running, you can connect to the management interface using a **telnet** client. For example:|
|----|




> ai:~ # telnet localhost 7505
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
>INFO:OpenVPN Management Interface Version 1 -- type 'help' for more info
help
Management Interface for OpenVPN 2.0_rc14 i686-suse-linux [SSL] [LZO] [EPOLL] built on Feb 15 2005
Commands:
echo [on|off] [N|all]  : Like log, but only show messages in echo buffer.
exit|quit              : Close management session.
help                   : Print this message.
hold [on|off|release]  : Set/show hold flag to on/off state, or
                         release current hold and start tunnel.
kill cn                : Kill the client instance(s) having common name cn.
kill IP:port           : Kill the client instance connecting from IP:port.
log [on|off] [N|all]   : Turn on/off realtime log display
                         + show last N lines or 'all' for entire history.
mute [n]               : Set log mute level to n, or show level if n is absent.
net                    : (Windows only) Show network info and routing table.
password type p        : Enter password p for a queried OpenVPN password.
signal s               : Send signal s to daemon,
                         s = SIGHUP|SIGTERM|SIGUSR1|SIGUSR2.
state [on|off] [N|all] : Like log, but show state history.
status [n]             : Show current daemon status info using format #n.
test n                 : Produce n lines of output for testing/debugging.
username type u        : Enter username u for a queried OpenVPN username.
verb [n]               : Set log verbosity level to n, or show if n is absent.
version                : Show current version number.
END
exit
Connection closed by foreign host.
ai:~ #



|For more information, see the [OpenVPN Management Interface Documentation](http://openvpn.net/management.html).## Expanding the scope of the VPN to include additional machines on either the client or server subnet.### Including multiple machines on the server side when using a routed VPN (dev tun)Once the VPN is operational in a point-to-point capacity between client and server, it may be desirable to expand the scope of the VPN so that clients can reach multiple machines on the server network, rather than only the server machine itself.For the purpose of this example, we will assume that the server-side LAN uses a subnet of**10.66.0.0/24** and the VPN IP address pool uses**10.8.0.0/24** as cited in the**server** directive in the OpenVPN server configuration file.First, you must *advertise* the **10.66.0.0/24** subnet to VPN clients as being accessible through the VPN. This can easily be done with the following server-side config file directive:> **push "route 10.66.0.0 255.255.255.0"**Next, you must set up a route on the server-side LAN gateway to route the VPN client subnet (**10.8.0.0/24**) to the OpenVPN server (this is only necessary if the OpenVPN server and the LAN gateway are different machines).Make sure that you've enabled [IP](http://openvpn.net/faq.html#ip-forward) and [TUN/TAP](http://openvpn.net/faq.html#firewall) forwarding on the OpenVPN server machine.### Including multiple machines on the server side when using a bridged VPN (dev tap)One of the benefits of using [ethernet bridging](http://openvpn.net/bridge.html) is that you get this for free without needing any additional configuration.### Including multiple machines on the client side when using a routed VPN (dev tun)In a typical road-warrior or remote access scenario, the client machine connects to the VPN as a single machine. But suppose the client machine is a gateway for a local LAN (such as a home office), and you would like each machine on the client LAN to be able to route through the VPN.For this example, we will assume that the client LAN is using the **192.168.4.0/24**subnet, and that the VPN client is using a certificate with a common name of**client2**. Our goal is to set up the VPN so that any machine on the client LAN can communicate with any machine on the server LAN through the VPN.Before setup, there are some basic prerequisites which must be followed:- The client LAN subnet (192.168.4.0/24 in our example) must not be exported to the VPN by the server or any other client sites which are using the same subnet. Every subnet which is joined to the VPN via routing must be unique.- The client must have a unique Common Name in its certificate ("client2" in our example), and the**duplicate-cn** flag must not be used in the OpenVPN server configuration file.First, make sure that [IP](http://openvpn.net/faq.html#ip-forward) and [TUN/TAP](http://openvpn.net/faq.html#firewall) forwarding is enabled on the client machine.Next, we will deal with the necessary configuration changes on the server side. If the server configuration file does not currently reference a client configuration directory, add one now:> **client-config-dir ccd**In the above directive, **ccd** should be the name of a directory which has been pre-created in the default directory where the OpenVPN server daemon runs. On Linux this tends to be**/etc/openvpn** and on Windows it is usually**\Program Files\OpenVPN\config**. When a new client connects to the OpenVPN server, the daemon will check this directory for a file which matches the common name of the connecting client. If a matching file is found, it will be read and processed for additional configuration file directives to be applied to the named client.The next step is to create a file called **client2** in the **ccd** directory. This file should contain the line:> **iroute 192.168.4.0 255.255.255.0**This will tell the OpenVPN server that the 192.168.4.0/24 subnet should be routed to**client2**.Next, add the following line to the main server config file (not the **ccd/client2**file):> **route 192.168.4.0 255.255.255.0**Why the redundant **route** and **iroute** statements, you might ask? The reason is that**route** controls the routing from the kernel to the OpenVPN server (via the TUN interface) while**iroute** controls the routing from the OpenVPN server to the remote clients. Both are necessary.Next, ask yourself if you would like to allow network traffic between client2's subnet (192.168.4.0/24) and other clients of the OpenVPN server. If so, add the following to the server config file.> **client-to-clientpush "route 192.168.4.0 255.255.255.0"**This will cause the OpenVPN server to *advertise* client2's subnet to other connecting clients.The last step, and one that is often forgotten, is to add a route to the server's LAN gateway which directs 192.168.4.0/24 to the OpenVPN server box (you won't need this if the OpenVPN server box*is* the gateway for the server LAN). Suppose you were missing this step and you tried to ping a machine (not the OpenVPN server itself) on the server LAN from 192.168.4.8? The outgoing ping would probably reach the machine, but then it wouldn't know how to route the ping reply, because it would have no idea how to reach 192.168.4.0/24. The rule of thumb to use is that when routing entire LANs through the VPN (when the VPN server is not the same machine as the LAN gateway), make sure that the gateway for the LAN routes all VPN subnets to the VPN server machine.Similarly, if the client machine running OpenVPN is not also the gateway for the client LAN, then the gateway for the client LAN must have a route which directs all subnets which should be reachable through the VPN to the OpenVPN client machine.### Including multiple machines on the client side when using a bridged VPN (dev tap)This requires a more complex setup (maybe not more complex in practice, but more complicated to explain in detail):- You must bridge the client TAP interface with the LAN-connected NIC on the client.- You must manually set the IP/netmask of the TAP interface on the client.- You must configure client-side machines to use an IP/netmask that is inside of the bridged subnet, possibly by[querying a DHCP server on the OpenVPN server side of the VPN](http://openvpn.net/INSTALL-win32.html#dhcp).## Pushing DHCP options to clientsThe OpenVPN server can push DHCP options such as DNS and WINS server addresses to clients (some[caveats](http://openvpn.net/faq.html#dhcpcaveats) to be aware of). Windows clients can accept pushed DHCP options natively, while non-Windows clients can accept them by using a client-side**up** script which parses the**foreign_option_*n*** environmental variable list. See the[man page](http://openvpn.net/man.html) or[openvpn-users mailing list archive](http://openvpn.net/mail.html) for non-Windows**foreign_option_*n*** documentation and script examples.For example, suppose you would like connecting clients to use an internal DNS server at 10.66.0.4 or 10.66.0.5 and a WINS server at 10.66.0.8. Add this to the OpenVPN server configuration:> **push "dhcp-option DNS 10.66.0.4"push "dhcp-option DNS 10.66.0.5"push "dhcp-option WINS 10.66.0.8"**To test this feature on Windows, run the following from a command prompt window after the machine has connected to an OpenVPN server:> **ipconfig /all**The entry for the TAP-Windows adapter should show the DHCP options which were pushed by the server.## Configuring client-specific rules and access policiesSuppose we are setting up a company VPN, and we would like to establish separate access policies for 3 different classes of users:- **System administrators** -- full access to all machines on the network- **Employees** -- access only to Samba/email server- **Contractors** -- access to a special server onlyThe basic approach we will take is (a) segregate each user class into its own virtual IP address range, and (b) control access to machines by setting up firewall rules which key off the client's virtual IP address.In our example, suppose that we have a variable number of employees, but only one system administrator, and two contractors. Our IP allocation approach will be to put all employees into an IP address pool, and then allocate fixed IP addresses for the system administrator and contractors.Note that one of the prerequisites of this example is that you have a software firewall running on the OpenVPN server machine which gives you the ability to define specific firewall rules. For our example, we will assume the firewall is Linux**iptables**.First, let's create a virtual IP address map according to user class:|**Class**|**Virtual IP Range**|**Allowed LAN Access**|**Common Names**||----|----|----|----||Employees|10.8.0.0/24|Samba/email server at 10.66.4.4|[variable]||System Administrators|10.8.1.0/24|Entire 10.66.4.0/24 subnet|sysadmin1||Contractors|10.8.2.0/24|Contractor server at 10.66.4.12|contractor1, contracter2|Next, let's translate this map into an OpenVPN server configuration. First of all, make sure you've followed the steps[above](http://openvpn.net/howto.html#scope) for making the 10.66.4.0/24 subnet available to all clients (while we will configure routing to allow client access to the entire 10.66.4.0/24 subnet, we will then impose access restrictions using firewall rules to implement the above policy table).First, define a static unit number for our **tun** interface, so that we will be able to refer to it later in our firewall rules:> **dev tun0**In the server configuration file, define the Employee IP address pool:> **server 10.8.0.0 255.255.255.0**Add routes for the System Administrator and Contractor IP ranges:> **route 10.8.1.0 255.255.255.0route 10.8.2.0 255.255.255.0**Because we will be assigning fixed IP addresses for specific System Administrators and Contractors, we will use a client configuration directory:> **client-config-dir ccd**Now place special configuration files in the **ccd** subdirectory to define the fixed IP address for each non-Employee VPN client.### ccd/sysadmin1> **ifconfig-push 10.8.1.1 10.8.1.2**### ccd/contractor1> **ifconfig-push 10.8.2.1 10.8.2.2**### ccd/contractor2> **ifconfig-push 10.8.2.5 10.8.2.6**Each pair of **ifconfig-push** addresses represent the virtual client and server IP endpoints. They must be taken from successive /30 subnets in order to be compatible with Windows clients and the TAP-Windows driver. Specifically, the last octet in the IP address of each endpoint pair must be taken from this set:|**Class**|**Virtual IP Range**|**Allowed LAN Access**|**Common Names**|Employees|10.8.0.0/24|Samba/email server at 10.66.4.4|[variable]|System Administrators|10.8.1.0/24|Entire 10.66.4.0/24 subnet|sysadmin1|Contractors|10.8.2.0/24|Contractor server at 10.66.4.12|contractor1, contracter2|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|**Class**|**Virtual IP Range**|**Allowed LAN Access**|**Common Names**| | | | | | | | | | | | | |
|Employees|10.8.0.0/24|Samba/email server at 10.66.4.4|[variable]| | | | | | | | | | | | | |
|System Administrators|10.8.1.0/24|Entire 10.66.4.0/24 subnet|sysadmin1| | | | | | | | | | | | | |
|Contractors|10.8.2.0/24|Contractor server at 10.66.4.12|contractor1, contracter2| | | | | | | | | | | | | |

Next, let's translate this map into an OpenVPN server configuration. First of all, make sure you've followed the steps[above](http://openvpn.net/howto.html#scope) for making the 10.66.4.0/24 subnet available to all clients (while we will configure routing to allow client access to the entire 10.66.4.0/24 subnet, we will then impose access restrictions using firewall rules to implement the above policy table).

First, define a static unit number for our **tun** interface, so that we will be able to refer to it later in our firewall rules:

> **dev tun0**

In the server configuration file, define the Employee IP address pool:

> **server 10.8.0.0 255.255.255.0**

Add routes for the System Administrator and Contractor IP ranges:

> **route 10.8.1.0 255.255.255.0route 10.8.2.0 255.255.255.0**

Because we will be assigning fixed IP addresses for specific System Administrators and Contractors, we will use a client configuration directory:

> **client-config-dir ccd**

Now place special configuration files in the **ccd** subdirectory to define the fixed IP address for each non-Employee VPN client.

### ccd/sysadmin1

> **ifconfig-push 10.8.1.1 10.8.1.2**

### ccd/contractor1

> **ifconfig-push 10.8.2.1 10.8.2.2**

### ccd/contractor2

> **ifconfig-push 10.8.2.5 10.8.2.6**

Each pair of **ifconfig-push** addresses represent the virtual client and server IP endpoints. They must be taken from successive /30 subnets in order to be compatible with Windows clients and the TAP-Windows driver. Specifically, the last octet in the IP address of each endpoint pair must be taken from this set:




> [  1,  2] [  5,  6] [  9, 10] [ 13, 14] [ 17, 18]
[ 21, 22] [ 25, 26] [ 29, 30] [ 33, 34] [ 37, 38]
[ 41, 42] [ 45, 46] [ 49, 50] [ 53, 54] [ 57, 58]
[ 61, 62] [ 65, 66] [ 69, 70] [ 73, 74] [ 77, 78]
[ 81, 82] [ 85, 86] [ 89, 90] [ 93, 94] [ 97, 98]
[101,102] [105,106] [109,110] [113,114] [117,118]
[121,122] [125,126] [129,130] [133,134] [137,138]
[141,142] [145,146] [149,150] [153,154] [157,158]
[161,162] [165,166] [169,170] [173,174] [177,178]
[181,182] [185,186] [189,190] [193,194] [197,198]
[201,202] [205,206] [209,210] [213,214] [217,218]
[221,222] [225,226] [229,230] [233,234] [237,238]
[241,242] [245,246] [249,250] [253,254]



|This completes the OpenVPN configuration. The final step is to add firewall rules to finalize the access policy. For this example, we will use firewall rules in the Linux**iptables** syntax:|
|----|




> **# Employee ruleiptables -A FORWARD -i tun0 -s 10.8.0.0/24 -d 10.66.4.4 -j ACCEPT# Sysadmin ruleiptables -A FORWARD -i tun0 -s 10.8.1.0/24 -d 10.66.4.0/24 -j ACCEPT# Contractor ruleiptables -A FORWARD -i tun0 -s 10.8.2.0/24 -d 10.66.4.12 -j ACCEPT**



|## Using alternative authentication methodsOpenVPN 2.0 and later include a feature that allows the OpenVPN server to securely obtain a username and password from a connecting client, and to use that information as a basis for authenticating the client.To use this authentication method, first add the **auth-user-pass** directive to the client configuration. It will direct the OpenVPN client to query the user for a username/password, passing it on to the server over the secure TLS channel.Next, configure the server to use an authentication plugin, which may be a script, shared object, or DLL. The OpenVPN server will call the plugin every time a VPN client tries to connect, passing it the username/password entered on the client. The authentication plugin can control whether or not the OpenVPN server allows the client to connect by returning a failure (1) or success (0) value.### Using Script PluginsScript plugins can be used by adding the **auth-user-pass-verify** directive to the server-side configuration file. For example:> **auth-user-pass-verify auth-pam.pl via-file**will use the **auth-pam.pl** perl script to authenticate the username/password of connecting clients. See the description of**auth-user-pass-verify** in the[manual page](http://openvpn.net/man.html) for more information.The **auth-pam.pl** script is included in the OpenVPN source file distribution in the**sample-scripts** subdirectory. It will authenticate users on a Linux server using a PAM authentication module, which could in turn implement shadow password, RADIUS, or LDAP authentication.**auth-pam.pl** is primarily intended for demonstration purposes. For real-world PAM authentication, use the**openvpn-auth-pam** shared object plugin described below.### Using Shared Object or DLL PluginsShared object or DLL plugins are usually compiled C modules which are loaded by the OpenVPN server at run time. For example if you are using an RPM-based OpenVPN package on Linux, the**openvpn-auth-pam** plugin should be already built. To use it, add this to the server-side config file:|
|----|




> **plugin /usr/share/openvpn/plugin/lib/openvpn-auth-pam.so login**



|This will tell the OpenVPN server to validate the username/password entered by clients using the**login** PAM module.For real-world production use, it's better to use the **openvpn-auth-pam** plugin, because it has several advantages over the**auth-pam.pl** script:- The shared object **openvpn-auth-pam** plugin uses a split-privilege execution model for better security. This means that the OpenVPN server can run with reduced privileges by using the directives**user nobody**,**group nobody**, and **chroot**, and will still be able to authenticate against the root-readable-only shadow password file.- OpenVPN can pass the username/password to a plugin via virtual memory, rather than via a file or the environment, which is better for local security on the server machine.- C-compiled plugin modules generally run faster than scripts.If you would like more information on developing your own plugins for use with OpenVPN, see the**README** files in the**plugin** subdirectory of the OpenVPN source distribution.To build the **openvpn-auth-pam** plugin on Linux, cd to the **plugin/auth-pam**directory in the OpenVPN source distribution and run **make**.### Using username/password authentication as the only form of client authenticationBy default, using **auth-user-pass-verify** or a username/password-checking**plugin** on the server will enable dual authentication, requiring that both client-certificate and username/password authentication succeed in order for the client to be authenticated.While it is discouraged from a security perspective, it is also possible to disable the use of client certificates, and force username/password authentication only. On the server:> **client-cert-not-required**Such configurations should usually also set:> **username-as-common-name**which will tell the server to use the username for indexing purposes as it would use the Common Name of a client which was authenticating via a client certificate.Note that **client-cert-not-required** will not obviate the need for a server certificate, so a client connecting to a server which uses**client-cert-not-required**may remove the**cert** and**key** directives from the client configuration file, but not the**ca** directive, because it is necessary for the client to verify the server certificate.## How to add dual-factor authentication to an OpenVPN configuration using client-side smart cardsAlso see Article: [The OpenVPN Smartcard HOWTO](http://michele.pupazzo.org/docs/smart-cards-openvpn.html)- [About dual-factor authentication](http://openvpn.net/howto.html?forward=no#pkcs11_about_dual_factor)- [What is PKCS#11?](http://openvpn.net/howto.html?forward=no#pkcs11_about)- [Finding PKCS#11 provider library](http://openvpn.net/howto.html?forward=no#pkcs11_determine_provider).- [How to configure a cryptographic token](http://openvpn.net/howto.html?forward=no#pkcs11_pkcs11_token_config)- [How to modify an OpenVPN configuration to make use of cryptographic tokens](http://openvpn.net/howto.html?forward=no#pkcs11_openvpn_config)- [Determine the correct object](http://openvpn.net/howto.html?forward=no#pkcs11_determine_object).- [Using OpenVPN with PKCS#11](http://openvpn.net/howto.html?forward=no#pkcs11_using_openvpn).- [PKCS#11 implementation considerations](http://openvpn.net/howto.html?forward=no#pkcs11_considerations).- [OpenSC PKCS#11 provider](http://openvpn.net/howto.html?forward=no#pkcs11_opensc).- [Difference between PKCS#11 and Microsoft Cryptographic API (CryptoAPI)](http://openvpn.net/howto.html?forward=no#pkcs11_vs_cryptoapi).### About dual-factor authenticationDual-factor authentication is a method of authentication that combines two elements: something you have and something you know.Something you have should be a device that cannot be duplicated; such a device can be a cryptographic token that contains a private secret key. This private key is generated inside the device and never leaves it. If a user possessing this token attempts to access protected services on a remote network, the authorization process which grants or denies network access can establish, with a high degree of certainty, that the user seeking access is in physical possession of a known, certified token.Something you know can be a password presented to the cryptographic device. Without presenting the proper password you cannot access the private secret key. Another feature of cryptographic devices is to prohibit the use of the private secret key if the wrong password had been presented more than an allowed number of times. This behavior ensures that if a user lost his device, it would be infeasible for another person to use it.Cryptographic devices are commonly called "smart cards" or "tokens", and are used in conjunction with a PKI (Public Key Infrastructure). The VPN server can examine a X.509 certificate and verify that the user holds the corresponding private secret key. Since the device cannot be duplicated and requires a valid password, the server is able to authenticate the user with a high degree of confidence.Dual-factor authentication is much stronger than password-based authentication, because in the worst-case scenario, only one person at a time can use the cryptographic token. Passwords can be guessed and can be exposed to other users, so in the worst-case scenario an infinite number of people could attempt to gain unauthorized access when resources are protected using password-only authentication.If you store the secret private key in a file, the key is usually encrypted by a password. The problem with this approach is that the encrypted key is exposed to decryption attacks or spyware/malware running on the client machine. Unlike when using a cryptographic device, the file cannot erase itself automatically after several failed decryption attempts.### What is PKCS#11?*This standard specifies an API, called Cryptoki, to devices which hold cryptographic information and perform cryptographic functions. Cryptoki, pronounced "crypto-key" and short for cryptographic token interface, follows a simple object-based approach, addressing the goals of technology independence (any kind of device) and resource sharing (multiple applications accessing multiple devices), presenting to applications a common, logical view of the device called a cryptographic token.*Source: RSA Security Inc. [http://www.rsasecurity.com/rsalabs/pkcs/pkcs-11](http://www.rsasecurity.com/rsalabs/pkcs/pkcs-11).To summarize, PKCS#11 is a standard that can be used by application software to access cryptographic tokens such as smart cards and other devices. Most device vendors provide a library that implements the PKCS#11 provider interface -- this library can be used by applications in order to access these devices. PKCS#11 is a cross-platform, vendor-independent free standard.### Finding PKCS#11 provider libraryThe first thing you need to do is to find the provider library, it should be installed with the device drivers. Each vendor has its own library. For example, the OpenSC PKCS#11 provider is located at /usr/lib/pkcs11/opensc-pkcs11.so on Unix or at opensc-pkcs11.dll on Windows.### How to configure cryptographic tokenYou should follow an enrollment procedure:- Initialize the PKCS#11 token.- Generate RSA key pair on the PKCS#11 token.- Create a certificate request based on the key pair, you can use [OpenSC and OpenSSL](http://www.opensc.org/files/doc/opensc.html#opensc.using.openssl) in order to do that.- Submit the certificate request to a certificate authority, and receive a certificate.- Load the certificate onto the token, while noting that the id and label attributes of the certificate must match those of the private key.A configured token is a token that has a private key object and a certificate object, where both share the same id and label attributes.A simple enrollment utility is Easy-RSA 2.0 which is part of OpenVPN 2.1 series. Follow the instructions specified in the README file, and then use the pkitool in order to enroll.Initialize a token using the following command:|
|----|




> $ ./pkitool --pkcs11-slots /usr/lib/pkcs11/
$ ./pkitool --pkcs11-init /usr/lib/pkcs11/  



|Enroll a certificate using the following command:|
|----|




> $ ./pkitool --pkcs11 /usr/lib/pkcs11/   client1




|### How to modify an OpenVPN configuration to make use of cryptographic tokensYou should have OpenVPN 2.1 or above in order to use the PKCS#11 features.#### Determine the correct objectEach PKCS#11 provider can support multiple devices. In order to view the available object list you can use the following command:|
|----|




> $ openvpn --show-pkcs11-ids /usr/lib/pkcs11/

The following objects are available for use.
Each object shown below may be used as parameter to
--pkcs11-id option please remember to use single quote mark.

Certificate
       DN:             /CN=User1
       Serial:         490B82C4000000000075
       Serialized id:  aaaa/bbb/41545F5349474E415455524581D2A1A1B23C4AA4CB17FAF7A4600



|Each certificate/private key pair have unique "Serialized id" string. The serialized id string of the requested certificate should be specified to the**pkcs11-id** option using single quote marks.> **pkcs11-id 'aaaa/bbb/41545F5349474E415455524581D2A1A1B23C4AA4CB17FAF7A4600'**#### Using OpenVPN with PKCS#11##### A typical set of OpenVPN options for PKCS#11> **pkcs11-providers /usr/lib/pkcs11/pkcs11-id 'aaaa/bbb/41545F5349474E415455524581D2A1A1B23C4AA4CB17FAF7A4600'**This will select the object which matches the pkcs11-id string.##### Advanced OpenVPN options for PKCS#11> **pkcs11-providers /usr/lib/pkcs11/provider1.so /usr/lib/pkcs11/provider2.sopkcs11-id 'aaaa/bbb/41545F5349474E415455524581D2A1A1B23C4AA4CB17FAF7A4600'pkcs11-pin-cache 300daemonauth-retry nointeractmanagement-holdmanagement-signalmanagement 127.0.0.1 8888management-query-passwords**This will load two providers into OpenVPN, use the certificate specified on **pkcs11-id**option, and use the management interface in order to query passwords. The daemon will resume into hold state on the event when token cannot be accessed. The token will be used for 300 seconds after which the password will be re-queried, session will disconnect if management session disconnects.#### PKCS#11 implementation considerationsMany PKCS#11 providers make use of threads, in order to avoid problems caused by implementation of LinuxThreads (setuid, chroot), it is highly recommend to upgrade to Native POSIX Thread Library (NPTL) enabled glibc if you intend to use PKCS#11.#### OpenSC PKCS#11 providerOpenSC PKCS#11 provider is located at /usr/lib/pkcs11/opensc-pkcs11.so on Unix or at opensc-pkcs11.dll on Windows.### Difference between PKCS#11 and Microsoft Cryptographic API (CryptoAPI)PKCS#11 is a free, cross-platform vendor independent standard. CryptoAPI is a Microsoft specific API. Most smart card vendors provide support for both interfaces. In the Windows environment, the user should select which interface to use.The current implementation of OpenVPN that uses the MS CryptoAPI (**cryptoapicert**option) works well as long as you don't run OpenVPN as a service. If you wish to run OpenVPN in an administrative environment using a service, the implementation will not work with most smart cards because of the following reasons:- Most smart card providers do not load certificates into the local machine store, so the implementation will be unable to access the user certificate.- If the OpenVPN client is running as a service without direct interaction with the end-user, the service cannot query the user to provide a password for the smart card, causing the password-verification process on the smart card to fail.Using the PKCS#11 interface, you can use smart cards with OpenVPN in any implementation, since PKCS#11 does not access Microsoft stores and does not necessarily require direct interaction with the end-user.## Routing all client traffic (including web-traffic) through the VPN### OverviewBy default, when an OpenVPN client is active, only network traffic to and from the OpenVPN server site will pass over the VPN. General web browsing, for example, will be accomplished with direct connections that bypass the VPN.In certain cases this behavior might not be desirable -- you might want a VPN client to tunnel all network traffic through the VPN, including general internet web browsing. While this type of VPN configuration will exact a performance penalty on the client, it gives the VPN administrator more control over security policies when a client is simultaneously connected to both the public internet and the VPN at the same time.### ImplementationAdd the following directive to the server configuration file:> **push "redirect-gateway def1"**If your VPN setup is over a wireless network, where all clients and the server are on the same wireless subnet, add the**local** flag:> **push "redirect-gateway local def1"**Pushing the **redirect-gateway** option to clients will cause all IP network traffic originating on client machines to pass through the OpenVPN server. The server will need to be configured to deal with this traffic somehow, such as by NATing it to the internet, or routing it through the server site's HTTP proxy.On Linux, you could use a command such as this to NAT the VPN client traffic to the internet:|
|----|




> **iptables -t nat -A POSTROUTING -s 10.8.0.0/24 -o eth0 -j MASQUERADE**



|This command assumes that the VPN subnet is **10.8.0.0/24** (taken from the**server** directive in the OpenVPN server configuration) and that the local ethernet interface is**eth0**.When **redirect-gateway** is used, OpenVPN clients will route DNS queries through the VPN, and the VPN server will need handle them. This can be accomplished by pushing a DNS server address to connecting clients which will replace their normal DNS server settings during the time that the VPN is active. For example:> **push "dhcp-option DNS 10.8.0.1"**will configure Windows clients (or non-Windows clients with some extra server-side scripting) to use 10.8.0.1 as their DNS server. Any address which is reachable from clients may be used as the DNS server address.### CaveatsRedirecting all network traffic through the VPN is not entirely a problem-free proposition. Here are some typical gotchas to be aware of:- Many OpenVPN client machines connecting to the internet will periodically interact with a DHCP server to renew their IP address leases. The**redirect-gateway** option might prevent the client from reaching the local DHCP server (because DHCP messages would be routed over the VPN), causing it to lose its IP address lease.- [Issues exist](http://openvpn.net/faq.html#dhcpcaveats) with respect to pushing DNS addresses to Windows clients.- Web browsing performance on the client will be noticably slower.For more information on the mechanics of the **redirect-gateway** directive, see the[manual page](http://openvpn.net/man.html).## Running an OpenVPN server on a dynamic IP addressWhile OpenVPN clients can easily access the server via a dynamic IP address without any special configuration, things get more interesting when the server itself is on a dynamic address. While OpenVPN has no trouble handling the situation of a dynamic server, some extra configuration is required.The first step is to get a dynamic DNS address which can be configured to "follow" the server every time the server's IP address changes. There are several dynamic DNS service providers available, such as[dyndns.org](http://www.dyndns.org/).The next step is to set up a mechanism so that every time the server's IP address changes, the dynamic DNS name will be quickly updated with the new IP address, allowing clients to find the server at its new IP address. There are two basic ways to accomplish this:- Use a NAT router appliance with dynamic DNS support (such as the**Linksys BEFSR41**). Most of the inexpensive NAT router appliances that are widely available have the capability to update a dynamic DNS name every time a new DHCP lease is obtained from the ISP. This setup is ideal when the OpenVPN server box is a single-NIC machine inside the firewall.- Use a dynamic DNS client application such as [ddclient](http://sourceforge.net/apps/trac/ddclient) to update the dynamic DNS address whenever the server IP address changes. This setup is ideal when the machine running OpenVPN has multiple NICs and is acting as a site-wide firewall/gateway. To implement this setup, you need to set up a script to be run by your DHCP client software every time an IP address change occurs. This script should (a) run**ddclient**to notify your dynamic DNS provider of your new IP address and (b) restart the OpenVPN server daemon.The OpenVPN client by default will sense when the server's IP address has changed, if the client configuration is using a**remote** directive which references a dynamic DNS name. The usual chain of events is that (a) the OpenVPN client fails to receive timely keepalive messages from the server's old IP address, triggering a restart, and (b) the restart causes the DNS name in the**remote**directive to be re-resolved, allowing the client to reconnect to the server at its new IP address.More information can be found in the [FAQ](http://openvpn.net/faq.html#dynamic-address).## Connecting to an OpenVPN server via an HTTP proxy.OpenVPN supports connections through an HTTP proxy, with the following authentication modes:- No proxy authentication- Basic proxy authentication- NTLM proxy authenticationFirst of all, HTTP proxy usage requires that you use TCP as the tunnel carrier protocol. So add the following to both client and server configurations:> **proto tcp**Make sure that any **proto udp** lines in the config files are deleted.Next, add the **http-proxy** directive to the client configuration file (see the[manual page](http://openvpn.net/man.html) for a full description of this directive).For example, suppose you have an HTTP proxy server on the client LAN at**192.168.4.1**, which is listening for connections on port **1080**. Add this to the client config:> **http-proxy 192.168.4.1 1080**Suppose the HTTP proxy requires Basic authentication:> **http-proxy 192.168.4.1 1080 stdin basic**Suppose the HTTP proxy requires NTLM authentication:> **http-proxy 192.168.4.1 1080 stdin ntlm**The two authentication examples above will cause OpenVPN to prompt for a username/password from standard input. If you would instead like to place these credentials in a file, replace**stdin** with a filename, and place the username on line 1 of this file and the password on line 2.## Connecting to a Samba share over OpenVPNThis example is intended show how OpenVPN clients can connect to a Samba share over a routed**dev tun** tunnel. If you are ethernet bridging (**dev tap**), you probably don't need to follow these instructions, as OpenVPN clients should see server-side machines in their network neighborhood.For this example, we will assume that:- the server-side LAN uses a subnet of **10.66.0.0/24**,- the VPN IP address pool uses **10.8.0.0/24** (as cited in the **server** directive in the OpenVPN server configuration file),- the Samba server has an IP address of **10.66.0.4**, and- the Samba server has already been configured and is reachable from the local LAN.If the Samba and OpenVPN servers are running on different machines, make sure you've followed the section on[expanding the scope of the VPN to include additional machines](http://openvpn.net/howto.html#scope).Next, edit your Samba configuration file (**smb.conf**). Make sure the**hosts allow**directive will permit OpenVPN clients coming from the**10.8.0.0/24** subnet to connect. For example:> **hosts allow = 10.66.0.0/24 10.8.0.0/24 127.0.0.1**If you are running the Samba and OpenVPN servers on the same machine, you may want to edit the**interfaces** directive in the**smb.conf** file to also listen on the TUN interface subnet of**10.8.0.0/24**:> **interfaces  = 10.66.0.0/24 10.8.0.0/24**If you are running the Samba and OpenVPN servers on the same machine, connect from an OpenVPN client to a Samba share using the folder name:> **\\10.8.0.1\\sharename**If the Samba and OpenVPN servers are on different machines, use folder name:> **\\10.66.0.4\sharename**For example, from a command prompt window:> **net use z: \\10.66.0.4\sharename /USER:myusername**## Implementing a load-balancing/failover configuration### ClientThe OpenVPN client configuration can refer to multiple servers for load balancing and failover. For example:> **remote server1.mydomainremote server2.mydomainremote server3.mydomain**will direct the OpenVPN client to attempt a connection with server1, server2, and server3 in that order. If an existing connection is broken, the OpenVPN client will retry the most recently connected server, and if that fails, will move on to the next server in the list. You can also direct the OpenVPN client to randomize its server list on startup, so that the client load will be probabilistically spread across the server pool.> **remote-random**If you would also like DNS resolution failures to cause the OpenVPN client to move to the next server in the list, add the following:> **resolv-retry 60**The **60** parameter tells the OpenVPN client to try resolving each**remote** DNS name for 60 seconds before moving on to the next server in the list.The server list can also refer to multiple OpenVPN server daemons running on the same machine, each listening for connections on a different port, for example:> **remote smp-server1.mydomain 8000remote smp-server1.mydomain 8001remote smp-server2.mydomain 8000remote smp-server2.mydomain 8001**If your servers are multi-processor machines, running multiple OpenVPN daemons on each server can be advantageous from a performance standpoint.OpenVPN also supports the **remote** directive referring to a DNS name which has multiple**A** records in the zone configuration for the domain. In this case, the OpenVPN client will randomly choose one of the**A** records every time the domain is resolved.### ServerThe simplest approach to a load-balanced/failover configuration on the server is to use equivalent configuration files on each server in the cluster, except use a different virtual IP address pool for each server. For example:**server1**> **server 10.8.0.0 255.255.255.0****server2**> **server 10.8.1.0 255.255.255.0****server3**> **server 10.8.2.0 255.255.255.0**## Hardening OpenVPN SecurityOne of the often-repeated maxims of network security is that one should never place so much trust in a single security component that its failure causes a catastrophic security breach. OpenVPN provides several mechanisms to add additional security layers to hedge against such an outcome.### tls-authThe **tls-auth** directive adds an additional HMAC signature to all SSL/TLS handshake packets for integrity verification. Any UDP packet not bearing the correct HMAC signature can be dropped without further processing. The**tls-auth**HMAC signature provides an additional level of security above and beyond that provided by SSL/TLS. It can protect against:- DoS attacks or port flooding on the OpenVPN UDP port.- Port scanning to determine which server UDP ports are in a listening state.- Buffer overflow vulnerabilities in the SSL/TLS implementation.- SSL/TLS handshake initiations from unauthorized machines (while such handshakes would ultimately fail to authenticate,**tls-auth** can cut them off at a much earlier point).Using **tls-auth** requires that you generate a shared-secret key that is used in addition to the standard RSA certificate/key:> **openvpn --genkey --secret ta.key**This command will generate an OpenVPN static key and write it to the file**ta.key**. This key should be copied over a pre-existing secure channel to the server and all client machines. It can be placed in the same directory as the RSA**.key** and**.crt** files.In the server configuration, add:> **tls-auth ta.key 0**In the client configuration, add:> **tls-auth ta.key 1**### proto udpWhile OpenVPN allows either the TCP or UDP protocol to be used as the VPN carrier connection, the UDP protocol will provide better protection against DoS attacks and port scanning than TCP:> **proto udp**### user/group (non-Windows only)OpenVPN has been very carefully designed to allow root privileges to be dropped after initialization, and this feature should always be used on Linux/BSD/Solaris. Without root privileges, a running OpenVPN server daemon provides a far less enticing target to an attacker.> **user nobodygroup nobody**### Unprivileged mode (Linux only)On Linux OpenVPN can be run completely unprivileged. This configuration is a little more complex, but provides best security.In order to work with this configuration, OpenVPN must be configured to use iproute interface, this is done by specifying --enable-iproute2 to configure script. sudo package should also be available on your system.This configuration uses the Linux ability to change the permission of a tun device, so that unprivileged user may access it. It also uses sudo in order to execute iproute so that interface properties and routing table may be modified.OpenVPN configuration:- Write the following script and place it at: /usr/local/sbin/unpriv-ip:> **#!/bin/shsudo /sbin/ip $***- Execute visudo, and add the followings to allow user 'user1' to execute /sbin/ip:> **user1 ALL=(ALL)  NOPASSWD: /sbin/ip**You can also enable a group of users with the following command:> **%users ALL=(ALL)  NOPASSWD: /sbin/ip**- Add the following to your OpenVPN configuration:> **dev tunX/tapXiproute /usr/local/sbin/unpriv-ip**Please note that you must select constant X and specify tun or tap not both.- As root add persistant interface, and permit user and/or group to manage it, the following create tunX (replace with your own) and allow user1 and group users to access it.> **openvpn --mktun --dev tunX --type tun --user user1 --group users**- Run OpenVPN in the context of the unprivileged user.Further security constraints may be added by examining the parameters at the /usr/local/sbin/unpriv-ip script.### chroot (non-Windows only)The **chroot** directive allows you to lock the OpenVPN daemon into a so-called*chroot jail*, where the daemon would not be able to access any part of the host system's filesystem except for the specific directory given as a parameter to the directive. For example,> **chroot jail**would cause the OpenVPN daemon to cd into the **jail** subdirectory on initialization, and would then reorient its root filesystem to this directory so that it would be impossible thereafter for the daemon to access any files outside of**jail** and its subdirectory tree. This is important from a security perspective, because even if an attacker were able to compromise the server with a code insertion exploit, the exploit would be locked out of most of the server's filesystem.Caveats: because **chroot** reorients the filesystem (from the perspective of the daemon only), it is necessary to place any files which OpenVPN might need after initialization in the**jail** directory, such as:- the **crl-verify** file, or- the **client-config-dir** directory.### Larger RSA keysThe RSA key size is controlled by the **KEY_SIZE** variable in the**easy-rsa/vars** file, which must be set before any keys are generated. Currently set to 1024 by default, this value can reasonably be increased to 2048 with no negative impact on VPN tunnel performance, except for a slightly slower SSL/TLS renegotiation handshake which occurs once per client per hour, and a much slower one-time Diffie Hellman parameters generation process using the**easy-rsa/build-dh**script.### Larger symmetric keysBy default OpenVPN uses **Blowfish**, a 128 bit symmetrical cipher.OpenVPN automatically supports any cipher which is supported by the OpenSSL library, and as such can support ciphers which use large key sizes. For example, the 256-bit version of AES (Advanced Encryption Standard) can be used by adding the following to both server and client configuration files:> **cipher AES-256-CBC**### Keep the root key (**ca.key**) on a standalone machine without a network connectionOne of the security benefits of using an X509 PKI (as OpenVPN does) is that the root CA key (**ca.key**) need not be present on the OpenVPN server machine. In a high security environment, you might want to specially designate a machine for key signing purposes, keep the machine well-protected physically, and disconnect it from all networks. Floppy disks can be used to move key files back and forth, as necessary. Such measures make it extremely difficult for an attacker to steal the root key, short of physical theft of the key signing machine.## Revoking Certificates*Revoking a certificate* means to invalidate a previously signed certificate so that it can no longer be used for authentication purposes.Typical reasons for wanting to revoke a certificate include:- The private key associated with the certificate is compromised or stolen.- The user of an encrypted private key forgets the password on the key.- You want to terminate a VPN user's access.### ExampleAs an example, we will revoke the **client2** certificate, which we generated above in the "key generation" section of the HOWTO.First open up a shell or command prompt window and cd to the **easy-rsa**directory as you did in the "key generation" section above. On Linux/BSD/Unix:> **. ./vars./revoke-full client2**On Windows:> **varsrevoke-full client2**You should see output similar to this:|
|----|




> Using configuration from /root/openvpn/20/openvpn/tmp/easy-rsa/openssl.cnf
DEBUG[load_index]: unique_subject = "yes"
Revoking Certificate 04.
Data Base Updated
Using configuration from /root/openvpn/20/openvpn/tmp/easy-rsa/openssl.cnf
DEBUG[load_index]: unique_subject = "yes"
client2.crt: /C=KG/ST=NA/O=OpenVPN-TEST/CN=client2/emailAddress=me@myhost.mydomain
error 23 at 0 depth lookup:certificate revoked



|Mode|Key usage|Extended key usage|
|----|----|----|
| | | |
|Client|digitalSignature|TLS Web Client Authentication|
|keyAgreement| | |
|digitalSignature, keyAgreement| | |
|Server|digitalSignature, keyEncipherment|TLS Web Server Authentication|
|digitalSignature, keyAgreement| | |

You can build your server certificates with the **build-key-server** script (see the[easy-rsa](http://openvpn.net/easyrsa.html) documentation for more info). This will designate the certificate as a server-only certificate by setting the right attributes. Now add the following line to your client configuration:

> **remote-cert-tls server**
- *[OpenVPN 2.0 and below]* Build your server certificates with the **build-key-server** script (see the [easy-rsa](http://openvpn.net/easyrsa.html) documentation for more info). This will designate the certificate as a server-only certificate by setting**nsCertType**=server. Now add the following line to your client configuration:



> **ns-cert-type server**

This will block clients from connecting to any server which lacks the**nsCertType**=server designation in its certificate, even if the certificate has been signed by the**ca** file in the OpenVPN configuration file.

- Use the **tls-remote**directive on the client to accept/reject the server connection based on the common name of the server certificate.



- Use a **tls-verify**script or plugin to accept/reject the server connection based on a custom test of the server certificate's embedded X509 subject details.



- Sign server certificates with one CA and client certificates with a different CA. The client configuration**ca** directive should reference the server-signing CA file, while the server configuration**ca**directive should reference the client-signing CA file.











## Sample OpenVPN 2.0 configuration files









|### sample-config-files/server.conf|
|----|




> **################################################## Sample OpenVPN 2.0 config file for            ## multi-client server.                          ##                                               ## This file is for the server side              ## of a many-clients  one-server                 ## OpenVPN configuration.                        ##                                               ## OpenVPN also supports                         ## single-machine  single-machine                ## configurations (See the Examples page         ## on the web site for more info).               ##                                               ## This config should work on Windows            ## or Linux/BSD systems.  Remember on            ## Windows to quote pathnames and use            ## double backslashes, e.g.:                     ## "C:\\Program Files\\OpenVPN\\config\\foo.key" ##                                               ## Comments are preceded with '#' or ';'         ################################################### Which local IP address should OpenVPN# listen on? (optional);local a.b.c.d# Which TCP/UDP port should OpenVPN listen on?# If you want to run multiple OpenVPN instances# on the same machine, use a different port# number for each one.  You will need to# open up this port on your firewall.port 1194# TCP or UDP server?;proto tcpproto udp# "dev tun" will create a routed IP tunnel,# "dev tap" will create an ethernet tunnel.# Use "dev tap0" if you are ethernet bridging# and have precreated a tap0 virtual interface# and bridged it with your ethernet interface.# If you want to control access policies# over the VPN, you must create firewall# rules for the the TUN/TAP interface.# On non-Windows systems, you can give# an explicit unit number, such as tun0.# On Windows, use "dev-node" for this.# On most systems, the VPN will not function# unless you partially or fully disable# the firewall for the TUN/TAP interface.;dev tapdev tun# Windows needs the TAP-Windows adapter name# from the Network Connections panel if you# have more than one.  On XP SP2 or higher,# you may need to selectively disable the# Windows firewall for the TAP adapter.# Non-Windows systems usually don't need this.;dev-node MyTap# SSL/TLS root certificate (ca), certificate# (cert), and private key (key).  Each client# and the server must have their own cert and# key file.  The server and all clients will# use the same ca file.## See the "easy-rsa" directory for a series# of scripts for generating RSA certificates# and private keys.  Remember to use# a unique Common Name for the server# and each of the client certificates.## Any X509 key management system can be used.# OpenVPN can also use a PKCS #12 formatted key file# (see "pkcs12" directive in man page).ca ca.crtcert server.crtkey server.key  # This file should be kept secret# Diffie hellman parameters.# Generate your own with:#   openssl dhparam -out dh1024.pem 1024# Substitute 2048 for 1024 if you are using# 2048 bit keys. dh dh1024.pem# Configure server mode and supply a VPN subnet# for OpenVPN to draw client addresses from.# The server will take 10.8.0.1 for itself,# the rest will be made available to clients.# Each client will be able to reach the server# on 10.8.0.1. Comment this line out if you are# ethernet bridging. See the man page for more info.server 10.8.0.0 255.255.255.0# Maintain a record of client  virtual IP address# associations in this file.  If OpenVPN goes down or# is restarted, reconnecting clients can be assigned# the same virtual IP address from the pool that was# previously assigned.ifconfig-pool-persist ipp.txt# Configure server mode for ethernet bridging.# You must first use your OS's bridging capability# to bridge the TAP interface with the ethernet# NIC interface.  Then you must manually set the# IP/netmask on the bridge interface, here we# assume 10.8.0.4/255.255.255.0.  Finally we# must set aside an IP range in this subnet# (start=10.8.0.50 end=10.8.0.100) to allocate# to connecting clients.  Leave this line commented# out unless you are ethernet bridging.;server-bridge 10.8.0.4 255.255.255.0 10.8.0.50 10.8.0.100# Push routes to the client to allow it# to reach other private subnets behind# the server.  Remember that these# private subnets will also need# to know to route the OpenVPN client# address pool (10.8.0.0/255.255.255.0)# back to the OpenVPN server.;push "route 192.168.10.0 255.255.255.0";push "route 192.168.20.0 255.255.255.0"# To assign specific IP addresses to specific# clients or if a connecting client has a private# subnet behind it that should also have VPN access,# use the subdirectory "ccd" for client-specific# configuration files (see man page for more info).# EXAMPLE: Suppose the client# having the certificate common name "Thelonious"# also has a small subnet behind his connecting# machine, such as 192.168.40.128/255.255.255.248.# First, uncomment out these lines:;client-config-dir ccd;route 192.168.40.128 255.255.255.248# Then create a file ccd/Thelonious with this line:#   iroute 192.168.40.128 255.255.255.248# This will allow Thelonious' private subnet to# access the VPN.  This example will only work# if you are routing, not bridging, i.e. you are# using "dev tun" and "server" directives.# EXAMPLE: Suppose you want to give# Thelonious a fixed VPN IP address of 10.9.0.1.# First uncomment out these lines:;client-config-dir ccd;route 10.9.0.0 255.255.255.252# Then add this line to ccd/Thelonious:#   ifconfig-push 10.9.0.1 10.9.0.2# Suppose that you want to enable different# firewall access policies for different groups# of clients.  There are two methods:# (1) Run multiple OpenVPN daemons, one for each#     group, and firewall the TUN/TAP interface#     for each group/daemon appropriately.# (2) (Advanced) Create a script to dynamically#     modify the firewall in response to access#     from different clients.  See man#     page for more info on learn-address script.;learn-address ./script# If enabled, this directive will configure# all clients to redirect their default# network gateway through the VPN, causing# all IP traffic such as web browsing and# and DNS lookups to go through the VPN# (The OpenVPN server machine may need to NAT# the TUN/TAP interface to the internet in# order for this to work properly).# CAVEAT: May break client's network config if# client's local DHCP server packets get routed# through the tunnel.  Solution: make sure# client's local DHCP server is reachable via# a more specific route than the default route# of 0.0.0.0/0.0.0.0.;push "redirect-gateway"# Certain Windows-specific network settings# can be pushed to clients, such as DNS# or WINS server addresses.  CAVEAT:# [http://openvpn.net/faq.html#dhcpcaveats](http://openvpn.net/faq.html#dhcpcaveats);push "dhcp-option DNS 10.8.0.1";push "dhcp-option WINS 10.8.0.1"# Uncomment this directive to allow different# clients to be able to "see" each other.# By default, clients will only see the server.# To force clients to only see the server, you# will also need to appropriately firewall the# server's TUN/TAP interface.;client-to-client# Uncomment this directive if multiple clients# might connect with the same certificate/key# files or common names.  This is recommended# only for testing purposes.  For production use,# each client should have its own certificate/key# pair.## IF YOU HAVE NOT GENERATED INDIVIDUAL# CERTIFICATE/KEY PAIRS FOR EACH CLIENT,# EACH HAVING ITS OWN UNIQUE "COMMON NAME",# UNCOMMENT THIS LINE OUT.;duplicate-cn# The keepalive directive causes ping-like# messages to be sent back and forth over# the link so that each side knows when# the other side has gone down.# Ping every 10 seconds, assume that remote# peer is down if no ping received during# a 120 second time period.keepalive 10 120# For extra security beyond that provided# by SSL/TLS, create an "HMAC firewall"# to help block DoS attacks and UDP port flooding.## Generate with:#   openvpn --genkey --secret ta.key## The server and each client must have# a copy of this key.# The second parameter should be '0'# on the server and '1' on the clients.;tls-auth ta.key 0 # This file is secret# Select a cryptographic cipher.# This config item must be copied to# the client config file as well.;cipher BF-CBC        # Blowfish (default);cipher AES-128-CBC   # AES;cipher DES-EDE3-CBC  # Triple-DES# Enable compression on the VPN link.# If you enable it here, you must also# enable it in the client config file.comp-lzo# The maximum number of concurrently connected# clients we want to allow.;max-clients 100# It's a good idea to reduce the OpenVPN# daemon's privileges after initialization.## You can uncomment this out on# non-Windows systems.;user nobody;group nobody# The persist options will try to avoid# accessing certain resources on restart# that may no longer be accessible because# of the privilege downgrade.persist-keypersist-tun# Output a short status file showing# current connections, truncated# and rewritten every minute.status openvpn-status.log# By default, log messages will go to the syslog (or# on Windows, if running as a service, they will go to# the "\Program Files\OpenVPN\log" directory).# Use log or log-append to override this default.# "log" will truncate the log file on OpenVPN startup,# while "log-append" will append to it.  Use one# or the other (but not both).;log         openvpn.log;log-append  openvpn.log# Set the appropriate level of log# file verbosity.## 0 is silent, except for fatal errors# 4 is reasonable for general usage# 5 and 6 can help to debug connection problems# 9 is extremely verboseverb 3# Silence repeating messages.  At most 20# sequential messages of the same message# category will be output to the log.;mute 20**









|### sample-config-files/client.conf|
|----|




> **############################################### Sample client-side OpenVPN 2.0 config file ## for connecting to multi-client server.     ##                                            ## This configuration can be used by multiple ## clients, however each client should have   ## its own cert and key files.                ##                                            ## On Windows, you might want to rename this  ## file so it has a .ovpn extension           ################################################ Specify that we are a client and that we# will be pulling certain config file directives# from the server.client# Use the same setting as you are using on# the server.# On most systems, the VPN will not function# unless you partially or fully disable# the firewall for the TUN/TAP interface.;dev tapdev tun# Windows needs the **TAP-Windows** adapter name# from the Network Connections panel# if you have more than one.  On XP SP2,# you may need to disable the firewall# for the TAP adapter.;dev-node MyTap# Are we connecting to a TCP or# UDP server?  Use the same setting as# on the server.;proto tcpproto udp# The hostname/IP and port of the server.# You can have multiple remote entries# to load balance between the servers.remote my-server-1 1194;remote my-server-2 1194# Choose a random host from the remote# list for load-balancing.  Otherwise# try hosts in the order specified.;remote-random# Keep trying indefinitely to resolve the# host name of the OpenVPN server.  Very useful# on machines which are not permanently connected# to the internet such as laptops.resolv-retry infinite# Most clients don't need to bind to# a specific local port number.nobind# Downgrade privileges after initialization (non-Windows only);user nobody;group nobody# Try to preserve some state across restarts.persist-keypersist-tun# If you are connecting through an# HTTP proxy to reach the actual OpenVPN# server, put the proxy server/IP and# port number here.  See the man page# if your proxy server requires# authentication.;http-proxy-retry # retry on connection failures;http-proxy [proxy server] [proxy port #]# Wireless networks often produce a lot# of duplicate packets.  Set this flag# to silence duplicate packet warnings.;mute-replay-warnings# SSL/TLS parms.# See the server config file for more# description.  It's best to use# a separate .crt/.key file pair# for each client.  A single ca# file can be used for all clients.ca ca.crtcert client.crtkey client.key# Verify server certificate by checking# that the certicate has the nsCertType# field set to "server".  This is an# important precaution to protect against# a potential attack discussed here:#  [http://openvpn.net/howto.html#mitm](http://openvpn.net/howto.html#mitm)## To use this feature, you will need to generate# your server certificates with the nsCertType# field set to "server".  The build-key-server# script in the easy-rsa folder will do this.;ns-cert-type server# If a tls-auth key is used on the server# then every client must also have the key.;tls-auth ta.key 1# Select a cryptographic cipher.# If the cipher option is used on the server# then you must also specify it here.;cipher x# Enable compression on the VPN link.# Don't enable this unless it is also# enabled in the server config file.comp-lzo# Set log file verbosity.verb 3# Silence repeating messages;mute 20**





|Source Tarball (gzip)|[**openvpn-2.4.2.tar.gz**](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.tar.gz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.4.2.tar.xz**](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.tar.xz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.tar.xz.asc)|
|Source Zip|[**openvpn-2.4.2.zip**](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.zip)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.4.2.zip.asc)|
|Installer, Windows Vista and later|[**openvpn-install-2.4.2-I601.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.4.2-I601.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.4.2-I601.exe.asc)|


|Source Tarball (gzip)|[**openvpn-2.3.16.tar.gz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.tar.gz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.tar.gz.asc)|
|----|----|----|
|Source Tarball (xz)|[**openvpn-2.3.16.tar.xz**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.tar.xz)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.tar.xz.asc)|
|Source Zip|[**openvpn-2.3.16.zip**](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.zip)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-2.3.16.zip.asc)|
|Installer (32-bit), Windows XP|[**openvpn-install-2.3.16-I001-i686.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I001-i686.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I001-i686.exe.asc)|
|Installer (64-bit), Windows XP|[**openvpn-install-2.3.16-I001-x86_64.exe**](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I001-x86_64.exe)|[GnuPG Signature](http://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I001-x86_64.exe.asc)|
|Installer (32-bit), Windows Vista and later|[**openvpn-install-2.3.16-I601-i686.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I601-i686.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I601-i686.exe.asc)|
|Installer (64-bit), Windows Vista and later|[**openvpn-install-2.3.16-I601-x86_64.exe**](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I601-x86_64.exe)|[GnuPG Signature](https://swupdate.openvpn.org/community/releases/openvpn-install-2.3.16-I601-x86_64.exe.asc)|





- 顶
3- 踩
1









