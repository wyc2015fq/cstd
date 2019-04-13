
# Linux  Linux 集群 - 高科的专栏 - CSDN博客

2011年09月02日 09:13:52[高科](https://me.csdn.net/pbymw8iwm)阅读数：882


# Linux 集群

## Page navigation
[什么是集群?](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#firstmajorhead)
[集群分类](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#secondmajorhead)
[基于 Linux 的集群](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#thirdmajorhead)
[Linux 服务器集群系统](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#fourthmajorhead)
[Linux 高性能计算集群](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#fifthmajorhead)
[集群系统 MOSIX](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#sixthmajorhead)
[构建 Linux 集群](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#seventhmajorhead)
[IBM 与 Linux 集群](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#eighththmajorhead)

本专题收集了 Linux 集群相关的文章和教程。

## 什么是集群?
简单的说，集群（cluster）就是一组计算机，它们作为一个整体向用户提供一组网络资源。这些单个的计算机系统就是集群的节点（node）。一个理想的集群是，用户从来不会意识到集群系统底层的节点，在他/她们看来，集群是一个系统，而非多个计算机系统。并且集群系统的管理员可以随意增加和删改集群系统的节点。
集群并不是一个全新的概念，其实早在七十年代计算机厂商和研究机构就开始了对集群系统的研究和开发。由于主要用于科学工程计算，所以这些系统并不为大家所熟知。直到Linux集群的出现，集群的概念才得以广为传播。集群系统主要分为高可用(High Availability)集群,简称 HA 集群，和高性能计算(High Perfermance Computing)集群，简称 HPC 集群。
通过下面这篇文章我们可以方方面面了解 Linux 集群涉及的硬件和软件。
[Linux 集群大全 -- 哪种群集适合您？](http://www.ibm.com/developerworks/cn/linux/cluster/lw-clustering.html)
[回页首](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#ibm-pcon)

## 集群分类
一般我们把集群系统分为两类：
高可用(High Availability)集群,简称HA集群。这类集群致力于提供高度可靠的服务。
高性能计算(High Perfermance Computing)集群，简称HPC集群。这类集群致力于提供单个计算机所不能提供的强大的计算能力。
[回页首](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#ibm-pcon)

## 基于 Linux 的集群
在 Linux 出现前，集群系统采用的操作系统主要有 VMS、UNIX 和 WindowsNT。到九十年代末期，Linux 操作系统不断走向成熟，它的健壮性不断增强，并且提供了 GNU 软件和标准化的 PVM、MPI 消息传递机制，最重要的是 Linux 在普通 PC 机上提供了对高性能网络的支持，这样就大大推动了基于 Linux 的集群系统的普及和发展。
[基于 linux 的集群系统，第 1 部分：集群系统概述](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part1/)
[基于 linux 的集群系统，第 2 部分：典型集群系统介绍之一
](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part2/index.html)
[基于 linux 的集群系统，第 3 部分：典型集群系统介绍之二](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part3/index.html)
[基于 linux 的集群系统，第 4 部分：典型系统小结及评测标准](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part4/index.html)
[基于 linux 的集群系统，第 5 部分：关键技术分析之 进程的放置和迁移](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part5/index.html)
[基于 linux 的集群系统，第 6 部分：关键技术分析之 高可用性](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part6/index.html)
[基于 linux 的集群系统，第 7 部分：关键技术分析之文件系统](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part7/index.html)
[基于 linux 的集群系统，第 8 部分：实现过程之理论先导篇(1)](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part8/index.html)
[基于 linux 的集群系统，第 9 部分：实现过程之理论先导篇(2)](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part9/index.html)
[基于 linux 的集群系统，第 10 部分：实现过程之理论先导篇(3)](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part10/index.html)
[基于 linux 的集群系统，第 11 部分：Linux集群系统的实现（上）](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part11/index1.html)
[基于 linux 的集群系统，第 12 部分：Linux集群系统的实现（下）](http://www.ibm.com/developerworks/cn/linux/cluster/linux_cluster/part11/index2.html)
[回页首](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#ibm-pcon)

## Linux 服务器集群系统
Linux Virtual Server 项目针对高可伸缩、高可用网络服务的需求，给出了基于 IP 层和基于内容请求分发的负载平衡调度解决方法，并在 Linux 内核中实现了这些方法，将一组服务器构成一个实现可伸缩的、高可用网络服务的虚拟服务器。
我们可以通过 Linux 集群项目－－LVS(Linux Virtual Server) 的创始人和主要开发人员章文嵩博士来了解这一集群系统。
[Linux 服务器集群系统，第 1 部分：LVS 项目介绍](http://www.ibm.com/developerworks/cn/linux/cluster/lvs/part1/index.html)
[Linux 服务器集群系统，第 2 部分：LVS 集群的体系结构](http://www.ibm.com/developerworks/cn/linux/cluster/lvs/part2/index.html)
[Linux 服务器集群系统，第 3 部分：LVS 集群中的IP负载均衡技术](http://www.ibm.com/developerworks/cn/linux/cluster/lvs/part3/index.html)
[Linux 服务器集群系统，第 4 部分：LVS 集群的负载调度](http://www.ibm.com/developerworks/cn/linux/cluster/lvs/part4/index.html)
[回页首](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#ibm-pcon)

## Linux 高性能计算集群
高性能计算(High-Performance Computing)是计算机科学的一个分支，它致力于开发超级计算机，研究并行算法和开发相关软件。高性能计算主要研究如下两类问题：1) 大规模科学问题，象天气预报、地形分析和生物制药等； 2) 存储和处理海量数据，象数据挖掘、图象处理和基因测序；
当论及 Linux 高性能集群时，许多人的第一反映就是 Beowulf。起初，Beowulf 只是一个著名的科学计算集群系统。以后的很多集群都采用 Beowulf 类似的架构，所以，实际上，现在 Beowulf 已经成为一类广为接受的高性能集群的类型。下面这系列文章就是围绕 Beowulf 展开的讨论。
[Linux 高性能计算集群，第 1 部分：概述](http://www.ibm.com/developerworks/cn/linux/cluster/hpc/part1/index.html)
[Linux 高性能计算集群，第 2 部分：Beowulf 集群](http://www.ibm.com/developerworks/cn/linux/cluster/hpc/part2/index.html)
[Linux 高性能计算集群，第 3 部分：硬件和网络体系结构](http://www.ibm.com/developerworks/cn/linux/cluster/hpc/part3/index.html)
[Linux 高性能计算集群，第 4 部分：软件体系结构](http://www.ibm.com/developerworks/cn/linux/cluster/hpc/part4/index.html)
[Linux 高性能计算集群，第 5 部分：资源管理和系统管理](http://www.ibm.com/developerworks/cn/linux/cluster/hpc/part5/index.html)
IBM 研发的 Linux 集群 Cluster1350 是定位于高性能计算的一套解决方案，集成了众多 IBM 与非 IBM 的先进的软硬件技术，有其特有的技术优势与强大的服务支持。
[IBM Cluster 1350 与 CSM](http://www.ibm.com/developerworks/cn/linux/cluster/l-ibm1350/index.html)
[回页首](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#ibm-pcon)

## 集群系统 MOSIX
MOSIX 是由 Jerusalem 的 Hebrew 大学开发的，它是一个软件管理层, 为 Linux 内核扩充了高性能集群计算支持能力。它采用单一系统映像模式 SSI(Single System Image)，支持所有的UNIX接口和机制，它最大特点是易使用性和透明性。
[集群系统 MOSIX 分析，第 1 部分：集群技术和进程迁移简介](http://www.ibm.com/developerworks/cn/linux/cluster/mosix/part1/index.html)
[集群系统 MOSIX 分析，第 2 部分：MOSIX 系统简介](http://www.ibm.com/developerworks/cn/linux/cluster/mosix/part2/index.html)
[集群系统 MOSIX 分析，第 3 部分：MOSIX 代理远程机制](http://www.ibm.com/developerworks/cn/linux/cluster/mosix/part3/index.html)
[集群系统 MOSIX 分析，第 4 部分：MOSIX 连接层](http://www.ibm.com/developerworks/cn/linux/cluster/mosix/part4/index.html)
[集群系统 MOSIX 分析，第 5 部分：MOSIX 进程迁移机制](http://www.ibm.com/developerworks/cn/linux/cluster/mosix/part5/index.html)
[集群系统 MOSIX 分析，第 6 部分：MOSIX 对系统调用的处理](http://www.ibm.com/developerworks/cn/linux/cluster/mosix/part6/index.html)
[集群系统 MOSIX 分析，第 7 部分：MOSIX 对信号机制的处理](http://www.ibm.com/developerworks/cn/linux/cluster/mosix/part7/index.html)
MOSIX 是一种特殊的透明形式的集群，它很容易建立并且只需投入最少的时间和精力就可产生积极结果。下面这篇教程可以指导您建立自己的 MOSIX 群集。
[使用 Mosix 的 Linux 集群](http://www.ibm.com/developerworks/cn/views/linux/tutorials.jsp?cv_doc_id=84921)
[回页首](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#ibm-pcon)

## 构建 Linux 集群
如何搭建自己的 Linux 集群呢？ 这可能是大多数读者非常希望知道的。“安装大型 Linux 集群”系列文章介绍了如何构建基于 IBM® System x®(TM)和 IBM® TotalStorage® systems 的 Linux® 集群。
[安装大型 Linux 集群，第 1 部分： 简介和硬件配置](http://www.ibm.com/developerworks/cn/linux/es-linuxclusterintro/index.html)
[安装大型 Linux 集群，第 2 部分： 配置管理服务器和安装节点](http://www.ibm.com/developerworks/cn/linux/es-clusterseriespt2/index.html)
[安装大型 Linux 集群，第 3 部分： 存储和共享文件系统](http://www.ibm.com/developerworks/cn/linux/es-clusterseriespt3/index.html)
[安装大型 Linux 集群，第 4 部分： 节点安装和 GPFS 集群配置](http://www.ibm.com/developerworks/cn/linux/es-clusterseriespt4/index.html)
此外，我们可以选择许多开源软件来构建自己的集群系统。
[使用 ClusterKnoppix 构建负载平衡集群](http://www.ibm.com/developerworks/cn/linux/l-clustknop.html)
[使用 coLinux 和 openMosix 构建异构集群](http://www.ibm.com/developerworks/cn/linux/l-colinux/)
[Linux 上的集群及其配置实例](http://www.ibm.com/developerworks/cn/linux/cluster/)
[使用 Mosix 的 Linux 集群](http://www.ibm.com/developerworks/cn/views/linux/tutorials.jsp?cv_doc_id=84921)
[回页首](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#ibm-pcon)

## IBM 与 Linux 集群
[IBM eServer OpenPower 710 集群入门介绍](http://www.ibm.com/developerworks/cn/linux/l-pow-opcluster/)
[IBM Cluster 1350 与 CSM](http://www.ibm.com/developerworks/cn/linux/cluster/l-ibm1350/index.html)
[回页首](http://www.ibm.com/developerworks/cn/linux/theme/cluster.html#ibm-pcon)







