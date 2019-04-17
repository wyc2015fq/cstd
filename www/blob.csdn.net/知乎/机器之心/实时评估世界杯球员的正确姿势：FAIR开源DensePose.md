# 实时评估世界杯球员的正确姿势：FAIR开源DensePose - 知乎
# 



选自Facebook Research，机器之心编译。

> 今天，Facebook AI 研究院（FAIR）开源了 [DensePose](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650737699%26idx%3D5%26sn%3D085b1f079222c5547f05ea6892802f3f%26chksm%3D871ac85db06d414b51f2f7c35bd2fa3124d558e8091ac5f0bfc5ac72bb14c4b390685773bc45%26scene%3D21%23wechat_redirect)，这是一个能将人体所有像素的 2D RGB 图像实时映射到 3D 人体模型的应用。这一模型利用 COCO 数据集中 50K 张密集型人体对应关系的标注，并在有遮挡和尺度变换等自然情况下能准确实现密集型人体姿态估计。

项目地址：[http://densepose.org/](https://link.zhihu.com/?target=http%3A//densepose.org/)
![](https://pic1.zhimg.com/v2-bf7b51f91a22bd5f9b41ffed56824244_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='367' height='104'></svg>)DensePose 应用演示![](https://pic4.zhimg.com/v2-f4f367a739efaeb11afed6d322fc1e4f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='984' height='349'></svg>)左图：输入；中图：对应的 DensePose-RCNN 结果；右图：人体分割和 UV 参数化
近期在人类理解上的研究聚焦于对稀疏的关节集合进行定位，例如手腕、手肘等。这也许对于手势或动作识别的应用是足够的，但它输出的其实是缩减的图像解释，我们希望更进一步解释人体姿态的更多信息。想象一下通过照片来试穿新衣服，或给你照片中的朋友穿上衣服等。对于这些任务，我们需要更复杂的基于表面的图像解释。

DensePose 项目解决了这个问题，并旨在以基于表面的模型来理解图像中的人物。我们的研究表明用 DensePose 可以高效地计算人体的 2D RGB 图像和 3D 表面模型之间的密集对应关系。和通常对 10 个或 20 个关节（手腕、手肘等）进行人体姿态估计的研究不同，该研究考虑的是整个人体，定义了超过 5000 个节点。最终获得的系统速度和准确率加速了和增强现实以及虚拟现实与计算机视觉之间的连接。

早期在这个问题上的研究需要分钟级的计算时间来通过外部系统初始化（例如在人体关节定位中），而且很脆弱。DensePose 可以在单块 GPU 上每秒处理多帧图像，并能同时对几十甚至几百人进行计算。

在这项研究中，我们引入了 DensePose-COCO，这是一个大规模对 COCO 数据集的 5 万个人图像到表面对应关系进行手工标注的真值数据集。其真值是以图像到表面的形式在随机采样的人体位置上标注得到，还包括分割的人体部分。我们是按照 COCO 挑战赛的训练/验证/测试集的划分方式进行划分的。
![](https://pic4.zhimg.com/v2-57cce0633b52292cc29eab6d5bf37c33_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='550'></svg>)DensePose-COCO 标注：给定一张 RGB 图像，对每个人以 UV 坐标分配多个像素点


![](https://pic4.zhimg.com/v2-d8008ee47e1707d9f7c69715367e6317_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='498' height='256'></svg>)DensePose-COCO 标注：我们在 3D 表面上对每个人分配多个不同位置的像素点
我们还为该任务开发了新的深度网络架构。我们将架构建立在 FAIR 的 Detectron 系统上，并将其整合了密集姿态估计的功能。正如在 Detectron 的 Mask-RCNN 系统中，我们使用的也是兴趣区域（RoI）池化然后是全卷积处理。我们用三个输出通道来增强网络，通过训练来输出对人体部分和和 UV 坐标的像素分配。最终得到的架构达到了和 Mask-RCNN 一样的有效速度，这多亏了 Caffe2 框架。
![](https://pic3.zhimg.com/v2-24c846aa585989b2cf89b3b2ce56e052_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1072' height='393'></svg>)DensePose-RCNN 架构：我们使用级联的候选区域生成与特征池化，并跟着一个全卷积网络以预测密集型离散部位标注和连续型表面坐标
我们开源 DensePose 的目的是希望分享 FAIR 的研究方向，并尽可能开放研究成果。FAIR 希望 DensePose 能够将计算机视觉、增强现实和计算机图形学的研究者与开发者结合在一起，并且很快产生新的经验，无论是创建全身过滤器还是从手机学习新的舞蹈。

DensePose 目前在 GitHub 上已经可以访问了，FAIR 还为 DensePose-COCO 发布了多个预训练模型与标注信息。

GitHub 项目地址：[https://github.com/facebookresearch/DensePose](https://link.zhihu.com/?target=https%3A//github.com/facebookresearch/DensePose)




**论文：DensePose: Dense Human Pose Estimation In The Wild**
![](https://pic1.zhimg.com/v2-04e71d50624a11b92e563a6b46dcc6e0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='251'></svg>)
论文地址：[https://arxiv.org/abs/1802.00434](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1802.00434)

**摘要：**在这项工作中，我们建立了 RGB 图像与基于人类身体表面表征之间的密集型对应关系，我们称这种任务为密集型人体姿态估计。我们首先引入高效的标注流程，并对 COCO 数据集中出现的 50K 个人体的图像进行密集型对应关系标注。然后使用我们的数据集训练基于 CNN 的系统，因此该系统能在「自然环境下」提供密集型的对应关系，其中自然环境表示可能存在背景、遮挡和尺度变化等。我们通过训练一个能填补缺失标注值的「修复」网络以提升训练集的有效性，并且该网络相对于过去能实现的最佳结果有显著的提升。我们实验了全卷积网络和基于区域的模型，并观察到后者更有优越性；我们通过级联进一步提升了准确度，且获得了能获得实时高准确度的系统。项目页面中提供了补充材料与视频：[http://densepose.org/](https://link.zhihu.com/?target=http%3A//densepose.org/)。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文链接：[https://research.fb.com/facebook-open-sources-densepose/](https://link.zhihu.com/?target=https%3A//research.fb.com/facebook-open-sources-densepose/)




