# 目标检测比赛的小tricks总结 - Snoopy_Dream - CSDN博客





2018年09月03日 21:02:29[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1062








### Data augmentation 数据增强

数据增强是增加深度模型鲁棒性和泛化性能的常用手段，随机翻转、随机裁剪、添加噪声等也被引入到检测任务的训练中来，个人认为数据（监督信息）的适时传入可能是更有潜力的方向。



## Multi-scale Training/Testing 多尺度训练/测试

multi-scale training/testing最早见于[1]，训练时，预先定义几个固定的尺度【mxnet上自的faster rcnn中就是】，每个epoch随机选择一个尺度进行训练。测试时，生成几个不同尺度的feature map，对每个Region Proposal，在不同的feature map上也有不同的尺度，我们选择最接近某一固定尺寸（即检测头部的输入尺寸）的Region Proposal作为后续的输入。在[2]中，选择单一尺度的方式被Maxout（element-wise max，逐元素取最大***【cascade rcnn中就是】***）取代：随机选两个相邻尺度，经过Pooling后使用Maxout进行合并，如下图所示。

![](https://pic2.zhimg.com/80/v2-31cd44d53ab7d0d7278dd1b3db1f240a_hd.jpg)

使用Maxout合并feature vector

近期的工作如FPN等已经尝试在不同尺度的特征图上进行检测，但多尺度训练/测试仍作为一种提升性能的有效技巧被应用在MS COCO等比赛中。

## Global Context 全局语境

这一技巧在ResNet的工作[3]中提出，做法是把整张图片作为一个RoI，对其进行RoI Pooling并将得到的feature vector拼接于每个RoI的feature vector上，作为一种辅助信息传入之后的R-CNN子网络。目前，也有把相邻尺度上的RoI【cascede rcnn 中的fpn】互相作为context共同传入的做法。



### 随机权值平均（Stochastic Weight Averaging，SWA）

这个内容摘自雷锋网：[https://www.leiphone.com/news/201808/WXeOHcxJ7z07GKwz.html](https://www.leiphone.com/news/201808/WXeOHcxJ7z07GKwz.html)

随机权值平均只需快速集合集成的一小部分算力，就可以接近其表现。SWA 可以用在任意架构和数据集上，都会有不错的表现。根据论文中的实验，SWA 可以得到我之前提到过的更宽的极小值。在经典认知下，SWA 不算集成，因为在训练的最终阶段你只得到一个模型，但它的表现超过了快照集成，接近 FGE（多个模型取平均）。

![新的网络优化方法：随机权值平均](https://static.leiphone.com/uploads/new/article/740_740/201808/5b76cfcb87560.png?imageMogr2/format/jpg/quality/90)

左图:W1、W2、W3分别代表3个独立训练的网络，Wswa为其平均值。

中图：WSWA 在测试集上的表现超越了SGD。

右图：WSWA 在训练时的损失比SGD要高。

结合 WSWA 在测试集上优于 SGD 的表现，这意味着尽管 WSWA 训练时的损失较高，它的泛化性更好。

SWA 的直觉来自以下由经验得到的观察：每个学习率周期得到的局部极小值倾向于堆积在损失平面的低损失值区域的边缘（上图左侧的图形中，褐色区域误差较低，点W1、W2、3分别表示3个独立训练的网络，位于褐色区域的边缘）。对这些点取平均值，可能得到一个宽阔的泛化解，其损失更低（上图左侧图形中的 WSWA）。

下面是 SWA 的工作原理。它只保存两个模型，而不是许多模型的集成：
- 
第一个模型保存模型权值的平均值（WSWA）。在训练结束后，它将是用于预测的最终模型。

- 
第二个模型（W）将穿过权值空间，基于周期性学习率规划探索权重空间。


![新的网络优化方法：随机权值平均](https://static.leiphone.com/uploads/new/article/740_740/201808/5b76cfce943b9.png?imageMogr2/format/jpg/quality/90)

SWA权重更新公式

在每个学习率周期的末尾，第二个模型的当前权重将用来更新第一个模型的权重（公式如上）。因此，在训练阶段，只需训练一个模型，并在内存中储存两个模型。预测时只需要平均模型，基于其进行预测将比之前描述的集成快很多，因为在那种集成中，你需要使用多个模型进行预测，最后再进行平均。

### 方法实现

论文的作者自己提供了一份 PyTorch 的实现 [https://github.com/timgaripov/swa](https://github.com/timgaripov/swa)。

此外，基于 fast.ai 库的 SWA 可见 [https://github.com/fastai/fastai/pull/276/commits](https://github.com/fastai/fastai/pull/276/commits)。墙裂安利这个库！



## Box Refinement/Voting 预测框微调/投票法

不同的训练策略，不同的 epoch 预测的结果，使用 NMS 来融合，或者softnms

需要调整的参数：
-  box voting 的阈值，
- 不同的输入中**这个框至少出现了几次来**允许它输出，
- 得分的阈值，一个目标框的得分低于这个阈值的时候，就删掉这个目标框。

## OHEM 在线难例挖掘

OHEM(Online Hard negative Example Mining，在线难例挖掘)见于[5]。两阶段检测模型中，提出的RoI Proposal在输入R-CNN子网络前，我们有机会对正负样本（背景类和前景类）的比例进行调整。通常，背景类的RoI Proposal个数要远远多于前景类，Fast R-CNN的处理方式是随机对两种样本进行上采样和下采样，以使每一batch的正负样本比例保持在1:3，这一做法缓解了类别比例不均衡的问题，是两阶段方法相比单阶段方法具有优势的地方，也被后来的大多数工作沿用。



![](https://pic1.zhimg.com/80/v2-8f2ecb395a32e0a87cc2a18939172d29_hd.jpg)

OHEM图解

但在OHEM的工作中，作者提出用R-CNN子网络对RoI Proposal预测的分数来决定每个batch选用的样本，这样，输入R-CNN子网络的RoI Proposal总为其表现不好的样本，提高了监督学习的效率。实际操作中，维护两个完全相同的R-CNN子网络，其中一个只进行前向传播来为RoI Proposal的选择提供指导，另一个则为正常的R-CNN，参与损失的计算并更新权重，并且将权重复制到前者以使两个分支权重同步。

OHEM以额外的R-CNN子网络的开销来改善RoI Proposal的质量，更有效地利用数据的监督信息，成为两阶段模型提升性能的常用部件之一。

## 拾遗

除去上面所列的技巧外，还有一些做法也值得注意：
- 更好的先验（YOLOv2）：**使用聚类方法统计数据中box标注的大小和长宽比，以更好的设置anchor box的生成配置**
- 更好的**pre-train模型**：检测模型的基础网络通常使用ImageNet（通常是ImageNet-1k）上训练好的模型进行初始化，使用更大的数据集（ImageNet-5k）预训练基础网络对精度的提升亦有帮助
- 超参数的调整：部分工作也发现如NMS中IoU阈值的调整（从0.3到0.5）也有利于精度的提升，但这一方面尚无最佳配置参照

最后，集成（Ensemble）作为通用的手段也被应用在比赛中。



## Reference
- [1]: [Spatial Pyramid Pooling in Deep Convolutional Networks for Visual Recognition](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1406.4729)
- [2]: [Object Detection Networks on Convolutional Feature Maps](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1504.06066)
- [3]: [Deep Residual Learning for Image Classification](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1512.03385)
- [4]: [Object Detection via a Multi-region & Semantic Segmentatio-aware CNN Model](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1505.01749)
- [5]: [Training Region-based Object Detectors with Online Hard Example Mining](https://link.zhihu.com/?target=https%3A//arxiv.org/1604.03540)
- [6]: [Improving Object Detection With One Line of Code](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1704.04503)
- [7]: [Mask R-CNN](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.06870)

————————————————————



