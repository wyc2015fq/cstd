
# Android init.rc 服务启动不成功 - 嵌入式Linux - CSDN博客

2018年09月21日 14:39:00[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：66



# 问题
在开发过程中发现一个问题，我们需要在开机的时候判断硬件版本号去启动服务，
服务的名字是ledservice和ledservice4,但是发现每次烧录完固件后，服务启动不
成功，需要再复位重启一次，服务才可以正常启动。
日志如下
[5.328699] init: Service'logd-reinit'(pid257) exited with status0[5.329681] init: Starting service'ledservice'...
[5.331442] init: Starting service'exec 2 (/system/bin/vdc)'...
[5.353605] init: Service'exec 2 (/system/bin/vdc)'(pid269) exited with status0[5.355534] init: Service'ledservice'isbeing killed...
[5.364499] init: Service'ledservice'(pid268) killed by signal9[5.364531] init: Service'ledservice'(pid268) killing any childreninprocess group
[5.365442] init: Not bootcharting.

# 分析
## 1、我们自己刚开始修改的代码如下
service ledservice /system/bin/ledserviceclassmain
    console
    disabled
    onrestart
service ledservice4/system/bin/ledservice4classmain
    console
    disabled
    onrestart
on property:ro.boot.wzb210_audio_hw_rev=3start ledservice
on property:ro.boot.wzb210_audio_hw_rev=4start ledservice4
on property:ro.boot.wzb210_audio_hw_rev=5start ledservice4
## 第一次实验
刚开始一直担心是因为我们服务本身有问题，所以在我们的两个服务里面不做任何操作，直接打一个Log
实验的结果还是有问题，服务启动不成功。

## 第二次试验
后面我考虑到有可能是因为增加了版本号的原因，然后我把代码修改成下面，结果还是服务启动不成功
service ledservice /system/bin/ledserviceclassmain
console
disabled
onrestart
start ledservice

## 最后
我考虑到android 的adbd也是刷机就要起来的，然后我参照了adbd的编写，看到了差异，修改如下，服务可以正常启动
参照adbd，把class main改成class core就可以了。 
service ledservice/system/bin/ledservice-classmain+classcore
console
disabled
onrestart

# class core 和class main有何不同
在分析Kitkat加密功能的时候，在一些资料上看到一句话“对于加密后的系统，重启后，进入到输入密码界面，此时并没有完全加载Android系统，仅仅加载必要的服务”
那么这个必要的服务又是指的是那些服务呢？
在解答这个问题前我们先区分一下class core ，class main， class late_start。
这三个“class”,只是标识这个服务的类型是哪一个，然后通过调用class_start, class_reset, class_stop等命令的时候，来统一操作同一类的服务。
举个例子，从system/core/rootdir/init.rc文件中搜索“class main”可以搜到许多，例如有netd， ril-deamon服务被标识为class main,那么当我们调用class_start main命令时，所有标识为main的服务都会被启动，这里的netd ril-deamon就会被启动。对于core， late_start类的服务也是这样的。
这里以源代码中三星的manta为例进行演示：
class core 的服务如下：
|序号|service名称|所属文件|命令bin文件位置|
|1|watchdogd|init.manta.rc|/sbin/watchdogd|
|2|watchdogd|init.recovery.manta.rc|/sbin/watchdogd|
|3|setup_fs|init.smdk5250.rc|/system/bin/setup_fs|
|4|ueventd|init.rc|/sbin/ueventd|
|5|logd|init.rc|/system/bin/logd|
|6|healthd|init.rc|/sbin/healthd|
|7|console|init.rc|/system/bin/sh|
|8|adbd|init.rc|/sbin/adbd|
|9|servicemanager|init.rc|/system/bin/servicemanager|
|10|vold|init.rc|/system/bin/vold|
可以看到，core服务都是系统最基本的服务，只要core服务全部启动，手机此时是可以运行的，但是却看不到东西，原因是framework没有启动。此时启动的都是C，C++的进程。此时是不能打电话的，因为ril-deamon没有启动

class main的服务如下：
|序号|service名称|所属文件|命令bin文件位置|
|1|p2p_supplicant|init.manata.rc|/system/bin/wpa_supplicant|
|2|wpa_supplicant|init.manata.rc|/system/bin/wpa_supplicant|
|3|dhcpcd_wlan0|init.manata.rc|/system/bin/dhcpcd|
|4|dhcpcd_p2p|init.manata.rc|/system/bin/dhcpcd|
|5|dhcpcd_eth0|init.manata.rc|/system/bin/dhcpcd|
|6|dhcpcd_bt-pan|init.manata.rc|/system/bin/dhcpcd|
|7|dhcpcd_bt-pan|init.manata.rc|/system/bin/dhcpcd|
|8|iprenew_p2p|init.manata.rc|/system/bin/dhcpcd|
|9|iprenew_eth0|init.manata.rc|/system/bin/dhcpcd|
|10|iprenew_bt-pan|init.manata.rc|/system/bin/dhcpcd|
|11|gpsd|init.manata.rc|/system/vendor/bin/gpsd|
|12|mobicore|init.manata.rc|/system/bin/mcDriverDaemon|
|13|bugreport|init.manata.rc|/system/bin/dumpstate|
|14|netd|init.rc|/system/bin/netd|
|15|debuggerd|init.rc|/system/bin/debuggerd|
|16|debuggerd64|init.rc|/system/bin/debuggerd64|
|17|ril-daemon|init.rc|/system/bin/rild|
|18|surfaceflinger|init.rc|/system/bin/surfaceflinger|
|19|drm|init.rc|/system/bin/drmserver|
|20|media|init.rc|/system/bin/mediaserver|
|21|bootanim|init.rc|/system/bin/bootanimation|
|22|installd|init.rc|/system/bin/installd|
|23|flash_recovery|init.rc|/system/etc/install-recovery.sh|
|24|racoon|init.rc|/system/bin/racoon|
|25|mtpd|init.rc|/system/bin/mtpd|
|26|keystore|init.rc|/system/bin/keystore|
|27|dumpstate|init.rc|/system/bin/dumpstate|
|28|sshd|init.rc|/system/bin/start-ssh|
|29|mdnsd|init.rc|/system/bin/mdnsd|
|30|zygote|init.zygote32_64.rc|/system/bin/app_process|
|31|zygote|init.zygote32.rc|/system/bin/app_process|
|32|zygote|init.zygote64.rc|/system/bin/app_process64|

可以看到main的服务相对多一些，看到zygote了吧，由此可见main服务大部分是建立在java层或者与java层息息相关的系统服务。
class late_start 的服务如下：
|序号|service名称|所属文件|命令bin文件位置|
|1|sdcard|init.manta.rc|/system/bin/sdcard|
|2|sdcard|init.msmdk5250.rc|/system/bin/sdcard|

在看Android手机加密的资料的时候，看到late_start服务，以为有很多呢.，但是一搜索才知道，坑爹啊，只有一个。
由以上的简单分析，再来分析Kitkat手机加密功能的流程，和加密后的启动过程，就更加清晰了。
其实手机加密完毕后，重启手机，进入输入密码界面，此时系统的core，main服务都已经启动，只有late_start服务没有启动，可以直接无视late_start。这样说来，此时手机的系统服务和正常状态下是一样的。我们想利用系统服务做什么都行啊。^_^
那么为什么在该界面只能做很少的事情呢？
例如可以拨打紧急呼救号码，弹出输入法，显示设置中的CryptKeeper界面，可以锁屏。但是却玩不了游戏，无法调用我们已经设置的“百度输入法”，而是使用很丑的Ladin输入法。锁屏也是系统默认的，拨号的界面也是系统原生的等等问题。这些问题，将在下一篇文章《Kitkat的加密功能对应用做了什么？》中解答





posted on2018-09-21 14:39[公众号；嵌入式Linux](https://www.cnblogs.com/weiqifa/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9686431)[收藏](#)


