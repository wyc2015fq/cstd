# SSD算法代码介绍（三）：网络结构参数配置 - AI之路 - CSDN博客





2018年02月17日 15:48:35[AI之路](https://me.csdn.net/u014380165)阅读数：2993








上一篇博客：[SSD算法代码介绍（二）：训练算法整体架构](http://blog.csdn.net/u014380165/article/details/79332365)

介绍了几乎所有的训练代码，在训练代码中比较重要的步骤应该就是网络结构的搭建，这也是SSD算法的核心。因此接下来介绍的symbol_factory.py就是网络结构搭建的起始脚本，主要包含网络的一些配置信息。

该脚本主要包含get_config，get_symbol_train，get_symbol三个函数，后面两个函数差不多，**可以从get_symbol_train函数开始看**，该函数中大部分是检测相关的参数配置，更详细的内容在另一个脚本（symbol_builder.py）中实现。

```python
"""Presets for various network configurations"""
from __future__ import absolute_import
import logging
from . import symbol_builder

# 这个函数就是给指定网络和输入数据尺寸生成一些参数配置
def get_config(network, data_shape, **kwargs):
    """Configuration factory for various networks

    Parameters
    ----------
    network : str
        base network name, such as vgg_reduced, inceptionv3, resnet...
    data_shape : int
        input data dimension
    kwargs : dict
        extra arguments
    """
    if network == 'vgg16_reduced':
        if data_shape >= 448:
    # 和下面else里面的from_layers做对比可以看出，对于输入图像为512*512的情况，需要额外增加5个卷积层，
    # 而如果是300*300（else那部分），则需要额外增加4个卷积层
            from_layers = ['relu4_3', 'relu7', '', '', '', '', ''] 
            num_filters = [512, -1, 512, 256, 256, 256, 256]
            strides = [-1, -1, 2, 2, 2, 2, 1]
            pads = [-1, -1, 1, 1, 1, 1, 1]
            sizes = [[.07, .1025], [.15,.2121], [.3, .3674], [.45, .5196], [.6, .6708], \
                [.75, .8216], [.9, .9721]]
            ratios = [[1,2,.5], [1,2,.5,3,1./3], [1,2,.5,3,1./3], [1,2,.5,3,1./3], \
                [1,2,.5,3,1./3], [1,2,.5], [1,2,.5]]
            normalizations = [20, -1, -1, -1, -1, -1, -1] # normalization如果等于-1，表示该层不做normalization
            steps = [] if data_shape != 512 else [x / 512.0 for x in
                [8, 16, 32, 64, 128, 256, 512]]
        else:
    # relu4_3表示原来VGG16网络中的第4个块中的第3个卷积，relu7表示原来的VGG16网络中的fc7层（VGG16最后一共有3个fc层，
    # 分别为fc6，fc7，fc8），后面的4个‘’表示在VGG16基础上增加的4个卷积层。这6层就是做特征融合的时候要提前特征的层。
            from_layers = ['relu4_3', 'relu7', '', '', '', ''] 
            num_filters = [512, -1, 512, 256, 256, 256] # 对应层的卷积核个数，这里-1表示extracted features
            strides = [-1, -1, 2, 2, 1, 1]
            pads = [-1, -1, 1, 1, 0, 0]
            sizes = [[.1, .141], [.2,.272], [.37, .447], [.54, .619], [.71, .79], [.88, .961]]
            ratios = [[1,2,.5], [1,2,.5,3,1./3], [1,2,.5,3,1./3], [1,2,.5,3,1./3], \
                [1,2,.5], [1,2,.5]]
            normalizations = [20, -1, -1, -1, -1, -1]
            steps = [] if data_shape != 300 else [x / 300.0 for x in [8, 16, 32, 64, 100, 300]]
        if not (data_shape == 300 or data_shape == 512):
            logging.warn('data_shape %d was not tested, use with caucious.' % data_shape)
        return locals() # locals函数返回以上这些变量
    elif network == 'inceptionv3':
        from_layers = ['ch_concat_mixed_7_chconcat', 'ch_concat_mixed_10_chconcat', '', '', '', '']
        num_filters = [-1, -1, 512, 256, 256, 128]
        strides = [-1, -1, 2, 2, 2, 2]
        pads = [-1, -1, 1, 1, 1, 1]
        sizes = [[.1, .141], [.2,.272], [.37, .447], [.54, .619], [.71, .79], [.88, .961]]
        ratios = [[1,2,.5], [1,2,.5,3,1./3], [1,2,.5,3,1./3], [1,2,.5,3,1./3], \
            [1,2,.5], [1,2,.5]]
        normalizations = -1
        steps = []
        return locals()
    elif network == 'resnet50':
        num_layers = 50
        image_shape = '3,224,224'  # resnet require it as shape check
        network = 'resnet'
        from_layers = ['_plus12', '_plus15', '', '', '', '']
        num_filters = [-1, -1, 512, 256, 256, 128]
        strides = [-1, -1, 2, 2, 2, 2]
        pads = [-1, -1, 1, 1, 1, 1]
        sizes = [[.1, .141], [.2,.272], [.37, .447], [.54, .619], [.71, .79], [.88, .961]]
        ratios = [[1,2,.5], [1,2,.5,3,1./3], [1,2,.5,3,1./3], [1,2,.5,3,1./3], \
            [1,2,.5], [1,2,.5]]
        normalizations = -1
        steps = []
        return locals()
    elif network == 'resnet101':
        num_layers = 101
        image_shape = '3,224,224'
        network = 'resnet'
        from_layers = ['_plus12', '_plus15', '', '', '', '']
        num_filters = [-1, -1, 512, 256, 256, 128]
        strides = [-1, -1, 2, 2, 2, 2]
        pads = [-1, -1, 1, 1, 1, 1]
        sizes = [[.1, .141], [.2,.272], [.37, .447], [.54, .619], [.71, .79], [.88, .961]]
        ratios = [[1,2,.5], [1,2,.5,3,1./3], [1,2,.5,3,1./3], [1,2,.5,3,1./3], \
            [1,2,.5], [1,2,.5]]
        normalizations = -1
        steps = []
        return locals()
    elif network == 'mobilenet':
        from_layers = ['activation22', 'activation26', '', '', '', '']
        num_filters = [-1, -1, 512, 256, 256, 128]
        strides = [-1, -1, 2, 2, 2, 2]
        pads = [-1, -1, 1, 1, 1, 1]
        sizes = [[.1, .141], [.2,.272], [.37, .447], [.54, .619], [.71, .79], [.88, .961]]
        ratios = [[1,2,.5], [1,2,.5,3,1./3], [1,2,.5,3,1./3], [1,2,.5,3,1./3], \
            [1,2,.5], [1,2,.5]]
        normalizations = -1
        steps = []
        return locals()
    else:
        msg = 'No configuration found for %s with data_shape %d' % (network, data_shape)
        raise NotImplementedError(msg)


# 这个函数就是用来获得整个网络的结构信息，包括新增的用于特征融合的层，生成anchor的层等等。
# 该函数主要是调用symbol_builder.py脚本中的几个函数来执行导入和生成symbol的操作。
def get_symbol_train(network, data_shape, **kwargs):
    """Wrapper for get symbol for train

    Parameters
    ----------
    network : str
        name for the base network symbol
    data_shape : int
        input shape
    kwargs : dict
        see symbol_builder.get_symbol_train for more details
    """
    if network.startswith('legacy'):
        logging.warn('Using legacy model.')
        return symbol_builder.import_module(network).get_symbol_train(**kwargs)

# 调用get_config函数得到一些配置参数
    config = get_config(network, data_shape, **kwargs).copy()
# 得到的配置参数config再加上kwargs里面的其他配置参数
    config.update(kwargs)
# 调用symbol_builder.py脚本中的get_symbol_train函数得到symbol
    return symbol_builder.get_symbol_train(**config)

def get_symbol(network, data_shape, **kwargs):
    """Wrapper for get symbol for test

    Parameters
    ----------
    network : str
        name for the base network symbol
    data_shape : int
        input shape
    kwargs : dict
        see symbol_builder.get_symbol for more details
    """
    if network.startswith('legacy'):
        logging.warn('Using legacy model.')
        return symbol_builder.import_module(network).get_symbol(**kwargs)
    config = get_config(network, data_shape, **kwargs).copy()
    config.update(kwargs)
    return symbol_builder.get_symbol(**config)
```

**这篇博客主要介绍了关于SSD的网络结构参数配置。从get_symbol_train函数可以看出，关于算法网络结构的构建是在symbol_builder.py脚本的get_symbol_train.py函数中进行的，因此接下来就一起来看看：[SSD算法代码介绍（四）：网络结构初探](https://blog.csdn.net/u014380165/article/details/79332466)。**



