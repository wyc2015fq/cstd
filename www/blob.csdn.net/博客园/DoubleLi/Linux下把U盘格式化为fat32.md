# Linux下把U盘格式化为fat32 - DoubleLi - 博客园






在linux下也是支持fat32的，如果U盘中了病毒可以插入linux系统进行格式化比较安全，下面介绍如何在linux下把u盘格式化为fat32的方法

一、执行fdisk -l查看linux设备，我的U盘是sdb4

![](http://hiphotos.baidu.com/exp/pic/item/e6508eef76c6a7efe898db22fffaaf51f3de663c.jpg)

二、执行mkfs -t vfat /dev/sdb4

![](http://hiphotos.baidu.com/exp/pic/item/0d968f2397dda144ab851fb6b0b7d0a20df486e2.jpg)

这样就可以格式化完成了然后在挂载就可以使用了，非常简单。mkfs -t 后面指定格式可以是ext3格式或者其它格式。









