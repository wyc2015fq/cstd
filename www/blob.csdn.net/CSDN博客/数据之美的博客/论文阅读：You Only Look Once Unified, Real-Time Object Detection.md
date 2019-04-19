# 论文阅读：You Only Look Once: Unified, Real-Time Object Detection - 数据之美的博客 - CSDN博客
2017年04月05日 14:37:19[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：771
个人分类：[通用物体检测																[深度学习/机器学习](https://blog.csdn.net/oppo62258801/article/category/6739087)](https://blog.csdn.net/oppo62258801/article/category/6840016)
# Preface
今天详细的看一下 CVPR 2016 年这篇：[You Only Look Once: Unified, Real-Time
 Object Detection](https://arxiv.org/abs/1506.02640)。另外，这篇的作者也有[Ross B. Girshick](https://people.eecs.berkeley.edu/~rbg/)。
这篇 Paper 的项目主页在这里：[http://pjreddie.com/darknet/yolo/](http://pjreddie.com/darknet/yolo/)
注：这篇今年 CVPR 2016 年的检测文章 YOLO，我之前写过这篇文章的解读。但因为不小心在 Markdown 编辑器中编辑时删除了。幸好同组的伙伴转载了我的，我就直接考过来了，重新发一下。以后得给自己的博文留个备份。
# abstract
这篇文章提出了一个新的物体检测的方法：You Only Look Once（YOLO）。
之前的物体检测方法通常都转变为了一个分类问题，如 R-CNN、Fast R-CNN 等等。另外，关于对 R-CNN、Fast R-CNN、Faster R-CNN 这一系列方法，知乎上有个特别好的帖子：[如何评价rcnn、fast-rcnn和faster-rcnn这一系列方法？](https://www.zhihu.com/question/35887527)
而这篇文章将检测变为一个 regression problem，YOLO 从输入的图像，仅仅经过一个 neural network，直接得到 bounding boxes 以及每个 bounding box 所属类别的概率。正因为整个的检测过程仅仅有一个网络，所以它可以直接
 end-to-end 的优化。
YOLO 结构十分的快，标准的 YOLO 版本每秒可以实时地处理 45 帧图像。一个较小版本：Fast YOLO，可以每秒处理 155 帧图像，它的 mAP（mean Average Precision） 依然可以达到其他实时检测[算法](http://lib.csdn.net/base/datastructure)的两倍。
同时相比较于其他的 state-of-art detection systems。尽管 YOLO 的定位更容易出错，这里的 定位出错，即是指 coordinate errors。
但是 YOLO 有更少的 false-positive，文章这里提到了一个词：background errors，背景误差。这里所谓的 背景误差 即是指 False
 Positive。在这篇 Paper 的[Assigned Reviewer](http://pjreddie.com/publications/yolo/)里，有 Reviewer 提到了这个问题：
> 
On overall, the paper reads well, even if some terms such as IOU (I guess it’s the abbreviation of intersection over union but it would be better to say it as it’s not a standard abbreviation)
 or “background errors” (I’m not really sure of the meaning of this expression. Are they False Positive? If yes, it should be better to use False Positive instead).
![这里写图片描述](https://img-blog.csdn.net/20160908093753784)
最后，YOLO 可以学习到物体的更加泛化的特征，在将 YOLO 用到其他领域的图像时（如 artwork 的图像上），其检测效果要优于 [DPM](http://cs.brown.edu/~pff/latent-release4/)、[R-CNN](https://github.com/rbgirshick/rcnn) 这类方法。
# Introduction
现在的 detection systems 将物体检测问题，最后会转变成一个分类问题。在检测中，detection systems 采用一个 classifier 去评估一张图像中，各个位置一定区域的 window 或 bounding box 内，是否包含一个物体？包含了哪种物体？
一些 detection systems，如 [Deformable Parts Models（DPM）](http://cs.brown.edu/~pff/latent-release4/)，采用的是 sliding
 window 的方式去检测。
最近的 [R-CNN](https://github.com/rbgirshick/rcnn)、[Fast
 R-CNN](https://github.com/rbgirshick/fast-rcnn) 则采用的是 region proposals 的方法，先生成一些可能包含待检测物体的 potential bounding box，再通过一个 classifier 去判断每个 bounding box 里是否包含有物体，以及物体所属类别的 probability 或者 confidence。这种方法的 pipeline 需要经过好几个独立的部分，所以检测速度很慢，也难以去优化，因为每个独立的部分都需要单独训练。
本文将 object detection 的框架设计为一个 regression problem。直接从图像像素到 bounding box 以及 probabilities。这个 YOLO 系统如图看了一眼图像就能 predict 是否存在物体，他们在哪个位置，所以也才叫
 You Only Look Once。
YOLO 的 idea 十分简单，如 Figure 1： 
将图像输入单独的一个 CNN 网络，就会 predict 出 bounding boxes，以及这些 bounding boxes 所属类别的概率。YOLO 用一整幅图像来训练，同时可以直接优化 detection performance。
![这里写图片描述](https://img-blog.csdn.net/20160907191215388)
这样的统一的架构，对比之前如 [R-CNN](https://github.com/rbgirshick/rcnn)、[Fast R-CNN](https://github.com/rbgirshick/fast-rcnn) 的
 pipeline，有以下几点好处：
（1）YOLO 检测系统非常非常的快。受益于将 detection [架构](http://lib.csdn.net/base/architecture)设计成一个 regression
 problem，以及简单的 pipeline。在 Titan X 上，不需要经过批处理，标准版本的 YOLO 系统可以每秒处理 45 张图像；YOLO 的极速版本可以处理 150 帧图像。这就意味着 YOLO 可以以小于 25 毫秒延迟的处理速度，实时地处理视频。同时，YOLO 实时检测的mean Average Precision（mAP） 是其他实时检测系统的两倍。
（2）YOLO 在做 predict 的时候，YOLO 使用的是全局图像。与 sliding window 和 region proposals 这类方法不同，YOLO 一次“看”一整张图像，所以它可以将物体的整体（contextual）的 class
 information 以及 appearance information 进行 encoding。目前最快最好的[Fast R-CNN](https://github.com/rbgirshick/fast-rcnn) ，较容易误将图像中的
 background patches 看成是物体，因为它看的范围比较小。YOLO 的 background errors 比 Fast R-CNN 少一半多。
（3）YOLO 学到物体更泛化的特征表示。当在自然场景图像上训练 YOLO，再在 artwork 图像上去[测试](http://lib.csdn.net/base/softwaretest) YOLO
 时，YOLO 的表现甩 DPM、R-CNN 好几条街。YOLO 模型更能适应新的 domain。
# Unified Detection
YOLO 检测系统，先将输入图像分成  个
 grid（栅格），如果一个物体的中心掉落在一个 grid cell 内，那么这个 grid cell 就负责检测这个物体。
每一个 grid cell 预测  个
 bounding boxes，以及这些 bounding boxes 的得分：score。这个 score 反应了模型对于这个 grid cell 中预测是否含有物体，以及是这个物体的可能性是多少。正式的公式：。如果这个
 cell 中不存在一个 object，则 score 应该为 ；否则的话，score
 则为 predicted box 与 ground truth 之间的 IoU（intersection over union）。
本文中的每一个 bounding box 包含了  个
 predictions：，坐标 代表了
 bounding box 的中心与 grid cell 边界的相对值。width、height 则是相对于整幅图像的预测值。confidence 就是 IoU 值。
每一个 grid cell 还要预测  个
 conditional class probability（条件类别概率）：。这个 基于
 gird cell 包含了哪个物体（所以为 conditional probabilities）。不管 grid cell 中包含的 boxes 有多少 ，每个
 grid cell 只 predict 每个类别的 conditional probabilities。
在测试阶段，将每个 grid cell 的 conditional class probabilities 与每个 bounding box 的 confidence 相乘：
上面得到每个 bounding box 的具体类别的 confidence score。这样就把 bounding box 中预测的 class 的 probability，以及 bounding box 与 object 契合的有多好，都进行了 encoding。
将 YOLO 用于 PASCAL VOC 数据集时：
- 本文使用的 ，即将一张图像分为 个
 grid cells
- 每一个 grid cell 预测  个
 boxes（每个 box 是 ，个数值）
- 同时，PASCAL 数据集中有  个类别，则，上面的 
因此，最后的 prediction 是  的 tensor
![这里写图片描述](https://img-blog.csdn.net/20160907185937043)
## Network Design
YOLO 仍是 CNN 的经典形式，开始是 convolutional layers 提取特征，再是 fully connected layers 进行 predict 结果：probabilities 以及 coordinates。
YOLO 的 CNN 结构取自两篇论文：[GoogLeNet](https://arxiv.org/abs/1409.4842)、[Network
 in Network](https://arxiv.org/abs/1312.4400). YOLO 有 个卷积层，随后就是全连接层。不像 [GoogLeNet](https://arxiv.org/abs/1409.4842) 中使用的 inception
 modules，YOLO 采用了 [Network in Network](https://arxiv.org/abs/1312.4400) 中的结构，在 卷积层之后，跟着一个  的层。如下图
 Figure 3 所示： 
![这里写图片描述](https://img-blog.csdn.net/20160907190145967)
网络结构，更详细的如下表所示：
|Layers|Parameters|
|----|----|
|Input Data|Images Size:|
|Convolution|num_filters: , kernel size: , stride:|
|Max Pooling|kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Max Pooling|kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Max Pooling|kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Max Pooling|kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Convolution|num_filters: , kernel size: , stride:|
|Fully Connected Layer|Input Number: , Output Number:|
|Fully Connected Layer|Input Number: , Output Number:|
这个 YOLO 先在 ImageNet classification 任务上进行预训练（以一半的图像尺寸：），然后再将图像尺寸变为，用于
 detection 任务。
之前说过，文本还有一个 Fast YOLO 版本。Fast YOLO 的网络结构中，只有  个卷积层，filters
 也更少。
最后输出的 Tensor 为 ， 对应了
 49 个 grid cells，30 对应了预测值。其中，8 维是回归的 box 坐标，2 维是 bounding box 的 confidence，最后还有 20 维是类别。
## Training
YOLO 在 [ImageNet 1000-class 的分类任务数据集](http://www.image-net.org/challenges/LSVRC/) 上进行 pretraining。预训练的网络是
 Figure 3 中网络的前 20 层卷积层，加上一个 average-pooling layer，最后是一个 fully connected layer。
这个预训练的网络，本文训练了大约一个星期，在 ImageNet 2012 的 validation dataset 上的 top-5 精度为，本文的
 training 以及 inference 都是用 [Darknet](http://pjreddie.com/darknet/) 卷积网络框架完成的。
之后就把预训练的 model 用于 detection，[Ren et al., Object Detection Networks
 on Convolutional Feature Maps](https://arxiv.org/abs/1504.06066) 指出了在预训练的 model 上增加 convolution layer 以及 connected layer 可以增加 model 的 performance。因此，在预训练的 model 上，本文增加了  个
 convolutional layers 以及  个
 connected layers，这新加的层的参数是随机初始化的。因为要用于 detection，本文在 detection 的 fine-grained 阶段，将图像的输入分辨率从 调整至 。
最后输出的为 class probabilities 以及 bounding box coordinates。但在输出时，根据图像的 width、height 将 bounding box 的 width、height 进行归一化，将值归一化到 0~1 的区间。同样将 bounding box 中的坐标 通过
 grid cells 的 offset 归一化到 0~1 之间。
模型的最后一层，本文使用一个线性激活函数，其余的层则使用的是 leaky rectified linear activation：
本文使用的是 sum-squared error 来作为优化目标。虽然 sum-squared error 较容易去优化，但是对于本问题，直接使用 sum-squared
 error 却不是那么合适。
因为将 localization error 以及 classification error 在优化求解中，相等的去衡量优化，本身就不合理。另外，每一张图像中，很多 grid cells 并没不包含物体，使得这些
 cells 的 confidence 置为 0，这些不包含物体的 grid cells 的梯度更新，将会以压倒性的优势，覆盖掉包含物体的 grid cells 进行的梯度更新。这些问题会使得模型不稳定，甚至造成网络的发散。
为了避免上述的问题，本文将 localization error 以及 classification error 的 loss 重新用权重衡量。以平衡上述的失衡问题。简单的说，就是增加 bounding
 box coordinate 的 loss，减少不包含物体 grid cells 的 confidence 的 loss。通过设置两个参数：以及 。
Sum-squared error loss 中还将 large box 与 small box 的 loss 同等对待。因为相比较于大的 box 与 groundtruth 的偏离，小的 box 的偏离一点，结果差别就很大，而大的 box 偏离大一点，对结果的影响较小。为了解决这个问题，用了一个很巧妙的
 trick，即最后并不是直接输出 bounding box 的 width、height，而是 width、height 的square root，平方根。
如下图所示，当 bounding box 的 width、height 越小时，发生偏移后，其反应在 Y 轴上的变化越大。这样就正确反映了 large box 与 small box 对于偏移的敏感性不同。
![这里写图片描述](https://img-blog.csdn.net/20160908163841162)
在 YOLO 中，每个 grid cell 预测多个 bounding box，但在网络模型的训练中，每一个物体，只想要一个 bounding box predictor 来负责预测。因此， 当前哪一个 predictor 预测的 prediction 与 groundtruth 的 IoU 最大 ，这个 predictor 就负责 predict object。这会使得每个 predictor 可以专门的负责特定的物体检测。随着训练的进行，每一个 predictor 对特定的物体尺寸，长宽比，物体的类别的预测会越来越好。
训练时，需要优化的目标函数（loss function）如下： 
![](https://img-blog.csdn.net/20160923193338096)
其中：
-  表示
 object 是否出现在 grid cell  中。
-  表示
 grid cell  中的第  个负责
 prediction 的 predictor。
注意：
- 
上式中 loss function 在只有当一个 grid cell 中存在 object 时，才会对 classification error 进行惩罚。
- 
上式中 loss function 也只有在当 box predictor 对 groundtruth box 负责的时候，才会对 bounding box coordinate error 进行惩罚。
训练中，总共进行了 135 轮 epoches，训练、验证集来自 PASCAL 2012、2007。当在 VOC 2012 数据集上测试时，训练集包括了 VOC 2007 的测试集。训练中，bacthsize 为 64，momentum 为 0.9，decay 为 0.0005.
Learning rate 的设置： 
（1）在第一轮 epoch 中，learning rate 逐渐从  增加到 。如果训练时从一个较大的
 learning rate 开始，通常因为不稳定的梯度，而使得模型发散。
（2）之后，保持 learning rate 为  直到
 epoch = 75； 
（3）再接下的 30 轮 epoch，learning rate 为 ；
（4）最后 30 轮 epoch，learning rate 为 。
在训练中，为了避免 overfitting，使用了 [dropout](https://www.cs.toronto.edu/~hinton/absps/JMLRdropout.pdf) 技术，在第一层全连接层后面增加了一个
 dropout layer，随机置零的 。
为了防止 overfitting，也使用了 [data augmentation](http://210.28.132.67/weixs/project/CNNTricks/CNNTricks.html) 技术。
## Inference
在训练好 YOLO 网络模型后，在 PASCAL VOC 数据集上进行 inference，每一张图像得到 98 个 bounding boxes，以及每个 bounding box 的所属类别概率。
当图像中的物体较大，或者处于 grid cells 边界的物体，可能在多个 cells 中被定位出来。可以用Non-Maximal Suppression（NMS，非极大值抑制） 进行去除重复检测的物体，可以使最终的 mAP 提高，相比较于
 NMS 对于 DPM、R-CNN 的提高，不算大。
## Limitations of YOLO
因为每个 grid cell 中只能预测两个 boxes，以及有一个类别。这种太强的空间约束，限制了 YOLO 对于相邻物体的检测能力，一旦相邻的物体数量过多，YOLO 就检测不好了。如对于一群鸟儿，这种相邻数量很多，而且又太小的物体，YOLO 难以进行很好的检测。
对于图像中，同一类物体出现新的、不常见的长宽比时，YOLO 的泛化能力较弱。
最后，loss functions 中对于 small bounding boxes，以及 large bounding boxes 的误差，均等对待。尽管正如前面提到的，大尺寸 bounding box 的 error 与小尺寸 bounding box 的 error，其影响是不同的。即使用了平方根的技巧优化了这个问题，但是这个问题还得得到更好的解决。
YOLO 中最主要的误差仍是定位不准造成的误差。
