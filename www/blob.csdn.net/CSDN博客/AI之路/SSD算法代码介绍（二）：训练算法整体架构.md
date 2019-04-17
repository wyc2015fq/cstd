# SSD算法代码介绍（二）：训练算法整体架构 - AI之路 - CSDN博客





2018年02月17日 15:29:06[AI之路](https://me.csdn.net/u014380165)阅读数：18345








上一篇：[SSD算法代码介绍（一）：训练参数配置](http://blog.csdn.net/u014380165/article/details/79332339)主要介绍了训练模型的一些参数配置信息，可以看出在训练脚本train.py中主要是调用train_net.py脚本中的train_net函数进行训练的，因此这一篇博客介绍train_net.py脚本的内容。

**train_net.py这个脚本一共包含convert_pretrained，get_lr_scheduler，train_net三个函数，其中最重要的是train_net函数，这个函数也是train.py脚本训练模型时候调用的函数，建议从train_net函数开始看起。**

```python
import tools.find_mxnet
import mxnet as mx
import logging
import sys
import os
import importlib
import re
# 导入生成模型可用的数据格式的类，是在dataset文件夹下的iterator.py脚本中实现的，
# 一般采用这种导入脚本中类的方式需要在dataset文件夹下写一个空的__init__.py脚本才能导入
from dataset.iterator import DetRecordIter 
from train.metric import MultiBoxMetric # 导入训练时候的评价标准类
# 导入测试时候的评价标准类，这里VOC07MApMetric类继承了MApMetric类，主要内容在MApMetric类中
from evaluate.eval_metric import MApMetric, VOC07MApMetric 
from config.config import cfg
from symbol.symbol_factory import get_symbol_train # get_symbol_train函数来导入symbol

def convert_pretrained(name, args):
    """
    Special operations need to be made due to name inconsistance, etc

    Parameters:
    ---------
    name : str
        pretrained model name
    args : dict
        loaded arguments

    Returns:
    ---------
    processed arguments as dict
    """
    return args


# get_lr_scheduler函数就是设计你的学习率变化策略，函数的几个输入的意思在这里都介绍得很清楚了，
# lr_refactor_step可以是3或6这样的单独数字，也可以是3,6,9这样用逗号间隔的数字，表示到第3,6,9个epoch的时候就要改变学习率
def get_lr_scheduler(learning_rate, lr_refactor_step, lr_refactor_ratio,
                     num_example, batch_size, begin_epoch):
    """
    Compute learning rate and refactor scheduler

    Parameters:
    ---------
    learning_rate : float
        original learning rate
    lr_refactor_step : comma separated str
        epochs to change learning rate
    lr_refactor_ratio : float
        lr *= ratio at certain steps
    num_example : int
        number of training images, used to estimate the iterations given epochs
    batch_size : int
        training batch size
    begin_epoch : int
        starting epoch

    Returns:
    ---------
    (learning_rate, mx.lr_scheduler) as tuple
    """
    assert lr_refactor_ratio > 0
    iter_refactor = [int(r) for r in lr_refactor_step.split(',') if r.strip()]
    # 学习率的改变一般都是越来越小，不接受学习率越来越大这种策略，在这种情况下采用学习率不变的策略
    if lr_refactor_ratio >= 1: 
        return (learning_rate, None)
    else:
        lr = learning_rate
        epoch_size = num_example // batch_size # 表示每个epoch最少包含多少个batch
# 这个for循环的内容主要是解决当你设置的begin_epoch要大于你的iter_refactor的某些值的时候，
# 会按照lr_refactor_ratio改变你的初始学习率，也就是说这个改变是还没开始训练的时候就做的。
        for s in iter_refactor: 
            if begin_epoch >= s:
                lr *= lr_refactor_ratio
# 如果有上面这个学习率的改变，那么打印出改变信息，这样以后看log也能很清楚地知道当时实际初始学习率是多少。
        if lr != learning_rate: 
            logging.getLogger().info("Adjusted learning rate to {} for epoch {}".format(lr, begin_epoch))
# 这个steps就是你要运行多少个batch才需要改变学习率，因此这个steps是以batch为单位的
        steps = [epoch_size * (x - begin_epoch) for x in iter_refactor if x > begin_epoch]
# 这个if条件满足的话就表示我的begin_epoch比你设置的iter_refactor里面的所有值都大，那么我就返回学习率lr，
# 至于更改的策略就只能是None了，也就是说用这个lr一直跑到结束，中间就不改变了
        if not steps: 
            return (lr, None)
# 最终用mx.lr_scheduler.MultiFactorScheduler函数生成模型可用的lr_scheduler
        lr_scheduler = mx.lr_scheduler.MultiFactorScheduler(step=steps, factor=lr_refactor_ratio)
        return (lr, lr_scheduler)


# 这是train_net.py脚本中的主要函数
def train_net(net, train_path, num_classes, batch_size,
              data_shape, mean_pixels, resume, finetune, pretrained, epoch,
              prefix, ctx, begin_epoch, end_epoch, frequent, learning_rate,
              momentum, weight_decay, lr_refactor_step, lr_refactor_ratio,
              freeze_layer_pattern='',
              num_example=10000, label_pad_width=350,
              nms_thresh=0.45, force_nms=False, ovp_thresh=0.5,
              use_difficult=False, class_names=None,
              voc07_metric=False, nms_topk=400, force_suppress=False,
              train_list="", val_path="", val_list="", iter_monitor=0,
              monitor_pattern=".*", log_file=None):
    """
    Wrapper for training phase.

    Parameters:
    ----------
    net : str
        symbol name for the network structure
    train_path : str
        record file path for training
    num_classes : int
        number of object classes, not including background
    batch_size : int
        training batch-size
    data_shape : int or tuple
        width/height as integer or (3, height, width) tuple
    mean_pixels : tuple of floats
        mean pixel values for red, green and blue
    resume : int
        resume from previous checkpoint if > 0
    finetune : int
        fine-tune from previous checkpoint if > 0
    pretrained : str
        prefix of pretrained model, including path
    epoch : int
        load epoch of either resume/finetune/pretrained model
    prefix : str
        prefix for saving checkpoints
    ctx : [mx.cpu()] or [mx.gpu(x)]
        list of mxnet contexts
    begin_epoch : int
        starting epoch for training, should be 0 if not otherwise specified
    end_epoch : int
        end epoch of training
    frequent : int
        frequency to print out training status
    learning_rate : float
        training learning rate
    momentum : float
        trainig momentum
    weight_decay : float
        training weight decay param
    lr_refactor_ratio : float
        multiplier for reducing learning rate
    lr_refactor_step : comma separated integers
        at which epoch to rescale learning rate, e.g. '30, 60, 90'
    freeze_layer_pattern : str
        regex pattern for layers need to be fixed
    num_example : int
        number of training images
    label_pad_width : int
        force padding training and validation labels to sync their label widths
    nms_thresh : float
        non-maximum suppression threshold for validation
    force_nms : boolean
        suppress overlaped objects from different classes
    train_list : str
        list file path for training, this will replace the embeded labels in record
    val_path : str
        record file path for validation
    val_list : str
        list file path for validation, this will replace the embeded labels in record
    iter_monitor : int
        monitor internal stats in networks if > 0, specified by monitor_pattern
    monitor_pattern : str
        regex pattern for monitoring network stats
    log_file : str
        log to file if enabled
    """
    # set up logger
# 这部分内容和生成日志文件相关，依赖logging这个库，if条件中的log_file就是生成的log文件的路径和名称。
# 这个logger是RootLogger类型，可以用来输出提示信息，
# 用法例子：logger.info("Start finetuning with {} from epoch {}".format(ctx， epoch))
    logging.basicConfig()
    logger = logging.getLogger()
    logger.setLevel(logging.INFO)
    if log_file:
        fh = logging.FileHandler(log_file)
        logger.addHandler(fh)

    # check args
# 这一部分主要是检查一些配置参数是不是异常，比如你的data_shape必须是个int型等
    if isinstance(data_shape, int):
        data_shape = (3, data_shape, data_shape)
    assert len(data_shape) == 3 and data_shape[0] == 3
    if prefix.endswith('_'):
        prefix += '_' + str(data_shape[1])

    if isinstance(mean_pixels, (int, float)):
        mean_pixels = [mean_pixels, mean_pixels, mean_pixels]
    assert len(mean_pixels) == 3, "must provide all RGB mean values"


# 这里的train_iter是通过调用dataset文件夹下的iterator.py脚本中的DetRecordIter类来得到的，
# 简单讲就是从.rec和.lst文件到模型可以用的数据迭代器的过程。输入中train_path是你的.rec文件的路径，
# label_pad_width这个参数在文中的解释是force padding training and validation labels to sync their labels widths，
# train_list是空字符串。
    train_iter = DetRecordIter(train_path, batch_size, data_shape, mean_pixels=mean_pixels,
        label_pad_width=label_pad_width, path_imglist=train_list, **cfg.train)

# 如果你给了验证集数据的路径，那么也生成验证集数据迭代器，做法和前面训练集的做法一样
    if val_path:
        val_iter = DetRecordIter(val_path, batch_size, data_shape, mean_pixels=mean_pixels,
            label_pad_width=label_pad_width, path_imglist=val_list, **cfg.valid)
    else:
        val_iter = None

    # load symbol
# 这里调用了symbol文件夹下的symbol_factory.py脚本的get_symbol_train函数来导入symbol。这个函数的输入中，net是一个str，
# 代码中默认是‘vgg16_reduced’，data_shape是一个tuple，是在前面计算得到的，比如data_shape是(3,300,300)，num_classes就是类别数，
# 在VOC数据集中，num_classes就是20，nms_thresh是nms操作的参数，默认是0.45，
# force_suppress和nms_topk两个参数都是采用默认的False和400。
# 这个函数的输出net就是最终的检测网络，是一个symbol。
    net = get_symbol_train(net, data_shape[1], num_classes=num_classes,
        nms_thresh=nms_thresh, force_suppress=force_suppress, nms_topk=nms_topk)

    # define layers with fixed weight/bias
# 这一步是设计一些层的参数在模型训练过程中不变，freeze_layer_pattern是在train.py里面设置的一个参数，表示要将哪些层的参数固定。
# 最后得到的fixed_param_names就是一个list，其中的每个元素就是层参数的名称，比如conv1_1_weight，是一个str。
    if freeze_layer_pattern.strip():
        re_prog = re.compile(freeze_layer_pattern)
        fixed_param_names = [name for name in net.list_arguments() if re_prog.match(name)]
    else:
        fixed_param_names = None

    # load pretrained or resume from previous state
# resume是指你在训练检测模型的时候如果训练到一半但是中断了，想要从中断的epoch继续训练，
# 那么可以导入训练中断前的那个epoch的.param文件，
# 这个文件就是检测模型的参数，从而用这个参数初始化检测模型，达到断点继续训练的目的。
    ctx_str = '('+ ','.join([str(c) for c in ctx]) + ')'
    if resume > 0:
        logger.info("Resume training with {} from epoch {}"
            .format(ctx_str, resume))
        _, args, auxs = mx.model.load_checkpoint(prefix, resume)
        begin_epoch = resume
    elif finetune > 0:
        logger.info("Start finetuning with {} from epoch {}"
            .format(ctx_str, finetune))
        _, args, auxs = mx.model.load_checkpoint(prefix, finetune)
        begin_epoch = finetune
        # check what layers mismatch with the loaded parameters
        exe = net.simple_bind(mx.cpu(), data=(1, 3, 300, 300), label=(1, 1, 5), grad_req='null')
        arg_dict = exe.arg_dict
    fixed_param_names = []
        for k, v in arg_dict.items():
            if k in args:
                if v.shape != args[k].shape:
                    del args[k]
                    logging.info("Removed %s" % k)
                else:
            if not 'pred' in k:
                fixed_param_names.append(k)

# 这个if条件是导入预训练好的分类模型来初始化检测模型的参数，其中mxnet.model.checkpoint就是执行这个导入参数的作用，
# 生成的_是分类模型的网络，args是分类模型的参数，类型是dictionary，每个item表示一个层参数，item的内容就是一个参数的NDArray格式。
# auxs在这里是一个空字典。最后调用的这个convert_pretrained函数就是该脚本定义的第一个函数，直接return args，没做什么操作。
    elif pretrained:
        logger.info("Start training with {} from pretrained model {}"
            .format(ctx_str, pretrained))
        _, args, auxs = mx.model.load_checkpoint(pretrained, epoch)
        args = convert_pretrained(pretrained, args)
    else:
        logger.info("Experimental: start training from scratch with {}"
            .format(ctx_str))
        args = None
        auxs = None
        fixed_param_names = None

    # helper information
    # 这一部分将前面得到的要固定参数的层信息打印出来
    if fixed_param_names:
        logger.info("Freezed parameters: [" + ','.join(fixed_param_names) + ']')

    # init training module
# 调用mx.mod.Module类初始化一个模型。参数中net就是前面通过get_symbol_train函数导入的检测模型的symbol。
# logger是和日志相关的参数。ctx就是你训练模型时候的cpu或gpu选择。初始化model的时候就要指定要固定的参数。
    mod = mx.mod.Module(net, label_names=('label',), logger=logger, context=ctx,
                        fixed_param_names=fixed_param_names)

    # fit parameters
 # 这个frequent就是你每隔frequent个batch显示一次训练结果（比如损失，准确率等等），代码中frequent采用20。
    batch_end_callback = mx.callback.Speedometer(train_iter.batch_size, frequent=frequent)
 # prefix是一个指定的路径，生成的epoch_end_callback作为最后fit()函数的参数之一，用来指定生成的模型的存放地址。
    epoch_end_callback = mx.callback.do_checkpoint(prefix)

# 调用get_lr_scheduler()函数生成初始的学习率和学习率变化策略，这个get_lr_scheduler()函数在前面有详细介绍
    learning_rate, lr_scheduler = get_lr_scheduler(learning_rate, lr_refactor_step,
        lr_refactor_ratio, num_example, batch_size, begin_epoch)

# 定义优化器的一些参数，比如学习率；momentum（该参数是在sgd算法中计算下一步更新方向时候会用到，默认是0.9）；
# wd是正则项的系数，一般采用0.0001到0.0005，代码中默认是0.0005；lr_scheduler是学习率的更新策略，
# 比如你间隔20个epoch就把学习率降为原来的0.1倍等；
# rescale_grad参数如果你是一块GPU跑，就是默认的1，如果是多GPU，那么相当于在做梯度更新的时候需要合并多个GPU的结果，
# 这里ctx就是代表你是用cpu还是gpu，以及gpu的话是采用哪几块gpu。
    optimizer_params={'learning_rate':learning_rate,
                      'momentum':momentum,
                      'wd':weight_decay,
                      'lr_scheduler':lr_scheduler,
                      'clip_gradient':None,
                      'rescale_grad': 1.0 / len(ctx) if len(ctx) > 0 else 1.0 }
# 这个monitor一般是调试时候采用，默认训练模型的时候这个monitor是None，也就是iter_monitor默认是0
    monitor = mx.mon.Monitor(iter_monitor, pattern=monitor_pattern) if iter_monitor > 0 else None

    # run fit net, every n epochs we run evaluation network to get mAP
# 这一步是对评价指标的选择，脚本中中默认采用voc07_metric，ovp_thresh默认是0.5，
# 表示计算MAp时类别相同的预测框和真实框的IOU值的阈值。
    if voc07_metric:
        valid_metric = VOC07MApMetric(ovp_thresh, use_difficult, class_names, pred_idx=3)
    else:
        valid_metric = MApMetric(ovp_thresh, use_difficult, class_names, pred_idx=3)


# 模型训练的入口，这个mod只有检测网络的结构信息，而fit的arg_params参数则是指定了用来初始化这个检测模型的参数，
# 这些参数来自预训练好的分类模型。
# 如果你在调试模型的时候运行到fit这个函数，进入这个函数的话就进入到mxnet项目的base_module.py脚本，
# 里面包含了参数初始化和模型前后向的具体操作。
    mod.fit(train_iter, # 训练数据
            val_iter, # 测试数据
            eval_metric=MultiBoxMetric(), # 训练时的评价指标
            validation_metric=valid_metric, # 测试时的评价指标
# 每多少个batch显示结果，这个batch_end_callback参数是由mx.callback.Speedometer()函数生成的，
# 这个函数的输入包括batch_size和间隔
            batch_end_callback=batch_end_callback, 
# 每个epoch结束后，得到的.param文件存放地址，这个epoch_end_callback由mx.callback,do_checkpoint()函数生成，
# 这个函数的输入就是存放地址。
            epoch_end_callback=epoch_end_callback, 
            optimizer='sgd', # 优化算法采用sgd，也就是随机梯度下降
            optimizer_params=optimizer_params, # 优化器的一些参数
            begin_epoch=begin_epoch, # epoch的初始值
            num_epoch=end_epoch, # 一共要训练多少个epoch
            initializer=mx.init.Xavier(), # 其他参数的初始化方式
            arg_params=args, # 导入的模型的参数，就是你预训练的模型的参数
            aux_params=auxs, # 导入的模型的参数的均值方差
            allow_missing=True, # 是否允许一些参数缺失
            monitor=monitor) # 如果monitor为None的话，就没什么用了，因为fit()函数默认monitor参数为None
```

**这篇博客介绍了SSD算法的整体架构，旨在从宏观上加深对该算法的认识。从上面的代码介绍可以看出，在train_net函数中关于网络结构的构建是通过symbol_factory.py脚本的get_symbol_train函数进行的，因为网络结构的构建是SSD算法的核心，因此接下来一篇博客先来介绍关于网络结构构建的一些参数配置：[SSD算法代码介绍（三）：网络结构参数配置](https://blog.csdn.net/u014380165/article/details/79332406)。**



