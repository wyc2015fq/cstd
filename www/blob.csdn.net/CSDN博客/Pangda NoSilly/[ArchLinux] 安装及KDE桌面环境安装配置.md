# [ArchLinux] 安装及KDE桌面环境安装配置 - Pangda NoSilly - CSDN博客

2018年09月22日 22:22:39[hepangda](https://me.csdn.net/hepangda)阅读数：2001


# ArchLinux 安装及KDE桌面环境安装配置

首先，安装之前，需要一个“启动介质”，我这里习惯使用USB设备作为启动介质，这是由于ArchLinux滚动更新的特性，而且占用空间很小，以其他的方式安装个人认为不太适合。

话不多说，访问[ArchLinux官方网站](https://archlinux.org/)，点击右上角`Download`，在下方选择适合你的下载方式或镜像进行下载，我常用的地址是[网易的镜像源](http://mirrors.163.com/archlinux/iso/)，和[清华大学的镜像源](https://mirrors.tuna.tsinghua.edu.cn/archlinux/iso/)。下载完毕后，我一般使用开源工具[Rufus](http://rufus.akeo.ie/)将之写入到U盘中。

## 安装

将启动U盘插入到电脑中，选择从U盘启动，进入到archlinux的LiveCD模式，若使用网线连接，此时应已经自动配置好网络，可使用`ping`命令测试网络；若使用无线网络，则键入`wifi-menu`命令选择并登录无线网络。然后，输入以下命令保证系统时间是正确的：

```
timedatectl set-ntp true
```

然后我们可以通过`fdisk -l`命令查看分区情况，若要在此时修改分区表，ArchLinux提供了命令`parted`和`fdisk`，可以在此时修改分区表。若要格式化分区，比如想要格式化分区`/dev/sdb2`，可以输入以下命令：

```
mkfs.ext4 /dev/sdb2
```

该命令将`/dev/sdb2`重新格式化为`ext4`格式。若要使用交换分区，可以使用命令`mkswap`建立swap分区，并使用命令`swapon`使用之。在修改好分区之后，需要挂在分区，比如我们在之后的系统中，想要将`/dev/sda2`挂载到`/boot`，将`/dev/sdb2`挂载到`/`，将`/dev/sdb3`挂载到`/dev/sdb3`中，那么我们应该输入以下命令：

```
mount /dev/sdb2 /mnt
mkdir /mnt/boot
mkdir /mnt/home
mount /dev/sda2 /mnt/boot
mount /dev/sdb3 /mnt/home
```

当我们挂载好之后，就可以准备开始安装了，ArchLinux采用网络安装的形式，ISO中并不附带任何内容，因此首先在此时我们应当再次确认网络是否连接成功，然后我们需要配置包管理器的镜像源，我一般使用网易和清华大学的源，那么输入命令：

```
vim /etc/pacman.d/mirrorlist
```

按`i`进入编辑模式，在文件的最开头加入下面几句：

```
Server = http://mirrors.163.com/archlinux/$repo/os/$arch
Server = https://mirrors.tuna.tsinghua.edu.cn/archlinux/$repo/os/$arch
```

按`ESC`后输入`:wq`保存，然后输入命令：

```
pacman -Syy
```

更新软件包仓库缓存，此时若网络连接不正确，那么将报错误，需要重新配置网络环节。一切准备完毕之后，输入命令：

```
pacstrap -i /mnt base base-devel
```

根据网络速度，将等待一段时间之后，基本的ArchLinux便安装完毕，但此时还有一些收尾工作要做，首先就是使用命令：

```
genfstab -U /mnt >> /mnt/etc/fstab
```

生成`fstab`文件，在生成完毕之后，建议使用`cat /mnt/etc/fstab`再次检查一遍生成是否正确。然后我们使用命令：

```
arch-chroot /mnt
```

切换到新系统中，使用以下两条命令将时区配置为`Asia/Shanghai`:

```
ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
hwclock --systohc
```

使用这条命令时，假定你已经将BIOS中的时间设定为了**UTC时间**。然后使用`pacman -S vim`安装`vim`编辑器，并使用它打开`/etc/locale.gen`，取消掉下面三行之前的`#`号：

```
en_US.UTF-8 UTF-8
zh_CN.UTF-8 UTF-8
zh_TW.UTF-8 UTF-8
```

然后使用命令`locale-gen`生成locale信息。然后使用编辑器创建`/etc/locale.conf`，将之设定为：

```
LANG=en_US.UTF-8
```

这里也可以设置为`zh_CN.UTF-8`，但有可能造成TTY下出现乱码。

接着使用编辑器创建`/etc/hostname`，内容为你的主机名，并将对应的信息填入`/etc/hosts`文件中去。

然后，若使用无线网络，需要在此时输入命令安装相关的包，不然在新系统中将无法再使用`wifi-menu`命令连接无线网络：

```
pacman -S dialog wpa_supplicant
```

然后， 使用`passwd`命令设置`root`用户密码。

最后，若有需要，可以使用以下命令安装`grub`作为启动管理器，并可以安装Microcode，使用以下命令安装，以下是使用`Intel`处理器的一个例子：

```
pacman -S intel-ucode
pacman -S grub efibootmgr grub os-prober
grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=ArchLinux
grub-mkconfig -o /boot/grub/grub.cfg
```

要注意的是，此时`os-prober`并不能起到作用，若要使用`os-prober`用于兼容Windows，需要在重启之后再次运行`grub-mkconfig -o /boot/grub/grub.cfg`。

最后，输入`exit`命令回到LiveCD，并使用`umount -R /mnt`卸载已挂载的磁盘，使用命令`reboot`重启。

## KDE

再次启动后，使用配置的`root`用户及密码登录，先安装XOrg，输入命令：

```
pacman -S xorg xorg-server xorg-xinit xorg-apps
```

安装完毕后，使用下面的命令安装KDE：

```
pacman -S plasma kde-applications
pacman -S sddm sddm-kcm
```

游戏本用户此时可能需要安装下面的包：

```
pacman -S nvidia bumblebee
```

此时在`~/.xinitrc`文件中添加`exec startkde`一句，此时已经可以通过`startx`命令进入桌面环境，但推荐使用启动管理器代替这一步`startx`，方法是输入命令：

```
systemctl enable sddm
```

## 还需要的其他工作及参考资料

到这里大部分的安装已经结束了，剩下的就是个人的个性化设置了，需要注意的还有：
- NetworkManager
- Alsa
- 非root用户的建立和Sudoer

还可以参考[Arch Wiki的建议](https://wiki.archlinux.org/index.php/General_recommendations)。

## 更好的中文体验

作为简体中文用户可能需要安装以下字体包以保证字体显示正常、美观：

```
sudo pacman -S wqy-microhei wqy-microhei-lite wqy-bitmapfont wqy-zenhei ttf-arphic-ukai ttf-arphic-uming adobe-source-han-sans-cn-fonts adobe-source-han-serif-cn-fonts noto-fonts-cjk
```

