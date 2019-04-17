# 目标检测（十六）--MS-CNN - 工作笔记 - CSDN博客





2017年09月12日 15:40:09[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：11070
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿
`A Unified Multi-scale Deep Convolutional Neural Network for Fast Object Detection`- intro: ECCV 2016
- intro: 640×480: 15 fps, 960×720: 8 fps
- arxiv: [http://arxiv.org/abs/1607.07155](http://arxiv.org/abs/1607.07155)
- github: [https://github.com/zhaoweicai/mscnn](https://github.com/zhaoweicai/mscnn)
- poster: [http://www.eccv2016.org/files/posters/P-2B-38.pdf](http://www.eccv2016.org/files/posters/P-2B-38.pdf)

**Multi-stage Object Detection with Group Recursive Learning**
- intro: VOC2007: 78.6%, VOC2012: 74.9%
- arxiv: [http://arxiv.org/abs/1608.05159](http://arxiv.org/abs/1608.05159)

**Subcategory-aware Convolutional Neural Networks for Object Proposals and Detection**
- intro: WACV 2017. SubCNN
- arxiv: [http://arxiv.org/abs/1604.04693](http://arxiv.org/abs/1604.04693)
- github: [https://github.com/yuxng/SubCNN](https://github.com/yuxng/SubCNN)

转自：[http://blog.csdn.net/ture_dream/article/details/52750565?locationNum=7](http://blog.csdn.net/ture_dream/article/details/52750565?locationNum=7)

最新看篇关于多尺度下快速目标检测的论文，是UCSD大学的SVCL实验室和IBM研究院一起研究的结果。

# 这篇文章主要解决多尺度同时存在时的检索问题，设计了MSCNN网络，提出了两点创新和几点技巧：

**（1）针对多尺度问题：**

   类似于FCNT跟踪方法，该文章也是观察到了卷积网络不同层得到的特征特点的不同，对不同层的特征采用不同的利用方式。比如conv-3的低网络层，有更小的感受野，可以进行小目标的检测；而高层如conv-5，对于大目标的检测更加准确。对于不同的输出层设计不同尺度的目标检测器，完成多尺度下的检测问题。

   注：　在卷积神经网络中，感受野的定义是卷积神经网络每一层输出的特征图（feature map）上的像素点 在原始图像上映射的区域大小。[点击打开链接](http://blog.csdn.net/kuaitoukid/article/details/46829355)

**（2）针对速度问题：**

   使用特征的上采样代替输入图像的上采样步骤。设计一个去卷积层，来增加特征图的分辨率，使得小目标依然可以被检测出来。这里使用了特征图的deconvolutional layer（去卷积层）来代替input图像的上采样，可以大大减少内存占用，提高速度。

  作者说：去卷积层一直用于分隔和边缘检测，我们第一次用它加速和提高检测率。

# 文章的网络结构类似RCNN，分为proposal提取和目标检测，两个部分独立进行。

proposal子网络和目标检测子网络结构图分别如下：

![](https://img-blog.csdn.net/20160906220243318)

注：中间有色部分可以看成CNN trunk(cnn 主干线），一些层延伸了分支，分支由单检测层构成，通过分支的proposal判断为最终proposal。

技巧一：Conv4-3上面有一个缓冲卷积层，因为这一次更靠近主干线的底层（接近输入图），更影响梯度导致学习过程不稳定，缓冲卷积层能防止检测分支的梯度直接反向传播回主干线层。



![](https://img-blog.csdn.net/20160906220212473)



# 实验细节：



 数据扩展： 在KITTI数据集中，大多数目标很小，候选集差别很大，而最大目标尺寸过小，故把原始图像随机缩放成多个规模。

  精调：   图像背景区域消耗计算内存，故在1000*600的大图里裁剪448*448的含目标图片，是12G显存够用

 训练技巧： booststrapping 和multi-task loss 使训练在早期迭代过程不稳定，采用两步走，

      第一步：随机采样，以学习率0.00005迭代10000次，生成模型

      第二部：用生成的模型初始化第二层，自举，以初始学习率0.00005迭代，每迭代一万次学习率下降十倍，一共迭代 25000次。

# 实验过程与结果：

文章主要在KITTI和Caltech Pedestrian数据集上进行了MSCNN的测试，因为这两个数据集中包含许多小目标。

作者对**proposal网络部分**和**目标检测部分**的实验结果分别进行了全部系统的分析和对比，这里仅仅将在KITTI上的检测结果展示：

![](https://img-blog.csdn.net/20160906221242999)



