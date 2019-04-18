# What’s New In RHEL6.1 ??? - weixin_33985507的博客 - CSDN博客
2011年05月19日 16:36:12[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
在红帽官方网站溜达了一圈，发现了rhel6.1的一些新特性，具体如下所示：
一、硬件支持：
1、网络接口的命名：
在6.0之前的版本中，网卡的命名规则都是以eth[x]进行命名的，而在6.1以后的版本中，网卡的命名依赖于主板上的网卡芯片名称。
2、在线添加CPU和内存。
3、ixgbe、mlx4、e1000e、be2iscsi、e1000等硬件驱动的更新。
二、内核：
首先是Control Groups内容的更新:
- 
Added control group ability to throttle block device I/O to a particular device, either bytes per second or Input/Output Operations Per Second (IOPS)
- 
Allow the creation of Hierarchical block device control groups, providing the ability to easily integrate with libvirt and other userspace
- 
The new block device control group tunable *group_idle*, providing better throughput with control groups while maintaining fairness
- 
The new autogroup feature to reduce latencies, for more interactive tasks during CPU intensive workloads
- 
the new cgsnapshot tool provides the ability to take a snapshot of the current control group configuration
1、Scheduler：
Scheduler updates include:
- 
optimize CPU scheduling algorithms by making better decisions on which CPU's processes should run on based on the system NUMA topology
- 
fix undesirable side effects of tickless timer
- 
several optimizations that prevents load balancing from occuring too often.
- 
more use of RCU locking in the scheduler for better concurrent processing.
- 
and many more optimizations and improvements
2、Performance updates and improvements
- 
Numerous updates and improvements to VM and Transparent Huge Page support
- 
Perf_event updates, and add perf lock feature for better way to analyze lock events
- 
Add ftrace events, such as wakeup latency tracer, to graph tracer: more insight into maximum latency for a task to get scheduled after wakeup, and trace of functions called during that time
- 
Introduce kprobes jump optimization to reduce overhead and enhance SystemTap performance
- 
i7300_edac, i7core_edac update supports monitoring of memory errors on motherboards using intel 7300 chipset
3、Networking updates
- 
Receive Packet Steering (RPS) breaks the bottleneck of having to receive network traffic for a NIC on one CPU
- 
Receive Flow Steering (RFS) allows the optimal CPU to receive network data intended for a specific application
- 
Add getsockopt support for TCP thin-streams to reduce latency from retransmission of lost packets in time-sensitive applications
- 
Add Transparent Proxy (TProxy) support for non-locally bound IPv4 TCP and UDP sockets (similar to Linux 2.2)
4、kdump
kdump is an advanced crash dumping mechanism. When enabled, the system is booted from the context of another kernel. This second kernel reserves a small amount of memory, and its only purpose is to capture the core dump image in case the system crashes.
In Red Hat Enterprise Linux 6.1 introduces the kernel message dumper, which is called when a kernel panic occurs. The kernel message dumper provides easier crash analysis and allows 3rd party kernel message logging to alternative targets.
Additionally, the crashkernel=auto parameter syntax is removed. The default parameter syntax is now crashkernel=:[@offset].
三、桌面环境：
1、Network Manager
In Red Hat Enterprise Linux 6.1, NetworkManager has improved support for the configuration of Wi-Fi Protected Access (WPA) Enterprise and Internet Protocol version 6 (IPv6).
2、Graphics Hardware
Red Hat Enterprise Linux 6.1 Beta provides a range of updates for graphics hardware, most notably:
- 
The new driver for Intel Generation 6 Graphics integrated into the Sandy Bridge processor provides full accelerated 2D and 3D graphics
- 
Added support for the Matrox MGA-G200ER graphics chipset
- 
An increased default resolution (1024 x 768 pixels) on monitors that do not supply Extended Display Identification Data (EDID)
四、Storage:
1、LVM Snapshots of Mirrors:
In Red Hat Enterprise Linux 6.1 the ability to take a snapshot of a mirrored logical volume is a fully supported feature.
2、LVM Stripe of Mirrors
It is now possible to combine RAID0 (striping) and RAID1 (mirroring) in a single logical volume in LVM. Creating a logical volume while simultaneously specifying the number of mirrors ('--mirrors X') and the number of stripes ('--stripes Y') results in a mirror devices whose constituent devices are striped.
五、集群：
- 
Additional watchdog integration allows a node to reboot itself when it loses quorum or is fenced using fence_scsi
- 
Red Hat Enterprise Linux High Availability on Red Hat Enterprise Linux KVM hosts is fully supported
六、虚拟化：
对于6.1中KVM，也有不少的改变，请参考官方文档。
七、常规的软件包更新：
主要包含了openCryptoki、Automated Bug Reporting Tool、OpenLDAP、TigerVNC、tuned这几个软件包的更新。
在技术预览方面，也有不少更改，其中对于btrfs文件系统，在rhel6.1中仍然是试验环境，不能用于生产系统。
官方文档：
[Red Hat Enterprise Linux 6.1 Release Notes](http://docs.redhat.com/docs/en-US/Red_Hat_Enterprise_Linux/96/html/6.1_Release_Notes/index.html)
