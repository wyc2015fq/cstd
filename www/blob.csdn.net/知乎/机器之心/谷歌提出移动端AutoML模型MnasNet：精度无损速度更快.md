# 谷歌提出移动端AutoML模型MnasNet：精度无损速度更快 - 知乎
# 



**选自Google AI Blog，作者： Mingxing Tan，机器之心编译。**

> 目前开发者可以使用非常多的移动端 CNN 架构，也可以在机器上训练新颖的视觉模型并部署到手机端。但各种各样的移动端任务可能并不能借助已有的 CNN 架构达到非常好的效果，因此谷歌近日将神经架构搜索方法引入了轻量级的 CNN 网络，并提出基于强化学习的 MnasNet 以自动设计移动端模型。

卷积神经网络（CNN）已被广泛用于图像分类、人脸识别、目标检测和其他领域。然而，为移动设备设计 CNN 是一项具有挑战性的工作，因为移动端模型需要体积小、速度快，还要保持精准。尽管人们已经做了大量努力来设计和改进移动端模型，如 MobileNet 和 MobileNetV2，但手动创建高效模型仍然是一项挑战，因为要考虑的因素太多。从最近 AutoML 神经架构搜索方面的进展受到启发，我们在想移动端 CNN 模型的设计是否也能受益于 AutoML 方法。

在《MnasNet: Platform-Aware Neural Architecture Search for Mobile》一文中，我们探索了一种使用强化学习设计移动端模型的自动化神经架构搜索方法。为了处理移动端速度限制，我们明确地将速度信息纳入搜索算法的主要奖励函数中，以便搜索可以识别一个在准确率和速度之间实现良好平衡的模型。如此，MnasNet 能够找到运行速度比 MobileNet V2（手工制造的最先进水平）快 1.5 倍、比 NASNet 快 2.4 倍的型号，同时达到同样的 ImageNet top-1 准确率。

不同于以前的架构搜索方法，在这种方法中，模型速度是通过另一个替代品（如 FLOPS）来考虑的。我们的方法通过在特定平台上执行模型来直接测量模型速度，例如本研究中使用的 Pixel 手机。通过这种方式，我们可以直接测量现实世界中可以实现的指标，因为每种类型的移动设备都有自己的软件和硬件特性，并且可能需要不同的架构来在准确率和速度之间做出最佳权衡。

我们方法的总体流程主要包括三个部分：一个基于 RNN 的学习和采样模型架构控制器，一个建立和训练模型以获得准确率的训练器，以及一个使用 TensorFlow Lite 测量真实手机上模型速度的推理引擎。我们制定了一个多目标优化问题，旨在实现高准确率和高速，并利用带有定制奖励函数的强化学习算法来寻找帕累托最优解 (例如，具有最高准确率但速度不变的模型)。
![](https://pic2.zhimg.com/v2-b86fab408e3bf79c42867495affa00b5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='752' height='386'></svg>)移动端自动神经架构搜索方法的整体流程图
为了在搜索灵活性与搜索空间大小之间取得适当的平衡，我们提出了一种新型因子化的层级搜索空间，它将卷积神经网络分解为一个由模块组成的序列，然后使用层级搜索空间决定每一个模块的层级结构。通过这种方式，我们的方法允许不同的层级使用不同的运算与连接。同时，我们强制每一个模块共享相同的结构，因此与逐层搜索相比搜索空间要显著地小几个数量级。
![](https://pic4.zhimg.com/v2-81eec9b645235dcd4f0d74dc6a6b785f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='179'></svg>)我们的 MnasNet 网络从新型因子化层级搜索空间中采样，上图展示了网络架构中的不同层级
我们在 ImageNet 分类和 COCO 目标检测任务上测试了该方法的有效性。我们的实验在典型的移动端资源下实现了新的当前最优准确度。特别地，下图展示了该网络在 ImageNet 上的结果。
![](https://pic1.zhimg.com/v2-a50235d0508db859da208379cafba390_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1045' height='773'></svg>)ImageNet 准确率和推理延迟的对比
在相同的准确率下，MnasNet 模型的速度比手工调参得到的当前最佳模型 MobileNet V2 快 1.5 倍，并且比 NASNet 快 2.4 倍，它也是使用架构搜索的算法。在应用压缩和激活（squeeze-and-excitation）优化方法后，MnasNet+SE 模型获得了 76.1% 的 ResNet 级别的 top-1 准确率，其中参数数量是 ResNet 的 1/19，且乘法-加法运算量是它的 1/10。在 COCO 目标检测任务上，我们的模型族获得了比 MobileNet 更快的速度和更高的准确率，并在 1/35 的计算成本下获得了和 SSD300 相当的准确率。

我们很高兴看到该自动化架构搜索方法可以在多个复杂移动设备视觉任务上取得当前最佳性能。在未来，我们计划把更多的运算和优化方法整合到搜索空间中，并将其应用到更多的移动设备视觉任务上，例如语义分割。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文地址：[https://ai.googleblog.com/2018/08/mnasnet-towards-automating-design-of.html](https://link.zhihu.com/?target=https%3A//ai.googleblog.com/2018/08/mnasnet-towards-automating-design-of.html)


