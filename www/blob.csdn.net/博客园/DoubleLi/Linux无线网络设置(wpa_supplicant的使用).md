# Linux无线网络设置(wpa_supplicant的使用) - DoubleLi - 博客园






主机环境：Gentoo Linux 3.1.10

WPA Supplicant工具包可以让您连接到那些使用WPA的AP。因为还只是beta版，所以它的配置方法仍会常常变化——尽管如此，在大部分情况下它已经能很好的工作。

安装上wap_supplicant后可以通过修改/etc/wpa_supplicant/wpa_supplicant.conf来进行配置无线接入点网络

下面是一个配置文件的实例。

- # 请不要修改下面这一行内容，否则将不能正常工作  
- ctrl_interface=/var/run/wpa_supplicant  
- 
- # 确保只有root用户能读取WPA的配置  
- ctrl_interface_group=0  
- 
- # 使用wpa_supplicant来扫描和选择AP  
- ap_scan=1  
- 
- # 简单的情形：WPA-PSk密码验证方式，PSK是ASCII密码短语，所有合法的加密方式都允许连接  
- network={  
- ssid="simple"  
- psk="very secret passphrase"  
- # 优先级越高，就能越早匹配到。  
- priority=5  
- }  
- 
- # 与前面的设置相同，www.linuxidc.com但要求对特定的SSID进行扫描（针对那些拒绝广播SSID的AP）  
- network={  
- ssid="second ssid"  
- scan_ssid=1  
- psk="very secret passphrase"  
- priority=2  
- }  
- 
- # 仅使用WPA-PSK方式。允许使用任何合法的加密方式的组合  
- network={  
- ssid="example"  
- proto=WPA  
- key_mgmt=WPA-PSK  
- pairwise=CCMP TKIP  
- group=CCMP TKIP WEP104 WEP40  
- psk=06b4be19da289f475aa46a33cb793029d4ab3db7a23ee92382eb0106c72ac7bb  
- priority=2  
- }  
- 
- # 明文连接方式（不使用WPA和IEEE802.1X）  
- network={  
- ssid="plaintext-test"  
- key_mgmt=NONE  
- }  
- 
- # 共享WEP秘钥连接方式（不使用WPA和IEEE802.1X）  
- network={  
- ssid="static-wep-test"  
- key_mgmt=NONE  
- wep_key0="abcde"  
- wep_key1=0102030405  
- wep_key2="1234567890123"  
- wep_tx_keyidx=0  
- priority=5  
- }  
- 
- # 共享WEP秘钥连接方式（无WPA和IEEE802.1X），使用共享秘钥IEEE802.11验证方式  
- network={  
- ssid="static-wep-test2"  
- key_mgmt=NONE  
- wep_key0="abcde"  
- wep_key1=0102030405  
- wep_key2="1234567890123"  
- wep_tx_keyidx=0  
- priority=5  
- auth_alg=SHARED  
- }  
- 
- # 在IBSS/ad-hoc网络中使用WPA-None/TKIP  
- network={  
- ssid="test adhoc"  
- mode=1  
- proto=WPA  
- key_mgmt=WPA-NONE  
- pairwise=NONE  
- group=TKIP  
- psk="secret passphrase"  
- }  




--

下面是我的配置文件

- ctrl_interface=/var/run/wpa_supplicant  
- ap_scan=1  
- 
- #Home Network  
- network={  
-     psk="yming0221"  
-     priority=1  
-     ssid=79616E277320776972656C657373  
-     mode=0  
-     bssid=E0:05:C5:17:F8:2C  
-     key_mgmt=WPA-PSK  
- }  
- #  
- network={  
-     ssid="351471azjlb"  
-     psk="CCTV1-CCTV2-KTV-1987"  
-     priority=2  
- }  


然后重启wlan0连接



/etc/init.d/net.wlan0 restart









