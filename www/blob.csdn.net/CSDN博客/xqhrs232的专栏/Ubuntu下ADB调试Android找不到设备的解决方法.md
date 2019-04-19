# Ubuntu下ADB调试Android找不到设备的解决方法 - xqhrs232的专栏 - CSDN博客
2012年05月02日 14:32:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：813标签：[android																[ubuntu																[permissions																[vmware																[file																[tools](https://so.csdn.net/so/search/s.do?q=tools&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=vmware&t=blog)](https://so.csdn.net/so/search/s.do?q=permissions&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://my.oschina.net/u/183622/blog/33797](http://my.oschina.net/u/183622/blog/33797)
相关网帖
1.Ubuntu下ADB调试Android找不到设备的解决方法----[http://os.51cto.com/art/201104/256341.htm](http://os.51cto.com/art/201104/256341.htm)
2.在Ubuntu下解决 adb devices :???????????? no permissions 方法----[http://www.ourunix.org/post/151.html](http://www.ourunix.org/post/151.html)
 3.Ubuntu下连接Android设备 ----[http://blog.csdn.net/gavin_dinggengjia/article/details/7366098](http://blog.csdn.net/gavin_dinggengjia/article/details/7366098)
  Ubuntu 11.04 下通过usb连接 Android设备，使用 adb 时，会返回如下带有？？？？的字符串。
[view plain](http://blog.csdn.net/psuaije/article/details/6845356#)
- List of devices attached   
- ???????????? no permissions  
解决方法如下
1. 在终端运行 lsusb
会发现结果有会有如下类似记录:
[view plain](http://blog.csdn.net/psuaije/article/details/6845356#)
- Bus 002 Device 003: ID 0e0f:0002 VMware, Inc. Virtual USB Hub  
- Bus 002 Device 002: ID 0e0f:0003 VMware, Inc. Virtual Mouse  
- Bus 002 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub  
- Bus 001 Device 002: ID 22b8:7087 Motorola PCS   
- Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub  
2.使用Vim创建一个空文件，命令如下
[view plain](http://blog.csdn.net/psuaije/article/details/6845356#)
- sudo vim /etc/udev/rules.d/50-Android.rules  
4.执行后输入如下文本 
[view plain](http://blog.csdn.net/psuaije/article/details/6845356#)
- SUBSYSTEM=="usb", SYSFS{"High Tech Computer Corp."}=="0bb4", MODE="0666"
输入完成后按Esc 然后使用  :qw  命令保存并退出 
4.继续运行以下命令
[view plain](http://blog.csdn.net/psuaije/article/details/6845356#)
- sudo chmod a+rx /etc/udev/rules.d/50-Android.rules   
- sudo /etc/init.d/udev restart  
5. 在 Android sdk 的 tools 目录下运行 。就是adb命令所在的目录
[view plain](http://blog.csdn.net/psuaije/article/details/6845356#)
- sudo ./adb kill-server   
- sudo ./adb devices  
然后就可以用adb命令来进行其他操作. 
我的电脑执行adb devices命令后显示如下正常字符串
[view plain](http://blog.csdn.net/psuaije/article/details/6845356#)
- orola_olympus$ adb devices  
- List of devices attached   
- TA01403JXL  device  
//=================================================================
备注::
1>i.MX_Android_R10.3.2_User_Guide.html
5.9 How to setup PC (Windows) to support ADB/RNDIS
**For Linux PC：**
- ADB function
- Download the Android SDK. 
- Update the adb configuration to scan for freescale's pid:
- Run the SDK's tools to generate a configure file:
android-sdk-linux_86/tools/android update adb
- - - Modify the files:~/.android/adb_usb.ini, to add freescale vendor id:
# ANDROID 3RD PARTY USB VENDOR ID LIST -- DO NOT EDIT.
# USE 'android update adb' TO GENERATE.
# 1 USB VENDOR ID PER LINE.
0x15a2 
- - Create a new udev rule file under the PC's /etc/udev/rules.d/ named: imx-android.rules. And fill in the following line into the file:
SUBSYSTEM=="usb", SYSFS{idVendor}=="15a2", MODE="0666"
- - Change the new udev rule file's permission:
chmod a+r /etc/udev/rules.d/imx-android.rules
- - Connect the Android Device
- Restart the adb server- adb kill-server 
- adb start-server
