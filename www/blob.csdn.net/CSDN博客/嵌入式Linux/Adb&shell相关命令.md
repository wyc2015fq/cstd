
# Adb&shell相关命令 - 嵌入式Linux - CSDN博客

2015年04月03日 15:52:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：6061


Adb&shell相关命令
作者：韦启发
1、过滤显示字符
adb logcat | grep MyApp
adb logcat | grep -i myapp \#忽略大小写。
adb logcat | grep --color=auto -i  myapp \#设置匹配字符串颜色。更多设置请查看 grep 帮助。
adb logcat | grep --color=auto 1679  1679是setting APK的PID号，用adbshell 进去后运行ps可以看到。
2、查看 TP相关的信息
adb shell getevent    查看input设备属于哪一个event
adb shell getevent dev/input/event1
与input event 对应的信息root@android:/ \# cat/proc/bus/input/devices
查看TP上报的速率：getevent -r /dev/input/event1
查看TP上报的键值：getevent -l/dev/input/event1
3、查看USB节点信息
1、查看persist.sys.usb.config
**adb shell getprop | find "usb"**
2、使能MTP功能
**adbshell setprop persist.sys.usb.config mtp,diag,serial_smd,serial_tty,rmnet_smd,mass_storage,serial_smd,adb**
3、不使能MTP功能
**adb shell setprop persist.sys.usb.config mass_storage,adb**
http://blog.csdn.net/innost/article/details/8876392
MTP--Media Transfer Protocol（媒体传输协议）
PTP--（Picture Transfer Protocol）
UsbDeviceManager.java设置逻辑在这里面修改
MTP：[sys.usb.config]:[mtp,adb]
PTP：[sys.usb.config]:[ptp,adb]
SD：[sys.usb.config]:[mass_storage,adb]
CHG：[sys.usb.config]:[sys.usb.config]: [mtp,mass_storage,adb]

4、用adb 命令启动某个apk
C:\Users\Administrator>adb shell
shell@android:/ $am start com.android.gallery3d/com.android.camera.Camera

