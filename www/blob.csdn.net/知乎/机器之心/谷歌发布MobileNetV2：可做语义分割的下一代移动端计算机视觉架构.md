# 谷歌发布MobileNetV2：可做语义分割的下一代移动端计算机视觉架构 - 知乎
# 



文章选自Google Blog，**作者：Mark Sandler、Andrew Howard，机器之心编译。**

> 深度学习在手机等移动端设备上的应用是机器学习未来的重要发展方向。2017 年 4 月，谷歌发布了 MobileNet——一个面向有限计算资源环境的轻量级神经网络。近日，谷歌将这一技术的第二代产品开源，开发者称，新一代 MobileNet 的模型更小，速度更快，同时还可以实现更高的准确度。 

项目链接：[https://github.com/tensorflow/models/tree/master/research/slim/nets/mobilenet](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/models/tree/master/research/slim/nets/mobilenet)

谷歌 2017 年推出了 MobileNetV1，它是一种为移动设备设计的通用计算机视觉神经网络，因此它也能支持图像分类和检测等。一般在个人移动设备上运行深度网络能提升用户体验、提高访问的灵活性，以及在安全、隐私和能耗上获得额外的优势。此外，随着新应用的出现，用户可以与真实世界进行实时交互，因此我们对更高效的神经网络有着很大的需求。

今天，谷歌很高兴地宣布下一代移动视觉应用 MobileNetV2 已经发布。MobileNetV2 在 MobileNetV1 的基础上获得了显著的提升，并推动了移动视觉识别技术的有效发展，包括分类、目标检测和语义分割。MobileNetV2 作为 TensorFlow-Slim 图像分类库的一部分而推出，读者也可以在 Colaboratory 中立即探索 MobileNetV2。此外，我们也可以下载代码到本地，并在 Jupyter Notebook 中探索。MobileNetV2 在 TF-Hub 中会作为模块使用，且预训练保存点可在以下地址中找到。
- Colaboratory 试验地址：[https://colab.research.google.com/github/tensorflow/models/blob/master/research/slim/nets/mobilenet/mobilenet_example.ipynb](https://link.zhihu.com/?target=https%3A//colab.research.google.com/github/tensorflow/models/blob/master/research/slim/nets/mobilenet/mobilenet_example.ipynb)
- MobileNetV2 本地实验地址：[https://github.com/tensorflow/models/blob/master/research/slim/nets/mobilenet/mobilenet_example.ipynb](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/models/blob/master/research/slim/nets/mobilenet/mobilenet_example.ipynb)
- 预训练模型下载：[https://github.com/tensorflow/models/tree/master/research/slim/nets/mobilenet](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/models/tree/master/research/slim/nets/mobilenet)

MobileNetV2 基于 MobileNetV1[1] 的基本概念构建，并使用在深度上可分离的卷积作为高效的构建块。此外，MobileNetV2 引入了两种新的架构特性：1）层之间的线性瓶颈层；2）瓶颈层之间的连接捷径。MobileNetV2 的基本架构展示如下： 
![](https://pic2.zhimg.com/v2-0439471f20d3bc87b23033a3d26aa39d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='909' height='865'></svg>)MobileNetV2 的架构概览，蓝色块如上所示为复合卷积构建块
我们可以直观理解为，瓶颈层对模型的中间输入与输出进行编码，而内层封装了模型从像素等低级概念到图像类别等高级概念的转换能力。最后，与传统的残差连接一样，捷径能快速训练并获得更优精确度。读者可查阅文末的 MobileNetV2 论文了解更多的详情。

V2 与第一代的 MobileNet 相比有什么区别？总体而言，MobileNetV2 模型在整体延迟范围内上实现相同的准确度要更快。特别是，目前新模型减少了两倍 operations 的数量，且只需要原来 70% 的参数，在 Google Pixel 手机上的测试表明 V2 要比 MobileNetV1 快 30% 到 40%，同时还能实现更高的准确度。
![](https://pic3.zhimg.com/v2-c50d16894084317848eaedc80ff3e4de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='818' height='465'></svg>)MobileNetV2 不仅速度更快（降低延迟），还刷新了 ImageNet Top 1 准确度
MobileNetV2 是一个用于目标检测和分割的非常有效的特征提取器。比如在检测方面，当 MobileNetV2 搭配上全新的 SSDLite [2]，在取得相同准确度的情况下速度比 MobileNetV1 提升了 35%。我们已通过 Tensorflow Object Detection API [4] 开源了该模型。 
![](https://pic3.zhimg.com/v2-f5dd22aedf0ffe4d2c26b52d56566eda_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1004' height='133'></svg>)
为实现实时语义分割，我们借助简化版 DeepLabv3 [3] 把 MobileNetV2 用作特征提取器，这将稍后公布。在语义分割基准 PASCAL VOC 2012 上，MobileNetV1 与 MobileNetV2 作为特征提取器表现相当，但是后者所需的参数量减少了 5.3 倍，在 Multiply-Adds 方面 operations 也减少了 5.2 倍。
![](https://pic4.zhimg.com/v2-61527368861c204ac7a2e0d4106496d7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1005' height='139'></svg>)
正如我们所看到的，MobileV2 面向移动端提供了一个非常高效的模型，它能处理许多基本的视觉识别任务。最后，谷歌也希望能与广泛的学术社区和开源社区分享这个新模型，并期待它有新的提升与应用。




**论文：MobileNetV2: Inverted Residuals and Linear Bottlenecks**
![](https://pic4.zhimg.com/v2-cc15b741fe1746989900fa154806276b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='974' height='274'></svg>)
论文链接：[https://arxiv.org/abs/1801.04381](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1801.04381)

本文中我们介绍了一种新的移动端架构——MobileNetV2，其在多任务和基准以及不同模型大小的范围上进一步刷新了移动端模型的当前最佳性能。我们还介绍了如何通过全新框架 SSDLite 将这些模型高效应用于目标检测。此外，我们也展示了通过简化版 DeepLabv3（我们称之为 Mobile DeepLabv3）构建移动端的语义分割方法。

MobileNetV2 架构基于反向残差结构，其中残差块的输入和输出是较短的瓶颈层，这与在输入中使用扩展表征的传统残差模型正相反。MobileNetV2 使用轻量级深度卷积过滤中间扩展层的特征。此外，我们发现为了保持表征能力，移除短层中的非线性很重要，这提升了性能，并带来了催生该设计的直观想法。最后，我们的方法允许将输入/输出域与转换的表现性分开，从而为未来的分析提供一个简便的框架。我们在 ImageNet 分类、COCO 目标检测、VOC 图像分割上测试了 MobileNetV2 的性能，同时也评估了精度、operations 数量（通过 MAdd 测量）以及参数量之间的权衡。




参考文献：

1. MobileNets: Efficient Convolutional Neural Networks for Mobile Vision Applications, Howard AG, Zhu M, Chen B, Kalenichenko D, Wang W, Weyand T, Andreetto M, Adam H, arXiv:1704.04861, 2017.

2. MobileNetV2: Inverted Residuals and Linear Bottlenecks, Sandler M, Howard A, Zhu M, Zhmoginov A, Chen LC. arXiv preprint. arXiv:1801.04381, 2018.

3. Rethinking Atrous Convolution for Semantic Image Segmentation, Chen LC, Papandreou G, Schroff F, Adam H. arXiv:1706.05587, 2017.

4. Speed/accuracy trade-offs for modern convolutional object detectors, Huang J, Rathod V, Sun C, Zhu M, Korattikara A, Fathi A, Fischer I, Wojna Z, Song Y, Guadarrama S, Murphy K, CVPR 2017.

5. Deep Residual Learning for Image Recognition, He, Kaiming, Xiangyu Zhang, Shaoqing Ren, and Jian Sun. arXiv:1512.03385,2015




原文地址：[https://research.googleblog.com/2018/04/mobilenetv2-next-generation-of-on.html](https://link.zhihu.com/?target=https%3A//research.googleblog.com/2018/04/mobilenetv2-next-generation-of-on.html)


