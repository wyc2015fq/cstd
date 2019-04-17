# Win10重装系统后Git指令异常(fatal: open /dev/null or dup failed: No such file or directory) - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年08月28日 11:37:08[boonya](https://me.csdn.net/boonya)阅读数：362








**重装win10专业版后git安装后运行指令报错：**

fatal: open /dev/null or dup failed: No such file or directory

**解决方法：**

在其他win10电脑上复制一个null.sys文件，替换C:\Windows\System32\drivers\null.sys

null.sys下载地址：[https://download.csdn.net/download/boonya/10630962](https://download.csdn.net/download/boonya/10630962)

注意：替换后需要重启计算机。



