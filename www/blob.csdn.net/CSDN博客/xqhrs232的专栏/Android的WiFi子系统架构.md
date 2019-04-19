# Android的WiFi子系统架构 - xqhrs232的专栏 - CSDN博客
2012年01月04日 14:23:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：938标签：[android																[frameworks																[command																[jni																[java																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=frameworks&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://hi.baidu.com/wuguohu/blog/item/6b518726d6ffda018b82a176.html](http://hi.baidu.com/wuguohu/blog/item/6b518726d6ffda018b82a176.html)
**一、WiFi基本框架**
Android的WiFi系统自上而下包括如下一些内容：
![wifi1.png](http://hi.csdn.net/attachment/201103/16/0_1300279343MwyV.gif)
WiFi部分在Android系统中作为网络部分使用的方法和通常的网络相同。唯一特殊的部分是在Settings程序中的WiFi相关设置内容，它们调用了WiFi提供了WiFi架构提供的Java层接口。
**二、WiFi本地实现**
Android的WiFi本地部分实现主要包括wpa_spplicant以及wpa_supplicant适配层。
WPA是WiFi Protected Access的缩写，中文含义为“WiFi网络安全存取”。WPA是一种基于标准的可互操作的WLAN安全性增强解决方案，可大大增强现有以及未来无线局域网络的数据保护和访问控制水平。
1、  wpa_supplicant
wpa_supplicant是一个开源项目，已经被移植到Linux，Windows以及很多嵌入式系统上。它是WPA的应用层认证客户端，负责完成认证相关的登录、加密等工作。wpa_supplicant的源代码目录为：
.\external\wpa_supplicant\
.\external\wpa_supplicant_6\
这个工程中的内容编译后主要结果是生成动态库libwpa_client.so和可执行程序wpa_supplicant。
wpa_supplicant是一个独立运行的守护进程，其核心是一个消息循环，在消息循环中处理WPA状态机、控制命令、驱动事件、配置信息等。wpa_supplicant有很多控制接口，也提供命令行和通行界面的控制模式：而Android与wpa_supplicant的通信通过Socket完成。
关于wpa_supplicant更多的信息，请参考: [http://w1.fi/wpa_supplicant/](http://w1.fi/wpa_supplicant/)
libwpa_client.so是一个给客户单链接和调用的库，用于和wpa_supplicant守护进程进行通信。
2、  wpa_supplicant适配层
wpa_supplicant适配层是通用的wpa_supplicant的封装，在Android中作为WiFi部分的硬件抽象层来使用。wpa_supplicant适配层主要用于与wpa_supplicant守护进程的通信，以提供给Android框架使用，它实现了加载、控制和消息监控等功能。
wpa_supplicant适配层的源代码路径是：
.\hardware\libhardware_legacy\include\hardware_legacy\wifi.h
.\hardware\libhardware_legacy\wifi\
wpa_supplicant适配层是libhardware_legacy.so的一部分，它需要包含wpa_supplicant的头文件wpa_ctrl.h，动态链接库libwpa_client.so。通过wifi.h文件为上一层提供程序调用接口（实际上就是WiFi的JNI部分）。
wpa_supplicant适配层的构成非常的简单，除了一些加载和连接的接口，它最重要的部分是以下两个接口：
int wifi_command(const char *command, char *reply, size_t *reply_len);
int wifi_wait_for_event(char *buf, size_t len);
wifi_command()提供讲明亮发送到WiFi系统下层的功能，wifi_wait_for_event()负责事件进入的通道，这个函数将被阻塞，直到收到一个WiFi事件，并以字符串的形式返回。
在wifi.c的实现中：
1）、wifi_command()是wifi_send_command()的封装, wifi_send_command()通过wpa_ctrl_request()直接命令转发给wpa_supplicant进程，并返回结果。
2）、wifi_wait_for_event()仅仅调用wpa_ctrl_recv()来接受上一次wpa_supplicant上报的事件，如果没有事件则阻塞于此，上层会通过循环由此来读取每一个上报的事件。
本层还提供一些一些与DHCP（动态主机分配协议）相关的操作。
**三、WiFi的JNI层**
Android中的WiFi系统的JNI部分实现的源码文件为：
.\frameworks\base\core\jni\android_net_wifi_Wifi.cpp
这里实现的本地函数，都是通过调用wpa_supplicant适配层的接口来实现的（包含适配层的头文件wifi.h）。
JNI层的接口注册到Java层的源代码文件为：
.\frameworks\base\wifi\java\android\net\wifi\WifiNative.java
WifiNative将为WifiService、WifiStateTracker、WifiMonitor等几个WiFi框架内部组件提供底层操作支持。
**四、WiFi的Java FrameWork层**
WiFi系统的Java部分代码实现的目录为：
.\frameworks\base\wifi\java\android\net\wifi\          // WiFi服务层的内容
.\ frameworks\base\services\java\com\android\server\   // WiFi部分的接口
WiFi系统Java层的核心是根据IWifiManger接口所创建的Binder服务器端和客户端，服务器端是WifiService，客户端是WifiManger。
IWifiManger.aidl编译后生成了IWifiManger.java，并生成IWifiManger.Stub（服务器端抽象类）和IWifiManger.Stub.Proxy（客户端代理实现类）。WifiService通过继承IWifiManger.Stub实现，而客户端通过getService()函数获取IWifiManger.Stub.Proxy（即Service的代理类），将其作为参数传递给WifiManger，供其与WifiService通信时使用。
![wifi2.png](http://hi.csdn.net/attachment/201103/16/0_1300279350yscZ.gif)
1．    WiFiManger是WiFi部分与外界的接口，用户通过它来访问WiFi的核心功能。WifiWatchdogService这一系统组件也是用WifiManger来执行一些具体操作。
2．    WifiService是服务器端的实现，作为WiFi部分的核心，处理实际的驱动加载、扫描、链接、断开等命令，已经底层上报的事件。对于主动的命令控制，WiFi是一个简单的封装，针对来自客户端的控制命令，调用相应的WifiNative底层实现。
一般接收到客户端的命令后，将其转换成对应的自身消息塞入消息队列中，以便客户端的调用可以及时返回，然后在WifiHandler的handleMessage()中处理对应的消息。而底层上报的事件，WifiService则通过启动WifiStateTracker来负责处理。
1）  WifiStateTracker除了负责WiFi的电源管理模式等功能外，其核心是WifiMonitor所实现的事件轮询机制，以及消息处理函数handleMessage()。
2）  WifiMonitor通过开启一个MonitorThread来实现事件的轮询，轮询的关键函数是前面提到的 阻塞式函数WifiNative.waitForEvent()。获取事件后，WifiMonitor通过一系列的Handler通知给WifiStateTracker。这里WifiMonitor的通知机制是将底层事件转换成WifiStateTracker所能识别的消息，塞入WifiStateTracker的消息循环中，最终在handleMessage()中由WifiStateTracker完成对应的处理。
注：WifiStateTracker同样是WiFi部分与外界的接口，它不像WifiManger那样直接被实例化来操作，而是通过Intent机制来发消息通知给客户端注册的BroadcastReceiver，以完成和客户端的接口。
3．    WifiWatchdogService是ConnectivityService所启动的服务，但它并不是通过Binder来实现的服务。它的作用是监控同一个网络内的接入点（Access Point），如果当前接入点的DNS无法ping通，就自动切换到下一个接入点。WifiWatchdogService通过WifiManger和WifiStateTracker辅助完成具体的控制动作。WifiWatchdogService初始化时，通过registerForWifiBroadcasts注册获取网络变化的BroadcastReceiver，也就是捕获WifiStateTracker所发出的通知消息，并开启一个WifiWatchdogThread线程来处理获取的消息。通过更改Setting.Secure.WIFI_WARCHDOG_ON的配置，可以开启和关闭WifiWatchdogService。
**五、Setting中的WiFi设置**
Android的Settings应用程序对WIFI的使用，是典型的WiFi应用方式，也是用户可见的Android WiFi管理程序。这部分源代码的目录为：
. \packages\apps\Settings\src\com\android\settings\wifi\
Setting里的WiFi部分是用户可见的设置界面，提供WiFi开关、扫描AP、链接/断开的基本功能。另外i，通过实现WifiLayer.Callback接口提供了一组回调函数，用以相应用户关心的WiFi状态的变化。
WifiEnabler和WifiLayer都是WifiSettings的组成部分，同样通过WifiManger来完成实际的功能，也同样注册一个BroadcastReceiver来响应WifiStateTracker所发出的通知消息。WifiEnabler其实是一个比较简单的类，提供开启和关闭WiFi的功能，设置里面的外层WiFi开关菜单，就是直接通过它来做到的；而WifiLayer则提供更复杂的一些WiFi功能，如AP选择等以供用户自定义。
