# ubuntu在shell中把文件拷贝进U盘 - happyhorizon的算法天空 - CSDN博客
2015年04月24日 11:30:06[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：2155
1、创建挂载位置，例如
sudo mkdir /mnt/u
这个位置只要建好，以后就可以不用再建了。
2、用mount命令将U盘挂载在这个位置
sudo mount  /dev/sdb1  /mnt/u
注意U盘的盘符不一定是/dev/sdb1.保险的做法是先查看一下U盘的信息。在终端输入：
sudo fdisk -l(英文字母l)
就会显示U盘的状态，例如：
Disk /dev/sdc: 32.2 GB, 32245809152 bytes
255 heads, 63 sectors/track, 3920 cylinders, total 62980096 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0xc3072e18
   Device Boot      Start         End      Blocks   Id  System
/dev/sdc1   *          32    62980095    31490032    c  W95 FAT32 (LBA)
这个U盘的盘符就是/dev/sdc1
3、将要拷贝进U盘的文件复制到U盘的挂载文件夹下
cd ~/Downloads
sudo cp -r 文件名   /mnt/u
要是剪切，也就是移动文件的话，可以用
sudo mv 文件名 /mnt/u
4、卸载U盘
umount  /mnt/u
