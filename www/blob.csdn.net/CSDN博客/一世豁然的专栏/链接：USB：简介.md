# 链接：USB：简介 - 一世豁然的专栏 - CSDN博客





2017年05月18日 08:59:09[一世豁然](https://me.csdn.net/Explorer_day)阅读数：366








通过USB配件和USB主机两种模式，Android支持各种USB外设和Android USB配件（实现Android配件协议的硬件）。 在USB配件模式下，外部USB硬件充当USB主机。 附件的例子可能包括机器人控制器; 坞站; 诊断和音乐设备;亭; 读卡器; 以及更多。这给具有主机功能的Android设备提供与USB硬件交互的能力。 Android USB配件必须设计为与Android设备配合使用，并且必须遵守Android附件通讯协议。 在USB主机模式下，Android驱动的设备充当主机。 设备的示例包括数码相机，键盘，鼠标和游戏控制器。
 专为各种应用和环境设计的USB设备仍然可以与可以正确与设备通信的Android应用程序进行交互。




图1显示了两种模式之间的差异。 当Android设备处于主机模式时，它充当USB主机并为总线供电。 当Android设备处于USB附件模式时，所连接的USB硬件（在这种情况下为Android USB配件）充当主机并为总线供电。


![](https://img-blog.csdn.net/20170518084821876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1：USB主机和配件模式





Android 3.1（API级别12）或更新版本直接支持USB附件和主机模式。 USB附件模式也作为附加库返回到Android 2.3.4（API级别10），以支持更广泛的设备。 设备制造商可以选择是否在设备的系统映像上包含附加库。





注意：支持USB主机和附件模式最终取决于设备的硬件，与平台级别无关。 您可以通过<uses-feature>元素过滤支持USB主机和附件的设备。 有关详细信息，请参阅USB附件和主机文档。








一、调试注意事项

当调试使用USB附件或主机功能的应用程序时，您很有可能将USB硬件连接到Android设备。 这将阻止您通过USB连接到Android设备的adb连接。 您仍然可以通过网络连接访问adb。 要通过网络连接启用adb：


1、将通过USB连接到Android电源的设备连接到计算机。




2、从您的SDK platform-tools /目录中，在命令提示符下输入adb tcpip 5555。




3、输入adb connect <device-ip-address>：5555您现在应该连接到Android设备，并且可以发出通常的adb命令，如adb logcat。




4、要将设备设置为在USB上侦听，请输入adb usb。



