# RefineDet算法源码（二）网络结构 - AI之路 - CSDN博客





2018年06月04日 08:22:57[AI之路](https://me.csdn.net/u014380165)阅读数：4767








论文：Single-Shot Refinement Neural Network for Object Detection 

论文链接：[https://arxiv.org/abs/1711.06897](https://arxiv.org/abs/1711.06897)

代码链接：[https://github.com/sfzhang15/RefineDet](https://github.com/sfzhang15/RefineDet)
关于RefineDet算法内容可以先看看博客：[RefineDet论文笔记](https://blog.csdn.net/u014380165/article/details/79502308)。

RefineDet算法是SSD算法的升级版本，所以大部分的代码也是基于SSD的开源代码来修改的。SSD开源代码参考链接：[https://github.com/weiliu89/caffe/tree/ssd](https://github.com/weiliu89/caffe/tree/ssd)。**RefineDet主要包含anchor refinement module (ARM) 、object detection module (ODM)、transfer connection block (TCB)3个部分，ARM部分可以直接用SSD代码，只不过将分类支路的类别数由object数量+1修改成2，类似RPN网络，目的是得到更好的初始bbox。ODM部分也可以基于SSD代码做修改，主要是原本采用的default box用ARM生成的bbox代替，剩下的分类和回归支路与SSD一样。TCB部分则通过一些卷积层和反卷积层即可实现。**

在博客：[RefineDet算法源码 （一）训练脚本](https://blog.csdn.net/u014380165/article/details/80562416)中介绍了训练RefineDet算法的代码，其中包含宏观上的网络结构构建，并未涉及细节内容。**因此这篇博客介绍RefineDet算法的具体网络结构构造细节**，代码所在路径：~RefineDet/python/caffe/model_libs.py脚本的CreateRefineDetHead函数。

```python
'''
CreateRefineDetHead函数是本文关于网络结构构造的重点，这部分代码也是在原来SSD的CreateMultiBoxHead函数
基础上修改得到的，可以看作是将原来SSD的CreateMultiBoxHead函数内容实现了两遍，一遍用来实现ARM部分，
另一边用来实现ORM部分。from_layers和from_layers2是两个重点输入，
分别对应论文中Figure1的ARM和OBM两部分输出。因此这两遍实现除了输入不同外，另一个不同是ARM部分
是类似RPN网络的bbox回归和二分类，而ORM部分是类似SSD检测网络的bbox回归和object分类。
'''
def CreateRefineDetHead(net, data_layer="data", num_classes=[], from_layers=[], from_layers2=[], normalizations=[], use_batchnorm=True, lr_mult=1, min_sizes=[], max_sizes=[], prior_variance = [0.1],aspect_ratios=[], steps=[], img_height=0, img_width=0, share_location=True, flip=True, clip=True, offset=0.5, inter_layer_depth=[], kernel_size=1, pad=0, conf_postfix='', loc_postfix='', **bn_param):
    assert num_classes, "must provide num_classes"
    assert num_classes > 0, "num_classes must be positive number"
    if normalizations:
        assert len(from_layers) == len(normalizations), "from_layers and normalizations should have same length"
    assert len(from_layers) == len(min_sizes), "from_layers and min_sizes should have same length"
    if max_sizes:
        assert len(from_layers) == len(max_sizes), "from_layers and max_sizes should have same length"
    if aspect_ratios:
        assert len(from_layers) == len(aspect_ratios), "from_layers and aspect_ratios should have same length"
    if steps:
        assert len(from_layers) == len(steps), "from_layers and steps should have same length"
    net_layers = net.keys()
    assert data_layer in net_layers, "data_layer is not in net's layers"
    if inter_layer_depth:
        assert len(from_layers) == len(inter_layer_depth), "from_layers and inter_layer_depth should have same length"

# 接下来的代码分为两部分，一部分是Anchor Refinement Module（ARM），另一部分
# 是Object Detection Module（ODM），首先看看Anchor Refinement Module（ARM）部分内容。
    prefix = 'arm'
    num_classes_rpn = 2
    num = len(from_layers)
    priorbox_layers = []
    loc_layers = []
    conf_layers = []
# 这个循环就是作用于每个融合层，文章中默认融合层有4个。
    for i in range(0, num):
        from_layer = from_layers[i]

        # Get the normalize value.
        if normalizations:
            if normalizations[i] != -1:
                norm_name = "{}_norm".format(from_layer)
                net[norm_name] = L.Normalize(net[from_layer], scale_filler=dict(type="constant", value=normalizations[i]),
                    across_spatial=False, channel_shared=False)
                from_layer = norm_name

        # Add intermediate layers.

# 这部分默认是执行的，而且inter_layer_depth=[1,1,1,1]，也就是每个融合层都接一个residual block，
# 这种在分类和回归支路之前再添加层的操作在很多object detection算法中都有。
        if inter_layer_depth:
            if inter_layer_depth[i] > 0:
                inter_name = "{}_inter".format(from_layer)
                ResBody(net, from_layer, inter_name, out2a=256, out2b=256, out2c=1024, stride=1, use_branch1=True)
                # ConvBNLayer(net, from_layer, inter_name, use_bn=use_batchnorm, use_relu=True, lr_mult=lr_mult,
                #       num_output=inter_layer_depth[i], kernel_size=3, pad=1, stride=1, **bn_param)
                from_layer = "res{}".format(inter_name)

        # Estimate number of priors per location given provided parameters.
        min_size = min_sizes[i]
        if type(min_size) is not list:
            min_size = [min_size]
        aspect_ratio = []
        if len(aspect_ratios) > i:
            aspect_ratio = aspect_ratios[i]
            if type(aspect_ratio) is not list:
                aspect_ratio = [aspect_ratio]
        max_size = []
        if len(max_sizes) > i:
            max_size = max_sizes[i]
            if type(max_size) is not list:
                max_size = [max_size]
            if max_size:
                assert len(max_size) == len(min_size), "max_size and min_size should have same length."
        if max_size:
            num_priors_per_location = (2 + len(aspect_ratio)) * len(min_size)
        else:
            num_priors_per_location = (1 + len(aspect_ratio)) * len(min_size)
        if flip:
            num_priors_per_location += len(aspect_ratio) * len(min_size)
        step = []
        if len(steps) > i:
            step = steps[i]

        # Create location prediction layer.
# 这部分代码是创建bbox的坐标回归层，num_priors_per_location是feature map层的每个点生成的bbox的数量。
# share_location默认是True，所以不执行条件语句。得到的结果就会插入loc_layers列表中，
# 这样经过4个融合层后，loc_layers就包含4个融合层的bbox坐标回归结果。
        name = "{}_mbox_loc{}".format(from_layer, loc_postfix)
        num_loc_output = num_priors_per_location * 4
        if not share_location:
            num_loc_output *= num_classes_rpn
        ConvBNLayer(net, from_layer, name, use_bn=use_batchnorm, use_relu=False, lr_mult=lr_mult, num_output=num_loc_output, kernel_size=kernel_size, pad=pad, stride=1, **bn_param)
        permute_name = "{}_perm".format(name)
        net[permute_name] = L.Permute(net[name], order=[0, 2, 3, 1])
        flatten_name = "{}_flat".format(name)
        net[flatten_name] = L.Flatten(net[permute_name], axis=1)
        loc_layers.append(net[flatten_name])

        # Create confidence prediction layer.
# 这部分代码是创建bbox的分类层，这里num_conf_output = num_priors_per_location * num_classes_rpn，
# 要注意的是num_classes_rpn设置为2，所以这里是对每个bbox做二分类，也就是前景（foreground）和
# 背景（background）的二分类。因此这里的分类支路就和RPN网络一样，得到的结果会插入conf_layers列表中，
# 这样经过4个融合层后，conf_layers就包含4个融合层的二分类结果了。
        name = "{}_mbox_conf{}".format(from_layer, conf_postfix)
        num_conf_output = num_priors_per_location * num_classes_rpn
        ConvBNLayer(net, from_layer, name, use_bn=use_batchnorm, use_relu=False, lr_mult=lr_mult,num_output=num_conf_output, kernel_size=kernel_size, pad=pad, stride=1, **bn_param)
        permute_name = "{}_perm".format(name)
        net[permute_name] = L.Permute(net[name], order=[0, 2, 3, 1])
        flatten_name = "{}_flat".format(name)
        net[flatten_name] = L.Flatten(net[permute_name], axis=1)
        conf_layers.append(net[flatten_name])

        # Create prior generation layer.
'''
这一部分代码是生成anchor（或者叫priorbox），这些anchor和RPN网络的anchor一样，生成后就固定不变了，
而前面所说的bbox是指预测的框，跟这些anchor不是一回事。那么生成这些anchor做什么呢？
这是为了计算损失用。不管是RefineDet、SSD还是Faster RCNN，对坐标的回归损失计算都一样，
计算的是预测得到的offset要尽可能和（ground truth与anchor之间）的offset接近。
所以计算ground truth和anchor之间的offset的时候就需要用到这里计算得到的输出（anchor的坐标）。
'''
        name = "{}_mbox_priorbox".format(from_layer)
        net[name] = L.PriorBox(net[from_layer], net[data_layer], min_size=min_size,
                clip=clip, variance=prior_variance, offset=offset)
        if max_size:
            net.update(name, {'max_size': max_size})
        if aspect_ratio:
            net.update(name, {'aspect_ratio': aspect_ratio, 'flip': flip})
        if step:
            net.update(name, {'step': step})
        if img_height != 0 and img_width != 0:
            if img_height == img_width:
                net.update(name, {'img_size': img_height})
            else:
                net.update(name, {'img_h': img_height, 'img_w': img_width})
        priorbox_layers.append(net[name])

    # Concatenate priorbox, loc, and conf layers.
# 接下来这部分是对不同层的输出做融合。
    mbox_layers = []
    name = '{}{}'.format(prefix, "_loc")
    net[name] = L.Concat(*loc_layers, axis=1)
    mbox_layers.append(net[name])
    name = '{}{}'.format(prefix, "_conf")
    net[name] = L.Concat(*conf_layers, axis=1)
    mbox_layers.append(net[name])
    name = '{}{}'.format(prefix, "_priorbox")
    net[name] = L.Concat(*priorbox_layers, axis=2)
    mbox_layers.append(net[name])

# 接下来这部分是Object Detection Module（ODM），大部分和ARM相同的代码这里不再重复介绍，主要介绍不同点。
    prefix = 'odm'
    num = len(from_layers2)
    loc_layers = []
    conf_layers = []
    for i in range(0, num):
        from_layer = from_layers2[i]

        # Get the normalize value.
        if normalizations:
            if normalizations[i] != -1:
                norm_name = "{}_norm".format(from_layer)
                net[norm_name] = L.Normalize(net[from_layer], scale_filler=dict(type="constant", value=normalizations[i]),
                    across_spatial=False, channel_shared=False)
                from_layer = norm_name

        # Add intermediate layers.
        if inter_layer_depth:
            if inter_layer_depth[i] > 0:
                inter_name = "{}_inter".format(from_layer)
                ResBody(net, from_layer, inter_name, out2a=256, out2b=256, out2c=1024, stride=1, use_branch1=True)
                # ConvBNLayer(net, from_layer, inter_name, use_bn=use_batchnorm, use_relu=True, lr_mult=lr_mult,
                #       num_output=inter_layer_depth[i], kernel_size=3, pad=1, stride=1, **bn_param)
                # from_layer = inter_name
                from_layer = "res{}".format(inter_name)

        # Estimate number of priors per location given provided parameters.
        min_size = min_sizes[i]
        if type(min_size) is not list:
            min_size = [min_size]
        aspect_ratio = []
        if len(aspect_ratios) > i:
            aspect_ratio = aspect_ratios[i]
            if type(aspect_ratio) is not list:
                aspect_ratio = [aspect_ratio]
        max_size = []
        if len(max_sizes) > i:
            max_size = max_sizes[i]
            if type(max_size) is not list:
                max_size = [max_size]
            if max_size:
                assert len(max_size) == len(min_size), "max_size and min_size should have same length."
        if max_size:
            num_priors_per_location = (2 + len(aspect_ratio)) * len(min_size)
        else:
            num_priors_per_location = (1 + len(aspect_ratio)) * len(min_size)
        if flip:
            num_priors_per_location += len(aspect_ratio) * len(min_size)

        # Create location prediction layer.
        name = "{}_mbox_loc{}".format(from_layer, loc_postfix)
        num_loc_output = num_priors_per_location * 4
        if not share_location:
            num_loc_output *= num_classes
        ConvBNLayer(net, from_layer, name, use_bn=use_batchnorm, use_relu=False, lr_mult=lr_mult,
                    num_output=num_loc_output, kernel_size=kernel_size, pad=pad, stride=1, **bn_param)
        permute_name = "{}_perm".format(name)
        net[permute_name] = L.Permute(net[name], order=[0, 2, 3, 1])
        flatten_name = "{}_flat".format(name)
        net[flatten_name] = L.Flatten(net[permute_name], axis=1)
        loc_layers.append(net[flatten_name])

        # Create confidence prediction layer.
# 这里的num_conf_output = num_priors_per_location * num_classes，
# num_classes是所有object的数量+背景。因此这里的分类支路就和SSD中的一样。
        name = "{}_mbox_conf{}".format(from_layer, conf_postfix)
        num_conf_output = num_priors_per_location * num_classes
        ConvBNLayer(net, from_layer, name, use_bn=use_batchnorm, use_relu=False, lr_mult=lr_mult,
                    num_output=num_conf_output, kernel_size=kernel_size, pad=pad, stride=1, **bn_param)
        permute_name = "{}_perm".format(name)
        net[permute_name] = L.Permute(net[name], order=[0, 2, 3, 1])
        flatten_name = "{}_flat".format(name)
        net[flatten_name] = L.Flatten(net[permute_name], axis=1)
        conf_layers.append(net[flatten_name])

    # Concatenate priorbox, loc, and conf layers.
# 最后在返回列表中添加了bbox的分类输出和回归输出。
    name = '{}{}'.format(prefix, "_loc")
    net[name] = L.Concat(*loc_layers, axis=1)
    mbox_layers.append(net[name])
    name = '{}{}'.format(prefix, "_conf")
    net[name] = L.Concat(*conf_layers, axis=1)
    mbox_layers.append(net[name])

    return mbox_layers
```




