# SysML 2019论文解读：视频分析系统的提升 - 知乎
# 



> SysML 2019 已于 3 月 31 日-4 月 2 日在斯坦福成功举办，本文将解读两篇在高效视频分析方面的研究。

**机器之心原创，作者：Andy Xiong，参与：Hao Wang、Panda。**

系统与机器学习会议（SysML）是一个非常新的会议（始于 2018 年），针对的是系统与机器学习的交叉领域。该会议的目标是引出这些领域之间的新联系，包括确定学习系统的最佳实践和设计原理，以及为实际机器学习工作流程开发全新的学习方法和理论。

**前言**

随着视频摄像头的日益普及，视频分析也吸引到了很多研究关注。因为[目标检测](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759989%26idx%3D2%26sn%3Df6f8facb1fdbde0a4c4a745b14b585c9%26chksm%3D871aa74bb06d2e5d77eeb38b345d13bd6bd977f194b9f9e94ac5aec7eacad7108982b9b2cfed%26token%3D141655712%26lang%3Dzh_CN)和[语义分割](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759989%26idx%3D2%26sn%3Df6f8facb1fdbde0a4c4a745b14b585c9%26chksm%3D871aa74bb06d2e5d77eeb38b345d13bd6bd977f194b9f9e94ac5aec7eacad7108982b9b2cfed%26token%3D141655712%26lang%3Dzh_CN)等大多数计算机视觉任务都需要密集的计算，所以有必要设计出在软件开发和硬件实现方面都高效的系统。下面介绍两篇遵循这些思想解决视频分析问题的论文。

第一篇论文提出了 Filter-Forward，这是一种新的边缘计算到云计算（edge-to-cloud）系统，能通过安装仅回传相关视频帧的轻量级边缘滤波器让基于数据中心的应用有能力处理数千个摄像头的内容。第二篇论文提出了 FixyNN，其包含一个用于生成通用的 CNN 特征的固定权重的特征提取器，还有一个用于处理特定数据集的 CNN 的典型的可编程 CNN 加速器。

声明：本文的所有图片和公式都来自于原论文。

### **论文1：Scaling Video Analytics on Constrained Edge Nodes**
- 地址：[http://www.sysml.cc/doc/2019/197.pdf](https://link.zhihu.com/?target=http%3A//www.sysml.cc/doc/2019/197.pdf)

**引言**

为了有效实现交通流量监控和行人检测等应用，需要解决两个问题：视频流是广角的且相关数据稀少，而数据中心需要高分辨率视频流来进一步处理。在这篇论文中，研究者设计了一个名为 FilterForward（FF）的系统，能为广域视频处理提供边缘计算和基于数据中心的方法的双重优势。FilterForward 引入了快速且表现力强的每应用（比如「向我发送包含狗的序列」）「微分类器」（MC）来识别与数据中心应用最相关的视频序列（过滤）并仅将匹配的事件传输给数据中心以便进一步分析（转发）。

**FilterForward 架构**

大多数边缘计算到云计算的视频分析系统都面临着三个关键挑战：带宽限制、检测现实视频中的微妙细节、支持许多并发应用。为了解决这些难题，FilterForward 为应用提供了将工作划分成边缘和云两部分的灵活性，即在边缘利用高质量的数据为云提供相关的视频序列。图 1 给出了 FilterForward 的整体架构。FilterForward 的两个主要组件是特征提取器和微分类器。
![](https://pic3.zhimg.com/v2-08208c9d988342b119ae442cd910c06e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='792' height='303'></svg>)
特征提取器的输入是实时的相机帧，并使用深度神经网络（DNN）提取一些常见的特征。每应用微分类器共享特征提取器，以在计算有限的边缘节点上同时检测数十个事件（就像语义滤波器）。微分类器是轻量级的二元分类神经网络，其以基础 DNN 提取的特征图为输入，输出一个帧与一个特定应用相关的概率。FilterForward 能通过并行地评估许多微分类器来扩展到许多独立的应用（比如找到狗或找到自行车）。

为了以不同方式解决广角监控视频的难题，研究者在这篇论文中提出了三种定制的微分类器架构：全帧目标检测器（Full-frame Object Detector）、局部化二元分类器（Localized Binary Classifier）以及窗口化的局部化二元分类器（Windowed, Localized Binary Classifer）；如图 2 所示。一方面，这些设计可以操作来自各种基础 DNN 层的激活，以关注它们各自任务的适当特征；另一方面，还可以选择裁剪掉帧中不相关的区域。
![](https://pic4.zhimg.com/v2-f92895550fc9ce3456441f523fd3250f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='417'></svg>)
全帧目标检测器微分类器的建模方法跟随了 SSD 和 Faster R-CNN 等滑动窗口式目标检测器，其在卷积层特征图的每个位置都应用一个小型的二元分类 DNN，然后将检测结果聚合起来得到全局预测。

局部化二元分类器微分类器包含两个单独的卷积层和一个全连接层，可以处理空间上裁剪的特征图，比如检测局部化区域内的突出目标（即，比如缩放到原始帧的一个区域）。

窗口化的局部化二元分类器微分类器是对局部化二元分类器微分类器的扩展，是为了通过指定 W 帧的时间窗口来集成临近的时间背景信息，从而提升每帧的准确度。

**评估**

FilterForward 系统的主要目标是在实现高过滤准确度的同时限制带宽用量并可扩展到多个应用。研究表明，FilterForward 能够通过在微分类器之间共享计算而在商用硬件上实现较高帧率，同时还能在两个事件检测任务上维持较高的事件级准确度。研究者在两个真实世界的相机流数据集上进行了评估，结果表明 FilterForward 在高难度的视频内容上能够提升计算效率和事件检测准确度，同时还能显著降低网络带宽的使用。

**数据集**

为了在真实世界的监控任务上评估所提出的 FilterForward 系统，研究者收集了两组真实世界视频。第一个数据集的视频来自怀俄明州杰克逊霍尔的一个交通摄像头（Jackson 数据集）。第二个数据集是来自一个城市街道的两段三小时长的视频（Roadway 数据集）。图 3 给出了这些数据的细节。
![](https://pic3.zhimg.com/v2-5091cd161bb229ef42934e8c652e2d8a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='990'></svg>)
**以事件为中心的指标：事件 F1 分数**

大多数分类指标都以每帧为基础，而 FilterForward 则是以事件为中心的。研究者针对跨越多帧的事件采用了来自近期研究工作的一个修改版的召回（recall）指标。对于事件 i，所得到的 EventRecall_i 指标衡量的是两个成功度度量：
- Existence_i 奖励的是至少检测到一个事件的一帧
- Overlap_i 奖励的是检测到一个事件的更大比例的帧
![](https://pic3.zhimg.com/v2-88747018bca611300323bc86323a423e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='422' height='151'></svg>)
其中 R_i 和 P_i 分别是基本真值和预测的事件范围。EventRecall 是 Existence 和 Overlap 的线性组合。作者选择了 α = 0.9 和 β = 0.1 的配置，说明更看重检测到每个事件的单帧。

与此同时，研究者还将标准的精度（预测的帧占真正例中的比例）与修改过的 EventRecall 定义结合起来计算了一个事件 F1 分数——精度和召回的调和平均值，并在整个评估阶段使用了这一指标。

**节省广域带宽**

FilterForward 的主要目标是节省边缘计算到云计算的带宽。图 4 展示了在 Roadway 数据集上两种微分类器架构（全帧二元分类器和局部化二元分类器）的带宽用量和事件 F1 分数，并对比了压缩整个数据流的方法。对于这两种微分类器，相比于高质量地将整个视频流发送回数据中心，FilterForward 可将网络带宽降低接近一个数量级。
![](https://pic4.zhimg.com/v2-d37ff404a0c23e9fd77cec7f1f3b53f7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='552' height='650'></svg>)
**端到端的性能可扩展性**

为了评估性能可扩展性以及展示微分类器的低边际成本，作者对比了三种微分类器与另外两种过滤技术：直接运行一个完全 DNN（[MobileNet](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759989%26idx%3D2%26sn%3Df6f8facb1fdbde0a4c4a745b14b585c9%26chksm%3D871aa74bb06d2e5d77eeb38b345d13bd6bd977f194b9f9e94ac5aec7eacad7108982b9b2cfed%26token%3D141655712%26lang%3Dzh_CN)）的多个实例以及训练专门的像素级分类器。图 5 展示了 FilterForward 的三种微分类器架构以及多 DNN 和 NoScope 式离散分类器（DC）的过滤吞吐量。离散分类器比通用型分类 DNN 速度更快，但成本比微分类器更高。

结果表明，当分类器的数量较低时（大约少于 4），离散分类器有更高的吞吐量，因为此时基础 DNN 的成本在 FilterForward 占主导。一旦 FilterForward 运行的微分类器数量大约 4，其计算共享的设计就能达到成本摊销的突破点。而运行多个 DNN 一直都更加低效，并且在超过 35 个实例后就会耗尽内存。
![](https://pic4.zhimg.com/v2-288ca98aa03ade7d1fd1bca822b96953_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='868'></svg>)
为了进一步理解 FilterForward 的吞吐量可扩展性，研究者还衡量了基础 DNN 和微分类器在每帧上所用的时间。图 6 展示了三种新提出的微分类器架构的细分结果。在查询较少时，基础 DNN 的执行时间占主要，这在意料之中。但即使增加数十个并发的微分类器，总的执行时间也只会适量增长。在最坏的情况下，基础 DNN 的 CPU 时间基本等于 20 个微分类器的时间。
![](https://pic3.zhimg.com/v2-2fb35d0392a969c5ea8aeafcedf68f3e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='952' height='269'></svg>)
**微分类器的成本和准确度**

图 7 展示了微分类器和离散分类器在两个真实世界数据集上的准确度（使用事件 F1 分数）和计算成本（以乘法-加法的数量衡量）。实验结果表明，微分类器的边际成本比离散分类器低得多，并且还能实现更高的准确度。
![](https://pic1.zhimg.com/v2-5e6af461f3ba0af636f91b63643a6edc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1190'></svg>)
**总结**

这篇论文解决的问题是：基于数据中心的应用需要高保真的视频序列，而每个相机又只有有限的带宽。通过设计一种混合式边缘计算到云计算的 FilterForward 系统，微分类器可以在真实世界计算机视觉任务上实现更高准确度的同时降低计算成本。作者相信 FilterForward 的混合式边缘计算到云计算设计能够为迅猛发展的智能摄像头部署提供重要的构建模块。

但是，FilterForward 的原型版本还存在一些局限性。首先，每个微分类器仅使用了单层的特征图。而某些分割任务需要来自多层的多个尺度的特征，这还没有得到解决。其次，对每个应用而言，找到选择哪一层作为每个分类器的输入对结果非常重要。而在这篇论文中，作者只是基于两个简单的启发而人工选择了一层，这方面还有改进空间。最后，和大多数边缘计算系统一样，FilterForward 可能存在中断时间更长以及数据可能丢失或损坏的风险。

### **论文2：FixyNN-Efficient Hardware for Mobile Computer Vision via Transfer Learning**
- 地址：[http://www.sysml.cc/doc/2019/69.pdf](https://link.zhihu.com/?target=http%3A//www.sysml.cc/doc/2019/69.pdf)

**引言**

目标检测/跟踪和语义分割等实时计算机视觉（CV）任务是人脸跟踪和人体分割等移动计算应用的关键使能技术。但是，实时计算机视觉应用日益增长的准确度需求与移动设备的功率限制之间存在一道能量效率的鸿沟。为了填平这道能效鸿沟，目前有两大主要研究趋势：MobileNet 等更高效的卷积神经网络（CNN）架构以及专用硬件加速器。

这篇论文设计的 FixyNN 是这两个趋势的融合，采用了硬件与 CNN 的联合设计方法来实现基于移动设备的 CV 应用的 CNN 推断。FixyNN（如图 1 所示）将 CNN 分成了两部分。第一部分是为多个不同的任务或数据集生成常见的低层面 CNN 特征。第二个部分提供特定于任务的 CNN 后端。然后，其前端层被实现成一种经过深度优化的固定权重的特征提取器（FFE）硬件加速器，后端部分则因为特定于具体任务，需要在典型的可编程 CNN 硬件加速器上实现。
![](https://pic4.zhimg.com/v2-c7760283d591514cef44ce52b88c0657_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='549' height='577'></svg>)
**固定权重特征提取器的硬件设计**

在这篇论文中，FixyNN 结合了两个专用硬件加速器：一个深度优化的固定权重特征提取器（FFE）和一个更传统的可编程 CNN 加速器。在硬件中固定权重方面有五项主要的优化：固定的移位加法缩放器、零开销权重剪枝、经过优化的中间精度、零 DRAM 带宽、最小激活存储。固定全并行的、全流水线式的 FFE 加速器中卷积（CONV）层的权重能为 FFE 实现很多激进的硬件优化，使其在吞吐量和能效方面优于传统的可编程加速器。FFE 的硬件设计可以描述为以下三个层级：

1．完全并行式固定权重* CNN *数据路径：*如图 2 所示，每个卷积层的计算都可实现为扁平的、全并行的、经过剪枝的固定权重算术逻辑层级。替代乘法器的固定缩放器是由综合工具（synthesis tool）生成的，而权重的嵌入方式则与 Verilog 硬件描述语言（HDL）完全一样。在卷积层之后的是批归一化、ReLU 和量化算子。
![](https://pic4.zhimg.com/v2-eb000d2c74bc865dcca0c44344a2114f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='407'></svg>)
2．全流水线式 CNN 缓冲：传统的可编程 CNN 加速器通常是通过将卷积转换成通用矩阵乘法（GEMM）来以串行方式计算 CNN；相对而言，FFE 是使用全流水线式的 CONV 层实现本地卷积。图 3 给出了一个核大小为 3×3 的简单卷积层的全流水线式排布概况。这个缓冲功能是使用一个 SRAM 行缓冲器（line buffer）实现的，其中逐行存储了每一层的激活，直到达到所需的张量大小。在 SRAM 行缓冲器之后是一个基于触发器（flip-flop）的移位寄存器，可使得卷积窗口在特征图上有效地移动，而无需重新读取数据。
![](https://pic2.zhimg.com/v2-c12e3688b9a4c1dec976fbfe424d63b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='260'></svg>)
3．DeepFreeze 工具流程：研究者开发了一个名为 DeepFreeze 的开源工具来帮助实现可能带有数百万个硬编码的权重的 FFE 加速器。给定一个标准机器学习软件框架（比如 TensorFlow）导出的模型，DeepFreeze 工具流程能够为一组特定的层生成 Verilog HDL 代码等固定的 CNN 硬件加速器设计，如图 4 所示。
![](https://pic2.zhimg.com/v2-d576fbf65e973c1a7f5a6576e6eaa445_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='551' height='329'></svg>)
**实验结果**

为了评估 FixyNN，研究者执行了硬件建模与迁移学习实验。硬件建模部分是比较 FixyNN 与当前最佳的硬件加速器设计的硬件性能。另一方面，为了展示固定特征提取器方法的泛化能力，研究者设计了两组包含一系列任务的实验，其分别使用了 MobileNetV1-0.25 和 MobileNetV1-1.0 来执行。MobileNetV1 首先在 ImageNet 数据集上进行了训练，然后在迁移到其它视觉任务。

**硬件**

 FixyNN 包含两个硬件组件：FFE 和可编程的 CNN 加速器。其中 FFE 使用 DeepFreeze 工具流程生成。可编程加速器基于在英伟达深度学习加速器（NVDLA）上发表的结果，这是一种当前最佳的开源的神经网络加速器，使用了 Verilog RTL 进行硬件实现，使用了 TLM SystemC 仿真模型进行软件开发、系统集成和测试。表 1 给出了硬件资源六种标称配置的已发表的 NVDLA 性能结果。
![](https://pic1.zhimg.com/v2-04c5ae27ac1364383dec9f59786e0b4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='462' height='191'></svg>)
首先，研究者比较了 FFE 和可编程 NVDLA 加速器在 MobileNet-0.25 的 13 层中每一层上的吞吐量（TOPS）和能效（TOPS/W），如图 5 所示。很明显，FFE 在所有方面都优于 NVDLA，在 TOPS 和 TOPS/W 上分别实现了 8.3 和 68.5 倍的提升。
![](https://pic3.zhimg.com/v2-123f5ffcf453ec0601e149944a29931a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='550' height='668'></svg>)
但是，FFE 所需的硅面积实际上会对层的数量带来限制。图 6 给出了在使用不同大小的 MobileNet 网络时，FFE 的面积随固定层的数量的变化情况。因此 FixyNN 的关键点是平衡 FFE 和可编程加速器之间的层分布，从而在给定硅面积约束的条件下最大化能效和泛化能力。
![](https://pic1.zhimg.com/v2-57aef3fda05353f93d6d6086ee315904_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='468' height='292'></svg>)
在演示了固定特征提取器在单个个体层上的优势之后，研究者又展示了实际的 FixyNN 系统。类似地，其基线是一个没有固定层的完全可编程 NVDLA 加速器，其代表了当前最佳。
![](https://pic2.zhimg.com/v2-240643c7f9da6cf0c3b416b570e7dbbd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='955' height='394'></svg>)
在吞吐量方面（图 7a），在预算面积小时，基线 NVDLA 的表现优于 FixyNN，因为此时 FFE 严重受制于可编程 NVDLA 的瓶颈。但是在预算面积更大时，FixyNN 可以固定更多层，在吞吐量上获得更大增益。而在能效方面（图 7b），由于数据复用和其它分摊的增加，基线 NVDLA 一开始能很好地随面积扩展，但随着在利用内存带宽方面的限制出现，增益会出现饱和（甚至回落）。由于 FFE 能效出色，当从 NVDLA 向 FFE 转移的负载增多时，能效也会随之提升。

表 2 展示了在不同的设计条件下，图 7 的设计空间中的帕累托最优 FixyNN 配置。
![](https://pic4.zhimg.com/v2-dc16a6c00e43af4b56e897c0f7ad962b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='275'></svg>)
此外，这篇论文还调查了在面积预算为 2-3 mm²（有 11 个固定层和 NVDLA 配置 C）时能效最优的配置。图 8 给出了 FFE 和可编程 NVDLA 之间的功率、性能和面积（PPA）细分结果。图中表明固定的数据路径占据了较大部分的面积和网络中较多的运算，但却仅占消耗能量和延迟的一小部分。
![](https://pic4.zhimg.com/v2-fa5af9ee33ee83880d884d0336054027_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='546' height='523'></svg>)
**模型准确度**

表 3 给出了在 MobileNet-0.25 上使用 FFE 的第一组迁移学习实验结果。MobileNet-0.25 首先在 ImageNet 上经过训练，然后再迁移到六个不同的视觉任务（CIFAR100、CIFAR10、SVHN、 Flwr、Airc 和 GTSR）。该网络中有更高的比例是固定的，也使用了更大的 FFE。第一行有 0 个固定层的结果是基线结果。
![](https://pic4.zhimg.com/v2-67d355ea8030d8fd7642dbe3de354eaf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='321'></svg>)
表 4 给出了 MobileNet-1.0 的迁移学习准确度，它也首先在 ImageNet 上进行了训练，然后再迁移到 CIFAR-100。可以观察到其迁移学习准确度损失有类似的趋势。整体而言，这里的准确度比上面的更高，因为 MobileNet-1.0 模型本身能力更强。固定网络的前 11 个卷积层并使用自适应批归一化（BN）只让准确度下降了 1.6%。
![](https://pic2.zhimg.com/v2-cea10c110405d76d16d1072b8548f1b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='549' height='310'></svg>)
**总结**

由于基于深度学习的模型计算量很大，传统的移动系统芯片（SoC）无法达到流畅运行深度学习模型所需要的性能。而目前市面上已有的硬件加速方案主要有（1）GPU：计算效率高，但功耗太大，价格昂贵；（2）FPGA：功耗低，但单颗性能低、导致总体性价比低，另外从业人员少，大规模开发难度高；（3）ASIC：效率能耗比，但不可更改任务，从而增大了普及难度。

这篇论文提出的 FixyNN 是一种更紧密整合计算机系统和机器学习的解决方案。通过将 DNN 分成两部分，一部分使用高度定制的硬件来提取 CNN 特征，另一部分是可编程的，使用迁移学习泛化到不同的计算机视觉任务，FixyNN 有能力实现能效的最优平衡。实验评估表明，FixyNN 硬件可以实现比 iso-area 可编程加速器更好的能效。

> [SysML 2019提前看：神经网络安全性](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759375%26idx%3D3%26sn%3D7db75011e9cb5750bd188e893be75ea3%26chksm%3D871aa4b1b06d2da7d8178491bcf2cec283440a68cfb3204bbdb890182d66fb4b9c59123a0450%26scene%3D21%23wechat_redirect)
本文作者为机器之心特邀技术分析师 Andy Xiong，他热爱人工智能和计算神经科学领域，乐观相信深度强化学习能帮助人类更接近实现通用人工智能。目前从事计算机视觉、图形学相关工作，欢迎交流。


