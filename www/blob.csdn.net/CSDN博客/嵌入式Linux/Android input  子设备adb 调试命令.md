
# Android input  子设备adb 调试命令 - 嵌入式Linux - CSDN博客

2015年03月03日 15:40:21[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：965


adb shell getevent    查看input设备属于哪一个event
adb shell getevent dev/input/event1
与input event 对应的信息root@android:/ \# cat/proc/bus/input/devices
查看TP上报的速率：getevent -r /dev/input/event1
查看TP上报的键值：getevent -l/dev/input/event1

