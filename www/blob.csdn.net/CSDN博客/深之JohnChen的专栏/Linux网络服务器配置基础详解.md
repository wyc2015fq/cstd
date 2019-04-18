# Linux网络服务器配置基础详解 - 深之JohnChen的专栏 - CSDN博客

2005年12月26日 16:55:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1613


                要建立一个安全Linux服务器就首先要了解Linux环境下和网络服务相关的配置文件的含义及如何进行安全的配置。在Linux系统中，TCP/IP网络是通过若干个文本文件进行配置的，也许你需要编辑这些文件来完成联网工作，但是这些配置文件大都可以通过配置命令linuxconf(其中网络部分的配置可以通过netconf命令来实现)命令来实现。下面介绍基本的 TCP/IP网络配置文件。 
* /etc/conf.modules 文件 

该配置文件定义了各种需要在启动时加载的模块的参数信息。这里主要着重讨论关于网卡的配置。在使用Linux做网关的情况下，Linux服务器至少需要配置两块网卡。为了减少启动时可能出现的问题，Linux内核不会自动检测多个网卡。 

对于没有将网卡的驱动编译到内核而是作为模块动态载入的系统若需要安装多块网卡，应该在“conf.modules”文件中进行相应的配置。 

若设备驱动被编译为模块（内核的模块）：对于PCI设备，模块将自动检测到所有已经安装到系统上的设备；对于ISA卡，则需要向模块提供IO地址，以使模块知道在何处寻找该卡，这些信息在“/etc/conf.modules”中提供。 

例如，我们有两块ISA总线的3c509卡，一个IO地址是0x300，另一个是0x320。编辑“conf.modules”文件如下： 

alias eth0 3c509 

alias eth1 3c509 

options 3c509 io=0x300,0x320

这是说明3c509的驱动程序应当分别以eth0或eth1的名称被加载（alias eth0，eth1），并且它们应该以参数io=0x300，0x320被装载，来通知驱动程序到哪里去寻找网卡，其中0x是不可缺少的。 

对于PCI卡，仅仅需要alias命令来使ethN和适当的驱动模块名关联，PCI卡的IO地址将会被自动的检测到。对于PCI卡，编辑“conf.modules”文件如下： 

alias eth0 3c905 

alias eth1 3c905

若驱动已经被编译进了内核：系统启动时的PCI检测程序将会自动找到所有相关的网卡。ISA卡一般也能够被自动检测到，但是在某些情况下，ISA卡仍然需要做下面的配置工作： 

在“/etc/lilo.conf”中增加配置信息，其方法是通过LILO程序将启动参数信息传递给内核。对于ISA卡，编辑“lilo.conf”文件，增加如下内容： 

append=" ether="0,0,eth0 ether="0,0,eth1"

注：先不要在“lilo.conf”中加入启动参数，测试一下你的ISA卡，若失败再使用启动参数。 

如果用传递启动参数的方法，eth0和eth1将按照启动时被发现的顺序来设置。 

* /etc/HOSTNAME 文件 

　　该文件包含了系统的主机名称，包括完全的域名，如：deep.openarch.com。 

*/etc/sysconfig/network-scripts/ifcfg-ethN 文件 

在RedHat中，系统网络设备的配置文件保存在“/etc/sysconfig/network-scripts”目录下，ifcfg-eth0包含第一块网卡的配置信息，ifcfg-eth1包含第二块网卡的配置信息。 

下面是“/etc/sysconfig/network-scripts/ifcfg-eth0”文件的示例： 

DEVICE=eth0 
IPADDR=208.164.186.1 
NETMASK=255.255.255.0 
NETWORK=208.164.186.0 
BROADCAST=208.164.186.255 
ONBOOT=yes 
BOOTPROTO=none 
USERCTL=no若希望手工修改网络地址或在新的接口上增加新的网络界面，可以通过修改对应的文件（ifcfg-ethN）或创建新的文件来实现。 

DEVICE=name　　　　 

name表示物理设备的名字 

IPADDR=addr　　　　 

addr表示赋给该卡的IP地址 

NETMASK=mask　　　　 
mask表示网络掩码 

NETWORK=addr　　　　 

addr表示网络地址 

BROADCAST=addr　　 

addr表示广播地址 

ONBOOT=yes/no　　 

启动时是否激活该卡 

none： 

无须启动协议 

bootp： 

使用bootp协议 

dhcp： 

使用dhcp协议 

USERCTL=yes/no　　　　　　 

是否允许非root用户控制该设备 

*/etc/resolv.conf 文件 

该文件是由域名解析器（resolver，一个根据主机名解析IP地址的库）使用的配置文件，示例如下： 

search openarch.com 
nameserver 208.164.186.1 
nameserver 208.164.186.2 
“search domainname.com”

表示当提供了一个不包括完全域名的主机名时，在该主机名后添加domainname.com的后缀；“nameserver”表示解析域名时使用该地址指定的主机为域名服务器。其中域名服务器是按照文件中出现的顺序来查询的。 

*/etc/host.conf 文件 

该文件指定如何解析主机名。Linux通过解析器库来获得主机名对应的IP地址。下面是一个“/etc/host.conf”的示例： 

order bind,hosts 

　　multi on 

　　ospoof on 

　　“order bind,hosts”

指定主机名查询顺序，这里规定先使用DNS来解析域名，然后再查询“/etc/hosts”文件(也可以相反)。 

“multi on”指定是否“/etc/hosts”文件中指定的主机可以有多个地址，拥有多个IP地址的主机一般称为多穴主机。 

“nospoof on”指不允许对该服务器进行IP地址欺骗。IP欺骗是一种攻击系统安全的手段，通过把IP地址伪装成别的计算机，来取得其它计算机的信任。 

*/etc/sysconfig/network 文件 

该文件用来指定服务器上的网络配置信息，下面是一个示例： 

NETWORK=yes 

RORWARD_IPV4=yes 

HOSTNAME=deep.openarch.com 

GAREWAY=0.0.0.0 

GATEWAYDEV= 

NETWORK=yes/no　　　　 

网络是否被配置； 

FORWARD_IPV4=yes/no　　　　 

是否开启IP转发功能 

HOSTNAME=hostname hostname 

表示服务器的主机名 

GAREWAY=gw-ip　　　　 

gw-ip表示网络网关的IP地址 

GAREWAYDEV=gw-dev　　 

gw-dw表示网关的设备名，如：etho等 

注意：为了和老的软件相兼容，“/etc/HOSTNAME”文件应该用和HOSTNAME=hostname相同的主机名。 

*/etc/hosts 文件 

当机器启动时，在可以查询DNS以前，机器需要查询一些主机名到IP地址的匹配。这些匹配信息存放在/etc/hosts文件中。在没有域名服务器情况下，系统上的所有网络程序都通过查询该文件来解析对应于某个主机名的IP地址。 

下面是一个“/etc/hosts”文件的示例： 

最左边一列是主机IP信息，中间一列是主机名。任何后面的列都是该主机的别名。一旦配置完机器的网络配置文件，应该重新启动网络以使修改生效。使用下面的命令来重新启动网络： 

/etc/rc.d/init.d/network restart

* /etc/inetd.conf 文件 

众所周知，作为服务器来说，服务端口开放越多，系统安全稳定性越难以保证。所以提供特定服务的服务器应该尽可能开放提供服务必不可少的端口，而将与服务器服务无关的服务关闭，比如：一台作为www和ftp服务器的机器，应该只开放80和25端口，而将其他无关的服务如：finger auth等服务关掉，以减少系统漏洞。 

而inetd，也叫作“超级服务器”，就是监视一些网络请求的守护进程，其根据网络请求来调用相应的服务进程来处理连接请求。inetd.conf则是inetd的配置文件。inetd.conf文件告诉inetd监听哪些网络端口，为每个端口启动哪个服务。 

在任何的网络环境中使用Linux系统，第一件要做的事就是了解一下服务器到底要提供哪些服务。不需要的那些服务应该被禁止掉，最好卸载掉，这样黑客就少了一些攻击系统的机会。 

查看“/etc/inetd.conf”文件，了解一下inetd提供哪些服务。用加上注释的方法（在一行的开头加上#号），禁止任何不需要的服务，再给inetd进程发一个SIGHUP信号。 

第一步：把文件的许可权限改成600。 

[root@deep]# chmod 600 /etc/inetd.conf

第二步：确信文件的所有者是root。 

[root@deep]# stat /etc/inetd.conf第三步：编辑“inetd.conf”文件（vi /etc/inetd.conf），禁止所有不需要的服务，如：ftp、 telnet、 shell、 login、 exec、talk、ntalk、 imap、 pop-2、pop-3、finger、auth，等等。 

如果你觉得某些服务有用，可以不禁止这些服务。但是，把这些服务禁止掉，系统受攻击的可能性就会小很多。改变后的“inetd.conf”文件的内容如下面所示：

# To re-read this file after changes, just do a 'killall -HUP inetd' 
# 
#echo stream tcp nowait root internal 
#echo dgram udp wait root internal 
#discard stream tcp nowait root internal 
#discard dgram udp wait root internal 
#daytime stream tcp nowait root internal 
#daytime dgram udp wait root internal 
#chargen stream tcp nowait root internal 
#chargen dgram udp wait root internal 
#time stream tcp nowait root internal 
#time dgram udp wait root internal 
# 
# These are standard services. 
# 
#ftp stream tcp nowait root
/usr/sbin/tcpd in.ftpd -l -a 
#telnet stream tcp nowait root
/usr/sbin/tcpd in.telnetd 
# 
# Shell, login, exec, comsat and talk are BSD protocols. 
# 
#shell stream tcp nowait root
/usr/sbin/tcpd in.rshd 
#login stream tcp nowait root 
/usr/sbin/tcpd in.rlogind 
#exec stream tcp nowait root 
/usr/sbin/tcpd in.rexecd 
#comsat dgram udp wait root 
/usr/sbin/tcpd in.comsat 
#talk dgram udp wait root 
/usr/sbin/tcpd in.talkd 
#ntalk dgram udp wait root 
/usr/sbin/tcpd in.ntalkd 
#dtalk stream tcp wait nobody 
/usr/sbin/tcpd in.dtalkd 
# 
# Pop and imap mail services et al 
# 
#pop-2 stream tcp nowait root 
/usr/sbin/tcpd ipop2d 
#pop-3 stream tcp nowait root 
/usr/sbin/tcpd ipop3d 
#imap stream tcp nowait root 
/usr/sbin/tcpd imapd 
# 
# The Internet UUCP service. 
# 
#uucp stream tcp nowait uucp 
/usr/sbin/tcpd /usr/lib/uucp/uucico -l 
# 
# Tftp service is provided primarily 
for booting. Most sites 
# run this only on machines acting as
"boot servers." Do not uncomment 
# this unless you *need* it. 
# 
#tftp dgram udp wait root
/usr/sbin/tcpd in.tftpd 
#bootps dgram udp wait root 
/usr/sbin/tcpd bootpd 
# 
# Finger, systat and netstat give out 
user information which may be 
# valuable to potential "system crackers."
Many sites choose to disable 
# some or all of these services to improve security. 
# 
#finger stream tcp nowait root 
/usr/sbin/tcpd in.fingerd 
#cfinger stream tcp nowait root 
/usr/sbin/tcpd in.cfingerd 

　　#systat stream tcp nowait guest 
/usr/sbin/tcpd /bin/ps -auwwx 

　　#netstat stream tcp nowait guest 
/usr/sbin/tcpd /bin/netstat -f inet 

　　# 

　　# Authentication 

　　# 

　　#auth stream tcp nowait nobody
/usr/sbin/in.identd in.identd -l -e -o 

　　# 

　　# End of inetd.conf

注意：改变了“inetd.conf”文件之后，别忘了给inetd进程发一个SIGHUP信号（killall –HUP inetd）。 

[root@deep /root]# killall -HUP inetd

第四步： 

为了保证“inetd.conf”文件的安全，可以用chattr命令把它设成不可改变。把文件设成不可改变的只要用下面的命令： 

[root@deep]# chattr +i /etc/inetd.conf

这样可以避免“inetd.conf”文件的任何改变（意外或是别的原因）。一个有“i”属性的文件是不能被改动的：不能删除或重命名，不能创建这个文件的链接，不能往这个文件里写数据。只有系统管理员才能设置和清除这个属性。如果要改变inetd.conf文件，你必须先清除这个不允许改变的标志： 

[root@deep]# chattr -i /etc/inetd.conf

但是对于诸如sendmail，named，www等服务，由于它们不象finger，telnet等服务，在请求到来时由inet守护进程启动相应的进程提供服务，而是在系统启动时，作为守护进程运行的。 

而对于redhat linux，提供了一个linuxconfig命令，可以通过它在图形界面下交互式地设置是否在启动时运行相关服务。也可以通过命令来设置是否启动时启动某个服务，如：[root@deep]# chkconfig –level 35 named off。 

