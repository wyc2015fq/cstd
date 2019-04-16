# 当SDN 遇到物联网 - 我相信...... - CSDN博客





2018年06月25日 06:51:44[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：823
所属专栏：[IoT与智能硬件](https://blog.csdn.net/column/details/19754.html)










工业物联网(Industrial Internet of Things，简称 IIoT)预示着一股新的现代化浪潮，在许多行业，客户和内部利益相关者要求在生产力、管理、安全和灵活性方面取得更多的进展。但是，IIoT的部署仍然面临相当大的阻力，主要是那些人工管理的基础设施，切这些基础设施大多不安全。 Fog 计算提供了一个创新的解决办法，通过在信息技术工具集框架内提供安全的技术基础设施，从而解决这些挑战。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1Gjczof02eTntUcMe1AG7exYN7mibRvqxxOianMibp1C2ANC1T5T4QM9ZVScEqolsZysb8CcVIz8lMNhtK2Q/640?wx_fmt=png)

图1 | 典型工厂的布局

图1显示了一个典型的工厂，服务和工作负载更多地以IT为中心(比如说，工厂数据中心) ，并随着它们向下层移动，逐渐变得以OT为中心(比如说，工厂机器)。软件定义的资源分配和管理正在Fog计算范式中获得了推动力，因为它使工厂运营者能够更好地适应未来的需求。从网络的角度来看，这将转化为使用软件定义的网络(SDN)来实现整个工厂的虚拟网络功能(VNF)。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1Gjczof02eTntUcMe1AG7exYN7mViaUeRqb3TVezOgpJDX51Wn42jVTjzJ9kv4hXhkhX4JcYkvzJZj9QhQ/640?wx_fmt=png)

图2 | SDN 内部组件

图2展示了典型 SDN 解决方案的一个视图，并包括以下组件。
- 
SDN应用程序

- 
SDN NBI允许SDN应用程序与 SDN控制器通信

- 
SDN控制器

- 
SDN控制数据平面接口(CDPI)允许 SDN控制器与 SDN 数据路径的对话

- 
SDN 数据路径

- 
这个方案的策略管理和性能监控


在以IT为中心的环境中，使用了很多的开源(和一些封闭的)解决方案来实现上述每一个组件。 其中比较突出的包括 OpenStack，VMware NSX，Cisco 数字网络架构等。

然而，IIoT 环境在 SDN 生态系统中引入了一些约束条件，需要重新设计一些SDN组件。
- 
恶劣的操作环境导致了大部分的无风扇计算系统。这些无风扇设计严重削减了可用的计算量，主要是因为他们的可用温控受到限制。虽然新的处理器设计一直提高低功耗的性能，但在可预见的未来，IIoT将滞后于(有时相当显著)在一个典型的数据中心。

- 
与数据中心环境不同，在一个典型的工厂中计算是稀疏的，通常不是普遍可达的(见图1)。 这引入了 SDN组件的扩展问题。

- 
客户业务逻辑对于可用计算的微弱数量而言，需要重新考虑如何实现这些 SDN 组件(并且它们的相互连接)

- 
在一个典型的工厂中(特别是底层)缺乏一个有效的界限，这意味着这些 SDN 组件需要特别注意它们的可用性和弹性。 这样的考虑，从计算上来说更加糟糕。


考虑一个简单的基于虚拟网络功能(VNF)的解决方案，其中包括:
- 
自学习桥接器

- 
连接到上述桥接器的多个虚拟机(或容器)

- 
接到上述桥接器的一个或多个机器或传感器

- 
防火墙，以限制上述通讯端点之间的数据流

- 
运营商管理防火墙的一些方法


![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1Gjczof02eTntUcMe1AG7exYN7mvgSdPWRVh9aERHoOymAePxvNRmLicQaib5m1g6rKjTI3nTH4r01rT1Eg/640?wx_fmt=png)

图3 | VNF实现的示例拓扑

表1提供了 VNF 解决方案映射到 SDN 组件的方式，以及如何实现(内核空间与用户空间)。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1Gjczof02eTntUcMe1AG7exYN7mp5O1pkB8vj8AG8z2LbdbAfEHGUic3a8wseSzvoW7UyiaDh1wdpSfExfQ/640?wx_fmt=png)

表1 | Linux 工具与 OVS 的比较

表2显示了使用(a) Linux 工具(使用 Linux 桥和 Linux 防火墙)和(b) OVS 处理各种流量类型的方式。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1Gjczof02eTntUcMe1AG7exYN7mOdicyztHMibcBqAxt6BKqadqLibcnx0mfuwh0zluMhiba9cTH4JshKckpw/640?wx_fmt=png)

表2 | Linux 工具与 OVS 的稳态比较

由于整个解决方案都是在软件中实现的，因此在选择与IIoT部署有关的任何选项时必须列举设计方面的因素：
- 
控制器和 CDPI之间的通信是软件交换(要么搭配到相同的计算机上，要么穿过工厂的网络) ，从而消耗CPU

- 
控制器本身消耗CPU

- 
OVS解决方案涉及到由CDPI代理和控制器(两者都在用户空间中实现)处理的双关(例如，异常和慢路径协议)流量。 这与 Linux工具的解决方案相反，在Linux解决方案中，这种流量完全是在内核内处理的。 因此，OVS解决方案会在每个数据包的基础上发生上下文交换(内核和用户空间之间)的开销。 如果存在网络干扰(比如因端点错误导致的流量洪流，预期的流量爆发，导致STP重新计算等等) ，这些上下文转换会变得相当严重。


一般来说，在 IIoT 环境中的 SDN 部署需要考虑到以下因素：

CPU核心计数很重要。在CPU核心方面，VNF需要在本地扩展。 此外，可用的 CPU 核心数量越大，越容易将VNF钉到特定的核心，从而提供更大的性能和弹性。 一般来说，这不仅提高了 VNF 的可用性，还提高了客户业务逻辑的可用性。

单线程性能很重要。由于在IIoT环境中的扩展困难，VN 必须具有处理网络工作负载爆发的能力。由于典型的VNF是单线程的，高的单线程性能将导致更好性能的SDN解决方案。

温控范围很重要。由于IIo 环境通常采用无风扇设计，所以一个有效的SDN解决方案要求 CPU 核心和单线程性能具有低功耗的特性。

VNF的卸载问题。虽然考虑快速路径卸载，如DPDK和 IOVisor，这些卸载仍然是以 IT 为中心的工作负载。例如，Intel 的 DPDK 需要为 DPDK 处理保留一些 CPU 核心。 这不符合在这些IIoT环境中的核心计数。需要卸载的不仅仅是一个通用的快速路径，而是卸载整个VNF。这并不意味着IIoT环境需要接受专用路由器和交换机。相反，卸载需要包括可重构的FPGA。每个VNF IP都可以优化用于 IIoT 环境，并且可以在工厂中使用的 FPGA 上的闪存。这不仅可以卸载VNF功能，而且也减轻了VNF的缓慢路径(和其他控制平面)方面，从而减轻了处理网络的IIoT计算。

软件堆栈很重要。一个以IT为中心的SDN软件栈缺乏在 IIoT 环境中运行所需的优化。 这种环境需要使用一个特定的软件栈，每个组件都经过了微调，以便在这些要求很高的环境中使用。 例如，与 Linux 桥相比，OVS 将产生上下文切换成本(内核空间快速路径和用户空间缓慢路径之间)。

Fog计算是工业自动化的下一个重大飞跃，并且被多个工业垂直领域所接受，不仅可以使设备现代化，还可以使大规模自动化基础设施的流程和可管理性工具集进行现代化。 NFV是Fog计算的一个重要方面，SDN或许成为在IIoT环境中实现NFV的实际手段。




（本文编译自http://www.embedded-computing.com/iot/fog-computing-bringing-sdn-to-iiot）

References
- 
Fog Networking: An Overview on Research Opportunities, Mung Chiang. http://www.princeton.edu/~chiangm/FogResearchOverview.pdf.

- 
Fog Computing Overview Video https://vimeo.com/228299847

- 
OpenFog Consortium Website https://www.openfogconsortium.org/

- 
Fog Computing and Its Role in the Internet of Things, Flavio Bonomi, Rodolfo Milito, Jiang Zhu, Sateesh Addepalli https://www.nebbiolo.tech/wp-content/uploads/fog-computing-and-its-role-in-the-internet-of-things- white-paper.pdf.

- 
Fog computing as enabler for the Industrial Internet of Things, Wilfried Steiner, Stefan Poledna. https://www.springerprofessional.de/en/fog-computing-as-enabler-for-the-industrial-internet-of- things/11002362.

- 
Fog for 5G and IoT, Mung Chiang (Editor), Bharath Balasubramanian (Editor), Flavio Bonomi (Editor). https://www.wiley.com/en-us/Fog+for+5G+and+IoT-p-9781119187134.

- 
Software-defined networking, Wikipedia. https://en.wikipedia.org/wiki/Software-defined_networking.








