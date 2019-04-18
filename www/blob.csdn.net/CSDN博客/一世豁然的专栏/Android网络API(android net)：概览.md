# Android网络API(android.net)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月10日 19:31:50[一世豁然](https://me.csdn.net/Explorer_day)阅读数：615








一、接口

[ConnectivityManager.OnNetworkActiveListener](https://developer.android.google.cn/reference/android/net/ConnectivityManager.OnNetworkActiveListener.html)


回调与addDefaultNetworkActiveListener（ConnectivityManager.OnNetworkActiveListener）一起使用，以了解系统默认网络何时进入高功率状态。





[UrlQuerySanitizer.ValueSanitizer](https://developer.android.google.cn/reference/android/net/UrlQuerySanitizer.ValueSanitizer.html)


用于清理单个查询值的函子。








二、类

[CaptivePortal](https://developer.android.google.cn/reference/android/net/CaptivePortal.html)


一个允许应用程序处理ACTION_CAPTIVE_PORTAL_SIGN_IN活动的类可以向系统指示强制门户登录的不同结果。





[ConnectivityManager](https://developer.android.google.cn/reference/android/net/ConnectivityManager.html)


回答关于网络连接状态的查询的类。





[ConnectivityManager.NetworkCallback](https://developer.android.google.cn/reference/android/net/ConnectivityManager.NetworkCallback.html)


NetworkRequest回调的基类。





[Credentials](https://developer.android.google.cn/reference/android/net/Credentials.html)


用于表示通过UNIX域套接字上的辅助数据传递的UNIX凭据的类。





[DhcpInfo](https://developer.android.google.cn/reference/android/net/DhcpInfo.html)


一个用于检索DHCP请求结果的简单对象。





[IpPrefix](https://developer.android.google.cn/reference/android/net/IpPrefix.html)


该类表示IP前缀，即在两个边界的功率（也称为“IP子网”）对齐的IP地址的连续块。





[LinkAddress](https://developer.android.google.cn/reference/android/net/LinkAddress.html)


标识网络链路上的IP地址。





[LinkProperties](https://developer.android.google.cn/reference/android/net/LinkProperties.html)


描述网络链接的属性。





[LocalServerSocket](https://developer.android.google.cn/reference/android/net/LocalServerSocket.html)


在Linux抽象命名空间中创建入站UNIX域套接字的非标准类。





[LocalSocket](https://developer.android.google.cn/reference/android/net/LocalSocket.html)


在UNIX-domain命名空间中创建（非服务器）套接字。





[LocalSocketAddress](https://developer.android.google.cn/reference/android/net/LocalSocketAddress.html)


UNIX域（AF_LOCAL）套接字地址。





[MailTo](https://developer.android.google.cn/reference/android/net/MailTo.html)


MailTo URL解析器此类解析mailto方案URL，然后可以查询解析的参数。





[Network](https://developer.android.google.cn/reference/android/net/Network.html)


识别网络。





[NetworkCapabilities](https://developer.android.google.cn/reference/android/net/NetworkCapabilities.html)


此类表示网络的功能。





[NetworkInfo](https://developer.android.google.cn/reference/android/net/NetworkInfo.html)


描述网络接口的状态。





[NetworkRequest](https://developer.android.google.cn/reference/android/net/NetworkRequest.html)


通过NetworkRequest.Builder定义网络请求，用于通过requestNetwork（NetworkRequest，PendingIntent）请求网络或通过registerNetworkCallback（NetworkRequest，PendingIntent）监听更改。





[NetworkRequest.Builder](https://developer.android.google.cn/reference/android/net/NetworkRequest.Builder.html)


Builder用于创建NetworkRequest对象。





[NetworkSpecifier](https://developer.android.google.cn/reference/android/net/NetworkSpecifier.html)


描述用于NetworkRequest的网络的特定属性。





[Proxy](https://developer.android.google.cn/reference/android/net/Proxy.html)


用于访问用户和默认代理设置的便利类。





[ProxyInfo](https://developer.android.google.cn/reference/android/net/ProxyInfo.html)


描述代理配置。





[RouteInfo](https://developer.android.google.cn/reference/android/net/RouteInfo.html)


代表网络路由。





[SSLCertificateSocketFactory](https://developer.android.google.cn/reference/android/net/SSLCertificateSocketFactory.html)


SSLSocketFactory实现带有几个额外的功能：


1、SSL握手操作的超时规范

2、大多数情况下，主机名验证（请参阅下面的警告）

3、可选SSL会话缓存与SSLSessionCache

4、可选地绕过所有SSL证书检查

握手超时不适用于实际的TCP套接字连接。





[SSLSessionCache](https://developer.android.google.cn/reference/android/net/SSLSessionCache.html)


已建立SSL会话的基于文件的缓存。





[TrafficStats](https://developer.android.google.cn/reference/android/net/TrafficStats.html)


提供网络流量统计的类





[Uri](https://developer.android.google.cn/reference/android/net/Uri.html)


不可变URI引用。





[Uri.Builder](https://developer.android.google.cn/reference/android/net/Uri.Builder.html)


用于构建或操作URI引用的Helper类。





[UrlQuerySanitizer](https://developer.android.google.cn/reference/android/net/UrlQuerySanitizer.html)


消除URL的“查询”部分。





[UrlQuerySanitizer.IllegalCharacterValueSanitizer](https://developer.android.google.cn/reference/android/net/UrlQuerySanitizer.IllegalCharacterValueSanitizer.html)


根据它们包含的字符来消除值。





[UrlQuerySanitizer.ParameterValuePair](https://developer.android.google.cn/reference/android/net/UrlQuerySanitizer.ParameterValuePair.html)


一个简单的元组，保存参数值对。





[VpnService](https://developer.android.google.cn/reference/android/net/VpnService.html)


VpnService是应用程序扩展和构建自己的VPN解决方案的基础类。





[VpnService.Builder](https://developer.android.google.cn/reference/android/net/VpnService.Builder.html)


Helper类创建一个VPN接口。








三、枚举

[LocalSocketAddress.Namespace](https://developer.android.google.cn/reference/android/net/LocalSocketAddress.Namespace.html)


该地址存在的命名空间。





[NetworkInfo.DetailedState](https://developer.android.google.cn/reference/android/net/NetworkInfo.DetailedState.html)


网络连接的细粒度状态。





[NetworkInfo.State](https://developer.android.google.cn/reference/android/net/NetworkInfo.State.html)


粗粒度网络状态。








四、异常

[ParseException](https://developer.android.google.cn/reference/android/net/ParseException.html)


解析URL时抛出失败。




