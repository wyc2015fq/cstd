# Android wifi porting document - xqhrs232的专栏 - CSDN博客
2012年01月04日 16:31:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：683标签：[android																[socket																[module																[interface																[permissions																[service](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=permissions&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/longfeey/article/details/5899330](http://blog.csdn.net/longfeey/article/details/5899330)
非常详细的Android wifi porting文章
开发板上的8686wifi wpa_driver_priv_driver_cmd failed 问题比较严重，连scanning都过不去
[http://blog.linuxconsulting.ro/2010/04/porting-wifi-drivers-to-android.html](http://blog.linuxconsulting.ro/2010/04/porting-wifi-drivers-to-android.html)
For [mini-box.com picoPC](http://www.mini-box.com/pico-SAM9G45-X) we want to support several USB and miniPCI WiFi dongles, this guide provides a step by step explanation of what's involved in adding a new wifi
 driver and making wifi work in a custom Android build (this guide was written for android 2.1 but should be applicable to previous android releases and hopefully future releases).
**Contents**
0. Understand how Android WiFi works.
1. Enable building of wpa_supplicant in your BoardConfig.mk
2. (Optional) Enable debug for wpa_supplicant.
3. Provide a proper wpa_supplicant.conf for your device 
4. Have the correct paths and permissions created from init.rc
5. Make sure your wpa_supplicant and dhcpcd (optional) are starting from init.rc 
6. Provide your driver either as a module or built in kernel and proper kernel support for it and modify Android source code accordingly.
7. Provide a firmware if your module needs it.
8. Make your driver work with Android custom wpa_supplicant commands and SIOCSIWPRIV ioctl
Now onto details.
**0. Understand how Android WiFi works.**
Android uses a modified *wpa_supplicant* (external/wpa_supplicant) daemon for wifi support which is controlled through a socket byhardware/libhardware_legacy/wifi/wifi.c (WiFiHW) that gets controlled from Android UI through *android.net.wifi* package
 fromframeworks/base/wifi/java/android/net/wifi/ and it's corresponding jni implementation in frameworks/base/core/jni/android_net_wifi_Wifi.cpp Higher level network management is done in frameworks/base/core/java/android/net
**1. Enable building of wpa_supplicant in your BoardConfig.mk**
This is by simply adding: BOARD_WPA_SUPPLICANT_DRIVER := WEXT
to your BoardConfig.mk. This will set WPA_BUILD_SUPPLICANT to true in
external/wpa_supplicant/Android.mk enabling building of driver_wext.c
If you have a custom wpa_supplicant driver (like madwifi or my custom android private commands emulation - see last paragraph) you can replace WEXT with AWEXT or your driver name (MADWIFI, PRISM etc).
**2. (Optional) Enable debug for wpa_supplicant.**
By default wpa_supplicant is set to MSG_INFO that doesn't tell much. To enable more messages:
2.1 modify common.c and set wpa_debug_level = MSG_DEBUG
2.2 modify common.h and change #define wpa_printf from if ((level) >= MSG_INFO) to if ((level) >= MSG_DEBUG)
**3. Provide a proper wpa_supplicant.conf for your device**
Providing a wpa_supplicant.conf it's important because the control socket for android is specified in this file (ctrl_interface= ). This file should be copied by your AndroidBoard.mk to $(TARGET_OUT_ETC)/wifi (usually /system/etc/wifi/wpa_supplicant.conf).
 This location will be used on wpa_supplicant service from init.rc. 
There are two different ways in which wpa_supplicant can be configured, one is to use a "private" socket in android namespace, created bysocket_local_client_connect() function in wpa_ctrl.c and another is by using a standard unix socket.
Minimum required config options in wpa_supplicant.conf :
**- Android private socket**
ctrl_interface=wlan0
update_config=1
**- Unix standard socket**
ctrl_interface=DIR=/data/system/wpa_supplicant GROUP=wifi
update_config=1
Depending on your driver you might also want to add:
ap_scan=1
If you have AP association problems with should change to ap_scan=0 to let the driver do the association instead of wpa_supplicant.
If you want to let wpa_supplicant connect to non-WPA or open wireless networks (by default it skips these kind) add:
network={
key_mgmt=NONE
}
**4. Have the correct permissions and paths created from init.rc**
Incorrect permisions will result in wpa_supplicant not being able to create/open the control socket and libhardware_legacy/wifi/wifi.c won't connect.
Since Google modified wpa_supplicant to run as **wifi** user/group the directory structure and file ownership should belong to wifi user/group (seeos_program_init() function in wpa_supplicant/os_unix.c).
Otherwise errors like:
E/WifiHW ( ): Unable to open connection to supplicant on "/data/system/wpa_supplicant/wlan0": No such file or directory will appear.
Also wpa_supplicant.conf should belong to wifi user/group because wpa_supplicant will want to modify this file. If your system has /system as read-only use a location like /data/misc/wifi/wpa_supplicant.conf and modify wpa_supplicant service in init.rc with
 new location.
Make sure the paths are correctly created in init.rc:
mkdir /system/etc/wifi 0770 wifi wifi
chmod 0770 /system/etc/wifi
chmod 0660 /system/etc/wifi/wpa_supplicant.conf
chown wifi wifi /system/etc/wifi/wpa_supplicant.conf
#wpa_supplicant control socket for android wifi.c (android private socket)
mkdir /data/misc/wifi 0770 wifi wifi
mkdir /data/misc/wifi/sockets 0770 wifi wifi
chmod 0770 /data/misc/wifi
chmod 0660 /data/misc/wifi/wpa_supplicant.conf
chown wifi wifi /data/misc/wifi
chown wifi wifi /data/misc/wifi/wpa_supplicant.conf
If you use a** Unix standard socket** in wpa_supplicant.conf (see above) add:
# wpa_supplicant socket (unix socket mode)
mkdir /data/system/wpa_supplicant 0771 wifi wifi
chmod 0771 /data/system/wpa_supplicant
chown wifi wifi /data/system/wpa_supplicant
Do not add these if you use Android private socket because it will make wpa_supplicant non-functional, becausehardware/libhardware_legacy/wifi/wifi.c check for existence of the /data/system/wpa_supplicant folder and will pass a wrong interface name towpa_ctrl_open()
 function. 
**5. Make sure your wpa_supplicant and dhcpcd are starting from init.rc**
For wpa_supplicant the init.rc startup like should be depending on which path you chosen:
**- Android private socket:**
service wpa_supplicant /system/bin/wpa_supplicant -dd -Dwext -iwlan0 -c /system/etc/wifi/wpa_supplicant.conf
socket wpa_wlan0 dgram 660 wifi wifi
group system wifi inet
disabled
oneshot 
**- Unix standard socket:**
service wpa_supplicant /system/bin/wpa_supplicant -dd -Dwext -iwlan0 -c /system/etc/wifi/wpa_supplicant.conf
group system wifi inet
disabled
oneshot
If your wifi driver creates a wifi interface with other name than wlan0 you will have to modify the above line accordingly.
You also should have dhcpcd starting from init.rc
service dhcpcd /system/bin/dhcpcd wlan0
group system dhcp
disabled
oneshot
**6. Provide your driver either as a module or built in kernel and proper kernel support for it.**
First make sure that CONFIG_PACKET and CONFIG_NET_RADIO (wireless extensions) are enabled in your kernel. The driver can be built as module (default android way) or built in kernel (if you want to rely in kernel auto probing
 to support multiple driver eg. USB wifi) but will require source code modifications (see below).
**- As kernel module:**
   Define in your BoardConfig.mk:
   1. WIFI_DRIVER_MODULE_PATH := path to the module to be loaded
       You need to specify module name in that path too, usually should look something like /system/lib/modules/wlan.ko
   2. WIFI_DRIVER_MODULE_NAME:= the name of the network interface that the driver creates, for example wlan0 
   3. WIFI_DRIVER_MODULE_ARG:= any arguments that you want to pass to the driver on insmod, for example nohwcrypt
   Make sure you copy your kernel module when building android to the correct location.
**- As built in kernel:**
- First init.rc needs to be modified to inform hardware/libhardware_legacy/wifi/wifi.c about the name of the interface, that the driver is already loaded and set the status of wpa_supplicant to running: 
setprop wifi.interface "wlan0"
setprop wlan.driver.status "ok"
Do **NOT** add setprop init.svc.wpa_supplicant "running" as I previously mentioned as it will prevent wpa_supplicant from starting from init.
- Secondly hardware/libhardware_legacy/wifi/wifi.c need to be modified so the functions insmod() and rmmod() return 0 (simply add return 0; as the first line in functions since they are not needed when driver is built in kernel) and return before checking for /proc/modules
 in check_driver_loaded() function.
You might encounter problems with WifiHW module not being able to connect to wpa_supplicant socket even with the correct permisions. Try to turn off / turn on Wifi from the GUI.
**7. Provide a firmware if your driver needs it**
If your driver needs a firmware you will have to copy this firmware file to /etc/firmware on your android build. Android doesn't use a standard hotplug binary (although there is an implementation available on [android-x86](http://www.android-x86.org/) system/code/toolbox/hotplug.c)
 instead the init process takes care of firmware events and loads the firmware file from /etc/firmware (see: system/core/init/devices.c handle_firmware_event() function).
Firmware file name is defined by the driver and might also contain a folder like: RTL8192SU/rtl8192sfw.bin, entire file path should be available in/etc/firmware
**8. Make your driver work with Android custom wpa_supplicant commands and SIOCSIWPRIV ioctl.**
Android uses SIOCSIWPRIV ioctl to send commands to modify driver behaviour and receive information like signal strength, mac address of the AP, link speed etc. This ioctl is usually not implemented in any known wireless drivers except bcm4329 which is in google [msm
 kernel branch](http://android.git.kernel.org/?p=kernel/msm.git;a=tree;f=drivers/net/wireless/bcm4329;h=a10842ca196951bc3399a8fa4cff09dce4ac305f;hb=refs/heads/android-msm-2.6.32).
The errors from not having this ioctl implemented will look like:
E/wpa_supplicant( ): wpa_driver_priv_driver_cmd failed wpa_driver_priv_driver_cmd RSSI len = 4096 
E/wpa_supplicant( ): wpa_driver_priv_driver_cmd failed 
D/wpa_supplicant( ): wpa_driver_priv_driver_cmd LINKSPEED len = 4096
E/wpa_supplicant( ): wpa_driver_priv_driver_cmd failed 
I/wpa_supplicant( ): CTRL-EVENT-DRIVER-STATE HANGED
After 4, WEXT_NUMBER_SEQUENTIAL_ERRORS errors, android will abort using the device.
To quickly test your wifi from interface you can disable error checking in external/wpa_supplicant/driver_wext.c by simply making ret = 0; inwpa_driver_priv_driver_cmd() function after the SIOCSIWPRIV ioctl call. This will make all access points in android
 UI appear without signal or MAC address.
To proper implement the ioctl you will need to modify your kernel driver to reply to SIOCSIWPRIV ioctl with RSSI (signal strength) and MACADDR commands being the most important.
A better way is to add a custom driver_xxx.c to google external/wpa_supplicant/ implementing wpa_driver_priv_driver_cmd() function that will take care of RSSI, MACADDR and others, through calls to SIOCGIWSTATS, SIOCGIFHWADDR ioctls, with the rest of the functions
 being called from driver_wext.c.
Below is a link to a patch for wpa_supplicant that I did for [mini-box.com picoPC](http://www.mini-box.com/pico-SAM9G45-X) Android build. It creates a new driver awext which "emulates" android driver commands using
 wireless extensions ioctls.
How to use the new driver:
1. In your BoardConfig.mk define: BOARD_WPA_SUPPLICANT_DRIVER := AWEXT
2. Change init.rc wpa_supplicant service command line by replacind -Dwext with -Dawext
AWEXT driver patch download: [0001-Added-a-separate-driver-awext-to-emulate-Android-dri.patch](http://www.linuxconsulting.ro/android/patches/0001-Added-a-separate-driver-awext-to-emulate-Android-dri.patch)
