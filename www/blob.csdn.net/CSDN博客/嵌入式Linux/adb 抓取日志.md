
# adb 抓取日志 - 嵌入式Linux - CSDN博客

2019年03月14日 10:33:04[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：36


`adb root
adb remount
mkdir C:\Users\Administrator\Desktop\idpadlog
C:
cd C:\Users\Administrator\Desktop\idpadlog
set h=%time:~0,2%
set h=%h: =0%
set m=%time:~3,2%
set m=%m: =0%
set y=%date:~0,4%%date:~5,2%%date:~8,2%%h%%m%
md %y%
cd %y%
mkdir sdcard
mkdir data
cd ..
adb pull /data/data/com.evergrande.idpad/databases %y%
adb shell ps >%y%/process.txt"
adb shell dumpsys meminfo >%y%/memory.txt
adb pull /sdcard/idpad/log %y%/sdcard
adb pull /sdcard/idpad/logcat %y%/sdcard
adb pull /sdcard/msc %y%/sdcard
adb pull /data/logs %y%/data
adb pull /data/anr %y%
adb pull /data/tombstones %y%
adb shell top -t -n 1 >%y%/thread.txt
pause`

