# SSD算法代码介绍（四）：网络结构初探 - AI之路 - CSDN博客





2018年02月17日 16:13:50[AI之路](https://me.csdn.net/u014380165)阅读数：4271








上一篇博客：[SSD算法代码介绍（三）：网络结构参数配置](http://blog.csdn.net/u014380165/article/details/79332406) 主要涉及网络结构构建时候的一些参数设置，最终是通过调用symbol_builder.py脚本的get_symbol_train函数完成网络的主干结构构建和损失函数的定义，因此接下来介绍symbol_builder.py脚本。

symbol_builder.py这个脚本主要包括import_module，get_symbol_train，get_symbol三个函数，后面两个函数类似，**可以从get_symbol_train函数开始看**，这里面调用了import_module函数。

```python
from __future__ import absolute_import
import mxnet as mx
# common.py脚本中的这两个函数主要是用到定义新层和anchor的。
from .common import multi_layer_feature, multibox_layer  

def import_module(module_name):
    """Helper function to import module"""
    import sys, os
    import importlib
    sys.path.append(os.path.dirname(__file__))
# module_name只是一个str，这里调用importlib库的import_modole函数来导入指定名称的symbol脚本
    return importlib.import_module(module_name)

def get_symbol_train(network, num_classes, from_layers, num_filters, strides, pads,
                     sizes, ratios, normalizations=-1, steps=[], min_filter=128,
                     nms_thresh=0.5, force_suppress=False, nms_topk=400, **kwargs):
    """Build network symbol for training SSD

    Parameters
    ----------
    network : str
        base network symbol name
    num_classes : int
        number of object classes not including background
    from_layers : list of str
        feature extraction layers, use '' for add extra layers
        For example:
        from_layers = ['relu4_3', 'fc7', '', '', '', '']
        which means extract feature from relu4_3 and fc7, adding 4 extra layers
        on top of fc7
    num_filters : list of int
        number of filters for extra layers, you can use -1 for extracted features,
        however, if normalization and scale is applied, the number of filter for
        that layer must be provided.
        For example:
        num_filters = [512, -1, 512, 256, 256, 256]
    strides : list of int
        strides for the 3x3 convolution appended, -1 can be used for extracted
        feature layers
    pads : list of int
        paddings for the 3x3 convolution, -1 can be used for extracted layers
    sizes : list or list of list
        [min_size, max_size] for all layers or [[], [], []...] for specific layers
    ratios : list or list of list
        [ratio1, ratio2...] for all layers or [[], [], ...] for specific layers
    normalizations : int or list of int
        use normalizations value for all layers or [...] for specific layers,
        -1 indicate no normalizations and scales
    steps : list
        specify steps for each MultiBoxPrior layer, leave empty, it will calculate
        according to layer dimensions
    min_filter : int
        minimum number of filters used in 1x1 convolution
    nms_thresh : float
        non-maximum suppression threshold
    force_suppress : boolean
        whether suppress different class objects
    nms_topk : int
        apply NMS to top K detections

    Returns
    -------
    mx.Symbol

    """
    label = mx.sym.Variable('label')
# 调用前面的import_module函数，network是一个str，比如代码里面默认是‘vgg16_reduced’，
# 因此后面的这个get_symbol就是调用的symbol文件夹下面的vgg16_reduced.py脚本中的get_symbol函数来得到symbol，
# 因此body的数据类型是Symbol，是vgg16网络结构的定义。
    body = import_module(network).get_symbol(num_classes, **kwargs)

# 这里是调用symbol文件夹下的common.py脚本中的multi_layer_feature函数，生成的layers是一个列表，
# 里面存放着包括relu4_3，relu7等6个symbol（这里是以输入图像大小为300*300的VGG16为例，所以是6个symbol）
    layers = multi_layer_feature(body, from_layers, num_filters, strides, pads,
        min_filter=min_filter)


# 然后是调用commong.py脚本中的multibox_layer函数得到loc_preds，cls_preds，anchor_boxes三个symbol，
# 这三个symbol分别表示回归层的计算，分类层的计算以及anchor是怎么生成和设计的。
# 另外这个函数包含具体的取哪些层做特征融合以及怎么融合的过程。
    loc_preds, cls_preds, anchor_boxes = multibox_layer(layers, \
        num_classes, sizes=sizes, ratios=ratios, normalization=normalizations, \
        num_channels=num_filters, clip=False, interm_layer=0, steps=steps)


# mxnet.contrib.symbol.MultiBoxTarget的官方解释是用来Compute Multibox training targets，
# 换句话说就是用来设定正负样本和处理模型预测值与真实值之间的关系。
# 在输入中，anchor_boxes和cls_preds都是前面刚刚生成的symbol，维度都是3维，label也是一个symbol，
# 是在该函数的第一行就定义的一个variable，在训练网络过程中这个label就是ground truth的信息。
# overlap_threshold这个参数是用来判断哪些anchor是正样本的阈值，
# 官方的介绍是：Anchor-GT overlap threshold to be regarded as a possitive match。
# negative_mining_ratio表示max negative to positive samples ratio，也就是负样本和正样本的最大比例，
# 之所以会有这个参数是因为一般数据中负样本的数量会远远大于正样本，通过限定负样本和正样本的比例可以使得模型更好收敛。
# minimum_negative_samples表示负样本的最少数量。negative_mining_thresh表示threshold used for negative mining。
# 最后得到的tmp也是一个symbol，包含3个部分，分别是loc_target，也就是回归的目标，简而言之是中心坐标和宽高的偏置；
# loc_target_mask，这个mask的存在是因为框回归时候的损失函数回传只会回传和真实object框的IOU大于设定的overlap_threshold的anchors；
# cls_target，表示每个anchor的真实标签。
    tmp = mx.contrib.symbol.MultiBoxTarget(
        *[anchor_boxes, label, cls_preds], overlap_threshold=.5, \
        ignore_label=-1, negative_mining_ratio=3, minimum_negative_samples=0, \
        negative_mining_thresh=.5, variances=(0.1, 0.1, 0.2, 0.2),
        name="multibox_target")
    loc_target = tmp[0]
    loc_target_mask = tmp[1]
    cls_target = tmp[2]


# 针对分类，采用softmaxoutput来输出概率，其主要输入是网络生成的类别预测值：cls_preds和每个anchor的真实值：cls_target。
    cls_prob = mx.symbol.SoftmaxOutput(data=cls_preds, label=cls_target, \
        ignore_label=-1, use_ignore=True, grad_scale=1., multi_output=True, \
        normalization='valid', name="cls_prob")

# 针对边框回归，采用smooth L1损失函数。mxnet.symbol.smooth_l1是用来计算smooth L1损失，
# 可以参看mxnet官网关于这个symbol的介绍：https://mxnet.incubator.apache.org/api/python/symbol.html#mxnet.symbol.smooth_l1。
# 这里的输入用到了网络生成的框坐标预测值：loc_preds，真实框坐标：loc_target，回归框的mask：loc_target_mask。
# loc_target_mask * (loc_preds - loc_target)表示只将有object的框的预测与实际值的差值作为损失函数的输入。
# 另外这3个输入的维度都是一样的。
    loc_loss_ = mx.symbol.smooth_l1(name="loc_loss_", \
        data=loc_target_mask * (loc_preds - loc_target), scalar=1.0)

# 这个mxnet.symbol.makeloss是用来生成你自己的loss函数的，其输入得是一个自定义的symbol，这里是用上面得到的smooth_l1损失作为输入。
# 官方的一个例子：cross_entropy = label * log(out) + (1 - label) * log(1 - out)，loss = MakeLoss(cross_entropy)。
# 这个normalization参数的含义如下：If this is set to null, the output gradient will not be normalized. 
# If this is set to batch, the output gradient will be divided by the batch size. 
# If this is set to valid, the output gradient will be divided by the number of valid input elements。
    loc_loss = mx.symbol.MakeLoss(loc_loss_, grad_scale=1., \
        normalization='valid', name="loc_loss")

    # monitoring training status
# 接下来这两个层并不参与梯度回传（因为grad_scale=0），只是用来提供后续计算某些值时的输入。
# 因此即便后面用mx.symbol.Group()接口将这两层的输出也绑定在一起，但是对loss是没有贡献的。
# nms操作是在mx.contrib.symbol.MultiBoxDetection()接口中实现的。
# 现在在MXNet文档是调整为mx.symbol.contrib.MultiBoxDetection()接口。
# 最后得到的det变量包含每个anchor的预测类别、置信度和4个坐标相关的值。
    cls_label = mx.symbol.MakeLoss(data=cls_target, grad_scale=0, name="cls_label")
    det = mx.contrib.symbol.MultiBoxDetection(*[cls_prob, loc_preds, anchor_boxes], \
        name="detection", nms_threshold=nms_thresh, force_suppress=force_suppress,
        variances=(0.1, 0.1, 0.2, 0.2), nms_topk=nms_topk)
    det = mx.symbol.MakeLoss(data=det, grad_scale=0, name="det_out")


    # group output
# 最后输出分类的类别概率（每个anchor都有）：cls_prob；回归的损失：loc_loss；每个anchor的真实标签：cls_label。
    out = mx.symbol.Group([cls_prob, loc_loss, cls_label, det])
    return out

def get_symbol(network, num_classes, from_layers, num_filters, sizes, ratios,
               strides, pads, normalizations=-1, steps=[], min_filter=128,
               nms_thresh=0.5, force_suppress=False, nms_topk=400, **kwargs):
    """Build network for testing SSD

    Parameters
    ----------
    network : str
        base network symbol name
    num_classes : int
        number of object classes not including background
    from_layers : list of str
        feature extraction layers, use '' for add extra layers
        For example:
        from_layers = ['relu4_3', 'fc7', '', '', '', '']
        which means extract feature from relu4_3 and fc7, adding 4 extra layers
        on top of fc7
    num_filters : list of int
        number of filters for extra layers, you can use -1 for extracted features,
        however, if normalization and scale is applied, the number of filter for
        that layer must be provided.
        For example:
        num_filters = [512, -1, 512, 256, 256, 256]
    strides : list of int
        strides for the 3x3 convolution appended, -1 can be used for extracted
        feature layers
    pads : list of int
        paddings for the 3x3 convolution, -1 can be used for extracted layers
    sizes : list or list of list
        [min_size, max_size] for all layers or [[], [], []...] for specific layers
    ratios : list or list of list
        [ratio1, ratio2...] for all layers or [[], [], ...] for specific layers
    normalizations : int or list of int
        use normalizations value for all layers or [...] for specific layers,
        -1 indicate no normalizations and scales
    steps : list
        specify steps for each MultiBoxPrior layer, leave empty, it will calculate
        according to layer dimensions
    min_filter : int
        minimum number of filters used in 1x1 convolution
    nms_thresh : float
        non-maximum suppression threshold
    force_suppress : boolean
        whether suppress different class objects
    nms_topk : int
        apply NMS to top K detections

    Returns
    -------
    mx.Symbol

    """
    body = import_module(network).get_symbol(num_classes, **kwargs)
    layers = multi_layer_feature(body, from_layers, num_filters, strides, pads,
        min_filter=min_filter)

    loc_preds, cls_preds, anchor_boxes = multibox_layer(layers, \
        num_classes, sizes=sizes, ratios=ratios, normalization=normalizations, \
        num_channels=num_filters, clip=False, interm_layer=0, steps=steps)

    cls_prob = mx.symbol.SoftmaxActivation(data=cls_preds, mode='channel', \
        name='cls_prob')
    out = mx.contrib.symbol.MultiBoxDetection(*[cls_prob, loc_preds, anchor_boxes], \
        name="detection", nms_threshold=nms_thresh, force_suppress=force_suppress,
        variances=(0.1, 0.1, 0.2, 0.2), nms_topk=nms_topk)
    return out
```

**从上述代码可以看出，整体思路是先调用common.py脚本中的multi_layer_feature函数获取分类网络的一些输出层以及新增加了几个层，然后通过common.py脚本中的multibox_layer函数进行预测层的定义从而完成网络结构的主干构建，最后定义了各损失函数等信息完成了这个网络的全部搭建。因此接下来就要深入了解网络结构主干搭建代码的common.py脚本：[SSD算法代码介绍（五）：网络结构细节](https://blog.csdn.net/u014380165/article/details/79332528)。**



