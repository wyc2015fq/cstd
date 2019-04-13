
# 超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！ - 喜欢打酱油的老鸟 - CSDN博客


2019年04月05日 12:13:19[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：18


[https://www.toutiao.com/a6675932397910884877/](https://www.toutiao.com/a6675932397910884877/)
**【新智元导读】**近日，南开大学、牛津大学和加州大学默塞德分校的研究人员共同提出了一种面向目标检测任务的新模块Res2Net，新模块可以和现有其他优秀模块轻松整合，在不增加计算负载量的情况下，在ImageNet、CIFAR-100等数据集上的测试性能超过了ResNet。
2015年，由何恺明等四位华人所提出的**ResNet**一战成名，可极快的加速神经网络的训练。
而近日，由南开大学、牛津大学和加州大学默塞德分校的研究人员共同提出的**Res2Net**，可以和现有其他优秀模块轻松整合：**在不增加计算负载量的情况下，在ImageNet、CIFAR-100等数据集上的测试性能超过了ResNet**。
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p1.pstatp.com/large/pgc-image/fb5d11f7064d4ee0a4dce69d666513f9)
关于代表性计算机视觉任务的进一步消融研究和实验结果，即目标检测，类激活 mapping和显著目标检测，进一步验证了Res2Net相对于现有技术的基线方法的优越性。
面向视觉任务的多尺度表示对于目标检测、语义分割和显著目标检测任务具有重大意义。通过CNN新模块Res2Net，能够实现与比以往优秀的基于CNN backbone 的模型（如ResNet，ResNeXt和DLA）更好的性能表现。
# Res2Net：计算负载不增加，特征提取能力更强大
在多个尺度上表示特征对于许多视觉任务非常重要。卷积神经网络（CNN） backbone 的最新进展不断展示出更强的多尺度表示能力，从而在广泛的应用中实现一致的性能提升。然而，大多数现有方法以分层方式（layer-wise）表示多尺度特征。
在本文中，研究人员在一个单个残差块内构造分层的残差类连接，为CNN提出了一种新的构建模块，即Res2Net——以更细粒度（granular level）表示多尺度特征，并增加每个网络层的感受野（receptive fields）范围。
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p3.pstatp.com/large/pgc-image/ff4b82e7a02d45419fecb3d98ab2bb16)
上图中，左侧为CNN网络架构的基本构成，右侧为本文新提出的Res2Net模块。新模块具备更强的多规模特征提取能力，但计算负载量与左侧架构类似。具体而言，新模块用一个较小的3×3过滤器取代了过滤器组，同时可以将不同的过滤器组以层级残差式风格连接。模块内部的连接形式与残差网络（ResNet）类似，故命名为Res2Net。
# 与其他现有模块的整合
本文所提出的Res2Net模块可以融合到最先进的backbone CNN模型中，例如ResNet，ResNeXt和DLA。研究人员在所有这些模型上评估 Res2Net 模块，并在广泛使用的数据集（例如CIFAR-100和ImageNet）上展示相对于基线模型的一致性能提升。
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p9.pstatp.com/large/pgc-image/7dc9fd5f77af41c494a6154700a5021d)
由于单独的Res2Net模块对于整体网络结构没有特定的要求，Res2Net模块的多尺度表示能力也和CNN的分层特征聚合模型彼此独立，所以可以很容易地将Res2Net模块集成到现有的其他优秀CNN模型中。比如ResNet，ResNeXt 和DLA 等。集成后的模型可称为Res2Net，Res2NeXt，和Res2Net-DLA。
# Res2Net模块性能及测试结果
**ImageNet数据集测试结果**
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p1.pstatp.com/large/pgc-image/6a1d2d8aa5554d1192f831274491ebf6)
ImageNet数据集Top1和Top5测试结果
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p1.pstatp.com/large/pgc-image/a47b8ac2bda04e42ad9ee4ef8f687243)
Res2Net-50在ImageNet数据集不同规模测试错误率结果。其中参数w为过滤器宽度，s为scale
**CIFAR-100数据集测试结果**
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p3.pstatp.com/large/pgc-image/563e9eca002d4189b8044be963616fce)
CIFAR-100数据集Top1错误率，以及模型大小
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p1.pstatp.com/large/pgc-image/24263d5f00044320807b453a99c4b45c)
CIFAR-100数据集不同模型大小下的检测准确率
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p1.pstatp.com/large/pgc-image/0a99785cf1f74a948e6c960563ba28b0)
ResNet-50和Res2Net-50的类激活映射可视化对比
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p1.pstatp.com/large/pgc-image/ee0ebc9937ea460fba0182112bac6287)
ResNet-101 和Res2Net-101的语义分割结果的可视化对比
![超越ResNet：南开提出Res2Net，不增计算负载，性能全面升级！](http://p1.pstatp.com/large/pgc-image/1030761e19c14d1eaaf6037dfedf4f22)
ResNet-50 和Res2Net-50的显著目标检测结果对比（图7）
**结论及未来方向**
Res2Net结构简单，性能优秀，可以进一步探索CNN在更细粒度级别的多尺度表示能力。 Res2Net揭示了一个新的维度，即“尺度”（Scale），除了深度，宽度和基数的现有维度之外，“规模”是一个必不可少的更有效的因素。
Res2Net模块可以很容易地与现有的最新模块整合。对CIFAR100和ImageNet基准测试的图像分类结果表明，使用Res2Net模块的网络始终在与对手的竞争中表现出更优秀的性能，这些对手包括ResNet，ResNeXt，DLA等。
Res2Net性能上的优越性已经在几个具有代表性的计算机视觉任务体现出来，包括类激活映射，对象检测和显着对象检测等。多尺度表示对于未来开拓更广泛的应用领域至关重要。
本文相关源代码将在论文被收稿后公开发布。
论文链接：
https://arxiv.org/pdf/1904.01169.pd

