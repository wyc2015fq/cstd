# Linux分区管理 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2016年08月05日 22:13:37[dutong0321](https://me.csdn.net/dutong0321)阅读数：255
### 题外话
最近，更新博客的速度都把自己给吓到了，继续。今天遇到个问题，需要把我SD卡上面的fat32分区格式化成ntfs分区，但是最近用的是树梅派，所以就懒得启动windows了，就在Linux下来完成这次格式化吧。
#### OS：Raspbian
思路： 
1.备份分区内容  
一般都直接挂载了，直接找到挂载到的目录就可以了
```
mkdir ~/backupDir
cp -r * ~/backupDir/
```
2.卸载格式化的分区 
一定要卸载分区，否则会造成设备冲突，而无法继续进行下去
```bash
sudo fdisk -l
```
根据自己情况找到自己要操作的设备，我的是/dev/sda1，因为树梅派的分区比较复杂就不往上放了。分辨的方式比较简单，根据分区、分区格式以及大小就可以找到自己对应的设备了。如果这三者相同的比较多，可以采用mount命令来查看对应的挂载，然后进挂载目录里查看文件来分辨各个设备。
```bash
sudo umount /dev/sda1
```
3.删除FAT32分区,重新划区 
比较复杂，我会在上面用//来标注一些注释，同时也是输入的地方，实际上是没有的，大家一定要小心。
```
sudo fdisk /dev/sda
Welcome to fdisk (util-linux 2.25.2).
Changes will remain in memory only, until you decide to write them.
Be careful before using the write command.
//d 代表删除分区
Command (m for help): d
Selected partition 1
Partition 1 has been deleted.
//n 新建一个分区
Command (m for help): n
Partition type
   p   primary (0 primary, 0 extended, 4 free)
   e   extended (container for logical partitions)
//p 新建一个主分区
Select (default p): p
//分区数 默认1个
Partition number (1-4, default 1): 1
//第一个扇区 如果要做启动磁盘 这里比较重要 默认2048 默认就好
First sector (2048-62341119, default 2048): 
//因为我只要划一个区 直接全部划好就可以了
Last sector, +sectors or +size{K,M,G,T,P} (2048-62341119, default 62341119): 
Created a new partition 1 of type 'Linux' and of size 29.7 GiB.
//w 写入到磁盘当中
Command (m for help): w
The partition table has been altered.
Calling ioctl() to re-read partition table.
Syncing disks.
```
4.将分区格式化为ntfs格式 
时间比较长，耐心一点。
```bash
sudo mkfs -t ntfs /dev/sda1
```
5.重新挂载，恢复备份
```bash
sudo mkdir /mnt/usb
sudo mount /dev/sda1 /mnt/usb/
mv backupDir/* /mnt/usb/
sudo umount /dev/sda1
sudo rmdir /mnt/usb/
rm ~/backupDir
```
