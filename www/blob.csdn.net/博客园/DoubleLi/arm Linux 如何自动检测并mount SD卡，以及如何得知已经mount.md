# arm Linux 如何自动检测并mount SD卡，以及如何得知已经mount - DoubleLi - 博客园






一、土八路做法：

SD 卡一旦插入系统，内核会自动在/dev/下创建设备文件：sdcard。 但有时可能时用户在拨出卡前并没有umount的话，第二次插卡进去后系统创建的就不是sdcard设备文件了，而是mmcblk0, mmcblk1p1, mmcblk2p1, 或mmcblk3p1.

所以只需用if ( fopen("/dev/sdcard", "r") == NULL ) 来检测SD卡是否已经被内核捉到。然后就可以mount ....以及你想干的事情了。

如何得知自己已经有mount 了呢？
cat /proc/mounts 之后就会有：
rootfs / rootfs rw 0 0
/dev/root / yaffs rw,relatime 0 0
none /proc proc rw,relatime 0 0
none /sys sysfs rw,relatime 0 0
none /dev ramfs rw,relatime 0 0
none /dev/pts devpts rw,relatime,mode=622 0 0
tmpfs /dev/shm tmpfs rw,relatime 0 0
none /tmp ramfs rw,relatime 0 0
none /var ramfs rw,relatime 0 0
/dev/sdcard /mnt vfat rw,relatime,fmask=0022,dmask=0000,allow_utime=0022,codepag
e=cp437,iocharset=iso8859-1 0 0
/dev/mmcblk1p1 /mnt vfat rw,relatime,fmask=0022,dmask=0000,allow_utime=0022,code
page=cp437,iocharset=iso8859-1 0 0

哈哈。。。 最后两个就是我刚刚mount的。



二、专业的mdev方法（还有udev）：

[http://www.embedu.org/Column/Column167.htm](http://www.embedu.org/Column/Column167.htm)



mdev是busybox中的一个udev管理程序的一个精简版，他也可以实现设备节点的自动创建和设备的自动挂载，只是在实现的过程中有点差异，在发生热插拔时间的时候，mdev是被hotplug直接调用，这时mdev通过环境变量中的 ACTION 和 DEVPATH，来确定此次热插拔事件的动作以及影响了/sys中的那个目录。接着会看看这个目录中是否有“dev”的属性文件，如果有就利用这些信息为这个设备在/dev 下创建设备节点文件。


1.在使用busybox制作根文件系统的时候，选择支持mdev

Linux System Utilities  --->   
           [*] mdev      
           [*]   Support /etc/mdev.conf
           [*]     Support command execution at device addition/removal

2.在文件系统添加如下内容

Vim /etc/init.d/rcS
        mount -t tmpfs mdev /dev 
        mount -t sysfs sysfs /sys
        mkdir /dev/pts
        mount -t devpts devpts /dev/pts

echo /sbin/mdev>/proc/sys/kernel/hotplug
        mdev –s

这些语句的添加在mdev的手册中可以找到。

3.添加对热插拔事件的响应，实现U盘和SD卡的自动挂载。

Vim /etc/mdev.conf
        mmcblk[0-9]p[0-9] 0:0 666 @ /etc/sd_card_inserting
        mmcblk[0-9] 0:0 666 $ /etc/sd_card_removing
        sd[a-z] [0-9] 0:0 666 @ /etc/usb/usb_inserting
        sd[a-z] 0:0 666 $ /etc/usb/usb_removing

灰色部分，是一个脚本，脚本内容可以根据我们的需要定制，可以实现挂载，卸载或其他一些功能。

如下是自动挂载和卸载的脚本：

/etc/sd_card_inserting
        #!/bin/sh
        mount -t vfat /dev/mmcblk0p1 /mnt/sd

/etc/sd_card_removing
        #!/bin/sh
        sync
        umount /mnt/sd










