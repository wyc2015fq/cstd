# 树莓派 安装  刷Android Things 小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月02日 15:41:22[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：433
**一句话说，Android Things就是让开发者可以使用Android开发工具开发嵌入式设备。**
**If you can build an app, you can build a device.**
**只要你会开发APP，你就能开发智能设备。**
谷歌2016年12月发布的Android Things是一个面向物联网的系统，让android开发者用原来开发app的方式开发硬件相关的应用，扩展了android开发的方向和前景，而谷歌的Android Things已经支持了主流的几个面向互联网的几个芯片(**Intel® Edison，NXP Pico i.MX6UL，Raspberry Pi 3**）**。**鉴于开发难度和集成度高低的选择，选择了树莓派作（Raspberry Pi 3）作为实践的。
Android Things 是Android系统的一个精简版, 前身是 Brillo -Google 在 2015 年推出的物联网操作系统。为了节约那一点点硬件成本，还要增加开发的成本，值得么？
**特性：**
 单一自启动APP，通常设备会只运行单一的程序，即开发者的APP会自动启动，系统APP不会呈现出来。
 显示是可选的，设备可以没有屏幕，通过按钮、触摸、LED灯、语音或其他形式与用户交互。
**1.材料准备**
材料都可以在淘宝上轻易购买，包括树莓派主板，sdcard（最好8g或者以上），sdcard读卡器，电源，HDMI转VGA转换器（如果显示器支持hdmi可以直接用HDMI线，因为树莓派主板视频口是HDMI）,网线。
**2.刷系统镜像**
（1）首先需要下载Android Things系统镜像，目前最新的android Things是预览版，可以到下面的地址下载相对应芯片的系统镜像。
Android  Things镜像地址 [https://developer.android.google.cn/things/preview/download.html](https://developer.android.google.cn/things/preview/download.html)
（2）用sdcard读卡器连接sdcard到电脑上。
（3）解压刚才下载的系统镜像。
（4）写入镜像到sdcard的工具，可以根据不同系统自行选择，本教程使用windows系统，可以到[https://sourceforge.net/projects/win32diskimager/](https://sourceforge.net/projects/win32diskimager/)  这个网站下载**Win32 Disk Imager工具。**运行win32 DiskImager ，最右边选择你sdcard的盘符（备注不要选到了其他的硬盘或移动存储设备，导致造成不必要的损失），然后选择镜像路径，再然后点击write等待写入结果。写入完后把sdcard插到树莓派上，插上网线（网线另一端连接路由），插上视频输出线连接到显示器，USB数据线连接电脑到树莓派，树莓派就会开始启动系统了，系统启动后，屏幕会出现局域网的ip。
（5）打开命令行工具，输入下面命令（ip -address就是显示器上android things系统显示的界面），不出情况的话就提示connected to <ip-address>:5555  这样就成功连接到树莓派。
adb connect <ip-address>
**3.连接WIFI**
如果不想每次调试都要电脑连接树莓派的话，可以设置树莓派连接wifi(必须要完成上面连接到树莓派才能设置无线wifi连接)
(1)发送意图到Android Things的wifi服务模块的命令如下
- $ adb shell am startservice \
- -n com.google.wifisetup/.WifiSetupService\
- -a WifiSetupService.Connect\
- -e ssid <Network_SSID>\
- -e passphrase <Network_Passcode>
（2）通过adb 的logcat确定你的连接是否成功
$ adb logcat -d |grepWifi
 输入命令行后，不出情况一般出现下面的输出
- ...
- V WifiWatcher:Network state changed to CONNECTED
- V WifiWatcher: SSID changed:...
- I WifiConfigurator:Successfully connected to...
（3）测试你的树莓派是否能访问外网，命令
- $ adb shell ping 8.8.8.8
- PING 8.8.8.8(8.8.8.8)56(84) bytes of data.
- 64 bytes from8.8.8.8: icmp_seq=1 ttl=57 time=6.67 ms
- 64 bytes from8.8.8.8: icmp_seq=2 ttl=57 time=55.5 ms
- 64 bytes from8.8.8.8: icmp_seq=3 ttl=57 time=23.0 ms
- 64 bytes from8.8.8.8: icmp_seq=4 ttl=57 time=245 ms
**4.编写Android Things程序(Android Studio编写)**
在开始编写Android Things程序之前，
（1）确保你的sdK tools版本是24或者24以上
（2）确保你的sdk 支持API支持24或者更高版本。
**添加依赖**
（1）往app的模块里的build.gradle文件添加如下依赖
- dependencies {
-     provided 'com.google.android.things:androidthings:0.1-devpreview'
- }
（2）往manifest .xml文件添加
- <application...>
- <uses-libraryandroid:name="com.google.android.things"/>
-     ...
- application>
**声明主Activity**
要把应用运行在嵌入式设备（本教程中的树莓派），必须包含一个[CATEGORY_LAUNCHER](https://developer.android.google.cn/reference/android/content/Intent.html#CATEGORY_LAUNCHER)这样的Intent Filter，这样，才能在部署和调试应用的时候，Android Studio才能启动默认的Activity。
具体需要在Manifest声明的设置如下。
- <application
- android:label="@string/app_name">
- <uses-libraryandroid:name="com.google.android.things"/>
- <activityandroid:name=".HomeActivity">
- <intent-filter>
- <actionandroid:name="android.intent.action.MAIN"/>
- <categoryandroid:name="android.intent.category.LAUNCHER"/>
- intent-filter>
- <intent-filter>
- <actionandroid:name="android.intent.action.MAIN"/>
- <categoryandroid:name="android.intent.category.IOT_LAUNCHER"/>
- <categoryandroid:name="android.intent.category.DEFAULT"/>
- intent-filter>
- activity>
- application>
物联网已不是新鲜事，我们的生活中，不知不觉已经开始用上了这类产品。
在景点游玩结束时，我会先通过手机往被在停车场曝晒中小车发送一条“通风”指令，这样我就不用驾驶“桑拿房”回家了。
目前IoT领域未能形成完整的生态：如每个汽车厂商均有自己的车联网平台，智能家居厂商的产品只能用自家或合作伙伴的App连接。各自圈地，又互不兼容。
