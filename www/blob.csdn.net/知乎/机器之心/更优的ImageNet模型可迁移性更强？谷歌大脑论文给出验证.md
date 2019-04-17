# 更优的ImageNet模型可迁移性更强？谷歌大脑论文给出验证 - 知乎
# 



选自arXiv，作者：Simon Kornblith、Jonathon Shlens、Quoc V. Le，机器之心编译。

> 现代计算机视觉研究的一个隐含的假设是，在 ImageNet 上性能更好的模型必定在其它的视觉任务上表现得更好。然而，这个假说从来没有被系统地验证过。谷歌大脑研究者在 3 组实验环境下对比了 13 个分类模型在 12 个图像分类任务上的性能。实验结果表明，ImageNet 架构在其它数据集上的泛化能力很强，在 ImageNet 数据集上的精确率的小小的提升就能够导致在其它数据集上性能的提升，但是 ImageNet 提取出的固定特征的泛化能力小于其架构的泛化能力。

过去十年来，计算机视觉研究一直将追赶学术上用于对比的基准模型的性能作为衡量进步的标准。然而没有任何一个基准能像 ImageNet [1] 一样被如此激烈地追赶。针对这一数据集构建的网络架构，在许多方面显著促进了计算机视觉研究的进步，包括将模型迁移到新的数据集 [2，3]、目标检测 [4]、图像分割 [5，6] 和图像质量评价 [7]。这一进展背后一个隐含的假设是，在 ImageNet 上性能更好的网络架构必定在其它视觉任务上表现得更好。另一个假设是，更好的网络架构能够学到更好的能在基于视觉的任务之间迁移的特征。尽管之前的研究已经为这些假说提供了一些证据（例如，[4，5，8-10]），但是这些假说还没有被系统地探究过。

在本文中，研究者试图通过研究 ImageNet 特征和 ImageNet 分类架构的可迁移性来检验这些假说。具体而言，研究者在 12 个图像分类数据集上对 13 个性能最好的图像分类卷积神经网络间的迁移学习进行了大规模的研究，实验环境如图 1 所示，包含以下三种类型：固定的特征提取器 [2，3]，从 ImageNet 的初始化 [8，11，12] 环境中进行调优，从随机初始化开始训练。本文的主要贡献如下：
- 最佳的 ImageNet 模型没有给出最佳的图像特征。在 ImageNet 上训练的 ResNet 模型 [14，15] 的特征始终优于那些在 ImageNet 上取得更高准确率的网络。
- 当网络经过调优后，ImageNet 的准确率是迁移任务准确率的更强有力的指标（r^2 = 0.86），目前最先进的 ImageNet 架构在许多任务中取得了最佳性能。
- 即使权重信息并不能在任务间很好地迁移，但是网络架构可以很好地被迁移。在 3 个小型细粒度分类数据集上，调优并不能为从随机初始化状态开始的训练提供显著的帮助，但是尽管如此，更好的 ImageNet 架构能取得更高的准确率。
![](https://pic2.zhimg.com/v2-843cb2d0a598b272b9810edd22686479_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='451' height='321'></svg>)
图 1: 细粒度数据集（FGVC 飞机）上的性能可以从调优或从头开始的训练中获得很大的提升，和 ImageNet 类似的数据集（CIFAR-10）上的提升则小一些。低维嵌入在从「inception v4」网络的倒数第二层中的特征上使用 t-SNE[13]，用于测试集中的 10 个类别。
![](https://pic4.zhimg.com/v2-b52f8bbf27f433f0c2d8fd276d166573_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='650' height='303'></svg>)
表 1: 在迁移学习中被检验的数据集。
![](https://pic3.zhimg.com/v2-d4d088e290de46d1ec24da8ffb15b65a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='612' height='568'></svg>)
图 2: 在 ImageNet 上取得的准确率对于迁移到其它数据集上的经过了调优的模型有很大的预测作用。每组图片都测量了使用固定的 ImageNet 特征（最上面的一组）、经过调优的网络（中间的一组）、以及随机初始化的网络（最下方的一组）在 ImageNet 上的准确率和迁移任务的准确率之间的相关性。左图：不同的训练环境下，在迁移数据集上的分类准确率（y 轴）和最佳的 ImageNet 模型的准确率（x 轴）之间的关系。右图：相对于数据集上所有分类器的平均值，在数据集上正确分类的平均对数几率。误差条表示标准差。与最佳模型没有显出差异（p>0.05）的模型对应的点为绿色。
![](https://pic2.zhimg.com/v2-4e4101c98229252c1881ca8c663fab59_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='818' height='288'></svg>)
图 3: 对网络进行调优在固定的 ImageNet 特征上或从随机初始化模型开始的训练得到的特征取得了比 logistic 回归更高的准确率。在固定的 ImageNet 特征上的 Logistic 回归和从随机初始化开始训练的网络的性能在很大程度上取决于数据集。图中的坐标经过了合理的放缩。
![](https://pic2.zhimg.com/v2-b9e47bd9c0426d689079436a0047d609_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='821' height='184'></svg>)
图 4: 对 ImageNet 调优取得了目前最好的性能。数据条表示 logistic 回归、参数调优以及从随机初始化开始训练的模型（不包括 VGG）准确率。误差条表示标准差。数据点代表独立的模型。黑色水平线代表之前最好的模型性能。
![](https://pic1.zhimg.com/v2-e133b19b57c160d2297ef268a7f134f8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='796' height='325'></svg>)
表 2: 最优模型的性能。
![](https://pic2.zhimg.com/v2-7b203dcc155b41e13e7a98335b56d4dd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='830' height='312'></svg>)
图 5: 在 ImageNet 上预训练的网络收敛得更快。每个点表示一个使用优化过的超参数训练的 inception v4 模型。图中的坐标轴被合理放缩过。
![](https://pic1.zhimg.com/v2-cc94beb74faec85f6153fa6cf9ab6ba4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='815' height='309'></svg>)
图 6: 在 ImageNet 上预训练能够提升拥有小规模数据的细粒度任务的性能。在不同规模数据集上的 Inception v4 模型的性能。误差条反映了 3 个子集的标准差。请注意，图中显示的最大数据集的大小并不是整个数据集的大小。




**论文：Do Better ImageNet Models Transfer Better? **
![](https://pic1.zhimg.com/v2-173ceebe90c81d5ba368fb3efef7e0f4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='869' height='349'></svg>)
论文链接：[https://arxiv.org/pdf/1805.08974.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1805.08974.pdf)




**摘要：**随着 ImageNet 中提取出的特征的出现，迁移学习已经成为了计算机视觉的基石。然而评估 ImageNet 架构在不同数据集上的表现的研究却很少。现代计算机视觉研究的一个隐含的假设是，在 ImageNet 上性能更好的模型必定在其它的视觉任务上表现得更好。然而，这个假说从来没有被系统地验证过。在本文中，研究者在 3 组实验环境（固定的特征提取器、经过调优的网络权重、从随机初始化开始训练）下对比了 13 个分类模型在 12 个图像分类任务上的性能。研究者发现，当网络直接被用作固定的特征提取器时，在 ImageNet 上的准确率对它在其它网络上准确率的预测能力很弱（r^2=0.24）。在这种情况下，ResNet 的迁移能力始终优于那些在 ImageNet 上取得更高的准确率的网络。当网络经过调优后，研究者观察到一个显著增强的 ImageNet 的准确率和迁移任务的准确率之间的相关性（r^2=0.86）。研究者通过对目前最先进的 ImageNet 架构进行调优在 8 个图像分类任务上取得了目前最好的模型性能，超越了之前基于专门的迁移学习方法得到的结果。最后，研究者观察到，在 3 个小型细粒度图像分类数据集上，从随机初始化开始训练而来的网络和用 ImageNet 预训练的网络的性能相似。总而言之，本文的实验结果表明，ImageNet 架构在其它数据集上的泛化能力很强，在 ImageNet 数据集上的精确率的小小的提升就能够导致在其它数据集上性能的提升，但是 ImageNet 提取出的固定特征的泛化能力小于其架构的泛化能力。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)





