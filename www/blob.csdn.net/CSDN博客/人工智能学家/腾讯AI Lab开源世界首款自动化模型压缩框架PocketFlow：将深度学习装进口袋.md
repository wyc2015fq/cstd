# 腾讯AI Lab开源世界首款自动化模型压缩框架PocketFlow：将深度学习装进口袋 - 人工智能学家 - CSDN博客
2018年09月18日 22:05:49[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：155
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBUuODCnQkDiczL24wLKxlBlFn3eVdfRUYg65zxcL7EE0rNb3xOOM6DEX5WA8icYMAtAYHibk1u9elJ9Q/640?wx_fmt=jpeg)
来源：腾讯AI 实验室
腾讯AI Lab机器学习中心今日宣布成功研发出世界上首款自动化深度学习模型压缩框架——PocketFlow，并即将在近期发布开源代码。这是一款面向移动端AI开发者的自动模型压缩框架，集成了当前主流（包括腾讯AI Lab自研）的模型压缩与训练算法，结合自研超参数优化组件实现了全程自动化托管式的模型压缩与加速。开发者无需了解具体算法细节，即可快速地将AI技术部署到移动端产品上，实现用户数据的本地高效处理。目前该框架正在为腾讯的多项移动端业务提供模型压缩与加速的技术支持，在多款手机APP中得到应用。
**PocketFlow背景**
随着AI技术的飞速发展，越来越多的公司希望在自己的移动端产品中注入AI能力，以优化用户使用体验。以深度学习为代表的AI技术极大地提升了图像理解、语音识别等诸多应用领域中的识别精度，但是主流的深度学习模型往往对计算资源要求较高，难以直接部署到消费级移动设备中。常用的解决方案是将复杂的深度学习模型部署在云端，移动端将待识别的数据上传至云端，再等待云端返回识别结果，但这对网络传输速度的要求较高，在网络覆盖不佳地区的用户使用体验较差，同时数据上传至云端后的隐私性也难以保证。
在这种情况下，众多模型压缩与加速算法应运而生，能够在较小的精度损失（甚至无损）下，有效提升CNN和RNN等网络结构的计算效率，从而使得深度学习模型在移动端的部署成为可能。但是，如何根据实际应用场景，选择合适的模型压缩与加速算法以及相应的超参数取值，往往需要较多的专业知识和实践经验，这无疑提高了这项技术对于一般开发者的使用门槛。
在此背景下，腾讯AI Lab机器学习中心研发了PocketFlow开源框架，以实现自动化的深度学习模型压缩与加速，助力AI技术在更多移动端产品中的广泛应用。通过集成多种深度学习模型压缩算法，并创新性地引入超参数优化组件，极大地提升了模型压缩技术的自动化程度。开发者无需介入具体的模型压缩算法及其超参数取值的选取，仅需指定设定期望的性能指标，即可通过PocketFlow得到符合需求的压缩模型，并快速部署到移动端应用中。
与韩松等人提出的AMC算法相比，PocketFlow框架支持更多模型压缩算法、更高效模型训练方式，更完善的自动超参数优化功能。最重要的是，该框架除了在内部业务被应用，也将在10月开源代码，给到开发者直接的便利。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJ2qLUVH7ZBHC0GSkc2xkF0hdqsib3icSBIaduacqbZ5N6icooAkPOgMAzd6B2SbD5pjLfPNm4kfK0icg/640?wx_fmt=png)
**AI Lab在PocketFlow上的研究进展**
近期，AI Lab机器学习中心在深度学习模型压缩以及超参数优化算法方面持续投入，并取得了诸多研究进展。在模型压缩算法方面，团队提出了一种基于判别力最大化准则的通道剪枝算法，在性能基本无损的前提下可以大幅度降低CNN网络模型的计算复杂度，相关论文发表于NIPS 2018。该算法在训练过程中引入多个额外的损失项，以提升CNN网络中各层的判别力，然后逐层地基于分类误差与重构误差最小化的优化目标进行通道剪枝，去除判别力相对较小的冗余通道，从而实现模型的无损压缩。在超参数优化算法方面，团队研发了AutoML自动超参数优化框架，集成了包括高斯过程（Gaussian Processes, GP）和树形结构Parzen估计器（Tree-structured Parzen Estimator, TPE）等在内的多种超参数优化算法，通过全程自动化托管解决了人工调参耗时耗力的问题，大幅度提升了算法人员的开发效率。
另一方面，考虑到深度学习模型的训练周期普遍较长，团队对基于TensorFlow的多机多卡训练过程进行优化，降低分布式优化过程中的梯度通信耗时，研发了名为TF-Plus的分布式优化框架，仅需十几行的代码修改即可将针对单个GPU的训练代码扩展为多机多卡版本，并取得接近线性的加速比。此外，团队还提出了一种误差补偿的量化随机梯度下降算法，通过引入量化误差的补偿机制加快模型训练的收敛速度，能够在没有性能损失的前提下实现一到两个数量级的梯度压缩，降低分布式优化中的梯度通信量，从而加快训练速度，相关论文发表于ICML 2018。
在PocketFlow框架的开发过程中，团队加入了对上述多个自研算法的支持，并且有效降低了模型压缩的精度损失，提升了模型的训练效率，并极大地提高了超参数调节方面的自动化程度。
**PocketFlow框架介绍**
PocketFlow框架主要由两部分组件构成，分别是模型压缩/加速算法组件和超参数优化组件，具体结构如下图所示。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJ2qLUVH7ZBHC0GSkc2xkF07ED2I22PuK98uaCeOxcxsIUia5JhVunhLzg2yWxe44EjWYBsXMvKguw/640?wx_fmt=png)
开发者将未压缩的原始模型作为PocketFlow框架的输入，同时指定期望的性能指标，例如模型的压缩和/或加速倍数；在每一轮迭代过程中，超参数优化组件选取一组超参数取值组合，之后模型压缩/加速算法组件基于该超参数取值组合，对原始模型进行压缩，得到一个压缩后的候选模型；基于对候选模型进行性能评估的结果，超参数优化组件调整自身的模型参数，并选取一组新的超参数取值组合，以开始下一轮迭代过程；当迭代终止时，PocketFlow选取最优的超参数取值组合以及对应的候选模型，作为最终输出，返回给开发者用作移动端的模型部署。
具体地，PocketFlow通过下列各个算法组件的有效结合，实现了精度损失更小、自动化程度更高的深度学习模型的压缩与加速：
**a) 通道剪枝（channel pruning）组件：**在CNN网络中，通过对特征图中的通道维度进行剪枝，可以同时降低模型大小和计算复杂度，并且压缩后的模型可以直接基于现有的深度学习框架进行部署。在CIFAR-10图像分类任务中，通过对ResNet-56模型进行通道剪枝，可以实现2.5倍加速下分类精度损失0.4%，3.3倍加速下精度损失0.7%。
**b) 权重稀疏化（weight sparsification）组件：**通过对网络权重引入稀疏性约束，可以大幅度降低网络权重中的非零元素个数；压缩后模型的网络权重可以以稀疏矩阵的形式进行存储和传输，从而实现模型压缩。对于MobileNet图像分类模型，在删去50%网络权重后，在ImageNet数据集上的Top-1分类精度损失仅为0.6%。
**c) 权重量化（weight quantization）组件：**通过对网络权重引入量化约束，可以降低用于表示每个网络权重所需的比特数；团队同时提供了对于均匀和非均匀两大类量化算法的支持，可以充分利用ARM和FPGA等设备的硬件优化，以提升移动端的计算效率，并为未来的神经网络芯片设计提供软件支持。以用于ImageNet图像分类任务的ResNet-18模型为例，在8比特定点量化下可以实现精度无损的4倍压缩。
**d) 网络蒸馏（network distillation）组件：**对于上述各种模型压缩组件，通过将未压缩的原始模型的输出作为额外的监督信息，指导压缩后模型的训练，在压缩/加速倍数不变的前提下均可以获得0.5%-2.0%不等的精度提升。
**e) 多GPU训练（multi-GPU training）组件：**深度学习模型训练过程对计算资源要求较高，单个GPU难以在短时间内完成模型训练，因此团队提供了对于多机多卡分布式训练的全面支持，以加快使用者的开发流程。无论是基于ImageNet数据的Resnet-50图像分类模型还是基于WMT14数据的Transformer机器翻译模型，均可以在一个小时内训练完毕。
**f) 超参数优化（hyper-parameter optimization）组件：**多数开发者对模型压缩算法往往不甚了解，但超参数取值对最终结果往往有着巨大的影响，因此团队引入了超参数优化组件，采用了包括强化学习等算法以及AI Lab自研的AutoML自动超参数优化框架来根据具体性能需求，确定最优超参数取值组合。例如，对于通道剪枝算法，超参数优化组件可以自动地根据原始模型中各层的冗余程度，对各层采用不同的剪枝比例，在保证满足模型整体压缩倍数的前提下，实现压缩后模型识别精度的最大化。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJ2qLUVH7ZBHC0GSkc2xkF01BcCx8kyVZibvIyJtRATBWZDouIibvUibp8IibibAukVibWRQEjfg644rOMA/640?wx_fmt=png)
**PocketFlow性能展示**
通过引入超参数优化组件，不仅避免了高门槛、繁琐的人工调参工作，同时也使得PocketFlow在各个压缩算法上全面超过了人工调参的效果。以图像分类任务为例，在CIFAR-10和ImageNet等数据集上，PocketFlow对ResNet和MobileNet等多种CNN网络结构进行有效的模型压缩与加速。
在CIFAR-10数据集上，PocketFlow以ResNet-56作为基准模型进行通道剪枝，并加入了超参数优化和网络蒸馏等训练策略，实现了2.5倍加速下分类精度损失0.4%，3.3倍加速下精度损失0.7%，且显著优于未压缩的ResNet-44模型；在ImageNet数据集上，PocketFlow可以对原本已经十分精简的MobileNet模型继续进行权重稀疏化，以更小的模型尺寸取得相似的分类精度；与Inception-V1、ResNet-18等模型相比，模型大小仅为后者的约20~40%，但分类精度基本一致（甚至更高）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJ2qLUVH7ZBHC0GSkc2xkF0s8hRA2fdYkA5AtYjNhfZcjLSyDUyhdo4icJHvuEq3hPF5Aic52gnTmfA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJ2qLUVH7ZBHC0GSkc2xkF0kbSVXKk8sgBNllmCicVuN2yiaUEo6DdSYsmd8l7hR5anr68fWciaAxf9Q/640?wx_fmt=png)
相比于费时费力的人工调参，PocketFlow框架中的AutoML自动超参数优化组件仅需10余次迭代就能达到与人工调参类似的性能，在经过100次迭代后搜索得到的超参数组合可以降低约0.6%的精度损失；通过使用超参数优化组件自动地确定网络中各层权重的量化比特数，PocketFlow在对用于ImageNet图像分类任务的ResNet-18模型进行压缩时，取得了一致性的性能提升；当平均量化比特数为4比特时，超参数优化组件的引入可以将分类精度从63.6%提升至68.1%（原始模型的分类精度为70.3%）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJ2qLUVH7ZBHC0GSkc2xkF0yrFSS3pdZXPe7dPsLnGpibGPBVtx3y5boicarWNia7v3xKjf5633o0wog/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJ2qLUVH7ZBHC0GSkc2xkF0mfvibJ6xiaTbAupzCicPwPcZKYbP38FTgDFd1PuAevZfezCOHTzW8C0ZQ/640?wx_fmt=png)
**PocketFlow助力移动端业务落地**
在腾讯公司内部，PocketFlow框架正在为多项移动端实际业务提供了模型压缩与加速的技术支持。例如，在手机拍照APP中，人脸关键点定位模型是一个常用的预处理模块，通过对脸部的百余个特征点（如眼角、鼻尖等）进行识别与定位，可以为后续的人脸识别、智能美颜等多个应用提供必要的特征数据。团队基于PocketFlow框架，对人脸关键点定位模型进行压缩，在保持定位精度不变的同时，大幅度地降低了计算开销，在不同的移动处理器上取得了25%-50%不等的加速效果，压缩后的模型已经在实际产品中得到部署。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANJ2qLUVH7ZBHC0GSkc2xkF0Gcn2HicFb7BGfiaciciaI3Cy2qibNc6siaf2NcbymSQ1UUyOJZjwNLXdOgHg/640?wx_fmt=png)
**结语**
深度学习模型的压缩与加速是当前学术界的研究热点之一，同时在工业界中也有着广泛的应用前景。随着PocketFlow的推出，开发者无需了解模型压缩算法的具体细节，也不用关心各个超参数的选择与调优，即可基于这套自动化框架，快速得到可用于移动端部署的精简模型，从而为AI能力在更多移动端产品中的应用铺平了道路。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
