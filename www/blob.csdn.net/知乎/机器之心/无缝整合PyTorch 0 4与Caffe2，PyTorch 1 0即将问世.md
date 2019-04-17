# 无缝整合PyTorch 0.4与Caffe2，PyTorch 1.0即将问世 - 知乎
# 



选自Facebook Research，作者：Bill Jia，机器之心编译。

> 在 F8 的第二天中，Facebook 正式宣布 PyTorch1.0 即将与大家见面，这是继一周前发布 [0.4.0](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650741377%26idx%3D1%26sn%3D3115999abcfa6c744cec334e636072f3%26chksm%3D871adeffb06d57e9155c373255ad38c30fd06f82ba7d31105b77e810b0cc0632de9c58419628%26scene%3D21%23wechat_redirect) 后的一次较大调整。这一次调整重点在于提升 PyTorch 在产品部署方面的应用，包括重构和统一 Caffe2 和 PyTorch 0.4 框架的代码库，并将 ONNX 作为模型导出格式。

在 AI 开发中，从研究到产品的过程通常涉及很多的步骤和工具，使得测试新方法、部署以及迭代提高准确率和性能很耗时、复杂。为了帮助加速和优化这个过程，Facebook 发布了 PyTorch 1.0，其开源 AI 框架的最新版本。

PyTorch 1.0 从 Caffe2 和 ONNX 移植了模块化和产品导向的功能，并将它们和 PyTorch 已有的灵活、专注研究的设计结合，已提供多种 AI 项目的从研究原型制作到产品部署的快速、无缝路径。利用 PyTorch 1.0，AI 开发者可以通过混合前端快速地实验和优化性能，该前端可以在命令式执行和声明式执行之间无缝地转换。PyTorch 1.0 中的技术已经让很多 Facebook 的产品和服务变得更强大，包括每天执行 60 亿次文本翻译。

PyTorch 1.0 将在接下来几个月内发布测试版，其中包含一系列工具、库、预训练模型和数据集，以服务于开发中的每个阶段，使社区能快速、大规模地创建和部署新的 AI 技术革新。




**从研究到产品**

PyTorch 强大的前端允许其通过灵活而又高效的编程模型快速实现原型设计与实验。第一版 PyTorch 在一年前发布，但它的速度、生产力和支持动态计算图等顶尖 AI 模型等能力，令其快速成为 AI 研究者中的流行和重要开发工具。目前 PyTorch 有超过 110 万次的下载量，并且在过去的一个月中，它是 arXiv 上引用量第二多的深度学习框架。例如 UC 伯克利计算机科学家使用 PyTorch 的动态计算图构建他的 CycleGAN 图像到图像的转换研究工作。
![](https://pic3.zhimg.com/v2-20105094a90f3b7ffbd34124385bad72_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='651'></svg>)CycleGAN 的 PyTorch 实现已经被用于图像到图像的转换
尽管当前版本的 PyTorch 为 AI 研究与开发提供了很大的灵活性，但由于与 Python 紧密结合，在生产上的性能有时是一项挑战。不论是训练脚本还是预训练模型，我们经常需要将研究代码转换为 Caffe2 中的计算图表征，因而能在生产规模上高效使用。Caffe2 基于计算图的执行器允许开发者利用顶尖的优化方法，例如计算图转换、高效的内存复用以及紧密的硬件接口集成。Caffe2 项目是两年前提出的，其目的是标准化 AI 模型的生产工具，目前该框架在 Facebook 服务器以及超过 10 亿台手机上运行，横跨了八代 iphone 和六代安卓 CPU 架构。Caffe2 目前每天在所有小或大模型上提供了 200 万亿次的预测，并优化了产品性能。

从 PyTorch 到 Caffe2 的迁移过程以前是手动的、耗时间的和容易出错的。为了解决这个问题，Facebook 与主要的硬件和软件公司合作创建了 ONNX（开放神经网络交换格式），这是一种用于表示深度学习模型的开放格式。通过 ONNX，开发者能在不同的框架间共享模型，例如我们可以导出由 PyTorch 构建的模型，并将它们导入到 Caffe2。在 Facebook 中，这令我们能在大规模服务器和移动端上快速实现 AI 的研究、训练和推断。

Facebook 已经用这些工具（PyTorch、Caffe2 和 ONNX）来构建和部署 Translate，这一工具能大规模运行并支持翻译 Facebook 中最常用的 48 种语言。在 VR 中，这些工具对于将基于 Oculus 的新研究部署到生产过程中至关重要。

然而，虽然结合这三种不同的工具是非常高效的，但仍然有非常多复杂且耗时的手动过程。因此它并不允许我们将 AI 研究创新无缝转换为产品。




**在一个框架里统一研究和产能**

PyTorch 1.0 融合了动态执行和图执行模式，为研究提供了灵活性，为产品提供了优化性能。具体来说，与其让开发者完全重写代码来进行优化或从 Python 迁移模型，PyTorch 1.0 提供了一个混合前端，使我们能在原型制作的动态执行模式和产品的图执行模式之间无缝地共享代码主体。

此外，ONNX 也被原生地编入了 PyTorch 1.0 作为模型导出格式，使来自 PyTorch 1.0 的模型可以和其它 AI 框架进行互操作。ONNX 还提供了整合接口以加速运行时或硬件特定的库。这给了开发者完全的自由度来混合、匹配最好的 AI 框架和工具，而不需要执行资源密集的自定义工程。Facebook 承诺在 ONNX 中支持新的特性和功能，它将继续作为强大的开放格式和 PyTorch 1.0 中的重要开发工具。




**构建端到端深度学习系统**

除了 PyTorch 1.0，Facebook 还开源了他们如今大规模使用的多个 AI 工具。其中包括 Translate（一个 PyTorch 语言库）：用于快速、灵活的神经机器翻译；以及下一代 ELF：AI 推理应用的综合平台。开发者还可以充分利用 Glow：这是一个机器学习编译器，可以在不同的硬件平台上加速框架的性能；以及 Tensor Comprehensions：可以从高级数学运算高效地自动生成 GPU 代码的工具。他们还开源了其他库，例如 Detectron：支持目标检测研究，可以输出边框和目标实例分割。在 facebook.ai/developers 可以找到完整的列表。

在接下来的几个月里，Facebook 将重构和统一 Caffe2 和 PyTorch 0.4 框架的代码库，以进行组件去重复和共享抽象。其结果是我们将得到一个统一的框架，它可以支持高效的图模式执行、移动部署、广泛的供应商整合等。正如其它开放的 AI 倡议如 ONNX，他们还将联合其他公司和社区为更多开发者提供这些加速研究以提高产能。作为开端，微软计划在他们的 Azure 云和开发者产品（包括 Azure Machine Learning 服务和 Data Science Virtual Machines）中支持 PyTorch 1.0；并且 AWS 目前也支持 PyTorch 的最新版本，为 P3 GPU 实例做了优化，并计划在发布他们的云产品（包括 Deep Learning AMI）之后的短期内支持 PyTorch 1.0。这仅仅是个开始，以后我们将可以创建和共享更好的 AI 编程模型、接口和自动优化。在如今的 Facebook，AI 是一项基础技术，使现有的产品更优并能处理全新的经验。通过论文、代码和模型开源这些工作，Facebook 将和所有 AI 研究者和从业者一起工作，来使当前最佳技术更快，并将它们迁移到新的用途上。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://code.facebook.com/posts/172423326753505/announcing-pytorch-1-0-for-both-research-and-production/](https://link.zhihu.com/?target=https%3A//code.facebook.com/posts/172423326753505/announcing-pytorch-1-0-for-both-research-and-production/)


