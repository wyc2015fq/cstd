# Hyper-V 2016 系列教程53 What's new in Hyper-V on Windows Server 2016 - weixin_33985507的博客 - CSDN博客
2017年09月22日 21:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
Translate from below link, copyright belong to below link author.
本文中内容从下面的链接翻译过来，版权属于下面链接中的外国友人。
[https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/what-s-new-in-hyper-v-on-windows](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/what-s-new-in-hyper-v-on-windows)
      This article explains the new and changed functionality of Hyper-V on Windows Server 2016 and Microsoft Hyper-V Server 2016. To use new features on virtual machines created with Windows Server 2012 R2 and moved or imported to a server that runs Hyper-V on Windows Server 2016, you'll need to manually upgrade the virtual machine configuration version. For instructions, see [Upgrade virtual machine version](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/deploy/upgrade-virtual-machine-version-in-hyper-v-on-windows-or-windows-server).
本文章解释了在Windows Server 2016和Microsoft hyper-v Server 2016中，Hyper-V的新功能。要在 Windows Server 2012 R2创建的虚拟机中使用Hyper-V 2016的新特性的话，只有将其移动或导入到在Windows Server 2016中运行的Hyper-V的服务器上，您需要手动升级虚拟机配置版本。关于指令，请参阅升级虚拟机版本。
Here's what's included in this article and whether the functionality is new or updated.
如下面是本文中包含的新功能的内容。
## Compatible with Connected Standby (new)
## 与已连接的备件兼容(新)
When the Hyper-V role is installed on a computer that uses the Always On/Always Connected (AOAC) power model, theConnected Standby power state is now available.
当Hyper-V角色安装在一台高可用性或者AOAC电源模型的计算机上时，已连接的备用电源状态现在可用。
## Discrete device assignment (new)
离散设备分配(新)
This feature lets you give a virtual machine direct and exclusive access to some PCIe hardware devices. Using a device in this way bypasses the Hyper-V virtualization stack, which results in faster access. For details on supported hardware, see "Discrete device assignment" in [System requirements for Hyper-V on Windows Server 2016](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/system-requirements-for-hyper-v-on-windows). For details, including how to use this feature and considerations, see the post "[Discrete Device Assignment -- Description and background](http://blogs.technet.com/b/virtualization/archive/2015/11/19/discrete-device-assignment.aspx)" in the Virtualization blog.
本特点允许您为某些PCIe硬件设备提供虚拟机直接和独占的访问权限。使用这样的设备可以绕过Hyper-V虚拟栈，从而加快系统访问速度。有关支持硬件的详细信息，请参阅Windows Server 2016上的“Hyper-V”系统需求的“离散设备分配”。有关细节，包括如何使用这个特性和注意事项，请参阅“离散设备分配—描述和背景”在虚拟化博客中。
## Encryption support for the operating system disk in generation 1 virtual machines (new)
## 在第1代虚拟机(new)中对操作系统磁盘的加密支持
You can now protect the operating system disk using BitLocker drive encryption in generation 1 virtual machines. A new feature, key storage, creates a small, dedicated drive to store the system drive’s BitLocker key. This is done instead of using a virtual Trusted Platform Module (TPM), which is available only in generation 2 virtual machines. To decrypt the disk and start the virtual machine, the Hyper-V host must either be part of an authorized guarded fabric or have the private key from one of the virtual machine's guardians. Key storage requires a version 8 virtual machine. For information on virtual machine version, see [Upgrade virtual machine version in Hyper-V on Windows 10 or Windows Server 2016](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/deploy/upgrade-virtual-machine-version-in-hyper-v-on-windows-or-windows-server).
现在，您可以在第一代虚拟机中使用BitLocker驱动器加密保护操作系统磁盘。这个新的特性，密钥存储，创建一个小型的专用驱动器来存储系统驱动器的BitLocker密钥。这是完成的，而不是使用虚拟可信平台模块(TPM)，它只在第2代虚拟机中可用。要解密磁盘并启动虚拟机，Hyper-V主机必须是经过授权的保护结构的一部分，或者从虚拟机的一个监护器中获得私钥。密钥存储需要一个版本8的虚拟机。有关虚拟机版本的信息，请参见Windows 10或Windows Server 2016上的Hyper-V升级虚拟机版本。
Host resource protection (new)
主机资源保护(新)
This feature helps prevent a virtual machine from using more than its share of system resources by looking for excessive levels of activity. This can help prevent a virtual machine's excessive activity from degrading the performance of the host or other virtual machines. When monitoring detects a virtual machine with excessive activity, the virtual machine is given fewer resources. This monitoring and enforcement is off by default. Use Windows PowerShell to turn it on or off. To turn it on, run this command:
这一特性有助于防止虚拟机通过查找过高的活动级别来使用超过其共享的系统资源。这有助于防止虚拟机的过度活动降低主机或其他虚拟机的性能。当监视检测到一个活动过度的虚拟机时，虚拟机的资源就会减少。这种监视和强制执行在默认情况下是关闭的。使用Windows PowerShell打开或关闭它，打开它，运行以下命令:
Set-VMProcessor -EnableHostResourceProtection $true
For details about this cmdlet, see [Set-VMProcessor](https://technet.microsoft.com/library/hh848533.aspx).
有关这个cmdlet的详细信息，请参见set-vmprocessor。
## Hot add and remove for network adapters and memory (new)
热添加和删除网络适配器和内存(新)
You can now add or remove a network adapter while the virtual machine is running, without incurring downtime. This works for generation 2 virtual machines that run either Windows or Linux operating systems.
您现在可以在虚拟机运行时添加或删除网络适配器，而不会导致停机。这适用于运行Windows或Linux操作系统的第二代虚拟机。
You can also adjust the amount of memory assigned to a virtual machine while it's running, even if you haven't enabled Dynamic Memory. This works for both generation 1 and generation 2 virtual machines, running Windows Server 2016 or Windows 10.
您还可以在运行时调整分配给虚拟机的内存数量，即使您没有启用动态内存。这适用于第一代和第二代虚拟机，运行Windows Server 2016或Windows 10。
## Hyper-V Manager improvements (updated)
hyper-v管理者改进(更新)
- 
Alternate credentials support - You can now use a different set of credentials in Hyper-V Manager when you connect to another Windows Server 2016 or Windows 10 remote host. You can also save these credentials to make it easier to log on again.
备用凭证支持——当您连接到另一个Windows Server 2016或Windows 10远程主机时，您现在可以在hyper-v管理器中使用不同的凭证。您还可以保存这些凭据，以便更容易地登录。
- 
Manage earlier versions - With Hyper-V Manager in Windows Server 2016 and Windows 10, you can manage computers running Hyper-V on Windows Server 2012, Windows 8, Windows Server 2012 R2 and Windows 8.1.
管理更早的版本——在Windows Server 2016和Windows 10中，你可以在Windows Server 2012、Windows 8、Windows Server 2012 R2和Windows 8.1中管理运行Hyper-V的电脑。
- 
Updated management protocol - Hyper-V Manager now communicates with remote Hyper-V hosts using the WS-MAN protocol, which permits CredSSP, Kerberos or NTLM authentication. When you use CredSSP to connect to a remote Hyper-V host, you can do a live migration without enabling constrained delegation in Active Directory. The WS-MAN-based infrastructure also makes it easier to enable a host for remote management. WS-MAN connects over port 80, which is open by default.
更新的管理协议——Hyper-V管理器现在使用ws-man协议与远程Hyper-V主机通信，该协议允许CredSSP、Kerberos或NTLM身份验证。当您使用CredSSP连接到远程Hyper-V主机时，您可以进行动态迁移，而不需要在Active Directory中启用受限的委托。基于ws-mana的基础设施也使支持远程管理的主机变得更加容易。ws-man连接端口80，默认情况下是打开的。
## Integration services delivered through Windows Update (updated)通过Windows Update(更新)提供的集成服务
Updates to integration services for Windows guests are distributed through Windows Update. For service providers and private cloud hosters, this puts the control of applying updates into the hands of the tenants who own the virtual machines. Tenants can now update their Windows virtual machines with all updates, including the integration services, using a single method. For details about integration services for Linux guests, see [Linux and FreeBSD Virtual Machines on Hyper-V](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/supported-linux-and-freebsd-virtual-machines-for-hyper-v-on-windows).
对Windows客户的集成服务的更新是通过Windows更新发布的。对于服务提供商和私有云服务提供商来说，这将控制将更新应用到拥有虚拟机的租户的手中。现在，租户可以使用单一的方法更新他们的Windows虚拟机，包括所有的更新，包括集成服务。有关Linux客户的集成服务的详细信息，请参阅“Hyper-V”上的Linux和FreeBSD虚拟机。
##### Important 重要
The vmguest.iso image file is no longer needed, so it isn't included with Hyper-V on Windows Server 2016.
这个vmguest.iso映像文件不再需要，因此它不会包含在Windows Server 2016的hyper-v功能中。
## Linux Secure Boot (new)Linux安全引导(新)
Linux operating systems running on generation 2 virtual machines can now boot with the Secure Boot option enabled. Ubuntu 14.04 and later, SUSE Linux Enterprise Server 12 and later, Red Hat Enterprise Linux 7.0 and later, and CentOS 7.0 and later are enabled for Secure Boot on hosts that run Windows Server 2016. Before you boot the virtual machine for the first time, you must configure the virtual machine to use the Microsoft UEFI Certificate Authority. You can do this from Hyper-V Manager, Virtual Machine Manager, or an elevated Windows Powershell session. For Windows PowerShell, run this command:
运行在第2代虚拟机上的Linux操作系统现在可以使用启用的安全启动选项启动。Ubuntu 14.04和稍后，SUSE Linux Enterprise Server 12和之后，Red Hat Enterprise Linux 7.0和稍后，以及CentOS 7.0和稍后在运行Windows Server 2016的主机上启用了安全引导。在您第一次启动虚拟机之前，您必须配置虚拟机以使用Microsoft UEFI证书授权。您可以从hyper-v管理器、虚拟机管理器或更高的Windows Powershell会话中执行此操作。对于Windows PowerShell，运行以下命令:
Set-VMFirmware vmname -SecureBootTemplate MicrosoftUEFICertificateAuthority
For more information about Linux virtual machines on Hyper-V, see [Linux and FreeBSD Virtual Machines on Hyper-V](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/supported-linux-and-freebsd-virtual-machines-for-hyper-v-on-windows). For more information about the cmdlet, see [Set-VMFirmware](https://technet.microsoft.com/library/dn464287.aspx).
有关Hyper-V上的Linux虚拟机的更多信息，请参阅“Hyper-V”上的Linux和FreeBSD虚拟机。有关cmdlet的更多信息，请参阅set-vm固件。
## More memory and processors for generation 2 virtual machines and Hyper-V hosts (updated)
第二代虚拟机和Hyper-V主机(更新)的更多内存和处理器
Starting with version 8, generation 2 virtual machines can use significantly more memory and virtual processors. Hosts also can be configured with significantly more memory and virtual processors than were previously supported. These changes support new scenarios such as running e-commerce large in-memory databases for online transaction processing (OLTP) and data warehousing (DW). The Windows Server blog recently published the performance results of a virtual machine with 5.5 terabytes of memory and 128 virtual processors running 4 TB in-memory database. Performance was greater than 95% of the performance of a physical server. For details, see [Windows Server 2016 Hyper-V large-scale VM performance for in-memory transaction processing](https://blogs.technet.microsoft.com/windowsserver/2016/09/28/windows-server-2016-hyper-v-large-scale-vm-performance-for-in-memory-transaction-processing/). For details about virtual machine versions, see [Upgrade virtual machine version in Hyper-V on Windows 10 or Windows Server 2016](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/deploy/upgrade-virtual-machine-version-in-hyper-v-on-windows-or-windows-server). For the full list of supported maximum configurations, see [Plan for Hyper-V scalability in Windows Server 2016](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/plan/plan-for-hyper-v-scalability-in-windows-server-2016).
   从第8版开始，第2代虚拟机可以使用更大的内存和虚拟处理器。主机也可以配置比以前支持的多得多的内存和虚拟处理器。这些更改支持新的场景，比如在在线事务处理(OLTP)和数据仓库(DW)中运行电子商务大型内存数据库。最近，Windows Server博客发布了一个虚拟机的性能结果，该机器拥有5.5兆字节的内存和128个虚拟处理器，运行着4 TB的内存数据库。性能超过了物理服务器95%的性能。有关细节，请参阅Windows Server 2016超级v大型虚拟机在内存中的事务处理中的性能。有关虚拟机版本的详细信息，请参阅Windows 10或Windows Server 2016上的升级版虚拟机版本。对于支持的最大配置的完整列表，请参见Windows Server 2016中的Hyper-V可伸缩性计划。
## Nested virtualization (new)嵌套虚拟化(新)
This feature lets you use a virtual machine as a Hyper-V host and create virtual machines within that virtualized host. This can be especially useful for development and test environments. To use nested virtualization, you'll need:
该特性允许您使用虚拟机作为Hyper-V主机，并在虚拟主机中创建虚拟机。这对于开发和测试环境尤其有用。要使用嵌套的虚拟化，您需要:
- 
To run at least Windows Server 2016 or Windows 10 on both the physical Hyper-V host and the virtualized host.在Hyper-V主机和虚拟主机上至少运行Windows Server 2016或Windows 10。
- 
A processor with Intel VT-x (nested virtualization is available only for Intel processors at this time).一个带有Intel vt-x的处理器(嵌套的虚拟化技术目前只适用于Intel处理器)。
For details and instructions, see [Nested Virtualization](https://msdn.microsoft.com/virtualization/hyperv_on_windows/user_guide/nesting).有关详细信息和说明，请参阅嵌套的虚拟化。
## Networking features (new)网络功能(新)
New networking features include:新的网络功能包括:
- 
Remote direct memory access (RDMA) and switch embedded teaming (SET). You can set up RDMA on network adapters bound to a Hyper-V virtual switch, regardless of whether SET is also used. SET provides a virtual switch with some of same capabilities as NIC teaming. For details, see [Remote Direct Memory Access (RDMA) and Switch Embedded Teaming (SET)](https://docs.microsoft.com/zh-cn/windows-server/networking/sdn/software-defined-networking#bkmk_rdma).
远程直接内存访问(RDMA)和转换嵌入式协作(SET)。您可以在网络适配器上设置RDMA，绑定到一个Hyper-V虚拟交换机，而不管是否使用了设置。SET提供了一个虚拟交换机，它具有与NIC合作的一些功能。有关详细信息，请参阅远程直接内存访问(RDMA)和切换嵌入式协作(SET)。
- 
Virtual machine multi queues (VMMQ). Improves on VMQ throughput by allocating multiple hardware queues per virtual machine. The default queue becomes a set of queues for a virtual machine, and traffic is spread between the queues.
虚拟机多队列(VMMQ)。通过在每个虚拟机上分配多个硬件队列来提高VMQ吞吐量。默认队列成为虚拟机的一组队列，并且在队列之间传播流量。
- 
Quality of service (QoS) for software-defined networks. Manages the default class of traffic through the virtual switch within the default class bandwidth.
软件定义网络的服务质量(QoS)。通过默认的类带宽中的虚拟交换机来管理默认的流量。
For more about new networking features, see [What's new in Networking](https://docs.microsoft.com/zh-cn/windows-server/networking/what-s-new-in-networking).要了解更多关于网络新特性的信息，请参阅网络中的新功能。
## Production checkpoints (new)生产检查点(新)
Production checkpoints are "point-in-time" images of a virtual machine. These give you a way to apply a checkpoint that complies with support policies when a virtual machine runs a production workload. Production checkpoints are based on backup technology inside the guest instead of a saved state. For Windows virtual machines, the Volume Snapshot Service (VSS) is used. For Linux virtual machines, the file system buffers are flushed to create a checkpoint that's consistent with the file system. If you'd rather use checkpoints based on saved states, choose standard checkpoints instead. For details, see[Choose between standard or production checkpoints](https://technet.microsoft.com/library/dn872510.aspx).
生产检查点是虚拟机的“时间点”映像。当虚拟机运行生产工作负载时，这些方法可以让您应用一个符合支持策略的检查点。生产检查点是基于客户的备份技术而不是保存状态。对于Windows虚拟机，使用了卷快照服务(VSS)。对于Linux虚拟机，文件系统缓冲区被刷新，以创建一个与文件系统一致的检查点。如果您宁愿使用基于保存状态的检查点，则选择标准的检查点。有关细节，在标准或生产检查点之间进行选择
##### Important (重要)
New virtual machines use production checkpoints as the default.
新的虚拟机使用生产检查点作为默认设置。
## Rolling Hyper-V Cluster upgrade (new)
回滚Hyper-V集群升级(新)
You can now add a node running Windows Server 2016 to a Hyper-V Cluster with nodes running Windows Server 2012 R2. This allows you to upgrade the cluster without downtime. The cluster runs at a Windows Server 2012 R2 feature level until you upgrade all nodes in the cluster and update the cluster functional level with the Windows PowerShell cmdlet,[Update-ClusterFunctionalLevel](https://technet.microsoft.com/library/mt589702.aspx).
   现在可以将运行Windows Server 2016的节点添加到运行Windows Server 2012 R2的Hyper-V集群中。这允许您在不停机的情况下升级集群。该集群运行在Windows Server 2012 R2特性级别上，直到您升级集群中的所有节点，并使用Windows PowerShell cmdlet、update-clusterfunctionallevel更新集群功能级别。
##### Important 重要的
After you update the cluster functional level, you can't return it to Windows Server 2012 R2.在更新集群功能级别之后，您无法将其返回到Windows Server 2012 R2中。
For a Hyper-V cluster with a functional level of Windows Server 2012 R2 with nodes running Windows Server 2012 R2 and Windows Server 2016, note the following:对于一个具有Windows Server 2012 R2功能级别的Hyper-V集群，以及运行Windows Server 2012 R2和Windows Server 2016的节点，请注意以下:
- 
Manage the cluster, Hyper-V, and virtual machines from a node running Windows Server 2016 or Windows 10.从运行Windows Server 2016或Windows 10的节点管理集群、Hyper-v和虚拟机。
- 
You can move virtual machines between all of the nodes in the Hyper-V cluster.您可以在hyper-v集群中的所有节点之间移动虚拟机。
- 
To use new Hyper-V features, all nodes must run Windows Server 2016 and the cluster functional level must be updated.要使用新的Hyper-v特性，所有节点必须运行Windows Server 2016，并且必须更新集群功能级别。
- 
The virtual machine configuration version for existing virtual machines isn't upgraded. You can upgrade the configuration version only after you upgrade the cluster functional level.现有虚拟机的虚拟机配置版本没有升级。只有在升级了集群功能级别之后，才可以升级配置版本。
- 
Virtual machines that you create are compatible with Windows Server 2012 R2, virtual machine configuration level 5.您所创建的虚拟机与Windows Server 2012 R2、虚拟机配置级别5是兼容的。
After you update the cluster functional level:更新集群功能级别之后:
- 
You can enable new Hyper-V features.您可以启用新的Hyper-V特性。
- 
To make new virtual machine features available, use the Update-VmConfigurationVersion cmdlet to manually update the virtual machine configuration level. For instructions, see [Upgrade virtual machine version](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/deploy/upgrade-virtual-machine-version-in-hyper-v-on-windows-or-windows-server).要使新的虚拟机功能可用，可以使用update-vmconfigurationversion cmdlet手动更新虚拟机配置级别。关于指令，请参阅升级虚拟机版本。
- 
You can't add a node to the Hyper-V Cluster that runs Windows Server 2012 R2.您不能向运行Windows Server 2012 R2的hyper-v集群添加节点。
##### Note
Hyper-V on Windows 10 doesn't support failover clustering.Windows 10上的hyper-v不支持故障转移集群。
For details and instructions, see the [Cluster Operating System Rolling Upgrade](https://technet.microsoft.com/library/dn850430.aspx).有关详细信息和说明，请参见集群操作系统的滚动升级。
## Shared virtual hard disks (updated)共享虚拟硬盘(更新)
You can now resize shared virtual hard disks (.vhdx files) used for guest clustering, without downtime. Shared virtual hard disks can be grown or shrunk while the virtual machine is online. Guest clusters can now also protect shared virtual hard disks by using Hyper-V Replica for disaster recovery.现在可以重新调整共享虚拟硬盘的大小了。vhdx文件)用于客户集群，没有停机时间。共享虚拟硬盘可以在虚拟机在线的情况下进行增长或缩小。现在，通过使用Hyper-V副本进行灾难恢复，客户集群也可以保护共享的虚拟硬盘。
Enable replication on the collection. Enabling replication on a collection is only exposed through the WMI interface. See the documentation for [Msvm_CollectionReplicationService class](https://msdn.microsoft.com/library/mt167787%28v=vs.85%29.aspx) for more details. You cannot manage replication of a collection through PowerShell cmdlet or UI. The VMs should be on hosts that are part of a Hyper-V cluster to access features that are specific to a collection. This includes Shared VHD - shared VHDs on stand-alone hosts are not supported by Hyper-V Replica.在集合上启用复制。在一个集合上启用复制只能通过WMI接口公开。更多细节请参见Msvm_CollectionReplicationService类的文档。您不能通过PowerShell cmdlet或UI管理一个集合的复制。vm应该位于Hyper-v集群的一部分的主机上，以访问特定于集合的特性。这包括在独立主机上共享的VHD-共享VHDs不支持Hyper-v副本。
Follow the guidelines for shared VHDs in [Virtual Hard Disk Sharing Overview](https://technet.microsoft.com/en-us/library/dn281956.aspx), and be sure that your shared VHDs are part of a guest cluster.在虚拟硬盘共享概述中遵循共享VHDs的指导原则，并确保共享的VHDs是客户集群的一部分。
A collection with a shared VHD but no associated guest cluster cannot create reference points for the collection (regardless of whether the shared VHD is included in the reference point creation or not).一个带有共享VHD的集合，但是没有关联的来宾集群不能为该集合创建引用点(无论是否包含共享的VHD是否包含在引用点的创建中)。
## Shielded virtual machines (new)保护虚拟机(新)
Shielded virtual machines use several features to make it harder for Hyper-V administrators and malware on the host to inspect, tamper with, or steal data from the state of a shielded virtual machine. Data and state is encrypted, Hyper-V administrators can't see the video output and disks, and the virtual machines can be restricted to run only on known, healthy hosts, as determined by a Host Guardian Server. For details, see [Guarded Fabric and Shielded VMs](http://go.microsoft.com/fwlink/?LinkId=746381).受保护的虚拟机使用了几个特性，使得在主机上的Hyper-V管理员和恶意软件更难检查、篡改或从屏蔽虚拟机的状态中窃取数据。数据和状态是经过加密的，Hyper-V管理员不能看到视频输出和磁盘，虚拟机只能在已知的、健康的主机上运行，这是由主机守护服务器决定的。有关细节，请参阅保护布和保护虚拟机。
##### Note 备注：
As of Technical Preview 5, shielded virtual machines are compatible with Hyper-V Replica. To replicate a shielded virtual machine, the host you want to replicate to must be authorized to run that shielded virtual machine.在技术预览版本5中，屏蔽虚拟机与Hyper-v副本兼容。要复制一个受保护的虚拟机，您想要复制的主机必须被授权运行该虚拟机。
## Start order priority for clustered virtual machines (new)为集群虚拟机(新)启动订单优先级
This feature gives you more control over which clustered virtual machines are started or restarted first. This makes it easier to start virtual machines that provide services before virtual machines that use those services. Define sets, place virtual machines in sets, and specify dependencies. Use Windows PowerShell cmdlets to manage the sets, such as New-ClusterGroupSet, Get-ClusterGroupSet, and Add-ClusterGroupSetDependency. .这个特性使您可以更控制集群虚拟机的启动或重新启动。这使得在使用这些服务的虚拟机之前，可以更容易地启动虚拟机。定义集合，在集合中放置虚拟机，并指定依赖项。使用Windows PowerShell cmdlets管理集,如New-ClusterGroupSet Get-ClusterGroupSet,Add-ClusterGroupSetDependency。。
## Storage quality of service (QoS) (updated)服务的存储质量(QoS)(更新)
You can now create storage QoS policies on a Scale-Out File Server and assign them to one or more virtual disks on Hyper-V virtual machines. Storage performance is automatically readjusted to meet policies as the storage load fluctuates. For details, see [Storage Quality of Service](https://docs.microsoft.com/zh-cn/windows-server/storage/storage-qos/storage-qos-overview).现在，您可以在一个扩展的文件服务器上创建存储QoS策略，并将它们分配到Hyper-v虚拟机上的一个或多个虚拟磁盘上。存储性能随着存储负载的波动而自动调整以满足策略。有关细节，请参阅服务的存储质量。
## Virtual machine configuration file format (updated)虚拟机配置文件格式(更新)
Virtual machine configuration files use a new format that makes reading and writing configuration data more efficient. The format also makes data corruption less likely if a storage failure occurs. Virtual machine configuration data files use a .vmcx file name extension and runtime state data files use a .vmrs file name extension.虚拟机配置文件使用一种新的格式，使读取和写入配置数据更加高效。如果发生存储故障，这种格式还会降低数据的损坏程度。虚拟机配置数据文件使用a.vmcx文件名称扩展和运行时状态数据文件使用.vmrs文件扩展名。
##### Important 重要
The .vmcx file name extension indicates a binary file. Editing .vmcx or .vmrs files isn't supported. .vmcx文件扩展名表示一个二进制文件。编辑.vmcx或.vmrs文件不受支持。
## Virtual machine configuration version (updated)虚拟机配置版本(更新)
The version represents the compatibility of the virtual machine's configuration, saved state, and snapshot files with the version of Hyper-V. Virtual machines with version 5 are compatible with Windows Server 2012 R2 and can run on both Windows Server 2012 R2 and Windows Server 2016 . Virtual machines with versions introduced in Windows Server 2016 won't run in Hyper-V on Windows Server 2012 R2.该版本表示虚拟机的配置、保存状态和带有Hyper-V版本的快照文件的兼容性。版本5的虚拟机与Windows Server 2012 R2兼容，可以在Windows Server 2012 R2和Windows Server 2016上运行。在Windows Server 2016中引入的虚拟机将不会在Windows Server 2012 R2中运行Hyper-V。
If you move or import a virtual machine to a server that runs Hyper-V on Windows Server 2016 from Windows Server 2012 R2, the virtual machine's configuration isn't automatically updated. This means you can move the virtual machine back to a server that runs Windows Server 2012 R2. But, this also means you can't use the new virtual machine features until you manually update the version of the virtual machine configuration.如果您将一台虚拟机转移到一台服务器上，并将其从Windows server 2012 R2上运行到Windows服务器上，那么虚拟机的配置就不会自动更新。这意味着您可以将虚拟机转回运行Windows server 2012 R2的服务器。但是，这也意味着在手动更新虚拟机配置的版本之前，您不能使用新的虚拟机特性。
For instructions on checking and upgrading the version, see [Upgrade virtual machine version](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/deploy/upgrade-virtual-machine-version-in-hyper-v-on-windows-or-windows-server). This article also lists the version in which some features were introduced.有关检查和升级版本的说明，请参阅升级虚拟机版本。本文还列出了引入一些特性的版本。
##### Important 重要
- 
After you update the version, you can't move the virtual machine to a server that runs Windows Server 2012 R2.在更新版本之后，您不能将虚拟机移动到运行Windows server 2012 R2的服务器上。
- 
You can't downgrade the configuration to a previous version.您不能将配置降级为以前的版本。
- 
The Update-VMVersion cmdlet is blocked on a Hyper-V Cluster when the cluster functional level is Windows Server 2012 R2.当集群功能级别是Windows Server 2012 R2时，更新-vmversion cmdlet在一个Hyper-v集群上被阻塞。
## Virtualization-based security for generation 2 virtual machines (new)第2代虚拟机的基于虚拟化的安全性(新)
Virtualization-based security powers features such as Device Guard and Credential Guard, offering increased protection of the operating system against exploits from malware. Virtualization based-security is available in generation 2 guest virtual machines starting with version 8. For information on virtual machine version, see [Upgrade virtual machine version in Hyper-V on Windows 10 or Windows Server 2016](https://docs.microsoft.com/zh-cn/windows-server/virtualization/hyper-v/deploy/upgrade-virtual-machine-version-in-hyper-v-on-windows-or-windows-server).基于虚拟化的安全功能，比如设备保护和凭证保护，提供了对操作系统的保护，以防止恶意软件的***。在第2代客户虚拟机中可以使用虚拟化的安全性，从第8版开始。有关虚拟机版本的信息，请参见Windows 10或Windows Server 2016上的Hyper-v升级虚拟机版本。
## Windows Containers (new)Windows容器(新)
Windows Containers allow many isolated applications to run on one computer system. They're fast to build and are highly scalable and portable. Two types of container runtime are available, each with a different degree of application isolation. Windows Server Containers use namespace and process isolation. Hyper-V Containers use a light-weight virtual machine for each container.Windows容器允许许多独立的应用程序在一个计算机系统上运行。它们的构建速度很快，而且具有高度的可伸缩性和可移植性。有两种类型的容器运行时，每种类型都具有不同程度的应用程序隔离。Windows服务器容器使用名称空间和进程隔离。Hyper-v容器为每个容器使用一个轻量级虚拟机。
Key features include:主要特点包括:
- 
Support for web sites and applications using HTTPS使用HTTPS支持web站点和应用程序
- 
Nano server can host both Windows Server and Hyper-V ContainersNano服务器可以同时承载Windows服务器和Hyper-v容器
- 
Ability to manage data through container shared folders通过容器共享文件夹管理数据的能力
- 
Ability to restrict container resources 限制容器资源的能力
For details, including quick start guides, see the [Windows Container documentation](https://msdn.microsoft.com/virtualization/windowscontainers/containers_welcome).有关详细信息，包括快速入门指南，请参阅Windows容器文档。
## Windows PowerShell Direct (new)Windows PowerShell直接(新)
This gives you a way to run Windows PowerShell commands in a virtual machine from the host. Windows PowerShell Direct runs between the host and the virtual machine. This means it doesn't require networking or firewall requirements, and it works regardless of your remote management configuration.这为您提供了一种在虚拟机中运行Windows PowerShell命令的方法。Windows PowerShell直接在主机和虚拟机之间运行。这意味着它不需要网络或防火墙的需求，并且不管您的远程管理配置如何，它都可以工作。
Windows PowerShell Direct is an alternative to the existing tools that Hyper-V administrators use to connect to a virtual machine on a Hyper-V host:Windows PowerShell Direct是一种替代现有的工具，即Hyper-V管理员用来连接Hyper-v主机上的虚拟机的工具:
- 
Remote management tools such as PowerShell or Remote Desktop诸如PowerShell或远程桌面之类的远程管理工具
- 
Hyper-V Virtual Machine Connection (VMConnect) Hyper-v虚拟机连接(VMConnect)
Those tools work well, but have trade-offs: VMConnect is reliable, but can be hard to automate. Remote PowerShell is powerful, but can be hard to set up and maintain. These trade-offs may become more important as your Hyper-V deployment grows. Windows PowerShell Direct addresses this by providing a powerful scripting and automation experience that's as simple as using VMConnect.
For requirements and instructions, see [Manage Windows virtual machines with PowerShell Direct](https://technet.microsoft.com/library/mt187808.aspx).
这些工具工作得很好，但是有一些权衡:VMConnect是可靠的，但是很难实现自动化。远程PowerShell是强大的，但是很难设置和维护。随着您的Hyper-v部署的增加，这些权衡可能变得更加重要。Windows PowerShell直接通过提供强大的脚本和自动化体验来解决这个问题，这就像使用VMConnect一样简单。
对于需求和指令，请参阅使用PowerShell直接管理的Windows虚拟机。

