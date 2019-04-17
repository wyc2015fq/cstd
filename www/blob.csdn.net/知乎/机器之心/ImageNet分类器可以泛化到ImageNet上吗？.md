# ImageNet分类器可以泛化到ImageNet上吗？ - 知乎
# 



> 当前的分类模型泛化到新数据时总会有不同程度的准确率下降，传统观点认为这种下降与模型的适应性相关。但本文通过实验证明，准确率下降的原因是模型无法泛化到比原始测试集中更难分类的图像上。
选自people.csail.mit，作者：Benjamin Recht 等，机器之心编辑部，参与：高璇、王淑婷。

机器学习的首要目标是生成泛化模型。我们常通过测量模型在测试集上的性能来量化模型的泛化能力。在测试集上的良好性能是指什么？至少在遵循相同的数据清理协议时，模型在由相同数据源组成的新测试集上也能表现良好。

在本文中，研究人员通过复制两个重要基线数据集（CIFAR-10 和 ImageNet）的创建过程来实现这一实验。与理想结果相反，他们发现很多分类模型未能达到其原始的准确率分数。CIFAR-10 的准确率下降了 3％~15％，[ImageNet](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757542%26idx%3D4%26sn%3Dfd2ade5a71c5b5ee53e951431efd84a2%26chksm%3D871a9dd8b06d14cea3fd32f9e4155e965ab5731567e248c405c99348d102354fa1a8d6c9161e%26token%3D488373462%26lang%3Dzh_CN) 的准确率下降了 11％~14％。在 ImageNet 上，损失的准确率大概需要机器学习研究高度活跃时期五年的进步来弥补。

传统观点认为，出现这种下降的原因是模型已经适应了原始测试集中的特定图像，例如，通过广泛的超参数调整。然而，本文的实验表明，模型的相对顺序在新测试集上几乎保持不变：在原始测试集上具有最高准确率的模型仍然是新测试集上具有最高准确率的模型。而且，准确率方面也没有递减。实际上，原始测试集上的准确率每提高一个百分点都意味着新测试集上更大的改进。因此，虽然后来的模型本可以更好地适应测试集，但它们的准确率下降较小。实验结果证明，全面的测试集评估是改进图像分类模型的有效方法。因此，自适应性不太可能是准确率下降的原因。

相反，研究人员基于原始和新测试集的相对难度提出了另一种解释。他们证明，如果新数据集只包含候选池中最简单的图像，几乎可以完全恢复原始的 ImageNet 准确率。这表明即使是最好的图像分类器，其准确率分数也对数据清理过程的细节高度敏感。它还表明，即使在精心控制的重复性实验的良性环境中，目前的分类器仍然不能可靠地泛化。

图 1 显示了实验的主要结果。为了支持未来研究，研究人员还发布了新的测试集和相应的代码。
![](https://pic1.zhimg.com/v2-4910e91a6dad97bf726029335bf7ac88_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='415'></svg>)
图 1：在原始测试集和新测试集上的模型准确率。每个数据点对应于测试平台中的一个模型（显示为 95％的 Clopper-Pearson [置信区间](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757542%26idx%3D4%26sn%3Dfd2ade5a71c5b5ee53e951431efd84a2%26chksm%3D871a9dd8b06d14cea3fd32f9e4155e965ab5731567e248c405c99348d102354fa1a8d6c9161e%26token%3D488373462%26lang%3Dzh_CN)）。该图揭示了两个主要现象：1）从原始测试集到新测试集的准确率明显下降。2）模型准确率遵循斜率大于 1 的线性函数（CIFAR-10 为 1.7，ImageNet 为 1.1）。这意味着原始测试集上的每个百分点的改进会转换为新测试集上超过一个百分点的改进。从上图可以直观地对比两边的斜率。红色区域是 10 万个 bootstrap 样本线性拟合的 95％置信区域。

**论文：Do ImageNet Classifiers Generalize to ImageNet?**
![](https://pic3.zhimg.com/v2-551993e9c83a57a8f8270ee467c0791a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='252'></svg>)
论文地址：[http://people.csail.mit.edu/ludwigs/papers/imagenet.pdf](https://link.zhihu.com/?target=http%3A//people.csail.mit.edu/ludwigs/papers/imagenet.pdf)

摘要：我们为 CIFAR-10 和 ImageNet 数据集构建了新的测试集。这两个基准测试集近十年来一直是研究的焦点，增加了过度重复使用测试集的风险。通过密切关注原始数据集创建过程，我们测试了当前分类模型泛化到新数据的程度。我们评估了大量模型，发现在 CIFAR-10 上的准确率下降了 3％~15％，在 ImageNet 上的准确率下降了 11％~14％。但是，原始测试集上准确率的改进能够为新测试集带来更大的改进。结果表明，准确率下降不是由适应性造成的，而是由于模型无法泛化到比原始测试集中更难分类的图像上。

**实验总结**

可重复性实验的主要步骤如下。附录 B 和 C 详细地描述了此方法。第一步是选择信息性数据集。
![](https://pic2.zhimg.com/v2-26aeefc2efba335fbb281e43ffc5aaa9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='741'></svg>)
表 1：原始 CIFAR-10 测试集、原始 ImageNet 验证集和新测试集上的模型准确率。ΔRank 是所有模型完整排序中从原始测试集到新测试集的排名相对差异（参见附录 B.3.3 和 C.4.4）。例如，ΔRank= -2 意味着与原始测试集相比，新测试集上的模型下降了两名。置信区间为 95％的 Clopper-Pearson 区间。由于篇幅限制，模型的参考资料见附录 B.3.2 和 C.4.3。
![](https://pic1.zhimg.com/v2-0ae19a6505f7a53dd4c0945f1e80131c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='214'></svg>)
表 2：三种采样策略对 ImageNet 测试集的影响。该表显示了结果数据集中的平均 MTurk 选择频率，以及与原始验证集相比模型准确率的平均变化。所有三个测试集的平均选择频率都超过 0.7，但模型准确率仍然相差很大。相比之下，在 MTurk 实验中，原始 ImageNet 验证集的平均选择频率为 0.71。在 top-1 和 top-5 中，平均准确率的变化分别为 14％和 10％。这表明采样策略的细节对结果的准确率有很大影响。
![](https://pic3.zhimg.com/v2-e53c752fe78ba47701433e2410e414b2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='435'></svg>)
图 2：原始 ImageNet [验证集](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757542%26idx%3D4%26sn%3Dfd2ade5a71c5b5ee53e951431efd84a2%26chksm%3D871a9dd8b06d14cea3fd32f9e4155e965ab5731567e248c405c99348d102354fa1a8d6c9161e%26token%3D488373462%26lang%3Dzh_CN)上的模型准确率 vs. 新测试集的两个变体上的准确率。每个数据点对应于测试平台中的一个模型（显示为 95％的 Clopper-Pearson 置信区间）。阈值为 0.7 时，模型准确率比在原始测试集上低了 3％。在 MTurk 工作人员最常选择图像的 TopImages 上，模型的性能比在原始测试集上提高了 2％。两个数据集的准确率都遵循线性函数规律，类似于图 1 中的 MatchedFrequency。红色阴影区域是 10 万个 bootstrap 样本线性拟合的 95％置信区域。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*



