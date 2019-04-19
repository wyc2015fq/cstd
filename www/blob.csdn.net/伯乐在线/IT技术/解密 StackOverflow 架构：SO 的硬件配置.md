# 解密 StackOverflow 架构：SO 的硬件配置 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Erucy](http://www.jobbole.com/members/Erucy) 翻译。未经许可，禁止转载！
英文出处：[nick craver](http://nickcraver.com/blog/2016/03/29/stack-overflow-the-hardware-2016-edition/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
这是「解密 Stack Overflow 架构」系列的第一篇，本系列会有非常多的内容。上一篇文章：《[StackOverflow 这么大，它的架构是怎么样的？](http://blog.jobbole.com/98633/)》
谁喜欢硬件？好吧，我喜欢，这是我的博客所以我说了算。如果你不喜欢硬件的话，我还是会继续，请关掉你的浏览器吧。
还在？很好。不过也可能是因为你的浏览器太特么慢了，所以你该考虑考虑要不要升级一下你的硬件了。
我已经重复过很多很多次了：[性能是个卖点](http://blog.codinghorror.com/performance-is-a-feature/)。只有硬件速度快，你的代码才能跑得快，所以硬件绝对是很重要的。和其他平台一样，Stack Overflow的架构也是分层的。对我们来说硬件是基础层，自己维护硬件给我们带来了其他方式（比如把应用运行在别人的服务器上）无法获得的奢侈享受。它同样也带来了直接和间接的开销。不过这不是本文的关注点，[这一比较会放在稍后的文章中](https://trello.com/c/4e6TOnA7/87-on-prem-vs-aws-azure-etc-why-the-cloud-isn-t-for-us)。在这篇文章中，作为参考和比较，我会提供我们的基础架构中详细的硬件配置清单，以及服务器的照片，其中有些是裸机的照片。这个页面本可以加载得更快些，不过我情不自禁地要加上这些照片。
在这个系列的多篇文章中，我会给出很多数字和规格。当我提到“我们的 SQL 服务器通常保持在 5% 到 10% 的 CPU 利用率”时，好吧，看起来是个不错的结果，可是这到底是什么样 CPU 的 5% 到 10% 呢？这时候就需要参考硬件配置清单了。本文中的配置清单会解答这些问题，同时也是和其他平台的一个比较：在那些平台上的资源利用率是如何的、需要比较的究竟是多少计算或存储的能力，等等。
### 我们如何规划硬件
声明：我不是一个人搞定这个的。George Beech ([@GABeech](https://twitter.com/GABeech)) 是我在 Stack 定制硬件规范时最主要的同谋。我们会谨慎地根据每台服务器预期的使用目的来规划它的硬件配置。我们不会大笔一挥先定下一个总体配置然后再进行分配。在这个过程中我们也不是独立在战斗，为了最优地定制硬件配置，我们必须知道在它上面到底需要运行什么样的应用。我们会跟开发人员和/或其他的网站可靠性工程师一起讨论，为每台服务器提供最佳的配置方案。
我们还需要关注什么方式对整个系统是最优的。每台服务器都不是孤岛，它们如何能够适应整个架构绝对是个需要好好考虑的问题。哪些服务能够共享这个平台？或是共享数据存储？共享日志系统？需要管理的东西越少越有价值、至少不要有那么多变化。
当我们完成硬件规划之后，我们会参考大量的需求来帮助我们决定订购什么硬件。我之前从来没有真的把我头脑中的清单写下来过，所以这次让我们试试看：
- 会不会存在纵向扩展或者横向扩展的问题？（我们需要购买更大的机器，还是更多小一些的机器？）
- 我们需要或者想要多少冗余？（需要保留多少余量和故障转移能力？）
- 存储：
- 这个服务器/应用需要使用磁盘吗？（除了操作系统盘之外我们还需要别的么？）
- 如果需要的话，需要多少？（需要多少带宽？有多少小文件？需要固态硬盘么？）
- 如果需要固态硬盘，它的写负载是什么样的？（是否需要考虑 Intel S3500/3700s？P360x？P3700s？）
- 我们需要多少固态硬盘控件？（是否和传统的机械硬盘一起组成一个两层的存储方案？）
- 这些数据是否完全是临时性的？（没有电容器的固态硬盘要便宜得多，会不会是更好的选择？译注：固态硬盘通常会使用电容器来保证在突然断电的情况下不会造成数据丢失）
- 存储需要考虑到扩展性问题么？（选用 1U/10-bay 的服务器，还是 2U/26-bay 的服务器？）
- 这是用于数据仓库类型的存储么？（是否使用 3.5 寸驱动器？如果是的话，在每个 2U 的机架上放置 12 还是 16 个驱动器？）
- 如果权衡使用 3.5 寸背板的话，120W 散热设计功耗（TDP）的限制值得吗？
- 我们需要直接暴露磁盘嘛？（控制器是否需要支持直通（pass-through）模式？）
- 内存：
- 它需要多少内存？（我们必须买多少？）
- 它能用上多少内存？（合理地来看需要买多少？）
- 我们会觉得它在未来可能会需要更多内存么？（我们需要使用那种内存通道配置？）
- 这是一个重度访问内存的应用吗？（我们是不是要让时钟频率达到最大化？）
- 内存访问会是高度并发的吗？（我们是否需要通过更多的内存插槽来容纳相同大小的内存？）
- CPU：
- 我们需要进行什么样的处理？（我们需要普通的CPU还是更强的？）
- 会不会有重度并发的处理？（我们是否需要较少、较快的核心？还是较多但是较慢的核心？）
- 哪种方式？会存在严重的 L2/L3 缓存争用问题么？（出于性能考虑，我们是否需要更大的 L3 缓存？）
- 是不是大多数情况下都是依靠单核性能？（我们是否需要最大化时钟频率？）
- 如果是的话，同时会存在多少进程？（我们需要什么样的 turbo spread？译注：没有查到 turbo spread 的含义，望知情者告知）
- 网络：
- 我们需要额外的 10Gb 网络连接吗？（它是否是一个“中介”（trough）机器，比如负载均衡器？）
- 在 Tx/Rx 缓冲区中需要多少均衡？（最好使用多少 CPU 核心来计算平衡？）
- 冗余：
- 我们同样需要在灾备中心部署这些服务器吗？
- 我们是否需要同样数量的服务器，或者只要可以接受，在冗余的时候少一些也可以？
- 我们需要电源线吗？不。我们不需要。
现在，让我们看看在纽约 QTS 数据中心内，是什么样的硬件在为我们的网站提供服务。悄悄地说，实际上数据中心是在新泽西，不过你知我知就好。为什么我们要说成是纽约的数据中心？因为我们不想把那些 NY- 开头的服务器重命名罢了。我会在后面的详细列表中进行备注说明，在丹佛的数据中心内，服务器有哪些规格的或是冗余级别的差异。
### 运行 Stack Overflow 和 Stack Exchange 网站的服务器
这里是一些全局配置，这样我就不用再提到每台服务器的时候单独说明了：
- 如果没有特别声明的话，操作系统盘不包括在配置之内。大多数服务器使用一组 250GB 或 500GB 的 SATA 机械硬盘作为操作系统分区，全部使用 RAID 1 模式。启动时间不是我们关注的问题，即使需要考虑的话，我们绝大多数物理服务器的启动时间也都和硬盘速度无关（比如检测 768GB 内存）。
- 所有服务器之间都是通过两路或多路 10Gb 双活模式（active/active）[LACP](https://en.wikipedia.org/wiki/Link_aggregation#Link_Aggregation_Control_Protocol) 网络连接的。
- 所有服务器都使用 208V 单相供电系统（通过两个 PSU（电源供应器）提供电力给两个 PDU（电源分配单元），来自两组来源）
- 所有在纽约的服务器都有电缆支架，所有在丹佛的服务器都没有（这是当地工程师的喜好）。
- 所有服务器都同时拥有 [iDRAC](http://en.community.dell.com/techcenter/systems-management/w/wiki/3204.dell-remote-access-controller-drac-idrac) 连接（通过管理网络）和 KVM 连接。
### 网络
- 2 台 Cisco Nexus [5596UP](http://www.cisco.com/c/en/us/products/switches/nexus-5596up-switch/index.html) 核心交换机（96 个 10 Gbps 的 SFP+ 端口）
- 10 台 Cisco Nexus [2232TM](http://www.cisco.com/c/en/us/products/switches/nexus-2232tm-10ge-fabric-extender/index.html) Fabric Extenders（每个机架 2 台，每台拥有 32 个 10Gbps 的 BASE-T 端口和 8 个 10Gbps 的 SFP+ 上行链路）
- 2 台 Fortinet [800C](http://www.fortinet.com/products/fortigate/enterprise-firewalls.html) 防火墙
- 2 台 Cisco [ASR-1001](http://www.cisco.com/c/en/us/products/routers/asr-1001-router/index.html) 路由器
- 2 台 Cisco [ASR-1001-x](http://www.cisco.com/c/en/us/products/routers/asr-1001-x-router/index.html) 路由器
- 6 台 Cisco [2960S-48TS-L](http://www.cisco.com/c/en/us/support/switches/catalyst-2960s-48ts-l-switch/model.html) 管理网络交换机（每个机架 1 台，每台拥有 48 个 1Gbps 端口和 4 个 1Gbps SFP 端口）
- 1 台 Dell [DMPU4032](http://accessories.us.dell.com/sna/productdetail.aspx?c=us&l=en&s=bsd&cs=04&sku=A7546775) KVM
- 7 台 Dell [DAV2216](http://accessories.us.dell.com/sna/productdetail.aspx?c=us&l=en&s=bsd&cs=04&sku=A7546777) KVM Aggregators（每个机架 1-2 台，每台都上行链接到 DPMU4032）
*备注：每台 FEX 拥有 80Gbps 上行带宽连接到核心交换机，而两台核心交换机之间拥有 160Gbps 端口通道。因为安装时间更新的原因，丹佛数据中心的硬件会稍微新一些：4 台路由器的型号都是 [ASR-1001-x](http://www.cisco.com/c/en/us/products/routers/asr-1001-x-router/index.html)，两台核心交换机是 [Cisco Nexus 56128P](http://www.cisco.com/c/en/us/products/switches/nexus-56128p-switch/index.html)，每台拥有 96 个 10Gbps 的 SFP+ 接口和 8个 40Gbps 的 QSFP+ 接口。这为未来的扩展省下了一些 10Gbps 端口，因为我们可以使用 4 组 40Gbps 的端口代替纽约数据中心内的 16 组 10Gbps 端口。*
这是我们在纽约的网络设备：
![](http://ww4.sinaimg.cn/mw690/63918611jw1f3jjrsoscaj20q80hhjv3.jpg)
![](http://ww2.sinaimg.cn/mw690/63918611jw1f3jjrtwa1rj20q80hhjwg.jpg)
![](http://ww3.sinaimg.cn/mw690/63918611jw1f3jjrum3lkj20q813cwq6.jpg)
这个是丹佛的：
![](http://ww1.sinaimg.cn/mw690/63918611jw1f3jjrvher0j21hw0kd7dk.jpg)
![](http://ww2.sinaimg.cn/mw690/63918611jw1f3jjrwsey6j20qi0zcwkc.jpg)
![](http://ww3.sinaimg.cn/mw690/63918611jw9f3jjyp7dr3j20ee0j6jt7.jpg)
请给 [Mark Henderson](https://twitter.com/thefarseeker)（我们的一位网站可靠性工程师）鼓掌，你在本文中看到的这些高清照片是他跑了一趟纽约的数据中心然后发给我的。
### SQL 服务器（Stack Overflow 集群）
- 2 台 Dell [R720xd](http://www.dell.com/us/business/p/poweredge-r720xd/pd) 服务器，每台拥有：
- 双 [E5-2697v2](http://ark.intel.com/products/75283/Intel-Xeon-Processor-E5-2697-v2-30M-Cache-2_70-GHz) 处理器（每个 12 核，主频 2.7-3.5GHz）
- 384 GB 内存（24 个 16 GB 插槽）
- 1 个 Intel [P3608](http://www.intel.com/content/www/us/en/solid-state-drives/solid-state-drives-dc-p3608-series.html) 4 TB NVMe PCIe 固态硬盘（RAID 0，每个硬盘卡双控制器）
- 24 个 Intel [710](http://ark.intel.com/products/56584/Intel-SSD-710-Series-200GB-2_5in-SATA-3Gbs-25nm-MLC) 200 GB SATA 固态硬盘（RAID 10）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
### SQL 服务器（Stack Exchange 及其他 集群）
- 2 台 Dell [R730xd](http://www.dell.com/us/business/p/poweredge-r730xd/pd) 服务器，每台拥有：
- 双 [E5-2667v3](http://ark.intel.com/products/83361/Intel-Xeon-Processor-E5-2667-v3-20M-Cache-3_20-GHz) 处理器（每个 8 核，主频 3.2-3.6GHz）
- 768 GB 内存（24 个 32 GB 插槽）
- 3 个 Intel [P3700](http://ark.intel.com/products/79620/Intel-SSD-DC-P3700-Series-2_0TB-12-Height-PCIe-3_0-20nm-MLC) 2 TB NVMe PCIe 固态硬盘（RAID 0）
- 24 个 10K Spinny 1.2 TB SATA 固态硬盘（RAID 10）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
*备注：丹佛的 SQL 硬件在配置上是完全一致的，只不过和纽约相比，每组只有一台 SQL 服务器。*
这是纽约的 SQL 服务器，是我们二月份升级 PCIe 固态硬盘的时候拍的：
![](http://ww1.sinaimg.cn/mw690/63918611jw1f3jjry0hynj20q80jotgt.jpg)
![](http://ww1.sinaimg.cn/mw690/63918611jw1f3jjryzifej20q80joagc.jpg)
![](http://ww4.sinaimg.cn/mw690/63918611jw1f3jjs038pqj20q80jowjk.jpg)
![](http://ww4.sinaimg.cn/mw690/63918611jw1f3jjs0ipmhj20q80k5ady.jpg)
### Web 服务器
- 11 台 Dell [R630](http://www.dell.com/us/business/p/poweredge-r630/pd) 服务器，每台拥有：
- 双 [E5-2690v3](http://ark.intel.com/products/81713/Intel-Xeon-Processor-E5-2690-v3-30M-Cache-2_60-GHz) 处理器（每个 12 核，主频 2.6-3.5GHz）
- 64 GB 内存（8 个 8 GB 插槽）
- 2 个 Intel [320](http://ark.intel.com/products/56567/Intel-SSD-320-Series-300GB-2_5in-SATA-3Gbs-25nm-MLC) 300 GB SATA 固态硬盘（RAID 1）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
![](http://ww4.sinaimg.cn/mw690/63918611jw1f3jjs1n7w3j20q80hh79k.jpg)
![](http://ww3.sinaimg.cn/mw690/63918611jw1f3jjs26yn6j20q80jo7a2.jpg)
![](http://ww1.sinaimg.cn/mw690/63918611jw1f3jjs3b5b9j20q80joqah.jpg)
![](http://ww2.sinaimg.cn/mw690/63918611jw1f3jjs3xs1nj20q80jodl7.jpg)
### Service 服务器（Workers）
- 2 台 Dell [R630](http://www.dell.com/us/business/p/poweredge-r630/pd) 服务器，每台拥有：
- 双 [E5-2643 v3](http://ark.intel.com/products/81900/Intel-Xeon-Processor-E5-2643-v3-20M-Cache-3_40-GHz) 处理器（每个 6 核，主频 3.4-3.7GHz）
- 64 GB 内存（8 个 8 GB 插槽）
- 1 台 Dell [R620](http://www.dell.com/us/business/p/poweredge-r620/pd) 服务器，拥有：
- 双 [E5-2667](http://ark.intel.com/products/64589/Intel-Xeon-Processor-E5-2667-15M-Cache-2_90-GHz-8_00-GTs-Intel-QPI) 处理器（每个 6 核，主频 2.9-3.5GHz）
- 32 GB 内存（8 个 4 GB 插槽）
- 2 个 Intel [320](http://ark.intel.com/products/56567/Intel-SSD-320-Series-300GB-2_5in-SATA-3Gbs-25nm-MLC) 300 GB SATA 固态硬盘（RAID 1）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
*备注：NY-SERVICE03 依然是 R620 服务器，因为还没有老到需要在这一次退役的地步。它会在今年稍后进行升级。*
### Redis 服务器（缓存）
- 2 台 Dell [R630](http://www.dell.com/us/business/p/poweredge-r630/pd) 服务器，每台拥有：
- 双 [E5-2687W v3](http://ark.intel.com/products/81909/Intel-Xeon-Processor-E5-2687W-v3-25M-Cache-3_10-GHz) 处理器（每个 10 核，主频 3.1-3.5GHz）
- 256 GB 内存（16 个 16 GB 插槽）
- 2 个 Intel [520](http://ark.intel.com/products/66250/Intel-SSD-520-Series-240GB-2_5in-SATA-6Gbs-25nm-MLC) 240 GB SATA 固态硬盘（RAID 1）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
### Elasticsearch 服务器（搜索）
- 3 台 Dell [R620](http://www.dell.com/us/business/p/poweredge-r620/pd) 服务器，每台拥有：
- 双 [E5-2680](http://ark.intel.com/products/64583/Intel-Xeon-Processor-E5-2680-20M-Cache-2_70-GHz-8_00-GTs-Intel-QPI) 处理器（每个 8 核，主频 2.7-3.5GHz）
- 192 GB 内存（12 个 16 GB 插槽）
- 2 个 Intel [S3500](http://ark.intel.com/products/75685/Intel-SSD-DC-S3500-Series-800GB-2_5in-SATA-6Gbs-20nm-MLC) 800 GB SATA 固态硬盘（RAID 1）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
### HAProxy 服务器（负载均衡）
- 2 台 Dell [R620](http://www.dell.com/us/business/p/poweredge-r620/pd) 服务器（CloudFlare 流量），每台拥有：
- 双 [E5-2637 v2](http://ark.intel.com/products/81900/Intel-Xeon-Processor-E5-2643-v3-20M-Cache-3_40-GHz) 处理器（每个 4 核，主频 3.5-3.8GHz）
- 192 GB 内存（12 个 16 GB 插槽）
- 6 个 Seagate [Constellation 7200 转](http://www.amazon.com/SEAGATE-ST91000640NS-Constellation-6-0Gb-internal/dp/B004HZEF2I) 1TB SATA 机械硬盘（RAID 10）（用于日志）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC） – 用于内部流量（DMZ）
- 双路 10 Gbps 网卡（Intel X540） – 用于外部流量
- 2 台 Dell [R620](http://www.dell.com/us/business/p/poweredge-r620/pd) 服务器（直接流量），每台拥有：
- 双 [E5-2650](http://ark.intel.com/products/64590/Intel-Xeon-Processor-E5-2650-20M-Cache-2_00-GHz-8_00-GTs-Intel-QPI) 处理器（每个 8 核，主频 2.0-2.8GHz）
- 64 GB 内存（4 个 16 GB 插槽）
- 2 个 Seagate [Constellation 7200 转](http://www.amazon.com/SEAGATE-ST91000640NS-Constellation-6-0Gb-internal/dp/B004HZEF2I) 1TB SATA 机械硬盘（RAID 10）（用于日志）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC） – 用于内部流量（DMZ）
- 双路 10 Gbps 网卡（Intel X540） – 用于外部流量
*备注：这些服务器是不同时间采购的，因此从结果上来看配置不太一样。此外，两台 CloudFlare 负载均衡拥有更多的内存是为了安装 memcached（不过现在我们已经不再运行了），用于 CloudFlare 的 [Railgun](https://www.cloudflare.com/railgun/)。*
上面提到的 Service、Redis、搜索和负载均衡服务器都是 1U 服务器，安装在一个机架中。这是纽约这个机架的样子：
![](http://ww3.sinaimg.cn/mw690/63918611jw1f3jjs4hxxbj20q80hhjus.jpg)
![](http://ww1.sinaimg.cn/mw690/63918611jw1f3jjs5ab2fj20q80jote4.jpg)
![](http://ww3.sinaimg.cn/mw690/63918611jw1f3jjs5ofe7j20q80jojxm.jpg)
![](http://ww1.sinaimg.cn/mw690/63918611jw1f3jjs68pt0j20q80jotf1.jpg)
### 其他服务器
我们还有一些其他的服务器，它们并没有直接或间接地参与到网站相关的流量中。它们可能是提供相关服务（比如域控制器，在少数情况下用于应用程序池的验证，并且以虚拟机形式运行），或是用于非关键目的，比如监控、日志存储、备份等等。
因为这篇博客是想作为[系列中](http://nickcraver.com/blog/2016/02/03/stack-overflow-a-technical-deconstruction/)后续内容的一个附录，我也会把这些“后台”的服务器包括在其中。这会让我把我们的服务器架构更裸露地展现给你们，谁不喜欢更裸露点呢？
### 虚拟机服务器（目前使用了 VMWare）
- 2 台 Dell [FX2s](http://www.dell.com/us/business/p/poweredge-fx/pd) 刀片机架，每台使用了 4 个刀片插槽中的 2 个
- 4 台 Dell [FC630](http://www.dell.com/us/business/p/poweredge-fx/pd#Misc) 刀片服务器（每个机架 2 台），每台拥有：
- 双 [E5-2698 v3](http://ark.intel.com/products/81900/Intel-Xeon-Processor-E5-2643-v3-20M-Cache-3_40-GHz) 处理器（每个 16 核，主频 2.3-3.6GHz）
- 768 GB 内存（24 个 32 GB 插槽）
- 2 个 16GB SD 卡（用于 Hypervisor – 没有本地存储）
- 双路 4x 10 Gbps 网卡（FX IOAs – BASET）
- 1 个 EqualLogic [PS6210X](http://www.dell.com/us/business/p/equallogic-ps6210-series/pd) iSCSI SAN
- 24 个 Dell 10K RPM 1.2 TB SAS 机械硬盘（RAID 10）
- 双路 10 Gbps 网卡（10 – BASET）
- 1 个 EqualLogic [PS6110X](http://www.dell.com/us/business/p/equallogic-ps6110x/pd) iSCSI SAN
- 24 个 Dell 10K RPM 900 GB SAS 机械硬盘（RAID 10）
- 双路 10 Gbps 网卡（SFP+）
![](http://ww1.sinaimg.cn/mw690/63918611jw1f3jjs6yht0j20q80yzn22.jpg)
![](http://ww1.sinaimg.cn/mw690/63918611jw1f3jjs7lb8yj20q80yz0xc.jpg)
![](http://ww3.sinaimg.cn/mw690/63918611jw1f3jjs8wl6aj20q813ctit.jpg)
![](http://ww4.sinaimg.cn/mw690/63918611jw1f3jjs99t9hj20q80yzjwh.jpg)
还有一些在后台的值得关注的服务器不是以虚拟机形式运行的。这些服务器会运行一些后台任务，帮助我们调试日志、存储海量的数据等等。
### 机器学习服务器（Providence）
这些服务器在 99% 的时间里都是空闲的，不过每天晚上都会进行繁重的工作：刷新 Providence。它们同样为内部的数据中心提供服务，用于在大型数据集上验证新的算法。
- 2 台 Dell [R620](http://www.dell.com/us/business/p/poweredge-r620/pd) 服务器，每台拥有：
- 双 [E5-2697 v2](http://ark.intel.com/products/75283/Intel-Xeon-Processor-E5-2697-v2-30M-Cache-2_70-GHz) 处理器（每个 12 核，主频 2.7-3.5GHz）
- 384 GB 内存（24 个 16 GB 插槽）
- 4 个 Intel [530](http://ark.intel.com/products/75336/Intel-SSD-530-Series-480GB-2_5in-SATA-6Gbs-20nm-MLC) 480 GB SATA 固态硬盘（RAID 10）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
### 机器学习 Redis 服务器（依然是 Providence）
这是用于 Providence 的 Redis 数据存储。通常的配置是一台主服务器、一台从服务器，还有一个实例用于验证我们最新版的机器学习算法。虽然不会用于给 Q&A 网站提供服务，但这些数据会用于在 Careers 网站和侧边栏的列表中显示相关的工作职位。
- 3 台 Dell [R720xd](http://www.dell.com/us/business/p/poweredge-r720xd/pd) 服务器，每台拥有：
- 双 [E5-2650 v2](http://ark.intel.com/products/75269/Intel-Xeon-Processor-E5-2650-v2-20M-Cache-2_60-GHz) 处理器（每个 8 核，主频 2.6-3.4GHz）
- 384 GB 内存（24 个 16 GB 插槽）
- 4 个 Samsung [840 Pro](http://www.samsung.com/semiconductor/products/flash-storage/client-ssd) 480 GB SATA 固态硬盘（RAID 10）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
### Logstash 服务器（你知道的，用于日志）
我们的 Logstash 集群（作为 Elasticsearch 的存储源）保存来自……好吧，每个地方的日志。我们计划把 HTTP 日志也复制到这里，不过遇到了性能问题。然而，我们在这里还是聚合了所有的网络设备日志、syslog、Windows 和 Linux 的操作系统日志，因此我们可以得到一个网络情况的概览，或者快速的检索问题。它同样作为 Bosun 的数据源，在警告发生的时候可以获取一些额外信息。这个集群总的原生存储空间是 6x12x4 = 288 TB。
- 6 台 Dell [R720xd](http://www.dell.com/us/business/p/poweredge-r720xd/pd) 服务器，每台拥有：
- 双 [E5-2660 v2](http://ark.intel.com/products/75272/Intel-Xeon-Processor-E5-2660-v2-25M-Cache-2_20-GHz) 处理器（每个 10 核，主频 2.2-3.0GHz）
- 192 GB 内存（12 个 16 GB 插槽）
- 12 个 7200 转 Spinny 4 TB SATA 机械硬盘（每 4 块一组，共三组 RAID 0）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
### HTTP 日志 SQL 数据库
这些服务器用于记录每一个负载均衡服务器收到的 HTTP 请求（来自 HAProxy 的 syslog），将其保存到 SQL 数据库中。在这里我们只记录最上层的一些属性，比如 Url、Query、UserAgent，以及 SQL、Redis 的响应时间等等，这些数据都会以天为单位进入到列存储索引（Clustered Columnstore Index）中。我们使用这些日志来调试用户的问题、检测僵尸网络（botnet）等等。
- 1 台 Dell [R730xd](http://www.dell.com/us/business/p/poweredge-r730xd/pd) 服务器，拥有：
- 双 [E5-2660 v3](http://ark.intel.com/products/81706/Intel-Xeon-Processor-E5-2660-v3-25M-Cache-2_60-GHz) 处理器（每个 10 核，主频 2.6-3.3GHz）
- 256 GB 内存（16 个 16 GB 插槽）
- 2 个 Intel [P3600](http://ark.intel.com/products/80995/Intel-SSD-DC-P3600-Series-2_0TB-2_5in-PCIe-3_0-20nm-MLC) 2 TB NVMe PCIe 固态硬盘（RAID 0）
- 16 个 Seagate [ST6000NM0024](http://www.seagate.com/internal-hard-drives/enterprise-hard-drives/hdd/enterprise-capacity-3-5-hdd/?sku=ST6000NM0024) 7200 转 Spinny 6 TB SATA 机械硬盘（RAID 10）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
### 开发 SQL 服务器
我们希望在开发的时候尽可能模拟生产环境，SQL 数据库也是一样……或者至少和之前一样。在最近一次采购中，我们已经升级了生产环境的处理器。我们也会在今年晚些时候，和 Stack Overflow 集群一起，将这台服务器升级为 2U 的版本。
- 1 台 Dell [R620](http://www.dell.com/us/business/p/poweredge-r620/pd) 服务器，拥有：
- 双 [E5-2620](http://ark.intel.com/products/64594/Intel-Xeon-Processor-E5-2620-15M-Cache-2_00-GHz-7_20-GTs-Intel-QPI) 处理器（每个 6 核，主频 2.0-2.5GHz）
- 384 GB 内存（24 个 16 GB 插槽）
- 8 个 Intel [S3700](http://ark.intel.com/products/71916/Intel-SSD-DC-S3700-Series-800GB-2_5in-SATA-6Gbs-25nm-MLC) 800 GB SATA 固态硬盘（RAID 10）
- 双路 10 Gbps 网卡（Intel X540/I350 NDC）
以上就是目前实际为网站提供服务的硬件配置了，大体上还是挺有意思的。当然，我们也有一些其他的后台任务服务器比如日志、监控、备份等等。如果你真的对这些系统特别感兴趣的话，请在博客下面留言，我很高兴详细给出这些内容。这里是在几个星期前，纽约的数据中心完整安装之后的样子：
![](http://ww3.sinaimg.cn/mw690/63918611jw1f3jjsamx4zj20q813ctju.jpg)
![](http://ww4.sinaimg.cn/mw690/63918611jw1f3jjsbdypdj20q813calu.jpg)
接下来？这个[系列博客](http://nickcraver.com/blog/2016/02/03/stack-overflow-a-technical-deconstruction/)我会按照大家最想要了解的顺序来写。去 [Trello borad](https://trello.com/b/0zgQjktX/blog-post-queue-for-stack-overflow-topics) 上看看，目前看起来 [部署](https://trello.com/c/bh4GZ30c/25-deployment) 可能是下一个大家最感兴趣的话题。所以下一次请期待我们是如何将代码从开发机器部署到生产环境，以及在这个过程中一切是如何运作的。我会包含数据库迁移、版本更迭、持续集成（CI）架构、我们如何建立开发环境等等，并分享所有和部署相关的信息。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
![](http://www.jobbole.com/wp-content/uploads/2016/04/67ad06bea02c29a3fdc28104113f80eb1.png)
