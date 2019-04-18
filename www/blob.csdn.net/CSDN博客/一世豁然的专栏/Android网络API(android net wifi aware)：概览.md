# Android网络API(android.net.wifi.aware)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月13日 09:30:28[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1647








提供允许应用程序使用Wi-Fi Aware发现对等体并创建与他们的连接的类。




使用Wi-Fi Aware API，应用程序可以广告服务，发现广告服务的对等体，并连接到它们。 Wi-Fi Aware独立于Wi-Fi基础设施（即，设备可能与AP可能并未与Wi-Fi意识并发）。





Wiener Fiware的主要功能是WifiAwareManager类，它通过调用Context.getSystemService（Context.WIFI_AWARE_SERVICE）来获取，





某些API可能需要以下用户权限：



- `[ACCESS_WIFI_STATE](https://developer.android.google.cn/reference/android/Manifest.permission.html#ACCESS_WIFI_STATE)`
- `[CHANGE_WIFI_STATE](https://developer.android.google.cn/reference/android/Manifest.permission.html#CHANGE_WIFI_STATE)`
- `[ACCESS_COARSE_LOCATION](https://developer.android.google.cn/reference/android/Manifest.permission.html#ACCESS_COARSE_LOCATION)`

注意：并非所有Android设备都支持Wi-Fi Aware功能。 如果您的应用程序仅适用于Wi-Fi Aware（即仅应安装在支持Wi-Fi Aware的设备上），请使用清单文件中的<uses-feature>元素声明：





```
<manifest ...>
    <uses-feature android:name="android.hardware.wifi.aware" />
    ...
</manifest>
```


或者，如果您的应用程序不需要Wi-Fi Aware，但可以利用它，如果可用，您可以使用带有FEATURE_WIFI_AWARE的hasSystemFeature（String）在运行时执行检查：



```java
getPackageManager().hasSystemFeature(PackageManager.FEATURE_WIFI_AWARE)
```



一、类

[AttachCallback](https://developer.android.google.cn/reference/android/net/wifi/aware/AttachCallback.html)

Aware的基类附加回调。





[Characteristics](https://developer.android.google.cn/reference/android/net/wifi/aware/Characteristics.html)


Wi-Fi Aware实现的特点。





[DiscoverySession](https://developer.android.google.cn/reference/android/net/wifi/aware/DiscoverySession.html)


表示单一发布或订阅Aware会话的类。





[DiscoverySessionCallback](https://developer.android.google.cn/reference/android/net/wifi/aware/DiscoverySessionCallback.html)


Aware会话事件回调的基类。





[IdentityChangedListener](https://developer.android.google.cn/reference/android/net/wifi/aware/IdentityChangedListener.html)


监听器的基类，只要改变了Aware接口的MAC地址即可调用。





[PeerHandle](https://developer.android.google.cn/reference/android/net/wifi/aware/PeerHandle.html)


用于表示Wi-Fi Aware对等体的不透明对象。





[PublishConfig](https://developer.android.google.cn/reference/android/net/wifi/aware/PublishConfig.html)


定义Aware发布会话的配置。





[PublishConfig.Builder](https://developer.android.google.cn/reference/android/net/wifi/aware/PublishConfig.Builder.html)


Builder用于构建PublishConfig对象。





[PublishDiscoverySession](https://developer.android.google.cn/reference/android/net/wifi/aware/PublishDiscoverySession.html)


表示Aware发布会话的类。





[SubscribeConfig](https://developer.android.google.cn/reference/android/net/wifi/aware/SubscribeConfig.html)


定义Aware订阅会话的配置。





[SubscribeConfig.Builder](https://developer.android.google.cn/reference/android/net/wifi/aware/SubscribeConfig.Builder.html)


Builder用于构建SubscribeConfig对象。




[SubscribeDiscoverySession](https://developer.android.google.cn/reference/android/net/wifi/aware/SubscribeDiscoverySession.html)


代表Aware订阅会话的类。





[WifiAwareManager](https://developer.android.google.cn/reference/android/net/wifi/aware/WifiAwareManager.html)


该类提供用于管理Wi-Fi Aware操作的主要API：发现和对等数据连接。





[WifiAwareSession](https://developer.android.google.cn/reference/android/net/wifi/aware/WifiAwareSession.html)


此类代表Wi-Fi Aware会话 - Wi-Fi Aware服务的附件，通过该服务应用程序可以执行发现操作。




