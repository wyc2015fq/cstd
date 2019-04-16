# IoT固/软件更新及开源选项 - 我相信...... - CSDN博客





2018年03月13日 00:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2314









物联网的迅速发展涌现了数十亿与互联网连接的无线嵌入式设备。 从医疗设备到坦克传感器, 智能恒温器, 智能路灯, 水监视器等等, 物联网比以往任何时候都应用广泛。

![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczo00FQe1BCQUmMXDSoc4L8fjFC4QKlVn4vfNSCPGI9iaPFcicX1iceXHmPnqDk6vgwZFiaUW1iaaxAiaoicw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)

## 会出什么问题呢？

大多数这些设备的设计都不像是被恶意攻击的目标。 嵌入式系统传统上被认为是稳定的产品, 但实施起来成本高昂, 因为投资回报率(ROI)在的周期比较长。 在过去一旦发货, 就很少需要更新这些设备。 随着智能手机和RTOS的爆发, 才使得固件更新以及应用更新的必要性凸显出来。

假想一下, 恶意黑客将所有这些易受攻击的连接设备作为潜在攻击目标的话, 这些设备运行在不安全或过时的Linux 内核上, 有些漏洞还没有被修补过, 并且可以被远程控制！

这可不是一个有吸引力的场景。

![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczo00FQe1BCQUmMXDSoc4L8f0YZvA997eP63vTe9qtXU9fwDoibcqxJ6QKRfyMHpNmMVTfBohUicriahQ/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)

## 安全更新： 嵌入式与服务器的对比

如今, 升级安全问题驱动了IoT软件的升级更新, 也提高了工程师添加新的功能和修复漏洞可能性。

对于嵌入式设备, 固件更新机制不仅必须是安全的, 而且是可靠的, 因为它要么成功更新, 要么失败到可恢复状态。 一般地, 很难在用户现场升级固件，而需要在无人看管的情况下完成自动升级。 大多数更新也必须保留先前的设备状态, 尽管在某些情况下恢复设备可能涉及将系统重新设置为默认状态。

还有一个原子性问题。 Linux 服务器世界已经习惯于执行基于软件包的更新, 所有的东西似乎都能运行良好。 但是嵌入式设备则不一定。

服务器通常运行在一个可控的环境中, 可能是安全的, 并且有电源的保障和网络连接。 也就是说位于一个受监控的、可访问的位置, 用户干预恢复是可能的, 即使并不是必要条件。

Linux 服务器通常依赖于包管理, 基于 RPM (或 YUM)或基于 deb 的apt , 具有非原子增量更新的依赖辨识。 由包版本更新驱动流程, 每个都有一组复杂的预安装脚本, 这些脚本可能会让系统处于一个未定义的状态, 甚至是非工作状态。

不幸的是, 嵌入式设备可能无法访问, 大部分时间可能处于低功耗模式, 有很长的存活周期, 可能会遭受电力或网络中断的困扰, 从而中断固件升级。

总之, 基于包管理器的更新不是原子的, 因此很难测试和支持它们。 这通常会导致对设备固件实际状态的跟踪, 以及令人畏惧的"上次更新了什么?" 等问题。 这种方法不适合嵌入式系统, 因为这些系统会要求始终保持一致性。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczo00FQe1BCQUmMXDSoc4L8f5Mibhb7iaxpmtebPvnsewPMEDmJeeVrXBOIuWFsL1k6gnwf5heOibcy5Q/640?wx_fmt=jpeg)

## 镜像更新

更新嵌入式设备的传统最佳方式是对镜像进行整体更新。 在设备中, 这将是整个镜像和所有的设备固件。 在嵌入式 Linux 设备中, 这通常转化为分区更新, 所以分区方案是一个重要的考虑因素, 因为它将影响可以执行的软件更新类型。

嵌入式 Linux 设备通常将媒介分为不同的分区, 可以分别更新:
- 
Bootloader 分区: 如果有的话, 很少更新, 更新嵌入式设备的引导程序最终将导致设备最终被退出。 因此, 完善的更新机制应尽可能避免这种情况。

- 
引导 / 内核分区: Linux 内核和相关固件, 如设备树和 initramfs 镜像,除非为了安全，通常不需要更新。

- 
根文件系统分区: 存储的 OS 文件通常是只读且不可变的。 这也很少更新, 但如果应用程序依赖于这里的库, 可能会发生较多的更新情况。

- 
用户分区: 用户应用程序的存储位置和持久性数据是最需要更新的分区。


基本上, 固件镜像更新可以从整个系统：内核、根和用户分区到其中的某些部分。

有两种可能的镜像更新: 对称和非对称。
- 
对称: 对称更新需要更新分区镜像的双重副本, 以便可以在另一个运行时完成更新。 这通常需要两个引导/内核分区、两个根文件系统以及两个用户分区。 然后, bootloader 会跟踪哪些分区用于给定的引导。 对称更新的停机时间很小, 通常只有重启时间, 并允许更新取消。

- 
非对称: 非对称更新使用了一个通常由内存运行的恢复操作系统, 它有一个 Linux 内核和 initramfs 镜像。 这样就减少了所需的分区数量, 因为恢复模式只在一个额外的分区中存在, 并且可以更新其他任何分区。 如果更新失败, 可以重新尝试恢复。 不对称更新在更新时会有较长的下行时间, 并且不允许用户取消。






![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczo00FQe1BCQUmMXDSoc4L8fwpRwyrQoadRMRglPQwLYibRWWaUdAapVgMd8vdMHiaw2D3C3aOmkibmng/640?wx_fmt=jpeg)



## 用户空间更新

通常情况下, 更新由用户空间应用程序执行, 该应用程序可以获取软件更新包, 使用它, 并通知 bootloader 更新。 它还需要允许安装后进行操作。 然后 bootloader 启动一个硬件监视器并尝试启动。 如果引导成功, 那么硬件监视器就会被关闭; 如果不成功, 它就会被触发, bootloader 再次尝试启动。 在多次尝试之后, 它要么回到已知运行良好的分区, 要么进入恢复模式。

一个重要的考虑是, 用户空间固件更新必须通过固件更新进程进行。 另一个风险是, 有可能更新到一个可启动系统, 该系统具有一个已损坏的固件更新机制。 不幸的是, 需要回到 bootloader 或者其他恢复机制来更新固件。

有些系统使用 bootloader 来执行更新。 这存在严重的缺点， 如果固件更新代码必须更新(例如因为分区更改) , 那么需要更新的是 bootloader, 这是非常危险的。 Bootloader 在驱动程序、工具、库和它所支持的网络协议数量方面也非常有限, 因此更新会发生在资源有限的环境中。

基于镜像的开放源码软件更新有两个主要选项, 分别支持对称和非对称更新:
- 
Swupdate[1] （GPLv2许可下)


swupdate 在 Yocto 通过 meta-swupdate 层(但仅限于对称更新)来支持 swupdate。 它还包含一个 Mongoose web 服务器—— Suricatta daemon, 可以从一个远程服务器上拉取 Eclipse HawkBit[2]提供的更新新(用于向终端设备执行软件更新的后端解决方案)。 它目前只适用于 U-Boot bootloader。
- 
RAUC [3] （在 LGPLv2.1许可下）


RAUC 在 Yocto 通过 meta-ptx 层提供支持, 支持 Grub 或 Barebox bootloader。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1Gjczo00FQe1BCQUmMXDSoc4L8ftztricQzc6ia3Aocmpd0lhliajhia5h25CmKpsy89icwhx0LEibfTOWE714w/640?wx_fmt=png)

## 远程镜像更新

固件更新过程不仅能够从本地来源(例如 FLASH、 USB、 SD 或 UART)完成更新, 还必须能够远程更新，即通常所说的OTA更新。 OTA更新使用远程服务器向运行在设备上的客户端推送更新。

开源远程 OTA 固件更新的一些选项包括:
- 
Mender.io [4]（在 Apache 2 许可下）


mender.io同时用于客户端和服务器。它是通过meta-mender层支持Yocto。服务器可以充当部署和构建管理器，但也可包含设备管理控制台。
- 
Digi International Remote Manager [5], （在 MPLv2 许可下）


Digi 远程管理器有一个基于云的专属服务器和一个开源客户端。 它通过 meta-digi 层在 Yocto 得到了支持。 服务器可以充当部署和构建管理器, 还包含一个设备管理控制台, 该控制台具有设备报告和监控功能。
- 
Eclipse HawkBit [2]（在Eclipse公共许可下）


Eclipse HawkBit 是一个 Eclipse公共许可证服务器, 同时充当部署和构建管理器, 以及具有设备报告和监视功能。

全量更新的问题通常是尺寸较大, 可能会导致资源的受限, 尤其是设备端带宽的限制, 如蜂窝网络。 差分惊喜固件更新是一个很好的妥协, 只传输前一版本的查分数据。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczo00FQe1BCQUmMXDSoc4L8fb3JZcekQOOtpYXia0RzlG6bn8CWCyIY1BSJxaWsuBlHLxnEzveheWHw/640?wx_fmt=jpeg)

## 容器式更新

使用容器化程序简化了软件更新的用例, 应用程序可以单独更新。

容器更新是建立在一个不可变的分发上(可能是只读文件系统) , 其应用程序只存在于容器升级的容器中。

一些使用基于容器的固件更新的开源项目的例子有:
- 
Resin.io [6]


resin.io基于Docker的专有OTA更新服务器，遵从Apache 2 的许可，包括服务器和客户端。它是通过meta-resin层来支持Yocto的。
- 
Ubuntu “Snappy” Core [7]


Ubuntu Core 是一个基于 Ubuntu 的最小化操作系统, 它提供了与docker类的应用程序。

还有新的 OS 设计来支持 Docker 应用程序, 这些应用程序最终可能用于嵌入式空间, 如 CoreOS[8] 和 Project Atomic9。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1Gjczo00FQe1BCQUmMXDSoc4L8fR5F9ialpo6PHLxmTblXGYRWcfUTXhHaRpgxlNpZQxAyMwUxXRk5icjZQ/640?wx_fmt=png)

## 增量二进制原子化OS更新

在嵌入式领域中，一个即将到来的趋势是对每个文件的原子化增量更新, 可以快速部署或回滚, 同时保持完整的部署历史。

一些开源项目:
- 
libOSTree [10]


libOSTree 由一个库和命令行工具组成, 定义为"操作系统二进制文件的 Git"。 它使用类似 git 的对象来存储和部署 OS 查分包, 每个都有一个持久的数据副本。 对于使用它的 Yocto, 有一个 meta-updater 层，也被用于类似于 Atomic 的OS更新。
- 
swupd [11]


swupd 最初是 ClearLinux 的一部分, 由英特尔赞助。 它非常类似于 libOSTree, 在 Yocto 通过 meta-swupd 层提供支持。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1Gjczo00FQe1BCQUmMXDSoc4L8fBT9KffkOK9DKMiaaj6sGQkUg6fNQrgljJAP2Fyib9V04JZPfWQGDaNqA/640?wx_fmt=png)

## 选择范围

随着物联网设备的出现, 不仅固件更新是必要的, 而且新产品开发同样必要。 固件更新策略的选择需要尽早考虑, 因为这将影响到未来的产品设计决策。 与所有早期的决定一样, 错误的选择会给发展带来沉重的负担。

那些时间与市场紧密相连的项目可能会倾向于更传统的、经过测试的、完整的固件更新策略。 这些包括通过 Yocto Project 的 meta-swupdate 层提供的各种技术, 以及像 Digi International 的 Remote Manager 这样的为企业准备的 OTA 更新方案。

然而, 在新涌现的边缘项目可以通过类似容器设计，来扩展整个系统固件的更新方法, 使应用程序能够从系统更新中分离出来。

### References:

[1] "SWUpdate: Software update for embedded system." SWUpdate: software update for embedded system — Embedded Software Update Documentation 2017.07 documentation. Accessed September 14, 2017. https://sbabic.github.io/swupdate/swupdate.html.

[2] Beaton, Wayne. "Eclipse hawkBit." Projects.eclipse.org. October 20, 2016. Accessed September 14, 2017. https://projects.eclipse.org/projects/iot.hawkbit.

[3] "Safe and Secure Updating!" Safe and secure embedded Linux updates | RAUC. Accessed September 14, 2017. http://rauc.io/.

[4] "Over-the-air software updates for embedded Linux devices." Open source OTA software updates for embedded Linux | Mender. Accessed September 14, 2017. https://mender.io/.

[5] "Digi Remote Manager®." Monitor and Manage Remote Connected Devices - Digi International. Accessed September 14, 2017. https://www.digi.com/products/cloud/digi-remote-manager.

[6] "Embedded software deployment done right." Home | Resin.io. Accessed September 14, 2017. https://resin.io/.

[7] Canonical. "Ubuntu Core." Ubuntu Core | Ubuntu. Accessed September 14, 2017. https://www.ubuntu.com/core.

[8] "CoreOS." CoreOS Blog ATOM. Accessed September 14, 2017. https://coreos.com/.

[9] "Building the Next Generation Container OS." Project Atomic. Accessed September 14, 2017. http://www.projectatomic.io/.

[10] OSTree. Accessed September 14, 2017. https://ostree.readthedocs.io/en/latest/.

[11] "Software update." Clear Linux Project. April 25, 2016. Accessed September 14, 2017. https://clearlinux.org/features/software-update.

编译自http://www.embedded-computing.com/dev-tools-and-os/identifying-secure-firmware-update-mechanisms-for-embedded-linux-devices-and-open-source-options



![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoZGKwk1f08kMXcVbwM3mVauyFumwIWKe4fdVRib6UqUDqticBkkCV0xHcMoMIK4A67d9SA5TDwODOA/640?wx_fmt=jpeg)










