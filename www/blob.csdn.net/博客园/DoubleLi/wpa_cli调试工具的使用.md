# wpa_cli调试工具的使用 - DoubleLi - 博客园






1: run wpa_supplicant first

use the following command:

       wpa_supplicant -Dwext -iwlan0 -C/data/system/wpa_supplicant -c/data/misc/wifi/wpa_supplicant.conf

      (use “ps”to make sure wpa_supplicant is running )



2: Run the command line tool wpa_cli to connect wifi

       wpa_cli -p/data/system/wpa_supplicant -iwlan0

       Then , it will let you set network interactively



**some common command:**

       >scan = to scan the neighboring AP

       >scan_results = show the scan results

       >status = check out the current connection information

       >terminate = terminate wpa_supplicant

       >quit = exit wpa_cli

       >add_network = it will return a network id to you

       >set_network <network id> <variable> <value> = set network variables (shows

list of variables when run without arguments), success will return OK, or will return Fail

       >select_network <network id> = select a network (disable others)

       >disable_network <network id> = disable a network

       >enable_network <network id> = enable a network



3: example



**for AP that doesn`t have encryption**

              >add_network      (It will display a network id for you, assume it returns 0)

              >set_network 0 ssid “666”

              >set_network 0 key_mgmt NONE

              >enable_network 0

              >quit

       if normal, we have connectted to the AP “666”, now you need a IP to access internet, for example:

              dhcpcd wlan0

              if everything is ok, it will get an IP & can access internet



**for AP that has WEP**

              >add_network      (assume returns 1)

              >set_network 1 ssid “666”

              >set_network 1 key_mgmt NONE

              >set_network 1 wep_key0 “your ap passwork”(if usting ASCII, it need double quotation marks, if using hex, then don`t need the double quotation marks)

              >set_network 1 wep_tx_keyidx 0

              >select_network 1  (optional, remember, if you are connecting with another AP, you should select it to disable the another)

              >enable_network 1

              and then ,get an IP to access internet



**for AP that has WPA-PSK/WPA2-PSK**

              >add_network      (assume returns 2)

              >set_network 2 ssid “666”

              >set_network 2 psk “your pre-shared key”

              >select_network 2  (optional, remember, if you are connecting with another AP, you should select it to disable the another)

              >enable_network 2

              there is still some others options to be set, but wpa_supplicant will choose the default for you, the default will include all we need to set

              and then ,get an IP to access internet



**       for Hidden AP（补充）**

**        原则上应该只要在上面的基础上去set_network netid scan_ssid 1即可，测试过无加密的Hidden AP，WEP/WPA/WPA2应该道理一样**



**实例操作：**

# 连接成功后的状态

root@Ultrasonic:/usr/local/bin# iwconfig wlan0

wlan0     IEEE 802.11bgn  ESSID:"uslab_testing"  Nickname:"<WIFI@REALTEK>"

          Mode:Managed  Frequency:2.432 GHz  Access Point: 84:C9:B2:88:0C:F2  

          Bit Rate:300 Mb/s   Sensitivity:0/0 

          Retry:off   RTS thr:off   Fragment thr:off

          Encryption key:****-****-****-****-****-****-****-****   Security mode:open

          Power Management:off

          Link Quality=98/100  Signal level=98/100  Noise level=0/100

          Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0

          Tx excessive retries:0  Invalid misc:0   Missed beacon:0



root@Ultrasonic:/usr/local/bin# wpa_cli

wpa_cli v0.7.3

Copyright (c) 2004-2010, Jouni Malinen <j@w1.fi> and contributors



This program is free software. You can distribute it and/or modify it

under the terms of the GNU General Public License version 2.



Alternatively, this software may be distributed under the terms of the

BSD license. See README and COPYING for more details.





Selected interface 'wlan0'



Interactive mode

> scan_results

bssid / frequency / signal level / flags / ssid

84:c9:b2:88:0c:f2 2432 94  [WPA-PSK-TKIP+CCMP][WPA2-PSK-TKIP+CCMP][ESS]    uslab_testing



> status

bssid=84:c9:b2:88:0c:f2

ssid=uslab_testing

id=0

mode=station

pairwise_cipher=CCMP

group_cipher=TKIP

key_mgmt=WPA2-PSK

wpa_state=COMPLETED

ip_address=192.168.0.101

> 

# 连接过程

root@Ultrasonic:/usr/local/bin# wpa_supplicant -B -iwlan0 -Dwext -c /usr/local/etc/wpa_supplicant.conf

+871x_drv - drv_open, bup=0

 ===> FirmwareDownload91C() fw:Rtl819XFwImageArray_TSMC

fw_ver=v75, fw_subver=0, sig=0x88c0

fw download ok!

Set RF Chip ID to RF_6052 and RF type to 2.

GPIO_IN=09

eRfPowerStateToSet=0

IQK:Start!!!

Path A IQK Success!!

Path B IQK Success!!

Path A IQK Success!!

Path B IQK Success!!

IQK: final_candidate is 0

IQK: RegE94=101 RegE9C=2 RegEA4=fa RegEAC=3 RegEB4=101 RegEBC=7 RegEC4=fd RegECC=7

 Path A IQ Calibration Success !

Path B IQ Calibration Success !

pdmpriv->TxPowerTrackControl = 1

MAC Address from REG_MACID = 84:c9:b2:7d:64:2d

MAC Address = 84:c9:b2:7d:64:2d

-871x_drv - drv_open, bup=1

[rtw_wx_set_pmkid] IW_PMKSA_FLUSH!

set_mode = IW_MODE_INFRA

ioctl[SIOCSIWAP]: Operation not permitted

[rtw_wx_set_pmkid] IW_PMKSA_FLUSH!

root@Ultrasonic:/usr/local/bin# wpa_cli

wpa_cli v0.7.3

Copyright (c) 2004-2010, Jouni Malinen <j@w1.fi> and contributors



This program is free software. You can distribute it and/or modify it

under the terms of the GNU General Public License version 2.



Alternatively, this software may be distributed under the terms of the

BSD license. See README and COPYING for more details.





Selected interface 'wlan0'



Interactive mode



> scan

OK

> survey done event(19)

<2>CTRL-EVENT-BSS-ADDED 0 84:c9:b2:88:0c:f2

<2>CTRL-EVENT-BSS-ADDED 1 02:e0:42:cb:c7:79

<2>CTRL-EVENT-SCAN-RESULTS



> scan_results

bssid / frequency / signal level / flags / ssid

84:c9:b2:88:0c:f2       2432    88      [WPA-PSK-TKIP+CCMP][WPA2-PSK-TKIP+CCMP][ESS]   uslab_testing

02:e0:42:cb:c7:79       2457    42      [WEP][IBSS]     ____13__1

>

> add_network

0

>

> set_network 0 ssid "uslab_testing"

OK

>

> set_network 0 key_mgmt WPA-PSK

OK

>

> set_network 0 psk "12345678"

OK

>

> enable_network 0

OK

> survey done event(19)

wpa_set_auth_algs, AUTH_ALG_OPEN_SYSTEM

set_mode = IW_MODE_INFRA



 wpa_ie(length:22):

0x30 0x14 0x01 0x00 0x00 0x0f 0xac 0x02

0x01 0x00 0x00 0x0f 0xac 0x04 0x01 0x00

0x00 0x0f 0xac 0x02 0x00 0x00 0x78 0xff

=>rtw_wx_set_essid

ssid=uslab_testing, len=13

new candidate: uslab_testing(84:c9:b2:88:0c:f2) rssi:-54

rtw_select_and_join_from_scanned_queue: candidate: uslab_testing(84:c9:b2:88:0c:f2)

#### Opt_Ant_(B) , cur_Ant(B)

rtw_restructure_ht_ie IEEE80211_HT_CAP_MAX_AMSDU is set

<=rtw_wx_set_essid

link to new AP



<2>CTRL-EVENT-SCAN-RESULTS

<2>Trying to associate with 84:c9:b2:88:0c:f2 (SSID='uslab_testing' freq=2432 MHz)

> link to Realtek 96B

OnAuthClient

OnAssocRsp

report_join_res(1)

HW_VAR_BASIC_RATE: BrateCfg(0x15d)

WMM(0): 0, a42b

WMM(1): 0, a44f

WMM(2): 0, 5e4322

WMM(3): 0, 2f3222

HTOnAssocRsp



 ~~~~stastakey:unicastkey



 ~~~~stastakey:groupkey

==> rtw_set_key algorithm(2),keyid(1),key_mask(2)

update raid entry, mask=0xfffffff, arg=0xa0

rtl8192c_set_FwJoinBssReport_cmd mstatus(1)

SetFwRsvdPagePkt

Set RSVD page location to Fw.

=>mlmeext_joinbss_event_callback

<2>Associated with 84:c9:b2:88:0c:f2

<2>WPA: Key negotiation completed with 84:c9:b2:88:0c:f2 [PTK=CCMP GTK=TKIP]

<2>CTRL-EVENT-CONNECTED - Connection to 84:c9:b2:88:0c:f2 completed (auth) [id=0 id_str=]



> save

OK

> status

bssid=84:c9:b2:88:0c:f2

ssid=uslab_testing

id=0

mode=station

pairwise_cipher=CCMP

group_cipher=TKIP

key_mgmt=WPA2-PSK

wpa_state=COMPLETED

ip_address=192.168.0.101

> 

> quit

root@Ultrasonic:/usr/local/bin# udhcpc -i wlan0

udhcpc (v0.9.9-pre) started

Sending discover...

OnAction_back

OnAction_back, action=0

issue_action_BA, category=3, action=1, status=0

Sending select for 192.168.0.101...

Lease of 192.168.0.101 obtained, lease time 86400

adding dns 192.168.0.101

adding dns 192.168.0.102

root@Ultrasonic:/usr/local/bin#

root@Ultrasonic:/usr/local/bin# iwconfig wlan0

wlan0     IEEE 802.11bgn  ESSID:"uslab_testing"  Nickname:"<WIFI@REALTEK>"

          Mode:Managed  Frequency:2.432 GHz  Access Point: 84:C9:B2:88:0C:F2  

          Bit Rate:300 Mb/s   Sensitivity:0/0 

          Retry:off   RTS thr:off   Fragment thr:off

          Encryption key:****-****-****-****-****-****-****-****   Security mode:open

          Power Management:off

          Link Quality=100/100  Signal level=100/100  Noise level=0/100

          Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0

          Tx excessive retries:0  Invalid misc:0   Missed beacon:0



root@Ultrasonic:/usr/local/bin# ./wpa_cli

wpa_cli v0.7.3

Copyright (c) 2004-2010, Jouni Malinen <j@w1.fi> and contributors



This program is free software. You can distribute it and/or modify it

under the terms of the GNU General Public License version 2.



Alternatively, this software may be distributed under the terms of the

BSD license. See README and COPYING for more details.





Selected interface 'wlan0'



Interactive mode



> disable_network 0

OK

<2>CTRL-EVENT-DISCONNECTED bssid=00:00:00:00:00:00 reason=0

> rtl8192c_set_FwJoinBssReport_cmd mstatus(0)

<2>CTRL-EVENT-BSS-REMOVED 0 84:c9:b2:88:0c:f2



 > status

wpa_state=DISCONNECTED

ip_address=192.168.0.101

> terminate

OK

> (2)871x_drv - drv_close, bup=1, hw_init_completed=1

-871x_drv - drv_close, bup=1

rtl8192c_set_FwJoinBssReport_cmd mstatus(0)

'PING' command failed.

Connection to wpa_supplicant lost - trying to reconnect



> quit

root@Ultrasonic:/usr/local/bin# ps

  PID  Uid     VmSize Stat Command

    1 root        528 S   init [3]  

    2 root            SW< [kthreadd]

    3 root            SW< [ksoftirqd/0]

    4 root            SW< [events/0]

    5 root            SW< [khelper]

   57 root            SW< [kblockd/0]

   63 root            SW< [ksuspend_usbd]

   68 root            SW< [khubd]

   71 root            SW< [kseriod]

   77 root            SW< [kmmcd]

  104 root            SW  [pdflush]

  105 root            SW  [pdflush]

  106 root            SW< [kswapd0]

  107 root            SW< [aio/0]

  215 root            SW< [kapmd]

  262 root            SW< [mtdblockd]

  332 root            SW< [w1_control]

  346 root            SW< [rpciod/0]

  424 root        388 S < /sbin/udevd --daemon

  445 root        584 S   /sbin/syslogd -n -C 64 -m 20

  447 root        564 S   /sbin/klogd -n

  470 root        928 S   -sh

  478 root            SW  [RTKTHREAD]

  485 root        764 R   ps

root@Ultrasonic:/usr/local/bin#









