# 【AI】NeurIPS 2018 腾讯AI Lab详解3大热点：模型压缩、自动机器学习及最优化算法 - 知乎
# 

编者按：模型压缩使得AI模型更小更快更易广泛部署；自动机器学习自动调节参数，降低AI使用门槛；最优化算法加速AI模型的训练与收敛。小编今天为大家准备的这篇文章来自腾讯AI lab，将NeurIPS 2018大会上关于模型压缩、自动机器学习及最优化算法的最新进展一网打尽～

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

> **文章作者：腾讯AI实验室**
**责任编辑：晋杰**
文章发表于**微信公众号【运筹OR帷幄】：**[【AI】NeurIPS 2018 | 腾讯AI Lab详解3大热点：模型压缩、自动机器学习及最优化算法](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/6eyEMW9dVBR5cZrHxn8iqA)
*欢迎原链接转发，转载请私信*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

NeurIPS (Conference on Neural Information Processing Systems，神经信息处理系统进展大会)与ICML并称为神经计算和机器学习领域两大顶级学术会议。今年为第32届会议，将于 12月3日至8日在加拿大蒙特利尔举办。腾讯AI Lab第三次参加NeurIPS，共20篇论文入选，位居国内企业前列。

腾讯AI Lab对会议的三大热点——模型压缩、自动机器学习、机器学习及最优化算法相关的23篇论文进行了解读。

> **模型压缩**
**Model Compression**

模型压缩是近期深度学习领域中一个较为热门的研究方向，通过对模型的网络结构或者权重表示方式进行简化，在精度损失可控的前提下，降低模型的计算复杂度和参数规模，以解决深度学习模型计算效率较低的问题。粗略统计，本届NIPS有15-20篇模型压缩相关论文，涵盖了网络剪枝、权重量化、知识蒸馏、精简网络结构设计等多个方面。

**这次会议的研究中体现出四大特点**：一、多种权重量化算法被提出，同时相应的训练算法与理论分析也有较多讨论；二、迁移学习与知识蒸馏的结合；三、新型精简网络结构的不断涌现；四、模型压缩的应用推广至目标检测、自然语言理解等领域。另外，“精简深度神经网络及其产业界应用”研讨会（Workshop）环节集中探讨了近期模型压缩方向的研究成果以及在实际场景中的应用。我们重点关注了以下文章：

## **1. 基于敏感度正则化的稀疏神经网络训练**

**Learning Sparse Neural Networks via Sensitivity-Driven Regularization**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11386](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11386)

本文由都灵理工大学、Nuance Communications公司、巴黎高等电信学院和意大利电信集团合作完成。深度神经网络中的海量参数使得其难以应用到内存有限的场景中，而正则化加剪枝的训练方式可以使得网络权重变得稀疏，以解决这一问题。本文对网络最终输出相对于网络权重的敏感度进行量化分析，并引入了一个正则化项，以逐步减小低敏感度的网络权重的绝对值。基于该算法，大量的网络权重会逐步收敛为零，从而可以被安全地从网络模型中删去。 实验结果表明，该算法在权重稀疏度和模型预测精度方面都超过了现有算法；在部分数据集上，当模型预测精度相同时，该算法可以将权重稀疏度提升至现有算法的两倍。
![](https://pic3.zhimg.com/v2-703b62c0ea2cb1847de314bf67e05832_b.jpg)

## **2. 一种高可扩展性的8比特神经网络训练算法**

**Scalable Methods for 8-bit Training of Neural Networks**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11503](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11503)

本文由Intel和以色列理工学院合作完成。对训练完毕的网络模型进行定点量化可以提升模型在推理过程中的计算效率，但是对于如何确定最优的量化比特数以及量化方案尚无定论。本文首先通过理论分析指出，在网络训练过程中，除部分特定的操作外，大部分操作对于模型权重精度的下降并不敏感。基于这一结论，本文提出对模型权重、各层特征图以及梯度信号进行量化，并且维护了两个量化精度不同的梯度信号，在不损失精度的情况下最大程度地提升计算效率。同时，由于batch normalization层对于量化精度要求更高，本文提出了Range BN层以提升对量化误差的容忍度。实验结果表明，在ImageNet-1K数据集上，本文对模型权重、各层特征图以及梯度信号进行8比特量化，量化后模型达到了目前最好的预测精度。
![](https://pic3.zhimg.com/v2-e318823a82db67eb3f00ceae2297eed2_b.jpg)

## **3. 判别力最大化的通道剪枝算法在深度神经网络中的应用**

**Discrimination-aware Channel Pruning for Deep Neural Networks**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11109](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11109)

本文由华南理工大学、阿德莱德大学、德州大学阿灵顿分校和腾讯AI Lab合作完成。现有的通道剪枝算法大多基于稀疏约束进行重新训练，或者基于预训练模型进行剪枝后模型的重构误差最小化，存在训练困难，忽视模型判别力等问题。本文提出了一种判别力最大化的通道剪枝算法，以筛选和保留模型中真正对判别力有贡献的通道。为实现这一目标，作者引入了额外的判别力损失项，以提高网络中间各层的判别力，再基于判别力损失项和重构误差项进行各层的通道选择。实验表明，在ILSVRC-12数据集上，基于该算法剪枝后的ResNet-50模型可以在删去30%通道的情况下，top-1分类精度比基准模型高0.39%。
![](https://pic1.zhimg.com/v2-14518a2041ec5d82557e6b58d1e07248_b.jpg)

## **4. 释义复杂模型：基于因子迁移学习的网络压缩**

**Paraphrasing Complex Network:Network Compression via Factor Transfer**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11283](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11283)

本文由国立首尔大学完成。本文提出了一种新的知识蒸馏训练算法，通过引入额外的卷积层，对参考模型的信息进行解释与重新组织，从而更好地指导目标模型的训练。这一过程由两个卷积模块共同完成，分别被称为释义器（paraphraser）和翻译器（translator）。释义器以无监督的方式进行训练，以提取参考模型中的有效信息；翻译器则位于目标模型中，用于将目标模型中的对应信息进行变换，以拟合参考模型的释义器所输出的信息。实验结果表明，基于该算法训练得到的目标模型，在多个数据集上的图像分类和目标检测任务上，都取得了超过已有知识蒸馏算法的训练效果。
![](https://pic1.zhimg.com/v2-559ef726fe7f0f229aa893b4e6767704_b.jpg)


## **5. Moonshine：基于精简卷积操作的知识蒸馏**

**Moonshine:Distilling with Cheap Convolutions**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11295](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11295)

本文由爱丁堡大学完成。本文提出了一种结构化的模型蒸馏算法以降低模型运行过程中的内存开销。具体地，基于预定义的参考模型的网络结构，经过简单的结构变换，可以得到目标模型的网络结构，从而避免了重新设计目标模型的网络结构，同时也可以直接复用参考模型的超参数设定。以残差模型为例，通过对残差单元中的两个3x3卷积层进行变换，以分组卷积+跨组混合卷积的结构作为替代，可以显著降低模型的计算复杂度。变换后得到的目标模型可以基于知识蒸馏和注意力迁移学习算法进行训练。实验结果表明，该算法可以以极低的精度损失，大幅度降低内存开销；同时，相比于直接训练目标模型，知识蒸馏训练机制的引入可以明显提升目标模型的预测精度。
![](https://pic3.zhimg.com/v2-d7ddc8215c41a56b45fb019d96825402_b.jpg)

## **6. 基于在线自我集成的知识蒸馏**

**Knowledge Distillation by On-the-Fly Native Ensemble**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11723](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11723)

本文由伦敦玛丽女王大学和Vision Semantics公司合作完成。知识蒸馏常被用于训练低复杂度、高泛化能力的模型，但是离线知识蒸馏算法往往需要一个高精度的预训练模型作为参考，并且训练过程需要分为两个阶段进行，而在线知识蒸馏算法不能有效地构造一个高精度的参考模型。本文提出了一种在线自我集成的知识蒸馏算法，以实现单阶段的在线蒸馏。具体地，该算法训练了一个多分支的网络，在线地构造参考模型以提升目标模型的预测精度。实验结果表明，该算法在多个数据集上，对于各种网络结构均取得了一致的性能提升，同时训练效率更高。
![](https://pic3.zhimg.com/v2-f94136729591766ac9a3f9bddfa0661e_b.jpg)


## **7. ChannelNet：基于逐通道卷积的高效卷积神经网络**

**ChannelNets:Compact and Efficient Convolutional Neural Networks via Channel-Wise Convolutions**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11508](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11508)

本文由德克萨斯A&M大学完成。本文提出了一种逐通道卷积操作，以取代CNN模型中特征图之间的稠密连接关系。基于这一逐通道卷积操作，作者提出了ChannelNet这一轻量级的网络结构。ChannelNet中采用了三个类型的逐通道卷积操作，分别是：分组逐通道卷积、深度可分逐通道卷积以及全卷积分类层。与之前常用于移动端的CNN模型（包括SqueezeNet、MobileNet和ShuffleNet）相比，ChannelNet在性能无损的前提下显著降低了模型的参数量与计算开销。下图中，(a)是深度可分卷积，(b)是将深度可分卷积中的1x1卷积替换为分组1x1卷积，(c)是本文提出的分组逐通道卷积（以融合各组信息），(d)是本文提出的深度可分逐通道卷积。
![](https://pic2.zhimg.com/v2-5061416b1cd3a65d5184e812e5935c8d_b.jpg)

## **8. Pelee：一种基于移动端设备的实时目标检测系统**

**Pelee:A Real-Time Object Detection System on Mobile Devices**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11208](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11208)

本文由西安大略大学完成。为实现深度学习模型在移动端设备上的部署，研究者们提出了一系列精简网络结构，包括MobileNet、ShuffleNet和NASNet-A等，但是此类模型严重依赖于深度可分卷积，而这一卷积操作在大多数深度学习框架中没有较好的高效实现。针对这一问题，本文基于传统的卷积操作，构建了PeleeNet这一高效网络结构。PeleeNet可以看作是DenseNet的一个变种，采用了类似的连接模型和设计思想。具体地，PeleeNet中采用了双卷积通道和名为stem block的初始卷积单元，同时动态地调整瓶颈层中的通道数量以节省计算开销，并对SSD模型的网络结构进行优化，与PeleeNet结合，实现了在移动端设备上的实时目标检测。
![](https://pic2.zhimg.com/v2-abf6b6bfcf729b50a4a78ca99b067789_b.jpg)

> **自动机器学习**
**Auto Machine Learning**

自动机器学习（AutoML）是机器学习中最近兴起的重要分支。它主要是指，把机器学习在现实问题上的应用过程自动化的技术。在一个典型的机器学习应用中，实践者必须人工去做特征预处理、算法选择、网络结构设计（如果选择了深度学习算法）、超参调节，以求最大化模型性能。这其中的很多步骤都超出了非算法专家的能力，所以自动机器学习被提出来将这些步骤自动化。自动机器学习，使得机器学习在实际问题的应用中变得更为容易，也更容易得到一些可以超越手工设计模型性能的新模型。初略统计本届NIPS有20余篇自动机器学习相关论文，涵盖贝叶斯优化、网络结构搜索、以及迁移学习等多个方面。

**这次会议的研究中体现了三大特点**：一、研究上开始解决更为复杂的任务（例如，语义分割）的网络结构搜索；二、小领域之间开始了融合，比如出现了用贝叶斯优化（传统上用于超参优化）来解决网络结构搜索的技术研究；三、其他非自动机器学习领域的技术开始被更多地应用过来，例如迁移学习。

**1. 为稠密图像预测搜索高效多规模结构**

**Searching for Efficient Multi-Scale Architectures for Dense Image Prediction**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11831](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11831)

神经网络结构设计是许多机器学习系统在很多任务上最先进性能的关键组件。许多工作都致力于通过将巧妙构建的搜索空间和简单的学习算法结合来自动设计和构建神经网络结构。最近的进展表明，这种元学习方法在图像分类问题上可以取得超出人类设计的神经网络的性能。一个悬而未决的问题是这些方法在其他领域中能够取得什么样的性能。在这项工作中，我们关注密集图像预测的元学习技术的构建，重点是场景解析，人体分割和语义图像分割的任务。由于视觉信息的多尺度性以及图像的高分辨率，在密集图像预测任务上进行合适的网络结构搜索是十分具有挑战性的。基于对密集图像预测技术的调查，我们构建了一个递归搜索空间，并证明即使使用有效的随机搜索，我们也可以找出出优于人类设计的网络结构，并在三个密集预测上实现最先进的性能任务：其中包括在Cityscapes（街景解析）数据集上取得82.7％的准确度，在PASCAL-Person-Part（人体分割）上71.3％的准确度，以及在PASCAL VOC 2012（语义图像分割）87.9％的准确度。另外，我们模型得到的网络结构在计算上更有效，跟现有的技术相比仅需要一半的参数和一半的计算力。
![](https://pic2.zhimg.com/v2-c99f624384b70327379b86e204fe1a0d_b.jpg)

## **2. 神经结构优化**

**Neural Architecture Optimization**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11750](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11750)

自动神经网络结构设计已经显示出其在发现强大的神经网络架构方面的潜力。无论是基于强化学习还是进化算法（EA）的现有技术都是在离散空间中进行搜索。这是非常低效的。在本文中，我们提出了一种简单有效的基于连续优化的自动神经网络结构设计方法。我们将这种新方法称为神经结构优化（NAO）。我们提出的方法有三个关键组成部分：（1）编码器将神经网络架构映射到连续空间中（2）预测器将网络的连续表示作为输入并预测其准确性（3）解码器将网络的连续表示映射回其架构。性能预测器和编码器使我们能够在连续空间中执行基于梯度优化，以找到具有更高精度的新架构的表达。然后，解码器将这种更好的嵌入解码到网络。实验表明，我们的方法发现的体系结构对于CIFAR-10上的图像分类任务和PTB上的语言建模任务具有很强的竞争力，优于或者与先前的网络结构搜索方法的最佳结果相当，并且计算资源显着减少。具体来说，我们在CIFAR-10图像分类任务的测试集上的错误率为2.07%和在PTB语言建模任务测试集perplexity为55.9。在上述两个任务上发现的最佳体系结构可以被迁移到其他诸如CIFAR-100和WikiText-2等数据集合。此外，通过使用最近人们提出的权重共享机制，我们的模型可以在有限计算资源条件下（单一GPU训练10小时）在CIFAR-10（错误率为3.53％）和PTB（测试集perplexity为56.3）上取得不错的结果。
![](https://pic1.zhimg.com/v2-06e588773118b7330ced2c6979d51ffc_b.jpg)

## **3. 利用贝叶斯优化和最优运输做神经结构搜索**

**Neural Architecture Search with Bayesian Optimisation and Optimal Transport**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11214](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11214)

贝叶斯优化是指一类用于对黑盒函数f进行全局优化的方法，其中对于f我们只能得到其在某些点处的评估值。这种方法通常应用于f的评估代价非常昂贵时，在机器学习中的一个应用情形是模型选择。由于模型的泛化性能是很难分析的，所以我们一般利用带噪音和高昂的训练、验证程序去选择最好的模型。传统的贝叶斯优化方法集中在欧式和类别数据的领域，所以它在模型选择中只能用来对机器学习模型调节超参。但是，随着对深度学习的兴趣的激增，调节网络结构的需求也在快速增长。在本文中，我们设计了一个名为NASBOT的算法，一个基于高斯过程来做网络结构搜索的贝叶斯优化框架。为了实现这个目标，我们针对神经网络结构空间设计了一个可以通过最优运输程序高效计算的度量。这个度量对于深度学习社区可能还有在贝叶斯优化之外的其他用处。我们在几个基于交叉验证的多层感知器和卷积神经网络模型选择问题上，展示了NASBOT可以超越其他网络结构搜索的替代方案。
![](https://pic4.zhimg.com/v2-a5c13ef5d4abda1dada94da1b15ddbcb_b.jpg)

## **4. 利用傅里叶特征的高效高维贝叶斯优化**

**Efficient High Dimensional Bayesian Optimization with Additivity and Quadrature Fourier Features**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11859](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11859)

我们针对高维的黑盒函数优化设计了一个高效的贝叶斯优化算法。我们假设了一个可重叠变量组的广义可加模型。当变量组之间不重叠时，我们可以为高维贝叶斯优化提供第一个可以证明的多项式时间算法。为了使得优化高效和可行，我们引入一个新的基于数值积分的确定性的傅里叶特征逼近，并为平方指数核提供了详细的分析。这个逼近的错误随着特征的个数指数衰减，并且可以对后验均值和方差给出精确逼近。另外，核矩阵的逆的计算复杂度从数据点个数的立方减少到线性。
![](https://pic1.zhimg.com/v2-d3a54e911b6573cf669f03178926ed2c_b.jpg)

## **5. 带有一个未知高斯过程先验的元贝叶斯优化的后悔上界**

**Regret bounds for meta Bayesian optimization with an unknown Gaussian process prior**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11991](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11991)

贝叶斯优化通常假设一个贝叶斯先验是提前给定的。但是，由于先验里未知的参数，在贝叶斯优化里的强理论保证经常是在实践里是达不到的。在这篇文章中，我们采用了一个经验贝叶斯的变体并且展示了，通过用相同先验里抽取的离线数据来估计高斯过程先验并构造后验的无偏估计，基于置信上界和提升概率的变体都可以达到一个接近零的后悔上界。该后悔上界随着离线数据个数和在线评估个数的增加，可以衰减到一个与观察噪音成比例的常数。实验上，我们在很有挑战的模拟机器人问题处理和运动规划上成功验证了所提的方法。
![](https://pic1.zhimg.com/v2-4d3b586623451ff763a6ce63e238e154_b.jpg)

## **6. 为贝叶斯优化最大化获得函数**

**Maximizing acquisition functions for Bayesian optimization**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11938](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11938)

贝叶斯优化是一个主要依赖于获得函数来引导搜索过程的，用于全局优化的高效抽样方法。对获得函数进行完全最大化就产生了贝叶斯决策规则，但是由于这些获得函数通常都是很难优化的，所以这个理想很难实现。尤其当并行评估几个询问时，由于获得函数是非凸、高维和棘手的，最大化获得函数就更加难以实现了。为此，我们利用了蒙特卡洛积分的可微性和并行询问的子模性，提出两个新的用于最大化获得函数的方法。
![](https://pic3.zhimg.com/v2-7e15e71ee0045cd557d44f111f037b52_b.jpg)

## **7. 用于自动化机器学习的概率矩阵分解**

**Probabilistic Matrix Factorization for Automated Machine Learning**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11337](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11337)

为了达到顶尖的性能，现代机器学习技术需要仔细的数据预处理和超参调节。此外，由于研发的机器学习模型的个数的增长，模型选择也变得越来越重要。自动化机器学习的选择和调节，一直以来都是机器学习社区的研究目标之一。在本文中，我们提出通过组合协同过滤和贝叶斯优化的想法来解决这个元学习的任务。具体来说，我们通过概率矩阵分解利用在数百个不同数据上的实验，然后利用获得函数来指导对可能的机器学习流程空间的探索。在我们的实验里，我们展示了我们的方法可以在很多数据上快速确认高性能的流程，并可以显著地超越目前的顶尖方法。
![](https://pic4.zhimg.com/v2-367e3effd5258c58fc7c5fe03545563f_b.jpg)

## **8. 可扩展的超参迁移学习**

**Scalable Hyperparameter Transfer Learning**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11660](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11660)

贝叶斯优化（BO）是一种针对例如超参优化等无梯度黑盒函数优化问题的基于模型的方法。通常来说，贝叶斯优化依赖于传统的高斯过程回归，然而传统的高斯过程的算法复杂度是评价测试数目的立方级。因此，基于高斯过程的贝叶斯优化不能够利用大量过去评价测试的经验作为热启动。在本文中，我们提出了一种多任务的自适应的贝叶斯线性回归模型来解决该问题。值得注意的是，该模型仅是评价测试数目的线性复杂度。在该模型中，每个黑盒函数优化问题（任务）都以一个贝叶斯线性回归模型来建模，同时多个模型通过共享一个深度神经网络进行耦合，从而实现迁移学习。实验表明该神经网络可以学到一个适合热启动黑盒优化问题的特征表示，并且当目标黑盒函数和其他相关信号（比如训练损失）一起学习的时候，贝叶斯优化可以变快。该模型被证明至少比最近发表的相关黑盒优化文献快了至少一个数量级。
![](https://pic3.zhimg.com/v2-531f3343951976585134a78a11f6620a_b.jpg)

## **9. 利用神经自动机器学习的迁移学习**

**Transfer Learning with Neural AutoML**

[https://nips.cc/Conferences/2018/Schedule?showEvent=11799](https://link.zhihu.com/?target=https%3A//nips.cc/Conferences/2018/Schedule%3FshowEvent%3D11799)

在本文中，我们通过迁移学习减小神经网络AutoML的计算代价。AutoML通过自动化机器学习算法以减少人的干预。尽管神经网络AutoML因可以自动设计深度学习网络的结构最近非常流行，但是其计算成本巨大。为了解决该问题，我们提出了基于迁移学习的神经网络AutoML，以充分利用之前任务上学习到的知识来加快网络结构设计。我们扩展了基于增强学习的网络结构搜索方法以支持多个任务上的并行训练，并将得到的搜索策略迁移到新的任务上。在自然语言及图像分类的任务上，该方法将单任务训练的收敛时间减少了至少一个数量级。
![](https://pic1.zhimg.com/v2-9f672f69e584ae728a2ced833cbe3ed4_b.jpg)

> **机器学习与最优化算法**
** Optimization For Learning**

众所周知，机器学习与最优化算法之间的关系密不可分。在机器学习建模的过程中，会出现各式的极小值优化模型。在学习参数的过程中，会使用到各种各样的最优化算法。机器学习算法和最优化算法以及它们的交叉，历年都是研究的热点。在本次nips收录的文章中，同样出现了大量的机器学习的模型建立以及模型训练算法设计的工作。

## **1. Adaptive Methods for Nonconvex Optimization**

[https://papers.nips.cc/paper/8186-adaptive-methods-for-nonconvex-optimization.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/8186-adaptive-methods-for-nonconvex-optimization.pdf)

本文由google research 和 卡内基梅隆大学合作完成。 本文证明，通过增加batch-size，ADAM和RMSProp可以保证全局收敛性质。基于此，文中也提出了一类新的训练深度神经网络的自适应随机梯度算法YOGI。通过增加batch-size，同样证明了YOGI的全局收敛性并且通过实验验证了YOGI比ADAM具有更快的训练速度和更高的测试精度。 注意到，本文的作者曾从理论层面指出了ADAM和RMSProp是发散的，并因此获得ICLR2018 Best Paper Award。
![](https://pic1.zhimg.com/v2-df29d327651058476059e14f92500b34_b.jpg)Figure1. ADAM和YOGI的算法框架对比![](https://pic4.zhimg.com/v2-a83cc25bc08a848e38b41bb174ceda83_b.jpg)Figure2. ResNet在CIFAR10数据集上的测试精度对比![](https://pic1.zhimg.com/v2-c3ef8b37a63e7ab09f00c813b9972084_b.jpg)Figure3. 在深度自动编解码模型上, YOGI和ADAM训练损失和测试损失对比图
## **2. Spider: Near-Optimal Non-Convex Optimization via Stochastic Path Integrated Differential Estimator**

[https://papers.nips.cc/paper/7349-spider-near-optimal-non-convex-optimization-via-stochastic-path-integrated-differential-estimator.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7349-spider-near-optimal-non-convex-optimization-via-stochastic-path-integrated-differential-estimator.pdf)

本文由北京大学和腾讯AI Lab合作完成。本文的主要贡献主要在理论层面，文中提出了寻找非凸随机优化的一阶和二阶稳定点的算法SPIDER。 文中证明SPIDER算法的计算复杂度在当前求解非凸随机优化算法中是最优的。 另外文中采用的一种全新的计算复杂度的分析技巧，该技巧具有很强的可移植性，可以应用到其他很多的随机优化算法的分析中。
![](https://pic2.zhimg.com/v2-236128ea3ca547984bc7f74ca92ee4a1_b.jpg)Figure1. SPIDER算法框架![](https://pic2.zhimg.com/v2-630ae393ea240c22e0c919a96682a285_b.jpg)Figure2. 不同算法的计算复杂度对比图

## **3. Escaping Saddle Points in Constrained Optimization**

[https://papers.nips.cc/paper/7621-escaping-saddle-points-in-constrained-optimization.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7621-escaping-saddle-points-in-constrained-optimization.pdf)

本文由麻省理工学院独立完成。针对带约束的非凸光滑优化问题 , 本文提出了一类通用的求解算法框架，并且首次证明了该算法框架可以逃离约束非凸优化问题的鞍点。值得说明的是，本文是第一个提出能求够逃离约束非凸优化问题鞍点的算法。
![](https://pic3.zhimg.com/v2-8418a4b40bcb67831828ebfcaf89d46a_b.jpg)![](https://pic1.zhimg.com/v2-ba979ff9e7ff807c74058577af9b9f54_b.jpg)

## **4. Online Adaptive Methods, Universality and Acceleration**

[https://papers.nips.cc/paper/7885-online-adaptive-methods-universality-and-acceleration.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7885-online-adaptive-methods-universality-and-acceleration.pdf)

本文由苏黎世联邦理工学院和洛桑联邦理工学院合作完成。本文提出一类新的自适应随机梯度算法来AcceleGrad求解无约束随机凸优化问题。AcceleGrad算法中采用了新颖的加权自适应学习率估计策略和Nesterov加速策略来提高效率。作者理论上证明了该算法能够达到最优的计算复杂度。另外，作者从实验上说明了AcceleGrad算法的优越性。
![](https://pic3.zhimg.com/v2-c87e73214b2ce177a7ef5930aadaec36_b.jpg)Figure 1. AcceleGrad 算法框架![](https://pic4.zhimg.com/v2-3d5c745c1971ab1290c822ea0a84588f_b.jpg)Figure2. AcceleGrad 算法与SGD和AdaGrad的效果对比图。


**5. Multi-Task Learning as Multi-Objective Optimization**

[https://papers.nips.cc/paper/7334-multi-task-learning-as-multi-objective-optimization.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7334-multi-task-learning-as-multi-objective-optimization.pdf)

本文由Intel实验室独立完成。多任务学习问题通常建模成为一个极小化问题，其中这个优化问题的损失函数为多个任务的损失函数的线性组合。在本文中，作者创造性的将多任务学习直接建模成为一个多目标优化问题，
![](https://pic1.zhimg.com/v2-10fbcf68c5da9031f627cf0d98302bb0_b.jpg)
并且提出了一类求解上述多目标优化问题的梯度型算法。最后，作者通过实验上说明了基于多目标优化的多任务学习算法的优越性。
![](https://pic1.zhimg.com/v2-913229afba95dd8e748cfc9a58dd8aa8_b.jpg)Figure1. 多目标优化问题的梯度型算法框架

![](https://pic1.zhimg.com/v2-daec2c5e2ce44f36824cb167bfb02a84_b.jpg)Figure2. 不同算法的效果对比图(越小越好)


## **6. Semidefinite relaxations for certifying robustness to adversarial examples**

[https://papers.nips.cc/paper/8285-semidefinite-relaxations-for-certifying-robustness-to-adversarial-examples.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/8285-semidefinite-relaxations-for-certifying-robustness-to-adversarial-examples.pdf)

本文由斯坦福大学独立完成。神经网络的攻防是近年的研究热点。本文基于最优化中的半正定松弛技巧来确定神经网络对攻击样本的防御能力。文中提出的半正定松弛技术验证任意的ReLU神经网络的防御鲁棒性，并且从理论上说明了文中采取的半正定松弛技术比线性规划松弛技术更加精准，从而得到更好的鲁棒性估计。
![](https://pic4.zhimg.com/v2-cf182c8e458b06a9f737381aa9695d13_b.jpg)Figurre1. 不同的松弛技巧的松弛效果对比图![](https://pic4.zhimg.com/v2-2577aab112c93cc9122752f734e32da3_b.jpg)
> **文章来源申明：腾讯AI实验室[https://mp.weixin.qq.com/s/lY0wT7P8JNi8LB8k1SJfhg](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzOTg4MjEwNw%3D%3D%26mid%3D2247484089%26idx%3D1%26sn%3D1884fdc37cff413619d805ee62483f53%26scene%3D21%23wechat_redirect)**

扫二维码关注『运筹OR帷幄』公众号：
![](https://pic1.zhimg.com/v2-872dc2d2c5eaf08b38d693490549136c_b.jpg)
[点击查看『运筹OR帷幄』志愿者招募介绍及加入方式](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/4ganskchRA_iO1CiNtWmOA)：
![](https://pic1.zhimg.com/v2-d1fbc7022dc62969dcf8a6afdacdfcb4_b.jpg)

