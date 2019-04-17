# 英伟达官方解读：Volta Tensor Core GPU实现AI性能新里程碑 - 知乎
# 



选自NVIDIA，作者：Loyd Case，机器之心编译。

> 大规模深度学习应用的开发对性能有很高的需求，作为深度学习加速器 GPU 硬件的核心供应商，英伟达一直在不断创新以满足 AI 社区对计算性能的需求。近日，英伟达开发者博客发文介绍了 Volta Tensor Core GPU 在 AI 性能提升上的里程碑进展。机器之心对该博客文章进行了编译介绍。更多有关 Volta Tensor Core GPU 的情况可参阅机器之心文章[《英伟达 Volta 架构深度解读：专为深度学习而生的 Tensor Core 到底是什么？》](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650726509%26idx%3D1%26sn%3Db51141dd53163bc6c17754ca20d06ffe%26chksm%3D871b2413b06cad051d864ce2f07e2d00d983108d9a5b86b874b409a28ac23cb1ab603ee85ba7%26scene%3D21%23wechat_redirect)

具备深度学习力量的人工智能现在已能解决一些曾被认为不可能解决的难题，比如自然语音和自动驾驶领域内的计算机理解和交谈。深度学习现在已能有效解决大量难题，在这种进展的推动下，算法复杂度的指数级增长已经带来了对更高速的计算的极大渴求。为了满足这些需求，英伟达设计了 Volta Tensor Core 架构。

英伟达和很多其它公司与研究者一直都在开发计算硬件和软件平台来解决这一需求。比如，谷歌打造了 TPU（张量处理单元）加速器，能够给可以运行在 TPU 上的数量有限的神经网络带来优良的表现。

在这篇博文中，我们将分享英伟达最近的一些进展，这些进展能为 AI 社区带来极大的 GPU 性能提升。采用这些改进，我们已经在单块芯片和单个服务器上实现了创纪录的 ResNet-50 性能表现。

最近，fast.ai 也宣布了他们在单个云实例上的创纪录性能表现，请参阅：[http://www.fast.ai/2018/04/30/dawnbench-fastai/](https://link.zhihu.com/?target=http%3A//www.fast.ai/2018/04/30/dawnbench-fastai/)




我们的结果表明：
- 在训练 ResNet-50 时，单个 V100 Tensor Core GPU 能实现每秒 1075 张图像的处理速度，相比于前一代 Pascal GPU，性能提升了 4 倍。
- 具有 8 个 Tensor Core V100 的单个 DGX-1 服务器能实现每秒 7850 张图像的处理速度，几乎是一年前同样系统 4200 张图像/秒的处理速度的 2 倍。
- 由 8 个 Tensor Core V100 驱动的单个 AWS P3 可以用不到 3 小时时间训练完 ResNet-50，比 TPU 实例快 3 倍。
![](https://pic4.zhimg.com/v2-0cfe38721669e92896db1ab8e178372b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='447'></svg>)图 1：Volta Tensor Core GPU 创造了 ResNet-50 的新速度记录（AWS P3.16xlarge 实例包含 8 个 Tesla V100 GPU）
英伟达 GPU 在多种不同算法上的大规模并行处理性能优异，自然非常适合深度学习。但不止于此。凭借我们多年的经验以及与全世界 AI 研究者的紧密合作，我们创造了一种针对多种深度学习模型优化过的新架构——英伟达 Tensor Core GPU。

通过将高速 NVLink 互连与深度优化组合到所有当前的框架中，我们实现了当前最佳的表现。英伟达 CUDA GPU 的编程能力能确保用于大量不同现代网络的性能表现，同时还能提供一个催生新型框架和未来深度网络创新发明的平台。




**V100 Tensor Core 创造单块处理器最快速度记录**

Volta GPU 中的英伟达 Tensor Core GPU 架构是英伟达深度学习平台巨大进步的代表。这种新型硬件能加速矩阵乘法和卷积计算，这些计算占到了训练神经网络计算操作的大部分。

英伟达 Tensor Core GPU 架构让我们既可以提供比单个功能的 ASIC 更优的性能，同时又是可编程的，可用于各种不同的工作负载。比如说，每个 Tesla V100 Tensor Core GPU 都能提供 125 TFLOPS（每秒万亿次浮点运算）的深度学习性能，相对而言，谷歌 TPU 芯片的速度是 45 TFLOPS。一个 Cloud TPU 中的 4 块 TPU 芯片能提供 180 TFLOPS 的性能；相比而言，4 块 V100 芯片能提供 500 TFLOPS 的性能。

我们的 CUDA 平台让每一种深度学习框架都能充分利用我们的 Tensor Core GPU 的全部力量来加速正在快速增多的各种神经网络类型，比如 CNN、RNN、GAN、RL 以及每年涌现的数以千计的变体。

让我们再向 Tensor Core 架构继续深入一点，以凸显其特有的能力。图 2 展示了 Tensor Core 正在操作以低精度 FP16 存储但以更高精度 FP32 计算的张量，这能在最大化吞吐量的同时仍然维持必要的精度。
![](https://pic4.zhimg.com/v2-4ea91a856ab91fb18dca09170f9bacc3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='600'></svg>)图 2：Volta Tensor Core 矩阵乘法和累加
使用最近的软件改进，ResNet-50 训练现在可以在独立测试（standalone testing）中在单个 V100 上达到 1360 张图像/秒的惊人速度。我们现在正在努力将这个训练软件整合进流行的框架中，如下所述。

为了实现最佳的表现，Tensor Core 所运行的张量应该处于内存中一个通道交织的数据布局中（数量-高度-宽度-通道，通常简称为 NHWC）。训练框架在内存中所预期的布局是以通道为主要的数据布局（数量-通道-宽度-高度，通常简称为 NCHW。所以要使用 cuDNN 库来执行 NCHW 和 NHWC 之间的张量转置操作，如图 3 所示。正如之前提到的，因为现在卷积本身已经非常快了，所以这些转置会占到相当可观的一部分运行时间。

为了消除这些转置，我们采用的方法是直接用 NHWC 格式表示 ResNet-50 模型图中的每个张量，这是 MXNet 框架支持的功能。此外，我们还向 MXNet 和 cuDNN 添加了用于所有其它非卷积层的优化过的 NHWC 实现，从而在训练阶段无需任何张量转置。
![](https://pic3.zhimg.com/v2-076f1bb5aa72acb7155b0481e01b6ac2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='864' height='500'></svg>)图 3：优化过的 NHWC 格式能消除张量转置
阿姆达尔定律（Amdahl's Law）带来了另一个优化机会，该定律预测了并行处理的理论加速能力。因为 Tensor Core 能显著提升矩阵乘法和卷积层的速度，训练工作负载中的其它层就会占到运行时间的更大一部分。因此，我们确定了这些新的性能瓶颈并且对它们进行了优化。

很多非卷积层的性能都受限于在 DRAM 中移入或移出数据，如图 4 所示。将连续层融合到一起要用到片上内存和避免与 DRAM 的数据流动。比如，我们在 MXNet 中创造了一种图优化传递（graph optimization pass）来检测连续的 ADD 和 ReLU 层，并在任何可以替代的时候用融合后的实现来替代它们。使用 NNVM（神经网络虚拟机），在 MXNet 中实现这些类型的优化是很简单的。
![](https://pic2.zhimg.com/v2-7410b39832dc0b0a9a04f8bd34756015_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='897' height='352'></svg>)图 4：融合层能消除数据读/写
最后，我们通过为常出现的卷积类型创建额外的专用核来继续优化单个卷积。

我们目前正将这些优化方法中的许多方法纳入到多个深度学习框架中，其中包括 TensorFlow、PyTorch 和 MXNet。通过我们为 MXNet 贡献的方法，使用标准的 90 epoch 训练方案，我们在单个 Tensor Core V100 上实现了 1075 张图像/秒的性能，同时还实现了与单精度训练一样的 Top-1 分类准确度（超过 75%）。这为我们留下了进一步提升的巨大空间，因为我们可以在独立测试中实现 1360 张图像/秒的速度。英伟达 GPU Cloud（NGC）上的英伟达优化的深度学习框架容器将会提供这些性能提升。




**最快的单节点速度记录**

多个 GPU 可以作为单个节点运行，从而实现显著更高的吞吐量。但是，扩展成在单个服务器节点中合作的多个 GPU 需要在 GPU 之间有高带宽/低延迟的通信路径。我们的 NVLink 高速互连结构让我们可以将性能扩展成单个服务器中的 8 个 GPU。这些得到了大规模加速的服务器能提供 1 PFLOPS（每秒千万亿次浮点计算）的深度学习性能，并且可广泛用于云和内部部署。

但是，扩展到 8 个 GPU 会显著增加训练性能，甚至足以让该框架中主 CPU 执行的其它工作成为性能的限制因素。具体而言，该框架中向 GPU 馈送数据的流程需要很大的性能提升。

这个数据流程包括从磁盘读取编码的 JPEG 样本、解码样本、调整尺寸和增强图像（见图 5）。这些增强操作能提升神经网络的学习能力，让训练后的模型有更高准确度的预测表现。使用 8 个 GPU 处理该框架的训练部分时，这些重要的操作就会限制整体的性能表现。
![](https://pic4.zhimg.com/v2-3512ffca262d73b9ee14e374ea4da2c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='722' height='136'></svg>)图 5：用于图像解码和增强的数据流程
为了解决这一问题，我们开发了 DALI（数据增强库），这是一个不受限于具体框架的库，可以将 CPU 的工作负载迁移到 GPU 上执行。如图 6 所示，DALI 将部分 JPEG 解码工作以及尺寸调整和所有其它增强工作移到了 GPU 上。这些操作在 GPU 上的执行速度比在 CPU 上快得多，因此减轻了 CPU 的工作负载。DALI 凸显了 CUDA 的通用并行处理能力。去除了 CPU 的瓶颈限制，我们可以在单个节点上维持 7850 张图像/秒的处理速度。
![](https://pic4.zhimg.com/v2-58de02e34b73f410f014c7241afdf927_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='772' height='499'></svg>)图 6：使用 DALI 进行了 GPU 优化的工作负载
英伟达正在助力将 DALI 整合进所有主要的 AI 框架中。这个解决方案也让我们可以将性能扩展到不止 8 个 GPU，比如最近宣布的英伟达 DGX-2 系统，带有 16 个 Tesla V100 GPU。




**最快的单个云实例速度记录**

对于我们的单个 GPU 和单节点运行，我们使用了事实上标准的 90 epoch 来训练 ResNet-50 达到超过 75% 的准确度。但是，训练时间可以通过算法创新和超参数调节来进一步减少，以便更少的 epoch 也能达到同样的准确度。GPU 为 AI 研究者提供了编程能力，并且支持所有深度学习框架，从而让他们可以探索新的算法方法和利用已有的算法。

fast.ai 团队最近分享了他们的出色结果，使用 PyTorch 在远低于 90 epoch 内实现了很高的准确度。Jeremy Howard 和 fast.ai 的研究者整合了关键的算法创新和调节技术来在 ImageNet 上训练 ResNet-50，在单个 AWS P3 实例上仅使用了 3 个小时——而该实例包含 8 个 V100 Tensor Core GPU。相比于基于 TPU 的云实例（训练 ResNet-50 需要接近 9 小时时间），ResNet-50 在 GPU 云实例上的运行速度可以达到其 3 倍之多。

我们还进一步期望本博客中所描述的提升吞吐量的方法也将适用于其它方法（比如 fast.ai 的方法），并能帮助它们实现更快的融合。




**带来指数级的性能提升**

自 Alex Krizhevsky 使用 2 个 GTX 580 GPU 赢得了第一届 ImageNet 比赛以来，我们在加速深度学习方面已经取得了非凡的进展。Krizhevsky 花了 6 天时间才训练完他那出色的神经网络 AlexNet，其表现超越了当时其它所有图像识别方法，开启了深度学习革命。现在，使用我们最近宣布的 DGX-2，我们可以在短短 18 分钟内训练完 AlexNet。图 7 展示了在过去短短 5 年时间里实现的 500 倍性能提升。
![](https://pic2.zhimg.com/v2-1ab4d9a450b993894654c7754b5ce871_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='992' height='800'></svg>)图 7：在 ImageNet 数据集上训练 AlexNet 的时间
Facebook 人工智能研究所（FAIR）已经共享了他们的语言翻译模型 Fairseq：[https://github.com/facebookresearch/fairseq](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/fairseq)。使用我们最近发布的 DGX-2 和我们为数众多的软件堆栈提升，我们在不到 1 年时间里在 Fairseq 上实现了 10 倍的性能提升（见图 8）。
![](https://pic3.zhimg.com/v2-c3c3a950b159c4e8022a87c244fe1332_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='662' height='534'></svg>)图 8：训练 Facebook 的 Fairseq 的时间
研究者正在使用 AI 的力量来解决数不胜数的用例，图像识别和语言翻译只是其中的两个代表。在 GitHub 上，使用 GPU 加速的框架的神经网络项目数量已经超过了 60000。我们的 GPU 的编程能力能为 AI 社区正在构建的各种各样的神经网络提供加速。这样的快速提升让 AI 研究者可以去构想更加复杂的神经网络，以使用 AI 攻克富有挑战性的难题。

这些持续的提升改进源自我们为 GPU 加速的计算开发的全栈式的优化方法。从构建最先进的深度学习加速器到复杂系统（HBM、COWOS、SXM、NVSwitch、DGX），从高级数值库和深度软件堆栈（cuDNN、NCCL、NGC）到加速所有深度学习框架，英伟达在 AI 方面的努力能为 AI 开发者提供无与伦比的灵活性。

我们将继续优化整个堆栈并继续提供指数级的性能提升，以为 AI 社区提供推动深度学习创新向前发展的工具。




**总结**

AI 在继续变革每个行业，催生数不胜数的用例。理想的 AI 计算平台要能提供出色的性能，能够扩展支持巨大且越来越大的模型规模，并且还要具备编程能力以应对越来越多样化的模型架构。

英伟达的 Volta Tensor Core GPU 是目前世界上最快的 AI 处理器，单块芯片就能提供高达 125 TFLOPS 的深度学习性能。我们很快就将在单个服务器节点中集成 16 个 Tesla V100，以打造世界上最快的计算服务器，能提供高达 2 PFLOPS 的性能。

除了性能，每家服务器制造商的每个云为整个 AI 社区所提供的 GPU 的编程能力和广泛的可用性都在推动实现下一代人工智能。

不管哪种深度学习框架，英伟达的硬件都能对其提供加速：Caffe2、Chainer、Cognitive Toolkit、Kaldi、Keras、Matlab、MXNET、PaddlePaddle、Pytorch 和 TensorFlow。此外，英伟达 GPU 能用于各种各样且越来越多的网络类型，其中包括 CNN、RNN、GAN、RL、混合网络架构以及每年新出现的数以千计的变体架构。AI 社区已经创造了很多精彩出色的应用，我们希望能为 AI 的未来发展提供力量。 
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://devblogs.nvidia.com/tensor-core-ai-performance-milestones/](https://link.zhihu.com/?target=https%3A//devblogs.nvidia.com/tensor-core-ai-performance-milestones/)


