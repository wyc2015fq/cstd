# Ubuntu磁盘错误修复 - 三少GG - CSDN博客
2011年11月26日 11:20:30[三少GG](https://me.csdn.net/scut1135)阅读数：8440
## [如何在Ubuntu磁盘错误修复](http://blog.cnfol.com/loelsldpw/article/35757059.html)『原创』
[ 2011-03-12 14:43:27 ]
标签：无  阅读对象：所有人
第一步:
插入到计算机的CD驱动器Ubuntu的安装光盘。
第二步:
重新启动计算机并选择“从CD - ROM启动选项。”
第三步:
选择“尝试没有任何改变到您的电脑Ubuntu的”进入Live CD的菜单选项启动。
第四步:
点击“应用”在屏幕顶部的菜单。
第五步:
选择“附件”子菜单。
第六步:
选择“终端”选项，打开一个终端窗口。
第七步:
键入命令“的**fdisk**- l”的定位为驱动器，你要修复的设备名称。该设备的名称将类似于“/ dev / sda的。”
第八步:
键入命令“sudo的**fsck**的使用- y / dev / sda的”检查文件系统并修复任何错误。替换“/ dev / sda的”与你的磁盘要检查设备的名称。在“- Y”的选项，运行该命令说非交互。如果没有这个选项，该程序会要求确认之前，修复遇到的任何错误。
例如： ** sudo fsck  -y /dev/sdb15**
ps:上面’sdb15‘部分代表需要修复的分区
 屏幕显示：
**sudo fsck -y /dev/sda3**
fsck from util-linux 2.19.1
e2fsck 1.41.14 (22-Dec-2010)
/dev/sda3 已挂载.
WARNING!!!  The filesystem is mounted.   If you continue you ***WILL***
cause ***SEVERE*** filesystem damage.
你真的想要要继续 (y/n)? 是
/dev/sda3: 正在修复日志
正在清除  inode 312965 (uid=1000, gid=1000, mode=0100600, size=192)
正在清除  inode 312848 (uid=1000, gid=1000, mode=0100600, size=168)
正在清除  inode 312646 (uid=1000, gid=1000, mode=0100600, size=992)
正在清除  inode 296517 (uid=0, gid=0, mode=0100644, size=20710160)
正在清除  inode 312547 (uid=1000, gid=1000, mode=0100600, size=64)
正在清除  inode 312524 (uid=1000, gid=1000, mode=0100600, size=128)
正在清除  inode 312523 (uid=1000, gid=1000, mode=0100600, size=439)
正在清除  inode 312001 (uid=0, gid=0, mode=0100600, size=89)
正在清除  inode 296515 (uid=1000, gid=1000, mode=0100600, size=2048)
正在清除  inode 266498 (uid=1000, gid=1000, mode=0100600, size=1544)
正在清除  inode 264594 (uid=1000, gid=1000, mode=0100600, size=8192)
/dev/sda3: clean, 200906/505920 files, 1252520/2020864 blocks (check in 3 mounts)
第九步:
重复这个命令，直到没有更多的错误的报告。
