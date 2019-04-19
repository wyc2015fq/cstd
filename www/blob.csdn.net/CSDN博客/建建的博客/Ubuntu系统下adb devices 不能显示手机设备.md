# Ubuntu系统下adb devices 不能显示手机设备 - 建建的博客 - CSDN博客
2018年02月28日 12:08:23[纪建](https://me.csdn.net/u013898698)阅读数：125
1. 查看usb设备，命令：**lsusb**
结果如下：
Bus 001 Device 002: ID 8087:0024 Intel Corp. Integrated Rate Matching Hub
Bus 002 Device 002: ID 8087:0024 Intel Corp. Integrated Rate Matching Hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 002 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 003 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 004 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 005: ID 0cf3:3005 Atheros Communications, Inc. AR3011
Bluetooth
Bus 001 Device 004: ID 1bcf:2883 Sunplus Innovation Technology Inc.
Bus 002 Device 003: ID 17ef:6019 Lenovo
Bus 002 Device 007: ID 2717:9039
最后一个 2717：9309就是小米2，2717是vendorId, 9309是productId（注：如果不确定自己手机是哪个设备，可以拔出USB线再次输入lsusb命令，进行
对比确认哪个Devices是自己的手机）
2. 在 ***/etc/udev/rules.d/***下面新建**70-Android.rules**文件， 命令 **sudo gedit /etc/udev/rules.d/70-android.rules**
在文件中添加：
**SUBSYSTEM=="usb", ATTR{idVendor}=="2717",ATTRS{idProduct}=="9309",MODE="0666"**
（注意，这个70-android.rules 文件名字可以随意命名，比如 51-android.rules，52-android.rules 是都可以的。要注意标点都是英文下的）
3. 给70-android.rules加权限，命令：**sudo chmod a+rx /etc/udev/rules.d/70-android.rules**
4. 重启 udev服务，命令： **sudo service udev restart**
5. **sudo adb kill-server**， 然后 **sudo adb devices**,如果仍然没有显示设备则进行第6步。
6.命令： **echo "0x2717" > ~/.android/adb_usb.ini**
7.手机断开，重新连接
8.**sudo adb kill-server**， 然后 **sudo adb devices**
（一般都能成功，如果下次再次连接又连接不上时，只需sudo adb kill-server，然后 sudo adb devices即可显示设备）
