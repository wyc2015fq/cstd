# Faster RCNN代码详解（一）：算法整体结构 - AI之路 - CSDN博客





2018年05月13日 21:30:37[AI之路](https://me.csdn.net/u014380165)阅读数：6143








**本系列博客介绍Faster RCNN算法的细节，以MXNet框架的代码为例**。希望可以通过该系列博客让更多同学了解Faster RCNN算法中关于RPN网络的构建、anchor、proposal、损失函数的定义、正负样本的定义等细节，这样对于理解Faster RCNN后续的延伸版本（比如R-FCN、FPN、Mask RCNN）以及其他object detection算法也有一定的帮助。接下来的讲解基本上按照训练代码的顺序进行。

项目地址：[https://github.com/precedenceguo/mx-rcnn](https://github.com/precedenceguo/mx-rcnn)

**该系列博客以端到端（end to end）的训练方式为例来介绍Faster RCNN算法，训练代码所在脚本：~/mx-rcnn/train_end2end.py**，该脚本构建了算法的整体结构，主要包含网络结构的构建（以特征提取主网络采用resnet为例，通过~mx-rcnn/rcnn/symbol/symbol_resnet.py脚本的get_resnet_train函数构建）和数据读取（通过~mx-rcnn/rcnn/core/loader.py脚本的AnchorLoader类和~/mx-rcnn/rcnn/io/rpn.py脚本的assign_anchor函数进行读取，前者会调用后者）两部分。

接下来就来看看训练启动脚本：~/mx-rcnn/train_end2end.py的代码细节：

```python
import argparse
import pprint
import mxnet as mx
import numpy as np

from rcnn.logger import logger
from rcnn.config import config, default, generate_config
from rcnn.symbol import *
from rcnn.core import callback, metric
from rcnn.core.loader import AnchorLoader
from rcnn.core.module import MutableModule
from rcnn.utils.load_data import load_gt_roidb, merge_roidb, filter_roidb
from rcnn.utils.load_model import load_param

def train_net(args, ctx, pretrained, epoch, prefix, begin_epoch, end_epoch,
              lr=0.001, lr_step='5'):
    # setup config
    config.TRAIN.BATCH_IMAGES = 1
    config.TRAIN.BATCH_ROIS = 128
    config.TRAIN.END2END = True
    config.TRAIN.BBOX_NORMALIZATION_PRECOMPUTED = True

    # load symbol
# eval语句是执行字符串命令，以args.network为resnet为例，就是调用~mx-rcnn/rcnn/symbol/symbol_resnet.py
# 脚本中的get_resnet_train函数来得到Faster RCNN的网络结构。
# 在该函数中涉及具体的RPN网络、RPN网络的损失函数、检测网络、检测网络的损失函数细节。
    sym = eval('get_' + args.network + '_train')(num_classes=config.NUM_CLASSES, num_anchors=config.NUM_ANCHORS)
    feat_sym = sym.get_internals()['rpn_cls_score_output']

    # setup multi-gpu
    batch_size = len(ctx)
    input_batch_size = config.TRAIN.BATCH_IMAGES * batch_size

    # print config
    logger.info(pprint.pformat(config))

    # load dataset and prepare imdb for training
# 这部分是从yml文件读取标注信息，主要调用的接口是load_gt_roidb函数
    image_sets = [iset for iset in args.image_set.split('+')]
    roidbs = [load_gt_roidb(args.dataset, image_set, args.root_path, args.dataset_path, flip=not args.no_flip)
              for image_set in image_sets]
    roidb = merge_roidb(roidbs)
    roidb = filter_roidb(roidb)

    # load training data
# 这部分是从yml文件读取标注信息，主要调用的接口是load_gt_roidb函数。
# args.no_flip参数用来设定是否对输入图像做flip操作，在Fast RCNN中是以0.5的概率来执行flip操作的。
    train_data = AnchorLoader(feat_sym, roidb, batch_size=input_batch_size, shuffle=not args.no_shuffle,  ctx=ctx, work_load_list=args.work_load_list,                          feat_stride=config.RPN_FEAT_STRIDE, anchor_scales=config.ANCHOR_SCALES,
                              anchor_ratios=config.ANCHOR_RATIOS, aspect_grouping=config.TRAIN.ASPECT_GROUPING)

    # infer max shape
    max_data_shape = [('data', (input_batch_size, 3, max([v[0] for v in config.SCALES]), max([v[1] for v in config.SCALES])))]
    max_data_shape, max_label_shape = train_data.infer_shape(max_data_shape)
    max_data_shape.append(('gt_boxes', (input_batch_size, 100, 5)))
    logger.info('providing maximum shape %s %s' % (max_data_shape, max_label_shape))

    # infer shape
    data_shape_dict = dict(train_data.provide_data + train_data.provide_label)
    arg_shape, out_shape, aux_shape = sym.infer_shape(**data_shape_dict)
    arg_shape_dict = dict(zip(sym.list_arguments(), arg_shape))
    out_shape_dict = dict(zip(sym.list_outputs(), out_shape))
    aux_shape_dict = dict(zip(sym.list_auxiliary_states(), aux_shape))
    logger.info('output shape %s' % pprint.pformat(out_shape_dict))

    # load and initialize params
# args.resume是True则表示断点训练，那样的话导入的模型是训到某个epoch的检测模型。
# 如果args.resume是False则不采用断点训练，这样的话就只导入分类模型进行参数初始化，
# 且需要对RPN网络中的层和整个网络最后的分类和回归支路的全连接层进行随机或0值初始化。
    if args.resume:
        arg_params, aux_params = load_param(prefix, begin_epoch, convert=True)
    else:
        arg_params, aux_params = load_param(pretrained, epoch, convert=True)
        arg_params['rpn_conv_3x3_weight'] = mx.random.normal(0, 0.01, shape=arg_shape_dict['rpn_conv_3x3_weight'])
        arg_params['rpn_conv_3x3_bias'] = mx.nd.zeros(shape=arg_shape_dict['rpn_conv_3x3_bias'])
        arg_params['rpn_cls_score_weight'] = mx.random.normal(0, 0.01, shape=arg_shape_dict['rpn_cls_score_weight'])
        arg_params['rpn_cls_score_bias'] = mx.nd.zeros(shape=arg_shape_dict['rpn_cls_score_bias'])
        arg_params['rpn_bbox_pred_weight'] = mx.random.normal(0, 0.01, shape=arg_shape_dict['rpn_bbox_pred_weight'])
        arg_params['rpn_bbox_pred_bias'] = mx.nd.zeros(shape=arg_shape_dict['rpn_bbox_pred_bias'])
        arg_params['cls_score_weight'] = mx.random.normal(0, 0.01, shape=arg_shape_dict['cls_score_weight'])
        arg_params['cls_score_bias'] = mx.nd.zeros(shape=arg_shape_dict['cls_score_bias'])
        arg_params['bbox_pred_weight'] = mx.random.normal(0, 0.001, shape=arg_shape_dict['bbox_pred_weight'])
        arg_params['bbox_pred_bias'] = mx.nd.zeros(shape=arg_shape_dict['bbox_pred_bias'])

    # check parameter shapes
    for k in sym.list_arguments():
        if k in data_shape_dict:
            continue
        assert k in arg_params, k + ' not initialized'
        assert arg_params[k].shape == arg_shape_dict[k], \
            'shape inconsistent for ' + k + ' inferred ' + str(arg_shape_dict[k]) + ' provided ' + str(arg_params[k].shape)
    for k in sym.list_auxiliary_states():
        assert k in aux_params, k + ' not initialized'
        assert aux_params[k].shape == aux_shape_dict[k], \
            'shape inconsistent for ' + k + ' inferred ' + str(aux_shape_dict[k]) + ' provided ' + str(aux_params[k].shape)

    # create solver
# model的初始化操作通过自定义的MutableModule类实现，
# 该类也是继承mxnet.module.base_modle.BaseModule这个基类进行重写，
# 使得该类可以处理size不同的输入数据。在MXNet框架中，定义了网络结构和输入后，
# 需要将二者bind在一起构成完整的网络，然后数据在这个完整的网络中流动，
# 也就是说在bind之后整个网络的输入数据维度就固定下来了，比如（1,3,600,800），
# 但是在Faster RCNN算法中对输入数据都是只对短边resize到600（长边是不定的），
# 而不是将所有输入图像都处理成统一个高宽，所以每次过模型时数据的维度都不是一样的，
# 比如这次是（1,3,600,800），下一张可能是（1,3,600,760），再下一张可能是（1,3,600,865）。
# 因此MutableModule这个类就是用来处理每次输入图像维度不一的问题，
# 实现起来就是当这次输入图像的维度和上一次的不一致，就重新调用Module接口初始化一个model，
# 然后调用model的bind方法将输入图像和网络结构做bind操作，这些都是在forward方法中实现的，并不复杂。
    fixed_param_prefix = config.FIXED_PARAMS
    data_names = [k[0] for k in train_data.provide_data]
    label_names = [k[0] for k in train_data.provide_label]
    mod = MutableModule(sym, data_names=data_names, label_names=label_names,
                        logger=logger, context=ctx, work_load_list=args.work_load_list,
                        max_data_shapes=max_data_shape, max_label_shapes=max_label_shape,
                        fixed_param_prefix=fixed_param_prefix)

    # decide training params
    # metric
# 定义评价标准，具体评价函数都是在~/mx-rcnn/rcnn/core/metric.py脚本中实现。
# 多个评价函数通过mx.metric.CompositeEvalMetric()类来管理，可以通过该类实例化得到的对象的add方法添加评价函数类。
    rpn_eval_metric = metric.RPNAccMetric()
    rpn_cls_metric = metric.RPNLogLossMetric()
    rpn_bbox_metric = metric.RPNL1LossMetric()
    eval_metric = metric.RCNNAccMetric()
    cls_metric = metric.RCNNLogLossMetric()
    bbox_metric = metric.RCNNL1LossMetric()
    eval_metrics = mx.metric.CompositeEvalMetric()
    for child_metric in [rpn_eval_metric, rpn_cls_metric, rpn_bbox_metric, eval_metric, cls_metric, bbox_metric]:
        eval_metrics.add(child_metric)
    # callback
    batch_end_callback = callback.Speedometer(train_data.batch_size, frequent=args.frequent)
    means = np.tile(np.array(config.TRAIN.BBOX_MEANS), config.NUM_CLASSES)
    stds = np.tile(np.array(config.TRAIN.BBOX_STDS), config.NUM_CLASSES)
    epoch_end_callback = callback.do_checkpoint(prefix, means, stds)

    # decide learning rate
# 接下来这一块都是对学习率的设置，最核心的是通过mx.lr_scheduler.MultiFactorScheduler(lr_iters, lr_factor)
# 接口来构造学习率对象，输入中lr_iters表示每隔多少个batch修改学习率，lr_factor是修改学习率时候变化的比例。
    base_lr = lr
    lr_factor = 0.1
    lr_epoch = [int(epoch) for epoch in lr_step.split(',')]
    lr_epoch_diff = [epoch - begin_epoch for epoch in lr_epoch if epoch > begin_epoch]
    lr = base_lr * (lr_factor ** (len(lr_epoch) - len(lr_epoch_diff)))
    lr_iters = [int(epoch * len(roidb) / batch_size) for epoch in lr_epoch_diff]
    logger.info('lr %f lr_epoch_diff %s lr_iters %s' % (lr, lr_epoch_diff, lr_iters))
    lr_scheduler = mx.lr_scheduler.MultiFactorScheduler(lr_iters, lr_factor)

    # optimizer
# 优化器的初始化
    optimizer_params = {'momentum': 0.9,
                        'wd': 0.0005,
                        'learning_rate': lr,
                        'lr_scheduler': lr_scheduler,
                        'rescale_grad': (1.0 / batch_size),
                        'clip_gradient': 5}

    # train
# 调用fit方法进行训练
    mod.fit(train_data, eval_metric=eval_metrics, epoch_end_callback=epoch_end_callback,
            batch_end_callback=batch_end_callback, kvstore=args.kvstore,
            optimizer='sgd', optimizer_params=optimizer_params,
            arg_params=arg_params, aux_params=aux_params, begin_epoch=begin_epoch, num_epoch=end_epoch)

def parse_args():
    parser = argparse.ArgumentParser(description='Train Faster R-CNN network')
    # general
    parser.add_argument('--network', help='network name', default=default.network, type=str)
    parser.add_argument('--dataset', help='dataset name', default=default.dataset, type=str)
    args, rest = parser.parse_known_args()
    generate_config(args.network, args.dataset)
    parser.add_argument('--image_set', help='image_set name', default=default.image_set, type=str)
    parser.add_argument('--root_path', help='output data folder', default=default.root_path, type=str)
    parser.add_argument('--dataset_path', help='dataset path', default=default.dataset_path, type=str)
    # training
    parser.add_argument('--frequent', help='frequency of logging', default=default.frequent, type=int)
    parser.add_argument('--kvstore', help='the kv-store type', default=default.kvstore, type=str)
    parser.add_argument('--work_load_list', help='work load for different devices', default=None, type=list)
    parser.add_argument('--no_flip', help='disable flip images', action='store_true')
    parser.add_argument('--no_shuffle', help='disable random shuffle', action='store_true')
    parser.add_argument('--resume', help='continue training', action='store_true')
    # e2e
    parser.add_argument('--gpus', help='GPU device to train with', default='0', type=str)
    parser.add_argument('--pretrained', help='pretrained model prefix', default=default.pretrained, type=str)
    parser.add_argument('--pretrained_epoch', help='pretrained model epoch', default=default.pretrained_epoch, type=int)
    parser.add_argument('--prefix', help='new model prefix', default=default.e2e_prefix, type=str)
    parser.add_argument('--begin_epoch', help='begin epoch of training, use with resume', default=0, type=int)
    parser.add_argument('--end_epoch', help='end epoch of training', default=default.e2e_epoch, type=int)
    parser.add_argument('--lr', help='base learning rate', default=default.e2e_lr, type=float)
    parser.add_argument('--lr_step', help='learning rate steps (in epoch)', default=default.e2e_lr_step, type=str)
    args = parser.parse_args()
    return args

def main():
    args = parse_args()
    logger.info('Called with argument: %s' % args)
    ctx = [mx.gpu(int(i)) for i in args.gpus.split(',')]
    train_net(args, ctx, args.pretrained, args.pretrained_epoch, args.prefix, args.begin_epoch, args.end_epoch,
              lr=args.lr, lr_step=args.lr_step)

if __name__ == '__main__':
    main()
```

从宏观上了解了训练代码的架构后，接下来就要详细了解每一个模块的具体实现了，接下来一篇博客就来看看网络结构是怎么构造的吧：[Faster RCNN代码详解（二）：网络结构构建](https://blog.csdn.net/u014380165/article/details/80303812)。



