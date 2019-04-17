# wifi测试相关（iwconfig，WPA Supplicant用法） - DoubleLi - 博客园






**iwconfig用法**

1.打开无线网卡电源

iwconfig wlan0 txpower no



2.列出区域内的无线网络

iwconfig wlan0 scan



3.假设要连接到网络myhome（即essid为myhome的网络），那么输入命令

iwconfig wlan0 essid "myhome"



如果网络是加密的，密码是0123456789，那么就输入命令

iwconfig wlan0 essid "myhome" key 0123-4567-89



4 如果正常的话，输入

iwconfig wlan0

就可以看到连接正常的各项参数了。



5.启动无线网卡

ifconfig wlan0 up



6.如果是用DHCP获取IP的，那么用dhclient或dhcpcd获取ip

dhclient wlan0 或dhcpcd wlan0



7.现在无线网卡应该可以正常使用了。



**WPA Supplicant**

**[WPA Supplicant](http://hostap.epitest.fi/wpa_supplicant)工具包可以让您连接到那些使用WPA的AP。因为还只是beta版，所以它的配置方法仍会常常变化——尽管如此，在大部分情况下它已经能很好的工作。**


|代码 2.1: 安装wpa_supplicant|
|----|
|# emerge net-wireless/wpa_supplicant|
|**重要: **要让wpa_supplicant正常工作，您必须在内核中打开CONFIG_PACKET支持。|
|----|



现在我们需要配置一下/etc/conf.d/net文件以便我们可以选择使用wpa_supplicant而不用wireless-tools（两者都安装在系统中时，默认使用的是wireless-tools）。


|代码 2.2: 在/etc/conf.d/net中配置wpa_supplicant|
|----|
|# 使用wpa_supplicant代替wireless-toolsmodules=( "wpa_supplicant" )# 因为wpa_supplicant还不能很好的自动侦测驱动，所以需要我们为其指定正在使用的驱动。wpa_supplicant_eth0="-Dmadwifi"|
|**注意: **如果您用host-ap驱动，您首先要将无线设备卡设置成Managed模式以便能正确地配合wpa_supplicant工作。你可以在/etc/conf.d/net中设置iwconfig_eth0="mode managed"来实现这一点。|
|----|

看起来这很简单，不是么？不过我们还需要配置wpa_supplicant本身，这将会比较麻烦一些，具体取决于你要连接的AP的安全程度。下面的例子是从/usr/share/doc/wpa_supplicant-<version>/wpa_supplicant.conf.gz中抽取并简化而来的，此文件出自wpa_supplicant软件包。


|代码 2.3: 一个/etc/wpa_supplicant/wpa_supplicant.conf的例子|
|----|
|**[cpp]**[view plain](http://blog.csdn.net/jacobywu/article/details/7366080)[copy](http://blog.csdn.net/jacobywu/article/details/7366080)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)- # 请不要修改下面这一行内容，否则将不能正常工作  - ctrl_interface=/var/run/wpa_supplicant  - - # 确保只有root用户能读取WPA的配置  - ctrl_interface_group=0  - - # 使用wpa_supplicant来扫描和选择AP  - ap_scan=1  - - # 简单的情形：WPA-PSk密码验证方式，PSK是ASCII密码短语，所有合法的加密方式都允许连接  - network={  -   ssid="simple"  -   psk="very secret passphrase"  -   # 优先级越高，就能越早匹配到。  -   priority=5  - }  - - # 与前面的设置相同，但要求对特定的SSID进行扫描（针对那些拒绝广播SSID的AP）  - network={  -   ssid="second ssid"  -   scan_ssid=1  -   psk="very secret passphrase"  -   priority=2  - }  - - # 仅使用WPA-PSK方式。允许使用任何合法的加密方式的组合  - network={  -   ssid="example"  -   proto=WPA  -   key_mgmt=WPA-PSK  -   pairwise=CCMP TKIP  -   group=CCMP TKIP WEP104 WEP40  -   psk=06b4be19da289f475aa46a33cb793029d4ab3db7a23ee92382eb0106c72ac7bb  -   priority=2  - }  - - # 明文连接方式（不使用WPA和IEEE802.1X）  - network={  -   ssid="plaintext-test"  -   key_mgmt=NONE  - }  - - # 共享WEP密钥连接方式（不使用WPA和IEEE802.1X）  - network={  -   ssid="static-wep-test"  -   key_mgmt=NONE  -   # 引号包含的密钥是ASCII密钥  -   wep_key0="abcde"  -   # 没有引号包含的密钥是十六进制密钥  -   wep_key1=0102030405  -   wep_key2="1234567890123"  -   wep_tx_keyidx=0  -   priority=5  - }  - - # 共享WEP密钥连接方式（无WPA和IEEE802.1X），使用共享密钥IEEE802.11验证方式  - network={  -   ssid="static-wep-test2"  -   key_mgmt=NONE  -   wep_key0="abcde"  -   wep_key1=0102030405  -   wep_key2="1234567890123"  -   wep_tx_keyidx=0  -   priority=5  -   auth_alg=SHARED  - }  - - # 在IBSS/ad-hoc网络中使用WPA-None/TKIP  - network={  -   ssid="test adhoc"  -   mode=1  -   proto=WPA  -   key_mgmt=WPA-NONE  -   pairwise=NONE  -   group=TKIP  -   psk="secret passphrase"  - }|

**Wifi图形配置软件可以用：**[http://wifi-radar.berlios.de/](http://wifi-radar.berlios.de/)

**Wifi更加详细的介绍可以参考：**[http://www.gentoo.org/doc/zh_cn/handbook/handbook-x86.xml?part=4&chap=4](http://www.gentoo.org/doc/zh_cn/handbook/handbook-x86.xml?part=4&chap=4)









