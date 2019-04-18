# 组装自己的tesla超级计算机 - wishchinYang的专栏 - CSDN博客
2014年08月13日 08:34:43[wishchin](https://me.csdn.net/wishchin)阅读数：1633
原文链接：[blog.csdn.net/xqj198404/article/details/20016279](blog.csdn.net/xqj198404/article/details/20016279)
NVIDIA链接：[http://www.nvidia.cn/object/tesla_build_your_own_cn.htm](http://www.nvidia.cn/object/tesla_build_your_own_cn.html)l
# 组装属于您自己的Tesla个人超级计算机
如何打造属于你自己的Tesla个人超级计算机系统
本指南将帮助你组装一台Tesla个人超级计算机。如果你具备组装系统和工作站的经验，那么你可能希望打造一个属于自己的系统。不过，最简单的方法还是从[这些经销商](http://www.nvidia.com/object/tesla_supercomputer_wtb.html)那里购买一台现货供应的Tesla个人超级计算机。
和打造任何其他系统一样，你需要自己承担其中的风险和责任。在打造这样一个系统时，有诸多部件供您选择。NVIDIA®（英伟达™）提供了一般性的指南，但无法对部件的每一种配置及组合进行测试。
我们也遇到了许多用户的提问，他们想要知道在打造个人超级计算机这方面，为什么Tesla产品优于消费级产品。[点此了解更多细节](http://www.nvidia.cn/object/why-choose-tesla-cn.html)。
主要部件的最低配置
这些最低技术规格是针对这些意欲打造“Tesla Personal Supercomputer”系统的用户而制订的。 如果是打造工作站的话，你可以将Tesla GPU的配备数量缩减一些。
- 3x [Tesla C1060](http://www.nvidia.cn/object/tesla_c1060_cn.html)
- 四核CPU: 2.33 GHz (Intel或AMD)
- 系统存储器12 GB (每片Tesla C1060搭载4GB内存)
- Linux 64-bit或者Windows XP 64-bit 
- 系统噪音 < 45 dBA
- 电源：1200 W
范例——搭载4片Tesla C1060计算处理器的系统配置
这是打造一台四路Tesla C1060个人超级计算机所需的一份推荐组件清单。这些组件中，像内存、CPU、电源、机箱等许多组件都可以由合适的同类产品替代。我们不对任何个人超级计算机组件进行认证，认证工作由系统集成商来完成。
搭载4片 Tesla C1060计算处理器的配置|主板|[Tyan S7025](http://www.tyan.com/product_board_detail.aspx?pid=641)|
|----|----|
|PCI-e带宽|x4 PCI-e x16 Gen2 插槽|
|Tesla GPUs|4x Tesla C1060|
|显卡|板载显卡 (当一条PCI-e插槽上搭载NVIDIA GPU时可支持Linux，Windows操作系统|
|CPU|双插口 Intel Xeon Nehalem|
|内存|24 GB (6x 4GB) DDR3 DIMM (主板搭载高达64 GB)|
|电源|Coolmax CUQ-1350B 1350W|
|机箱|Lian Li PC-P80|
|硬盘驱动器|640 GB|
|DVD 光驱|刻录DVD|
|CPU风扇，散热片|适用于Intel Xeon|
**[预置SuperMicro SuperServer 7046GT-TRF](http://www.supermicro.com/products/system/4U/7046/SYS-7046GT-TRF.cfm)**
4 GPU 系统，dual-socket Nehalem，高达96 GB存储
搭载3片Tesla的系统配置 + 1片Quadro FX5800|PCI-e带宽|x4 PCI-e x16 Gen2 插槽|x4 PCI-e x8 Gen2 插槽|
|----|----|----|
||搭载3片 Tesla计算处理器的配置+ 1块Quadro FX双槽宽度显卡|搭载3片 Tesla计算处理器的配置+ 1块Quadro FX单槽宽度显卡|
|主板|[ASUS P6T7 WS SuperComputer](http://www.asus.com/product.aspx?P_ID=9ca8hJfGz483noLk)|[AsRock X58 SuperComputera](http://www.asrock.com/mb/overview.asp?Model=X58%20SuperComputer)|
|Tesla GPUs|3x Tesla C1060|3x Tesla C1060s|
|Display GPU|1x Quadro FX 或 NVS|1x Quadro NVS 或 FX (单槽)|
|CPU|Intel Xeon E55xx / Core i7|Intel Core i7|
|最小显存容量|24 GB DDR3|24 GB DDR3|
|电源|1200W|1200W|
主板
[Tesla C1060](http://www.nvidia.cn/object/tesla_c1060_cn.html)计算处理器是一块具有双倍宽度PCI-e x16插槽的第二代载板。它同样适用于第一代PCI-e x16插槽，不过会导致CPU和GPU之间的系统带宽变窄，从而影响到应用性能（取决于应用）。
因此，你需要使用具有3或4条PCI-e x16插槽的主板，并且是彼此隔开的双插槽。
CPUs
CPU的选择取决于你所使用的主板。我们推荐使用不小于2.33 GHz的四核CPU，如:
- Intel Xeon 或 Core i7 quad-core
- AMD Phenom 或 Opteron quad-core
系统存储器
因为每一块Tesla C2050均拥有4GB显存，所以我们建议至少为每一块Tesla C2050配备3GB，最好是4GB的系统内存。 因此，一台配备了3块Tesla C2050的系统，至少要配备12 GB系统内存。一台4块Tesla C2050的系统则至少要配备16GB系统内存。
电源
可以选择的电源有许多种。 要选择一部额定功率至少1350瓦的高品质电源。 例如可以选择Coolmax CUQ-1350B 1350W电源。 虽然我们建议每一块Tesla C2050都使用一根6针和一根8针辅助电源连接器，但是一些电源例如Coolmax CUQ-1350B用两根6针电源连接器即可提供足够的电力。 选择电源时，了解技术规格是非常重要的。
硬盘驱动器、DVD及其他部件
由你自己决定硬盘及DVD驱动器的选择。一般而言，最好是选用至少160GB的硬盘驱动器。 
电脑机箱
电脑机箱的选择很重要，这是因为当你插入4块Tesla C1060板，它需要一个具有8个插槽的机箱（比常见的ATX机箱大）。
具有8个插槽且能现货供应的机箱有 
- ABS Canyon 695
- [Ashinne ASR405](http://www.ashinne.com.tw/index.php?page=prod&id=12)
- Chenbro RM41300G
- Lian-Li PC-P80
- ThermalTake ArmorPlus
也可以选用具有7个插槽的机箱，切割出第8个插槽让其适用于4块双倍宽度的GPU板！这些都由你自己决定！
系统散热
有些系统机箱（如Lian-Li）自己配有系统风扇。一般而言，我们建议至少安装一个机箱风扇，以吹散Tesla板边的空气（例如，对准主板吹气）。其关键在于使机箱内的环境温度低于45C。
操作系统
建议采用Linux 64位元或 Window XP 64位元，使高容量内存、高效能系统能进行最佳运作。如果你更青睐Win7，那么我们建议使用TCC驱动程序以实现最高CUDA性能。 TCC驱动程序是Tesla系列产品专用的驱动程序。 用户可在[nvidia.cn](http://www.nvidia.cn)网站上的一般驱动程序下载页中找到最新的TCC驱动程序，如果想要下载旧版的TCC驱动程序，敬请访问:
[http://www.nvidia.cn/object/tesla-win7-64bit-259.03-whql-driver-cn.html](http://www.nvidia.cn/object/tesla-win7-64bit-259.03-whql-driver-cn.html)。请确保已经看过了驱动程序版本亮点，已经了解TCC驱动程序不支持的内容。
对系统进行校验
在你组装完系统并安装好操作系统之后，请从[CUDA Zone](http://www.nvidia.cn/object/cuda_get_cn.html)下载CUDA 驱动程序、CUDA工具箱，还可以选择下载CUDA SDK示例。
在下载CUDA 驱动程序、CUDA工具箱、CUDA SDK示例之后，运行CUDA SDK中的下列命令。
- deviceQuery
- 由此可得到系统中Tesla GPU的数目 
- bandwidthTest --memory=pinned --device=N
- 分别在N=0, 1, 2, 3 时为每一片 C1060运行一次
- 由此可得到距离CPU及每一个GPU的PCI-E带宽
- 第二代PCI-E x16带宽的峰值为5-6 GBytes/秒。第一代 PCI-E x16和第二代PCI-E x8的带宽峰值大约为该值的一半
- nbody --benchmark --n=131072 --device=N
- 同时运行示例的数目同系统中Tesla GPU的数目一致
- 对于搭载4片Tesla C1060的系统，同时运行4个示例 N=0,1,2,3
- 执行这一命令将在所有的Tesla GPU上运行nbody程序 
请使用[DGEMM老化测试](http://forums.nvidia.com/index.php?showtopic=86624&hl=dgemm)来测试你所装系统的稳定性。
**反馈所遇到的问题**
NVIDIA®（英伟达™）不会为组装Tesla个人超级计算机的个人直接提供支持。我们建议大家利用 [CUDA 论坛](http://www.cudachina.net/)向其他CUDA 开发人员咨询他们在打造这些系统方面的经验。 
