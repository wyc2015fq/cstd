# 谈谈Android中WIFI 架构和控制流程 - xqhrs232的专栏 - CSDN博客
2012年01月01日 15:46:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：914标签：[android																[interface																[socket																[module																[signal																[cmd](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.itivy.com/android/archive/2011/7/9/talk-about-android-wifi-architecture-and-control-flow.html](http://www.itivy.com/android/archive/2011/7/9/talk-about-android-wifi-architecture-and-control-flow.html)
一、WIFI的基本架构
    1、wifi用户空间的程序和库:
         external/wpa_supplicant/
       生成库libwpaclient.so和守护进程wpa_supplicant。
    2、hardware/libhardware_legary/wifi/是wifi管理库。
    3、JNI部分：
         frameworks/base/core/jni/android_net_wifi_Wifi.cpp
    4、JAVA部分：
         frameworks/base/services/java/com/android/server/
         frameworks/base/wifi/java/android/net/wifi/
    5、WIFI Settings应用程序位于：
       packages/apps/Settings/src/com/android/settings/wifi/
    6、WIFI 驱动模块  wlan.ko
        wpa_supplicant通过wireless_ext 接口和驱动通信
    7、WIFI 硬件模块 
二、WIFI在Android中如何工作
   Android使用一个修改版wpa_supplicant作为daemon来控制WIFI，代码位于
   external/wpa_supplicant。wpa_supplicant是通过socket与
   hardware/libhardware_legacy/wifi/wifi.c通信。UI通过android.net.wifi package
   （frameworks/base/wifi/java/android/net/wifi/）发送命令给wifi.c。
   相应的JNI实现位于frameworks/base/core/jni/android_net_wifi_Wifi.cpp。
   更高一级的网络管理位于frameworks/base/core/java/android/net。
三、配置Android支持WIFI
   在BoardConfig.mk中添加：
      BOARD_HAVE_WIFI := true
      BOARD_WPA_SUPPLICANT_DRIVER := WEXT
   这将在external/wpa_supplicant/Android.mk设置WPA_BUILD_SUPPLICANT为true，
   默认使用驱动driver_wext.c。
   如果使用定制的wpa_supplicant驱动(例如 madwifi)，可以设置:
       BOARD_WPA_SUPPLICANT_DRIVER := MADWIFI
四、使能wpa_supplicant调试信息
   默认wpa_supplicant设置为MSG_INFO，为了输出更多信息，可修改：
   1、在common.c中设置wpa_debug_level = MSG_DEBUG;
   2、在common.c中把#define wpa_printf宏中的
      if ((level) >= MSG_INFO)
      改为
      if ((level) >= MSG_DEBUG)
五、配置wpa_supplicant.conf
   wpa_supplicant是通过wpa_supplicant.conf中的ctrl_interface=来指定控制socket的，应该在
   AndroidBoard.mk中配置好复制到$(TARGET_OUT_ETC)/wifi（也就是
   /system/etc/wifi/wpa_supplicant.conf）
   这个位置会在init.rc中再次检测的。
   一般的wpa_supplicant.conf配置为：
      ctrl_interface=DIR=/data/system/wpa_supplicant GROUP=wifi
      update_config=1
      fast_reauth=1  
   有时，驱动需要增加：
      ap_scan=1
   如果遇到AP连接问题，需要修改ap_scan=0来让驱动连接，代替wpa_supplicant。
   如果要连接到non-WPA or open wireless networks，要增加：
      network={
              key_mgmt=NONE
      }
六、配置路径和权限
   Google修改的wpa_supplicant要运行在wifi用户和组下的。代码可见wpa_supplicant/os_unix.c
   中的os_program_init()函数。
   如果配置不对，会出现下面错误：
      E/WifiHW  (  ): Unable to open connection to supplicant on
      "/data/system/wpa_supplicant/wlan0": No such file or directory will appear.
   确认init.rc中有如下配置：
       mkdir /system/etc/wifi 0770 wifi wifi
       chmod 0770 /system/etc/wifi
       chmod 0660 /system/etc/wifi/wpa_supplicant.conf
       chown wifi wifi /system/etc/wifi/wpa_supplicant.conf
       # wpa_supplicant socket
       mkdir /data/system/wpa_supplicant 0771 wifi wifi
       chmod 0771 /data/system/wpa_supplicant
       #wpa_supplicant control socket for android wifi.c
       mkdir /data/misc/wifi 0770 wifi wifi
       mkdir /data/misc/wifi/sockets 0770 wifi wifi
       chmod 0770 /data/misc/wifi
       chmod 0660 /data/misc/wifi/wpa_supplicant.conf
   如果系统的/system目录为只读，那应该使用路径/data/misc/wifi/wpa_supplicant.conf。
七、运行wpa_supplicant和dhcpcd
   在init.rc中确保有如下语句：
      service wpa_supplicant /system/bin/logwrapper /system/bin/wpa_supplicant -dd
                   -Dwext -iwlan0 -c /data/misc/wifi/wpa_supplicant.conf
         user root
         group wifi inet
      socket wpa_wlan0 dgram 660 wifi wifi
         oneshot
      service dhcpcd /system/bin/logwrapper /system/bin/dhcpcd -d -B wlan0
         disabled
         oneshot
   根据所用的WIFI驱动名字，修改wlan0为自己驱动的名字。
七、编译WIFI驱动为module或kernel built in
   1、编译为module
      在BoardConfig.mk中添加：
         WIFI_DRIVER_MODULE_PATH := "/system/lib/modules/ar6000.ko"
         WIFI_DRIVER_MODULE_ARG := ""  #for example nohwcrypt
         WIFI_DRIVER_MODULE_NAME := "ar6000"  #for example wlan0 
         WIFI_FIRMWARE_LOADER := ""         
   2、编译为kernel built in  
     1）在hardware/libhardware_legacy/wifi/wifi.c要修改interface名字，
     2）在init.rc中添加：
        setprop wifi.interface "wlan0"
     3）在hardware/libhardware_legacy/wifi/wifi.c中当insmod/rmmod时，
        直接return 0。
八、WIFI需要的firmware
   Android不使用标准的hotplug binary，WIFI需要的firmware要复制到/etc/firmware。
   或者复制到WIFI驱动指定的位置，然后WIFI驱动会自动加载。
九、修改WIFI驱动适合Android
   Google修改的wpa_supplicant要求SIOCSIWPRIV ioctl发送命令到驱动，及接收信息，例如signal
   strength, mac address of the AP, link speed等。所以要正确实现WIFI驱动，需要从
   SIOCSIWPRIV ioctl返回RSSI (signal strength)和MACADDR信息。
   如果没实现这个ioctl，会出现如下错误：
     E/wpa_supplicant(  ): wpa_driver_priv_driver_cmd failed
                               wpa_driver_priv_driver_cmd RSSI len = 4096 
     E/wpa_supplicant(  ): wpa_driver_priv_driver_cmd failed 
     D/wpa_supplicant(  ): wpa_driver_priv_driver_cmd LINKSPEED len = 4096
     E/wpa_supplicant(  ): wpa_driver_priv_driver_cmd failed 
     I/wpa_supplicant(  ): CTRL-EVENT-DRIVER-STATE HANGED
十、设置dhcpcd.conf
   一般/system/etc/dhcpcd/dhcpcd.conf的配置为：
      interface wlan0
      option subnet_mask, routers, domain_name_server
