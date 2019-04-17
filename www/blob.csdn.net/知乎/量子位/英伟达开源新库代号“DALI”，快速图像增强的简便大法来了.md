# 英伟达开源新库代号“DALI”，快速图像增强的简便大法来了 - 知乎
# 



> 林鳞 编译整理
量子位 出品 | 公众号 QbitAI

又有新的开源库来了，英伟达出品，数量×2。

正在举行的CVPR 2018大会上，英伟达开源了数据增强库NVIDIA DALI和图像解码库NVIDIA nvJPEG。

两个新库都是什么画风？简而言之，**NVIDIA DALI**是一个GPU加速的数据增强和图像加载库，为优化深度学习框架数据pipeline而设计，而**NVIDIA nvJPEG**是用于JPEG解码的高性能GPU加速库。

具体效果如何？我们继续往下看。



![](https://pic1.zhimg.com/v2-449b58ebd492d0ce1aa69a73eb274a08_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='328'></svg>)
△ 英伟达CEO黄仁勋

## **详细介绍**

一般来说，计算机视觉应用里有多阶段预处理数据pipeline，包括从磁盘加载和提取数据、解码、裁剪和调整大小、颜色和空间转换和格式转换等数据密集型步骤。

通过DALI，研究人员可利用MXNet、TensorFlow和PyTorch在AWS P3 8 GPU实例或Volta GPU的DGX-1系统上扩展训练性能。

综合来看，DALI的方便之处在于：
- 能与MxNet、TensorFlow和PyTorch等简单的框架集成
- 具有支持多种数据格式的可移植训练工作流，如JPEG、原始格式、LMDB、RecordIO和TFRecord等
- 具有可配置图形和自定义operator的自定义数据pipeline

当DALI进行高性能GPU加速解码时，就需要nvJPEG登场了。这个GPU加速库支持单个和批处理图像的解码、颜色空间转换、多相位解码，以及CPU和GPU混合解码。

与只使用CPU解码的应用程序相比，用nvJPEG进行解码的吞吐量更高，延迟低了。



![](https://pic4.zhimg.com/v2-a99d6d71b70ef18a8a49653a78b8f9c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='339'></svg>)



## **你要的相关链接来了**

英伟达官网上有DALI和nvJPEG的相关介绍，地址给你：

[https://news.developer.nvidia.com/announcing-nvidia-dali-and-nvidia-nvjpeg/?ncid=](https://link.zhihu.com/?target=https%3A//news.developer.nvidia.com/announcing-nvidia-dali-and-nvidia-nvjpeg/%3Fncid%3D)—43656

目前DALI已完成开源，可在GitHub上使用：

[https://github.com/NVIDIA/dali](https://link.zhihu.com/?target=https%3A//github.com/NVIDIA/dali)

nvJPEG的候选版本可以免费下载，征集反馈和测试中：
[nvJPEG​developer.nvidia.com![图标](https://pic3.zhimg.com/v2-0bc4a85e5905be37ba4e0e8caf82b31a_ipico.jpg)](https://link.zhihu.com/?target=https%3A//developer.nvidia.com/nvjpeg)



— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


