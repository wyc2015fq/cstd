# RON 算法及代码笔记 - AI之路 - CSDN博客





2018年06月10日 21:32:23[AI之路](https://me.csdn.net/u014380165)阅读数：3473








论文：RON: Reverse Connection with Objectness Prior Networks for Object Detection 

论文链接：[https://arxiv.org/abs/1707.01691](https://arxiv.org/abs/1707.01691)

代码链接：[https://github.com/taokong/RON](https://github.com/taokong/RON)
Reverse Connection with Objectness Prior Networks（RON）是**CVPR2017**的文章。这篇文章主要通过融合不同层的特征后对object的框进行预测，可以检测图像中multi-scale的object，因此在网络结构上也主要是针对这个做改进。**另外RON跟Faster RCNN有一些相似的地方，比如其中的objectness prior和Faster RCNN中的RPN网络思想类似，都是用来辅助object的分类，但并不是直接引入RPN网络，这是RON算法的速度比Faster RCNN算法的速度快3倍的一个原因，还有一个重要原因是Faster RCNN中的roi pooling层存在较多的重复计算，而RON中没有roi pooling层，对于坐标的回归直接在类似RPN网络的回归分支中完成，因此对坐标的回归操作只进行一次**。

Figure2是RON的网络结构。以主网络为VGG-16为例，假设输入图像大小是320，则conv4的输出feature map大小是40*40，conv5的输出feature map大小是20*20，conv6的输出feature map大小是10*10，conv7的输出feature map大小是5*5。当图像输入网络时先经过特征提取网络提取特征（conv4到conv7），然后在conv7层的输出后面接一个reverse connection block，对conv7而言因为是最后一层，所以只是对conv7的输出接一个卷积层（代码上是卷积核大小为2*2，数量为512的卷积层）就得到reverse fusion map（简写为rf-map），然后对conv7输出的rf-map做反卷积后和conv6的输出做加法得到conv6的rf-map，再基于conv6的rf-map和conv5的输出得到conv5的rf-map，一直到最后得到conv4的rf-map。**这4个rf-map是后续bbox的分类和回归操作的基础**。reverse connection其实在思想上和ResNet的residual connection有点类似，在ResNet的residual connection中是浅层特征连接到高层特征，而这里的reverse connection是高层特征连接到浅层特征，这也是reverse这个单词的含义。 
**RON和SSD虽然都是做特征融合，但是其中的区别还是比较大的：SSD虽然也是基于几个层的feature map做预测，但是对bbox的预测是基于每个层的feature map，每层预测得到bbox的信息后再将各层的信息整合在一起，所以比如说对于小目标的检测，bbox的尺寸较小，因此主要靠浅层的feature map上的bbox来预测，因此实际用到的feature map信息只有浅层的，高语义的feature map信息并没能帮助到小目标的检测，这也就是SSD对小目标检测效果不好的原因。再来看RON算法，在RON中是先将不同尺寸的feature map通过deconvolution的方式融合在一起，这样浅层的feature map就融合了高层的feature map信息，然后基于融合后的4个feature map层做预测，最后将这4个层预测得到的bbox整合在一起输出就得到整个网络的输出结果**。 
![这里写图片描述](https://img-blog.csdn.net/20180610212631978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
reverse connection block的结构图如Figure3所示。这个结构就实现了不同尺寸的feature map的融合，**具体过程是这样的：conv n表示第n层的输出，该输出经过一个3*3卷积层后得到的输出与n+1层输出的rf-map经过2*2的反卷积层后得到的输出做element-wise addition得到第n层的rf-map**。 
![这里写图片描述](https://img-blog.csdn.net/20180610212655826?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
关于每一个融合层中anchor（文中用reference boxes或者default boxes名字，实质上是一样的，初始化后就不变了）的初始scale和aspect ratio的设置如下，其中k表示feature map层。 
![这里写图片描述](https://img-blog.csdn.net/20180610212800546?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中smin是输入图像大小的1/10（假设输入图像大小是320*320，那么smin就是32），因此当k=1时，Sk={32,64}，当k=2时，Sk={96,128}，当k=3时，Sk={160,192}，当k=4时，Sk={224,256}。这样每一层就会初始化得到2种scale和5种aspect ratio的anchor。

**Figure1是在RON算法中的另一个内容：objectness prior**。objectness prior本质上是一个bbox包含object的概率。如何得到objectness prior呢？**实现上就是分别在4个rf-map层后先接一个卷积核数量为20的3*3卷积层，20的含义是2(bg/fg)*10(anchors)；再加上一个softmax层就得到每个bbox的2分类概率，这一部分其实和RPN网络的分类支路类似，只不过一方面这里是对4个融合特征层rf-map都执行二分类操作，另一方面这里不显式地生成和过滤proposal，这也是RON速度比Faster RCNN快的一个原因**。Figure1中的(a)(b)(c)(d)表示在4个rf-map上计算objectness prior的结果，可以看出尺寸较大的沙发会在(a)(b)两个rf-map上得到（conv7和conv5对应的rf-map），而尺寸较小的狗则在(c)(d)两个rf-map上得到（conv5和conv4对应的rf-map）。 
**因此，虽然这部分操作和RPN网络的分类支路类似，但是有一个很大的不同点是这里并不生成proposal，因此不是像Faster RCNN算法中RPN网络和检测网络有两部分损失函数，在Faster RCNN中RPN网络用来生成proposal，此时的坐标回归目标rpn_bbox_target是根据anchor坐标和ground truth生成的，而在检测部分对bbox的坐标回归时，bbox_target是根据proposal坐标和ground truth生成的。RON中的二分类损失和坐标回归以及object分类损失函数是一起回传的，不涉及proposal的生成，而是通过二分类支路不断refine预测得到的bbox是否包含object的概率，然后在object分类支路中以refine后的bbox概率为训练目标**。 
![这里写图片描述](https://img-blog.csdn.net/20180610212850462?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
RON算法中对bbox的坐标回归还是采用卷积层来实现，而且坐标回归的目标还是anchor和ground truth生成的offset。但是对bbox的类别判定（object分类支路）采用的是inception结构，这一点和其他object detection算法不大一样（Faster RCNN中用全连接层实现，SSD中用一个3*3的卷积层实现），文中采用的inception结构如Figure4所示。**object分类支路还有一个不同点在于分类的bbox是经过”精挑细选“的，这个精挑细选的过程是由bbox二分类支路实现的，最终体现在object分类支路中正负样本的比例控制在1:3，剩下都是无效样本**。 
![这里写图片描述](https://img-blog.csdn.net/20180610212909548?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
因此基于每一个融合层rf-map n的回归和分类支路如Figure5所示。回归支路部分对于每一个融合层rf-map n都接一个512个3*3卷积核的卷积层。A表示anchor数量，比如默认的A=10（包含2个scales和5个aspect ratio）。**需要注意的是这里对bbox的回归并不区分bbox的object类别，而在Faster RCNN算法中检测部分对bbox的坐标回归是对每个object类别的bbox都会得到坐标预测值（卷积核数量是num_classes*4）**。 
![这里写图片描述](https://img-blog.csdn.net/20180610213031718?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**因此总体来看，基于每个融合特征rf-map将引出3条支路：1、用于bbox是否包含object的二分类；2、用于bbox的坐标回归的支路（卷积核数量为4+A，A表示每个点的bbox数量）；3、用于bbox包含哪一类object的K+1分类（K表示object的类别，1表示背景）**。因此损失函数如公式2所示。Lobj是第一条支路的损失：objectness loss；Lloc是第二条支路的损失：bbox regression loss；Lcls/obj是第三条支路的损失：classification loss。公式中α和β默认取1/3。 
![这里写图片描述](https://img-blog.csdn.net/20180610213051300?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
RON算法中对正负样本的定义和Faster RCNN一样。正样本包含两种情况：1、某个ground truth和所有bbox的IOU最大的那个bbox；2、和任意一个ground truth的IOU大于0.5的bbox。负样本则是和所有ground truth的IOU小于0.3的bbox。另外当输入图像大小为320*320时，batch size取18。

**实验结果：**

Table1是在VOC2007上的实验结果。RON320++表示采取multi-scale testing， bounding box voting和flipping。 
![这里写图片描述](https://img-blog.csdn.net/20180610213113321?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table2是在VOC2012上的实验结果。 
![这里写图片描述](https://img-blog.csdn.net/20180610213148270?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table3是在COCO数据集上的结果。 
![这里写图片描述](https://img-blog.csdn.net/20180610213200117?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table5是对融合层的效果对比。 
![这里写图片描述](https://img-blog.csdn.net/20180610213212903?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**代码**

代码路径：~/RON/models/pascalvoc/VGG16/train.prototxt

```python
# line794。rpn_lrn7（对应论文中的rf-map）是4个融合层其中的一个，
# 因为4个融合层的操作都类似，所以这里就以rpn_lrn7为例介绍。
layer {
  name: "rpn_cls_score_7a"
  type: "Convolution"
  bottom: "rpn_lrn7"
  top: "rpn_cls_score_7a"
  param {
    lr_mult: 1.0 
    }
  param {
    lr_mult: 2.0 
    }
  convolution_param {
    num_output: 512   
    kernel_size: 3 pad: 1 stride: 1
    engine: CUDNN
    weight_filler { type: "gaussian" std: 0.01 }
    bias_filler { type: "constant" value: 0 }
  }
}
layer {
  name: "rpn_cls_score_7a_bn"
  type: "BatchNorm"
  bottom: "rpn_cls_score_7a"
  top: "rpn_cls_score_7a"
}
layer {
  bottom: "rpn_cls_score_7a"
  top: "rpn_cls_score_7a"
  name: "rpn_cls_score_7a_scale"
  type: "Scale"
  scale_param {
    bias_term: true
  }
}

layer {
  bottom: "rpn_cls_score_7a"
  top: "rpn_cls_score_7a"
  name: "rpn_cls_score_7a_relu"
  type: "ReLU"
  relu_param {
    engine: CUDNN
  }
}

# line837。这部分是bbox的二分类支路，卷积核数量是20，表示 2(bg/fg) * 10(anchors)。
layer {
  name: "rpn_cls_score_7"
  type: "Convolution"
  bottom: "rpn_cls_score_7a"
  top: "rpn_cls_score_7"
  param {
    lr_mult: 1.0 
    }
  param {
    lr_mult: 2.0 
    }
  convolution_param {
    num_output: 20   # 2(bg/fg) * 10(anchors)
    kernel_size: 3 pad: 1 stride: 1
    engine: CUDNN
    weight_filler { type: "gaussian" std: 0.01 }
    bias_filler { type: "constant" value: 0 }
  }
}

layer {
   bottom: "rpn_cls_score_7"
   top: "rpn_cls_score_reshape_7"
   name: "rpn_cls_score_reshape_7"
   type: "Reshape"
   reshape_param { shape { dim: 0 dim: 2 dim: -1 dim: 0 } }
}

# line1114。这部分是bbox的回归支路，也就是对应Figure5中的卷积核数量为4*A的3*3卷积层，
# A默认是10，这里省略了前面一个512卷积核的3*3卷积层。
layer {
  name: "rpn_bbox_pred_7"
  type: "Convolution"
  bottom: "rpn_bbox_7a"
  top: "rpn_bbox_pred_7"
  param { 
    lr_mult: 1.0 
    }
  param { 
    lr_mult: 2.0 
    }
  convolution_param {
    num_output: 40   # 4 * 10(anchors)
    kernel_size: 3 pad: 1 stride: 1
    weight_filler { type: "gaussian" std: 0.01 }
    bias_filler { type: "constant" value: 0 }
  }
}

# line1315。这一层是生成object二分类的目标：rpn_labels_7，坐标回归的目标：rpn_bbox_targets_7，
# 非越界框的权重：rpn_bbox_inside_weights_7，
# 越界框的权重：rpn_bbox_outside_weights_7
layer {
  name: 'rpn-data_7'
  type: 'Python'
  bottom: 'rpn_cls_score_7'
  bottom: 'gt_rois'
  bottom: 'im_info'
  bottom: 'data'
  top: 'rpn_labels_7'
  top: 'rpn_bbox_targets_7'
  top: 'rpn_bbox_inside_weights_7'
  top: 'rpn_bbox_outside_weights_7'
  python_param {
    module: 'ron_layer.anchor_target_layer'
    layer: 'AnchorTargetLayer'
    param_str: "'stride_scale_border_batchsize': 64,7,32,256"

  }
}

# line1396。这一层就是object二分类的损失函数
layer {
  name: "rpn_loss_cls_7"
  type: "SoftmaxWithLoss"
  bottom: "rpn_cls_score_reshape_7"
  bottom: "rpn_labels_7"
  propagate_down: 1
  propagate_down: 0
  top: "rpn_loss_cls_7"
  loss_weight: 1
  loss_param {
    ignore_label: -1
    normalize: true
  }
}

# line1457。这一层是bbox的回归损失计算，输入rpn_bbox_pred_7就是前面回归支路的输出，
# 剩下3个输入都是在line1315那一层生成的关于回归的目标和权重。
layer {
  name: "rpn_loss_bbox_7"
  type: "SmoothL1Loss"
  bottom: "rpn_bbox_pred_7"
  bottom: "rpn_bbox_targets_7"
  bottom: 'rpn_bbox_inside_weights_7'
  bottom: 'rpn_bbox_outside_weights_7'
  top: "det_loss_bbox_7"
  loss_weight: 1
  smooth_l1_loss_param { sigma: 3.0 }
}

## proposal
# line1507。接下来用softmax层得到bbox的二分类概率：rpn_cls_prob_7，
# 该输出用于后面的object分类支路的分类（具体而言是用于object分类的目标生成），
# 这部分也就是文章中objectness prior的含义，相当于给object分类支路一些指导。
# 其实这部分就和Faster RCNN中得到RPN网络输出的二分类概率过程很像。
layer {
  name: "rpn_cls_prob_7"
  type: "Softmax"
  bottom: "rpn_cls_score_reshape_7"
  top: "rpn_cls_prob_7"
}

layer {
  name: 'rpn_cls_prob_reshape_7'
  type: 'Reshape'
  bottom: 'rpn_cls_prob_7'
  top: 'rpn_cls_prob_reshape_7'
  reshape_param { shape { dim: 0 dim: 20 dim: -1 dim: 0 } }
}

# line2249。这一层对应论文中Figure5的(k+1)*A卷积层示例，也就是object分类支路，
# 该层前面是2个inception结构，因为inception篇幅较大，这里省略不写，
# 总之该层的输入det_cls_score_7b是第二个inception的输出。
# 另外因为默认是在VOC数据集上做检测，因此k是20，A是10，所以卷积核数量是210。
layer {
  name: "det_cls_score_7"
  type: "Convolution"
  bottom: "det_cls_score_7b"
  top: "det_cls_score_7"
  param {
    lr_mult: 1.0 
    }
  param {
    lr_mult: 2.0 
    }
  convolution_param {
    num_output: 210 
    kernel_size: 3 pad: 1 stride: 1
    engine: CUDNN
    weight_filler { type: "gaussian" std: 0.01 }
    bias_filler { type: "constant" value: 0 }
  }
}

layer {
   bottom: "det_cls_score_7"
   top: "det_cls_score_reshape_7"
   name: "det_cls_score_reshape_7"
   type: "Reshape"
   reshape_param { shape { dim: 0 dim: 21 dim: -1 dim: 0 } }
}

# line2281。这一层是生成检测网络的分类标签，输入rpn_cls_prob_reshape_7是前面得到
# 的bbox是否包含object的概率（或者叫score），根据概率可以对bbox做排序和过滤，
# 再结合ground truth可以为每个bbox分配标签（因此如果某个bbox的标签是-1，
# 那么object分类支路的损失函数就不会考虑这些标签）。除此之外还会对正负样本的比例做控制（1:3），
# 正样本全部保留。其实类似Faster RCNN中对proposal的处理（采取NMS对一万多的proposal做
# 过滤得到2000个proposal，然后再对这些proposal做进一步的正负样本比例控制），
# 只不过这里没有显式地生成和过滤proposal，而是通过det_labels_7中0,1,-1值的形式维护每个bbox是否
# 包含object的标签，由于bbox的二类别支路损失会不断修正rpn_cls_prob_reshape_7中bbox的输出概率，
# 因此det_labels_7得到的0,1,-1标签会更加准确，这就是objectness prior起到的指导作用。
layer {
  name: 'det-data_7'
  type: 'Python'
  bottom: 'rpn_cls_prob_reshape_7'
  bottom: 'gt_rois'
  bottom: 'im_info'
  top: 'det_labels_7'
  python_param {
    module: 'ron_layer.det_target_layer'
    layer: 'DetTargetLayer'
    param_str: "'stride_scale_border_batchsize_numcls': 64,7,32,256,21"   
  }
}

# object分类采用softmax loss损失函数，计算过程中忽略标签为-1的无效样本
layer {
  name: "det_loss_cls_7"
  type: "SoftmaxWithLoss"
  bottom: "det_cls_score_reshape_7"
  bottom: "det_labels_7"
  propagate_down: 1
  propagate_down: 0
  top: "det_loss_cls_7"
  loss_weight: 1
  loss_param {
    ignore_label: -1
    normalize: true
  }
}
```















