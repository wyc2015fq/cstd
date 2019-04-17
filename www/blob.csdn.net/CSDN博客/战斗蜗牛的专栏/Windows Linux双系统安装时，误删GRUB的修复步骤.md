# Windows Linux双系统安装时，误删GRUB的修复步骤 - 战斗蜗牛的专栏 - CSDN博客





2012年11月17日 13:06:21[vbskj](https://me.csdn.net/vbskj)阅读数：1054
个人分类：[Linux开发](https://blog.csdn.net/vbskj/article/category/643637)









                
本来机器上装有Windows XP 和 Fedora 14，后来在XP下硬盘全新安装Windows 7。这样机器上就只有Windows 7 和 Fedora 14 了。

不过在安装Windows 7时，它无情的把Fedora 14 装在硬盘MBR的GRUB给干掉了。



所以要想办法把Fedora 14 的引导选项找回来。Google搜索后，决定用EasyBCD。



下载地址：

EasyBCD 2.0.2英文版 http://neosmart.net/dl.php?id=1 





注意：

在Windows 7中安装运行EasyBCD，需要管理员权限。





下载好以后安装并运行，如图：





点击 Add New Entry －－ NeoGrub －－ Install，这样就安装好 NeoGrub了。



点击 Configure 可以编辑 NeoGrub 启动菜单文件。

这个启动菜单文件就是：C:\NST\menu.lst

编辑C:\NST\menu.lst，把Fedora 14 的引导选项添加进去。



如果忘记Fedora 的引导怎么写，那么重新启动机器。

在启动选项菜单时，选 NeoGrub Bootloader。



因为C:\NST\menu.lst 没有引导选项，所以会自动进入GRUB 命令行

GRUB >



在这里可以用cat 命令查看Fedora 14 中 /boot/grub/menu.lst 的内容



在GRUB > 这个提示符后输入



cat (hdX,Y)/boot/grub/menu.lst



##### (hdX,Y) 请替换成Fedora 14 根目录挂载的分区,请善用Tab按键补全命令和路径。



如果/boot 单独挂载在一个分区，那么命令就应该是这样了



cat (hdX,Y)/grub/menu.lst



##### (hdX,Y) 请替换成Fedora 14 /boot 挂载的分区



查看以后拿笔记下，然后进入Windows 7 中修改C:\NST\menu.lst，

也可以直接在GRUB > 后输入并引导

比如：

kernel (hd0,9)/boot/vmlinuz-2.6.35.6-45.fc14.i686 ro root=UUID=614ecdf7-137e-4b18-aed7-0fae210d5762 rhgb quiet

initrd (hd0,9)/boot/initramfs-2.6.35.6-45.fc14.i686.img

boot



####(hd0,9) 请替换成实际根目录挂载的分区，如果/boot挂载在独立分区，请按实际情况修改









进入Fedora后，可以把GRUB再装回MBR

比如只有一个硬盘，在Fedora14中就显示为/dev/sda 那么安装命令如下：

#grub-install /dev/sda
            


