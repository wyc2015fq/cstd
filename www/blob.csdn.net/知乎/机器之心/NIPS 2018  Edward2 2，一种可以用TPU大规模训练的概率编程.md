# NIPS 2018 | Edward2.2，一种可以用TPU大规模训练的概率编程 - 知乎
# 



选自 arXiv，作者：Dustin Tran等，机器之心编译。

> 本文描述了一种用于在深度学习生态系统中嵌入概率编程的简单、底层方法。具体来说，作者将概率编程归结为一个抽象：随机变量。

深度学习的很多研究结果都模糊了模型和计算之间的界限，有的甚至表明是一种「可微分编程」的新范式，它们的目标不仅仅是训练模型，同时还希望实现一般的程序综合体。在这一观点下，注意力机制和门控机制可以描述布尔逻辑运算符，残差连接和条件计算可以描述控制流，外部记忆可以访问函数内部作用范围外的元素。此外，学习算法也将变得越来越动态，例如学习如何学习、神经架构搜索和层级内的最优化等。

可微分编程范式鼓励建模者明确考虑计算成本，我们不仅需要考虑模型的统计属性，例如模型到底能不能拟合真实数据分布，拟合得又有多好。同时我们还需要考虑计算量、内存占用和传输带宽的成本等，这里主要关注训练和预测的效率究竟怎样。

相比之下，概率编程社区主要倾向于在模型和计算之间划清界限：首先我们可以指定一个概率模型作为程序，其次我们可以执行「inference query」以在给定数据的情况下自动训练模型。这种设计使得概率编程很难在大规模真实环境下实现概率模型，其中大规模可能需要训练有数十亿参数量的模型，并要求通过多个加速器和调度通信切分模型计算量。

Edward 等最近的研究能更好地控制深度学习中的推断过程，然而它们都将推断过程作为一个封闭系统，因此很难构成任意的计算或更广泛的机器学习生态系统。

在这篇论文中，作者描述了一种在深度学习生态系统中嵌入概率编程的简单方法，且他们在 TensorFlow 和 Python 实现了新的库 Edward2.2。这种轻量方法为灵活建模提供了底层模块，深度学习研究者可以使用概率基元灵活地设计原型。本论文主要的贡献有三点：
- 作者将概率编程的核心提炼为单个抽象：随机变量。
- 这种底层设计有两个重要意义：首先它允许研究具有足够的灵活性；其次它允许使用张量计算单元（TPU）等加速器实现更大的模型。
- 作者后面还举例了三种应用：通过 TPU 训练的模型并行化变分自编码器；由 TPU 训练的数据并行化自回归模型；和多 GPU No-U-Turn Sampler (NUTS)。本文并没有介绍这三种应用，感兴趣的读者可查看原论文。

**论文：Simple, Distributed, and Accelerated Probabilistic Programming**
![](https://pic3.zhimg.com/v2-398bc8e58d59dcba94d5270873f8ca7e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1064' height='416'></svg>)
论文地址：[https://arxiv.org/abs/1811.02091](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1811.02091)

**摘要：**我们描述了一种用于在深度学习生态系统中嵌入概率编程的简单、底层方法。具体来说，我们将概率编程归结为一个抽象：随机变量。我们在 TensorFlow 中的轻量级实现支持许多应用：通过 TPU（TPUv2）训练的模型并行变分自编码器（VAE）；通过 TPUv2 训练的数据并行自回归模型（Image Transformer）；多 GPU 且无 U-Turn 的取样器（NUTS）。对于 64x 64 ImageNet 上的最新 VAE 和 256 x256 CelebA-HQ 上的 Image Transformer，我们的方法实现了从 1 到 256 块 TPUv2 芯片的线性加速。通过 NUTS，我们发现在 GPU 我们的方法比 Stan 快 100 倍，比 PyMC3 快 37 倍。

**随机变量**

我们在 Edward2 中概述了概率编程。它们只需要一个抽象：随机变量。然后，我们描述了如何利用追踪来执行灵活的底层操作。Edward 2 将任何可计算的概率分布具化为 Python 函数（程序），通常该函数执行生成过程并返回示例。
![](https://pic3.zhimg.com/v2-41cc3c72dadf551854d29f407bd618ba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='975' height='374'></svg>)
图 1：Beta-Bernoulli 程序。在即时运行模式中，model() 生成一个包含 50 个因素的二进制向量，model() 返回一个 op，该 op 在 TensorFlow 会话中进行评估。
![](https://pic1.zhimg.com/v2-17e38c3b61f0cf769f4ca78d3996c0c0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='407'></svg>)
图 2：变分程序 [35]，可在即时运行模式下获得。Python 控制流可用于生成流程：给定硬币翻转，该程序可根据两个神经网络中的一个 生成流程。其输出具有不同的形状和结构。
![](https://pic2.zhimg.com/v2-92dfa6fb4d014ff7be52357d7ae6389d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='605'></svg>)
图 3：分布式自回归流。（右）默认长度是 8，每个有 4 个独立的流。每个流通过关于自回归排序的层来转换输入。（左）流跨越 4x4 内核（矩形）的虚拟拓扑进行划分；每个内核计算两个流，并在本地连接；最后会汇集一个内核。虚拟拓扑与物理 TPU 拓扑对齐：对于 4x4 TPU，它很精确；对于 16x16 TPU，它要为了数据并行性而复制。
![](https://pic4.zhimg.com/v2-1c772ef832a5598fb7432ea803d478d7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='452'></svg>)
图 4：通过 TPU 训练模型并行的 VAE，从 8 位延迟生成 16 位 latents。先验和解码器根据分布式自回归流来分割计算。编码器可能会根据压缩器分割计算；为了节省空间我们选择忽略它。
![](https://pic4.zhimg.com/v2-fddc86e368577759f4886d6175100fdb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1077' height='576'></svg>)
图 6：一个程序执行。它是有向无环图，并追踪累计对数概率等各种 op 或寻找条件独立性。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*





