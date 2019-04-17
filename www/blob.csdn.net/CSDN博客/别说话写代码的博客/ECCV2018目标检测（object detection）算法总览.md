# ECCV2018目标检测（object detection）算法总览 - 别说话写代码的博客 - CSDN博客





2018年11月21日 00:41:39[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：134








文章转自：https://blog.csdn.net/u014380165/article/details/82025720    作者：AI之路

这篇博客记录我个人比较感兴趣的ECCV2018关于目标检测（object detection）的一些文章。

1、IOU-Net

论文：Acquisition of Localization Confidence for Accurate Object Detection

论文链接：https://arxiv.org/abs/1807.11590

详细博客介绍：IOU-Net 算法笔记

这篇文章个人非常喜欢，我们知道在目标检测算法中最后一般都会通过NMS算法移除重复预测框，移除的依据是预测框的分类得分（classification confidence，也就是softmax层的概率输出），但是这种依据并不能很好地表征预测框的准确性，换句话说对于同一个ground truth而言，分类得分高的预测框并不一定比分类得分低的预测框好，这样就容易导致那些分类得分低，但是坐标和ground truth更接近，也就是坐标回归更准确的预测框在NMS阶段被过滤掉，导致最终指标的下降。因此这篇文章就提出IOU-Net，通过预测检测框和ground truth的IOU来解决这2个问题：1、提出IOU-guided NMS，也就是在NMS阶段引入回归得分（localization confidence）作为排序指标而不是采用传统的分类得分。2、提出optimization-based bbox refinement替换传统的regression-based方法，提高了回归部分的可解释性。另外这篇文章还提出了Precise ROI Pooling（PrROI Pooling），通过积分方式计算ROI特征使得前向计算的误差进一步降低，同时反向传播时基于连续输入值计算梯度使得反向传播连续可导，相比之下ROI Pooling和ROI Align由于采用量化或几个点插值方式求ROI特征，不可避免地带来一些噪声，而且在反向求导时只对特定输入回传梯度。

Figure1是关于这篇文章出发点的介绍。



2、DetNet

论文：DetNet: A Backbone network for Object

链接：https://arxiv.org/abs/1804.06215

详细博客介绍：DetNet 算法笔记

这篇关于目标检测的文章，主要是对检测算法的特征提取网络（backbone）做优化。我们知道目前大部分的目标检测算法在训练时都会用预训练的分类模型来提取特征，这些预训练模型是在ImageNet数据集上训练得到的，众多的实验也证明了这种做法的有效性。而DetNet这篇文章相当于研究更加有效的特征提取网络，出发点也非常直接，主要包含两点：1、分类任务和检测任务还是有一定差别的，因此用分类数据上训练的分类模型来提取特征用于检测任务不一定合适，比如检测任务比较关注目标的尺度特征，但是分类任务就不一定了。2、检测任务不仅仅要做目标的分类，而且要做目标的定位，这样的差异容易导致一些问题，比如在分类网络中常用的降采样操作可能对分类有效，因为增大了感受野，但是对于需要定位目标的检测任务而言就不一定有利，因为丢失了目标的位置信息。因此DetNet的提出主要也是针对这两个出发点，换句话说是设计了一个专门用于目标检测算法的特征提取网络，主要改进点包括：1、增加网络高层输出特征的分辨率，换句话说就是高层不对特征图做尺寸缩减。2、引入dilated卷积层增加网络高层的感受野，这是因为第一个改进点引起的感受野减小。3、减小网络高层的宽度，减少因增大分辨率带来的计算量。

Figure1是第1个改进点。



Figure2是关于第2、3个改进点。



3、RFB Net

论文：Receptive Field Block Net for Accurate and Fast Object Detection

论文链接：https://arxiv.org/abs/1711.07767

代码链接：https://github.com/ruinmessi/RFBNet

详细博客介绍：RFB Net算法笔记

这篇关于目标检测的文章，提出了RFB Net网络用于目标检测，可以在兼顾速度的同时达到良好的效果。该网络主要在SSD网络中引入Receptive Field Block (RFB) ，引入RFB的出发点通过模拟人类视觉的感受野加强网络的特征提取能力，在结构上RFB借鉴了Inception的思想，主要是在Inception的基础上加入了dilated卷积层（dilated convolution），从而有效增大了感受野（receptive field）。整体上因为是基于SSD网络进行改进，所以检测速度还是比较快，同时精度也有一定的保证。

RFB结构如Figure4所示。



RFB Net结构如Figure5所示。



4、PFPNet

论文：Parallel Feature Pyramid Network for Object Detection

论文链接：http://openaccess.thecvf.com/content_ECCV_2018/html/Seung-Wook_Kim_Parallel_Feature_Pyramid_ECCV_2018_paper.html

详细博客介绍：PFPNet 算法笔记

这篇文章借鉴了SPP的思想并通过MSCA（multi-scale context aggregation）模块进行特征融合从而提出PFPNet（Parallel Feature Pyramid Network）算法来提升目标检测的效果。PFPNet整体上还是one stage类型的目标检测算法，思想借鉴了SSD，速度上有保证，效果上因为引入SPP思想构造特征金字塔，相当于加宽了网络，同时MSCA模块做类似FPN的特征融合操作，最后基于多层融合特征进行预测，因此效果也是很不错的，和CVPR2018的RefineDet效果基本上差不多。

PFPNet的网络结构如Figure3所示。



5、CornerNet

论文：CornerNet: Detecting Objects as Paired Keypoints

论文链接：https://arxiv.org/abs/1808.01244

代码链接：https://github.com/umich-vl/CornerNet

详细博客介绍：CornerNet 算法笔记

这篇目标检测文章给人一种眼前一亮的感觉，简单说一下几个比较吸引我的点：1、将目标检测问题当作关键点检测问题来解决，也就是通过检测目标框的左上角和右下角两个关键点得到预测框，因此CornerNet算法中没有anchor的概念，这种做法在目标检测领域是比较创新的而且能够取得不错效果是很难的。2、整个检测网络的训练是从头开始的，并不基于预训练的分类模型，这使得用户能够自由设计特征提取网络，不用受预训练模型的限制。

CornerNet算法整体结构如Figure4所示。





