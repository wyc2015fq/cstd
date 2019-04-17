# 基于PyTorch的GAN框架TorchGAN：用架构级API轻松定制GAN项目 - 知乎
# 



**机器之心报道，参与：刘晓坤。**

> TorchGAN 是基于 PyTorch 的 GAN 设计开发框架。该框架旨在为流行的 GAN 提供构造模块，且允许为前沿研究进行定制化。

使用 TorchGAN 的模块化结构可以：
- 在数据集上尝试流行的 GAN 模型；
- 对传统损失函数、架构等插入新的损失函数、新的架构等；
- 用多种 logging 后端无缝可视化 GAN 训练过程。

项目地址：[https://github.com/torchgan/torchgan](https://link.zhihu.com/?target=https%3A//github.com/torchgan/torchgan)

TorchGAN 包由各种生成对抗网络和实用程序组成，这些网络和实用程序在训练时非常有用。该软件包提供了易于使用的 API，可用于训练流行的 GAN 以及开发 GAN 变体。作者写了一个教程文档，帮助你使用该软件包。

文档地址：[https://torchgan.readthedocs.io/en/latest/](https://link.zhihu.com/?target=https%3A//torchgan.readthedocs.io/en/latest/)

**文档目录**

TorchGAN 的文档包含入门、API 文档和教程三大部分。

入门部分会介绍 TorchGAN 的多种安装方法、必要的依赖包安装以及给该项目贡献的指南。
![](https://pic3.zhimg.com/v2-69496d1c0b492ac60c2265ba7d054482_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='423' height='580'></svg>)
API 文档会介绍构建 GAN 网络的各种常用模块或层、损失函数、评估指标、经典模型（例如 InfoGAN、DCGAN 等）以及训练器，这些 API 可以帮助你快速从架构层面自定制模型。
![](https://pic4.zhimg.com/v2-70f751a52d2122bece1ed74f00694edf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='415' height='476'></svg>)


![](https://pic2.zhimg.com/v2-5115e693c85f402169bbd88d57e538ed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='410' height='856'></svg>)
教程部分会介绍具体的 GAN 项目构建过程，涉及了数据集构建、架构设计、超参数和优化器设置、损失函数定义、可视化、训练等机器学习项目实现的完整过程。作者使用 SAGAN 和 CycleGAN 作为典型案例的展示，还特地写了一个章节介绍如何自定制损失函数，这其实也是整个项目中最重要的环节之一，模型的训练是否收敛、收敛速度如何以及最终的收敛效果都受到损失函数定义的很大影响。
![](https://pic2.zhimg.com/v2-8b99e686cc58084dab3b8cf40f6f0301_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='740' height='805'></svg>)


![](https://pic4.zhimg.com/v2-4b9ef5daaa8e6a51b8624d5eb4b01857_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='724' height='541'></svg>)
**SAGAN 教程示例**
![](https://pic3.zhimg.com/v2-8c1260a757aeb5945342ddd9904a5cd6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='542'></svg>)


![](https://pic2.zhimg.com/v2-834d658c93aa2b8ac958e822e2e807a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='414'></svg>)


![](https://pic3.zhimg.com/v2-c30325027c891340bd2de51f52061242_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='251'></svg>)


![](https://pic1.zhimg.com/v2-b38593ea64d4afd860674efd25aab178_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='497'></svg>)


![](https://pic2.zhimg.com/v2-b0414d51698e9c96de13f05ea415fd81_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='564'></svg>)


![](https://pic4.zhimg.com/v2-044c50ec6e9c4e04f28e29cf293fb3d3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='692'></svg>)



