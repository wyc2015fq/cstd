# Ubuntu14.04引导菜单修复 - wishchinYang的专栏 - CSDN博客
2014年10月04日 10:16:51[wishchin](https://me.csdn.net/wishchin)阅读数：2796
原文链接：[http://www.metsky.com/archives/636.html](http://www.metsky.com/archives/636.html)
        独立分区下的Ubuntu引导菜单修复有点麻烦，执行挂载等命令时要小心检查，修复此类引导，首先需要确保当前系统和数据安全（包括Windows和Ubuntu），不可因为分区盘符输入错误导致数据丢失或误删除。
Wubi安装菜单修复要简单些，请参考《[Ubuntu引导菜单修复（一）](http://www.metsky.com/archives/577.html)》。
### 一、使用Ubuntu LiveCD
（亲测，可行.....）
使用Ubuntu LiveCD光盘启动系统，然后在Terminal终端中查找Ubuntu安装根分区（fdisk -l），并修复启动，步骤如下（依次执行）：
1、提升root权限
```cpp
sudo -i
```
2.查看磁盘列表
```cpp
fdisk -l
```
（该命令会显示当前磁盘列表，找到Ubuntu根分区，假设为sda2) 
3、挂载根分区
```cpp
mkdir /mnt/temp
mount /dev/sda2 /mnt
```
(根分区/dev/sda2需据实际情况确定)
4、挂载boot分区
```cpp
mount /dev/sda3 /mnt/boot
```
（如果有/boot分区，假设为sda3，如果无/boot独立分区则跳过本步骤）
5、挂载系统目录
```cpp
for i in /dev /dev/pts /proc /sys; do sudo mount -B $i /mnt$i; done
6、进入原系统
chroot /mnt 
7、重装grub到MBR
grub-install /dev/sda
update-grub
8、退出原系统
exit
9、重启电脑
```
备注：swap分区似乎不用挂载？
以上命令执行后，有可能WIN7的菜单又没有了，不过没关系，只用进入Ubuntu然后，再次执行sudo update-grub，会自动添加WIN7引导菜单，重启即可。
```python
tst@tst-laptop:~$ sudo update-grub
[sudo] password for tst:
Generating grub.cfg ...
Found linux image: /boot/vmlinuz-2.6.32-38-generic
Found initrd image: /boot/initrd.img-2.6.32-38-generic
Found memtest86+ image: /boot/memtest86+.bin
Found Windows 7 (loader) on /dev/sda1
done
tst@tst-laptop:~$
```
## 二、使用传统Grub menu.lst
如没有Ubuntu光盘，也可以修复引导，但需要通过Windows或WinPE等方式先安装一个Grub引导程序，比如Grub4dos，（安装方法可参考[Windows下如何硬盘安装CentOS](http://www.metsky.com/archives/553.html) 和[Windows
 7下硬盘安装Ubuntu方法](http://www.metsky.com/archives/254.html)）。然后再启动进入Grub菜单后使用命令行形式修改menu.lst。
注意：Ubuntu的Grub2引导启动菜单放在/boot/grub/grub.cfg，早期则用/boot/grub/menu.lst和/boot/grub/stage1。
如果想windows引导Ubuntu（即Grub1引导Grub2），示例如下（该方法已验证，参考来源下文论坛地址）：
/boot非单独分区
title Ubuntu
root (hd0,x)
kernel /boot/grub/core.img
boot
/boot单独分区
title Ubuntu
root (hd0,x)
kernel /grub/core.img
boot
请注意，不同版本的Ubuntu，core.img可能在不同的目录下，比如：
- Ubuntu 10.04 /boot/grub/core.img
- Ubuntu 12.04 /boot/grub/core.img
- Ubuntu 12.10 /boot/grub/i386-pc/core.img
- Ubuntu 14.04 /boot/grub/i386-pc/core.img
这样修改后的menu.lst就可以引导Ubuntu，即windows引导Ubuntu。
如果想使用Ubuntu默认的引导器来引导Windows，可以在进入Ubuntu之后执行如下两句即可：
- sudo grub-install /dev/sda
- sudo update-grub
这样系统引导菜单就又交给Ubuntu，把前面的Windows引导菜单就多余了，在Windows下删除即可。
