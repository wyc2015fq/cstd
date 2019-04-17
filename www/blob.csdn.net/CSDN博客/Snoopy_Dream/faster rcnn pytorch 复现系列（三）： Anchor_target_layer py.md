# faster rcnn pytorch 复现系列（三）： Anchor_target_layer.py - Snoopy_Dream - CSDN博客





2018年11月02日 21:49:47[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：351
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)









注：此篇文章的基础知识可见链接：

[pytoch faster rcnn复现系列（一） RPN层输入输出维度理解](https://blog.csdn.net/e01528/article/details/83618169)

[faster rcnn pytorch 复现系列（二）：generate_anchors源码解析](https://blog.csdn.net/e01528/article/details/83576796)

[RPN 预备编程知识（一） : Numpy VS Pytorch （ Anchor_target_layer ）](https://blog.csdn.net/e01528/article/details/83685031)

[RPN 预备编程知识（二） : 函数篇章 bbox_transform.py](https://blog.csdn.net/e01528/article/details/83662611)

[faster rcnn中 softmax，softmax loss和cross entropy的讲解](https://blog.csdn.net/e01528/article/details/83795970)

首先附上，参考的两篇caffe中faster rcnn的 Anchor_target_layer.py源码分析链接：

[Faster rcnn代码理解（3）](https://www.cnblogs.com/zf-blog/p/7307502.html)

[r-cnn学习（六）：RPN及AnchorTargetLayer学习](https://www.cnblogs.com/573177885qq/p/6141507.html)

然后写下pytorch中、Anchor_target_layer.py的解析，pytorch允许batchsize>1：



**目录**

[0. 变量名说明](#0.%20%E5%8F%98%E9%87%8F%E5%90%8D%E8%AF%B4%E6%98%8E)

[1. 导入相关包](#1.%20%E5%AF%BC%E5%85%A5%E7%9B%B8%E5%85%B3%E5%8C%85)

[2. __init__初始化](#2.%20__init__%E5%88%9D%E5%A7%8B%E5%8C%96)

[3. 通过ravel、meshgrid合用，得到平移坐标，构造通过原始anchor+平移，得到所有anchors。](#3.%20%E9%80%9A%E8%BF%87ravel%E3%80%81meshgrid%E5%90%88%E7%94%A8%EF%BC%8C%E5%BE%97%E5%88%B0%E5%B9%B3%E7%A7%BB%E5%9D%90%E6%A0%87%EF%BC%8C%E6%9E%84%E9%80%A0%E9%80%9A%E8%BF%87%E5%8E%9F%E5%A7%8Banchor%2B%E5%B9%B3%E7%A7%BB%EF%BC%8C%E5%BE%97%E5%88%B0%E6%89%80%E6%9C%89anchors%E3%80%82)

[4. 去除过界的anchor留下，**inds_inside+anchors**](#4.%20%E5%8E%BB%E9%99%A4%E8%BF%87%E7%95%8C%E7%9A%84anchor%E7%95%99%E4%B8%8B%EF%BC%8Cinds_inside%2Banchors)

[5.  通过overlaps制作labels，找到anchor对应的gt最大索引，和gt对应的anchor最大索引](#5.%C2%A0%20%E9%80%9A%E8%BF%87overlaps%E5%88%B6%E4%BD%9Clabels%EF%BC%8C%E6%89%BE%E5%88%B0anchor%E5%AF%B9%E5%BA%94%E7%9A%84gt%E6%9C%80%E5%A4%A7%E7%B4%A2%E5%BC%95%EF%BC%8C%E5%92%8Cgt%E5%AF%B9%E5%BA%94%E7%9A%84anchor%E6%9C%80%E5%A4%A7%E7%B4%A2%E5%BC%95)

[6.  降采样（正样本或者负样本太多）](#6.%C2%A0%20%E9%99%8D%E9%87%87%E6%A0%B7%EF%BC%88%E6%AD%A3%E6%A0%B7%E6%9C%AC%E6%88%96%E8%80%85%E8%B4%9F%E6%A0%B7%E6%9C%AC%E5%A4%AA%E5%A4%9A%EF%BC%89)

[7. bbox_targets、bbox_inside_weights、bbox_inside_weights](#7.%20bbox_targets%E3%80%81bbox_inside_weights%E3%80%81bbox_inside_weights)

[8.** 所有anchors**赋上label、bbox_targets、bbox_inside_weights、bbox_outside_weights属性](#8.%20%E6%89%80%E6%9C%89anchors%E8%B5%8B%E4%B8%8Alabel%E3%80%81bbox_targets%E3%80%81bbox_inside_weights%E3%80%81bbox_outside_weights%E5%B1%9E%E6%80%A7)

[9. _unmap见该函数：](#9.%20_unmap%E8%A7%81%E8%AF%A5%E5%87%BD%E6%95%B0%EF%BC%9A)

[10. 不需要反向传播](#10.%20%E4%B8%8D%E9%9C%80%E8%A6%81%E5%8F%8D%E5%90%91%E4%BC%A0%E6%92%AD)

11. 手绘流程图

12. caffe 版本

### 补充. cnn 大局观

y^=wx+b，x是输入，**w是特征权值向量。**

w：**往往我们说的3*3*256的卷积就是w了，**

y^：**通过卷积生成的（1,36，H，W）的就是y^，**

**y* ：gt对应的**

**L(y^,y*): y^和y*，做损失函数，通过优化器，迭代更新3*3*256的w卷积，学习到复杂的特征（就是w），来使得loss减少，预测的接近于真实的。**

比如faster rcnn 中**smooth l1 loss **就使得**rpn_box_pred(预测的偏移)接近于rpn_box_target**(**真实的偏移**),降低loss的方法，就是不断学习调整各个w，以学习复杂特征。（在这个过程中利用gt在anchor_target_layer中，先删除边界框外的anchor，然后获得边界框内的rpn_box_inside，然后重新弄一个新的整个anchor的，统一赋值0，然后讲框内的rpn_box_inside  再利用之前的赋值，来确定哪一个是整个anchor中，正样本对应的anchor，只对他进行**smooth l1 loss，也就是回归正anchor**），

在这个过程中**rpn_box_pred是直接在共享特征层进行3*3*256的卷积后，进行1*1*36的全卷积w（这个就是要学的w）生成y**^**（1,36，H，W），来存储预测的anchor。**

**而rpn_box_target则是通过寻找原始anchor最近的gt的方法，找到每个anchor对应的gt的偏移，用数学表达为：**y***（1,36，H，W）的向量。其中（1,36，H，W）可以理解为(1,4,9*H,W)也就是每个anchor的4个偏移。**

**首先：让我学习到：**

看源码
- 首先要弄清楚论文中的原理，
- 搞清楚这个py的输入和输出
- 分清楚流程，需要哪些步骤，求取中间值，然后构造中间函数【输入+输出】和中间变量。

### 0. 变量名说明

学习总结别人的命名。
- shift_x和shift_y：分别对应x和y轴上的偏移量。
- _num_anchors： anchor的3*3 9种数量，【最后的名词类似于num短小这种放在最前面】
- gt_boxes：gt的boxes【保证不头重脚轻，这种时候长名词放在后面】
- shifts： 偏移量，只要原始anchor+shifts，就可以实现逐行算anchor，得到所有anchor
- inds_inside：inside anchors的index
- overlaps ： （B，M，N）表示B个batchsize，M表示内部anchor总数，N表示gt的个数。IOU
- max_overlaps：每个内部anchor，找iou最大的gt （B，M）
- argmax_overlaps：每个内部anchor，找iou最大的gt 的索引
- gt_max_overlaps：每个内部gt，找iou最大的anchor （B，N）两种赋正样本的规则
- offset：因为batchsize导致的计算bbox_targets需要输入(B*K,5)的gt_boxes，索引经过了reshape，索引会发生便宜。
- bbox_targets：（1，4*9，H，W）把他看成（1，4, 9*h，w）每个anchor的偏移，边框回归的输入   M表示内部anchor的数量
- bbox_inside_weights：label是1，它就是1。其他为0，只就算正样本的损失。#(B,M) 与caffe版本的不同， # use a single value instead of 4 values for easy index，**pi***
- bbox_outside_weights：#(B,M) 样本权重归一化后正负样本都是1/256.其他背景为0

### 1. 导入相关包

from .generate_anchors .表示同一文件路径下。

```python
import torch
import torch.nn as nn
import numpy as np
import numpy.random as npr
from model.utils.config import cfg
from .generate_anchors import generate_anchors
from .bbox_transform import clip_boxes, bbox_overlaps_batch, bbox_transform_batch
import pdb

DEBUG = False
try:
    long        # Python 2
except NameError:
    long = int  # Python 3
```

### 2. __init__初始化

传入特征图与原图的倍数差，scales，ratios，初始化这三个参数，并generate_anchors生成tensor形式的**初始anchor**

```python
class _AnchorTargetLayer(nn.Module):
    """
        给所有的anchors赋对应的gt目标，制造anchor二分类的labels和bbox的回归用的targets
        targets 包括:dx dy dw dh
    """ #                   16      8 16 32 0.5 1 2   
    def __init__(self, feat_stride, scales, ratios):
        super(_AnchorTargetLayer, self).__init__()
        self._feat_stride = feat_stride
        self._scales = scales
        anchor_scales = scales
        #!这里从np转为了float_tensor，方便运用torch中函数
        #传入的scales和ratios是元组形式，需要np.array转换
        self._anchors =torch.from_numpy(generate_anchors(scales=np.array(anchor_scales), ratios=np.array(ratios))).float() #float_tensor
        self._num_anchors = self._anchors.size(0)
        # 是否允许包括少部分边界外的anchor
        self._allowed_border = 0  # default is 0
```

### 3. 通过ravel、meshgrid合用，得到平移坐标，构造通过原始anchor+平移，得到所有anchors。

shift_x和shift_y分别对应x和y轴上的偏移量，用在之前说过的用generate_anchors（）函数生成的最左上角的anchors上，对其进行偏移，从而获得所有图像上的anchors；all_anchors用来存储所有这些anchors，total_anchors用来存储这些anchors的数量K×A,其中，K是输入图像的num,A是一幅图像上anchor的num；之后作者还对这些anchors进行了筛选，超出图像边界的anchors都将其丢弃～继续：

```python
def forward(self, input):
        """ 
        对于公共特征图(H, W)，在每一个位置i，生成9个anchors
        超出边界的去掉
        """
        #input_>torch.Tensor
        rpn_cls_score = input[0]#map存有特征图的W H
        gt_boxes = input[1]#boxes labels[1,5]
        im_info = input[2]#(im_info[0][1])原图w(im_info[0][0])原图h
        num_boxes = input[3]

        # map of shape (..., H, W) torch里的size和shape一样
        height, width = rpn_cls_score.size(2), rpn_cls_score.size(3)
        #输如多少个gt
        batch_size = gt_boxes.size(0)

        feat_height, feat_width = rpn_cls_score.size(2), rpn_cls_score.size(3)
        #见RPN编程预备知识
        shift_x = np.arange(0, feat_width) * self._feat_stride
        shift_y = np.arange(0, feat_height) * self._feat_stride
        shift_x, shift_y = np.meshgrid(shift_x, shift_y)
        shifts = torch.from_numpy(np.vstack((shift_x.ravel(), shift_y.ravel(),
                                  shift_x.ravel(), shift_y.ravel())).transpose())
        # tensor.contiguous() → Tensor
        #返回一个内存连续的有相同数据的tensor，如果原tensor内存连续则返回原tensor
        #shifts.view need 内存连续，可以改成reshape函数，就不需要这一步了。
        #type_as(tesnor)将tensor投射为参数给定tensor类型并返回。 如果tensor已经是正确的类型则不会执行操作。等效于：self.type(tensor.type())
        #shifts int_>float
        shifts = shifts.contiguous().type_as(rpn_cls_score).float()

        # add A anchors (1, A, 4) to
        # cell K shifts (K, 1, 4) to get
        # shift anchors (K, A, 4)
        # reshape to (K*A, 4) shifted anchors

        A = self._num_anchors
        K = shifts.size(0)
        self._anchors = self._anchors.type_as(gt_boxes) #int_tensor_>float_tensor
        #board
        all_anchors = self._anchors.view(1, A, 4) + shifts.view(K, 1, 4)
        all_anchors = all_anchors.view(K * A, 4)

        total_anchors = int(K * A)
```

### 4. 去除过界的anchor留下，inds_inside+anchors，没有anchors[np.where()[0],:]方便

```python
total_anchors = int(K * A)
        #没有过界的anchors索引   xmin>=0 ymin>=0  xmax<w+0 ymax<h+0
        keep = ((all_anchors[:, 0] >= -self._allowed_border) &
                (all_anchors[:, 1] >= -self._allowed_border) &
                (all_anchors[:, 2] < long(im_info[0][1]) + self._allowed_border) &
                (all_anchors[:, 3] < long(im_info[0][0]) + self._allowed_border))

        #inds_inside ：没有过界的anchors索引
        inds_inside = torch.nonzero(keep).view(-1)

        #anchors：没有过界的anchors
        # keep only inside anchors
        anchors = all_anchors[inds_inside, :]
```

### 5.  通过overlaps制作labels，找到anchor对应的gt最大索引，和gt对应的anchor最大索引

这一部分主要就是获得这些anchors和对应gt的最大重叠率的情况，以及正样本的划分标准：

a.对于每一个gt，重叠率最大的那个anchor为fg；

b,对于每一个anchor，与gts中最大重叠率大于0.7的为fg；

cfg.TRAIN.RPN_CLOBBER_POSITIVE则涉及到一种情况，即如果最大重叠率小于cfg.TRAIN.RPN_NEGATIVE_OVERLAP=0.3,则到底正还是负，这里的cfg.TRAIN.RPN_CLOBBER_POSITIVE默认是False；

```python
# label: 1 is positive, 0 is negative, -1 is dont care
        labels = gt_boxes.new(batch_size, inds_inside.size(0)).fill_(-1) #labels (B*M)
        bbox_inside_weights = gt_boxes.new(batch_size, inds_inside.size(0)).zero_()
        bbox_outside_weights = gt_boxes.new(batch_size, inds_inside.size(0)).zero_()
        #返回（B，M，N）表示B个batchsize，M表示内部anchor总数，N表示gt的个数。
        overlaps = bbox_overlaps_batch(anchors, gt_boxes)
        #axis=2，意味着从（B，M，N）的 N 个gts中找出最大的iou 的 gt
        #返回（B，M）
        max_overlaps, argmax_overlaps = torch.max(overlaps, 2)
        #axis=1，意味着从（B，M，N）的 M 个anchor中找出最大iou 的 anchor
        #返回（B，N）
        gt_max_overlaps, _ = torch.max(overlaps, 1)

        if not cfg.TRAIN.RPN_CLOBBER_POSITIVES:
            labels[max_overlaps < cfg.TRAIN.RPN_NEGATIVE_OVERLAP] = 0
        #如果gt与所有anchors相交最大的那一个iou = 0,
        gt_max_overlaps[gt_max_overlaps==0] = 1e-5
        #gt_max_overlaps.view(batch_size,1,-1)的维度：（B，1，N）
        #overlaps 的维度：（B，M，N）
        #在这里我认为没有必要expand_as，因为执行运算会自动广播啊？
        #eq 相等返回1，不相等返回0,不像np中的==返回true，false
        #torch.sum（...，2）返回（B，M）
	keep = torch.sum(overlaps.eq(gt_max_overlaps.view(batch_size,1,-1).expand_as(overlaps)), 2)#keep（B，M）
        
        if torch.sum(keep) > 0:
            #找出与gt相交最大且iou不为0的那个anchor，作为正样本
            labels[keep>0] = 1

        # fg label: above threshold IOU
        #如果一个anchor与gts相交的最大值>0.7，赋予正样本
        labels[max_overlaps >= cfg.TRAIN.RPN_POSITIVE_OVERLAP] = 1
        #默认FALse
        if cfg.TRAIN.RPN_CLOBBER_POSITIVES:
            ##如果最大的anchor的iou都小于0.3，那么这个anchor是背景
            labels[max_overlaps < cfg.TRAIN.RPN_NEGATIVE_OVERLAP] = 0
```

### 6.  降采样（正样本或者负样本太多）

这一部分是说，如果我们得到的正样本或者负样本太多的话，那么就选取一定数量的，丢弃一定数量的anchors，应该是为了加速（这里的选取方法也很直接，就是随机选取），继续：

```python
#前景要求的数量
        num_fg = int(cfg.TRAIN.RPN_FG_FRACTION * cfg.TRAIN.RPN_BATCHSIZE)
        
        sum_fg = torch.sum((labels == 1).int(), 1)#（B,）
        sum_bg = torch.sum((labels == 0).int(), 1)#（B,）
       #如果多了，随机删除 感觉不严谨
        for i in range(batch_size):
            # subsample positive labels if we have too many
            if sum_fg[i] > num_fg:
                fg_inds = torch.nonzero(labels[i] == 1).view(-1)
                # torch.randperm seems has a bug on multi-gpu setting that cause the segfault.
                # See https://github.com/pytorch/pytorch/issues/1868 for more details.
                # use numpy instead.
                #rand_num = torch.randperm(fg_inds.size(0)).type_as(gt_boxes).long()
                rand_num = torch.from_numpy(np.random.permutation(fg_inds.size(0))).type_as(gt_boxes).long()
                disable_inds = fg_inds[rand_num[:fg_inds.size(0)-num_fg]]
                labels[i][disable_inds] = -1

#           num_bg = cfg.TRAIN.RPN_BATCHSIZE - sum_fg[i]
            num_bg = cfg.TRAIN.RPN_BATCHSIZE - torch.sum((labels == 1).int(), 1)[i]
	    #RPN_BATCHSIZE=256 RPN_FG_FRACTION=0.5 
	    #正样本如果最开始就是不够128个（256*0.5），就不够，负样本会大于128
            # subsample negative labels if we have too many
            if sum_bg[i] > num_bg:
                bg_inds = torch.nonzero(labels[i] == 0).view(-1)
                #rand_num = torch.randperm(bg_inds.size(0)).type_as(gt_boxes).long()

                rand_num = torch.from_numpy(np.random.permutation(bg_inds.size(0))).type_as(gt_boxes).long()
                disable_inds = bg_inds[rand_num[:bg_inds.size(0)-num_bg]]
                labels[i][disable_inds] = -1
```

### 7. bbox_targets、bbox_inside_weights、bbox_inside_weights

bbox_targets是一个anchor与他**最近的gt**【ancho对应的gt最大索引】之前的回归四要素。tx,ty,tw,th

这一部分是生成bbox_targets、bbox_inside_weights、bbox_inside_weights；其中对于bbox_targets，它这里是调用了**_compute_targets**()函数，见**bbox_transform_batch**：.

而对于后两个bbox_inside_weights和bbox_outside_weights，函数中定义的是bbox_inside_weights初始化为n×4的0数组，然后其中正样本的坐标的权值均为1；而bbox_outside_weights同样的初始化，其中正样本和负样本都被赋值1/num(anchors的数量)



def _compute_targets_batch(ex_rois, gt_rois):

    """Compute bounding-box regression targets for an image."""

    return **bbox_transform_batch**(ex_rois, gt_rois[:, :, :4])

```python
# gt_boxes (B,K,5) 
        offset = torch.arange(0, batch_size)*gt_boxes.size(1)#offset(batch_size,)
        # argmax_overlaps（B，M）         +  （B，1）    ！！索引需要
        argmax_overlaps = argmax_overlaps + offset.view(batch_size, 1).type_as(argmax_overlaps)
        # gt_boxes.view(-1,5) (B*K,5)所以需要offset
        # bbox_targets (batch_size, -1, 5)
        bbox_targets = _compute_targets_batch(anchors, gt_boxes.view(-1,5)[argmax_overlaps.view(-1), :].view(batch_size, -1, 5))

        # use a single value instead of 4 values for easy index.  
        #求出的偏移，是要看这个anchor离哪一个gt最近【那如果都不近呢，返回的应该是第一个gt的偏移】
        bbox_inside_weights[labels==1] = cfg.TRAIN.RPN_BBOX_INSIDE_WEIGHTS[0]
        #默认RPN_POSITIVE_WEIGHT=-1
        if cfg.TRAIN.RPN_POSITIVE_WEIGHT < 0:
            #num_examples = torch.sum(labels[i] >= 0)
            # #样本权重归一化
            num_examples = torch.sum(labels[i] >= 0).item()#正负的样本总数目
            positive_weights = 1.0 / num_examples#正样本权重 1/总样本，感觉可以直接256上啊
            negative_weights = 1.0 / num_examples

        else:
            assert ((cfg.TRAIN.RPN_POSITIVE_WEIGHT > 0) &
                    (cfg.TRAIN.RPN_POSITIVE_WEIGHT < 1))

        bbox_outside_weights[labels == 1] = positive_weights
        bbox_outside_weights[labels == 0] = negative_weights
```

### 8. 所有anchors赋上label、bbox_targets、bbox_inside_weights、bbox_outside_weights属性

至于为什么先view然后permute，可以参考[reshape和transpose的区别](https://blog.csdn.net/e01528/article/details/83786013)

```python
#利用inds_inside索引，建立一个所有anchors数组，按照labels中正负样本打1 0 标签，其余的fill-1
        labels = _unmap(labels, total_anchors, inds_inside, batch_size, fill=-1)
        bbox_targets = _unmap(bbox_targets, total_anchors, inds_inside, batch_size, fill=0)
        #(B,H*W)
        bbox_inside_weights = _unmap(bbox_inside_weights, total_anchors, inds_inside, batch_size, fill=0)
        bbox_outside_weights = _unmap(bbox_outside_weights, total_anchors, inds_inside, batch_size, fill=0)

        outputs = []
        #B,9*W*H
        labels = labels.view(batch_size, height, width, A).permute(0,3,1,2).contiguous()#B,9,h,w
        labels = labels.view(batch_size, 1, A * height, width)#B 1 9*H W
        outputs.append(labels)

        bbox_targets = bbox_targets.view(batch_size, height, width, A*4).permute(0,3,1,2).contiguous()#B,9*4,H,W
        outputs.append(bbox_targets)

        anchors_count = bbox_inside_weights.size(1)
        bbox_inside_weights = bbox_inside_weights.view(batch_size,anchors_count,1).expand(batch_size, anchors_count, 4)#

        bbox_inside_weights = bbox_inside_weights.contiguous().view(batch_size, height, width, 4*A)\
                            .permute(0,3,1,2).contiguous()##B,W,9*4,H

        outputs.append(bbox_inside_weights)

        bbox_outside_weights = bbox_outside_weights.view(batch_size,anchors_count,1).expand(batch_size, anchors_count, 4)
        bbox_outside_weights = bbox_outside_weights.contiguous().view(batch_size, height, width, 4*A)\
                            .permute(0,3,1,2).contiguous()
        outputs.append(bbox_outside_weights)

        return outputs
```

### 9. _unmap见该函数：

```python
def _unmap(data, count, inds, batch_size, fill=0):
    """ Unmap a subset of item (data) back to the original set of items (of
    size count) """

    if data.dim() == 2:
        ret = torch.Tensor(batch_size, count).fill_(fill).type_as(data)
        ret[:, inds] = data
    else:
        ret = torch.Tensor(batch_size, count, data.size(2)).fill_(fill).type_as(data)
        ret[:, inds,:] = data
    return ret
```

之后会把这些属性信息经过reshape封装进该网络层即top[0]、top[1]、top[2]、top[3]中；之后：

### 10. 不需要反向传播

```python
def backward(self, top, propagate_down, bottom):
        """This layer does not propagate gradients."""
        pass

    def reshape(self, bottom, top):
        """Reshaping happens during the call to forward."""
        pass
```

由于该层不需要反向传播，所以backward函数也不需要写了，在前向传播中已经reshape了，就不用再写reshape函数了～

### 11. 手绘流程图

![](https://img-blog.csdnimg.cn/20181106113520364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### 12. caffe版本的源码解析

```python
# --------------------------------------------------------
# Faster R-CNN
# Copyright (c) 2015 Microsoft
# Licensed under The MIT License [see LICENSE for details]
# Written by Ross Girshick and Sean Bell
# --------------------------------------------------------

import os
import caffe
import yaml
from fast_rcnn.config import cfg
import numpy as np
import numpy.random as npr
from generate_anchors import generate_anchors
from utils.cython_bbox import bbox_overlaps
from fast_rcnn.bbox_transform import bbox_transform

DEBUG = False

class AnchorTargetLayer(caffe.Layer):
    """
    Assign anchors to ground-truth targets. Produces anchor classification
    labels and bounding-box regression targets.
    """

    def setup(self, bottom, top):
        layer_params = yaml.load(self.param_str_)
        anchor_scales = layer_params.get('scales', (8, 16, 32))
        self._anchors = generate_anchors(scales=np.array(anchor_scales))
        self._num_anchors = self._anchors.shape[0]
        self._feat_stride = layer_params['feat_stride']

        if DEBUG:
            print 'anchors:'
            print self._anchors
            print 'anchor shapes:'
            print np.hstack((
                self._anchors[:, 2::4] - self._anchors[:, 0::4],
                self._anchors[:, 3::4] - self._anchors[:, 1::4],
            ))
            self._counts = cfg.EPS
            self._sums = np.zeros((1, 4))
            self._squared_sums = np.zeros((1, 4))
            self._fg_sum = 0
            self._bg_sum = 0
            self._count = 0

        # allow boxes to sit over the edge by a small amount
        self._allowed_border = layer_params.get('allowed_border', 0)

        height, width = bottom[0].data.shape[-2:]
        if DEBUG:
            print 'AnchorTargetLayer: height', height, 'width', width

        A = self._num_anchors
        # labels
        top[0].reshape(1, 1, A * height, width)
        # bbox_targets
        top[1].reshape(1, A * 4, height, width)
        # bbox_inside_weights
        top[2].reshape(1, A * 4, height, width)
        # bbox_outside_weights
        top[3].reshape(1, A * 4, height, width)

    def forward(self, bottom, top):
        # Algorithm:
        #
        # for each (H, W) location i
        #   generate 9 anchor boxes centered on cell i
        #   apply predicted bbox deltas at cell i to each of the 9 anchors
        # filter out-of-image anchors
        # measure GT overlap

        assert bottom[0].data.shape[0] == 1, \
            'Only single item batches are supported'

        # map of shape (..., H, W)
        height, width = bottom[0].data.shape[-2:]
        # GT boxes (x1, y1, x2, y2, label)
        gt_boxes = bottom[1].data
        # im_info
        im_info = bottom[2].data[0, :]

        if DEBUG:
            print ''
            print 'im_size: ({}, {})'.format(im_info[0], im_info[1])
            print 'scale: {}'.format(im_info[2])
            print 'height, width: ({}, {})'.format(height, width)
            print 'rpn: gt_boxes.shape', gt_boxes.shape
            print 'rpn: gt_boxes', gt_boxes

        # 1. Generate proposals from bbox deltas and shifted anchors
        shift_x = np.arange(0, width) * self._feat_stride
        shift_y = np.arange(0, height) * self._feat_stride
        shift_x, shift_y = np.meshgrid(shift_x, shift_y)
        shifts = np.vstack((shift_x.ravel(), shift_y.ravel(),
                            shift_x.ravel(), shift_y.ravel())).transpose()
        # add A anchors (1, A, 4) to
        # cell K shifts (K, 1, 4) to get
        # shift anchors (K, A, 4)
        # reshape to (K*A, 4) shifted anchors
        A = self._num_anchors
        K = shifts.shape[0]
        all_anchors = (self._anchors.reshape((1, A, 4)) +
                       shifts.reshape((1, K, 4)).transpose((1, 0, 2)))
        all_anchors = all_anchors.reshape((K * A, 4))
        total_anchors = int(K * A)

        # only keep anchors inside the image
        #注意np.where返回的是行+列，【0】表示行号
        inds_inside = np.where(
            (all_anchors[:, 0] >= -self._allowed_border) &
            (all_anchors[:, 1] >= -self._allowed_border) &
            (all_anchors[:, 2] < im_info[1] + self._allowed_border) &  # width
            (all_anchors[:, 3] < im_info[0] + self._allowed_border)    # height
        )[0]

        if DEBUG:
            print 'total_anchors', total_anchors
            print 'inds_inside', len(inds_inside)

        # keep only inside anchors
        #返回内部anchor
        anchors = all_anchors[inds_inside, :]
        if DEBUG:
            print 'anchors.shape', anchors.shape

        # label: 1 is positive, 0 is negative, -1 is dont care
        #创建一个(n,)的接近于0的数组
        labels = np.empty((len(inds_inside), ), dtype=np.float32)
        #fill全部变成-1
        labels.fill(-1)

        # overlaps between the anchors and the gt boxes
        # overlaps (ex, gt)     k表示gt的数量  N=len(inds_inside)，内部ANCHOR的数量
        # overlaps: (N, K) ndarray of overlap between boxes and query_boxes 
        #overlaps(m,n):m行，列，第m个anchor与第n个box的iou
        overlaps = bbox_overlaps(
            np.ascontiguousarray(anchors, dtype=np.float),
            np.ascontiguousarray(gt_boxes, dtype=np.float))
        #找到某个anchor与所有gt_box最大的overlaps【np.array】，按行找，返回列索引
            #(axis=1)返回（原行，） #(axis=0 返回（原列，）
        argmax_overlaps = overlaps.argmax(axis=1)#N行1列 argmax返回【np.array】
        #最大的overlaps值  overlaps【行索引，列索引】找出每一行对应的最大值！！！
        max_overlaps = overlaps[np.arange(len(inds_inside)), argmax_overlaps]#（N，）
        #按列取最大index，找出K个gt_box与哪一行的anchor的iou最大，返回的是行索引
        gt_argmax_overlaps = overlaps.argmax(axis=0)#找到的是每一个的第一个最大值gt_argmax_overlaps（k，）
        #写上行索引，然后np.arange（len（gt_box）），对应的iou的大小
        gt_max_overlaps = overlaps[gt_argmax_overlaps,#(K， )
                                   np.arange(overlaps.shape[1])]#(K， )
        #overlaps (N, K) gt_max_overlaps(K， )，数组广播！！！！！！1行k列填充成N行k列
                                    #找到所有的最大值，可能有相同大小的！！！
                                   #overlaps == gt_max_overlaps是 (N, K)的true false
        gt_argmax_overlaps = np.where(overlaps == gt_max_overlaps)[0]#【0】行【1】列

        #RPN_CLOBBER_POSITIVES默认是FALSE
        if not cfg.TRAIN.RPN_CLOBBER_POSITIVES:
            # assign bg labels first so that positive labels can clobber them
            #如果最大的anchor的iou都小于0.3，那么这个anchor是背景
            labels[max_overlaps < cfg.TRAIN.RPN_NEGATIVE_OVERLAP] = 0#max_overlaps （N，）广播
        #与每个gt相交最大的，可能不止一个，标签设置为1
        # fg label: for each gt, anchor with highest overlap
        labels[gt_argmax_overlaps] = 1
        #iou>0.7设置为前景
        # fg label: above threshold IOU
        labels[max_overlaps >= cfg.TRAIN.RPN_POSITIVE_OVERLAP] = 1

        if cfg.TRAIN.RPN_CLOBBER_POSITIVES:#默认是false，如果是true的话，如果同时满足正负样本条件，弄成负样本。
            # assign bg labels last so that negative labels can clobber positives
            labels[max_overlaps < cfg.TRAIN.RPN_NEGATIVE_OVERLAP] = 0

# subsample positive labels if we have too many
        num_fg = int(cfg.TRAIN.RPN_FG_FRACTION * cfg.TRAIN.RPN_BATCHSIZE)
        
        fg_inds = np.where(labels == 1)[0]#找到所有正样本的行号
        #如果多了，随机删除 感觉不严谨
        if len(fg_inds) > num_fg:
            disable_inds = npr.choice(
                fg_inds, size=(len(fg_inds) - num_fg), replace=False)
            labels[disable_inds] = -1

# subsample negative labels if we have too many，
#RPN_BATCHSIZE=256 RPN_FG_FRACTION=0.5 
#正样本如果最开始就是不够128个（256*0.5），就不够，负样本会大于128
        num_bg = cfg.TRAIN.RPN_BATCHSIZE - np.sum(labels == 1)#正样本比较少的情况
        bg_inds = np.where(labels == 0)[0]
        
        if len(bg_inds) > num_bg:
            disable_inds = npr.choice(
                bg_inds, size=(len(bg_inds) - num_bg), replace=False)
            labels[disable_inds] = -1#随机去除背景，当做不关系的对象
            #print "was %s inds, disabling %s, now %s inds" % (
                #len(bg_inds), len(disable_inds), np.sum(labels == 0))

        bbox_targets = np.zeros((len(inds_inside), 4), dtype=np.float32)
        #求出的偏移，是要看这个anchor离哪一个gt最近【那如果都不近呢，返回的应该是第一个gt的偏移】
        bbox_targets = _compute_targets(anchors, gt_boxes[argmax_overlaps, :])#argmax_overlaps所有anchor对应的最大iou的gtbox的索引

        bbox_inside_weights = np.zeros((len(inds_inside), 4), dtype=np.float32)
       # ！！特别注意下类似的切片 == 的赋值方式的学习#前景的权重为1.0
        bbox_inside_weights[labels == 1, :] = np.array(cfg.TRAIN.RPN_BBOX_INSIDE_WEIGHTS)#1.0 1.0 1.0 1.0

        bbox_outside_weights = np.zeros((len(inds_inside), 4), dtype=np.float32)
        
        
        #默认RPN_POSITIVE_WEIGHT=-1
        if cfg.TRAIN.RPN_POSITIVE_WEIGHT < 0:
            #样本权重归一化
            # uniform weighting of examples (given non-uniform sampling)
            num_examples = np.sum(labels >= 0)#正负的样本总数目
            positive_weights = np.ones((1, 4)) * 1.0 / num_examples#正样本权重 1/总样本，感觉可以直接256上啊
            negative_weights = np.ones((1, 4)) * 1.0 / num_examples#负样本权重 1/总样本
        #反而觉得RPN_POSITIVE_WEIGHT=0.5更合适些。。
        else:
            assert ((cfg.TRAIN.RPN_POSITIVE_WEIGHT > 0) &
                    (cfg.TRAIN.RPN_POSITIVE_WEIGHT < 1))
            positive_weights = (cfg.TRAIN.RPN_POSITIVE_WEIGHT /
                                np.sum(labels == 1))#自己设置正负样本的权重，
            negative_weights = ((1.0 - cfg.TRAIN.RPN_POSITIVE_WEIGHT) /
                                np.sum(labels == 0))
                                
        bbox_outside_weights[labels == 1, :] = positive_weights
        bbox_outside_weights[labels == 0, :] = negative_weights

        if DEBUG:
            self._sums += bbox_targets[labels == 1, :].sum(axis=0)
            self._squared_sums += (bbox_targets[labels == 1, :] ** 2).sum(axis=0)
            self._counts += np.sum(labels == 1)
            means = self._sums / self._counts
            stds = np.sqrt(self._squared_sums / self._counts - means ** 2)
            print 'means:'
            print means
            print 'stdevs:'
            print stds

        # map up to original set of anchors
        #total_anchors所有anchor的数量

        #利用inds_inside索引，建立一个所有anchors数组，按照labels中正负样本打1 0 标签，其余的fill-1
        labels = _unmap(labels, total_anchors, inds_inside, fill=-1)
        bbox_targets = _unmap(bbox_targets, total_anchors, inds_inside, fill=0)
        bbox_inside_weights = _unmap(bbox_inside_weights, total_anchors, inds_inside, fill=0)
        bbox_outside_weights = _unmap(bbox_outside_weights, total_anchors, inds_inside, fill=0)

        if DEBUG:
            print 'rpn: max max_overlap', np.max(max_overlaps)
            print 'rpn: num_positive', np.sum(labels == 1)
            print 'rpn: num_negative', np.sum(labels == 0)
            self._fg_sum += np.sum(labels == 1)
            self._bg_sum += np.sum(labels == 0)
            self._count += 1
            print 'rpn: num_positive avg', self._fg_sum / self._count
            print 'rpn: num_negative avg', self._bg_sum / self._count

        #变形赋值top[0]
        # labels
        labels = labels.reshape((1, height, width, A)).transpose(0, 3, 1, 2)
        labels = labels.reshape((1, 1, A * height, width))
        top[0].reshape(*labels.shape)
        top[0].data[...] = labels

        # bbox_targets
        bbox_targets = bbox_targets \
            .reshape((1, height, width, A * 4)).transpose(0, 3, 1, 2)
        top[1].reshape(*bbox_targets.shape)
        top[1].data[...] = bbox_targets

        # bbox_inside_weights
        bbox_inside_weights = bbox_inside_weights \
            .reshape((1, height, width, A * 4)).transpose(0, 3, 1, 2)
        assert bbox_inside_weights.shape[2] == height
        assert bbox_inside_weights.shape[3] == width
        top[2].reshape(*bbox_inside_weights.shape)
        top[2].data[...] = bbox_inside_weights

        # bbox_outside_weights
        bbox_outside_weights = bbox_outside_weights \
            .reshape((1, height, width, A * 4)).transpose(0, 3, 1, 2)
        assert bbox_outside_weights.shape[2] == height
        assert bbox_outside_weights.shape[3] == width
        top[3].reshape(*bbox_outside_weights.shape)
        top[3].data[...] = bbox_outside_weights

    def backward(self, top, propagate_down, bottom):
        """This layer does not propagate gradients."""
        pass

    def reshape(self, bottom, top):
        """Reshaping happens during the call to forward."""
        pass



def _unmap(data, count, inds, fill=0):
    """ Unmap a subset of item (data) back to the original set of items (of
    size count) """
    if len(data.shape) == 1:
        ret = np.empty((count, ), dtype=np.float32)
        ret.fill(fill)
        ret[inds] = data
    else:
        ret = np.empty((count, ) + data.shape[1:], dtype=np.float32)
        ret.fill(fill)
        ret[inds, :] = data
    return ret



def _compute_targets(ex_rois, gt_rois):
    """Compute bounding-box regression targets for an image."""

    assert ex_rois.shape[0] == gt_rois.shape[0]#都是N行
    assert ex_rois.shape[1] == 4
    assert gt_rois.shape[1] == 5# # GT boxes (x1, y1, x2, y2, label)

    return bbox_transform(ex_rois, gt_rois[:, :4]).astype(np.float32, copy=False)
```



