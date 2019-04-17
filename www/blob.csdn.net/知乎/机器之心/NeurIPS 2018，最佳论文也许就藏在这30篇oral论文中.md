# NeurIPS 2018，最佳论文也许就藏在这30篇oral论文中 - 知乎
# 



机器之心报道，机器之心编辑部。

未来一周，AI 顶会 NeurIPS 2018 将于当地时间 12 月 2 日-8 日在加拿大蒙特利尔举行。2016 年有 5000 人注册参加该会议，2017 年参会人数飙升至 8000，今年则出现了 11 分钟大会门票被抢光的盛况。近年来，很多科技巨头在 NeurIPS 会议期间举行一些 party 来招揽人才，如英特尔、亚马逊、IBM、英伟达、谷歌、苹果、特斯拉、Uber 等。

今年，NeurIPS 增加了 EXPO，当地时间 12 月 8 日众多来自国内的公司将进行展示。如下为一个 Room 的讨论主题，Expo 是给工业界组织的活动，它主要会关注各种前沿技术在产业中的应用与部署。
![](https://pic3.zhimg.com/v2-eca60540ca604338487afb8f09ed6782_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='522'></svg>)
9 月，NeurIPS 2018 接收论文结果公布。据悉，NeurIPS 2018 共收到 4856 篇投稿，创历史最高记录，最终录取了 1011 篇论文，其中 Spotlight 168 篇 (3.5%)，oral 论文 30 篇 (0.6%)。




**Oral 论文**

机器之心在梳理接收论文列表时，重点关注 30 篇 Oral 论文，很可能今年的最佳论文就从里面选出。在今年的 30 篇 Oral 论文中，可以看到总体上有几个比较突出的方面，首先对于神经网络、模块及损失函数的理论分析会比较多，此外对反向传播及最优化过程的研究也比较多，最后从策略到结构对强化学习的整体研究就更多了。这里主要介绍这三方面的研究，完整列表可看后文。




**理论分析**

近来模型或方法的理论分析越来越多，很多「技巧」的底层机制正在逐步揭开。在今年的 NIPS 论文中，很多研究者也希望了解我们曾忽视的原理，例如在 Oral 论文 On Neuronal Capacity 中，Pierre Baldi 等研究者定义学习机器的能力（capacity）可以是函数量的对数。他们分析了一些神经模型的能力，包括线性和多项式阈值门控单元、权重受限的阈值门控单元和 ReLU 神经元。更重要的是，他们分析了完整循环神经网络和前馈神经网络的大概能力。
![](https://pic1.zhimg.com/v2-199c0a0721187e8ddcbc8ea00abfeb5c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='446'></svg>)- 论文：Neuronal Capacity
- 论文地址：[http://papers.nips.cc/paper/7999-on-neuronal-capacity.pdf](https://link.zhihu.com/?target=http%3A//papers.nips.cc/paper/7999-on-neuronal-capacity.pdf)

这样的理论基础对于实践来说非常有借鉴作用，但是理解已有的「技巧」可能就是实在的实践指引了。在今年的 Oral 论文中，Shibani Santurkar 等研究者更关注批量归一化（BN）如何帮助模型的最优化过程。在他们的研究工作中，研究者表示尽管我们常常以为批量归一化主要在于修正输入的分布，并在训练过程中减少所谓的「internal covariate shift」。但实际上他们发现层级输入的分布稳定性与 BN 的成功没啥关系。

Santurkar 表示他们发现 BN 在训练过程中有更基础的影响，即 BN 会令损失函数表面更平滑一些，也令最优化过程更加容易一些。这种平滑使得梯度流更加稳定，并允许更快速的训练。
![](https://pic4.zhimg.com/v2-626a77cc409b286be26bc4b51e82aae3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1058' height='361'></svg>)- 论文：How Does Batch Normalization Help Optimization?
- 论文地址：[https://papers.nips.cc/paper/7515-how-does-batch-normalization-help-optimization.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7515-how-does-batch-normalization-help-optimization.pdf)
- 机器之心相关文章：[MIT新研究参透批归一化原理](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650751889%26idx%3D3%26sn%3Db3fce2d756c342d79218222590b94703%26chksm%3D871a87efb06d0ef994983e11f55f8205b7e5d2bfb08922b8438a0152cb6231fa1115bed77fe7%26scene%3D21%23wechat_redirect)

除了这两篇外，理论分析在整体 Oral 论文都非常突出，包括如何从理论上理解词嵌入及其维度、一种新型神经网络模型神经常微分方程、半正定规划中低秩方法的平滑分析等。其中词嵌入的理解提出了一种新的损失函数（Pairwise Inner Product/PIP），并借此揭示了词嵌入向量中维度选择的偏差-方差权衡。

**最优化与学习**

最优化与学习过程一直是研究重点，且自从 Adam 最优化算法提出以来也有很多相关研究。其实学习过程其实是一个很广的研究领域，它可以包括最优化方法、训练过程或方法、最大似然估计或其它构建损失函数的方法以及泛化问题与黑箱问题等话题。这一次的 Oral 论文比较受关注的是 Bengio 等人提出的一种反向传播近似方法，以及其它非凸优化问题。

Bengio 等研究者介绍了一种由简化的树突元素组成的多层神经网络模型，它同样以误差驱动的突触可塑性来调整神经网络而得到全局期望的输出。这一篇论文有非常浓厚的神经科学色彩，研究者表示这种模型在分类和回归任务中都有很好的效果，且分析表明它可以近似误差反向传播算法。
![](https://pic1.zhimg.com/v2-620c7995b69415e91a8212ad0f6c89e4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1058' height='556'></svg>)- 论文：Dendritic cortical microcircuits approximate the backpropagation algorithm
- 论文地址：[http://papers.nips.cc/paper/8089-dendritic-cortical-microcircuits-approximate-the-backpropagation-algorithm.pdf](https://link.zhihu.com/?target=http%3A//papers.nips.cc/paper/8089-dendritic-cortical-microcircuits-approximate-the-backpropagation-algorithm.pdf)

其它最优化算法就跟理论了，基本上都在尝试解决神经网络模型下的非凸优化问题，当然基本上还是采用一阶梯度，但也有分析二阶梯度的。例如在论文 Optimal Algorithms for Non-Smooth Distributed Optimization in Networks 中，研究者提出了一种一阶去中心化的算法，即 multi-step primal-dual /MSPD。

**强化学习**

强化学习在整个 Oral 论文中可能是接受量最多的，不论是通过重要采样的策略优化，还是「Non-delusional」Q 学习和价值迭代，强化学习的研究非常多。在这些研究中，比较有意思的是 Jurgen Schmidhuber 的「世界模型」，它可以让人工智能在「梦境」中对外部环境的未来状态进行预测，大幅提高完成任务的效率。

在 Jurgen 等研究者的论文中，他们表示「世界模型」抽取的特征可以馈送到紧致且简单的策略中，这些策略是由进化策略训练得出。这样的模型能在多种环境中实现最佳的结果，并且智能体能自己生成内部的「世界模型」。
![](https://pic2.zhimg.com/v2-8c20710a45d1e903225813d036543585_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='880' height='313'></svg>)- 论文：Recurrent World Models Facilitate Policy Evolution
- 论文地址：[https://papers.nips.cc/paper/7512-recurrent-world-models-facilitate-policy-evolution.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7512-recurrent-world-models-facilitate-policy-evolution.pdf)

最后，30 篇 Oral 论文的完整列表如下所示，读者可根据标题搜索具体的论文：

列表地址：[https://nips.cc/Conferences/2018/Schedule?type=Oral](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3Ftype%3DOral)
![](https://pic3.zhimg.com/v2-6f8ed61a8a215bd5321be4f69bd3099a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='806'></svg>)![](https://pic3.zhimg.com/v2-d6141c15351ba4049a6a1aaef6323356_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='833'></svg>)![](https://pic2.zhimg.com/v2-240e5edef542aeac162557d5f5d20089_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='815'></svg>)![](https://pic2.zhimg.com/v2-8b2fbba41fda5e68eb9cd4f6cef1d15d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='607'></svg>)
至于论文地址，读者可以在论文集页面中按照论文标题搜索。

NeurIPS 2018论文集：[https://papers.nips.cc/book/advances-in-neural-information-processing-systems-31-2018](https://link.zhihu.com/?target=https%3A//papers.nips.cc/book/advances-in-neural-information-processing-systems-31-2018)


