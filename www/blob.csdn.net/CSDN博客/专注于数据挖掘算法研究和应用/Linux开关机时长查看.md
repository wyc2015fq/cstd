# Linux开关机时长查看 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月05日 11:41:28[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3734








由于断电或供电故障突然停机，需要查看Linux开机时间/重启时间，常用命令：

1、who 命令查看


      who -b 查看最后一次系统启动的时间。


      who -r 查看当前系统运行时间。




2、last reboot查看Linux系统历史启动的时间

      $last reboot | head -1    查看最后一次Linux系统启动的时间。




3、top、w、uptime命令查看up。




4、查看/proc/uptime



```
$ date -d "$(awk -F. '{print $1}' /proc/uptime) second ago" +"%Y-%m-%d %H:%M:%S" 
2016-05-18 07:20:19
```


5、查看日志

   Linux系统下系统重启操作的日志信息，都是/var/log/messages目录下下保存着


`#cat /var/log/messages |more`



