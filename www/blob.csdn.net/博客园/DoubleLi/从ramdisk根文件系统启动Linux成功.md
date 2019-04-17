# 从ramdisk根文件系统启动Linux成功 - DoubleLi - 博客园






这几天参考国嵌的实验手册和网上的资料完成了u-boot定制、内核定制、ramdisk根文件系统的制作，并成功。趁热打铁，总结一下。本文引用了很多网络上的文章，就不一一注明了。感谢各大侠的帮助，如有雷同，望见谅。

开发环境：红帽企业版5

交叉编译工具链：arm-linux-gcc4.3.2

嵌入式Linux内核版本：友善之臂的mini2440开发板光盘自带的内核linux-2.6.32.2

开发板：mini2440-64MNand Flash

Bootloader：u-boot-1.20

具体步骤如下：

**1.U-BOOT定制**

因为内容较多，改天再总结一篇吧，在这先不多说

**2.解压内核源码树**

具体步骤参照友善之臂mini2440开发板用户手册，具体不详述了。

**3.修改内核配置选项**

make menuconfig，打开配置菜单，修改两个配置项，分别是：

a）：General setup-->选择 Initial RAM filesystem and RAM disk...... 项

b）：DeviceDrivers-->Block devices-->选择 RAM block devicesupport 项

修改（4920）Default RAM disk size kbytes选项为（8192）Default RAM disk size kbytes

特别要注意的一点是，ramdisk是一种内存虚拟磁盘技术，实质上并不是一种文件系统，它使用的文件系统时ext2文件系统。所以一定要在make menuconfig的时候进入File systems菜单，选上<*>Second extended fs support。以提供内核对ext2文件系统的支持。

然后保存配置退出。

这样就为内核添加好了ramdisk启动功能和ramdisk的驱动支持了。
**4.修改内核启动参数**

方法有二：

a）：修改.config的第310行，修改"CONFIG_CMDLINE"的定义

修改为CONFIG_CMDLINE="initrd=0x32000000,0x200000root=/dev/ram rw console=ttySAC0 mem=64M" 保存。

意思为从ramdisk启动，ramdisk压缩文件起始地址在内存地址0x32000000处，文件大小为0x200000。

此参数也可以在makemenuconfig ARCH=arm时进入Boot options菜单，然后在Default kernel command string里修改。效果是一样的。

b）：或者不修改.config的的第310行CMDLINE定义,而是用u-boot的bootargs环境变量来传递启动参数。

同样也是修改该环境变量为bootargs=initrd=0x31000000,0x200000root=/dev/ram rw init=/linuxrc console=ttySAC0 mem=64M

并saveenv保存u-boot环境变量

以上a），b）的效果是一样的。（注：我只试了a，b未试）
**5.编译内核**

注意：好像友善之臂把内核源码目录里的uImage目标给注释了,直接makeuImage会提示没有uImage的目标。zImage 是ARMLinux常用的一种压缩映像文件，uImage是U-boot专用的映像文件，它是在zImage之前加上一个长度为0x40的“头”，说明这个映像文 件的类型、加载位置、生成时间、大小等信息。换句话说，如果直接从uImage的0x40位置开始执行，zImage和uImage没有任何区别。另外，Linux2.4内核不支持uImage，Linux2.6内核加入了很多对嵌入式系统的支持，但是uImage的生成也需要设置。

在这里，我直接把u-boot根目录下/tools中的mkimage工具拷贝到主机的/usr/bin目录下，就可以直接在内核根目录下执行

#make uImageARCH=arm CROSS_COMPILE=arm-linux-

等待一会，内核编译完成。

**6.制作ramdisk根文件系统(实际上是建立根文件系统）**

该过程是制作ramdisk根文件系统的核心步骤，方法如下：

a）创建根文件系统目录：

#cd转入到你的工作目录。

#mkdir rootfs

#cd rootfs

#mkdir bin dev etclib proc sbin sys usr mnt tmp var

#mkdir usr/binusr/lib usr/sbin lib/modules
b）创建最基本的设备文件：

#cd dev

#mknod -m 666console c 5 1

#mknod -m 666 nullc 1 3

#cd ..
c）安装/etc配置文件：

我是用的国嵌提供的etc配置文件，未作修改，直接解压到根文件系统里。

操作如下：

#tar etc.tar.gz -C/xxx/rootwhj

xxx表示你要制作的根目录所在路径。
d）编译内核模块：

进入Linux内核源码根目录

#make modulesARCH=arm CROSS_COMPILE=arm-linux-
e）安装内核模块：

#makemodules_install ARCH=arm INSTALL_MOD_PATH=/xxx/rootwhj//嵌入式中的内核模块都要在文件系统中

xxx表示你要制作的rootfs所在的目录。
f）配置busybox

进入busybox目录执行#make menuconfig

进入BusyboxSettings -> build Options ->选中"Build busyboxas a static binary“，即静态链接，免去拷贝大量的库文件。

InstallationOptions -> 选中"Don't use /usr"，以免busybox不慎被安装到宿主机系统的相应目录下，破坏宿主机系统。

BusyboxInstallation Prefix （/xxx/rootwhj），修改该选项表明编译后的busybox将安装到该位置。
g）编译、安装busybox

#make ARCH=arm CROSS_COMPILE=arm-linux-

几分钟编译完成后

#make install

安装到BusyboxInstallation Prefix （/xxx/rootwhj）设定的目录里。当前为我要制作的根文件系统目录（/xxx/rootwhj）。

//完成根文件系统的创建

h）制作ramdisk根文件系统镜像

方法如下：

上[http://genext2fs.sourceforge.net/](http://genext2fs.sourceforge.net/)下载可以简单方便的制作ramdisk文件系统的工具genext2fs，这样就不用像网上大多数说的那样繁琐的制作ramdisk映像了，当前最新版本为genext2fs-1.4.1.tar.gz 。

编译生成该工具genext2fs，并将其放入宿主机的/sbin/目录下以方便使用。

跳转到要制作的rootfs的上一级目录

#genext2fs -b 8192-d rootfs ramdisk

-b是指制作的ramdisk大小为8MB

-d是指要制作成ramdisk的根文件系统目录

最后的ramdisk是制作出来的ramdisk的名字，当然可以改名了。

#gzip -9 -f ramdisk

将该ramdisk以最优方式压缩为ramdisk.gz









