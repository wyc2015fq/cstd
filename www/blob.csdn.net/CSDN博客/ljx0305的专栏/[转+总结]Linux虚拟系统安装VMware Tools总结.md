# [转+总结]Linux虚拟系统安装VMware Tools总结 - ljx0305的专栏 - CSDN博客
2008年07月16日 22:21:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：556
                 一、VMware Tools安装手记(For Linux Guest OS) 
为什么要装 VMware Tools？ 
因为它可以改善 Virtual Machine 的运行性能，而且可以让 Host OS 和 Guest OS 互通有无，这样我们就不用伤脑筋，要架设什么服务器，来沟通两个 OS，现在就让我们开始吧！ 
VMware Tools所在位置：VMware 安装路径 /VMware/VMware Workstation/linux.iso
[root@rd01 ~]# mount /cdrom 
# 有时可能加载不了，这时就要先将系统关闭，再手动指定 ISO 映像，看下图 
[root@rd01 ~]# cd /cdrom
[root@rd01 ~]# ls -a
[root@rd01 ~]# cp VMwareTools-5.5.1-19175.tar.gz /tmp
[root@rd01 ~]# cd /tmp
[root@rd01 ~]# tar zxpf VMwareTools-5.5.1-19175.tar.gz
[root@rd01 ~]# cd vmware-tools-distrib
[root@rd01 vmware-tools-distrib]# ./vmware-install.pl
Creating a new installer database using the tar3 format. 
Installing the content of the package. 
# 安装过程的画面，全部使用默认值，一直按 Enter 就对了 
In which directory do you want to install the binary files? 
[/usr/bin]
What is the directory that contains the init directories (rc0.d/ to rc6.d/)? 
[/etc/rc.d]
What is the directory that contains the init scripts? 
[/etc/rc.d/init.d]
In which directory do you want to install the daemon files? 
[/usr/sbin]
In which directory do you want to install the library files? 
[/usr/lib/vmware-tools]
Thepath "/usr/lib/vmware-tools" does not exist currently. This programisgoingto create it, including needed parent directories. Is thiswhatyou want? 
[yes] 
In which directory do you want to install the documentation files? 
[/usr/share/doc/vmware-tools]
Thepath "/usr/share/doc/vmware-tools" does not exist currently.Thisprogram isgoing to create it, including needed parent directories.Isthis what you want? 
[yes]
The installation of VMware Tools 5.5.1 build-19175 for Linux completed 
successfully. You can decide to remove this software from your system at any 
time by invoking the following command: "/usr/bin/vmware-uninstall-tools.pl". 
Before running VMware Tools for the first time, you need to configure it by 
invoking the following command: "/usr/bin/vmware-config-tools.pl". Do you want 
this program to invoke the command for you now? [yes]
Stopping VMware Tools services in the virtual machine: 
Guest operating system daemon: [ 确定 ]
Trying to find a suitable vmhgfs module for your running kernel. 
The module bld-2.6.9-5.EL-i686-RHEL4 loads perfectly in the running kernel. 
pcnet32 30153 0 
Unloading pcnet32 module 
Trying to find a suitable vmxnet module for your running kernel. 
The module bld-2.6.9-5.EL-i686-RHEL4 loads perfectly in the running kernel. 
Detected X.org version 6.8. 
关闭控制台鼠标服务： [ 确定 ] 
启动控制台鼠标服务： [ 确定 ] 
Please choose one of the following display sizes (1 - 13): 
# 显示分辨率，这里是以 1024x768 为例 
# VMware Tools 安装的时候，会自动修改 X server 的配置文件 
[1] "640x480" 
[2]< "800x600" 
[3] "1024x768" 
[4] "1152x864" 
[5] "1280x800" 
[6] "1152x900" 
[7] "1280x1024" 
[8] "1376x1032" 
[9] "1400x1050" 
[10] "1680x1050" 
[11] "1600x1200" 
[12] "1920x1200" 
[13] "2364x1773" 
Please enter a number between 1 and 13: 
[2] 3
X Window System Version 6.8.2 
Release Date: 9 February 2005 
X Protocol Version 11, Revision 0, Release 6.8.2 
Build Operating System: Linux 2.6.9-34.EL i686 [ELF] 
Current Operating System: Linux rd01.domain 2.6.9-34.EL #1 Wed Mar 8 00:07:35 CST 2006 i686 
Build Date: 04 May 2006 
Build Host: x8664-build.centos.org 
Before reporting problems, check ![url.gif](http://www.xuniji.com/images/skin/small/url.gif)[http://wiki.X.Org](http://wiki.x.org/)
to make sure that you have the latest version. 
Module Loader present 
OSKernel: Linux version 2.6.9-34.EL (buildcentos@build-i386) (gccversion3.4.5 20051201 (Red Hat 3.4.5-2)) #1 Wed Mar 8 00:07:35 CST 2006P 
Markers: (--) probed, (**) from config file, (==) default setting, 
(++) from command line, (!!) notice, (II) informational, 
(WW) warning, (EE) error, (NI) not implemented, (??) unknown. 
(++) Log file: "/tmp/vmware-config0/XF86ConfigLog.3131", Time: Mon Jun 12 20:57:34 2006 
(++) Using config file: "/tmp/vmware-config0/XF86Config.3131" 
(WW) VMWARE(0): Failed to set up write-combining range (0xf0000000,0x1000000) 
X is running fine with the new config file. 
Starting VMware Tools services in the virtual machine: 
Switching to guest configuration: [ 确定 ] 
Guest filesystem driver: [ 确定 ] 
DMA setup: [ 确定 ] 
Guest operating system daemon: [ 确定 ] 
The configuration of VMware Tools 5.5.1 build-19175 for Linux for this running 
kernel completed successfully. 
You must restart your X session before any mouse or graphics changes take 
effect. 
You can now run VMware Tools by invoking the following command: 
"/usr/bin/vmware-toolbox" during an XFree86 session. 
To use the vmxnet driver, restart networking using the following commands: 
/etc/init.d/network stop 
rmmod pcnet32 
rmmod vmxnet 
depmod -a 
modprobe vmxnet 
/etc/init.d/network start 
Enjoy, 
--the VMware team 
[root@rd01 vmware-tools-distrib]# shutdown -r now
# 修改完成之后，重新启动计算机，让 VMware Tools 生效 
![20070210140937oy.jpg](http://www.xuniji.com/upload/forum/200702/20070210140937oy.jpg)
# 重新启动计算机之后，我们就会发觉到，当我们要离开 Guest OS 的时候，不再需要按 Ctrl + Alt 了， 
# 现在我们来共享 Host OS 的文件夹给 Guest 使用，〔VM〕→〔设置 Ctrl + D〕 
# 注：左下角原本都会显示「You do not have VMware Tools installed」，现在我们装了 VMware Tools，就不再显示了 
![2007021014101599.jpg](http://www.xuniji.com/upload/forum/200702/2007021014101599.jpg)
![20070210141225rb.jpg](http://www.xuniji.com/upload/forum/200702/20070210141225rb.jpg)
![200702101412442s.jpg](http://www.xuniji.com/upload/forum/200702/200702101412442s.jpg)
![20070210141250iy.jpg](http://www.xuniji.com/upload/forum/200702/20070210141250iy.jpg)
![20070210141259n1.jpg](http://www.xuniji.com/upload/forum/200702/20070210141259n1.jpg)
![20070210141308jq.jpg](http://www.xuniji.com/upload/forum/200702/20070210141308jq.jpg)
# 这就是共享完毕的画面，然后我们就需要到 Guest OS 内，看看可不可以读取到刚刚共享的目录 
# 顺利的话，我们只要到「/mnt/hgfs」文件夹，就可以看到刚刚共享的文件夹了 
此文章结合            
