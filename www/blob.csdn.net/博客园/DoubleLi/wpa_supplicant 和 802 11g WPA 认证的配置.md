# wpa_supplicant 和 802.11g WPA 认证的配置 - DoubleLi - 博客园






# cd /etc/init.d# ln -s net.lo net.eth0默认的接口名是 wlan0，让它开机时自动 up：cp /etc/init.d/net.lo /etc/init.d/net.wlan0ifconfig wlan0 up

 根据接入点设置编辑 /etc/wpa_supplicant/wpa_supplicant.conf:
ctrl_interface=/var/run/wpa_supplicant
ctrl_interface_group=wheel
update_config=0
fast_reauth=1
#WEP网络
network={
        ssid="SYSTEX-GROUP"
        psk="密码"
        pairwise=TKIP
        group=TKIP
        priority=5
}
＃WPA网络
network={
        ssid="SYSTEX"
        key_mgmt=WPA-EAP IEEE8021X
        group=CCMP
        pairwise=CCMP
        identity="用户名"
        password="密码"
        priority=5
}


 根据接入点设置编辑 /etc/conf.c/net
modules=( "wpa_supplicant" )
wpa_supplicant_wlan0="-Dwext"
wpa_timeout_wlan0="60"
config_wlan0=( "dhcp" )
#dhcp_wlan0="nodns nontp nonis"
#config_wlan0=( "10.206.1.80 netmask 255.255.255.0 brd 255.255.255.0" )
#routes_wlan0=( "default via 10.206.1.254" )
#depend_wlan0() {
#need ipw3945d
#}


关于/etc/resolve.conf
如使用DHCP，则需要编辑/etc/resolve.conf.head中的内容来替代resolve.conf的内容

另外如使用无线网络，但有线网络同时存在时，要禁用有线网卡
/etc/init.d/net.eth0 stop

##########################################################
##########################################################
##########################################################
##########################################################
另外一篇设置教学

1. 此无线网络不设置安全机制，换句话说，么有设置密码，OK，我们该怎么做呢？命令行伺候

# iwlist scanning            # 搜索无线网络，我们假设搜索到了 oceanboo-wireless
# iwconfig interface essid oceanboo-wireless
# dhcpcd interface

够简单了吧，这样，你的机器就连接上 oceanboo-wireless 这个无线网络了，dhcp 获得了 ip 地址。不过不给无线路由器设置密码是很愚蠢的做法，假如你不是开咖啡厅、快餐店，还是设置一个密码吧。

2. 此无线网络设置了安全机制，使用 WEP 加密认证。

# iwlist scanning           # 还是一样的搜索，记住这个命令。
# iwconfig  interface essid oceanboo-wireless key XXXX-XXXX-XXXX-XXXX or XXXXXXXX.   # XXX多了点，不过如果你设置过无线路由，你一定知道这是啥玩意，我就不多说了。
# dhcpcd interface

哈，还是一样的简单。
关于 WEP 认证加密的输入还有下列这些方式，按需要索取吧。

引用
# iwconfig interface key 0123-4567-89
# iwconfig interface key [3] 0123-4567-89
# iwconfig interface key s:password [2]
# iwconfig interface key [2]
# iwconfig interface key open
# iwconfig interface key off
# iwconfig interface key restricted [3] 0123456789
# iwconfig interface key 01-23 key 45-67 [4] key [4]

其实，今天的重点在第三点，请继续观看。

3. 此无线网络设置了安全机制，而且使用了 WPA 加密认证。
光是用 wireless-tools 来干这个活，素么有办法的，我们需要另外安装一个包 wpa_supplicant。观众的瓜皮扔上来了，为虾米要使用这种认证方法呢？多麻烦啊，还要另外安装包，用 WEP 多好啊，那我们就先来看看两者之间的介绍吧，也许看了介绍，你就会回心转意了。

引用
WEP 全名为 Wired Equivalent Privacy
有线等效加密(Wired Equivalent Privacy,WEP)是个保护无线网路（Wi-Fi )的资料安全体制。因为无线网路是用无线电把讯息传播出去，它特别容易被偷听。WEP 的设计是要提供和传统有线的区域网路相当的机密性，而依此命名的。不过密码分析学家已经找出 WEP 好几个弱点，因此在2003年被 Wi-Fi Protected Access (WPA) 淘汰，又在2004年由完整的 IEEE 802.11i 标准（又称为 WPA2）所取代。WEP 虽然有些弱点，但也足以吓阻非专业人士的窥探了。
WEP 是1999年9月通过的 IEEE 802.11 标准的一部分，使用 RC4 (Rivest Cipher) 串流加密技术达到机密性，并使用 CRC-32 验和达到资料正确性。
标准的64比特WEP使用40比特的钥匙接上24比特的初向量(initialisation vector，IV) 成为 RC4 用的钥匙。在起草原始的 WEP 标准的时候，美国政府在加密技术的输出限制中限制了钥匙的长度，一旦这个限制放宽之后，所有的主要业者都用 104 比特的钥匙实作了 128 比特的 WEP 延伸协定。用户输入 128 比特的 WEP 钥匙的方法一般都是用含有 26 个十六进制数 (0-9 和 A-F)的字串来表示，每个字符代表钥匙中的 4 个比特， 4 * 26 = 104 比特，再加上 24 比特的 IV 就成了所谓的 "128 比特 WEP 钥匙"。有些厂商还提供 256 比特的 WEP 系统，就像上面讲的，24 比特是 IV，实际上剩下 232 比特作为保护之用，典型的作法是用 58 个十六进制数来输入，(58 * 4 = 232 比特) + 24 个 IV 比特 = 256 个 WEP 比特。
钥匙长度不是 WEP 安全性的主要因素，破解较长的钥匙需要拦截较多的封包，但是有某些主动式的攻击可以激发所需的流量。WEP 还有其他的弱点，包括 IV 雷同的可能性和变造的封包，这些用长一点的钥匙根本没有用，见 stream cipher attack 一页。

WPA 全名为 Wi-Fi Protected Access
Wi-Fi Protected Access (WPA 和 WPA2) 是一种保护无线电脑网络(Wi-Fi)安全的系统，它是应研究者在前一代的系统有线等效加密 (WEP) 中找到的几个严重的弱点而产生的。WPA 实作了 IEEE 802.11i 标准的大部分，是在 802.11i 完备之前替代 WEP 的过渡方案。WPA 的设计可以用在所有的无线网卡上，但未必能用在第一代的无线取用点上。WPA2 实作了完整的标准，但不能用在某些古老的网卡上。
WPA 是由 Wi-Fi 联盟这个业界团体建立的，他们拥有 Wi-Fi 这个名词的商标，并且会检验要使用 Wi-Fi 这个名词的设备以核发证书。
对 WPA 实作的检验在2003年4月开始，于2003年11月变成强制性。完整的 802.11i 标准是在2004年6月通过的。
在 WPA 的设计中要用到一个 802.1X 认证服务器来散布不同的钥匙给各个用户；不过它也可以用在较不保险的 "pre-shared key" (PSK) 模式，让每个用户都用同一个密语。Wi-Fi 联盟把这个使用 pre-shared key 的版本叫做 WPA 个人版 或 WPA2 个人版，用 802.1X 认证的版本叫做 WPA 企业版 或 WPA2 企业版。
WPA 的资料是以一把 128 位元的钥匙和一个 48 位元的初向量 (IV) 的 RC4 stream cipher 来加密。WPA 超越 WEP 的主要改进就是在使用中可以动态改变钥匙的“临时钥匙完整性协定”(Temporal Key Integrity Protocol，TKIP)，加上更长的初向量，这可以击败知名的针对 WEP 的金钥撷取攻击。
除了认证跟加密外，WPA 对于所载资料的完整性也提供了巨大的改进。WEP 所使用的 CRC（循环冗余校验） 先天就不安全，在不知道 WEP 钥匙的情况下，要篡改所载资料和对应的 CRC 是可能的，而 WPA 使用了称为 "Michael" 的更安全的讯息认证码（在 WPA 中叫做讯息完整性查核，MIC）。进一步地，WPA 使用的 MIC 包含了帧计数器，以避免 WEP 的另一个弱点－replay attack（回放攻击）－的利用。
有两个理由使得 WPA 被定位为到达较安全的 802.11 保全之前的过渡步骤：
  1. 制定 802.11i 的工作比原先预期的久了很多，在大家越来越担心无线安全性之时竟然花了四年之久；
  2. 它包含了与 WEP 兼容的 802.11i 子集合，即使是最早的 802.11b 接口卡也能用。

--------摘录自维基百科


看了介绍，你是不是已经准备好安装 wpa_supplicant 了呢？

# emerge -av wpa_supplicant

安装好后，就有了 wpa_supplicant 、 wpa_cli 、 wpa_passphrase 、 wpa_gui （前面的日志就介绍过这个工具，当然在 Gentoo 下，这个工具被无情的咔嚓掉了，因为 USE=“-qt”）这四个工具。
这里提供一个相对比较简单的解决方法。
A. 首先当然是确定无线路由是采用的 WPA 加密方式，还有需要知道密码(废话)，然后我们执行如下命令：

# wpa_passphrase [ ssid ] [ passphrase ] >> /etc/wpa_supplicant/wpa_supplicant.conf
上面这条命令可以自动生成一段配置，我们将它输出添加到 wpa_supplicant 的默认配置文件里面。
# wpa_supplicant -i interface -c /etc/wpa_supplicant/wpa_supplicant.conf  -D wext -B 
上面的命令 -i = interface 、-c 读取配置文件 、 -D = dirver （wext 是一个通用驱动） -B 后台运行
# dhcpcd interface     


需要详细了解几个命令参数的，可以 man 一下看看，都比较简单。
现在，无线上网倒是没问题了，不过该怎么让它更自动化一点是该想想了，写到这里，睡觉先。


##########################################################
##########################################################
##########################################################
##########################################################

WPA配置

使用wpa_passphrase命令产生PSK：
用法：wpa_passphrase <ssid> [passphrase]
比如：wpa_passphrase my_ssid abcd1234

network={
        ssid="my_ssid"
        #psk="abce1234"
        psk=xxxxxxxxxxxxxxxxxxxxx
}
完整的配置文件/etc/wpa_supplicant.conf:

ctrl_interface=/var/run/wpa_supplicant
ctrl_interface_group=0
ap_scan=1

network={
        ssid="my_ssid"
        pairwise=CCMP TKIP
        group=CCMP TKIP WEP104 WEP40
        proto=WPA
        key_mgmt=WPA-PSK
        priority=5
        psk=xxxxxxxxxxxxxxxxxxxx
}
手工启动：

wpa_supplicant -Dmadwifi -iath0 -c/etc/wpa_supplicant.conf
使用命令

iwconfig ath0
可以查看，Access Poin是否连接成功了。如果是使用dncp的话，使用dhclient ath0来自动获取IP，如果使用静态的IP的话，直接使用ifconfig配置IP。

不过，有时候有些AP和无线网卡芯片不太兼容的话，有可能用不了WPA协议进行连接。比如我的笔记本就是这样，所以也可以使用WEP协议进行连接，安全性方面，我想使用128位的加密以及隐藏接入点，应该是没太大的问题的。

使用WEP协议的话，除了在AP上面要配置好之外，在debian上面只需要使用“iwconfig”命令，就可以进行实现连接，也可以写成一个启动脚本，在开机的时候自动进行链接。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/Crob/archive/2008/11/13/3292950.aspx](http://blog.csdn.net/Crob/archive/2008/11/13/3292950.aspx)









