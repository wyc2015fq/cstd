# 不同品牌BREW手机间的应用移植 - 我相信...... - CSDN博客





2011年03月18日 20:34:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1145








            针对手机的硬件差异性，为了减少应用移植中的工作，可以在应用中采用自适应的方法。每个手机都有各自的类型标识——平台标识，如果应用自身根据平台标识以及相应硬件信息动态加载相应的资源，无疑可以实现某些应用的零工作量移植。获得手机平台标识和相关硬件信息的方式之一是使用ISHELL_GetDeviceInfo()和ISHELL_GetDeviceInfoEx()。

            ISHELL_GetDeviceInfo()可以在 AEE层中查询有关手机性能的信息，例如所支持的 RAM 总量、屏幕显示等其它项目。要获取 AEEDeviceInfo结构中的 dwNetLinger 和 dwSleepDefer值，在调用此函数之前，wStructSize 必须等于sizeof(AEEDeviceInfo)。

            ISHELL_GetDeviceInfoEx()用于获取有关设备的特定信息（AEEDeviceItem）。此函数使用 ID 指定所需的信息，返回时在缓冲区中包含对应的信息。这些ID所表示的含义如表15-2所示。

ID的名称

 含义说明


AEE_DEVICEITEM_CHIP_ID

 标识芯片组 ID 的字符串


AEE_DEVICEITEM_MOBILE_ID

 标识手持设备的移动 ID 的一个 ASCII 字符串


AEE_DEVICEITEM_AMR_VOCODER_SUPPORT

 是否支持 AMR (Adaptive Multi-Rate) 声码器


AEE_DEVICEITEM_EVRC_VOCODER_SUPPORT

 是否支持 EVRC（增强型变速率编码）声码器


AEE_DEVICEITEM_IS96_VOCODER_SUPPORT

 否支持 QCELP-IS96(8K)（Qualcomm 码激励线性预测编码）声码器


AEE_DEVICEITEM_IS96A_VOCODER_SUPPORT

 否支持 QCELP-IS96A(8K)（Qualcomm 码激励线性预测编码）声码器


AEE_DEVICEITEM_IS733_VOCODER_SUPPORT

 是否支持 QCELP-IS733（Qualcomm 码激励线性预测编码）声码器


AEE_DEVICEITEM_SMV_VOCODER_SUPPORT

 是否支持可选择模式的声码器


AEE_DEVICEITEM_SYS_COLORS_DISP1 AEE_DEVICEITEM_SYS_COLORS_DISP2 AEE_DEVICEITEM_SYS_COLORS_DISP3 AEE_DEVICEITEM_SYS_COLORS_DISP4

 返回与特定显示屏幕关联的系统颜色列表


AEE_DEVICEITEM_DISPINFO1 AEE_DEVICEITEM_DISPINFO2 AEE_DEVICEITEM_DISPINFO3 AEE_DEVICEITEM_DISPINFO4

 返回特定显示屏幕的相关信息


AEE_DEVICEITEM_CARRIER_ID

 标识手持设备的运营商 ID 的一个 uint32 值


AEE_DEVICEITEM_SIDHASH

 返回手持设备的支付帐户的一个散列。 可用于确定是否已更换了支付帐户，例如在调换了 RUIM 卡时。 请注意，不会返回具体的帐号，而只是返回一个帐户字符串的散列。


AEE_DEVICESTATE_KEYS_PRESSED

 返回处于按下状态的按键的值


AEE_DEVICESTATE_FLIP_OPEN

 在设备翻盖处于打开状态时返回 TRUE，否则返回 FALSE


AEE_DEVICESTATE_HEADPHONE_ON

 如果设备耳机是打开的，返回 TRUE，否则返回 FALSE


AEE_DEVICESTATE_KEYGUARD_ON

 如果设备的键盘是打开，将返回 TRUE，否则返回 FALSE


AEE_DEVICESTATE_SPEAKER_ON

 如果设备的扬声器为打开则返回 TRUE，否则返回 FALSE


AEE_DEVICEITEM_USER

 OEM 定义项目的起始点


表 15-2：可以用ISHELL_GetDeviceInfoEx()获取的有关设备的特定信息

根据AEEDeviceInfo和AEEDeviceItem中的信息，应用可以做到动态加载资源文件。当然这些资源信息也可以保存在服务器上，应用程序通过网络通信的方式获得这些资源数据，从而完成一个应用在不同手机上的适配。

如果某些手机不支持某些接口,应用程序可以通过判断该接口是否创建成功获得该信息。例如在内存没有问题的情况下，创建IPosDet接口失败，很可能是由于该手机不支持定位功能造成的。这时，应用程序就应该不提供定位功能，从而成为只具备浏览查询功能的一个LBS应用。

如果是两台手机对同一接口的功能存在差异（一般出现在IRingerMgr或者IAddrBook），这时需要应用程序多做一些工作。例如采用一个配置文件来定义地址簿或者铃声目录中的域，应用程序在启动时根据手机信息来读取配置文件，这样就能够进一步弥补手机间的差异进而很好的实现移植了。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/11/02/5981424.aspx](http://blog.csdn.net/wireless_com/archive/2010/11/02/5981424.aspx)



