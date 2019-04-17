# wpa_supplicant 配置与应用 - DoubleLi - 博客园






- 
# 概述


wpa_supplicant是wifi客户端（client）加密认证工具，和iwconfig不同，wpa_supplicant支持wep、wpa、wpa2等完整的加密认证，而iwconfig只能支持wep。

和wpa_supplocant相对应的，ap端的加密认证工具为hostapd。

wpa_supplicant运行于后台，它需要借助控制台工具wpa_cli来进行手动操作。
- 
# wpa_supplicant配置文件
- 
## Config文件



在wpa_supplicant源码目录下，存在参考的配置文件wpa_supplicant.conf，几乎包含里所有的配置项。

我们的配置文件不需要这么复杂，开始测试阶段，我只写最简单的配置文件，其它手动操作先。

在/etc/下建立配置文件wpa_supplicant.conf，内容如下：
|ctrl_interface=/var/run/wpa_supplicantupdate_config=1|
|----|

其中，update_config=1使能配置更改。


- 
## wpa_supplicant参数


wpa_supplicant可以通过如下命令查看其所有操作参数：


|#wpa_supplicant --helpwpa_supplicant: invalid option -- -wpa_supplicant v0.8.xCopyright (c) 2003-2011, Jouni Malinen <j@w1.fi> and contributorsThis program is free software. You can distribute it and/or modify itunder the terms of the GNU General Public License version 2.Alternatively, this software may be distributed under the terms of theBSD license. See README and COPYING for more details.usage:wpa_supplicant [-BddhKLqqstuvW] [-P<pid file>] [-g<global ctrl>] \-i<ifname> -c<config file> [-C<ctrl>] [-D<driver>] [-p<driver_param>] \[-b<br_ifname>] [-f<debug file>] [-e<entropy file>] \[-o<override driver>] [-O<override ctrl>] \[-N -i<ifname> -c<conf> [-C<ctrl>] [-D<driver>] \[-p<driver_param>] [-b<br_ifname>] ...]drivers:athr = Atheros Linux driveroptions:-b = optional bridge interface name-B = run daemon in the background-c = Configuration file-C = ctrl_interface parameter (only used if -c is not)-i = interface name-d = increase debugging verbosity (-dd even more)-D = driver name (can be multiple drivers: nl80211,wext)-e = entropy file-g = global ctrl_interface-K = include keys (passwords, etc.) in debug output-t = include timestamp in debug messages-h = show this help text-L = show license (GPL and BSD)-o = override driver parameter for new interfaces-O = override ctrl_interface parameter for new interfaces-p = driver parameters-P = PID file-q = decrease debugging verbosity (-qq even less)-v = show version-W = wait for a control interface monitor before starting-N = start describing new interfaceexample:wpa_supplicant -Dwext -iwlan0 -c/etc/wpa_supplicant.conf|
|----|



其中最常用的为：-i 指定端口，-c 指定配置文件，-D 指定使用的wifi驱动

我们这里只指定端口和配置文件，驱动使用默认的。

wpa_supplicant启动操作命令如下：
|#wpa_supplicant -i ath0 -c /etc/wpa_supplicant.conf &|
|----|



注意：启动wpa_supplicant之前wifi必须先启动，wpa_supplicnat使用在wifi client端口上。



在测试过程中，如果出现问题，可以在wpa_supplicant启动时使用参数使其输出更多的debug信息：

#wpa_supplicant -i ath0 -c /etc/wpa_supplicant.conf -dddd
- 
#  手动操作


wpa_supplicant启动后，就可以通过wpa_cli来操作设置了。

wpa_cli操作有两种方法，一是直接进入CLI控制台操作，如下：

#wpa_cli
|# wpa_cliwpa_cli v0.8.xCopyright (c) 2004-2011, Jouni Malinen <j@w1.fi> and contributorsThis program is free software. You can distribute it and/or modify itunder the terms of the GNU General Public License version 2.Alternatively, this software may be distributed under the terms of theBSD license. See README and COPYING for more details.Selected interface 'ath0'Interactive mode> add0> list_networknetwork id / ssid / bssid / flags0 any [DISABLED]>|
|----|



另一种是直接在终端中敲击完整的命令行操作，如下：
|# wpa_cli -i ath0 set_network 0 ssid '"206"'OK|
|----|



这两种方法的效果是一样的，主要区别如下：
- 
wpa_cli控制台中写的命令行要简单些，不需要指定端口(如wpa_cli –i ath0),但写错了不能回退，需要重新完整写入

- 
终端写入的命令行长写，但写错里可以回退重新写。

- 
对于ssid和密码，终端中操作时需要使用单引号把双引号的字符包括起来，而cli控制台中只需要双引号，如下：





|> set_network 0 psk "12345678"OK|
|----|



wpa_cli支持的所有命令可以通过help操作来查看
|> help|
|----|




- 
##  扫描于连接


以下是我的一个完整的扫描连接过程：
- 
启动wpa_supplicant和wpa_cli.

|# wpa_supplicant -i ath0 -c /etc/wpa_supplicant.conf &# wpa_cli|
|----|


- 
扫描ap


使用scan命令扫描网络，结束后使用scan_results命令查看网络。
|> scanOK> driver_atheros_event_wireless: scan result event - SIOCGIWSCAN<3>CTRL-EVENT-SCAN-RESULTS<3>WPS-AP-AVAILABLE> > scan_resultsbssid / frequency / signal level / flags / ssid00:23:68:26:40:c8 2412 36 [ESS] CMCC00:26:5a:26:33:ac 2437 38 [WPA-PSK-TKIP][WPA2-PSK-TKIP][WPS][ESS] LZ205>|
|----|


- 
增加网络


使用add_network命令增加一个网络，可以使用list_network查看增加的网络信息。
|> add_network0> list_networknetwork id / ssid / bssid / flags0 any [DISABLED]|
|----|


- 
设定网络连接的ssid和密码


使用set_network命令设定连接的ap的用户名和密码
|> set_network 0 ssid "LZ205"OK> set_network 0 psk "20100208"OK|
|----|


- 
使能网络连接


使用enable_network命令使能网络连接
|> enable_network 0OK> driver_atheros_event_wireless: scan result event - SIOCGIWSCANath0: Trying to associate with 00:26:5a:26:33:ac (SSID='LZ205' freq=2437 MHz)<3>CTRL-EVENT-SCA ieee80211_ioctl_setmlme: os_opmode=1[ieee80211_ioctl_setmlme] set desired bssid 00:26:5a:26:33:acN-RESULTS<3>WPS-AP-AVAILABLE<3>Trying to associate with 00:26:5a:26:33:ac (SSID='LZ205' freq=2437 MHz)ath_paprd_cal PAPRD excessive failure disabling PAPRD nowath0: Associated with 00:26:5a:26:33:ac<3>Associated with 00:26:5a:26:33:acvap-0: mlme_sta_swbmiss_timer_handler: SW Beacon miss!!vap-0: mlme_sta_swbmiss_timer_handler: SW Beacon miss!!vap-0: ieee80211_vap_iter_beacon_miss: Beacon miss, will indicate to OS!!ath0: WPA: 4-Way Handshake failed - pre-shared key may be incorrect<3>WPA: 4-Way Handshake failed - pre-shared key may be incorrectath0: CTRL-EVENT-DISCONNECTED bssid=00:26:5a:26:33:ac reason=0<3>CTRL-EVENT-DISCONNECTED bssid=00:26:5a:26:33:ac reason=0ieee80211_ioctl_setmlme: os_opmode=1 sult event - SIOCGIWSCAN[ieee80211_ioctl_setmlme] set desired bssid 00:26:5a:26:33:ac<3>CTRL-EVENT-SCAN-RESULTS<3>WPS-AP-AVAILABLEath0: Trying to associate with 00:26:5a:26:33:ac (SSID='LZ205' freq=2437 MHz)<3>Trying to associate with 00:26:5a:26:33:ac (SSID='LZ205' freq=2437 MHz)ath0: Associated with 00:26:5a:26:33:ac<3>Associated with 00:26:5a:26:33:acath0: WPA: Key negotiation completed with 00:26:5a:26:33:ac [PTK=TKIP GTK=TKIP]<3>WPA: Key negotiation completed with 00:26:5a:26:33:ac [PTK=TKIP GTK=TKIP]ath0: CTRL-EVENT-CONNECTED - Connection to 00:26:5a:26:33:ac completed (auth) [id=0 id_str=]<3>CTRL-EVENT-CONNECTED - Connection to 00:26:5a:26:33:ac completed (auth) [id=0 id_str=]|
|----|



看到CTRL-EVENT-CONNECTED信息，表示连接完成。
- 
状态查看


使用status命令可以查看网络状态，使用quit退出CLI后，可以使用iwconfig命令查看ath0连接状态。
|> statusbssid=00:26:5a:26:33:acssid=LZ205id=0mode=stationpairwise_cipher=TKIPgroup_cipher=TKIPkey_mgmt=WPA2-PSKwpa_state=COMPLETEDaddress=20:13:08:15:16:13|
|----|


- 
config保存


使用save命令可以保存当前的连接设置，下次wifi启动时会自动连接此AP.

保存后，wpa_supplicant.conf文件被自动修改为如下内容：
|ctrl_interface=/var/run/wpa_supplicantupdate_config=1device_type=0-00000000-0network={ssid="LZ205"psk="20100208"}|
|----|


- 
#  自动连接


若希望wifi启动后自动连接到预先设置的ap，只需按如下设置脚本即可以，如果预先设置里多个ap则每个网络需要设置优先级参数priority。
|ctrl_interface=/var/run/wpa_supplicantupdate_config=1network={ssid="LZ205"psk="20100208"priority=2}|
|----|










