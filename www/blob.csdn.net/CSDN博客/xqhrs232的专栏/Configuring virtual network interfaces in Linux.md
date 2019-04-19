# Configuring virtual network interfaces in Linux - xqhrs232的专栏 - CSDN博客
2018年04月25日 17:20:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：86
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[https://linuxconfig.org/configuring-virtual-network-interfaces-in-linux](https://linuxconfig.org/configuring-virtual-network-interfaces-in-linux)
相关文章
1、How To Assign Multiple IP Addresses To Single Network Card In Linux----[https://www.ostechnix.com/how-to-assign-multiple-ip-addresses-to-single-network-card-in-linux/](https://www.ostechnix.com/how-to-assign-multiple-ip-addresses-to-single-network-card-in-linux/)
# Introduction
Did you know that you can assign more that one IP address to a single physical network interface? This technique is quite useful, for example when working with Apache and virtual hosts, as it allows you to
 access same Apache server by using two different IP addresses.
# Temporary virtual network interface
The process of creating a virtual network interface in Linux is a quite simple matter. It involves a single execution of the ifconfig command.
ifconfig eth0:0 123.123.22.22
The above command will create a new virtual network interface based on original eth0 physical interface. The only most important condition for creating the virtual network interface is the physical network
 interface, as in our case eth0 must exists. The whole example is shown below:
# ifconfig eth0 
eth0 Link encap:Ethernet  HWaddr 3c:97:0e:02:98:c8  
inet addr:192.168.100.23  Bcast:192.168.100.255  
Mask:255.255.255.0# ping 192.168.100.23
PING 192.168.100.23  bytes of data.
64 bytes from 192.168.100.23 time=0.023 ms
64 bytes from 192.168.100.23 time=0.059 m
At this point we can configure a new virtual interface based on eth0. Once you execute the ifconfigcommand the new virtual interface will be immediately
 ready to use.
# ifconfig eth0:0
eth0:0    Link encap:Ethernet  HWaddr 3c:97:0e:02:98:c8  
          UP BROADCAST MULTICAST  MTU:1500  Metric:1
          Interrupt:20 Memory:f1600000-f1620000 
# ifconfig eth0:0 123.123.22.22
# ifconfig eth0:0
eth0:0    Link encap:Ethernet  HWaddr 3c:97:0e:02:98:c8  
          inet addr:123.123.22.22  Bcast:123.255.255.255  Mask:255.0.0.0# ping 123.123.22.22
PING 123.123.22.22 (123.123.22.22) 56(84) bytes of data.
64 bytes from 123.123.22.22: icmp_req=1 ttl=64 time=0.060 ms
64 bytes from 123.123.22.22: icmp_req=2 ttl=64 time=0.057 ms
## Disable virtual network interface
To disable our temporary network interface created earlier we can use the ifconfig command again but with a down flag.
# ifconfig eth0:0 down
# Virtual address permanent assignment
The above configuration would not last after reboot. If you wish the virtual network interface to be permanent you would need to modify config files relevant to your particular Linux distribution. Below, you
 can find information describing this procedure for the major Linux distributions:
## Debian / Ubuntu
### Static
For Debian or Ubuntu Linux you need to edit /etc/network/interfaces file with your favorite text editor and add the following lines:
iface eth0:0 inet static
address 123.123.22.22
netmask 255.0.0.0
broadcast 123.255.255.255
### Dhcp
It is also possible to use virtual network interface with DHCP. In that case, you will need to add to your /etc/network/interfaces the following line.
iface eth0:0 inet dhcp
To apply your change restart the network with:
 # /etc/init.d/networking restart
## Redhat / Fedora / CentOS
### Static
For Redhat, Fedora or CentOS Linux the directory responsible for permanent IP address assignment is /etc/sysconfig/network-scripts. In this directory
 you need to create a file corresponding to your new virtual interface. In our case, that will be a file called: ifcfg-eth0:0 . Create this new file and insert following lines to make your virtual address assignment permanent after reboot:
DEVICE=eth0:0
IPADDR=123.123.22.22
NETMASK=255.0.0.0
NETWORK=123.0.0.0
BROADCAST=123.255.255.255
ONBOOT=yes
### Dhcp
DEVICE=eth0:0
BOOTPROTO=dhcp
ONBOOT=yes
Once ready restart your interfaces with:
# service network restart
# Conclusion
In the old days one physical server served a single website. This is no longer a viable option for hosting websites today so ability of an operating system to create virtual network interface is really a must.
