# Hi3516A开发--挂载SD卡和U盘 - maopig的专栏 - CSDN博客
2017年02月08日 18:44:59[maopig](https://me.csdn.net/maopig)阅读数：1039
**一、SD卡**
**1、通过fdisk -l命令确认板子上的[Linux](http://lib.csdn.net/base/linux)系统是否识别SD卡**
/ # fdisk -l 
Disk /dev/mmcblk0: 63.8 GB, 63864569856 bytes
255 heads, 63 sectors/track, 7764 cylinders
Units = cylinders of 16065 * 512 = 8225280 bytes
        Device Boot      Start         End      Blocks  Id System
/dev/mmcblk0p1   *           1           5       40131   c Win95 FAT32 (LBA)
/dev/mmcblk0p2               6        7764    62324167+ 83 Linux
**2、查看分区中的块分配信息**
/ # cat /proc/partitions 
major minor  #blocks  name
 179        0   62367744 mmcblk0
 179        1      40131 mmcblk0p1
 179        2   62324167 mmcblk0p2
3、挂载SD卡
mount -t vfat   /dev/mmcblk0p1 /mnt/mmcblk0p1
mount -t ext3  /dev/mmcblk0p2 /mnt/mmcblk0p2 
**4、卸载SD卡**
umount  /mnt/mmcblk0p1
umount /mnt/mmcblk0p2 
**5、格式化SD卡**
mkfs.vfat /dev/mmcblk0p1
**6、扩展**
下面列出SD/MMC 所有相关驱动：
**文件系统和存储设备相关模块**
nls_base
nls_cp437
fat
vfat
msdos
nls_iso8859-1
nls_ascii
**SD/MMC 相关模块**
mmc_core
himci
mmc_block
**二、U盘**
**1、插入U盘**
~ # usb 1-1: new
 high-speed USB device number 2 using hiusb-ehci
scsi0 : usb-storage 1-1:1.0
usbdev11 -> /dev/usbdev1.2
usb 1-1: USB disconnect, device number 2
usb 2-1: new full-speed USB device number 2 using hiusb-ohci
usb 2-1: not running at top speed; connect to a high speed hub
scsi1 : usb-storage 2-1:1.0
usbdev21 -> /dev/usbdev2.2
scsi 1:0:0:0: Direct-Access     Kingston DataTraveler 3.0 PMAP PQ: 0 ANSI: 6
sd 1:0:0:0: [sda] 30720000 512-byte logical blocks: (15.7 GB/14.6 GiB)
sd 1:0:0:0: [sda] Write Protect is off
sd 1:0:0:0: [sda] No Caching mode page present
sd 1:0:0:0: [sda] Assuming drive cache: write through
sd 1:0:0:0: [sda] No Caching mode page present
sd 1:0:0:0: [sda] Assuming drive cache: write through
 sda: sda4
sd 1:0:0:0: [sda] No Caching mode page present
sd 1:0:0:0: [sda] Assuming drive cache: write through
sd 1:0:0:0: [sda] Attached SCSI removable disk
udisk2110 -> /dev/sda
udisk2110p4 -> /dev/sda4
其中： sda1 表示 U 盘或移动硬盘上的第一个分区，当存在多个分区时，会出现
 sda1、sda2、sda3 等字样。
**2、挂载**
mount -t vfat /dev/sda
 /mnt/usb
**3、卸载**
umount /dev/sda
**4、格式化**
mkdosfs –F 32 /dev/sda
**5、扩展**
下面列出所有USB相关驱动：
**文件系统和存储设备相关模块**
vfat
scsi_mod
sd_mod
nls_ascii
nls_iso8859-1
**键盘相关模块**
evdev
usbhid
**鼠标相关模块**
mousedev
usbhid
evdev
**USB2.0 模块**
ohci-hcd
ehci-hcd
usb-storage
hiusb-hi3516A
//http://blog.csdn.net/qq_29350001/article/details/52680643
