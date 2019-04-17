# Android网络API(android.net.wifi)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月11日 08:01:53[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1502








提供类以管理设备上的Wifi功能。




Wi-Fi API提供了一种应用程序可以与提供Wi-Fi网络访问的较低级别无线网络通信的方法。 几乎所有来自设备请求者的信息都可用，包括连接的网络的链路速度，IP地址，协商状态等，以及有关其他可用网络的信息。 一些其他API功能包括扫描，添加，保存，终止和启动Wi-Fi连接的功能。





某些API可能需要以下用户权限：


- `[ACCESS_WIFI_STATE](https://developer.android.google.cn/reference/android/Manifest.permission.html#ACCESS_WIFI_STATE)`
- [CHANGE_WIFI_STATE](https://developer.android.google.cn/reference/android/Manifest.permission.html#CHANGE_WIFI_STATE)
- [CHANGE_WIFI_MULTICAST_STATE](https://developer.android.google.cn/reference/android/Manifest.permission.html#CHANGE_WIFI_MULTICAST_STATE)




注意：并非所有Android设备都提供Wi-Fi功能。 如果您的应用程序使用Wi-Fi，请在清单文件中使用<uses-feature>元素声明：



```
<manifest ...>
    <uses-feature android:name="android.hardware.wifi" />
    ...
</manifest>
```





一、类

[ScanResult](https://developer.android.google.cn/reference/android/net/wifi/ScanResult.html)


描述有关检测到的接入点的信息。





[WifiConfiguration](https://developer.android.google.cn/reference/android/net/wifi/WifiConfiguration.html)


表示配置的Wi-Fi网络的类，包括安全配置。





[WifiConfiguration.AuthAlgorithm](https://developer.android.google.cn/reference/android/net/wifi/WifiConfiguration.AuthAlgorithm.html)


认可的IEEE 802.11认证算法。





[WifiConfiguration.GroupCipher](https://developer.android.google.cn/reference/android/net/wifi/WifiConfiguration.GroupCipher.html)


识别组密码。





[WifiConfiguration.KeyMgmt](https://developer.android.google.cn/reference/android/net/wifi/WifiConfiguration.KeyMgmt.html)


认可的关键管理方案。





[WifiConfiguration.PairwiseCipher](https://developer.android.google.cn/reference/android/net/wifi/WifiConfiguration.PairwiseCipher.html)


WPA的识别成对密码。





[WifiConfiguration.Protocol](https://developer.android.google.cn/reference/android/net/wifi/WifiConfiguration.Protocol.html)


认可的安全协议。





[WifiConfiguration.Status](https://developer.android.google.cn/reference/android/net/wifi/WifiConfiguration.Status.html)


网络配置的可能状态。





[WifiEnterpriseConfig](https://developer.android.google.cn/reference/android/net/wifi/WifiEnterpriseConfig.html)


Wifi的企业配置详细信息。





[WifiEnterpriseConfig.Eap](https://developer.android.google.cn/reference/android/net/wifi/WifiEnterpriseConfig.Eap.html)


使用可扩展认证协议方法





[WifiEnterpriseConfig.Phase2](https://developer.android.google.cn/reference/android/net/wifi/WifiEnterpriseConfig.Phase2.html)


使用内部认证方法





[WifiInfo](https://developer.android.google.cn/reference/android/net/wifi/WifiInfo.html)


描述处于活动状态或正在建立的任何Wifi连接的状态。





[WifiManager](https://developer.android.google.cn/reference/android/net/wifi/WifiManager.html)


该类提供用于管理Wi-Fi连接的所有方面的主要API。





[WifiManager.LocalOnlyHotspotCallback](https://developer.android.google.cn/reference/android/net/wifi/WifiManager.LocalOnlyHotspotCallback.html)


回调类用于应用程序接收关于LocalOnlyHotspot状态的更新。





[WifiManager.LocalOnlyHotspotReservation](https://developer.android.google.cn/reference/android/net/wifi/WifiManager.LocalOnlyHotspotReservation.html)


LocalOnlyHotspotReservation包含活动的LocalOnlyHotspot请求的WifiConfiguration。





[WifiManager.MulticastLock](https://developer.android.google.cn/reference/android/net/wifi/WifiManager.MulticastLock.html)


允许应用程序接收Wifi多播数据包。





[WifiManager.WifiLock](https://developer.android.google.cn/reference/android/net/wifi/WifiManager.WifiLock.html)


允许应用程序保持Wifi收音机清醒。





[WifiManager.WpsCallback](https://developer.android.google.cn/reference/android/net/wifi/WifiManager.WpsCallback.html)


在启动WPS操作时回调调用的接口





[WpsInfo](https://developer.android.google.cn/reference/android/net/wifi/WpsInfo.html)


代表Wi-Fi保护设置的类








二、枚举

[SupplicantState](https://developer.android.google.cn/reference/android/net/wifi/SupplicantState.html)


来自wpa_supplicant的defs.h。






