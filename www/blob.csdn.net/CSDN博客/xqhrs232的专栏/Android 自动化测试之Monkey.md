# Android 自动化测试之Monkey - xqhrs232的专栏 - CSDN博客
2012年02月29日 16:10:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：481标签：[测试																[android																[shell																[application																[eclipse																[框架](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/andy_android/article/details/7242086](http://blog.csdn.net/andy_android/article/details/7242086)
前言：
最近开始研究Android自动化测试方法，对其中的一些工具、方法和框架做了一些简单的整理，其中包括android测试框架、CTS、Monkey、Monkeyrunner、benchmark、其它test tool等等。因接触时间很短，很多地方有不足之处，希望能和大家多多交流。
一、  什么是Monkey
Monkey是Android中的一个命令行工具，可以运行在模拟器里或实际设备中。它向系统发送伪随机的用户事件流(如按键输入、触摸屏输入、手势输入等)，实现对正在开发的应用程序进行压力测试。Monkey测试是一种为了测试软件的稳定性、健壮性的快速有效的方法。
二、 Monkey的特征
1、测试的对象仅为应用程序包，有一定的局限性。
2、Monky测试使用的事件流数据流是随机的，不能进行自定义。
3、可对MonkeyTest的对象，事件数量，类型，频率等进行设置。
三、Monkey的基本用法
基本语法如下：
$ adb shell monkey [options]
如果不指定options，Monkey将以无反馈模式启动，并把事件任意发送到安装在目标环境中的全部包。下面是一个更为典型的命令行示例，它启动指定的应用程序，并向其发送500个伪随机事件：
$ adb shell monkey -p your.package.name -v 500
四、Monkey测试的一个实例
通过这个实例，我们能理解Monkey测试的步骤以及如何知道哪些应用程序能够用Monkey进行测试。
Windows下（注：2—4步是为了查看我们可以测试哪些应用程序包，可省略）：
1、 通过eclipse启动一个Android的emulator
2、 在命令行中输入：adb devices查看设备连接情况
C:\Documents and Settings\Administrator>adbdevices
List of devices attached
emulator-5554  device
3、 在有设备连接的前提下，在命令行中输入：adb shell 进入shell界面
C:\Documents and Settings\Administrator>adb shell
#
4、 查看data/data文件夹下的应用程序包。注：我们能测试的应用程序包都在这个目录下面
C:\Documents and Settings\Administrator>adb shell
# ls data/data
ls data/data
com.google.android.btrouter
com.android.providers.telephony
com.android.mms
com.android.providers.downloads
com.android.deskclock
com.android.email
com.android.providers.media
com.android.settings
jp.co.omronsoft.openwnn
com.android.providers.userdictionary
com.android.quicksearchbox
com.android.protips
com.android.browser
com.android.launcher
com.android.term
com.android.speechrecorder
com.android.server.vpn
com.android.defcontainer
com.svox.pico
com.android.customlocale
com.android.development
com.android.soundrecorder
com.android.providers.drm
com.android.spare_parts
com.android.providers.downloads.ui
com.android.fallback
com.android.providers.applications
com.android.netspeed
com.android.wallpaper.livepicker
android.tts
com.android.htmlviewer
com.android.music
com.android.certinstaller
com.android.inputmethod.pinyin
com.android.providers.subscribedfeeds
com.android.inputmethod.latin
com.android.gallery
com.android.systemui
com.android.contacts
com.android.phone
com.android.sdksetup
com.android.calculator2
com.android.packageinstaller
com.android.camera
com.android.providers.settings
com.thestore.main
com.android.providers.contacts
5、 以com.android.calculator2作为对象进行MonkeyTest
#monkey-p com.android.calculator2 -v 500
其中-p表示对象包 –v 表示事件数量
运行过程中，Emulator中的应用程序在不断地切换画面。
按照选定的不同级别的反馈信息，在Monkey中还可以看到其执行过程报告和生成的事件。 
注：具体参数的设定可参考：
[http://developer.android.com/guide/developing/tools/monkey.html](http://developer.android.com/guide/developing/tools/monkey.html)
五、关于Monkey测试的停止条件
Monkey Test执行过程中在下列三种情况下会自动停止：
1、如果限定了Monkey运行在一个或几个特定的包上，那么它会监测试图转到其它包的操作，并对其进行阻止。
2、如果应用程序崩溃或接收到任何失控异常，Monkey将停止并报错。
3、如果应用程序产生了应用程序不响应(application not responding)的错误，Monkey将会停止并报错。
通过多次并且不同设定下的Monkey测试才算它是一个稳定性足够的程序。
