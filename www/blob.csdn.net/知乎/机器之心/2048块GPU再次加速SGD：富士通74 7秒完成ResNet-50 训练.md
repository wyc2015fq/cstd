# 2048块GPU再次加速SGD：富士通74.7秒完成ResNet-50 训练 - 知乎
# 



> 随着训练数据越来越大，我们需要一种高效的训练方法，而今年提出了很多大批量、大学习率的大规模分布式训练方法。例如 2018 年 8 月，fast.ai 学员在 [18 分钟内成功完成训练 ImageNet](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650746818%26idx%3D1%26sn%3Db01d31a60e11036aba0d870f2052a1d9%26chksm%3D871aebbcb06d62aa260797acb182f51d837fcaec77639fc148f3a8fee2b5d3732531d48e0409%26scene%3D21%23wechat_redirect)的任务，准确率达到 93%（128 块 V100 GPU）；腾讯在 ImageNet 数据集上以 64K 的批量大小用[4 分钟训练好 AlexNet](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650745993%26idx%3D2%26sn%3Da7cbba27e46a68588d7eb5084201717c%26chksm%3D871ae8f7b06d61e19debfbd288a1220a643888cbba7a7e66112b988577223da44bfaae76dd79%26scene%3D21%23wechat_redirect)、6.6 分钟训练好 ResNet-50（1024-2048 块 Tesla P40）；索尼随后以 34K 到 64K 的批量大小用[224 秒在 ImageNet 训练 ResNet-50](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650751990%26idx%3D1%26sn%3D32bab4510ad6e0eacb8988a85dbae346%26chksm%3D871a8788b06d0e9ee92785c0b230f49d4fcd118edce2992b3bd3db94d124240cf15604b58869%26scene%3D21%23wechat_redirect)（2176 块 V100 GPU）。

选自arXiv，作者：Masafumi Yamazaki 等，机器之心编译，参与：杜伟、Chita。

这一次，来自富士通的研究人员用上了 2048 块 GPU，以 81,920 的批量大小用 74.7 秒完成了 ResNet-50 训练。

在大规模数据集上训练的深度神经网络（DNN）在目标检测、语言翻译等多个领域取得了令人印象深刻的结果。但是，随着 DNN 模型和数据集规模不断增加，DNN 训练的计算成本也随之增长。众所周知，数据并行的分布式深度学习在集群上执行训练，是一种有效的加速方法。在这种方法中，集群上启动的所有进程具有相同的 DNN 模型和权重。每一个进程用不同的小批量（mini-batch）来训练模型，并结合来自所有进程的权重梯度以更新所有权重。这种通信开销成为大型集群的一大问题。为了减少大型集群的开销，来自富士通的研究人员增加 DNN 的批量大小，并以并行方式执行 DNN 训练。但是，large mini-batch 的训练通常会导致 DNN 模型的验证准确率下降。所以，研究人员使用了几项技术来增加批量大小（每次迭代中计算的输入图像数量），同时不影响验证准确率。

来自富士通的研究人员使用 AI Bridging Cloud Infrastructure（ABCI）集群（具有 2048 个 GPU）和自优化的 MXNet 深度学习框架来执行实验，结果 74.7 秒内在 ImageNet 上使 ResNet-50 的验证准确率达到 75.08%，使用的批量大小为 81920。

一般而言，大型集群上分布式深度学习的批量大小应该很大。Goyal 等人 [2] 提议使用预热（warm-up）技术来保持使用批量大小为 8192 时的验证准确率。谷歌 [3] 和索尼 [7] 采用可变且增加了的批量大小实现了高度并行化处理。

所以，每一层的权重梯度范数和权重范数之间的差异导致了训练的不稳定，[10] 使用层级自适应速率缩放（Layer-wise Adaptive Rate Scaling，LARS）规范了每一层的差异，使 DNN 可以在不影响验证准确率的情况下以 32768 的批量大小进行训练。

### **方法**

**准确率提升**

研究者采用了深度学习优化器中常用的[随机梯度下降](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759664%26idx%3D3%26sn%3D9f199894495c0706e86e2af67940e2a1%26chksm%3D871aa58eb06d2c985bf58ddd0787e27ff1e507c81f363b3f2c2a0fb3e3bd06e43944ffc95a23%26token%3D783613120%26lang%3Dzh_CN)法（SGD）。当使用 large mini-batch 训练时，SGD 更新数量会随着小批量的增加而减少。所以，提高 large mini-batch 上最终验证准确率是一个很大的挑战，研究者采取了以下方法：

1）控制[学习率](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759664%26idx%3D3%26sn%3D9f199894495c0706e86e2af67940e2a1%26chksm%3D871aa58eb06d2c985bf58ddd0787e27ff1e507c81f363b3f2c2a0fb3e3bd06e43944ffc95a23%26token%3D783613120%26lang%3Dzh_CN)：由于更新数量较小，研究人员需要采用较高的学习率来加速训练。但是，较高的学习率会导致模型在早期阶段训练不稳定。因此，研究人员采用能够逐步提升学习率的预热技术来稳定 SGD。此外，所有层都用相同的学习率对于某些层来说太高了，所以研究人员还使用层级自适应速率缩放（LARS）[10] 来稳定训练，该方法根据权重范数和梯度范数调整每一层的学习率。

对于权重收敛，研究人员尝试了步长、多项式以及线性等多种学习率衰减模式。此外，研究人员在多次试验的基础上采用了优化的权重衰减模式。

2）其他技术：先前的研究 [7] 使用标签平滑（label smoothing）[11] 在小批量大小为 32,768 的情况下实现了准确率提升。因此本论文作者也采用了这种方法，并证实该方法可在批量大小为 81,920 的情况下提高准确率。

批归一化层均值和方差的移动平均值（moving average）在每一进程中单独进行计算，其中权重也是同步的。这些值在 large mini-batch 训练中变得不准确，所以研究人员微调一些超参数以优化移动平均值。
![](https://pic1.zhimg.com/v2-17f33dfbcfcded0e4d88eb7a672c2704_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='298'></svg>)表 1：ImageNet 上 ResNet-50 的训练时间和 top-1 验证准确率
**框架优化**

研究人员使用以 C++ 和 CUDA C 语言写就的开源深度学习框架 [MXNet](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759664%26idx%3D3%26sn%3D9f199894495c0706e86e2af67940e2a1%26chksm%3D871aa58eb06d2c985bf58ddd0787e27ff1e507c81f363b3f2c2a0fb3e3bd06e43944ffc95a23%26token%3D783613120%26lang%3Dzh_CN)，该框架提供多种语言绑定。MXNet 具有灵活性和可扩展性，能够高效地在集群上训练模型。但是，在中小规模集群环境中仅占总时间一小部分的部分进程可能成为大型集群环境的瓶颈。研究人员使用多种分析器来分析 CPU 和 GPU 性能，并找出瓶颈问题。研究人员用以下方法优化了瓶颈，以提高训练吞吐量：

1）并行 DNN 模型初始化：在数据并行分布式深度学习中，所有层必须初始化，以使所有进程中的权重具有相同的值。一般而言，根进程（root process）初始化模型的所有权重。之后，根进程将这些权重广播到所有进程。广播时间随进程数量的增加而延长，当分布式深度学习具有数千个进程时，广播操作成本不可忽视。因此，研究人员采用其它初始化方法：每一进程拥有相同的种子，然后以并行化方式初始化权重。该方法可以在没有广播操作的情况下同步初始权重。

2）GPU 上的批量范数计算：每层的范数计算都是必要的，以使用 LARS 更新权重。与 GPU 上的内核数量相比，ResNet-50 网络的大部分层没有足够数量的权重。如果我们计算 GPU 上每层的权重范数，就会发现线程的数量不足以占据所有 CUDA 内核。因此，研究人员实现了一个特殊的 GPU 内核，以在 MXNet 中批量进行范数计算。该 GPU 内核可以启动足够数量的线程，并且可以并行计算所有层的范数。

**通信优化**

分布式并行深度学习需要 allreduce 通信，以在所有进程之间交换每一层的梯度。在大型集群环境中，allreduce 通信开销是不可忽略的，因为由于每个 GPU 的批量大小变小，通信时间变得更长，而计算时间变得更短。为了解决这些问题，研究人员采用了以下两种优化方法。

1）调节通信的数据大小：深度学习模型由很多层组成，而梯度的数据大小因层而异。每层的 allreduce 操作会由于频繁调用通信而导致大量开销，而如果梯度的数据大小变小了，情况会变得更糟，因为网络带宽无法被高效使用。因此，扩大 allreduce 的数据大小非常重要。研究人员收集了各层的梯度，然后将 allreduce 的数据大小调整为几兆字节。

2）做最佳的通信调度：研究人员开始对部分层执行 allreduce 操作，而没有等所有层完成。这导致 allreduce 操作与反向处理重叠。开始 allreduce 操作的时机应是梯度的数据大小大于阈值时。每个进程需要与其它进程保持同步，因为每个进程只能发送相同层的梯度。使用 allgather 操作通常可以找到完成的层，但是这会导致额外的开销。为了去掉这项开销，研究人员预先以统计的方式将所有层分为好几组。一旦每个进程完成一组中所有层的反向处理，就计划开始 allreduce 操作。

**环境和实验结果**

研究人员使用 ABCI 集群来评估其基于 MXNet 的优化框架的性能。ABCI 集群的每个节点由两个 Xeon Gold 6148 CPU、四个英伟达 Tesla V100 SXM2 GPU 组成。另外，一个节点上的 GPU 通过 NVLink 连接，节点还具有两个 InfiniBand 网卡。
![](https://pic2.zhimg.com/v2-7133e24bb9c3fee196992d696b8699f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='712' height='694'></svg>)
图 1：ABCI 集群中的计算节点图解。它由两个 CPU、四个 GPU 以及两个连接相应 CPU 的 HCA 组成。
![](https://pic2.zhimg.com/v2-c8def6197ab438b4c612cee4ee353ed5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='972' height='640'></svg>)
图 3：在 49,152 或更大规模的批量大小上训练的 top-1 验证准确率。

**论文：Yet Another Accelerated SGD: ResNet-50 Training on ImageNet in 74.7 seconds**
![](https://pic3.zhimg.com/v2-fdfd0596b07bb05664aa71787fd00bd6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='933' height='283'></svg>)



论文链接：[https://arxiv.org/abs/1903.12650](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1903.12650)

**摘要**：人们对于能够快速执行机器学习的算法有有着强烈的需求，而深度学习的速度仅在过去两年就加快了 30 倍。使用 large mini-batch 的分布式深度学习是解决这个需求的关键技术，也是一项巨大的挑战，因为在不影响准确率的情况下，很难在大型集群上达到很高的可扩展性。在本文中，研究人员引入优化方法来应对这个挑战。他们应用这些方法在 ABCI 集群上使用 2,048 个 GPU 在 74.7 秒时间内完成训练。训练吞吐量为 173 万张图像/秒，top-1 验证准确率为 75.08%。




