# Windows自带虚拟化服务工具Hyper-V学习了解和实操 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年08月27日 10:32:51[boonya](https://me.csdn.net/boonya)阅读数：1351








原文地址：[https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/about/](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/about/)

微软提供的虚拟化服务：[https://docs.microsoft.com/zh-cn/virtualization/index](https://docs.microsoft.com/zh-cn/virtualization/index)

![](https://img-blog.csdn.net/20180827103120242?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
Hyper-V 替换了 Microsoft Virtual PC。


无论你是软件开发人员、IT 专业人员还是技术爱好者，你们中的许多人都需要运行多个操作系统。 Hyper-V 让你可以在 Windows 上以虚拟机形式运行多个操作系统。

![运行 Windows 的虚拟机](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/about/media/hypervnesting.png)

具体来说，Hyper-V 提供硬件虚拟化。 这意味着每个虚拟机都在虚拟硬件上运行。 Hyper-V 允许你创建虚拟硬盘驱动器、虚拟交换机以及许多其他虚拟设备，所有这些都可以添加到虚拟机中。

## 使用虚拟化的原因

虚拟化允许你：
- 
运行需要早期版本的 Windows 操作系统或非 Windows 操作系统的软件。

- 
实验其他操作系统。 通过 Hyper-V，可轻松创建和删除不同的操作系统。

- 
使用多个虚拟机在多个操作系统上测试软件。 通过 Hyper-V，可以在一部台式机或便携式计算机上运行所有内容。 可以将这些虚拟机导出并随后导入到任何其他 Hyper-V 系统中，包括 Azure。


## 系统要求

Hyper-V 可用于 Windows 8 及更高版本的 64 位 Windows 专业版、企业版和教育版。 它无法用于 Windows 家庭版。

> 
打开**设置** > **更新和安全** > **激活**，从 Windows 10 家庭版升级到 Windows 10 专业版。 可以在此处访问应用商店并购买升级。


大多数计算机将运行 Hyper-V，但每个虚拟机都是一个完全独立的操作系统。 通常，你可以在具有 4GB RAM 的计算机上运行一个或多个虚拟机，但是你需要更多的资源以供其他虚拟机使用，或安装和运行资源密集型软件，如游戏、视频编辑或工程设计软件。

有关 Hyper-V 的系统要求以及如何验证 Hyper-V 在计算机上运行的详细信息，请参阅 [Hyper-V 要求参考](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/reference/hyper-v-requirements)。

## 可以在虚拟机中运行的操作系统

Windows 上的 Hyper-V 支持虚拟机中的许多不同操作系统，其中包括各种版本的 Linux、FreeBSD 和 Windows。

提醒一下，对于你在 VM 中使用的任何操作系统，都需要具有有效的许可证。

有关 Windows 上的 Hyper-V 中作为来宾支持的操作系统的信息，请参阅[受支持的 Windows 来宾操作系统](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/about/supported-guest-os)和 [受支持的 Linux 来宾操作系统](https://technet.microsoft.com/library/dn531030.aspx)。

## Windows 上的 Hyper-V 和 Windows Server 上的 Hyper-V 之间的差异

对于某些功能来说，其工作方式在 Windows 上的 Hyper-V 中和在运行于 Windows Server 上的 Hyper-V 中不同。

仅在 Windows Server 中可用的 Hyper-V 功能：
- 将虚拟机从一台主机实时迁移到另一台主机
- Hyper-V 副本
- 虚拟光纤通道
- SR-IOV 网络
- 共享的 .VHDX

仅在 Windows 10 中可用的 Hyper-V 功能：
- 快速创建和 VM 库
- 默认网络（NAT 交换机）

对于 Windows 上的 Hyper-V，内存管理模块不同。 在服务器上，通过假设只有虚拟机在该服务器上运行来管理 Hyper-V 内存。 在 Windows 上的 Hyper-V 中，通过大多数客户端计算机都在运行主机上的软件以及运行虚拟机的预期来管理内存。

## 限制

依赖于特定硬件的程序不能在虚拟机中良好运行。 例如，需要使用 GPU 进行处理的游戏或应用程序可能无法良好运行。 依赖于子 10 毫秒计时器的应用程序（如实时音乐混合应用程序或高精度时间）在虚拟机中运行时也可能会出问题。

此外，如果已启用了 Hyper-V，这些易受延迟影响的高精度应用程序在主机中运行时可能也会出问题。 这是因为在启用了虚拟化后，主机操作系统也会在 Hyper-V 虚拟化层的顶部运行，就如来宾操作系统那样。 但是，与来宾操作系统不同，主机操作系统在这点上很特殊，它是直接访问所有硬件，这意味着具有特殊硬件要求的应用程序仍然可以在主机操作系统中运行，而不会出问题。

## 学习了解和实操
- [安装 Hyper-V](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/quick-start/enable-hyper-v)
- [创建虚拟机](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/quick-start/quick-create-virtual-machine)
- 使用 Hyper-V 管理虚拟机
	- [Hyper-V 和 PowerShell](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/quick-start/try-hyper-v-powershell)
- [与虚拟机共享设备](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/user-guide/enhanced-session-mode)
- [使用检查点](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/user-guide/checkpoints)
- [使用 PowerShell Direct 进行连接](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/user-guide/powershell-direct)
- [创建预发行版虚拟机](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/user-guide/create-pre-release-vm)

- 管理 Hyper-V 主机
	- [启用嵌套虚拟化](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/user-guide/nested-virtualization)
- [构建自定义虚拟机库](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/user-guide/custom-gallery)
- [设置 NAT 网络](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/user-guide/setup-nat-network)
- [创建虚拟交换机](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/quick-start/connect-to-network)
- [生成集成服务](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/user-guide/make-integration-service)
- [移植 Hyper-V WMI](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/user-guide/refactor-wmiv1-to-wmiv2)

- 参考
	- [Hyper-V 要求](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/reference/hyper-v-requirements)
- [支持的来宾操作系统](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/about/supported-guest-os)
- [Hyper-V PowerShell](https://technet.microsoft.com/library/hh848559.aspx)
- [Hyper-V 集成服务](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/reference/integration-services)
- [Hyper-V 体系结构](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/reference/hyper-v-architecture)
- [虚拟机监控程序规范](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/reference/tlfs)
- [HYPER-V 备份方法](https://docs.microsoft.com/zh-cn/virtualization/hyper-v-on-windows/reference/hypervbackupapproaches)

- 社区和支持
	- [Hyper-V 论坛](https://social.technet.microsoft.com/Forums/windowsserver/en-US/home?forum=winserverhyperv)




