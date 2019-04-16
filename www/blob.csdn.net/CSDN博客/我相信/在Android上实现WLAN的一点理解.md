# 在Android上实现WLAN的一点理解 - 我相信...... - CSDN博客





2011年04月30日 17:38:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：3662








众所周知，Android 是基于Linux内核的，所以直接支持Linux的WLAN接口。WLAN的内核驱动是根据用户在UI上的设定动态加载的，通过ctrl_interface和openssl来实现WLAN的控制和安全性，TCPIP则基于WLAN的标准实现。

WLAN 在Android上的体系构架如下：

Application Framework : base/wifi/java/android/net/wifi 

WifiManager/WifiMonitor/WifiConfiguration …

Service Framework: base/services/java/com/android/server/

WifiService / WifiWatchdogService 

JNI (Java Native Interface)

jni/android_net_wifi_Wifi.cpp

HAL (Hardware Abstract Layer)

hardware/libhardware/wifi/wifi.c 

wpa_supplicant / ctrl_interface 

Kernel driver

在Android手机上打开WIFI的流程如下:

1)Application Framework: WifiManager 

setWifiEnabled()

2)Service: WifiService 

setWifiEnabled()

handleMessage() : MESSAGE_ENABLE_WIFI

3)JNI: android_net_wifi_Wifi.cpp : 

JNINativeMethod:loadDriver 

android_net_wifi_loadDriver()

4)HAL : hardware/libhardware/wifi/wifi.c 

wifi_load_driver()

5)Kernel : 加载内核驱动



WLAN的搜索流程如下：

1）Application Framework: WifiManager 

startScan()

2）Service: 

WifiService::startScan()

WifiNative::scanCommand();

3）JNI: android_net_wifi_Wifi.cpp : 

JNINativeMethod:scanCommand / scanResultsCommand 

android_net_wifi_scanCommand / android_net_wifi_scanResultsCommand 

4）HAL : hardware/libhardware/wifi/wifi.c 

wifi_send_command : SCAN / SCAN_RESULTS

wpa_supplicant/driver_wext.c 

wpa_driver_wext_scan() : SIOCSIWSCAN

5）Kernel : WLAN 驱动处理相关接口的命令

WAPI是我国WLAN的国家标准----- GB 15629.11,并予以802.11i兼容，在Android上的实现只需增加相应的鉴权即可。







