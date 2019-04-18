# SSD 目标检测 - wsp_1138886114的博客 - CSDN博客





2018年09月30日 17:23:57[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：166








转载自：[https://blog.csdn.net/a8039974/article/details/77592395](https://blog.csdn.net/a8039974/article/details/77592395)

SSD github : https://github.com/weiliu89/caffe/tree/ssd


SSD paper : https://arxiv.org/abs/1512.02325

SSD eccv2016 slide pdf : http://download.csdn[.NET](http://lib.csdn.net/base/dotnet)/download/zy1034092330/9940054

SSD pose estimation paper : http://download.csdn[.net](http://lib.csdn.net/base/dotnet)/download/zy1034092330/9940059







![](https://img-blog.csdn.net/20170606112008443)

*图1*


缩进SSD，全称Single Shot MultiBox Detector，是Wei Liu在ECCV 2016上提出的一种目标检测[算法](http://lib.csdn.net/base/datastructure)，截至目前是主要的检测框架之一，相比Faster
 RCNN有明显的速度优势，相比YOLO又有明显的mAP优势（不过已经被CVPR 2017的YOLO9000超越）。SSD具有如下主要特点：
- 从YOLO中继承了将detection转化为regression的思路，同时一次即可完成网络训练
- 基于Faster RCNN中的anchor，提出了相似的prior box；
- 加入基于特征金字塔（Pyramidal Feature Hierarchy）的检测方式，相当于半个FPN思路

本文接下来都以SSD 300为例进行分析。




## 1 SSD网络结构

![](https://img-blog.csdn.net/20170728221519678)

*图2 SSD网络结构（和代码貌似有点差别）*

缩进上图2是原论文中的SSD 300网络结构图。可以看到YOLO在卷积层后接全连接层，即检测时只利用了最高层feature maps（包括Faster RCNN也是如此）；而SSD采用了特征金字塔结构进行检测，即检测时利用了conv4-3，conv-7（FC7），conv6-2，conv7-2，conv8_2，conv9_2这些大小不同的feature maps，在多个feature maps上同时进行softmax分类和位置回归，如图3。


![](https://img-blog.csdn.net/20170729003827513)

*图3 单层feature map预测和特征金字塔预测对比*




## 2 Prior Box

缩进在SSD中引入了Prior Box，实际上与anchor非常类似，就是一些目标的预选框，后续通过softmax分类+bounding box regression获得真实目标的位置。SSD按照如下规则生成prior box：
- 以feature map上每个点的中点为中心（offset=0.5），生成一些列同心的prior box（然后中心点的坐标会乘以step，相当于从feature map位置映射回原图位置）
- 正方形prior box最小边长为![](https://img-blog.csdn.net/20170804114039980)，最大边长为：![](https://img-blog.csdn.net/20170731153321449)- 每在prototxt设置一个aspect ratio，会生成2个长方形，长宽为：![](https://img-blog.csdn.net/20170731153333081) 和 ![](https://img-blog.csdn.net/20170731153336512)
![](https://img-blog.csdn.net/20170810155820993)

*图4 prior box*
- 而每个feature map对应prior box的min_size和max_size由以下公式决定，公式中m是使用feature map的数量（SSD 300中m=6）：

![](https://img-blog.csdn.net/20170804114433275)

第一层feature map对应的min_size=S1，max_size=S2；第二层min_size=S2，max_size=S3；其他类推。在原文中，Smin=0.2，Smax=0.9，但是在SSD
 300中prior box设置并不能和paper中上述公式对应：
||min_size|max_size| | | | | | | |
|----|----|----|----|----|----|----|----|----|----|
|30|60| | | | | | | | |
|60|111|111|162|162|213|213|264|264|315|
|111|162|162|213|213|264|264|315| | |
|162|213|213|264|264|315| | | | |
|213|264|264|315| | | | | | |
|264|315| | | | | | | | |

不过依然可以看出，SSD使用低层feature map检测小目标，使用高层feature map检测大目标，这也应该是SSD的突出贡献了。其中SSD 300在conv4_3生成prior box的conv4_3_norm_priorbox层prototxt定义如下：





**[cpp]**[view plain](http://blog.csdn.net/zy1034092330/article/details/72862030#)[copy](http://blog.csdn.net/zy1034092330/article/details/72862030#)





- layer {  
-   name: "conv4_3_norm_mbox_priorbox"
-   type: "PriorBox"
-   bottom: "conv4_3_norm"
-   bottom: "data"
-   top: "conv4_3_norm_mbox_priorbox"
-   prior_box_param {  
-     min_size: 30.0  
-     max_size: 60.0  
-     aspect_ratio: 2  
-     flip: true
-     clip: false
-     variance: 0.1  
-     variance: 0.1  
-     variance: 0.2  
-     variance: 0.2  
-     step: 8  
-     offset: 0.5  
-   }  
- }  


知道了priorbox如何产生，接下来分析prior box如何使用。这里以conv4_3为例进行分析。

![](https://img-blog.csdn.net/20170731005334751)

*图5*

从图5可以看到，在conv4_3 feature map网络pipeline分为了3条线路：
- 经过一次batch norm+一次卷积后，生成了**[1, num_class*num_priorbox, layer_height, layer_width]**大小的feature用于softmax分类目标和非目标（其中num_class是目标类别，SSD 300中num_class = 21)
- 经过一次batch norm+一次卷积后，生成了**[1, 4*num_priorbox, layer_height, layer_width]**大小的feature用于bounding box regression（即每个点一组[dxmin，dymin，dxmax，dymax]，参考[Faster
 RCNN](http://blog.csdn.net/zy1034092330/article/details/62044941) 2.5节）
- 生成了**[1, 2, 4*num_priorbox]**大小的prior box blob，其中2个channel分别存储prior box的4个点坐标和对应的4个variance

缩进后续通过softmax分类+bounding box regression即可从priox box中预测到目标，熟悉Faster RCNN的读者应该对上述过程应该并不陌生。其实pribox box的与Faster RCNN中的anchor非常类似，都是目标的预设框，没有本质的差异。区别是每个位置的prior box一般是4~6个，少于Faster RCNN默认的9个anchor；同时prior box是设置在不同尺度的feature
 maps上的，而且大小不同。

缩进还有一个细节就是上面prototxt中的4个variance，这实际上是一种bounding regression中的权重。在图4线路(2)中，网络输出[dxmin，dymin，dxmax，dymax]，即对应下面代码中bbox；然后利用如下方法进行针对prior box的位置回归：



**[cpp]**[view plain](http://blog.csdn.net/zy1034092330/article/details/72862030#)[copy](http://blog.csdn.net/zy1034092330/article/details/72862030#)





- decode_bbox->set_xmin(  
-     prior_bbox.xmin() + prior_variance[0] * bbox.xmin() * prior_width);  
- decode_bbox->set_ymin(  
-     prior_bbox.ymin() + prior_variance[1] * bbox.ymin() * prior_height);  
- decode_bbox->set_xmax(  
-     prior_bbox.xmax() + prior_variance[2] * bbox.xmax() * prior_width);  
- decode_bbox->set_ymax(  
-     prior_bbox.ymax() + prior_variance[3] * bbox.ymax() * prior_height);  


上述代码可以在SSD box_utils.cpp的void DecodeBBox()函数见到。




## 3 Permute，Flatten And Concat Layers

![](https://img-blog.csdn.net/20170802233849955)

*图6*

缩进上一节以conv4_3 feature map分析了如何检测到目标的真实位置，但是SSD 300是使用包括conv4_3在内的共计6个feature maps一同检测出最终目标的。在网络运行的时候显然不能像图6一样：一个feature map单独计算一次softmax socre+box regression（虽然原理如此，但是不能如此实现）。那么多个feature maps如何协同工作？这时候就要用到Permute，Flatten和Concat这3种层了。其中conv4_3_norm_conf_perm的prototxt定义如下：

Permute是SSD中自带的层，上面conv4_3_norm_mbox_conf_perm的的定义。Permute相当于交换caffe blob中的数据维度。在正常情况下caffe blob的顺序为：

bottom blob = [batch_num, channel, height, width]

经过conv4_3_norm_mbox_conf_perm后的caffe blob为：

top blob = [batch_num, height, width, channel]

而Flattlen和Concat层都是caffe自带层，请参照[caffe official documentation](http://caffe.berkeleyvision.org/tutorial/layers.html)理解。




![](https://img-blog.csdn.net/20170815014625009)

*图7 SSD中部分层caffe blob shape变化*




缩进那么接下来以conv4_3和fc7为例分析SSD是如何将不同size的feature map组合在一起进行prediction。图7展示了conv4_3和fc7合并在一起的过程中caffe blob shape变化（其他层类似，考虑到图片大小没有画出来，请脑补）。
- 对于conv4_3 feature map，conv4_3_norm_priorbox（priorbox层）设置了每个点共有4个prior box。由于SSD 300共有21个分类，所以conv4_3_norm_mbox_conf的channel值为num_priorbox * num_class = 4 * 21 = 84；而每个prior box都要回归出4个位置变换量，所以conv4_3_norm_mbox_loc的caffe blob channel值为4 * 4 = 16。
- fc7每个点有6个prior box，其他feature map同理。
- 经过一系列图7展示的caffe blob shape变化后，最后拼接成mbox_conf和mbox_loc。而mbox_conf后接reshape，再进行softmax（为何在softmax前进行reshape，Faster RCNN有提及）。
- 最后这些值输出detection_out_layer，获得检测结果

## 4 SSD网络结构优劣分析

缩进SSD算法的优点应该很明显：运行速度可以和YOLO媲美，检测精度可以和Faster RCNN媲美。除此之外，还有一些鸡毛蒜皮的优点，不解释了。这里谈谈缺点：
- 需要人工设置prior box的min_size，max_size和aspect_ratio值。网络中prior box的基础大小和形状不能直接通过学习获得，而是需要手工设置。而网络中每一层feature使用的prior box大小和形状恰好都不一样，导致调试过程非常依赖经验。
- 虽然采用了pyramdial feature hierarchy的思路，但是对小目标的recall依然一般，并没有达到碾压Faster RCNN的级别。作者认为，这是由于SSD使用conv4_3低级feature去检测小目标，而低级特征卷积层数少，存在特征提取不充分的问题。




## 5 SSD训练过程

![](https://img-blog.csdn.net/20170803124105824)

缩进对于SSD，虽然paper中指出采用了所谓的“multibox loss”，但是依然可以清晰看到SSD loss分为了confidence loss和location loss两部分，其中N是match到GT（Ground Truth）的prior box数量；而α参数用于调整confidence loss和location loss之间的比例，默认α=1。SSD中的confidence loss是典型的softmax loss：

![](https://img-blog.csdn.net/20170803124309620)

其中![](https://img-blog.csdn.net/20170804105238757)代表第i个prior box匹配到了第j个class为p类别的GT box；而location loss是典型的smooth L1 loss：

![](https://img-blog.csdn.net/20170803124111958)

**Matching strategy：**

缩进在训练时，groundtruth boxes 与 default boxes（就是prior boxes） 按照如下方式进行配对：

- 首先，寻找与每一个ground truth box有最大的jaccard overlap的default box，这样就能保证每一个groundtruth box与唯一的一个default box对应起来（所谓的jaccard overlap就是IoU，如图8）。
- SSD之后又将剩余还没有配对的default box与任意一个groundtruth box尝试配对，只要两者之间的jaccard overlap大于阈值，就认为match（SSD 300 阈值为0.5）。
- 显然配对到GT的default box就是positive，没有配对到GT的default box就是negative。


![](https://img-blog.csdn.net/20170803000716903)![](https://img-blog.csdn.net/20170803000646177)
![](https://img-blog.csdn.net/20170803000600719)

*图8 jaccard overlap*

**Hard negative mining：**

缩进值得注意的是，一般情况下negative default boxes数量>>positive default boxes数量，直接训练会导致网络过于重视负样本，从而loss不稳定。所以需要采取：
- 所以SSD在训练时会依据confidience score排序default box，挑选其中confidience高的box进行训练，控制positive：negative=1：3

**Data augmentation：**

缩进数据增广，即每一张训练图像，随机的进行如下几种选择：

- 使用原始的图像
- 采样一个 patch，与物体之间最小的 jaccard overlap 为：0.1，0.3，0.5，0.7 或 0.9
- 随机的采样一个 patch

采样的 patch 是原始图像大小比例是[0.1，1]，aspect ratio在1/2与2之间。当 groundtruth box 的 中心（center）在采样的patch中时，保留重叠部分。在这些采样步骤之后，每一个采样的patch被resize到固定的大小，并且以0.5的概率随机的 水平翻转（horizontally flipped）。




其实Matching strategy，Hard negative mining，Data augmentation，都是为了加快网络收敛而设计的。尤其是Data augmentation，翻来覆去的randomly crop，保证每一个prior box都获得充分训练而已。不过当数据达到一定量的时候，不建议再进行Data augmentation，毕竟“真”的数据比“假”数据还是要好很多。











