# SSD算法代码介绍（五）：网络结构细节 - AI之路 - CSDN博客





2018年02月17日 16:36:38[AI之路](https://me.csdn.net/u014380165)阅读数：4673








上一篇博客：[SSD算法代码介绍（四）：网络结构初探](http://blog.csdn.net/u014380165/article/details/79332466) 介绍了网络主干结构构建、损失函数定义等代码，**但是网络的详细构建内容是在common.py脚本中的两个重要函数：multi_layer_feature和multibox_layer进行的，比如基于分类网络新增加的一些层、预测层的定义等，因此接下来这篇博客介绍common.py脚本**。

common.py这个脚本主要包括conv_act_layer，legacy_conv_act_layer，multi_layer_feature，multibox_layer四个函数，因为在symbol_builder.py中先后调用了multi_layer_feature和multibox_layer两个函数，前面一个主要用来生成特征融合的几个层，后面一个主要用来生成分类层，回归层，生成anchor层，比较重要，因此可以先看看这两个函数。

```python
import mxnet as mx
import numpy as np

def multi_layer_feature(body, from_layers, num_filters, strides, pads, min_filter=128):
    """Wrapper function to extract features from base network, attaching extra
    layers and SSD specific layers

    Parameters
    ----------
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
    min_filter : int
        minimum number of filters used in 1x1 convolution

    Returns
    -------
    list of mx.Symbols

    """
    # arguments check
    assert len(from_layers) > 0
    assert isinstance(from_layers[0], str) and len(from_layers[0].strip()) > 0
    assert len(from_layers) == len(num_filters) == len(strides) == len(pads)


# 这个body是你导入的vgg16的symbol，get_internals函数是获取这个symbol的所有参数层的信息
    internals = body.get_internals()
    layers = []
    for k, params in enumerate(zip(from_layers, num_filters, strides, pads)):
        from_layer, num_filter, s, p = params
        # from_layer大概是这样的['relu4_3','relu7','','','','']，因此如果是后面的空字符串，那么就会跳到else那一部分
        if from_layer.strip(): 
            # extract from base network
# 得到from_layer里面指定名称的层，然后append到layers这个列表中
            layer = internals[from_layer.strip() + '_output']
            layers.append(layer)
        else: # 这一部分就是给原来的VGG16网络加上几个新的卷积操作
            # attach from last feature layer
            assert len(layers) > 0
            assert num_filter > 0
            layer = layers[-1] # 这一行表示现在要加的层的输入是上一层的输出
            num_1x1 = max(min_filter, num_filter // 2) 
# 一共添加了1*1和3*3两个卷积层，前面一个起减少计算量的作用，一般把这两个当做一个层，或者叫symbol
            conv_1x1 = conv_act_layer(layer, 'multi_feat_%d_conv_1x1' % (k),
                num_1x1, kernel=(1, 1), pad=(0, 0), stride=(1, 1), act_type='relu')
            conv_3x3 = conv_act_layer(conv_1x1, 'multi_feat_%d_conv_3x3' % (k),
                num_filter, kernel=(3, 3), pad=(p, p), stride=(s, s), act_type='relu')
            layers.append(conv_3x3)
# 最终这个layers列表会有6个symbol（以输入大小是300*300的vgg16的basemodel为例，就是上面的那个from_layer）
    return layers

def multibox_layer(from_layers, num_classes, sizes=[.2, .95],
                    ratios=[1], normalization=-1, num_channels=[],
                    clip=False, interm_layer=0, steps=[]):
    """
    the basic aggregation module for SSD detection. Takes in multiple layers,
    generate multiple object detection targets by customized layers

    Parameters:
    ----------
    from_layers : list of mx.symbol
        generate multibox detection from layers
    num_classes : int
        number of classes excluding background, will automatically handle
        background in this function
    sizes : list or list of list
        [min_size, max_size] for all layers or [[], [], []...] for specific layers
    ratios : list or list of list
        [ratio1, ratio2...] for all layers or [[], [], ...] for specific layers
    normalizations : int or list of int
        use normalizations value for all layers or [...] for specific layers,
        -1 indicate no normalizations and scales
    num_channels : list of int
        number of input layer channels, used when normalization is enabled, the
        length of list should equals to number of normalization layers
    clip : bool
        whether to clip out-of-image boxes
    interm_layer : int
        if > 0, will add a intermediate Convolution layer
    steps : list
        specify steps for each MultiBoxPrior layer, leave empty, it will calculate
        according to layer dimensions

    Returns:
    ----------
    list of outputs, as [loc_preds, cls_preds, anchor_boxes]
    loc_preds : localization regression prediction
    cls_preds : classification prediction
    anchor_boxes : generated anchor boxes
    """
    assert len(from_layers) > 0, "from_layers must not be empty list"
    assert num_classes > 0, \
        "num_classes {} must be larger than 0".format(num_classes)

    assert len(ratios) > 0, "aspect ratios must not be empty list"
    if not isinstance(ratios[0], list):
        # provided only one ratio list, broadcast to all from_layers
        ratios = [ratios] * len(from_layers)
    assert len(ratios) == len(from_layers), \
        "ratios and from_layers must have same length"

    assert len(sizes) > 0, "sizes must not be empty list"
    if len(sizes) == 2 and not isinstance(sizes[0], list):
        # provided size range, we need to compute the sizes for each layer
         assert sizes[0] > 0 and sizes[0] < 1
         assert sizes[1] > 0 and sizes[1] < 1 and sizes[1] > sizes[0]
         tmp = np.linspace(sizes[0], sizes[1], num=(len(from_layers)-1))
         min_sizes = [start_offset] + tmp.tolist()
         max_sizes = tmp.tolist() + [tmp[-1]+start_offset]
         sizes = zip(min_sizes, max_sizes)
    assert len(sizes) == len(from_layers), \
        "sizes and from_layers must have same length"

    if not isinstance(normalization, list):
        normalization = [normalization] * len(from_layers)
    assert len(normalization) == len(from_layers)

    assert sum(x > 0 for x in normalization) <= len(num_channels), \
        "must provide number of channels for each normalized layer"

    if steps:
        assert len(steps) == len(from_layers), "provide steps for all layers or leave empty"


# 初始化了3个空列表，后面会用到
    loc_pred_layers = []
    cls_pred_layers = []
    anchor_layers = []
    num_classes += 1 # always use background as label 0


# 前面基本上都是一些check操作，从这里开始才是硬货，这个大的for循环将循环每一个需要做特征融合的层（以VGG16网络，
# 输入为300*300为例，有2个层是VGG16本身的，有4个层是额外加上去的，因此from_layers这个列表包含上述6个symbol）
    for k, from_layer in enumerate(from_layers):
        from_name = from_layer.name
        # normalize
# normalization是一个列表，以输入为300*300的VGG16网络为例，默认的normalization是[20,-1,-1,-1,-1,-1]，
# 这里的-1表示该层不进行normalization。另外，normalization在这里是通过symbol中的L2Normalization和Variable来实现的。
# 在L2Normalization中mode是channel，表示按照channel里面数值的L2范数进行归一化，
# 类似这样：for i in 0...N , out[:,i,:,...,:] = data[:,i,:,...,:] / sqrt(sum(data[:,i,:,...,:] ** 2) + eps)。
# broadcast_mul是做一个broadcast的乘法，简单讲就是将scale和from_layer做点乘
        if normalization[k] > 0:
            from_layer = mx.symbol.L2Normalization(data=from_layer, \
                mode="channel", name="{}_norm".format(from_name))
            scale = mx.symbol.Variable(name="{}_scale".format(from_name),
                shape=(1, num_channels.pop(0), 1, 1),
                init=mx.init.Constant(normalization[k]),
                attr={'__wd_mult__': '0.1'})
            from_layer = mx.symbol.broadcast_mul(lhs=scale, rhs=from_layer)
        if interm_layer > 0:
            from_layer = mx.symbol.Convolution(data=from_layer, kernel=(3,3), \
                stride=(1,1), pad=(1,1), num_filter=interm_layer, \
                name="{}_inter_conv".format(from_name))
            from_layer = mx.symbol.Activation(data=from_layer, act_type="relu", \
                name="{}_inter_relu".format(from_name))

        # estimate number of anchors per location
        # here I follow the original version in caffe
        # TODO: better way to shape the anchors??
        size = sizes[k]
        assert len(size) > 0, "must provide at least one size"
        size_str = "(" + ",".join([str(x) for x in size]) + ")"
        ratio = ratios[k]
        assert len(ratio) > 0, "must provide at least one ratio"
        ratio_str = "(" + ",".join([str(x) for x in ratio]) + ")"
# anchor数目的计算，这里采用的并非len(size)*len(ratio)
        num_anchors = len(size) -1 + len(ratio)

        # create location prediction layer
# 这一部分主要是和location相关的，也就是回归框的具体位置，
# 主要是用一个filter数量为（anchor数目乘以4）的3*3的卷积操作得到预测的各anchor的坐标，该层是接在from_layer的后面。
# 因为pad=1，stride=1，所以该卷积是不改变feature map的大小的。最后把增加的层append到loc_pred_layers列表。
# 每个anchor对应4个坐标值
        num_loc_pred = num_anchors * 4
# __lr_mult__是指在该层的学习率要乘以的数值，得到的结果作为该层的学习率
        bias = mx.symbol.Variable(name="{}_loc_pred_conv_bias".format(from_name),
            init=mx.init.Constant(0.0), attr={'__lr_mult__': '2.0'})
        loc_pred = mx.symbol.Convolution(data=from_layer, bias=bias, kernel=(3,3), \
            stride=(1,1), pad=(1,1), num_filter=num_loc_pred, \
            name="{}_loc_pred_conv".format(from_name))
# transpose是交换的作用，比如loc_pred的维度是32*24*19*19，那么经过交换后得到的维度变成32*19*19*24，
# Flatten是将4个维度转换成2个维度，比如从32*19*19*24转换到32*8664。
# 之所以这样操作是为了后面特征融合时不受feature map大小不一致的影响。
        loc_pred = mx.symbol.transpose(loc_pred, axes=(0,2,3,1))
        loc_pred = mx.symbol.Flatten(data=loc_pred)
        loc_pred_layers.append(loc_pred)

        # create class prediction layer
# 这一部分主要是和类别预测相关，num_cls_pred对应要用到的3*3卷积的卷积核数量，这里num_classes是实际类别数+1（背景算一类），
# 最后把增加的层append到cls_pred_layers列表。
        num_cls_pred = num_anchors * num_classes
        bias = mx.symbol.Variable(name="{}_cls_pred_conv_bias".format(from_name),
            init=mx.init.Constant(0.0), attr={'__lr_mult__': '2.0'})
        cls_pred = mx.symbol.Convolution(data=from_layer, bias=bias, kernel=(3,3), \
            stride=(1,1), pad=(1,1), num_filter=num_cls_pred, \
            name="{}_cls_pred_conv".format(from_name))
        cls_pred = mx.symbol.transpose(cls_pred, axes=(0,2,3,1))
        cls_pred = mx.symbol.Flatten(data=cls_pred)
        cls_pred_layers.append(cls_pred)

        # create anchor generation layer
# 这一部分是用来生成anchor的，在mxnet中有专门的symbol用来做这个是，那就是mxnet.contrib.symbol.MultiBoxPrior，
# 输入也是from_layer，也就是要提取特征做融合的层。size_str是该层的anchor尺寸是原图尺寸的倍数关系，
# 比如(0.1,0.141)，ratio_str是anchor的宽高比例，比如(1,2,0.5)，clip是False，step是(0.0266667,0.0266667)，
# 表示该层的feature map的尺寸是输入图像尺寸的多少倍，
# 生成的anchors的维度是batch size*（h*w*（len(sizes)+len(ratios)-1））*4，是3维的。
# 最后把增加的层append到anchor_layers列表。
        if steps:
            step = (steps[k], steps[k])
        else:
            step = '(-1.0, -1.0)'
        anchors = mx.contrib.symbol.MultiBoxPrior(from_layer, sizes=size_str, ratios=ratio_str, \
            clip=clip, name="{}_anchors".format(from_name), steps=step)
        anchors = mx.symbol.Flatten(data=anchors)
        anchor_layers.append(anchors)


# 出来这个for循环后，就要对几个层做concate操作了，分别生成loc_preds，cls_preds，anchor_boxes这三个symbol，
# 这里做Reshape的时候，参数shape中有些值是-1，表示该维度的大小等于（原来几个维度的乘积）除以（现在其他维度的乘积），
# 因此shape里面最多只能有一个-1，要不就没法计算了，因此reshape后的cls_preds的维度就是：batch size*number of anchors*num_classes。
# 同样，reshape后的anchor_boxes的维度是：batch size*number of anchors*coordinate。
    loc_preds = mx.symbol.Concat(*loc_pred_layers, num_args=len(loc_pred_layers), \
        dim=1, name="multibox_loc_pred")
    cls_preds = mx.symbol.Concat(*cls_pred_layers, num_args=len(cls_pred_layers), \
        dim=1)
    cls_preds = mx.symbol.Reshape(data=cls_preds, shape=(0, -1, num_classes))
    cls_preds = mx.symbol.transpose(cls_preds, axes=(0, 2, 1), name="multibox_cls_pred")
    anchor_boxes = mx.symbol.Concat(*anchor_layers, \
        num_args=len(anchor_layers), dim=1)
    anchor_boxes = mx.symbol.Reshape(data=anchor_boxes, shape=(0, -1, 4), name="multibox_anchors")
    return [loc_preds, cls_preds, anchor_boxes]
```

**至此，该项目的主要训练代码就介绍完了。我们知道，对于一个完整的图像算法而言，网络结构的构建只是其中一部分，还有一部分比较重要的是评价函数的定义，比如准确率、损失函数等，可以帮助我们观测网络训练的效果，因此接下来一篇博客介绍该项目在训练过程中评价函数的定义：[SSD算法代码介绍（六）模型训练的评价函数](https://blog.csdn.net/u014380165/article/details/79332587)。**



