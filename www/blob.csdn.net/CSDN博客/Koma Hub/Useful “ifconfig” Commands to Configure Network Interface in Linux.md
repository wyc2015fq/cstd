# Useful “ifconfig” Commands to Configure Network Interface in Linux - Koma Hub - CSDN博客
2019年01月26日 10:15:11[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：304
个人分类：[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)
[https://www.tecmint.com/ifconfig-command-examples/](https://www.tecmint.com/ifconfig-command-examples/)
**ifconfig** in short “**interface configuration**” utility for system/network administration in **Unix/Linux** operating systems to configure, manage and query network interface parameters via command line interface or in a system configuration scripts.
The “**ifconfig**” command is used for displaying current network configuration information, setting up an ip address, netmask or broadcast address to an network interface, creating an alias for network interface, setting up hardware address and enable or disable network interfaces.
This article covers “**15 Useful “ifconfig” Commands**” with their practical examples, that might be very helpful to you in managing and configuring network interfaces in Linux systems.
**Update** : The networking command **ifconfig** is deprecated and replaced by [ip command (Learn 10 Examples of IP Command)](https://www.tecmint.com/ip-command-examples/) in most Linux distributions.
**Don’t Miss:**[ifconfig vs ip: What’s Difference Between Them](https://www.tecmint.com/ifconfig-vs-ip-command-comparing-network-configuration/)
### 1. View All Network Setting
The “**ifconfig**” command with no arguments will display all the active interfaces details. The **ifconfig** command also used to check the assigned IP address of an server.
[root@tecmint ~]# ifconfig
eth0      Link encap:Ethernet  HWaddr 00:0B:CD:1C:18:5A
          inet addr:172.16.25.126  Bcast:172.16.25.63  Mask:255.255.255.224
          inet6 addr: fe80::20b:cdff:fe1c:185a/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:2341604 errors:0 dropped:0 overruns:0 frame:0
          TX packets:2217673 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:293460932 (279.8 MiB)  TX bytes:1042006549 (993.7 MiB)
          Interrupt:185 Memory:f7fe0000-f7ff0000
lo        Link encap:Local Loopback
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:16436  Metric:1
          RX packets:5019066 errors:0 dropped:0 overruns:0 frame:0
          TX packets:5019066 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:2174522634 (2.0 GiB)  TX bytes:2174522634 (2.0 GiB)
tun0      Link encap:UNSPEC  HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00
          inet addr:10.1.1.1  P-t-P:10.1.1.2  Mask:255.255.255.255
          UP POINTOPOINT RUNNING NOARP MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:100
          RX bytes:0 (0.0 b)  TX bytes:0 (0.0 b)
### 2. Display Information of All Network Interfaces
The following **ifconfig** command with **-a** argument will display information of all active or inactive network interfaces on server. It displays the results for **eth0**, **lo**, **sit0** and **tun0**.
[root@tecmint ~]# ifconfig -a
eth0      Link encap:Ethernet  HWaddr 00:0B:CD:1C:18:5A
          inet addr:172.16.25.126  Bcast:172.16.25.63  Mask:255.255.255.224
          inet6 addr: fe80::20b:cdff:fe1c:185a/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:2344927 errors:0 dropped:0 overruns:0 frame:0
          TX packets:2220777 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:293839516 (280.2 MiB)  TX bytes:1043722206 (995.3 MiB)
          Interrupt:185 Memory:f7fe0000-f7ff0000
lo        Link encap:Local Loopback
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:16436  Metric:1
          RX packets:5022927 errors:0 dropped:0 overruns:0 frame:0
          TX packets:5022927 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:2175739488 (2.0 GiB)  TX bytes:2175739488 (2.0 GiB)
sit0      Link encap:IPv6-in-IPv4
          NOARP  MTU:1480  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:0 (0.0 b)  TX bytes:0 (0.0 b)
tun0      Link encap:UNSPEC  HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00
          inet addr:10.1.1.1  P-t-P:10.1.1.2  Mask:255.255.255.255
          UP POINTOPOINT RUNNING NOARP MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:100
          RX bytes:0 (0.0 b)  TX bytes:0 (0.0 b)
### 3. View Network Settings of Specific Interface
Using interface name (**eth0**) as an argument with “**ifconfig**” command will display details of specific network interface.
[root@tecmint ~]# ifconfig eth0
eth0      Link encap:Ethernet  HWaddr 00:0B:CD:1C:18:5A
          inet addr:172.16.25.126  Bcast:172.16.25.63  Mask:255.255.255.224
          inet6 addr: fe80::20b:cdff:fe1c:185a/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:2345583 errors:0 dropped:0 overruns:0 frame:0
          TX packets:2221421 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:293912265 (280.2 MiB)  TX bytes:1044100408 (995.7 MiB)
          Interrupt:185 Memory:f7fe0000-f7ff0000
### 4. How to Enable an Network Interface
The “**up**” or “**ifup**” flag with interface name (**eth0**) activates an network interface, if it is not in active state and allowing to send and receive information. For example, “**ifconfig eth0 up**” or “**ifup eth0**” will activate the **eth0**interface.
[root@tecmint ~]# ifconfig eth0 up
OR
[root@tecmint ~]# ifup eth0
### 5. How to Disable an Network Interface
The “**down**” or “**ifdown**” flag with interface name (**eth0**) deactivates the specified network interface. For example, “**ifconfig eth0 down**” or “**ifdown eth0**” command deactivates the **eth0** interface, if it is in active state.
[root@tecmint ~]# ifconfig eth0 down
OR
[root@tecmint ~]# ifdown eth0
### 6. How to Assign a IP Address to Network Interface
To assign an IP address to an specific interface, use the following command with an interface name (**eth0**) and ip address that you want to set. For example, “**ifconfig eth0 172.16.25.125**” will set the IP address to interface **eth0**.
[root@tecmint ~]# ifconfig eth0 172.16.25.125
### 7. How to Assign a Netmask to Network Interface
Using the “**ifconfig**” command with “**netmask**” argument and interface name as (**eth0**) allows you to define an netmask to an given interface. For example, “**ifconfig eth0 netmask 255.255.255.224**” will set the network mask to an given interface **eth0**.
[root@tecmint ~]# ifconfig eth0 netmask 255.255.255.224
### 8. How to Assign a Broadcast to Network Interface
Using the “**broadcast**” argument with an interface name will set the broadcast address for the given interface. For example, “**ifconfig eth0 broadcast 172.16.25.63**” command sets the broadcast address to an interface **eth0**.
[root@tecmint ~]# ifconfig eth0 broadcast 172.16.25.63
### 9. How to Assign a IP, Netmask and Broadcast to Network Interface
To assign an IP address, Netmask address and Broadcast address all at once using “**ifconfig**” command with all arguments as given below.
[root@tecmint ~]# ifconfig eth0 172.16.25.125 netmask 255.255.255.224 broadcast 172.16.25.63
### 10. How to Change MTU for an Network Interface
The “**mtu**” argument set the maximum transmission unit to an interface. The **MTU** allows you to set the limit size of packets that are transmitted on an interface. The **MTU** able to handle maximum number of octets to an interface in one single transaction. For example, “**ifconfig eth0 mtu 1000**” will set the maximum transmission unit to given set (i.e. **1000**). Not all network interfaces supports **MTU** settings.
[root@tecmint ~]# ifconfig eth0 mtu 1000
### 11. How to Enable Promiscuous Mode
What happens in normal mode, when a packet received by a network card, it verifies that the packet belongs to itself. If not, it drops the packet normally, but in the promiscuous mode is used to accept all the packets that flows through the network card.
Most of the today’s network tools uses the promiscuous mode to capture and analyze the packets that flows through the network interface. To set the promiscuous mode, use the following command.
[root@tecmint ~]# ifconfig eth0 promisc
### 12. How to Disable Promiscuous Mode
To disable promiscuous mode, use the “**-promisc**” switch that drops back the network interface in normal mode.
[root@tecmint ~]# ifconfig eth0 -promisc
### 13. How to Add New Alias to Network Interface
The **ifconfig** utility allows you to configure additional network interfaces using **alias** feature. To add alias network interface of **eth0**, use the following command. Please note that alias network address in same sub-net mask. For example, if your **eth0** network ip address is **172.16.25.125**, then alias ip address must be **172.16.25.127**.
[root@tecmint ~]# ifconfig eth0:0 172.16.25.127
Next, verify the newly created alias network interface address, by using “**ifconfig eth0:0**” command.
[root@tecmint ~]# ifconfig eth0:0
eth0:0    Link encap:Ethernet  HWaddr 00:01:6C:99:14:68
          inet addr:172.16.25.123  Bcast:172.16.25.63  Mask:255.255.255.240
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          Interrupt:17
### 14. How to Remove Alias to Network Interface
If you no longer required an alias network interface or you incorrectly configured it, you can remove it by using the following command.
[root@tecmint ~]# ifconfig eth0:0 down
### 15. How to Change the MAC address of Network Interface
To change the **MAC** (**Media Access Control**) address of an **eth0** network interface, use the following command with argument “**hw ether**“. For example, see below.
[root@tecmint ~]# ifconfig eth0 hw ether AA:BB:CC:DD:EE:FF
These are the most useful commands for configuring network interfaces in **Linux**, for more information and usage of **ifconfig** command use the manpages like “**man ifconfig**” at the terminal. Check out some other networking utilities below.
Other Networking Utilities
- [Tcmpdump](https://www.tecmint.com/12-tcpdump-commands-a-network-sniffer-tool/) — is an command-line packet capture and analyzer tool for monitoring network traffic.
- [Netstat](https://www.tecmint.com/12-tcpdump-commands-a-network-sniffer-tool/) — is an open source command line network monitoring tool that monitors incoming and outgoing network packets traffic.
- [Wireshark](https://www.tecmint.com/wireshark-network-protocol-analyzer-tool-for-rhelcentosfedora/) — is an open source network protocol analyzer that is used to troubleshoot network related issues.
- [Munin](https://www.tecmint.com/install-munin-network-monitoring-in-rhel-cent-6-3-fedora-17/) — is an web based network and system monitoring application that is used to display results in graphs using rrdtool.
- [Cacti](https://www.tecmint.com/install-cacti-network-monitoring-on-rhel-centos-6-3-5-8-and-fedora-17-12/) — is an complete web based monitoring and graphing application for network monitoring.
To get more information and options for any of the above tools, see the manapages by entering “**man toolname**” at the command prompt. For example, to get the information for “**netstat”** tool, use the command as “**man netstat**“.
Ravi Saive
I am Ravi Saive, creator of TecMint. A Computer Geek and Linux Guru who loves to share tricks and tips on Internet. Most Of My Servers runs on Open Source Platform called Linux. **Follow Me**: **[Twitter](https://twitter.com/ravisaive)**, **[Facebook](https://www.facebook.com/ravi.saive)** and **[Google+](https://plus.google.com/u/0/+RaviSaive)**
Your name can also be listed here. Got a tip? [Submit it here](https://www.tecmint.com/submit-articles/) to become an **TecMint** author.
