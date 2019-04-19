# MARK linux下 手机调试模式 no permissions的问题 - u013366022的专栏 - CSDN博客
2017年11月27日 14:22:52[slitaz](https://me.csdn.net/u013366022)阅读数：124
个人分类：[android](https://blog.csdn.net/u013366022/article/category/7312102)
1. 确保usb链接正确：
Bus 001 Device 003: ID 05c6:9025 Qualcomm, Inc. 
Bus 002 Device 002: ID 0e0f:0003 VMware, Inc. Virtual Mouse
Bus 002 Device 003: ID 0e0f:0002 VMware, Inc. Virtual USB Hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 002 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 002 Device 004: ID 0e0f:0008 VMware, Inc.
2. 新建或修改
/etc/udev/rules.d/70-android.rules
加入
SUBSYSTEM=="usb", ATTR{idVendor}=="05c6", ATTR{idProduct}=="9025", MODE="0666", OWNER="“user”"
idVendor是设备号，高通对应05c6
|Company|USB Vendor ID|
|----|----|
|Acer|`0502`|
|ASUS|`0b05`|
|Dell|`413c`|
|Foxconn|`0489`|
|Fujitsu|`04c5`|
|Fujitsu Toshiba|`04c5`|
|Garmin-Asus|`091e`|
|Google|`18d1`|
|Hisense|`109b`|
|HTC|`0bb4`|
|Huawei|`12d1`|
|K-Touch|`24e3`|
|KT Tech|`2116`|
|Kyocera|`0482`|
|Lenovo|`17ef`|
|LG|`1004`|
|Motorola|`22b8`|
|NEC|`0409`|
|Nook|`2080`|
|Nvidia|`0955`|
|OTGV|`2257`|
|Pantech|`10a9`|
|Pegatron|`1d4d`|
|Philips|`0471`|
|PMC-Sierra|`04da`|
|Qualcomm|`05c6`|
|SK Telesys|`1f53`|
|Samsung|`04e8`|
|Sharp|`04dd`|
|Sony|`054c`|
|Sony Ericsson|`0fce`|
|Teleepoch|`2340`|
|Toshiba|`0930`|
|ZTE|`19d2`|
3. 重新启动udev
sudo chmod 777 /etc/udev/rules.d/70-android.rules
sudo service udev restart
4. 重新启动adb
sudo adb kill-server 
adb devices
List of devices attached 
820a1669        device
可以看到设备了
