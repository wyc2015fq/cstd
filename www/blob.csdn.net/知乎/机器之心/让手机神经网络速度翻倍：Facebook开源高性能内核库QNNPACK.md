# 让手机神经网络速度翻倍：Facebook开源高性能内核库QNNPACK - 知乎
# 



选自code.fb，作者：MARAT DUKHAN、YIMING WU、HAO LU，机器之心编译。

> 今天，Facebook 开源了一个高性能内核库——QNNPACK，专为移动 AI 进行优化。该内核库加速了许多运算，如深度类型的卷积，促进了神经网络架构的使用。QNNPACK 已经被整合进 Facebook 应用，部署到了数十亿台设备中。在 MobileNetV2 等基准测试中，QNNPACK 在各种手机上表现出的性能是当前最佳实现的两倍。

链接：[https://github.com/pytorch/QNNPACK](https://link.zhihu.com/?target=https%3A//github.com/pytorch/QNNPACK)

为了将最新的计算机视觉模型部署到移动设备中，Facebook 开发了一个用于低密度卷积的优化函数库——QNNPACK，用在最佳神经网络中。

QNNPACK 的全称是 Quantized Neural Network PACKage（量化神经网络包），是 Facebook 应用的一部分，已经被部署到全球数十亿台移动设备中。这个新库可以执行高级计算机视觉任务，如在手机上实时运行 Mask R-CNN 和 DensePose 或在性能受限的移动设备中用 100ms 以内的时间实施图像分类。

Facebook 开源 QNNPACK，为优化推理提供全方位的支持，作为构建 PyTorch 1.0 平台的一部分。QNNPACK 借助 Caffe2 模型表征即刻可用，Facebook 正在开发实用程序，将 PyTorch 的 Python 前端模型导出到图表征中。他们还在其他平台上优化这些运算，而不仅限于移动设备。

由于移动设备的计算力仅仅是数据中心服务器的十分之一到千分之一，运行当前最佳人工智能应用需要作出一些调整，压缩来自硬件的所有可用性能。QNNPACK 通过提供量化张量上的卷积、解卷积及全连接运算高性能实现来做到这一点。在 QNNPACK 之前，几个常见的神经网络基元（分组卷积、扩张卷积）缺乏良好的开源实现；因此，ResNeXt、CondenseNet 和 ShuffleNet 等颇有前景的研究模型没有得到充分利用。

**移动设备前沿 AI 技术新优化**

两年前，[Facebook 开始在手机上部署神经网络](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650720399%26idx%3D1%26sn%3D2b5e854e6606eaf556a73e83d179eb5c%26chksm%3D871b0cf1b06c85e7547b69a44471d377eca83cbdeaec379fabc187218280c761824a5600e1ac%26scene%3D21%23wechat_redirect)，多数计算机视觉架构随着大型内核被部署到卷积运算中。这些运算因计算强度高而饱受诟病：直接实现涉及每个加载元素的许多乘-加运算。Caffe2Go 使用的是一种叫做 NNPACK 的内核库，该库实现基于 Winograd 变换或快速傅立叶变换的渐近快速卷积算法，以减少卷积计算中的乘-加运算。例如，3×3 卷积比 1×1 卷积运算慢两倍，但使用直接算法要慢 9 倍。

计算机视觉领域发展迅猛，然而，这种新的神经网络架构使用的是几种无法从快速卷积算法中获益的卷积，即 1×1 卷积、分组卷积、转置卷积、空洞卷积和深度卷积。这些类型的卷积计算强度相对较低，因此可以通过利用低精度计算从内存降低的带宽中受益。

用于计算机视觉的神经网络将多数推理时间用在卷积和全连接算子中。这些算子与矩阵相乘紧密相关：全连接算子和 1×1 卷积直接映射到矩阵相乘，具有较大内核的卷积可以分解成一种名为 im2col 的内存布局转换和矩阵相乘的组合。因此，卷积神经网络中的有效推理问题很大程度上可以看做矩阵乘法的有效实现问题——在线性代数库中也称为 GEMM。




**实现矩阵相乘**

不直接在科学计算或者深度学习软件上工作的软件工程师可能不熟悉库是如何实现矩阵相乘的，所以在详细介绍 QNNPACK 之前，会有一个总体介绍。

在以下示例中，A 是输入，B 是权重，C 是输出。在推理过程中，B 从不变化，也因此不需要消耗时间就能迁移到任何方便的存储配置中。
![](https://pic2.zhimg.com/v2-277c0d0bb525fd0577156b2d1ff38f0d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
MxK 矩阵 A 与 KxN 矩阵 B 相乘得到 MxN 矩阵 C。C 中的每个元素都可以认为是 A 行与对应 B 列的点积。

在点积基元上实现整个矩阵相乘是可能的，但这样的实现过于低效。在一个点积中，每一个乘-加运算需要上传两个元素，在当前的处理器上，这一实现会受到内存和缓存带宽，而不是乘-加单元计算力的限制。但一个小小的修改——同时计算几行 A 和几行 B 的点积——却使得性能大大提升。

修改后的基元加载 A 的 MR 及 B 的 NR 元素，实施 MRxNR 乘积累加运算。MR 和 NR 的最大值受到整数个数和处理器架构其它细节的限制。但在多数现代系统中，这些最大值足够大，可以使运算只受计算限制，所有高性能矩阵乘法实现都建立在这个基元上，该基元通常被称为 PDOT（panel dot product）微内核。




**神经网络中的优化及 QNNPACK 如何提高效率**

PyTorch 及其它深度学习框架在训练期间通常利用浮点数来表示权重和神经网络的神经元。模型训练完成之后，浮点数及运算就会显得过分：许多类型的模型可以在调整后使用推理用的低精度整数运算，不会出现明显的准确率损失。低精度整数表征在单精度、甚至是半精度浮点上提供一些益处：内存占用减小 2/1 或 3/4，有助于将神经网络模型保存在移动处理器的小缓存中；提高内存带宽受限的运算性能；提高能源利用率；在许多类型的硬件上提高计算吞吐量。

QNNPACK 使用与安卓神经网络 API 兼容的线性量化方案。它假设量化值 q[i] 表示为 8 位无符号整数，并且它们与实值表示 r[i] 相关，公式如下：

r[i] = scale * (q[i] – zero_point)

公式中的 scale 是一个正浮点数，zero_point 是一个无符号的 8 位整数，就像 q[i] 一样。

尽管 QNNPACK 像其它 BLAS 库一样利用 PDOT 微内核，但它对具有 8 位元素的量化张量和移动 AI 用例的关注为性能优化带来了截然不同的视角。多数 BLAS 库针对的是矩阵高达数千个双精度浮点元素的科学计算用例，但 QNNPACK 的输入矩阵来自低精度、移动专用的计算机视觉模型，并且具有非常不同的维度。在 1×1 卷积中，K 是输入通道的数量，N 是输出通道的数量，M 是图像中像素的数量。在实用移动优化网络中，K 和 N 不超过 1024，取值范围通常在 32-256 之间。

由于移动架构的局限，MR 和 NR 不超过 8。因此即使是在有 1024 个通道的最大模型中，整个内存块在 PDOT 微内核中的读取速度也只能达到 16KB，即使在超低端移动内核上也能适用于一级缓存。这标志着 QNNPACK 和其他 GEMM 实现之间的一个重要区别：虽然其它库重新打包 A 和 B 矩阵以更好地利用缓存层次结构，希望在大量计算中分摊打包开销，但 QNNPACK 针对 A 和 B 的面板适用于一级缓存的情况进行了优化。因此，它的目的是删除所有计算非必需的内存转换。
![](https://pic1.zhimg.com/v2-0d7d799c90171a1138ce1bfe75a079b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
在量化矩阵-矩阵乘法中，8 位整数的乘积通常会被累加至 32 位的中间结果中，随后重新量化以产生 8 位的输出。常规的实现会对大矩阵尺寸进行优化——有时 K 太大无法将 A 和 B 的面板转入缓存中。为了有效利用缓存层次结构，传统的 GEMM 实现将 A 和 B 的面板沿 K 维分割成固定大小的子面板，从而每个面板都适应 L1 缓存，随后为每个子面板调用微内核。这一缓存优化需要 PDOT 为内核输出 32 位中间结果，最终将它们相加并重新量化为 8 位整数。

由于 ONNPACK 对于面板 A 和 B 总是适应 L1 缓存的移动神经网络进行了优化，因此它在调用微内核时处理整个 A 和 B 的面板。而由于无需在微内核之外积累 32 位的中间结果，QNNPACK 会将 32 位的中间结果整合进微内核中并写出 8 位值，这节省了内存带宽和缓存占用。
![](https://pic4.zhimg.com/v2-f440803270fbade1b4284219744df7fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
使整个 A、B 面板适配缓存帮助实现了 QNNPACK 中的另一个优化：取消了矩阵 A 的重新打包。矩阵 B 包含静态权重，可以一次性转换成任何内存布局，但矩阵 A 包含卷积输入，每次推理运行都会改变。因此，重新打包矩阵 A 在每次运行时都会产生开销。尽管存在开销，传统的 GEMM 实现还是出于以下两个原因对矩阵 A 进行重新打包：缓存关联性及微内核效率受限。如果不重新打包，微内核将不得不读取被潜在的大跨距隔开的几行 A。如果这个跨距恰好是 2 的许多次幂的倍数，面板中不同行 A 的元素可能会落入同一缓存集中。如果冲突的行数超过了缓存关联性，它们就会相互驱逐，性能也会大幅下降。幸运的是，当面板适配一级缓存时，这种情况不会发生，就像 QNNPACK 优化的模型一样。

打包对微内核效率的影响与当前所有移动处理器支持的 SIMD 向量指令的使用密切相关。这些指令加载、存储或者计算小型的固定大小元素向量，而不是单个标量（scalar）。在矩阵相乘中，充分利用向量指令达到高性能很重要。在传统的 GEMM 实现中，微内核把 MR 元素重新打包到向量暂存器里的 MR 线路中。在 QNNPACK 实现中，MR 元素在存储中不是连续的，微内核需要把它们加载到不同的向量暂存器中。越来越大的暂存器压力迫使 QNNPACK 使用较小的 MRxNR 拼贴，但实际上这种差异很小，而且可以通过消除打包开销来补偿。例如，在 32 位 ARM 架构上，QNNPACK 使用 4×8 微内核，其中 57% 的向量指令是乘-加；另一方面，gemmlowp 库使用效率稍高的 4×12 微内核，其中 60% 的向量指令是乘-加。

微内核加载 A 的多个行，乘以 B 的满列，结果相加，然后完成再量化并记下量化和。A 和 B 的元素被量化为 8 位整数，但乘积结果相加到 32 位。大部分 ARM 和 ARM64 处理器没有直接完成这一运算的指令，所以它必须分解为多个支持运算。QNNPACK 提供微内核的两个版本，其不同之处在于用于乘以 8 位值并将它们累加到 32 位的指令序列。




**默认微内核**

NEON 是 ARM 架构上的向量扩展（vector extension），它包含很多不寻常的指令。QNNPACK 中的默认微内核广泛使用了两种 NEON 特定类型的指令：「长」指令，产生的元素向量是其输入的两倍宽；向量暂存器与另一向量暂存器中的元素相乘。微内核加载 8 位整数（无正负之分）的向量，将其扩展到 16 位，并使用向量 x 标量+长指令（VMLAL.S16 in AArch32 and SMLAL/SMLAL2 in AArch64）的结果与累加到 32 位的 16 位元素相乘。

ARM NEON 提供了一条指令（VSUBL.U8 on AArch32 and USUBL/USUBL2 on AArch64）来减去 8 位整数的向量并产生 16 位整数结果的向量，在大多数 ARM 微架构中，这条指令和简单的整数扩展指令（VMOVL.U8 on AArch32 and UMOVL/UMOVL2 on AArch64）一样快。作为额外的优化，微内核结合了 A 和 B 矩阵元素零点的减法和从 8 位整数到 16 位整数的扩展。




**双发射微内核（Dual-issue microkernel）**

默认微内核使用最少的命令，因此它在低端核上的性能最优，但每个周期默认微内核仅能执行一个 NEON 命令。类似地，高端 Cortex-A 内核也是每个周期仅能执行一次 NEON 整数乘法命令，但是它至少能够并行执行 NEON 整数乘法和 NEON 整数加法命令。因此理论上通过精心写并行执行两个命令的汇编代码可以改进性能：vector multiply long (VMULL.U8 in AArch32, UMULL in AArch64) 乘 8-bit 元素得到 16-bit 乘积；向量成对相加（vector pairwise add）(VPADAL.U16 in AArch32, UADALP in AArch64) 加邻近的 16-bit 乘积得到 32-bit 结果。假设向量相乘（vector multiply）和向量成对相加命令的调度完美，则双发射微内核每个周期可输出 8 个乘加结果，是默认微内核的 2 倍。

在高端 Cortex-A 内核上实际利用双发射能力较为复杂，原因如下：一，在高端 Cortex-A 内核上的双发射能力并不完美，可以维持两个周期内执行三个命令的速度；二，NEON 不支持 8-bit 整数向量的 vector-by-scalar 乘法，因此研究中使用的是向量乘法以及额外的命令 (VEXT.8 on AArch32, EXT on AArch64)，以旋转矩阵 A 中的向量；三，在 8-bit 元素上执行乘法，则无法在乘法之前减去零点（减去后结果的宽度是 9bit），需要预计算 A 的行的总和以在重新量化之前调整累加的 32-bit 结果。

尽管上述因素导致了一些开销，但在 Cortex-A75 核上，利用双发射能力的微内核对于较大的通道数（K > 64）速度提升了 15% 到 20%。




**从矩阵相乘到卷积**
![](https://pic2.zhimg.com/v2-40dcfac449248e0cc597e357606641e1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
简单的 1×1 卷积可直接映射到矩阵相乘，但对于具备较大卷积核、padding 或子采样（步幅）的卷积而言则并非如此。但是，这些较复杂的卷积能够通过记忆变换 im2col 映射到矩阵相乘。对于每个输出像素，im2col 复制输入图像的图像块并将其计算为 2D 矩阵。由于每个输出像素都受 KHxKWxC 输入像素值的影响（KH 和 KW 分别指卷积核的高度和宽度，C 指输入图像中的通道数），因此该矩阵的大小是输入图像的 KHxKW 倍，im2col 给内存占用和性能都带来了一定的开销。和 Caffe 一样，大部分深度学习框架转而使用基于 im2col 的实现，利用现有的高度优化矩阵相乘库来执行卷积操作。

Facebook 研究者在 QNNPACK 中实现了一种更高效的算法。他们没有变换卷积输入使其适应矩阵相乘的实现，而是调整 PDOT 微内核的实现，在运行中执行 im2col 变换。这样就无需将输入张量的实际输入复制到 im2col 缓存，而是使用输入像素行的指针设置 indirection buffer，输入像素与每个输出像素的计算有关。研究者还修改了矩阵相乘微内核，以便从 indirection buffer 加载虚构矩阵（imaginary matrix）A 的行指针，indirection buffer 通常比 im2col buffer 小得多。此外，如果两次推断运行的输入张量存储位置不变，则 indirection buffer 还可使用输入张量行的指针进行初始化，然后在多次推断运行中重新使用。研究者观察到具备 indirection buffer 的微内核不仅消除了 im2col 变换的开销，其性能也比矩阵相乘微内核略好（可能由于输入行在计算不同输出像素时被重用）。




**QNNPACK 和深度卷积**

分组卷积（grouped convolution）将输入和输出通道分割成多组，然后对每个组进行分别处理。在有限条件下，当组数等于通道数时，该卷积就是深度卷积，常用于当前的神经网络架构中。深度卷积对每个通道分别执行空间滤波，展示了与正常卷积非常不同的计算模式。因此，通常要向深度卷积提供单独实现，QNNPACK 包括一个高度优化版本 3×3 深度卷积。

深度卷积的传统实现是每次都在卷积核元素上迭代，然后将一个卷积核行和一个输入行的结果累加到输出行。对于一个 3×3 的深度卷积，此类实现将把每个输出行更新 9 次。在 QNNPACK 中，研究者计算所有 3×3 卷积核行和 3×3 输入行的结果，一次性累加到输出行，然后再处理下个输出行。

QNNPACK 实现高性能的关键因素在于完美利用通用暂存器（GPR）来展开卷积核元素上的循环，同时避免在 hot loop 中重新加载地址寄存器。32-bit ARM 架构将实现限制在 14 个 GPR。在 3×3 深度卷积中，需要读取 9 个输入行和 9 个卷积核行。这意味着如果想完全展开循环必须存储 18 个地址。然而，实践中推断时卷积核不会发生变化。因此 Facebook 研究者使用之前在 CxKHxKW 中的滤波器，将它们封装进 [C/8]xKWxKHx8，这样就可以仅使用具备地址增量（address increment）的一个 GPR 访问所有滤波器。（研究者使用数字 8 的原因在于，在一个命令中加载 8 个元素然后减去零，在 128-bit NEON 暂存器中生成 8 个 16-bit 值。）然后使用 9 个输入行指针，指针将滤波器重新装进 10 个 GPR，完全展开滤波器元素上的循环。64-bit ARM 架构相比 32-bit 架构，GPR 的数量翻了一倍。QNNPACK 利用额外的 ARM64 GPR，一次性存储 3×5 输入行的指针，并计算 3 个输出行。
![](https://pic3.zhimg.com/v2-3058d072672bfda5d1fc0badf215afd2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
**QNNPACK 的性能优势**

测试结果显示出 QNNPACK 在端到端基准上的性能优势。在量化当前最优 MobileNetV2 架构上，基于 QNNPACK 的 Caffe2 算子的速度大约是 TensorFlow Lite 速度的 2 倍，在多种手机上都是如此。除了 QNNPACK 之外，Facebook 还开源了 Caffe2 quantized MobileNet v2 模型，其 top-1 准确率比相应的 TensorFlow 模型高出 1.3%。

Caffe2 quantized MobileNet v2 模型开源地址：[https://github.com/caffe2/models/tree/master/mobilenet_v2_quantized](https://link.zhihu.com/?target=https%3A//github.com/caffe2/models/tree/master/mobilenet_v2_quantized)




**MobileNetV1**

MobileNetV1 架构在使用深度卷积（depthwise convolution）使模型更适合移动设备方面具备开创性。MobileNetV1 包括几乎整个 1×1 卷积和 3×3 卷积。Facebook 研究者将量化 MobileNetV1 模型从 TensorFlow Lite 转换而来，并在 TensorFlow Lite 和 QNNPACK 的 32-bit ARM 设备上对 MobileNetV1 进行基准测试。二者运行时均使用 4 线程，研究者观察到 QNNPACK 的运行速度几何平均值是 TensorFlow Lite 的 1.8 倍。
![](https://pic3.zhimg.com/v2-2e0231325cd90405cb35be18b207caea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='780' height='458'></svg>)



**MobileNetV2**

作为移动视觉任务的当前最优架构之一，MobileNetV2 引入了瓶颈构造块和瓶颈之间的捷径连接。研究者在 MobileNetV2 分类模型的量化版上对比基于 QNNPACK 的 Caffe2 算子和 TensorFlow Lite 实现。使用的量化 Caffe2 MobileNetV2 模型已开源，量化 TensorFlow Lite 模型来自官方库：[https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/g3doc/models.md](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/lite/g3doc/models.md)。下表展示了二者在常用测试集上的 top1 准确率：
![](https://pic1.zhimg.com/v2-59a629f67abf323ec3fad24ffcf41a40_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='780' height='300'></svg>)
Facebook 研究者利用这些模型建立了 Facebook AI 性能评估平台（[https://github.com/facebook/FAI-PEP](https://link.zhihu.com/?target=https%3A//github.com/facebook/FAI-PEP)）的基准，该基准基于 32-bit ARM 环境的大量手机设备。对于 TensorFlow Lite 线程设置，研究者尝试了一到四个线程，并报告了最快速的结果。结果显示 TensorFlow Lite 使用四线程的性能最优，因此后续研究中使用四线程来对比 TensorFlow Lite 和 QNNPACK。下表展示了结果，以及在典型智能手机和高端机上，基于 QNNPACK 的算子速度比 TensorFlow Lite 快得多。
![](https://pic3.zhimg.com/v2-ac21ea2813795e65d7e7e18775ca5f06_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='780' height='472'></svg>)



**展望**

QNNPACK 已经帮助 Facebook 的 app 在全世界的移动设备中部署人工智能。研究者正在尝试进一步提升 QNNPACK 的性能，包括 FP16 格式的低精度计算，利用 NEON 点积（VDOT）和 16-bit 累积（16-bit accumulation）来使移动设备上的 AI 更加轻便。

Facebook 期待通过 PyTorch API 提供 QNNPACK 算子支持，以及为移动开发者提供工具。Facebook 希望 QNNPACK 能够通过提升模型的移动性能惠及 AI 研究者和开发者。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文链接：[https://code.fb.com/ml-applications/qnnpack/](https://link.zhihu.com/?target=https%3A//code.fb.com/ml-applications/qnnpack/)


