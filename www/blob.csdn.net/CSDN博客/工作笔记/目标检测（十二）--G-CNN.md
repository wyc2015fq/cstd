# 目标检测（十二）--G-CNN - 工作笔记 - CSDN博客





2017年09月12日 09:44:29[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5760
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

**G-CNN: an Iterative Grid Based Object Detector**
- arxiv: [http://arxiv.org/abs/1512.07729](http://arxiv.org/abs/1512.07729)

**Factors in Finetuning Deep Model for object detection**

**Factors in Finetuning Deep Model for Object Detection with Long-tail Distribution**
- intro: CVPR 2016.rank 3rd for provided data and 2nd for external data on ILSVRC 2015 object detection
- project page: [http://www.ee.cuhk.edu.hk/~wlouyang/projects/ImageNetFactors/CVPR16.html](http://www.ee.cuhk.edu.hk/~wlouyang/projects/ImageNetFactors/CVPR16.html)
- arxiv: [http://arxiv.org/abs/1601.05150](http://arxiv.org/abs/1601.05150)

**We don’t need no bounding-boxes: Training object class detectors using only human verification**
- arxiv: [http://arxiv.org/abs/1602.08405](http://arxiv.org/abs/1602.08405)

转自：[http://blog.csdn.net/u012905422/article/details/52664918](http://blog.csdn.net/u012905422/article/details/52664918)

为了目标检测精度和速度的进一步提高，在CVPR2016上各种算法可谓各显神通。今天要说的G-CNN就和YOLO算法类似，着重于速度的提升进行检测算子的改进。而与YOLO不同的是，改算法着眼于初始化proposal数量的减少，使得**数以万计的proposal变成极少的初始格网**，通过后面不停迭代获得最终较为准确的bbox。

## **其简要流程示意图：**

proposal技术一直是高效的目标检测算子的一个计算量的瓶颈，巨大的proposal数量使得后续检测效率降低，因此，本文受迭代优化启发，介绍了一种起始于网格的迭代算法，通过更新来使得网格box包含目标并且为目标分类。一步的回归无法处理网格向目标靠近的非线性问题，因此，本文采用分段回归模型多次迭代。
![这里写图片描述](https://img-blog.csdn.net/20160925222437194)

通过示意图可以看出，在训练阶段，首先在图像中获取叠加的多尺度的规则网格（实际网格相互叠加，示意图中显示平铺以视觉化），然后通过ground truth与每一个网格的IoU进行每一个网格ground truth的分配，并完成训练过程，使得网格在回归过程中渐渐接近ground truth。在检测阶段，对于每一个box针对每一类获得置信分数，用最可能类别的回归器来更新box的位置。

## **网络结构：**

![这里写图片描述](https://img-blog.csdn.net/20160925223430242)

从网络结构可以看出，G-CNN主要定义检测问题：**所有可能bbox的迭代搜索问题**。因此，其目标是训练网络使得网格最后可以将初始的网格向着目标ground truth移动。这和Faster R-CNN有较大区别。

## **损失函数：**

G-CNN是一个迭代方法，它将bounding box在尽可能少的步骤下朝着目标位置移动。所以，其loss函数包含：（1）每个训练样本训练时的loss，（2）每个迭代时间步iterative step的loss。

在每一步迭代中，bounding box将要移动的目标定义函数如下：

![这里写图片描述](https://img-blog.csdn.net/20160926085343045)

其中：*Strain* 第几次迭代 ![这里写图片描述](https://img-blog.csdn.net/20160926085357505)
![这里写图片描述](https://img-blog.csdn.net/20160926084917429)

其整体loss函数如下：
![这里写图片描述](https://img-blog.csdn.net/20160925224605787)

这里*Bi* 代表第i步迭代时候的bounding box位置，*Bbg* 代表所有的背景bounding box（负样本）。

因此可以看出，整体loss函数主要分为2大块：**（1）候选bounding box是否不属于负样本的指示函数，（2）候选bbox与ground truth之间的loss。**前部分主要判断该bbox是否满足条件，后部分则计算bbox的相对变化与ground truth直接的loss，loss函数通过每一iterative step进行更新。

## **实验结果：**

文章展示大量优秀和不好的实验结果，这里仅仅展示优秀结果，可以看出，一步步下bbox向ground truth的回归还是非常成功的。
![这里写图片描述](https://img-blog.csdn.net/20160925224713342)



