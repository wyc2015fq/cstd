# Linux如何在系统启动时自动加载模块 - xqhrs232的专栏 - CSDN博客
2018年01月18日 11:09:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：274
原文地址::[http://www.cnblogs.com/image-eye/archive/2011/08/19/2145777.html](http://www.cnblogs.com/image-eye/archive/2011/08/19/2145777.html)
下面是以前学习Linux时写的，后来仔细研究rc.sysinit后发现，只需要修改下列地方就可以了，不必这么麻烦的：
rc.sysinit中有这样的一段代码：
# Load other user-defined modules
for file in /etc/sysconfig/modules/*.modules ; do
[ -x $file ] && $file
done
# Load modules (for backward compatibility with VARs)
if [ -f /etc/rc.modules ]; then
/etc/rc.modules
fi
可见只需要配置两个地方的任何一个就可以了（以加载fuse内核模块为例）
(1) 在/etc/sysconfig/modules/下面创建*.modules文件，参考已经有的*.modules文件，例如我写创建文件my.modules，内容为modprobe fuse
记得最后chmod 755 my.modules
(2) 或者在/etc/rc.modules里面加上modprobe fuse，没有的话创建该文件。
然后reboot，lsmod | grep fuse验证一下就OK了。
==============
Automatically load kernel modules:
为搞清楚如何在系统启动时自动加载模块，搜索了好久，网上有很多人提出这个问题，但都没有正确的答案，无论是中文社区还是英文社区，大家的回答都没有讲到点子上，无非是围绕modprobe.conf、modprobe讲来讲去的，要不就是针对特定问题尝试不同的方法。有的还建议把modprobe modulename写入rc.local，却不曾想，rc.local的执行被放在整个启动顺序的很后面，而启动init.d下面定义的服务却在rc.local前面，那么如果某个服务要用这个模块，就不行了。
在测试LVS时，因为我的Fedora7的Kernel(2.6.21-1)缺省没有加载ip_vs模块，而内核中已经包含编译好的IPVS相关的模块了，放在：/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/下面，有：
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_dh.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_ftp.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_lblc.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_lblcr.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_lc.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_nq.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_rr.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_sed.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_sh.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_wlc.ko
/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/ip_vs_wrr.ko
其中ip_vs.ko是IPVS的基本模块，不加载IPVS就不能工作（运行ipvsadm会报错的），而其他的都是IPVS的调度算法或特定协议的辅助模块，需要时则须加载。
如果系统运行时手动加载则需：modprobe ip_vs 和modprobe ip_vs_sh等。
要了解如何在系统启动时自动加载模块(Automatically load kernel modules)，就得先了解系统是如阿启动的，启动的过程中按什么顺序做了什么，怎么做的，这些启动操作都有那些文件和脚本控制。由于Google和Baidu出来的东西都解决不了问题，而且man modprobe和man
 modprobe.conf发现并不是需要修改的文件。
于是温习鸟哥的[http://linux.vbird.org/](http://linux.vbird.org/)“开机关机流程与Loader”：
1. 整个开机流程是
(1) 载入BIOS的硬件信息，并取得第一个开机装置的代号
(2)读取第一个开机装置的MBR的boot Loader (grub)的开机信息
(3)载入OS Kernel信息，解压Kernel，尝试驱动硬件
(4) Kernel执行init程序并获得run-lebel信息(如3或5)
(5) init执行/etc/rc.d/rc.sysinit
(6)启动内核外挂模块(/etc/modprobe.conf)
(7) init执行run-level的各种Scripts，启动服务
(8) init执行/etc/rc.d/rc.local
(9)执行/bin/login，等待用户Login
(10)Login后进入Shell
看来正确的方式是把需要加载的模块放在(5)或(6)，但正如网络上很多人的尝试，修改modprobe.conf都没有成功(例如在modprobe.conf中增加install ip_vs...)。于是我修改了/etc/rc.d/rc.sysinit就成功加载了。
初步尝试在rc.sysinit最后增加 modprobe.conf ip_vs，重启后lsmod | grep ip_vs，发现成功自动加载了。
于是仿效rc.sysinit中其他模块的加载方法，扩展改脚本文件，在最后增加下来一段：
# load LVS IPVS modules
if [ -d /lib/modules/$unamer/kernel/net/ipv4/ipvs ]; then
for module in /lib/modules/$unamer/kernel/net/ipv4/ipvs/* ; do
module=${module##*/}
module=${module%.ko}
modprobe $module >/dev/null 2>&1
done
fi
就把/lib/modules/2.6.21-1.3194.fc7/kernel/net/ipv4/ipvs/下的所有模块都自动加载了。其中：
if语句检查ipvs模块的目录是否存在
for循环遍历该目录下面的所有文件
module=${module##*/} ：其中##表示从前面删除字符，*/表示删除到最后一个/，如果一个#就表示只删除到第一个/。如果变量后面接##，表示在##后面的字符串取最长的(一直到最后面)，如果接#，表示取最小的一段。
module=${module%.ko}：表示从后面删除.ko。如果变量后面接%%，表示在%%后面的字符串取最长的(一直到最前面)，如果接%，表示取最小的一段。
这样多module的两次修改就得到了模块名，就是文件名不带路径和.ko后缀。
modprobe $module >/dev/null 2>&1：加载模块，输出都指向空设备
这样重启后lsmod | grep ip_vs就会得到：
ip_vs_wrr 6977 0
ip_vs_wlc 6081 0
ip_vs_sh 6593 0
ip_vs_sed 6081 0
ip_vs_rr 6081 0
ip_vs_nq 5953 0
ip_vs_lc 5953 0
ip_vs_lblcr 10565 0
ip_vs_lblc 9797 0
ip_vs_ftp 10053 0
ip_vs_dh 6593 0
ip_vs 79425 22 ip_vs_wrr,ip_vs_wlc,ip_vs_sh,ip_vs_sed,ip_vs_rr,ip_vs_nq,ip_vs_lc,ip_vs_lblcr,ip_vs_lblc,ip_vs_ftp,ip_vs_dh
//=============================================================================
备注::
1>编辑/etc/rc.d/rc.local文件，里面加insmod xxx.ko语句也是可以自动加载起模块的！！！

