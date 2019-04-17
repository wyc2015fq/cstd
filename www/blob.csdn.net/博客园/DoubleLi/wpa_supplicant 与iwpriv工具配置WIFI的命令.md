# wpa_supplicant 与iwpriv工具配置WIFI的命令 - DoubleLi - 博客园






=====================================================
hostapd 配置命令
=====================================================
/system/bin/hostapd -e/data/misc/wifi/entropy.bin /data/misc/wifi/hostapd.conf &

Android:
ndc softap set wlan0 henryking broadcast 6 wpa-psk 11112222
busybox ifconfig wlan0 up
ndc softap startap
ndc tether start "192.168.43.2" "192.168.43.254"
ndc busybox ifconfig wlan0 192.168.43.1 netmask 255.255.255.0
=====================================================
=====================================================
wpa_supplicant -iwlan0 -Dwext -c/system/etc/wifi/wpa_supplicant.conf -dd -B
wpa_supplicant -iwlan0 -Dnl80211 -c/system/etc/wifi/wpa_supplicant.conf  -B


wpa_supplicant -iwlan0 -Dwext -c/data/misc/wifi/wpa_supplicant.conf -dd -B
wpa_supplicant -iwlan0 -Dnl80211 -c/data/misc/wifi/wpa_supplicant.conf -dd -B
=====================================================
WAPI ASCII/HEX:
set_network 0 ssid "WAPIPSK"
set_network 0 auth_alg OPEN
set_network 0 proto WAPI
set_network 0 key_mgmt WAPI-PSK
set_network 0 psk_key_type 0  //set_network 0 psk_key_type 1
set_network 0 psk "11111111"  //set_network 0 psk "3131313131313131"
WAPI CERT:
//设置锁屏
set_network 0 ssid "WAPICERT"
set_network 0 auth_alg OPEN
set_network 0 proto WAPI
set_network 0 key_mgmt WAPI-CERT
set_network 0 wapi_as_cert "/data/as.cer"
set_network 0 wapi_user_cert "/data/ASUE.cer"
=====================================================
wpa_cli -iwlan0 scan
wpa_cli -iwlan0 scan_results
wpa_cli -iwlan0 remove_network 0
wpa_cli -iwlan0 add_network
wpa_cli -iwlan0 set_network 0 ssid '"henryking"'
wpa_cli -iwlan0 set_network 0 key_mgmt WPA-PSK
wpa_cli -iwlan0 set_network 0 psk '"12345678"'

wpa_cli -iwlan0 set_network 0 key_mgmt NONE

wpa_cli -iwlan0 set_network 0 wep_key0 123456
wpa_cli -iwlan0 set_network 0 wep_tx_keyidx 0

wpa_cli -iwlan0 select_network 0
wpa_cli -iwlan0 save_config
wpa_cli -iwlan0 list_networks
wpa_cli -iwlan0 add_network;wpa_cli -iwlan0 set_network 0 ssid '"82883008"';wpa_cli -iwlan0 set_network 0 psk '"12345678"';wpa_cli -iwlan0 select_network 0
=====================================================
wpa_cli -irausb0 scan
wpa_cli -irausb0 scan_results
wpa_cli -irausb0 add_network
wpa_cli -irausb0 set_network 0 ssid '"henry"'
wpa_cli -irausb0 set_network 0 psk '"12345678"'
wpa_cli -irausb0 set_network 0 key_mgmt WPA-PSK
//wpa_cli -irausb0 set_network 0 key_mgmt NONE
wpa_cli -irausb0 select_network 0
wpa_cli -irausb0 enable_network 0
wpa_cli -irausb0 ap_scan '"1"'
wpa_cli -irausb0 save_config
=====================================================

=====================================================
iwpriv 配置网络
=====================================================
WPA/WPA2:
iwpriv rausb0 set NetworkType=Infra
iwpriv rausb0 set AuthMode=WPA2PSK
iwpriv rausb0 set EncrypType=AES
iwpriv rausb0 set SSID="sabre"
iwpriv rausb0 set WPAPSK="12345678"
iwpriv rausb0 set NetworkType=Infra;iwpriv rausb0 set AuthMode=WPA2PSK;iwpriv rausb0 set EncrypType=AES;iwpriv rausb0 set WPAPSK="12345678";iwpriv rausb0 set SSID="henryking";
iwpriv rausb0 set NetworkType=Infra;iwpriv rausb0 set AuthMode=WPAPSK;iwpriv rausb0 set EncrypType=AES;iwpriv rausb0 set SSID="82883008";iwpriv rausb0 set WPAPSK="12345678"
iwpriv rausb0 set NetworkType=Infra;iwpriv wlan0 set AuthMode=WPAPSK;iwpriv wlan0 set EncrypType=AES;iwpriv wlan0 set WPAPSK="12345678";iwpriv wlan0 set SSID="henryking";
=====================================================
WEP:
iwpriv rausb0 set AuthMode=SHARED
iwpriv rausb0 set EncrypType=WEP
iwpriv rausb0 set DefaultKeyID=1
iwpriv rausb0 set Key1="1111222233"
iwpriv rausb0 set SSID="sabre"

iwconfig wlan0 essid "sabre"
iwconfig wlan0 key 1111222233
=====================================================

=====================================================
wpa_supplicant.conf
=====================================================
linux下格式：
=====================================================
update_config=1
ctrl_interface=/var/run/wpa_supplicant
network={
        ssid="henry"
        psk="12345678"
        key_mgmt=WPA-PSK
}
=====================================================
Android下格式：
=====================================================
update_config=1
ctrl_interface=DIR=/data/system/wpa_supplicant GROUP=wifi
eapol_version=1
ap_scan=1
fast_reauth=1
=====================================================









