# 走向世界系列1： IPsec L2TP PPTP介绍与搭建vpn服务器 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月27日 11:22:10[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1266
IPsec：**互联网安全协议**（英语：Internet Protocol
 Security，缩写为IPsec）
L2TP ：**第二层隧道协议**（英语：Layer Two Tunneling Protocol，缩写为L2TP)是一种[虚拟隧道协议](https://zh.wikipedia.org/wiki/%E8%99%9A%E6%8B%9F%E9%9A%A7%E9%81%93%E5%8D%8F%E8%AE%AE)，通常用于[虚拟专用网](https://zh.wikipedia.org/wiki/%E8%99%9A%E6%8B%9F%E4%B8%93%E7%94%A8%E7%BD%91)。L2TP协议自身不提供[加密](https://zh.wikipedia.org/wiki/%E5%8A%A0%E5%AF%86)与可靠性验证的功能，可以和[安全协议](https://zh.wikipedia.org/wiki/%E5%AE%89%E5%85%A8%E5%8D%8F%E8%AE%AE)搭配使用，从而实现数据的加密传输。经常与L2TP协议搭配的加密协议是[IPsec](https://zh.wikipedia.org/wiki/IPsec)，当这两个协议搭配使用时，通常合称L2TP/IPsec。
PPTP：**点对点隧道协议**（英语：Point
 to Point Tunneling Protocol，缩写为PPTP）是实现[虚拟专用网](https://zh.wikipedia.org/wiki/%E8%99%9B%E6%93%AC%E7%A7%81%E4%BA%BA%E7%B6%B2%E8%B7%AF)（VPN）的方式之一。因为它的加密方式容易被破解，微软已经不再建议使用这个协议。
本文搭建参考：[https://github.com/hwdsl2/setup-ipsec-vpn/blob/master/README-zh.md](https://github.com/hwdsl2/setup-ipsec-vpn/blob/master/README-zh.md)
使用 [Libreswan](https://libreswan.org/) 作为
 IPsec 服务器，以及 [xl2tpd](https://github.com/xelerance/xl2tpd) 作为 L2TP 提供者
一键快速搭建！！！
apt-get update
wget https://git.io/vpnsetup -O vpnsetup.sh
&& sudo sh vpnsetup.sh
如果使用 CentOS，请将上面的地址换成 https://git.io/vpnsetup-centos。先yum update一下
 VPN 登录凭证将会被自动随机生成，并在安装完成后显示在屏幕上。
选项 2: 编辑脚本并提供你自己的 VPN 登录凭证：
wget https://git.io/vpnsetup -O vpnsetup.shnano -w vpnsetup.sh[替换为你自己的值： YOUR_IPSEC_PSK,
 YOUR_USERNAME 和 YOUR_PASSWORD]sudo sh vpnsetup.sh

docker版本：
[https://github.com/hwdsl2/docker-ipsec-vpn-server/blob/master/README-zh.md](https://github.com/hwdsl2/docker-ipsec-vpn-server/blob/master/README-zh.md)
[https://github.com/gaomd/docker-ikev2-vpn-server](https://github.com/gaomd/docker-ikev2-vpn-server)

配置你的计算机或其它设备使用 VPN 。请参见：
[配置
 IPsec/L2TP VPN 客户端](https://github.com/hwdsl2/setup-ipsec-vpn/blob/master/docs/clients-zh.md)
对于有外部防火墙的服务器，请为 VPN 打开 UDP 端口 500 和 4500。
显示当前已建立的 VPN 连接：
ipsec whack --trafficstatus
缺陷：由于 IPsec/L2TP 的局限性，如果需要同时连接在同一个 NAT （比如家用路由器）后面的多个设备到 VPN 服务器，你必须仅使用 [IPsec/XAuth
 模式](https://github.com/hwdsl2/setup-ipsec-vpn/blob/master/docs/clients-xauth-zh.md)配置 IPsec/XAuth VPN 客户端。另外，你的服务器必须运行 [Libreswan
 3.19 或更新版本](https://github.com/hwdsl2/setup-ipsec-vpn/blob/master/README-zh.md#%E5%8D%87%E7%BA%A7libreswan)。
IPsec/XAuth 模式也称为 "Cisco IPsec"。该模式通常能够比 IPsec/L2TP 更高效地传输数据。
鉴于这个目前能用，不能用了再尝试下面的：
[https://github.com/Nyr/openvpn-install](https://github.com/Nyr/openvpn-install)
其实未看的链接：
[http://ilinuxkernel.com/?p=1594](http://ilinuxkernel.com/?p=1594)
[https://cnodejs.org/topic/54b3fc05edf686411e1b9ce1](https://cnodejs.org/topic/54b3fc05edf686411e1b9ce1)
[http://www.liuchungui.com/blog/2016/04/23/ubuntuxia-da-jian-vpn/](http://www.liuchungui.com/blog/2016/04/23/ubuntuxia-da-jian-vpn/)
[http://cuiqingcai.com/512.html](http://cuiqingcai.com/512.html)
[http://www.jianshu.com/p/1713fe645b37](http://www.jianshu.com/p/1713fe645b37)
[https://www.figotan.org/2016/05/04/cook-your-own-vpn/](https://www.figotan.org/2016/05/04/cook-your-own-vpn/)
[https://www.howtogeek.com/221001/how-to-set-up-your-own-home-vpn-server/](https://www.howtogeek.com/221001/how-to-set-up-your-own-home-vpn-server/)

