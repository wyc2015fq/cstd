# 腾讯AI Lab提出自动化模型压缩框架PocketFlow - 知乎
# 



**机器之心发布，来源：腾讯 AI Lab。**

> 据机器之心了解，腾讯 AI Lab 机器学习中心今日宣布成功研发自动化深度学习模型压缩框架——PocketFlow，并即将在近期发布开源代码。据介绍，这是一款面向移动端 AI 开发者的自动模型压缩框架，集成了当前主流（包括腾讯 AI Lab 自研）的模型压缩与训练算法，结合自研超参数优化组件实现了全程自动化托管式的模型压缩与加速。开发者无需了解具体算法细节，即可快速地将 AI 技术部署到移动端产品上，实现用户数据的本地高效处理。目前该框架正在为腾讯的多项移动端业务提供模型压缩与加速的技术支持，在多款手机 APP 中得到应用。值得一提的是，不久之前 MIT 韩松等人发表论文，提出了用于[移动端模型自动压缩与加速的AutoML方法AMC](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650748544%26idx%3D2%26sn%3Da028bec83587c58cca32b308fce28bee%26chksm%3D871af2feb06d7be8709b0109b34714c7e5028b85a19f31dcb3219d6a1acc59e75aef5190aa28%26scene%3D21%23wechat_redirect)。

随着 AI 技术的飞速发展，越来越多的公司希望在自己的移动端产品中注入 AI 能力，以优化用户使用体验。以深度学习为代表的 AI 技术极大地提升了图像理解、语音识别等诸多应用领域中的识别精度，但是主流的深度学习模型往往对计算资源要求较高，难以直接部署到消费级移动设备中。常用的解决方案是将复杂的深度学习模型部署在云端，移动端将待识别的数据上传至云端，再等待云端返回识别结果，但这对网络传输速度的要求较高，在网络覆盖不佳地区的用户使用体验较差，同时数据上传至云端后的隐私性也难以保证。

在这种情况下，众多模型压缩与加速算法应运而生，能够在较小的精度损失（甚至无损）下，有效提升 CNN 和 RNN 等网络结构的计算效率，从而使得深度学习模型在移动端的部署成为可能。但是，如何根据实际应用场景，选择合适的模型压缩与加速算法以及相应的超参数取值，往往需要较多的专业知识和实践经验，这无疑提高了这项技术对于一般开发者的使用门槛。

在此背景下，腾讯 AI Lab 机器学习中心研发了 PocketFlow 开源框架，以实现自动化的深度学习模型压缩与加速，助力 AI 技术在更多移动端产品中的广泛应用。通过集成多种深度学习模型压缩算法，并创新性地引入超参数优化组件，极大地提升了模型压缩技术的自动化程度。开发者无需介入具体的模型压缩算法及其超参数取值的选取，仅需指定设定期望的性能指标，即可通过 PocketFlow 得到符合需求的压缩模型，并快速部署到移动端应用中。




**AI Lab 在 PocketFlow 上的研究进展**

据了解，近期腾讯 AI Lab 机器学习中心在深度学习模型压缩以及超参数优化算法方面持续投入，并取得了诸多研究进展。在模型压缩算法方面，团队提出了一种基于判别力最大化准则的通道剪枝算法，在性能基本无损的前提下可以大幅度降低 CNN 网络模型的计算复杂度，相关论文已被 NIPS 2018 接收 [1]。该算法在训练过程中引入多个额外的损失项，以提升 CNN 网络中各层的判别力，然后逐层地基于分类误差与重构误差最小化的优化目标进行通道剪枝，去除判别力相对较小的冗余通道，从而实现模型的无损压缩。在超参数优化算法方面，团队研发了 AutoML 自动超参数优化框架，集成了包括高斯过程（Gaussian Processes, GP）和树形结构 Parzen 估计器（Tree-structured Parzen Estimator, TPE）等在内的多种超参数优化算法，通过全程自动化托管解决了人工调参耗时耗力的问题，大幅度提升了算法人员的开发效率。

另一方面，考虑到深度学习模型的训练周期普遍较长，团队对基于 TensorFlow 的多机多卡训练过程进行优化，降低分布式优化过程中的梯度通信耗时，研发了名为 TF-Plus 的分布式优化框架，仅需十几行的代码修改即可将针对单个 GPU 的训练代码扩展为多机多卡版本，并取得接近线性的加速比。此外，团队还提出了一种误差补偿的量化随机梯度下降算法，通过引入量化误差的补偿机制加快模型训练的收敛速度，能够在没有性能损失的前提下实现一到两个数量级的梯度压缩，降低分布式优化中的梯度通信量，从而加快训练速度，相关论文发表于 ICML 2018 [2]。

在 PocketFlow 框架的开发过程中，团队加入了对上述多个自研算法的支持，并且有效降低了模型压缩的精度损失，提升了模型的训练效率，并极大地提高了超参数调节方面的自动化程度。




**PocketFlow 框架介绍**

PocketFlow 框架主要由两部分组件构成，分别是模型压缩/加速算法组件和超参数优化组件，具体结构如下图所示。
![](https://pic2.zhimg.com/v2-29d7abf13c5952a55344f82ca0306e41_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='381'></svg>)
开发者将未压缩的原始模型作为 PocketFlow 框架的输入，同时指定期望的性能指标，例如模型的压缩和/或加速倍数；在每一轮迭代过程中，超参数优化组件选取一组超参数取值组合，之后模型压缩/加速算法组件基于该超参数取值组合，对原始模型进行压缩，得到一个压缩后的候选模型；基于对候选模型进行性能评估的结果，超参数优化组件调整自身的模型参数，并选取一组新的超参数取值组合，以开始下一轮迭代过程；当迭代终止时，PocketFlow 选取最优的超参数取值组合以及对应的候选模型，作为最终输出，返回给开发者用作移动端的模型部署。

具体地，PocketFlow 通过下列各个算法组件的有效结合，实现了精度损失更小、自动化程度更高的深度学习模型的压缩与加速： 

**a) 通道剪枝（channel pruning）组件**：在 CNN 网络中，通过对特征图中的通道维度进行剪枝，可以同时降低模型大小和计算复杂度，并且压缩后的模型可以直接基于现有的深度学习框架进行部署。在 CIFAR-10 图像分类任务中，通过对 ResNet-56 模型进行通道剪枝，可以实现 2.5 倍加速下分类精度损失 0.4%，3.3 倍加速下精度损失 0.7%。

**b) 权重稀疏化（weight sparsification）组件：**通过对网络权重引入稀疏性约束，可以大幅度降低网络权重中的非零元素个数；压缩后模型的网络权重可以以稀疏矩阵的形式进行存储和传输，从而实现模型压缩。对于 MobileNet 图像分类模型，在删去 50% 网络权重后，在 ImageNet 数据集上的 Top-1 分类精度损失仅为 0.6%。

**c) 权重量化（weight quantization）组件：**通过对网络权重引入量化约束，可以降低用于表示每个网络权重所需的比特数；团队同时提供了对于均匀和非均匀两大类量化算法的支持，可以充分利用 ARM 和 FPGA 等设备的硬件优化，以提升移动端的计算效率，并为未来的神经网络芯片设计提供软件支持。以用于 ImageNet 图像分类任务的 ResNet-18 模型为例，在 8 比特定点量化下可以实现精度无损的 4 倍压缩。

**d) 网络蒸馏（network distillation）组件：**对于上述各种模型压缩组件，通过将未压缩的原始模型的输出作为额外的监督信息，指导压缩后模型的训练，在压缩/加速倍数不变的前提下均可以获得 0.5%-2.0% 不等的精度提升。

**e) 多 GPU 训练（multi-GPU training）组件：**深度学习模型训练过程对计算资源要求较高，单个 GPU 难以在短时间内完成模型训练，因此团队提供了对于多机多卡分布式训练的全面支持，以加快使用者的开发流程。无论是基于 ImageNet 数据的 Resnet-50 图像分类模型还是基于 WMT14 数据的 Transformer 机器翻译模型，均可以在一个小时内训练完毕。

**f) 超参数优化（hyper-parameter optimization）组件：**多数开发者对模型压缩算法往往不甚了解，但超参数取值对最终结果往往有着巨大的影响，因此团队引入了超参数优化组件，采用了包括强化学习等算法以及 AI Lab 自研的 AutoML 自动超参数优化框架来根据具体性能需求，确定最优超参数取值组合。例如，对于通道剪枝算法，超参数优化组件可以自动地根据原始模型中各层的冗余程度，对各层采用不同的剪枝比例，在保证满足模型整体压缩倍数的前提下，实现压缩后模型识别精度的最大化。
![](https://pic2.zhimg.com/v2-0da498dde75d689c3652d3daf84b5f39_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='562'></svg>)






**PocketFlow 性能展示**

通过引入超参数优化组件，不仅避免了高门槛、繁琐的人工调参工作，同时也使得 PocketFlow 在各个压缩算法上全面超过了人工调参的效果。以图像分类任务为例，在 CIFAR-10 和 ImageNet 等数据集上，PocketFlow 对 ResNet 和 MobileNet 等多种 CNN 网络结构进行有效的模型压缩与加速。

在 CIFAR-10 数据集上，PocketFlow 以 ResNet-56 作为基准模型进行通道剪枝，并加入了超参数优化和网络蒸馏等训练策略，实现了 2.5 倍加速下分类精度损失 0.4%，3.3 倍加速下精度损失 0.7%，且显著优于未压缩的 ResNet-44 模型； 在 ImageNet 数据集上，PocketFlow 可以对原本已经十分精简的 MobileNet 模型继续进行权重稀疏化，以更小的模型尺寸取得相似的分类精度；与 Inception-V1、ResNet-18 等模型相比，模型大小仅为后者的约 20~40%，但分类精度基本一致（甚至更高）。
![](https://pic1.zhimg.com/v2-298ff881ffdb1006c9771e5047c60bd0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='867' height='507'></svg>)![](https://pic4.zhimg.com/v2-4a1bf646933c4f04f9d6070d38cb33bb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='867' height='507'></svg>)
相比于费时费力的人工调参，PocketFlow 框架中的 AutoML 自动超参数优化组件仅需 10 余次迭代就能达到与人工调参类似的性能，在经过 100 次迭代后搜索得到的超参数组合可以降低约 0.6% 的精度损失；通过使用超参数优化组件自动地确定网络中各层权重的量化比特数，PocketFlow 在对用于 ImageNet 图像分类任务的 ResNet-18 模型进行压缩时，取得了一致性的性能提升；当平均量化比特数为 4 比特时，超参数优化组件的引入可以将分类精度从 63.6% 提升至 68.1%（原始模型的分类精度为 70.3%）。
![](https://pic4.zhimg.com/v2-60ff6ff8bd44a69737b3f68ee4c0b6b3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='867' height='507'></svg>)![](https://pic2.zhimg.com/v2-4ea3447f86da2d795d3fb11668cb6465_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='867' height='507'></svg>)



**PocketFlow 助力移动端业务落地**

据了解，在腾讯公司内部，PocketFlow 框架正在为多项移动端实际业务提供了模型压缩与加速的技术支持。例如，在手机拍照 APP 中，人脸关键点定位模型是一个常用的预处理模块，通过对脸部的百余个特征点（如眼角、鼻尖等）进行识别与定位，可以为后续的人脸识别、智能美颜等多个应用提供必要的特征数据。团队基于 PocketFlow 框架，对人脸关键点定位模型进行压缩，在保持定位精度不变的同时，大幅度地降低了计算开销，在不同的移动处理器上取得了 25%-50% 不等的加速效果，压缩后的模型已经在实际产品中得到部署。
![](https://pic1.zhimg.com/v2-d1debde0b813314a7c4a7070c0e6f34c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='867' height='507'></svg>)



**结语**

深度学习模型的压缩与加速是当前学术界的研究热点之一，同时在工业界中也有着广泛的应用前景。腾讯 AI Lab 表示，随着 PocketFlow 的推出，开发者无需了解模型压缩算法的具体细节，也不用关心各个超参数的选择与调优，即可基于这套自动化框架，快速得到可用于移动端部署的精简模型，从而为 AI 能力在更多移动端产品中的应用铺平了道路。




**参考文献**

[1] Zhuangwei Zhuang, Mingkui Tan, Bohan Zhuang, Jing Liu, Jiezhang Cao, Qingyao Wu, Junzhou Huang, Jinhui Zhu,「Discrimination-aware Channel Pruning for Deep Neural Networks", In Proc. of the 32nd Annual Conference on Neural Information Processing Systems, NIPS '18, Montreal, Canada, December 2018.

[2] Jiaxiang Wu, Weidong Huang, Junzhou Huang, Tong Zhang,「Error Compensated Quantized SGD and its Applications to Large-scale Distributed Optimization」, In Proc. of the 35th International Conference on Machine Learning, ICML’18, Stockholm, Sweden, July 2018.


