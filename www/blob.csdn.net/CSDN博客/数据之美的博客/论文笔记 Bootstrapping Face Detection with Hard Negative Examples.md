# 论文笔记 Bootstrapping Face Detection with Hard Negative Examples - 数据之美的博客 - CSDN博客
2017年04月13日 19:56:40[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：540
来自小米研究院的文章。主要介绍了一种优秀的人脸检测技术：采用hard negative mining想法，反复通过背景得到的hard negatives，对Faster RCNN的人脸检测算子进行更新。通过FDDB数据集的验证，该方法效果显著。
# Related Work：
Hard Negative Mining 开始是用在选择function approximation learning tasks中的高质量样本中，后来在训练 region-based 目标检测算子中大量使用。该文章主要在Fast RCNN的输出中获得大量的Hard Negative Examples，放入Faster RCNN中的RPN和 Fast rcnn 预训练部分。
# Overview & Details:
Faster RCNN主要分为2个模块：RPN和Fast RCNN模块。前者用来产生可能存在目标的RoI，后者对RoI进行分类，并确定其位置和尺度。为了节约资源，RPN和Fast RCNN共享卷积层到全连接层。这里存在下面几个概念：foreground RoIs and background RoIs。
**Foreground RoIs：**即为与ground truth bounding box 的 IoU重叠大于一个阈值* thfg*. 在RCNN，SPPnet，FastRCNN和Faster RCNN中， th_fg=0.5.
**Background RoIs：**即为与ground truth bounding box的最大IoU重叠处于两个阈值之间，如[*thbg*, 0.5).*thbg*= 0.1，这部分的区域最有可能成为 hard negatives。
为了平衡 两种RoI数据，Fast RCNN中采用*bg/fg*=3:1的比例随机选择Background RoIs。
**Hard Negative Mining的主要思想：**构造一个原始的训练集平衡正负RoIs。人脸检测在这个训练集上学习，随后applied to all negatives来获得 false positives，下面将false positives加入训练集重新训练，不停迭代到达到要求。
**该进程主要包括两个循环**。第一个循环中，训练Faster RCNN人脸检测模型，第二个循环中，通过人脸检测模型得到的负样本作为hard negatives，加入到每一个mini-batch中，再重新训练得到人脸检测模型。最终得到了很好的结果。
# Some Results：
![](https://img-blog.csdn.net/20160826211517209)
![](https://img-blog.csdn.net/20160826211533398)
