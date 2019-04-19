# Wifi移植文档 - android2.2及android2.3版本 - xqhrs232的专栏 - CSDN博客
2012年01月01日 15:05:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：864标签：[android																[文档																[buffer																[frameworks																[linux内核																[interface](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=frameworks&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/andyhuabing/article/details/7004451](http://blog.csdn.net/andyhuabing/article/details/7004451)
# 不久前移植过ranlink公司的wifi无线网卡，发现网上的一些文档不完整或者对一些碰到的问题没有说明，在此详细说明一，本人将其移植到android系统上碰到了不少的问题，修改了驱动及框架代码，费了两周多的时间才完全搞好。
# 一.   Android平台Wifi的基本代码路径
1.       Wpa_supplicant源码部分
external/wpa_supplicant_6/
生成库libwpa_client.so和守护进程wpa_supplicant
2.       Wifi的HAL层代码
位于hardware/libhardware_legary/wifi/
3.       Wifi的JNI部分
位于frameworks/base/core/jni/android_net_wifi_Wifi.cpp
4.       JAVA部分
位于frameworks/base/services/java/com/android/server/
        frameworks/base/wifi/java/android/net/wifi/
5.       wifi设置部分
位于packages/apps/Settings/src/com/android/settings/wifi/
**二.  linux内核配置**
在原有android内核支持情况下，增加wifi内核配置，具体配置如下：
1.       Networkingsupport  --->Wireless下增加802.11 协议栈的支持
![](http://hi.csdn.net/attachment/201111/23/0_1322033357KZDs.gif)
2.       USB 支持WIFI的配置
USB 支持WIFI 的配置选项位于Device Drivers>USB support 配置菜单下USB Wireless
Device Management support。
![](http://hi.csdn.net/attachment/201111/23/0_1322033688qQ5b.gif)
3.       用户空间的mdev 和firmware 支持配置
进入DeviceDriver > Generic Driver Options 配置菜单，按照下图所示配置用户空间
的mdev 和firmware支持。
![](http://hi.csdn.net/attachment/201111/23/0_1322033715zsLT.gif)
4.       WIFI 设备支持配置
DeviceDrivers  ---> Network devicesupport  ---> Wireless LAN  ---> Ralink driver support--->Ralinkrt2800 (USB) support (EXPERIMENTAL) --->rt2800usb - Include support forrt30xx (USB) devices
以及Wireless LAN 目录里IEEE 802.11 for Host AP (Prism2/2.5/3 andWEP/TKIP/CCMP)都选择上，目的是打开CONFIG_WIRELESS_EXT=y CONFIG_WEXT_PRIV=y
# 三. 驱动配置与编译
1.       修改驱动SDK包中的配置文件
1.1   修改env.mk，将RT28xx_DIR 设为当前目录，RT28xx_DIR = $(shell pwd)。
1.2   修改makefile中对应的kernel与交叉编译器路径
1.3   修改os/linux目录下config.mk中gcc 与 ld变量
1.4   打开os/linux目录下config.mk中HAS_WPA_SUPPLICANT与HAS_NATIVE_WPA_SUPPLICANT_SUPPORT宏
2.       修改驱动SDK包中的驱动源码
2.1   将rt_linux.h中的RTUSB_URB_ALLOC_BUFFER与RTUSB_URB_FREE_BUFFER宏修改，定义如下
#define RTUSB_URB_ALLOC_BUFFER(pUsb_Dev,BufSize, pDma_addr)     usb_alloc_coherent(pUsb_Dev,BufSize, GFP_ATOMIC, pDma_addr)
#defineRTUSB_URB_FREE_BUFFER(pUsb_Dev, BufSize, pTransferBuf, Dma_addr)       usb_free_coherent(pUsb_Dev, BufSize,pTransferBuf, Dma_addr)
2.2   修改rt_main_dev.c，直接将MainVirtualIF_close函数放空，return  0，解决不能反复关闭wifi问题。
2.3   修改rt_linux.c里RtmpOSNetDevAttach函数里增加devname赋值。strcpy( pNetDev->name, "mlan0");注：(此处所用的名字要与上层使用的节点名保持一致，在此说明一下上层主要有这几处用到节点名：
1，\frameworks\base\wifi\java\android\net\wifiWifiStateTracker.java
2，init.rc启动wpa_supplicant守护进程里面与启动dhcpcd服务
3，dhcpcd服务配置文件，dhcpcd.conf里面
4，init.rc设置setprop wifi.interface "mlan0")
3.       编译方法
Source env.mk;make;即可，驱动是在的路径为os/linux下的rt3070sta.ko。
此处所用的驱动名字应与HAL层wifi.c所指定驱动名保持一致
**四. Wap_supplicant相关配置**
3.1   Wpa_supplicant.conf配置文件的修改
ctrl_interface=DIR=/data/system/wpa_supplicantGROUP=wifi #这个路径在wifi.c中用到
3.2   整个环境必须要让wext类型相关代码进行编译。也就是要打开wext相关的宏CONFIG_DRIVER_WEXT。  即在device/hisi/Hi3716C/BoardConfig.mk中添加：
      BOARD_HAVE_WIFI := true
      BOARD_WPA_SUPPLICANT_DRIVER := WEXT
该配置的作用是使external/wpa_supplicant/Android.mk设置WPA_BUILD_SUPPLICANT为true。
3.3   在init.rc里面增加启动wpa_supplicant守护进程及dhcpcd进程
3.4   在init.rc里面增加wifi相关文件的权限设定，设置如下：
chmod 0771 /system/etc/wifi
      chmod 0660/system/etc/wifi/wpa_supplicant.conf
      chown wifiwifi /system/etc/wifi/wpa_supplicant.conf #wifi的原始配置文件
      #wpa_supplicantsocket
      mkdir/data/system/wpa_supplicant 0770 wifi wifi
      chmod 0771/data/system/wpa_supplicant  #放置wifiinterface的地方
      mkdir/data/misc/wifi 0770 wifi wifi
      chmod 0771/data/misc/wifi
chmod 0660 /data/misc/wifi/wpa_supplicant.conf  #wifi的配置文件，将由wpa_supplicant根据实际配置写入该文件
      chown wifiwifi /data/misc/wifi
      chown wifiwifi /data/misc/wifi/wpa_supplicant.conf
      mkdir/data/misc/wifi/sockets 0770 wifi wifi  #与上层通过socket通信的路径
      cp/system/etc/wifi/wpa_supplicant.conf /data/misc/wifi/
      mkdir/data/misc/dhcp 0777 dhcp dhcp
      chown dhcpdhcp /data/misc/dhcp
      # Preparefor wifi
    setpropwifi.interface "mlan0"
          setprop wlan.driver.status "ok"
3.5   启动wpa_supplicant守护进程与dhcpcd服务
在init.rc里面添加wpa_supplicant启动：
service wpa_supplicant/system/bin/logwrapper /system/bin/wpa_supplicant \
   -Dwext -imlan0 -c/data/misc/wifi/wpa_supplicant.conf -dd
             user root
             group system wifi inet
             socket wpa_mlan0 dgram 660 wifi wifi
             disable
                     oneshot
       在init.rc里面添加dhcpcd启动：
service dhcpcd /system/bin/logwrapper/system/bin/dhcpcd -d -B wlan0
         disabled
         oneshot
3.6   在init.godbox.rc里增加dns设置
Setprop net.dns1 192.168.10.247
Setprop net.dns2 192.168.10.248
# 五. wifi移植所需在android系统添加的一些文件
4.1   添加wifi的wpa_supplicant.conf配置文件
放置目录与hardware/libhardware_legacy/wifi/wifi.c中的目录保持一致
4.2   添加驱动的配置文件
在system/etc/Wireless/RT2870STA目录放置配置文件RT2870STA.dat，与rt_linux.h中配置文件的路径保持一致。
4.3   添加dhcpcd启动配置文件
设置/system/etc/dhcpcd/dhcpcd.conf的配置为：
      interface mlan0
      option subnet_mask, routers,domain_name_servers 
# 七. 其它平台移植记录
6.1    内核
内核的修改如上述第二大点内核配置
6.2    Wpa_supplicant
将wpa_supplicant_6编译打开
Wpa_supplicant 主要是在device/hisi/Hi3716C/BoardConfig.mk中添加：
      BOARD_HAVE_WIFI := true
      BOARD_WPA_SUPPLICANT_DRIVER := WEXT
         以及在wpa_supplicant_6 里面的.config增加ANDROID=y
//===============================================================
在Android环境下S3C6410开发板对USB WIFI 的支持----[http://bbs.eeworld.com.cn/viewthread.php?tid=230418](http://bbs.eeworld.com.cn/viewthread.php?tid=230418)
## 在Android环境下S3C6410开发板对USB WIFI 的支持
S3C6410的USBWIFI支持(Android2.1)
Topic:S3C6410支持USB WFI(作者：Sturtle)
//---------------------------------------------------------------------------------------------------------------------
// 平台：友坚UT-S3C6410开发板+Android2.1 OS+USB WIFI模块
// 注意事项：未经作者同意，商业网站不能转载，更不得在转载的时候擅自修改、删除文章的任何部分
//------------------------------------------------------------------------------------------------------------------------------
1、解压文件：
     tar jxvf ut6410-kernel2.6.29-v2.0-usbwifi.tar.bz2
     cd   ut6410-kernel2.6.29-v2.0-usbwifi
     make zImage
2、复制驱动：
编译完成，把ut6410-kernel2.6.29-v2.0-usbwifi\DPO_RT3070_LinuxSTA_V2.3.0.2_20100412\tftpboot\rt3070sta.ko
复制到ut6410-android2.1-v2.0\vendor\sec\ut6410\conf目录下，重新编译android2.1
3、系统设置：
烧写zImage和android之后，进入系统，在Settings->Location & security里面把Use
 Wireless network勾选上，然后重新启动，在Settings->Wireless & networks里面把Wi-Fi勾选上，在Wi-Fi settings里面可以看到在搜索局域网
//如有错误，请留言指正；同时希望有更好的方法与大家交流和分享，让我们大家一起进步！
本文来自友坚科技，转载请标明出处：http://www.urbetter.com
