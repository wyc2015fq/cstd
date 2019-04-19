# /etc/rc.d文件目录详解 - xqhrs232的专栏 - CSDN博客
2016年03月16日 17:24:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1096
原文地址::[http://blog.sina.com.cn/s/blog_414d78870102vqj5.html](http://blog.sina.com.cn/s/blog_414d78870102vqj5.html)
相关文章
1、/etc/rc.d/rc与/etc/rc.d/init.d的关系介绍----[http://www.jb51.net/LINUXjishu/86332.html](http://www.jb51.net/LINUXjishu/86332.html)
2、[linux /etc/rc.d/目录的详解](http://blog.chinaunix.net/uid-22690847-id-2117987.html) ----[http://blog.chinaunix.net/uid-22690847-id-2117987.html](http://blog.chinaunix.net/uid-22690847-id-2117987.html)
控制脚本目录/etc/rc.d，该目录下存在各个运行级别的脚本文件，以下是rhel-server-6.3为例的查询结果：
**[root@localhost etc]# cd /etc/rc.d**
**[root@localhost rc.d]# ls**
init.d  rc  rc0.d  rc1.d  rc2.d  rc3.d  rc4.d  rc5.d  rc6.d  rc.local  rc.sysinit
**[root@localhost rc.d]# cd rc0.d**
**[root@localhost rc0.d]# ls -l**
总用量0
lrwxrwxrwx. 1 root root 20 9月   7
 19:53 K01certmonger -> ../init.d/certmonger
lrwxrwxrwx. 1 root root 16 9月   7
 19:59 K01smartd -> ../init.d/smartd
lrwxrwxrwx. 1 root root 22 9月   7
 19:53 K02avahi-daemon -> ../init.d/avahi-daemon
lrwxrwxrwx. 1 root root 17 9月   7
 19:53 K02oddjobd -> ../init.d/oddjobd
lrwxrwxrwx. 1 root root 19 9月   7
 19:55 K02rhsmcertd -> ../init.d/rhsmcertd
lrwxrwxrwx. 1 root root 15 9月   7
 19:55 K03rhnsd -> ../init.d/rhnsd
lrwxrwxrwx. 1 root root 13 9月   7
 19:52 K05atd -> ../init.d/atd
lrwxrwxrwx. 1 root root 17 9月   7
 19:59 K05wdaemon -> ../init.d/wdaemon
lrwxrwxrwx. 1 root root 14 9月   7
 19:52 K10cups -> ../init.d/cups
lrwxrwxrwx. 1 root root 16 9月   7
 19:59 K10psacct -> ../init.d/psacct
lrwxrwxrwx. 1 root root 19 9月   7
 19:53 K10saslauthd -> ../init.d/saslauthd
lrwxrwxrwx. 1 root root 15 9月   7
 2015 K15httpd -> ../init.d/httpd
lrwxrwxrwx. 1 root root 19 9月   7
 19:55 K16abrt-ccpp -> ../init.d/abrt-ccpp
lrwxrwxrwx. 1 root root 15 9月   7
 19:55 K16abrtd -> ../init.d/abrtd
lrwxrwxrwx. 1 root root 19 9月   7
 19:55 K16abrt-oops -> ../init.d/abrt-oops
lrwxrwxrwx. 1 root root 14 9月   7
 19:59 K25sshd -> ../init.d/sshd
lrwxrwxrwx. 1 root root 17 9月   7
 19:53 K30postfix -> ../init.d/postfix
lrwxrwxrwx. 1 root root 24 9月   7
 19:59 K30spice-vdagentd -> ../init.d/spice-vdagentd
lrwxrwxrwx. 1 root root 32 9月   7
 2015 K43vmware-tools-thinprint -> ../init.d/vmware-tools-thinprint
lrwxrwxrwx. 1 root root 17 9月   7
 19:52 K50dnsmasq -> ../init.d/dnsmasq
lrwxrwxrwx. 1 root root 20 9月   7
 19:52 K50netconsole -> ../init.d/netconsole
lrwxrwxrwx. 1 root root 15 9月   7
 19:53 K50snmpd -> ../init.d/snmpd
lrwxrwxrwx. 1 root root 19 9月   7
 19:53 K50snmptrapd -> ../init.d/snmptrapd
lrwxrwxrwx. 1 root root 16 9月   7
 15:29 K50xinetd -> ../init.d/xinetd
lrwxrwxrwx. 1 root root 15 9月   7
 19:53 K60crond -> ../init.d/crond
lrwxrwxrwx. 1 root root 13 9月   7
 19:55 K60nfs -> ../init.d/nfs
lrwxrwxrwx. 1 root root 20 9月   7
 19:55 K69rpcsvcgssd -> ../init.d/rpcsvcgssd
lrwxrwxrwx. 1 root root 16 9月   7
 19:58 K72autofs -> ../init.d/autofs
lrwxrwxrwx. 1 root root 16 9月   7
 19:52 K73ypbind -> ../init.d/ypbind
lrwxrwxrwx. 1 root root 15 9月   7
 19:59 K74acpid -> ../init.d/acpid
lrwxrwxrwx. 1 root root 19 9月   7
 19:53 K74haldaemon -> ../init.d/haldaemon
lrwxrwxrwx. 1 root root 14 9月   7
 2015 K74ntpd -> ../init.d/ntpd
lrwxrwxrwx. 1 root root 15 9月   7
 19:52 K75netfs -> ../init.d/netfs
lrwxrwxrwx. 1 root root 17 9月   7
 19:52 K75ntpdate -> ../init.d/ntpdate
lrwxrwxrwx. 1 root root 19 9月   7
 19:59 K75quota_nld -> ../init.d/quota_nld
lrwxrwxrwx. 1 root root 19 9月   7
 19:52 K75udev-post -> ../init.d/udev-post
lrwxrwxrwx. 1 root root 15 9月   7
 2015 K80kdump -> ../init.d/kdump
lrwxrwxrwx. 1 root root 19 9月   7
 19:58 K83bluetooth -> ../init.d/bluetooth
lrwxrwxrwx. 1 root root 17 9月   7
 15:29 K83nfslock -> ../init.d/nfslock
lrwxrwxrwx. 1 root root 17 9月   7
 15:29 K83rpcgssd -> ../init.d/rpcgssd
lrwxrwxrwx. 1 root root 19 9月   7
 15:29 K83rpcidmapd -> ../init.d/rpcidmapd
lrwxrwxrwx. 1 root root 24 9月   7
 19:52 K84NetworkManager -> ../init.d/NetworkManager
lrwxrwxrwx. 1 root root 24 9月   7
 15:29 K84wpa_supplicant -> ../init.d/wpa_supplicant
lrwxrwxrwx. 1 root root 19 9月   7
 19:52 K85mdmonitor -> ../init.d/mdmonitor
lrwxrwxrwx. 1 root root 20 9月   7
 19:50 K85messagebus -> ../init.d/messagebus
lrwxrwxrwx. 1 root root 20 9月   7
 19:59 K87irqbalance -> ../init.d/irqbalance
lrwxrwxrwx. 1 root root 21 9月   7
 19:54 K87restorecond -> ../init.d/restorecond
lrwxrwxrwx. 1 root root 17 9月   7
 19:51 K87rpcbind -> ../init.d/rpcbind
lrwxrwxrwx. 1 root root 16 9月   7
 19:59 K88auditd -> ../init.d/auditd
lrwxrwxrwx. 1 root root 17 9月   7
 19:52 K88rsyslog -> ../init.d/rsyslog
lrwxrwxrwx. 1 root root 14 9月   7
 15:29 K88sssd -> ../init.d/sssd
lrwxrwxrwx. 1 root root 21 9月   7
 19:51 K89portreserve -> ../init.d/portreserve
lrwxrwxrwx. 1 root root 15 9月   7
 19:52 K89rdisc -> ../init.d/rdisc
lrwxrwxrwx. 1 root root 17 9月   7
 19:52 K90network -> ../init.d/network
lrwxrwxrwx. 1 root root 19 9月   7
 19:51 K92ip6tables -> ../init.d/ip6tables
lrwxrwxrwx. 1 root root 18 9月   7
 19:50 K92iptables -> ../init.d/iptables
lrwxrwxrwx. 1 root root 19 9月   7
 2015 K95firstboot -> ../init.d/firstboot
lrwxrwxrwx. 1 root root 18 9月   7
 19:59 K99cpuspeed -> ../init.d/cpuspeed
lrwxrwxrwx. 1 root root 22 9月   7
 19:59 K99lvm2-monitor -> ../init.d/lvm2-monitor
lrwxrwxrwx. 1 root root 14 9月   7
 19:59 K99rngd -> ../init.d/rngd
lrwxrwxrwx. 1 root root 17 9月   7
 19:59 K99sysstat -> ../init.d/sysstat
lrwxrwxrwx. 1 root root 22 9月   7
 2015 K99vmware-tools -> ../init.d/vmware-tools
lrwxrwxrwx. 1 root root 17 9月   7
 19:52 S00killall -> ../init.d/killall
lrwxrwxrwx. 1 root root 14 9月   7
 19:52 S01halt -> ../init.d/halt
以rc0.d目录下的配置文件为例，rc0.d调用了/etc/rc.d/init.d目录下的程序。
说明：
**/etc/rc.d/init.d**
该文件夹包含所有服务在各个运行等级中的全部启动脚本。一般来说，它们都是标准的shell脚本，遵守最基本的标准。每个脚本最少接受两个参数start和stop，它们分别代表启动和停止服务（如网页服务）。除此之外，init脚本通常还会接受一些额外的选项，如restart（重启服务器）、status（返回服务当前状态）、reload（告知服务从配置文件中重新载入配置）以及force-reload（强制服务重载它的配置）。当用不带参数的方式运行脚本的时候，一般应该返回一个它会接受的参数列表。
**/etc/rc.d/rc0.d～/etc/rc.d/rc6.d**
这些文件夹分别包含每个运行等级对应的init脚本。在实际使用中，它们一般通过符号链接到/etc/init.d文件夹下的实际文件。不过要注意的是，这些文件夹下的init脚本都有一些特别的名字，命名都以S（start）、K（kill）或D（disable）开头，后面跟一个数字。当init进入一个运行等级的时候，它会按照数字顺序运行所有以K开头的脚本并传入stop参数，除非对应的init脚本在前一个运行等级中没有启动。然后init按照数字顺序运行所有以S开头的脚本并传入start参数。任何以D开头的init脚本都会被忽略—这让你可以在指定的运行等级禁止一个脚本，或者你也可以仅仅移除全部符号链接。所以如果你有两个脚本，S01foo和S05bar，init首先会运行S01foo
 start，当它进入特定的运行等级后再执行S05bar start。
**/etc/rc.d/rc.local**
并非所有的发行版都使用了rc.local，通常它是一个留给用户修改的shell脚本。一般会在init进程结束的时候运行它，所以你可以在这里放一些想要运行的额外脚本，而不用再创建自己的init脚本。
**/etc/rc.d/rc.sysinit**
/etc/rc.d/rc.sysinit主要做在各个运行模式中相同的初始化工作，包括设定PATH、设定网络配置（/etc/sysconfig/network）、启动swap分区、设定/proc等等。
**/etc/rc.d/rc**
当运行级别改变时，负责启动/停止各种服务。
