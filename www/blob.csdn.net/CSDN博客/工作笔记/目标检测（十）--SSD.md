# 目标检测（十）--SSD - 工作笔记 - CSDN博客





2017年09月10日 14:45:59[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：11357
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿转自：[http://blog.csdn.net/u010167269/article/details/52563573](http://blog.csdn.net/u010167269/article/details/52563573)

论文：SSD single shot multibox detector

论文链接：[https://arxiv.org/abs/1512.02325](https://arxiv.org/abs/1512.02325)

## Abstract

这篇文章在既保证速度，又要保证精度的情况下，提出了 SSD 物体检测模型，与现在流行的检测模型一样，将检测过程整个成一个 single deep neural network。便于训练与优化，同时提高检测速度。

SSD 将输出一系列 离散化（discretization） 的 bounding boxes，这些 bounding boxes 是在不同层次（layers） 上的 feature maps 上生成的，并且有着不同的aspect ratio。

**在 prediction 阶段**：
- 
要计算出每一个 default box 中的物体，其属于每个类别的可能性，即 score，得分。如对于[PASCAL VOC](http://host.robots.ox.ac.uk/pascal/VOC/) 数据集，总共有 20 类，那么得出每一个 bounding box 中物体属于这 20 个类别的每一种的可能性。

- 
同时，要对这些 bounding boxes 的 shape 进行微调，以使得其符合物体的 **外接矩形**。

- 
还有就是，为了处理相同物体的不同尺寸的情况，SSD 结合了不同分辨率的 feature maps 的 predictions。


相对于那些需要 object proposals 的检测模型，本文的 SSD 方法完全取消了proposals generation、pixel resampling 或者feature resampling 这些阶段。这样使得 SSD 更容易去优化训练，也更容易地将检测模型融合进系统之中。

在 [PASCAL VOC](http://host.robots.ox.ac.uk/pascal/VOC/)、[MS COCO](http://mscoco.org/home/)、[ILSVRC](http://image-net.org/index) 数据集上的实验显示，SSD 在保证精度的同时，其速度要比用 region proposals 的方法要快很多。

SSD 相比较于其他单结构模型（YOLO），SSD 取得更高的精度，即是是在输入图像较小的情况下。如输入 300×300  大小的 PASCAL VOC 2007 test 图像，在 Titan X 上，SSD 以 58 帧的速率，同时取得了 72.1%

的 mAP。

如果输入的图像是 500×500 ，SSD 则取得了75.1% 的 mAP，比目前最 state-of-art 的 [Faster R-CNN](https://arxiv.org/abs/1506.01497) 要好很多。



## Introduction

现金流行的 state-of-art 的检测系统大致都是如下步骤，先生成一些假设的 bounding boxes，然后在这些 bounding boxes 中提取特征，之后再经过一个分类器，来判断里面是不是物体，是什么物体。

这类 pipeline 自从 [IJCV 2013, Selective Search for Object Recognition](https://ivi.fnwi.uva.nl/isis/publications/bibtexbrowser.php?key=UijlingsIJCV2013&bib=all.bib) 开始，到如今在 [PASCAL VOC](http://host.robots.ox.ac.uk/pascal/VOC/)、[MS COCO](http://mscoco.org/home/)、[ILSVRC](http://image-net.org/index) 数据集上取得领先的基于 Faster R-CNN 的[ResNet](https://arxiv.org/abs/1512.03385) 。但这类方法对于嵌入式系统，所需要的计算时间太久了，不足以实时的进行检测。当然也有很多工作是朝着实时检测迈进，但目前为止，都是牺牲检测精度来换取时间。

本文提出的实时检测方法，消除了中间的 bounding boxes、pixel or feature resampling 的过程。虽然本文不是第一篇这样做的文章（[YOLO](http://pjreddie.com/darknet/yolo/)），但是本文做了一些提升性的工作，既保证了速度，也保证了检测精度。

这里面有一句非常关键的话，基本概括了本文的核心思想：

> 
Our improvements include using a small convolutional filter to predict object categories and offsets in bounding box locations, using separate predictors (filters) for different aspect ratio detections, and applying these filters to multiple feature maps from the later stages of a network in order to perform detection at multiple scales.


本文的主要贡献总结如下：
- 
提出了新的物体检测方法：SSD，比原先最快的 [YOLO: You Only Look Once](http://pjreddie.com/darknet/yolo/) 方法，还要快，还要精确。保证速度的同时，其结果的 mAP 可与使用region proposals 技术的方法（如[Faster R-CNN](https://arxiv.org/abs/1506.01497)）相媲美。

- 
SSD 方法的核心就是 **predict**object（物体），以及其归属类别的 score（得分）；同时，在 feature map 上使用小的卷积核，去**predict** 一系列 bounding boxes 的 box offsets。

- 
本文中为了得到高精度的检测结果，在不同层次的 feature maps 上去 predict object、box offsets，同时，还得到不同aspect ratio 的 predictions。

- 
本文的这些改进设计，能够在当输入分辨率较低的图像时，保证检测的精度。同时，这个整体 end-to-end 的设计，训练也变得简单。在检测速度、检测精度之间取得较好的**trade-off**。

- 
本文提出的模型（model）在不同的数据集上，如 [PASCAL VOC](http://host.robots.ox.ac.uk/pascal/VOC/)、[MS COCO](http://mscoco.org/home/)、[ILSVRC](http://image-net.org/index)， 都进行了测试。在检测时间（timing）、检测精度（accuracy）上，均与目前物体检测领域 state-of-art 的检测方法进行了比较。


## The Single Shot Detector（SSD）

这部分详细讲解了 SSD 物体检测框架，以及 SSD 的训练方法。

这里，先弄清楚下文所说的 default box 以及 feature map cell 是什么。看下图：
- 
feature map cell 就是将 feature map 切分成8*8或者4*4之后的一个个 **格子**；

- 
而 default box 就是每一个格子上，一系列固定大小的 box，即图中虚线所形成的一系列 boxes。




![这里写图片描述](https://img-blog.csdn.net/20160918092529925)

### Model

SSD 是基于一个前向传播 CNN 网络，产生一系列 固定大小（fixed-size） 的 bounding boxes，以及每一个 box 中包含物体实例的可能性，即 score。之后，进行一个[非极大值抑制（Non-maximum suppression）](http://www.pyimagesearch.com/2014/11/17/non-maximum-suppression-object-detection-python/) 得到最终的 predictions。

SSD 模型的最开始部分，本文称作 base network，是用于图像分类的标准架构。在 base network 之后，本文添加了额外辅助的网络结构

1. Mult-scale feature map for detection

       在base network后，添加一些卷积层，这些层的大小逐渐减小，可以进行多尺度预测

2. Convolutional predictors for detection

       每一个新添加的层，可以使用一系列的卷积核进行预测。对于一个大小为m*n、p通道的特征层，使用3*3的卷积核进行预测，在某个位置上预测出一个值，该值可以是某一类别的得分，也可以是相对于default bounding boxes的偏移量，并且在图像的每个位置都将产生一个值，如图2所示。

3. Default boxes and aspect ratio

       在特征图的每个位置预测K个box。对于每个box，预测C个类别得分，以及相对于default bounding box的4个偏移值，这样需要(C+4)*k个预测器，在m*n的特征图上将产生(C+4)*k*m*n个预测值。这里，default bounding box类似于FasterRCNN中anchors，如图1所示。

这里的 default box 很类似于 [Faster R-CNN](https://arxiv.org/abs/1506.01497) 中的 [Anchor boxes](https://github.com/rbgirshick/py-faster-rcnn/issues/112)，关于这里的 Anchor boxes，详细的参见原论文。但是又不同于 Faster R-CNN 中的，本文中的 Anchor boxes 用在了不同分辨率的 feature maps 上。

![这里写图片描述](https://img-blog.csdn.net/20160918092701558)

### Training

在训练时，本文的 SSD 与那些用 region proposals + pooling 方法的区别是，SSD 训练图像中的 groundtruth 需要赋予到那些固定输出的 boxes 上。在前面也已经提到了，SSD 输出的是事先定义好的，一系列固定大小的 bounding boxes。

如下图中，狗狗的 groundtruth 是红色的 bounding boxes，但进行 label 标注的时候，要将红色的 groundtruth box 赋予 图（c）中一系列固定输出的 boxes 中的一个，即 图（c）中的红色虚线框。

![这里写图片描述](https://img-blog.csdn.net/20160918092529925)

事实上，文章中指出，像这样定义的 groundtruth boxes 不止在本文中用到。在 [YOLO](http://pjreddie.com/darknet/yolo/) 中，在 [Faster R-CNN](https://arxiv.org/abs/1506.01497) 中的 region proposal 阶段，以及在 [MultiBox](http://arxiv.org/abs/1312.2249) 中，都用到了。

当这种将训练图像中的 groundtruth 与固定输出的 boxes 对应之后，就可以 end-to-end 的进行 loss function 的计算以及 back-propagation 的计算更新了。

训练中会遇到一些问题：
- 
选择一系列 default boxes

- 
选择上文中提到的 scales 的问题

- 
hard negative mining

- 
数据增广的策略


下面会谈本文的解决这些问题的方式，分为以下下面的几个部分。 

### **Matching strategy**：

如何将 groundtruth boxes 与 default boxes 进行配对，以组成 label 呢？



将每个groundtruth box与具有最大jaccard overlap的defalult box进行匹配， 这样保证每个groundtruth都有对应的default box；并且，将每个defalut box与任意ground truth配对，只要两者的jaccard overlap大于某一阈值，本文取0.5，这样的话，一个groundtruth box可能对应多个default box。

jaccard overlap的计算:

![](https://img-blog.csdn.net/20170619094141205)![](https://img-blog.csdn.net/20170619094412027)![](https://img-blog.csdn.net/20170619094253752)

### **Training objective**：





SSD 训练的目标函数（training objective）源自于 [MultiBox](http://arxiv.org/abs/1312.2249) 的目标函数，但是本文将其拓展，使其可以处理多个目标类别。用 Xpij表示 第 i个 default box 与 类别p的 第j个 ground truth box 相匹配，否则若不匹配的话，则Xpij 0 = 0 。

根据上面的匹配策略，一定有∑Xpij≥1意味着对于第j个 ground truth box，有可能有多个 default box 与其相匹配。



Let ![](https://img-blog.csdn.net/20170619101004297)be an indicator for matching the i-th default box to the j-th ground truth box of category p 。

损失函数的计算类似于Fast RCNN中的损失函数，总的损失函数是localization loss (loc) 和 confidence loss (conf) 的加权和，如下:

![](https://img-blog.csdn.net/20170619101051874)

confidence loss:

![](https://img-blog.csdn.net/20170619101134408)

localization loss (loc) :

![](https://img-blog.csdn.net/20170619101232175)

其中，（gcx, gcy, gw, gh）表示groundtruth box，（dcx, dcy, dw, dh）表示default box，（lcx, lcy, lw, lh）表示预测的box相对于default box的偏移量。

其中：
- 
N是与 ground truth box 相匹配的 default boxes 个数

- 
localization loss（loc） 是 [Fast R-CNN 中 Smooth L1 Loss](http://arxiv.org/abs/1504.08083)，用在 predict box（l ） 与 ground truth box（g ） 参数（即中心坐标位置，width、height）中，回归 bounding boxes 的中心位置，以及 width、height

- 
confidence loss（conf） 是 Softmax Loss，输入为每一类的置信度c 

- 
权重项 α ，设置为 1


### **Choosing scales and aspect ratios for default boxes**：

大部分 CNN 网络在越深的层，feature map 的尺寸（size）会越来越小。这样做不仅仅是为了减少计算与内存的需求，还有个好处就是，最后提取的 feature map 就会有某种程度上的平移与尺度不变性。

同时为了处理不同尺度的物体，一些文章，如 [ICLR 2014, Overfeat: Integrated recognition, localization and detection using convolutional networks](https://arxiv.org/abs/1312.6229)，还有[ECCV 2014, Spatial pyramid pooling in deep convolutional networks for visual recognition](https://arxiv.org/abs/1406.4729)，他们将图像转换成不同的尺度，将这些图像独立的通过 CNN 网络处理，再将这些不同尺度的图像结果进行综合。

但是其实，如果使用同一个网络中的、不同层上的 feature maps，也可以达到相同的效果，同时在所有物体尺度中共享参数。

之前的工作，如 [CVPR 2015, Fully convolutional networks for semantic segmentation](https://people.eecs.berkeley.edu/~jonlong/long_shelhamer_fcn.pdf)，还有 [CVPR 2015, Hypercolumns for object segmentation and fine-grained localization](https://people.eecs.berkeley.edu/~rbg/papers/cvpr15/hypercolumn.pdf) 就用了 CNN 前面的 layers，来提高图像分割的效果，因为越底层的 layers，保留的图像细节越多。文章[ICLR 2016, ParseNet: Looking wider to see better](http://www.cs.unc.edu/~wliu/papers/parsenet.pdf) 也证明了以上的想法是可行的。

因此，本文同时使用 **lower** feature maps、**upper** feature maps 来 predict detections。下图展示了本文中使用的两种不同尺度的 feature map,8*8的feature map，以及4*4的 feature map：

![这里写图片描述](https://img-blog.csdn.net/20160918092529925)

一般来说，一个 CNN 网络中不同的 layers 有着不同尺寸的 感受野（receptive fields）。这里的感受野，指的是输出的 feature map 上的一个节点，其对应输入图像上尺寸的大小。具体的感受野的计算，参见两篇 blog：
- [http://blog.csdn.net/kuaitoukid/article/details/46829355](http://blog.csdn.net/kuaitoukid/article/details/46829355)
- [http://blog.cvmarcher.com/posts/2015/05/17/cnn-trick/](http://blog.cvmarcher.com/posts/2015/05/17/cnn-trick/)





为了处理不同尺度的物体，一些文章，如：Overfeat，处理不同大小的图像，然后将结果综合。实际上，采用同一个网络，不同层上的feature map，也能达到同样的效果。图像分割算法FCN表明，采用低层的特征图可以提高分割效果，因为低层保留的图像细节信息比较多。因此，该论文采用lower feature map、upper feature map进行预测。

     一般来说，CNN的不同层有着不同的感受野。然而，在SSD结构中，default box不需要和每一层的感受野相对应，特定的特征图负责处理图像中特定尺度的物体。在每个特征图上，default box的尺度计算如下：

![](https://img-blog.csdn.net/20170619110259949)

      其中，smin = 0.2，smax = 0.9



      default box的aspect ratios 有：{1， 2， 3，1/2，1/3}，对于 aspect ratio = 1，额外增加一个default box，该box的尺度为![](https://img-blog.csdn.net/20170619110846061) 。

      每一个default box，宽度、高度、中心点计算如下：

![](https://img-blog.csdn.net/20170619111755325)

![](https://img-blog.csdn.net/20170619111836098)

![](https://img-blog.csdn.net/20170619112052068)

在结合 feature maps 上，所有 不同尺度、不同 aspect ratios 的 default boxes，它们预测的 predictions 之后。可以想见，我们有许多个 predictions，包含了物体的不同尺寸、形状。如下图，狗狗的 ground truth box 与4*4feature map 中的红色 box 吻合，所以其余的 boxes 都看作负样本。

![这里写图片描述](https://img-blog.csdn.net/20160918092529925)











### **Hard negative mining**

在生成一系列的 predictions 之后，会产生很多个符合 ground truth box 的 predictions boxes，但同时，不符合 ground truth boxes 也很多，而且这个 negative boxes，远多于 positive boxes。这会造成 negative boxes、positive boxes 之间的不均衡。训练时难以收敛。

因此，本文采取，先将每一个物体位置上对应 predictions（default boxes）是 negative 的 boxes 进行排序，按照 default boxes 的 confidence 的大小。 选择最高的几个，保证最后 negatives、positives 的比例在3：1。

本文通过实验发现，这样的比例可以更快的优化，训练也更稳定。


### **Data augmentation**

本文同时对训练数据做了 data augmentation，数据增广。关于数据增广，推荐一篇文章：[Must Know Tips/Tricks in Deep Neural Networks](http://210.28.132.67/weixs/project/CNNTricks/CNNTricks.html)，其中的 section 1 就讲了 data augmentation 技术。

每一张训练图像，随机的进行如下几种选择：
- 使用原始的图像
- 采样一个 patch，与物体之间最小的 jaccard overlap 为：0.1，0.3，0.5，0.7，0.9 
- 随机的采样一个 patch

采样的 patch 是原始图像大小比例[0.1,1]，aspect ratio 在1/2与 2之间。

当 groundtruth box 的 中心（center）在采样的 patch 中时，我们保留重叠部分。

在这些采样步骤之后，每一个采样的 patch 被 **resize** 到固定的大小，并且以0.5的概率随机的 水平翻转（horizontally flipped）



## Experimental Results

### Base network and hole filling algorithm

本文的 Base network 是基于 [ICLR 2015, VGG16](http://arxiv.org/abs/1409.1556v6) 来做的，在 [ILSVRC CLS-LOC](http://link.springer.com/article/10.1007/s11263-015-0816-y) 数据集上进行了预训练。

与 [ICLR 2015, DeepLab-LargeFOV](http://arxiv.org/pdf/1412.7062.pdf) 的工作类似，本文将 VGG 中的 FC6 layer、FC7 layer 转成为 卷积层，并从模型的 FC6、FC7 上的参数，进行采样得到这两个卷积层的 parameters。

还将 Pool5 layer 的参数，从 2×2−s2  转变成 3×3−s1 ，外加一个 pad（1），如下图：

![这里写图片描述](https://img-blog.csdn.net/20160922170005394)



但是这样变化后，会改变感受野（receptive field）的大小。因此，采用了 atrous algorithm 的技术，这里所谓的atrous algorithm，我查阅了资料，就是hole filling algorithm。



在 DeepLab 的主页上：[http://liangchiehchen.com/projects/DeepLab.html](http://liangchiehchen.com/projects/DeepLab.html)，有一张如下的图：

![这里写图片描述](https://img-blog.csdn.net/20160920111501195)



博客 1：[http://www.cnblogs.com/jianyingzhou/p/5386222.html](http://www.cnblogs.com/jianyingzhou/p/5386222.html)

> 
最早用的就是 deeplab 的文章了，Semantic Image Segmentation with Deep Convolutional Nets and Fully Connected CRFS 这篇文章和 fcn 不同的是，在最后产生 score map 时，不是进行upsampling，而是采用了 hole algorithm，就是在 pool4 和 pool 5层，步长由 2 变成 1，必然输出的 score map 变大了，但是 receptive field 也变小了，为了不降低 receptive field，怎么做呢？利用 hole algorithm，将卷积 weights 膨胀扩大，即原来卷积核是 3x3，膨胀后，可能变成 7x7 了，这样 receptive field 变大了，而 score map 也很大，即输出变成 dense 的了。

这么做的好处是，输出的 score map 变大了，即是 dense 的输出了，而且 receptive field 不会变小，而且可以变大。这对做分割、检测等工作非常重要。


博客 2：[http://blog.csdn.net/tangwei2014/article/details/50453334](http://blog.csdn.net/tangwei2014/article/details/50453334)

> 
既想利用已经训练好的模型进行 fine-tuning，又想改变网络结构得到更加 dense 的 score map.

这个解决办法就是采用 Hole 算法。如下图 (a) (b) 所示，在以往的卷积或者 pooling 中，一个 filter 中相邻的权重作用在 feature map 上的位置都是物理上连续的。如下图 (c) 所示，为了保证感受野不发生变化，某一层的 stride 由 2 变为 1 以后，后面的层需要采用 hole 算法，具体来讲就是将连续的连接关系是根据 hole size 大小变成 skip 连接的（图 (c) 为了显示方便直接画在本层上了）。不要被 (c) 中的 padding 为 2 吓着了，其实 2 个 padding 不会同时和一个 filter 相连。

pool4 的 stride 由 2 变为 1，则紧接着的 conv5_1, conv5_2 和 conv5_3 中 hole size 为 2。接着 pool5 由 2 变为 1 , 则后面的 fc6 中 hole size 为 4。

![这里写图片描述](https://img-blog.csdn.net/20160920112301985)




本文还将 [fully convolutional reduced (atrous) VGGNet](https://gist.github.com/weiliu89/2ed6e13bfd5b57cf81d6) 中的所有的 **dropout layers、fc8 layer** 移除掉了。

本文在 [fine-tuning 预训练的 VGG model](http://nbviewer.jupyter.org/github/BVLC/caffe/blob/master/examples/02-fine-tuning.ipynb) 时，初始 learning rate 为 10 −3  ，momentum 为 0.9 ，weight decay 为 0.0005 ，batch size 为 32 ，learning rate decay 的策略随数据集的不同而变化。



### PASCAL VOC 2007

在这个数据集中，与 [Fast R-CNN](http://arxiv.org/abs/1504.08083)、[Faster R-CNN](https://arxiv.org/abs/1506.01497) 进行了比较，几种检测网络都用相同的训练数据集，以及预训练模型（VGG16）。

本文训练图像是 [VOC 2007 train](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/index.html) + [VOC 2007 validation](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/index.html) + [VOC 2012 train](http://host.robots.ox.ac.uk/pascal/VOC/voc2012/index.html) + [VOC 2012 validation](http://host.robots.ox.ac.uk/pascal/VOC/voc2012/index.html)，共计 16551 张图像；

测试集选取的是 VOC 2007 test，共计 4952 张图像。

下图展示了 SSD300 model 的结构：

![这里写图片描述](https://img-blog.csdn.net/20160920174630775)



我们用 conv4_3，conv7（原先的 FC7），conv8_2，conv9_2，conv10_2，以及pool11，这些 layer 来 predict location、 confidence。

在 VGG16 上新加的 convolutional layers，其参数初始化都用 [JMLR 2010, Understanding the difficulty of training deep feedforward neural networks](http://jmlr.org/proceedings/papers/v9/glorot10a/glorot10a.pdf) 提出的xavier 方法。

因为 conv4_3 的尺寸比较大，size 为 38×38  的大小，我们只在上面放置 3 个 default boxes，一个 box 的 scale 为 0.1 ，另外两个 boxes 的 aspect ratio 分别为 12  、2 。但对于其他的用来做 predictions 的 layers，本文都放了 6  个 default boxes。

文献 [ICLR 2016, ParseNet: Looking wider to see better](http://www.cs.unc.edu/~wliu/papers/parsenet.pdf) 指出，conv4_3 相比较于其他的 layers，**有着不同的 feature scale**，我们使用[ParseNet](http://www.cs.unc.edu/~wliu/papers/parsenet.pdf) 中的 L2 normalization 技术将 conv4_3 feature map 中每一个位置的 feature norm**scale** 到 20，并且在 back-propagation 中学习这个 scale。

在最开始的 40K 次迭代中，本文使用的 learning rate 是 10 −3  ，之后将其减小到10 −4  ，再接着迭代 20K 次。

下面 Table 1 显示了，我们的 SSD300 model 的精度已经超过了 Fast R-CNN，当我们用 SSD 在更大的图像尺寸上，500×500  训练得到的 model，甚至要比 Faster R-CNN 还要高出 1.9%  的 mAP。

![这里写图片描述](https://img-blog.csdn.net/20160920192958367)



为了更细节的了解本文的两个 SSD model，我们使用了 [ECCV 2012, Diagnosing error in object detectors](http://web.engr.illinois.edu/~dhoiem/projects/detectionAnalysis/) 的检测分析工具。下图 Figure 3 显示了 SSD 可以高质量的检测不同种类的物体。

![这里写图片描述](https://img-blog.csdn.net/20160920195426957)



下图 Figure 4 展示了 SSD 模型对 bounding box 的 size 非常的敏感。也就是说，SSD 对小物体目标较为敏感，在检测小物体目标上表现较差。其实这也算情理之中，因为对于小目标而言，经过多层卷积之后，就没剩多少信息了。虽然提高输入图像的 size 可以提高对小目标的检测效果，但是对于小目标检测问题，还是有很多提升空间的。

同时，积极的看，SSD 对大目标检测效果非常好。同时，因为本文使用了不同 aspect ratios 的 default boxes，SSD 对于不同 aspect ratios 的物体检测效果也很好。

![这里写图片描述](https://img-blog.csdn.net/20160920201009884)





### Model analysis

为了更好的理解 SSD，本文还使用控制变量法来验证 SSD 中的每一部分对最终结果性能的影响。测试如下表 Table 2 所示：

![这里写图片描述](https://img-blog.csdn.net/20160920202101748)



从上表可以看出一下几点：


- 
数据增广（Data augmentation）对于结果的提升非常明显

	Fast R-CNN 与 Faster R-CNN 使用原始图像，以及 0.5 的概率对原始图像进行水平翻转（horizontal flip），进行训练。如上面写的，本文还使用了额外的 sampling 策略，YOLO 中还使用了亮度扭曲（photometric distortions），但是本文中没有使用。

	做了数据增广，将 mAP 从 65.4%  提升到了 72.1% ，提升了6.7% 。

	我们还不清楚，本文的 sampling 策略会对 Fast R-CNN、Faster R-CNN 有多少好处。但是估计不会很多，因为 Fast R-CNN、Faster R-CNN 使用了feature pooling，这比人为的对数据进行增广扩充，还要更 robust。

- 
使用更多的 feature maps 对结果提升更大

	类似于 FCN，使用含图像信息更多的低 layer 来提升图像分割效果。我们也使用了 lower layer feature maps 来进行 predict bounding boxes。

	我们比较了，当 SSD 不使用 conv4_3 来 predict boxes 的结果。当不使用 conv4_3，mAP 下降到了 68.1% 。

	可以看见，低层的 feature map 蕴含更多的信息，对于图像分割、物体检测性能提升帮助很大的。

- 
使用更多的 default boxes，结果也越好

	如 Table 2 所示，SSD 中我们默认使用 6 个 default boxes（除了 conv4_3 因为大小问题使用了 3 个 default boxes）。如果将 aspect ratios 为13  、3  的 boxes 移除，performance 下降了 0.9% 。如果再进一步的，将12  、2  的 default boxes 移除，那么 performance 下降了近 2% 。

- 
Atrous 使得 SSD 又好又快

	如前面所描述，我们根据 [ICLR 2015, DeepLab-LargeFOV](http://arxiv.org/pdf/1412.7062.pdf)，使用结合 atrous algorithm 的 VGG16 版本。

	如果我们使用原始的 VGG16 版本，即保留 pool5 的参数为：2×2−s2 ，且不从 FC6，FC7 上采集 parameters，同时添加 conv5_3 来做 prediction，结果反而会下降 0.7% 。同时最关键的，速度慢了50% 。




### PASCAL VOC 2012

本文又在 VOC 2012 test 上进行的实验，比较结果如下：

![这里写图片描述](https://img-blog.csdn.net/20160920213808455)





### MS COCO

为了进一步的验证本文的 SSD 模型，我们将 SSD300、SSD500 在 MS COCO 数据集上进行训练检测。

因为 COCO 数据集中的检测目标更小，我们在所有的 layers 上，使用更小的 default boxes。

这里，还跟 [ION 检测方法](http://arxiv.org/abs/1512.04143) 进行了比较。

总的结果如下：

![这里写图片描述](https://img-blog.csdn.net/20160920214032370)





### Inference time

本文的方法一开始会生成大量的 bounding boxes，所以有必要用 Non-maximum suppression（NMS）来去除大量重复的 boxes。

通过设置 confidence 的阈值为 0.01 ，我们可以过滤掉大多数的 boxes。

之后，我们再用 [Thrust CUDA library](http://docs.nvidia.com/cuda/thrust/#axzz4Knxgge9X) 进行排序，用 GPU 版本的实现来计算剩下的 boxes 两两之间的 overlap。然后，进行 NMS，每一张图像保留 top 200 detections。这一步 SSD300 在 VOC 20 类的每张图像上，需要耗时2.2 msec 。

下面是在 PASCAL VOC 2007 test 上的速度统计：

![这里写图片描述](https://img-blog.csdn.net/20160920215210527)





## Related work and result images

这篇文章居然把相关工作总结放在最后面，我还是第一次见到。

具体的看原文吧。

最后放几张结果图：



![这里写图片描述](https://img-blog.csdn.net/20160920222622433)





![这里写图片描述](https://img-blog.csdn.net/20160920222544369)

﻿﻿



