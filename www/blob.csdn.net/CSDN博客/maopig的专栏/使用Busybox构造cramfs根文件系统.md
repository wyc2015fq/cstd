# 使用Busybox构造cramfs根文件系统 - maopig的专栏 - CSDN博客
2012年03月03日 16:44:53[maopig](https://me.csdn.net/maopig)阅读数：1266标签：[build																[makefile																[脚本																[工具																[file																[applet](https://so.csdn.net/so/search/s.do?q=applet&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)
个人分类：[u-boot/驱动开发](https://blog.csdn.net/maopig/article/category/1090389)
使用Busybox构造cramfs根文件系统 
11.1、下载Busybox，如果系统中没有mkcramfs工具则还要下载mkcramfs压缩工具。本文件系统使用Busybox-1.10.1，cramfs-1.1。压缩文件Busybox-1.10.1.tar.bz2，cramfs-1.1.tar.gz。
22.解压文件：
tar jxvf Busybox-1.10.1.tar.bz2
tar zxvf cramfs-1.1.tar.gz
2、进入解包之后生成cramfs-1.1目录，执行编译命令：
make
编译完成之后，会生成mkcramfs和cramfsck两个工具，其中**mkcramfs**工具是用来创建cramfs文件系统的，而**cramfsck**工具则用来进行cramfs文件系统的释放以及检查。如果该目录下已有这两个可执行程序，则无需编译，直接使用。
3.修改Makefile文件
以ARM板为例，打开Makefile文件，找到ARCH?=和CROSS_COMPILE?=两项，修改为
ARCH ?= arm
CROSS_COMPILE ?=arm-linux-//arm-linux-为交叉编译器在环境变量中的目录前缀，视自己交叉编译环境而定。
4.进入解压后的Busybox-1.10.1目录，运行make
 menuconfig或make gconfig进行配置。
在进行配置时有几项需要注意：
ØBuild Options－>
Build BusyBox as a static binary (no shared libs)
Force NOMMU build
Build shared libbusybox
Build with Large File Support (for accessing file>2GB)
如果选择 Build BusyBox as a static binary (no shared libs)方式进行编译时，所需的库已经与程序静态地链接在一起，这些程序不需要额外的库就可以单独运行，但是自己编写的程序在文件系统上运行必须采用静态编译，否则会报诸如：bin/sh:
 hello :not found的错误。
静态编译如：
arm-linux-gcc–static
hello.c –o hello
　如果选择Build shared libbusybox方式进行编译时，要将交叉编译的动态库或符号链接复制到对应的目录中，否则程序不能运行。同时在配置时应去掉Build
 shared libbusybox目录下的两项：如下　
Build shared libbusybox－>
Producebinary for each applet,linked against libbusybox
Produceadditional busybox binary linked against libbusybox
同时去掉
Build with Large File Support (for accessing file>2GB)
否则编译会报错。
Ø一定要勾选上如下配置：
Init Utilities—>
(*) init
(*) Supporting reading an inittab file//支技init进程读取/etc/inittab配置文件。
(*) Supporting running commands with controlling-tty//使busybox在真实的串口设备中运行命令行，不使用可能会报类似与：sh：can’t
 access tty：job control turned off的错误。
其它基本可按默认配置。
5.执行
make
编译完成后执行
make install
则在Busybox-1.10.1目录下有_install这个目录，这正是我们想要的。
66. _install目录下已经有三个包含应用程序的目录：bin、sbin、usr和脚本linuxrc，注意此linuxrc不要随便换掉，就用此linuxrc。
在此_install目录下创建其它几个需要的目录，如下
mkdir etc dev proc tmp lib var sys
7.转到dev目录下创建设备节点
mknod ram0 b 1 0
mknod-m660consolec51
mknod-m660nullc13
mknod-m660zeroc13
mknod-m666ttySAC0 c20464
8.编写脚本，这一点很重要，没写好或脚本有错误，文件系统都起不来。
转到etc目录，编写inittab脚本，内容如下：
# Startup the system
null::sysinit: /bin/mount –o remount,rw/
null::sysinit: /bin/mount –t procproc/proc
null::sysinit: /bin/mount –a
console::sysinit: /etc/init.d/rcS
ttySAC0::respawn: /bin/sh
9.编写挂载表fstab
proc/procprocdefaults00
sysfs/syssysfsdefaults00
none/dev/ptsdevptsmod=062200 
tmpfs/dev/shmtmpfsdefaults00
10.在etc下创建目录init.d
cd init.d
创建脚本rcS
内容如下：
#！ /bin/sh
保存，并修改rcS的权限，否则会出现Cannot
 run ‘/etc/init.d/rcS’:Permission denied的错误：
chmod775 rcS
1111.生成文件系统映像。
11、./mkcramfs _install rootfs.cramfs
至此，文件系统制作完毕。
