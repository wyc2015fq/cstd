# Windows\Ubuntu修复UEFI引导 - DumpDoctorWang的博客 - CSDN博客





2018年11月09日 16:26:30[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：666











### 目录
- [1、修复Windows引导](#1Windows_1)
- [2、修复Ubuntu的引导](#2Ubuntu_4)




# 1、修复Windows引导

修复Windows的EFI引导需要使用到的工具为大白菜装机工具，官网是http://www.winbaicai.com/。使用大白菜制作好装机工具以后，重启选择U盘启动（一定要选“UEFI：你的U盘名”那个启动项），进入大白菜的PE，然后在桌面就有一个引导修复工具，傻瓜式操作，我就不细说了。修复完之后就能正常启动了。

# 2、修复Ubuntu的引导

需要用到的工具为：
- Ubuntu的安装镜像（.iso文件），最好和你安装Ubuntu的镜像版本相同。
- 一个U盘（不能是启动盘，否则不能启动U盘里的系统）。

打开Windows，双击.iso文件，把Ubuntu的文件全部复制到U盘里面去。这里需要Windows来复制，否则不能正常启动Ubuntu的安装程序。

然后开机，选择从U盘启动（一定要选“UEFI：你的U盘名”那个启动项），然后进入如下界面
![安装界面](https://img-blog.csdnimg.cn/2018110916040453.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)

然后选择“Try ubuntu without installing”，进入演示系统
![演示系统](https://img-blog.csdnimg.cn/20181109160616363.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)

打开终端，输入如下命令
```bash
sudo mount /dev/sdXXX /mnt
sudo mount /dev/sdXX /mnt/boot/efi
for i in /dev /dev/pts /proc /sys /run; do sudo mount -B $i /mnt$i; done
sudo chroot /mnt
grub-install /dev/sdX
update-grub
```

注 :
- sdX = disk / 安装Ubuntu的硬盘
- sdXX = efi partition / EFI 分区
- sdXXX = system partition / Ubuntu系统所在分区

比如说，我安装Ubuntu的硬盘为/dev/sda（=disk），EFI分区在/dev/sda2（ efi partition），Ubuntu系统所在分区为/dev/sda5（ efi partition），那么，上面的命令就应该为：

```bash
sudo mount /dev/sda5 /mnt
sudo mount /dev/sda2 /mnt/boot/efi
for i in /dev /dev/pts /proc /sys /run; do sudo mount -B $i /mnt$i; done
sudo chroot /mnt
grub-install /dev/sda
update-grub
```

执行完上面的命令，没有报错，引导修复就完成了。至于如何查看Ubuntu系统是在哪个分区，方法很多，比较简单的在演示系统里面打开“Disks”，大约100～105M那个分区就是EFI分区，点一下，下面就有“Device”这项信息（就是分区号）。假如说你的EFI分区的分区为/dev/sda2，那么disk=/dev/sda。

参考链接：
[https://askubuntu.com/questions/831216/how-can-i-reinstall-grub-to-the-efi-partition](https://askubuntu.com/questions/831216/how-can-i-reinstall-grub-to-the-efi-partition)





