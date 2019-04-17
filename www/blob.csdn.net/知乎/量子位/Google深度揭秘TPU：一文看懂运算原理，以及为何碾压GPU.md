# Google深度揭秘TPU：一文看懂运算原理，以及为何碾压GPU - 知乎
# 



在发布了TPU论文之后，Google今天又在谷歌云官方博客上发文，详解了TPU是如何处理运算的，与CPU、GPU和核心差异在哪里。

量子位编译了这篇文章的要点，原文地址：[https://cloud.google.com/blog/big-data/2017/05/an-in-depth-look-at-googles-first-tensor-processing-unit-tpu](https://link.zhihu.com/?target=https%3A//cloud.google.com/blog/big-data/2017/05/an-in-depth-look-at-googles-first-tensor-processing-unit-tpu)

> 
李林 舒石 编译整理

量子位 报道 | 公众号 QbitAI


搜索、街景、照片、翻译，这些Google提供的服务，都使用了Google的TPU（张量处理器）来加速背后的神经网络计算。
![](https://pic1.zhimg.com/v2-bcc8ad98a05042d1003d35f80c3074c4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='458' height='218'></svg>)

△ 在PCB板上的Google首款TPU和部署了TPU的数据中心

去年Google推出TPU并在近期对这一芯片的性能和架构进行了详细的研究。简单的结论是：TPU与同期的CPU和GPU相比，可以提供15-30倍的性能提升，以及30-80倍的效率（性能/瓦特）提升。

这意味着，Google的服务既可以大规模运行于最先进的神经网络，而且可以把成本控制在可接受的程度上。以下的内容，将深入解读Google TPU中的技术，并且讨论如何实现更为出色的性能。

# 通往TPU之路

早在2006年，Google就在考虑为神经网络构建一个专用集成电路（ASIC）。2013年这个需求变得更加紧迫，当时Google意识到快速增长的计算需求，可能意味着数据中心的数量需要翻番才能满足。

通常而言，ASIC的开发需要耗时数年。但具体到TPU而言，从设计到验证、构建和部署到数据中心里，只需要15个月。

TPU ASIC采用了28nm工艺制造，主频700MHz，功耗40W。为了尽快把TPU部署到现有的服务器中，Google选择把这个芯片打包成外部扩展加速器，然后插到SATA硬盘插槽里使用。所以TPU通过PCIe Gen3 x16总线与主机相连，也就是说12.5GB/s的有效带宽。

# 用神经网络预测

要说明TPU的设计思路，需要先来简介一下神经网络的计算。
![](https://pic4.zhimg.com/v2-a31be7fe07838a59401c5e66187eee13_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='411'></svg>)
这是一个TensorFlow Playground的例子。用以训练一个神经网络，以标签对数据进行分类，或者对缺失数据进行估计，或者推断未来的数据。对于推断来说，神经网络中的每个神经元都进行如下计算：
- 
输入数据(x)乘以权重(w)以表示信号强度

- 
乘积加总，成为代表神经元状态的唯一值

- 
应用激活函数(f)，例如ReLU、Sigmoid等调节神经元

![](https://pic3.zhimg.com/v2-8529630529357a361956ed670004b996_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='782' height='342'></svg>)![](https://pic4.zhimg.com/v2-81d6cd7938724fd3746884cd02f00e2f_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='752' height='356'></svg>)
△ 神经网络把输入数据与权重矩阵相乘，并输入激活函数

例如，对于有三个输入数据和两个全连接神经元的单层神经网络而言，需要把输入和权重进行六次相乘，并得出两组乘积之和。这个乘法和加法序列，可以写成一个矩阵乘法，然后通过激活函数进一步处理矩阵的输出。

在更复杂的神经网络架构中，乘法矩阵通常也是计算量最大的部分。

实际业务中需要多少次乘法运算？2016年7月，Google团队调查了实际业务中，六个有代表性的神经网络应用，结果如下表所示：![](https://pic2.zhimg.com/v2-56a26b92558f26dd20304d9554a9cbdd_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1372' height='546'></svg>)

如上表所示，每个神经网络中的权重数量从500万到1亿不等。每一个预测，都需要许多步的输入数据和权重矩阵相乘，并输入到激活函数中。

总而言之，计算量超大。作为优化的第一步，Google应用了一种称为量化的技术进行整数运算，而不是在CPU或者GPU上对所有数学工作进行32位或者16位浮点运算。这能减少所需的内存容量和计算资源。

# 神经网络中的量化

通常而言，神经网络的预测不需要32位或16浮点计算精度，通过一些方法，可以用8位整数对神经网络进行预测，并保持适当的准确度。

所谓量化，就是一种使用8位整数来近似预设的最小值和最大值之间任意数值的优化技术。![](https://pic1.zhimg.com/v2-0494327fe79e4fe35c2a8199df2d72c4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1598' height='810'></svg>)

△ TensorFlow中的量化

量化是降低神经网络预测成本的利器，同时带来的内存减少也很重要，特别是对于移动和嵌入式部署。举个例子，在Inception中应用量化之后，这个图像识别模型能从91MB压缩到23MB，成功瘦身四分之三。

使用整数而不是浮点计算，大大减小了TPU的硬件尺寸和功耗。一个TPU钟包含65,536个8位整数乘法器。云环境中使用的主流GPU，通常包含数千个32位浮点乘法器。只要能用8位满足精度需求，就能带来25倍以上的性能提升。

# RISC，CISC和TPU指令集

可编程性是TPU的另一个重要设计目标。TPU不是设计用来运行某一种神经网络，而是要能加速许多不同类型的模型。

大多数当代CPU都采用了精简指令集(RISC)。但Google选择复杂指令集(CISC)作为TPU指令集的基础，这一指令集侧重于运行更复杂的任务。

我们来看看TPU的结构图。![](https://pic2.zhimg.com/v2-0a6f0b0d9b2b618aaf57a70c6f2c47f9_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='470' height='357'></svg>)

TPU包括以下计算资源：
- 
矩阵乘法单元(MUX)：65,536个8位乘法和加法单元，运行矩阵计算

- 
统一缓冲(UB)：作为寄存器工作的24MB容量SRAM

- 
激活单元(AU)：硬件连接的激活函数


为了控制MUX、UB和AU进行计算，Google定义了十几个专门为神经网络推理而设计的高级指令。以下是五个例子。![](https://pic3.zhimg.com/v2-146be2e42bd203e1078e5aadad1a4916_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1372' height='508'></svg>)

简而言之，TPU设计封装了神经网络计算的本质，可以针对各种神经网络模型进行编程。为了编程，Google还创建了一个编译器和软件栈，将来自TensorFlow图的API调用，转化成TPU指令。![](https://pic1.zhimg.com/v2-916ff726f6fe34e9db39de623e192178_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='515'></svg>)


△ 从TensorFlow到TPU：软件堆栈

# 矩阵乘法单元的并行计算

典型的RISC处理器提供简单计算的指令，例如乘法或加法。这些事所谓的标量（Scalar）处理器，因为它们每个指令处理单一运算，即标量运算。

即使主频千兆赫兹的CPU，仍然需要很长时间才能通过一系列标量运算来完成大型矩阵的计算。改进的方法就是矢量（Vector）运算，同时针对多个数据元素执行相同的操作。

GPU的流处理器（SM）就是一种高效的向量处理器，赞单个时钟周期内，可以处理数百到数千次运算。![](https://pic1.zhimg.com/v2-3995aee805bd9c8f475dbe20d5714240_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1480' height='622'></svg>)

至于TPU，Google为其设计了MXU作为矩阵处理器，可以在单个时钟周期内处理数十万次运算，也就是矩阵（Matrix）运算。

# TPU的核心：脉动阵列

MXU有着与传统CPU、GPU截然不同的架构，称为脉动阵列（systolic array）。之所以叫“脉动”，是因为在这种结构中，数据一波一波地流过芯片，与心脏跳动供血的方式类似。![](https://pic1.zhimg.com/v2-02db4ecb05839a3782659dcdb06daac4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='412'></svg>)

如图所示，CPU和GPU在每次运算中都需要从多个寄存器（register）中进行存取；而TPU的脉动阵列将多个运算逻辑单元（ALU）串联在一起，复用从一个寄存器中读取的结果。

MXU中的权值阵列专门为矩阵乘法运算进行了优化，并不适用于通用计算。![](https://pic3.zhimg.com/v2-f147c7e0a1aa2167f8720348583bc9e6_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='398'></svg>)

△ 脉动阵列中，输入向量与权值矩阵相乘![](https://pic1.zhimg.com/v2-f615b3df1815f46899666f2eb8545878_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='776' height='420'></svg>)

△ 脉动阵列中，输入矩阵与权值矩阵相乘

MXU的脉动阵列包含256 × 256 = 65,536个ALU，也就是说TPU每个周期可以处理65,536次8位整数的乘法和加法。

TPU以700兆赫兹的功率运行，也就是说，它每秒可以运行65,536 × 700,000,000 = 46 × 1012次乘法和加法运算，或每秒92万亿（92 × 1012）次矩阵单元中的运算。![](https://pic4.zhimg.com/v2-23215c60814868cc009fdd1120961f33_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='926' height='684'></svg>)

△ TPU中的MXU

我们对CPU、GPU和TPU的每周期算术运算量进行一下对比：![](https://pic2.zhimg.com/v2-169d6ee6adf6e0e14e9e0c6c75216a69_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1352' height='372'></svg>)


这种基于复杂指令集计算（CISC）的矩阵运算设计，实现了出色的性能功耗比：TPU的性能功耗比，比同时期的CPU强83倍，比同时期的GPU强29倍。![](https://pic1.zhimg.com/v2-0db2229a04065609e743ae05673448c0_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1194' height='652'></svg>)

# 极简&确定性的设计

极简这一点，在Google之前发布的TPU论文第8页提到过。与CPU和GPU相比，单用途的TPU就是一个单线程芯片，不需要考虑缓存、分支预测、多道处理等问题。

TPU的设计之简洁，从冲模平面图上就能看出来：![](https://pic3.zhimg.com/v2-836f2d50a7b3ba118f53dfedfab9a09a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='474' height='384'></svg>)

△ 黄色代表运算单元；蓝色是数据单元；绿色是I/O，红色是控制逻辑单元。

与CPU和GPU相比，TPU的控制单元更小，更容易设计，面积只占了整个冲模的2%，给片上存储器和运算单元留下了更大的空间。而且，TPU的大小只有其他芯片的一半。硅片越小，成本越低，良品率也越高。

而确定性，是单用途带来的另一个优势。CPU和GPU需要考虑各种任务上的性能优化，因此会有越来越复杂的机制，带来的副作用就是这些处理器的行为非常难以预测。

而用TPU，我们能轻易预测运行一个神经网络、得出预测，需要多长时间，这样，我们能让芯片以吞吐量接近峰值的状态运行，同时严格控制延迟。

以上面提到的MLP0为例，在同样将延迟控制在7毫秒之内的情况下，TPU的吞吐量是CPU和GPU的15到30倍。![](https://pic1.zhimg.com/v2-9ec4c4d16acaaa78e244fe42bb5d8a00_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1324' height='790'></svg>)

△ 各种处理器上每秒可运行的MLP0预测

下面，是TPU、CPU、GPU在六种神经网络上的性能对比。在CNN1上，TPU性能最为惊人，达到了CPU的71倍。![](https://pic3.zhimg.com/v2-a93b6fc74f896994a63c06a20733b3d6_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1354' height='822'></svg>)

# 总结

如上文所述，TPU性能强劲的秘诀，是因为它专注于神经网络推断。这使得量化选择、CISC指令集、矩阵处理器和最小设计都成为可能。

神经网络正推动计算模式的转变，Google预计未来几年中，TPU将成为快速、智能和价格实惠的重要芯片。

作者：

Kaz Sato（谷歌云Staff Developer Advocate）

Cliff Young（谷歌大脑软件工程师）


David Patterson（谷歌大脑杰出工程师）


