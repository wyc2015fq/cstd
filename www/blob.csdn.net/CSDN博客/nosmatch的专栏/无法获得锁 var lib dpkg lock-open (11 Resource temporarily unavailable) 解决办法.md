# 无法获得锁 /var/lib/dpkg/lock - open (11: Resource temporarily unavailable) 解决办法 - nosmatch的专栏 - CSDN博客
2011年07月21日 17:04:59[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：2264
无法获得锁 /var/lib/dpkg/lock - open (11: Resource temporarily unavailable) 
无法获得锁 /var/lib/dpkg/lock - open (11 Resource temporarily unavailable)
E: Unable to lock the administration directory (/var/lib/dpkg/), is another process using it?
在网上搜索到的解决方案──“将/var/lib/apt/list/lock删除掉即可”──其实是一种极端的情况，也就是在上次更新没有正常关闭的情况下使用。
在大部分情况下，问题的原因在于其它的程序如系统的自动更新、新立得等正在使用apt-get进程，所以解决方法也就是将这一进程关闭。
具体如下：
1、ps -aux 查出apt-get进程的PID，通常是一个四位数字。
2、用sudo kill PID代码 杀死进程
3、用sudo apt-get update，sudo apt-get dist-upgrade升级。
记住了不要你用sudo apt-get update强制关闭终端。如果你关闭就会出现这样的情况！
**情况二:**
无法获得锁 /var/lib/dpkg/lock - open (11 Resource temporarily unavailable)
E: Unable to lock the administration directory (/var/lib/dpkg/), is another process using it?
  这样的解决办法是：
先看到底有没开两个apt(新立得软件包管理器是否也开着)，如果确定没开两个APT-GET，请sudo rm /var/lib/dpkg/lock，这样就删除了锁文件，然后运行sudo dpkg --configure -a重新配置。
sudo rm /var/lib/dpkg/lock
