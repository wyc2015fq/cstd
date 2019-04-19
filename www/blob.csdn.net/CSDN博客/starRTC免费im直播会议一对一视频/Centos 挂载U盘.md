# Centos 挂载U盘 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月04日 11:25:58[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：185
插入U盘提示：
[sdb] No Caching mode page present
[sdb] Assuming drive cache: write through

fdisk -l (查看磁盘分区信息)
Disk identifier: 0x00000000   Device Boot      Start         End      Blocks   Id  System/dev/sdb11378230371840    c  W95 FAT32 (LBA)
mount -t vfat /dev/sdb1 /mnt/myusb
umount /mnt/usb（卸载U盘）
执行umount命令卸载U盘时，报如下错误“device is busy”，可以用参数l解决问题。
如果没有/mnt/udisk文件夹，可以创建一个
mkdir /mnt/udisk
