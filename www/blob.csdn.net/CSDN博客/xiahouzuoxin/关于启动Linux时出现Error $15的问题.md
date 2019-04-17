# 关于启动Linux时出现Error $15的问题 - xiahouzuoxin - CSDN博客





2012年10月05日 11:11:51[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2903








Linux版本：CentOS6.3




问题描述1：在使用U盘安装完系统后，拔掉U盘进入不了系统，插上U盘又能进入。

问题描述2：硬盘安装完系统后，在Windows下修改了硬盘（压缩、扩展、分区等操作），重启后进入不了系统。




以上是同一类问题：它们找不到GRUB引导，因此启动计算机后会进入

**GRUB > **



界面。问题1是默认将GRUB启动安装到了U盘，问题2是因为修改硬盘后Linux启动盘的sda号变化了，自然也就找不到GRUB引导项了。




解决办法是，在GRUB界面运行：

root (hd0,x)  

x为安装Linux所在的硬盘sda号，如果不清楚，可以从0开始一个个试

setup hd0  

这行命令作用是根据设定的root位置设定启动位置，若出现“无法找到image、vzlinuz等”，则可在root中尝试其他可用的x位置。

reboot

若前两步都能成功，则重启后就能看到启动项了。      



