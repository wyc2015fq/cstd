# 现在，所有人都可以在18分钟内训练ImageNet了 - 知乎
# 



选自Fast.ai，**作者：Jeremy Howard，机器之心编译，参与：路、张倩。**

> 由 fast.ai 学员 Andrew Shaw、DIU 研究员 Yaroslav Bulatov 和 Jeremy Howard（fast.ai 创始研究员）组成的团队在 18 分钟内成功完成训练 Imagenet 的任务，准确率达到 93%。他们使用了 16 个 AWS 云实例（每个实例使用 8 个英伟达 V100 GPU）来运行 fastai 和 PyTorch 库。这一速度打破了在公共基础架构上训练 Imagenet 达到 93% 准确率的速度记录，且比谷歌在 DAWNBench 竞赛中使用其专有 TPU Pod 集群的训练速度快 40%。该团队使用的处理单元数量和谷歌的基准（128）一样，运行成本约 40 美元。
![](https://pic1.zhimg.com/v2-4639022a58c26548910bd3da09035070_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='606'></svg>)
该团队的主要训练方法是：fast.ai 用于分类任务的渐进式调整大小和矩形图像验证；英伟达的 NCCL 库，该库整合了 PyTorch 的 all-reduce 分布式模块；腾讯的权重衰减调整方法；谷歌大脑的动态批量大小的一个变体，学习率逐步预热（Goyal 等人 2018、Leslie Smith 2018）。该团队使用经典的 ResNet-50 架构和具备动量的 SGD。




**背景**

四个月前，fast.ai 团队在 DAWNBench 竞赛中取得了巨大成功，他们使用单个机器（标准 AWS 公有云实例）实现了最快的 Imagenet 训练速度。谷歌在竞赛中的表现也很抢眼，其使用尚未公开的 TPU Pod 集群赢得了整个 Imagenet 速度类别的冠军。fast.ai 团队使用单个机器的训练时间为三小时，而谷歌的 TPU Pod 集群仅用了约半小时。而在该比赛之前，在公有云上训练 ImageNet 需要花费数天。




**实验基础架构**

快速迭代需要面临诸多挑战，比如：
- 如何在没有大量可持续运行的昂贵实例时，在多台机器上轻松运行多个实验？
- 如何便捷地利用 AWS 的 EC2 Spot 实例（比一般实例便宜 70%）？每次使用该实例时都需要从零开始重新设置。

fast.ai 为 DAWNBench 竞赛构建了一个系统，包括用于启动和配置新实例、运行实验、收集结果和查看进展的 Python API。该系统还有一些更有趣的设计：
- 未使用配置文件，而是利用 Python API 写代码来配置实验。因此，该团队能够使用循环（loop）、条件句（conditional）等来快速设计和运行结构化实验，如超参数搜索。
- 围绕 tmux 和 ssh 写一个 Python API 封装，并启动 tmux 会话中的所有设置和训练任务。这方便后续登录机器和连接 tmux 会话，以监控进展、修复问题等。
- 一切「从简」：避免 Docker 等容器技术或 Horovod 等分布式计算系统。该团队未使用复杂的集群架构（这样的架构需要单独的参数服务器、存储数组、集群管理节点等），而是仅使用具备常规 EBS 存储卷的单个实例类型。

单独来看，DIU 面临一系列挑战，最终开发出一个具备相似动机和设计的集群网络，能够并行运行很多大规模训练实验。该 nexus-scheduler 解决方案受到 Yaroslav 在谷歌 Borg 系统上运行机器学习实验的经验启发。

fast.ai 开发的工具集则关注单个机器实验的快速迭代，DIU 开发的 nexus-scheduler 关注鲁棒性和多机器实验。Andrew Shaw 将 fast.ai 软件的一部分融入 nexus-scheduler，这样创建出的系统就融合了二者最好的部分，可用于实验。

使用 nexus-scheduler 帮助该团队在分布式实验上进行迭代，如：
- 为单个实验启动多台机器以实现分布式训练。这些用于分布式运行的机器被自动归为放置组（placement group），实现了更快的网络性能。
- 通过 Tensorboard 进行监控，事件文件和检查点存储在区域级文件系统上。
- 自动设置。分布式训练的多种必备资源（如 VPC、安全组和 EFS）以透明的方式进行创建。

AWS 提供了一个非常有用的 API（[https://boto3.readthedocs.io/en/latest/](https://link.zhihu.com/?target=https%3A//boto3.readthedocs.io/en/latest/)），帮助该团队快速轻松地构建好所需的一切。他们使用英伟达的 NCCL 库进行分布式计算，该库实现了环形集群，且整合了 PyTorch 的 all-reduce 分布式模块。该团队发现 AWS 的实例非常可靠，且提供持续性能，这对利用 all-reduce 算法取得最好结果非常重要。

nexus-scheduler 将在 8 月 25 日进行首次官方发布。
![](https://pic2.zhimg.com/v2-77bdc6c3f756415dc722210885d054fd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='572'></svg>)使用 Tensorboard 分析网络利用情况



**一种简单的训练新技巧：矩形图像！**

DAWNBench 竞赛之后，Jeremy Howard 认为深度学习实践者在做一件非常愚蠢的事情：大家用的是矩形图像（如 ImageNet 中使用的那些），在做预测的时候只裁剪中间的部分。还有一种广泛使用的方法（速度很慢）是裁剪 5 个位置（左上、左下、右上、右下及中心），并算出预测的平均值。这就引出了一个明显的问题：为什么不直接用矩形图像呢？

很多人对卷积神经网络有误解，认为它只能处理一种固定尺寸的图像，而且一定要是矩形。然而，多数库支持「适应」或「全局」池化层，这就完全克服了这一局限。当一些库（如 Pytorch）分配不使用这一特征的模型时，这没有什么用处——这意味着除非这些库的用户替换掉那些层，否则他们就只能使用固定尺寸和形状的图像（通常是 224x224 像素）。fastai 库可以自动将固定尺寸的模型转换为动态尺寸的模型。

目前还没看到有人尝试使用矩形图像进行训练，也没在任何论文中看到类似表述。目前也没有一个标准的深度学习库支持这一点。因此 Andrew 找到一种方法：结合 fastai 和 Pytorch 进行预测。

结果令人惊艳——fast.ai 达到 93% 的基准准确率所花费的时间缩短了 23%。
- 不同方法的对比可点击以下链接进行查看：[https://nbviewer.jupyter.org/github/bearpelican/Experiments/blob/master/rectangular_images/RectangularImageVisualization.html；](https://link.zhihu.com/?target=https%3A//nbviewer.jupyter.org/github/bearpelican/Experiments/blob/master/rectangular_images/RectangularImageVisualization.html)
- 它们的准确率对比参见：[https://nbviewer.jupyter.org/github/bearpelican/Experiments/blob/master/rectangular_images/RectangularImageValidationComparison.html](https://link.zhihu.com/?target=https%3A//nbviewer.jupyter.org/github/bearpelican/Experiments/blob/master/rectangular_images/RectangularImageValidationComparison.html)。
![](https://pic1.zhimg.com/v2-4c33ed35b30dbaf1a7b79fda981adb20_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='978'></svg>)
对比不同剪裁方法的 Jupyter Notebook 片段。「Center Crop Image」是原始图像，「FastAi rectangular」是 fastai 团队提出的新方法，「Imagenet Center」是标准方法，「Test Time Augmentation」是 multi-crop 方法的示例。




**渐进式调整大小、动态批量大小等**

fast.ai 在 DAWNBench 竞赛中取得的主要进展是引入了渐进式图像尺寸调整来进行分类——在训练开始时使用小图像，随着训练的进行逐渐增加图像尺寸。采用这种做法，刚开始的时候模型非常不准确，但它可以很快看到大量图像并取得快速进展，在接下来的训练中，模型可以看更大的图像，学习更加细粒度的区别。

这一新研究还对一些中间的 epoch 使用更大的批量大小，以更好地利用 GPU RAM 并避免网络延迟。

最近，腾讯发布了一篇很不错的研究[论文](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650745993%26idx%3D2%26sn%3Da7cbba27e46a68588d7eb5084201717c%26chksm%3D871ae8f7b06d61e19debfbd288a1220a643888cbba7a7e66112b988577223da44bfaae76dd79%26scene%3D21%23wechat_redirect)（[https://arxiv.org/abs/1807.11205](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1807.11205)），显示可以在 2048 块 GPU 上用不到 7 分钟的时间训练 Imagenet。他们提到了一个 fast.ai 之前没有尝试过的方法，但这个方法非常合理：移除批归一化层上的权重衰减。使用这种方法可以从训练时间中再缩减几个 epoch。-（腾讯的论文还使用了 NVIDIA Research 开发的动态学习率方法 LARS，fastai 团队也开发了研究人员为 fastai 开发的，但还没有包含在这些结果中。）




**结果**

综合以上种种，fast.ai 在 16 个 AWS 实例上用 18 分钟的时间完成 Imagenet 训练，计算总成本大约为 40 美元（包括机器配置时间的成本）。能够在超过 100 万张图像的数据集上进行训练有很多好处，比如：
- 拥有大型图像库的组织（如放射科、汽车保险公司、房源挂牌服务和电子商务网站）现在可以创建自己的定制化模型。虽然使用如此多的图像进行迁移学习通常会过犹不及，但是对于高度专业化的图像类型或细粒度分类（如医学成像中常见的），使用大量数据可能会得到更好的结果。
- 小型研究实验室可以利用不同的架构、损失函数、优化器等进行实验，并在 Imagenet 上进行测试，这也是许多评审人员希望在论文中看到的。
- 使用标准公共云基础架构，则开启前沿深度学习研究就不需要前期成本投入。

过去 25 年，各种关于「好结果需要大量计算」的言论层出不穷，但是研究人员目前提出的有趣想法很少是利用大量计算做出的，如批归一化、ReLU、dropout、adam/adamw、LSTM 等。今天，每个人都可以获取到所需的大量计算基础设施。使深度学习更平民化比只关注大公司的成就有更长远的影响力，因为前者可以利用全世界数百万研究人员的聪明才智。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[http://www.fast.ai/2018/08/10/fastai-diu-imagenet/](https://link.zhihu.com/?target=http%3A//www.fast.ai/2018/08/10/fastai-diu-imagenet/)


