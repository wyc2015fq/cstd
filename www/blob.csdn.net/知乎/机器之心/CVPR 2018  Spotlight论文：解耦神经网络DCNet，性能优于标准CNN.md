# CVPR 2018 | Spotlight论文：解耦神经网络DCNet，性能优于标准CNN - 知乎
# 



选自arXiv，作者：Weiyang Liu等，机器之心编译。

> 本论文提出一个通用的解耦学习框架，并构建了解耦神经网络 DCNet，实验表明解耦操作可大大提高模型性能，加速收敛，提升稳健性。这篇论文已被 CVPR 2018 接收，是大会的 Spotlight 论文。

卷积神经网络（CNN）大大拓展了视觉任务的边界，如目标识别 [24, 25, 5]、目标检测 [2, 23, 22]、语义分割 [16] 等。最近，CNN 领域一个重要研究方向是：通过引入捷径连接（shortcut connection）[5, 8] 、多分支卷积（multi-branch convulsion）[25, 30] 等改进架构，来增加网络的深度和表征能力。但另一方面，尽管 CNN 有了诸多改进和提升，对于卷积本身为何能够实现判别表征和优秀的泛化能力，这依然是一个有趣的、值得探索的问题。 

如今，CNN 中通常利用内积来编码表示 patch x 和卷积核 w 之间的相似性。但内积公式 
![](https://pic3.zhimg.com/v2-af918fc7a4e042b463356c2c0efaf566_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='311' height='64'></svg>)
将语义差异（即类间差异）和类内差异耦合到同一个度量中。因此，当两个样本间的内积很大时，我们很难确定是由于两个样本间存在很大的语义/标签差异，还是由于存在很大的类内差异。为了更好地研究 CNN 表征的性质，进而改善现有的 CNN 框架，本文作者提出明确地解耦（decouple）语义差异和类内差异。具体而言，研究者使用范数和角度（angle）将内积重新参数化为：
![](https://pic4.zhimg.com/v2-b3c1b8741b9d25d6bd73303d33270f9b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='78'></svg>)
。该直觉来自图 1 中的观察，其中角度表示语义/标签差异，而特征范数（feature norm）则表示类内差异。特征范数越大，则预测越可信。这种直观的解耦方法启发研究者提出了解耦卷积算子（decoupled convolution operator）。研究者希望，通过将内积解耦为范数和角度，能够更好地对深度网络中的类内差异和语义差异进行建模。

基于将内积解耦为范数和角度这一思路，研究者通过将传统的基于内积的卷积算子（||w|| ||x|| cos(θ_(w,x))）扩展至解耦算子，提出了一个全新的解耦神经网络 DCNet。为此，研究者将此类解耦算子定义为：某个范数函数 h(||w||, ||x||) 与某个角度函数 g(θ_(w,x)) 的乘积形式。解耦算子为更好地建模类内差异和语义差异提供了一个通用框架，原始的 CNN 等价于将 h(||w||, ||x||) 设置为 ||w|| ||x||，将 g(θ_(w,x)) 设置为 cos(θ_(w,x))。（在解耦算子中），幅度函数（magnitude function）h(||w||, ||x||) 建模类内差异，而角度函数（angular function）g(θ_(w,x)) 则建模语义差异。 

从解耦的角度看，原始 CNN 包含了一个很强大的假设：类内差异可通过范数乘积的形式进行线性建模，而语义差异可利用角度余弦值刻画。但这个建模方法并非在所有任务中都是最优的，而通过解耦学习框架，我们可以根据任务本身设计解耦算子，或者直接从数据中「学习」出来。DCNet 共有以下四个优点：一，DCNet 不仅允许我们使用一些替代函数更好地建模类内差异和语义差异，还允许我们直接学习这些函数，而不是修复它们。二，通过使用有界幅度函数，DCNet 可以改善 [14] 中分析的问题，进而实现更快的收敛，同时取得和原始 CNN 相当甚至更好的准确率。三，DCNet 的一些实例展现出了面对对抗攻击时更强的稳健性：通过一个有界函数 h(·) 压缩各类别的特征空间，可以获得一定的稳健性。四，解耦算子具有很强的灵活性，且是架构不敏感的（architecture-agnostic），因此我们可以很轻松地将其引入各种网络架构，如 VGG [24]、GooleNet [25] 以及 ResNet [5]。 

具体而言，研究者提出了两种不同的解耦卷积算子：有界算子和无界算子，并利用两种算子完成多个实例。结果显示，有界算子具有更快的收敛速度，且在对抗攻击中具有更好的稳健性；而无界算子则具有更好的表征能力。解耦算子可以是平滑或非平滑的，平滑与否会影响其表现。另外，研究者针对解耦算子提出了一个全新的概念：算子半径（operator radius）。算子半径刻画了幅度函数 h(·) 对输入 ||x|| 的导数的重大变化。通过利用反向传播算法联合学习算子半径，研究者还提出了可学习的解耦算子。最后，研究者展示了多种通过改进标准反向传播算法优化解耦算子的替代方式。本论文的主要贡献如下： 
- CNN 学得的特征是天然解耦的，受此启发，研究者提出了一个明确解耦的框架来研究神经网络。 
- 研究者展示了 CNN 中包含了一个很强大的假设，以完成对类内差异和类间差异的建模，而此假设可能不是最优的。通过对内积解耦，研究者能够针对不同任务设计出更有效的幅度函数和角度函数，而非使用原始的卷积。 
- 和标准 CNN 相比，DCNet 能够更容易地收敛，且具有更好的准确率和稳健性。 
![](https://pic3.zhimg.com/v2-9b478c97118f1fcc2d9ed11a4158c89a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='867' height='600'></svg>)
图 1：CNN 学得的特征天然是解耦的。图中的 2D 特征是通过将 CNN 特征维度设置为 2 直接得到的输出。 
![](https://pic1.zhimg.com/v2-106b8b825736d5d78fb3f6e9e12b052c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='855' height='514'></svg>)
图 2：解耦卷积算子的几何解释。绿线表示原始向量，红线表示投影向量。
![](https://pic1.zhimg.com/v2-63b3316845ab05c8fb6e15843bd102a4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='367'></svg>)
表 1：加权算子（TanhConv）在 CIFAR-100 上的评估结果。 
![](https://pic4.zhimg.com/v2-e260d05e42b2835cf27c8c22309f820b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='901' height='281'></svg>)
表 2：未使用反向传播的原始 CNN-9 在 CIFAR-100 上的测试误差（%）。「N/C」表示模型未收敛，「-」表示没有结果。不同列中的结果来自于不同的角度激活函数（angular activation）。 
![](https://pic1.zhimg.com/v2-4faa3eef46ca455e70eae62bbe60e1a4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='985' height='367'></svg>)
表 6：标准 ResNet-18 和修正 ResNet-18 在 ImageNet-2012 上的 Center-crop Top-5 误差（%）。「*」表示使用了原始 CNN 在 ImageNet-2012 上的预训练模型作为初始模型（见 4.3）。
![](https://pic2.zhimg.com/v2-72b821fba0c4b2833dd8d1a6187c5d69_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='433'></svg>)
表 7：CIFAR-10 上的白盒（White-box）攻击，性能用准确率（%）度量。前三行是标准训练模型的结果，后三行是对抗训练模型的结果。
![](https://pic3.zhimg.com/v2-27c6a0f2b0087cccaf6a59c2ebfb0efa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='430'></svg>)
表 8：CIFAR-10 上的黑盒（Black-box）攻击，性能用准确率（%）度量。前三行是标准训练模型的结果，后三行是对抗训练模型的结果。




**论文：Decoupled Networks**
![](https://pic1.zhimg.com/v2-817073a813d5c40f0f17b91dca22f4e8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='203'></svg>)
论文链接：[https://arxiv.org/abs/1804.08071](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.08071)

摘要：长期以来，基于内积的卷积都是卷积神经网络（CNN）中的核心成分，也是学习视觉表征的关键所在。而 CNN 学得的特征天然能够解耦为特征范数（对应类内差异）和角度（对应语义差异），受此启发，我们提出了一个通用的解耦学习框架，对类内差异和语义差异分别进行建模。具体而言，我们首先将内积重新参数化为解耦形式，然后将其扩展至解耦卷积算子，并作为解耦神经网络的重要组成成分。我们展示了解耦卷积算子的多个有效实例，其中每个解耦算子都有很好的根据以及直观的几何解释。基于这些解耦算子，我们进一步提出直接从数据中学习算子。大量实验表明，这种解耦重新参数化操作极大地提升了模型性能，加快了模型的收敛速度，显著提升了模型的稳健性。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)



