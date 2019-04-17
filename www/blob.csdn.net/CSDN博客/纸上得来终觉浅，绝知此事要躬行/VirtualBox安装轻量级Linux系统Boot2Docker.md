# VirtualBox安装轻量级Linux系统Boot2Docker - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月03日 18:46:54[boonya](https://me.csdn.net/boonya)阅读数：882








VirualBox快速安装Docker的轻量级Linux系统BootDocker 。

## VirtualBox

### 介绍

![](https://img-blog.csdn.net/20170703182941574)


VirtualBox 是一款开源虚拟机软件。VirtualBox 是由德国 Innotek 公司开发，由Sun Microsystems公司出品的软件，使用Qt编写，在 Sun 被 Oracle 收购后正式更名成 Oracle VM VirtualBox。Innotek 以 GNU General Public License (GPL) 释出 VirtualBox，并提供二进制版本及 OSE 版本的代码。使用者可以在VirtualBox上安装并且执行Solaris、Windows、DOS、Linux、OS/2 Warp、BSD等系统作为客户端操作系统。现在则由甲骨文公司进行开发，是甲骨文公司xVM虚拟化平台技术的一部份。



VirtualBox号称是最强的免费虚拟机软件，它不仅具有丰富的特色，而且性能也很优异！它简单易用，可虚拟的系统包括Windows（从Windows 3.1到Windows10、Windows Server 2012，所有的Windows系统都支持）、Mac OS X、Linux、OpenBSD、Solaris、IBM OS2甚至Android等操作系统！使用者可以在VirtualBox上安装并且运行上述的这些操作系统！ 与同性质的VMware及Virtual PC比较下，VirtualBox独到之处包括远端桌面协定（RDP）、iSCSI及USB的支持，VirtualBox在客户端操作系统上已可以支持USB 2.0的硬件装置，不过要安装 VirtualBox Extension Pack。

### 下载

virtualbox[:https://www.virtualbox.org/](https://www.virtualbox.org/)




## Boot2Docker
Boot2Docker是专门用于运行Docker容器的轻量级Linux发行版。它完全从RAM运行，是一个小〜38MB的下载和靴子〜5s（YMMV）。

### 特征
- 内核4.4.74与AUFS，Docker v17.06.0-ce  - 使用libcontainer
- 通过/ var / lib / docker上的disk automount进行容器持久化
- SSH密钥通过磁盘自动挂载持久化





### 下载

Boot2Docker: [https://github.com/boot2docker/boot2docker/releases](https://github.com/boot2docker/boot2docker/releases)

### 安装

在虚拟机中新建一个BootDocker的实例：

![](https://img-blog.csdn.net/20170703183848198)


选择iso镜像：

![](https://img-blog.csdn.net/20170703184006926)

配置桥接网络：

![](https://img-blog.csdn.net/20170703184041968)

启动系统：

![](https://img-blog.csdn.net/20170703184150538)


看到以上界面表示成功。查看BootDocker实例系统的Ip：

![](https://img-blog.csdn.net/20170703184259935)

在Windows宿主机上ping：171.17.0.1

![](https://img-blog.csdn.net/20170703184346915)

至此，网络也是通的了，可以正常使用。




