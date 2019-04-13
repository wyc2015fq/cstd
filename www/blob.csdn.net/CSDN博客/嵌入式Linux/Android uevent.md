
# Android uevent - 嵌入式Linux - CSDN博客

2013年12月14日 22:49:12[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1772


[Android](http://lytsing.org/wiki/android/index.html)很多事件都是通过uevent跟kernel来异步通信的。其中类UEventObserver是核心。 UEventObserver接收kernel的uevent信息的抽象类。
1、server层代码 1)battery server:
frameworks/base/services/java/com/android/server/SystemServer.java  frameworks/base/services/java/com/android/server/BatteryService.java2、java层代码
frameworks/base/core/java/android/os/UEventObserver.java3、JNI层代码
frameworks/base/core/jni/android_os_UEventObserver.cpp4、底层代码
hardware/libhardware_legacy/uevent/uevent.c读写kernel的接口socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT); 原理：通过kobject_uevent的方式通知应用层，往一个socket广播一个消息。应用层打开socket监听NETLINK_KOBJECT_UEVENT组的消息，就可以收到了。插入耳机相关的代码:
hardware/libhardware_legacy/uevent  frameworks/base/core/jni/android_os_UEventObserver.cpp  frameworks/base/core/java/android/os/UEventObserver.java  frameworks/base/services/java/com/android/server/SystemServer.java  frameworks/base/services/java/com/android/server/HeadsetObserver.java这个应该是最简单的，添加新的uevent事件，可以参考这个。
高通HDMI在 frameworks的修改:
[http://gitorious.org/pandroid/frameworks-base/commit/651e286bb32264b650e6adaf84b91b9719043b9a/diffs](http://gitorious.org/pandroid/frameworks-base/commit/651e286bb32264b650e6adaf84b91b9719043b9a/diffs)
refer
[Android](http://lytsing.org/wiki/android/index.html)的用户层 uevent处理机制[http://blog.csdn.net/linphusen/archive/2010/06/12/5667647.aspx](http://blog.csdn.net/linphusen/archive/2010/06/12/5667647.aspx)
001-[Android](http://lytsing.org/wiki/android/index.html)底层分析-[Android](http://lytsing.org/wiki/android/index.html)uevent
 分析[http://blog.chinaunix.net/space.php?uid=24605155&do=blog&cuid=2363481](http://blog.chinaunix.net/space.php?uid=24605155&do=blog&cuid=2363481)
[Android](http://lytsing.org/wiki/android/index.html)中的BatteryService及相关组件[http://www.mcuol.com/Tech/265/34518.htm](http://www.mcuol.com/Tech/265/34518.htm)
ANDROID中的VOLD分析[http://blog.csdn.net/datangsoc/archive/2010/10/08/5928132.aspx](http://blog.csdn.net/datangsoc/archive/2010/10/08/5928132.aspx)
[Android](http://lytsing.org/wiki/android/index.html)Vold 分析（一）—system/vold/main.cpp——-mian函数分析[http://blog.csdn.net/sustzombie/archive/2011/01/04/6116123.aspx](http://blog.csdn.net/sustzombie/archive/2011/01/04/6116123.aspx)

