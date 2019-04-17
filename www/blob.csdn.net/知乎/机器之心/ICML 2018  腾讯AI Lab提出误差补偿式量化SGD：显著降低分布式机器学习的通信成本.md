# ICML 2018 | 腾讯AI Lab提出误差补偿式量化SGD：显著降低分布式机器学习的通信成本 - 知乎
# 



选自arXiv，作者：Jiaxiang Wu、Weidong Huang、Junzhou Huang、Tong Zhang，机器之心编译。

> 在量子在分布式机器学习应用中，各个节点之间的通信效率往往会成为整体性能的关键制约因素，目前的常见解决方法是对节点之间的通信信息进行压缩，但这会引入量化误差。为了解决这一问题，腾讯 AI Lab 提出了一种误差补偿式量化随机梯度下降（ECQ-SGD）方法。该论文已被将于当地时间 7 月 10-15 日在瑞典斯德哥尔摩举办的 ICML 2018 接收，此次实验室共有 16 篇论文被收录。

在 ICML 2018 与 IJCAI 2018 大会期间（今年都在斯德哥尔摩），腾讯将举办 Tencent Academic and Industrial Conference (TAIC)，诚邀全球顶尖 AI 学者、青年研究员与腾讯七大事业群专家团队探讨最前沿 AI 研究与应用。

由于数据近年来的爆炸式增长，很多不同领域都已经越来越关注大规模机器学习了，比如计算机视觉和语音识别领域。在这些应用中，训练数据的量往往过于庞大，以至于难以通过单个计算节点实现有效的处理，所以分布式优化方法是这些应用中的一个核心构建模块。

数据并行化是一种常用的分布式学习框架，其中整个数据集会被分割和存储在一个集群中的多个节点上。每个节点都会计算各自的局部梯度并与其它节点交流梯度以更新模型参数。对于这样的学习系统，消耗的时间可大致归类为计算时间和通信时间。其中通信往往会成为性能的瓶颈，尤其是对于具有大量参数的大型集群和/或模型。

目前已有一些试图通过降低通信成本来提升分布式学习的效率的研究工作。某些方法关注的是将梯度量化为定点数（Zhou et al., 2016; Alistarh et al., 2017），这样需要传输的比特数就会少很多。还有一些研究探索过更为激进的量化方法，比如二元或三元表征（Seide et al., 2014; Strom, 2015; Wen et al., 2017）。还有方法是在通信过程中在梯度上施加稀疏性，这样每轮迭代中就仅有一小部分梯度在节点之间交换（Wangni et al., 2017; Lin et al., 2018）。

这些方法的基本思想基本都是将梯度压缩成某种特定的形式，让其中每一项都能使用远少于原来的 32 位浮点数的比特数表示。这样的压缩会在优化过程中引入额外的随机噪声，即量化误差，这会减缓收敛速度，甚至导致发散。1Bit-SGD（Seide et al., 2014）采用了误差反馈方案，即使用上一轮迭代的量化误差来补偿当前的局部梯度，之后再将其输入量化函数。尽管该方法的提出者表示这有助于改善收敛行为，但却没有给出理论分析来证明其有效性。

在本论文中，我们提出了误差补偿式量化随机梯度下降（ECQ-SGD）方法。我们的算法也使用了误差反馈方案，但我们会累积所有之前的量化误差，而不是像 1Bit-SGD 一样只使用上一轮迭代的量化误差。尽管实验评估表明这种修改能实现比很多基准方法更快更稳定的收敛，但为这种现象提供理论保证却并不简单。

Alistarh et al., 2017 已经证明，对于他们提出的 QSGD 算法，达到特定次优间隙（sub-optimality gap）所需的迭代数量正比于随机量化梯度的方差界限。但是，这不能解释我们的方法的收敛行为，因为我们的量化梯度是对原始梯度的有偏估计，这与 QSGD 的情况不同。实际上，由于使用的是累积的量化误差，我们的量化梯度的方差界限甚至比 QSGD 的还大。为了解决这一问题，我们从另一个角度给出了收敛性分析，并且证明了在合适的超参选择下，我们的算法比 QSGD 具有更紧致的最坏情况误差界限（worst-case error bound）。事实证明，我们提出的误差反馈方案可以很好地抑制量化误差对误差界限的贡献；正如我们在实验中观察到的那样，这能实现比 QSGD 更小的次优间隙。
![](https://pic3.zhimg.com/v2-e4dc98a85f832eb04622f775270d9456_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='414' height='467'></svg>)算法 1：误差补偿式量化 SGD
在量化完成之后，总体通信成本会降至 32+dr 比特（r ≪ 32），远少于原来的 32 位全精度梯度所需的 32d 比特；其中 d 是原向量的维度；
![](https://pic2.zhimg.com/v2-75ac69a8b86b97cf8f1e5eec70eef739_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='210' height='36'></svg>)
，其中 s 是非零量化级别的数量：s 越大，则量化越细粒度，通信成本也就越高。
![](https://pic4.zhimg.com/v2-c86e9493eb3d5aa2fd23cdf8d791e55b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='866' height='387'></svg>)图 2：损失函数值与当前迭代解到最优解距离的比较（左图：Syn-256；中图：Syn-512；右图：Syn-1024）![](https://pic4.zhimg.com/v2-a9bb3b9be2be60075e5b74fb2895b16f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='618' height='482'></svg>)图 3：在 Syn-20K 数据集上各阶段所消耗的时间以及测试损失（括号中）的比较。QSGD 和 ECQ-SGD 的数字后缀表示非零量化级别的数量 s。这里的总时间是 1000 次迭代所消耗的时间![](https://pic1.zhimg.com/v2-1837d9aab63438ee9a50eca447096d7c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='558' height='288'></svg>)图 6：在 ILSVRC-12 数据集上训练 ResNet-50 模型时，使用不同数量的 GPU 的吞吐量比较。
**论文：误差补偿式量化 SGD 及其在大规模分布式优化中的应用（Error Compensated Quantized SGD and its Applications to Large-scale Distributed Optimization）**
![](https://pic1.zhimg.com/v2-0234c1a79829f02f1b1b0da86cad87e8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='866' height='188'></svg>)
论文地址：[https://arxiv.org/abs/1806.08054](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1806.08054)

**摘要：**大规模分布式优化对很多不同应用而言都很重要。对于基于数据并行的分布式学习，节点之间的梯度通信往往会成为性能的瓶颈。我们在本论文中提出了可提升训练效率的误差补偿式量化随机梯度下降算法。该方法通过量化局部梯度来降低通信开销，并且使用累积的量化误差来加快收敛速度。此外，我们还提供了对其收敛行为的理论分析，并展示了其相对于其它竞争方法的优势。我们进行了大量实验，结果表明我们的算法在不降低表现水平的情况下能将梯度压缩高达两个数量级。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)





