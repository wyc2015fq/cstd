# linux 无线网络配置工具wpa_supplicant与wireless-tools - DoubleLi - 博客园






**内容:**
- [介绍](http://www.gentoo.org/doc/zh_cn/handbook/handbook-x86.xml?part=4&chap=4#doc_chap1)
- [WPA Supplicant](http://www.gentoo.org/doc/zh_cn/handbook/handbook-x86.xml?part=4&chap=4#doc_chap2)
- [Wireless Tools](http://www.gentoo.org/doc/zh_cn/handbook/handbook-x86.xml?part=4&chap=4#doc_chap3)
- [针对每个ESSID的网络配置](http://www.gentoo.org/doc/zh_cn/handbook/handbook-x86.xml?part=4&chap=4#doc_chap4)





4.a. 介绍

目前您可以使用我们提供的wireless-tools 或wpa_supplicant工具来配置无线网络。请记住重要的一点是，您对无线网络的配置是全局性的，而非针对具体的接口。

wpa_supplicant是一个最好的选择，但缺点是它不支持所有的驱动。请浏览[wpa_supplicant网站](http://hostap.epitest.fi/wpa_supplicant)获得它所支持的驱动列表。另外，wpa_supplicant目前只能连接到那些你已经配置好ESSID的无线网络。

wireless-tools支持几乎所有的无线网卡和驱动，但它不能连接到那些只支持WPA的AP。
|**警告: **由于linux-wlan-ng驱动有其自己特有的安装和配置方式，而且与其他软件的方式完全不同，因此目前它并不被baselayout所支持。有传闻说linux-wlan-ng的开发者要将它的设置方式改成wireless-tools的方式——到时候您就可以在baselayout中使用linux-wlan-ng了。|
|----|

4.b. WPA Supplicant

[WPA Supplicant](http://hostap.epitest.fi/wpa_supplicant)工具包可以让您连接到那些使用WPA的AP。因为还只是beta版，所以它的配置方法仍会常常变化——尽管如此，在大部分情况下它已经能很好的工作。


|代码 2.1: 安装wpa_supplicant|
|----|
|# emerge net-wireless/wpa_supplicant|
|**重要: **要让wpa_supplicant正常工作，您必须在内核中打开CONFIG_PACKET支持。|
|----|

现在我们需要配置一下/etc/conf.d/net文件以便我们可以选择使用wpa_supplicant而不用wireless-tools（两者都安装在系统中时，默认使用的是wireless-tools）。


|代码 2.2: 在/etc/conf.d/net中配置wpa_supplicant|
|----|
|# 使用wpa_supplicant代替wireless-toolsmodules=( "wpa_supplicant" )# 因为wpa_supplicant还不能很好的自动侦测驱动，所以需要我们为其指定正在使用的驱动。wpa_supplicant_eth0="-Dmadwifi"|
|**注意: **如果您用host-ap驱动，您首先要将无线设备卡设置成Managed模式以便能正确地配合wpa_supplicant工作。你可以在/etc/conf.d/net中设置iwconfig_eth0="mode managed"来实现这一点。|
|----|

看起来这很简单，不是么？不过我们还需要配置wpa_supplicant本身，这将会比较麻烦一些，具体取决于你要连接的AP的安全程度。下面的例子是从/usr/share/doc/wpa_supplicant-/wpa_supplicant.conf.gz中抽取并简化而来的，此文件出自wpa_supplicant软件包。


|代码 2.3: 一个/etc/wpa_supplicant/wpa_supplicant.conf的例子|
|----|
|# 请不要修改下面这一行内容，否则将不能正常工作ctrl_interface=/var/run/wpa_supplicant# 确保只有root用户能读取WPA的配置ctrl_interface_group=0# 使用wpa_supplicant来扫描和选择APap_scan=1# 简单的情形：WPA-PSk密码验证方式，PSK是ASCII密码短语，所有合法的加密方式都允许连接network={ssid="simple"psk="very secret passphrase"# 优先级越高，就能越早匹配到。priority=5}# 与前面的设置相同，但要求对特定的SSID进行扫描（针对那些拒绝广播SSID的AP）network={ssid="second ssid"scan_ssid=1psk="very secret passphrase"priority=2}# 仅使用WPA-PSK方式。允许使用任何合法的加密方式的组合network={ssid="example"proto=WPAkey_mgmt=WPA-PSKpairwise=CCMP TKIPgroup=CCMP TKIP WEP104 WEP40psk=06b4be19da289f475aa46a33cb793029d4ab3db7a23ee92382eb0106c72ac7bbpriority=2}# 明文连接方式（不使用WPA和IEEE802.1X）network={ssid="plaintext-test"key_mgmt=NONE}# 共享WEP密钥连接方式（不使用WPA和IEEE802.1X）network={ssid="static-wep-test"key_mgmt=NONE# 引号包含的密钥是ASCII密钥wep_key0="abcde"# 没有引号包含的密钥是十六进制密钥wep_key1=0102030405wep_key2="1234567890123"wep_tx_keyidx=0priority=5}# 共享WEP密钥连接方式（无WPA和IEEE802.1X），使用共享密钥IEEE802.11验证方式network={ssid="static-wep-test2"key_mgmt=NONEwep_key0="abcde"wep_key1=0102030405wep_key2="1234567890123"wep_tx_keyidx=0priority=5auth_alg=SHARED}# 在IBSS/ad-hoc网络中使用WPA-None/TKIPnetwork={ssid="test adhoc"mode=1proto=WPAkey_mgmt=WPA-NONEpairwise=NONEgroup=TKIPpsk="secret passphrase"}|

4.c. Wireless Tools

初始设置和管理模式

[Wireless Tools](http://www.hpl.hp.com/personal/Jean_Tourrilhes/Linux/Tools.html)提供了一个通用的方法设置无线网络接口，最高可达WEP安全等级。虽然WEP是一种较弱的安全方式，但它也是最普遍使用的加密方式。

Wireless Tools的配置由几个主要变量来控制，以下配置文件的例子描述了您所需要了解的所有内容。要牢记于心的是：可确保“连接到没有加密的并且信号最强的AP”的配置并不存在——但我们会一直尝试并帮您连接到某个AP。


|代码 3.1: 安装wireless-tools|
|----|
|# emerge net-wireless/wireless-tools|
|**注意: **尽管您可以将无线设置保存在/etc/conf.d/wireless中，但是本指南还是推荐您将它们保存在/etc/conf.d/net之中。|
|----|
|**重要: **您将需要参阅文档[变量名称](http://www.gentoo.org/doc/zh_cn/handbook/handbook-x86.xml?part=4&chap=2#variable_name)。|
|----|


|代码 3.2: 在/etc/conf.d/net中配置iwconfig的例子|
|----|
|# 使用iwconfig而不用wpa_supplicantmodules=( "iwconfig" )# 为名为ESSID1和ESSID2的AP配置WEP密钥# 您最多可以配置4个WEP密钥，但任何时候只有其中1个起作用。# 所以我们提供一个默认的下标[1]来设置密钥[1]，之后紧接着把活动密钥设置为[1]。# 我们这么做以备你让其他ESSID使用[1]以外的WEP密钥。## key加上前缀s：意味着它是一个ASCII密钥，否则它就是一个16进制密钥。## enc open 指定开放安全性（最安全）# enc restricted 指定限制安全性（较不安全）key_ESSID1="[1] s:yourkeyhere key [1] enc open"key_ESSID2="[1] aaaa-bbbb-cccc-dd key [1] enc restricted"# 以下仅在我们扫描可用的AP时起作用# 有时有多个AP可见，所以我们需要规定一个首选的连接次序。preferred_aps=( "ESSID1" "ESSID2" )|

细致调整AP选择

您可以添加一些额外的选项来细致的调整AP的选择，不过正常情况下并不需要这么做。

您可以决定是否只连接首选的AP。默认情况下，当配置中列出的所有AP的连接都失败后，这时如果环境中有一个非加密的AP，系统将会与其连接。这个行为可以用associate_order变量来进行控制。下面给出一个相关的值的列表以及它们如何控制AP的选择。
|**值**|**描述**|
|----|----|
|any|默认行为|
|preferredonly|只连接首选列表里的可见AP|
|forcepreferred|按首选列表里的顺序强制连接AP，如果扫描不到的话|
|forcepreferredonly|不进行扫描——按首选列表里的顺序直接尝试连接AP|
|forceany|和forcepreferred一样，外加连接到任何其他可用的AP|

最后我们还提供了一些blacklist_aps和unique_ap的选择。blacklist_aps的工作方式和preferred_aps类似。unique_ap是一个yes或no的值，它决定了是否允许两个无线接口同时接入一个AP。


|代码 3.3: blacklist_aps和unique_ap的例子|
|----|
|# 有时您根本不想连接到某些APblacklist_aps=( "ESSID3" "ESSID4" )#如果您有多个无线网卡，您可以决定是否允许每个卡都能连接到同一个无线AP# 可以取的值是"yes"或者"no"# 默认设置为"yes"unique_ap="yes"|

Ad-Hoc和Master模式

当您在管理模式中无法连接到任何AP时，您也可以将自己的设备设置成Ad-Hoc节点。


|代码 3.4: 失败后转用ad-hoc模式|
|----|
|adhoc_essid_eth0="This Adhoc Node"|

那么，要如何连接到Ad-Hoc网络，或者干脆运行于Master模式使自身成为一个无线接入点呢？这里有这样一个设置！您可能需要参照本章前面的内容来指定WEP密钥。


|代码 3.5: ad-hoc/master配置的例子|
|----|
|# 设定模式为managed（默认）、ad-hoc或者master。并不是所有的设备都支持所有的模式。mode_eth0="ad-hoc"# 设定接口的ESSID# 在managed模式中，这将强制此接口只尝试连接特定的ESSID。essid_eth0="This Adhoc Node"# 指定使用的频道，否则将默认使用频道3。channel_eth0="9"|
|**重要: **以下是从[NetBSD 文档](http://www.netbsd.org/Documentation/network/wavelan.html)的BSD wavelan文档中逐字逐句复制过来的内容。目前共计有14个可用的频道；1-11频道在北美是合法的，而在欧洲大部分地区则是频道1-13，在法国是 频道10-13，在日本只允许使用频道14。如果有疑问，请参考随您所购买的无线网卡或AP附带的说明书。确保调整您的无线网卡与AP（或工作于ad- hoc模式的另一块无线网卡）使用同一个频道。默认情况下，在北美和欧洲大部分地区销售的无线网卡使用频道3；在法国使用频道11，在日本使用频道14。|
|----|

Wireless Tools故障修除

一些环境或驱动的问题可能会使无线网络不能正常工作，下表多给出一些变量，可能有助于你解决问题。
|**参数**|**默认值**|**描述**|
|----|----|----|
|iwconfig_eth0||请参看iwconfig的man page了解iwconfig各项参数|
|iwpriv_eth0||请参看iwpriv的man pages了解iwpriv各项参数|
|sleep_scan_eth0|0|在尝试扫描前的休眠时间（以秒为单位）。当驱动／固件需要时间激活时设置这个值。|
|sleep_associate_eth0|5|无线网络尝试连接到AP等待的秒数，超时则转向下一个AP。|
|associate_test_eth0|MAC|一些驱动程序在失去连接或尝试连接时不会重置无效AP的MAC地址，而有一些驱动程序在碰到这些情况时不会重设quality level。这里有效的设定是MAC，quality和all。|
|scan_mode_eth0||某些驱动必须在ad-hoc模式下扫描，因此若扫描失败的话请尝试把此变量设置成ad-hoc|
|iwpriv_scan_pre_eth0||扫描前先向接口发送一些iwpriv命令。更多细节请参看iwpriv的man page。|
|iwpriv_scan_post_eth0||扫描后向接口发送一些iwpriv命令。更多细节请参看iwpriv的man page。|

4.d. 针对每个ESSID的网络配置

有时，您连接ESSID1需要使用一个固定IP，而连接ESSID2要使用DHCP。实际上，大多数模块变量可以针对每个ESSID来定义。下面我们给出具体的做法。
|**注意: **只有使用WPA Supplicant或者Wireless Tools时，这些设定才起作用。|
|----|
|**重要: **您将需要参阅文档[变量名称](http://www.gentoo.org/doc/zh_cn/handbook/handbook-x86.xml?part=4&chap=2#variable_name)。|
|----|




|代码 4.1: 覆盖每一个ESSID的配置|
|----|
|config_ESSID1=( "192.168.0.3/24 brd 192.168.0.255" )routes_ESSID1=( "default via 192.168.0.1" )config_ESSID2=( "dhcp" )fallback_ESSID2=( "192.168.3.4/24" )fallback_route_ESSID2=( "default via 192.168.3.1" )# 我们可以定义DNS服务器和其他的一些东西# 注意:DHCP将覆盖这些设定，除非我们要求它不要覆盖dns_servers_ESSID1=( "192.168.0.1" "192.168.0.2" )dns_domain_ESSID1="some.domain"dns_search_domains_ESSID1="search.this.domain search.that.domain"# 根据无线AP的MAC地址来覆盖相关设定# 这在不同地点有相同ESSID的情况下非常有用config_001122334455=( "dhcp" )dhcpcd_001122334455="-t 10"dns_servers_001122334455=( "192.168.0.1" "192.168.0.2" )|









