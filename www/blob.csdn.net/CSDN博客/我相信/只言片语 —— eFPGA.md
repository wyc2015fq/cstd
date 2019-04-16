# 只言片语 —— eFPGA - 我相信...... - CSDN博客





2018年03月11日 00:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：936
所属专栏：[IoT与智能硬件](https://blog.csdn.net/column/details/19754.html)










```
科学就是那些我们能对计算机说明白的东西，余下的都叫艺术。
——高德纳
```




从读书开始时的EPLD到大学毕业时的FPGA，一晃多年，仿若回到原点，只是很多的技能都似随风而逝，现在从IoT领域试图找回一些原来的影子，也许是为了忘却的纪念，也许是因为FPGA在物联网中有着它自己的天地。

捕风捉影，并不是走火入魔才好，FPGA在IoT中有着哪些踪影呢？

## 可配置的硬件加速

一个流行的用法是将FPGA与处理器总线结合起来作为可配置的加速器。 芯片设计者发现这种方法比固定功能加速器提供了更多的灵活性, 并且与主流处理器相比可以提供更高的性能。

这样做的的一个关键好处是可以通过重新编程来加速多个任务。 每个芯片都有一个或多个处理器(ARM, ARC, MIPS 等)执行代码， 对于占据大部分处理器带宽的任务, 硬件中的加速器通常可以在更短的时间内完成任务。 加速器不会取代处理器, 而是加速了工作量最大的任务。 如果加速器可重新配置, 就可以加速多个任务, 允许它根据需要或根据不同的客户或应用的需求处理更多的工作负载。 FPGA是可重新配置编程的, 是作为加速器的完美选择之一。

看几个FPGA 如何用于加速 axi / ahb 总线的几个例子, 以及每个用例潜在的性能改进。 以 ARM 为例,主要因为它是目前使用最广泛的处理器, 而且在性能方面很容易验证。

### AES-128

在FPGA 中实现了用于数据流动的 axi4流总线和用于控制逻辑的 APB 总线。 由于这个接口功能不变, 可以实现外部硬连接。

![](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQEdxPobGDjh87j8I8IbLdjTzxiaCsCibenf4xQa5mYTJBNicLCWm2uE9ZQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)

图1 AES-128

这个 AES-128加速器的 RTL 需要1142个 LUTs, 并且适用于单一的 EFLX-2.5k IP 核, 可在多个工艺节点中使用。 在 TSMC16FFC 中, AES-128加速器的运行频率仅为374 MHz。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQStdOvHmoevgIQibsN8bD2zL6vx9lSloiaicMcXcLBR9TtDov7cGIiaxJRA/640?wx_fmt=png)

图2 AES-128 性能对比 

这种性能比在同一个进程中运行在 ARM Cortex M4上的 AES-128软件代码快136至300倍,当然，这取决于 ARM M4的时钟速度。

### SHA-256

在这个例子中，AXI4是隶属于 RTL 外部的 FPGA, 用于加速器的数据移动和加速器寄存器的配置。 对于数据移动的最低总线延迟, AXI4的从属逻辑是外部的。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQdXQXmLa5b7lcZNXIPZTE5q3K1Ggx690NNMDIzicnlibVCsv89ibMJS66A/640?wx_fmt=png)

图3 SHA-256

这个在64字节数据块上的 SHA-256加速器， RTL需要1634个 LUTs, 并且适用于单个 EFLX-2.5k IP 核, 可在多个进程节点中使用。 在 TSMC16FFC 中, SHA-256加速器的171 MHz。

这个性能比运行在 ARM Cortex M4上的 SHA-256软件代码快40倍。

### Jpeg 编码器

下面是一个配置为 JPEG 编码器的 FPGA 框图。 在这个例子中, axi4流和 APB 接口逻辑在FPGA 中得到了实现, 但是这个 RTL 可以很容易的放在外面, 并且是硬连接的, 因为它不需要重新配置。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQgLib4a9RicHJP2l4eED55D39reDQKic8rAibe05Y6QcDibIv46oezx8nUkg/640?wx_fmt=png)

图4 JPEG 编码器

这个 RTL 需要11364个 LUTs 和大量的内存(2256 Kbyte 双端口RAM) , 需要附加到FPGA上。 与可用的 I/O相比, 加到内存中的信号数量非常小。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQWmAM1VHG5icACkicst2prTmMWtZxRogXuEeKoMdb4r47jCpaDQV80uuA/640?wx_fmt=png)

图5 编码器的外置内存 

在 TSMC16FFC 中, 运行频率最低为149 MHz。 这大约是在同一进程中运行在 ARM Cortex M4上JPEG 编码软件代码的31倍。

### 256点 FFT

下例是FPGA 配置为256点的 FFT 加速器, 作为 axi4流总线上的一个从属 / 主控, 在 EFLX 阵列中实现 AXI RTL。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQHvoo1M8EROk7UNpicibZ81mRgl4iclw5K6RseicwYiauDVvrdjic81yaVPbQ/640?wx_fmt=png)

图6 256点的FFT

RTL 需要8360个 LUT 和16个外部 RAM。 在这个示例中, RAM 被附加到阵列中以提高性能。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQOFLfPsict6ceMdzgSia22UDx3b7ZUVetX0KOFibQe0Q130Zlicp2JP7svg/640?wx_fmt=png)

图7 FFT的外置内存 

在 TSMC16FFC 中, 运行频率最低为303MHz。 ARM 处理器的基准是不可用的, 但是由于在 MAC 和内存引用中存在大量的并行处理, 可以期望这个加速器的性能远远超过一个典型的处理器。 

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQicc9hibsvyyshtPdewjl61w68VEmw05ibeDxKuSbT9fIicPnTG5EpbCZ7w/640?wx_fmt=png)

图8 FPGA FFT的性能

因此， 使用FPGA 作为一个可重新配置的加速器带来了显著收益, 可以提供30到300倍的性能增长。 其重新配置的能力使得FPGA能够同时加速多个功能, 并且能够随时根据不断变化的标准或协议无缝升级。 这些优势在数据中心、网络、深度学习、人工智能、航空航天等处理器密集型应用中非常可观。

## 硬件安全

嵌入式设计的安全性一直是一个热门话题, 但是安全对于不同的人和组织来说意味着不同的东西。 尽管嵌入式系统需要更高级别的安全性, 但是现在很少有系统具有授权的安全规范。 不幸的是, 对于大多数设计规范来说, 安全性是事后考虑的问题。 许多工程师和架构师认为软件可以保护系统, 所以他们只需要关注保护处理器或系统的IP。 然而, 事实并非如此。

设计工程师和架构师不仅需要在软件中实现安全特性, 还需要在硬件上实现。 硬件安全可以:
- 
保护产品不被克隆或过度生产

- 
保护知识产权, 使得企业能够不断地进行区分

- 
保护数据通信, 防止欺诈, 防止公司品牌被玷污


幸运的是, 可以选择中密度的FPGA, 以满足系统对安全的需求。

第一步是将设计的安全需求分解为两大类: 设计安全和数据安全。
- 
设计安全意味着采取措施防止从芯片中提取IP, 这将转化为差分功率分析(DPA)的对策。

- 
数据安全包括保护云和/或M2M通信, 利用安全存储, 实现物理上不可复制函数的密钥生成和一个抗DPA的加密引擎。


### 设计安全

保护 FPGA 的位流不被提取是很重要的，通常的方法是用密钥加密位流。 尽管对于黑客来说, 钥匙是一个障碍, 但是它们不足以满足今天的需求。 为什么呢？ 因为一种叫 DPA 的技术。

当设备执行诸如读取密钥或基于密钥解密文件等操作时, 它会发出磁信号。 这些信号可以通过廉价的电磁探测器探测到。 在一次测试中, DPA 技术被证明能够确定何时读取或访问一个密钥，一个探测器和 PC 或逻辑分析仪只需要几分钟就能找到信号中的模式并确定密钥是什么。 一旦拥有了这个密钥, 就可以解密 FPGA 的位流。

幸运的是, 有一种方法可以消除 DPA 现象，即在设备中建立对 DPA 有对抗措施的FPGA。当前只有少量的FPGA具有这种能力, 但都是高端和昂贵的。 然而, 经过成本优化的中密度FPGA, 如 Microsemi 的 PolarFire 系列, 采用了 DPA 对策, 应用广泛。 偏振式FPGA采用了设计特性来解决 DPA 泄漏问题, 并防止 DPA 攻击影响位流。

### 数据安全

越来越多的设备连接到其他设备或云端, 这使得系统的数据安全存在危险。光靠软件解决方案是不够的，硬件架构师同样需要承担解决这个问题的责任。为了确保数据通信的安全, 所发送的信息必须用特定的算法和密钥对接收端进行加密和解密。 有许多常见的算法(包括 AES-256、 SHA 和 ECC等)都必须基于一个需要使用的密钥。

对于与云计算的连接, 需要一种称为公钥基础设施(PKI)的双键策略。 PKI 基于公钥和私钥，网络上的每个节点都有一个被信任的第三方签名的公钥。 网络上的每个节点也都有它自己的私钥, 只有这个节点自己知道。

在安全通信时, 需要使用正在发送数据的节点中经过认证的公钥, 以及用于加密数据的私钥。 只有具有正确公钥和相应私钥的节点才能够对数据进行解密。 这是对云计算如何保护数据的基本描述。

但是硬件工程师可以解决两个重要问题： 第一种是 DPA: 密码引擎(FPGA 逻辑还是加密处理器)是否有内置的 DPA 对抗措施？ 如果不能, 则可以确定私钥, 否则数据通信可能会置于风险之中。

第二个问题是: 如何保护私钥？ 有许多硬件组件可以保护一个密钥, 但是最安全的系统是一个 PUF 被设计成 FPGA 设备。 利用了每个单独芯片的独特属性, 作为设备的一种生物识别标识。

通过 PUF 和NVM , 一个密钥能够以较高级别的方式来加密存储。当设计具有必须保护的数据通信时, 需调用FPGA的功能模块, 将 DPA 对策应用于安全的密钥生成和存储。

Polarfire FPGA 能够保护私钥并提供完全安全的数据通信(图9)。 所有关键的构建模块都包含在芯片上, 包括 DPA安全的加密处理器、 PUF、密钥存储和 RNG。 设计者只需要通过编程处理器来生成一个使用 RNG 的密钥和一个特定的安全协议(如 AES 256) 即可, 然后加载 PUF 提供的安全密钥存储, 密码处理器实现安全的通信！ 非常少的 FPGA 资源就能实现安全通信。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQNv1prX7Wibr1YxMRO8e23tYDyb3vrSVzESMZx0SxRiaIV95fm4QIWYXA/640?wx_fmt=png)

图9 这是由一个数据安全处理器、物理不合理函数(PUF)、随机数生成器(RNG)和安全非挥发性记忆体(NVM)组成的中密度 FPGA 功能框图。

随着安全设备变得越来越具有挑战性, 设计者需要技术来构建系统的固有安全。 使用中密度FPGA, 可以应对嵌入式工程师面临的新安全挑战, 包括通过 DPA 对策进行的设计安全和加密处理器的数据安全。

## 模拟仿真

硬件的模拟，调试和仿真一直是个老大难的问题，对于FPGA的设计更是如此。去年（2017年11约), Aldec 公司发布了验证平台 Riviera-PRO, 其中提供了QEMU Bridge 对SoC FPGA 上运行的设计进行软硬件模拟和仿真。

将 FPGA自定义的IP与软件应用程序以及驱动程序在 QEMU 中执行，能够简化 FPGA的集成和仿真，而且被证明是可行的。 QEMU Bridge将 SystemC TLM 事务转换为 AXI, 反之亦然, 为协同仿真提供了一个快速的接口。

随着设计规模和设备的复杂性不断增加, 系统集成变得越来越具有挑战性。QEMU Bridge提供的协同模拟方式使软硬件工程师都能够在开发周期的早期阶段展开工作, 以确定、识别和解决问题, 从而节省开发时间和成本。

硬件工程师可以在 HDL 中设置断点, 检查数据流, 甚至对于在QEMU中执行的软件应用程序，可以分析所运行的代码覆盖率和代码路径。 软件工程师在 QEMU中可以使用GDB对内核和驱动程序进行断点调试。

## 基于ARM 总线的接口实现

从最简单的接口APB到复杂的 AXI 主机，芯片设计者可以将FPGA 引入到 APB、 AHB和 ARM 体系结构的AXI等各种总线中。随着网络、深度学习和人工智能(AI)、航空航天等应用系统的需求日益增加, 芯片(SoC)和微控制器(MCU)设计对系统的需求也发生了变化。 如今, 在主 CPU 核心处理通用任务时, 还会使用DSP 和 FPGA IP 块的芯片。 这些架构使得整体处理解决方案更加高效而灵活。

特别是, 由于上述原因, 嵌入式FPGAs 与传统的 DSP 和 FPGA 不同, eFPGAs 可以设计成芯片, 作为可重构的 IP 块, 可以重新编程以处理不同的工作负载加速任务或管理不同的I/O (图10)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQfn3BE2ictwd4dJcRagwdzhA2CicvApXqAaiadGUtaOyZOKHyKcibIfwtBA/640?wx_fmt=jpeg)

图10 Flex Logix 的 EFLX eFPGA IP 作为可匹配的硬件加速块或重新编程的I/O 块在 MCU 和 SoC 设计中的实现

例如, 对于基于 ARM 架构的芯片设计者来说, eFPGA可以与 APB、 AHB 或 AXI 处理器总线相连, 以利用这些优势。 然而, 考虑到大量的组合(主 / 从 AXI/AHB/APB外部接口逻辑或/与 eFPGA 实现的组合）, 这些设计需要仔细考虑。

### eFPGA 的I/O资源

为了更好地理解 eFPGA 如何使用基于ARM的 MCU 或 SoC的标准接口进行设计, 这里使用 Flex Logix EFLX-2.5k 嵌入式 FPGA IP核看一个例子。

EFLX-2.5k eFPGA 是一个1x1阵列, 由2520个六输入查找表(LUTs)组成, 输入632个, 输出632个。 它在16纳米工艺中工作在1ghz 左右, 在28nm 工艺中运行在500mhz 以上。 因此, 即使是最小的 eFPGA 也有足够的I/O 资源和足够的速度连接到又宽又快的 ARM 总线上。

### eFPGA 作为 APB 的从设备

配置为 APB 的 eFPGA 通常将作为可重新配置的I/O函数来实现。 例如, 许多类型的串行接口，如 UART, I2C 和 SPI。

在这种配置中, 多个块连接到 APB 总线, 该总线每次运行一个块(图11)。 每个块都有内存映射地址空间(如8位128个寄存器) , 而总线上的数据可以是8位、16位或者32位。 8位地址空间和32位数据需要45个输入和33个输出, 而16位总线使用16个输入和16个输出。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQPOaUg1DVFHKKwWT26MYaB5JtjayhRhacNnOmmMzY3PYiapCB2RtnibRQ/640?wx_fmt=jpeg)

图11 配置为 APB 的 eFPGA从设备作为一个可配置的I/O函数实现, 其中包含多个内存映射块。 Apb 总线同步运行在这些块上。

APB 的从接口快速、简单, 可以在外部或 eFPGA中实现逻辑。 例如, 在这个配置中, 一个 EFLX eFPGA 需要少于10个 LUTs。 如图12所示, APB 的从接口中 Verilog 代码和状态图也很简短。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQEVE4ny6qXIB4BAEQGDYiaJ4l2RIvVKlW9yW9qQkRkuKn1wtRHtmEwfA/640?wx_fmt=jpeg)

图12 当 eFPGAs 配置为 APB 从属界面时, Verilog 代码和状态图很简单

### eFPGA作为 AXI的主设备

AXI 总线有三个版本: AXI4, AXI4流, AXI4lite。图13 展示了一个 AXI4主机, 其封装器由外部的 eFPGA实现。 AXI 主封装器的功能是处理所有总线协议, 管理数据流控制, 并在配置时从 AXI 总线上隔离 EFLX FPGA。

在这里, EFLX的可重新编程加速器充当总线主机, 这样，CPU 就不需要从内存传输数据，就可以卸载 CPU 以完成与 EFLX 加速器并行运行的其他任务。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQEVE4ny6qXIB4BAEQGDYiaJ4l2RIvVKlW9yW9qQkRkuKn1wtRHtmEwfA/640?wx_fmt=jpeg)

图13 eFPGA配置为 AXI 主设备, EFLX的可重新配置加速器可以卸载总线管理的 CPU

EFLX eFPGA 能够支持窄或宽(128字节)的 AXI 数据宽度, 这取决于加速器功能所需的带宽。 在 EFLX 阵列中的控制寄存器和 APB 的从逻辑在 EFLX 阵列中实现, 并控制 APB 总线。 图11所示的 APB 从接口示例可以实现控制寄存器接口。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQBJ6jO7G1qgUlJp58pm8H1mVzuZQuBFLM2xvh8DYEO1OCbpAUjwJSqQ/640?wx_fmt=jpeg)

图14显示了 AXI 总线体系结构的高层框图, 说明了主从连接

当 eFPGA 作为 AXI master 实现时, eFPGA 应该被用来简化关闭, 时间, 输入和输出。 数据总线宽度可以是1,2, 或4字节等, 最多可达128字节。

即使对于一个复杂的总线主设备, Verilog 代码也相对简单(图15)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQ8VhJX6FoHxgicY0dozR7wNaaU1B4E44wbVJ6veYnCicdibRP5IDTHw5og/640?wx_fmt=png)

图15 当使用 eFPGA 作为 AXI 总线这设备时, Verliog的简单代码

### eFPGA作为独立的从接口

除了作为加速器或I/O处理器的阵列操作总线连接之外, 像 EFLX 这样的 eFPGA还提供了单独的从接口, 用于按照需要重新配置。 一个外部的 AXI 从封装器也可以用来与配置端口的接口。

例如, 使用 EFLX, 配置位通常与 ARM 处理器代码一样存储在相同的闪存中。 在处理器启动后, 它使用直接内存访问(DMA)来配置由从配置到 eFPGA 的配置位访问。

随着现代工业解决方案对灵活性和性能的日益增长, eFPGA可以使用少量的高速逻辑, 轻松地连接到 ARM 总线的任何类型和数据宽度。 经过多年的复杂性和成本挑战, 基于 eFPGA 技术的可重新配置加速器和I/O处理器已经成为任何一种基于ARM的SoC或ASIC架构师的一种易用资源。

## eFPGA 的兴起

嵌入式 FPGA (eFPGA)技术并不是什么新想法。 事实上, 几十年来, 它已经以各种形式进行着部署实施。 那么, 为什么它突然得到如此多的关注呢？ 答案可能在于半导体产业内两个基本经济趋势的交汇。

首先, 随着每一个新流程的产生, 开发成本都会随之急剧上升。抽象设计本身的日益复杂性, 以及这些设计在实际 SoC 设备中的真实实现, 包括了诸如软件工具、工程时间和掩码成本等等。

其次, 这些设备中每个单位功能的成本一直在下降。 例如, 二十年前, FPGA 相对昂贵, 因此 被倾向于原型建立和前期生产, 而不是大批量生产。 试图将 FPGA 添加到一个 ASIC 中, 通常会增加整体的模具尺寸和复杂度, 使得新的混合设备变得太昂贵而不切实际。 而现在, 已经不是这样了。

相反, SoC 设计所涉及的高成本增加了与没有合适产品来满足特定市场需求的相关风险, 而且 FPGA 的成本相对较低, 这意味着嵌入式 FPGA 可以获得一定程度的设计灵活性(从而降低市场风险)，在经济上也是有意义的。

### eFPGA 的技术优势

然而, 除了解决这两个经济趋势所带来的挑战外, 基于eFPGA的设计方法还提供了其他一些潜在的好处。 例如, 通过在同一块芯片上放置一个 eFPGA IP 作为一个 SoC 中的其他功能模块, 还具有低功耗, 低延迟, 非常高的带宽连接等。 其结果是, 一个嵌入式 FPGA 块的 ASIC 具有更低的功耗, 更高的性能, 更低的成本, 并且比 ASIC + 普通FPGA 的解决方案使用更少的空间, 同时仍然能够保持设计的灵活性。

另一个优势是 eFPGA 的可编程特性使设计团队能够轻松地将SoC适应于新的、快速变化的或者略有不同的制造后市场需求, 从而延长产品在市场上的时间, 增加收入、毛利润和整体盈利能力。 拥有eFPGA的SoC可以成为一个特别有效的范例, 包括迅速支持新的或不断变化的接口标准, 增加一个新的功能, 以迅速应对新出现的竞争威胁, 或为物联网(IoT)等高度分散的市场创造多种产品变体。

最后, 在 SoC 设计中加入 eFPGA 技术可以提高整体设计性能, 同时减少总功耗。 在 FPGA的逻辑中, 有些功能具有更高的性能或者更高的功耗, 特别是如果它们需要在芯片处理器上的灵活性, 就像如今大多数 SoC 设计中所发现的那样。 此外, 通过使用大多数嵌入式 FPGA 技术, 设计工程师可以创建基于硬件的解决方案, 这些解决方案可以重新配置以适应特定的问题, 进一步提高设计性能，并降低功耗。

### 执行方面的考虑

为了最有效地获得嵌入 FPGA IP 的好处, 设计团队必须考虑其 eFPGA 实现的各个方面。 首先考虑的是综合工具与设计流程其他部分之间的整合质量。合成工具应该明确支持 eFPGA 架构, 并且应该能够生成一个最佳的设计 连线表, 以便更有效的实现设计。 例如, QuickLogic 与 Mentor 合作, 为其 eFPGA 技术提供了一个设计和开发环境, 特别地, 该软件能够支持该公司在eFPGA IP 中使用的 QuickLogic ArcticPro 架构。

综合在设计过程中起着关键作用, 因为它对设计的结果质量(QoR)影响最大。 一个 QoR 度量是多维的, 可能是一个频率、面积和功率的函数, 这取决于用户的设计要求。 对于合成工具来说, 目前的行业标准是 HDL 语言, 如 Verilog、 SystemVerilog (SV2009)和 VHDL (VHDL-2008)等行业标准 中的HDL 语言支持。 而且, 对于最佳的 QoR, 综合工具必须对目标体系结构有深入的了解, 以便以最佳方式映射到体系结构中的可用资源。

设计流程中的第二个考虑因素是 eFPGA 实现本身的软件质量, 因为它最终将决定 SoC 设备的效率、性能和成本。 FPGA架构及其相关的开发工具倾向于共同进化, 因为一个架构的变化可能会对另一个架构产生巨大的影响。因此, 那些已经售卖了数百万台FPGA的公司, 并且拥有数十年的开发经验, 以及为这些设备提供设计支持的软件, 这些公司更有可能提供良好的 eFPGA IP/软件解决方案。

在某些情况下, 经验丰富的 FPGA 供应商也已经学会了如何有效地将 eFPGA 技术整合到自己的 SoC 设备中, 这使他们能够充当自己的客户, 进一步完善 eFPGA 的 IP 服务。 例如, QuickLogic 开发了一个可听、可穿戴和物联网应用的传感器处理 SoC (图16)。 这个设备包含了eFPGA 技术, 拥有一个可编程逻辑块可以让客户快速实现新的传感器处理算法和接口标准。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczoRYicw9GQ95RWd9OnBv7ibEQKcMXpIZDics4DLE9540jLguPqOjj2sojF826PgHnibWCY9sORVDKP8UA/640?wx_fmt=png)

图16 EOS S3传感器处理平台框图

作为 eFPGA 工具本身的用户, QuickLogic很快了解了他们所需的工具, 以及如何最好地优化 eFPGA 实现设计流程。过程中, 他们遇到了各种问题并予以解决, 最终创建了集成 eFPGA IP 所需的完整工具和设计文件。

这些工具生成的文件包括设备包装文件、备份注释文件和布局数据。 设备封装器文件是一个连线表, 定义了 eFPGA的"黑盒"端口, 以及如何与SoC设计的其余部分相连接。后面的注释文件提供了对设计验证过程中至关重要的库和定时信息。布局数据包括设计团队用来实例化物理设计块的物理布局信息。

## eFPGA 在人工智能、物联网、安全等领域的应用

显然, SoC应用广泛, 而嵌入式FPGA技术只会扩展潜在的用例集。 然而, 在有一些应用场景中, 与传统的 SoCs 相比, SoC-eFPGA解决方案具有显著的优势。

从上面传感器处理的例子中看到, 其中含有嵌入式FPGA的 EOS S3 SoC 使得传感器算法和接口的更新能够实现, 而无需录制新版本的设备。 基于云的人工智能进行语音处理是人机交互的下一个重大进步, 并且有能力迅速和容易地支持亚马逊的 Alexa 等"智能扬声器"， 为产品的生态系统添加新的触发词。

高度分散的物联网市场是另一个很好的例子, eFPGA 技术能够为之提供巨大的好处。 整体市场是巨大的, 但是几乎没有一个应用能够单独满足大量的市场需求。 因此, 使用基于平台方法的一个 SoC 设计, 其"基础设施"实现每个应用共同的所有功能是有意义的。 然后, eFPGA 技术可以用来快速并低成本地创建多个产品变体来处理特定的应用， 随时间涌现的新应用也可以无需重新设计 ASIC 的情况下得以解决。

几乎任何包括大数据和深度学习在内的机器学习应用都可以利用大多数 eFPGA 解决方案所提供的可重构性。 可以对 eFPGA 块中的硬件进行配置, 然后根据需要进行重新配置, 以便廉价和有效地解决当今计算机所处理的一些复杂问题。

另一个很好的例子是需要不断更新的硬件安全算法。 随着新的安全漏洞被识别出来, 提供一个更新的防御解决方案变得至关重要, 这是一个完美适合于 eFPGA 硬件SoC的场景。

### eFPGA 技术的未来

由于嵌入式 FPGA 技术的发展和单位成本的下降趋势将使eFPGA 技术对 SoC 设计团队的吸引力越来越大, eFPGA 的发展前景是非常可观。 然而, 每一项复杂的技术都存在挑战。 其中一个是规模密度并不适合所有的eFPGA IP 架构组合, 以及跨不同的代工厂和流程节点的 SoC 实现。 还有一些潜在的一体化问题, 如需要不同数量的金属层, 管理时钟域边界, 以及协调不同的电源管理等。

虽然 eFPGA 技术已经以各种形式出现了一段时间, 但市场刚刚从早期采用转向主流阶段。 从移动设备、自动驾驶、可穿戴设备和物联网, 到大型数据和通信中心, 再到可重组计算平台, 乃至基于云计算的人工智能，这种趋势可能会在短期内加速。

但是, 只有当可编程逻辑在 SoC 中高效集成时, eFPGA 技术才能成功。 这需要技术、架构和软件工具的正确结合, 使开发团队拥有完整和有效的设计流程, 从而能够从概念到芯片无缝转移到制造后的市场调整和现成产品。

## 参考资料

http://www.embedded-computing.com/hardware/using-embedded-embedded-fpga-as-a-reconfigurable-accelerator

http://www.embedded-computing.com/processing/a-new-security-solution-in-mid-range-fpgas

http://www.embedded-computing.com/hardware/hardware-and-software-engineers-designing-soc-fpgas-stand-to-profit-from-aldec-qemu-bridge

http://www.embedded-computing.com/hardware/arm-bus-interfaces-for-embedded-fpgas

http://www.embedded-computing.com/hardware/now-is-the-time-for-efpga-technology







